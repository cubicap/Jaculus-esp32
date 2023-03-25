#pragma once

#include <set>
#include <string>


template<class Next>
class PlatformInfoFeature: public Next {
public:
    struct PlatformInfo {
        static inline const std::string NAME = "ESP32-S3";

        struct PinConfig {
            static inline const std::set<int> DIGITAL_PINS = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 38, 45, 46, 47, 48 };
            static inline const std::set<int> ANALOG_PINS = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            static inline const std::set<int> INTERRUPT_PINS = DIGITAL_PINS;
        };
    };

    void initialize() {
        Next::initialize();

        jac::Object platformInfo = jac::Object::create(this->_context);
        platformInfo.set("name", PlatformInfo::NAME);

        this->registerGlobal("PlatformInfo", platformInfo);
    }
};
