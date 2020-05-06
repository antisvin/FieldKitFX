import("stdfaust.lib");

freq = hslider("Frequency", 0.5, 0.0001, 0.9999, 0.0001);
//resonance = hslider("Resonance", 0.7, 0.01, 32, 0.01);
resonance = 10;
morph = hslider("Morph", 0, 0, 1, 0.0001);

process(in) = ba.if(
    morph < 0.5,
    crossfade(morph * 2, bpf, lpf),
    crossfade(2 - morph * 2, bpf, hpf))
with {
    crossfade(x, a, b) = x * a + (1 - x) * b;
    vcf = in : ve.oberheim(freq, resonance);
    bsf = vcf : ba.selectn(4, 0);
    bpf = vcf : ba.selectn(4, 1);
    hpf = vcf : ba.selectn(4, 2);
    lpf = vcf : ba.selectn(4, 3);
};