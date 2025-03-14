#pragma once

#include "bases/msg.hpp"
#include "bases/obj.hpp"

class Drawable : public virtual Object {
  public:
	Drawable() : mShowing(false) {}
	virtual ~Drawable() {}
	CLASS_NAME_CRUFT(Drawable)
	virtual void Draw() = 0;
	virtual void OnMsg(Message &msg) {
		if (msg == Message::kDraw)
			Draw();
	}

	bool mShowing;
};
