#include "pos.hpp"

Positionable::Positionable() : mTransform(), mBillboarding(false) {}

Positionable::~Positionable() {}

Vector3 Positionable::WorldPos() const {
    Vector3 v = mTransform.mPos;
    const Object* cur = mParent;
    while (cur != nullptr) {
        if (cur->CanBecome<Positionable>()) {
            v += dynamic_cast<const Positionable*>(cur)->mTransform.mPos;
        }
        cur = cur->mParent;
    }
    return v;
}

Vector3 Positionable::WorldRot() const {
    Vector3 v = mTransform.mRot;
    const Object* cur = mParent;
    while (cur != nullptr) {
        if (cur->CanBecome<Positionable>()) {
            v += dynamic_cast<const Positionable*>(cur)->mTransform.mRot;
        }
        cur = cur->mParent;
    }
    return v;
}

Vector3 Positionable::WorldScl() const {
    Vector3 v = mTransform.mScale;
    const Object* cur = mParent;
    while (cur != nullptr) {
        if (cur->CanBecome<Positionable>()) {
            v *= dynamic_cast<const Positionable*>(cur)->mTransform.mScale;
        }
        cur = cur->mParent;
    }
    return v;
}

