#include "uploader.h"
#include "logger.h"

#include <fstream>
#include <memory>
#include <string>
#include <filesystem>
#include <dirent.h>
#include <optional>


std::optional<std::pair<std::vector<std::string>, size_t>> listDir(std::string path) {
    size_t dataSize = 0;
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ent;
    dir = opendir(path.c_str());
    if (dir == NULL) {
        return std::nullopt;
    }
    while ((ent = readdir(dir)) != NULL) {
        try {
            files.push_back(ent->d_name);
        }
        catch (std::bad_alloc& e) {
            closedir(dir);
            return std::nullopt;
        }
        dataSize += files.back().size() + 1;
    }
    closedir(dir);
    return std::make_pair(files, dataSize);
}

bool deleteDir(std::string path) {
    auto list = listDir(path);
    if (!list) {
        return false;
    }

    for (auto& file : list->first) {
        std::string fullPath = path + "/" + file;
        if (std::filesystem::is_directory(fullPath)) {
            if (!deleteDir(fullPath)) {
                return false;
            }
        }
        else if (!std::filesystem::remove(fullPath)) {
            return false;
        }
    }
    if (path == "/" || path == "/data" || path == "/data/") {
        return true;
    }
    return std::filesystem::remove(path);
}

void Uploader::lockTimeout() {
    _state = State::NONE;
    _file.close();
    _onData = nullptr;
    _onDataComplete = nullptr;
}

bool Uploader::processPacket(int sender, std::span<const uint8_t> data) {
    if (!_controllerLock.ownedBy(sender)) {
        Logger::debug("Uploader: lock not owned by sender " + std::to_string(sender));
        auto response = this->_output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::LOCK_NOT_OWNED));
        response->send();
        return false;
    }

    if (data.size() < 1) {
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::UNKNOWN_COMMAND));
        response->send();
        return false;
    }
    auto begin = data.begin();
    Command cmd = static_cast<Command>(*begin);
    ++begin;

    if (_state == State::WAITING_FOR_DATA) {
        bool success = false;
        switch (cmd) {
            case Command::HAS_MORE_DATA:
                success = _onData(std::span<const uint8_t>(begin, data.end()));
                break;
            case Command::LAST_DATA:
                success = _onData(std::span<const uint8_t>(begin, data.end()));
                if (success) {
                    success = (!_onDataComplete) || _onDataComplete();
                    _state = State::NONE;
                    _file.close();
                    _onData = nullptr;
                    _onDataComplete = nullptr;
                }
                break;
            default:
                auto response = _output->buildPacket({sender});
                response->put(static_cast<uint8_t>(Command::ERROR));
                response->put(static_cast<uint8_t>(Error::UNKNOWN_COMMAND));
                response->put(static_cast<uint8_t>(cmd));
                response->send();
                break;
        }

        if (!success) {
            _state = State::NONE;
            _file.close();
            _onData = nullptr;
            _onDataComplete = nullptr;
        }
        return success;
    }

    switch (cmd) {
        case Command::READ_FILE:
            return processReadFile(sender, std::span<const uint8_t>(begin, data.end()));
        case Command::WRITE_FILE:
            return processWriteFile(sender, std::span<const uint8_t>(begin, data.end()));
        case Command::DELETE_FILE:
            return processDeleteFile(sender, std::span<const uint8_t>(begin, data.end()));
        case Command::LIST_DIR:
            return processListDir(sender, std::span<const uint8_t>(begin, data.end()));
        case Command::CREATE_DIR:
            return processCreateDir(sender, std::span<const uint8_t>(begin, data.end()));
        case Command::DELETE_DIR:
            return processDeleteDir(sender, std::span<const uint8_t>(begin, data.end()));
        default:
            auto response = _output->buildPacket({sender});
            response->put(static_cast<uint8_t>(Command::ERROR));
            response->put(static_cast<uint8_t>(Error::UNKNOWN_COMMAND));
            response->put(static_cast<uint8_t>(cmd));
            response->send();
            return false;
    }
}

bool Uploader::processReadFile(int sender, std::span<const uint8_t> data) {
    auto begin = data.begin();
    std::string filename(begin, data.end());

    _file = std::fstream(filename, std::ios::in | std::ios::binary);
    if (!_file.is_open()) {
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::NOT_FOUND));
        response->send();
        return false;
    }

    std::vector<uint8_t> buff(_output->maxPacketSize({sender}) - 1);

    Command prefix = Command::HAS_MORE_DATA;
    size_t read = 1;
    while (read > 0) {
        _file.read(reinterpret_cast<char*>(buff.data()), buff.size());
        read = _file.gcount();

        if (read < buff.size()) {
            prefix = Command::LAST_DATA;
        }
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(prefix));
        response->put(std::span(buff.data(), read));
        response->send();
    }
    return true;
}

bool Uploader::processWriteFile(int sender, std::span<const uint8_t> data) {
    auto filenameEnd = std::find(data.begin(), data.end(), '\0');
    if (filenameEnd == data.end()) {
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::INVALID_FILENAME));
        response->send();
        return false;
    }

    std::string filename(data.begin(), filenameEnd);
    auto begin = ++filenameEnd;
    _state = State::WAITING_FOR_DATA;

    _file = std::fstream(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!_file.is_open()) {
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::FILE_OPEN_FAILED));
        response->send();
        return false;
    }
    _onData = [this, sender](std::span<const uint8_t> data) {
        _file.write(reinterpret_cast<const char*>(data.data()), data.size());
        _file.sync();
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::CONTINUE));
        response->send();
        return true;
    };
    _onDataComplete = [this, sender]() {
        _file.close();
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::OK));
        response->send();
        return true;
    };

    if (begin != data.end()) {
        processPacket(sender, std::span<const uint8_t>(begin, data.end()));
    }

    return true;
}

