#ifndef TEMPERATURESENSOR_HPP
#define TEMPERATURESENSOR_HPP

#include "main.h" // Ensure this includes or forwards declarations for HAL types used
#include <cstddef> // For size_t

class TemperatureSensor {
public:
    TemperatureSensor(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart);
    void scanI2CDevices();
    void updateTemperatures();
    void sendAllTemperaturesOverUART();
    void createTemperatureJSON(char* jsonOutput, size_t jsonSize);

private:
    I2C_HandleTypeDef *hi2c;
    UART_HandleTypeDef *huart;
    static const uint8_t MAX_SENSORS = 4;
    uint8_t sensorAddresses[MAX_SENSORS];
    uint8_t numSensors = 0;
    float temperatures[MAX_SENSORS]; // To store temperature readings

    float readLM75Temperature(uint8_t sensorAddress);
};

#endif // TEMPERATURESENSOR_HPP
