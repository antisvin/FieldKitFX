#ifndef _ENGINE_FOLLOWER_H_
#define _ENGINE_FOLLOWER_H_

namespace fieldkitfx {

class EnvelopeFollower {
public:
    EnvelopeFollower(float smooth_up, float smooth_down)
        : smooth_up(smooth_up)
        , smooth_down(smooth_down) {};

    EnvelopeFollower()
        : smooth_up(0.001)
        , smooth_down(0.01) {};

    void reset() {
        amp_prev = 0.0f;
    }

    float process(float in);

private:
    float smooth_up, smooth_down, amp_prev;
};

}

#endif