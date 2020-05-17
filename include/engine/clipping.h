#ifndef _ENGINE_CLIPPING_H_
#define _ENGINE_CLIPPING_H_

#include <algorithm>
#include <cmath>
#include "utils/wavetables.h"
#include "stmlib/dsp/dsp.h"

namespace fieldkitfx {

// Source: https://github.com/fabianesqueda/Agave/blob/master/src/dsp/Waveshaping.hpp

/*
Other clipping algos we could try:

Tangential soft clipper:
F0(x) = tanh(x)
F1(x) = ln(cosh(x))

tanh(x) can be approximated with sign(x) when |x| >> 1
then
F1(x) ~~ F1(x0 * sign(x)) + (x ** 2 - x0 ** 2) / 2 * sign(x) , if |x| >= x0 >> 1

-----

Exponential soft clipper:
|x| > 2/3 -> sgn(x)
else -> sgn(x) * (1 - (|3x / 2 - sgn(x)|) ** E)

Sinusoid soft clipper:
|x| > 2/3 -> sgn(x)
else -> sin(3 * PI * x / 4)

Two stage quadratic soft clipper
|x| > 2/3 -> sgn(x)
1/3 <= |x| <= 2/3 -> sgn(x) * (3 - (2 - |3 * x|) ** 2) / 3
- 1/3 < x < 1/3 -> 2 * x

Cubic:
|x| > 2/3 -> sgn(x)
else -> 9 * x / 4 - 27 * (x ** 3) / 16

Reciprocal:
sgn(x) * (1 - 1 / (|30 * x| + 1))
*/

static constexpr float onesixths = 1.0f / 6.0f;

class BaseClipper {

public:
    float process(float input) {
        return antialiasedClipN1(input);
    }

    void reset() {
        xn1 = 0.0f;
        Fn = 0.0f;
        Fn1 = 0.0f;
    }

protected:
    // Antialiasing variables
    float xn1;
    float Fn;
    float Fn1;
    static constexpr float thresh = 10.0e-2;

    virtual float clipN0(float x) = 0;
    virtual float clipN1(float x) = 0;
    virtual float clipN2(float x) = 0;

    float signum(float x) {
        return (x > 0.0f) ? 1.0f : ((x < 0.0f) ? -1.0f : 0.0f);
    }

    float antialiasedClipN1(float x) {
        Fn = clipN1(x);
        float tmp = 0.0;
        if (std::abs(x - xn1) < thresh) {
            tmp = clipN0(0.5f * (x + xn1));
        }
        else {
            tmp = (Fn - Fn1) / (x - xn1);
        }

        // Update states
        xn1 = x;
        Fn1 = Fn;

        return tmp;
    }
};

class CubicClipper : public BaseClipper {
public:
    float clipN0(float x) override {
        if (std::abs(x) < 1) {
            return (1.5f - 0.5f * x * x) * x;
        }
        else {
            return signum(x);
        }
        /*
        if (std::abs(x) < 1) {
            return (1.5f - 0.5f * x * x) * x;
        }
        else {
            return signum(x);
        }
        */
    }

    float clipN1(float x) override {
        if (std::abs(x) < 1) {
            float sqr_x = x * x;
            // return (0.75f - 0.125f * sqr_x) * sqr_x;
            return sqr_x * x - 0.05 * sqr_x * sqr_x * x;
        }
        else {
            return signum(x) * x;
        }
    }

    float clipN2(float x) override {
        if (std::abs(x) < 1) {
            float sqr_x = x * x;
            return 0.25 * sqr_x * x - 0.025 * sqr_x * sqr_x * x;
        }
        else {
            return onesixths * signum(x) * x * x;
        }
    }
};

class SineClipper : public BaseClipper {
public:
    float clipN0(float x) override {
        if (std::abs(x) < 1) {
            return std::sin(0.5 * M_PI * x);
        }
        else {
            return signum(x);
        }

        // else -> sin(3 * PI * x / 4)
    }

    float clipN1(float x) override {
        if (std::abs(x) < 1) {
            return 2.0 / M_PI - (2.0f / M_PI) * std::cos(0.5 * M_PI * x);
        }
        else {
            return signum(x);
        }
    }

    float clipN2(float x) override {
        // second antiderivative of hardClipN0
    }
};

class HardClipper : public BaseClipper {
public:
    float clipN0(float x) override {
        // Hard clipping function
        return 0.5f * (signum(x + 1.0f) * (x + 1.0f) - signum(x - 1.0f) * (x - 1.0f));
    }

    float clipN1(float x) override {
        // First antiderivative of hardClipN0
        return 0.25f *
            (signum(x + 1.0f) * (x + 1.0f) * (x + 1.0f) -
                signum(x - 1.0f) * (x - 1.0f) * (x - 1.0f) - 2.0f);
    }

    float clipN2(float x) override {
        // second antiderivative of hardClipN0
        return oneTwelfth *
            (signum(x + 1.0f) * (x + 1.0f) * (x + 1.0f) * (x + 1.0f) -
                signum(x - 1.0f) * (x - 1.0f) * (x - 1.0f) * (x - 1.0f) - 6.0f * x);
    }

private:
    static constexpr float oneTwelfth = 1.0 / 12.0;
};
}

#endif