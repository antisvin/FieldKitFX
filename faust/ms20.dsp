import("stdfaust.lib");

freq = hslider("Frequency", 0.5, 0.0, 1.0, 0.01);
resonance = hslider("Resonance", 0.5, 0, 1, 0.01);


process = ve.korg35LPF(freq, resonance * 10);