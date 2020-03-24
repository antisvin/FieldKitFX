/*
 * rollodecks.h
 *
 */

#ifndef ROLLODECKS_H_
#define ROLLODECKS_H_

#include "utils/utils.h"

#define ROLLOMUX_PIN1 GPIO_PIN_0
#define ROLLOMUX_PIN2 GPIO_PIN_1
#define ROLLOMUX_PIN3 GPIO_PIN_2
#define ROLLOMUX_PORT GPIOB
#define ROLLOMUX_CLKENABLE __HAL_RCC_GPIOB_CLK_ENABLE

namespace fieldkitfx {

constexpr uint8_t num_rollo_cv = 4;

enum RollodecksState {
    SETMUX1, // only set the MUXs and start ADC
    SETMUXREAD1, // read ADC, set MUXs and start new conversion
    SETMUXREAD2, // read ADC, set MUXs and start new conversion
    SETMUXREAD3, // read ADC, set MUXs and start new conversion
    SETMUXREAD4 // read ADC, set MUXs and start new conversion
};

class Rollodecks {
private:
    RollodecksState state = SETMUX1;
    DISALLOW_COPY_AND_ASSIGN(Rollodecks);

public:
    uint16_t cv_values[num_rollo_cv] { 1, 1, 1, 1 };

    Rollodecks() = default;
    void init();
    void set(uint8_t position);
    void update();
};

extern Rollodecks rollodecks;
}

#endif /* ROLLODECKS_H_ */
