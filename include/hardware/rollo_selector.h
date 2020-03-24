/*
 * rolloSelector_switch.h
 *
 */

#ifndef ROLLOSELECTOR_SWITCH_H_
#define ROLLOSELECTOR_SWITCH_H_

#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#include "utils/utils.h"

#define ROLLOSELECTOR_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE

#define ROLLOSELECTOR_PORT GPIOD
#define ROLLOSELECTOR_PIN GPIO_PIN_2

namespace fieldkitfx {

enum RolloSelectorState { ROLLO_SEQ = 0, ROLLO_ENV, ROLLO_START };

class RolloSelector {
private:
    DISALLOW_COPY_AND_ASSIGN(RolloSelector);

public:
    RolloSelectorState previous, current;

    RolloSelector() = default;
    void init();
    void update();
    bool isSelected(RolloSelectorState rollo);
    bool justSwitchedTo(RolloSelectorState rollo);
};

extern RolloSelector rolloSelector;
}

#endif /* ROLLOSELECTOR_SWITCH_H_ */
