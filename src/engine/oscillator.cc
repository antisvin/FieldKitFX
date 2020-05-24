/*
 * oscillator.c
 *
 */
#include <math.h>
#include "engine/oscillator.h"
#include "utils/moving_average_filter.h"
#include "utils/wavetables.h"

namespace fieldkitfx {

void OscillatorBase::setFreq(float freq) {
    // set the frequency field
    frequency = freq;
    // check the sign of the new frequency
    // negative frequencies
    if (freq < 0.0f) {
        freq_neg = true;
        increment =
            (uint32_t)((float)OSC_OVF_VALUE * (((-freq)) / (float)F_SAMPLING));
    }

    // positive frequencies
    // basically the same deal than for the negative ones (just a sign thing on the sine output)
    else {
        freq_neg = false;
        increment = (uint32_t)((float)OSC_OVF_VALUE * ((freq) / (float)F_SAMPLING));
    }
}

void Phasor::update() {
    if (freq_neg) {
        phaseAcc -= increment;
    }
    else {
        phaseAcc += increment;
    };
    phaseOut = (float)phaseAcc / OSC_OVF_VALUE;
}

void SawOscillator::update() {
    if (freq_neg) {
        phaseAcc -= increment;
    }
    else {
        phaseAcc += increment;
    };
    sawOut = (float)phaseAcc / OSC_OVF_VALUE * 2 - 1.0f;
}

void TriangleOscillator::update() {
    if (freq_neg) {
        phaseAcc -= increment;
    }
    else {
        phaseAcc += increment;
    };
    triangleOut = (float)phaseAcc / OSC_OVF_VALUE * 2;
    if (triangleOut > 1.0) {
        triangleOut = 1.0 - triangleOut;
    }
}

void SineOscillator::update() {
    // check if we should read the LUT backwards (neg freq) or not
    if (freq_neg) {
        phaseAcc -= increment;
    }
    else {
        phaseAcc += increment;
    }

    float temp = phaseAcc * OSC_PHASE_FACTOR;
    uint32_t integerPart = temp; // the integer part of the index
    // float fractionalPart = temp - integerPart;

    // sample interpolation (linear)
    // the interpolation has to follow the LUT reading direction (forward or backward)
    int16_t nextIndex;

    if (freq_neg) {
        nextIndex = integerPart - 1;
        if (nextIndex < 0) { // if we arrive at the end of the wavetable
            nextIndex += OSC_RES;
        }
#ifdef OSC_INTERP
        float sample1 = sineWT[nextIndex];
        float sample2 = sineWT[integerPart];
        sineOut = (sample1 + (sample2 - sample1) * fractionalPart);
#else
        sineOut = sineWT[integerPart];
#endif
    }
    else {
        nextIndex = integerPart + 1;
        if (nextIndex >= OSC_RES) { // if we arrive at the end of the wavetable
            nextIndex = 0;
        }
#ifdef OSC_INTERP
        float sample1 = sineWT[integerPart];
        float sample2 = sineWT[nextIndex];
        sineOut = (sample1 + (sample2 - sample1) * fractionalPart);
#else
        sineOut = sineWT[integerPart];
#endif
    }
}

void QuadratureOscillator::update(void) {
    // check if we should read the LUT backwards (neg freq) or not
    if (freq_neg) {
        phaseAcc -= increment;
    }
    else {
        phaseAcc += increment;
    }

    float temp = phaseAcc * OSC_PHASE_FACTOR;
    uint32_t integerPart = temp; // the integer part of the index
    // float fractionalPart = temp - integerPart;

    // sample interpolation (linear)
    // the interpolation has to follow the LUT reading direction (forward or backward)
    int16_t nextIndex;

    if (freq_neg) {
        nextIndex = integerPart - 1;
        if (nextIndex < 0) { // if we arrive at the end of the wavetable
            nextIndex += OSC_RES;
        }
#ifdef OSC_INTERP
        float sample1 = sineWT[nextIndex];
        float sample2 = sineWT[integerPart];
        sineOut = (sample1 + (sample2 - sample1) * fractionalPart);
#else
        sineOut = sineWT[integerPart];
#endif
#ifdef OSC_INTERP
        sample1 = cosineWT[nextIndex];
        sample2 = cosineWT[integerPart];
        cosineOut = sample1 + (sample2 - sample1) * fractionalPart;
#else
        cosineOut = cosineWT[integerPart];
#endif
    }
    else {
        nextIndex = integerPart + 1;
        if (nextIndex >= OSC_RES) { // if we arrive at the end of the wavetable
            nextIndex = 0;
        }
#ifdef OSC_INTERP
        float sample1 = sineWT[integerPart];
        float sample2 = sineWT[nextIndex];
        sineOut = (sample1 + (sample2 - sample1) * fractionalPart);
#else
        sineOut = sineWT[integerPart];
#endif
#ifdef OSC_INTERP
        sample1 = cosineWT[integerPart];
        sample2 = cosineWT[nextIndex];
        cosineOut = sample1 + (sample2 - sample1) * fractionalPart;
#else
        cosineOut = cosineWT[integerPart];
#endif
    }
}
}