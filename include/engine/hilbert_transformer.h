/*
 * hilbertTransformer.h
 *
 */

#ifndef HILBERTTRANSFORMER_H_
#define HILBERTTRANSFORMER_H_

#include "utils/utils.h"

/*
 *
 *	Series allpass filters based Hilbert transformer
 *
 *		|------BR1----- out_1
 *	----|
 *		|------BR2----- out_2
 *
 *	each branch is composed of 6 1st order allpass filters in series
 *	the coefficients were pre-computed in matlab beforehand. See https://github.com/csound/csound/blob/develop/Opcodes/ugsc.c#L120
 *
 *	The first 6 coefficients of the "coefficients" field of the structure are the ones related to the first branch, the rest is for the 2nd branch
 */
#define NUM_ALLPASS 12
#define NUM_ALLPASS_BRANCH 6

namespace fieldkitfx {

class HilbertTransformer {
private:
    DISALLOW_COPY_AND_ASSIGN(HilbertTransformer);

public:
    float oldx[NUM_ALLPASS];
    float oldy[NUM_ALLPASS];
    float outputSample_1, outputSample_2;

    HilbertTransformer() = default;
    void process(float inputSample);
};
}

#endif /* HILBERTTRANSFORMER_H_ */
