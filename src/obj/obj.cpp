#include "obj.hpp"
#include <memory>
#include <vector>

std::shared_ptr<Object> gSceneRootNode(new Object("[ROOT NODE]"));

Object::Object() : mName("[unnamed]"), mParent(nullptr) {}

Object::Object(const char* name) : mName(name), mParent(nullptr) {}

Object::~Object() {
	mChildObjs.clear();
}

void Object::Print(std::ostream& stream) const {
	static int indent_lvl = 0;

	for (int _ = 0; _ < indent_lvl; _++) stream << '\t';
	stream << mName << " (" << ClassName() << ")\n";
	
	for (int _ = 0; _ < indent_lvl; _++) stream << '\t';
	if (!mChildObjs.empty()) {
		indent_lvl++;
		stream << "Children:\n";
		for (auto& child : mChildObjs) {
			child->Print(stream);
			stream << '\n';
		}
		indent_lvl--;
	} else {
		stream << "Leaf node." << std::endl;
	}
}

void Object::Reparent(Object* new_parent) {
	if (mParent != nullptr) {
//		std::erase_if(mParent->mChildObjs, [this](std::shared_ptr<Object> ptr) -> bool { return ptr.get() == this; });
		for (std::vector<Object*>::iterator it = mParent->mChildObjs.begin(); it != mParent->mChildObjs.end(); it++) {
			if (*it == this) {
				mParent->mChildObjs.erase(it);
				break;
			}
		}
	}

	mParent = new_parent;
	mParent->mChildObjs.push_back(this);
}
