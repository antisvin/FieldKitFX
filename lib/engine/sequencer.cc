/*
 * sequencer.c
 *
 */

#include "sequencer.h"
#include "debug.h"

void Sequencer::init(void){
	//init the GPIO CLK of the LED port
	__HAL_RCC_GPIOE_CLK_ENABLE();

	//assign and init all the LEDs
	/*
	led0.port = LED0_PORT;
	led0.pin = LED0_PIN;

	led1.port = LED1_PORT;
	led1.pin = LED1_PIN;

	led2.port = LED2_PORT;
	led2.pin = LED2_PIN;

	led3.port = LED3_PORT;
	led3.pin = LED3_PIN;

	LED_init(&led0);
	LED_init(&led1);
	LED_init(&led2);
	LED_init(&led3);
	*/

	led0.setLow();
	led1.setLow();
	led2.setLow();
	led3.setLow();

	//populate the  step array structures with their led and cv

	/*
	Step steps[] {
		{&led0, &rolloCV_values[0]},
		{&led1, &rolloCV_values[1]},
		{&led2, &rolloCV_values[2]},
		{&led3, &rolloCV_values[3]}
	};
	*/

	setStep(0);

	mode = GATE;

	processCv(ADC_getThresholdPot() >> 2);
}

void Sequencer::watch() {
	time_passed++;
	switch (mode) {
	case POT:
		if (time_passed >= period) {
			setNextStep();
			time_passed = 0;
		}
		break;
	case GATE:
		gate.update();
		if (gate.isTransitionToHigh()) {
			setNextStep();
		}
	case CV:
		if(time_passed == 10){//that's done to reduce the "sampling frequency"
			time_passed = 0;
			sequencer_envelope_state.previous = sequencer_envelope_state.current;
			//using only the postive halfwave for envelope following
			sequencer_envelope_state.current = (ADC_getThresholdCV() >> 2);
			if(sequencer_envelope_state.current >= 512) {
				sequencer_envelope_state.current -= 512;
				sequencer_envelope_state.current <<= 1;
			}
			else {
				sequencer_envelope_state.current = 0;
			}

			sequencer_thresholdCV_ma.process(
				(float)(sequencer_envelope_state.current - sequencer_envelope_state.previous));

			sequencer_envelope_state.counter++;
			if (sequencer_thresholdCV_ma.output > 0.0) {
				if (sequencer_envelope_state.current >= envelope_threshold && envelope_threshold > sequencer_envelope_state.previous && sequencer_envelope_state.counter > 10) {
					setNextStep();
					sequencer_envelope_state.counter = 0;
				}
			}
		}
	}
}

void Sequencer::uiInit() {
	for (uint8_t i = 0; i < NUM_STEPS; i++) {
		steps[i].step_led->setLow();
	}
	setStep(0);
}

void Sequencer::setStep(uint16_t step) {
	uint16_t previous_step = step - 1;
	if(previous_step >= NUM_STEPS){
		previous_step = NUM_STEPS - 1;
	}
	steps[previous_step].step_led->setLow();
	steps[step].step_led->setHigh();
	DAC_setValue(*steps[step].step_cv);
}

float Sequencer::mapTimeTarget(uint16_t CV, float min, float max) {
	return ((float)CV / SEQUENCER_CV_RESOLUTION * (max - min) + min);
}

void Sequencer::processCv(uint32_t cv){
	sequencer_threshold_ma.process((float)cv);
	period = (uint16_t)(
		mapTimeTarget(
			(uint16_t)sequencer_threshold_ma.output, SEQUENCER_MIN_PERIOD,
			SEQUENCER_MAX_PERIOD) / SEQUENCER_TIMER_PERIOD);
	envelope_threshold = SEQUENCER_CV_RESOLUTION - (uint16_t) sequencer_threshold_ma.output;
}

void Sequencer::updateDac(uint16_t step){
	DAC_setValue(*steps[step].step_cv);
}

void Sequencer::setMode(uint8_t routing){
	//see CVRouter, the CVRouter_getSource function returns 0 if nothing is assigned
	if (routing) {
		if (envelope_threshold > SEQUENCER_CV_TRESHOLD){
			mode = CV;
		}else{
			mode = GATE;
		}
	}
	else {
		mode = POT;
	}
}

void Sequencer::setNextStep(){
	index = (index + 1) % NUM_STEPS;
	setStep(index);
}

Sequencer sequencer;
EnvelopeState sequencer_envelope_state;