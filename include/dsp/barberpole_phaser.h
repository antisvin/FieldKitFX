#ifndef _DSP_BARPERPOLE_PHASER_H_
#define _DSP_BARBERPOLE_PHASER_H_

#include "dsp/dsp.h"
#include "engine/delay_line.h"
#include "engine/hilbert_transformer.h"
#include "engine/oscillator.h"
#include "utils/freq_cv_lut.h"
#include "utils/utils.h"

namespace fieldkitfx {

class BarberpolePhaserEffect : public DspEffect {
public:
    void init(float* shared_buffer) {
        for (auto i = 0; i < num_stages; i++) {
            stages[i].Init(shared_buffer + i);
        }
    }

    void reset() {
        for (auto i = 0; i < num_stages; i++) {
            stages[i].Reset();
        }
    }

    void process(const float* in, float* out) override {
        for (int i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            float tmp = in[i]; // + prev_sample * feedback;
            tmp += prev_sample * feedback;
            // Active number of stages are written and read
            for (auto j = 0; j < num_stages; j++) {
                tmp = stages[j].Allpass(tmp, 0.0f, -0.5f);
            }

            transformer.process(tmp);
            oscillator.update();

            float sample_I = oscillator.sineOut * transformer.outputSample_1;
            float sample_Q = oscillator.cosineOut * transformer.outputSample_2;
            // float sample_USB = sample_I - sample_Q;
            // float sample_LSB = sample_I + sample_Q;

            prev_sample = clip(sample_I + sample_Q);
            *(out++) = (in[i] + prev_sample) * 0.5;
        }
    }

    void updateParams(DspParam* param1, DspParam* param2) {
        float amount = freqCVLUT[ADC_RESOLUTION_DEZ - param1->last_result] * 0.001;
        oscillator.setFreq(amount);

        feedback = (param2->asFloat() / ADC_RESOLUTION_DEZ * 2 - 1.0f) * 0.7f;
    }

private:
    QuadratureOscillator oscillator {};
    HilbertTransformer transformer {};
    constexpr static size_t num_stages = 32;
    DelayLine<float, 1> stages[num_stages];
    float prev_sample, feedback;

    // DelayLine<float, max_delay_size> ap[num_delay_lines];
};
}
#endif