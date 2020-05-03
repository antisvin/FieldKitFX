#ifndef _DSP_CHORUS_FILTER_H_
#define _DSP_CHORUS_FILTER_H_

#include "dsp/dsp.h"
#include "engine/delay_line.h"
#include "engine/oscillator.h"
#include "utils/utils.h"

namespace fieldkitfx {

class ChorusEffect : public DspEffect {
public:
    constexpr static uint16_t max_depth = 256;
    constexpr static uint16_t pre_delay1 = 480;
    constexpr static uint16_t pre_delay2 = 700;

    void init(float* buffer) {
        delay1.Init(buffer);
        delay2.Init(buffer + delay_size);
    };

    void reset() {
        delay1.Reset();
        delay2.Reset();
    }

    void process(const float* in, float* out) override {
        for (size_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            lfo1.update();
            lfo2.update();
            float tmp = in[i];
            tmp += delay1.WriteRead(in[i], pre_delay1 + lfo1.sineOut * depth1) * gain1;
            tmp += delay2.WriteRead(in[i], pre_delay2 + lfo2.triangleOut * depth2) * gain2;
            out[i] = tmp / (1 + gain1 + gain2);
        }
    };
    void updateParams(const DspParam* param1, const DspParam* param2) {
        depth1 = param1->asFloat() / ADC_RESOLUTION_DEZ;
        if (depth1 <= 0.5f) {
            gain2 = 0.0f;
        }
        else {
            gain2 = depth1 * 2.0 - 1.0;
        }
        depth1 = std::min(depth1 * 2, 1.0f) * max_depth;

        float rate = (freqCVLUT[param2->last_result] / freqCVLUT[4095] + 1.001) * 0.49;
        lfo1.setFreq(rate);
        lfo2.setFreq(rate * 0.5);
    }

private:
    constexpr static size_t delay_size = 1024;
    DelayLine<float, delay_size> delay1, delay2;
    SineOscillator lfo1;
    TriangleOscillator lfo2;
    constexpr static float gain1 = 1.0f;
    float gain2;
    float depth1;
    constexpr static float depth2 = (float)max_depth;
};

}

#endif