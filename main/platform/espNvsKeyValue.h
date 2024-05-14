#pragma once

#include <memory>
#include <jac/device/keyvalue.h>
#include <jac/device/logger.h>
#include "nvs_flash.h"

class EspNvsKeyValue : public jac::KeyValueNamespace {
    nvs_handle_t _handle;
public:
    static std::unique_ptr<EspNvsKeyValue> open(const std::string& nsname) {
        nvs_handle_t handle;
        auto err = nvs_open(nsname.c_str(), NVS_READWRITE, &handle);
        if(err != ESP_OK) {
            jac::Logger::error("Failed to open NVS namespace " + nsname + ": "+ std::string(esp_err_to_name(err)));
            return std::unique_ptr<EspNvsKeyValue>();
        }
        return std::make_unique<EspNvsKeyValue>(handle);
    }

    EspNvsKeyValue(nvs_handle_t handle) : _handle(handle) {

    }

    ~EspNvsKeyValue() {
        nvs_close(_handle);
    }

    bool erase(const std::string& name) {
        auto res = nvs_erase_key(_handle, name.c_str());
        if(res != ESP_OK && res != ESP_ERR_NVS_NOT_FOUND) {
            jac::Logger::error("Unexpected error in EspNvsKeyValue::erase for " + name + ": " + std::string(esp_err_to_name(res)));
        }
        return res == ESP_OK;
    }

    void setInt(const std::string& name, int64_t value) {
        auto res = nvs_set_i64(_handle, name.c_str(), value);
        if(res != ESP_OK) {
            jac::Logger::error("Unexpected error in EspNvsKeyValue::setInt for " + name + ": " + std::string(esp_err_to_name(res)));
        }
    }
    
    void setFloat(const std::string& name, float value) {
        auto res = nvs_set_u32(_handle, name.c_str(), *static_cast<uint32_t*>(static_cast<void*>(&value)));
        if(res != ESP_OK) {
            jac::Logger::error("Unexpected error in EspNvsKeyValue::setInt for " + name + ": " + std::string(esp_err_to_name(res)));
        }
    }

    void setString(const std::string& name, const std::string& value) {
        auto res = nvs_set_str(_handle, name.c_str(), value.c_str());
        if(res != ESP_OK) {
            jac::Logger::error("Unexpected error in EspNvsKeyValue::setString for " + name + ": " + std::string(esp_err_to_name(res)));
        }
    }

    int64_t getInt(const std::string& name, int64_t def_value = 0) {
        auto res = nvs_get_i64(_handle, name.c_str(), &def_value);
        if(res != ESP_OK && res != ESP_ERR_NVS_NOT_FOUND) {
            jac::Logger::error("Unexpected error in EspNvsKeyValue::getInt for " + name + ": " + std::string(esp_err_to_name(res)));
        }
        return def_value;
    }

    float getFloat(const std::string& name, float def_value = 0.f) {
        auto res = nvs_get_u32(_handle, name.c_str(), static_cast<uint32_t*>(static_cast<void*>(&def_value)));
        if(res != ESP_OK && res != ESP_ERR_NVS_NOT_FOUND) {
            jac::Logger::error("Unexpected error in EspNvsKeyValue::getFloat for " + name + ": " + std::string(esp_err_to_name(res)));
        }
        return def_value;
    }

    std::string getString(const std::string& name, std::string def_value = "") {
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

    bool commit() {
        auto res = nvs_commit(_handle);
        if(res != ESP_OK) {
            jac::Logger::error("Unexpected error in EspNvsKeyValue::commit: " + std::string(esp_err_to_name(res)));
        }
        return res == ESP_OK;
    }
};

