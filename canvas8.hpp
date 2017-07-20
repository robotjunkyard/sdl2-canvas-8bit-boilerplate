#pragma once
#include <cstdint>
#include <set>

const int CANVAS_WIDTH = 512,
          CANVAS_HEIGHT = 288;
const int canvasToWindowScale = 2;

typedef uint8_t byte;
typedef std::pair<int, int> Point;

// confirmed this generates a nice 2-CPU-instruction-long thing (under Release build)
inline byte reduceToMask(byte val)
{
    return (val == 0) ? 0 : 0xFF;
}

inline byte reduceToInverseMask(byte val)
{
    return (val == 0) ? 0xFF : 0;
}
