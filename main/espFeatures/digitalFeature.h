#pragma once


#include <jac/machine/machine.h>
#include <jac/machine/values.h>
#include <jac/machine/functionFactory.h>

#include "driver/gpio.h"


enum class DigitalMode : int {
    DISABLE = 0,
    OUTPUT = 1,
    OUTPUT_OPEN_DRAIN = 2,
    INPUT = 3,
    INPUT_PULLUP = 4,
    INPUT_PULLDOWN = 5,
    INPUT_PULLUPDOWN = 6
};

enum class DigitalEdge : int {
    DISABLE = 0,
    RISING = 1,
    FALLING = 2,
    CHANGE = 3
};

template<>
struct jac::ConvTraits<DigitalMode> {
    static Value to(ContextRef ctx, DigitalMode val) {
        int mode = static_cast<int>(val);
        return Value::from(ctx, mode);
    }

    static DigitalMode from(ContextRef ctx, ValueWeak val) {
        int mode = val.to<int>();
        if (mode < 0 || mode > 6) {
            throw jac::Exception::create(jac::Exception::Type::TypeError, "Invalid digital mode");
        }
        return static_cast<DigitalMode>(mode);
    }
};

template<>
struct jac::ConvTraits<DigitalEdge> {
    static Value to(ContextRef ctx, DigitalEdge val) {
        int mode = static_cast<int>(val);
        return Value::from(ctx, mode);
    }

    static DigitalEdge from(ContextRef ctx, ValueWeak val) {
        int mode = val.to<int>();
        if (mode < 0 || mode > 3) {
            throw jac::Exception::create(jac::Exception::Type::TypeError, "Invalid digital edge");
        }
        return static_cast<DigitalEdge>(mode);
    }
};

namespace detail {

    template<size_t N>
    class InterruptQueue {
        using ArrayType = std::array<DigitalEdge, N>;
        ArrayType queue;
        ArrayType::iterator head = queue.begin();
        ArrayType::iterator tail = queue.begin();

        auto next(ArrayType::iterator it) {
            it++;
            return it == queue.end() ? queue.begin() : it;
        }

    public:
        bool push(DigitalEdge mode) {
            if (next(tail) == head) {
                return false;
            }
            *tail = mode;
            tail = next(tail);
            return true;
        }

        DigitalEdge pop() {
            if (head == tail) {
                return DigitalEdge::DISABLE;
            }
            DigitalEdge mode = *head;
            head = next(head);
            return mode;
        }
    };

    struct InterruptConf {
        const TickType_t _debounceTicks;
        InterruptQueue<16> queue;
        bool lastRising = false;
        TickType_t lastTime = 0;

        DigitalEdge _mode = DigitalEdge::DISABLE;
        bool _isAssigned = false;
        bool _synchronous; // TODO: maybe remove asynchronous mode?
        std::function<void(bool, std::chrono::time_point<std::chrono::steady_clock>)> _callback;

        InterruptConf(int debounceMs = 0) : _debounceTicks(debounceMs / portTICK_PERIOD_MS) {}

        bool updateLast(bool risingEdge) {
            auto now = xTaskGetTickCountFromISR();

            // TODO: replace naive debounce with a better solution
            if (now - lastTime < _debounceTicks) {
                return false;
            }
            if (_mode == DigitalEdge::FALLING && risingEdge) {
                return false;
            }
            if (_mode == DigitalEdge::RISING && !risingEdge) {
                return false;
            }

            lastTime = now;
            lastRising = risingEdge;
            return true;
        }

        void setCallback(std::function<void(bool, std::chrono::time_point<std::chrono::steady_clock>)> callback,
                         bool synchronous) {
            _isAssigned = false;
            _callback = callback;
            _synchronous = synchronous;
            _isAssigned = bool(callback);
        }
    };

} // namespace detail


template<typename Feature>
class Digital {
    Feature* const _feature;
    const gpio_num_t _pin;
    detail::InterruptConf _interruptConf;

