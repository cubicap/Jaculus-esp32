#pragma once

#include <jac/link/communicator.h>
#include <string>

struct Logger {
    static std::unique_ptr<OutputStreamCommunicator> _logStream;
    static void log(std::string message) {
        if (!_logStream) {
            return;
        }
        _logStream->write(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(message.c_str()), message.size()));
    }

    static std::unique_ptr<OutputStreamCommunicator> _debugStream;
    static void debug(std::string message) {
        if (!_debugStream) {
            return;
        }
        _debugStream->write(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(message.c_str()), message.size()));
    }
};
