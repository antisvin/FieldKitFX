/*
 * magnitudeTracker.c
 *
 */

#include "magnitudeTracker.h"

uint16_t trackTime = 0;
float trackMax = 0.0f;

void magnitudeTracker_processSample(float sample){
	//check wether we got a new maximum, otherwise increase the hold counter
	if(sample > trackMax){
		trackTime = 0;
		trackMax = sample;
	}else{
		trackTime++;
	}
	//check if the hold time elapsed
	if(trackTime > MAX_HOLD_TIME){
		trackTime = 0;
		trackMax = sample;
	}
}

inline float magnitudeTracker_getMax(void){
	return trackMax;
}
