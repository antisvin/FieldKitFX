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
#include "engine/audio_buffers.h"

namespace fieldkitfx {

class Frame {
private:
    DISALLOW_COPY_AND_ASSIGN(Frame);

public:
    uint8_t samples[USER_AUDIO_IO_BUFFER_SIZE];

    Frame() = default;

    void encode(const float* stream);
    void decode(float* stream);
};

}
#endif /* FRAME_H_ */
