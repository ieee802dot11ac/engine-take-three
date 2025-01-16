#pragma once

#include "math/vec.hpp"

struct Transform {
public:
    Transform() : mPos(), mRot(), mScale(1, 1, 1) {}
    Vector3 mPos;
    Vector3 mRot;
    Vector3 mScale;
};
