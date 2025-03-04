#pragma once

#include <bit>
#include <cmath>
#include <queue>
#include <cstdint>
#include <unordered_map>

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_surface.h>

#include "iface/istream.hpp"
#include "math/color.hpp"

class Texture {
public:

	enum PixFmt { // >0x10 means non-uniform/unorthodox storage
		Idx4	= 0x00,
		Idx8	= 0x01,
		Rgb5	= 0x02,
		Rgb8	= 0x03,
		Rgba4	= 0x04,
		Rgba8	= 0x05,

		Rgb332	= 0x10,
		Rgb565	= 0x11,
		Rgb5a1	= 0x12,
		Rgb5a3	= 0x13,

		Idx8_Defl	= 0x20 | Idx8,
		Rgb8_Defl	= 0x20 | Rgb8,
		Rgba8_Defl	= 0x20 | Rgba8,
	};

	struct PixelFmtData {
		PixFmt type;
		uint32_t bpp;
		GLenum gl_format;
		GLenum gl_format_2;
		GLenum gl_type;
	};

	Texture();
	~Texture();

	void LoadFromSurface(SDL_Surface* src_img);
	void Activate();
	void Save(IStream&) const;
	void Load(IStream&);

	uint16_t w;
	uint16_t h;
	uint8_t pixel_fmt;
	uint texture_id;
	Color* palette;
	void* img;

	constexpr bool IsPaletted() const { return pixel_fmt == Idx4 || pixel_fmt == Idx8; }
	bool IsPow2() const { return std::has_single_bit(w) && std::has_single_bit(h); }

	static std::queue<uint> sTextureIds;
	static const std::unordered_map<PixFmt, PixelFmtData> sPixFmt2DataStruct;
	
	static void Init();
};
