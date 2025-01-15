#include "pos.hpp"

Positionable::Positionable() : mPos(0,0,0), mRot(0,0,0), mScale(1,1,1), mBillboarding(false) {}

Positionable::~Positionable() {}

Vector3 Positionable::WorldPos() const {
    Vector3 v = mPos;
    const Object* cur = (mParent.lock()).get();
    while (cur != nullptr) {
        if (cur->CanBecome<Positionable>()) {
            v += dynamic_cast<const Positionable*>(cur)->mPos;
        }
        cur = (cur->mParent.lock()).get();
    }
    return v;
}

Vector3 Positionable::WorldRot() const {
    Vector3 v = mRot;
    const Object* cur = (mParent.lock()).get();
    while (cur != nullptr) {
        if (cur->CanBecome<Positionable>()) {
            v += dynamic_cast<const Positionable*>(cur)->mRot;
        }
        cur = (cur->mParent.lock()).get();
    }
    return v;
}

Vector3 Positionable::WorldScl() const {
    Vector3 v = mScale;
    const Object* cur = (mParent.lock()).get();
    while (cur != nullptr) {
        if (cur->CanBecome<Positionable>()) {
            v *= dynamic_cast<const Positionable*>(cur)->mScale;
        }
        cur = (cur->mParent.lock()).get();
    }
    return v;
}

