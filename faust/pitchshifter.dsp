import("stdfaust.lib");

process = _ <: _ , ef.transpose(window, xfade, shift1), ef.transpose(window, xfade, shift2) :> _ : *(0.33) 
with {
    window = hslider("window (samples)", 1000, 50, 10000, 1);
    xfade = hslider("xfade (samples)", 10, 1, 10000, 1);
    shift1 = hslider("shift1 (semitones) ", 0, -12, +12, 0.01);
    shift2 = hslider("shift2 (semitones) ", 0, -12, +12, 0.01);

};
