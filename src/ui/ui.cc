/*
 * UI.c
 *
 */

#include "ui/ui.h"

namespace fieldkitfx {

/*
 * Initialize all the parts necessary to control and process the UI.
 */

void UI::init(void) {
    cvMatrix.init();
    fxSelector.init();
    current_ui_state = UI_modeSwitches_update;
    rollodecks.init();
    cvMatrix.reset();
    loopButton.init(&(cvMatrix.U54));
    rolloSelector.init();

    render();

    TIM_ClockConfigTypeDef clockSourceStruct;

    __HAL_RCC_TIM2_CLK_ENABLE();
    UITimer.Instance = TIM2;
    UITimer.Init.CounterMode = TIM_COUNTERMODE_UP;
    UITimer.Init.Period = 65535;
    UITimer.Init.Prescaler = 72; // 1MHz update rate
    UITimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    UITimer.Init.RepetitionCounter = 0;

    clockSourceStruct.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&UITimer, &clockSourceStruct);

    if (HAL_TIM_Base_Init(&UITimer) != HAL_OK) {
        __asm("bkpt");
    }

    HAL_TIM_Base_Start(&UITimer);
}

/*
 * This function is called in the main loop. It executes the different tasks of
 * the UI according to the current state of the state machine.
 */
void UI::render() {
    switch (current_ui_state) {
    case UI_matrixButtons_update:
        if (__HAL_TIM_GET_COUNTER(&UITimer) > MATRIX_REFRESH_PERIOD) {
            matrixRefreshFlag = 1;
            // reset the timer value
            UITimer.Instance->CNT = 0;
            cvMatrix.updateButtonStates();
            if (cvMatrix.ba.getActiveCombination() == CALIBRATION_COMBINATION) {
                cvMatrix.ba.resetCombinations();
                fxSelector.switchToCalibration();
                // we don't want to update the matrix
                matrixRefreshFlag = 0;
            }
        }
        break;
    case UI_matrixRouting_update:
        if (matrixRefreshFlag) {
            cvMatrix.updateCvDestinations();
        }
        break;
    case UI_matrixLEDs_update:
        if (matrixRefreshFlag) {
            cvMatrix.syncLedsToDestinations();
        }
        break;
    case UI_rolloMUX_update:
        if (matrixRefreshFlag) {
            rollodecks.update();
            matrixRefreshFlag = 0;
        }
        break;
    case UI_loopButton_update:
        loopButton.updateStates();
        if (fxSelector.getSelectedFx() == FX_LOOPER) {
            switch (looper.state) {
            case ARMED:
                looper.previousState = ARMED;
                if (loopButton.checkRisingEdge()) {
                    looper.flag = 0;
                    looper.framePointer = 0;
                    looper.state = RECORD;
                    looper.previousState = ARMED;
                    loopButton.setColor(LOOPLED_RECORD);
                    loopButton.setIntensity(LOOPLED_HIGH_INTENSITY);
                }
                break;

            case RECORD:
                looper.previousState = RECORD;
                if (loopButton.checkFallingEdge() ||
                    looper.framePointer >= MAX_FRAME_NUM) {
                    looper.state = PLAYBACK;
                    looper.previousState = RECORD;
                    looper.endFramePosition = looper.framePointer - 1;
                    looper.framePointer = 0;
                    looper.doFadeOut = FRAME_NUMBER_FADE_IN;
                    loopButton.setIntensity(LOOPLED_NORMAL_INTENSITY);
                    loopButton.setColor(LOOPLED_PLAYBACK);
                }
                break;

            case PLAYBACK:
                looper.previousState = PLAYBACK;
                if (loopButton.isLow()) {
                    looper.flag = 1;
                }
                if (looper.endFramePosition >=
                    BLINK_FRAME_THRESHOLD + BLINK_GUARD_INTERVAL) {
                    if (looper.framePointer >=
                        looper.endFramePosition - BLINK_FRAME_THRESHOLD) {
                        loopButton.setIntensity(0);
                    }
                    else if (looper.framePointer == 0) {
                        loopButton.setIntensity(LOOPLED_NORMAL_INTENSITY);
                    }
                }
                if (loopButton.checkRisingEdge()) {
                    looper.state = OVERDUB;
                    looper.previousState = PLAYBACK;
                    looper.doOverdubFadeIn = FRAME_NUMBER_FADE_IN;
                    loopButton.setColor(LOOPLED_OVERDUB);
                    loopButton.setIntensity(LOOPLED_NORMAL_INTENSITY);
                }
                break;

            case OVERDUB:
                looper.previousState = OVERDUB;
                if (loopButton.checkFallingEdge()) {
                    if (loopButton.wasShortPress()) {
                        looper.state = ERASE;
                        looper.previousState = OVERDUB;
                        loopButton.setColor(LOOPLED_ERASE);
                        loopButton.setIntensity(LOOPLED_NORMAL_INTENSITY);
                    }
                    else {
                        looper.state =
                            OVERDUB; // the looper switches to playback after the fadeout
                        looper.doOverdubFadeOut = FRAME_NUMBER_FADE_IN;
                        looper.previousState = OVERDUB;
                        loopButton.setColor(LOOPLED_PLAYBACK);
                        loopButton.setIntensity(LOOPLED_NORMAL_INTENSITY);
                    }
                }
                break;

            case ERASE:
                looper.previousState = ERASE;
                if (looper.framePointer >= looper.endFramePosition) {
                    looper.state = ARMED;
                    looper.previousState = ERASE;
                    loopButton.setColor(LOOPLED_ARMED);
                }
                break;

            default:
                break;
            }
        }
        if (fxSelector.justSwitchedTo(FX_FREQ_SHIFT)) {
            loopButton.setColor(LOOPLED_OFF);
        }
        break;
    case UI_modeSwitches_update:
        fxSelector.update();
        if (fxSelector.justSwitchedTo(FX_LOOPER)) {
            looper.state = ARMED;
            loopButton.setColor(LOOPLED_ARMED);
            loopButton.setIntensity(LOOPLED_NORMAL_INTENSITY);
        }
        break;
    case UI_tresholdcv_update:
        rolloSelector.update();

        if (rolloSelector.justSwitchedTo(ROLLO_SEQ)) {
            sequencer.uiInit();
        }
        else if (rolloSelector.justSwitchedTo(ROLLO_ENV)) {
            rollo_env.uiInit();
        }
        if (rolloSelector.isSelected(ROLLO_SEQ)) {
            sequencer.processCv((ADC_getThresholdPot() >> 2));
            sequencer.setMode(cvMatrix.router.getSource(SEQUENCER_CV_DESTINATION));
        }
        else {
            rollo_env.processAdsrCv(rollodecks.cv_values[0] >> 2,
                rollodecks.cv_values[1] >> 2, rollodecks.cv_values[2] >> 2,
                rollodecks.cv_values[3] >> 2);
            rollo_env.processThresholdPot(ADC_getThresholdPot() >> 2);
            if (rollo_env.process_ui) {
                rollo_env.setLengthThreshold();
                rollo_env.setMode(ADC_getThresholdPot() >> 2,
                    cvMatrix.router.getSource(SEQUENCER_CV_DESTINATION));
                rollo_env.setA();
                rollo_env.setD();
                rollo_env.setS();
                rollo_env.setR();
                rollo_env.process_ui = 0;
            }
        }
        break;
    }
    // update the state
    current_ui_state = (UiState)(((uint8_t)current_ui_state + 1) % NUMBEROFUISTATES);
}

