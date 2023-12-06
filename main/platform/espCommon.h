#pragma once

#include <jac/machine/machine.h>

#include "hal/adc_types.h"
#include "soc/adc_channel.h"


template<class PlatformInfo_, class Next>
class EspCommon : public Next {
public:
    using PlatformInfo = PlatformInfo_;

    static gpio_num_t getDigitalPin(int pin) {
        if (PlatformInfo::PinConfig::DIGITAL_PINS.find(pin) == PlatformInfo::PinConfig::DIGITAL_PINS.end()) {
            throw std::runtime_error("Invalid digital pin");
        }
        return static_cast<gpio_num_t>(pin);
    }

    static gpio_num_t getInterruptPin(int pin) {
        if (PlatformInfo::PinConfig::INTERRUPT_PINS.find(pin) == PlatformInfo::PinConfig::INTERRUPT_PINS.end()) {
            throw std::runtime_error("Invalid interrupt pin");
        }
        return static_cast<gpio_num_t>(pin);
    }

    void initialize() {
        Next::initialize();

        jac::ContextRef ctx = this->context();

        jac::Object platformInfo = jac::Object::create(ctx);
        platformInfo.defineProperty("name", jac::Value::from(ctx, PlatformInfo::NAME), jac::PropFlags::Enumerable);

        ctx.getGlobalObject().defineProperty("PlatformInfo", platformInfo, jac::PropFlags::Enumerable);
    }
};
