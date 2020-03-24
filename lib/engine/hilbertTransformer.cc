/*
 * hilbertTransformer.c
 *
 */

#include "stdint.h"

/*
 * 		The structure of the hilbert transformer :
 *
 *
 *
 *
 * 						+---[AP]---[AP]---[AP]--	(+45° output)
 * 						|
 * 		IN -------------|
 * 						|
 * 						+---[AP]---[AP]---[AP]--	(-45° output)
 *
 *
 *
 */

#include "hilbertTransformer.h"

static constexpr float coefficients[NUM_ALLPASS] {
	-0.999229004278298, -0.994158787551880, -0.976436246932096, -0.908711820239948,
	-0.677935627586549, -0.0792101408955407, -0.997327025721254, -0.988172698429361,
	-0.953364806814201, -0.825195989410729, -0.439051054845000, 0.494956506139769
};

void HilbertTransformer::process(float inputSample) {
	float y;
	float x = inputSample;

	//first branch
	for (uint8_t i = 0; i < NUM_ALLPASS_BRANCH; i++) {
		y = coefficients[i] * (x - oldy[i]) + oldx[i];
		oldx[i] = x;
		oldy[i] = y;
		x = y;
	}
	outputSample_1 = y;

	//second branch
	x = inputSample;
	for (uint8_t i = NUM_ALLPASS_BRANCH; i < NUM_ALLPASS; i++)
	{
		y = coefficients[i] * (x - oldy[i]) + oldx[i];
		oldx[i] = x;
		oldy[i] = y;
		x = y;
	}
	outputSample_2 = y;
};
