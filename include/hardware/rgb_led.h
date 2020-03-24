/*
 * RGBLED.h
 *
 */

#ifndef RGBLED_H_
#define RGBLED_H_

/*
 * Include(s)
 */
#include "hardware/led_driver.h"
#include "utils/utils.h"

namespace fieldkitfx {

class RgbLed {
private:
    DISALLOW_COPY_AND_ASSIGN(RgbLed);

public:
    // current flowing thru the RGB LED
    LedDriver* driver;
    uint8_t baseAddress;
    uint8_t intensity;

    // Color definition
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    // RgbLed(LedDriver *driver, uint8_t baseAddress);
    RgbLed() = default;
    void setColor();
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setIntensity();
    void setIntensity(uint8_t intensity);
};
}
#endif /* RGBLED_H_ */
