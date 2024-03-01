#include "LM75.hpp"
#include <cstdint> // for uint8_t, etc.

// Constructor
LM75::LM75(I2C_HandleTypeDef* handle, uint16_t deviceAddress) : I2CDevice(handle, deviceAddress) {}

// Destructor



// Implement the TemperatureSensor interface
float LM75::readTemperature() {
    uint8_t tempReg = 0x00; // The LM75 temperature register
    uint8_t data[2] = {0, 0};
    if (read(tempReg, data[0]) && read(tempReg + 1, data[1])) {
        // Convert the data to temperature
        int16_t temp = (data[0] << 8) | data[1];
        return temp * 0.125; // LM75 temperature resolution
    }
    return -999; // Error value
}
