#include "engine/effects.h"

namespace fieldkitfx {

EffectsLibrary effects_library;

EffectsLibrary::EffectsLibrary() {
    algo = DSP_FREQUENCY_SHIFTER;
}

void EffectsLibrary::nextEffect() {
    algo = (EffectAlgo)((((uint8_t)algo) + 1) % num_effects);
    refreshUi = true;
}

void EffectsLibrary::updateParams() {
    effects[static_cast<uint8_t>(algo)]->updateParams(
        static_cast<uint16_t>(param1.process(ADC_getMixedCV1()) & 0xfff),
        static_cast<uint16_t>(param2.process(ADC_getMixedCV2()) & 0xfff));
}

void EffectsLibrary::process(const float* in, float* out) {
    effects[static_cast<uint8_t>(algo)]->process(in, out);
}

}