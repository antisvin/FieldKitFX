/*
 * magnitudeTracker.c
 *
 */

#include "magnitudeTracker.h"


void MagnitudeTracker::processSample(float sample) {
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

float MagnitudeTracker::getMax(void) {
	return trackMax;
}

MagnitudeTracker magnitude_tracker;