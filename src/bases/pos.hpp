#pragma once

#include "bases/obj.hpp"
#include "math/vec.hpp"

class Positionable : public Object {
public:
    Positionable();
    virtual ~Positionable();
    
    Vector3 mPos;
    Vector3 mRot;
    Vector3 mScale;
    bool mBillboarding; // forces rotation to match camera angle

    Vector3 WorldPos() const;
    Vector3 WorldRot() const;
    Vector3 WorldScl() const;
};
