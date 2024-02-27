/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_SENSORS 4

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t sensorAddresses[MAX_SENSORS];
uint8_t numSensors = 0;
float temperatures[MAX_SENSORS];      // To store temperature readings
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
float readLM75Temperature(I2C_HandleTypeDef *hi2c, uint8_t sensorAddresses);
void scanI2CDevices(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart);
void updateTemperatures(I2C_HandleTypeDef *hi2c);
void sendAllTemperaturesOverUART(UART_HandleTypeDef *huart, float *temperatures);
void createTemperatureJSON(char* jsonOutput, size_t jsonSize, uint8_t *sensorAddresses, float *temperatures, uint8_t numSensors);
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
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  scanI2CDevices(&hi2c1, &huart2); // Scan once for devices at the start

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	    updateTemperatures(&hi2c1); // Update temperature readings from sensors

	    char jsonOutput[512]; // Allocate a buffer for the JSON output
	    createTemperatureJSON(jsonOutput, sizeof(jsonOutput), sensorAddresses, temperatures, numSensors); // Generate JSON string

	    HAL_UART_Transmit(&huart2, (uint8_t*)jsonOutput, strlen(jsonOutput), HAL_MAX_DELAY); // Transmit JSON string over UART

	    HAL_Delay(1000); // Delay between readings, adjust as needed
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void scanI2CDevices(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart) {
    char buffer[64];
    numSensors = 0; // Ensure numSensors is reset here if scanI2CDevices can be called multiple times
    for (uint8_t i = 0x48; i <= 0x4F; i++) {
        if (HAL_I2C_IsDeviceReady(hi2c, (i << 1), 3, 100) == HAL_OK && numSensors < MAX_SENSORS) {
            sensorAddresses[numSensors++] = i;
            sprintf(buffer, "Sensor found at address: 0x%X\r\n", i);
            HAL_UART_Transmit(huart, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
        }
    }
    // Initialize temperatures to 0.0
    for (int i = 0; i < MAX_SENSORS; i++) {
        temperatures[i] = 0.0;
    }
}


float readLM75Temperature(I2C_HandleTypeDef *hi2c, uint8_t sensorAddress) {
    uint8_t tempReg[2]; // Array to store the two temperature bytes
    float temperature = 0.0f; // Variable to store the final temperature

    if (HAL_I2C_Master_Receive(hi2c, (sensorAddress << 1), tempReg, 2, HAL_MAX_DELAY) == HAL_OK) {
        int16_t temp = (int16_t)(tempReg[0] << 8 | tempReg[1]) >> 5;
        temperature = temp * 0.125; // Corrected line
    }

    return temperature;
}




/* Function to read temperatures from all sensors and update the temperatures array */
void updateTemperatures(I2C_HandleTypeDef *hi2c) {
    for (int i = 0; i < MAX_SENSORS ; i++) {
        temperatures[i] = readLM75Temperature(hi2c, sensorAddresses[i]);
    }
}

/* Function to send consolidated temperature readings over UART */
void sendAllTemperaturesOverUART(UART_HandleTypeDef *huart, float *temperatures) {

	char tempString[32];
	for (int i = 0; i < numSensors ; i++) {
	    snprintf(tempString, sizeof(tempString), "Temperature: %.2f°C\r\n", temperatures[i]);
	    HAL_UART_Transmit(huart, (uint8_t*)tempString, strlen(tempString), HAL_MAX_DELAY);
}
}

void createTemperatureJSON(char* jsonOutput, size_t jsonSize, uint8_t *sensorAddresses, float *temperatures, uint8_t numSensors) {
    char buffer[256]; // Temporary buffer for individual JSON objects
    size_t usedSize = 0; // Track the used size to avoid buffer overflow

    // Start the JSON array
    usedSize += snprintf(jsonOutput + usedSize, jsonSize - usedSize, "[");

    for (int i = 0; i < numSensors; ++i) {
        // Format the current sensor's data into a JSON object
        int len = snprintf(buffer, sizeof(buffer), "{\"address\":\"0x%X\",\"temperature\":%.2f}", sensorAddresses[i], temperatures[i]);

        // Check if there's enough space left in the output buffer
        if (usedSize + len < jsonSize - 1) { // Leave space for null-terminator and potential comma
            // Append this sensor's JSON object to the output
            strncpy(jsonOutput + usedSize, buffer, len);
            usedSize += len;
            // Add a comma if this is not the last sensor
            if (i < numSensors - 1) {
                jsonOutput[usedSize++] = ',';
            }
        } else {
            // Not enough space to add more data
            break;
        }
    }

    // Close the JSON array
    if (usedSize < jsonSize - 1) {
        jsonOutput[usedSize++] = ']';
    } else {
        // Ensure the string is properly closed in case of overflow
        jsonOutput[jsonSize - 1] = ']';
        usedSize = jsonSize; // Adjust usedSize to reflect the forced closure
    }
    jsonOutput[usedSize] = '\0'; // Ensure null-termination
}


/* USER CODE END 4 */


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

#ifdef  USE_FULL_ASSERT
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
