#ifndef _ENGINE_PARAMETERS_H_
#define _ENGINE_PARAMETERS_H_

#include "hardware/adc.h"
#include "hardware/rollodecks.h"
#include "utils/moving_average_filter.h"

namespace fieldkitfx {

constexpr size_t fx_filter_size = 8;
using FxParam = MovingAverageFilter<uint32_t, fx_filter_size>;

constexpr size_t adsr_filter_size = 8;
using AdsrParam = MovingAverageFilter<uint32_t, adsr_filter_size>;

class ParameterController {
public:
    FxParam fx_params[2];
    AdsrParam adsr_params[4];

    void updateParams() {
        fx_params[0].process(ADC_getMixedCV2());
        fx_params[1].process(ADC_getMixedCV1());
        for (uint8_t i = 0; i < 4; i++) {
            adsr_params[i].process(rollodecks.cv_values[i]);
        }
    }
};

}

#endif