#include "TemperatureSensor.hpp"
#include <stdio.h> // For sprintf and snprintf
#include <string.h> // For strlen

TemperatureSensor::TemperatureSensor(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart)
    : hi2c(hi2c), huart(huart), numSensors(0) {
    // Constructor can also initialize the sensor if required
}

void TemperatureSensor::scanI2CDevices() {
    char buffer[64];
    numSensors = 0;
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

void TemperatureSensor::updateTemperatures() {
    for (int i = 0; i < numSensors; i++) {
        temperatures[i] = readLM75Temperature(sensorAddresses[i]);
    }
}

void TemperatureSensor::sendAllTemperaturesOverUART() {
    char tempString[32];
    for (int i = 0; i < numSensors; i++) {
        snprintf(tempString, sizeof(tempString), "Temperature: %.2f°C\r\n", temperatures[i]);
        HAL_UART_Transmit(huart, (uint8_t*)tempString, strlen(tempString), HAL_MAX_DELAY);
    }
}

void TemperatureSensor::createTemperatureJSON(char* jsonOutput, size_t jsonSize) {
    char buffer[256];
    size_t usedSize = 0;
    usedSize += snprintf(jsonOutput + usedSize, jsonSize - usedSize, "[");

    for (int i = 0; i < numSensors; ++i) {
        int len = snprintf(buffer, sizeof(buffer), "{\"address\":\"0x%X\",\"temperature\":%.2f}", sensorAddresses[i], temperatures[i]);
        if (usedSize + len < jsonSize - 1) {
            strncpy(jsonOutput + usedSize, buffer, len);
            usedSize += len;
            if (i < numSensors - 1) {
                jsonOutput[usedSize++] = ',';
            }
        } else {
            break;
        }
    }
    jsonOutput[usedSize++] = ']';
    jsonOutput[usedSize] = '\0';
}

float TemperatureSensor::readLM75Temperature(uint8_t sensorAddress) {
    uint8_t tempReg[2];
    float temperature = 0.0f;

    if (HAL_I2C_Master_Receive(hi2c, (sensorAddress << 1), tempReg, 2, HAL_MAX_DELAY) == HAL_OK) {
        int16_t temp = (int16_t)(tempReg[0] << 8 | tempReg[1]) >> 5;
        temperature = temp * 0.125;
    }
    return temperature;
}
