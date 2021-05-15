#ifndef _SVF_H_
#define _FILTERS_TRIPLE_PEAKS_H_

#include <cmath>
#include "dsp/dsp.h"
#include "engine/clipping.h"
#include "engine/svf.h"

namespace fieldkitfx {

class StateVariableFilterEffect : public DspEffect {
public:
    StateVariableFilterEffect()
        : mode(StateVariableFilter::Mode::SVF_LO) {
    }

    void process(const float* input, float* output) override {
        filter.process(input, output, USER_AUDIO_IO_BUFFER_SIZE);
        clipper.process(output, output, USER_AUDIO_IO_BUFFER_SIZE);
    }

    void setMode(StateVariableFilter::Mode mode) {
        this->mode = mode;
    }
    void updateParams(const DspParam* param1, const DspParam* param2) {
        float fc = param1->asFloat() / ADC_RESOLUTION_DEZ;
        fc = 0.02 + fc * 0.98f;
        float q = ((float)param2->asFloat() / ADC_RESOLUTION_DEZ) * 2.99f + 0.02f;
        switch (mode) {
        case StateVariableFilter::Mode::SVF_LO:
            filter.setLowPass(fc, q);
            break;
        case StateVariableFilter::Mode::SVF_HI:
            filter.setHighPass(fc, q);
            break;
        case StateVariableFilter::Mode::SVF_BP:
            filter.setBandPass(fc, q);
            break;
        case StateVariableFilter::Mode::SVF_NO:
            filter.setNotch(fc, q);
            break;
        case StateVariableFilter::Mode::SVF_PK:
            filter.setPeak(fc, q);
            break;
        case StateVariableFilter::Mode::SVF_BL:
            filter.setBell(fc, q, default_gain);
            break;
        case StateVariableFilter::Mode::SVF_LS:
            filter.setLowShelf(fc, q, default_gain);
            break;
        case StateVariableFilter::Mode::SVF_HS:
            filter.setHighShelf(fc, q, default_gain);
            break;
        default:
            break;
        }
    }

private:
    StateVariableFilter::Mode mode;
    StateVariableFilter filter;
    HardClipper clipper;
    static constexpr float default_gain = 40.f;
};

}
#endif
