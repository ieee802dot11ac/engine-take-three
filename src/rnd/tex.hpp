#pragma once

#include "math/color.hpp"
#include <bit>
#include <cstdint>

class Texture {
public:
    uint16_t w;
    uint16_t h;
    uint8_t bpp;
    Color* palette;
    void* img;

    uint32_t texture_id;

    bool IsPaletted() const { return bpp <= 8; }
    bool IsPow2() const { return std::has_single_bit(w) && std::has_single_bit(h); }
};
