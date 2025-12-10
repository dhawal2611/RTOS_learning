/*Create Hotspot in mobile/PC
ESP Connects to given SSID and Password
ESP connects to it.
Client Socket created and send/receive the data
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "nvs_flash.h"
#include "lwip/inet.h"

#define PING_HOST "google.com"
#define PING_TIMEOUT_MS 1000


static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;

#define WIFI_SSID       "Realme 2 Pro"
#define WIFI_PASSWORD   "12345678"
#define SERVER_IP       "192.168.43.1"  // IP address of the server
#define SERVER_PORT     12345            // Port number of the server

static const char *TAG = "DUL";

/*static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
    if (event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "Connection to the AP failed");
    }
}

static void ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP address: %s", ip4addr_ntoa(&event->ip_info.ip));
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
    }
}
*/

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "WiFi started");
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "WiFi disconnected");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}

void wifi_init_sta() {
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    //esp_event_handler_instance_t instance_any_id;
    //esp_event_handler_instance_t instance_got_ip;
    

    //esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id);
    //esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler, NULL, &instance_got_ip);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
}

void send_data_task(void *pvParameters) {
    int sockfd;
    struct sockaddr_in server_addr;

    while (1) {
    sockfd = 0;
        // Create socket
        sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        if (sockfd < 0) {
            ESP_LOGE(TAG, "Failed to create socket");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            //continue;
	goto end;
        }

        // Set up the server address
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
        server_addr.sin_port = htons(SERVER_PORT);
	if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
            close(sockfd);
	goto end;
	}

        // Connect to the server
        if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            ESP_LOGE(TAG, "connect API failed");
            perror("Connection Check->");
            close(sockfd);
            vTaskDelay(5000 / portTICK_PERIOD_MS);
	goto end;
            //continue;
        }

	while (1) {
        // Send data
        const char *data = "Hello, Server!";
        if (write(sockfd, data, strlen(data)) < 0) {
            ESP_LOGE(TAG, "Failed to send data");
            close(sockfd);
            goto end;
        }
        ESP_LOGI(TAG, "data sent and Waiting to receive data");
        // Wait for ping response
    //struct timeval timeout = {PING_TIMEOUT_MS / 1000, (PING_TIMEOUT_MS % 1000) * 1000};
    //setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        /*char recv_packet[64];
        read(sockfd, recv_packet, 64);
        ESP_LOGI(TAG, "Received data is %s", recv_packet);*/
        int valread;
        char buffer[1024] = { 0 };
        valread = read(sockfd, buffer,
                   1024 - 1); // subtract 1 for the null
                              // terminator at the end
    printf("%s\n", buffer);
     ESP_LOGI(TAG, "Received data is %s", buffer);
        /*int received = recv(sockfd, recv_packet, sizeof(recv_packet), 0);
    if (received >= 0) {
        ESP_LOGI(TAG, "Received data is %s", recv_packet);
    } else {
        ESP_LOGE(TAG, "No Data recerived Timeout occured");
    }*/
	 vTaskDelay(5000 / portTICK_PERIOD_MS);
        
        
        }

        // Close the socket
        close(sockfd);
end:
        vTaskDelay(10000 / portTICK_PERIOD_MS); // Send data every 5 seconds
    }
}

void app_main() {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    //ESP_ERROR_CHECK(wifi_init_sta());
    wifi_init_sta();

    xTaskCreate(send_data_task, "send_data_task", 4096, NULL, 5, NULL);
}
