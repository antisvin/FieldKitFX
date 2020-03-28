#ifndef FIELDKITFX_SETTINGS_H_
#define FIELDKITFX_SETTINGS_H_

#include "stmlib/stmlib.h"
#include "hardware/adc.h"

namespace fieldkitfx {

struct ChannelCalibrationData {
    float offset;
    float scale;
    float offset[NB_CV_IN];

    inline float transform(float x) const {
        return x * scale + offset;
    }
};

struct State {
    uint8_t quality;
    uint8_t blend_parameter;
    uint8_t playback_mode;
    uint8_t blend_value[4];
    uint8_t padding;
};

struct SettingsData {
    ChannelCalibrationData calibration_data; // 48 bytes
    State state; // 8 bytes
    uint8_t padding[8];
};

class PersistentBlock;

class Settings {
public:
    Settings() {
    }
    ~Settings() {
    }

    void init();
    void savePersistentData();
    void saveState();

    inline const ChannelCalibrationData& calibration_data(int channel) const {
        return persistent_data_.channel_calibration_data[channel];
    }

    inline ChannelCalibrationData* mutable_calibration_data(int channel) {
        return &persistent_data_.channel_calibration_data[channel];
    }

    inline const State& state() const {
        return state_;
    }

    inline State* mutable_state() {
        return &state_;
    }

    DISALLOW_COPY_AND_ASSIGN(Settings);
};

} // namespace fieldkitfx

#endif // FIELDKITFX_SETTINGS_H_