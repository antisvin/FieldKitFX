#ifndef _ENGINE_EFFECTS_H_
#define _ENGINE_EFFECTS_H_

#include "dsp/allpass_filter.h"
#include "dsp/bypass.h"
#include "dsp/comb_filter.h"
#include "dsp/decimator.h"
#include "dsp/frequency_shifter.h"
#include "utils/moving_average_filter.h"
#include "utils/utils.h"

namespace fieldkitfx {

enum EffectAlgo {
    DSP_BYPASS = 0,
    DSP_FREQUENCY_SHIFTER,
    DSP_DECIMATOR,
    DSP_ALLPASS,
    DSP_COMB,
};

//__attribute__((section(".ccmram")))
static float shared_buffer[shared_buffer_size];

class EffectsLibrary {
public:
    EffectAlgo algo;
    bool refreshUi = true;
    // float shared_buffer[shared_buffer_size];
    DspParam param1, param2;

    EffectsLibrary() {
        allpass_filter.init(shared_buffer);
        comb_filter.init(shared_buffer);
        algo = DSP_FREQUENCY_SHIFTER;
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
    FrequencyShifterEffect frequency_shifter;
    AllpassFilterEffect allpass_filter;
    CombFilterEffect comb_filter;

    static constexpr uint8_t num_effects = 5;
    DspEffect* effects[num_effects] = {
        &bypass,
        &frequency_shifter,
        &decimator,
        &allpass_filter,
        &comb_filter,
    };

    DISALLOW_COPY_AND_ASSIGN(EffectsLibrary);
};

extern EffectsLibrary effects_library;
}

#endif