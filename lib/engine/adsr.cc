/*
 * adsr.c
 *
 */

#include "adsr.h"


MovingAverageFilter<float, ADSR_THRSH_MA_LENGTH> envelope_treshold_ma;
MovingAverageFilter<float, ADSR_POTA_MA_LENGTH> envelope_potA_ma;
MovingAverageFilter<float, ADSR_POTD_MA_LENGTH> envelope_potD_ma;
MovingAverageFilter<float, ADSR_POTR_MA_LENGTH> envelope_potR_ma;


ADSR::ADSR() {
	state = IDLE;
	length = 1000;
	process_ui = 1;
	mode = POT_ADSR;
}

void ADSR::uiInit(void) {
	led0.setLow();
	led1.setLow();
	led2.setLow();
	led3.setLow();
}

void ADSR::processGate(Gate *gate) {
	gateIn = gate->isHigh();
}

void ADSR::update(void) {
	tick++; //increase tick counter
	switch (state) {
	case ATTACK:
		accumulator += A_inc;
		if (accumulator >= ADSR_ACC_MAX_VAL) {
			led0.setLow();
			led1.setHigh();
			state = DECAY;
			accumulator = ADSR_ACC_MAX_VAL;
		}
		if (mode == GATE_ADSR || mode == CV_ADSR) {
			if (gateIn == 0) {
				led0.setLow();
				led3.setHigh();
				state = RELEASE;
				setROutput();
			}
		}
		break;
	case DECAY:
		accumulator -= D_inc;
		if (accumulator <= S_lev) {
			state = SUSTAIN;
			led1.setLow();
			led2.setHigh();
			accumulator = S_lev;
		}
		//check wether decay might take the full envelope time and if so switch to release
		if (mode == POT_ADSR) {
			if (tick >= length * ADSR_FS) {
				state = RELEASE;
				led1.setLow();
				led3.setHigh();
				setROutput();
			}
		}
		else if (mode == GATE_ADSR || mode == CV_ADSR) {
			if (!gateIn) {
				led1.setLow();
				led3.setHigh();
				state = RELEASE;
				setROutput();
			}
		}
		break;
	case SUSTAIN:
		if (mode == POT_ADSR) {
			if (tick >= length * ADSR_FS) {
				state = RELEASE;
				led2.setLow();
				led3.setHigh();
			}
		}
		else if (mode == GATE_ADSR || mode == CV_ADSR) {
			if (!gateIn || tick > 30000) {
				state = RELEASE;
				led2.setLow();
				led3.setHigh();
			}
		}
		break;
	case RELEASE:
		accumulator -= R_inc;
		if (accumulator <= 0.0) {
			state = IDLE;
			led3.setLow();
			accumulator = 0.0;
		}
		if (mode == GATE_ADSR || mode == CV_ADSR) {
			if (gateIn) { //retriggering
				state = ATTACK;
				led0.setHigh();
				led3.setLow();
				tick = 0; //reset tick counter
				process_ui = 1;
			}
		}
		break;
	case IDLE:
		if (mode == POT_ADSR) {
			state = ATTACK;
			led0.setHigh();
			tick = 0; //reset tick counter
			process_ui = 1;
		}
		else if (mode == GATE_ADSR || mode == CV_ADSR) {
			if (gateIn) {
				state = ATTACK;
				led0.setHigh();
				tick = 0; //reset tick counter
				process_ui = 1;
			}
			else {
				process_ui = 1;
			}
		}
		else {
			process_ui = 1;
		}
		break;
	}

	//only take the 12 important bits of the accumulator
	output = accumulator * 4095;
}

float ADSR::mapTimeTarget(uint16_t CV, float min, float max) {
	return ((float)CV / ADSR_CV_RESOLUTION * (max - min) + min);
}

void ADSR::processThresholdPot(uint16_t CV1) {
	if (mode == POT_ADSR) {
		envelope_treshold_ma.process(inverselogLUT_10BitFloat[(uint16_t)ADSR_CV_RESOLUTION - CV1]);
	}
	else if (mode == CV_ADSR) {
		envelope_treshold_ma.process((uint16_t)ADSR_CV_RESOLUTION - CV1);
	}
}

