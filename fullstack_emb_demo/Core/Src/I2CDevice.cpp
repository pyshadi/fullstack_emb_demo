// I2CDevice.cpp
#include "I2CDevice.hpp"

I2CDevice::I2CDevice(I2C_HandleTypeDef* handle, uint16_t deviceAddress)
    : handle(handle), deviceAddress(deviceAddress) {
    // Initialisation code specific to the I2C device could go here
}

I2CDevice::~I2CDevice() {
    // Cleanup code could go here
}
