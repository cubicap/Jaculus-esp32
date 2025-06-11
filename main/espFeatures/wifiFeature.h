#pragma once

#include <jac/machine/class.h>
#include <jac/machine/functionFactory.h>
#include <jac/machine/machine.h>

#include "../platform/espWifi.h"

template<class Next>
class WifiFeature : public Next {
public:

    void initialize() {
        Next::initialize();

        jac::FunctionFactory ff(this->context());
        auto& module = this->newModule("wifi");

        module.addExport("currentIp", ff.newFunction(noal::function([this]() {
            auto& wifi = EspWifiController::get();
            if(wifi.currentIp().addr == 0) {
                return jac::Value::null(this->context());
            } else {
                return jac::Value::from(this->context(), wifi.currentIpStr());
            }
        })));

        module.addExport("waitForIp", ff.newFunctionVariadic([this](std::vector<jac::ValueWeak> args) {
            if (args.size() > 1) {
                throw std::runtime_error("Invalid number of arguments for waitForIp method");
            }

            uint32_t timeoutMs = 0; // Default: wait forever
            if (args.size() == 1) {
                timeoutMs = args[0].to<uint32_t>();
            }

            auto& wifi = EspWifiController::get();
            return jac::Value::from(this->context(), wifi.waitForIp(timeoutMs));
        }));
    }
};
