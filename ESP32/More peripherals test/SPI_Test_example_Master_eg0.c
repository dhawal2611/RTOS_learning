// Master as a receiver for SPI communication

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/igmp.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "soc/rtc_periph.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "esp_spi_flash.h"

#include "driver/gpio.h"
#include "esp_intr_alloc.h"

// Pins in use
#define GPIO_MOSI 12
#define GPIO_MISO 13
#define GPIO_SCLK 15
#define GPIO_CS 14

// Main application
void app_main(void)
{
    spi_device_handle_t handle;

    // Configuration for the SPI bus
    spi_bus_config_t buscfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1};

    // Configuration for the SPI device on the other side of the bus
    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .clock_speed_hz = 5000000,
        .duty_cycle_pos = 128, // 50% duty cycle
        .mode = 0,
        .spics_io_num = GPIO_CS,
        .cs_ena_posttrans = 3, // Keep the CS low 3 cycles after transaction
        .queue_size = 3};

    spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    spi_bus_add_device(HSPI_HOST, &devcfg, &handle);

    /*char recvbuf[129] = "";
    memset(recvbuf, 0, 33);
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    printf("Master input:\n");
    while (1)
    {
        t.length = 128 * 8;
        t.rx_buffer = recvbuf;
        spi_device_transmit(handle, &t);
        printf("Received: %s\n", recvbuf);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }*/
    char sendbuf[128] = {0};

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    //spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    //spi_bus_add_device(HSPI_HOST, &devcfg, &handle);

    printf("Master output:\n");
    while (1)
    {
        snprintf(sendbuf, sizeof(sendbuf), "A");
        t.length = sizeof(sendbuf) * 8;
        t.tx_buffer = sendbuf;
        spi_device_transmit(handle, &t);
        printf("Transmitted: %s\n", sendbuf);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}


/*#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include <string.h>

#define PIN_MISO 19
#define PIN_MOSI 23
#define PIN_CLK  18
#define PIN_CS   5

//#define PIN_MISO 25
//#define PIN_MOSI 23
//#define PIN_CLK  19
//#define PIN_CS   22

spi_device_handle_t spi;

void spi_master_init() {
    esp_err_t ret;

    // Configuration for the SPI bus
    spi_bus_config_t bus_cfg = {
        .miso_io_num = PIN_MISO,
        .mosi_io_num = PIN_MOSI,
        .sclk_io_num = PIN_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0,
    };

    // Initialize the SPI bus
    ret = spi_bus_initialize(VSPI_HOST, &bus_cfg, 0);
    assert(ret == ESP_OK);

    // Configuration for the SPI device
    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = 10 * 1000 * 1000,           // Clock frequency
        .mode = 0,                                      // SPI mode (0 for CPOL=0, CPHA=0)
        .spics_io_num = PIN_CS,                         // Chip select pin
        .queue_size = 7,                                // Queue size
    };
    
    // Attach the SPI device to the SPI bus
    ret = spi_bus_add_device(VSPI_HOST, &dev_cfg, &spi);
    assert(ret == ESP_OK);
}

void spi_master_write_data(uint8_t *data, size_t len) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));  // Zero out the transaction object

    t.length = 8 * len;         // Length of data to be sent, in bits
    t.tx_buffer = data;         // Data buffer
    t.user = (void *)0;         // Dummy value

    // Perform SPI transaction
    esp_err_t ret = spi_device_transmit(spi, &t);
    assert(ret == ESP_OK);
}

void app_main(void) {
    spi_master_init();

    // Example data to be sent
    uint8_t data[4] = {0xAA, 0xBB, 0xCC, 0xDD};

    // Write data to the SPI slave device
    //spi_master_write_data(data, sizeof(data));

	//uint8_t TxData[2] = {0x01, 0x02};

	spi_transaction_t SpiTransaction;

	//----- RECEIVE BYTES -----
	uint8_t RxData[] = {0x04};
	memset(&SpiTransaction, 0, sizeof(SpiTransaction));			//Zero out the transaction ready to use
	SpiTransaction.length = 3 * 8;								//Transaction length is in bits
	SpiTransaction.rx_buffer = &RxData[0];
	//Do it
	spi_device_polling_transmit(spi, &SpiTransaction);		//Waits until the transfer is complete

	//Release bus
	//spi_device_release_bus(spi);

    spi_master_write_data(data, sizeof(data));





}
*/
