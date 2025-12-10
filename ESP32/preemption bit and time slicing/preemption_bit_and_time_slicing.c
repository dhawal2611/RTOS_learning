#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOSConfig.h"

#define LED1_PIN GPIO_NUM_2
#define LED2_PIN GPIO_NUM_0

TaskHandle_t task1_handle = NULL;
TaskHandle_t task2_handle = NULL;

void delay_ms(int ms) {
    //TickType_t delay_ticks = pdMS_TO_TICKS(ms);
    //vTaskDelay(delay_ticks);
    for (volatile int i = 0; i < 1000000; i++) {}  // Blocking loop based delay
}


// Task function for Task 1
void task1(void *pvParameters) {
    //gpio_pad_select_gpio(LED1_PIN);
    gpio_set_direction(LED1_PIN, GPIO_MODE_OUTPUT);
    while (1) {
        gpio_set_level(LED1_PIN, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
	//delay_ms(500);
        gpio_set_level(LED1_PIN, 0);
	//delay_ms(500);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

// Task function for Task 2
void task2(void *pvParameters) {
    //gpio_pad_select_gpio(LED2_PIN);
    gpio_set_direction(LED2_PIN, GPIO_MODE_OUTPUT);
    while (1) {
        gpio_set_level(LED2_PIN, 1);
	//delay_ms(500);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        gpio_set_level(LED2_PIN, 0);
	//delay_ms(500);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    // Create Task 1 with higher priority
    xTaskCreatePinnedToCore(task1, "task1", 2048, NULL, 2, &task1_handle, 0);

    // Create Task 2 with lower priority
    xTaskCreatePinnedToCore(task2, "task2", 2048, NULL, 1, &task2_handle, 0);
}