    void enableInterrupt(DigitalEdge mode) {
        switch (mode) {
        case DigitalEdge::DISABLE:
            return;
        case DigitalEdge::RISING:
            gpio_set_intr_type(_pin, GPIO_INTR_POSEDGE);
            break;
        case DigitalEdge::FALLING:
            gpio_set_intr_type(_pin, GPIO_INTR_NEGEDGE);
            break;
        case DigitalEdge::CHANGE:
            gpio_set_intr_type(_pin, GPIO_INTR_ANYEDGE);
            break;
        }

        static constexpr auto call = +[](void* arg, std::chrono::time_point<std::chrono::steady_clock> time) {
            auto& self = *static_cast<Digital*>(arg);
            switch (self._interruptConf.queue.pop()) {
                case DigitalEdge::RISING:
                    self._interruptConf._callback(true, time);
                    break;
                case DigitalEdge::FALLING:
                    self._interruptConf._callback(false, time);
                    break;
                default:
                    break;
            }
        };

        gpio_isr_handler_add(_pin, [](void* arg) {
            auto& self = *static_cast<Digital*>(arg);
            bool risingEdge = gpio_get_level(self._pin) == 1;

            if (!self._interruptConf._isAssigned) {
                return;
            }

            if (!self._interruptConf.updateLast(risingEdge)) {
                return;
            }

            self._interruptConf.queue.push(risingEdge ? DigitalEdge::RISING : DigitalEdge::FALLING);
            self._feature->scheduleEventISR(call, &self);
        }, this);
        gpio_intr_enable(_pin);

        _interruptConf._mode = mode;
    }

    void disableInterrupt() {
        if (_interruptConf._isAssigned) {
            gpio_intr_disable(_pin);
            gpio_isr_handler_remove(_pin);

            _interruptConf._isAssigned = false;

            // FIXME: callback may still be scheduled in the event queue/being executed
            //        so the callback can't be safely destroyed (by delaying the destruction
            //        the chance of a crash is reduced but not eliminated)
            // _interruptConf._callback = nullptr;
        }
    }

    void pinMode(DigitalMode mode) {
        switch (mode) {
        case DigitalMode::DISABLE:
            gpio_set_direction(_pin, GPIO_MODE_DISABLE);
            break;
        case DigitalMode::OUTPUT:
            gpio_set_direction(_pin, GPIO_MODE_OUTPUT);
            break;
        case DigitalMode::INPUT:
            gpio_set_direction(_pin, GPIO_MODE_INPUT);
            gpio_set_pull_mode(_pin, GPIO_FLOATING);
            break;
        case DigitalMode::INPUT_PULLUP:
            gpio_set_direction(_pin, GPIO_MODE_INPUT);
            gpio_set_pull_mode(_pin, GPIO_PULLUP_ONLY);
            break;
        case DigitalMode::INPUT_PULLDOWN:
            gpio_set_direction(_pin, GPIO_MODE_INPUT);
            gpio_set_pull_mode(_pin, GPIO_PULLDOWN_ONLY);
            break;
        case DigitalMode::INPUT_PULLUPDOWN:
            gpio_set_direction(_pin, GPIO_MODE_INPUT);
            gpio_set_pull_mode(_pin, GPIO_PULLUP_PULLDOWN);
            break;
        case DigitalMode::OUTPUT_OPEN_DRAIN:
            gpio_set_direction(_pin, GPIO_MODE_OUTPUT_OD);
            break;
        }
    }
public:
    Digital(Feature* feature, int pin, DigitalMode mode) :
        _feature(feature),
        _pin(Feature::getDigitalPin(pin)),
        _interruptConf(0)
    {
        pinMode(mode);
    }

    Digital(Feature* feature, int pin, DigitalMode mode, DigitalEdge interruptMode,
            std::function<void(bool, std::chrono::time_point<std::chrono::steady_clock>)> callback,
            int debounceMs):
        _feature(feature),
        _pin(Feature::getDigitalPin(pin)),
        _interruptConf(debounceMs)
    {
        pinMode(mode);
        enableInterrupt(interruptMode);
        _interruptConf.setCallback(callback, true);
    }

    Digital(const Digital&) = delete;
    Digital& operator=(const Digital&) = delete;
    Digital(Digital&&) = delete;
    Digital& operator=(Digital&&) = delete;

    ~Digital() {
        close();
    }

    void write(bool value) {
        gpio_set_level(_pin, value);
    }

    bool read() {
        return gpio_get_level(_pin);
    }

    void close() {
        disableInterrupt();
        pinMode(DigitalMode::DISABLE);
    }
};

template<class Feature>
struct DigitalProtoBuilder : public jac::ProtoBuilder::Opaque<Digital<Feature>>, public jac::ProtoBuilder::Properties {
    using Digital_ = Digital<Feature>;

