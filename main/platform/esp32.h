#pragma once

#include <set>
#include <string>
#include <jac/machine/machine.h>


template<class Next>
class PlatformInfoFeature : public Next {
public:
    struct PlatformInfo {
        static inline const std::string NAME = "ESP32";

        struct PinConfig {
            static inline const std::set<int> DIGITAL_PINS = { 0, 2, 4, 5, 12, 13, 14, 15, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 37, 38, 39, 16, 17 };
            static inline const std::set<int> ANALOG_PINS = { 32, 33, 34, 35, 36, 37, 38, 39 };
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
