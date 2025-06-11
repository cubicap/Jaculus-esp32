#pragma once

#include <jac/machine/machine.h>
#include <jac/machine/functionFactory.h>

#include <stdexcept>
#include <set>

#include "esp_http_client.h"
#include "esp_log.h"
#include "../platform/espWifi.h"


template<class Next>
class HttpClientFeature : public Next {

    class HttpClient {
    private:
        static const char* TAG;

        struct HttpResponse {
            char* data;
            size_t size;
            int status_code;
            bool success;

            HttpResponse() : data(nullptr), size(0), status_code(0), success(false) {
                data = (char*)malloc(1);
                if (data) {
                    data[0] = '\0';
                }
            }

            ~HttpResponse() {
                if (data) {
                    free(data);
                }
            }
        };

        static esp_err_t httpEventHandler(esp_http_client_event_t *evt) {
            HttpResponse* response = (HttpResponse*)evt->user_data;

            switch(evt->event_id) {
                case HTTP_EVENT_ON_DATA:
                    if (response->data) {
                        char* new_data = (char*)realloc(response->data, response->size + evt->data_len + 1);
                        if (new_data) {
                            response->data = new_data;
                            memcpy(response->data + response->size, evt->data, evt->data_len);
                            response->size += evt->data_len;
                            response->data[response->size] = '\0';
                        }
                    }
                    break;
                case HTTP_EVENT_ON_FINISH:
                    response->status_code = esp_http_client_get_status_code(evt->client);
                    response->success = (response->status_code == 200);
                    break;
                case HTTP_EVENT_ERROR:
                    ESP_LOGE(TAG, "HTTP_EVENT_ERROR");
                    break;
                default:
                    break;
            }
            return ESP_OK;
        }

    public:
        std::string get(std::string url) {
            // Check if WiFi is connected
            auto& wifi = EspWifiController::get();
            if (wifi.currentIp().addr == 0) {
                return "\\ERR:1"; // No IP address - WiFi not connected
            }

            HttpResponse response;

            esp_http_client_config_t config = {};
            config.url = url.c_str();
            config.event_handler = httpEventHandler;
            config.user_data = &response;
            config.timeout_ms = 5000;

            esp_http_client_handle_t client = esp_http_client_init(&config);
            if (!client) {
                return "\\ERR:2"; // Failed to initialize HTTP client
            }

            esp_err_t err = esp_http_client_perform(client);
            esp_http_client_cleanup(client);

            if (err != ESP_OK) {
                return "\\ERR:3"; // HTTP request failed
            }

            if (!response.success || response.status_code != 200) {
                return "\\ERR:" + std::to_string(response.status_code); // HTTP error status
            }

            if (!response.data) {
                return "\\ERR:4"; // No response data
            }

            return std::string(response.data);
        }
    };

public:
    HttpClient http;

    void initialize() {
        Next::initialize();

        jac::FunctionFactory ff(this->context());
        jac::Module& httpClientModule = this->newModule("httpClient");

        httpClientModule.addExport("get", ff.newFunction(noal::function(&HttpClient::get, &http)));
    }
};

template<class Next>
const char* HttpClientFeature<Next>::HttpClient::TAG = "HTTP_CLIENT";
