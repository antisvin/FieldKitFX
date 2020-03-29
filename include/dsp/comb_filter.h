#ifndef _DSP_COMB_FILTER_H_
#define _DSP_COMB_FILTER_H_

#include <array>
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
            // delay_line.Write(in[i]);
            // out[i] = in[i] + delay_line.Read(delay) * feedback;

            // out[i] = delay_line.Read(delay) * feedback + in[i];
            // delay_line.Write(out[i]);

            float tmp1 = delay_line.Read(delay);
            float tmp2 = in[i] - tmp1 * feedback;
            delay_line.Write(tmp2);
            out[i] = tmp1 + tmp2 * feedback;
            // out[i] = delay_line.Read(delay)
        }
    };
    void reset() {
        delay_line.Reset();
    }

    void updateParams(DspParam* param1, DspParam* param2) {
        delay = param1->asFloat() / 4095.0f * (shared_buffer_size - 1);
        feedback = param2->asFloat() / 4095.0f;
    }

private:
    DelayLine<float, shared_buffer_size> delay_line;
    DISALLOW_COPY_AND_ASSIGN(CombFilterEffect);
};

}

#endif