/*
 * rollodecks.c
 *
 */

#include "stm32f3xx_hal.h"
#include "hardware/adc.h"
#include "hardware/rollodecks.h"

namespace fieldkitfx {

Rollodecks rollodecks;

void Rollodecks::init() {
    GPIO_InitTypeDef GPIOStruct;

    ROLLOMUX_CLKENABLE();
    GPIOStruct.Pin = ROLLOMUX_PIN1 | ROLLOMUX_PIN2 | ROLLOMUX_PIN3;
    GPIOStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOStruct.Pull = GPIO_NOPULL;
    GPIOStruct.Speed = GPIO_SPEED_HIGH;

    HAL_GPIO_Init(ROLLOMUX_PORT, &GPIOStruct);

    // populate the rollo CV values with something different than 0 to avoid
    // adsr blocking rolloCV_values[0] = 1; rolloCV_values[1] = 1;
    // rolloCV_values[2] = 1; rolloCV_values[3] = 1;

    /*
     * Init substate
     */
    // state = SETMUX1;
    set(0);
}

void Rollodecks::set(uint8_t position) {
    GPIOB->ODR &= ~(0x0007); // the three last bits
    GPIOB->ODR |= (position & 0x0007);
}

void Rollodecks::update() {
    switch(state) {
    case SETMUX1:
        set(0);
        state = SETMUXREAD2;
        break;
    case SETMUXREAD1:
        cv_values[3] = ADC_getRolloCV();
        set(0);
        state = SETMUXREAD2;
        break;
    case SETMUXREAD2:
        cv_values[0] = ADC_getRolloCV();
        set(1);
        state = SETMUXREAD3;
        break;
    case SETMUXREAD3:
        cv_values[1] = ADC_getRolloCV();
        set(2);
        state = SETMUXREAD4;
        break;
    case SETMUXREAD4:
        cv_values[2] = ADC_getRolloCV();
        set(3);
        state = SETMUXREAD1;
        break;
    }
}

}