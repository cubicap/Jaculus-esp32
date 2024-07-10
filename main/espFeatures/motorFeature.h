#pragma once

#include <jac/machine/class.h>
#include <jac/machine/machine.h>
#include <jac/machine/values.h>

#include <dcmotor.h>

#include <vector>
#include <optional>


template<typename Feature>
struct MotorPins {
    gpio_num_t motA;
    gpio_num_t motB;
    gpio_num_t encA;
    gpio_num_t encB;
};

template<typename Feature>
struct LedcConfig {
    ledc_timer_t timer;
    ledc_channel_t channelA;
    ledc_channel_t channelB;
};

template<typename Feature>
struct jac::ConvTraits<MotorPins<Feature>> {
    static Value to(ContextRef ctx, MotorPins<Feature> conf) {
        auto obj = jac::Object::create(ctx);
        obj.set("motA", jac::Value::from(ctx, static_cast<int>(conf.motA)));
        obj.set("motB", jac::Value::from(ctx, static_cast<int>(conf.motB)));
        obj.set("encA", jac::Value::from(ctx, static_cast<int>(conf.encA)));
        obj.set("encB", jac::Value::from(ctx, static_cast<int>(conf.encB)));
        return obj;
    }

    static MotorPins<Feature> from(ContextRef ctx, ValueWeak val) {
        auto obj = val.to<jac::ObjectWeak>();
        MotorPins<Feature> conf;
        conf.motA = Feature::getDigitalPin(obj.get("motA").to<int>());
        conf.motB = Feature::getDigitalPin(obj.get("motB").to<int>());
        conf.encA = Feature::getDigitalPin(obj.get("encA").to<int>());
        conf.encB = Feature::getDigitalPin(obj.get("encB").to<int>());
        return conf;
    }
};

template<typename Feature>
struct jac::ConvTraits<LedcConfig<Feature>> {
    static Value to(ContextRef ctx, LedcConfig<Feature> conf) {
        auto obj = jac::Object::create(ctx);
        obj.set("timer", jac::Value::from(ctx, static_cast<int>(conf.timer)));
        obj.set("channelA", jac::Value::from(ctx, static_cast<int>(conf.channelA)));
        obj.set("channelB", jac::Value::from(ctx, static_cast<int>(conf.channelB)));
        return obj;
    }

    static LedcConfig<Feature> from(ContextRef ctx, ValueWeak val) {
        auto obj = val.to<jac::ObjectWeak>();
        LedcConfig<Feature> conf;
        conf.timer = static_cast<ledc_timer_t>(obj.get("timer").to<int>());
        conf.channelA = static_cast<ledc_channel_t>(obj.get("channelA").to<int>());
        conf.channelB = static_cast<ledc_channel_t>(obj.get("channelB").to<int>());

        if (conf.timer >= LEDC_TIMER_MAX) {
            throw std::runtime_error("Invalid timer");
        }
        if (conf.channelA >= LEDC_CHANNEL_MAX || conf.channelB >= LEDC_CHANNEL_MAX) {
            throw std::runtime_error("Invalid channel");
        }

        return conf;
    }
};


struct PromiseFunctions {
    jac::Function resolve;
    jac::Function reject;
};


struct JSDCMotor {
    std::optional<DCMotor> motor;
    std::optional<PromiseFunctions> pendingPromise;
    int encTicks;
    double circumference;

    template<typename Feature>
    JSDCMotor(MotorPins<Feature> pins, int regP, LedcConfig<Feature> ledcConf, int encTicks, double circumference):
        motor(std::in_place, pins.motA, pins.motB, pins.encA, pins.encB, regP, ledcConf.timer, ledcConf.channelA, ledcConf.channelB),
        encTicks(encTicks), circumference(circumference)
    {
        motor->startTicker();
    }

    ~JSDCMotor() {
        clear();
    }

    void clear() {
        if (motor) {
            motor->stopTicker();
            motor.reset();
        }
        pendingPromise.reset();
    }
};


template<typename Feature>
class MotorProtoBuilder : public jac::ProtoBuilder::Opaque<JSDCMotor>, public jac::ProtoBuilder::Properties, jac::ProtoBuilder::LifetimeHandles {
public:
    static JSDCMotor* constructOpaque(jac::ContextRef ctx, std::vector<jac::ValueWeak> args) {
        if (args.size() != 1) {
            throw std::runtime_error("Expected 2 arguments");
        }

        auto options = args[0].to<jac::Object>();
        auto pins = options.get("pins").to<MotorPins<Feature>>();
        auto encTicks = options.get("encTicks").to<int>();
        auto circumference = options.get("circumference").to<double>();

        auto ledcConf = options.get("ledc").to<LedcConfig<Feature>>();

        auto machine = reinterpret_cast<Feature*>(JS_GetContextOpaque(ctx));
        std::unique_ptr<JSDCMotor> mot = std::make_unique<JSDCMotor>(pins, 50, ledcConf, encTicks, circumference);
        mot->motor->onTarget([mot = mot.get(), machine]() {
            static constexpr auto resolve = +[](void* data) {
                JSDCMotor* mot = reinterpret_cast<JSDCMotor*>(data);
                if (mot->pendingPromise) {
                    mot->pendingPromise->resolve.call<void>();
                    mot->pendingPromise.reset();
                }
            };

            if (mot->pendingPromise) {
                machine->scheduleEventISR(resolve, mot); // FIXME: potential out-of-memory access
            }
        });
        mot->motor->setEndPosTolerance(3);


        return mot.release();
    }

