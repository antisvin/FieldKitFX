#ifndef _DSP_PHASER_H_
#define _DSP_PHASER_H_

#include "dsp/dsp.h"
#include "engine/delay_line.h"
#include "utils/utils.h"

namespace fieldkitfx {

class PhaserEffect : public DspEffect {
public:
    constexpr static size_t num_stages = 16;
    constexpr static uint8_t stages_bits = 5;
    DelayLine<float, 1> stages[num_stages];

    PhaserEffect() = default;

    void init(float* shared_buffer) {
        for (size_t i = 0; i < num_stages; i++) {
            stages[i].Init(shared_buffer + i);
        }
    }

    void reset() {
        for (size_t i = 0; i < num_stages; i++) {
            stages[i].Reset();
        }
    }

    void process(const float* in, float* out) override {
        for (size_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            lfo.update();
            float tmp = in[i]; //+ prev_sample * feedback;
            // Active number of stages are written and read
            for (size_t j = 0; j < num_stages; j++) {
                tmp = stages[j].Allpass(tmp, 0.0f, ap_coef + lfo.sineOut * depth);
            }
            // prev_sample =
            out[i] = (in[i] + tmp) * 0.5f;
        }
    }

    void updateParams(const DspParam* param1, const DspParam* param2) {
        depth = param1->asFloat() / ADC_RESOLUTION_DEZ;
        if (depth <= 0.5f) {
            lfo.setFreq(0.133);
        }
        else {
            float rate =
                freqCVLUT[param1->last_result] / freqCVLUT[4095] * 0.867f + 0.133f;
            lfo.setFreq(rate);
            depth = 0.5f;
        }

        ap_coef = param2->asFloat() / ADC_RESOLUTION_DEZ - 0.5f;
    }

private:
    float depth, ap_coef;
    SineOscillator lfo;
    DISALLOW_COPY_AND_ASSIGN(PhaserEffect);
};
}

#endif