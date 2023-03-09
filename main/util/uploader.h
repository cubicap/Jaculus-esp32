#pragma once

#include <jac/link/router.h>
#include <fstream>
#include <deque>
#include <memory.h>

#include <jac/link/routerCommunicator.h>


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
        CONTINUE = 0x23
    };

    enum class Error : uint8_t {
        UNKNOWN_COMMAND = 0x01,
        FILE_OPEN_FAILED = 0x02,
        FILE_DELETE_FAILED = 0x03,
        DIR_OPEN_FAILED = 0x04,
        DIR_CREATE_FAILED = 0x05,
        DIR_DELETE_FAILED = 0x06,
        INVALID_FILENAME = 0x07
    };
private:
    enum class State {
        NONE,
        WAITING_FOR_DATA
    };

    State state = State::NONE;
    std::function<bool(std::span<const uint8_t>)> onData;
    std::fstream file;
    std::function<bool()> onDataComplete;

    std::unique_ptr<BufferedInputPacketCommunicator> input;
    std::unique_ptr<OutputPacketCommunicator> output;

    bool processPacket(int sender, std::span<const uint8_t> data);
    bool processReadFile(int sender, std::span<const uint8_t> data);
    bool processWriteFile(int sender, std::span<const uint8_t> data);
    bool processDeleteFile(int sender, std::span<const uint8_t> data);
    bool processListDir(int sender, std::span<const uint8_t> data);
    bool processCreateDir(int sender, std::span<const uint8_t> data);
    bool processDeleteDir(int sender, std::span<const uint8_t> data);

    std::thread _thread;
public:
    Uploader(std::unique_ptr<BufferedInputPacketCommunicator> input, std::unique_ptr<OutputPacketCommunicator> output):
        input(std::move(input)),
        output(std::move(output))
    {
        _thread = std::thread([this]() {
            while (true) {
                auto [sender, data] = this->input->get();
                processPacket(sender, std::span<const uint8_t>(data.begin(), data.end()));
            }
        });
    }

    Uploader(const Uploader&) = delete;
    Uploader(Uploader&&) = delete;
    Uploader& operator=(const Uploader&) = delete;
    Uploader& operator=(Uploader&&) = delete;
};
