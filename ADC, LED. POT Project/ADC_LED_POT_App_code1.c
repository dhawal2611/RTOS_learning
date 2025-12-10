#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "driver/ledc.h"

#define LEDC_CHANNEL_0    LEDC_CHANNEL_0
#define LEDC_TIMER        LEDC_TIMER_0
#define LEDC_MODE         LEDC_LOW_SPEED_MODE
#define LEDC_RESOLUTION   LEDC_TIMER_12_BIT
#define LEDC_FREQ_HZ      5000
#define LEDC_DUTY         5000

#define LED_PIN         GPIO_NUM_2
#define SWITCH1_PIN     GPIO_NUM_0
#define SWITCH2_PIN     GPIO_NUM_15
#define ADC_CHANNEL     ADC_CHANNEL_6
#define ADC_WIDTH       ADC_WIDTH_BIT_12
#define ADC_ATTEN       ADC_ATTEN_DB_0

#define TAG "LED_Control"

// Global variables
uint16_t adc_value = 0;

// Task function for reading ADC value
void adc_task(void *pvParameters) {
    adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);

    while (1) {
        adc_value = adc1_get_raw(ADC_CHANNEL);
        vTaskDelay(pdMS_TO_TICKS(100)); // Read ADC value every 100ms
    }
}

// Task function for controlling LED brightness
void led_brightness_task(void *pvParameters) {
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_RESOLUTION,
        .freq_hz = LEDC_FREQ_HZ,
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .channel = LEDC_CHANNEL_0,
        .duty = 0,
        .gpio_num = LED_PIN,
        .speed_mode = LEDC_MODE,
        .timer_sel = LEDC_TIMER
    };
    ledc_channel_config(&ledc_channel);

    while (1) {
        // Update LED brightness based on ADC value
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_0, adc_value);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_0);
        vTaskDelay(pdMS_TO_TICKS(100)); // Check ADC value every 100ms
    }
}

// Task function for monitoring switch events
void switch_monitor_task(void *pvParameters) {
    //gpio_pad_select_gpio(SWITCH1_PIN);
    //gpio_pad_select_gpio(SWITCH2_PIN);
    gpio_set_direction(SWITCH1_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(SWITCH2_PIN, GPIO_MODE_INPUT);

    while (1) {
        // Monitor switch 1
        if (gpio_get_level(SWITCH1_PIN) == 0) {
            ESP_LOGI(TAG, "Switch 1 pressed");
        }
        // Monitor switch 2
        if (gpio_get_level(SWITCH2_PIN) == 0) {
            ESP_LOGI(TAG, "Switch 2 pressed");
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Check switches every 100ms
    }
}

// Task function for printing ADC values
void adc_print_task(void *pvParameters) {
    while (1) {
        ESP_LOGI(TAG, "ADC value: %u", (unsigned int)adc_value);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Print ADC value every 1s
    }
}

void app_main() {
    // Create tasks
    xTaskCreatePinnedToCore(adc_task, "adc_task", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(led_brightness_task, "led_brightness_task", 2048, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(switch_monitor_task, "switch_monitor_task", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(adc_print_task, "adc_print_task", 2048, NULL, 1, NULL, 0);

    // Calculate application footprint
    ESP_LOGI(TAG, "Application Footprint: %u bytes", (unsigned int)esp_get_free_heap_size() + heap_caps_get_free_size(MALLOC_CAP_8BIT));

    // Calculate stack size for each thread
    ESP_LOGI(TAG, "Stack size for adc_task: %u bytes", (unsigned int)uxTaskGetStackHighWaterMark(NULL));
    ESP_LOGI(TAG, "Stack size for led_brightness_task: %u bytes", (unsigned int)uxTaskGetStackHighWaterMark(NULL));
    ESP_LOGI(TAG, "Stack size for switch_monitor_task: %u bytes", (unsigned int)uxTaskGetStackHighWaterMark(NULL));
    ESP_LOGI(TAG, "Stack size for adc_print_task: %u bytes", (unsigned int)uxTaskGetStackHighWaterMark(NULL));

    // Calculate heap utilization
    ESP_LOGI(TAG, "Heap utilization: %u bytes", (unsigned int)esp_get_free_heap_size());
}

