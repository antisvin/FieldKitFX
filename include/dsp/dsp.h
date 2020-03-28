#ifndef _DSP_DSP_H_
#define _DSP_DSP_H_

#include "hardware/adc.h"
#include "hardware/colors.h"
#include "utils/utils.h"

namespace fieldkitfx {

constexpr size_t dsp_filter_size = 8;

class DspEffect {
private:
    DISALLOW_COPY_AND_ASSIGN(DspEffect);

protected:
    uint16_t last_param1, last_param2;
    static const Color color;

public:
    DspEffect() = default;

    // TODO: look into block-based rendering to reduce number of function calls
    virtual void process(const float* in, float* out) = 0;

    virtual void updateParams(float param1, float param2) {
        last_param1 = param1;
        last_param2 = param2;
    }

    // virtual void reset() {};
};

}
#endif