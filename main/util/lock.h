#pragma once

#include <atomic>
#include <optional>
#include "timeout.h"


class TimeoutLock {
    Timeout _timeout;
    bool _locked = false;
    int _owner = 0;
    std::mutex _mutex;
    int _stops = 0;
public:
    TimeoutLock(std::chrono::milliseconds duration) : _timeout(duration) {}

    TimeoutLock(const TimeoutLock&) = delete;
    TimeoutLock(TimeoutLock&&) = delete;
    TimeoutLock& operator=(const TimeoutLock&) = delete;
    TimeoutLock& operator=(TimeoutLock&&) = delete;

    bool lock(int who, std::function<void()> callback) {
        std::lock_guard<std::mutex> _(_mutex);

        if (_locked && _owner == who) {
            throw std::runtime_error("Lock already locked, reset() should be used to reset the timeout");
        }

        if (!_locked) {
            _locked = true;
            _owner = who;
            _timeout.start([this, callback]() {
                std::lock_guard<std::mutex> _(_mutex);
                if (callback) {
                    callback();
                }
                _locked = false;
            });

            return true;
        }
        return false;
    }

    void resetTimeout(int who) {
        std::lock_guard<std::mutex> _(_mutex);

        if (!_locked || _owner != who) {
            return;
        }

        _stops = _stops > 0 ? _stops - 1 : 0;

        if (_stops == 0) {
            _timeout.reset();
        }
    }

    void stopTimeout(int who) {
        std::lock_guard<std::mutex> _(_mutex);

        if (!_locked || _owner != who) {
            return;
        }

        _timeout.stop();
        _stops++;
    }

    bool unlock(int who) {
        std::lock_guard<std::mutex> _(_mutex);

        if (_owner != who) {
            return false;
        }

        _timeout.stop();
        _locked = false;
        return true;
    }

    void forceUnlock() {
        std::lock_guard<std::mutex> _(_mutex);

        _timeout.stop();
        _locked = false;
    }

    bool ownedBy(int who) {
        std::lock_guard<std::mutex> _(_mutex);
        return _locked && _owner == who;
    }

    ~TimeoutLock() {
        _timeout.stop();
    }
};
