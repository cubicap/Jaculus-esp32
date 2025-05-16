\
#pragma once

#include <jac/machine/machine.h>
#include <jac/machine/functionFactory.h>
#include <jac/machine/promise.h>
#include <jac/machine/error.h>
#include <jac/machine/object.h>
#include <jac/machine/value.h>

#include "../platform/espWifi.h" // For EspWifiController
#include "esp_http_client.h"    // For ESP-IDF HTTP client
#include "esp_log.h"            // For ESP_LOGE, ESP_LOGI

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <string>
#include <vector>
#include <map>
#include <functional> // For std::function
#include <algorithm> // For std::transform

// Forward declaration for the task arguments struct
template<class Next>
struct HttpClientTaskArgs;

// Helper struct to collect HTTP response data
struct HttpResponseData {
    std::string body;
    std::map<std::string, std::string> headers;
    int status_code = 0;
    std::string error_message; // Store error message if any during http processing
    bool success = false;
};

// ESP-IDF HTTP event handler
static esp_err_t http_event_handler(esp_http_client_event_t *evt) {
    HttpResponseData* response_data = static_cast<HttpResponseData*>(evt->user_data);
    if (!response_data) {
        ESP_LOGE("http_event_handler", "No user_data context");
        return ESP_FAIL;
    }

    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGE("http_event_handler", "HTTP_EVENT_ERROR");
            response_data->error_message = "HTTP client error";
            response_data->success = false;
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI("http_event_handler", "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            break;
        case HTTP_EVENT_ON_HEADER:
            if (evt->header_key && evt->header_value) {
                response_data->headers[std::string(evt->header_key)] = std::string(evt->header_value);
            }
            break;
        case HTTP_EVENT_ON_DATA:
            if (evt->data_len > 0) {
                response_data->body.append(static_cast<const char*>(evt->data), evt->data_len);
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI("http_event_handler", "HTTP_EVENT_ON_FINISH");
            response_data->status_code = esp_http_client_get_status_code(evt->client);
            response_data->success = true; // Mark as success, actual status code will determine outcome
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI("http_event_handler", "HTTP_EVENT_DISCONNECTED");
            // If disconnected before ON_FINISH and not an error, it might be an issue.
            // However, ON_FINISH should typically be the indicator of a completed request.
            break;
        default:
            break;
    }
    return ESP_OK;
}


template<class Next>
class HttpClientFeature : public Next {
public:
    // Struct to pass arguments to the FreeRTOS task
    struct TaskArgs {
        HttpClientFeature* feature;
        JSContext* ctx; // Store context for safety, though feature->context() should work
        JSValue promise_val; // Duplicated JSValue for the promise

        esp_http_client_method_t method;
        std::string url;
        std::optional<std::string> body;
        std::map<std::string, std::string> headers;
        int timeout_ms;

        // Cleanup method to be called by the task
        void cleanup() {
            // The promise_val is handled by the main thread callback
        }
    };

    void initialize() {
        Next::initialize();

        jac::FunctionFactory ff(this->context());
        auto& module = this->newModule("httpClient");

        // Specific methods
        module.addExport("get", ff.newFunction([this](jac::ValueWeak url_val, jac::ValueWeak options_val) {
            return this->handle_http_request("GET", url_val, jac::Value::undefined(this->context()), options_val);
        }));

        module.addExport("post", ff.newFunction([this](jac::ValueWeak url_val, jac::ValueWeak body_val, jac::ValueWeak options_val) {
            return this->handle_http_request("POST", url_val, body_val, options_val);
        }));

        module.addExport("put", ff.newFunction([this](jac::ValueWeak url_val, jac::ValueWeak body_val, jac::ValueWeak options_val) {
            return this->handle_http_request("PUT", url_val, body_val, options_val);
        }));

        module.addExport("del", ff.newFunction([this](jac::ValueWeak url_val, jac::ValueWeak options_val) { // 'delete' is a keyword
            return this->handle_http_request("DELETE", url_val, jac::Value::undefined(this->context()), options_val);
        }));

        module.addExport("patch", ff.newFunction([this](jac::ValueWeak url_val, jac::ValueWeak body_val, jac::ValueWeak options_val) {
            return this->handle_http_request("PATCH", url_val, body_val, options_val);
        }));

        module.addExport("head", ff.newFunction([this](jac::ValueWeak url_val, jac::ValueWeak options_val) {
            return this->handle_http_request("HEAD", url_val, jac::Value::undefined(this->context()), options_val);
        }));

        // Generic request function
        module.addExport("request", ff.newFunction([this](jac::ValueWeak method_val, jac::ValueWeak url_val, jac::ValueWeak body_val, jac::ValueWeak options_val) {
            if (!method_val.isString()) {
                auto promise = jac::Promise::create(this->context());
                promise.reject(this->newError("TypeError", "Method must be a string."));
                return static_cast<jac::Value>(promise);
            }
            std::string method_str = method_val.to<std::string>();
            // Convert method_str to uppercase for consistent matching
            std::transform(method_str.begin(), method_str.end(), method_str.begin(), ::toupper);
            return this->handle_http_request(method_str, url_val, body_val, options_val);
        }));
    }

private:
    static esp_http_client_method_t map_method_str_to_enum(const std::string& method_str) {
        if (method_str == "GET") return HTTP_METHOD_GET;
        if (method_str == "POST") return HTTP_METHOD_POST;
        if (method_str == "PUT") return HTTP_METHOD_PUT;
        if (method_str == "PATCH") return HTTP_METHOD_PATCH;
        if (method_str == "DELETE") return HTTP_METHOD_DELETE;
        if (method_str == "HEAD") return HTTP_METHOD_HEAD;
        if (method_str == "OPTIONS") return HTTP_METHOD_OPTIONS;
        return HTTP_METHOD_MAX; // Invalid/Unsupported
    }

    static void http_task_runner(void* params) {
        TaskArgs* args = static_cast<TaskArgs*>(params);
        HttpClientFeature* self = args->feature;
        JSContext* ctx = args->ctx; // Use stored context

        HttpResponseData response_collector;
        esp_err_t err_perform = ESP_FAIL; // Initialize to failure

        // WiFi check (already done before task creation, but good for safety)
        if (EspWifiController::get().currentIp().addr == 0) {
            response_collector.error_message = "WiFi not connected";
            response_collector.success = false;
        } else {
            esp_http_client_config_t config = {};
            config.url = args->url.c_str();
            config.method = args->method;
            config.event_handler = http_event_handler;
            config.user_data = &response_collector;
            config.timeout_ms = args->timeout_ms;
            // config.disable_auto_redirect = true; // Optional: handle redirects manually if needed
            // config.crt_bundle_attach = esp_crt_bundle_attach; // For HTTPS

            esp_http_client_handle_t client = esp_http_client_init(&config);

            if (!client) {
                response_collector.error_message = "Failed to initialize HTTP client";
                response_collector.success = false;
            } else {
                for (const auto& header_pair : args->headers) {
                    esp_http_client_set_header(client, header_pair.first.c_str(), header_pair.second.c_str());
                }

                if ((args->method == HTTP_METHOD_POST || args->method == HTTP_METHOD_PUT || args->method == HTTP_METHOD_PATCH) && args->body.has_value()) {
                    esp_http_client_set_post_field(client, args->body.value().c_str(), args->body.value().length());
                    // Default Content-Type for methods with body if not specified by user
                    if (args->headers.find("Content-Type") == args->headers.end() && args->headers.find("content-type") == args->headers.end()) {
                         esp_http_client_set_header(client, "Content-Type", "application/json"); // Common default
                    }
                }

                err_perform = esp_http_client_perform(client);

                if (err_perform != ESP_OK) {
                    ESP_LOGE("http_task_runner", "HTTP perform failed: %s", esp_err_to_name(err_perform));
                    // http_event_handler might have set an error, or we set a generic one
                    if (response_collector.error_message.empty()) {
                         response_collector.error_message = std::string("HTTP request failed: ") + esp_err_to_name(err_perform);
                    }
                    response_collector.success = false; // Ensure success is false
                }
                // If err_perform is OK, response_collector.success should be true from ON_FINISH
                // and status_code populated.

                esp_http_client_cleanup(client);
            }
        }

        // Schedule the promise resolution/rejection back on the main JS thread
        self->schedule([self, ctx_cap = ctx, promise_val_cap = args->promise_val, response_data_copy = response_collector]() {
            // Ensure we are using the correct context for JS operations
            auto promise = jac::Promise(ctx_cap, promise_val_cap);
            if (response_data_copy.success && response_data_copy.status_code > 0) { // Check status_code too
                jac::Object js_response = jac::Object::create(ctx_cap);
                js_response.set(ctx_cap, "status", jac::Value::from(ctx_cap, response_data_copy.status_code));
                js_response.set(ctx_cap, "body", jac::Value::from(ctx_cap, response_data_copy.body));

                jac::Object js_headers = jac::Object::create(ctx_cap);
                for (const auto& header : response_data_copy.headers) {
                    js_headers.set(ctx_cap, header.first, jac::Value::from(ctx_cap, header.second));
                }
                js_response.set(ctx_cap, "headers", js_headers);
                promise.resolve(js_response);
            } else {
                std::string error_msg = response_data_copy.error_message;
                if (error_msg.empty()) { // Generic error if not set
                    error_msg = "HTTP request failed with status " + std::to_string(response_data_copy.status_code);
                }
                promise.reject(self->newError("HttpError", error_msg));
            }

            JS_FreeValue(ctx_cap, promise_val_cap); // Free the duplicated promise value
            // Assuming releaseLifetime takes ValueWeak or similar
            self->releaseLifetime(jac::ValueWeak(ctx_cap, promise_val_cap));
        });

        args->cleanup(); // Perform any cleanup in args
        delete args;     // Free task arguments
        vTaskDelete(NULL); // Delete task itself
    }

    jac::Value handle_http_request(const std::string& method_str_upper, jac::ValueWeak url_val, jac::ValueWeak body_val, jac::ValueWeak options_val) {
        auto promise = jac::Promise::create(this->context());

        esp_http_client_method_t http_method = map_method_str_to_enum(method_str_upper);

        if (http_method == HTTP_METHOD_MAX) {
            promise.reject(this->newError("ValueError", "Unsupported HTTP method: " + method_str_upper));
            return promise;
        }

        if (EspWifiController::get().currentIp().addr == 0) {
            promise.reject(this->newError("NetworkError", "WiFi not connected or no IP address."));
            return promise;
        }

        if (!url_val.isString()) {
            promise.reject(this->newError("TypeError", "URL must be a string."));
            return promise;
        }
        std::string url_str = url_val.to<std::string>();

        std::optional<std::string> body_str_opt;
        // Check if method can have a body
        bool method_can_have_body = (http_method == HTTP_METHOD_POST || http_method == HTTP_METHOD_PUT || http_method == HTTP_METHOD_PATCH);

        if (method_can_have_body) {
            if (body_val.isString()) {
                body_str_opt = body_val.to<std::string>();
            } else if (body_val.isObject() || body_val.isArray()) { // Auto-stringify JSON objects/arrays
                JSValue str_val = JS_JSONStringify(this->context(), body_val.getVal(), JS_UNDEFINED, JS_UNDEFINED);
                if (JS_IsException(str_val)) {
                    promise.reject(this->newError("TypeError", "Failed to stringify body object/array."));
                    // JS_FreeValue(this->context(), str_val); // Not needed, JS_GetException does it
                    return promise;
                }
                const char* c_str = JS_ToCString(this->context(), str_val);
                if (c_str) {
                    body_str_opt = std::string(c_str);
                    JS_FreeCString(this->context(), c_str);
                }
                JS_FreeValue(this->context(), str_val);

            } else if (!body_val.isUndefined() && !body_val.isNull()) {
                 promise.reject(this->newError("TypeError", "Request body for " + method_str_upper + " must be a string, object, or array."));
                 return promise;
            }
        } else {
            // If method doesn't typically have a body, but one is provided, it might be an error or ignored.
            // For simplicity, we'll reject if a body is provided for GET/DELETE/HEAD etc.
            if (!body_val.isUndefined() && !body_val.isNull()) {
                promise.reject(this->newError("TypeError", "Request body is not applicable for " + method_str_upper + " method."));
                return promise;
            }
        }

        std::map<std::string, std::string> headers_map;
        int timeout_ms = 5000; // Default timeout

        if (options_val.isObject()) {
            jac::Object options_obj = options_val.to<jac::Object>();
            if (options_obj.has("headers")) {
                jac::ValueWeak headers_val = options_obj.get(this->context(), "headers");
                if (headers_val.isObject()) {
                    jac::Object headers_obj = headers_val.to<jac::Object>();
                    std::vector<std::string> keys = headers_obj.keys(this->context());
                    for (const std::string& key : keys) {
                        jac::ValueWeak header_v = headers_obj.get(this->context(), key);
                        if (header_v.isString()) {
                            headers_map[key] = header_v.to<std::string>();
                        }
                    }
                }
            }
            if (options_obj.has("timeout")) {
                jac::ValueWeak timeout_v = options_obj.get(this->context(), "timeout");
                if (timeout_v.isNumber()) {
                    timeout_ms = timeout_v.to<int>();
                    if (timeout_ms < 0) timeout_ms = 0; // 0 means wait indefinitely for esp_http_client
                }
            }
        }

        // Extend lifetime of the promise object so it's not GC'd while the task runs
        this->extendLifetime(promise);

        TaskArgs* task_args = new TaskArgs{
            this,
            this->context(),
            JS_DupValue(this->context(), promise.getVal()), // Duplicate for the new task
            http_method,
            url_str,
            body_str_opt,
            headers_map,
            timeout_ms
        };

        // Create a FreeRTOS task to handle the blocking HTTP request
        BaseType_t task_created = xTaskCreate(
            http_task_runner,    // Function to implement the task
            "http_client_task",  // Name of the task
            8192,                // Stack size in words (adjust as needed)
            task_args,           // Task input parameter
            5,                   // Priority of the task
            NULL                 // Task handle (optional)
        );

        if (task_created != pdPASS) {
            ESP_LOGE("HttpClientFeature", "Failed to create HTTP client task");
            // Task creation failed, clean up and reject promise
            JS_FreeValue(this->context(), task_args->promise_val); // Free the duplicated value
            delete task_args;
            this->releaseLifetime(promise); // Release from lifetime management
            promise.reject(this->newError("InternalError", "Failed to create HTTP task."));
        }

        return promise;
    }
};
