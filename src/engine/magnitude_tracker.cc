/*
 * magnitudeTracker.c
 *
 */
#include <cmath>
#include "utils/magnitude_tracker.h"

namespace fieldkitfx {

void MagnitudeTracker::processSample(float sample) {
    // check wether we got a new maximum, otherwise increase the hold counter
    sample = fabs(sample);
    if (sample > trackMax || ++trackTime > MAX_HOLD_TIME) {
        trackTime = 0;
        trackMax = sample;
    }
}

float MagnitudeTracker::getMax(void) {
    return trackMax;
}

}