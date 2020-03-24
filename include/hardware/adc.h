/*
 * ADC.h
 *
 */

/*
 * Summary of the CV inputs :
 *
 * 		- Mixed CV1 (Bipolar)	-> PA0	->	ADC1 IN1
 * 		- Mixed CV2 (Bipolar)	-> PA1	->	ADC1 IN2
 * 		- Rollo CV				-> PA2	->	ADC1 IN3
 * 		- Threshold Pot			-> PA3	->	ADC1 IN4
 * 		- Threshold CV			-> PF4  -> 	ADC1 IN5
 */

#ifndef ADC_H_
#define ADC_H_

/*
 * HAL include(s)
 */
#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
//#include "utils/moving_average_filter.h"

#define ADC_INSTANCE ADC1
#define NB_CV_IN 5
#define NB_CV_CNV_MULTIPLIER 10
#define NB_CNV NB_CV_IN* NB_CV_CNV_MULTIPLIER
#define ADC_RESOLUTION_BIT 12
#if ADC_RESOLUTION_BIT == 12
#define ADC_RESOLUTION_DEZ 4095
#elif ADC_RESOLUTION_BIT == 10
#define ADC_RESOLUTION_DEZ 1023
#else
#error "undefined ADC bitdepth"
#endif

/*
 * CV acquisition functions
 */
namespace fieldkitfx {
extern "C" {
void ADC_Init(void);
void ADC_GPIOConfig(void);
void ADC_Start(void);
void ADC_Xfr_cplt(DMA_HandleTypeDef* hdma);
void ADC_Xfr_Half_cplt(DMA_HandleTypeDef* hdma);
uint32_t ADC_getMixedCV1(void);
uint32_t ADC_getMixedCV2(void);
uint32_t ADC_getRolloCV(void);
uint32_t ADC_getThresholdPot(void);
uint32_t ADC_getThresholdCV(void);
}
}
#endif /* ADC_H_ */
