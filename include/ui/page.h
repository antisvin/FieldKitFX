#ifndef _UI_PAGE_H_
#define _UI_PAGE_H_

#include "engine/settings.h"
#include "engine/effects.h"
#include "hardware/colors.h"
#include "hardware/loop_button.h"
#include "hardware/rgb_led.h"
#include "hardware/routing_matrix.h"

namespace fieldkitfx {

constexpr uint8_t num_ui_pages = 11U;

enum UiPageId {
    UI_FX1,
    UI_FX2,
    UI_FX3,
    UI_FX4,
    UI_LOOPER,
    UI_MODULATION,
    UI_PARAMETERS,
    UI_VOLUME,
    UI_PRESET_RANDOM,
    UI_PRESET_SAVE,
    UI_PRESET_LOAD,
};

class BaseUiPage {
public:
    BaseUiPage(UiPageId page_id)
        : page_id(page_id) {};

    BaseUiPage(UiPageId page_id, EffectsLibraryBase* effects_library)
        : effects_library(effects_library)
        , page_id(page_id) {};

    void reset() {
        cvMatrix.la.setIntensity((uint8_t)page_id, CV_RGB_LED_INTENSITY);
        fromSettings();
    }

    virtual void fromSettings() = 0;
    virtual void toSettings() = 0;
    virtual void onLoopButtonPressed(bool shouldSave) = 0;
    virtual void onButtonPressed() = 0;
    virtual void onButtonDepressed() = 0;

    void blink() {
        if (cvMatrix.ba.isPressed())
            loopButton.setColor(COL_WHITE);
        cvMatrix.la.setColor((uint8_t)page_id, COL_WHITE);
    }

    void renderState() {
        loopButton.setColor(pending_state);
        cvMatrix.la.setColor((uint8_t)page_id, last_state);
    }
    EffectsLibraryBase* effects_library;

protected:
    Color pending_state, last_state;
    UiPageId page_id;
};

template <Color max_color>
class StatefulUiPage : public BaseUiPage {
public:
    StatefulUiPage(UiPageId page_id)
        : BaseUiPage(page_id) {};

    StatefulUiPage(UiPageId page_id, EffectsLibraryBase* effects_library)
        : BaseUiPage(page_id, effects_library) {};

    void onLoopButtonPressed(bool shouldSave) override {
        if (pending_state == max_color) {
            pending_state = COL_NONE;
        }
        else {
            pending_state = (Color)((uint8_t)pending_state + 1);
        }
        if (shouldSave) {
            last_state = pending_state;
            toSettings();
        }
    }

    void onButtonPressed() override {
        fromSettings();
    }

    void onButtonDepressed() final {
        last_state = pending_state;
        toSettings();
    };
};

class UiFxPage : public StatefulUiPage<COL_RED> {
public:
    UiFxPage(UiPageId page_id, EffectsLibraryBase* effects_library)
        : StatefulUiPage(page_id, effects_library) {};

    void onButtonPressed() override;

    void fromSettings() override {
        pending_state = (Color)settings.current_preset.fx_state[page_id].engine;
        last_state = pending_state;
        cvMatrix.la.array[page_id].setColor(pending_state);
    }

    void toSettings() override {
        settings.current_preset.fx_state[page_id].engine = (uint8_t)pending_state;
        effects_library->setAlgo((uint8_t)pending_state);
    }
};

class UiLooperPage : public StatefulUiPage<COL_PINK> {
public:
    UiLooperPage(UiPageId page_id)
        : StatefulUiPage<COL_PINK>(page_id) {};

    void fromSettings() override {
        pending_state = (Color)settings.current_preset.looper_state.engine;
        last_state = pending_state;
        cvMatrix.la.array[page_id].setColor(pending_state);
    }

    void toSettings() override {
        settings.current_preset.looper_state.engine = (uint8_t)pending_state;
    }
};

class UiModulationPage : public StatefulUiPage<COL_PINK> {
public:
    UiModulationPage(UiPageId page_id)
        : StatefulUiPage<COL_PINK>(page_id) {};

