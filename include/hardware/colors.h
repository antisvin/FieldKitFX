#ifndef _HARDWARE_COLORS_H_
#define _HARDWARE_COLORS_H_

#include <cstdint>

#define NBR_COLORS 5

namespace fieldkitfx {

enum Color { COL_NONE = 0, COL_BLUE, COL_PINK, COL_GREEN, COL_ORANGE, COL_RED };

// We have an extra color (red), but it's only used effect selection
const uint8_t colors[NBR_COLORS + 1][3] = {
    { 0x00, 0x00, 0x00 }, // No color
    { 0x00, 0x66, 0x89 }, // Seapunk blue
    { 0xe0, 0x00, 0x93 }, // Chrisi pink
    { 0x00, 0x81, 0x08 }, // Deep Dan
    { 0xff, 0x10, 0x00 }, // OG orange
    { 0xff, 0x00, 0x00 }, // Let's call it Bloody Mary red to keep up with tradition
};

}
#endif /* _HARDWARE_COLORS_H_ */
