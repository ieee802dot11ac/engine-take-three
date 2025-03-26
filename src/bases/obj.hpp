#pragma once

#include "bases/msg.hpp"
#include <SDL2/SDL_assert.h>
#include <memory>
#include <ostream>
#include <unordered_map>
#include <vector>

// macros to make loading from disk easier
// uses const char* instead of std::string due to not changing @ runtime, and
// thus can't have as many handling bugs. and i am NOT using a giant enum
#define CLASS_NAME_CRUFT(name)                                                 \
	static const char *StaticClassName() {                                     \
		static const char *cc = #name;                                         \
		return cc;                                                             \
	}                                                                          \
	virtual const char *ClassName() const { return StaticClassName(); }

#define NEW_OBJECT(cls)                                                        \
	static Object *NewObject() { return new cls; }

#define REGISTER                                                               \
	static void Register() {                                                   \
		Object::gObjectGenerators.emplace(StaticClassName(), NewObject);       \
	}

class IStream;

typedef class Object *(*ObjFunc)(void);

/** A basic object. Can be used in the scene hierarchy. */
class Object {
  public:
	Object();
	Object(const char *name);
	Object(const Object &&) = delete;
	virtual ~Object();
	CLASS_NAME_CRUFT(Object)
	virtual void Print(std::ostream &) const;
	//	virtual void Save(IStream&) const; // on hold until i write a DirLoader
	// clone
	// virtual void Load(IStream&);
	virtual void OnMsg(Message &);

	/// recursively searches mChildObjs to find an object. grabs the first node
	/// it finds with the matching name.
	const Object *FindByName(std::string name) const;
	Object *FindByName(std::string name);

	/// Reparents `this` to new_parent.
	void Reparent(std::unique_ptr<Object> &new_parent);
	void Reparent(std::shared_ptr<Object> &new_parent);
	void Reparent(Object *new_parent);

	/// Recursively runs a function to `this` and to its children.
	void ApplyFuncToChildren(void (*)(Object *));

	template <typename T> bool CanBecome() const {
		const Object *o = this;
		SDL_assert(o != nullptr);
		if (dynamic_cast<const T *>(this) != NULL)
			return true;
		else
			return false;
	}

	template <typename T> void DoXIfIs(void (*func)(T *)) {
		Object *o = this;
		SDL_assert(o != nullptr);
		T *obj = dynamic_cast<T *>(this);
		if (obj != nullptr)
			func(obj);
	}

	std::string mName;
	Object *mParent;
	std::vector<Object *> mChildObjs;

	/// Create a new object from gObjectGenerators via the name given by
	/// cls_name.
	static Object *New(std::string cls_name);

	static std::unordered_map<const char *, ObjFunc> gObjectGenerators;

	NEW_OBJECT(Object)
	REGISTER
};

template <typename T>
concept IsObj = std::is_base_of_v<T, Object> || std::is_same_v<T, Object>;

extern std::unique_ptr<Object> gSceneRootNode;
