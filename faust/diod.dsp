import("stdfaust.lib");

freq = hslider("Frequency", 60, 20, 10000, 0.01);
resonance = hslider("Resonance", 0.7, 0, 32, 0.01);

process = ve.diodeLadder(freq / 10001.0, resonance);