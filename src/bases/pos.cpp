#include "pos.hpp"
#include "math/xfm.hpp"
#include <sys/types.h>
#include <unordered_map>

static std::unordered_map<const Object *, const Positionable *> WorldCalcCache;

Positionable::Positionable() : mTransform(), mBillboarding(false) {}

Positionable::~Positionable() {}

void Positionable::OnMsg(Message &msg) {
	if (msg.mType == Message::kTranslate && std::get<uint32_t>(msg[6]) != 0) {
		mTransform.mPos.x += std::get<double>(msg[0]);
		mTransform.mPos.y += std::get<double>(msg[1]);
		mTransform.mPos.z += std::get<double>(msg[2]);
		mTransform.mRot.x += std::get<double>(msg[3]);
		mTransform.mRot.y += std::get<double>(msg[4]);
		mTransform.mRot.z += std::get<double>(msg[5]);
		msg.mNodes.reserve(7);
		msg.mNodes.at(6) = (uint32_t)1;
	}
}

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
