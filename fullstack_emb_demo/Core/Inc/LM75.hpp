// LM75.hpp
#ifndef LM75_HPP
#define LM75_HPP

#include "I2CDevice.hpp"

class LM75 : public I2CDevice {
public:
    LM75(I2C_HandleTypeDef* handle, uint16_t deviceAddress);
    virtual ~LM75() {}

    bool write(uint8_t reg, uint8_t data) override;
    bool read(uint8_t reg, uint8_t& data) override;
    float readTemperature(); // Example additional method specific to LM75
};

#endif // LM75_HPP
