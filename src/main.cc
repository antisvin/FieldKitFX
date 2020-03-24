/*
 * main.c
 *
 */

#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"

#include "main.h"
#include "engine/engine.h"
#include "engine/frequency_shifter.h"
#include "engine/looper.h"
#include "engine/sequencer.h"
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

    /*
     * set up a frequency shifter instance
     */
    FrequencyShifter fs;
    fs.setSideBandXfade(1.0); // only output the USB

    /*
     * set a initial shift amount (500Hz)
     */
    fs.setShiftAmt(500);

    /*
     * sample to process
     */
    float sample;

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

    for (;;) {
        switch (fxSelector.getSelectedFx()) {
        case FX_FREQ_SHIFT:
            /*
             * Frequency shifter
             */
            if (!user_audio_out_buffer.isFull() && !user_audio_in_buffer.isEmpty()) {
                /*
                 * Process a sample with the frequency shifter
                 */
                user_audio_in_buffer.pop(&sample);
                fs.processSample(&sample);
                user_audio_out_buffer.push(sample);
            }
            else {
                if (eventCounter & (1 << 0)) {
                    fs.updateCVs();
                }
                else {
                    ui.render();
                }
                eventCounter++;
            }
            break;
        case FX_LOOPER:
            /*
             * Looper
             */
            if (user_audio_in_buffer.isFull()) {
                // Looper state machine
                looper.process();
                // apply the looper effects
                /*
                 * Bitcrusher
                 */
                looper.processBitcrush(user_audio_out_buffer.buffer,
                    user_audio_out_buffer.buffer, ADC_getMixedCV1());
                /*
                 * SRR
                 */
                looper.processSampleRateReducer(user_audio_out_buffer.buffer,
                    user_audio_out_buffer.buffer, ADC_getMixedCV2());
            }
            else {
                ui.render();
            }
            break;
        default:
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
