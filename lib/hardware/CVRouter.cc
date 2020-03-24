/*
 * CVRouter.c
 *
 */

#include "CVRouter.h"

/*
 * router.state[i]  <-- i=Destination
 * Destinations:
 * 1 -> AMOUNT
 * 2 -> CONTROL
 * 3 -> TRESHOLD
 * 4 -> TIME
 * 5 -> FBACK
 * 6 -> CUTOFF
 * 7 -> FBACK (SPRING)
 * 8 -> VCA1
 * 9 -> VCA2
 * 10 -> VCA3
 * 11 -> VCA4
 */

void CvRouter::init(SPI_HandleTypeDef *spi_controller) {
	/*
	 * Init the GPIO load pin
	 */
	__HAL_RCC_GPIOF_CLK_ENABLE();
	GPIO_InitTypeDef GPIOStruct;

	GPIOStruct.Pin = GPIO_PIN_6;
	GPIOStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOStruct.Pull = GPIO_NOPULL;
	GPIOStruct.Speed = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOF, &GPIOStruct);

	/*
	 * The SPI related parameters are initialized in the routingMatrix Init routine
	 */
	spi_controller = spi_controller;
}

void CvRouter::setSwitches() {
	/*
	 * create the data to send from the state register
	 */
	uint8_t temp[8];
    temp[6] = (state[1] << 5) | (state[0] & 0x1F); //dest 1 and 0
    temp[5] = (state[2] << 7) | ((state[10] & 0x1F) << 2) | ((state[1] & 0x1F) >> 3); //dest 0 and 2
    temp[4] = (state[3] << 4) | ((state[2] & 0x1F) >> 1);
    temp[3] = (state[5] << 6) | ((state[4] & 0x1F) << 1) | ((state[3] & 0x1F) >> 4);
    temp[2] = (state[6] << 3) | ((state[5] & 0x1F) >> 2);
    temp[1] = (state[8] << 5) | (state[7] & 0x1F);
    temp[0] = ((state[9] & 0x1F) << 2) | ((state[8] & 0x1F) >> 3); //dest 0 and 2

	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);

	HAL_SPI_Transmit(spi_controller, temp, 7, 100);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET);
}

void CvRouter::disableAllRoutings(void) {
	for (uint8_t i = 0; i < 11; i++) {
		state[i] = CV_SOURCE_REFERENCE;
	}
	setSwitches();
}

void CvRouter::setDestination(uint8_t src, uint8_t dst) {
	state[dst] = (1 << src);
	setSwitches();
}

void CvRouter::cycleSource(uint8_t dst) {
	switch (state[dst]) {
	case CV_SOURCE_REFERENCE:
		state[dst] = CV_SOURCE_1;
		break;
	case CV_SOURCE_1:
		state[dst] = CV_SOURCE_2;
		break;
	case CV_SOURCE_2:
		state[dst] = CV_SOURCE_3;
		break;
	case CV_SOURCE_3:
		state[dst] = CV_SOURCE_4;
		break;
	case CV_SOURCE_4:
		state[dst] = CV_SOURCE_REFERENCE;
		break;
	}
}

uint8_t CvRouter::getSource(uint8_t dst) {
	switch (state[dst]) {
	case CV_SOURCE_1:
		return 1;
	case CV_SOURCE_2:
		return 2;
	case CV_SOURCE_3:
		return 3;
	case CV_SOURCE_4:
		return 4;
	default:
		return 0;
	}
}
