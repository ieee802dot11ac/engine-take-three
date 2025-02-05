#include "tex.hpp"
#include <SDL2/SDL_opengl.h>
#include <cstring>

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
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
