#ifndef _DSP_ALLPASS_FILTER_H_
#define _DSP_ALLPASS_FILTER_H_

#include "dsp/dsp.h"
#include "utils/utils.h"

namespace fieldkitfx {

class AllpassFilterEffect : public DspEffect {
public:
    float delay, feedback;

    AllpassFilterEffect() = default;

    void init(float* shared_buffer) {
        delay_line.Init(shared_buffer);
    };

    void process(const float* in, float* out) override {
        for (size_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            out[i] = delay_line.Allpass(in[i], delay, feedback);
        }
    };
    void reset() {
        delay_line.Reset();
    }

    void updateParams(const DspParam* param1, const DspParam* param2) {
        delay = param1->asFloat() / ADC_RESOLUTION_DEZ * (shared_buffer_size - 1);
        feedback = param2->asFloat() / ADC_RESOLUTION_DEZ;
    }

private:
    DelayLine<float, shared_buffer_size> delay_line;
    DISALLOW_COPY_AND_ASSIGN(AllpassFilterEffect);
};

}

#endif