/*
 * frequencyShifter.h
 *
 * A frequency shifter based one ssb-modulation and iir-hilbert-transformer.
 */

#ifndef FREQUENCYSHIFTER_H_
#define FREQUENCYSHIFTER_H_

/*
 * DSP blocks includes
 */
#include "engine/dc_remover.h"
#include "engine/hilbert_transformer.h"
#include "engine/oscillator.h"
#include "engine/xfader.h"
#include "utils/moving_average_filter.h"
#include "utils/utils.h"

/*
 * Constants definition
 */
#define MAX_FBKAMT 0.33f
#define FREQLOCK_THRESHOLD 8

namespace fieldkitfx {

constexpr size_t FSMAFILTER_LEN = 8;

/*
 * Structure definition
 */
class FrequencyShifter {
private:
    float fbkAmt;
    float inputSample, outputSample, fbkSample;
    DISALLOW_COPY_AND_ASSIGN(FrequencyShifter);

public:
    //	The CV input filters
    MovingAverageFilter<uint32_t, FSMAFILTER_LEN> shiftAmountMAFilter;
    MovingAverageFilter<uint32_t, FSMAFILTER_LEN> sidebandControlMAFilter;

    QuadratureOscillator localOscillator {};
    HilbertTransformer transformer {};
    Xfader xfader {};
    DCRemover dcFilter { 0.99f };

    FrequencyShifter() = default;
    void setShiftAmt(float freq);
    void setSideBandXfade(float sidebandCoeff); // coeff € [0, 1] -> [LSB, DSB, USB]
    void setFbkAmt(float fbkAmt);
    void processSample(float* sample);
    void updateCVs();
};

}
#endif /* FREQUENCYSHIFTER_H_ */
