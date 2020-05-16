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
    effects[algo_id]->updateParams(&param1, &param2);
}

void EffectsLibraryBase::process(const float* in, float* out) {
    effects[algo_id]->process(in, out);
}

void EffectsLibraryBase::setAlgo(const uint8_t new_algo_id) {
    algo_id = new_algo_id;
    refreshUi = true;
    auto effect = getCurrentEffect();
    effect->reset();
}

}