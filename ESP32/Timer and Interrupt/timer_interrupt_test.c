#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define TAG "TIMER_DELAY"
#define LED_PIN GPIO_NUM_2

bool state = 0;

// Timer handler function
void timer_callback(TimerHandle_t xTimer) {
    // Task code to be executed after the timer expires
    state = !state;
    gpio_set_level(LED_PIN, state);
    ESP_LOGI(TAG, "Timer expired. Task code executed.");
}

void app_main() {
    
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    // Create a timer
    TimerHandle_t timer = xTimerCreate("delay_timer", pdMS_TO_TICKS(10000), pdFALSE, NULL, timer_callback);

    if (timer == NULL) {
        ESP_LOGE(TAG, "Failed to create timer.");
    } else {
        // Start the timer
        if (xTimerStart(timer, portMAX_DELAY) != pdPASS) {
            ESP_LOGE(TAG, "Failed to start timer.");
        }
    }

    // Application loop
    while (1) {
        // Your application code here
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}


/*#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN     GPIO_NUM_2
#define SWITCH1_PIN GPIO_NUM_15
#define SWITCH2_PIN GPIO_NUM_0

TaskHandle_t task1_handle = NULL;
TaskHandle_t task2_handle = NULL;

SemaphoreHandle_t led_semaphore = NULL;

// Task function to handle LED toggle for switch 1
void switch1_task(void *pvParameters) {
    while (1) {
        if (gpio_get_level(SWITCH1_PIN) == 0) {
            if (xSemaphoreTake(led_semaphore, portMAX_DELAY) == pdTRUE) {
                gpio_set_level(LED_PIN, 1);
                vTaskDelay(5000 / portTICK_PERIOD_MS); // LED ON for 5 seconds
                gpio_set_level(LED_PIN, 0);
                xSemaphoreGive(led_semaphore);
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Poll switch every 100ms
    }
}

// Task function to handle LED blink for switch 2
void switch2_task(void *pvParameters) {
    while (1) {
        if (gpio_get_level(SWITCH2_PIN) == 0) {
            if (xSemaphoreTake(led_semaphore, portMAX_DELAY) == pdTRUE) {
                for (int i = 0; i < 6; i++) {
                    gpio_set_level(LED_PIN, 1);
                    vTaskDelay(1000 / portTICK_PERIOD_MS); // LED ON for 1 second
                    gpio_set_level(LED_PIN, 0);
                    vTaskDelay(1000 / portTICK_PERIOD_MS); // LED OFF for 1 second
                }
                xSemaphoreGive(led_semaphore);
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Poll switch every 100ms
    }
}

// ISR for switch press
void IRAM_ATTR switch_isr_handler(void* arg) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (task1_handle != NULL && xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) {
        vTaskNotifyGiveFromISR(task1_handle, &xHigherPriorityTaskWoken);
    }
    if (task2_handle != NULL && xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) {
        vTaskNotifyGiveFromISR(task2_handle, &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void app_main() {
    // Configure LED pin as output
    //gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Configure switch pins as input with pull-up resistor
    //gpio_pad_select_gpio(SWITCH1_PIN);
    gpio_set_direction(SWITCH1_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(SWITCH1_PIN, GPIO_PULLUP_ONLY);

    //gpio_pad_select_gpio(SWITCH2_PIN);
    gpio_set_direction(SWITCH2_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(SWITCH2_PIN, GPIO_PULLUP_ONLY);

    //gpio_set_intr_type(SWITCH1_PIN, GPIO_INTR_HIGH_LEVEL);
    //gpio_set_intr_type(SWITCH2_PIN, GPIO_INTR_HIGH_LEVEL);
    // Install ISR service
    gpio_install_isr_service(0);

    // Hook ISR handler for specific GPIO pins
    gpio_isr_handler_add(SWITCH1_PIN, switch_isr_handler, (void*) SWITCH1_PIN);
    gpio_isr_handler_add(SWITCH2_PIN, switch_isr_handler, (void*) SWITCH2_PIN);

    // Create semaphore for LED access control
    led_semaphore = xSemaphoreCreateMutex();

    // Create tasks for switch handling
    xTaskCreate(switch1_task, "switch1_task", 2048, NULL, 5, &task1_handle);
    xTaskCreate(switch2_task, "switch2_task", 2048, NULL, 4, &task2_handle);
}
*/
