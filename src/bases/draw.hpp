#pragma once

#include "bases/obj.hpp"

class Drawable : public virtual Object {
public:
	Drawable() : mShowing(false) {}
	virtual ~Drawable() {}
	CLASS_NAME_CRUFT(Drawable)
	virtual void Draw() {}

	bool mShowing;
};
