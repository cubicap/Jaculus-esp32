#pragma once


#include <jac/machine/machine.h>
#include <jac/machine/values.h>
#include <jac/machine/functionFactory.h>

#include <unordered_map>
#include <memory>
#include <array>

#include "driver/gpio.h"
#include "freertos/portmacro.h"


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

    static PinMode from(ContextRef ctx, ValueWeak val) {
        int mode = val.to<int>();
        return static_cast<PinMode>(mode);
    }
};


namespace detail {

    using Callback_t = std::function<void(std::chrono::time_point<std::chrono::steady_clock>)>;

    template<size_t N>
    class InterruptQueue {
        using ArrayType = std::array<std::shared_ptr<Callback_t>, N>;
        ArrayType queue;
        ArrayType::iterator head = queue.begin();
        ArrayType::iterator tail = queue.begin();

        auto next(ArrayType::iterator it) {
            it++;
            return it == queue.end() ? queue.begin() : it;
        }

    public:
        bool push(std::shared_ptr<Callback_t> callback) {
            if (next(tail) == head) {
                return false;
            }
            *tail = callback;
            tail = next(tail);
            return true;
        }

        std::shared_ptr<Callback_t> pop() {
            if (head == tail) {
                return nullptr;
            }
            auto callback = *head;
            *head = nullptr;
            head = next(head);
            return callback;
        }
    };

    template<class GpioFeature>
    class Interrupts {
        static constexpr size_t DEBOUNCE_TIME = 2;

        std::shared_ptr<Callback_t> rising;
        std::shared_ptr<Callback_t> falling;
        std::shared_ptr<Callback_t> change;
        gpio_num_t pin;
        TickType_t lastTime = 0;
        bool lastRising = false;
        GpioFeature* _feature;

    public:
        Interrupts(gpio_num_t pin, GpioFeature* feature) : pin(pin), _feature(feature) {}

        std::shared_ptr<Callback_t>& operator[](InterruptMode mode) {
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
            return rising || falling || change;
        }

        gpio_num_t getPin() const {
            return pin;
        }

        GpioFeature* getFeature() const {
            return _feature;
        }

        bool updateLast(bool risingEdge) {
            auto now = xTaskGetTickCountFromISR();
            if (lastRising == risingEdge && now - lastTime < DEBOUNCE_TIME) {
                return false;
            }
            lastTime = now;
            lastRising = risingEdge;
            return true;
        }
    };

} // namespace detail


template<class GpioFeature>
class Gpio {
    using InterruptQueue_ = detail::InterruptQueue<32>;
    using Interrupts_ = detail::Interrupts<GpioFeature>;

    InterruptQueue_ _interruptQueue;

    std::unordered_map<int, std::unique_ptr<Interrupts_>> _interruptCallbacks;
    GpioFeature* _feature;
public:
    Gpio(GpioFeature* feature) : _feature(feature) {}

    void pinMode(int pinNum, PinMode mode) {
        gpio_num_t pin = GpioFeature::getDigitalPin(pinNum);

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
        gpio_num_t pin = GpioFeature::getDigitalPin(pinNum);
        gpio_set_level(pin, value);
    }

    int read(int pinNum) {
        gpio_num_t pin = GpioFeature::getDigitalPin(pinNum);
        return gpio_get_level(pin);
    }

    void attachInterrupt(int pinNum, InterruptMode mode, detail::Callback_t callback) {
        gpio_num_t pin = GpioFeature::getInterruptPin(pinNum);

        if (_interruptCallbacks.find(pinNum) == _interruptCallbacks.end()) {
            _interruptCallbacks[pinNum] = std::make_unique<Interrupts_>(pin, _feature);
            gpio_set_intr_type(pin, GPIO_INTR_ANYEDGE);
            gpio_isr_handler_add(pin, [](void* arg) {
                auto& callbacks = *static_cast<Interrupts_*>(arg);
                auto* feature = callbacks.getFeature();
                auto& interruptQueue = feature->gpio._interruptQueue;

                static auto call = +[](void* arg, GpioFeature::TimePoint timestamp) {
                    auto& queue = *static_cast<InterruptQueue_*>(arg);

                    auto callback = queue.pop();
                    if (callback) {
                        (*callback)(timestamp);
                    }
                };

                bool risingEdge = gpio_get_level(callbacks.getPin()) == 1;
                if (!callbacks.updateLast(risingEdge)) {
                    return;
                }

                if (callbacks[InterruptMode::CHANGE]) {
                    interruptQueue.push(callbacks[InterruptMode::CHANGE]);
                    feature->scheduleEventISR(call, &interruptQueue);
                }
                if (callbacks[InterruptMode::RISING] && risingEdge) {
                    interruptQueue.push(callbacks[InterruptMode::RISING]);
                    feature->scheduleEventISR(call, &interruptQueue);
                }
                if (callbacks[InterruptMode::FALLING] && !risingEdge) {
                    interruptQueue.push(callbacks[InterruptMode::FALLING]);
                    feature->scheduleEventISR(call, &interruptQueue);
                }
            }, _interruptCallbacks[pinNum].get());
            gpio_intr_enable(pin);
        }

        if ((*_interruptCallbacks[pinNum])[mode]) {
            throw std::runtime_error("Interrupt already attached");
        }

        (*_interruptCallbacks[pinNum])[mode] = std::make_unique<detail::Callback_t>(std::move(callback));
    }

