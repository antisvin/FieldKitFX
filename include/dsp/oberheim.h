#ifndef _FILTERS_OBERHEIM_H_
#define _FILTERS_OBERHEIM_H_

#include <cmath>
#include "dsp/dsp.h"

namespace fieldkitfx {

class OberheimSvfFilter : public DspEffect {

public:
    OberheimSvfFilter() {
    }

    void process(const float* input, float* output) {
        float fSlow0 = morph;
        int iSlow1 = (fSlow0 < 0.5f);
        float fSlow2 = (2.0f * (1.0f - fSlow0));
        float fSlow3 = std::tan((fConst0 * std::pow(10.0f, ((3.0f * fc) + 1.0f))));
        float fSlow4 = (fSlow3 + 0.100000001f);
        float fSlow5 = ((fSlow3 * fSlow4) + 1.0f);
        float fSlow6 = (fSlow3 / fSlow5);
        float fSlow7 = (1.0f / fSlow5);
        float fSlow8 = (2.0f * fSlow3);
        float fSlow9 = (1.0f - fSlow2);
        float fSlow10 = (2.0f * fSlow0);
        float fSlow11 = (1.0f - fSlow10);
        for (int i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            float fTemp0 = (input[i] - (fRec3[1] + (fSlow4 * fRec4[1])));
            float fTemp1 = (fSlow6 * fTemp0);
            float fTemp2 =
                std::max<float>(-1.0f, std::min<float>(1.0f, (fRec4[1] + fTemp1)));
            float fTemp3 =
                (fTemp2 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp2))));
            float fTemp4 = (fSlow3 * fTemp3);
            float fRec0 = (fRec3[1] + fTemp4);
            float fTemp5 = (fSlow7 * fTemp0);
            float fRec1 = fTemp5;
            float fRec2 = fTemp3;
            fRec3[0] = (fRec3[1] + (fSlow8 * fTemp3));
            fRec4[0] = (fTemp1 + fTemp3);
            output[i] = float((iSlow1 ? ((fSlow10 * fRec2) + (fSlow11 * fRec0))
                                      : ((fSlow2 * fRec2) + (fSlow9 * fRec1))));
            fRec3[1] = fRec3[0];
            fRec4[1] = fRec4[0];
        }
    }

    void updateParams(const DspParam* param1, const DspParam* param2) {
        fc = (float)param1->last_result / 4095.0f;
        morph = (float)param2->last_result / 4095.0f;
    }

private:
    float fc, morph;
    float fRec3[2];
    float fRec4[2];
    static constexpr float fSampleRate = 48000.0f;
    static constexpr float fConst0 = (6.28318548f /
        std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate))));

    static float mydsp_faustpower2_f(float value) {
        return (value * value);
    }
};

}

#endif