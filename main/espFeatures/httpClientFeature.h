#pragma once

#include <jac/machine/machine.h>
#include <jac/machine/functionFactory.h>
#include <jac/machine/values.h>

#include <stdexcept>
#include <set>

#include "esp_http_client.h"
#include "esp_log.h"
#include "../platform/espWifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


template<class Next>
class HttpClientFeature : public Next {

    class HttpClient {
    private:
        static const char* TAG;
        jac::ContextRef _ctx;
        HttpClientFeature* _feature;

        struct HttpResponse {
            char* data;
            size_t size;
            int status_code;
            bool success;

        private:
            void initializeEmptyData() {
                data = (char*)malloc(1);
                if (data) {
                    data[0] = '\0';
                }
            }

            void copyDataFrom(const HttpResponse& other) {
                if (other.data && other.size > 0) {
                    data = (char*)malloc(other.size + 1);
                    if (data) {
                        memcpy(data, other.data, other.size);
                        data[other.size] = '\0';
                    }
                } else {
                    initializeEmptyData();
                }
            }

        public:
            HttpResponse() : data(nullptr), size(0), status_code(0), success(false) {
                initializeEmptyData();
            }

            HttpResponse(const HttpResponse& other) : data(nullptr), size(other.size), status_code(other.status_code), success(other.success) {
                copyDataFrom(other);
            }

            HttpResponse(HttpResponse&& other) noexcept : data(other.data), size(other.size), status_code(other.status_code), success(other.success) {
                other.data = nullptr;
                other.size = 0;
            }

            HttpResponse& operator=(const HttpResponse& other) {
                if (this != &other) {
                    free(data);
                    size = other.size;
                    status_code = other.status_code;
                    success = other.success;
                    copyDataFrom(other);
                }
                return *this;
            }

            HttpResponse& operator=(HttpResponse&& other) noexcept {
                if (this != &other) {
                    free(data);
                    data = other.data;
                    size = other.size;
                    status_code = other.status_code;
                    success = other.success;
                    other.data = nullptr;
                    other.size = 0;
                }
                return *this;
            }

            ~HttpResponse() {
                free(data);
            }
        };

        struct HttpTaskData {
            std::string url;
            std::string method;
            std::string data;
            std::string contentType;
            jac::Function resolve;
            jac::Function reject;
            jac::ContextRef ctx;
            HttpClientFeature* feature;

            HttpTaskData(std::string url, std::string method, std::string data, std::string contentType,
                        jac::Function resolve, jac::Function reject, jac::ContextRef ctx, HttpClientFeature* feature)
                : url(std::move(url)), method(std::move(method)), data(std::move(data)), contentType(std::move(contentType)),
                  resolve(std::move(resolve)), reject(std::move(reject)), ctx(ctx), feature(feature) {}
        };

        // Helper function to create JavaScript Error objects
        static jac::Object createErrorObject(jac::ContextRef ctx, const std::string& message) {
            jac::Object errorObj = jac::Object::create(ctx);
            errorObj.set("name", "Error");
            errorObj.set("message", message);
            return errorObj;
        }

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

        static void httpRequestTask(void* pvParameters) {
            HttpTaskData* taskData = static_cast<HttpTaskData*>(pvParameters);

            // Double-check WiFi connectivity inside the task
            auto& wifi = EspWifiController::get();
            if (wifi.currentIp().addr == 0) {
                taskData->feature->scheduleEvent([taskData]() {
                    auto errorObj = createErrorObject(taskData->ctx, "WiFi connection lost during request");
                    taskData->reject.template call<void>(errorObj);
                    delete taskData;
                });
                vTaskDelete(NULL);
                return;
            }

            HttpResponse* response = new HttpResponse();

            esp_http_client_config_t config = {};
            config.url = taskData->url.c_str();
            config.event_handler = httpEventHandler;
            config.user_data = response;
            config.timeout_ms = 5000;

            esp_http_client_handle_t client = esp_http_client_init(&config);
            if (!client) {
                taskData->feature->scheduleEvent([taskData, response]() {
                    auto errorObj = createErrorObject(taskData->ctx, "Failed to initialize HTTP client");
                    taskData->reject.template call<void>(errorObj);
                    delete response; // Clean up response
                    delete taskData;
                });
                vTaskDelete(NULL);
                return;
            }

            // Configure HTTP method and data
            esp_http_client_method_t httpMethod = HTTP_METHOD_GET;
            if (taskData->method == "POST") {
                httpMethod = HTTP_METHOD_POST;
            } else if (taskData->method == "PUT") {
                httpMethod = HTTP_METHOD_PUT;
            } else if (taskData->method == "DELETE") {
                httpMethod = HTTP_METHOD_DELETE;
            }
            esp_http_client_set_method(client, httpMethod);

            // Set data for POST/PUT methods
            if ((taskData->method == "POST" || taskData->method == "PUT") && !taskData->data.empty()) {
                esp_http_client_set_header(client, "Content-Type", taskData->contentType.c_str());
                esp_http_client_set_post_field(client, taskData->data.c_str(), taskData->data.length());
            }

            esp_err_t err = esp_http_client_perform(client);
            esp_http_client_cleanup(client);

            // Schedule the response callback on the main thread
            taskData->feature->scheduleEvent([taskData, response, err]() mutable {
                if (err != ESP_OK) {
                    auto errorObj = createErrorObject(taskData->ctx, "HTTP request failed");
                    taskData->reject.template call<void>(errorObj);
                } else {
                    jac::Object result = jac::Object::create(taskData->ctx);
                    result.set("status", response->status_code);
                    result.set("body", response->data ? std::string(response->data) : "");
                    taskData->resolve.template call<void>(result);
                }
                delete response;
                delete taskData;
            });

            vTaskDelete(NULL);
        }

