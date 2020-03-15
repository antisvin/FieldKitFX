/*
 * looper.c
 *
 */

#include "looper.h"
#include "engine.h"
#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#include "ADC.h"
#include "logLUT.h"
#include "fadeLUT.h"
#include "randomUtils.h"


void looper_init(looper_t* l) {
	l->state = ARMED;
	l->doFadeIn = 0;
	l->doFadeOut = 0;
	l->doOverdubFadeIn = 0;
	l->doOverdubFadeOut = 0;
}


void looper_process(looper_t * l) {
	frame_t processingFrame;
	float looperSampleBuffer_dry[USER_AUDIO_IO_BUFFER_SIZE];

	switch(l->state) {
	case ARMED:
		//empty the buffer
		user_audio_in_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		//compress to frame
		encodeFrame(user_audio_in_buffer.buffer, &processingFrame);
		//decompress frame
		decodeFrame(&processingFrame, user_audio_out_buffer.buffer);
		user_audio_out_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		break;
	case RECORD:
		//empty the buffer
		user_audio_in_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		//fade the first six frames (3ms) in
		if(l->framePointer < FRAME_NUMBER_FADE_IN){
			for(uint8_t k=0; k < USER_AUDIO_IO_BUFFER_SIZE; k++) {
				user_audio_in_buffer.buffer[k] *= fadeLUT144[k + l->framePointer * 24];
			}
		}

		//compress to frame
		encodeFrame(user_audio_in_buffer.buffer, &processingFrame);
		//write frame to RAM
		RAM_writeFrame(l->memory, l->framePointer, &processingFrame);
		//update frame pointer
		l->framePointer ++;
		//decompress the frame and toss it in the output buffer
		decodeFrame(&processingFrame, user_audio_out_buffer.buffer);
		user_audio_out_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		break;
	case PLAYBACK:
		//discard input buffer
		user_audio_in_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		//compress to frame
		encodeFrame(user_audio_in_buffer.buffer, &processingFrame);
		//decompress frame
		decodeFrame(&processingFrame, user_audio_out_buffer.buffer);
		//get frame from RAM
		RAM_readFrame(l->memory, l->framePointer, &processingFrame);
		//		update frame pointer
		l->framePointer++;
		if(l->framePointer > l->endFramePosition) {
			l->framePointer = 0;
		}
		//		decode the frame
		decodeFrame(&processingFrame, looperSampleBuffer_dry);
		//		decodeFrame(&processingFrame, user_audio_out_buffer.buffer);
		//compress to frame
		encodeFrame(user_audio_in_buffer.buffer, &processingFrame);
		//decompress frame
		decodeFrame(&processingFrame, user_audio_out_buffer.buffer);
		//add the contents of the buffer to the decoded frame
		for(uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
			user_audio_out_buffer.buffer[i] = clip(user_audio_in_buffer.buffer[i] + looperSampleBuffer_dry[i]);
		}

		if(l->doFadeOut){
			RAM_readFrame(l->memory, l->endFramePosition - l->doFadeOut + 1, &processingFrame);
			decodeFrame(&processingFrame, looperSampleBuffer_dry);
			//fade it out
			for(uint8_t k=0; k<USER_AUDIO_IO_BUFFER_SIZE; k++) {
				looperSampleBuffer_dry[k] *= fadeLUT144[(l->doFadeOut * 24 - 1) - k];
			}
			//rewrite it to RAM
			encodeFrame(looperSampleBuffer_dry, &processingFrame);
			RAM_writeFrame(l->memory, l->endFramePosition - l->doFadeOut + 1, &processingFrame);
			l->doFadeOut--;
		}
		user_audio_out_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		break;

	case OVERDUB:
		//empty the buffer
		user_audio_in_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		//get the next frame
		RAM_readFrame(l->memory, l->framePointer, &processingFrame);
		//decompress it
		decodeFrame(&processingFrame, looperSampleBuffer_dry);
		//if first overdubbed frames, fade them in
		if(l->doOverdubFadeIn) {
			for(uint8_t k=0; k < USER_AUDIO_IO_BUFFER_SIZE; k++) {
				user_audio_in_buffer.buffer[k] *= fadeLUT144[k + ((FRAME_NUMBER_FADE_IN - l->doOverdubFadeIn) * 24)];
			}
			l->doOverdubFadeIn--;
		}
		if(l->doOverdubFadeOut){
			for(uint8_t k=0; k < USER_AUDIO_IO_BUFFER_SIZE; k++) {
				user_audio_in_buffer.buffer[k] *= fadeLUT144[(l->doOverdubFadeOut * 24 - 1) - k];
			}
			l->doOverdubFadeOut--;
			if(l->doOverdubFadeOut == 0){
				l->state = PLAYBACK;
			}
		}
		//add the contents of the buffer to the decoded frame
		for(uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
			looperSampleBuffer_dry[i] = clip(looperSampleBuffer_dry[i] + user_audio_in_buffer.buffer[i]);
		}

		//compress back in frame
		encodeFrame(looperSampleBuffer_dry, &processingFrame);
		//write the frame
		RAM_writeFrame(l->memory, l->framePointer, &processingFrame);
		//update frame pointer
		l->framePointer++;
		if(l->framePointer > l->endFramePosition) {
			l->framePointer = 0;
		}
		//need to decode the frame to avoid buffer corruption
		decodeFrame(&processingFrame, looperSampleBuffer_dry);
		//send to output buffer
		for(uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
			user_audio_out_buffer.buffer[i] = looperSampleBuffer_dry[i];
		}
		user_audio_out_buffer.index = AUDIO_IO_BUFFER_SIZE/4;
		break;
	case ERASE:
		//empty the input buffer
		user_audio_in_buffer.index = USER_AUDIO_IO_BUFFER_SIZE;
		//get the next frame
		RAM_readFrame(l->memory, l->framePointer, &processingFrame);
		//update frame pointer
		l->framePointer++;
		//decompress it to output buffer
		decodeFrame(&processingFrame, user_audio_out_buffer.buffer);
		user_audio_out_buffer.index = AUDIO_IO_BUFFER_SIZE/4;
		break;
	default:
		break;
	}
}

