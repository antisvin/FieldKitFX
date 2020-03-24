/*
 * buttonArray.c
 *
 */

#include "buttonArray.h"


void ButtonArray::init(SPI_HandleTypeDef *spi_controller) {
	spiController = spi_controller;

	GPIO_InitTypeDef GPIOStruct;

	__HAL_RCC_GPIOC_CLK_ENABLE();

	//Load pin
	GPIOStruct.Pin = BA_LOAD_PIN;
	GPIOStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOStruct.Pull = GPIO_NOPULL;
	GPIOStruct.Speed = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(BA_LOAD_PORT, &GPIOStruct);

	//Chip Enable pin
	GPIOStruct.Pin = BA_CHIP_ENABLE_PIN;
	GPIOStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOStruct.Pull = GPIO_NOPULL;
	GPIOStruct.Speed = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(BA_CHIP_ENABLE_PORT, &GPIOStruct);

	//set the state registers
	currentState = 0x0000;
	previousState = 0x0000;

	//init the counter
	longPressCounter = 0;

	//init the combination flag
	activeCombination = NO_COMBINATION;

	//set the pins to the right levels
	HAL_GPIO_WritePin(BA_LOAD_PORT, BA_LOAD_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BA_CHIP_ENABLE_PORT, BA_CHIP_ENABLE_PIN, GPIO_PIN_SET);
}

void ButtonArray::update() {
	uint8_t temp[2];

	//shift the current state into the old one
	previousState = currentState;

	HAL_SPIEx_FlushRxFifo(spiController);	//just to be sure

	//load pin low
	HAL_GPIO_WritePin(BA_LOAD_PORT, BA_LOAD_PIN, GPIO_PIN_RESET);
	//and high
	HAL_GPIO_WritePin(BA_LOAD_PORT, BA_LOAD_PIN, GPIO_PIN_SET);
	//put the CE pin low
	HAL_GPIO_WritePin(BA_CHIP_ENABLE_PORT, BA_CHIP_ENABLE_PIN, GPIO_PIN_RESET);
	//read thru SPI
	HAL_SPI_Receive(spiController, temp, 2, 1000);
	//put the CE pin high
	HAL_GPIO_WritePin(BA_CHIP_ENABLE_PORT, BA_CHIP_ENABLE_PIN, GPIO_PIN_SET);

	currentState = (temp[1] << 8) | temp[0];
}

void ButtonArray::checkCombinations(){
	/*
	 * check for the calibration button combination
	 */
	if(isPressed(0) && isPressed(1) && isPressed(9) && isPressed(10)) {
		if(++(longPressCounter) > BUTTON_ARRAY_LONG_PRESS){
			activeCombination = CALIBRATION_COMBINATION;
		}
	}
	else {
		longPressCounter = 0;
		activeCombination = NO_COMBINATION;
	}
}

void ButtonArray::resetCombinations(){
	longPressCounter = 0;
	activeCombination = 0;
}

uint16_t ButtonArray::getActiveCombination(){
	return activeCombination;
}

bool ButtonArray::isRisingEdge(uint8_t index) {
	return (!(previousState & (1 << index))) && (currentState & (1 << index));
}
bool ButtonArray::isFallingEdge(uint8_t index) {
	return (previousState & (1 << index)) && (!(currentState & (1 << index)));
}

bool ButtonArray::isPressed(uint8_t index) {
	return (previousState & (1 << index)) && (currentState & (1 << index));
}
