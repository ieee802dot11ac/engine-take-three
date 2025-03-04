#pragma once
#include "iface/istream.hpp"
#include <cstdint>

struct Color {
    uint8_t r, g, b, a;
};

inline IStream& operator<<(IStream& lhs, const Color& rhs) {
	uint32_t tmp;
	tmp = *(uint32_t*)&rhs.r;
	lhs << tmp;
	return lhs;
}

inline IStream& operator>>(IStream& lhs, Color& rhs) {
	uint32_t tmp;
	lhs >> tmp;
	*(uint32_t*)&rhs.r = tmp;
	return lhs;
}
