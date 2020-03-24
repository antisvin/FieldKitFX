/*
 * frame.h
 *
 * Simple frame decoding system to safe some space in memory.
 * A block contains two 12bit samples in arranged into three bytes.
 * The frame size has to be adjusted according to the buffer size of the system making use of the encoding.
 */

#ifndef FRAME_H_
#define FRAME_H_

#include "utils/utils.h"

// based on USER_AUDIO_IO_BUFFER_SIZE=24 and 2 samples per block
#define BLOCKS_PER_FRAME 12
#define BLOCK_SIZE 3

namespace fieldkitfx {

constexpr uint8_t FRAME_BYTECOUNT = BLOCK_SIZE * BLOCKS_PER_FRAME;

struct SampleBlock {
    uint8_t nibbleByte;
    uint8_t lesser_samples[2];
};

class Frame {
private:
    DISALLOW_COPY_AND_ASSIGN(Frame);

public:
    SampleBlock samples[BLOCKS_PER_FRAME];

    Frame() = default;

    void encode(const float* stream);
    void decode(float* stream);
};

}
#endif /* FRAME_H_ */
