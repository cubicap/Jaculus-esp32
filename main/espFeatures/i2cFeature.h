#pragma once

#include <jac/machine/machine.h>
#include <jac/machine/functionFactory.h>

#include <memory>

#include "driver/i2c.h"


template<class Feature>
class I2C {
    i2c_port_t _port;
    uint8_t _address;
    bool open = true;  // the port is open upon successful construction
public:
    I2C(int sda, int scl, int bitrate, int address, int port):
        _port(static_cast<i2c_port_t>(port)),
        _address(static_cast<uint8_t>(address & 0x7F))  // 7-bit address
    {
        if (_address != address) {
            throw std::runtime_error("Invalid I2C address");
        }

        i2c_config_t conf = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = Feature::getDigitalPin(sda),
            .scl_io_num = Feature::getDigitalPin(scl),
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master = {
                .clk_speed = static_cast<uint32_t>(bitrate),
            },
            .clk_flags = 0,
        };

        esp_err_t err = i2c_param_config(_port, &conf);
        if (err != ESP_OK) {
            throw std::runtime_error(std::string("Error configuring I2C: ") + esp_err_to_name(err));
        }

        err = i2c_driver_install(_port, conf.mode, 0, 0, 0);
        if (err != ESP_OK) {
            throw std::runtime_error(std::string("Error installing I2C driver: ") + esp_err_to_name(err));
        }
    }

    size_t readInto(std::span<uint8_t> data, bool stopBit = true) {
        if (!open) {
            throw std::runtime_error("I2C is closed");
        }

        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (_address << 1) | I2C_MASTER_READ, true);
        i2c_master_read(cmd, data.data(), data.size(), stopBit ? I2C_MASTER_LAST_NACK : I2C_MASTER_ACK);
        i2c_master_stop(cmd);

        esp_err_t err = i2c_master_cmd_begin(_port, cmd, 100 / portTICK_PERIOD_MS);
        if (err != ESP_OK) {
            throw std::runtime_error(std::string("Error reading from I2C: ") + esp_err_to_name(err));
        }

        i2c_cmd_link_delete(cmd);

        return data.size();
    }

    std::vector<uint8_t> read(size_t quantity, bool stopBit = true) {
        std::vector<uint8_t> data(quantity);
        readInto(data, stopBit);
        return data;
    }

    void write(std::span<const uint8_t> data, bool stopBit = true) {
        if (!open) {
            throw std::runtime_error("I2C is closed");
        }

        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (_address << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write(cmd, data.data(), data.size(), stopBit);
        i2c_master_stop(cmd);

        esp_err_t err = i2c_master_cmd_begin(_port, cmd, 100 / portTICK_PERIOD_MS);
        if (err != ESP_OK) {
            throw std::runtime_error(std::string("Error writing to I2C: ") + esp_err_to_name(err));
        }

        i2c_cmd_link_delete(cmd);
    }

    void close() {
        if (!open) {
            return;
        }

        esp_err_t err = i2c_driver_delete(_port);
        if (err != ESP_OK) {
            throw std::runtime_error(std::string("Error deleting I2C driver: ") + esp_err_to_name(err));
        }

        open = false;
    }

    ~I2C() {
        close();
    }
};

template<class Feature>
struct I2CProtoBuilder : public jac::ProtoBuilder::Opaque<I2C<Feature>>, public jac::ProtoBuilder::Properties, public jac::ProtoBuilder::LifetimeHandles {
    using I2C_ = I2C<Feature>;

