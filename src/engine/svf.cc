#include <engine/svf.h>
#include <utils/fast_math.h>

namespace fieldkitfx {

void StateVariableFilter::setLowPass(float fc, float q) {
    const float w = tanf(fc);
    const float g = w;
    const float k = 1. / q;
    m_a1 = 1. / (1. + g * (g + k));
    m_a2 = g * m_a1;
    m_a3 = g * m_a2;
    m_m0 = 0;
    m_m1 = 0;
    m_m2 = 1.;
}

void StateVariableFilter::setHighPass(float fc, float q) {
    const float w = tanf(fc);
    const float g = w;
    const float k = 1. / q;
    m_a1 = 1. / (1. + g * (g + k));
    m_a2 = g * m_a1;
    m_a3 = g * m_a2;
    m_m0 = 1.;
    m_m1 = -k;
    m_m2 = -1.;
}

void StateVariableFilter::setBandPass(float fc, float q) {
    const float w = tanf(fc);
    const float g = w;
    const float k = 1. / q;
    m_a1 = 1. / (1. + g * (g + k));
    m_a2 = g * m_a1;
    m_a3 = g * m_a2;
    m_m0 = 0.;
    m_m1 = 1.;
    m_m2 = 0.;
}

void StateVariableFilter::setNotch(float fc, float q) {
    const float w = tanf(fc);
    const float g = w;
    const float k = 1. / q;
    m_a1 = 1. / (1. + g * (g + k));
    m_a2 = g * m_a1;
    m_a3 = g * m_a2;
    m_m0 = 1.;
    m_m1 = -k;
    m_m2 = 0.;
}

void StateVariableFilter::setPeak(float fc, float q) {
    const float w = tanf(fc);
    const float g = w;
    const float k = 1. / q;
    m_a1 = 1. / (1. + g * (g + k));
    m_a2 = g * m_a1;
    m_a3 = g * m_a2;
    m_m0 = 1.;
    m_m1 = -k;
    m_m2 = -2.;
}

void StateVariableFilter::setBell(float fc, float q, float gain) {
    const float w = tanf(fc);
    const float A = fast_exp10f(gain / 40.);
    const float g = w;
    const float k = 1 / q;
    m_a1 = 1. / (1. + g * (g + k));
    m_a2 = g * m_a1;
    m_a3 = g * m_a2;
    m_m0 = 1.;
    m_m1 = k * (A * A - 1.);
    m_m2 = 0.;
}

void StateVariableFilter::setLowShelf(float fc, float q, float gain) {
    const float w = tanf(fc);
    const float A = fast_exp10f(gain / 40.);
    const float g = w / sqrtf(A);
    const float k = 1. / q;
    m_a1 = 1. / (1. + g * (g + k));
    m_a2 = g * m_a1;
    m_a3 = g * m_a2;
    m_m0 = 1.;
    m_m1 = k * (A - 1.);
    m_m2 = (A * A - 1.);
}

void StateVariableFilter::setHighShelf(float fc, float q, float gain) {
    const float w = tanf(fc);
    const float A = fast_exp10f(gain / 40.);
    const float g = w / sqrtf(A);
    const float k = 1. / q;
    m_a1 = 1. / (1. + g * (g + k));
    m_a2 = g * m_a1;
    m_a3 = g * m_a2;
    m_m0 = A * A;
    m_m1 = k * (1. - A) * A;
    m_m2 = (1. - A * A);
}

void StateVariableFilter::process(const float* in, float* out, uint8_t size) {
    float v0, v1, v2, v3;
    for (size_t s = 0; s < size; s++) {
        v0 = in[s];
        v3 = v0 - mIc2eq;
        v1 = m_a1 * mIc1eq + m_a2 * v3;
        v2 = mIc2eq + m_a2 * mIc1eq + m_a3 * v3;
        mIc1eq = 2. * v1 - mIc1eq;
        mIc2eq = 2. * v2 - mIc2eq;
        out[s] = m_m0 * v0 + m_m1 * v1 + m_m2 * v2;
    }
}
}
