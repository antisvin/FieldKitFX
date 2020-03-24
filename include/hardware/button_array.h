/*
 * buttonArray.h
 *
 */

#ifndef BUTTONARRAY_H_
#define BUTTONARRAY_H_

#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"

#define BUTTON_ARRAY_LONG_PRESS 100

#include "utils/utils.h"
/*
 * Possible button combinations
 */

#define NO_COMBINATION 0
#define CALIBRATION_COMBINATION 1

#define BA_LOAD_PORT GPIOC
#define BA_LOAD_PIN GPIO_PIN_0
#define BA_CHIP_ENABLE_PORT GPIOC
#define BA_CHIP_ENABLE_PIN GPIO_PIN_1

namespace fieldkitfx {

class ButtonArray {
private:
    SPI_HandleTypeDef* spiController;
    uint16_t previousState, currentState;
    uint16_t longPressCounter;
    uint16_t activeCombination;
    DISALLOW_COPY_AND_ASSIGN(ButtonArray);

public:
    ButtonArray() = default;
    void init(SPI_HandleTypeDef* spi_controller);
    void update();
    void checkCombinations();
    void resetCombinations();
    uint16_t getActiveCombination();
    bool isRisingEdge(uint8_t index);
    bool isFallingEdge(uint8_t index);
    bool isPressed(uint8_t index);
};

}
#endif /* BUTTONARRAY_H_ */
