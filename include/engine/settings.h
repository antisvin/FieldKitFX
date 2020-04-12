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

struct PersistentData {
    ChannelCalibrationData input_calibration_data;
    PresetData presets[num_presets];
    uint8_t in_gain;
    uint8_t out_gain;
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

    inline const State& state() const {
        return state_;
    }

    inline State* mutable_state() {
        return &state_;
    }

private:
    PersistentData persistent_data_;
    State state_;

    ChunkStorage<0x8038000, 0x8040000, PersistentData, State> chunk_storage_;

    DISALLOW_COPY_AND_ASSIGN(Settings);
};

}

#endif