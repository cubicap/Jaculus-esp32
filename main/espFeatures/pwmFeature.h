#pragma once

#include <jac/machine/machine.h>
#include <jac/machine/functionFactory.h>

#include <set>
#include <stdexcept>

#include "driver/ledc.h"


struct Timer {
    int _freq;
    int _resolution;
    int num;
    mutable std::atomic<int> _refs;

    Timer(int freq, int resolution, int num):
        _freq(freq),
        _resolution(resolution),
        num(num),
        _refs(0)
    {
        ledc_timer_config_t timerConfig = {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .duty_resolution = static_cast<ledc_timer_bit_t>(resolution),
            .timer_num = static_cast<ledc_timer_t>(num),
            .freq_hz = static_cast<uint32_t>(freq),
            .clk_cfg = LEDC_AUTO_CLK,
        };

        esp_err_t err = ledc_timer_config(&timerConfig);
        if (err != ESP_OK) {
            throw std::runtime_error(esp_err_to_name(err));
        }
    }

    void incRefs() const {
        _refs++;
    }

    bool decRefs() const {
        _refs--;
        return _refs == 0;
    }

    auto operator<=>(const std::pair<int, int>& p) const {
        return std::pair(_freq, _resolution) <=> p;
    }

    auto operator<=>(const Timer& other) const {
        return std::tuple(_freq, _resolution, num) <=> std::tuple(other._freq, other._resolution, other.num);
    }
};

template<class Feature>
class PWM {
public:
    // TODO: hide these (make view const)
    Feature* const _feature;
    gpio_num_t _pin;
    int _hz;
    int _duty;
    const Timer* _timer;
    int _channel;

private:
    PWM(Feature* feature, int pin, double hz, const Timer* timer, int channel):
        _feature(feature),
        _pin(Feature::getDigitalPin(pin)),
        _hz(hz),
        _duty(0),
        _timer(timer),
        _channel(channel)
    {
        ledc_channel_config_t ledc_channel = {
            .gpio_num = _pin,
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .channel = static_cast<ledc_channel_t>(_channel),
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = static_cast<ledc_timer_t>(timer->num),
            .duty = 0,
            .hpoint = 0,
            .flags = { 0 }
        };
        esp_err_t err = ledc_channel_config(&ledc_channel);
        if (err != ESP_OK) {
            throw std::runtime_error(esp_err_to_name(err));
        }
    }
public:

    PWM(Feature* feature, int pin, double hz, int resolution):
        PWM(feature, pin, hz,
            feature->takeTimer(hz, resolution),
            feature->takeChannel()
        )
    {}

    ~PWM() {
        close();
    }

    void write(int duty) {
        if (!_timer) {
            throw std::runtime_error("PWM is closed");
        }
        if (duty < 0 || duty > (1 << _timer->_resolution)) {
            throw std::runtime_error("Duty must be between 0 and 2^resolution");
        }

        esp_err_t err = ledc_set_duty(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(_channel), duty);
        if (err != ESP_OK) {
            throw std::runtime_error(esp_err_to_name(err));
        }

        err = ledc_update_duty(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(_channel));
        if (err != ESP_OK) {
            throw std::runtime_error(esp_err_to_name(err));
        }
        _duty = duty;
    }

    void close() {
        if (!_timer) {
            return;
        }
        ledc_stop(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(_channel), 0);
        _feature->releaseTimer(_timer);
        _timer = nullptr;
    }
};

template<class Feature>
struct PWMProtoBuilder : public jac::ProtoBuilder::Opaque<PWM<Feature>>, public jac::ProtoBuilder::Properties, public jac::ProtoBuilder::LifetimeHandles {
    using PWM_ = PWM<Feature>;

    static PWM_* constructOpaque(jac::ContextRef ctx, std::vector<jac::ValueWeak> args) {
        // TODO: check if pin is already in use

        if (args.size() < 1) {
            throw jac::Exception::create(jac::Exception::Type::TypeError, "PWM constructor requires an options argument");
        }

        jac::ObjectWeak options = args[0].to<jac::ObjectWeak>();

        int pin = options.get<int>("pin");
        double hz = options.get<double>("hz");
        int resolution = options.get<int>("resolution");

        return new PWM_(static_cast<Feature*>(JS_GetContextOpaque(ctx)), pin, hz, resolution);
    }

    static void postConstruction(jac::ContextRef ctx, jac::Object thisVal, std::vector<jac::ValueWeak> args) {
        PWM_& self = *PWMProtoBuilder::getOpaque(ctx, thisVal);
        self._feature->extendLifetime(thisVal);
    }

    static void addProperties(jac::ContextRef ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        // TODO: add required properties
        PWMProtoBuilder::template addMethodMember<void(PWM_::*)(int), &PWM_::write>(ctx, proto, "write", jac::PropFlags::Enumerable);

        PWMProtoBuilder::template addPropMember<int, &PWM_::_hz>(ctx, proto, "hz", jac::PropFlags::Enumerable);

        proto.defineProperty("timer", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            PWM_& pwm = *PWMProtoBuilder::getOpaque(ctx, thisVal);
            return pwm._timer->num;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("close", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            PWM_& self = *PWMProtoBuilder::getOpaque(ctx, thisVal);
            self.close();
            self._feature->releaseLifetime(thisVal);
        }), jac::PropFlags::Enumerable);
    }
};

template<class Next>
class PwmFeature : public Next {
    std::set<Timer, std::less<>> _timers;
    std::set<int> _usedTimers;
    std::set<int> _usedChannels;
public:
    int findMin(std::set<int>& set) {
        int last = 0;
        for (int item : set) {
            if (item - last > 1) {
                break;
            }
            last = item;
        }
        return last + 1;
    }

    const Timer* takeTimer(int freq, int resolution) {
        auto it = _timers.find(std::make_pair(freq, resolution));
        if (it == _timers.end()) {
            int num = findMin(_usedTimers);
            if (num == LEDC_TIMER_MAX) {
                throw std::runtime_error("No timers available");
            }
            it = _timers.emplace(freq, resolution, _timers.size()).first;
            _usedTimers.insert(num);
        }
        it->incRefs();
        return &*it;
    }

    void releaseTimer(const Timer* timer) {
        if (timer->decRefs()) {
            _timers.erase(*timer);
            _usedTimers.erase(timer->num);
        }
    }

    int takeChannel() {
        int num = findMin(_usedChannels);

        if (num == LEDC_CHANNEL_MAX) {
            throw std::runtime_error("No channels available");
        }

        _usedChannels.insert(num);
        return num;
    }

    void releaseChannel(int channel) {
        _usedChannels.erase(channel);
    }

    using PWMClass = jac::Class<PWMProtoBuilder<PwmFeature>>;

    PwmFeature() {
        PWMClass::init("PWM");
    }

    void initialize() {
        Next::initialize();

        jac::Module& pwmModule = this->newModule("embedded:io/pwm");

        PWMClass::initContext(this->context());

        pwmModule.addExport("PWM", PWMClass::getConstructor(this->context()));
    }
};