    static Digital_* constructOpaque(jac::ContextRef ctx, std::vector<jac::ValueWeak> args) {
        // TODO: extend instance lifetime until close or program end
        // TODO: check if pin is already in use

        if (args.size() < 1) {
            throw jac::Exception::create(jac::Exception::Type::TypeError, "Digital constructor requires an options argument");
        }

        jac::ObjectWeak options = args[0].to<jac::ObjectWeak>();

        int pin = options.get<int>("pin");
        DigitalMode mode = options.get<DigitalMode>("mode");
        if (options.hasProperty("onReadable")) {
            jac::Function callback = options.get<jac::Function>("onReadable");
            DigitalEdge interruptMode = options.get<DigitalEdge>("edge");
            int debounceMs = 10;
            if (options.hasProperty("debounce")) {
                debounceMs = options.get<int>("debounce");
            }

            return new Digital_(static_cast<Feature*>(JS_GetContextOpaque(ctx)), pin, mode, interruptMode,
                [ctx, callback](bool risingEdge, std::chrono::time_point<std::chrono::steady_clock> timestamp) mutable {
                    jac::Object arg = jac::Object::create(ctx);
                    arg.set("edge", risingEdge ? DigitalEdge::RISING : DigitalEdge::FALLING);
                    arg.set<int>("timestamp", std::chrono::duration_cast<std::chrono::milliseconds>(timestamp.time_since_epoch()).count());
                    callback.call<void>(arg);
                },
                debounceMs
            );
        }
        else {
            return new Digital_(static_cast<Feature*>(JS_GetContextOpaque(ctx)), pin, mode);
        }
    }

    static void addProperties(jac::ContextRef ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        // TODO: add required properties

        DigitalProtoBuilder::template addMethodMember<void(Digital_::*)(bool), &Digital_::write>(ctx, proto, "write", jac::PropFlags::Enumerable);
        DigitalProtoBuilder::template addMethodMember<bool(Digital_::*)(), &Digital_::read>(ctx, proto, "read", jac::PropFlags::Enumerable);
        DigitalProtoBuilder::template addMethodMember<void(Digital_::*)(), &Digital_::close>(ctx, proto, "close", jac::PropFlags::Enumerable);
    }
};


template<class Next>
class DigitalFeature : public Next {
    using PinConfig = typename Next::PlatformInfo::PinConfig;
public:
    using DigitalClass = jac::Class<DigitalProtoBuilder<DigitalFeature>>;

    DigitalFeature() {
        DigitalClass::init("Digital");
    }

    void initialize() {
        Next::initialize();

        for (auto pin : PinConfig::DIGITAL_PINS) {
            gpio_set_direction(Next::getDigitalPin(pin), GPIO_MODE_DISABLE);
        }
        for (auto pin : PinConfig::INTERRUPT_PINS) {
            gpio_intr_disable(Next::getDigitalPin(pin));
        }

        gpio_install_isr_service(0);

        auto& module = this->newModule("embedded:io/digital");

        DigitalClass::initContext(this->context());

        jac::Object digitalConstructor = DigitalClass::getConstructor(this->context());

        module.addExport("Digital", digitalConstructor);

        digitalConstructor.set("Input", DigitalMode::INPUT);
        digitalConstructor.set("InputPullUp", DigitalMode::INPUT_PULLUP);
        digitalConstructor.set("InputPullDown", DigitalMode::INPUT_PULLDOWN);
        digitalConstructor.set("InputPullUpDown", DigitalMode::INPUT_PULLUPDOWN);
        digitalConstructor.set("Output", DigitalMode::OUTPUT);
        digitalConstructor.set("OutputOpenDrain", DigitalMode::OUTPUT_OPEN_DRAIN);

        digitalConstructor.set("None", DigitalEdge::DISABLE);
        digitalConstructor.set("Rising", DigitalEdge::RISING);
        digitalConstructor.set("Falling", DigitalEdge::FALLING);
    }

    ~DigitalFeature() {
        for (auto pin : PinConfig::INTERRUPT_PINS) {
            gpio_intr_disable(Next::getDigitalPin(pin));
            gpio_isr_handler_remove(Next::getDigitalPin(pin));
        }

        gpio_uninstall_isr_service();
    }
};
