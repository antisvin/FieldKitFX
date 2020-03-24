/*
 * LEDDriver.h
 *
 */

#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_

/*
 * Include(s)
 */
#include "hardware/i2c.h"
#include "hardware/pca9956b.h"
#include "utils/utils.h"

namespace fieldkitfx {

class LedDriver {
private:
    I2C_HandleTypeDef* i2cController;
    uint8_t i2cAddr;
    DISALLOW_COPY_AND_ASSIGN(LedDriver);

public:
    LedDriver() = default;
    void init(uint8_t i2c_addr, I2C_HandleTypeDef* i2c_controller);
    void writeReg(uint8_t regAddr, uint8_t data, uint8_t autoInc);
    void write3Val(uint8_t startAddr, uint8_t data1, uint8_t data2, uint8_t data3);
    void write1Val(uint8_t addr, uint8_t data);
};
}
#endif /* LEDDRIVER_H_ */
