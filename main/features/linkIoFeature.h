#pragma once

#include <memory>

#include <jac/machine/machine.h>
#include <noal_func.h>
#include <jac/link/communicator.h>
#include <jac/machine/functionFactory.h>

#include <vector>
#include <memory>


template<class Next>
class LinkIoFeature : public Next {
private:
    class Stdio {
    public:
        std::unique_ptr<typename Next::Writable> out;
        std::unique_ptr<typename Next::Writable> err;
        std::unique_ptr<typename Next::Readable> in;
    };
public:
    class LinkWritable : public Next::Writable {
        std::unique_ptr<OutputStreamCommunicator> comm;
    public:
        LinkWritable(std::unique_ptr<OutputStreamCommunicator> comm): comm(std::move(comm)) {}

        void print(std::string str) override {
            comm->write(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(str.data()), str.size()));
        }
    };

    class LinkReadable : public Next::Readable {
        std::unique_ptr<BufferedInputStreamCommunicator> comm;
    public:
        LinkReadable(std::unique_ptr<BufferedInputStreamCommunicator> comm): comm(std::move(comm)) {}
        std::string readLine() override {
            std::string line;
            while (true) {
                auto c = comm->get();
                if (c == '\n') {
                    break;
                }
                line += c;
            }
            return line;
        }
    };

    Stdio stdio;

    void initialize() {
        Next::initialize();

        // assert(stdio.out && "not set");
        // assert(stdio.err && "not set");
        // assert(stdio.in && "not set");

        jac::FunctionFactory ff(this->_context);

        this->registerGlobal("print", ff.newFunctionVariadic([this](std::vector<jac::ValueWeak> args) {
            this->stdio.out->println(args);
        }));

        auto& module = this->newModule("stdio");
        module.addExport("stdout", Next::WritableClass::createInstance(this->_context, new Next::WritableRef(stdio.out.get())));
        module.addExport("stderr", Next::WritableClass::createInstance(this->_context, new Next::WritableRef(stdio.err.get())));
        module.addExport("stdin", Next::ReadableClass::createInstance(this->_context, new Next::ReadableRef(stdio.in.get())));

    }
};
