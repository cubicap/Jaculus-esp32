#pragma once


#include <jac/machine/machine.h>
#include <jac/machine/values.h>
#include <jac/machine/functionFactory.h>
#include <map>
#include <memory>
#include <set>
#include "driver/gpio.h"


enum class PinMode {
    DISABLE,
    OUTPUT,
    INPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN
};

enum class InterruptMode {
    DISABLE = static_cast<int>(GPIO_INTR_DISABLE),
    RISING = static_cast<int>(GPIO_INTR_POSEDGE),
    FALLING = static_cast<int>(GPIO_INTR_NEGEDGE),
    CHANGE = static_cast<int>(GPIO_INTR_ANYEDGE),
};

template<>
struct jac::ConvTraits<PinMode> {
    static Value to(ContextRef ctx, PinMode val) {
        int mode = static_cast<int>(val);
        return Value::from(ctx, mode);
    }

    static PinMode from(ContextRef ctx, ValueConst val) {
        int mode = val.to<int>();
        return static_cast<PinMode>(mode);
    }
};


template<class Next>
class GpioFeature : public Next {
    using PinConfig = Next::PlatformInfo::PinConfig;

    static gpio_num_t getDigitalPin(int pin) {
        if (PinConfig::DIGITAL_PINS.find(pin) == PinConfig::DIGITAL_PINS.end()) {
            throw std::runtime_error("Invalid digital pin");
        }
        return static_cast<gpio_num_t>(pin);
    }

    static gpio_num_t getAnalogPin(int pin) {
        if (PinConfig::ANALOG_PINS.find(pin) == PinConfig::ANALOG_PINS.end()) {
            throw std::runtime_error("Invalid analog pin");
        }
        return static_cast<gpio_num_t>(pin);
    }

    static gpio_num_t getInterruptPin(int pin) {
        if (PinConfig::INTERRUPT_PINS.find(pin) == PinConfig::INTERRUPT_PINS.end()) {
            throw std::runtime_error("Invalid interrupt pin");
        }
        return static_cast<gpio_num_t>(pin);
    }
public:

    class Gpio {
        class InterruptQueue {
            std::array<std::shared_ptr<std::function<void()>>, 32> queue;
            std::array<std::shared_ptr<std::function<void()>>, 32>::iterator head = queue.begin();
            std::array<std::shared_ptr<std::function<void()>>, 32>::iterator tail = queue.begin();

            auto next(std::array<std::shared_ptr<std::function<void()>>, 32>::iterator it) {
                it++;
                return it == queue.end() ? queue.begin() : it;
            }

        public:
            bool push(std::shared_ptr<std::function<void()>> callback) {
                if (next(tail) == head) {
                    return false;
                }
                *tail = callback;
                tail = next(tail);
                return true;
            }

            std::shared_ptr<std::function<void()>> pop() {
                if (head == tail) {
                    return nullptr;
                }
                auto callback = *head;
                *head = nullptr;
                head = next(head);
                return callback;
            }
        };
        InterruptQueue _interruptQueue;

        class Interrupts {
            std::pair<std::shared_ptr<std::function<void()>>, bool> rising;
            std::pair<std::shared_ptr<std::function<void()>>, bool> falling;
            std::pair<std::shared_ptr<std::function<void()>>, bool> change;
            gpio_num_t pin;
            GpioFeature* _feature;

        public:
            Interrupts(gpio_num_t pin, GpioFeature* feature) : pin(pin), _feature(feature) {}

            std::pair<std::shared_ptr<std::function<void()>>, bool>& operator[](InterruptMode mode) {
                switch (mode) {
                    case InterruptMode::RISING:
                        return rising;
                    case InterruptMode::FALLING:
                        return falling;
                    case InterruptMode::CHANGE:
                        return change;
                    default:
                        throw std::runtime_error("Invalid interrupt mode");
                }
            }

