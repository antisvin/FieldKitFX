/*
 * RGBLED.c
 *
 */
#include "RGBLED.h"

//RgbLed::RgbLed(LedDriver *driver, uint8_t baseAddress) : driver(driver), baseAddress(baseAddress) {}

void RgbLed::setColor() {
	driver->write3Val(baseAddress, red, green, blue);
}


void RgbLed::setIntensity() {
	driver->write3Val(baseAddress + 24, intensity, intensity, intensity);
}
