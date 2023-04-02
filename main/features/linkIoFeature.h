#pragma once

#include <jac/machine/machine.h>
#include <jac/link/routerCommunicator.h>
#include <jac/machine/functionFactory.h>

#include <atomic>
#include <vector>
#include <thread>


template<class Next>
class LinkIoFeature : public Next {
public:
    class LinkWritable : public Next::Writable {
        OutputStreamCommunicator* comm;
    public:
        LinkWritable(OutputStreamCommunicator* comm): comm(std::move(comm)) {}

        void write(std::string data) override {
            comm->write(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(data.data()), data.size()));
        }
    };

    class LinkReadable : public Next::Readable {
        LinkIoFeature* _machine;
        BufferedInputStreamCommunicator* comm;
        std::thread _thread;
        std::atomic<bool> _running = false;

        bool startRead(auto callback) {
            if (_running) {
                return false;
            }
            if (_thread.joinable()) {
                _thread.join();
            }
            _running = true;

            _thread = std::thread(std::move(callback));

            return true;
        }
    public:
        LinkReadable(LinkIoFeature* machine, BufferedInputStreamCommunicator* comm): _machine(machine), comm(std::move(comm)) {}
        LinkReadable(LinkReadable&) = delete;
        LinkReadable(LinkReadable&&) = delete;

        bool get(std::function<void(char)> callback) override {
            return startRead([this, callback = std::move(callback)]() mutable {
                int res = comm->get();

                if (res != 0) {
                    _machine->scheduleEvent([callback = std::move(callback), res]() mutable {
                        callback(static_cast<char>(res));
                    });
                }

                _running = false;
            });
        }

        bool read(std::function<void(std::string)> callback) override {
            return startRead([this, callback = std::move(callback)]() mutable {
                std::string buffer;
                buffer.resize(256);

                auto count = comm->read(std::span<uint8_t>(reinterpret_cast<uint8_t*>(buffer.data()), buffer.size()));

                buffer.resize(count);
                _machine->scheduleEvent([callback = std::move(callback), buffer = std::move(buffer)]() mutable {
                    callback(std::move(buffer));
                });

                _running = false;
            });
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
