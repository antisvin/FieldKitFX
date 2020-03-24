/*
 * audioBuffers.h
 *
 * Buffers used for the audio i/o-stream.
 * The division by 4 results from using only on channel of the stereo stream
 * (/2) and the DMA writing interval of half-buffers (/2)
 */

#ifndef AUDIOBUFFERS_H_
#define AUDIOBUFFERS_H_

#include "utils/utils.h"

constexpr uint8_t AUDIO_IO_BUFFER_SIZE = 96;
// divide by 4 to have the audio latency (in samples)
constexpr uint8_t USER_AUDIO_IO_BUFFER_SIZE = AUDIO_IO_BUFFER_SIZE / 4;

namespace fieldkitfx {

class BaseAudioBuffer {
private:
    DISALLOW_COPY_AND_ASSIGN(BaseAudioBuffer);

public:
    uint8_t index;
    float buffer[USER_AUDIO_IO_BUFFER_SIZE];

    BaseAudioBuffer() = default;
    bool isFull(void);
    bool isEmpty(void);
    void reset();
};

class InputAudioBuffer : public BaseAudioBuffer {
public:
    void pop(float* sample);
    bool isFull(void);
    bool isEmpty(void);
};

class OutputAudioBuffer : public BaseAudioBuffer {
public:
    void push(float sample);
    bool isFull(void);
    bool isEmpty(void);
};

}
#endif /* AUDIOBUFFERS_H_ */
