#ifndef _DSP_FORMANTS_H_
#define _DSP_FORMANTS_H_

#include <cmath>
#include "engine/filters.h"

namespace fieldkitfx {
#if 0
static constexpr uint8_t voice_type_offset = 3;
static constexpr uint8_t voice_type_0 = 0 << voice_type_offset;
static constexpr uint8_t voice_type_1 = 1 << voice_type_offset;
static constexpr uint8_t voice_type_2 = 2 << voice_type_offset;
static constexpr uint8_t voice_type_3 = 3 << voice_type_offset;
static constexpr uint8_t voice_type_4 = 4 << voice_type_offset;

static constexpr uint8_t vowel_mask = 0x7;
static constexpr uint8_t vowel_a = 0;
static constexpr uint8_t vowel_b = 1;
static constexpr uint8_t vowel_c = 2;
static constexpr uint8_t vowel_d = 3;
static constexpr uint8_t vowel_e = 4;

static const uint8_t formants_map[25] { voice_type_0 | vowel_a,
    voice_type_0 | vowel_b, voice_type_0 | vowel_c, voice_type_0 | vowel_d,
    voice_type_0 | vowel_e, voice_type_1 | vowel_e, voice_type_1 | vowel_d,
    voice_type_1 | vowel_c, voice_type_1 | vowel_b, voice_type_1 | vowel_a,
    voice_type_2 | vowel_a, voice_type_2 | vowel_b, voice_type_2 | vowel_c,
    voice_type_2 | vowel_d, voice_type_2 | vowel_e, voice_type_3 | vowel_e,
    voice_type_3 | vowel_d, voice_type_3 | vowel_c, voice_type_3 | vowel_b,
    voice_type_3 | vowel_a, voice_type_4 | vowel_a, voice_type_4 | vowel_b,
    voice_type_4 | vowel_c, voice_type_4 | vowel_d, voice_type_4 | vowel_e };
#endif

class FormantFilter : public BaseFilterEffect {
public:
    FormantFilter() {
    }

