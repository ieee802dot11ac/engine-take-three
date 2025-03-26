#include "pos.hpp"
#include "math/xfm.hpp"
#include <unordered_map>

static std::unordered_map<const Object *, const Positionable *> WorldCalcCache;

Positionable::Positionable() : mTransform(), mBillboarding(false) {}

Positionable::~Positionable() {}

Transform Positionable::WorldXfm() const {
	Transform t = mTransform;
	const Object *current = mParent;
	while (current != nullptr) {
		if (current->CanBecome<Positionable>()) {
			const Positionable *dipshit = nullptr;
			if (!WorldCalcCache.contains(current)) {
				dipshit = dynamic_cast<const Positionable *>(current);
				WorldCalcCache.emplace(current, dipshit);
			} else
				dipshit = WorldCalcCache.at(current);
			t += dipshit->mTransform;
		}
		current = current->mParent;
	}
	return t;
}

Vector3 Positionable::WorldPos() const { return WorldXfm().mPos; }

Vector3 Positionable::WorldRot() const { return WorldXfm().mRot; }

Vector3 Positionable::WorldScl() const { return WorldXfm().mScale; }
