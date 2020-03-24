/*
 * frequencyShifter.cc
 *
 */
#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#include "engine/frequency_shifter.h"
#include "hardware/adc.h"
#include "utils/freq_cv_lut.h"
#include "utils/moving_average_filter.h"

namespace fieldkitfx {

void FrequencyShifter::setShiftAmt(float freq) {
    localOscillator.setFreq(freq);
}

void FrequencyShifter::setSideBandXfade(float sidebandCoeff) {
    xfader.lambda = sidebandCoeff;
}

void FrequencyShifter::setFbkAmt(float fbkAmt_) {
    fbkAmt = fbkAmt_;
}

void FrequencyShifter::processSample(float* sample) {
    float sample_I = *sample;
    // remove DC component
    dcFilter.process(sample_I);
    // process the sample thru the hilbert transformer
    transformer.process(sample_I);
    // update the oscillator
    localOscillator.update();

    // compute the USB and LSB samples
    sample_I = localOscillator.sineOut * transformer.outputSample_1;
    float sample_Q = localOscillator.cosineOut * transformer.outputSample_2;

    float sample_USB = sample_I - sample_Q;
    float sample_LSB = sample_I + sample_Q;

    // crossfade the two sideband signals
    sample_I = xfader.process(sample_USB, sample_LSB);

    *sample = clip(sample_I);
}

void FrequencyShifter::updateCVs() {
    // scale the CV values and update the CV filters
    uint32_t amount = ADC_getMixedCV2();

    /*
     * WITH CV FILTERING
     */

    amount = shiftAmountMAFilter.process(amount & 0xfff);
    uint32_t control =
        sidebandControlMAFilter.process(ADC_getMixedCV1() & 0xfff) & 0xff0;

    // then make the actual changes
    setShiftAmt(freqCVLUT[ADC_RESOLUTION_DEZ - amount]);
    setSideBandXfade((float)control / ADC_RESOLUTION_DEZ);
}
}