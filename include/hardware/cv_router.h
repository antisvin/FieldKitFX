/*
 * CVRouter.h
 *
 */

#ifndef CVROUTER_H_
#define CVROUTER_H_

#include "stm32f3xx_hal.h"
#include "stm32f3xx_ll_spi.h"
#include "utils/utils.h"

/*
 * Defines
 */
#define MATRIX_SPI_TIMEOUT 100
#define MATRIX_LOADPORT GPIOF
#define MATRIX_LOADPIN 2
#define MATRIX_SPI_CONTROLLER SPI1

#define CV_SOURCE_1 0x01
#define CV_SOURCE_2 0x02
#define CV_SOURCE_3 0x04
#define CV_SOURCE_4 0x08
#define CV_SOURCE_REFERENCE 0x10

namespace fieldkitfx {
class CvRouter {
private:
    SPI_HandleTypeDef* spi_controller;
    DISALLOW_COPY_AND_ASSIGN(CvRouter);

public:
    uint8_t state[12];

    CvRouter() = default;
    void init(SPI_HandleTypeDef* spi);
    void disableAllRoutings();
    void setDestination(uint8_t src, uint8_t dst);
    void cycleSource(uint8_t dst);
    uint8_t getSource(uint8_t dst);
    void setSwitches();
};
}
#endif /* CVROUTER_H_ */