            operator bool() const {
                return rising.first || falling.first || change.first;
            }

            gpio_num_t getPin() const {
                return pin;
            }

            GpioFeature* getFeature() const {
                return _feature;
            }
        };

        std::map<int, std::unique_ptr<Interrupts>> _interruptCallbacks;
        GpioFeature* _feature;
    public:
        Gpio(GpioFeature* feature) : _feature(feature) {}

        void pinMode(int pinNum, PinMode mode) {
            gpio_num_t pin = getDigitalPin(pinNum);

            switch (mode) {
            case PinMode::DISABLE:
                gpio_set_direction(pin, GPIO_MODE_DISABLE);
                break;
            case PinMode::OUTPUT:
                gpio_set_direction(pin, GPIO_MODE_OUTPUT);
                break;
                break;
            case PinMode::INPUT:
                gpio_set_direction(pin, GPIO_MODE_INPUT);
                gpio_set_pull_mode(pin, GPIO_FLOATING);
                break;
            case PinMode::INPUT_PULLUP:
                gpio_set_direction(pin, GPIO_MODE_INPUT);
                gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
                break;
            case PinMode::INPUT_PULLDOWN:
                gpio_set_direction(pin, GPIO_MODE_INPUT);
                gpio_set_pull_mode(pin, GPIO_PULLDOWN_ONLY);
                break;
            }
        }

        void write(int pinNum, int value) {
            gpio_num_t pin = getDigitalPin(pinNum);
            gpio_set_level(pin, value);
        }

        int read(int pinNum) {
            gpio_num_t pin = getDigitalPin(pinNum);
            return gpio_get_level(pin);
        }

        void attachInterrupt(int pinNum, InterruptMode mode, std::function<void()> callback, bool synchronous) {
            gpio_num_t pin = getInterruptPin(pinNum);

            if (_interruptCallbacks.find(pinNum) == _interruptCallbacks.end()) {
                _interruptCallbacks[pinNum] = std::make_unique<Interrupts>(pin, _feature);
                gpio_set_intr_type(pin, GPIO_INTR_ANYEDGE);
                gpio_isr_handler_add(pin, [](void* arg) {
                    auto& callbacks = *static_cast<Interrupts*>(arg);
                    auto* feature = callbacks.getFeature();
                    auto& interruptQueue = feature->gpio._interruptQueue;

                    static void (*call)(void*) = [](void* arg) {
                        auto& queue = *static_cast<InterruptQueue*>(arg);
                        auto callback = queue.pop();
                        if (callback) {
                            (*callback)();
                        }
                    };

                    if (callbacks[InterruptMode::CHANGE].first) {
                        if (callbacks[InterruptMode::CHANGE].second) {
                            interruptQueue.push(callbacks[InterruptMode::CHANGE].first);
                            feature->scheduleEventISR(call, &interruptQueue);
                        } else {
                            (*callbacks[InterruptMode::CHANGE].first)();
                        }
                    }
                    if (callbacks[InterruptMode::RISING].first && gpio_get_level(callbacks.getPin()) == 1) {
                        if (callbacks[InterruptMode::RISING].second) {
                            interruptQueue.push(callbacks[InterruptMode::RISING].first);
                            feature->scheduleEventISR(call, &interruptQueue);
                        } else {
                            (*callbacks[InterruptMode::RISING].first)();
                        }
                    }
                    if (callbacks[InterruptMode::FALLING].first && gpio_get_level(callbacks.getPin()) == 0) {
                        if (callbacks[InterruptMode::FALLING].second) {
                            interruptQueue.push(callbacks[InterruptMode::FALLING].first);
                            feature->scheduleEventISR(call, &interruptQueue);
                        } else {
                            (*callbacks[InterruptMode::FALLING].first)();
                        }
                    }
                }, _interruptCallbacks[pinNum].get());
                gpio_intr_enable(pin);
            }

            if ((*_interruptCallbacks[pinNum])[mode].first) {
                throw std::runtime_error("Interrupt already attached");
            }

            (*_interruptCallbacks[pinNum])[mode] = std::make_pair(std::make_unique<std::function<void()>>(std::move(callback)), synchronous);
        }

