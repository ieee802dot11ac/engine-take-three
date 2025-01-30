#pragma once

#include "math/color.hpp"
#include <bit>
#include <queue>
#include <cstdint>
#include <cmath>

class Texture {
public:

    Texture();
    ~Texture();
    // TODO figure out loading; will have the TexImage2D step

    void Activate();

    uint16_t w;
    uint16_t h;
    uint8_t bpp;
    Color* palette;
    void* img;
    uint texture_id;

    bool IsPaletted() const { return bpp <= 8; }
    bool IsPow2() const { return std::has_single_bit(w) && std::has_single_bit(h); }

    static std::queue<uint> sTextureIds;
    
    static void Init();
};
