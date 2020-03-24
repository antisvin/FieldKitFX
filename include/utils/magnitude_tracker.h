/*
 * magnitudeTracker.h
 *
 */

#ifndef MAGNITUDETRACKER_H_
#define MAGNITUDETRACKER_H_

#include "utils/utils.h"

/*
 * A fairly simple magnitude tracker. Used in the calibration mode to track
 * the magnitude of the incoming audio signal.
 */

// this defines the time (in samples) that a maximum value is hold
#define MAX_HOLD_TIME 24000

namespace fieldkitfx {

class MagnitudeTracker {
private:
    DISALLOW_COPY_AND_ASSIGN(MagnitudeTracker);

protected:
    float trackMax;

public:
    uint16_t trackTime;

    MagnitudeTracker() {};
    void processSample(float sample);
    float getMax(void);
};

}

#endif /* MAGNITUDETRACKER_H_ */
