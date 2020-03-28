/*
 * looper.h
 *
 * The Looper is controlled trough a state machine, which reacts to user input
 * according to the current state.
 * The Effects, Sample Rate Reduction and Bitcrusher, are handled within separate functions.
 * Altering those effects might be the easiest way to start editing the code.
 */

#ifndef LOOPER_H_
#define LOOPER_H_

#include <cmath>
#include "hardware/memory.h"
#include "utils/utils.h"

#define LOOPER_FS 48000

// maximum frame number for USER_AUDIO_IO_BUFFER_SIZE=24 and 12 blocks à 3bytes (24bits) per frame
#define MAX_FRAME_NUM 7281

#define FRAME_NUMBER_FADE_IN 6

#define BLINK_FRAME_THRESHOLD 400
#define BLINK_GUARD_INTERVAL 400

/*
 * Effects
 */
#define LOOPER_MIN_BITDEPTH 1
#define LOOPER_MAX_DECIMATION_FACTOR 8

#define BITCRUSHER_LOWER_LIMIT -10
#define BITCRUSHER_UPPER_LIMIT 10

namespace fieldkitfx {

enum LooperState { ARMED = 0, RECORD, PLAYBACK, OVERDUB, ERASE };

class Looper {
private:
    RamChip* memory;
    DISALLOW_COPY_AND_ASSIGN(Looper);

public:
    uint16_t framePointer;
    uint16_t endFramePosition;
    LooperState state;
    uint8_t doFadeIn, doFadeOut;
    uint8_t doOverdubFadeIn, doOverdubFadeOut;
    bool refreshUi;

    Looper() = default;
    void init(RamChip* ram);
    void process(float* in, float* out);
    void switchState(LooperState new_state);
};

extern Looper looper;
}

#endif /* LOOPER_H_ */
