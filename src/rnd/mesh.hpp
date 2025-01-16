#pragma once

#include "bases/draw.hpp"
#include "bases/obj.hpp"
#include "bases/pos.hpp"
#include "math/vec.hpp"
#include "rnd/tex.hpp"
#include <cmath>
#include <memory>
#include <optional>

struct alignas(32) Vtx { // this order seems weird, but it makes InterleavedArrays easier
    float_t u, v; // 0x8
    float_t nx, ny, nz; // 0x14
    float_t x, y, z; // 0x20

    Vector3* Pos() { return (Vector3*)&x; }
    Vector3* Norm() { return (Vector3*)&nx; }
    Vector2* Uv() { return (Vector2*)&u; }
};

struct Face {
    uint16_t idx[3];

    operator const uint16_t*() const {
        return idx;
    }
    uint16_t operator[](uint i) const {
        assert(i < 3);
        return idx[i];
    }
};

class Mesh : public Positionable, public Drawable {
public:
    Mesh();
    virtual ~Mesh();
    CLASS_NAME_CRUFT(Mesh)
    virtual void Draw();

    void InitDisplayList();

    std::vector<Vtx> mVerts;
    std::vector<Face> mFaces;
    std::shared_ptr<Texture> mTexture;
    std::optional<uint> mDisplayListId; // nonvalue designates a dynamic mesh, i.e. has animations applied

    static uint display_list_num;
};
