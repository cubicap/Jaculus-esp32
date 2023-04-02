#pragma once

#include <functional>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include "logger.h"

class Timeout {
    std::function<void()> _callback;
    std::chrono::milliseconds _duration;
    std::chrono::steady_clock::time_point _endTime = std::chrono::steady_clock::time_point::max();
    bool _stop = false;
    bool _running = false;
    std::mutex _mutex;
    std::condition_variable _cv;
    std::thread _thread;
public:
    Timeout(std::chrono::milliseconds duration):
        _duration(duration)
    {
        _thread = std::thread([this]() {
            while (!_stop) {
                std::unique_lock<std::mutex> lock(_mutex);
                if (!_running) {
                    _cv.wait(lock);
                    continue;
                }
                if (std::chrono::steady_clock::now() >= _endTime) {
                    _running = false;
                    if (_callback) {
                        _callback();
                    }
                }
                _cv.wait_until(lock, _endTime);
            }
        });
    }

    Timeout(const Timeout&) = delete;
    Timeout(Timeout&&) = delete;
    Timeout& operator=(const Timeout&) = delete;
    Timeout& operator=(Timeout&&) = delete;

    void start(std::function<void()> callback) {
        std::lock_guard<std::mutex> lock(_mutex);
        _endTime = std::chrono::steady_clock::now() + _duration;
        _callback = callback;
        _running = true;
        _cv.notify_all();
    }

    void reset() {
        std::lock_guard<std::mutex> lock(_mutex);
        _endTime = std::chrono::steady_clock::now() + _duration;
        _running = true;
    }

    void cancel() {
        std::lock_guard<std::mutex> lock(_mutex);
        _running = false;
        _cv.notify_all();
    }

    ~Timeout() {
        std::lock_guard<std::mutex> lock(_mutex);
        _stop = true;
        _running = false;
        _cv.notify_all();
        _thread.join();
    }
};
