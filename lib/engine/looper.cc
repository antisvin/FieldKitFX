/*
 * looper.c
 *
 */

#include "looper.h"
#include "ADC.h"
#include "engine.h"
#include "fadeLUT.h"
#include "logLUT.h"
#include "randomUtils.h"
#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"

void Looper::init(RamChip *ram) {
	state = ARMED;
	memory = ram;
}

void Looper::process() {
	Frame processingFrame;
	float looperSampleBuffer_dry[USER_AUDIO_IO_BUFFER_SIZE];

	switch (state) {
	case ARMED:
		// empty the buffer
		user_audio_in_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		// compress to frame
		processingFrame.encode(user_audio_in_buffer.buffer);
		// decompress frame
		processingFrame.decode(user_audio_out_buffer.buffer);
		user_audio_out_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		break;
	case RECORD:
		// empty the buffer
		user_audio_in_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		// fade the first six frames (3ms) in
		if (framePointer < FRAME_NUMBER_FADE_IN) {
			for (uint8_t k = 0; k < USER_AUDIO_IO_BUFFER_SIZE; k++) {
				user_audio_in_buffer.buffer[k] *= fadeLUT144[k + framePointer * 24];
			}
		}

		// compress to frame
		processingFrame.encode(user_audio_in_buffer.buffer);
		// write frame to RAM
		memory->write(framePointer, &processingFrame);
		// update frame pointer
		framePointer++;
		// decompress the frame and toss it in the output buffer
		processingFrame.decode(user_audio_out_buffer.buffer);
		user_audio_out_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		break;
	case PLAYBACK:
		// discard input buffer
		user_audio_in_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		// compress to frame
		processingFrame.encode(user_audio_in_buffer.buffer);
		// decompress frame
		processingFrame.decode(user_audio_out_buffer.buffer);
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
		processingFrame.encode(user_audio_in_buffer.buffer);
		// decompress frame
		processingFrame.decode(user_audio_out_buffer.buffer);
		// add the contents of the buffer to the decoded frame
		for (uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
			user_audio_out_buffer.buffer[i] =
				clip(user_audio_in_buffer.buffer[i] + looperSampleBuffer_dry[i]);
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
		user_audio_out_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		break;

	case OVERDUB:
		// empty the buffer
		user_audio_in_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		// get the next frame
		memory->read(framePointer, &processingFrame);
		// decompress it
		processingFrame.decode(looperSampleBuffer_dry);
		// if first overdubbed frames, fade them in
		if (doOverdubFadeIn) {
			for (uint8_t k = 0; k < USER_AUDIO_IO_BUFFER_SIZE; k++) {
				user_audio_in_buffer.buffer[k] *=
					fadeLUT144[k + ((FRAME_NUMBER_FADE_IN - doOverdubFadeIn) * 24)];
			}
			doOverdubFadeIn--;
		}
		if (doOverdubFadeOut) {
			for (uint8_t k = 0; k < USER_AUDIO_IO_BUFFER_SIZE; k++) {
				user_audio_in_buffer.buffer[k] *=
					fadeLUT144[(doOverdubFadeOut * 24 - 1) - k];
			}
			doOverdubFadeOut--;
			if (doOverdubFadeOut == 0) {
				state = PLAYBACK;
			}
		}
		// add the contents of the buffer to the decoded frame
		for (uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
			looperSampleBuffer_dry[i] =
				clip(looperSampleBuffer_dry[i] + user_audio_in_buffer.buffer[i]);
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
			user_audio_out_buffer.buffer[i] = looperSampleBuffer_dry[i];
		}
		user_audio_out_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		break;
	case ERASE:
		// empty the input buffer
		user_audio_in_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		// get the next frame
		memory->read(framePointer, &processingFrame);
		// update frame pointer
		framePointer++;
		// decompress it to output buffer
		processingFrame.decode(user_audio_out_buffer.buffer);
		user_audio_out_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		break;
	default:
		break;
	}
}

void Looper::processBitcrush(float *inputBuffer, float *outputBuffer,
							 uint16_t ADCValue) {
	float bitReduction =
		(logLUT_12bit[ADCValue] * (16.0 - LOOPER_MIN_BITDEPTH)) / 4095.0;

	uint8_t bitReduction_integerPart = bitReduction;

	for (uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
		int16_t tempSample1 = (int16_t)(inputBuffer[i] * 32767);
		int16_t tempSample2 = tempSample1;
		// shift
		tempSample1 >>= bitReduction_integerPart;
		tempSample1 <<= bitReduction_integerPart;

		// shift
		tempSample2 >>= (bitReduction_integerPart + 1);
		tempSample2 <<= (bitReduction_integerPart + 1);

		// de-offset
		outputBuffer[i] =
			crossfade(((float)tempSample1) / 32767, ((float)tempSample2) / 32767,
					  bitReduction_integerPart + 1 - bitReduction);
	}
}

void Looper::processSampleRateReducer(float *inputBuffer, float *outputBuffer,
									  uint16_t ADCValue) {
	const uint8_t decimationFactors[7] = {1, 2, 4, 8, 16, 32, 64};
	float decimationFactor = ((ADCValue * 6) / 4095.0);

	uint8_t decimationFactor_integerPart;
	float tempSample1, tempSample2;

	decimationFactor_integerPart = (uint8_t)decimationFactor;

	for (uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
		if ((i % decimationFactors[decimationFactor_integerPart]) == 0) {
			tempSample1 = inputBuffer[i];
		}
		if ((i % (decimationFactors[decimationFactor_integerPart + 1])) == 0) {
			tempSample2 = inputBuffer[i];
		}

		outputBuffer[i] =
			(tempSample1 + (tempSample2 - tempSample1) *
							   (decimationFactor - decimationFactor_integerPart));
	}
}

// UNUSED
/*
void looper_linearInterp(float *clean, float *dirty, float *out,
						 uint16_t ADCValue)
{
	uint8_t i;
	float temp = (float)ADCValue / 4095.0;
	for (i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++)
	{
		out[i] = clean[i] * (1.0f - temp) + dirty[i] * temp;
	}
}
*/

Looper looper;