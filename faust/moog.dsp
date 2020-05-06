import("stdfaust.lib");

freq = hslider("Frequency", 60, 20, 10000, 0.01);
resonance = hslider("Resonance", 0.7, 0, 32, 0.01);

//process = ve.moog_vcf(resonance, freq); // Ok-ish
//process = ve.moog_vcf_2b(resonance, freq); // Good
//process = ve.moog_vcf_2bn(resonance, freq); // Weird
process = ve.moogLadder(freq / 10001.0, resonance);