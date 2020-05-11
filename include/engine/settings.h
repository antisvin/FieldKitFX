#ifndef _ENGINE_SETTINGS_H_
#define _ENGINE_SETTINGS_H_

#include "hardware/flash.h"
#include "utils/utils.h"

namespace fieldkitfx {

constexpr size_t num_presets = 4;

// This is used for FX or filter slots
struct Slot2Data {
    uint8_t engine;
    uint16_t params[2];
    uint16_t mix;
    uint8_t chain;
    int16_t mod_influence[2];
    int16_t env_influence[2];
};

// This is used for looper and modulators
struct Slot4Data {
    uint8_t engine;
    uint16_t params[4];
};

struct PresetData {
    Slot2Data vcf_state;
    Slot2Data fx_state[3];
    Slot4Data looper_state;
    Slot4Data mod_state[2];
};

struct SettingsData {
    uint8_t last_preset_id;
    uint8_t in_gain;
    uint8_t out_gain;
};

struct PersistentData {
    PresetData presets[num_presets];
    SettingsData settings_data;
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

    inline const PresetData& preset_data(size_t position) const {
        return persistent_data_.presets[position];
    }

    inline PresetData* mutable_preset_data(size_t position) {
        return &persistent_data_.presets[position];
    }

    inline const SettingsData& settings_data() const {
        return persistent_data_.settings_data;
    }

    inline SettingsData* mutable_settings_data() {
        return &persistent_data_.settings_data;
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
        persistent_data_.settings_data.last_preset_id = preset_id;
        SavePersistentData();
    }

private:
    PersistentData persistent_data_;
    State state_;

    ChunkStorage<0x8038000, 0x8040000, PersistentData, State> chunk_storage_;

    DISALLOW_COPY_AND_ASSIGN(Settings);
};

extern Settings settings;

}

#endif