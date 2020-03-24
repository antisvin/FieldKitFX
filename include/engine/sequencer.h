/*
 * sequencer.h
 *
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "hardware/adc.h"
#include "hardware/dac.h"
#include "hardware/gate.h"
#include "hardware/led.h"
#include "hardware/rollodecks.h"
#include "hardware/routing_matrix.h"
#include "utils/moving_average_filter.h"
#include "utils/utils.h"

/*
 * DEFINES
 */

/*
 * The number of steps available to the sequencer
 */

#define NUM_STEPS 4

#define SEQUENCER_CV_RESOLUTION 1023

#define SEQUENCER_MIN_PERIOD 0.05
#define SEQUENCER_MAX_PERIOD 5.0

#define SEQUENCER_TIMER_PERIOD 0.0001

#define SEQUENCER_RISING_TRESHOLD 2

// this is the number of the actual button
#define SEQUENCER_CV_DESTINATION 10

#define SEQUENCER_CV_TRESHOLD 50

// moving average filer for the threshold pot
#define SEQUENCER_THRSH_MA_LENGTH 30
// float sequencer_threshold_ma_array[SEQUENCER_THRSH_MA_LENGTH];

// moving average filer for the threshold cv
#define SEQUENCER_THRSHCV_MA_LENGTH 10
// float sequencer_thresholdCV_ma_array[SEQUENCER_THRSHCV_MA_LENGTH];

namespace fieldkitfx {

enum SequencerMode {
    POT,
    CV,
    GATE,
};

/*
 * Declaration of the step, allows to connect one LED to one CV-source
 */
class Step
{
private:
    DISALLOW_COPY_AND_ASSIGN(Step);

public:
    Led* step_led;
    uint16_t* step_cv;

    //	Step(Led *step_led, uint16_t *step_cv);
};

/*
 * Declaration of the sequencer_period, which defines the speed of the sequencer
 * this value is compared against the counter register of the timer
 */

class EnvelopeState
{
private:
    DISALLOW_COPY_AND_ASSIGN(EnvelopeState);

public:
    uint16_t previous;
    uint16_t current;
    uint16_t counter;

    EnvelopeState() {};
};

class Sequencer
{
private:
    DISALLOW_COPY_AND_ASSIGN(Sequencer);

public:
    uint16_t period;
    uint16_t time_passed;
    uint16_t index;
    uint16_t envelope_threshold;
    SequencerMode mode;
    EnvelopeState envelope_state;

    MovingAverageFilter<float, SEQUENCER_THRSH_MA_LENGTH> sequencer_threshold_ma;
    MovingAverageFilter<float, SEQUENCER_THRSHCV_MA_LENGTH> sequencer_thresholdCV_ma;
    Step steps[NUM_STEPS] {
        { &led0, &(rollodecks.cv_values[0]) },
        { &led1, &(rollodecks.cv_values[1]) },
        { &led2, &(rollodecks.cv_values[2]) },
        { &led3, &(rollodecks.cv_values[3]) },
    };

    Sequencer() = default;
    void init();
    void uiInit();
    void setStep(uint16_t step);
    void watch(void);
    void processCv(uint32_t cv);
    void updateDac(uint16_t step);
    void setMode(uint8_t routing);
    float mapTimeTarget(float cv, float min, float max);
    void setNextStep();
};

extern Sequencer sequencer;
}

#endif /* SEQUENCER_H_ */
