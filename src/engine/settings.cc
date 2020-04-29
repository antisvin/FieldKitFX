#include "engine/settings.h"

namespace fieldkitfx {

bool Settings::Init() {
    persistent_data_.codec_settings_data.in_gain = 127U;
    persistent_data_.codec_settings_data.out_gain = 127U;

    current_preset.fx_state[0].engine = 1U;
    current_preset.looper_state.engine = 1U;
    current_preset.mod_state.engine = 1U;
    bool success = chunk_storage_.Init(&persistent_data_, &state_);
    return success;
}

void Settings::SavePersistentData() {
    chunk_storage_.SavePersistentData();
}

void Settings::SaveState() {
    chunk_storage_.SaveState();
}

Settings settings;

}