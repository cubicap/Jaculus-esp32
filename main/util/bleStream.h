#pragma once

#include <jac/link/stream.h>
#include <jac/device/logger.h>

#include <atomic>
#include <cstdint>
#include <deque>
#include <functional>
#include <span>
#include <string>
#include <thread>
#include <mutex>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_gatt_common_api.h"
#include "nvs_flash.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

// BLE Debug Logging Control
// Uncomment the next line to enable detailed BLE logging for debugging
// #define BLE_STREAM_DEBUG_LOGS

#ifdef BLE_STREAM_DEBUG_LOGS
    #define BLE_LOG_DEBUG(msg) jac::Logger::debug(msg)
    #define BLE_LOG_ERROR(msg) jac::Logger::error(msg)
    #define BLE_LOG_INFO(msg) jac::Logger::debug(msg)   // Use debug for info when enabled
#else
    #define BLE_LOG_DEBUG(msg) ((void)0)
    #define BLE_LOG_ERROR(msg) jac::Logger::error(msg)  // Keep errors always visible
    #define BLE_LOG_INFO(msg) ((void)0)                 // Disable info messages when debugging is off
#endif


class BleStream : public jac::Duplex {
    std::function<void()> _onData;
    std::deque<uint8_t> _buffer;
    std::mutex _bufferMutex;
    std::atomic<bool> _isConnected = false;
    std::atomic<bool> _notifyEnabled = false;
    std::atomic<bool> _isInitialized = false;

    // BLE configuration
    static constexpr uint16_t GATTS_SERVICE_UUID = 0x00FF;
    static constexpr uint16_t GATTS_CHAR_UUID = 0xFF01;
    static constexpr uint16_t GATTS_DESCR_UUID = 0x2902; // Client Characteristic Configuration
    static constexpr uint16_t GATTS_NUM_HANDLE = 4;
    static constexpr const char* DEVICE_NAME = "ESP32_JAC_BLE";

    // BLE handles
    static inline esp_gatt_if_t gatts_if = ESP_GATT_IF_NONE;
    static inline uint16_t service_handle = 0;
    static inline uint16_t char_handle = 0;
    static inline uint16_t descr_handle = 0;
    static inline uint16_t conn_id = 0;

    // Global instance pointer for callbacks
    static inline BleStream* instance = nullptr;

    // BLE advertising data
    static esp_ble_adv_data_t adv_data;
    static esp_ble_adv_params_t adv_params;

public:
    BleStream(const std::string& deviceName = "BLE_STREAM") {
        if (instance != nullptr) {
            throw std::runtime_error("Only one BleStream instance is allowed");
        }
        instance = this;
        // Note: deviceName could be used to customize the device name if needed
    }

    BleStream(BleStream&&) = delete;
    BleStream(const BleStream&) = delete;
    BleStream& operator=(BleStream&&) = delete;
    BleStream& operator=(const BleStream&) = delete;

