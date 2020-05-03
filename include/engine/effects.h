#ifndef _ENGINE_EFFECTS_H_
#define _ENGINE_EFFECTS_H_

//#include "dsp/allpass_filter.h"
#include "dsp/barberpole_phaser.h"
#include "dsp/bypass.h"
#include "dsp/chorus.h"
#include "dsp/comb_filter.h"
#include "dsp/decimator.h"
#include "dsp/distfolder.h"
#include "dsp/frequency_shifter.h"
#include "dsp/phaser.h"
#include "dsp/tz_flanger.h"
#include "utils/moving_average_filter.h"
#include "utils/utils.h"

namespace fieldkitfx {

enum EffectAlgo {
    DSP_BYPASS = 0,
    DSP_BARBERPOLE_PHASER,
    DSP_TZ_FLANGER,
    DSP_PHASER,
    DSP_CHORUS,
    DSP_DECIMATOR,
};

//__attribute__((section(".ccmram")))
// static float shared_buffer[shared_buffer_size];

template <size_t shared_buffer_size>
class SharedBuffer {
public:
    float shared_buffer[shared_buffer_size];

    template <typename Effect>
    void initBuffer(Effect** effects) {
        for (size_t i = 1; i < sizeof(effects); i++) {
            effects[i]->init(shared_buffer);
        }
    }
};

class EffectsLibrary : public SharedBuffer<shared_buffer_size> {
public:
    EffectAlgo algo;
    bool refreshUi = true;
    // float shared_buffer[shared_buffer_size];
    DspParam param1, param2;

    EffectsLibrary() {
        SharedBuffer<shared_buffer_size>::initBuffer<DspEffect>(effects);
        algo = (EffectAlgo)(1);
    }
    void nextEffect();
    void updateParams();
    void process(const float* in, float* out);
    DspEffect* getCurrentEffect() {
        return effects[algo];
    }

private:
    BypassEffect bypass;
    DecimatorEffect decimator;
    BarberpolePhaserEffect barberpole_phaser;
    TzFlangerEffect tz_flanger;
    // AllpassFilterEffect allpass_filter;
    // CombFilterEffect comb_filter;
    ChorusEffect chorus;
    // DistFolderEffect dist_folder;
    PhaserEffect phaser;

    static constexpr uint8_t num_effects = 6;
    DspEffect* effects[num_effects] = {
        &bypass,
        &barberpole_phaser,
        &tz_flanger,
        &phaser,
        //&allpass_filter,
        //&comb_filter,
        &chorus,
        &decimator,
    };

    DISALLOW_COPY_AND_ASSIGN(EffectsLibrary);
};

extern EffectsLibrary effects_library;
}

#endif