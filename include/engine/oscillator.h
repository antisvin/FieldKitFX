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

class OscillatorBase {
public:
    OscillatorBase() = default;
    void setFreq(float freq);
    void update(void);

protected:
    bool freq_neg; // sign of the frequency, 0 if positive, 1 if negative
    uint32_t phaseAcc;
    uint32_t increment;
    float frequency;

private:
    DISALLOW_COPY_AND_ASSIGN(OscillatorBase);
};

class SineOscillator : public OscillatorBase {
public:
    float sineOut;
    void update(void);
};

class SawOscillator : public OscillatorBase {
public:
    float sawOut;
    void update(void);
};

class TriangleOscillator : public OscillatorBase {
public:
    float triangleOut;
    void update(void);
};

class QuadratureOscillator : public SineOscillator {
public:
    float cosineOut;
    void update(void);
};

}
#endif /* OSCILLATOR_H_ */
