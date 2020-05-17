#ifndef _ENGINE_EFFECTS_H_
#define _ENGINE_EFFECTS_H_

//#include "dsp/allpass_filter.h"
#include "dsp/barberpole_phaser.h"
#include "dsp/bypass.h"
#include "dsp/chorus.h"
#include "dsp/comb_filter.h"
#include "dsp/decimator.h"
#include "dsp/dual_pitchshifter.h"
#include "dsp/distfolder.h"
#include "dsp/formants.h"
#include "dsp/frequency_shifter.h"
#include "dsp/ms20.h"
#include "dsp/oberheim.h"
#include "dsp/overdrive.h"
#include "dsp/phaser.h"
#include "dsp/triple_peaks.h"
#include "dsp/tz_flanger.h"
#include "dsp/waveshaper.h"
#include "utils/moving_average_filter.h"
#include "utils/utils.h"

namespace fieldkitfx {

enum EffectAlgo {
    DSP_BYPASS = 0,
    DSP_BARBERPOLE_PHASER,
    DSP_TZ_FLANGER,
    DSP_PHASER,
    DSP_CHORUS,
    DSP_DECIMATOR,
};

template <size_t shared_buffer_size>
class SharedBuffer {
public:
    float shared_buffer[shared_buffer_size];

    template <typename Effect>
    void initBuffer(Effect** effects) {
        for (size_t i = 0; i < sizeof(effects); i++) {
            effects[i + 1]->init(shared_buffer);
        }
    }
};

class EffectsLibraryBase {
public:
    static constexpr uint8_t num_effects = 5;
    DspEffect* effects[num_effects];
    uint8_t algo_id;
    bool refreshUi = true;
    DspParam param1, param2;

    void setAlgo(uint8_t algo_id);
    void updateParams();
    void process(const float* in, float* out);
    DspEffect* getCurrentEffect() {
        return effects[algo_id];
    }
};

class FiltersLibrary : public EffectsLibraryBase {
public:
    static constexpr uint8_t num_effects = 5;
    FiltersLibrary() {
        effects[0] = &bypass;
        effects[1] = &ms20;
        effects[2] = &oberheim_svf;
        effects[3] = &triple_peaks;
        effects[4] = &formant;
    }

private:
    BypassEffect bypass;
    Ms20Filter ms20;
    OberheimSvfFilter oberheim_svf;
    TriplePeaksFilter triple_peaks;
    FormantFilter formant;

    DISALLOW_COPY_AND_ASSIGN(FiltersLibrary);
};

class EffectsLibraryNoMemory : public EffectsLibraryBase {
public:
    EffectsLibraryNoMemory() {
        effects[0] = &bypass;
        effects[1] = &overdrive;
        effects[2] = &waveshaper;
        effects[3] = &bypass;
        effects[4] = &bypass;
    }

private:
    BypassEffect bypass;
    OverdriveEffect overdrive;
    WaveshaperEffect waveshaper;
    DISALLOW_COPY_AND_ASSIGN(EffectsLibraryNoMemory);
};

class EffectsLibrarySmallMemory : public EffectsLibraryBase, public SharedBuffer<16> {
public:
    EffectsLibrarySmallMemory() {
        effects[0] = &bypass;
        effects[1] = &frequency_shifter;
        effects[2] = &phaser;
        effects[3] = &distfolder;
        effects[4] = &decimator;
        SharedBuffer<16>::initBuffer<DspEffect>(effects);
    }

private:
    BypassEffect bypass;
    FrequencyShifterEffect frequency_shifter;
    PhaserEffect phaser;
    DistFolderEffect distfolder;
    DecimatorEffect decimator;

    DISALLOW_COPY_AND_ASSIGN(EffectsLibrarySmallMemory);
};

class EffectsLibraryLargeMemory : public EffectsLibraryBase,
                                  public SharedBuffer<2048> {
public:
    EffectsLibraryLargeMemory() {
        effects[0] = &bypass;
        effects[1] = &barberpole_phaser;
        effects[2] = &tz_flanger;
        effects[3] = &comb;
        effects[4] = &dual_pitchshifter;
        //        effects[4] = &chorus;
        SharedBuffer<2048>::initBuffer<DspEffect>(effects);
    };

private:
    BypassEffect bypass;
    BarberpolePhaserEffect barberpole_phaser;
    TzFlangerEffect tz_flanger;
    CombFilterEffect comb;
    DualPitchshifterEffect dual_pitchshifter;
    //    ChorusEffect chorus;

    DISALLOW_COPY_AND_ASSIGN(EffectsLibraryLargeMemory);
};

extern FiltersLibrary filters_library;
extern EffectsLibraryNoMemory effects_library1;
extern EffectsLibrarySmallMemory effects_library2;
extern EffectsLibraryLargeMemory effects_library3;

}

#endif