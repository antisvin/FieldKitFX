/*
 * routingMatrix.c
 *
 */


#include "routingMatrix.h"
#include "colors.h"

#include "debug.h"

static SPI_HandleTypeDef spiController;

void RoutingMatrix::init() {
	/*
	 * --------- Init the SPI controller
	 */

	__HAL_RCC_SPI1_CLK_ENABLE();

	spiController.Instance = SPI1;
	spiController.Init.Mode = SPI_MODE_MASTER;		//Master mode
	spiController.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;	//32MHz/2 => 16.5Mbps
	spiController.Init.Direction = SPI_DIRECTION_2LINES;		//2ways communication
	spiController.Init.DataSize = SPI_DATASIZE_8BIT;		//8 bit transmission
	spiController.Init.FirstBit = SPI_FIRSTBIT_MSB;		//MSB first
	spiController.Init.CLKPolarity = SPI_POLARITY_LOW;		//clock idle low
	spiController.Init.CLKPhase	= SPI_PHASE_1EDGE;		//sampling on rising edge
	spiController.Init.NSS = SPI_NSS_SOFT;		//Software NSS
	spiController.Init.NSSPMode = SPI_NSS_PULSE_DISABLED;
	spiController.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;		//No CRC calculation
	spiController.Init.TIMode = SPI_TIMODE_DISABLE;

	HAL_SPI_Init(&spiController);

	//And the pins that go with it
	GPIO_InitTypeDef GPIOStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIOStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIOStruct.Mode = GPIO_MODE_AF_PP;
	GPIOStruct.Alternate = GPIO_AF5_SPI1;
	GPIOStruct.Pull = GPIO_PULLDOWN;
	GPIOStruct.Speed = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOA, &GPIOStruct);

	/*
	 * --------- Init the CV router
	 */
	router.init(&spiController);

	/*
	 * --------- Init the buttons array
	 */
	ba.init(&spiController);

	/*
	 * --------- Init the RGB LEDs array
	 */
	/*
	 * I2C controller setup
	 * the MCU is the bus master
	 */
//	i2c_Config();
	//U55.i2cAddr = U7_I2C_ADDRESS;
	//U55.i2cController = &I2c2Handle;
	//U54.i2cAddr = U6_I2C_ADDRESS;
	//U54.i2cController = &I2c2Handle;

	U54.init(U6_I2C_ADDRESS, &I2c2Handle);
	U55.init(U7_I2C_ADDRESS, &I2c2Handle);

	/*
	 * RGB LED Array config
	 */
	la.init(&U54, &U55);
}


void RoutingMatrix::reset() {
	/*
	 * set all the modulation destinations to off
	 */
	router.disableAllRoutings();
	/*
	 * update the LEDs
	 */
	syncLedsToDestinations();
}


void RoutingMatrix::updateButtonStates() {
	ba.update();
	ba.checkCombinations();
}


void RoutingMatrix::updateCvDestinations() {
	for (uint8_t i = 0; i < 11; i++) {
		if (ba.isRisingEdge(i)) {
			router.cycleSource(i);
			//set the "has changed" bit (bit7)
			router.state[i] |= 0x80;

			switchUpdateFlag = 1;
			switchUpdateStartTime = HAL_GetTick();
		}
	}
	if (switchUpdateFlag && (HAL_GetTick() - switchUpdateStartTime > MATRIX_SWITCHUPDATE_DELAY)) {
		router.setSwitches();
		switchUpdateFlag = false;
	}
}

void RoutingMatrix::syncLedsToDestinations() {
	/*
	 * for each destination in the router's state register, associate the right color
	 */
	for(uint8_t i = 0; i < 11; i++) {
		if(router.state[i] & 0x80) {	//check if this state has changed
			//reset the "has changed" bit
			router.state[i] &= ~(0x80);
			switch(router.state[i]) {
			case CV_SOURCE_REFERENCE:
				la.setColor(i, colors[0][0], colors[0][1], colors[0][2]);
				break;
			case CV_SOURCE_1:
				la.setColor(i, colors[1][0], colors[1][1], colors[1][2]);
				break;
			case CV_SOURCE_2:
				la.setColor(i, colors[2][0], colors[2][1], colors[2][2]);
				break;
			case CV_SOURCE_3:
				la.setColor(i, colors[3][0], colors[3][1], colors[3][2]);
				break;
			case CV_SOURCE_4:
				la.setColor(i, colors[4][0], colors[4][1], colors[4][2]);
				break;
			}
		}
	}
}

void RoutingMatrix::forceSyncLedsToDestinations() {
	/*
	 * for each destination in the router's state register, associate the right color
	 */
	for(uint8_t i = 0; i < 11; i++) {
		switch(router.state[i]) {
		case CV_SOURCE_REFERENCE:
			la.setColor(i, colors[0][0], colors[0][1], colors[0][2]);
			break;
		case CV_SOURCE_1:
			la.setColor(i, colors[1][0], colors[1][1], colors[1][2]);
			break;
		case CV_SOURCE_2:
			la.setColor(i, colors[2][0], colors[2][1], colors[2][2]);
			break;
		case CV_SOURCE_3:
			la.setColor(i, colors[3][0], colors[3][1], colors[3][2]);
			break;
		case CV_SOURCE_4:
			la.setColor(i, colors[4][0], colors[4][1], colors[4][2]);
			break;
		}
	}
}

RoutingMatrix cvMatrix;