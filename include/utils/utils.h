/*
 * randomUtils.h
 *
 */

#ifndef RANDOMUTILS_H_
#define RANDOMUTILS_H_

#include <cstdint>
#include "stmlib/dsp/dsp.h"

namespace fieldkitfx {
/*
 * Random little utility functions to manipulate data
 */

/*
 * clips a float to [-1.0,1.0]
 */
inline float clip(float input) {
    if (input > 1.0f)
        return 1.0f;
    else if (input < -1.0f)
        return -1.0f;
    else
        return input;
}

/*
 * scales an unsigned 12bit signal to a float [-1.0,1.0]
 */
inline float scale_uint12_to_float(uint16_t input) {
    return clip(((float)input) / 2047.0f - 1.0f);
}

/*
 * scales a float [-1.0,1.0] to a unsigned 12bit signal
 */
inline uint16_t scale_float_to_uint12(float input) {
    // return (uint16_t)((clip(input) + 1.0) * 2047U);
    return 2047U + (int16_t)(clip(input) * 2047.0);
}

/*
 * scales an signed 16bit signal to a float [-1.0,1.0]
 */
inline float scale_int16_to_float(int16_t input) {
    return (float)input / 32767.0;
}

/*
 * scales a float [-1.0,1.0] to a signed 16bit signal
 */
inline int16_t scale_float_to_int16(float input) {
    return (int16_t)(input * 32767.0);
}

/* crossfades two values, fade parameter must be in [0.0, 1.0] range */
inline float crossfade(float a, float b, float fade) {
    return a * fade + b * (1 - fade);
}

}

/*
#define DISALLOW_COPY_AND_ASSIGN(TypeName)                                     \
    TypeName(const TypeName&) = delete;                                        \
    void operator=(const TypeName&) = delete;
*/

#endif /* RANDOMUTILS_H_ */
