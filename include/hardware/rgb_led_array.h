/*
 * RGBLEDArray.h
 *
 */

#ifndef RGBLEDARRAY_H_
#define RGBLEDARRAY_H_

/*
 * Includes
 */
#include "hardware/rgb_led.h"
#include "utils/utils.h"

/*
 * Defines of all the important RGB related addresses and stuff
 */
#define U6_I2C_ADDRESS 0x0d
#define U7_I2C_ADDRESS 0x33

#define CV_RGB_LED_INTENSITY 0x04

/*
 * LED Array related defines
 */
#define RGBLEDARRAY_SIZE 11

namespace fieldkitfx {

class RgbLedArray {
private:
    DISALLOW_COPY_AND_ASSIGN(RgbLedArray);

public:
    RgbLed array[RGBLEDARRAY_SIZE];

    RgbLedArray() = default;
    void init(LedDriver* driver1, LedDriver* driver2);
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
    void setIntensity(uint8_t intensity);
    void setIntensity(uint8_t index, uint8_t intensity);
};

}
#endif /* RGBLEDARRAY_H_ */
