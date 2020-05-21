#ifndef _DSP_SHORT_DELAY_FILTER_H_
#define _DSP__FILTER_H_

#include "dsp/dsp.h"
#include "engine/delay_line.h"
#include "engine/oscillator.h"
#include "utils/utils.h"

namespace fieldkitfx {

class ShortDelayEffect : public DspEffect {
public:
    void init(float* buffer) {
        // int16_t* int_buf = reinterpret_cast<int16_t*>(buffer);
        delay.Init(reinterpret_cast<int16_t*>(buffer));
    }

    void reset() {
        delay.Reset();
    }

    void process(const float* in, float* out) override {
        for (size_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            float tmp = in[i] + prev * feedback;
            // TODO: clip here!
            prev = delay.WriteRead(tmp * 32767, delay_pos) / 32767;
            out[i] = prev;
        }
    };
    void updateParams(const DspParam* param1, const DspParam* param2) {
        delay_pos = param1->asFloat() * delay_size / (float)ADC_RESOLUTION_DEZ;
        feedback = param2->asFloat() / ADC_RESOLUTION_DEZ * 2 - 1;
    }

private:
    // constexpr static size_t delay_size = 32768;
    constexpr static size_t delay_size = 16384;
    DelayLine<int16_t, delay_size> delay;
    float delay_pos, feedback;
    float prev;
};

}

#endif