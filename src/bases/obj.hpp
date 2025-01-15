#pragma once

#include <ostream>
#include <unordered_map>
#include <vector>
#include <memory>

// macros to make loading from disk easier
// uses const char* instead of std::string due to not changing @ runtime, and
// thus can't have as many handling bugs. and i am NOT using a giant enum
#define CLASS_NAME_CRUFT(name) \
	static const char* StaticClassName() { static const char* cc = #name; return cc; }\
	virtual const char* ClassName() const { return StaticClassName(); }

class IStream;

typedef class Object* (*ObjFunc)(void);

/** A basic object. Can be used in the scene hierarchy. */
class Object {
public:
	Object();
	Object(const char* name);
	Object(const Object&&) = delete;
	virtual ~Object();
	CLASS_NAME_CRUFT(Object)
	virtual void Print(std::ostream&) const;
//	virtual void Save(IStream&) const; // on hold until i write a DirLoader clone
//	virtual void Load(IStream&);

	/// recursively searches mChildObjs to find an object. grabs the first node it finds with the matching name.
	const Object* FindByName(std::string name) const;
	Object* FindByName(std::string name);

	/// Reparents `this` to new_parent.
	void Reparent(std::shared_ptr<Object> new_parent);

	/// Recursively runs a function to `this` and to its children.	
	void ApplyFuncToChildren(void (*)(Object*));
	
	// Create a new object from gObjectGenerators via the name given by cls_name.
	Object* New(std::string cls_name);

	static std::unordered_map<const char*, ObjFunc> gObjectGenerators; 

	template <typename T>
	bool CanBecome() const { if (dynamic_cast<const T*>(this) != NULL) return true; else return false; }

	template <typename T>
	void DoXIfIs(void (*func)(T*)) { T* obj = dynamic_cast<T*>(this); if (obj != nullptr) func(obj); }


	std::string mName;
	std::weak_ptr<Object> mParent;
	std::vector<Object*> mChildObjs;
};

template <typename T>
concept IsObj = std::is_base_of_v<T, Object> || std::is_same_v<T, Object>;

extern std::unique_ptr<Object> gSceneRootNode;
