/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
static void I2CTask_handler(void *pdData);

static void I2CDelay(void);
static void vStartI2C();
static void vStopI2C();
static uint8_t WriteI2C(uint8_t byte);
static uint8_t ReadI2C1B(uint8_t ack);
static void SetI2CSDAGPIO();
static void ClearI2CSDAGPIO();
static void SetI2CSCLGPIO();
static void ClearI2CSCLGPIO();
static void ConfigI2CSDAGPIOOutput();
static void ConfigI2CSDAGPIOInput();
static void vStopI2C();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	TaskHandle_t Task_Handle;
	BaseType_t status_i2c;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  status_i2c = xTaskCreate(I2CTask_handler, "I2CTask", 200, NULL, 1, &Task_Handle);

  configASSERT(status_i2c  == pdPASS);

  vTaskStartScheduler();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : I2S3_WS_Pin */
  GPIO_InitStruct.Pin = I2S3_WS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(I2S3_WS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_SCK_Pin SPI1_MISO_Pin SPI1_MOSI_Pin */
  GPIO_InitStruct.Pin = SPI1_SCK_Pin|SPI1_MISO_Pin|SPI1_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : I2S3_MCK_Pin I2S3_SCK_Pin I2S3_SD_Pin */
  GPIO_InitStruct.Pin = I2S3_MCK_Pin|I2S3_SCK_Pin|I2S3_SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : VBUS_FS_Pin */
  GPIO_InitStruct.Pin = VBUS_FS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBUS_FS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OTG_FS_ID_Pin OTG_FS_DM_Pin OTG_FS_DP_Pin */
  GPIO_InitStruct.Pin = OTG_FS_ID_Pin|OTG_FS_DM_Pin|OTG_FS_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB6 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static void ConfigI2CSDAGPIOOutput() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/*Configure GPIO pins : PB6 PB9 */
	  GPIO_InitStruct.Pin = I2C_SDA_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	  HAL_GPIO_TogglePin(GPIOD, LED_RED_PIN); // Red led gets toggle when SDA pin changes its state
	  I2CDelay();
}

static void ConfigI2CSDAGPIOInput() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/*Configure GPIO pins : PB6 PB9 */
	  GPIO_InitStruct.Pin = I2C_SDA_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	  HAL_GPIO_TogglePin(GPIOD, LED_RED_PIN); // Red led gets toggle when SDA pin changes its state
	  I2CDelay();
}

static void SetI2CSDAGPIO() {
	HAL_GPIO_WritePin(GPIOB, I2C_SDA_PIN, GPIO_PIN_SET);
}

static void ClearI2CSDAGPIO() {
	HAL_GPIO_WritePin(GPIOB, I2C_SDA_PIN, GPIO_PIN_RESET);
}

static void SetI2CSCLGPIO() {
	HAL_GPIO_WritePin(GPIOB, I2C_CLK_PIN, GPIO_PIN_SET);
}

static void ClearI2CSCLGPIO() {
	HAL_GPIO_WritePin(GPIOB, I2C_CLK_PIN, GPIO_PIN_RESET);
}

static uint8_t ReadI2C1B(uint8_t ack) {
	int i = 0;
	uint8_t byte = 0;

	ConfigI2CSDAGPIOOutput();
	I2CDelay();
	SetI2CSDAGPIO();

	ConfigI2CSDAGPIOInput();
//	I2CDelay();

	for(;i<8;i++) {
		byte<<=1;
		SetI2CSCLGPIO();
		I2CDelay();
		if(HAL_GPIO_ReadPin(GPIOB, I2C_SDA_PIN)) {
			byte|=1;
		}

		ClearI2CSCLGPIO();
		I2CDelay();


	}

	ConfigI2CSDAGPIOOutput();
//	I2CDelay();

	if(ack) {
		ClearI2CSDAGPIO();
	} else {
		SetI2CSDAGPIO();
	}
	I2CDelay();

	SetI2CSCLGPIO();
//	HAL_Delay(1);
	I2CDelay();
	ClearI2CSCLGPIO();

	SetI2CSDAGPIO();

	return byte;
}

static uint8_t WriteI2C(uint8_t byte){
	int i = 0;

	ConfigI2CSDAGPIOOutput();
//	I2CDelay();
	for(i=7;i>=0;i--) {
		if((byte>>i) & 1) {
			HAL_GPIO_WritePin(GPIOB, I2C_SDA_PIN, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOB, I2C_SDA_PIN, GPIO_PIN_RESET);
		}
		I2CDelay();

		SetI2CSCLGPIO();
		I2CDelay();
		ClearI2CSCLGPIO();
//		I2CDelay();

	}


	SetI2CSDAGPIO();
	I2CDelay();

	SetI2CSCLGPIO();
	I2CDelay();

	ConfigI2CSDAGPIOInput();
//	I2CDelay();

	int ack = HAL_GPIO_ReadPin(GPIOB, I2C_SDA_PIN);
//	ConfigI2CSDAGPIOOutput();
//	I2CDelay();

	ClearI2CSCLGPIO();
	I2CDelay();

	return (ack == 0); //1 - ACK; 0 - NACK
}



static void I2CDelay(void) {
    for (volatile int i = 0; i < 100; i++);
}


static void vStartI2C() {
	ConfigI2CSDAGPIOOutput();

	SetI2CSDAGPIO();
	SetI2CSCLGPIO();
	I2CDelay();

	ClearI2CSDAGPIO();
	I2CDelay();
	ClearI2CSCLGPIO();
}

