#pragma once

#include <cstdint>

class Texture {
public:
    uint16_t w;
    uint16_t h;
    uint8_t bpp;
    void* data;
};
