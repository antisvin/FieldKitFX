/*
 * LED.cc
 *
 */

#include "hardware/led.h"

namespace fieldkitfx {

Led led0, led1, led2, led3;

// Led::Led(GPIO_TypeDef *port, uint32_t pin) : port(port), pin(pin) {}	//sets up the LED

void Led::init(GPIO_TypeDef* port_, uint32_t pin_) {
    pin = pin_;
    port = port_;

    GPIOInitStruct.Pin = pin;
    GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOInitStruct.Speed = GPIO_SPEED_MEDIUM;
    GPIOInitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(port, &GPIOInitStruct);
}

void Led::setHigh() {
    // sets the LED high
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

void Led::setLow() {
    // sets the LED low
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}
}
