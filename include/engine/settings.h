#ifndef _ENGINE_SETTINGS_H_
#define _ENGINE_SETTINGS_H_

#include "hardware/flash.h"
#include "utils/utils.h"

namespace fieldkitfx {

constexpr size_t num_presets = 4;

struct Slot2Data {
    uint8_t engine;
    uint16_t params[2];
};

struct Slot4Data {
    uint8_t engine;
    uint16_t params[4];
};

struct PresetData {
    Slot2Data vcf_state;
    Slot2Data fx_state[4];
    Slot4Data looper_state;
    Slot4Data mod_state;
};

struct CodecSettingsData {
    uint8_t in_gain;
    uint8_t out_gain;
};

struct GeneralSettingsData {
    uint8_t last_preset_id;
};

struct PersistentData {
    PresetData presets[num_presets];
    CodecSettingsData codec_settings_data;
    GeneralSettingsData general_settings_data;
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

    inline const CodecSettingsData& codec_settings_data() const {
        return persistent_data_.codec_settings_data;
    }

    inline CodecSettingsData* mutable_codec_settings_data() {
        return &persistent_data_.codec_settings_data;
    }

    inline const GeneralSettingsData& general_settings_data() const {
        return persistent_data_.general_settings_data;
    }

    inline GeneralSettingsData* mutable_general_settings_data() {
        return &persistent_data_.general_settings_data;
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
        persistent_data_.general_settings_data.last_preset_id = preset_id;
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