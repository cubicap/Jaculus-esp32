#pragma once

#include <jac/link/mux.h>
#include <jac/link/router.h>
#include "uploader.h"
#include "logger.h"
#include <jac/link/routerCommunicator.h>
#include "lock.h"

#include <atomic>
#include <filesystem>
#include <optional>
#include <sstream>
#include <thread>

#include "esp_pthread.h"


template<class Machine>
class Controller {
    Router _router;

    std::optional<Uploader> _uploader;

    std::unique_ptr<BufferedInputPacketCommunicator> _ctrlInput;
    std::unique_ptr<OutputPacketCommunicator> _ctrlOutput;
    std::thread _controllerThread;
    std::atomic<bool> _controllerStop = false;

    TimeoutLock _lock = TimeoutLock(std::chrono::seconds(2));

    std::unique_ptr<Machine> _machine;
    std::vector<std::function<void(Machine&)>> _onConfigureMachine;
    std::atomic<bool> _machineRunning = false;
    std::thread _machineThread;

    struct MachineIO {
        std::unique_ptr<BufferedInputStreamCommunicator> in;
        std::unique_ptr<OutputStreamCommunicator> out;
        std::unique_ptr<OutputStreamCommunicator> err;
    } _machineIO;

    std::function<std::string()> _getMemoryStats;
    std::function<std::string()> _getStorageStats;


    void configureMachine() {
        _machine = nullptr;
        _machine = std::make_unique<Machine>();
        for (auto& f : _onConfigureMachine) {
            f(*_machine);
        }
        Logger::debug("Controller: machine configured");
    }

    void processPacket(int sender, std::span<const uint8_t> data);
    void processStart(int sender, std::span<const uint8_t> data);
    void processStop(int sender);
    void processStatus(int sender);
    void processLock(int sender);
    void processUnlock(int sender);
    void processForceUnlock(int sender);

    void lockTimeout() {
        Logger::debug("Controller: lock timeout");

        _uploader->lockTimeout();
    }
public:

    enum class Command : uint8_t {
        START = 0x01,
        STOP = 0x02,
        STATUS = 0x03,
        LOCK = 0x10,
        UNLOCK = 0x11,
        FORCE_UNLOCK = 0x12,
        OK = 0x20,
        ERROR = 0x21,
        LOCK_NOT_OWNED = 0x22,
    };

    Controller(std::function<std::string()> getMemoryStats, std::function<std::string()> getStorageStats):
        _getMemoryStats(getMemoryStats),
        _getStorageStats(getStorageStats)
    {
        Logger::_errorStream = std::make_unique<TransparentOutputStreamCommunicator>(_router, 255, std::vector<int>{});
        Logger::_logStream = std::make_unique<TransparentOutputStreamCommunicator>(_router, 253, std::vector<int>{});
        Logger::_debugStream = std::make_unique<TransparentOutputStreamCommunicator>(_router, 251, std::vector<int>{});

        auto uploaderInput = std::make_unique<UnboundedBufferedInputPacketCommunicator>();
        auto uploaderOutput = std::make_unique<TransparentOutputPacketCommunicator>(_router, 1);
        _router.subscribeChannel(1, *uploaderInput);

        _uploader.emplace(std::move(uploaderInput), std::move(uploaderOutput), _lock);

        auto controllerInput = std::make_unique<UnboundedBufferedInputPacketCommunicator>();
        _router.subscribeChannel(0, *controllerInput);
        _ctrlInput = std::move(controllerInput);
        _ctrlOutput = std::make_unique<TransparentOutputPacketCommunicator>(_router, 0);

        auto _machineIn = std::make_unique<UnboundedBufferedInputStreamCommunicator>(std::set<int>{});
        _router.subscribeChannel(16, *_machineIn);
        _machineIO.in = std::move(_machineIn);
        _machineIO.out = std::make_unique<TransparentOutputStreamCommunicator>(_router, 16, std::vector<int>{});
        _machineIO.err = std::make_unique<TransparentOutputStreamCommunicator>(_router, 17, std::vector<int>{});

        _controllerThread = std::thread([this]() {
            while (!_controllerStop) {
                int sender;
                std::vector<uint8_t> data;
                try {
                    std::tie(sender, data) = _ctrlInput->get();
                }
                catch (const std::exception& e) {
                    continue;
                }
                _lock.stopTimeout(sender);  // does nothing if not locked by sender
                processPacket(sender, data);
                _lock.resetTimeout(sender);  // does nothing if not locked by sender
            }
        });
    }

    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(Controller&&) = delete;

    ~Controller() {
        _controllerStop = true;
        _ctrlInput->cancelRead();
        if (_controllerThread.joinable()) {
            _controllerThread.join();
        }

        stopMachine();
    }

    Router& router() {
        return _router;
    }

    Uploader& uploader() {
        return *_uploader;
    }

    TimeoutLock& lock() {
        return _lock;
    }

    MachineIO& machineIO() {
        return _machineIO;
    }

    bool startMachine(std::string path);
    bool stopMachine();

    void onConfigureMachine(std::function<void(Machine&)> f) {
        _onConfigureMachine.push_back(f);
    }
};


