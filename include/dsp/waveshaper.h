#ifndef _DSP_WAVESHAPER_H_
#define _DSP_WAVESHAPER_H_

#include "stmlib/dsp/dsp.h"
#include "dsp/dsp.h"
#include "utils/utils.h"
#include "utils/wavetables.h"
#include "utils/waveshapes.h"

namespace fieldkitfx {

using namespace stmlib;

static constexpr uint16_t arcsin_lut_size = 256;
static constexpr uint16_t wavetable_size = 256;
static constexpr uint16_t wavetable_size_full = 260;
static constexpr uint16_t wavetable_half { wavetable_size / 2 };

class WaveshaperEffect : public DspEffect {

public:
    void process(const float* in, float* out) override {
        for (int i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            float input = (in[i] * 0.5 + 0.5) * arcsin_lut_size;

            MAKE_INTEGRAL_FRACTIONAL(input);
            float lookup_index =
                interpolate_wave(arcsin_lut, input_integral, input_fractional) / 32767;
            lookup_index = wrap(lookup_index) * wavetable_half + wavetable_half;

            MAKE_INTEGRAL_FRACTIONAL(lookup_index);

            float wave_a = interpolate_wave_hermite(
                table_a, lookup_index_integral, lookup_index_fractional);
            float wave_b = interpolate_wave_hermite(
                table_b, lookup_index_integral, lookup_index_fractional);

            // float wave_a = InterpolateWave(
            //    table_a, lookup_index_integral, lookup_index_fractional);
            out[i] = // wave_a / 32767;
                Crossfade(wave_a, wave_b, mix) / 32767;
        }
    }

    void updateParams(const DspParam* param1, const DspParam* param2) {
        gain = param1->asFloat() / ADC_RESOLUTION_DEZ * 4 + 1;
        mix = param2->asFloat() / ADC_RESOLUTION_DEZ * 16;
        MAKE_INTEGRAL_FRACTIONAL(mix);
        table_a = &wavetables_lut[mix_integral * wavetable_size_full];
        table_b = &wavetables_lut[(mix_integral + 1) * wavetable_size_full];
        mix = mix_fractional;
    }

private:
    float gain, mix;
    const int16_t* table_a;
    const int16_t* table_b;

    inline float wrap(float val) {
        float result = std::fmod(gain * val, 2.0);
        if (result > 1.0) {
            result = 2.0 - result;
        }
        else if (result < -1.0) {
            result = -2.0 - result;
        }
        return result;
    }
};
}

#endif