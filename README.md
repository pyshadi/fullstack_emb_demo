# fullstack_emb_demo
# Temperature Sensor Data Logger
## Description
This project is designed to interface with a temperature sensor using the I2C protocol on STM32F407. It reads temperature data, formats the data into a JSON string, and transmits it over UART. 

### External Dependencies
```
extern "C" {
  #include "main.h"
  #include "i2c.h"
  #include "spi.h"
  #include "usart.h"
  #include "gpio.h"
}
```

### C++ Headers
Necessary C++ headers for the project, such for I2C device handling (I2CDevice.hpp), temperature sensing (TemperatureSensor.hpp), and the temperature sensor model (LM75.hpp).

```
#include "I2CDevice.hpp"
#include "TemperatureSensor.hpp"
#include "LM75.hpp"
```
### Main Function
The main() function is the entry point of the application. It follows these steps:

1. Calls HAL_Init() to initialize the HAL Library and then sets up system clock, GPIO, I2C, SPI, and USART peripherals with specific initialization functions. 
2. Creates an instance of TemperatureSensor, passing pointers to the I2C and UART handles (&hi2c1, &huart2). It then calls scanI2CDevices() to scan for connected I2C devices.
3. Main Loop: Continuously updates temperature readings, formats them into a JSON string, and sends this over UART. The loop then waits for 5 seconds (HAL_Delay(5000)) before repeating.

## Requirements
STM32F407 with I2C, SPI, USART, and GPIO capabilities.
HAL (Hardware Abstraction Layer) libraries.
I2C temperature sensor (LM75).
Development environment (STM32CubeIDE).
## License
MIT - Feel free to use and modify this code for your projects.