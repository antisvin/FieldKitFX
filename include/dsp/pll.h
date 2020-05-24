#ifndef _DSP_PLL_H_
#define _DSP_PLL_H_

#include "dsp/dsp.h"
#include "engine/follower.h"
#include "engine/ramp_extractor.h"
#include "utils/waveshapes.h"
#include "stmlib/utils/gate_flags.h"

namespace fieldkitfx {

const Ratio frequency_ratios[] = {
    { 0.25f, 4 },
    { 0.3333333333333333333f, 3 },
    { 0.5f, 2 },
    { 0.6f, 5 },
    { 0.6666666666666666667f, 3 },
    { 0.75f, 4 },
    { 0.8f, 5 },
    { 1.0f, 1 },
    { 1.25f, 4 },
    { 1.3333333333333333333f, 3 },
    { 1.5f, 2 },
    { 1.6666666666666666667f, 3 },
    { 2.0f, 1 },
    { 3.0f, 1 },
    { 4.0f, 1 },
};

static constexpr int16_t num_frequency_ratios { sizeof(frequency_ratios) /
    sizeof(Ratio) };

class PllEffect : public DspEffect {

    void init() {
        ramp_extractor.Init(48000, 0.0008333333333333334);
    }
    void reset() {
        ramp_extractor.Reset();
        follower.reset();
    }

    void process(const float* in, float* out) {
        // Extract gate flags
        stmlib::GateFlags gate_flags[USER_AUDIO_IO_BUFFER_SIZE];
        for (size_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            prev_gate = ExtractGateFlags(prev_gate, in[i] > 0.02f);
            gate_flags[i] = prev_gate;
        }

        // Output is used as temp buffer since input is const
        std::copy(in, in + USER_AUDIO_IO_BUFFER_SIZE, out);

        // Ramp extractor would replace buffer with phases - that's all we need from it
        ramp_extractor.Process(false, true, current_ratio, gate_flags, out,
            USER_AUDIO_IO_BUFFER_SIZE);

        for (size_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            float lookup_index = out[i] * wavetable_lut_size;
            MAKE_INTEGRAL_FRACTIONAL(lookup_index);

            float wave_a = interpolate_wave_hermite(
                table_a, lookup_index_integral, lookup_index_fractional);
            float wave_b = interpolate_wave_hermite(
                table_b, lookup_index_integral, lookup_index_fractional);
            out[i] =
                Crossfade(wave_a, wave_b, shape) / 32767 * follower.process(*in++);
        }
    }

    void updateParams(const DspParam* param1, const DspParam* param2) {
        float ratio =
            param1->asFloat() / ADC_RESOLUTION_DEZ * (num_frequency_ratios - 1);
        current_ratio = frequency_ratios[(uint8_t)ratio];

        shape = param2->asFloat() / ADC_RESOLUTION_DEZ * 16;
        MAKE_INTEGRAL_FRACTIONAL(shape);
        shape = shape_fractional;
        table_a = &wavetables_lut[shape_integral * wavetable_size_full];
        table_b = &wavetables_lut[(shape_integral + 1) * wavetable_size_full];
    }

private:
    stmlib::GateFlags prev_gate;
    RampExtractor ramp_extractor;
    Ratio current_ratio;

    EnvelopeFollower follower;

    float shape;
    const int16_t* table_a;
    const int16_t* table_b;
};
}

#endif