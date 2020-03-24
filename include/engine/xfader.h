/*
 * Xfader.h
 *
 */

#ifndef XFADER_H_
#define XFADER_H_

#include "utils/utils.h"

namespace fieldkitfx {

class Xfader {
private:
    DISALLOW_COPY_AND_ASSIGN(Xfader);

public:
    float lambda;

    Xfader() = default;
    float process(float in1, float in2) {
        return lambda * in1 + (1.0 - lambda) * in2;
    }
};

}
#endif /* XFADER_H_ */
