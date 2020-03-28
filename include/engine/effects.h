#ifndef _ENGINE_EFFECTS_H_
#define _ENGINE_EFFECTS_H_

#include "dsp/bypass.h"
#include "dsp/decimator.h"
#include "dsp/frequency_shifter.h"
#include "utils/moving_average_filter.h"
#include "utils/utils.h"

namespace fieldkitfx {

enum EffectAlgo {
    DSP_BYPASS = 0,
    DSP_FREQUENCY_SHIFTER,
    DSP_DECIMATOR,
};

class EffectsLibrary {
private:
    MovingAverageFilter<uint32_t, dsp_filter_size> param1, param2;

    BypassEffect bypass {};
    DecimatorEffect decimator {};
    FrequencyShifterEffect frequency_shifter {};

    static constexpr uint8_t num_effects = 3;
    DspEffect* effects[num_effects] = { &bypass, &frequency_shifter, &decimator };

    DISALLOW_COPY_AND_ASSIGN(EffectsLibrary);

public:
    EffectsLibrary();
    EffectAlgo algo;
    bool refreshUi = true;

    void nextEffect();
    void updateParams();
    void process(const float* in, float* out);
};

extern EffectsLibrary effects_library;
}

#endif