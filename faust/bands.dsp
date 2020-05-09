import("stdfaust.lib");

base_freq = hslider("Frequency", 0.5, 0.0, 1.0, 0.001);
band_width = hslider("Band width", 0.5, 0.001, 1, 0.001);
resonance = 5.0;
drive = 0.0;
offset = 0.0;
//resonance = hslider("Resonance", 0.5, 0.001, 1, 0.001) * 10;
//drive = hslider("Drive", 0, 0, 1, 0.001);
//offset = hslider("Offset", 0, 0, 1, 0.001);

/*

band_pass_reject(in) = ba.if(
    band_width > 0.495 & band_width < 0.505,
    in,
    ba.if(
        band_width < 0.495,
        bandpass(band_width / 0.495),
        bandreject((band_width - 0.505) / 0.495)))
with {
    vcf(freq) = ve.sallenKey2ndOrder(freq ,resonance);
    lpf = ba.selectn(3, 0);
    bpf = ba.selectn(3, 1);
    hpf = ba.selectn(3, 2);    

    clamp = min(1.0, max(0.0));
    bandpass(bw) = in : vcf(clamp(base_freq + bw)) : lpf : vcf(clamp(base_freq - bw)) : hpf;
    //bandreject(bw) = in;
    bandreject(bw) = in <: (vcf(clamp(base_freq - bw)) : lpf), (vcf(clamp(base_freq + bw)) : hpf) :> _;
};
*/

band_pass_reject(in) = ba.if(
    band_width < 0.5,
    bandpass(band_width / 0.5),
    bandreject((band_width - 0.5) / 0.5))
with {
    vcf(freq) = ve.sallenKey2ndOrder(clamp(freq) ,resonance);
    lpf = ba.selectn(3, 0);
    bpf = ba.selectn(3, 1);
    hpf = ba.selectn(3, 2);    
    clamp = min(1.0, max(0.0));
    bandpass(bw) = in : vcf(base_freq * (1 - bw) + bw) : lpf : vcf(base_freq - base_freq * bw) : hpf;
    bandreject(bw) = in <: (vcf(base_freq - base_freq * bw) : lpf), (vcf(base_freq * (1 - bw) + bw) : hpf) :> _;
};

process = band_pass_reject : ef.cubicnl(drive, offset); //: fi.dcblocker;