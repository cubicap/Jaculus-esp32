#pragma once

#include <jac/machine/machine.h>
#include <jac/machine/values.h>
#include <jac/machine/functionFactory.h>
#include <noal_func.h>
#include <memory>
#include <map>
#include "driver/gpio.h"

template<class Next>
class GpioFeature : public Next {
public:
    class Gpio {
    public:
        enum class DIRECTION : int {
            DISABLE = static_cast<int>(GPIO_MODE_DISABLE),
            INPUT = static_cast<int>(GPIO_MODE_INPUT),
            OUTPUT = static_cast<int>(GPIO_MODE_OUTPUT),
            OUTPUT_OD = static_cast<int>(GPIO_MODE_OUTPUT_OD),
            INPUT_OUTPUT_OD = static_cast<int>(GPIO_MODE_INPUT_OUTPUT_OD),
            INPUT_OUTPUT = static_cast<int>(GPIO_MODE_INPUT_OUTPUT),
        };

        enum class PULL_MODE : int {
            FLOATING = static_cast<int>(GPIO_FLOATING),
            PULL_UP = static_cast<int>(GPIO_PULLUP_ONLY),
            PULL_DOWN = static_cast<int>(GPIO_PULLDOWN_ONLY),
            PULL_UP_DOWN = static_cast<int>(GPIO_PULLUP_PULLDOWN),
        };

        void pinMode(int pin, int mode) {
            // gpio_pad_select_gpio(pin);
            gpio_set_direction((gpio_num_t)pin, (gpio_mode_t)mode);
        }

        void pullMode(int pin, int mode) {
            gpio_set_pull_mode((gpio_num_t)pin, (gpio_pull_mode_t)mode);
        }

        void write(int pin, int value) {
            gpio_set_level((gpio_num_t)pin, value);
        }

        int read(int pin) {
            return gpio_get_level((gpio_num_t)pin);
        }
    };
    Gpio gpio;

    void initialize() {
        Next::initialize();

        jac::FunctionFactory ff(this->_context);

        auto& module = this->newModule("gpio");
        module.addExport("pinMode", ff.newFunction(noal::function(&Gpio::pinMode, &gpio)));
        module.addExport("pullMode", ff.newFunction(noal::function(&Gpio::pullMode, &gpio)));
        module.addExport("write", ff.newFunction(noal::function(&Gpio::write, &gpio)));
        module.addExport("read", ff.newFunction(noal::function(&Gpio::read, &gpio)));

        module.addExport("DISABLE", jac::Value::from(this->_context, static_cast<int>(Gpio::DIRECTION::DISABLE)));
        module.addExport("INPUT", jac::Value::from(this->_context, static_cast<int>(Gpio::DIRECTION::INPUT)));
        module.addExport("OUTPUT", jac::Value::from(this->_context, static_cast<int>(Gpio::DIRECTION::OUTPUT)));
        module.addExport("OUTPUT_OD", jac::Value::from(this->_context, static_cast<int>(Gpio::DIRECTION::OUTPUT_OD)));
        module.addExport("INPUT_OUTPUT_OD", jac::Value::from(this->_context, static_cast<int>(Gpio::DIRECTION::INPUT_OUTPUT_OD)));
        module.addExport("INPUT_OUTPUT", jac::Value::from(this->_context, static_cast<int>(Gpio::DIRECTION::INPUT_OUTPUT)));

        module.addExport("FLOATING", jac::Value::from(this->_context, static_cast<int>(Gpio::PULL_MODE::FLOATING)));
        module.addExport("PULL_UP", jac::Value::from(this->_context, static_cast<int>(Gpio::PULL_MODE::PULL_UP)));
        module.addExport("PULL_DOWN", jac::Value::from(this->_context, static_cast<int>(Gpio::PULL_MODE::PULL_DOWN)));
        module.addExport("PULL_UP_DOWN", jac::Value::from(this->_context, static_cast<int>(Gpio::PULL_MODE::PULL_UP_DOWN)));
    }
};
