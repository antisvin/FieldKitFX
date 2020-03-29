#ifndef _DSP_DSP_H_
#define _DSP_DSP_H_

#include "engine/audio_buffers.h"
#include "hardware/adc.h"
#include "hardware/colors.h"
#include "utils/moving_average_filter.h"
#include "utils/utils.h"

namespace fieldkitfx {

constexpr size_t dsp_filter_size = 8;
constexpr size_t shared_buffer_size = 2048;
using DspParam = MovingAverageFilter<uint32_t, dsp_filter_size>;

class DspEffect {
private:
    DISALLOW_COPY_AND_ASSIGN(DspEffect);

protected:
    static const Color color;

public:
    DspEffect() = default;

    // TODO: look into block-based rendering to reduce number of function calls
    virtual void process(const float* in, float* out) = 0;

    virtual void updateParams(DspParam* param1, DspParam* param2);

    // Note: if an effect uses shared_buffer, it should reset it in this function
    virtual void reset() {};
};

}
#endif