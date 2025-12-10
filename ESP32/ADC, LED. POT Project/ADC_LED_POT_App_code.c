#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/ledc.h"
//#include "esp_adc/adc_continuous.h"
#include "esp_log.h"
#include "esp_adc_cal.h"


#define SWITCH_PIN_2  GPIO_NUM_15  // GPIO pin for switch
#define SWITCH_PIN_1  GPIO_NUM_0  // GPIO pin for switch

#define ADC_CHANNEL     ADC1_CHANNEL_6 // ADC1 channel 6 is GPIO34
#define ADC_WIDTH       ADC_WIDTH_BIT_12
#define ADC_ATTEN       ADC_ATTEN_DB_11

#define LED_PIN		GPIO_NUM_2

#define DEFAULT_VREF    1100        // Default ADC reference voltage is 1100mV
#define LEDC_CHANNEL    LEDC_CHANNEL_0
#define LEDC_TIMER      LEDC_TIMER_0
#define LEDC_MODE       LEDC_HIGH_SPEED_MODE
#define LEDC_RESOLUTION LEDC_TIMER_13_BIT // PWM resolution

#define TAG "POT_Read"


SemaphoreHandle_t status_semaphore = NULL;

static esp_adc_cal_characteristics_t adc1_chars;
int adc_value = 0;

void adc_task(void *pvParameters) {
    // Configure ADC
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN, ADC_WIDTH, 0, &adc1_chars);

    adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);

    while (1) 
    {
        if (xSemaphoreTake(status_semaphore, portMAX_DELAY) == pdTRUE) {
        	adc_value = adc1_get_raw(ADC_CHANNEL);
        	xSemaphoreGive(status_semaphore);
        }
        //adc_value = adc1_get_raw(ADC_CHANNEL);
        //printf("ADC Value: %d\n", adc_value);
        vTaskDelay(500/ portTICK_PERIOD_MS);
    	//ESP_LOGI(TAG, "Stack size for adc_task: %u bytes", (unsigned int)uxTaskGetStackHighWaterMark(NULL));
    }
    /*adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);

    while (1) {
        // Read ADC value
        uint16_t adc_value = adc1_get_raw(ADC_CHANNEL);
        ESP_LOGI(TAG, "ADC value: %d", adc_value);

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }*/
}

void pwm_task(void *pvParameters) {
	// Configure LED PWM
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT,
        .freq_hz = 5000, // Set PWM frequency to 5 kHz
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .channel = LEDC_CHANNEL,
        .duty = 0,
        .gpio_num = LED_PIN,
        .speed_mode = LEDC_MODE,
        .timer_sel = LEDC_TIMER
    };
    ledc_channel_config(&ledc_channel);
    while (1) {
        if (xSemaphoreTake(status_semaphore, portMAX_DELAY) == pdTRUE) {
		uint32_t duty = adc_value * (1 << LEDC_RESOLUTION) / 4095; // Scale ADC reading to PWM duty cycle
		ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
		ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
        	xSemaphoreGive(status_semaphore);
        }
	// Convert ADC reading to LED brightness
	//uint32_t duty = adc_value * (1 << LEDC_RESOLUTION) / 4095; // Scale ADC reading to PWM duty cycle

	// Update LED brightness
	//ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
	//ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
	vTaskDelay(pdMS_TO_TICKS(100)); // Check ADC value every 100ms
    	//ESP_LOGI(TAG, "Stack size for pwm_task: %u bytes", (unsigned int)uxTaskGetStackHighWaterMark(NULL));
    }
}

void switch_task(void *pvParameter) {
    // Configure switch pin as input
    gpio_set_direction(SWITCH_PIN_1, GPIO_MODE_INPUT);
    gpio_set_direction(SWITCH_PIN_2, GPIO_MODE_INPUT);

    // Create LED task

    while (1) {
        if (gpio_get_level(SWITCH_PIN_1) == 0) {
            if (xSemaphoreTake(status_semaphore, portMAX_DELAY) == pdTRUE) {
	    	printf("Swich1 Pressed, ADC Value: %d\n", adc_value);
                xSemaphoreGive(status_semaphore);
            }
        } else if (gpio_get_level(SWITCH_PIN_2) == 0) {
            if (xSemaphoreTake(status_semaphore, portMAX_DELAY) == pdTRUE) {
	    	printf("Swich2 Pressed, ADC Value: %d\n", adc_value);
                xSemaphoreGive(status_semaphore);
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Poll switch every 100ms
    }
    /*while (1) {
        // Monitor the switch state
        if (gpio_get_level(SWITCH_PIN_1) == 0) {
	    printf("Swich1 Pressed, ADC Value: %d\n", adc_value);
            //gpio_set_level(LED_PIN, 1); // Turn on LED when switch is pressed
        } else if (gpio_get_level(SWITCH_PIN_2) == 0) {
	    printf("Swich2 Pressed, ADC Value: %d\n", adc_value);
            //gpio_set_level(LED_PIN, 0); // Turn off LED when switch is not pressed
        }
	//vTaskDelay(pdMS_TO_TICKS(100)); // Check ADC value every 100ms
        vTaskDelay(100 / portTICK_PERIOD_MS); // Delay for 100 milliseconds
    	//ESP_LOGI(TAG, "Stack size for switch__task: %u bytes", (unsigned int)uxTaskGetStackHighWaterMark(NULL));
    }*/
}

void app_main() {
    
    // Create semaphore for LED access control
    status_semaphore = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(adc_task, "adc_task", 2048, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(pwm_task, "pwm_task", 2048, NULL, 4, NULL, 0);
    //xTaskCreate(&switch_task, "switch_task", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
    xTaskCreate(&switch_task, "switch_task", 2048, NULL, 3, NULL);

    // Calculate application footprint
    ESP_LOGI(TAG, "Application Footprint: %u bytes", (unsigned int)esp_get_free_heap_size() + heap_caps_get_free_size(MALLOC_CAP_8BIT));

    // Calculate stack size for each thread
    ESP_LOGI(TAG, "Stack size for adc_task: %u bytes", (unsigned int)uxTaskGetStackHighWaterMark(NULL));
    ESP_LOGI(TAG, "Stack size for pwm_task: %u bytes", (unsigned int)uxTaskGetStackHighWaterMark(NULL));
    ESP_LOGI(TAG, "Stack size for switch__task: %u bytes", (unsigned int)uxTaskGetStackHighWaterMark(NULL));

    // Calculate heap utilization
    ESP_LOGI(TAG, "Heap utilization: %u bytes", (unsigned int)esp_get_free_heap_size());
}



