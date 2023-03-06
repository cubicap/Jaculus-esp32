#pragma once

#include <jac/link/stream.h>
#include "driver/uart.h"


class SerialStream : public Duplex {
    uart_port_t port;
public:
    SerialStream(uart_port_t port) : port(port) {}

    bool put(uint8_t c) override {
        return uart_write_bytes(port, (const char*)&c, 1) == 1;
    }

    size_t write(std::span<const uint8_t> data) override {
        return uart_write_bytes(port, (const char*)data.data(), data.size());
    }

    int get() override {
        uint8_t c;
        if (uart_read_bytes(port, &c, 1, 0) == 1) {
            return c;
        }
        return -1;
    }

    size_t read(std::span<uint8_t> data) override {
        return uart_read_bytes(port, data.data(), data.size(), 0);
    }

    bool flush() override {
        return uart_wait_tx_done(port, portMAX_DELAY) == ESP_OK;
    }
};

void initUart(uart_port_t uartNum, int baudRate, int rxPin, int txPin) {
    uart_config_t uart_config = {
        .baud_rate = baudRate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_DEFAULT
    };
    ESP_ERROR_CHECK(uart_driver_install(uartNum, 4096, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(uartNum, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uartNum, txPin, rxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}
