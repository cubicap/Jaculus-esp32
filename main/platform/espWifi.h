#pragma once

#include <mutex>
#include <jac/device/logger.h>
#include "esp_netif.h"
#include "esp_timer.h"
#include "esp_wifi.h"

#include "espNvsKeyValue.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

class EspWifiController {
private:
    enum Mode : uint8_t {
        DISABLED,
        STATION,
        AP,

        MAX,
    };

    enum StaMode : uint8_t  {
        // Connect to any known network, pick the one with better signal if multiple found
        BEST_SIGNAL,
        // Connect to SSID specified in sta_ssid only
        SPECIFIC_SSID,
    };

    EspWifiController(EspWifiController&) = delete;

    Mode _mode;
    StaMode _staMode;

    bool _eventLoopOurs;
    esp_event_handler_instance_t _handler_wifi;
    esp_event_handler_instance_t _handler_ip;
    std::unique_ptr<esp_netif_t, decltype(&esp_netif_destroy_default_wifi)> _netIf;
    esp_timer_handle_t _kvModifyTimer;
    std::mutex _modeLock;

    bool initGlobalsLocked();
    void deinitGlobalsLocked();

    bool switchMode(Mode newMode, std::unique_ptr<jac::KeyValueNamespace> mainNs = nullptr);
    static void onKvModifiedTimer(void *selfVoid);

    bool registerEventHandlersLocked();
    void unregisterEventHandlersLocked();
    void stopWifiLocked();

    static void eventHandlerWifi(void* selfVoid, esp_event_base_t event_base,
        int32_t event_id, void* event_data);
    static void eventHandlerWifiStop(void* semVoid, esp_event_base_t event_base,
        int32_t event_id, void* event_data);
    static void eventHandlerIp(void* selfVoid, esp_event_base_t event_base,
        int32_t event_id, void* event_data);

public:
    EspWifiController();

    bool initialize();

    void onKeyValueModified(const std::string& nsname, const std::string& key);
};
