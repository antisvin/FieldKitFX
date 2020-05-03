#ifndef _DSP_DISTFOLDER_H_
#define _DSP_DISTFOLDER_H_

#include "dsp/dsp.h"
#include "engine/delay_line.h"
#include "engine/oscillator.h"
#include "engine/wavefolder.h"
#include "utils/utils.h"

namespace fieldkitfx {

class DistFolderEffect : public DspEffect {
public:
    void init(float* shared_buffer) {
        ap.Init(shared_buffer);
    }

    void reset() {
        ap.Reset();
    }

    void process(const float* in, float* out) override {
        for (size_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            lfo.update();
            float tmp = ap.Allpass(in[i], 1.0f, depth);
            out[i] = wavefolder.process(tmp * gain);
        }
    };

    void updateParams(const DspParam* param1, const DspParam* param2) {
        // offset = param1->asFloat() / 4095.0;
        depth = param1->asFloat() / ADC_RESOLUTION_DEZ;
        /*
        float lfo_param = param1->asFloat() / ADC_RESOLUTION_DEZ;
        if (lfo_param <= 0.5) {
            // CCW turn - constant rate, mod depth decreasing
            lfo.setFreq(1.0);
            depth = lfo_param * 2.0;
        }
        else {
            // Constant mod depth, rate increasing
            lfo.setFreq(lfo_param * 50.0 - 24);
            depth = 1.0;
        }
        // ap_delay = depth * (lfo.sineOut * 0.499 + 0.5);
        */

        gain = param2->asFloat() * (max_gain - 1) / ADC_RESOLUTION_DEZ + 1;
    };

private:
    Wavefolder wavefolder;
    DelayLine<float, 1> ap;
    SineOscillator lfo;
    static constexpr uint8_t max_gain = 6;
    float gain, depth;
};

};

#endif
