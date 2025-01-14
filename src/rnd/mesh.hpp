#pragma once

#include "bases/draw.hpp"
#include "bases/obj.hpp"
#include "bases/pos.hpp"
#include "math/vec.hpp"
#include <cmath>
#include <memory>

struct alignas(32) Vtx {
    float_t x, y, z; // 0xC
    float_t nx, ny, nz; // 0x18
    float_t u, v; // 0x20

    Vector3* Pos() { return (Vector3*)&x; }
    Vector3* Norm() { return (Vector3*)&nx; }
    Vector2* Uv() { return (Vector2*)&u; }
};

typedef float Face[3]; // i do not care

class Mesh : public Positionable, public Drawable {
    Mesh();
    virtual ~Mesh();
    CLASS_NAME_CRUFT(Mesh)
    virtual void Draw();

    std::vector<Vtx> mVerts;
    std::vector<Face> mFaces;
    std::shared_ptr<struct Texture> mTexture;
};
