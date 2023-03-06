#pragma once

#include <jac/machine/machine.h>
#include <noal_func.h>
#include <memory>
#include <map>
#include <SmartLeds.h>
#include <jac/machine/functionFactory.h>
#include <jac/machine/class.h>


template<>
struct jac::ConvTraits<Rgb> {
    static Value to(ContextRef ctx, Rgb val) {
        auto obj = Object::create(ctx);
        obj.set<int>("r", val.r);
        obj.set<int>("g", val.g);
        obj.set<int>("b", val.b);
        return obj;
    }

    static Rgb from(ContextRef ctx, ValueConst val) {
        auto obj = val.to<Object>();
        return Rgb(obj.get("r").to<int>(), obj.get("g").to<int>(), obj.get("b").to<int>());
    }
};


template<class Next>
class NeopixelFeature : public Next {
    struct NeopixelProtoBuilder : public jac::ProtoBuilder::Opaque<SmartLed>, public jac::ProtoBuilder::Properties {
        static SmartLed* constructOpaque(JSContext* ctx, std::vector<jac::ValueConst> args) {
            if (args.size() != 2) {
                throw std::runtime_error("Invalid number of arguments");
            }
            int pin = args[0].to<int>();
            int count = args[1].to<int>();

            return new SmartLed(LED_WS2812, count, pin, 0, SingleBuffer);
        }

        static void addProperties(JSContext* ctx, jac::Object proto) {
            jac::FunctionFactory ff(ctx);

            addProp(ctx, proto, "show", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueConst thisValue) {
                SmartLed& led = *getOpaque(ctx, thisValue);
                led.wait();
                led.show();
            }));

            addProp(ctx, proto, "set", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueConst thisValue, int idx, Rgb color) {
                SmartLed& strip = *getOpaque(ctx, thisValue);
                strip[idx] = color;
            }));

            addProp(ctx, proto, "get", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueConst thisValue, int idx) {
                SmartLed& strip = *getOpaque(ctx, thisValue);
                return strip[idx];
            }));
        }
    };
public:
    using NeopixelClass = jac::Class<NeopixelProtoBuilder>;

    NeopixelFeature() {
        NeopixelClass::init("Neopixel");
    }

    void initialize() {
        Next::initialize();

        auto& mod = this->newModule("neopixel");
        jac::Function ctor = NeopixelClass::getConstructor(this->_context);
        mod.addExport("Neopixel", ctor);
    }
};
