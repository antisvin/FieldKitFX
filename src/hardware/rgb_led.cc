/*
 * RGBLED.c
 *
 */
#include "hardware/rgb_led.h"

namespace fieldkitfx {
// RgbLed::RgbLed(LedDriver *driver, uint8_t baseAddress) : driver(driver), baseAddress(baseAddress) {}

void RgbLed::setColor() {
    driver->write3Val(baseAddress, red, green, blue);
}

void RgbLed::setColor(Color color) {
    setColor(colors[color][0], colors[color][1], colors[color][2]);
}

void RgbLed::setColor(uint8_t red_, uint8_t green_, uint8_t blue_) {
    red = red_;
    green = green_;
    blue = blue_;
    setColor();
}

void RgbLed::setIntensity() {
    driver->write3Val(baseAddress + 24, intensity, intensity, intensity);
}

void RgbLed::setIntensity(uint8_t intensity_) {
    intensity = intensity_;
    setIntensity();
}

}
