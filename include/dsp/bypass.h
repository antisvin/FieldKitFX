#ifndef _DSP_BYPASS_H_
#define _DSP_BYPASS_H_

#include <algorithm>
#include "dsp/dsp.h"
#include "engine/audio_buffers.h"
#include "utils/utils.h"

namespace fieldkitfx {

class BypassEffect : public DspEffect {
public:
    BypassEffect() = default;
    void process(const float* in, float* out) override {
        std::copy(in, in + USER_AUDIO_IO_BUFFER_SIZE, out);
    }

private:
    DISALLOW_COPY_AND_ASSIGN(BypassEffect);
};

}

#endif