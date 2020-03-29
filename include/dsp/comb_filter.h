#ifndef _DSP_COMB_FILTER_H_
#define _DSP_COMB_FILTER_H_

#include "dsp/dsp.h"
#include "engine/delay_line.h"
#include "utils/utils.h"

namespace fieldkitfx {

class CombFilterEffect : public DspEffect {
public:
    float delay, feedback;

    CombFilterEffect() = default;

    void init(float* buffer) {
        delay_line.Init(buffer);
    };

    void process(const float* in, float* out) override {
        for (size_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            if (feedback < 0) {
                // Feed forward comb
                delay_line.Write(in[i]);
                out[i] = in[i] - delay_line.Read(delay) * feedback;
            }
            else if (feedback > 0) {
                // Feed back comb
                out[i] = in[i] + delay_line.Read(delay) * feedback;
                delay_line.Write(out[i]);
            }
            else {
                // Pass-thru, but add data to delay line
                delay_line.Write(in[i]);
                out[i] = in[i];
            }
        }
    };
    void reset() {
        delay_line.Reset();
    }

    void updateParams(DspParam* param1, DspParam* param2) {
        delay = param1->asFloat() / ADC_RESOLUTION_DEZ * (shared_buffer_size - 1.0f);
        feedback = (param2->asFloat() * 2) / ADC_RESOLUTION_DEZ - 1.0f;
    }

private:
    DelayLine<float, shared_buffer_size> delay_line;
    DISALLOW_COPY_AND_ASSIGN(CombFilterEffect);
};

}

#endif