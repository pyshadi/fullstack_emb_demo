extern "C" {
  #include "main.h"
  #include "i2c.h"
  #include "spi.h"
  #include "usart.h"
  #include "gpio.h"
}

#include "I2CDevice.hpp"
#include "TemperatureSensor.hpp"
#include <vector>
#include <cstdio>
#include <cstring> // Add this line to include strlen

#include "LM75.hpp"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);


int main() {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_SPI1_Init();
    MX_USART2_UART_Init();


	char hexStr[11]; // Each byte to 2 hex characters + space or end ('\0')


    TemperatureSensor sensor(&hi2c1, &huart2);
    sensor.scanI2CDevices();

    while (1) {

        sensor.updateTemperatures();
        char jsonOutput[512];
        sensor.createTemperatureJSON(jsonOutput, sizeof(jsonOutput));
        HAL_UART_Transmit(&huart2, (uint8_t*)jsonOutput, strlen(jsonOutput), HAL_MAX_DELAY);
        HAL_Delay(5000);
    }

}


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

void Error_Handler(void) {
    // User can add his own implementation to report the HAL error return state
    __disable_irq();
    while (1) {
    }
}
