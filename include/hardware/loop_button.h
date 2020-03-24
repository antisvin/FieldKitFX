/*
 * loopButton.h
 *
 */

#ifndef LOOPBUTTON_H_
#define LOOPBUTTON_H_

#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#include "hardware/rgb_led.h"
#include "utils/utils.h"

/*
 * UI related defines
 * Timings in milliseconds
 */
#define LONGPRESS_PERIOD_MIN 200

/*
 * GPIO Definition
 */
#define LOOPBUTTON_PORT_CLOCK __HAL_RCC_GPIOB_CLK_ENABLE()
#define LOOPBUTTON_PORT GPIOB
#define LOOPBUTTON_PIN GPIO_PIN_7
/*
 * RGB LED related definitions
 */
#define LOOPLED_BASEADDRESS 0x0a // first LED
#define LOOPLED_NORMAL_INTENSITY 0x05
#define LOOPLED_HIGH_INTENSITY 0x20

#define LOOPLED_ARMED 0, 255, 0
#define LOOPLED_RECORD 255, 0, 0
#define LOOPLED_PLAYBACK 0, 0, 255
#define LOOPLED_OVERDUB 255, 0, 255
#define LOOPLED_ERASE 255, 255, 255

#define LOOPLED_OFF 0, 0, 0

namespace fieldkitfx {

class LoopButton {
private:
    uint8_t state;
    uint32_t last_up_timestamp;
    uint32_t last_down_timestamp;
    uint32_t current_timestamp;
    RgbLed led;
    GPIO_InitTypeDef GPIOStruct;

    DISALLOW_COPY_AND_ASSIGN(LoopButton);

public:
    LoopButton() = default;
    void init(LedDriver* driver);
    void updateStates();
    bool checkRisingEdge();
    bool checkFallingEdge();
    bool wasShortPress();
    bool wasLongPress();
    bool isLongPress();
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setIntensity(uint8_t intensity);
    bool isLow();
    bool isHigh();
    void resetStates();
};

extern LoopButton loopButton;
}

#endif /* LOOPBUTTON_H_ */
