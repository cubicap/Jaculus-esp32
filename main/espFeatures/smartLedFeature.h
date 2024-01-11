#pragma once

#include <jac/machine/machine.h>
#include <jac/machine/functionFactory.h>
#include <jac/machine/class.h>

#include <noal_func.h>
#include <memory>
#include <unordered_map>
#include <SmartLeds.h>

#include <set>


template<>
struct jac::ConvTraits<Rgb> {
    static Value to(ContextRef ctx, Rgb val) {
        auto obj = Object::create(ctx);
        obj.set<int>("r", val.r);
        obj.set<int>("g", val.g);
        obj.set<int>("b", val.b);
        return obj;
    }

    static Rgb from(ContextRef ctx, ValueWeak val) {
        auto obj = val.to<Object>();
        return Rgb(obj.get<int>("r"), obj.get<int>("g"), obj.get<int>("b"));
    }
};

template<>
struct jac::ConvTraits<LedType> {
    static Value to(ContextRef ctx, LedType val) {
        auto obj = Object::create(ctx);
        obj.set<int>("T0H", val.T0H);
        obj.set<int>("T1H", val.T1H);
        obj.set<int>("T0L", val.T0L);
        obj.set<int>("T1L", val.T1L);
        obj.set<int>("TRS", val.TRS);
        return obj;
    }

    static LedType from(ContextRef ctx, ValueWeak val) {
        auto obj = val.to<Object>();
        return LedType(obj.get<int>("T0H"), obj.get<int>("T1H"), obj.get<int>("T0L"), obj.get<int>("T1L"), obj.get<int>("TRS"));
    }
};

template<class Feature>
class SmartLedWrapper {
    std::optional<SmartLed> _led;

public:
    SmartLedWrapper(LedType type, int count, int pin) {
        gpio_num_t gpio = Feature::getDigitalPin(pin);

        int channel = 0;
        while (Feature::_usedRmtChannels.find(channel) != Feature::_usedRmtChannels.end()) {
            channel++;
        }
        if (channel >= 4) {
            throw std::runtime_error("No available RMT channels");
        }
        Feature::_usedRmtChannels.insert(channel);

        _led.emplace(type, count, gpio, channel, SingleBuffer);

        _led->show();
    }

    void close() {
        if (_led) {
            int channel = _led->channel();
            _led->wait();
            _led = std::nullopt;

            Feature::_usedRmtChannels.erase(channel);
        }
    }

    void begin(int x, int y, int width, int height) {
        if (!_led) {
            throw std::runtime_error("SmartLed is closed");
        }
        if (x != 0 || y != 0 || width != _led->size() || height != 1) {
            throw std::runtime_error("SmartLed only supports full width");
        }
    }

    void send(std::span<Rgb> data) {
        if (!_led) {
            throw std::runtime_error("SmartLed is closed");
        }
        if (data.size() != _led->size()) {
            throw std::runtime_error("SmartLed only supports full width");
        }

        std::copy(data.begin(), data.end(), _led->begin());
        _led->show();
    }

    ~SmartLedWrapper() {
        close();
    }
};

template<typename Feature>
struct SmartLedProtoBuilder : public jac::ProtoBuilder::Opaque<SmartLedWrapper<Feature>>, public jac::ProtoBuilder::Properties {
    using SmartLed_ = SmartLedWrapper<Feature>;

    static SmartLed_* constructOpaque(JSContext* ctx, std::vector<jac::ValueWeak> args) {
        if (args.size() < 1) {
            throw jac::Exception::create(jac::Exception::Type::TypeError, "SmartLed constructor requires an options argument");
        }

        jac::ObjectWeak options = args[0].to<jac::ObjectWeak>();

        int pin = options.get<int>("pin");
        int count = options.get<int>("count");

        LedType type = LED_WS2812;
        if (options.hasProperty("type")) {
            type = options.get<LedType>("type");
        }

        return new SmartLed_(type, count, pin);
    }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        proto.defineProperty("begin", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisValue, jac::ObjectWeak options) {
            SmartLed_& self = *SmartLedProtoBuilder::getOpaque(ctx, thisValue);
            int x = options.get<int>("x");
            int y = options.get<int>("y");
            int width = options.get<int>("width");
            int height = options.get<int>("height");
            self.begin(x, y, width, height);
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("send", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisValue, jac::ArrayBufferWeak data) {
            SmartLed_& self = *SmartLedProtoBuilder::getOpaque(ctx, thisValue);
            self.send(data.typedView<Rgb>());
        }), jac::PropFlags::Enumerable);

        SmartLedProtoBuilder::template addMethodMember<void(SmartLed_::*)(), &SmartLed_::close>(ctx, proto, "close", jac::PropFlags::Enumerable);
    }
};


template<class Next>
class SmartLedFeature : public Next {
    static inline std::set<int> _usedRmtChannels;
public:
    using SmartLedClass = jac::Class<SmartLedProtoBuilder<SmartLedFeature>>;

    SmartLedFeature() {
        SmartLedClass::init("SmartLed");
    }

    void initialize() {
        Next::initialize();

        jac::Module& mod = this->newModule("smartled");
        jac::Function ctor = SmartLedClass::getConstructor(this->context());
        mod.addExport("SmartLed", ctor);
        mod.addExport("LED_WS2812", jac::Value::from(this->context(), LED_WS2812));
        mod.addExport("LED_WS2812B", jac::Value::from(this->context(), LED_WS2812B));
        mod.addExport("LED_WS2812B_2020", jac::Value::from(this->context(), LedType{ 400, 800, 850, 450, 300000 }));
        mod.addExport("LED_WS2813", jac::Value::from(this->context(), LED_WS2813));
        mod.addExport("LED_SK6812", jac::Value::from(this->context(), LED_SK6812));
    }

    friend SmartLedWrapper<SmartLedFeature>;
};
