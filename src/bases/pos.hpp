#pragma once

#include "bases/obj.hpp"
#include "math/vec.hpp"
#include "math/xfm.hpp"

class Positionable : public virtual Object {
  public:
	Positionable();
	virtual ~Positionable();
	CLASS_NAME_CRUFT(Positionable)

	Transform mTransform;
	bool mBillboarding; // forces rotation to match camera angle

	Transform WorldXfm() const;
	Vector3 WorldPos() const;
	Vector3 WorldRot() const;
	Vector3 WorldScl() const;

	NEW_OBJECT(Positionable)
	REGISTER
};
