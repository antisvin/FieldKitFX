import("stdfaust.lib");


process = ef.cubicnl(drive, offset)
with {
    drive = hslider("Drive", 1, 1, 2, 0.001);
    offset = hslider("Offset", 0, -0.5, 0.5, 0.001);
}