/*
 * This inits the Calibration-Mode once the correspind button combination has been pressed.
 */
void UI::initCalibration() {
    current_ui_state_calibration = UI_matrixButtons_update_Calibration;
    renderMagnitudeInit();
}

/*
 * This renders the UI in calibration mode.
 */
void UI::renderCalibration() {
    switch (current_ui_state_calibration) {
    case UI_matrixButtons_update_Calibration:
        // check if the correct amount of time has elapsed
        if (__HAL_TIM_GET_COUNTER(&UITimer) > MATRIX_REFRESH_PERIOD) {
            // reset the timer value
            UITimer.Instance->CNT = 0;
            cvMatrix.updateButtonStates();
            if (cvMatrix.ba.getActiveCombination() == CALIBRATION_COMBINATION) {
                // reset the active Combination
                cvMatrix.ba.resetCombinations();
                // switch back to the effect depending on the actual switch position
                fxSelector.update();
                // reinit Brightness settings
                cvMatrix.la.setIntensity(CV_RGB_LED_INTENSITY);
                // reset all CV routings
                cvMatrix.router.disableAllRoutings();
                // sync LEDs to actual routing
                cvMatrix.forceSyncLedsToDestinations();
            }
        }
        break;
    case UI_renderMagnitude_Calibration:
        renderMagnitude(magnitude_tracker.getMax());
        break;
    }
    // update the state
    current_ui_state_calibration = (UiStateCalibration)(
        ((uint8_t)current_ui_state_calibration + 1) % NUMBEROFUISTATESCALIBRATION);
}

void UI::renderMagnitudeInit() {
    //	first 9 leds green
    for (uint8_t i = 0; i < 9; i++) {
        cvMatrix.la.setColor(i, 0, 127, 0);
    }
    //	10th led yellow
    cvMatrix.la.setColor(9, 0xff, 0x10, 0);
    //	11th led red
    cvMatrix.la.setColor(10, 0xff, 0, 0);
    //	turn all leds off
    cvMatrix.la.setIntensity(0);
}

void UI::renderMagnitude(float magnitude) {
    uint8_t scaledMagnitude = (uint16_t)(magnitude * 11) % 11;
    static uint8_t previousMagnitude = 0;
    if (scaledMagnitude != previousMagnitude) {
        for (uint8_t i = 0; i <= scaledMagnitude; i++) {
            cvMatrix.la.setIntensity(i, 0x04);
        };
        for (uint8_t i = scaledMagnitude + 1; i < 11; i++) {
            cvMatrix.la.setIntensity(i, 0x00);
        };
        previousMagnitude = scaledMagnitude;
    }
}

}