#pragma once

#include <jac/machine/machine.h>
#include <deque>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


template<class Next>
class WdtResetFeature : public Next {
public:
    void onEventLoop() {
        Next::onEventLoop();
        vTaskDelay(1);
    };
};
