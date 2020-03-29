#include "engine/effects.h"

namespace fieldkitfx {

EffectsLibrary effects_library;

void EffectsLibrary::nextEffect() {
    algo = (EffectAlgo)((((uint8_t)algo) + 1) % num_effects);
    refreshUi = true;
    auto effect = getCurrentEffect();
    effect->reset();
}

void EffectsLibrary::updateParams() {
    param1.process(ADC_getMixedCV2()) & 0xfff;
    param2.process(ADC_getMixedCV1()) & 0xfff;
    effects[static_cast<uint8_t>(algo)]->updateParams(&param1, &param2);
}

void EffectsLibrary::process(const float* in, float* out) {
    effects[static_cast<uint8_t>(algo)]->process(in, out);
}

}