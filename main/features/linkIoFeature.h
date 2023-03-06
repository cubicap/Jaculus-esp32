#pragma once

#include <memory>

#include <jac/machine/machine.h>
#include <noal_func.h>
#include <jac/link/communicator.h>
#include <jac/machine/functionFactory.h>

template<class Next>
class LinkIoFeature : public Next {
public:
    class LinkIo {
    public:
        std::unique_ptr<OutputStreamCommunicator> _output;
        void print(std::string str) {
            _output->write(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(str.data()), str.size()));
        }
    };
    LinkIo linkIo;

    void initialize() {
        Next::initialize();

        jac::FunctionFactory ff(this->_context);

        auto& module = this->newModule("stdio");
        module.addExport("print", ff.newFunction(noal::function(&LinkIo::print, &linkIo)));
    }
};