    void fromSettings() override {
        pending_state = (Color)settings.current_preset.mod_state[0].engine;
        last_state = pending_state;
        cvMatrix.la.array[page_id].setColor(pending_state);
    }

    void toSettings() override {
        settings.current_preset.mod_state[0].engine = (uint8_t)pending_state;
    }
};

class UiParametersPage : public StatefulUiPage<COL_NONE> {
public:
    UiParametersPage(UiPageId page_id)
        : StatefulUiPage<COL_NONE>(page_id) {};

    // This page has 4 subpages: envelope modulation, parameters modulation,
    // feedback levels, unit chains

    void fromSettings() override {
        // pending_state = (Color)settings.current_preset.mod_state.engine;
        last_state = pending_state;
        cvMatrix.la.array[page_id].setColor(pending_state);
    }

    void toSettings() override {
        // settings.current_preset.mod_state.engine = (uint8_t)pending_state;
    }
};

class UiVolumePage : public BaseUiPage {
public:
    UiVolumePage(UiPageId page_id)
        : BaseUiPage(page_id) {};

    void fromSettings() override {
        // This menu is write-only, at least until we can restore
        // pot state when switching states
    }

    void toSettings() override {
        auto settings_data = settings.mutable_settings_data();
        settings_data->in_gain = in_gain;
        settings_data->out_gain = out_gain;
        settings.SavePersistentData();
    }

    // void nextState() override;
    // void render() override;
    void onLoopButtonPressed(bool shouldSave) override;
    void onButtonPressed() override;
    void onButtonDepressed() override;

private:
    uint16_t in_gain, out_gain;
};

class UiPresetRandomPage : public BaseUiPage {
public:
    UiPresetRandomPage(UiPageId page_id)
        : BaseUiPage(page_id) {};

    void fromSettings() override {
    }

    void toSettings() override {
    }

    void onLoopButtonPressed(bool shouldSave) override;
    void onButtonPressed() override {};
    void onButtonDepressed() override {};
};

class UiPresetSavePage : public StatefulUiPage<COL_ORANGE> {
public:
    UiPresetSavePage(UiPageId page_id)
        : StatefulUiPage<COL_ORANGE>(page_id) {};

    void fromSettings() override {
        pending_state = (Color)last_saved_preset_id;
        last_state = pending_state;
        cvMatrix.la.array[page_id].setColor(pending_state);
    }

    void toSettings() override {
        if (pending_state) {
            // Use memcpy?
            *(settings.mutable_preset_data(pending_state - 1)) = settings.current_preset;
            settings.SavePersistentData();
            last_saved_preset_id = (uint8_t)pending_state - 1;
        }
    }

private:
    uint8_t last_saved_preset_id;
};

class UiPresetLoadPage : public StatefulUiPage<COL_ORANGE> {
public:
    UiPresetLoadPage(UiPageId page_id)
        : StatefulUiPage<COL_ORANGE>(page_id) {};

    void fromSettings() override {
        pending_state = (Color)last_loaded_preset_id;
        last_state = pending_state;
        cvMatrix.la.array[page_id].setColor(pending_state);
    }

    void toSettings() override {
        if (pending_state) {
            settings.current_preset = settings.preset_data(pending_state - 1);
            settings.SavePersistentData();
            last_loaded_preset_id = (uint8_t)pending_state - 1;
        }
    }

private:
    uint8_t last_loaded_preset_id;
};

class ParameterController {
public:
    ParameterController(BaseUiPage* params2, BaseUiPage* params4)
        : params2_page(params2)
        , params4_page(params4) {};

    void setParams2Page(BaseUiPage* page) {
        // params2_page->lockParams();
        params2_page = page;
    }
    void setParams4Page(BaseUiPage* page) {
        // params4_page->lockParams();
        params4_page = page;
    }
    void updateParams2() {
        params2_page->effects_library->updateParams();
    };
    void updateParams4() {
        // params4_page->updateParams();
    }

private:
    BaseUiPage* params2_page;
    BaseUiPage* params4_page;
};

extern ParameterController parameter_controller;

}

#endif
