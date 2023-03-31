#pragma once

#include <jac/machine/machine.h>
#include <jac/link/routerCommunicator.h>
#include <jac/machine/functionFactory.h>

#include <vector>
#include <thread>


template<class Next>
class LinkIoFeature : public Next {
public:
    class LinkWritable : public Next::Writable {
        OutputStreamCommunicator* comm;
    public:
        LinkWritable(OutputStreamCommunicator* comm): comm(std::move(comm)) {}

        void write(std::span<const uint8_t> buffer) override {
            comm->write(buffer);
        }
    };

    class LinkReadable : public Next::Readable {
        LinkIoFeature* _machine;
        BufferedInputStreamCommunicator* comm;
        std::thread _thread;
        bool _running = false;
    public:
        LinkReadable(LinkIoFeature* machine, BufferedInputStreamCommunicator* comm): _machine(machine), comm(std::move(comm)) {}
        LinkReadable(LinkReadable&) = delete;
        LinkReadable(LinkReadable&&) = delete;

        bool read(std::function<void(std::span<const uint8_t>)> callback) override {
            if (_running) {
                return false;
            }
            if (_thread.joinable()) {
                _thread.join();
            }

            _running = true;

            _thread = std::thread([this, callback = std::move(callback)]() mutable {
                std::vector<char> buffer;
                buffer.resize(256);

                auto count = comm->read(std::span<uint8_t>(reinterpret_cast<uint8_t*>(buffer.data()), buffer.size()));

                buffer.resize(count);
                _machine->scheduleEvent([callback = std::move(callback), buffer = std::move(buffer)]() mutable {
                    callback(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(buffer.data()), buffer.size()));
                });

                _running = false;
            });

            return true;
        }

        ~LinkReadable() override {
            if (_running) {
                comm->cancelRead();
            }
            if (_thread.joinable()) {
                _thread.join();
            }
        }
    };
};
