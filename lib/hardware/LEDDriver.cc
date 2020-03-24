/*
 * LEDDriver.c
 */

#include "LEDDriver.h"


/*
 * Initializes the LEDDriver to it's default behavior
 */
void LedDriver::init(uint8_t i2c_addr, I2C_HandleTypeDef *i2c_controller) {
	i2cAddr = i2c_addr;
	i2cController = i2c_controller;
	//Set the AI0 and AI1 bits correctly to allow autoIncrementation for the PWMx registers
	writeReg(MODE1, 0x28, 0);	//no autoInc
}


/*
 * Writes the specified data in the specified register
 * The last parameter is used to set or not the AutoIncrement Flag (AIF)
 */
void LedDriver::writeReg(uint8_t regAddr, uint8_t data, uint8_t autoInc) {
	//if we want the autoIncrement flag to be set or not
	if(autoInc) {
		regAddr |= (1 << 7);
	}
	else {
		regAddr &= ~(1 << 7);
	}

	//Concatenate the regAddress and the data
	uint8_t temp[] {regAddr, data};

	//Send the data
	if(HAL_I2C_Master_Transmit(i2cController, (uint16_t)(i2cAddr << 1), temp, 2, I2C_TIMEOUT_VAL) != HAL_OK)
		__asm("bkpt");
}

/*
 * Writes the specified data in the specified register
 * The last parameter is used to set or not the AutoIncrement Flag (AIF)
 */
void LedDriver::write3Val(uint8_t startAddr, uint8_t data1, uint8_t data2, uint8_t data3) {
	//set the autoInc
	startAddr |= (1 << 7);

	//Concatenate the regAddress and the data
	uint8_t temp[] {startAddr, data1, data2, data3};

	//Send the data
	while(HAL_I2C_Master_Transmit(i2cController, (uint16_t)(i2cAddr << 1), temp, 4, I2C_TIMEOUT_VAL) != HAL_OK) {
	}
	while(HAL_I2C_GetState(i2cController) != HAL_I2C_STATE_READY) {		
	}
}

/*
 * Writes the specified data in the specified register
 * The last parameter is used to set or not the AutoIncrement Flag (AIF)
 */
void LedDriver::write1Val(uint8_t addr, uint8_t data) {
	//Concatenate the regAddress and the data
	uint8_t temp[] {addr, data};

	//Send the data
	while(HAL_I2C_Master_Transmit(i2cController, (uint16_t)(i2cAddr << 1), temp, 2, I2C_TIMEOUT_VAL) != HAL_OK) {
	};
}