bool Uploader::processDeleteFile(int sender, std::span<const uint8_t> data) {
    auto begin = data.begin();
    std::string filename(begin, data.end());

    bool success;

    try {
        success = !std::filesystem::is_directory(filename) && std::filesystem::remove(filename);
    }
    catch (const std::filesystem::filesystem_error& e) {
        Logger::error(std::string("Failed to delete file: ") + e.what());
        success = false;
    }

    if (success) {
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::OK));
        response->send();
        return true;
    }
    else {
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::FILE_DELETE_FAILED));
        response->send();
        return false;
    }
}

bool Uploader::processListDir(int sender, std::span<const uint8_t> data) {
    auto dataIt = std::find(data.begin(), data.end(), '\0');
    std::string filename(data.begin(), dataIt);

    struct {
        bool directory = false;
        bool size = false;
    } flags;

    for (dataIt++; dataIt < data.end(); dataIt++) {
        switch (*dataIt) {
            case 'd':
                flags.directory = true;
                break;
            case 's':
                flags.size = true;
                break;
            default:
                break;
        }
    }

    std::filesystem::path path(filename);
    bool isDir;
    try {
        isDir = std::filesystem::is_directory(path);
    }
    catch (const std::filesystem::filesystem_error& e) {
        Logger::error(std::string("Failed to list directory: ") + e.what());
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::DIR_OPEN_FAILED));
        response->send();
        return false;
    }

    if (flags.directory || !isDir) {
        if (std::filesystem::exists(path)) {
            std::string name = path.filename().string();
            auto response = _output->buildPacket({sender});
            response->put(static_cast<uint8_t>(Command::LAST_DATA));
            response->put(static_cast<uint8_t>(isDir ? 'd' : 'f'));
            response->put(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(name.data()), name.size()));
            response->put(static_cast<uint8_t>('\0'));
            response->send();
            return true;
        }
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::NOT_FOUND));
        response->send();
        return false;
    }

    std::vector<std::string> files;
    size_t dataSize = 0;
    // for (const auto& entry : std::filesystem::directory_iterator(path)) {
    //     files.push_back(entry.path().filename().string());
    //     dataSize += files.back().size() + 1;
    // }
    // XXX: std::filesystem::directory_iterator not working on esp-idf
    auto result = listDir(path);

    if (!result) {
        Logger::error(std::string("Failed to list directory"));
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::DIR_OPEN_FAILED));
        response->send();
        return false;
    }

    std::tie(files, dataSize) = *result;
    dataSize += files.size() * 5;  // for the type byte and size

    auto it = files.begin();
    Command prefix = Command::HAS_MORE_DATA;
    do {
        if (dataSize <= _output->maxPacketSize({sender}) - 1) {
            prefix = Command::LAST_DATA;
        }
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(prefix));
        while (it != files.end() && it->size() + 1 <= response->space()) {
            char type = 'f';
            uint32_t size = 0;
            try {
                type = std::filesystem::is_directory(path / *it) ? 'd' : 'f';
            }
            catch (const std::filesystem::filesystem_error& e) {
                Logger::error(std::string("Failed to check file type: ") + e.what());
            }
            try {
                size = (flags.size && type == 'f') ? std::filesystem::file_size(path / *it) : 0;
            }
            catch (const std::filesystem::filesystem_error& e) {
                Logger::error(std::string("Failed to get file size: ") + e.what());
            }

            response->put(static_cast<uint8_t>(type));
            response->put(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(it->data()), it->size()));
            response->put(static_cast<uint8_t>('\0'));
            for (int i = 3; i >= 0; i--) {
                response->put(static_cast<uint8_t>((size & (uint32_t(0xff) << (i * 8))) >> (i * 8)));
            }
            dataSize -= it->size() + 6;
            ++it;
        }
        response->send();
    } while (it != files.end());
    return true;
}

bool Uploader::processCreateDir(int sender, std::span<const uint8_t> data) {
    auto begin = data.begin();
    std::string filename(begin, data.end());

    bool success;

    try {
        success = std::filesystem::create_directory(filename);
    }
    catch (const std::filesystem::filesystem_error& e) {
        Logger::error(std::string("Failed to create directory: ") + e.what());
        success = false;
    }

    if (success) {
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::OK));
        response->send();
        return true;
    }
    else {
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::DIR_CREATE_FAILED));
        response->send();
        return false;
    }
}

bool Uploader::processDeleteDir(int sender, std::span<const uint8_t> data) {
    auto begin = data.begin();
    std::string filename(begin, data.end());

    bool success;

    try {
        success = deleteDir(filename);
        // XXX: std::filesystem::remove_all not working on esp-idf
    }
    catch (const std::filesystem::filesystem_error& e) {
        Logger::error(std::string("Failed to delete directory: ") + e.what());
        success = false;
    }

    if (success) {
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::OK));
        response->send();
        return true;
    }
    else {
        auto response = _output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::DIR_DELETE_FAILED));
        response->send();
        return false;
    }
}
