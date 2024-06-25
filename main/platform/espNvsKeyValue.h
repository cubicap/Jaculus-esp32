#pragma once

#include <memory>
#include <jac/device/keyvalue.h>
#include "nvs_flash.h"

class EspNvsKeyValue : public jac::KeyValueNamespace {
    std::string _nsname;
    nvs_handle_t _handle;
public:
    static std::unique_ptr<EspNvsKeyValue> open(const std::string& nsname);

    EspNvsKeyValue(const std::string& nsname, nvs_handle_t handle);
    ~EspNvsKeyValue();

    bool erase(const std::string& name);

    void setInt(const std::string& name, int64_t value);
    void setFloat(const std::string& name, float value);
    void setString(const std::string& name, const std::string& value);

    int64_t getInt(const std::string& name, int64_t def_value = 0);
    float getFloat(const std::string& name, float def_value = 0.f);
    std::string getString(const std::string& name, std::string def_value = "");

    bool commit();
};

