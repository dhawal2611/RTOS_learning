#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON_PIN GPIO_NUM_15
#define LED1_PIN GPIO_NUM_2
#define LED2_PIN GPIO_NUM_0

bool l1 = 0;

// Task function to toggle LED2
void toggle_led2_task(void *pvParameters) {
    //gpio_pad_select_gpio(LED2_PIN);
    gpio_set_direction(LED2_PIN, GPIO_MODE_OUTPUT);
    while (1) {
        gpio_set_level(LED2_PIN, 1);  // Turn LED2 ON
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(LED2_PIN, 0);  // Turn LED2 OFF
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Interrupt service routine for button press
void IRAM_ATTR button_isr_handler(void* arg) {
	l1 = !l1;
	gpio_set_level(LED1_PIN, l1); // Toggle LED1
}

void app_main() {
    // Configure LED1 pin as output
    //gpio_pad_select_gpio(LED1_PIN);
    gpio_set_direction(LED1_PIN, GPIO_MODE_OUTPUT);

    // Configure button pin as input with pull-up resistor
   // gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_PULLDOWN);
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_HIGH_LEVEL);

    //gpio_intr_enable(BUTTON_PIN);
    // Install ISR service
    gpio_install_isr_service(0);

    // Hook ISR handler for specific GPIO pin
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, (void*) BUTTON_PIN);

    // Create task to toggle LED2
    xTaskCreate(&toggle_led2_task, "toggle_led2_task", 2048, NULL, 5, NULL);
}




/*#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED1_PIN GPIO_NUM_2
#define LED2_PIN GPIO_NUM_0

// Task function to blink LED1
void blink_led1_task(void *pvParameters) {
    //gpio_pad_select_gpio(LED1_PIN);
    gpio_set_direction(LED1_PIN, GPIO_MODE_OUTPUT);
    while (1) {
        gpio_set_level(LED1_PIN, 1);  // Turn LED1 ON
        vTaskDelay(500 / portTICK_PERIOD_MS);
        gpio_set_level(LED1_PIN, 0);  // Turn LED1 OFF
        vTaskDelay(500 / portTICK_PERIOD_MS);
	printf("LED1 operated\n");
    }
}

// Task function to blink LED2
void blink_led2_task(void *pvParameters) {
    //gpio_pad_select_gpio(LED2_PIN);
    gpio_set_direction(LED2_PIN, GPIO_MODE_OUTPUT);
    while (1) {
        gpio_set_level(LED2_PIN, 1);  // Turn LED2 ON
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(LED2_PIN, 0);  // Turn LED2 OFF
        vTaskDelay(5000 / portTICK_PERIOD_MS);
	printf("LED2 operated\n");
    }
}

void app_main() {
    xTaskCreate(&blink_led1_task, "blink_led1_task", 2048, NULL, 5, NULL);
    xTaskCreate(&blink_led2_task, "blink_led2_task", 2048, NULL, 4, NULL);
}
*/

/*

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON_PIN     GPIO_NUM_15
#define LED_PIN        GPIO_NUM_2
#define LED_PIN_1        GPIO_NUM_0

int ledchangestate = 0;
int ledchangestate1 = 0;

// Task function to toggle LED
void toggle_led_task(void *pvParameters) {
    while (1) {
        // Toggle LED
	ledchangestate = !ledchangestate;
        gpio_set_level(LED_PIN, ledchangestate);
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Toggle LED every 1 second
    }
}

// Interrupt service routine for button press
void IRAM_ATTR button_isr_handler(void* arg) {
    // Toggle LED
    ledchangestate1 = !ledchangestate1;
    gpio_set_level(LED_PIN_1, ledchangestate1);
}

void app_main() {
    // Configure LED pin as output
    //gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_PIN_1, GPIO_MODE_OUTPUT);

    // Configure button pin as input with pull-up resistor
    //gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    // Install ISR service
    gpio_install_isr_service(0);

    // Hook ISR handler for specific GPIO pin
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    // Create task to toggle LED
    xTaskCreate(toggle_led_task, "toggle_led_task", 2048, NULL, 5, NULL);
}

*/

