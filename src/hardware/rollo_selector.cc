/*
 * rolloSelector_switch.c
 *
 */

#include "hardware/rollo_selector.h"

namespace fieldkitfx {

RolloSelector rolloSelector;

void RolloSelector::init() {
    GPIO_InitTypeDef GPIOStruct;

    ROLLOSELECTOR_CLK_ENABLE();
    GPIOStruct.Pin = ROLLOSELECTOR_PIN;
    GPIOStruct.Pull = GPIO_NOPULL;
    GPIOStruct.Speed = GPIO_SPEED_HIGH;
    GPIOStruct.Mode = GPIO_MODE_INPUT;

    HAL_GPIO_Init(ROLLOSELECTOR_PORT, &GPIOStruct);

    current = ROLLO_START;
}

void RolloSelector::update() {
    previous = current;
    if(HAL_GPIO_ReadPin(ROLLOSELECTOR_PORT, ROLLOSELECTOR_PIN)) {
        current = ROLLO_SEQ;
    } else {
        current = ROLLO_ENV;
    }
}

bool RolloSelector::isSelected(RolloSelectorState rollo) {
    return current == rollo;
}

bool RolloSelector::justSwitchedTo(RolloSelectorState rollo) {
    return current == rollo && previous != rollo;
}

}