    void start() {
        if (_isInitialized) {
            BLE_LOG_DEBUG("BLE Stream already initialized");
            return;
        }

        BLE_LOG_INFO("Starting BLE Stream initialization...");
        esp_err_t ret;

        // Note: NVS is already initialized in main.cpp, so we skip it here
        // Release Classic BT memory (only if not already released)
        static bool bt_mem_released = false;
        if (!bt_mem_released) {
            BLE_LOG_DEBUG("Releasing Classic BT memory...");
            ret = esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
            if (ret == ESP_OK) {
                bt_mem_released = true;
                BLE_LOG_DEBUG("Classic BT memory released successfully");
            } else {
                BLE_LOG_ERROR("Failed to release Classic BT memory: " + std::string(esp_err_to_name(ret)));
            }
        } else {
            BLE_LOG_DEBUG("Classic BT memory already released");
        }

        // Initialize BT controller
        BLE_LOG_DEBUG("Initializing BT controller...");
        esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
        ret = esp_bt_controller_init(&bt_cfg);
        if (ret) {
            BLE_LOG_ERROR("BLE controller init failed: " + std::string(esp_err_to_name(ret)));
            return;
        }
        BLE_LOG_DEBUG("BT controller initialized successfully");

        BLE_LOG_DEBUG("Enabling BLE mode...");
        ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
        if (ret) {
            BLE_LOG_ERROR("BLE controller enable failed: " + std::string(esp_err_to_name(ret)));
            return;
        }
        BLE_LOG_DEBUG("BLE mode enabled successfully");

        // Initialize Bluedroid
        BLE_LOG_DEBUG("Initializing Bluedroid stack...");
        ret = esp_bluedroid_init();
        if (ret) {
            BLE_LOG_ERROR("Bluedroid init failed: " + std::string(esp_err_to_name(ret)));
            return;
        }
        BLE_LOG_DEBUG("Bluedroid stack initialized successfully");

        BLE_LOG_DEBUG("Enabling Bluedroid stack...");
        ret = esp_bluedroid_enable();
        if (ret) {
            BLE_LOG_ERROR("Bluedroid enable failed: " + std::string(esp_err_to_name(ret)));
            return;
        }
        BLE_LOG_DEBUG("Bluedroid stack enabled successfully");

        // Register callbacks
        BLE_LOG_DEBUG("Registering GATTS callback...");
        ret = esp_ble_gatts_register_callback(gatts_event_handler);
        if (ret) {
            BLE_LOG_ERROR("GATTS register callback failed: " + std::string(esp_err_to_name(ret)));
            return;
        }
        BLE_LOG_DEBUG("GATTS callback registered successfully");

        BLE_LOG_DEBUG("Registering GAP callback...");
        ret = esp_ble_gap_register_callback(gap_event_handler);
        if (ret) {
            BLE_LOG_ERROR("GAP register callback failed: " + std::string(esp_err_to_name(ret)));
            return;
        }
        BLE_LOG_DEBUG("GAP callback registered successfully");

        // Register GATT application
        BLE_LOG_DEBUG("Registering GATT application...");
        ret = esp_ble_gatts_app_register(0);
        if (ret) {
            BLE_LOG_ERROR("GATTS app register failed: " + std::string(esp_err_to_name(ret)));
            return;
        }
        BLE_LOG_DEBUG("GATT application registration initiated");

        // Set MTU
        esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(500);
        if (local_mtu_ret) {
            BLE_LOG_ERROR("Set local MTU failed: " + std::string(esp_err_to_name(local_mtu_ret)));
        }

        _isInitialized = true;
        BLE_LOG_INFO("BLE Stream initialized successfully");
    }

    bool put(uint8_t c) override {
        return write(std::span<const uint8_t>(&c, 1)) == 1;
    }

    size_t write(std::span<const uint8_t> data) override {
        if (!_isConnected || !_notifyEnabled) {
            return data.size(); // Pretend write succeeded
        }

        // BLE has MTU limitations, so we may need to split large data
        size_t written = 0;
        const size_t maxChunkSize = 500; // Conservative MTU size

        while (written < data.size()) {
            size_t chunkSize = std::min(maxChunkSize, data.size() - written);

            esp_err_t ret = esp_ble_gatts_send_indicate(
                gatts_if,
                conn_id,
                char_handle,
                chunkSize,
                const_cast<uint8_t*>(data.data() + written),
                false // notification, not indication
            );

            if (ret != ESP_OK) {
                BLE_LOG_ERROR("BLE notification failed: " + std::string(esp_err_to_name(ret)));
                break;
            }

            written += chunkSize;

            // Small delay to avoid overwhelming the BLE stack
            if (written < data.size()) {
                vTaskDelay(pdMS_TO_TICKS(1));
            }
        }

        return written;
    }

    int get() override {
        std::lock_guard<std::mutex> lock(_bufferMutex);
        if (_buffer.empty()) {
            return -1;
        }
        uint8_t c = _buffer.front();
        _buffer.pop_front();
        return c;
    }