/*#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define INPUT_PIN 15
#define LED_PIN 2

int state = 0;
QueueHandle_t interputQueue;
//QueueHandle_t button_queue; // Queue for communication between ISR and task

static void IRAM_ATTR gpio_interrupt_handler(void *args)
{
    //int pinNumber = (int)args;
    gpio_set_level(LED_PIN, !gpio_get_level(LED_PIN));
    //xQueueSendFromISR(interputQueue, &pinNumber, NULL);
}

void LED_Control_Task(void *params)
{
    int pinNumber, count = 0;
    while (true)
    {
        if (xQueueReceive(interputQueue, &pinNumber, portMAX_DELAY))
        {
            printf("GPIO %d was pressed %d times. The state is %d\n", pinNumber, count++, gpio_get_level(INPUT_PIN));
            //gpio_set_level(LED_PIN, !gpio_get_level(INPUT_PIN));
        }
    }
}

void app_main()
{
    //gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    //gpio_pad_select_gpio(INPUT_PIN);
    gpio_set_direction(INPUT_PIN, GPIO_MODE_INPUT);
    gpio_pulldown_en(INPUT_PIN);
    gpio_pullup_dis(INPUT_PIN);
    gpio_set_intr_type(INPUT_PIN, GPIO_INTR_POSEDGE);

    //interputQueue = xQueueCreate(10, sizeof(int));
    //xTaskCreate(LED_Control_Task, "LED_Control_Task", 2048, NULL, 1, NULL);

    gpio_install_isr_service(0);
    //gpio_isr_handler_add(INPUT_PIN, gpio_interrupt_handler, (void *)INPUT_PIN);
    gpio_isr_handler_add(INPUT_PIN, gpio_interrupt_handler, NULL);
    while(1) {

    }
}
*/
/*
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

#define BUTTON_PIN GPIO_NUM_15 // Example GPIO pin for a button

QueueHandle_t button_queue; // Queue for communication between ISR and task

// Interrupt service routine for button press
void IRAM_ATTR button_isr_handler(void* arg) {
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(button_queue, &gpio_num, NULL); // Send GPIO number to queue
}

// Task function to process data from ISR
void button_task(void *pvParameters) {
    uint32_t gpio_num;
    while(1) {
        if (xQueueReceive(button_queue, &gpio_num, portMAX_DELAY) == pdTRUE) {
            printf("Button pressed on GPIO %ld\n", gpio_num);
            // Process data or trigger action here
        }
    }
}

void app_main() {
    // Create queue for button presses
    button_queue = xQueueCreate(10, sizeof(uint32_t));

    // Configure button pin as input with pull-up resistor
    gpio_config_t gpio_config;
    gpio_config.intr_type = GPIO_INTR_NEGEDGE; // Interrupt on falling edge
    gpio_config.mode = GPIO_MODE_INPUT;
    gpio_config.pin_bit_mask = (1ULL << BUTTON_PIN);
    gpio_config.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);

    // Install ISR service
    gpio_install_isr_service(0);
    // Hook ISR handler for specific GPIO pin
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, (void*) BUTTON_PIN);

    // Create button task
    xTaskCreate(&button_task, "button_task", 2048, NULL, 5, NULL);

    // Your application code continues here
    while(1) {
        // Your application logic
    }
}
*/



/*#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "freertos/semphr.h"

#define BUTTON_PIN GPIO_NUM_15 // GPIO pin for the button
#define LED_PIN GPIO_NUM_2     // GPIO pin for the LED
#define ESP_INTR_FLAG_DEFAULT 0

SemaphoreHandle_t xSemaphore = NULL;
bool led_status = false;


// interrupt service routine, called when the button is pressed
void IRAM_ATTR button_isr_handler(void* arg) {
	
    // notify the button task
	xSemaphoreGiveFromISR(xSemaphore, NULL);
}

// task that will react to button clicks
void button_task(void* arg) {
	
	// infinite loop
	for(;;) {
		
		// wait for the notification from the ISR
		if(xSemaphoreTake(xSemaphore,portMAX_DELAY) == pdTRUE) {
			printf("Button pressed!\n");
			led_status = !led_status;
			gpio_set_level(LED_PIN, led_status);
		}
	}
}

void app_main()
{
	
	// create the binary semaphore
	xSemaphore = xSemaphoreCreateBinary();
	
	// configure button and led pins as GPIO pins
	//gpio_pad_select_gpio(BUTTON_PIN);
	//gpio_pad_select_gpio(LED_PIN);
	
	// set the correct direction
	gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
	
	// enable interrupt on falling (1->0) edge for button pin
	gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);
	
	// start the task that will handle the button
	xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
	
	// install ISR service with default configuration
	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
	
	// attach the interrupt service routine
	gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);
}
*/
