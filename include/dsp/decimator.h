#ifndef _DESTRUCTO_H_
#define _DESTRUCTO_H

#include "dsp/dsp.h"
#include "engine/engine.h"
#include "utils/log_lut.h"

namespace fieldkitfx {

constexpr uint8_t min_bitdepth = 1;
constexpr uint8_t max_decimation_factor = 8;
constexpr int16_t bitcrusher_lower_limit = -10;
constexpr int16_t bitcrusher_upper_limit = 10;
static const uint8_t decimation_factors[7] = { 1, 2, 4, 8, 16, 32, 64 };

class DecimatorEffect : public DspEffect {
public:
    void render(const float* in, float* out) override {
        float bit_reduction =
            (logLUT_12bit[last_param1] * (16.0 - min_bitdepth)) / 4095.0;
        //        uint8_t bit_reduction_int = static_cast<uint8_t>(bit_reduction);
        uint8_t bit_reduction_int = bit_reduction;

        float decimation_factor = ((last_param2 * 6) / 4095.0);
        //        uint8_t decimation_factor_int = static_cast<uint8_t>(decimation_factor);
        uint8_t decimation_factor_int = decimation_factor;

        float sr_tmp1 {};
        float sr_tmp2 {};

        for (uint8_t i = 0; i < USER_AUDIO_IO_BUFFER_SIZE; i++) {
            int16_t bc_tmp1 = (int16_t)(*(in++) * 32767);
            int16_t bc_tmp2 = bc_tmp1;
            // shift
            bc_tmp1 >>= bit_reduction_int;
            bc_tmp1 <<= bit_reduction_int;

            // shift
            bc_tmp2 >>= (bit_reduction_int + 1);
            bc_tmp2 <<= (bit_reduction_int + 1);

            // de-offset
            *out = crossfade(((float)bc_tmp1) / 32767, ((float)bc_tmp2) / 32767,
                bit_reduction_int + 1 - bit_reduction);

            // TODO: use bit masks here?
            if ((i % decimation_factors[decimation_factor_int]) == 0) {
                sr_tmp1 = *out;
            }
            if ((i % (decimation_factors[decimation_factor_int + 1])) == 0) {
                sr_tmp2 = *out;
            }

            *(out++) = (sr_tmp1 +
                (sr_tmp2 - sr_tmp1) * (decimation_factor - decimation_factor_int));
        }
    }
};

} // namespace fieldkitfx
#endif