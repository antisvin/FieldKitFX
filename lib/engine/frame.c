/*
 * frame.c
 *
 */

#include "frame.h"
#include "randomUtils.h"


void encodeFrame(float* rawStream, frame_t* frame) {
	for(uint8_t block_counter = 0; block_counter < BLOCKS_PER_FRAME; block_counter++) {
		//reduce to unsigned 12bit
		uint16_t temp1 = scale_float_to_uint12(rawStream[block_counter * 2]);
		uint16_t temp2 = scale_float_to_uint12(rawStream[block_counter * 2 + 1]);

		frame->samples[block_counter].nibbleByte = ((temp1 >> 4) & 0xf0) | (temp2 >> 8);
		frame->samples[block_counter].lesser_samples[0] = temp1 & 0xff;
		frame->samples[block_counter].lesser_samples[1] = temp2 & 0xff;
	}
}

void decodeFrame(frame_t* frame, float* decodedStream) {
	for(uint8_t block_counter = 0; block_counter < BLOCKS_PER_FRAME; block_counter++) {
		uint16_t temp1 = ((frame->samples[block_counter].nibbleByte & 0xf0) << 4) | frame->samples[block_counter].lesser_samples[0];
		uint16_t temp2 = ((frame->samples[block_counter].nibbleByte & 0x0f) << 8) | frame->samples[block_counter].lesser_samples[1];
		//convert to float
		decodedStream[block_counter * 2] = scale_uint12_to_float(temp1);
		decodedStream[block_counter * 2 + 1] = scale_uint12_to_float(temp2);
	}
}
