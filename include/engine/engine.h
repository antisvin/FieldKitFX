/*
 * engine.h
 */

#ifndef ENGINE_H_
#define ENGINE_H_

/*
 * This file contains all the audio engine related variables and functions
 *
 */

#include "engine/audio_buffers.h"

/*
 * General definitions
 */
#define F_SAMPLING                                                             \
    48000 // Attention, this definition is only used for the DSP calculations and not the codec setup, please configure the codes accrodingly

namespace fieldkitfx {

extern OutputAudioBuffer user_audio_out_buffer;
extern InputAudioBuffer user_audio_in_buffer;

}

#endif /* ENGINE_H_ */
