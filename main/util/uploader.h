#pragma once

#include <jac/link/router.h>
#include <jac/link/routerCommunicator.h>

#include <atomic>
#include <deque>
#include <fstream>
#include <memory.h>

#include "lock.h"

#include "esp_pthread.h"


namespace jac {


class Uploader {
public:
    enum class Command : uint8_t {
        READ_FILE = 0x01,
        WRITE_FILE = 0x02,
        DELETE_FILE = 0x03,
        LIST_DIR = 0x04,
        CREATE_DIR = 0x05,
        DELETE_DIR = 0x06,
        HAS_MORE_DATA = 0x10,
        LAST_DATA = 0x11,
        OK = 0x20,
        ERROR = 0x21,
        NOT_FOUND = 0x22,
        CONTINUE = 0x23,
        LOCK_NOT_OWNED = 0x24,
    };

    enum class Error : uint8_t {
        UNKNOWN_COMMAND = 0x01,
        FILE_OPEN_FAILED = 0x02,
        FILE_DELETE_FAILED = 0x03,
        DIR_OPEN_FAILED = 0x04,
        DIR_CREATE_FAILED = 0x05,
        DIR_DELETE_FAILED = 0x06,
        INVALID_FILENAME = 0x07,
    };
private:
    enum class State {
        NONE,
        WAITING_FOR_DATA
    };

    State _state = State::NONE;
    std::function<bool(std::span<const uint8_t>)> _onData;
    std::fstream _file;
    std::function<bool()> _onDataComplete;

    std::unique_ptr<BufferedInputPacketCommunicator> _input;
    std::unique_ptr<OutputPacketCommunicator> _output;

    bool processPacket(int sender, std::span<const uint8_t> data);
    bool processReadFile(int sender, std::span<const uint8_t> data);
    bool processWriteFile(int sender, std::span<const uint8_t> data);
    bool processDeleteFile(int sender, std::span<const uint8_t> data);
    bool processListDir(int sender, std::span<const uint8_t> data);
    bool processCreateDir(int sender, std::span<const uint8_t> data);
    bool processDeleteDir(int sender, std::span<const uint8_t> data);

    std::thread _thread;
    std::atomic<bool> _stop = false;

    TimeoutLock& _controllerLock;
public:
    Uploader(std::unique_ptr<BufferedInputPacketCommunicator> input, std::unique_ptr<OutputPacketCommunicator> output, TimeoutLock& lock):
        _input(std::move(input)),
        _output(std::move(output)),
        _controllerLock(lock)
    {
        esp_pthread_cfg_t cfg = esp_pthread_get_default_config();
        cfg.stack_size = 6 * 1024;
        esp_pthread_set_cfg(&cfg);

        _thread = std::thread([this]() {
            while (!_stop) {
                auto res = _input->get();
                if (!res) {
                    continue;
                }
                auto [sender, data] = *res;

                _controllerLock.stopTimeout(sender);  // does nothing if not locked by sender
                processPacket(sender, std::span<const uint8_t>(data.begin(), data.end()));
                _controllerLock.resetTimeout(sender);  // does nothing if not locked by sender
            }
        });
    }

    Uploader(const Uploader&) = delete;
    Uploader(Uploader&&) = delete;
    Uploader& operator=(const Uploader&) = delete;
    Uploader& operator=(Uploader&&) = delete;

    void lockTimeout();

    ~Uploader() {
        _stop = true;
        if (_thread.joinable()) {
            _thread.join();
        }
    }
};


} // namespace jac