    void process(const float* input, float* output) {
        float fSlow0 = vowel;
        float fSlow1 = voice_type;
        float fSlow2 = (fSlow0 + (5.0f * fSlow1));
        int iSlow3 = (fSlow2 < 23.0f);
        int iSlow4 = (fSlow2 < 24.0f);
        float fSlow5 = (fSlow2 + -23.0f);
        int iSlow6 = (fSlow2 < 22.0f);
        float fSlow7 = (fSlow2 + -22.0f);
        int iSlow8 = (fSlow2 < 21.0f);
        float fSlow9 = (fSlow2 + -21.0f);
        int iSlow10 = (fSlow2 < 20.0f);
        float fSlow11 = (fSlow2 + -20.0f);
        int iSlow12 = (fSlow2 < 19.0f);
        float fSlow13 = (fSlow2 + -19.0f);
        int iSlow14 = (fSlow2 < 18.0f);
        int iSlow15 = (fSlow2 < 17.0f);
        int iSlow16 = (fSlow2 < 16.0f);
        int iSlow17 = (fSlow2 < 15.0f);
        int iSlow18 = (fSlow2 < 14.0f);
        float fSlow19 = (fSlow2 + -14.0f);
        int iSlow20 = (fSlow2 < 13.0f);
        float fSlow21 = (fSlow2 + -13.0f);
        int iSlow22 = (fSlow2 < 12.0f);
        float fSlow23 = (fSlow2 + -12.0f);
        int iSlow24 = (fSlow2 < 11.0f);
        float fSlow25 = (fSlow2 + -11.0f);
        int iSlow26 = (fSlow2 < 10.0f);
        float fSlow27 = (fSlow2 + -10.0f);
        float fSlow28 = (50.0f * fSlow27);
        int iSlow29 = (fSlow2 < 9.0f);
        float fSlow30 = (fSlow2 + -9.0f);
        int iSlow31 = (fSlow2 < 8.0f);
        float fSlow32 = (fSlow2 + -8.0f);
        float fSlow33 = (50.0f * fSlow32);
        int iSlow34 = (fSlow2 < 7.0f);
        float fSlow35 = (fSlow2 + -7.0f);
        int iSlow36 = (fSlow2 < 6.0f);
        float fSlow37 = (fSlow2 + -6.0f);
        int iSlow38 = (fSlow2 < 5.0f);
        float fSlow39 = (fSlow0 - (5.0f * (1.0f - fSlow1)));
        float fSlow40 = (350.0f * fSlow39);
        int iSlow41 = (fSlow2 < 4.0f);
        float fSlow42 = (fSlow2 + -4.0f);
        float fSlow43 = (iSlow3
                ? (iSlow6 ? (iSlow8 ? (iSlow10
                                              ? (iSlow12
                                                        ? (iSlow14
                                                                  ? (iSlow15
                                                                            ? (iSlow16
                                                                                      ? (iSlow17
                                                                                                ? (iSlow18
                                                                                                          ? (iSlow20
                                                                                                                    ? (iSlow22
                                                                                                                              ? (iSlow24
                                                                                                                                        ? (iSlow26 ? (iSlow29 ? (iSlow31 ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? 4950.0f : (iSlow38 ? (4950.0f - (2200.0f * fSlow42)) : 2750.0f))
                                                                                                                                                                                                          : (iSlow36 ? (fSlow40 + 2750.0f) : 3100.0f))
                                                                                                                                                                                               : (iSlow34 ? ((240.0f * fSlow37) + 3100.0f) : 3340.0f))
                                                                                                                                                                                    : (iSlow31 ? (3340.0f - (440.0f * fSlow35)) : 2900.0f))
                                                                                                                                                                         : (iSlow29 ? (fSlow33 + 2900.0f) : 2950.0f))
                                                                                                                                                              : (iSlow26 ? ((400.0f * fSlow30) + 2950.0f) : 3350.0f))
                                                                                                                                                   : (iSlow24 ? (3350.0f - fSlow28) : 3300.0f))
                                                                                                                                        : (iSlow22 ? ((290.0f * fSlow25) + 3300.0f) : 3590.0f))
                                                                                                                              : (iSlow20 ? (3590.0f - (290.0f * fSlow23)) : 3300.0f))
                                                                                                                    : (iSlow18 ? ((100.0f * fSlow21) + 3300.0f) : 3400.0f))
                                                                                                          : (iSlow17 ? ((1550.0f * fSlow19) + 3400.0f)
                                                                                                                     : 4950.0f))
                                                                                                : 4950.0f)
                                                                                      : 4950.0f)
                                                                            : 4950.0f)
                                                                  : 4950.0f)
                                                        : (iSlow10 ? (4950.0f - (1700.0f * fSlow13))
                                                                   : 3250.0f))
                                              : (iSlow8 ? ((330.0f * fSlow11) + 3250.0f) : 3580.0f))
                                    : (iSlow6 ? (3580.0f - (40.0f * fSlow9)) : 3540.0f))
                          : (iSlow3 ? (3540.0f - (540.0f * fSlow7)) : 3000.0f))
                : (iSlow4 ? ((300.0f * fSlow5) + 3000.0f) : 3300.0f));
        float fSlow44 = std::tan((fConst0 * fSlow43));
        float fSlow45 = (1.0f / fSlow44);
        float fSlow46 = (20.0f * fSlow11);
        float fSlow47 = (fSlow2 + -18.0f);
        float fSlow48 = (fSlow2 + -16.0f);
        float fSlow49 = (80.0f * fSlow48);
        float fSlow50 = (fSlow2 + -15.0f);
        float fSlow51 = (20.0f * fSlow19);
        float fSlow52 = (20.0f * fSlow27);
        float fSlow53 = (20.0f * fSlow30);
        float fSlow54 = (10.0f * fSlow39);
        int iSlow55 = (fSlow2 < 3.0f);
        float fSlow56 = (fSlow2 + -3.0f);
        int iSlow57 = (fSlow2 < 2.0f);
        float fSlow58 = (fSlow2 + -2.0f);
        int iSlow59 = (fSlow2 < 1.0f);
        int iSlow60 = (fSlow2 < 0.0f);
        float fSlow61 =
            ((iSlow3 ? (iSlow6 ? (iSlow8 ? (iSlow10
                                                   ? (iSlow12
                                                             ? (iSlow14
                                                                       ? (iSlow15
                                                                                 ? (iSlow16
                                                                                           ? (iSlow17
                                                                                                     ? (iSlow18
                                                                                                               ? (iSlow20
                                                                                                                         ? (iSlow22
                                                                                                                                   ? (iSlow24
                                                                                                                                             ? (iSlow26 ? (iSlow29 ? (iSlow31 ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? (iSlow55 ? (iSlow57 ? (iSlow59 ? (iSlow60 ? 140.0f : (iSlow59 ? ((60.0f * fSlow2) + 140.0f) : 200.0f)) : 200.0f)
                                                                                                                                                                                                                                                : (iSlow55 ? (200.0f - (65.0f * fSlow58)) : 135.0f))
                                                                                                                                                                                                                                     : (iSlow41 ? ((65.0f * fSlow56) + 135.0f) : 200.0f))
                                                                                                                                                                                                                          : (iSlow38 ? (200.0f - (70.0f * fSlow42)) : 130.0f))
                                                                                                                                                                                                               : (iSlow36 ? (130.0f - fSlow54) : 120.0f))
                                                                                                                                                                                                    : 120.0f)
                                                                                                                                                                                         : 120.0f)
                                                                                                                                                                              : 120.0f)
                                                                                                                                                                   : (iSlow26 ? (fSlow53 + 120.0f) : 140.0f))
                                                                                                                                                        : (iSlow24 ? (140.0f - fSlow52) : 120.0f))
                                                                                                                                             : 120.0f)
                                                                                                                                   : 120.0f)
                                                                                                                         : 120.0f)
                                                                                                               : (iSlow17 ? (fSlow51 + 120.0f) : 140.0f))
                                                                                                     : (iSlow16 ? ((60.0f * fSlow50) + 140.0f) : 200.0f))
                                                                                           : (iSlow15 ? (200.0f - fSlow49) : 120.0f))
                                                                                 : 120.0f)
                                                                       : (iSlow12 ? ((80.0f * fSlow47) + 120.0f)
                                                                                  : 200.0f))
                                                             : (iSlow10 ? (200.0f - (60.0f * fSlow13))
                                                                        : 140.0f))
                                                   : (iSlow8 ? (140.0f - fSlow46) : 120.0f))
                                         : 120.0f)
                               : (iSlow3 ? ((15.0f * fSlow7) + 120.0f) : 135.0f))
                     : (iSlow4 ? (135.0f - (15.0f * fSlow5)) : 120.0f)) /
                fSlow43);
        float fSlow62 = (((fSlow45 - fSlow61) / fSlow44) + 1.0f);
        float fSlow63 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow44)));
        float fSlow64 = (((fSlow45 + fSlow61) / fSlow44) + 1.0f);
        int iSlow65 = (((fSlow1 == 0.0f) ? 1 : ((fSlow1 == 3.0f) ? 1 : 0)) == 0);
        float fSlow66 = freq;
        float fSlow67 = ((0.000839999993f * (1000.0f - fSlow66)) + 0.800000012f);
        float fSlow68 = (fSlow2 + -17.0f);
        float fSlow69 =
            (iSlow22 ? (0.100000001f - (0.0841509998f * fSlow25)) : 0.0158489998f);
        float fSlow70 = (iSlow3
                ? (iSlow6 ? (iSlow8 ? (iSlow10
                                              ? (iSlow12
                                                        ? (iSlow14
                                                                  ? (iSlow15
                                                                            ? (iSlow16
                                                                                      ? (iSlow17
                                                                                                ? (iSlow18
                                                                                                          ? (iSlow20
                                                                                                                    ? (iSlow22
                                                                                                                              ? (iSlow24
                                                                                                                                        ? (iSlow26
                                                                                                                                                  ? (iSlow29 ? (iSlow31 ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? (iSlow55 ? (iSlow57 ? 0.00100000005f
                                                                                                                                                                                                                                          : (iSlow55 ? ((0.000777999987f * fSlow58) + 0.00100000005f) : 0.00177800003f))
                                                                                                                                                                                                                               : (iSlow41 ? (0.00177800003f - (0.001147f * fSlow56)) : 0.000630999974f))
                                                                                                                                                                                                                    : (iSlow38 ? ((0.0993689969f * fSlow42) + 0.000630999974f) : 0.100000001f))
                                                                                                                                                                                                         : (iSlow36 ? ((0.0258930009f * fSlow39) + 0.100000001f) : 0.125892997f))
                                                                                                                                                                                              : (iSlow34 ? (0.125892997f - (0.0860819966f * fSlow37)) : 0.0398110002f))
                                                                                                                                                                                   : (iSlow31 ? (0.0398110002f - (0.0298110005f * fSlow35)) : 0.00999999978f))
                                                                                                                                                                        : (iSlow29 ? ((0.00584900007f * fSlow32) + 0.00999999978f)
                                                                                                                                                                                   : 0.0158489998f))
                                                                                                                                                             : (iSlow26 ? (0.0158489998f - (0.00325999991f * fSlow30))
                                                                                                                                                                        : 0.0125890002f))
                                                                                                                                                  : (iSlow24 ? ((0.0874110013f * fSlow27) + 0.0125890002f)
                                                                                                                                                             : 0.100000001f))
                                                                                                                                        : fSlow69)
                                                                                                                              : (iSlow20 ? ((0.00410400005f * fSlow23) + 0.0158489998f)
                                                                                                                                         : 0.0199529994f))
                                                                                                                    : 0.0199529994f)
                                                                                                          : (iSlow17 ? (0.0199529994f - (0.0167909991f * fSlow19))
                                                                                                                     : 0.0031620001f))
                                                                                                : (iSlow16 ? (0.0031620001f - (0.00157700002f * fSlow50))
                                                                                                           : 0.00158499996f))
                                                                                      : (iSlow15 ? ((0.00472499989f * fSlow48) + 0.00158499996f)
                                                                                                 : 0.00631000008f))
                                                                            : (iSlow14 ? (0.00631000008f - (0.00314799999f * fSlow68))
                                                                                       : 0.0031620001f))
                                                                  : (iSlow12 ? (0.0031620001f - (0.00216199993f * fSlow47))
                                                                             : 0.00100000005f))
                                                        : (iSlow10 ? ((0.0784329996f * fSlow13) + 0.00100000005f)
                                                                   : 0.0794330016f))
                                              : (iSlow8 ? ((0.0205669999f * fSlow11) + 0.0794330016f)
                                                        : 0.100000001f))
                                    : (iSlow6 ? (0.100000001f - (0.0683770031f * fSlow9))
                                              : 0.0316229984f))
                          : (iSlow3 ? ((0.0184959993f * fSlow7) + 0.0316229984f)
                                    : 0.0501190014f))
                : 0.0501190014f);
        float fSlow71 = ((0.00366666657f * (400.0f - fSlow66)) + 3.0f);
        float fSlow72 = (iSlow65 ? (fSlow71 * fSlow70) : (fSlow67 * fSlow70));
        float fSlow73 = (fSlow44 * fSlow64);
        float fSlow74 = (0.0f - (fSlow72 / fSlow73));
        float fSlow75 = (100.0f * fSlow5);
        float fSlow76 = (fSlow2 + -1.0f);
        float fSlow77 = (iSlow3
                ? (iSlow6 ? (iSlow8 ? (iSlow10
                                              ? (iSlow12
                                                        ? (iSlow14
                                                                  ? (iSlow15
                                                                            ? (iSlow16
                                                                                      ? (iSlow17
                                                                                                ? (iSlow18
                                                                                                          ? (iSlow20
                                                                                                                    ? (iSlow22
                                                                                                                              ? (iSlow24
                                                                                                                                        ? (iSlow26
                                                                                                                                                  ? (iSlow29
                                                                                                                                                            ? (iSlow31 ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? (iSlow55 ? (iSlow57 ? (iSlow59 ? (iSlow60 ? 3500.0f : (iSlow59 ? (3500.0f - (200.0f * fSlow2)) : 3300.0f))
                                                                                                                                                                                                                                                    : (iSlow57 ? ((400.0f * fSlow76) + 3300.0f) : 3700.0f))
                                                                                                                                                                                                                                         : (iSlow55 ? (3700.0f - (200.0f * fSlow58)) : 3500.0f))
                                                                                                                                                                                                                              : 3500.0f)
                                                                                                                                                                                                                   : (iSlow38 ? (3500.0f - (1050.0f * fSlow42)) : 2450.0f))
                                                                                                                                                                                                        : (iSlow36 ? (fSlow40 + 2450.0f) : 2800.0f))
                                                                                                                                                                                             : (iSlow34 ? ((250.0f * fSlow37) + 2800.0f) : 3050.0f))
                                                                                                                                                                                  : (iSlow31 ? (3050.0f - (450.0f * fSlow35)) : 2600.0f))
                                                                                                                                                                       : (iSlow29 ? ((75.0f * fSlow32) + 2600.0f) : 2675.0f))
                                                                                                                                                            : (iSlow26 ? ((325.0f * fSlow30) + 2675.0f) : 3000.0f))
                                                                                                                                                  : 3000.0f)
                                                                                                                                        : (iSlow22 ? ((350.0f * fSlow25) + 3000.0f) : 3350.0f))
                                                                                                                              : (iSlow20 ? (3350.0f - (350.0f * fSlow23)) : 3000.0f))
                                                                                                                    : 3000.0f)
                                                                                                          : (iSlow17 ? ((900.0f * fSlow19) + 3000.0f)
                                                                                                                     : 3900.0f))
                                                                                                : (iSlow16 ? (3900.0f - (300.0f * fSlow50))
                                                                                                           : 3600.0f))
                                                                                      : (iSlow15 ? ((300.0f * fSlow48) + 3600.0f)
                                                                                                 : 3900.0f))
                                                                            : (iSlow14 ? (3900.0f - (100.0f * fSlow68))
                                                                                       : 3800.0f))
                                                                  : 3800.0f)
                                                        : (iSlow10 ? (3800.0f - (900.0f * fSlow13))
                                                                   : 2900.0f))
                                              : (iSlow8 ? ((300.0f * fSlow11) + 2900.0f) : 3200.0f))
                                    : (iSlow6 ? ((50.0f * fSlow9) + 3200.0f) : 3250.0f))
                          : (iSlow3 ? (3250.0f - (450.0f * fSlow7)) : 2800.0f))
                : (iSlow4 ? (fSlow75 + 2800.0f) : 2900.0f));
        float fSlow78 = std::tan((fConst0 * fSlow77));
        float fSlow79 = (1.0f / fSlow78);
        float fSlow80 = (10.0f * fSlow7);
        float fSlow81 = (10.0f * fSlow11);
        float fSlow82 = (50.0f * fSlow13);
        float fSlow83 = (20.0f * fSlow50);
        float fSlow84 = (10.0f * fSlow27);
        float fSlow85 = (10.0f * fSlow30);
        float fSlow86 = (60.0f * fSlow42);
        float fSlow87 = (20.0f * fSlow58);
        float fSlow88 = (20.0f * fSlow2);
        float fSlow89 =
            ((iSlow3 ? (iSlow6 ? (iSlow8 ? (iSlow10
                                                   ? (iSlow12
                                                             ? (iSlow14
                                                                       ? (iSlow15
                                                                                 ? (iSlow16
                                                                                           ? (iSlow17
                                                                                                     ? (iSlow18
                                                                                                               ? (iSlow20
                                                                                                                         ? (iSlow22 ? (iSlow24 ? (iSlow26 ? (iSlow29 ? (iSlow31 ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? (iSlow55 ? (iSlow57 ? (iSlow59 ? (iSlow60 ? 130.0f : (iSlow59 ? (fSlow88 + 130.0f) : 150.0f)) : 150.0f)
                                                                                                                                                                                                                                                  : (iSlow55 ? (150.0f - fSlow87) : 130.0f))
                                                                                                                                                                                                                                       : (iSlow41 ? ((50.0f * fSlow56) + 130.0f) : 180.0f))
                                                                                                                                                                                                                            : (iSlow38 ? (180.0f - fSlow86) : 120.0f))
                                                                                                                                                                                                                 : 120.0f)
                                                                                                                                                                                                      : 120.0f)
                                                                                                                                                                                           : 120.0f)
                                                                                                                                                                                : 120.0f)
                                                                                                                                                                     : (iSlow26 ? (fSlow85 + 120.0f) : 130.0f))
                                                                                                                                                          : (iSlow24 ? (130.0f - fSlow84) : 120.0f))
                                                                                                                                               : 120.0f)
                                                                                                                                    : 120.0f)
                                                                                                                         : 120.0f)
                                                                                                               : (iSlow17 ? ((10.0f * fSlow19) + 120.0f) : 130.0f))
                                                                                                     : (iSlow16 ? (fSlow83 + 130.0f) : 150.0f))
                                                                                           : (iSlow15 ? (150.0f - (30.0f * fSlow48))
                                                                                                      : 120.0f))
                                                                                 : 120.0f)
                                                                       : (iSlow12 ? ((60.0f * fSlow47) + 120.0f)
                                                                                  : 180.0f))
                                                             : (iSlow10 ? (180.0f - fSlow82) : 130.0f))
                                                   : (iSlow8 ? (130.0f - fSlow81) : 120.0f))
                                         : 120.0f)
                               : (iSlow3 ? (fSlow80 + 120.0f) : 130.0f))
                     : (iSlow4 ? (130.0f - (10.0f * fSlow5)) : 120.0f)) /
                fSlow77);
        float fSlow90 = (((fSlow79 - fSlow89) / fSlow78) + 1.0f);
        float fSlow91 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow78)));
        float fSlow92 = (((fSlow79 + fSlow89) / fSlow78) + 1.0f);
        float fSlow93 =
            (iSlow14 ? ((0.0293140002f * fSlow68) + 0.0501190014f) : 0.0794330016f);
        float fSlow94 = (iSlow3
                ? (iSlow6 ? (iSlow8 ? (iSlow10
                                              ? (iSlow12
                                                        ? (iSlow14
                                                                  ? (iSlow15
                                                                            ? (iSlow16
                                                                                      ? (iSlow17
                                                                                                ? (iSlow18
                                                                                                          ? (iSlow20
                                                                                                                    ? (iSlow22
                                                                                                                              ? (iSlow24
                                                                                                                                        ? (iSlow26
                                                                                                                                                  ? (iSlow29
                                                                                                                                                            ? (iSlow31
                                                                                                                                                                      ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? (iSlow55 ? (iSlow57 ? (iSlow59 ? (iSlow60 ? 0.0158489998f
                                                                                                                                                                                                                                                              : (iSlow59 ? ((0.00193400006f * fSlow2) + 0.0158489998f) : 0.0177829992f))
                                                                                                                                                                                                                                                   : (iSlow57 ? (0.0177829992f - (0.00193400006f * fSlow76)) : 0.0158489998f))
                                                                                                                                                                                                                                        : (iSlow55 ? ((0.0239620004f * fSlow58) + 0.0158489998f) : 0.0398110002f))
                                                                                                                                                                                                                             : (iSlow41 ? (0.0398110002f - (0.0298110005f * fSlow56)) : 0.00999999978f))
                                                                                                                                                                                                                  : (iSlow38 ? ((0.344812989f * fSlow42) + 0.00999999978f) : 0.35481301f))
                                                                                                                                                                                                       : (iSlow36 ? (0.35481301f - (0.103624001f * fSlow39)) : 0.251188993f))
                                                                                                                                                                                            : (iSlow34 ? (0.251188993f - (0.171755999f * fSlow37)) : 0.0794330016f))
                                                                                                                                                                                 : (iSlow31 ? ((0.0205669999f * fSlow35) + 0.0794330016f) : 0.100000001f))
                                                                                                                                                                      : (iSlow29 ? (0.100000001f - (0.0601890013f * fSlow32)) : 0.0398110002f))
                                                                                                                                                            : (iSlow26 ? ((0.0232849997f * fSlow30) + 0.0398110002f)
                                                                                                                                                                       : 0.0630960017f))
                                                                                                                                                  : (iSlow24 ? ((0.0369039997f * fSlow27) + 0.0630960017f)
                                                                                                                                                             : 0.100000001f))
                                                                                                                                        : fSlow69)
                                                                                                                              : (iSlow20 ? ((0.0635839999f * fSlow23) + 0.0158489998f)
                                                                                                                                         : 0.0794330016f))
                                                                                                                    : (iSlow18 ? (0.0794330016f - (0.0478099994f * fSlow21))
                                                                                                                               : 0.0316229984f))
                                                                                                          : (iSlow17 ? ((0.0683770031f * fSlow19) + 0.0316229984f)
                                                                                                                     : 0.100000001f))
                                                                                                : (iSlow16 ? (0.100000001f - (0.0900000036f * fSlow50))
                                                                                                           : 0.00999999978f))
                                                                                      : (iSlow15 ? ((0.0401189998f * fSlow48) + 0.00999999978f)
                                                                                                 : 0.0501190014f))
                                                                            : fSlow93)
                                                                  : (iSlow12 ? (0.0794330016f - (0.0694330037f * fSlow47))
                                                                             : 0.00999999978f))
                                                        : (iSlow10 ? ((0.388107002f * fSlow13) + 0.00999999978f)
                                                                   : 0.398106992f))
                                              : (iSlow8 ? (0.398106992f - (0.198580995f * fSlow11))
                                                        : 0.199525997f))
                                    : (iSlow6 ? (0.199525997f - (0.099526003f * fSlow9))
                                              : 0.100000001f))
                          : (iSlow3 ? ((0.151188999f * fSlow7) + 0.100000001f) : 0.251188993f))
                : (iSlow4 ? (0.251188993f - (0.0516630001f * fSlow5)) : 0.199525997f));
        float fSlow95 = (iSlow65 ? (fSlow71 * fSlow94) : (fSlow67 * fSlow94));
        float fSlow96 = (fSlow78 * fSlow92);
        float fSlow97 = (0.0f - (fSlow95 / fSlow96));
        float fSlow98 = (iSlow3
                ? (iSlow6 ? (iSlow8 ? (iSlow10
                                              ? (iSlow12
                                                        ? (iSlow14
                                                                  ? (iSlow15
                                                                            ? (iSlow16
                                                                                      ? (iSlow17
                                                                                                ? (iSlow18
                                                                                                          ? (iSlow20
                                                                                                                    ? (iSlow22
                                                                                                                              ? (iSlow24
                                                                                                                                        ? (iSlow26
                                                                                                                                                  ? (iSlow29
                                                                                                                                                            ? (iSlow31 ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? (iSlow55 ? (iSlow57 ? (iSlow59 ? (iSlow60 ? 2800.0f : (iSlow59 ? (2800.0f - (100.0f * fSlow2)) : 2700.0f)) : 2700.0f)
                                                                                                                                                                                                                                         : (iSlow55 ? ((130.0f * fSlow58) + 2700.0f) : 2830.0f))
                                                                                                                                                                                                                              : (iSlow41 ? (2830.0f - (300.0f * fSlow56)) : 2530.0f))
                                                                                                                                                                                                                   : (iSlow38 ? (2530.0f - (280.0f * fSlow42)) : 2250.0f))
                                                                                                                                                                                                        : (iSlow36 ? ((150.0f * fSlow39) + 2250.0f) : 2400.0f))
                                                                                                                                                                                             : (iSlow34 ? ((200.0f * fSlow37) + 2400.0f) : 2600.0f))
                                                                                                                                                                                  : (iSlow31 ? (2600.0f - (200.0f * fSlow35)) : 2400.0f))
                                                                                                                                                                       : 2400.0f)
                                                                                                                                                            : (iSlow26 ? ((350.0f * fSlow30) + 2400.0f) : 2750.0f))
                                                                                                                                                  : (iSlow24 ? (2750.0f - fSlow28) : 2700.0f))
                                                                                                                                        : (iSlow22 ? ((200.0f * fSlow25) + 2700.0f) : 2900.0f))
                                                                                                                              : (iSlow20 ? (2900.0f - (200.0f * fSlow23)) : 2700.0f))
                                                                                                                    : (iSlow18 ? ((50.0f * fSlow21) + 2700.0f) : 2750.0f))
                                                                                                          : (iSlow17 ? ((150.0f * fSlow19) + 2750.0f)
                                                                                                                     : 2900.0f))
                                                                                                : (iSlow16 ? (2900.0f - (100.0f * fSlow50))
                                                                                                           : 2800.0f))
                                                                                      : (iSlow15 ? ((150.0f * fSlow48) + 2800.0f)
                                                                                                 : 2950.0f))
                                                                            : (iSlow14 ? (2950.0f - (120.0f * fSlow68))
                                                                                       : 2830.0f))
                                                                  : (iSlow12 ? (2830.0f - (130.0f * fSlow47))
                                                                             : 2700.0f))
                                                        : (iSlow10 ? (2700.0f - fSlow82) : 2650.0f))
                                              : (iSlow8 ? (2650.0f - (50.0f * fSlow11)) : 2600.0f))
                                    : (iSlow6 ? ((200.0f * fSlow9) + 2600.0f) : 2800.0f))
                          : (iSlow3 ? (2800.0f - (200.0f * fSlow7)) : 2600.0f))
                : (iSlow4 ? (fSlow75 + 2600.0f) : 2700.0f));
        float fSlow99 = std::tan((fConst0 * fSlow98));
        float fSlow100 = (1.0f / fSlow99);
        float fSlow101 =
            ((iSlow3 ? (iSlow6 ? (iSlow8 ? (iSlow10
                                                   ? (iSlow12
                                                             ? (iSlow14
                                                                       ? (iSlow15
                                                                                 ? (iSlow16
                                                                                           ? (iSlow17
                                                                                                     ? (iSlow18
                                                                                                               ? (iSlow20
                                                                                                                         ? (iSlow22 ? (iSlow24 ? (iSlow26 ? (iSlow29 ? (iSlow31 ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? (iSlow55 ? (iSlow57 ? 120.0f : (iSlow55 ? (120.0f - fSlow87) : 100.0f))
                                                                                                                                                                                                                                       : (iSlow41 ? ((70.0f * fSlow56) + 100.0f) : 170.0f))
                                                                                                                                                                                                                            : (iSlow38 ? (170.0f - fSlow86) : 110.0f))
                                                                                                                                                                                                                 : (iSlow36 ? (110.0f - fSlow54) : 100.0f))
                                                                                                                                                                                                      : 100.0f)
                                                                                                                                                                                           : 100.0f)
                                                                                                                                                                                : 100.0f)
                                                                                                                                                                     : (iSlow26 ? (fSlow53 + 100.0f) : 120.0f))
                                                                                                                                                          : (iSlow24 ? (120.0f - fSlow52) : 100.0f))
                                                                                                                                               : 100.0f)
                                                                                                                                    : 100.0f)
                                                                                                                         : 100.0f)
                                                                                                               : (iSlow17 ? (fSlow51 + 100.0f) : 120.0f))
                                                                                                     : 120.0f)
                                                                                           : (iSlow15 ? (120.0f - (20.0f * fSlow48))
                                                                                                      : 100.0f))
                                                                                 : 100.0f)
                                                                       : (iSlow12 ? ((70.0f * fSlow47) + 100.0f)
                                                                                  : 170.0f))
                                                             : (iSlow10 ? (170.0f - fSlow82) : 120.0f))
                                                   : (iSlow8 ? (120.0f - fSlow46) : 100.0f))
                                         : 100.0f)
                               : 100.0f)
                     : 100.0f) /
                fSlow98);
        float fSlow102 = (((fSlow100 - fSlow101) / fSlow99) + 1.0f);
        float fSlow103 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow99)));
        float fSlow104 = (((fSlow100 + fSlow101) / fSlow99) + 1.0f);
        float fSlow105 = (iSlow3
                ? (iSlow6 ? (iSlow8 ? (iSlow10
                                              ? (iSlow12
                                                        ? (iSlow14
                                                                  ? (iSlow15
                                                                            ? (iSlow16
                                                                                      ? (iSlow17
                                                                                                ? (iSlow18
                                                                                                          ? (iSlow20
                                                                                                                    ? (iSlow22
                                                                                                                              ? (iSlow24
                                                                                                                                        ? (iSlow26
                                                                                                                                                  ? (iSlow29
                                                                                                                                                            ? (iSlow31 ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? (iSlow55 ? (iSlow57 ? (iSlow59 ? (iSlow60 ? 0.100000001f : (iSlow59 ? (0.100000001f - (0.0683770031f * fSlow2)) : 0.0316229984f))
                                                                                                                                                                                                                                                    : 0.0316229984f)
                                                                                                                                                                                                                                         : (iSlow55 ? ((0.126865998f * fSlow58) + 0.0316229984f) : 0.158489004f))
                                                                                                                                                                                                                              : (iSlow41 ? (0.158489004f - (0.126865998f * fSlow56)) : 0.0316229984f))
                                                                                                                                                                                                                   : (iSlow38 ? ((0.323190004f * fSlow42) + 0.0316229984f) : 0.35481301f))
                                                                                                                                                                                                        : 0.35481301f)
                                                                                                                                                                                             : (iSlow34 ? (0.35481301f - (0.196324006f * fSlow37)) : 0.158489004f))
                                                                                                                                                                                  : (iSlow31 ? (0.158489004f - (0.0693639964f * fSlow35)) : 0.0891249999f))
                                                                                                                                                                       : (iSlow29 ? (0.0891249999f - (0.0640060008f * fSlow32)) : 0.0251189992f))
                                                                                                                                                            : (iSlow26 ? ((0.0456760004f * fSlow30) + 0.0251189992f)
                                                                                                                                                                       : 0.0707949996f))
                                                                                                                                                  : (iSlow24 ? ((0.0550980009f * fSlow27) + 0.0707949996f)
                                                                                                                                                             : 0.125892997f))
                                                                                                                                        : (iSlow22 ? (0.125892997f - (0.0627970025f * fSlow25))
                                                                                                                                                   : 0.0630960017f))
                                                                                                                              : (iSlow20 ? (0.0630960017f - (0.0129770003f * fSlow23))
                                                                                                                                         : 0.0501190014f))
                                                                                                                    : (iSlow18 ? ((0.020676f * fSlow21) + 0.0501190014f)
                                                                                                                               : 0.0707949996f))
                                                                                                          : (iSlow17 ? (0.0707949996f - (0.0456760004f * fSlow19))
                                                                                                                     : 0.0251189992f))
                                                                                                : (iSlow16 ? ((0.152709007f * fSlow50) + 0.0251189992f)
                                                                                                           : 0.177827999f))
                                                                                      : (iSlow15 ? (0.177827999f - (0.127709001f * fSlow48))
                                                                                                 : 0.0501190014f))
                                                                            : fSlow93)
                                                                  : (iSlow12 ? (0.0794330016f - (0.0616500005f * fSlow47))
                                                                             : 0.0177829992f))
                                                        : (iSlow10 ? ((0.428900987f * fSlow13) + 0.0177829992f)
                                                                   : 0.446684003f))
                                              : (iSlow8 ? (0.446684003f - (0.195494995f * fSlow11))
                                                        : 0.251188993f))
                                    : (iSlow6 ? (0.251188993f - (0.125295997f * fSlow9))
                                              : 0.125892997f))
                          : (iSlow3 ? ((0.125295997f * fSlow7) + 0.125892997f) : 0.251188993f))
                : (iSlow4 ? (0.251188993f - (0.109935001f * fSlow5)) : 0.141253993f));
        float fSlow106 = (iSlow65 ? (fSlow71 * fSlow105) : (fSlow67 * fSlow105));
        float fSlow107 = (fSlow99 * fSlow104);
        float fSlow108 = (0.0f - (fSlow106 / fSlow107));
        float fSlow109 = (iSlow3
                ? (iSlow6 ? (iSlow8 ? (iSlow10
                                              ? (iSlow12
                                                        ? (iSlow14
                                                                  ? (iSlow15
                                                                            ? (iSlow16
                                                                                      ? (iSlow17
                                                                                                ? (iSlow18
                                                                                                          ? (iSlow20
                                                                                                                    ? (iSlow22
                                                                                                                              ? (iSlow24
                                                                                                                                        ? (iSlow26
                                                                                                                                                  ? (iSlow29
                                                                                                                                                            ? (iSlow31
                                                                                                                                                                      ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? (iSlow55 ? (iSlow57 ? (iSlow59 ? (iSlow60 ? 1150.0f : (iSlow59 ? ((450.0f * fSlow2) + 1150.0f) : 1600.0f))
                                                                                                                                                                                                                                                   : (iSlow57 ? ((100.0f * fSlow76) + 1600.0f) : 1700.0f))
                                                                                                                                                                                                                                        : (iSlow55 ? (1700.0f - (900.0f * fSlow58)) : 800.0f))
                                                                                                                                                                                                                             : (iSlow41 ? (800.0f - (100.0f * fSlow56)) : 700.0f))
                                                                                                                                                                                                                  : (iSlow38 ? ((340.0f * fSlow42) + 700.0f) : 1040.0f))
                                                                                                                                                                                                       : (iSlow36 ? ((580.0f * fSlow39) + 1040.0f) : 1620.0f))
                                                                                                                                                                                            : (iSlow34 ? ((130.0f * fSlow37) + 1620.0f) : 1750.0f))
                                                                                                                                                                                 : (iSlow31 ? (1750.0f - (1000.0f * fSlow35)) : 750.0f))
                                                                                                                                                                      : (iSlow29 ? (750.0f - (150.0f * fSlow32)) : 600.0f))
                                                                                                                                                            : (iSlow26 ? ((520.0f * fSlow30) + 600.0f) : 1120.0f))
                                                                                                                                                  : (iSlow24 ? ((680.0f * fSlow27) + 1120.0f) : 1800.0f))
                                                                                                                                        : (iSlow22 ? ((50.0f * fSlow25) + 1800.0f) : 1850.0f))
                                                                                                                              : (iSlow20 ? (1850.0f - (1030.0f * fSlow23)) : 820.0f))
                                                                                                                    : (iSlow18 ? (820.0f - (190.0f * fSlow21)) : 630.0f))
                                                                                                          : (iSlow17 ? ((520.0f * fSlow19) + 630.0f) : 1150.0f))
                                                                                                : (iSlow16 ? ((850.0f * fSlow50) + 1150.0f)
                                                                                                           : 2000.0f))
                                                                                      : (iSlow15 ? ((140.0f * fSlow48) + 2000.0f)
                                                                                                 : 2140.0f))
                                                                            : (iSlow14 ? (2140.0f - (1340.0f * fSlow68))
                                                                                       : 800.0f))
                                                                  : (iSlow12 ? (800.0f - (100.0f * fSlow47))
                                                                             : 700.0f))
                                                        : (iSlow10 ? ((380.0f * fSlow13) + 700.0f)
                                                                   : 1080.0f))
                                              : (iSlow8 ? ((620.0f * fSlow11) + 1080.0f) : 1700.0f))
                                    : (iSlow6 ? ((170.0f * fSlow9) + 1700.0f) : 1870.0f))
                          : (iSlow3 ? (1870.0f - (1070.0f * fSlow7)) : 800.0f))
                : (iSlow4 ? (800.0f - (200.0f * fSlow5)) : 600.0f));
        float fSlow110 = ((2.0f * fSlow66) + 30.0f);
        float fSlow111 = ((fSlow1 != 2.0f)
                ? (((fSlow109 >= 1300.0f) & (fSlow66 >= 200.0f))
                          ? (fSlow109 -
                                ((0.000952380942f * (fSlow66 + -200.0f)) *
                                    (fSlow109 + -1300.0f)))
                          : ((fSlow109 <= fSlow110) ? fSlow110 : fSlow109))
                : fSlow109);
        float fSlow112 = std::tan((fConst0 * fSlow111));
        float fSlow113 = (1.0f / fSlow112);
        float fSlow114 = (10.0f * fSlow42);
        float fSlow115 = (20.0f * fSlow56);
        float fSlow116 =
            ((iSlow3 ? (iSlow6 ? (iSlow8 ? (iSlow10
                                                   ? (iSlow12
                                                             ? (iSlow14
                                                                       ? (iSlow15
                                                                                 ? (iSlow16
                                                                                           ? (iSlow17
                                                                                                     ? (iSlow18
                                                                                                               ? (iSlow20
                                                                                                                         ? (iSlow22
                                                                                                                                   ? (iSlow24
                                                                                                                                             ? (iSlow26
                                                                                                                                                       ? (iSlow29 ? (iSlow31 ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? (iSlow55 ? (iSlow57 ? (iSlow59 ? (iSlow60 ? 90.0f : (iSlow59 ? (90.0f - (10.0f * fSlow2)) : 80.0f))
                                                                                                                                                                                                                                                          : (iSlow57 ? ((20.0f * fSlow76) + 80.0f) : 100.0f))
                                                                                                                                                                                                                                               : (iSlow55 ? (100.0f - fSlow87) : 80.0f))
                                                                                                                                                                                                                                    : (iSlow41 ? (80.0f - fSlow115) : 60.0f))
                                                                                                                                                                                                                         : (iSlow38 ? (fSlow114 + 60.0f) : 70.0f))
                                                                                                                                                                                                              : (iSlow36 ? (fSlow54 + 70.0f) : 80.0f))
                                                                                                                                                                                                   : (iSlow34 ? ((10.0f * fSlow37) + 80.0f) : 90.0f))
                                                                                                                                                                                        : (iSlow31 ? (90.0f - (10.0f * fSlow35)) : 80.0f))
                                                                                                                                                                             : 80.0f)
                                                                                                                                                                  : (iSlow26 ? (fSlow85 + 80.0f) : 90.0f))
                                                                                                                                                       : (iSlow24 ? (90.0f - fSlow84) : 80.0f))
                                                                                                                                             : (iSlow22 ? ((10.0f * fSlow25) + 80.0f) : 90.0f))
                                                                                                                                   : (iSlow20 ? (90.0f - (10.0f * fSlow23)) : 80.0f))
                                                                                                                         : (iSlow18 ? (80.0f - (20.0f * fSlow21)) : 60.0f))
                                                                                                               : (iSlow17 ? ((30.0f * fSlow19) + 60.0f) : 90.0f))
                                                                                                     : (iSlow16 ? ((10.0f * fSlow50) + 90.0f) : 100.0f))
                                                                                           : (iSlow15 ? (100.0f - (10.0f * fSlow48))
                                                                                                      : 90.0f))
                                                                                 : (iSlow14 ? (90.0f - (10.0f * fSlow68))
                                                                                            : 80.0f))
                                                                       : (iSlow12 ? (80.0f - (20.0f * fSlow47))
                                                                                  : 60.0f))
                                                             : (iSlow10 ? ((30.0f * fSlow13) + 60.0f)
                                                                        : 90.0f))
                                                   : (iSlow8 ? (90.0f - fSlow81) : 80.0f))
                                         : (iSlow6 ? ((10.0f * fSlow9) + 80.0f) : 90.0f))
                               : (iSlow3 ? (90.0f - fSlow80) : 80.0f))
                     : (iSlow4 ? (80.0f - (20.0f * fSlow5)) : 60.0f)) /
                fSlow111);
        float fSlow117 = (((fSlow113 - fSlow116) / fSlow112) + 1.0f);
        float fSlow118 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow112)));
        float fSlow119 = (((fSlow113 + fSlow116) / fSlow112) + 1.0f);
        float fSlow120 = (iSlow3
                ? (iSlow6 ? (iSlow8 ? (iSlow10
                                              ? (iSlow12
                                                        ? (iSlow14
                                                                  ? (iSlow15
                                                                            ? (iSlow16
                                                                                      ? (iSlow17
                                                                                                ? (iSlow18
                                                                                                          ? (iSlow20
                                                                                                                    ? (iSlow22
                                                                                                                              ? (iSlow24
                                                                                                                                        ? (iSlow26
                                                                                                                                                  ? (iSlow29
                                                                                                                                                            ? (iSlow31
                                                                                                                                                                      ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? (iSlow55 ? (iSlow57 ? (iSlow59 ? (iSlow60 ? 0.630957007f : (iSlow59 ? (0.630957007f - (0.567861021f * fSlow2)) : 0.0630960017f))
                                                                                                                                                                                                                                                   : (iSlow57 ? ((0.0369039997f * fSlow76) + 0.0630960017f) : 0.100000001f))
                                                                                                                                                                                                                                        : (iSlow55 ? ((0.254812986f * fSlow58) + 0.100000001f) : 0.35481301f))
                                                                                                                                                                                                                             : (iSlow41 ? (0.35481301f - (0.103624001f * fSlow56)) : 0.251188993f))
                                                                                                                                                                                                                  : (iSlow38 ? ((0.195494995f * fSlow42) + 0.251188993f) : 0.446684003f))
                                                                                                                                                                                                       : (iSlow36 ? (0.446684003f - (0.195494995f * fSlow39)) : 0.251188993f))
                                                                                                                                                                                            : (iSlow34 ? (0.251188993f - (0.219566002f * fSlow37)) : 0.0316229984f))
                                                                                                                                                                                 : (iSlow31 ? ((0.250214994f * fSlow35) + 0.0316229984f) : 0.281838f))
                                                                                                                                                                      : (iSlow29 ? (0.281838f - (0.181838006f * fSlow32)) : 0.100000001f))
                                                                                                                                                            : (iSlow26 ? ((0.401187003f * fSlow30) + 0.100000001f) : 0.501187027f))
                                                                                                                                                  : (iSlow24 ? (0.501187027f - (0.301661015f * fSlow27))
                                                                                                                                                             : 0.199525997f))
                                                                                                                                        : (iSlow22 ? (0.199525997f - (0.136429995f * fSlow25))
                                                                                                                                                   : 0.0630960017f))
                                                                                                                              : (iSlow20 ? ((0.253131986f * fSlow23) + 0.0630960017f)
                                                                                                                                         : 0.316228002f))
                                                                                                                    : (iSlow18 ? (0.316228002f - (0.216227993f * fSlow21))
                                                                                                                               : 0.100000001f))
                                                                                                          : (iSlow17 ? ((0.401187003f * fSlow19) + 0.100000001f)
                                                                                                                     : 0.501187027f))
                                                                                                : (iSlow16 ? (0.501187027f - (0.401187003f * fSlow50))
                                                                                                           : 0.100000001f))
                                                                                      : (iSlow15 ? ((0.151188999f * fSlow48) + 0.100000001f)
                                                                                                 : 0.251188993f))
                                                                            : (iSlow14 ? ((0.0306490008f * fSlow68) + 0.251188993f)
                                                                                       : 0.281838f))
                                                                  : (iSlow12 ? (0.281838f - (0.123349003f * fSlow47))
                                                                             : 0.158489004f))
                                                        : (iSlow10 ? ((0.342698008f * fSlow13) + 0.158489004f)
                                                                   : 0.501187027f))
                                              : (iSlow8 ? (0.501187027f - (0.301661015f * fSlow11))
                                                        : 0.199525997f))
                                    : (iSlow6 ? (0.199525997f - (0.0216979999f * fSlow9))
                                              : 0.177827999f))
                          : (iSlow3 ? ((0.138400003f * fSlow7) + 0.177827999f) : 0.316228002f))
                : (iSlow4 ? (0.316228002f - (0.216227993f * fSlow5)) : 0.100000001f));
        float fSlow121 = (iSlow65 ? (fSlow71 * fSlow120) : (fSlow67 * fSlow120));
        float fSlow122 = (fSlow112 * fSlow119);
        float fSlow123 = (0.0f - (fSlow121 / fSlow122));
        float fSlow124 = (iSlow3
                ? (iSlow6 ? (iSlow8 ? (iSlow10
                                              ? (iSlow12
                                                        ? (iSlow14
                                                                  ? (iSlow15
                                                                            ? (iSlow16
                                                                                      ? (iSlow17
                                                                                                ? (iSlow18
                                                                                                          ? (iSlow20
                                                                                                                    ? (iSlow22
                                                                                                                              ? (iSlow24
                                                                                                                                        ? (iSlow26
                                                                                                                                                  ? (iSlow29
                                                                                                                                                            ? (iSlow31
                                                                                                                                                                      ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? (iSlow55 ? (iSlow57 ? (iSlow59 ? (iSlow60 ? 800.0f : (iSlow59 ? (800.0f - (400.0f * fSlow2)) : 400.0f))
                                                                                                                                                                                                                                                   : (iSlow57 ? (400.0f - (50.0f * fSlow76)) : 350.0f))
                                                                                                                                                                                                                                        : (iSlow55 ? ((100.0f * fSlow58) + 350.0f) : 450.0f))
                                                                                                                                                                                                                             : (iSlow41 ? (450.0f - (125.0f * fSlow56)) : 325.0f))
                                                                                                                                                                                                                  : (iSlow38 ? ((275.0f * fSlow42) + 325.0f) : 600.0f))
                                                                                                                                                                                                       : (iSlow36 ? (600.0f - (200.0f * fSlow39)) : 400.0f))
                                                                                                                                                                                            : (iSlow34 ? (400.0f - (150.0f * fSlow37)) : 250.0f))
                                                                                                                                                                                 : (iSlow31 ? ((150.0f * fSlow35) + 250.0f) : 400.0f))
                                                                                                                                                                      : (iSlow29 ? (400.0f - fSlow33) : 350.0f))
                                                                                                                                                            : (iSlow26 ? ((310.0f * fSlow30) + 350.0f) : 660.0f))
                                                                                                                                                  : (iSlow24 ? (660.0f - (220.0f * fSlow27)) : 440.0f))
                                                                                                                                        : (iSlow22 ? (440.0f - (170.0f * fSlow25)) : 270.0f))
                                                                                                                              : (iSlow20 ? ((160.0f * fSlow23) + 270.0f) : 430.0f))
                                                                                                                    : (iSlow18 ? (430.0f - (60.0f * fSlow21)) : 370.0f))
                                                                                                          : (iSlow17 ? ((430.0f * fSlow19) + 370.0f) : 800.0f))
                                                                                                : (iSlow16 ? (800.0f - (450.0f * fSlow50))
                                                                                                           : 350.0f))
                                                                                      : (iSlow15 ? (350.0f - fSlow49) : 270.0f))
                                                                            : (iSlow14 ? ((180.0f * fSlow68) + 270.0f)
                                                                                       : 450.0f))
                                                                  : (iSlow12 ? (450.0f - (125.0f * fSlow47))
                                                                             : 325.0f))
                                                        : (iSlow10 ? (325.0f * (fSlow13 + 1.0f))
                                                                   : 650.0f))
                                              : (iSlow8 ? (650.0f - (250.0f * fSlow11)) : 400.0f))
                                    : (iSlow6 ? (400.0f - (110.0f * fSlow9)) : 290.0f))
                          : (iSlow3 ? ((110.0f * fSlow7) + 290.0f) : 400.0f))
                : (iSlow4 ? (400.0f - (50.0f * fSlow5)) : 350.0f));
        float fSlow125 = ((fSlow124 <= fSlow66) ? fSlow66 : fSlow124);
        float fSlow126 = std::tan((fConst0 * fSlow125));
        float fSlow127 = (1.0f / fSlow126);
        float fSlow128 =
            ((iSlow3 ? (iSlow6 ? (iSlow8 ? (iSlow10
                                                   ? (iSlow12
                                                             ? (iSlow14
                                                                       ? (iSlow15
                                                                                 ? (iSlow16
                                                                                           ? (iSlow17
                                                                                                     ? (iSlow18
                                                                                                               ? (iSlow20
                                                                                                                         ? (iSlow22
                                                                                                                                   ? (iSlow24
                                                                                                                                             ? (iSlow26
                                                                                                                                                       ? (iSlow29 ? (iSlow31 ? (iSlow34 ? (iSlow36 ? (iSlow38 ? (iSlow41 ? (iSlow55 ? (iSlow57 ? (iSlow59 ? (iSlow60 ? 80.0f : (iSlow59 ? (80.0f - fSlow88) : 60.0f))
                                                                                                                                                                                                                                                          : (iSlow57 ? (60.0f - (10.0f * fSlow76)) : 50.0f))
                                                                                                                                                                                                                                               : (iSlow55 ? (fSlow87 + 50.0f) : 70.0f))
                                                                                                                                                                                                                                    : (iSlow41 ? (70.0f - fSlow115) : 50.0f))
                                                                                                                                                                                                                         : (iSlow38 ? (fSlow114 + 50.0f) : 60.0f))
                                                                                                                                                                                                              : (iSlow36 ? (60.0f - (20.0f * fSlow39)) : 40.0f))
                                                                                                                                                                                                   : (iSlow34 ? ((20.0f * fSlow37) + 40.0f) : 60.0f))
                                                                                                                                                                                        : (iSlow31 ? (60.0f - (20.0f * fSlow35)) : 40.0f))
                                                                                                                                                                             : 40.0f)
                                                                                                                                                                  : (iSlow26 ? (40.0f * (fSlow30 + 1.0f)) : 80.0f))
                                                                                                                                                       : (iSlow24 ? (80.0f - fSlow84) : 70.0f))
                                                                                                                                             : (iSlow22 ? (70.0f - (30.0f * fSlow25)) : 40.0f))
                                                                                                                                   : 40.0f)
                                                                                                                         : 40.0f)
                                                                                                               : (iSlow17 ? (40.0f * (fSlow19 + 1.0f)) : 80.0f))
                                                                                                     : (iSlow16 ? (80.0f - fSlow83) : 60.0f))
                                                                                           : 60.0f)
                                                                                 : (iSlow14 ? (60.0f - (20.0f * fSlow68))
                                                                                            : 40.0f))
                                                                       : (iSlow12 ? ((10.0f * fSlow47) + 40.0f)
                                                                                  : 50.0f))
                                                             : 50.0f)
                                                   : (iSlow8 ? (fSlow46 + 50.0f) : 70.0f))
                                         : (iSlow6 ? (70.0f - (30.0f * fSlow9)) : 40.0f))
                               : (iSlow3 ? ((30.0f * fSlow7) + 40.0f) : 70.0f))
                     : (iSlow4 ? (70.0f - (30.0f * fSlow5)) : 40.0f)) /
                fSlow125);
        float fSlow129 = (((fSlow127 - fSlow128) / fSlow126) + 1.0f);
        float fSlow130 = (1.0f - (1.0f / mydsp_faustpower2_f(fSlow126)));
        float fSlow131 = (((fSlow127 + fSlow128) / fSlow126) + 1.0f);
        float fSlow132 = (iSlow65 ? fSlow71 : fSlow67);
        float fSlow133 = (fSlow126 * fSlow131);
        float fSlow134 = (0.0f - (fSlow132 / fSlow133));
        for (int i = 0; (i < USER_AUDIO_IO_BUFFER_SIZE); i = (i + 1)) {
            float fTemp0 = float(input[i]);
            fRec0[0] = (fTemp0 -
                (((fRec0[2] * fSlow62) + (2.0f * (fRec0[1] * fSlow63))) / fSlow64));
            fRec1[0] = (fTemp0 -
                (((fRec1[2] * fSlow90) + (2.0f * (fRec1[1] * fSlow91))) / fSlow92));
            fRec2[0] = (fTemp0 -
                (((fRec2[2] * fSlow102) + (2.0f * (fRec2[1] * fSlow103))) / fSlow104));
            fRec3[0] = (fTemp0 -
                (((fRec3[2] * fSlow117) + (2.0f * (fRec3[1] * fSlow118))) / fSlow119));
            fRec4[0] = (fTemp0 -
                (((fRec4[2] * fSlow129) + (2.0f * (fRec4[1] * fSlow130))) / fSlow131));
            float out =
                float(((fRec0[2] * fSlow74) +
                    (((fRec1[2] * fSlow97) +
                         (((fRec2[2] * fSlow108) +
                              (((fRec3[2] * fSlow123) +
                                   ((((fRec4[0] * fSlow132) / fSlow133) + (fRec4[2] * fSlow134)) +
                                       ((fRec3[0] * fSlow121) / fSlow122))) +
                                  ((fRec2[0] * fSlow106) / fSlow107))) +
                             ((fRec1[0] * fSlow95) / fSlow96))) +
                        ((fRec0[0] * fSlow72) / fSlow73)))) *
                0.25f;

            output[i] = clipper.process(out);

            fRec0[2] = fRec0[1];
            fRec0[1] = fRec0[0];
            fRec1[2] = fRec1[1];
            fRec1[1] = fRec1[0];
            fRec2[2] = fRec2[1];
            fRec2[1] = fRec2[0];
            fRec3[2] = fRec3[1];
            fRec3[1] = fRec3[0];
            fRec4[2] = fRec4[1];
            fRec4[1] = fRec4[0];
        }
    }

    void updateParams(const DspParam* param1, const DspParam* param2) {
        // freq = (float)param1->last_result / 4095.0f;
        voice_type = (float)param1->last_result / 4095.0f * 5.0f;
        vowel = (float)param2->last_result / 4095.0f * 5.0f;

        /*
        // Here we will use a single control to crossfade between
        // rows and columns in a 5x5 table. Rows are used for different voice
        // types, columns are for for vowels.
        float ctrl = (float)param2->last_result * 25.0f / 4095.0f;
        uint8_t ctrl_int = (uint8_t)ctrl;
        ctrl = ctrl - ctrl_int;

        if (ctrl > 0.0f) {
            // We only want to do this when there's fractional part for param.
            // This also sorts out handling last element.
            uint8_t next_ctrl = formants_map[ctrl_int + 1];
            ctrl_int = formants_map[ctrl_int];
            voice_type = ctrl_int >> 3;
            vowel = ctrl_int & vowel_mask;
            if (vowel == (next_ctrl & vowel_mask)) {
                // Morph voice type
                voice_type += ctrl;
            }
            else {
                // Morph vowel
                vowel += ctrl;
            }
        }
        else {
            ctrl_int = formants_map[ctrl_int];
            voice_type = ctrl_int >> voice_type_offset;
            vowel = ctrl_int & vowel_mask;
        }
        */
    }

private:
    float freq, voice_type, vowel;
    float fRec0[3];
    float fRec1[3];
    float fRec2[3];
    float fRec3[3];
    float fRec4[3];
    static constexpr float fSampleRate = 48000.0f;
    static constexpr float fConst0 = (6.28318548f /
        std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate))));
};

}

#endif