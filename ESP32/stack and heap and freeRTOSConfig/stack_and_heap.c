// Get Heap Size
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#define TAG "HeapInfoTask"

// Task function
void heap_info_task(void *pvParameters) {
    while (1) {
        // Get the available heap size
        size_t free_heap_size = xPortGetFreeHeapSize();

        // Log the available heap size
        ESP_LOGI(TAG, "Available heap size: %u bytes", free_heap_size);

        // Delay for a while before checking again
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main() {
    // Create the heap info task
    xTaskCreatePinnedToCore(heap_info_task, "heap_info_task", 2048, NULL, 1, NULL, 0);
}


/* //Get Stack Size
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define TAG "StackInfoTask"

// Task function
void stack_info_task(void *pvParameters) {
    // Get the handle of the current task
    TaskHandle_t task_handle = xTaskGetCurrentTaskHandle();

    while (1) {
        // Get the available stack size
        uint32_t stack_size = uxTaskGetStackHighWaterMark(task_handle);

        // Log the available stack size (cast to unsigned int)
        ESP_LOGI(TAG, "Available stack size: %u bytes", (unsigned int)stack_size);

        // Delay for a while before checking again
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main() {
    // Create the stack info task
    xTaskCreatePinnedToCore(stack_info_task, "stack_info_task", 2048, NULL, 1, NULL, 0);
}
*/
