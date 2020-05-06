/*
 * audioBuffers.c
 *
 */

#include "engine/audio_buffers.h"

namespace fieldkitfx {

/*
 * Common buffer functions
 */

void BaseAudioBuffer::reset(void) {
    index = 0;
}

/*
 * Output buffer functions
 */

bool OutputAudioBuffer::isFull(void) {
    return index >= USER_AUDIO_IO_BUFFER_SIZE;
}

bool OutputAudioBuffer::isEmpty(void) {
    return index == 0;
}

void OutputAudioBuffer::push(float sample) {
    buffer[index++] = sample;
}

/*
 * Input buffer functions
 */

bool InputAudioBuffer::isFull(void) {
    return !index;
}

bool InputAudioBuffer::isEmpty() {
    return (index >= USER_AUDIO_IO_BUFFER_SIZE);
}

void InputAudioBuffer::pop(float* sample) {
    *sample = buffer[index++];
}
}