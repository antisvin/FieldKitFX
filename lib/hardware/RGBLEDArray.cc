/*
 * RGBLEDArray.c
 *
 */

#include "RGBLEDArray.h"

constexpr uint8_t LEDBaseAddrLUT[RGBLEDARRAY_SIZE] = {0x0e, 0x12, 0x1f, 0x1b, 0x17, 0x0a, 0x0e, 0x12, 0x17, 0x1b, 0x1f};



void RgbLedArray::init(LedDriver *driver1, LedDriver *driver2) {
	for(uint8_t i = 0; i < RGBLEDARRAY_SIZE; i++) {
		/*
		 * Init the RGBLED struct
		 */
		/*
		if(i < 5) {	//LEDs 1 to 6 are attached to the first LED driver (U54)
			array[i] = RgbLed(driver1, LEDBaseAddrLUT[i]);
		}
		else {
			array[i] = RgbLed(driver2, LEDBaseAddrLUT[i]);
		}
		*/
		array[i]= RgbLed {
			i < 5 ? driver1 : driver2, LEDBaseAddrLUT[i],
			CV_RGB_LED_INTENSITY, 0, 0, 0};
		array[i].setIntensity();
		array[i].setColor();
	}
}

void RgbLedArray::setColor(uint8_t red, uint8_t green, uint8_t blue) {
	for(uint8_t i = 0; i < RGBLEDARRAY_SIZE; i++) {
		setColor(i, red, green, blue);
	}
}

void RgbLedArray::setColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue) {
	array[index].red = red;
	array[index].green = green;
	array[index].blue = blue;
	array[index].setColor();
}

void RgbLedArray::setIntensity(uint8_t intensity) {
	for(uint8_t i = 0; i < RGBLEDARRAY_SIZE; i++) {
		setIntensity(i, intensity);
	}
}

void RgbLedArray::setIntensity(uint8_t index, uint8_t intensity) {
	array[index].intensity = intensity;
	array[index].setIntensity();
}