    void detachInterrupt(int pinNum, InterruptMode mode) {
        gpio_num_t pin = GpioFeature::getInterruptPin(pinNum);

        if (_interruptCallbacks.find(pinNum) == _interruptCallbacks.end() || !(*_interruptCallbacks[pinNum])[mode]) {
            throw std::runtime_error("Interrupt not attached");
        }

        (*_interruptCallbacks[pinNum])[mode] = nullptr;

        if (!_interruptCallbacks[pinNum]) {
            gpio_intr_disable(pin);
            gpio_isr_handler_remove(pin);
            _interruptCallbacks.erase(pinNum);
        }
    }

    void on(std::string event, int pinNum, detail::Callback_t callback) {
        if (event == "rising") {
            attachInterrupt(pinNum, InterruptMode::RISING, std::move(callback));
        }
        else if (event == "falling") {
            attachInterrupt(pinNum, InterruptMode::FALLING, std::move(callback));
        }
        else if (event == "change") {
            attachInterrupt(pinNum, InterruptMode::CHANGE, std::move(callback));
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


template<class Next>
class GpioFeature : public Next {
    using PinConfig = Next::PlatformInfo::PinConfig;
    using Gpio_ = Gpio<GpioFeature>;
public:
    Gpio_ gpio;

    void initialize() {
        Next::initialize();

        for (auto pin : PinConfig::DIGITAL_PINS) {
            gpio.pinMode(pin, PinMode::DISABLE);
        }
        for (auto pin : PinConfig::INTERRUPT_PINS) {
            gpio_intr_disable(Next::getDigitalPin(pin));
        }

        gpio_install_isr_service(0);

        jac::FunctionFactory ff(this->context());
        auto& module = this->newModule("gpio");
        module.addExport("pinMode", ff.newFunction(noal::function(&Gpio_::pinMode, &gpio)));
        module.addExport("read", ff.newFunction(noal::function(&Gpio_::read, &gpio)));
        module.addExport("write", ff.newFunction(noal::function(&Gpio_::write, &gpio)));

        // TODO: rename pinMode and PinMode to avoid confusion
        jac::Object pinModeEnum = jac::Object::create(this->context());
        pinModeEnum.set("DISABLE", static_cast<int>(PinMode::DISABLE));
        pinModeEnum.set("OUTPUT", static_cast<int>(PinMode::OUTPUT));
        pinModeEnum.set("INPUT", static_cast<int>(PinMode::INPUT));
        pinModeEnum.set("INPUT_PULLUP", static_cast<int>(PinMode::INPUT_PULLUP));
        pinModeEnum.set("INPUT_PULLDOWN", static_cast<int>(PinMode::INPUT_PULLDOWN));
        module.addExport("PinMode", pinModeEnum);

        module.addExport("on", ff.newFunction([this](std::string event, int pin, jac::Function callback) {
            this->gpio.on(event, pin, [ctx = this->context(), callback = std::move(callback)](Next::TimePoint timestamp) mutable {
                jac::Object info = jac::Object::create(ctx);
                info.set<int>("timestamp", std::chrono::duration_cast<std::chrono::milliseconds>(timestamp.time_since_epoch()).count());

                callback.call<void>(info);
            });
        }));
        module.addExport("off", ff.newFunction(noal::function(&Gpio_::off, &gpio)));
    }

    GpioFeature() : gpio(this) {}

    ~GpioFeature() {
        for (auto pin : PinConfig::INTERRUPT_PINS) {
            gpio_intr_disable(Next::getDigitalPin(pin));
            gpio_isr_handler_remove(Next::getDigitalPin(pin));
        }

        gpio_uninstall_isr_service();
    }
};
