/*
 * main.c
 *
 */

#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"

#include "main.h"
#include "engine/engine.h"
#include "engine/effects.h"
#include "engine/looper.h"
#include "engine/sequencer.h"
#include "engine/settings.h"
#include "engine/timer.h"
#include "hardware/adc.h"
#include "hardware/codec.h"
#include "hardware/i2c.h"
#include "hardware/gate.h"
#include "hardware/memory.h"
#include "ui/ui.h"
#include "utils/magnitude_tracker.h"

using namespace fieldkitfx;

UI ui;

int main(void) {
    /*
     * This function initializes the HAL :
     * configures the Flash prefetch, time base source, NVIC and Low level hardware
     * This function is called at the beginning of program after reset and before the clock configuration
     */
    HAL_Init();

    /*
     * Configures the clock distribution system, turns the external crystal on, initializes the peripheral clock prescalers
     * end sets the SysTick to increment every millisecond
     */
    systemClock_Config_HSE();

    /*============================================================================================================*/
    /*
     * ADC config
     */
    ADC_Init();
    ADC_Start();
    DAC_Init();
    DAC_Start();

    /*
     * I2C controller setup
     * the MCU is the bus master
     */
    i2c_Config();

    /*
     * set up the codec
     */
    codec_ConfigGPIO();
    codec_ConfigAudioInterface();

    /*============================================================================================================*/
    /*
     * External memory Init
     */
    Memory_initSPI();
    RamChip RAMArray;
    /*
     * Initializes the CS pin and sets it idle
     */
    RAMArray.init();

    settings.Init();

    /*
     * Looper init
     */
    looper.init(&RAMArray);

    sequencer.init();

    /*============================================================================================================*/
    /*
     * UI Init
     */
    ui.init();

    // cvMatrix.forceSyncLedsToDestinations();

    GATE_IN_CLK_ENABLE;
    gate.init();

    /*
     * and start the codec
     */
    codec_ConfigCodec_48K();
    codec_start();

    Timer_init();

    /*============================================================================================================*/
    /*
     * Main loop
     */

    uint32_t eventCounter = 0;
    // float *tmp_buffer_src, *tmp_buffer_dst;
    float tmp_buffer1[USER_AUDIO_IO_BUFFER_SIZE];
    float tmp_buffer2[USER_AUDIO_IO_BUFFER_SIZE];

    for (;;) {
        /*
         * Looper
         */
        if (user_audio_in_buffer.isFull()) {
            filters_library.process(user_audio_in_buffer.buffer, tmp_buffer1);
            user_audio_in_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;

            effects_library1.process(tmp_buffer1, tmp_buffer2);
            effects_library2.process(tmp_buffer2, tmp_buffer1);
            effects_library3.process(tmp_buffer1, tmp_buffer2);

            // apply the looper effects
            // looper.process(tmp_buffer1, user_audio_out_buffer.buffer);
            looper.process(tmp_buffer2, user_audio_out_buffer.buffer);
            user_audio_out_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;

            parameter_controller.updateParams2();
        }
        ui.render();

        /*
            // in calibration mode
            if (fxSelector.justSwitchedTo(CALIBRATION)) {
                ui.initCalibration();
                fxSelector.switchToCalibration();
            }
            if (!user_audio_out_buffer.isFull() && !user_audio_in_buffer.isEmpty()) {
                // we want to feed trough the audio
                user_audio_in_buffer.pop(&sample);
                user_audio_out_buffer.push(sample);
                // and we want to track the maximum amplitude
                ui.magnitude_tracker.processSample(sample);
            }
            else {
                if (eventCounter & (1 << 0)) {
                    ui.renderCalibration();
                }
                else {
                    codec_setInputGain(
                        CODEC_REG_LEFT_LINE_IN, (ADC_getMixedCV2() >> 7));
                }
                eventCounter++;
            }
        }
        */
    }
}

void systemClock_Config_HSE(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL14;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        __asm("bkpt");
    }

    /**Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        __asm("bkpt");
    }

    /**Configure the Systick interrupt time
     */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick
     */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 1);

    /* GPIO Ports Clock Enable */
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin : PC9 */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
