import("stdfaust.lib");

base_freq = hslider("Frequency", 0.5, 0.0, 1.0, 0.001);
band_width = hslider("Band width", 0.5, 0.001, 1, 0.001);
resonance = 2.0;
drive = 0.0;
offset = 0.0;
//resonance = hslider("Resonance", 0.5, 0.2, 1, 0.001) * 10;

process = _ : *(0.5) <: (
    ve.sallenKey2ndOrderBPF(base_freq, resonance),
    ve.sallenKey2ndOrderBPF(peak_lo, peak_res),
    ve.sallenKey2ndOrderBPF(peak_hi, peak_res)) :> _ : ef.cubicnl(drive, offset)
with {
    peak_res = resonance - 1 + band_width * 2;
    peak_lo = base_freq - base_freq * band_width;
    peak_hi = base_freq * (1 - band_width) + band_width;
};