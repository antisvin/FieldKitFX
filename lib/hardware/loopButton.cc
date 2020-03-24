/*
 * loopButton.c
 *
 */


#include "loopButton.h"


/*
 * Loop button specific colors definition
 */
const uint8_t loopButtonColors[3][3] = {
		{0, 0, 0},	//button OFF
		{255, 0, 0},	//red
		{255, 0, 255}	//purple
};


void LoopButton::init(LedDriver *driver) {
	LOOPBUTTON_PORT_CLOCK;
	GPIO_InitTypeDef GPIOStruct;

	GPIOStruct.Pin = LOOPBUTTON_PIN;
	GPIOStruct.Mode = GPIO_MODE_INPUT;
	GPIOStruct.Pull = GPIO_NOPULL;
	GPIOStruct.Speed = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(LOOPBUTTON_PORT, &GPIOStruct);

	//init the LED
	RgbLed led {
		driver, LOOPLED_BASEADDRESS, LOOPLED_NORMAL_INTENSITY,
		loopButtonColors[0][0], loopButtonColors[0][1], loopButtonColors[0][2]};
	led.setColor();
	led.setIntensity();
}

void LoopButton::updateStates() {
	//read the button and push the thing in the state register
	state <<= 1;	//shift the states register to make place for the new state
	if(HAL_GPIO_ReadPin(LOOPBUTTON_PORT, LOOPBUTTON_PIN) == GPIO_PIN_SET) {
		state |= 0x01;
	}

	//update the timestamp
	current_timestamp = HAL_GetTick();

	//detect rising/falling edges
	checkRisingEdge();
	checkFallingEdge();
}

bool LoopButton::checkRisingEdge() {
	if((state & 0x01) && !(state & 0x02)) {
		//update the timestamps
		last_up_timestamp = current_timestamp;
		return true;
	}
	else {
		return false;
	}
}

bool LoopButton::checkFallingEdge() {
	if (!(state & 0x01) && (state & 0x02)) {
		last_down_timestamp = current_timestamp;
		return true;
	}
	else {
		return false;
	}
}

/*
 * If the user is "longpressing"
 */
bool LoopButton::isLongPress() {
	return (current_timestamp - last_up_timestamp > LONGPRESS_PERIOD_MIN) && (state & 0x01);
}

/*
 * was the last falling edge one of a long press ?
 */
bool LoopButton::wasLongPress() {
	return current_timestamp - last_up_timestamp > LONGPRESS_PERIOD_MIN;
}

bool LoopButton::wasShortPress() {
	return (last_down_timestamp - last_up_timestamp) < LONGPRESS_PERIOD_MIN;
}

void LoopButton::setColor(uint8_t red, uint8_t green, uint8_t blue) {
	led.red = red;
	led.green = green;
	led.blue = blue;
	led.setColor();
}


void LoopButton::setIntensity(uint8_t intensity) {
	led.intensity = intensity;
	led.setIntensity();
}

void LoopButton::resetStates() {
	state = 0x00;
	last_up_timestamp = current_timestamp;
	last_down_timestamp = current_timestamp;
}

bool LoopButton::isLow() {
	return !(state & 0x01);
}

bool LoopButton::isHigh() {
	return state & 0x01;
}

LoopButton loopButton;