#include "engine/settings.h"

namespace fieldkitfx {

bool Settings::Init() {
    persistent_data_.input_calibration_data.offset = 25.71f;
    persistent_data_.input_calibration_data.scale = -60.0f;
    persistent_data_.codec_settings_data.in_gain = 127U;
    persistent_data_.codec_settings_data.out_gain = 127U;

    bool success = chunk_storage_.Init(&persistent_data_, &state_);
    return success;
}

void Settings::SavePersistentData() {
    chunk_storage_.SavePersistentData();
}

void Settings::SaveState() {
    chunk_storage_.SaveState();
}

}