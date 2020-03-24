/*
 * Xfader.c
 *
 */

#include "Xfader.h"

float Xfader::process(float in1, float in2) {
	return lambda * in1 + (1.0 - lambda) * in2;
}
