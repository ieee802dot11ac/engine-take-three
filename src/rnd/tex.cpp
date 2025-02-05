#include "tex.hpp"
#include <SDL2/SDL_opengl.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <unordered_map>

std::queue<uint> Texture::sTextureIds;
const std::unordered_map<Texture::PixFmt, Texture::PixelFmtData> Texture::sPixFmt2DataStruct = {
    {Idx4, {Idx4, 4, GL_COLOR_INDEX, GL_COLOR_INDEX, GL_UNSIGNED_BYTE}}, // has to be manually promoted, not supported on PC
    {Idx8, {Idx8, 8, GL_COLOR_INDEX, GL_COLOR_INDEX, GL_UNSIGNED_BYTE}},
    {Rgb332, {Rgb332, 8, GL_R3_G3_B2, GL_RGB, GL_UNSIGNED_BYTE_3_3_2}},
    {Rgb5, {Rgb5, 16, GL_RGB5, GL_RGB, GL_UNSIGNED_SHORT}}, // TODO upconvert to 565
    {Rgb565, {Rgb565, 16, GL_RGB565, GL_RGB, GL_UNSIGNED_SHORT}},
    {Rgba4, {Rgba4, 16, GL_RGBA4, GL_RGBA, GL_UNSIGNED_SHORT}},
    {Rgb5a1, {Rgb5a1, 16, GL_RGB5_A1, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV}},
    {Rgb8, {Rgb8, 32, GL_RGB, GL_RGB, GL_UNSIGNED_INT}},
    {Rgba8, {Rgba8, 32, GL_RGBA, GL_RGBA, GL_UNSIGNED_INT}}
};

void Texture::Init() {
    constexpr uint tex_ct = 256;

    uint texes[tex_ct];
    glGenTextures(tex_ct, texes);

    for (uint i = 0; i < tex_ct; i++) {
        sTextureIds.push(texes[i]);
    }
}

Texture::Texture() : w(0), h(0), pixel_fmt(Rgba8), palette(NULL), img(NULL) {
    texture_id = sTextureIds.front(); sTextureIds.pop();
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void Texture::LoadFromSurface(SDL_Surface* src_img) {
    const std::unordered_map<uint32_t, PixFmt> sdl_fmt_to_pixfmt = {
        {SDL_PIXELFORMAT_INDEX4LSB, Idx4},
        {SDL_PIXELFORMAT_INDEX8, Idx8},
        {SDL_PIXELFORMAT_RGB555, Rgb5},
        {SDL_PIXELFORMAT_RGB888, Rgb8},
        {SDL_PIXELFORMAT_RGBA4444, Rgba4},
        {SDL_PIXELFORMAT_RGBA8888, Rgba8},

        {SDL_PIXELFORMAT_RGB565, Rgb565},
        {SDL_PIXELFORMAT_ARGB1555, Rgb5a1}
        // deflate is only encoded in our textures, and 5a3 isn't supported by SDL2
    };
    assert(palette == nullptr);
    assert(img == nullptr);
    pixel_fmt = sdl_fmt_to_pixfmt.at(src_img->format->format);
    if (IsPaletted()) {
        if (src_img->format->palette->ncolors > 256) {
            fprintf(stderr, "tried to palettize an image with %d colors!\n", src_img->format->palette->ncolors);
            return;
        } else if (src_img->format->palette->ncolors > 16 && src_img->format->palette->ncolors <= 256) {
            assert(pixel_fmt == Idx8);
            palette = new Color[256];
        } else if (src_img->format->palette->ncolors <= 16) {
            assert(pixel_fmt == Idx4);
            palette = new Color[16];
        }
        memcpy(palette, src_img->format->palette->colors, src_img->format->palette->ncolors * sizeof(SDL_Color));
    }
    assert(IsPow2());
    uint32_t size = src_img->format->BytesPerPixel * w * h;
    img = new uint8_t[size];
    memcpy(img, src_img->pixels, size);

    const PixelFmtData& format = sPixFmt2DataStruct.at((PixFmt)pixel_fmt);
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        format.gl_format, 
        w, 
        h, 
        0, 
        format.gl_format_2, 
        format.gl_type, 
        img
    );
}

Texture::~Texture() {
    delete palette;
    delete (uint8_t*)img;
}

/* format:
width 64
height 64
filename ./res/test.png
*/

void Texture::LoadTextFile(std::istream stream) {
    while (!stream.eof()) {
        char linebuf[64] = {0};
        stream.getline(linebuf, 63);
        uint num_buf = 0; char key_buf[64] = {0}; char filename_buf[256] = {0};
        if (sscanf(linebuf, "%s %u\n", key_buf, &num_buf) == 2) {
            if (strcasecmp(key_buf, "width") == 0) w = num_buf;
            else if (strcasecmp(key_buf, "height") == 0) h = num_buf;
        } else if (sscanf(linebuf, "%s %s\n", key_buf, filename_buf) == 2) {
            if (strcasecmp(key_buf, "filename") == 0) {
                // TODO track down external file, load compressed data
            }
        }
    }
}

void Texture::Activate() {
    glBindTexture(GL_TEXTURE_2D, texture_id);
}
