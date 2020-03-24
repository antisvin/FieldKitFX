/*
 * routingMatrix.h
 *
 */

#ifndef ROUTINGMATRIX_H_
#define ROUTINGMATRIX_H_

#include "hardware/button_array.h"
#include "hardware/cv_router.h"
#include "hardware/rgb_led_array.h"
#include "utils/utils.h"

#define MATRIX_SWITCHUPDATE_DELAY 700

namespace fieldkitfx {

class RoutingMatrix {
private:
    uint32_t switchUpdateStartTime;
    bool switchUpdateFlag;
    DISALLOW_COPY_AND_ASSIGN(RoutingMatrix);

public:
    LedDriver U55, U54;
    ButtonArray ba;
    RgbLedArray la;
    CvRouter router;
    SPI_HandleTypeDef spiController;

    RoutingMatrix() = default;
    void init();
    void reset();
    void updateButtonStates();
    void updateCvDestinations();
    void syncLedsToDestinations();
    void forceSyncLedsToDestinations();
};

extern RoutingMatrix cvMatrix;

}

#endif /* ROUTINGMATRIX_H_ */
