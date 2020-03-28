#ifndef _EFFECTS_H_
#define _EFFECTS_H_

#include "dsp/decimator.h"
#include "dsp/frequency_shifter.h"
#include "utils/moving_average_filter.h"
#include "utils/utils.h"

namespace fieldkitfx {

class EffectsLibrary {
private:
    MovingAverageFilter<uint32_t, dsp_filter_size> param1, param2;

    DecimatorEffect decimator {};
    FrequencyShifterEffect frequency_shifter {};

    static constexpr uint8_t num_effects = 2;
    DspEffect* effects[num_effects] = { &frequency_shifter, &decimator };
    DspEffect* current_effect;

    DISALLOW_COPY_AND_ASSIGN(EffectsLibrary);

public:
    EffectsLibrary();

    void nextEffect();

    void updateParams();

    void render(const float* in, float* out);
};

extern EffectsLibrary effects_library;
}

#endif