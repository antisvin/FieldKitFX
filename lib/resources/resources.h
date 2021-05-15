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
// Resources definitions.
//
// Automatically generated with:
// make -f warps/makefile resources


#ifndef LIB_RESOURCES_RESOURCES_H_
#define LIB_RESOURCES_RESOURCES_H_


#include "stmlib/stmlib.h"



namespace fieldkitfx {

typedef uint8_t ResourceId;

extern const float* filter_bank_table[];

extern const float fb__87_4800[];
extern const float fb_110_4800[];
extern const float fb_139_4800[];
extern const float fb_175_4800[];
extern const float fb_220_4800[];
extern const float fb_277_4800[];
extern const float fb_349_4800[];
extern const float fb_440_4800[];
extern const float fb_554_4800[];
extern const float fb_698_4800[];
extern const float fb_880_48000[];
#define FB__87_4800 0
#define FB__87_4800_SIZE 7
#define FB_110_4800 1
#define FB_110_4800_SIZE 7
#define FB_139_4800 2
#define FB_139_4800_SIZE 7
#define FB_175_4800 3
#define FB_175_4800_SIZE 7
#define FB_220_4800 4
#define FB_220_4800_SIZE 7
#define FB_277_4800 5
#define FB_277_4800_SIZE 7
#define FB_349_4800 6
#define FB_349_4800_SIZE 7
#define FB_440_4800 7
#define FB_440_4800_SIZE 7
#define FB_554_4800 8
#define FB_554_4800_SIZE 7
#define FB_698_4800 9
#define FB_698_4800_SIZE 7
#define FB_880_48000 10
#define FB_880_48000_SIZE 7

}  // namespace fieldkitfx

#endif  // LIB_RESOURCES_RESOURCES_H_
