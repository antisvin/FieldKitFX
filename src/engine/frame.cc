/*
 * frame.cc
 *
 */

#include "engine/frame.h"
#include "engine/mu_law.h"

namespace fieldkitfx {

void Frame::encode(const float* stream) {
    for (uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
        samples[i] = clouds::Lin2MuLaw(int16_t(stream[i] * 32124));
    }
}

void Frame::decode(float* stream) {
    for (uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
        stream[i] = float(clouds::MuLaw2Lin(samples[i])) / 32124;
    }
}

}
