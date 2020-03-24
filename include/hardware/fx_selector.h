/*
 * FXSelector_switch.h
 *
 */

#ifndef FXSELECTOR_SWITCH_H_
#define FXSELECTOR_SWITCH_H_

#include "stm32f3xx_hal.h"
#include "stm32f3xx.h"
#include "utils/utils.h"

#define FXSELECTOR_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE

#define FXSELECTOR_PORT GPIOD
#define FXSELECTOR_PIN GPIO_PIN_1

namespace fieldkitfx {

enum FxSelectorState { FX_FREQ_SHIFT = 0, FX_LOOPER, CALIBRATION, FX_UNSET };

class FxSelector {
protected:
    FxSelectorState previous, current = FX_UNSET;
    DISALLOW_COPY_AND_ASSIGN(FxSelector);
    GPIO_InitTypeDef GPIOStruct;

public:
    FxSelector() = default;
    void init();
    void update();
    void switchToCalibration();
    FxSelectorState getSelectedFx();
    bool justSwitchedTo(FxSelectorState fx);
};

extern FxSelector fxSelector;

}

#endif /* FXSELECTOR_SWITCH_H_ */
