#pragma once

#include <ostream>
#include <vector>
#include <memory>

#define CLASS_NAME_CRUFT(name) \
	static const char* StaticClassName() { static const char* cc = #name; return cc; }\
	virtual const char* ClassName() const { return StaticClassName(); }

class IStream;

/** A basic object. Can be used in the scene hierarchy. */
class Object {
public:
	Object();
	Object(const char* name);
	Object(const Object&&) = delete;
	virtual ~Object();
	CLASS_NAME_CRUFT(Object)
	virtual void Print(std::ostream&) const;
	virtual void Save(IStream&) const;
	virtual void Load(IStream&);
	
	template <typename T>
	bool CanBecome() const { if (dynamic_cast<const T*>(this) != NULL) return true; else return false; }
	
	void Reparent(Object* new_parent);

	/// recursively searches mChildObjs to find an object.
	const Object* FindByName(std::string name) const;

	std::string mName;
	std::shared_ptr<Object> mParent;
	std::vector<Object*> mChildObjs;
};

template <typename T>
concept IsObj = std::is_base_of_v<T, Object> || std::is_same_v<T, Object>;

extern std::shared_ptr<Object> gSceneRootNode;