    static void addProperties(jac::ContextRef ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        proto.defineProperty("setSpeed", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, double speed) {
            auto& motor = *getOpaque(ctx, thisVal);
            if (!motor.motor) {
                throw jac::Exception::create(jac::Exception::Type::InternalError, "Motor is closed");
            }

            int ticksPerSec = motor.encTicks * speed / (motor.circumference);
            motor.motor->setSpeed(ticksPerSec);
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("move", ff.newFunctionThisVariadic([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::vector<jac::ValueWeak> args) {
            auto& motor = *getOpaque(ctx, thisVal);
            if (!motor.motor) {
                throw jac::Exception::create(jac::Exception::Type::InternalError, "Motor is closed");
            }

            if (motor.pendingPromise) {
                throw jac::Exception::create(jac::Exception::Type::InternalError, "Motor is already moving");
            }

            std::optional<jac::Promise> pms;

            if (args.size() == 0) {
                motor.motor->moveInfinite();

                auto [ promise, resolve, reject ] = jac::Promise::create(ctx);
                motor.pendingPromise = PromiseFunctions{resolve, reject};
                pms = promise;
            }
            else {
                jac::Object duration = args[0].to<jac::Object>();

                bool hasDistance = duration.hasProperty("distance");
                bool hasTime = duration.hasProperty("time");

                if (hasDistance && hasTime) {
                    throw jac::Exception::create(jac::Exception::Type::TypeError, "Cannot specify both distance and time");
                }

                auto [ promise, resolve, reject ] = jac::Promise::create(ctx);
                motor.pendingPromise = PromiseFunctions{resolve, reject};
                pms = promise;

                if (hasDistance) {
                    double distance = duration.get("distance").to<double>();
                    int64_t ticks = motor.encTicks * distance / (motor.circumference);
                    motor.motor->moveDistance(ticks);
                }
                else if (hasTime) {
                    int64_t time = duration.get("time").to<int64_t>();
                    motor.motor->moveTime(time);
                }
                else {
                    motor.motor->moveInfinite();
                }
            }

            return pms.value();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("stop", ff.newFunctionThisVariadic([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::vector<jac::ValueWeak> args) {
            auto& motor = *getOpaque(ctx, thisVal);
            if (!motor.motor) {
                throw jac::Exception::create(jac::Exception::Type::InternalError, "Motor is closed");
            }

            if (args.size() == 0) {
                motor.motor->stop(false);
            }
            else {
                motor.motor->stop(args[0].to<bool>());
            }

            auto [ promise, resolve, reject ] = jac::Promise::create(ctx);
            resolve.call<void>();
            return promise;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("getPosition", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& motor = *getOpaque(ctx, thisVal);
            if (!motor.motor) {
                throw jac::Exception::create(jac::Exception::Type::InternalError, "Motor is closed");
            }

            int64_t ticks = motor.motor->getPosition();
            return jac::Value::from(ctx, static_cast<double>(ticks) * motor.circumference / motor.encTicks);
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("close", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& motor = *getOpaque(ctx, thisVal);
            if (!motor.motor) {
                throw jac::Exception::create(jac::Exception::Type::InternalError, "Motor is closed");
            }

            motor.clear();

            auto machine = reinterpret_cast<Feature*>(JS_GetContextOpaque(ctx));
            machine->unregisterMotor(thisVal);
        }), jac::PropFlags::Enumerable);
    }

    static void postConstruction(jac::ContextRef ctx, jac::Object thisVal, std::vector<jac::ValueWeak> args) {
        auto machine = reinterpret_cast<Feature*>(JS_GetContextOpaque(ctx));
        machine->registerMotor(thisVal);
    }
};


struct NonexistentMotorProtoBuilder {
    static void postConstruction(jac::ContextRef ctx, jac::Object thisVal, std::vector<jac::ValueWeak> args) {
        throw jac::Exception::create(jac::Exception::Type::InternalError, "Motor driver is not available on this device");
    }
};


template<typename Next>
class MotorFeature : public Next {
    std::vector<jac::Object> _motors;
public:

#if defined(CONFIG_IDF_TARGET_ESP32C3)
    using MotorClass = jac::Class<NonexistentMotorProtoBuilder>;
#else
    using MotorClass = jac::Class<MotorProtoBuilder<MotorFeature<Next>>>;
#endif


    MotorFeature() {
        MotorClass::init("Motor");
    }

#if !defined(CONFIG_IDF_TARGET_ESP32C3)
    ~MotorFeature() {
        for(auto& motor : _motors) {
            auto& jsdcMotor = *MotorProtoBuilder<MotorFeature<Next>>::getOpaque(this->context(), motor);
            jsdcMotor.clear();
        }
    }

    void registerMotor(jac::Object motor) {
        _motors.emplace_back(motor);
    }

    void unregisterMotor(jac::ValueWeak motor) {
        for(auto itr = _motors.begin(); itr != _motors.end();) {
            if(itr->getVal() == motor.getVal()) {
                _motors.erase(itr);
                return;
            }
            ++itr;
        }
    }
#endif

    void initialize() {
        Next::initialize();

        auto& mod = this->newModule("motor");

        jac::Object motorCtor = MotorClass::getConstructor(this->context());

        mod.addExport("Motor", motorCtor);
    }
};
