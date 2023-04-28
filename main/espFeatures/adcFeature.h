#pragma once

#include <jac/machine/machine.h>
#include <jac/machine/functionFactory.h>

#include <stdexcept>
#include <set>

#include "driver/adc.h"


template<class Next>
class AdcFeature : public Next {
    using PinConfig = Next::PlatformInfo::PinConfig;

    static std::pair<int, int> getAnalogPin(int pin) {
        auto it = PinConfig::ANALOG_PINS.find(pin);
        if (it == PinConfig::ANALOG_PINS.end()) {
            throw std::runtime_error("Invalid analog pin");
        }

        return it->second;
    }

    class Adc {
    public:
        void configure(int pin) {
            int adcNum;
            int channel;
            std::tie(adcNum, channel) = getAnalogPin(pin);

            esp_err_t err = adc1_config_width(static_cast<adc_bits_width_t>(ADC_WIDTH_BIT_DEFAULT));
            if (err != ESP_OK) {
                throw std::runtime_error(esp_err_to_name(err));
            }

            err = adc1_config_channel_atten(static_cast<adc1_channel_t>(channel), ADC_ATTEN_DB_11);
            if (err != ESP_OK) {
                throw std::runtime_error(esp_err_to_name(err));
            }
        }

        int read(int pin) {
            int adcNum;
            int channel;
            std::tie(adcNum, channel) = getAnalogPin(pin);

            return adc1_get_raw(static_cast<adc1_channel_t>(channel)) >> (ADC_WIDTH_BIT_DEFAULT - 10);
        }
    };

public:
    Adc adc;

    void initialize() {
        Next::initialize();

        jac::FunctionFactory ff(this->context());

        jac::Module& adcModule = this->newModule("adc");

        adcModule.addExport("configure", ff.newFunction(noal::function(&Adc::configure, &adc)));
        adcModule.addExport("read", ff.newFunction(noal::function(&Adc::read, &adc)));
    }
};
