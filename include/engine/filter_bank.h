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

#ifndef FILTER_BANK_H_
#define FILTER_BANK_H_

#include "stmlib/stmlib.h"
#include "stmlib/dsp/dsp.h"
#include "stmlib/dsp/filter.h"
#include "resources.h"

namespace fieldkitfx {

const int32_t kNumBands = 11;
const int32_t kMaxFilterBankBlockSize = 64;
const int32_t kSampleMemorySize = kMaxFilterBankBlockSize * kNumBands;

struct Band {
    float post_gain;
    float post_gain2;
    stmlib::CrossoverSvf svf[2];
    float* samples;
};

class FilterBank {
public:
    FilterBank() {
    }
    ~FilterBank() {
    }
    void Init(float sample_rate);
    void Analyze(const float* in, size_t size);
    const Band& band(int32_t index) {
        return band_[index];
    }
    void SetGain(int32_t index, float gain) {
        band_[index].post_gain2 = band_[index].post_gain * gain;
    }

private:
    float samples_[kSampleMemorySize];

    Band band_[kNumBands + 1];

    DISALLOW_COPY_AND_ASSIGN(FilterBank);
};

} // namespace fieldkitfx

#endif // FILTER_BANK_H_
