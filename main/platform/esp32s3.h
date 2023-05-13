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
        static inline const std::string NAME = "ESP32-S3";

        struct PinConfig {
            static inline const std::set<int> DIGITAL_PINS = {
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21,
                33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 45, 46, 47, 48
            };
            static inline const std::unordered_map<int, std::pair<int, int>> ANALOG_PINS = {
                { 1, { 1, ADC1_GPIO1_CHANNEL } },
                { 2, { 1, ADC1_GPIO2_CHANNEL } },
                { 3, { 1, ADC1_GPIO3_CHANNEL } },
                { 4, { 1, ADC1_GPIO4_CHANNEL } },
                { 5, { 1, ADC1_GPIO5_CHANNEL } },
                { 6, { 1, ADC1_GPIO6_CHANNEL } },
                { 7, { 1, ADC1_GPIO7_CHANNEL } },
                { 8, { 1, ADC1_GPIO8_CHANNEL } },
                { 9, { 1, ADC1_GPIO9_CHANNEL } },
                { 10, { 1, ADC1_GPIO10_CHANNEL } }
            };
            static inline const std::set<int> INTERRUPT_PINS = DIGITAL_PINS;
        };
    };

    void initialize() {
        Next::initialize();

        jac::ContextRef ctx = this->context();

        jac::Object platformInfo = jac::Object::create(ctx);
        platformInfo.defineProperty("name", jac::Value::from(ctx, PlatformInfo::NAME), jac::PropFlags::Enumerable);

        ctx.getGlobalObject().defineProperty("PlatformInfo", platformInfo, jac::PropFlags::Enumerable);
    }
};
