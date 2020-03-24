/*
 * LED.cc
 *
 */

#include "LED.h"

Led::Led(GPIO_TypeDef *port, uint32_t pin) : port(port), pin(pin) {	//sets up the LED
	GPIO_InitTypeDef GPIOInitStruct;

	GPIOInitStruct.Pin = pin;
	GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOInitStruct.Speed = GPIO_SPEED_MEDIUM;
	GPIOInitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(port, &GPIOInitStruct);
}


void Led::setHigh(){
	//sets the LED high
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}


void Led::setLow(){
	//sets the LED low
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

Led led0(LED0_PORT, LED0_PIN);
Led led1(LED1_PORT, LED1_PIN);
Led led2(LED2_PORT, LED2_PIN);
Led led3(LED3_PORT, LED3_PIN);