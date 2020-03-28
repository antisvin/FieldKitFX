#include "engine/effects.h"

namespace fieldkitfx {

EffectsLibrary effects_library;

EffectsLibrary::EffectsLibrary() {
    current_effect = effects[1];
}

void EffectsLibrary::nextEffect() {
    if (current_effect == effects[num_effects - 1]) {
        current_effect = *effects;
    }
    else {
        current_effect++;
    }
}

void EffectsLibrary::updateParams() {
    current_effect->updateParams(
        static_cast<uint16_t>(param1.process(ADC_getMixedCV1()) & 0xfff),
        static_cast<uint16_t>(param2.process(ADC_getMixedCV2()) & 0xfff));
}

void EffectsLibrary::render(const float* in, float* out) {
    current_effect->render(in, out);
}

}