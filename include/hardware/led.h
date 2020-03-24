/*
 * LED.h
 *
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#include "utils/utils.h"

/*
 * The LEDs used to indicate the state of the sequencer
 */

#define LED0_PORT GPIOE
#define LED0_PIN GPIO_PIN_2

#define LED1_PORT GPIOE
#define LED1_PIN GPIO_PIN_3

#define LED2_PORT GPIOE
#define LED2_PIN GPIO_PIN_4

#define LED3_PORT GPIOE
#define LED3_PIN GPIO_PIN_5

namespace fieldkitfx {

class Led {
private:
    DISALLOW_COPY_AND_ASSIGN(Led);

public:
    GPIO_InitTypeDef GPIOInitStruct;

    GPIO_TypeDef* port;
    uint32_t pin;

    Led() = default;
    void init(GPIO_TypeDef* port, uint32_t pin);
    void setHigh();
    void setLow();
};

extern Led led0, led1, led2, led3;
}
#endif /* LED_H_ */
