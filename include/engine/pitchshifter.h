#ifndef _ENGINE_PITCHSHIFTER_H_
#define _ENGINE_PITCHSHIFTER_H_

#include "engine/delay_line.h"
#include "stmlib/dsp/dsp.h"

using namespace stmlib;

namespace fieldkitfx {

template <size_t window_size, size_t crossfade_size = 20>
class Pitchshifter {
public:
    void init(float* buf) {
        lines[0].Init(buf);
        lines[1].Init(buf + window_size);
    }

    void reset() {
        lines[0].Reset();
        lines[1].Reset();
    }

    inline float process(float in) {
        delay_len = fmodf(delay_len + shift + window_size, window_size);
        float sample_a = lines[0].WriteRead(in, delay_len);
        float sample_b = lines[1].WriteRead(in, delay_len + window_size);
        return Crossfade(sample_a, sample_b,
            1.0f - std::min(delay_len / crossfade_size, 1.0f));
    }

    void setScale(float scale) {
        shift = 1.0f - scale;
    }

private:
    DelayLine<float, 512> lines[2];
    float delay_len, shift;
};

}

#endif