    public:
        HttpClient() : _ctx(nullptr), _feature(nullptr) {}
        HttpClient(jac::ContextRef ctx, HttpClientFeature* feature) : _ctx(ctx), _feature(feature) {}

        jac::Value request(std::string url, std::string method = "GET", std::string data = "", std::string contentType = "application/json") {
            // Check if WiFi is connected - throw exception if no IP
            auto& wifi = EspWifiController::get();
            if (wifi.currentIp().addr == 0) {
                throw jac::Exception::create(jac::Exception::Type::Error, "No IP address - WiFi not connected");
            }

            auto [promise, resolve, reject] = jac::Promise::create(_ctx);

            // Create task data with all the request information
            HttpTaskData* taskData = new HttpTaskData(url, method, data, contentType, resolve, reject, _ctx, _feature);

            // Create a FreeRTOS task to handle the HTTP request asynchronously
            BaseType_t result = xTaskCreate(
                httpRequestTask,     // Task function
                "http_request",      // Task name
                8192,                // Stack size (8KB)
                taskData,            // Parameters passed to task
                5,                   // Priority
                nullptr              // Task handle (we don't need it)
            );

            if (result != pdPASS) {
                delete taskData;
                throw jac::Exception::create(jac::Exception::Type::Error, "Failed to create HTTP request task");
            }

            return promise;
        }

        jac::Value get(std::string url) {
            return request(url, "GET");
        }

        jac::Value post(std::string url, std::string data = "", std::string contentType = "application/json") {
            return request(url, "POST", data, contentType);
        }

        jac::Value put(std::string url, std::string data = "", std::string contentType = "application/json") {
            return request(url, "PUT", data, contentType);
        }

        jac::Value del(std::string url) {
            return request(url, "DELETE");
        }
    };

public:
    HttpClient http;

private:
    // Helper function to create variadic HTTP method functions
    auto createHttpMethodFunction(const std::string& method) {
        return [this, method](std::vector<jac::ValueWeak> args) {
            if (args.size() < 1 || args.size() > 3) {
                throw std::runtime_error("Invalid number of arguments for " + method + " method");
            }

            std::string url = args[0].to<std::string>();
            std::string data = (args.size() > 1) ? args[1].to<std::string>() : "";
            std::string contentType = (args.size() > 2) ? args[2].to<std::string>() : "application/json";

            return http.request(url, method, data, contentType);
        };
    }

public:
    void initialize() {
        Next::initialize();

        http = HttpClient(this->context(), this);

        jac::FunctionFactory ff(this->context());
        jac::Module& httpClientModule = this->newModule("httpClient");

        httpClientModule.addExport("get", ff.newFunction(noal::function(&HttpClient::get, &http)));
        httpClientModule.addExport("post", ff.newFunctionVariadic(createHttpMethodFunction("POST")));
        httpClientModule.addExport("put", ff.newFunctionVariadic(createHttpMethodFunction("PUT")));
        httpClientModule.addExport("del", ff.newFunction(noal::function(&HttpClient::del, &http)));
    }
};

template<class Next>
const char* HttpClientFeature<Next>::HttpClient::TAG = "HTTP_CLIENT";
