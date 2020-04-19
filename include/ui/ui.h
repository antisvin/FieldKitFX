/*
 * UI.h
 *
 * UI takes care of handling any user input and displaying the current settings.
 *
 * It's organized in a fairly simple state machine. UI_render is called in the main loop
 * and the task according to the current state is executed. Some of those tasks are dependent
 * on the currently selected effect.
 *
 * In calibration mode (currently only used to set the input gain), the ui switches over to
 * a seperate state machine.
 *
 * The UI consists of the following parts:
 * - the Effect-Selectswitch (either Frequencyshifter or Looper) : FXSelector
 * - the CV-Routingmatrix, including all Buttons, LEDs and internal Hardware: routingMatrix
 * - the Multiplexer used to switch between the four Pots of the Roll-O-Decks: rolloMux
 * - the Looper-Button: loopButton
 * - the Modeselector-Switch of the Roll-O-Decks: rolloSelector
 *
 */

#ifndef UI_H_
#define UI_H_

#include "hardware/codec.h"
#include "hardware/fx_selector.h"
#include "hardware/gate.h"
#include "hardware/loop_button.h"
#include "hardware/rollodecks.h"
#include "hardware/rollo_selector.h"
#include "hardware/routing_matrix.h"
#include "engine/adsr.h"
#include "engine/looper.h"
#include "engine/sequencer.h"
#include "engine/settings.h"
#include "utils/magnitude_tracker.h"
#include "ui/page.h"

/*
 * Refresh period of the CV-Matrix.
 */
#define MATRIX_REFRESH_PERIOD 10000 // 1MHz/10000 = 100Hz

/*
 * States of the default state machine.
 */
#define NUMBEROFUISTATES 7

namespace fieldkitfx {

enum UiState {
    UI_matrixButtons_update = 0,
    UI_matrixRouting_update,
    UI_matrixLEDs_update,
    UI_rolloMUX_update,
    UI_loopButton_update,
    UI_modeSwitches_update,
    UI_tresholdcv_update
};

/*
 * States of the calibration state machine.
 */

#define NUMBEROFUISTATESCALIBRATION 2

enum UiStateCalibration {
    UI_matrixButtons_update_Calibration = 0,
    UI_renderMagnitude_Calibration
};

class UI {
public:
    MagnitudeTracker magnitude_tracker;
    UI() {};
    void init(Settings* settings);
    void render();
    void initCalibration();
    void renderCalibration();
    void renderMagnitudeInit();
    void renderMagnitude(float magnitude);
    void renderSettingsInit();

    bool pauseAudio() {
        return current_page_id == UI_VOCT;
    }

private:
    UiState current_ui_state;
    UiStateCalibration current_ui_state_calibration;
    TIM_HandleTypeDef UITimer;
    Settings* settings;
    uint8_t blink_counter;
    uint8_t last_pressed_button;
    bool matrixRefreshFlag;
    void renderLooper();
    void renderSettings();
    void setBlinkState(bool blink_started);
    void updateBlink();

    UiPageId current_page_id;
    UiFxPage ui_page_fx1;
    UiFxPage ui_page_fx2;
    UiFxPage ui_page_fx3;
    UiFxPage ui_page_fx4;
    UiVcoPage ui_page_vco;
    UiLooperPage ui_page_looper;
    UiModulationPage ui_page_modulation;
    UiVoctPage ui_page_voct;
    UiVolumePage ui_page_volume;
    UiPresetSavePage ui_page_preset_save;
    UiPresetLoadPage ui_page_preset_load;

    BaseUiPage* pages[num_ui_pages] = {
        &ui_page_fx1,
        &ui_page_fx2,
        &ui_page_fx3,
        &ui_page_fx4,
        &ui_page_vco,
        &ui_page_looper,
        &ui_page_modulation,
        &ui_page_voct,
        &ui_page_volume,
        &ui_page_preset_save,
        &ui_page_preset_load,
    };
    BaseUiPage* current_page = &ui_page_fx1;
};

}
#endif /* UI_H_ */
