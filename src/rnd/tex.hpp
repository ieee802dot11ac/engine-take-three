#pragma once

#include "iface/istream.hpp"
#include "math/color.hpp"
#include <bit>
#include <queue>
#include <cstdint>
#include <cmath>

class Texture {
public:

	enum PixFmt { // >0x10 means nonlinear/unorthodox storage
		Idx4	= 0x00,
		Idx8	= 0x01,
		Rgb5	= 0x02,
		Rgb8	= 0x03,
		Rgba4	= 0x04,
		Rgba8	= 0x05,

		Rgb332	= 0x10,
		Rgb5a1	= 0x11,
		Rgb4a3	= 0x12,
		Rgb5a3	= 0x13,

		Idx8_Defl	= 0x20 | Idx8,
		Rgb8_Defl	= 0x20 | Rgb8,
		Rgba8_Defl	= 0x20 | Rgba8,
	};

	Texture();
	~Texture();

	void Load(IStream& stream);
	void Save(IStream& stream);
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
