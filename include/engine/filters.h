#ifndef _ENGINE_FILTERS_H_
#define _ENGINE_FILTERS_H_

#include "dsp/dsp.h"
#include "engine/clipping.h"

namespace fieldkitfx {

class BaseFilterEffect : public DspEffect {
protected:
    inline static HardClipper clipper;
};

}

#endif