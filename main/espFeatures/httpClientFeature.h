#pragma once

#include <jac/machine/machine.h>
#include <jac/machine/functionFactory.h>
#include <jac/machine/values.h>

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
        jac::ContextRef _ctx;

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
        HttpClient() : _ctx(nullptr) {}
        HttpClient(jac::ContextRef ctx) : _ctx(ctx) {}

        jac::Object get(std::string url) {
            // Check if WiFi is connected
            auto& wifi = EspWifiController::get();
            if (wifi.currentIp().addr == 0) {
                jac::Object result = jac::Object::create(_ctx);
                result.set("status", -1);
                result.set("body", "\\ERR:1"); // No IP address - WiFi not connected
                return result;
            }

            HttpResponse response;

            esp_http_client_config_t config = {};
            config.url = url.c_str();
            config.event_handler = httpEventHandler;
            config.user_data = &response;
            config.timeout_ms = 5000;

            esp_http_client_handle_t client = esp_http_client_init(&config);
            if (!client) {
                jac::Object result = jac::Object::create(_ctx);
                result.set("status", -2);
                result.set("body", "\\ERR:2"); // Failed to initialize HTTP client
                return result;
            }

            esp_err_t err = esp_http_client_perform(client);
            esp_http_client_cleanup(client);

            if (err != ESP_OK) {
                jac::Object result = jac::Object::create(_ctx);
                result.set("status", -3);
                result.set("body", "\\ERR:3"); // HTTP request failed
                return result;
            }

            jac::Object result = jac::Object::create(_ctx);
            result.set("status", response.status_code);

            if (!response.data) {
                result.set("body", ""); // No response data
            } else {
                result.set("body", std::string(response.data));
            }

            return result;
        }
    };

public:
    HttpClient http;

    void initialize() {
        Next::initialize();

        // Initialize the HttpClient with the context
        http = HttpClient(this->context());

        jac::FunctionFactory ff(this->context());
        jac::Module& httpClientModule = this->newModule("httpClient");

        httpClientModule.addExport("get", ff.newFunction(noal::function(&HttpClient::get, &http)));
    }
};

template<class Next>
const char* HttpClientFeature<Next>::HttpClient::TAG = "HTTP_CLIENT";