static void vStopI2C() {

	ConfigI2CSDAGPIOOutput();

//	SetI2CSCLGPIO();

	ClearI2CSDAGPIO();
	I2CDelay();

	SetI2CSCLGPIO();
	I2CDelay();

	SetI2CSDAGPIO();
	I2CDelay();

}

void WriteBytesOnI2C(uint8_t ADDRESS, uint8_t *data, uint8_t NoOfBytes, uint8_t *Status)
{
	uint8_t i = 0;
	vStartI2C();
//	I2CDelay();
	if(WriteI2C(ADDRESS << 1) != 0) // Write mode config
	{
		printf("Write successfully\n");
		//HAL_GPIO_WritePin(GPIOD, LED_ORANGE_PIN, GPIO_PIN_SET);

	} else {
		printf("Write not successfully - Failed\n");
		//HAL_GPIO_WritePin(GPIOD, LED_GREEN_PIN, GPIO_PIN_SET);
		*Status = 1;
		vStopI2C();
//		I2CDelay();
		return;
	}

	while(NoOfBytes > 0)
	{
		if(WriteI2C(data[i++]) != 0) {
			printf("Write successfully\n");
		//	HAL_GPIO_WritePin(GPIOD, LED_ORANGE_PIN, GPIO_PIN_SET);
		} else {
			printf("Write not successfully - Failed\n");
		//	HAL_GPIO_WritePin(GPIOD, LED_GREEN_PIN, GPIO_PIN_SET);
			*Status = 1;
			vStopI2C();
//			I2CDelay();
			return;
		}

		NoOfBytes--;
	}
	vStopI2C();
//	I2CDelay();
}

#define SLAVE_ADDRESS 0x38

static void I2CTask_handler(void *Data) {

	uint8_t rec_data[7] = {0};
	int i=0;

	printf("Hello from I2C task\n");

//	uint8_t data[] = {0xAC, 0x33, 0x00};
//	uint8_t Status = 0;
//	WriteBytesOnI2C(SLAVE_ADDRESS, data, 3, &Status);
//	if(Status == 0)
//	{
//		printf("Write successfully\n");
//		HAL_GPIO_WritePin(GPIOD, LED_ORANGE_PIN, GPIO_PIN_SET);
//	}
//	HAL_Delay(80);


//	uint8_t data1[] = {0xBE, 0x08, 0x00};
//	uint8_t Status1 = 0;
//	WriteBytesOnI2C(SLAVE_ADDRESS, data1, 3, &Status1);
//	if(Status1 == 0)
//	{
//		printf("-----------------------------------Write successfully\n");
//		HAL_GPIO_WritePin(GPIOD, LED_ORANGE_PIN, GPIO_PIN_SET);
//	}
//	HAL_Delay(80);

	uint8_t data[] = {0xAC, 0x33, 0x00}; // Writing Measurment Command
	uint8_t Status = 0;
	WriteBytesOnI2C(SLAVE_ADDRESS, data, 3, &Status);
	if(Status == 0)
	{
		printf("-----------------------------------Write successfully\n");
		HAL_GPIO_WritePin(GPIOD, LED_ORANGE_PIN, GPIO_PIN_SET);
	}
	HAL_Delay(80);

	while(1) {
		HAL_Delay(80);

		//Read Data
		vStartI2C();
		printf("Write address: %x\n", ((0x38 << 1) | 1)); // Read mode config

		if(WriteI2C((0x38 << 1) | 1) != 0) {
//			printf("Write successfully\n");
			HAL_GPIO_WritePin(GPIOD, LED_ORANGE_PIN, GPIO_PIN_SET);
		} else {
//			printf("Read ACK  not successfully - Failed\n");
			HAL_GPIO_WritePin(GPIOD, LED_GREEN_PIN, GPIO_PIN_SET);
		}

//		HAL_Delay(80);
		for(i=0; i<5; i++)
		{
			rec_data[i] = ReadI2C1B(1);
//			if((rec_data[0]>>7) & 1) {
//				HAL_Delay(80);
//				i = -1;
//				continue;
//			}
//			printf("-------Recv data[%d]: %d\n", i, rec_data[i]);
//			HAL_Delay(80);
		}
//		printf("Reading index out of for loop: %d\n", i);
		rec_data[i] = ReadI2C1B(0);
//		printf("*********Recv data[%d]: %d\n", i, rec_data[i]);
//		I2CDelay();

		vStopI2C();

		printf("Recv data\n");
		for(i=0; i<7; i++) {
			printf("%x\t", rec_data[i]);
		}printf("\n");

		// Convert Humidity (20-bit)
		uint32_t raw_hum = (rec_data[1] << 12) | (rec_data[2] << 4) | (rec_data[3] >> 4);
		// Convert Temperature (20-bit)
		uint32_t raw_temp =  (((rec_data[3] & 0x0F) << 16) | (rec_data[4] << 8) | rec_data[5]);

		printf("Raw HumidityValue is : %ld \n", raw_hum);
		printf("Raw Temperature Value is : %ld \n", raw_temp);

		float humidity = (float)(raw_hum / 1048576.0f) * 100.0f;
		float temperature = (float)(raw_temp / 1048576.0f) * 200.0f - 50.0f;

		printf("HumidityValue is : %f %%\n", humidity);
		printf("Temperature Value is : %f C\n", temperature);

		vTaskDelay(pdMS_TO_TICKS(5000));
		printf("********************************************\n");

	}

	vTaskDelete(NULL);
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
