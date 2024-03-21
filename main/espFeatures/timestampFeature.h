#pragma once

#include <jac/machine/class.h>
#include <jac/machine/functionFactory.h>
#include <jac/machine/machine.h>

#include <chrono>


class Timestamp {
    std::chrono::time_point<std::chrono::steady_clock> point;
public:
    Timestamp(std::chrono::time_point<std::chrono::steady_clock> point) : point(point) {}

    int32_t millis() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(point.time_since_epoch()).count();
    }

    int32_t micros() {
        return std::chrono::duration_cast<std::chrono::microseconds>(point.time_since_epoch()).count() % 1000;
    }
};


struct TimestampProtoBuilder : public jac::ProtoBuilder::Opaque<Timestamp>, public jac::ProtoBuilder::Properties {
    static void addProperties(jac::ContextRef ctx, jac::Object proto) {
        TimestampProtoBuilder::template addMethodMember<int32_t(Timestamp::*)(), &Timestamp::millis>(ctx, proto, "millis");
        TimestampProtoBuilder::template addMethodMember<int32_t(Timestamp::*)(), &Timestamp::micros>(ctx, proto, "micros");
    }
};


template<class Next>
class TimestampFeature : public Next {
public:
    using TimestampClass = jac::Class<TimestampProtoBuilder>;

    TimestampFeature() {
        TimestampClass::init("Timestamp");
    }

    void initialize() {
        Next::initialize();
    }

    jac::Object createTimestamp(std::chrono::time_point<std::chrono::steady_clock> point) {
        return TimestampClass::createInstance(this->context(), new Timestamp(point));
    }
};
