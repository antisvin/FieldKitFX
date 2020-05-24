#include <algorithm>
#include "engine/follower.h"

namespace fieldkitfx {

float EnvelopeFollower::process(float in) {
    float amp = std::abs(in);
    float smooth = (amp > amp_prev) ? smooth_up : smooth_down;
    float amp_filtered = amp * (1.0f - smooth);
    amp_prev = amp_filtered + amp_prev * smooth;
    return amp_prev;
}

}