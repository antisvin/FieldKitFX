#ifndef COLORS_H_
#define COLORS_H_

#include <cstdint>

#define NBR_COLORS 5

namespace fieldkitfx {
const uint8_t colors[NBR_COLORS][3] = {
    { 0x00, 0x00, 0x00 }, // No color
    { 0x00, 0x66, 0x89 }, // Seapunk blue
    { 0xe0, 0x00, 0x93 }, // Chrisi pink
    { 0x00, 0x81, 0x08 }, // Deep Dan
    { 0xff, 0x10, 0x00 }, // OG orange
};
}
#endif /* COLORS_H_ */
