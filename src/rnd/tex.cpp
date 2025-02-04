#include "tex.hpp"
#include <SDL2/SDL_opengl.h>

std::queue<uint> Texture::sTextureIds;

void Texture::Init() {
    constexpr uint tex_ct = 256;

    uint texes[tex_ct];
    glGenTextures(tex_ct, texes);

    for (uint i = 0; i < tex_ct; i++) {
        sTextureIds.push(texes[i]);
    }
}

Texture::Texture() : w(0), h(0), bpp(32), palette(NULL), img(NULL) {
    texture_id = sTextureIds.front(); sTextureIds.pop();
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

Texture::~Texture() {
    delete palette;
    delete (uint8_t*)img;
}

void Texture::Activate() {
    glBindTexture(GL_TEXTURE_2D, texture_id);
}
