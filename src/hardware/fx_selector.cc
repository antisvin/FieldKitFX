/*
 * FXSelector_switch.c
 *
 */

#include "hardware/fx_selector.h"

namespace fieldkitfx {

FxSelector fxSelector;

void FxSelector::init() {
    FXSELECTOR_CLK_ENABLE();
    GPIOStruct.Pin = FXSELECTOR_PIN;
    GPIOStruct.Pull = GPIO_NOPULL;
    GPIOStruct.Speed = GPIO_SPEED_HIGH;
    GPIOStruct.Mode = GPIO_MODE_INPUT;

    HAL_GPIO_Init(FXSELECTOR_PORT, &GPIOStruct);
}

void FxSelector::update() {
    previous = current;
    if(HAL_GPIO_ReadPin(FXSELECTOR_PORT, FXSELECTOR_PIN)) {
        current = FX_FREQ_SHIFT;
    } else {
        current = FX_LOOPER;
    }
}

void FxSelector::switchToCalibration() {
    previous = current;
    current = CALIBRATION;
}

FxSelectorState FxSelector::getSelectedFx() {
    return current;
}

bool FxSelector::justSwitchedTo(FxSelectorState fx) {
    return (current == fx) && (previous != fx);
}

}