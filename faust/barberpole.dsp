import("IIRHilbert.lib");
import("stdfaust.lib");

mix = 0.5;
maxfeedback = 0.7;

num_ap = 16;

rate = hslider("Rate [unit:hz] [OWL:PARAMETER_A]", 0, 0., 1, 0.001);
rateScalar = hslider("Rate Scalar [OWL:PARAMETER_B]", 1., -20., 20., 0.001);
//ap_delay = hslider("AP delay", 0.001, 0.001, 16., 0.01);
ap_coef = hslider("AP coefficient[OWL:PARAMETER_C]", -0.5, -1.0, 1., 0.01);
fbk = hslider("Feedback [OWL:PARAMETER_D]", 0.0, -1.0, 1., 0.01) : *(maxfeedback) : si.smooth(ba.tau2pole(0.005));


ssbfreqshift(x) = (+ : negative) ~ (*(fbk) : clip(-1, 1))
with {
  negative(x) = real(x)*cosv - imag(x)*sinv;
  positive(x) = real(x)*cosv + imag(x)*sinv;
  real(x) = hilbert(x) : _ , !;
  imag(x) = hilbert(x) : ! , _;
  phasor = fmod(((rate*rateScalar)/float(ma.SR) : (+ : ma.decimal) ~ _), 1.) * (2*ma.PI);
  sinv = sin(phasor);
  cosv = cos(phasor);
  hilbert = hilbertef;
  clip(lo,hi) = min(hi) : max(lo);
};

process = _ <: _, ((apf <: shifter)) :> _ : *(0.5)
with {
    //(seq(N, num_ap, apf) <: shifter)
    apf = seq(N, 32, fi.allpass_fcomb(2, 0, ap_coef)) :> _;
    shifter = ssbfreqshift;
};