void looper_process_bitcrush(float *inputBuffer, float *outputBuffer, uint16_t ADCValue) {
	float bitReduction = (logLUT_12bit[ADCValue] * (16.0 - LOOPER_MIN_BITDEPTH)) / 4095.0;

	uint8_t bitReduction_integerPart = bitReduction;

	for(uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
		int16_t tempSample1 = (int16_t)(inputBuffer[i] * 32767);
		int16_t tempSample2 = tempSample1;
		//shift
		tempSample1 >>= bitReduction_integerPart;
		tempSample1 <<= bitReduction_integerPart;

		//shift
		tempSample2 >>= (bitReduction_integerPart + 1);
		tempSample2 <<= (bitReduction_integerPart + 1);

		//de-offset
		outputBuffer[i] = crossfade(
			((float)tempSample1) / 32767, ((float)tempSample2) / 32767, bitReduction_integerPart + 1 - bitReduction);
	}
}

void looper_process_sampleRateReducer(float *inputBuffer, float *outputBuffer, uint16_t ADCValue) {
	const uint8_t decimationFactors[7] = {1, 2, 4, 8, 16, 32, 64};
	float decimationFactor = ((ADCValue * 6)/4095.0);

	uint8_t decimationFactor_integerPart;
	float tempSample1, tempSample2;

	decimationFactor_integerPart = (uint8_t) decimationFactor;

	for(uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
		if((i % decimationFactors[decimationFactor_integerPart]) == 0) {
			tempSample1 = inputBuffer[i];
		}
		if((i % (decimationFactors[decimationFactor_integerPart + 1])) == 0) {
			tempSample2 = inputBuffer[i];
		}

		outputBuffer[i] = (tempSample1 + (tempSample2 - tempSample1)*(decimationFactor - decimationFactor_integerPart));
	}
}

// UNUSED
void looper_linearInterp(float* clean, float* dirty, float* out, uint16_t ADCValue) {
	uint8_t i;
	float temp = (float)ADCValue / 4095.0;
	for(i=0; i<USER_AUDIO_IO_BUFFER_SIZE; i++) {		
		out[i] = clean[i]*(1.0f - temp) + dirty[i] * temp;
	}
}
