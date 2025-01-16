#include "obj.hpp"
#include <memory>
#include <vector>

#include <SDL2/SDL_log.h>

// #include "iface/istream.hpp"

// static int OBJ_REV = 0;
std::unordered_map<const char*, ObjFunc> Object::gObjectGenerators;

std::unique_ptr<Object> gSceneRootNode(new Object("[ROOT NODE]"));

Object::Object() : mName("[unnamed]"), mParent() {}

Object::Object(const char* name) : mName(name), mParent() {}

Object::~Object() {
	for (Object* obj : mChildObjs) delete obj;
	if (mParent.lock() != nullptr) {
		for (Object*& obj : mParent.lock()->mChildObjs) {
			if (obj == this) obj = nullptr;
		}
	}
	mParent.reset();
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

/* 
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
} */

void Object::Reparent(std::shared_ptr<Object> new_parent) {
	std::shared_ptr<Object> old_parent = mParent.lock();
	if (old_parent != nullptr) {
		for (std::vector<Object*>::iterator it = old_parent->mChildObjs.begin(); it != old_parent->mChildObjs.end(); it++) {
			if (*it == this) {
				old_parent->mChildObjs.erase(it);
				break;
			}
		}
	}

	mParent = new_parent;
	mParent.lock()->mChildObjs.push_back(this);
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

Object* Object::FindByName(std::string name) {
	if (mName == name) {
		return this;
	} else {
		for (Object* o : mChildObjs) {
			Object* ptr;
			if ((ptr = o->FindByName(name)) != nullptr) return ptr;
			else continue;
		}
		return nullptr;
	}
}

void Object::ApplyFuncToChildren(void (*func)(Object *)) {
	if (mChildObjs.empty()) goto skip_childen;
	for (Object* child : mChildObjs) {
		child->ApplyFuncToChildren(func);
	}
skip_childen:
	func(this);
}

Object* Object::New(std::string cls_name) {
	if (gObjectGenerators.contains(cls_name.c_str())) {
		return gObjectGenerators[cls_name.c_str()]();
	}
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "probably fine, but class %s failed to new properly", cls_name.c_str());
	return nullptr;
}
