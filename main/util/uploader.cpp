#include "uploader.h"

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
        files.push_back(ent->d_name);
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

bool Uploader::processPacket(int sender, std::span<const uint8_t> data) {
    if (data.size() < 1) {
        return false;
    }
    auto begin = data.begin();
    Command cmd = static_cast<Command>(*begin);
    ++begin;

    if (state == State::WAITING_FOR_DATA) {
        bool success = false;
        switch (cmd) {
            case Command::HAS_MORE_DATA:
                success = onData(std::span<const uint8_t>(begin, data.end()));
                break;
            case Command::LAST_DATA:
                success = onData(std::span<const uint8_t>(begin, data.end()));
                if (success) {
                    success = (!onDataComplete) || onDataComplete();
                    state = State::NONE;
                    file.close();
                    onData = nullptr;
                    onDataComplete = nullptr;
                }
                break;
            default:
                // TODO: ignore/reset state
                return false;
        }

        if (!success) {
            state = State::NONE;
            file.close();
            onData = nullptr;
            onDataComplete = nullptr;
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
            auto response = output->buildPacket({sender});
            response->put(static_cast<uint8_t>(Command::ERROR));
            response->put(static_cast<uint8_t>(Error::UNKNOWN_COMMAND));
            response->put(static_cast<uint8_t>(cmd));
            response->send();
            return false;
    }
}

bool Uploader::processReadFile(int sender, std::span<const uint8_t> data) {
    // TODO: check if file is open for writing
    auto begin = data.begin();
    std::string filename(begin, data.end());

    file = std::fstream(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        auto response = output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::NOT_FOUND));
        response->send();
        return false;
    }

    std::vector<uint8_t> buff(output->maxPacketSize({sender}) - 1);

    Command prefix = Command::HAS_MORE_DATA;
    size_t read = 1;
    while (read > 0) {
        file.read(reinterpret_cast<char*>(buff.data()), buff.size());
        read = file.gcount();

        if (read < buff.size()) {
            prefix = Command::LAST_DATA;
        }
        auto response = output->buildPacket({sender});
        response->put(static_cast<uint8_t>(prefix));
        size_t sent = response->put(std::span(buff.data(), read));
        (void)sent;
        response->send();
    }
    return true;
}

bool Uploader::processWriteFile(int sender, std::span<const uint8_t> data) {
    // TODO: check if file is available for writing
    auto filenameEnd = std::find(data.begin(), data.end(), '\0');
    if (filenameEnd == data.end()) {
        auto response = output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::INVALID_FILENAME));

        response->send();
        return false;
    }
    std::string filename(data.begin(), filenameEnd);
    auto begin = ++filenameEnd;
    state = State::WAITING_FOR_DATA;
    // TODO: delete file
    file = std::fstream(filename, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        auto response = output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::FILE_OPEN_FAILED));
        response->send();
        return false;
    }
    onData = [this, sender](std::span<const uint8_t> data) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.sync();
        auto response = output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::CONTINUE));
        response->send();
        return true;
    };
    onDataComplete = [this, sender]() {
        file.close();
        auto response = output->buildPacket({sender});
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
    // TODO: check if file is available for writing
    auto begin = data.begin();
    std::string filename(begin, data.end());

    if (!std::filesystem::is_directory(filename) && std::filesystem::remove(filename)) {
        auto response = output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::OK));
        response->send();
        return true;
    }
    else {
        auto response = output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::FILE_DELETE_FAILED));
        response->send();
        return false;
    }
}

bool Uploader::processListDir(int sender, std::span<const uint8_t> data) {
    // TODO: check if dir is available for reading
    auto begin = data.begin();
    std::string filename(begin, data.end());

    std::filesystem::path path(filename);
    if (!std::filesystem::is_directory(path)) {
        auto response = output->buildPacket({sender});
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
    // TODO: check std::filesystem::directory_iterator
    auto result = listDir(path);

    if (!result) {
        auto response = output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::DIR_OPEN_FAILED));
        response->send();
        return false;
    }

    std::tie(files, dataSize) = *result;

    auto it = files.begin();
    Command prefix = Command::HAS_MORE_DATA;
    do {
        if (dataSize <= output->maxPacketSize({sender}) - 1) {
            prefix = Command::LAST_DATA;
        }
        auto response = output->buildPacket({sender});
        response->put(static_cast<uint8_t>(prefix));
        while (it != files.end() && it->size() + 1 <= response->space()) {
            response->put(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(it->data()), it->size()));
            response->put(static_cast<uint8_t>('\0'));
            dataSize -= it->size() + 1;
            ++it;
        }
        response->send();
    } while (it != files.end());
    return true;
}

bool Uploader::processCreateDir(int sender, std::span<const uint8_t> data) {
    // TODO: check if dir exists
    auto begin = data.begin();
    std::string filename(begin, data.end());

    if (std::filesystem::create_directory(filename)) {
        auto response = output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::OK));
        response->send();
        return true;
    }
    else {
        auto response = output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::DIR_CREATE_FAILED));
        response->send();
        return false;
    }
}

bool Uploader::processDeleteDir(int sender, std::span<const uint8_t> data) {
    // TODO: check if is available for writing
    auto begin = data.begin();
    std::string filename(begin, data.end());

    if (deleteDir(filename)) {
        auto response = output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::OK));
        response->send();
        return true;
    }
    else {
        auto response = output->buildPacket({sender});
        response->put(static_cast<uint8_t>(Command::ERROR));
        response->put(static_cast<uint8_t>(Error::DIR_DELETE_FAILED));
        response->send();
        return false;
    }
}
