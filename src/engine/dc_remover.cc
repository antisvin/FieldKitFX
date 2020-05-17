/*
 * DCRemover.c
 *
 */

#include "engine/dc_remover.h"

namespace fieldkitfx {

DCRemover::DCRemover(float pole)
    : pole(pole) {};

float DCRemover::process(float inputSample) {
    float temp = inputSample - old_x + old_y * pole;
    old_x = inputSample;
    old_y = temp;
    return temp;
};
}