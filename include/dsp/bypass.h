#ifndef _DSP_BYPASS_H_
#define _DSP_BYPASS_H_

#include "dsp/dsp.h"
#include "utils/utils.h"

namespace fieldkitfx {

class BypassEffect : public DspEffect {
public:
    BypassEffect() = default;
    void process(const float* in, float* out) override {
        std::copy(in, in + USER_AUDIO_IO_BUFFER_SIZE, out);
    }

    void updateParams(DspParam* param1, DspParam* param2) {};

private:
    DISALLOW_COPY_AND_ASSIGN(BypassEffect);
};

}

#endif