import("stdfaust.lib");

base_freq = hslider("Frequency", 0.5, 0.0, 1.0, 0.001);
band_width = hslider("Band width", 0.5, 0.001, 1, 0.001);
//resonance = 5.0;
drive = 0.0;
offset = 0.0;
resonance = hslider("Resonance", 0.5, 0.001, 1, 0.001) * 10;

process = _ <: (
    ve.sallenKey2ndOrderBPF(peak_lo, resonance),
    ve.sallenKey2ndOrderBPF(peak_hi, resonance)) :> _ : ef.cubicnl(drive, offset)
with {
    peak_lo = base_freq - base_freq * band_width;
    peak_hi = base_freq * (1 - band_width) + band_width;
};