        void detachInterrupt(int pinNum, InterruptMode mode) {
            gpio_num_t pin = getInterruptPin(pinNum);

            if (_interruptCallbacks.find(pinNum) == _interruptCallbacks.end() || !(*_interruptCallbacks[pinNum])[mode].first) {
                throw std::runtime_error("Interrupt not attached");
            }

            (*_interruptCallbacks[pinNum])[mode].first = nullptr;

            if (!_interruptCallbacks[pinNum]) {
                gpio_intr_disable(pin);
                gpio_isr_handler_remove(pin);
                _interruptCallbacks.erase(pinNum);
            }
        }

        void on(std::string event, int pinNum, std::function<void()> callback) {
            if (event == "rising") {
                attachInterrupt(pinNum, InterruptMode::RISING, std::move(callback), true);
            }
            else if (event == "falling") {
                attachInterrupt(pinNum, InterruptMode::FALLING, std::move(callback), true);
            }
            else if (event == "change") {
                attachInterrupt(pinNum, InterruptMode::CHANGE, std::move(callback), true);
            }
            else {
                throw std::runtime_error("Invalid event");
            }
        }

        void off(std::string event, int pinNum) {
            if (event == "rising") {
                detachInterrupt(pinNum, InterruptMode::RISING);
            }
            else if (event == "falling") {
                detachInterrupt(pinNum, InterruptMode::FALLING);
            }
            else if (event == "change") {
                detachInterrupt(pinNum, InterruptMode::CHANGE);
            }
            else {
                throw std::runtime_error("Invalid event");
            }
        }
    };
    Gpio gpio;

    void initialize() {
        Next::initialize();

        for (auto pin : PinConfig::DIGITAL_PINS) {
            gpio.pinMode(pin, PinMode::DISABLE);
        }
        for (auto pin : PinConfig::INTERRUPT_PINS) {
            gpio_intr_disable(getDigitalPin(pin));
        }

        gpio_install_isr_service(0);

        jac::FunctionFactory ff(this->_context);
        auto& module = this->newModule("gpio");
        module.addExport("pinMode", ff.newFunction(noal::function(&Gpio::pinMode, &gpio)));
        module.addExport("read", ff.newFunction(noal::function(&Gpio::read, &gpio)));
        module.addExport("write", ff.newFunction(noal::function(&Gpio::write, &gpio)));

        jac::Object pinModeEnum = jac::Object::create(this->_context);
        pinModeEnum.set("DISABLE", static_cast<int>(PinMode::DISABLE));
        pinModeEnum.set("OUTPUT", static_cast<int>(PinMode::OUTPUT));
        pinModeEnum.set("INPUT", static_cast<int>(PinMode::INPUT));
        pinModeEnum.set("INPUT_PULLUP", static_cast<int>(PinMode::INPUT_PULLUP));
        pinModeEnum.set("INPUT_PULLDOWN", static_cast<int>(PinMode::INPUT_PULLDOWN));
        module.addExport("PinMode", pinModeEnum);

        module.addExport("on", ff.newFunction([this](std::string event, int pin, jac::Function callback) {
            this->gpio.on(event, pin, [callback = std::move(callback)]() mutable {
                callback.call<void>();
            });
        }));
        module.addExport("off", ff.newFunction(noal::function(&Gpio::off, &gpio)));
    }

    GpioFeature() : gpio(this) {}

    ~GpioFeature() {
        for (auto pin : PinConfig::INTERRUPT_PINS) {
            gpio_intr_disable(getDigitalPin(pin));
            gpio_isr_handler_remove(getDigitalPin(pin));
        }

        gpio_uninstall_isr_service();
    }
};
