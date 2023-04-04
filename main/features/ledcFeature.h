#pragma once

#include <jac/machine/machine.h>
#include <jac/machine/functionFactory.h>

#include <stdexcept>
#include <set>

#include "driver/ledc.h"


template<class Next>
class LedcFeature : public Next {
    class Ledc {
        std::set<int> _usedTimers;
        std::set<int> _usedChannels;
    public:
        void configureTimer(int timerNum, int frequency) {
            if (frequency < 1 || frequency > 5000) {
                throw std::runtime_error("Frequency must be between 1 and 5000");
            }
            if (_usedTimers.find(timerNum) != _usedTimers.end()) {
                throw std::runtime_error("Timer already in use");
            }

            ledc_timer_config_t ledc_timer = {
                .speed_mode = LEDC_LOW_SPEED_MODE,
                .duty_resolution = LEDC_TIMER_10_BIT,
                .timer_num = static_cast<ledc_timer_t>(timerNum),
                .freq_hz = static_cast<uint32_t>(frequency),
                .clk_cfg = LEDC_AUTO_CLK
            };
            esp_err_t err = ledc_timer_config(&ledc_timer);
            if (err != ESP_OK) {
                throw std::runtime_error(esp_err_to_name(err));
            }

            _usedTimers.insert(timerNum);
        }

        void configureChannel(int channelNum, int gpioNum, int timerNum, int duty = 512) {
            if (duty < 0 || duty > 1023) {
                throw std::runtime_error("Duty must be between 0 and 1023");
            }
            if (_usedChannels.find(channelNum) != _usedChannels.end()) {
                throw std::runtime_error("Channel already in use");
            }

            ledc_channel_config_t ledc_channel = {
                .gpio_num = Next::getDigitalPin(gpioNum),
                .speed_mode = LEDC_LOW_SPEED_MODE,
                .channel = static_cast<ledc_channel_t>(channelNum),
                .intr_type = LEDC_INTR_DISABLE,
                .timer_sel = static_cast<ledc_timer_t>(timerNum),
                .duty = static_cast<uint32_t>(duty),
                .hpoint = 0
            };
            esp_err_t err = ledc_channel_config(&ledc_channel);
            if (err != ESP_OK) {
                throw std::runtime_error(esp_err_to_name(err));
            }

            _usedChannels.insert(channelNum);
        }

        void setFrequency(int timerNum, int frequency) {
            if (frequency < 1 || frequency > 5000) {
                throw std::runtime_error("Frequency must be between 1 and 5000");
            }
            if (_usedTimers.find(timerNum) == _usedTimers.end()) {
                throw std::runtime_error("Timer not in use");
            }

            esp_err_t err = ledc_set_freq(LEDC_LOW_SPEED_MODE, static_cast<ledc_timer_t>(timerNum), frequency);
            if (err != ESP_OK) {
                throw std::runtime_error(esp_err_to_name(err));
            }
        }

        void setDuty(int channelNum, int duty) {
            if (duty < 0 || duty > 1023) {
                throw std::runtime_error("Duty must be between 0 and 1023");
            }
            if (_usedChannels.find(channelNum) == _usedChannels.end()) {
                throw std::runtime_error("Channel not in use");
            }

            esp_err_t err = ledc_set_duty(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(channelNum), duty);
            if (err != ESP_OK) {
                throw std::runtime_error(esp_err_to_name(err));
            }

            err = ledc_update_duty(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(channelNum));
            if (err != ESP_OK) {
                throw std::runtime_error(esp_err_to_name(err));
            }
        }

        void stop(int channelNum) {
            if (_usedChannels.find(channelNum) == _usedChannels.end()) {
                throw std::runtime_error("Channel not in use");
            }

            esp_err_t err = ledc_stop(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(channelNum), 0);
            if (err != ESP_OK) {
                throw std::runtime_error(esp_err_to_name(err));
            }
        }

        ~Ledc() {
            for (auto channel : _usedChannels) {
                ledc_stop(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(channel), 0);
            }
            for (auto timer : _usedTimers) {
                ledc_timer_rst(LEDC_LOW_SPEED_MODE, static_cast<ledc_timer_t>(timer));
            }
        }
    };
public:
    Ledc ledc;

    void initialize() {
        Next::initialize();

        jac::FunctionFactory ff(this->_context);

        jac::JSModule& ledcModule = this->newModule("ledc");
        ledcModule.addExport("configureTimer", ff.newFunction(noal::function(&Ledc::configureTimer, &ledc)));
        ledcModule.addExport("configureChannel", ff.newFunction(noal::function(&Ledc::configureChannel, &ledc)));
        ledcModule.addExport("setFrequency", ff.newFunction(noal::function(&Ledc::setFrequency, &ledc)));
        ledcModule.addExport("setDuty", ff.newFunction(noal::function(&Ledc::setDuty, &ledc)));
    }
};
