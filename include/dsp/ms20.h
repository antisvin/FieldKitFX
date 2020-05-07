#ifndef _FILTERS_MS20_H_
#define _FILTERS_MS20_H_

#include <cmath>
#include "dsp/dsp.h"

namespace fieldkitfx {

class Ms20Filter : public DspEffect {

public:
    Ms20Filter() {
    }

    void process(const float* input, float* output) {
        float fSlow0 = std::tan((fConst0 * std::pow(10.0f, ((3 * fc) + 1.0f))));
        float fSlow1 = (fSlow0 + 1.0f);
        float fSlow2 = (fSlow0 / fSlow1);
        float fSlow3 = (resonance + -0.707000017f);
        float fSlow4 = (1.0f - fSlow2);
        float fSlow5 = (1.0f /
            (1.0f - (0.215215757f * (((fSlow0 * fSlow3) * fSlow4) / fSlow1))));
        float fSlow6 = (1.0f / fSlow1);
        float fSlow7 = (0.215215757f * (fSlow3 * fSlow4));
        float fSlow8 = (0.0f - fSlow6);
        float fSlow9 = (2.0f * fSlow2);
        float fSlow10 = (0.215215757f * fSlow3);
        for (int i = 0; (i < USER_AUDIO_IO_BUFFER_SIZE); i = (i + 1)) {
            float fTemp0 = (input[i] - fRec3[1]);
            float fTemp1 =
                ((fSlow5 *
                     ((fRec3[1] + (fSlow6 * ((fSlow0 * fTemp0) + (fSlow7 * fRec1[1])))) +
                         (fSlow8 * fRec2[1]))) -
                    fRec1[1]);
            float fTemp2 = (fRec1[1] + (fSlow2 * fTemp1));
            float fRec0 = fTemp2;
            fRec1[0] = (fRec1[1] + (fSlow9 * fTemp1));
            fRec2[0] = (fRec2[1] + (fSlow9 * ((fSlow10 * fTemp2) - fRec2[1])));
            fRec3[0] = (fRec3[1] + (fSlow9 * fTemp0));

            if (fabsf(fRec0) >= 1.0f) {
                fRec0 = copysign(2.0f / 3.0f, fRec0);
            }
            else {
                fRec0 = fRec0 - fRec0 * fRec0 * fRec0 / 3.0f;
            }

            output[i] = fRec0;
            fRec1[1] = fRec1[0];
            fRec2[1] = fRec2[0];
            fRec3[1] = fRec3[0];
        }
    }

    void updateParams(const DspParam* param1, const DspParam* param2) {
        fc = (float)param1->last_result / 4095.0f;
        resonance = (float)param2->last_result / 4095.0f * 10;
    }

private:
    float fc, resonance;
    float fRec1[2];
    float fRec2[2];
    float fRec3[2];
    static constexpr float fSampleRate = 48000.0f;
    static constexpr float fConst0 = (6.28318548f /
        std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate))));
};

};

#endif