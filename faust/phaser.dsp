import("IIRHilbert.lib");
import("stdfaust.lib");

/*

num_ap = 32;

mix = hslider("Mix", 0.5, 0.0, 1.0, 0.01);
ap_coef = hslider("Allpass coef", 0.0, -1.0, 1.0, 0.01);


//process = _ <: ((ap_chain : *(mix)), *(1.0 - mix)) :> _
process = _ <: (ap_chain : *(mix)), *(1.0 - mix) :> _
with {
    ap_chain = seq(N, num_ap, _ : fi.allpassn1m(1, ap_coef));
    //ap_chain = fi.allpassnt(1, ap_coef) :> _;
};

*/


mix = 0.5;
maxfeedback = 0.7;

num_ap = 16;

rate = hslider("Rate [unit:hz] [OWL:PARAMETER_A]", 0, 0., 1, 0.001);
depth = hslider("Depth", 0.0, 0.0, 0.5, 0.01);
rateScalar = hslider("Rate Scalar [OWL:PARAMETER_B]", 1., -20., 20., 0.001);
//ap_delay = hslider("AP delay", 0.001, 0.001, 16., 0.01);
ap_coef = hslider("AP coefficient[OWL:PARAMETER_C]", 0., -0.5, 0.5, 0.01);
fbk = hslider("Feedback [OWL:PARAMETER_D]", 0.0, -1.0, 1., 0.01) : *(maxfeedback) : si.smooth(ba.tau2pole(0.005));

process = _ <: (_, apf) :> _ : *(0.5)
with {
    //(seq(N, num_ap, apf) <: shifter)
    apf = (+ : (seq(N, 32, fi.allpass_fcomb(2, 0, (ap_coef + lfo * depth) : clip(-1.0, 1.0)))) :> _)  ~ (*(fbk) : clip(-1, 1));
    lfo = os.osc(rate);
    clip(lo,hi) = min(hi) : max(lo);
    shifter = ssbfreqshift;
};