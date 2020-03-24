/*
 * oscillator.h
 *
 */

#ifndef OSCILLATOR_H_
#define OSCILLATOR_H_

#include "utils/utils.h"
#include "engine/engine.h"

#define OSC_RES 1024 // size of the LUT
#define OSC_OVF_VALUE 4294967296 // 2^32
#define OSC_PHASE_FACTOR ((float)OSC_RES) / ((float)OSC_OVF_VALUE)

//#define OSC_INTERP

namespace fieldkitfx {

class QuadratureOscillator {
private:
    uint32_t phaseAcc;
    uint32_t increment;
    uint8_t freq_neg; // sign of the frequency, 0 if positive, 1 if negative
    // the outputs of the oscillator
    // for reference during the warping process
    float frequency;
    DISALLOW_COPY_AND_ASSIGN(QuadratureOscillator);

public:
    QuadratureOscillator() = default;
    float sineOut, cosineOut;
    void setFreq(float freq);
    void update(void);
};

}
#endif /* OSCILLATOR_H_ */
