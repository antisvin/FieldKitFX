/*
 * Gate.c
 *
 */

#include "Gate.h"


void Gate::init() {
	GPIO_InitTypeDef GPIOStruct;

	GPIOStruct.Pin = GATE_IN_PIN;
	GPIOStruct.Pull = GPIO_NOPULL;
	GPIOStruct.Mode = GPIO_MODE_INPUT;

	HAL_GPIO_Init(GATE_IN_PORT, &GPIOStruct);
}

void Gate::update() {
	previous_state = current_state;
	current_state = HAL_GPIO_ReadPin(GATE_IN_PORT, GATE_IN_PIN);
}

bool Gate::isHigh() {
	return HAL_GPIO_ReadPin(GATE_IN_PORT, GATE_IN_PIN);
}

bool Gate::isLow(){
	return (!(HAL_GPIO_ReadPin(GATE_IN_PORT, GATE_IN_PIN)) & 1);
}

bool Gate::isTransitionToHigh(){
	return current_state && !previous_state;
}

bool Gate::isTransitionToLow(){
	return (!current_state) && previous_state;
}

Gate gate;