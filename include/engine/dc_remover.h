/*
 * DCRemover.h
 *
 * A single pole high pass.
 */

#ifndef DCREMOVER_H_
#define DCREMOVER_H_

#include "utils/utils.h"

namespace fieldkitfx {

class DCRemover {
private:
    float pole;
    float old_x;
    float old_y;
    DISALLOW_COPY_AND_ASSIGN(DCRemover);

public:
    DCRemover()
        : DCRemover(0.995) {};
    DCRemover(float pole);
    float process(const float in);
};

/*
 * Function definitions
 */
}
#endif /* DCREMOVER_H_ */
