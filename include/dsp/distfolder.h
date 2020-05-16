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
            float tmp = ap.Allpass(in[i], 0.0f, ap_coef);
            out[i] = wavefolder.process(tmp * gain);
        }
    };

    void updateParams(const DspParam* param1, const DspParam* param2) {
        ap_coef = param1->asFloat() / ADC_RESOLUTION_DEZ;
        gain = param2->asFloat() * (max_gain - 1) / ADC_RESOLUTION_DEZ + 1;
    };

private:
    Wavefolder wavefolder;
    DelayLine<float, 1> ap;
    static constexpr uint8_t max_gain = 6;
    float gain, ap_coef;
};

};

#endif
