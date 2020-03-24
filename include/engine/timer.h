/*
 * timer.h
 *
 * This Timer is used to process sequencer and adsr at a fixed sample rate.
 * The corresponding functions are attached to the timer overflow isr.
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#include "engine/adsr.h"
#include "engine/sequencer.h"
#include "hardware/rollo_selector.h"

namespace fieldkitfx {

extern "C" {
extern TIM_HandleTypeDef SamplingTimer;

void Timer_init(void);
}
}

#endif /* TIMER_H_ */
