#ifndef _DSP_OVERDRIVE_H_
#define _DSP_OVERDRIVE_H_

#include "dsp/dsp.h"
#include "engine/clipping.h"
#include "engine/dc_remover.h"

namespace fieldkitfx {

class OverdriveEffect : public DspEffect {
public:
    OverdriveEffect() = default;

    void process(const float* in, float* out) override {
        for (size_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            out[i] = hard_clipper.process(in[i] * gain + offset);
            dc.process(out[i]);
        }
    }

    void updateParams(const DspParam* param1, const DspParam* param2) {
        gain = param1->asFloat() / ADC_RESOLUTION_DEZ * 3 + 1;
        offset = param2->asFloat() / ADC_RESOLUTION_DEZ - 0.5f;
    }

private:
    float gain;
    HardClipper hard_clipper;
    DCRemover dc;
    float offset;

    DISALLOW_COPY_AND_ASSIGN(OverdriveEffect);
};
}

#endif