    size_t read(std::span<uint8_t> data) override {
        std::lock_guard<std::mutex> lock(_bufferMutex);
        size_t len = std::min(data.size(), _buffer.size());
        std::copy_n(_buffer.begin(), len, data.begin());
        _buffer.erase(_buffer.begin(), _buffer.begin() + len);
        return len;
    }

    bool flush() override {
        return true; // BLE notifications are sent immediately
    }

    void onData(std::function<void(void)> callback) override {
        _onData = callback;
    }

    ~BleStream() override {
        if (_isInitialized) {
            esp_bluedroid_disable();
            esp_bluedroid_deinit();
            esp_bt_controller_disable();
            esp_bt_controller_deinit();
        }
        if (instance == this) {
            instance = nullptr;
        }
    }

private:
    // Add received data to buffer and trigger callback
    void addReceivedData(const uint8_t* data, size_t len) {
        {
            std::lock_guard<std::mutex> lock(_bufferMutex);
            _buffer.insert(_buffer.end(), data, data + len);
        }

        if (_onData) {
            _onData();
        }
    }

    // GAP event handler
    static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
        BLE_LOG_DEBUG("GAP Event: " + std::to_string(event));

        switch (event) {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            BLE_LOG_DEBUG("Advertisement data set complete, starting advertising...");
            esp_ble_gap_start_advertising(&adv_params);
            break;
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                BLE_LOG_ERROR("Advertising start failed with status: " + std::to_string(param->adv_start_cmpl.status));
            } else {
                BLE_LOG_INFO("BLE advertising started successfully! Device should be discoverable now.");
            }
            break;
        case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
            if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                BLE_LOG_ERROR("Advertising stop failed");
            } else {
                BLE_LOG_DEBUG("BLE advertising stopped");
            }
            break;
        default:
            BLE_LOG_DEBUG("Unhandled GAP event: " + std::to_string(event));
            break;
        }
    }

    // GATTS event handler
    static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if_param, esp_ble_gatts_cb_param_t *param) {
        if (!instance) return;

        BLE_LOG_DEBUG("GATTS Event: " + std::to_string(event));

        switch (event) {
        case ESP_GATTS_REG_EVT:
            {
                BLE_LOG_DEBUG("GATTS register event - Starting BLE service setup");
                gatts_if = gatts_if_param;

                BLE_LOG_DEBUG("Setting device name to: " + std::string(DEVICE_NAME));
                esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(DEVICE_NAME);
                if (set_dev_name_ret) {
                    BLE_LOG_ERROR("Set device name failed: " + std::string(esp_err_to_name(set_dev_name_ret)));
                } else {
                    BLE_LOG_DEBUG("Device name set successfully");
                }

                // Configure advertising data
                esp_ble_adv_data_t adv_data = {
                    .set_scan_rsp = false,
                    .include_name = true,
                    .include_txpower = true,
                    .min_interval = 0x0006,
                    .max_interval = 0x0010,
                    .appearance = 0x00,
                    .manufacturer_len = 0,
                    .p_manufacturer_data = nullptr,
                    .service_data_len = 0,
                    .p_service_data = nullptr,
                    .service_uuid_len = 0,
                    .p_service_uuid = nullptr,
                    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
                };

                BLE_LOG_DEBUG("Configuring advertisement data...");
                esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
                if (ret) {
                    BLE_LOG_ERROR("Config adv data failed: " + std::string(esp_err_to_name(ret)));
                } else {
                    BLE_LOG_DEBUG("Advertisement data configured successfully");
                }

                // Create service
                BLE_LOG_DEBUG("Creating GATT service with UUID: 0x" + std::to_string(GATTS_SERVICE_UUID));
                esp_gatt_srvc_id_t service_id;
                service_id.is_primary = true;
                service_id.id.inst_id = 0x00;
                service_id.id.uuid.len = ESP_UUID_LEN_16;
                service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID;
                esp_err_t create_ret = esp_ble_gatts_create_service(gatts_if, &service_id, GATTS_NUM_HANDLE);
                if (create_ret) {
                    BLE_LOG_ERROR("Create service failed: " + std::string(esp_err_to_name(create_ret)));
                } else {
                    BLE_LOG_DEBUG("Service creation initiated");
                }
                break;
            }

        case ESP_GATTS_CREATE_EVT:
            {
                BLE_LOG_DEBUG("Service created with handle: " + std::to_string(param->create.service_handle));
                service_handle = param->create.service_handle;

                BLE_LOG_DEBUG("Starting service...");
                esp_err_t start_ret = esp_ble_gatts_start_service(service_handle);
                if (start_ret) {
                    BLE_LOG_ERROR("Start service failed: " + std::string(esp_err_to_name(start_ret)));
                } else {
                    BLE_LOG_DEBUG("Service start initiated");
                }

                // Add characteristic
                BLE_LOG_DEBUG("Adding characteristic with UUID: 0x" + std::to_string(GATTS_CHAR_UUID));
                esp_bt_uuid_t char_uuid = {
                    .len = ESP_UUID_LEN_16,
                    .uuid = {.uuid16 = GATTS_CHAR_UUID}
                };

                esp_gatt_char_prop_t char_property = ESP_GATT_CHAR_PROP_BIT_READ |
                                                    ESP_GATT_CHAR_PROP_BIT_WRITE |
                                                    ESP_GATT_CHAR_PROP_BIT_NOTIFY;

                esp_err_t add_char_ret = esp_ble_gatts_add_char(service_handle, &char_uuid,
                                     ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                     char_property, nullptr, nullptr);
                if (add_char_ret) {
                    BLE_LOG_ERROR("Add characteristic failed: " + std::string(esp_err_to_name(add_char_ret)));
                } else {
                    BLE_LOG_DEBUG("Characteristic addition initiated");
                }
                break;
            }

        case ESP_GATTS_ADD_CHAR_EVT:
            {
                BLE_LOG_DEBUG("Characteristic added with handle: " + std::to_string(param->add_char.attr_handle));
                char_handle = param->add_char.attr_handle;

                // Add descriptor for notifications
                esp_bt_uuid_t descr_uuid = {
                    .len = ESP_UUID_LEN_16,
                    .uuid = {.uuid16 = GATTS_DESCR_UUID}
                };

                BLE_LOG_DEBUG("Adding descriptor for notifications...");
                esp_err_t add_descr_ret = esp_ble_gatts_add_char_descr(service_handle, &descr_uuid,
                                           ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                           nullptr, nullptr);
                if (add_descr_ret) {
                    BLE_LOG_ERROR("Add descriptor failed: " + std::string(esp_err_to_name(add_descr_ret)));
                } else {
                    BLE_LOG_DEBUG("Descriptor addition initiated");
                }
                break;
            }

        case ESP_GATTS_ADD_CHAR_DESCR_EVT:
            {
                BLE_LOG_DEBUG("Descriptor added with handle: " + std::to_string(param->add_char_descr.attr_handle));
                descr_handle = param->add_char_descr.attr_handle;
                // Now that everything is set up, start advertising
                BLE_LOG_DEBUG("All services set up, starting BLE advertising...");
                esp_err_t adv_start_ret = esp_ble_gap_start_advertising(&adv_params);
                if (adv_start_ret) {
                    BLE_LOG_ERROR("Failed to start advertising: " + std::string(esp_err_to_name(adv_start_ret)));
                } else {
                    BLE_LOG_DEBUG("Advertising start command sent successfully");
                }
                break;
            }

        case ESP_GATTS_START_EVT:
            BLE_LOG_DEBUG("Service started");
            break;

        case ESP_GATTS_CONNECT_EVT:
            {
                BLE_LOG_INFO("BLE client connected");
                conn_id = param->connect.conn_id;
                instance->_isConnected = true;

                // Update connection parameters
                esp_ble_conn_update_params_t conn_params = {};
                memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
                conn_params.latency = 0;
                conn_params.max_int = 0x20;
                conn_params.min_int = 0x10;
                conn_params.timeout = 400;
                esp_ble_gap_update_conn_params(&conn_params);
                break;
            }

        case ESP_GATTS_DISCONNECT_EVT:
            BLE_LOG_INFO("BLE client disconnected");
            instance->_isConnected = false;
            instance->_notifyEnabled = false;
            esp_ble_gap_start_advertising(&adv_params);
            break;

        case ESP_GATTS_WRITE_EVT:
            {
                if (param->write.handle == descr_handle && param->write.len == 2) {
                    // Client Characteristic Configuration descriptor written
                    uint16_t descr_value = param->write.value[1] << 8 | param->write.value[0];
                    if (descr_value == 0x0001) {
                        BLE_LOG_INFO("BLE notifications enabled");
                        instance->_notifyEnabled = true;
                    } else {
                        BLE_LOG_DEBUG("BLE notifications disabled");
                        instance->_notifyEnabled = false;
                    }
                } else if (param->write.handle == char_handle) {
                    // Data written to characteristic
                    BLE_LOG_DEBUG("Received " + std::to_string(param->write.len) + " bytes via BLE");
                    instance->addReceivedData(param->write.value, param->write.len);
                }

                if (param->write.need_rsp) {
                    esp_ble_gatts_send_response(gatts_if, param->write.conn_id,
                                              param->write.trans_id, ESP_GATT_OK, nullptr);
                }
                break;
            }

        case ESP_GATTS_READ_EVT:
            {
                BLE_LOG_DEBUG("Read request for handle: " + std::to_string(param->read.handle));

                // Respond to read requests with empty data
                esp_gatt_rsp_t rsp = {};
                rsp.attr_value.handle = param->read.handle;
                rsp.attr_value.len = 0;
                rsp.attr_value.value[0] = 0x00; // Set at least one byte

                esp_err_t rsp_err = esp_ble_gatts_send_response(gatts_if, param->read.conn_id,
                                          param->read.trans_id, ESP_GATT_OK, &rsp);
                if (rsp_err != ESP_OK) {
                    BLE_LOG_ERROR("Failed to send read response: " + std::string(esp_err_to_name(rsp_err)));
                }
                break;
            }

        // Handle other events without error
        case ESP_GATTS_EXEC_WRITE_EVT:
            BLE_LOG_DEBUG("GATTS Execute Write Event");
            break;
        case ESP_GATTS_MTU_EVT:
            BLE_LOG_DEBUG("MTU updated to: " + std::to_string(param->mtu.mtu));
            break;
        case ESP_GATTS_CONF_EVT:
            // Confirmation received for notification/indication
            break;
        case ESP_GATTS_UNREG_EVT:
        case ESP_GATTS_ADD_INCL_SRVC_EVT:
        case ESP_GATTS_DELETE_EVT:
        case ESP_GATTS_STOP_EVT:
        case ESP_GATTS_OPEN_EVT:
        case ESP_GATTS_CANCEL_OPEN_EVT:
        case ESP_GATTS_CLOSE_EVT:
        case ESP_GATTS_LISTEN_EVT:
        case ESP_GATTS_CONGEST_EVT:
        case ESP_GATTS_RESPONSE_EVT:
        case ESP_GATTS_CREAT_ATTR_TAB_EVT:
        case ESP_GATTS_SET_ATTR_VAL_EVT:
        case ESP_GATTS_SEND_SERVICE_CHANGE_EVT:
        default:
            break;
        }
    }
};

// Static member initialization
esp_ble_adv_data_t BleStream::adv_data = {};
esp_ble_adv_params_t BleStream::adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x40,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .peer_addr = {},
    .peer_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};
