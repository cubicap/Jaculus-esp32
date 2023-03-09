#pragma once

#include <jac/link/mux.h>
#include <jac/link/router.h>
#include "uploader.h"
#include "logger.h"
#include <jac/link/routerCommunicator.h>

#include <sstream>
#include <filesystem>
#include <optional>
#include <thread>

#include "esp_pthread.h"


template<class Machine>
class Controller {
    Router _router;
    std::optional<Uploader> _uploader;

    std::unique_ptr<BufferedInputPacketCommunicator> _input;
    std::unique_ptr<OutputPacketCommunicator> _output;

    std::unique_ptr<Machine> _machine;
    bool _running = false;
    std::thread _thread;

    std::function<std::string()> _getMemoryStats;
    std::function<std::string()> _getStorageStats;

    std::vector<std::function<void(Machine&)>> _onConfigureMachine;

    std::thread _controllerThread;

    void configureMachine() {
        _machine = std::make_unique<Machine>();
        for (auto& f : _onConfigureMachine) {
            f(*_machine);
        }
    }
public:
    void processStart(int sender, std::span<const uint8_t> data);
    void processStop(int sender);
    void processStatus(int sender);

    enum class Command : uint8_t {
        START = 0x01,
        STOP = 0x02,
        STATUS = 0x03,
        OK = 0x20,
        ERROR = 0x21,
    };

    Controller(std::function<std::string()> getMemoryStats, std::function<std::string()> getStorageStats):
        _getMemoryStats(getMemoryStats),
        _getStorageStats(getStorageStats)
    {
        Logger::_logStream = std::make_unique<TransparentOutputStreamCommunicator>(_router, 255, std::vector<int>{});
        Logger::_debugStream = std::make_unique<TransparentOutputStreamCommunicator>(_router, 254, std::vector<int>{});

        auto uploaderInput = std::make_unique<AsyncBufferedInputPacketCommunicator>();
        auto uploaderOutput = std::make_unique<TransparentOutputPacketCommunicator>(_router, 1);
        _router.subscribeChannel(1, *uploaderInput);

        _uploader.emplace(std::move(uploaderInput), std::move(uploaderOutput));

        auto controllerInput = std::make_unique<AsyncBufferedInputPacketCommunicator>();
        _router.subscribeChannel(0, *controllerInput);
        _input = std::move(controllerInput);
        _output = std::make_unique<TransparentOutputPacketCommunicator>(_router, 0);

        _controllerThread = std::thread([this]() {
            while (true) {
                auto [sender, data] = _input->get();
                if (data.size() == 0) {
                    continue;
                }
                auto begin = data.begin();
                Command cmd = static_cast<Command>(data[0]);
                begin++;

                Logger::debug(std::string("Controller: ") + std::to_string(static_cast<int>(cmd)));

                switch (cmd) {
                    case Command::START: {
                        processStart(sender, std::span<const uint8_t>(begin, data.end()));
                        break;
                    }
                    case Command::STOP: {
                        processStop(sender);
                        break;
                    }
                    case Command::STATUS: {
                        processStatus(sender);
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        });
    }

    Router& router() {
        return _router;
    }

    Uploader& uploader() {
        return *_uploader;
    }

    bool startMachine(std::string path);
    bool stopMachine();

    void onConfigureMachine(std::function<void(Machine&)> f) {
        _onConfigureMachine.push_back(f);
    }
};


template<class Machine>
void Controller<Machine>::processStart(int sender, std::span<const uint8_t> data) {
    std::string filename(data.begin(), data.end());

    auto result = Command::OK;

    if (!startMachine(filename)) {
        result = Command::ERROR;
    }

    auto response = this->_output->buildPacket({sender});
    response->put(static_cast<uint8_t>(result));
    response->send();
}

template<class Machine>
void Controller<Machine>::processStop(int sender) {
    auto result = Command::OK;

    if (!stopMachine()) {
        result = Command::ERROR;
    }

    auto response = this->_output->buildPacket({sender});
    response->put(static_cast<uint8_t>(result));
    response->send();
}

template<class Machine>
void Controller<Machine>::processStatus(int sender) {
    auto response = this->_output->buildPacket({sender});
    response->put(static_cast<uint8_t>(Command::STATUS));
    response->put(static_cast<uint8_t>(_running));
    response->put(_machine->eventLoop_getExitCode());

    std::stringstream oss;
    oss << "Memory usage: " << _getMemoryStats() << std::endl;
    oss << "Storage usage: " << _getStorageStats() << std::endl;
    std::string data = oss.str();
    response->put(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(data.data()), data.size()));
    response->send();
}

template<class Machine>
bool Controller<Machine>::startMachine(std::string path) {
    if (_running) {
        return false;
    }

    if (!std::filesystem::exists(path)) {
        Logger::log("File not found: " + path);
        return false;
    }

    esp_pthread_cfg_t cfg = esp_pthread_get_default_config();
    cfg.stack_size = 8 * 1024;
    esp_pthread_set_cfg(&cfg);

    _thread = std::thread([this, path]() {
        Controller<Machine>& self = *this;
        self._running = true;

        Logger::log("Starting machine");

        self.configureMachine();
        self._machine->initialize();
        try {
            self._machine->evalFile(path);
            self._machine->eventLoop_run();
        }
        catch (const std::runtime_error& e) {
            Logger::log("Runtime error - " + std::string(e.what()));
        }
        catch (const std::exception& e) {
            Logger::log("Exception - " + std::string(e.what()));
        }
        catch (...) {
            Logger::log("Unknown exception");
        }

        self._running = false;
    });

    return true;
}

template<class Machine>
bool Controller<Machine>::stopMachine() {
    if (!_running) {
        return false;
    }

    _machine->eventLoop_exit();

    if (_thread.joinable()) {
        _thread.join();
    }
    _thread = std::thread();

    return true;
}
