#pragma once

#include <ostream>
#include <vector>
#include <memory>

#define CLASS_NAME_CRUFT(name) \
	static const char* StaticClassName() { static const char* cc = #name; return cc; }\
	virtual const char* ClassName() const { return StaticClassName(); }

/** A basic object. Can be used in the scene hierarchy. */
class IObject {
public:
	IObject();
	IObject(const char* name);
	virtual ~IObject();
	CLASS_NAME_CRUFT(IObject)
	virtual void Print(std::ostream&) const;
	
	template <typename T>
	bool CanBecome() const { if (dynamic_cast<T*>(this) != NULL) return true; else return false; }

	const char* mName;
	std::vector<std::shared_ptr<IObject>> mChildObjs;
};

extern std::unique_ptr<IObject> gSceneRootNode;
