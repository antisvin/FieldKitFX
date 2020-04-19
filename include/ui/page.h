#ifndef _UI_PAGE_H_
#define _UI_PAGE_H_

#include "engine/settings.h"
#include "hardware/colors.h"
#include "hardware/rgb_led.h"
#include "hardware/routing_matrix.h"

namespace fieldkitfx {

constexpr uint8_t num_ui_pages = 11U;

enum UiPageId {
    UI_FX1,
    UI_FX2,
    UI_FX3,
    UI_FX4,
    UI_VCO,
    UI_LOOPER,
    UI_MODULATION,
    UI_VOCT,
    UI_VOLUME,
    UI_PRESET_SAVE,
    UI_PRESET_LOAD,
};

class BaseUiPage {
public:
    void init(UiPageId page_id, Settings* settings_ptr) {
        page_id = page_id;
        settings = settings_ptr;
    }

    void reset() {
        cvMatrix.la.setIntensity((uint8_t)page_id, CV_RGB_LED_INTENSITY);
        fromSettings();
    }

    virtual void fromSettings() = 0;
    virtual void toSettings() = 0;
    virtual void onLoopButtonPressed() = 0;
    virtual void onButtonDepressed() = 0;

protected:
    Color pending_state;
    UiPageId page_id;
    Settings* settings;
};

template <Color max_color>
class StatefulUiPage : public BaseUiPage {
public:
    void onLoopButtonPressed() override {
        if (pending_state == max_color) {
            pending_state = COL_NONE;
        }
        else {
            pending_state = (Color)((uint8_t)pending_state + 1);
        }
        cvMatrix.la.array[page_id].setColor(pending_state);
    }

    void onButtonDepressed() {
        toSettings();
    };
};

class UiFxPage : public StatefulUiPage<COL_RED> {
public:
    void fromSettings() override {
        pending_state = (Color)settings->current_preset.fx_state[page_id].engine;
        cvMatrix.la.array[page_id].setColor(pending_state);
    }

    void toSettings() override {
        settings->current_preset.fx_state[page_id].engine = (uint8_t)pending_state;
    }
};

class UiVcoPage : public StatefulUiPage<COL_NONE> {
    void fromSettings() override {
        pending_state = (Color)settings->current_preset.vco_state.engine;
        cvMatrix.la.array[page_id].setColor(pending_state);
    }

    void toSettings() override {
        settings->current_preset.vco_state.engine = (uint8_t)pending_state;
    }
};

class UiLooperPage : public StatefulUiPage<COL_PINK> {
    void fromSettings() override {
        pending_state = (Color)settings->current_preset.looper_state.engine;
        cvMatrix.la.array[page_id].setColor(pending_state);
    }

    void toSettings() override {
        settings->current_preset.looper_state.engine = (uint8_t)pending_state;
    }
};

class UiModulationPage : public StatefulUiPage<COL_PINK> {
    void fromSettings() override {
        pending_state = (Color)settings->current_preset.mod_state.engine;
        cvMatrix.la.array[page_id].setColor(pending_state);
    }

    void toSettings() override {
        settings->current_preset.mod_state.engine = (uint8_t)pending_state;
    }
};

class UiVoctPage : public BaseUiPage {
public:
    void fromSettings() override {
        // Looks like this function shouldn't do anything - cal data is
        // write-only in this menu.
    }

    void toSettings() override {
        // TODO: UPDATE VCO STATE
        auto cal_data = settings->mutable_calibration_data();
        // TODO set cal_data properties from calibrator
        settings->SavePersistentData();

        // cal_data->offset = calibrator.getOffset();
        // cal_data->scale = calibrator.getScale();
    }

    // void nextState();
    // void render() override;
    void onLoopButtonPressed() override;
    void onButtonDepressed() override;
};

class UiVolumePage : public BaseUiPage {
public:
    void fromSettings() override {
        // This menu is write-only, at least until we can restore
        // pot state when switching states
    }

    void toSettings() override {
        auto codec_settings = settings->mutable_codec_settings_data();
        codec_settings->in_gain = in_gain;
        codec_settings->out_gain = out_gain;
        settings->SavePersistentData();
    }

    // void nextState() override;
    // void render() override;
    void onLoopButtonPressed() override;
    void onButtonDepressed() override;

private:
    uint16_t in_gain, out_gain;
};

class UiPresetSavePage : public StatefulUiPage<COL_ORANGE> {
public:
    void fromSettings() override {
        pending_state = (Color)last_saved_preset_id;
        cvMatrix.la.array[page_id].setColor(pending_state);
    }

    void toSettings() override {
        if (pending_state) {
            // Use memcpy?
            *(settings->mutable_preset_data(pending_state - 1)) =
                settings->current_preset;
            settings->SavePersistentData();
            last_saved_preset_id = (uint8_t)pending_state - 1;
        }
    }

private:
    uint8_t last_saved_preset_id;
};

class UiPresetLoadPage : public StatefulUiPage<COL_ORANGE> {
public:
    void fromSettings() override {
        pending_state = (Color)last_loaded_preset_id;
        cvMatrix.la.array[page_id].setColor(pending_state);
    }

    void toSettings() override {
        if (pending_state) {
            settings->current_preset = settings->preset_data(pending_state - 1);
            settings->SavePersistentData();
            last_loaded_preset_id = (uint8_t)pending_state - 1;
        }
    }

private:
    uint8_t last_loaded_preset_id;
};

}

#endif