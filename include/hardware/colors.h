#ifndef _HARDWARE_COLORS_H_
#define _HARDWARE_COLORS_H_

#include <cstdint>

#define NBR_COLORS 5

namespace fieldkitfx {

enum Color {
    COL_NONE = 0,
    COL_CYAN,
    COL_PINK,
    COL_LIME,
    COL_ORANGE,
    COL_BLUE,
    COL_MAGENTA,
    COL_GREEN,
    COL_RED,
    COL_WHITE,
};

// We have an extra color (red), but it's only used effect selection
// White is used for blinking
const uint8_t colors[NBR_COLORS + 6][3] = {
    { 0x00, 0x00, 0x00 }, // No color
    { 0x00, 0x66, 0x89 }, // Seapunk blue
    { 0xe0, 0x00, 0x93 }, // Chrisi pink
    { 0x00, 0x81, 0x08 }, // Deep Dan
    { 0xff, 0x10, 0x00 }, // OG orange
    { 0x00, 0x33, 0x44 }, // Darker blue
    { 0x70, 0x00, 0x49 }, // Magenta
    { 0x00, 0x41, 0x04 }, // Green
    { 0x78, 0x00, 0x00 }, // Let's call it Bloody Mary red to keep up with tradition
    { 0xff, 0xff, 0xff },
};

}
#endif /* _HARDWARE_COLORS_H_ */
