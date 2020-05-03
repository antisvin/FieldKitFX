#ifndef _DSP_TZ_FLANGER_H_
#define _DSP_TZ_FLANGER_H_

#include "dsp/dsp.h"
#include "engine/hilbert_transformer.h"
#include "engine/oscillator.h"
#include "utils/freq_cv_lut.h"
#include "utils/utils.h"

namespace fieldkitfx {

class TzFlangerEffect : public DspEffect {
private:
    constexpr static uint16_t flanger_max_delay = 32;
    // constexpr float flanger_min_delay = 0.001f;
    float depth;
    SineOscillator lfo;
    DelayLine<float, flanger_max_delay / 2> dry_delay;
    DelayLine<float, flanger_max_delay> wet_delay;

public:
    void init(float* shared_buffer) {
        dry_delay.Init(shared_buffer);
        wet_delay.Init(shared_buffer + flanger_max_delay / 2);
    }

    void reset() {
        dry_delay.Reset();
        wet_delay.Reset();
    }

    void process(const float* in, float* out) override {
        for (int i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            lfo.update();
            float dry = dry_delay.WriteRead(in[i], flanger_max_delay / 2);
            float wet_delay_len =
                (float)(flanger_max_delay / 2) * (1.0f + lfo.sineOut * depth);
            float wet = wet_delay.WriteRead(in[i], wet_delay_len);
            *(out++) = (dry + wet) / 2;
        }
    }

    void updateParams(const DspParam* param1, const DspParam* param2) {
        depth = param1->asFloat() / ADC_RESOLUTION_DEZ;
        float rate =
            freqCVLUT[ADC_RESOLUTION_DEZ / 2 - param2->last_result / 2] * 0.001;
        lfo.setFreq(rate);
    }
};
}
#endif