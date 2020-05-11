#include "engine/effects.h"

namespace fieldkitfx {

FiltersLibrary filters_library;
EffectsLibraryNoMemory effects_library1;
EffectsLibrarySmallMemory effects_library2;
EffectsLibraryLargeMemory effects_library3;

/*
void EffectsLibraryBase::nextEffect() {
    algo = (EffectAlgo)((((uint8_t)algo) + 1) % num_effects);
    refreshUi = true;
    auto effect = getCurrentEffect();
    effect->reset();
}
*/

void EffectsLibraryBase::updateParams() {
    param1.process(ADC_getMixedCV2());
    param2.process(ADC_getMixedCV1());
    effects[static_cast<uint8_t>(algo)]->updateParams(&param1, &param2);
}

void EffectsLibraryBase::process(const float* in, float* out) {
    effects[static_cast<uint8_t>(algo)]->process(in, out);
}

void EffectsLibraryBase::setAlgo(const uint8_t algo_id) {
    algo = (EffectAlgo)algo_id;
    refreshUi = true;
    auto effect = getCurrentEffect();
    effect->reset();
}

}