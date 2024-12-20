#pragma once

#include <ostream>
#include <vector>
#include <memory>

#define CLASS_NAME_CRUFT(name) \
	static const char* StaticClassName() { static const char* cc = #name; return cc; }\
	virtual const char* ClassName() const { return StaticClassName(); }

/** A basic object. Can be used in the scene hierarchy. */
class Object {
public:
	Object();
	Object(const char* name);
	Object(const Object&&) = delete;
	virtual ~Object();
	CLASS_NAME_CRUFT(Object)
	virtual void Print(std::ostream&) const;
//	virtual void Save() const;
//	virtual void Load();
	
	template <typename T>
	bool CanBecome() const { if (dynamic_cast<const T*>(this) != NULL) return true; else return false; }
	void Reparent(Object* new_parent);

	std::string mName;
	Object* mParent;
	std::vector<Object*> mChildObjs;
};

extern std::shared_ptr<Object> gSceneRootNode;
