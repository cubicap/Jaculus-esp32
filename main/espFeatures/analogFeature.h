#pragma once

#include <jac/machine/machine.h>
#include <jac/machine/functionFactory.h>

#include <stdexcept>
#include <set>

#include "driver/adc.h"


template<class Feature>
class Analog {
    Feature* const _feature;
    const int _adcNum;
    const adc1_channel_t _channel;
    const adc_bits_width_t _resolution;

    Analog(Feature* feature, std::pair<int, int> pinConf, adc_bits_width_t resolution):
        _feature(feature),
        _adcNum(pinConf.first),
        _channel(static_cast<adc1_channel_t>(pinConf.second)),
        _resolution(resolution)
    {
        if (_adcNum != 1) {
            throw std::runtime_error("Only ADC1 is supported");
        }

        esp_err_t err = adc1_config_width(_resolution);
        if (err != ESP_OK) {
            throw std::runtime_error(std::string("Error configuring ADC width: ") + esp_err_to_name(err));
        }

        // TODO: make attenuation configurable
        err = adc1_config_channel_atten(_channel, ADC_ATTEN_DB_11);
        if (err != ESP_OK) {
            throw std::runtime_error(std::string("Error configuring ADC attenuation: ") + esp_err_to_name(err));
        }
    }
public:
    Analog(Feature* feature, int pin, int resolution = ADC_WIDTH_BIT_DEFAULT):
        Analog(feature, Feature::getAnalogPin(pin), static_cast<adc_bits_width_t>(resolution))
    {}

    ~Analog() {
        close();
    }

    int read() {
        return adc1_get_raw(_channel) >> (ADC_WIDTH_BIT_DEFAULT - _resolution);
    }

    void close() {
        // do nothing for now
        // possibly reconfigure to some default state?
    }
};

template<class Feature>
struct AnalogProtoBuilder : public jac::ProtoBuilder::Opaque<Analog<Feature>>, public jac::ProtoBuilder::Properties {
    using Analog_ = Analog<Feature>;

    static Analog_* constructOpaque(jac::ContextRef ctx, std::vector<jac::ValueWeak> args) {
        // TODO: extend instance lifetime until close or program end
        //       *currently not a problem, as the instance destrutor does nothing*
        // TODO: check if pin is already in use

        if (args.size() < 1) {
            throw jac::Exception::create(jac::Exception::Type::TypeError, "Analog constructor requires an options argument");
        }

        jac::ObjectWeak options = args[0].to<jac::ObjectWeak>();

        int pin = options.get<int>("pin");
        if (options.hasProperty("resolution")) {
            int resolution = options.get<int>("resolution");
            return new Analog_(static_cast<Feature*>(JS_GetContextOpaque(ctx)), pin, resolution);
        } else {
            return new Analog_(static_cast<Feature*>(JS_GetContextOpaque(ctx)), pin);
        }
    }

    static void addProperties(jac::ContextRef ctx, jac::Object proto) {
        AnalogProtoBuilder::template addMethodMember<int(Analog_::*)(), &Analog_::read>(ctx, proto, "read", jac::PropFlags::Enumerable);
        AnalogProtoBuilder::template addMethodMember<void(Analog_::*)(), &Analog_::close>(ctx, proto, "close", jac::PropFlags::Enumerable);
    }
};

template<class Next>
class AnalogFeature : public Next {
public:
    using AnalogClass = jac::Class<AnalogProtoBuilder<AnalogFeature>>;

    void initialize() {
        Next::initialize();

        auto& module = this->newModule("embedded:io/analog");

        AnalogClass::init("Analog");
        AnalogClass::initContext(this->context());

        module.addExport("Analog", AnalogClass::getConstructor(this->context()));
    }
};
