import("stdfaust.lib");

formants_mapping = (
    0,  1,  2,  3,  4,
    9,  8,  7,  6,  5,
    10, 11, 12, 13, 14,
    19, 18, 17, 16, 15,
    20, 21, 22, 23, 24);


process = pm.formantFilterbankBP(voiceType,vowel,freq) 
//: *(0.15) : ef.cubicnl(0, 0) : *(1.45)// 
with {
    voiceType = hslider("Voice Type", 0, 0, 4, 0.0001);
    vowel = hslider("Vowel", 0, 0, 4, 0.0001);
    freq = hslider("Freq", 120, 50, 1000, 0.0001);
};
