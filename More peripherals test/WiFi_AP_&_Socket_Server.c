/*Open WiFi in mobile/PC
Mobile/PC Connect to SSID and Password
Server Socket created and send/receive the data
*/

#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "lwip/sockets.h"
#include "nvs_flash.h"
#include "lwip/inet.h"
#include "esp_netif.h"

#define TAG "AP_MODE"

#define WIFI_SSID       "ESP32_AP"
#define WIFI_PASS       "password"
#define MAX_STA_CONN    4



#define LISTEN_BACKLOG  5
#define MAX_CONN        5
#define BUF_SIZE        1024

#define SERVER_IP       "192.168.4.2"  // IP address of the server
#define PORT     12345       

#define IP_ADDR         "192.168.1.1"
#define GATEWAY_ADDR    "192.168.1.1"
#define NETMASK         "255.255.255.0"


static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data) {
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "Station connected, %02X:%02X:%02X:%02X:%02X:%02X AID=%d",
         event->mac[0], event->mac[1], event->mac[2], event->mac[3] , event->mac[4], event->mac[5], event->aid);
      
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "Station connected, %02X:%02X:%02X:%02X:%02X:%02X AID=%d",
         event->mac[0], event->mac[1], event->mac[2], event->mac[3] , event->mac[4], event->mac[5], event->aid);
    } 
}


void wifi_init_softap() {

    esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();
        /* STATIC IP BEGIN*/
    assert(ap_netif);
        /* STATIC IP ENDS*/
    
    //esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL);

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .ssid_len = strlen(WIFI_SSID),
            .password = WIFI_PASS,
            .max_connection = MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };


    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    
    /* STATIC IP BEGIN*/
    /*ESP_ERROR_CHECK(esp_netif_dhcps_stop(ap_netif));
 
    esp_netif_ip_info_t ip_info;
 
    IP4_ADDR(&ip_info.ip, 10, 34, 46, 5);
    IP4_ADDR(&ip_info.gw, 10, 34, 46, 1);
    IP4_ADDR(&ip_info.netmask, 255, 255, 255, 0);
 
    ESP_ERROR_CHECK(esp_netif_set_ip_info(ap_netif, &ip_info));
    esp_netif_dhcps_start(ap_netif);*/
    /* STATIC IP ENDS*/
    
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "SoftAP started");
}

void send_data_task(void *pvParameters) {
    char rx_buffer[BUF_SIZE];
    char addr_str[INET_ADDRSTRLEN];
    int addr_family;
    int ip_protocol;

    while (1) {
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);

        int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
        if (listen_sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created");

        int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket bound, port %d", PORT);

        err = listen(listen_sock, LISTEN_BACKLOG);
        if (err != 0) {
            ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket listening");

        while (1) {
            struct sockaddr_in source_addr;
            uint addr_len = sizeof(source_addr);
            int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
            if (sock < 0) {
                ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
                break;
            }
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
            ESP_LOGI(TAG, "Socket accepted ip address: %s", addr_str);

            int len;
            while ((len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0)) > 0) {
                rx_buffer[len] = 0;
                ESP_LOGI(TAG, "Received %d bytes: %s", len, rx_buffer);
                printf("Received %d bytes: %s", len, rx_buffer);
                send(sock, rx_buffer, len, 0);
            }
            close(sock);
        }

        close(listen_sock);
    }
    vTaskDelete(NULL);
}



void app_main() {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_softap();
    xTaskCreate(send_data_task, "send_data_task", 4096, NULL, 5, NULL);
}

