/*
 * Gate.h
 *
 */

#ifndef GATE_H_
#define GATE_H_

#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#include "utils/utils.h"

#define GATE_IN_PIN GPIO_PIN_2
#define GATE_IN_PORT GPIOC
#define GATE_IN_CLK_ENABLE __HAL_RCC_GPIOC_CLK_ENABLE()

namespace fieldkitfx {

class Gate {
private:
    DISALLOW_COPY_AND_ASSIGN(Gate);

public:
    uint8_t current_state;
    uint8_t previous_state;

    Gate() = default;
    void init();
    void update();
    bool isHigh();
    bool isLow();
    bool isTransitionToHigh();
    bool isTransitionToLow();
};

extern Gate gate;

}
#endif /* GATE_H_ */
