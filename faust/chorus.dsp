import("stdfaust.lib");

max_delay = 2048;

pre_delay1 = hslider("Pre delay1", 256.0, 256.0, 1024.0, 0.01);
pre_delay2 = hslider("Pre delay2", 256.0, 256.0, 1024.0, 0.01);
depth = hslider("Depth", 0.0, 0.0, 1.0, 0.01);
rate = hslider("Rate", 0.01, 0.01, 1.0, 0.01);


process = _ <: (_, d1, d2) :> _ : adjust_gain
with {
    adjust_gain(in) = in / (1 + gain1 + gain2);
    lfo1 = os.osc(rate);
    lfo2 = os.triangle(rate * depth / 2);
    depth1 = min(depth * 2, 1.0) * 128;    
    depth2 = 256;
    //gain1 = ba.if(depth <= 0.5, 1.0, 1.25 - depth / 2.0);
    //gain2 = 1.0 - gain1;
    gain1 = 1.0;
    gain2 = ba.if(depth <= 0.5, 0.0, depth * 2.0 - 1.0);
    d1 = de.fdelay(max_delay, pre_delay1 + lfo1 * depth1) * gain1;
    d2 = de.fdelay(max_delay, pre_delay2 + lfo2 * depth2) * gain2;
};
/*

depth = hslider("Depth", 5.0, 5.0, 512.0, 0.01);
rate = hslider("Rate", 0.01, 0.01, 1.0, 0.01);

process = _ <: (_, de.fdelay(max_delay, pre_delay + os.osc(rate) * depth)) :> _ : *(0.5);

*/
