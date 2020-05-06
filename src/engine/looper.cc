/*
 * looper.cc
 *
 */
#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#include "engine/engine.h"
#include "engine/looper.h"
#include "hardware/adc.h"
#include "utils/fade_lut.h"
#include "utils/log_lut.h"
#include "utils/utils.h"

namespace fieldkitfx {

Looper looper;

void Looper::init(RamChip* ram) {
    state = ARMED;
    memory = ram;
}

void Looper::process(float* in, float* out) {
    Frame processingFrame;

    switch (state) {
    case ARMED:
        // compress to frame
        processingFrame.encode(in);
        // decompress frame
        processingFrame.decode(out);
        break;
    case RECORD:
        // fade the first six frames (3ms) in
        if (framePointer < FRAME_NUMBER_FADE_IN) {
            for (uint8_t k = 0; k < USER_AUDIO_IO_BUFFER_SIZE; k++) {
                in[k] *= fadeLUT144[k + framePointer * 24];
            }
        }
        // compress to frame
        processingFrame.encode(in);
        // write frame to RAM
        memory->write(framePointer, &processingFrame);
        // update frame pointer
        framePointer++;
        // decompress the frame and toss it in the output buffer
        processingFrame.decode(out);
        break;
    case PLAYBACK:
        // compress to frame
        processingFrame.encode(in);
        // decompress frame
        processingFrame.decode(out);
        // get frame from RAM
        memory->read(framePointer, &processingFrame);
        // update frame pointer
        framePointer++;
        if (framePointer > endFramePosition) {
            framePointer = 0;
        }
        // decode the frame
        processingFrame.decode(looperSampleBuffer_dry);
        // compress to frame
        processingFrame.encode(in);
        // decompress frame
        processingFrame.decode(out);
        // add the contents of the buffer to the decoded frame
        for (uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            out[i] = clip(in[i] + looperSampleBuffer_dry[i]);
        }

        if (doFadeOut) {
            memory->read(endFramePosition - doFadeOut + 1, &processingFrame);
            processingFrame.decode(looperSampleBuffer_dry);
            // fade it out
            for (uint8_t k = 0; k < USER_AUDIO_IO_BUFFER_SIZE; k++) {
                looperSampleBuffer_dry[k] *= fadeLUT144[(doFadeOut * 24 - 1) - k];
            }
            // rewrite it to RAM
            processingFrame.encode(looperSampleBuffer_dry);
            memory->write(endFramePosition - doFadeOut + 1, &processingFrame);
            doFadeOut--;
        }
        break;

    case OVERDUB:
        // get the next frame
        memory->read(framePointer, &processingFrame);
        // decompress it
        processingFrame.decode(looperSampleBuffer_dry);
        // if first overdubbed frames, fade them in
        if (doOverdubFadeIn) {
            for (uint8_t k = 0; k < USER_AUDIO_IO_BUFFER_SIZE; k++) {
                in[k] *=
                    fadeLUT144[k + ((FRAME_NUMBER_FADE_IN - doOverdubFadeIn) * 24)];
            }
            doOverdubFadeIn--;
        }
        if (doOverdubFadeOut) {
            for (uint8_t k = 0; k < USER_AUDIO_IO_BUFFER_SIZE; k++) {
                in[k] *= fadeLUT144[(doOverdubFadeOut * 24 - 1) - k];
            }
            doOverdubFadeOut--;
            if (doOverdubFadeOut == 0) {
                switchState(PLAYBACK);
            }
        }
        // add the contents of the buffer to the decoded frame
        for (uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            looperSampleBuffer_dry[i] = clip(looperSampleBuffer_dry[i] + in[i]);
        }

        // compress back in frame
        processingFrame.encode(looperSampleBuffer_dry);
        // write the frame
        memory->write(framePointer, &processingFrame);
        // update frame pointer
        framePointer++;
        if (framePointer > endFramePosition) {
            framePointer = 0;
        }
        // need to decode the frame to avoid buffer corruption
        processingFrame.decode(looperSampleBuffer_dry);
        // send to output buffer
        for (uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            out[i] = looperSampleBuffer_dry[i];
        }
        break;
    case ERASE:
        // get the next frame
        memory->read(framePointer, &processingFrame);
        // update frame pointer
        framePointer++;
        // decompress it to output buffer
        processingFrame.decode(out);
        break;
    default:
        break;
    }
}

void Looper::switchState(LooperState new_state) {
    refreshUi = true;
    state = new_state;
}

}
