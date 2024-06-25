#include <memory>
#include <jac/device/keyvalue.h>
#include <jac/device/logger.h>
#include "nvs_flash.h"
#include "espWifi.h"

std::unique_ptr<EspNvsKeyValue> EspNvsKeyValue::open(const std::string& nsname) {
    nvs_handle_t handle;
    auto err = nvs_open(nsname.c_str(), NVS_READWRITE, &handle);
    if(err != ESP_OK) {
        jac::Logger::error("Failed to open NVS namespace " + nsname + ": "+ std::string(esp_err_to_name(err)));
        return std::unique_ptr<EspNvsKeyValue>();
    }
    return std::make_unique<EspNvsKeyValue>(nsname, handle);
}

EspNvsKeyValue::EspNvsKeyValue(const std::string& nsname, nvs_handle_t handle) : _nsname(nsname), _handle(handle) {
}

EspNvsKeyValue::~EspNvsKeyValue() {
    nvs_close(_handle);
}

bool EspNvsKeyValue::erase(const std::string& name) {
    auto res = nvs_erase_key(_handle, name.c_str());
    if(res != ESP_OK && res != ESP_ERR_NVS_NOT_FOUND) {
        jac::Logger::error("Unexpected error in EspNvsKeyValue::erase for " + name + ": " + std::string(esp_err_to_name(res)));
    }
    return res == ESP_OK;
}

void EspNvsKeyValue::setInt(const std::string& name, int64_t value) {
    auto res = nvs_set_i64(_handle, name.c_str(), value);
    if(res != ESP_OK) {
        jac::Logger::error("Unexpected error in EspNvsKeyValue::setInt for " + name + ": " + std::string(esp_err_to_name(res)));
    }
}

void EspNvsKeyValue::setFloat(const std::string& name, float value) {
    auto res = nvs_set_u32(_handle, name.c_str(), *static_cast<uint32_t*>(static_cast<void*>(&value)));
    if(res != ESP_OK) {
        jac::Logger::error("Unexpected error in EspNvsKeyValue::setInt for " + name + ": " + std::string(esp_err_to_name(res)));
    }
}

void EspNvsKeyValue::setString(const std::string& name, const std::string& value) {
    auto res = nvs_set_str(_handle, name.c_str(), value.c_str());
    if(res != ESP_OK) {
        jac::Logger::error("Unexpected error in EspNvsKeyValue::setString for " + name + ": " + std::string(esp_err_to_name(res)));
    }
}

int64_t EspNvsKeyValue::getInt(const std::string& name, int64_t def_value) {
    auto res = nvs_get_i64(_handle, name.c_str(), &def_value);
    if(res != ESP_OK && res != ESP_ERR_NVS_NOT_FOUND) {
        jac::Logger::error("Unexpected error in EspNvsKeyValue::getInt for " + name + ": " + std::string(esp_err_to_name(res)));
    }
    return def_value;
}

float EspNvsKeyValue::getFloat(const std::string& name, float def_value) {
    auto res = nvs_get_u32(_handle, name.c_str(), static_cast<uint32_t*>(static_cast<void*>(&def_value)));
    if(res != ESP_OK && res != ESP_ERR_NVS_NOT_FOUND) {
        jac::Logger::error("Unexpected error in EspNvsKeyValue::getFloat for " + name + ": " + std::string(esp_err_to_name(res)));
    }
    return def_value;
}

std::string EspNvsKeyValue::getString(const std::string& name, std::string def_value) {
    if(_nsname == EspWifiController::KvNsWifiMain && name == EspWifiController::KeyWifiCurrentIp) {
        auto& wifi = EspWifiController::get();
        if(wifi.currentIp().addr == 0) {
            return def_value;
        }
        return wifi.currentIpStr();
    }

    size_t str_len = 0;

    auto err = nvs_get_str(_handle, name.c_str(), NULL, &str_len);
    if(err == ESP_ERR_NVS_NOT_FOUND) {
        return def_value;
    } else if(err != ESP_OK) {
        jac::Logger::error("Unexpected error in EspNvsKeyValue::getString for " + name + ": " + std::string(esp_err_to_name(err)));
        return def_value;
    }

    std::string result(str_len-1, '0');
    err = nvs_get_str(_handle, name.c_str(), result.data(), &str_len);
    if(err != ESP_OK) {
        jac::Logger::error("Unexpected error in EspNvsKeyValue::getString for " + name + ": " + std::string(esp_err_to_name(err)));
        return def_value;
    }
    return result;
}

bool EspNvsKeyValue::commit() {
    auto res = nvs_commit(_handle);
    if(res != ESP_OK) {
        jac::Logger::error("Unexpected error in EspNvsKeyValue::commit: " + std::string(esp_err_to_name(res)));
    }
    return res == ESP_OK;
}


