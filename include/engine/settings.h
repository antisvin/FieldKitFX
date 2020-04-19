#ifndef _ENGINE_SETTINGS_H_
#define _ENGINE_SETTINGS_H_

#include "hardware/flash.h"
#include "utils/utils.h"

namespace fieldkitfx {

constexpr size_t num_presets = 4;

struct ChannelCalibrationData {
    float offset;
    float scale;
    inline float transform(float x) const {
        return x * scale + offset;
    }
};

struct FxData {
    uint8_t engine;
    uint16_t params[2];
};

struct VcoData {
    uint8_t engine;
    uint16_t params[2];
};

struct LooperData {
    uint8_t engine;
    uint16_t params[4];
};

struct ModulationData {
    uint8_t engine;
    uint16_t params[4];
};

struct PresetData {
    FxData fx_state[4];
    VcoData vco_state;
    LooperData looper_state;
    ModulationData mod_state;
};

struct CodecSettingsData {
    uint8_t in_gain;
    uint8_t out_gain;
};

struct PersistentData {
    ChannelCalibrationData input_calibration_data;
    PresetData presets[num_presets];
    CodecSettingsData codec_settings_data;
    uint8_t padding[16];
    enum { tag = 0x494C4143 }; // CALI
};

struct State {
    uint8_t mod_matrix[11];
    uint8_t padding[8];
    enum { tag = 0x54415453 }; // STAT
};

class Settings {
public:
    PresetData current_preset;

    Settings() {
    }
    ~Settings() {
    }

    bool Init();

    void SavePersistentData();
    void SaveState();

    inline const ChannelCalibrationData& calibration_data() const {
        return persistent_data_.input_calibration_data;
    }

    inline ChannelCalibrationData* mutable_calibration_data() {
        return &persistent_data_.input_calibration_data;
    }

    inline const PresetData& preset_data(size_t position) const {
        return persistent_data_.presets[position];
    }

    inline PresetData* mutable_preset_data(size_t position) {
        return &persistent_data_.presets[position];
    }

    inline const CodecSettingsData& codec_settings_data() const {
        return persistent_data_.codec_settings_data;
    }

    inline CodecSettingsData* mutable_codec_settings_data() {
        return &persistent_data_.codec_settings_data;
    }

    inline const State& state() const {
        return state_;
    }

    inline State* mutable_state() {
        return &state_;
    }

    void loadPreset(uint8_t preset_id) {
        current_preset = persistent_data_.presets[preset_id];
    }

    void storePreset(uint8_t preset_id) {
        persistent_data_.presets[preset_id] = current_preset;
        SavePersistentData();
    }

private:
    PersistentData persistent_data_;
    State state_;

    ChunkStorage<0x8038000, 0x8040000, PersistentData, State> chunk_storage_;

    DISALLOW_COPY_AND_ASSIGN(Settings);
};

}

#endif