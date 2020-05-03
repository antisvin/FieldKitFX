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

static UiVcfPage ui_page_vcf(UI_VCF);
static UiFxPage ui_page_fx1(UI_FX1, &effects_library1);
static UiFxPage ui_page_fx2(UI_FX2, &effects_library2);
static UiFxPage ui_page_fx3(UI_FX3, &effects_library3);
static UiFxPage ui_page_fx4(UI_FX4, &effects_library4);
static UiLooperPage ui_page_looper(UI_LOOPER);
static UiModulationPage ui_page_modulation(UI_MODULATION);
static UiVolumePage ui_page_volume(UI_VOLUME);
static UiPresetRandomPage ui_page_preset_random(UI_PRESET_RANDOM);
static UiPresetSavePage ui_page_preset_save(UI_PRESET_SAVE);
static UiPresetLoadPage ui_page_preset_load(UI_PRESET_LOAD);

class UI {
public:
    MagnitudeTracker magnitude_tracker;
    UI() {};
    void init();
    void render();
    void initCalibration();
    void renderCalibration();
    void renderMagnitudeInit();
    void renderMagnitude(float magnitude);
    void renderSettingsInit();

private:
    UiState current_ui_state;
    UiStateCalibration current_ui_state_calibration;
    TIM_HandleTypeDef UITimer;
    uint8_t blink_counter;
    uint8_t last_pressed_button;
    bool matrixRefreshFlag;
    void renderLooper();
    void renderSettings();
    void setBlinkState(bool blink_started);
    void updateBlink();

    UiPageId current_page_id;

    BaseUiPage* pages[num_ui_pages] = {
        &ui_page_vcf,
        &ui_page_fx1,
        &ui_page_fx2,
        &ui_page_fx3,
        &ui_page_fx4,
        &ui_page_looper,
        &ui_page_modulation,
        &ui_page_volume,
        &ui_page_preset_random,
        &ui_page_preset_save,
        &ui_page_preset_load,
    };
    BaseUiPage* current_page = &ui_page_vcf;
};

}
#endif /* UI_H_ */
