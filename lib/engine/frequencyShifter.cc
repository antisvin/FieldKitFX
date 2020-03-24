/*
 * frequencyShifter.cc
 *
 */

#include "frequencyShifter.h"
#include "ADC.h"
#include "freqCVLUT.h"
#include "movingAverageFilter.h"
#include "randomUtils.h"
#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"

/*
 * Global definitions of the frequency shifter's components
 */

void FrequencyShifter::setShiftAmt(float freq) {
	localOscillator.setFreq(freq);
}

void FrequencyShifter::setSideBandXfade(float sidebandCoeff) {
	xfader.lambda = sidebandCoeff;
}

void FrequencyShifter::setFbkAmt(float fbkAmt_) {
	fbkAmt = fbkAmt_;
}

void FrequencyShifter::processSample(float *sample) {
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
	sample_I = xfader.lambda * sample_USB + (1.0 - xfader.lambda) * sample_LSB;

	*sample = clip(sample_I);
}

void FrequencyShifter::updateCVs() {
	// scale the CV values and update the CV filters
	volatile uint32_t temp_amount = ADC_getMixedCV2() & (0xff0);

	/*
   * WITH CV FILTERING
   */

	shiftAmountMAFilter.process(temp_amount);
	sidebandControlMAFilter.process(ADC_getMixedCV1());

	// then make the actual changes
	setShiftAmt(
		freqCVLUT[ADC_RESOLUTION_DEZ - shiftAmountMAFilter.output]);
	setSideBandXfade(
		(float)(sidebandControlMAFilter.output) / (float)(ADC_RESOLUTION_DEZ));
}
