#include "tex.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <unordered_map>

#define TEX_REV 1

std::queue<uint> Texture::sTextureIds;
const std::unordered_map<Texture::PixFmt, Texture::PixelFmtData>
	Texture::sPixFmt2DataStruct = {
		{Idx4,
		 {Idx4, 4, GL_COLOR_INDEX, GL_COLOR_INDEX,
		  GL_UNSIGNED_BYTE}}, // has to be manually promoted, not supported on
							  // PC
		{Idx8, {Idx8, 8, GL_COLOR_INDEX, GL_COLOR_INDEX, GL_UNSIGNED_BYTE}},
		{Rgb332, {Rgb332, 8, GL_R3_G3_B2, GL_RGB, GL_UNSIGNED_BYTE_3_3_2}},
		{Rgb5,
		 {Rgb5, 16, GL_RGB5, GL_RGB,
		  GL_UNSIGNED_SHORT}}, // TODO upconvert to 565
		{Rgb565, {Rgb565, 16, GL_RGB565, GL_RGB, GL_UNSIGNED_SHORT_5_6_5}},
		{Rgba4, {Rgba4, 16, GL_RGBA4, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4}},
		{Rgb5a1,
		 {Rgb5a1, 16, GL_RGB5_A1, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV}},
		{Rgb8, {Rgb8, 32, GL_RGB, GL_RGB, GL_UNSIGNED_INT}},
		{Rgba8, {Rgba8, 32, GL_RGBA, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8}}};

void Texture::Init() {
	constexpr uint tex_ct = 256;

	uint texes[tex_ct];
	glGenTextures(tex_ct, texes);

	for (uint i = 0; i < tex_ct; i++) {
		sTextureIds.push(texes[i]);
	}
}

Texture::Texture() : w(0), h(0), pixel_fmt(Rgba8), palette(NULL), img(NULL) {
	texture_id = sTextureIds.front();
	sTextureIds.pop();
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

Texture::~Texture() {
	delete palette;
	delete (uint8_t *)img;
}

void Texture::LoadFromSurface(SDL_Surface *src_img) {
	const std::unordered_map<uint32_t, PixFmt> sdl_fmt_to_pixfmt = {
		{SDL_PIXELFORMAT_INDEX4LSB, Idx4},
		{SDL_PIXELFORMAT_INDEX8, Idx8},
		{SDL_PIXELFORMAT_RGB555, Rgb5},
		{SDL_PIXELFORMAT_RGB888, Rgb8},
		{SDL_PIXELFORMAT_RGBA4444, Rgba4},
		{SDL_PIXELFORMAT_RGBA8888, Rgba8},
		{SDL_PIXELFORMAT_RGBA32, Rgba8},

		{SDL_PIXELFORMAT_RGB565, Rgb565},
		{SDL_PIXELFORMAT_ARGB1555, Rgb5a1}
		// deflate is only encoded in our textures, and 5a3 isn't supported by
		// SDL2
	};
	SDL_assert(palette == nullptr);
	SDL_assert(img == nullptr);
	w = src_img->w;
	h = src_img->h;
	pixel_fmt = sdl_fmt_to_pixfmt.at(src_img->format->format);
	if (IsPaletted() && src_img->format->palette != nullptr) {
		if (src_img->format->palette->ncolors > 256) {
			fprintf(stderr, "tried to palettize an image with %d colors!\n",
					src_img->format->palette->ncolors);
			return;
		} else if (src_img->format->palette->ncolors > 16 &&
				   src_img->format->palette->ncolors <= 256) {
			SDL_assert(pixel_fmt == Idx8);
			palette = new Color[256];
		} else if (src_img->format->palette->ncolors <= 16) {
			SDL_assert(pixel_fmt == Idx4);
			palette = new Color[16];
		}
		memcpy(palette, src_img->format->palette->colors,
			   src_img->format->palette->ncolors * sizeof(SDL_Color));
	}
	SDL_assert(IsPow2());
	uint32_t size = src_img->format->BytesPerPixel * w * h;
	img = new uint8_t[size];
	memcpy(img, src_img->pixels, size);

	// certified OGL moment
	uint8_t *temp_row = new uint8_t[src_img->pitch];
	uint8_t *ptr = (uint8_t *)img;
	for (int i = h - 1, j = 0; i > (h / 2) - 1; i--, j++) {
		memcpy(temp_row, &ptr[i * src_img->pitch], src_img->pitch);
		memcpy(&ptr[i * src_img->pitch], &ptr[j * src_img->pitch],
			   src_img->pitch);
		memcpy(&ptr[j * src_img->pitch], temp_row, src_img->pitch);
	}
	delete[] temp_row;

	const PixelFmtData &format = sPixFmt2DataStruct.at((PixFmt)pixel_fmt);
	glTexImage2D(GL_TEXTURE_2D, 0, format.gl_format, w, h, 0,
				 format.gl_format_2, format.gl_type, img);
}

void Texture::Activate() { glBindTexture(GL_TEXTURE_2D, texture_id); }

void Texture::Load(IStream &strm) {
	int rev;
	strm >> rev;
	SDL_assert(rev <= TEX_REV);
	strm >> w >> h;
	strm >> pixel_fmt;
	if (IsPaletted()) {
		palette = new Color[(pixel_fmt == Idx4 ? 16 : 256)];
		for (int i = 0; i < (pixel_fmt == Idx4 ? 16 : 256); i++) {
			strm >> palette[i];
		}
	}
	int row_length = (w * (sPixFmt2DataStruct.at((PixFmt)pixel_fmt).bpp / 8));
	img = new uint8_t[row_length * h];
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < row_length; x++) {
			uint8_t tmp;
			strm >> tmp;
			((uint8_t *)img)[x + y * row_length] = tmp;
		}
	}
}

void Texture::Save(IStream &strm) const {
	strm << TEX_REV;
	strm << w << h;
	strm << pixel_fmt;
	if (IsPaletted()) {
		for (int i = 0; i < (pixel_fmt == Idx4 ? 16 : 256); i++) {
			strm << palette[i];
		}
	}
	int row_length = (w * (sPixFmt2DataStruct.at((PixFmt)pixel_fmt).bpp / 8));
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < row_length; x++) {
			strm << ((uint8_t *)img)[x + y * row_length];
		}
	}
}
