#include "obj.hpp"
#include <memory>
#include <vector>

#include "iface/istream.hpp"

static int OBJ_REV = 0;

std::shared_ptr<Object> gSceneRootNode(new Object("[ROOT NODE]"));

Object::Object() : mName("[unnamed]"), mParent(nullptr) {}

Object::Object(const char* name) : mName(name), mParent(nullptr) {}

Object::~Object() {
	mChildObjs.clear();
}

void Object::Print(std::ostream& stream) const {
	static int indent_lvl = 0;

	for (int _ = indent_lvl; _--;) stream << '\t';
	stream << mName << " (" << ClassName() << ")\n";
	
	for (int _ = indent_lvl; _--;) stream << '\t';
	if (!mChildObjs.empty()) {
		indent_lvl++;
		stream << "Children:\n";
		for (const Object* child : mChildObjs) {
			child->Print(stream);
			stream << '\n';
		}
		indent_lvl--;
	} else {
		stream << "Leaf node." << std::endl;
	}
}

void Object::Save(IStream& strm) const {
	strm << OBJ_REV;
	strm << mName;
	if (mParent) strm << mParent->mName;
	else strm << std::string("");
	strm << mChildObjs;
}

void Object::Load(IStream& strm) {
	int rev;
	strm >> rev;
	strm >> mName;
	std::string parent_name;
	strm >> parent_name;
	mParent.reset(const_cast<Object*>(FindByName(parent_name)));
	strm >> mChildObjs;
}

void Object::Reparent(Object* new_parent) {
	if (mParent != nullptr) {
		for (std::vector<Object*>::iterator it = mParent->mChildObjs.begin(); it != mParent->mChildObjs.end(); it++) {
			if (*it == this) {
				mParent->mChildObjs.erase(it);
				break;
			}
		}
	}

	mParent.reset(new_parent);
	mParent->mChildObjs.push_back(this);
}

const Object* Object::FindByName(std::string name) const {
	if (mName == name) {
		return this;
	} else {
		for (const Object* o : mChildObjs) {
			const Object* ptr;
			if ((ptr = o->FindByName(name)) != nullptr) return ptr;
			else continue;
		}
		return nullptr;
	}
}