void ADSR::setLengthThreshold() {
	if (mode == POT_ADSR)
	{
		length = ADSR::mapTimeTarget((uint16_t)envelope_treshold_ma.output, ADSR_MIN_OVERALL_TIME, ADSR_MAX_OVERALL_TIME);
		if (length < ADSR_MIN_OVERALL_TIME) {
			length = ADSR_MIN_OVERALL_TIME;
		}
	}
	else if (mode == CV_ADSR) {
		threshold_cv = envelope_treshold_ma.output;
		if (threshold_cv < 0) {
			threshold_cv = 0;
		}
	}
}

void ADSR::processThresholdCv(uint16_t CV) {
	gatePrevious = gateIn;
	deadtime_counter++;
	if (CV >= 512) {
		CV -= 512;
		CV <<= 1;
	}
	else {
		CV = 0;
	}
	if (CV > threshold_cv) {
		if (!gatePrevious) { //rising edge
			if (deadtime_counter > ADSR_DEADTIME) {
				gateIn = 1;
				deadtime_counter = 0;
			}
		}
	}
	else {
		if (gatePrevious) { //falling edge
			if (deadtime_counter > ADSR_DEADTIME) {
				gateIn = 0;
				deadtime_counter = 0;
			}
		}
	}
}

void ADSR::processAdsrCv(uint16_t CVA, uint16_t CVD, uint16_t CVS, uint16_t CVR) {
	envelope_potA_ma.process(inverselogLUT_10BitFloat[CVA]);
	A_cv = envelope_potA_ma.output;
	if (A_cv < 0) {
		A_cv = 0;
	}
	envelope_potD_ma.process(inverselogLUT_10BitFloat[CVD]);
	D_cv = envelope_potD_ma.output;
	if (D_cv < 0) {
		D_cv = 0;
	}
	S_cv = (float)CVS;
	envelope_potR_ma.process(inverselogLUT_10BitFloat[CVR]);
	R_cv = envelope_potR_ma.output;
	if (R_cv < 0) {
		R_cv = 0;
	}
}

void ADSR::setA(void) {
	volatile float target = ADSR::mapTimeTarget((uint16_t)A_cv, ADSR_MIN_A_TIME, ADSR_MAX_A_TIME);
	A_inc = (ADSR_ACC_MAX_VAL / (target * ADSR_FS));
}

void ADSR::setD(void) {
	volatile float target = ADSR::mapTimeTarget((uint16_t)D_cv, ADSR_MIN_D_TIME, ADSR_MAX_D_TIME);
	D_inc = (ADSR_ACC_MAX_VAL - (S_lev)) / (target * ADSR_FS);
}

void ADSR::setS(void) {
	S_lev = (float)S_cv / ADSR_CV_RESOLUTION;
}

void ADSR::setR(void) {
	volatile float target = ADSR::mapTimeTarget((uint16_t)R_cv, ADSR_MIN_R_TIME, ADSR_MAX_R_TIME);
	R_inc = S_lev / (target * ADSR_FS);
}

//this recalculates the R_inc based on the current output, that is needed when the decay stage overlaps the overall length of the envelope
void ADSR::setROutput(void) {
	volatile float target = ADSR::mapTimeTarget(R_cv, ADSR_MIN_R_TIME, ADSR_MAX_R_TIME);
	R_inc = accumulator / (target * ADSR_FS);
}

void ADSR::setMode(uint32_t cv, uint8_t routing) {
	//see CVRouter, the state function returns 0 if nothing is assigned
	if (!routing) {
		mode = POT_ADSR;
	}
	else {
		if (ADSR_CV_TRESHOLD_SCALED < ADSR_CV_RESOLUTION - cv) {
			mode = CV_ADSR;
		}
		else {
			mode = GATE_ADSR;
		}
	}
}

void ADSR::processSample() {
	//Check gate input
	if (mode == GATE_ADSR) {
		processGate(&gate);
	}
	else if (mode == CV_ADSR) {
		processThresholdCv(ADC_getThresholdCV() >> 2);
	}
	//		update the ADSR output
	update();
	//		put that to the DAC
	DAC_setValue(output);
}

ADSR rollo_env;