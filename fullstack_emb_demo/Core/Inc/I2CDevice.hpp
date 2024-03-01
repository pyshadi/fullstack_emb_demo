// I2CDevice.hpp
#ifndef I2CDEVICE_HPP
#define I2CDEVICE_HPP

#include "stm32f4xx_hal.h" // Adjust this include path as necessary
#include <cstdint>

class I2CDevice {
public:
    I2CDevice(I2C_HandleTypeDef* handle, uint16_t deviceAddress);
    virtual ~I2CDevice();

    virtual bool write(uint8_t reg, uint8_t data) = 0;
    virtual bool read(uint8_t reg, uint8_t& data) = 0;

protected:
    I2C_HandleTypeDef* handle;
    uint16_t deviceAddress;
};

#endif // I2CDEVICE_HPP
