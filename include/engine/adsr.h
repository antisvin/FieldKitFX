/*
 * adsr.h
 *
 * The ADSR of the Field Kit FX.
 */

#ifndef ADSR_H_
#define ADSR_H_

#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#include "hardware/adc.h"
#include "hardware/dac.h"
#include "hardware/gate.h"
#include "hardware/led.h"
#include "hardware/rollo_selector.h"
#include "hardware/routing_matrix.h"
#include "utils/log_lut.h"
#include "utils/moving_average_filter.h"
#include "utils/utils.h"

// Definition of the extrema of all envelope stages. Times are given in seconds.
#define ADSR_MIN_A_TIME 0.001
#define ADSR_MAX_A_TIME 1.0
#define ADSR_MIN_D_TIME 0.01
#define ADSR_MAX_D_TIME 1.0
#define ADSR_MIN_R_TIME 0.01
#define ADSR_MAX_R_TIME 1.0
#define ADSR_MIN_OVERALL_TIME 0.01
#define ADSR_MAX_OVERALL_TIME 3.0

#define ADSR_ACC_MAX_VAL 1.0
#define ADSR_FS 10000 // to link w/ timer

#define ADSR_CV_RESOLUTION 1023.0

// Threshold to distinguish between "Gate-Mode" or "Envelope-Mode" when external CV is routed to Threshold Input
#define ADSR_CV_TRESHOLD_SCALED 5

// Deadtime in ADSR-Samples. Prevents retriggering within untrustworthy time spans (for example due to noise...)
#define ADSR_DEADTIME 50

namespace fieldkitfx {

constexpr size_t ADSR_THRSH_MA_LENGTH = 8;
constexpr size_t ADSR_POTA_MA_LENGTH = 8;
constexpr size_t ADSR_POTD_MA_LENGTH = 8;
constexpr size_t ADSR_POTR_MA_LENGTH = 8;

enum ADSRState { IDLE = 0, ATTACK, DECAY, SUSTAIN, RELEASE };

enum ADSRMode { POT_ADSR = 0, CV_ADSR, GATE_ADSR };

class ADSR {
private:
    DISALLOW_COPY_AND_ASSIGN(ADSR);

public:
    MovingAverageFilter<float, ADSR_THRSH_MA_LENGTH> envelope_treshold_ma;
    MovingAverageFilter<float, ADSR_POTA_MA_LENGTH> envelope_potA_ma;
    MovingAverageFilter<float, ADSR_POTD_MA_LENGTH> envelope_potD_ma;
    MovingAverageFilter<float, ADSR_POTR_MA_LENGTH> envelope_potR_ma;

    ADSRState state;
    uint8_t gateIn; // flag to signalize wether the gate input is high or low
    uint8_t gatePrevious; // the previous state of the flag, needed to detect edges and base dead timing on those
    float accumulator;
    float A_inc, D_inc, S_lev, R_inc;
    float A_cv, D_cv, S_cv, R_cv;
    uint16_t tick; // time passed inside the envelope
    float length; // length of the envelope in s
    uint8_t process_ui; // flag to signalize wether ui-inputs should be processed
    uint16_t r_cv; // varible that holds the cv for release stage, needed for recalculation
    ADSRMode mode; // determines the mode of the adsr
    uint16_t output; // 12 bits
    float envelope_threshold;
    float threshold_cv; // to be compared with an incoming signal
    uint16_t deadtime_counter; // prevents triggering the envelope in cv mode within an untrustworthy time span (for example due to noise...)

    ADSR();
    void processGate(Gate* gate);
    void uiInit(void);
    void update();
    static float mapTimeTarget(uint16_t CV, float min, float max);
    void processThresholdPot(uint16_t CV1);
    void processThresholdCv(uint16_t CV);
    void setLengthThreshold(void);
    void processAdsrCv(uint16_t CVA, uint16_t CVD, uint16_t CVS, uint16_t CVR);
    void setA(void);
    void setD(void);
    void setS(void);
    void setR(void);
    void setROutput(void);
    void setMode(uint32_t cv, uint8_t routing);
    void processSample(void);
};

extern ADSR rollo_env;
}
#endif /* ADSR_H_ */
