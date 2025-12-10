#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include <string.h>

#define UART_NUM UART_NUM_1 // Choose UART port (UART0 or UART1)
#define BUF_SIZE (1024)
#define TX_PIN (GPIO_NUM_17)  // Replace with the GPIO pin number connected to UART Tx
#define RX_PIN (GPIO_NUM_16)  // Replace with the GPIO pin number connected to UART Rx



void uart_init() {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
}

void uart_tx_task(void *pvParameters) {
    while (1) {
        const char *data = "Hello, UART!\n";
        uart_write_bytes(UART_NUM, data, strlen(data));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void uart_rx_task(void *pvParameters) {
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    while (1) {
        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE, 20 / portTICK_PERIOD_MS);
        if (len > 0) {
            data[len] = 0;
            printf("Received: %s", data);
        }
    }
    free(data);
}

void app_main() {
    uart_init();
    xTaskCreate(uart_tx_task, "uart_tx_task", 1024 * 2, NULL, 1, NULL);
    xTaskCreate(uart_rx_task, "uart_rx_task", 1024 * 2, NULL, 1, NULL);
}