template<class Machine>
void Controller<Machine>::processPacket(int sender, std::span<const uint8_t> data) {
    if (data.size() == 0) {
        return;
    }
    auto begin = data.begin();
    Command cmd = static_cast<Command>(data[0]);
    begin++;

    switch (cmd) {
        case Command::LOCK:
            processLock(sender);
            return;
        case Command::UNLOCK:
            processUnlock(sender);
            return;
        case Command::FORCE_UNLOCK:
            processForceUnlock(sender);
            return;
        case Command::STATUS:
            processStatus(sender);
            return;
        default:
            break;
    }

    if (!_lock.ownedBy(sender)) {
        Logger::debug("Controller: lock not owned by sender " + std::to_string(sender));
        auto response = this->_ctrlOutput->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::LOCK_NOT_OWNED));
        response->send();
        return;
    }

    switch (cmd) {
        case Command::START:
            processStart(sender, std::span<const uint8_t>(begin, data.end()));
            break;
        case Command::STOP:
            processStop(sender);
            break;
        default:
            break;
    }
}

template<class Machine>
void Controller<Machine>::processStart(int sender, std::span<const uint8_t> data) {
    std::string filename(data.begin(), data.end());

    auto result = Command::OK;

    if (!startMachine(filename)) {
        result = Command::ERROR;
    }

    auto response = this->_ctrlOutput->buildPacket({sender});
    response->put(static_cast<uint8_t>(result));
    response->send();
}

template<class Machine>
void Controller<Machine>::processStop(int sender) {
    auto result = Command::OK;

    if (!stopMachine()) {
        result = Command::ERROR;
    }

    auto response = this->_ctrlOutput->buildPacket({sender});
    response->put(static_cast<uint8_t>(result));
    response->send();
}

template<class Machine>
void Controller<Machine>::processStatus(int sender) {
    auto response = this->_ctrlOutput->buildPacket({sender});
    response->put(static_cast<uint8_t>(Command::STATUS));
    response->put(static_cast<uint8_t>(_machineRunning));

    if (_machine) {
        response->put(_machine->getExitCode());
    }
    else {
        response->put(static_cast<uint8_t>(0));
    }

    std::stringstream oss;
    oss << "Memory usage: " << _getMemoryStats() << std::endl;
    oss << "Storage usage: " << _getStorageStats() << std::endl;
    std::string data = oss.str();
    response->put(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(data.data()), data.size()));
    response->send();
}

template<class Machine>
void Controller<Machine>::processLock(int sender) {
    auto response = this->_ctrlOutput->buildPacket({sender});

    auto callback = [this]() {
        lockTimeout();
    };

    if (_lock.ownedBy(sender)) {
        response->put(static_cast<uint8_t>(Command::ERROR));
    }
    else if (_lock.lock(sender, callback)) {
        response->put(static_cast<uint8_t>(Command::OK));
    }
    else {
        response->put(static_cast<uint8_t>(Command::ERROR));
    }

    response->send();
}

template<class Machine>
void Controller<Machine>::processUnlock(int sender) {
    auto response = this->_ctrlOutput->buildPacket({sender});

    if (_lock.unlock(sender)) {
        response->put(static_cast<uint8_t>(Command::OK));
    }
    else {
        response->put(static_cast<uint8_t>(Command::ERROR));
    }

    response->send();
}

template<class Machine>
void Controller<Machine>::processForceUnlock(int sender) {
    auto response = this->_ctrlOutput->buildPacket({sender});

    _lock.forceUnlock();

    response->put(static_cast<uint8_t>(Command::OK));
}

template<class Machine>
bool Controller<Machine>::startMachine(std::string path) {
    if (_machineRunning) {
        return false;
    }
    if (_machineThread.joinable()) {
        _machineThread.join();
    }

    if (!std::filesystem::exists(path)) {
        Logger::log("File not found: " + path);
        return false;
    }

    esp_pthread_cfg_t cfg = esp_pthread_get_default_config();
    cfg.stack_size = 8 * 1024;
    esp_pthread_set_cfg(&cfg);

    _machineThread = std::thread([this, path]() {
        Controller<Machine>& self = *this;
        self._machineRunning = true;

        Logger::log("Starting machine");

        self.configureMachine();
        self._machine->initialize();
        try {
            self._machine->evalFile(path);
            self._machine->runEventLoop();
        }
        catch (jac::Exception& e) {
            std::string message = "Uncaught " + std::string(e.what()) + "\n" + e.stackTrace();
            this->_machineIO.err->write(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(message.data()), message.size()));
        }
        catch (const std::exception& e) {
            std::string message = "Internal error: " + std::string(e.what());
            this->_machineIO.err->write(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(message.data()), message.size()));
            Logger::log(message);
        }
        catch (...) {
            std::string message = "Unkown internal error";
            this->_machineIO.err->write(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(message.data()), message.size()));
            Logger::log(message);
        }

        {
            std::string message = "Machine exited with code " + std::to_string(self._machine->getExitCode()) + "\n";
            this->_machineIO.err->write(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(message.data()), message.size()));
        }

        self._machineRunning = false;
    });

    return true;
}

template<class Machine>
bool Controller<Machine>::stopMachine() {
    if (!_machineRunning) {
        return false;
    }

    _machine->kill();

    if (_machineThread.joinable()) {
        _machineThread.join();
    }
    _machineThread = std::thread();

    return true;
}
