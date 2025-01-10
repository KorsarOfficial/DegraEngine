#pragma once
#include <cstdint>

struct Color {
    uint8_t r, g, b, a;

    Color(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255)
        : r(r), g(g), b(b), a(a) {}
}; 