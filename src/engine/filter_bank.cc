// Copyright 2014 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Filter bank.

#include "engine/filter_bank.h"

#include <algorithm>

#include "resources.h"

namespace fieldkitfx {

using namespace std;
using namespace stmlib;

void FilterBank::Init(float sample_rate) {
    float* samples = &samples_[0];

    int32_t group = -1;
    int32_t decimation_factor = -1;
    for (int32_t i = 0; i < kNumBands; ++i) {
        const float* coefficients = filter_bank_table[i];

        Band& b = band_[i];

        b.samples = samples;
        samples += kMaxFilterBankBlockSize;
        b.post_gain = coefficients[2];
        b.post_gain2 = coefficients[2];

        for (int32_t pass = 0; pass < 2; ++pass) {
            b.svf[pass].Init();
            b.svf[pass].set_f_fq(
                coefficients[pass * 2 + 3], coefficients[pass * 2 + 4]);
        }
    }
}

void FilterBank::Analyze(const float* input, size_t size) {
    for (int32_t i = 0; i < kNumBands; ++i) {
        Band& b = band_[i];
        for (int32_t pass = 0; pass < 2; ++pass) {
            const float* source = pass == 0 ? input : b.samples;
            float* destination = b.samples;
            if (i == 0) {
                b.svf[pass].Process<FILTER_MODE_LOW_PASS>(source, destination, size);
            }
            else if (i == kNumBands - 1) {
                b.svf[pass].Process<FILTER_MODE_HIGH_PASS>(source, destination, size);
            }
            else {
                b.svf[pass].Process<FILTER_MODE_BAND_PASS_NORMALIZED>(
                    source, destination, size);
            }
        }
        // Apply post-gain
        const float gain = b.post_gain2;
        float* output = b.samples;
        for (size_t i = 0; i < size; ++i) {
            output[i] *= gain;
        }
    }
}

} // namespace fieldkitfx
