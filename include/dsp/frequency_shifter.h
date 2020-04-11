#ifndef _DSP_FREQUENCY_SHIFTER_H_
#define _DSP_FREQUENCY_SHIFTER_H_

#include "dsp/dsp.h"
#include "engine/hilbert_transformer.h"
#include "engine/oscillator.h"
#include "utils/freq_cv_lut.h"
#include "utils/utils.h"

namespace fieldkitfx {

class FrequencyShifterEffect : public DspEffect {
private:
    QuadratureOscillator oscillator {};
    HilbertTransformer transformer {};
    float amount, control;

public:
    void process(const float* in, float* out) override {
        oscillator.setFreq(amount);

        for (int i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            float sample_I = *(in++);
            transformer.process(sample_I);
            oscillator.update();

            sample_I = oscillator.sineOut * transformer.outputSample_1;
            float sample_Q = oscillator.cosineOut * transformer.outputSample_2;
            float sample_USB = sample_I - sample_Q;
            float sample_LSB = sample_I + sample_Q;

            // crossfade the two sideband signals
            sample_I = crossfade(sample_USB, sample_LSB, control);

            *(out++) = clip(sample_I);
        }
    }

    void updateParams(DspParam* param1, DspParam* param2) {
        amount = freqCVLUT[ADC_RESOLUTION_DEZ - param1->last_result];
        // float control = (float)(last_param1 & 0xff0) / ADC_RESOLUTION_DEZ;
        control = param2->asFloat() / ADC_RESOLUTION_DEZ;
    }
};
}
#endif