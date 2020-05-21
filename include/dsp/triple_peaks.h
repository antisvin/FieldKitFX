#ifndef _FILTERS_TRIPLE_PEAKS_H_
#define _FILTERS_TRIPLE_PEAKS_H_

#include <cmath>
#include "engine/filters.h"

namespace fieldkitfx {

class TriplePeaksFilter : public BaseFilterEffect {

public:
    TriplePeaksFilter() {
    }

    void process(const float* input, float* output) {
        float fSlow0 = fc;
        float fSlow1 =
            std::tan((fConst0 * std::pow(10.0f, ((3.0f * fSlow0) + 1.0f))));
        float fSlow2 = (fSlow1 + 0.5f);
        float fSlow3 = ((fSlow1 * fSlow2) + 1.0f);
        float fSlow4 = (fSlow1 / fSlow3);
        float fSlow5 = (2.0f * fSlow1);
        float fSlow6 = (2.0f * fSlow4);
        float fSlow7 = float(band_width);
        float fSlow8 = std::tan((fConst0 *
            std::pow(10.0f, ((3.0f * ((1.0f - fSlow7) * fSlow0)) + 1.0f))));
        float fSlow9 = (1.0f / ((2.0f * fSlow7) + 1.0f));
        float fSlow10 = (fSlow9 + fSlow8);
        float fSlow11 = ((fSlow8 * fSlow10) + 1.0f);
        float fSlow12 = (fSlow8 / fSlow11);
        float fSlow13 = (2.0f * fSlow8);
        float fSlow14 = (2.0f * fSlow12);
        float fSlow15 = std::tan((fConst0 *
            std::pow(10.0f, ((3.0f * (fSlow7 + (fSlow0 * (1.0f - fSlow7)))) + 1.0f))));
        float fSlow16 = (fSlow9 + fSlow15);
        float fSlow17 = ((fSlow15 * fSlow16) + 1.0f);
        float fSlow18 = (fSlow15 / fSlow17);
        float fSlow19 = (2.0f * fSlow15);
        float fSlow20 = (2.0f * fSlow18);
        for (int i = 0; (i < USER_AUDIO_IO_BUFFER_SIZE); i = (i + 1)) {
            float fTemp0 = (0.5f * float(input[i]));
            float fTemp1 = (fTemp0 - (fRec1[1] + (fSlow2 * fRec2[1])));
            float fTemp2 = (fRec2[1] + (fSlow4 * fTemp1));
            float fRec0 = fTemp2;
            fRec1[0] = (fRec1[1] + (fSlow5 * fTemp2));
            float fTemp3 = (fRec2[1] + (fSlow6 * fTemp1));
            fRec2[0] = fTemp3;
            float fTemp4 = (fTemp0 - (fRec4[1] + (fSlow10 * fRec5[1])));
            float fTemp5 = (fRec5[1] + (fSlow12 * fTemp4));
            float fRec3 = fTemp5;
            fRec4[0] = (fRec4[1] + (fSlow13 * fTemp5));
            float fTemp6 = (fRec5[1] + (fSlow14 * fTemp4));
            fRec5[0] = fTemp6;
            float fTemp7 = (fTemp0 - (fRec7[1] + (fSlow16 * fRec8[1])));
            float fTemp8 = (fRec8[1] + (fSlow18 * fTemp7));
            float fRec6 = fTemp8;
            fRec7[0] = (fRec7[1] + (fSlow19 * fTemp8));
            float fTemp9 = (fRec8[1] + (fSlow20 * fTemp7));
            fRec8[0] = fTemp9;
            float fTemp10 = std::max<float>(
                -1.0f, std::min<float>(1.0f, ((fRec0 + fRec3) + fRec6)));
            float out = float(
                (fTemp10 * (1.0f - (0.333333343f * mydsp_faustpower2_f(fTemp10)))));

            output[i] = clipper.process(out) * 0.9;

            fRec1[1] = fRec1[0];
            fRec2[1] = fRec2[0];
            fRec4[1] = fRec4[0];
            fRec5[1] = fRec5[0];
            fRec7[1] = fRec7[0];
            fRec8[1] = fRec8[0];
        }
    }

    void updateParams(const DspParam* param1, const DspParam* param2) {
        fc = (float)param1->last_result / 4095.0f;
        band_width = (float)param2->last_result / 4095.0f;
    }

private:
    float fc, band_width;
    float fRec1[2];
    float fRec2[2];
    float fRec4[2];
    float fRec5[2];
    float fRec7[2];
    float fRec8[2];

    static constexpr float fSampleRate = 48000.0f;
    static constexpr float fConst0 = (6.28318548f /
        std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate))));
};

};

#endif