    static I2C_* constructOpaque(jac::ContextRef ctx, std::vector<jac::ValueWeak> args) {
        // TODO: check if pins are already in use

        if (args.size() < 1) {
            throw jac::Exception::create(jac::Exception::Type::TypeError, "Analog constructor requires an options argument");
        }

        jac::ObjectWeak options = args[0].to<jac::ObjectWeak>();
        if (!options.hasProperty("sda")) {
            throw jac::Exception::create(jac::Exception::Type::TypeError, "I2C constructor requires an sda pin");
        }
        int sda = options.get<int>("sda");

        if (!options.hasProperty("scl")) {
            throw jac::Exception::create(jac::Exception::Type::TypeError, "I2C constructor requires an scl pin");
        }
        int scl = options.get<int>("scl");

        if (!options.hasProperty("bitrate")) {
            throw jac::Exception::create(jac::Exception::Type::TypeError, "I2C constructor requires a bitrate");
        }
        int bitrate = options.get<int>("bitrate");

        if (!options.hasProperty("address")) {
            throw jac::Exception::create(jac::Exception::Type::TypeError, "I2C constructor requires an address");
        }
        int address = options.get<int>("address");

        int port = I2C_NUM_0;
        if (options.hasProperty("port")) {
            port = options.get<int>("port");
        }

        return new I2C_(sda, scl, bitrate, address, port);
    }

    static void postConstruction(jac::ContextRef ctx, jac::Object thisVal, std::vector<jac::ValueWeak> args) {
        auto& machine = *reinterpret_cast<Feature*>(JS_GetContextOpaque(ctx));
        machine.extendLifetime(thisVal);
    }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        // XXX: ugly hack with inline javascript
        proto.defineProperty("read", ff.newFunctionThisVariadic([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::vector<jac::ValueWeak> args) {
            auto& i2c = *I2CProtoBuilder::getOpaque(ctx, thisVal);
            auto& machine = *reinterpret_cast<Feature*>(JS_GetContextOpaque(ctx));
            if (args.size() < 1) {
                throw jac::Exception::create(jac::Exception::Type::TypeError, "Read requires at least one argument");
            }

            jac::Value checkBufferType = machine.eval(
R"--(
(data) => {
    if (data instanceof ArrayBuffer) return true;
    if (typeof data === 'number') return false;
    throw new Error('Invalid data type');
}
)--", "<I2CFeature::read>");

            bool isBuffer = checkBufferType.to<jac::Function>().call<bool>(args[0]);

            bool stopBit = true;
            if (args.size() > 1) {
                stopBit = args[1].to<bool>();
            }

            if (isBuffer) {
                auto data = args[0].to<jac::ArrayBuffer>();
                auto dataView = data.typedView<uint8_t>();
                return jac::Value::from(ctx, static_cast<int>(i2c.readInto(dataView, stopBit)));
            }
            else {
                int quantity = args[0].to<int>();
                auto data = i2c.read(quantity, stopBit);
                return jac::ArrayBuffer::create(ctx, std::span(data.data(), data.size())).template to<jac::Value>();
            }
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("write", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::vector<jac::ValueWeak> args) {
            auto& i2c = *I2CProtoBuilder::getOpaque(ctx, thisVal);
            if (args.size() < 1) {
                throw jac::Exception::create(jac::Exception::Type::TypeError, "Write requires at least one argument");
            }

            jac::ArrayBufferWeak buffer = args[0].to<jac::ArrayBufferWeak>();
            bool stopBit = true;
            if (args.size() > 1) {
                stopBit = args[1].to<bool>();
            }

            auto dataView = buffer.typedView<uint8_t>();
            i2c.write(dataView, stopBit);
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("close", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            I2C_& self = *I2CProtoBuilder::getOpaque(ctx, thisVal);
            self.close();
            auto& machine = *reinterpret_cast<Feature*>(JS_GetContextOpaque(ctx));
            machine.releaseLifetime(thisVal);
        }), jac::PropFlags::Enumerable);
    }
};


template<class Next>
class I2CFeature : public Next {
public:
    using I2CClass = jac::Class<I2CProtoBuilder<I2CFeature>>;

    I2CFeature() {
        I2CClass::init("I2C");
    }

    void initialize() {
        Next::initialize();

        jac::Module& mod = this->newModule("embedded:io/i2c");

        I2CClass::initContext(this->context());

        jac::Object i2cConstructor = I2CClass::getConstructor(this->context());

        mod.addExport("I2C", i2cConstructor);
    }
};
