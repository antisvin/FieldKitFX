#ifndef _ENGINE_SVF_H_
#define _ENGINE_SVF_H_

#include <engine/engine.h>

namespace fieldkitfx {

class StateVariableFilter {
public:
    enum Mode {
        SVF_LO,
        SVF_HI,
        SVF_BP,
        SVF_NO,
        SVF_PK,
        SVF_BL,
        SVF_LS,
        SVF_HS,
        SVF_NUM_MODES,
    };

    void setLowPass(float fc, float q);
    void setHighPass(float fc, float q);
    void setBandPass(float fc, float q);
    void setNotch(float fc, float q);
    void setPeak(float fc, float q);
    void setBell(float fc, float q, float gain);
    void setLowShelf(float fc, float q, float gain);
    void setHighShelf(float fc, float q, float gain);
    void process(const float* in, float* out, uint8_t size);

private:
    // coefficients
    float m_a1 = 0.f;
    float m_a2 = 0.f;
    float m_a3 = 0.f;
    float m_m0 = 0.f;
    float m_m1 = 0.f;
    float m_m2 = 0.f;
    float mIc1eq = 0.f;
    float mIc2eq = 0.f;
};

}

#endif
