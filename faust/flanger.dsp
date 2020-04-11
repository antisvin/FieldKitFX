import("stdfaust.lib");

max_delay = 32;
min_delay = 0.01;
max_feedback = 0.7;

rate = hslider("Rate", 0.01, 0.01, 3.0, 0.01);
depth = hslider("Depth", 0.0, 0.0, 1.0, 0.01);
fbk = hslider("Feedback", 0., -1., 1., 0.01) : *(max_feedback) : si.smooth(ba.tau2pole(0.005));


process = _ <: delay_dry, flange :> _ : *(0.5)
with {
    lfo = os.osc(rate) * depth / 2;
    clip(lo,hi) = min(hi) : max(lo);
    delay_dry = de.fdelay(max_delay, max_delay / 2);
    flange = (+ : de.fdelay(max_delay, max_delay / 2 * (1 + lfo)))  ~ (*(fbk) : clip(-1, 1));
};
