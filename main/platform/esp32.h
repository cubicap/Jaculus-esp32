#pragma once

#include <jac/machine/machine.h>
#include <set>
#include <unordered_map>
#include <string>

#include "hal/adc_types.h"
#include "soc/adc_channel.h"


template<class Next>
class PlatformInfoFeature : public Next {
public:
    struct PlatformInfo {
        static inline const std::string NAME = "ESP32";

        struct PinConfig {
            static inline const std::set<int> DIGITAL_PINS = { 0, 2, 4, 5, 12, 13, 14, 15, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 37, 38, 39, 16, 17 };
            static inline const std::unordered_map<int, std::pair<int, int>> ANALOG_PINS = {
                { 32, { 1, ADC1_GPIO32_CHANNEL } },
                { 33, { 1, ADC1_GPIO33_CHANNEL } },
                { 34, { 1, ADC1_GPIO34_CHANNEL } },
                { 35, { 1, ADC1_GPIO35_CHANNEL } },
                { 36, { 1, ADC1_GPIO36_CHANNEL } },
                { 37, { 1, ADC1_GPIO37_CHANNEL } },
                { 38, { 1, ADC1_GPIO38_CHANNEL } },
                { 39, { 1, ADC1_GPIO39_CHANNEL } }
            };
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
