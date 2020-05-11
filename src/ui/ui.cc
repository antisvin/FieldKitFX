/*
 * UI.c
 *
 */

#include "engine/effects.h"
#include "ui/ui.h"

namespace fieldkitfx {

/*
 * Initialize all the parts necessary to control and process the UI.
 */

void UI::init() {
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
        if (matrixRefreshFlag && fxSelector.getSelectedFx() == FX_LOOPER) {
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
        switch (fxSelector.justSwitchedTo()) {
        case FX_LOOPER:
            cvMatrix.setState(FX_LOOPER);
            looper.refreshUi = true;
            break;
        case FX_FREQ_SHIFT:
            cvMatrix.setState(FX_FREQ_SHIFT);
            renderSettingsInit();
            break;
        default:
            break;
        }
        if (fxSelector.getSelectedFx() == FX_LOOPER) {
            renderLooper();
        }
        else {
            renderSettings();
        }
        break;
    case UI_modeSwitches_update:
        fxSelector.update();
        // if (fxSelector.justSwitchedTo(FX_LOOPER)) { }
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

void UI::renderSettingsInit() {
    // Reset intensity to normal in case if it was blinking
    loopButton.setColor(COL_NONE);
    loopButton.setIntensity(LOOPLED_NORMAL_INTENSITY);

    for (uint8_t page = 0; page < num_ui_pages; page++) {
        pages[page]->reset();
    }
    filters_library.refreshUi = true;
    effects_library1.refreshUi = true;
    effects_library2.refreshUi = true;
    effects_library3.refreshUi = true;
}

void UI::updateBlink() {
    switch (blink_counter++) {
    case 0:
        current_page->blink();
        break;
    case 1 << 7:
        current_page->renderState();
        break;
    default:
        break;
    }
}

void UI::renderSettings() {
    if (cvMatrix.ba.isPressed()) {
        last_pressed_button = cvMatrix.ba.getFirstPressed();
        if (last_pressed_button != (uint8_t)current_page_id) {
            // New page selected
            // Render state once more in case if we were blinking
            current_page->renderState();
            // Switch to new page
            current_page_id = (UiPageId)last_pressed_button;
            current_page = pages[last_pressed_button];
            current_page->reset();
            current_page->onButtonPressed();
        }
        else {
            // Same page that was previously active is selected
            if (cvMatrix.ba.isRisingEdge(last_pressed_button)) {
                current_page->onButtonPressed();
            }
            // Loop button pressed - switch pending state, but don't save yet
            else if (loopButton.checkRisingEdge()) {
                current_page->onLoopButtonPressed(false);
                current_page->renderState();
            }
        }
    }
    else {
        if (cvMatrix.ba.isFallingEdge(last_pressed_button)) {
            // Button got depressed. It needs some antidepressants to feel
            // better!
            current_page->onButtonDepressed();
            current_page->renderState();
        }
        // Loop button pressed - store settings
        if (loopButton.checkRisingEdge()) {
            current_page->onLoopButtonPressed(true);
            current_page->renderState();
            // current_page->renderState();
            //            updateBlink();
        }
    }
    updateBlink();
}

void UI::renderLooper() {
    switch (looper.state) {
    case ARMED:
        if (looper.refreshUi) {
            loopButton.setColor(LOOPLED_ARMED);
            loopButton.setIntensity(LOOPLED_NORMAL_INTENSITY);
            looper.refreshUi = false;
        }
        if (loopButton.checkRisingEdge()) {
            looper.switchState(RECORD);
            looper.framePointer = 0;
        }
        break;
    case RECORD:
        if (looper.refreshUi) {
            loopButton.setColor(LOOPLED_RECORD);
            loopButton.setIntensity(LOOPLED_HIGH_INTENSITY);
            looper.refreshUi = false;
        }
        if (loopButton.checkFallingEdge() || looper.framePointer >= MAX_FRAME_NUM) {
            looper.switchState(PLAYBACK);
            looper.endFramePosition = looper.framePointer - 1;
            looper.framePointer = 0;
            looper.doFadeOut = FRAME_NUMBER_FADE_IN;
        }
        break;
    case PLAYBACK:
        if (looper.refreshUi) {
            loopButton.setIntensity(LOOPLED_NORMAL_INTENSITY);
            loopButton.setColor(LOOPLED_PLAYBACK);
            looper.refreshUi = false;
        }
        if (looper.endFramePosition >= BLINK_FRAME_THRESHOLD + BLINK_GUARD_INTERVAL) {
            if (looper.framePointer >= looper.endFramePosition - BLINK_FRAME_THRESHOLD) {
                loopButton.setIntensity(0);
            }
            else if (looper.framePointer == 0) {
                loopButton.setIntensity(LOOPLED_NORMAL_INTENSITY);
            }
        }
        if (loopButton.checkRisingEdge()) {
            looper.switchState(OVERDUB);
            looper.doOverdubFadeIn = FRAME_NUMBER_FADE_IN;
        }
        break;
    case OVERDUB:
        if (looper.refreshUi) {
            loopButton.setColor(LOOPLED_OVERDUB);
            loopButton.setIntensity(LOOPLED_NORMAL_INTENSITY);
            looper.refreshUi = false;
        }
        if (loopButton.checkFallingEdge()) {
            if (loopButton.wasShortPress()) {
                looper.switchState(ERASE);
            }
            else {
                looper.doOverdubFadeOut = FRAME_NUMBER_FADE_IN;
                //    looper.switchState(PLAYBACK);
            }
        }
        break;
    case ERASE:
        if (looper.refreshUi) {
            loopButton.setColor(LOOPLED_ERASE);
            loopButton.setIntensity(LOOPLED_NORMAL_INTENSITY);
            looper.refreshUi = false;
        }
        if (looper.framePointer >= looper.endFramePosition) {
            looper.switchState(ARMED);
        }
        break;
    }
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

UiFxPage ui_page_fx1(UI_FX1, &filters_library);
UiFxPage ui_page_fx2(UI_FX2, &effects_library1);
UiFxPage ui_page_fx3(UI_FX3, &effects_library2);
UiFxPage ui_page_fx4(UI_FX4, &effects_library3);
UiLooperPage ui_page_looper(UI_LOOPER);
UiModulationPage ui_page_modulation(UI_MODULATION);
UiParametersPage ui_page_parameters(UI_PARAMETERS);
UiVolumePage ui_page_volume(UI_VOLUME);
UiPresetRandomPage ui_page_preset_random(UI_PRESET_RANDOM);
UiPresetSavePage ui_page_preset_save(UI_PRESET_SAVE);
UiPresetLoadPage ui_page_preset_load(UI_PRESET_LOAD);

ParameterController parameter_controller(&ui_page_fx1, &ui_page_looper);

}