#ifndef _DSP_DUAL_PITCHIFTER_H_
#define _DSP_DUAL_PITCHIFTER_H_

#include "dsp/dsp.h"
#include "engine/engine.h"
#include "engine/pitchshifter.h"

namespace fieldkitfx {

static constexpr size_t num_intervals = 9;
static const float odd_tones[9] = { 1.0f / 3, 4.0f / 5, 3.0f / 5, 2.0f / 3,
    1.0f, 3.0f / 2, 5.0f / 3, 5.0f / 4, 3.0f };
static const float even_tones[9] = { 1.0f / 2, 5.0f / 8, 5.0f / 6, 3.0f / 4,
    1.0f, 4.0f / 3, 6.0f / 5, 8.0f / 5, 2.0f };

class DualPitchshifterEffect : public DspEffect {
public:
    void init(float* shared_buffer) {
        pc[0].init(shared_buffer);
        pc[1].init(shared_buffer + 1024 * sizeof(float));
    }

    void reset() {
        pc[0].reset();
        pc[1].reset();
    }

    void process(const float* in, float* out) override {
        for (int i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            float sample_a = pc[0].process(in[i]);
            float sample_b = pc[1].process(in[i]);
            out[i] = (sample_a + sample_b) * 0.5;
        }
    }

    void updateParams(const DspParam* param1, const DspParam* param2) {
        float odd_tone_position = param1->asFloat() / ADC_RESOLUTION_DEZ * num_intervals;
        pc[0].setScale(odd_tones[(size_t)odd_tone_position]);
        float even_tone_position =
            param2->asFloat() / ADC_RESOLUTION_DEZ * num_intervals;
        pc[1].setScale(even_tones[(size_t)even_tone_position]);
    }

private:
    float odd_tone_position, even_tone_position;
    Pitchshifter<128> pc[2];
};

}

#endif