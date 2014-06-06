#ifndef HEADER_ZN_OBJECT_HPP_INCLUDED
#define HEADER_ZN_OBJECT_HPP_INCLUDED

#include <fm/reflect/ObjectType.hpp>

// Constraints of the reflection system:
// Classes must define a parameter-less constructor
// Single inheritance only
// Type IDs must be the same on whatever platform or compiler
// Must be usable from application scripts (not only native code)

#define _ZN_OBJECT(_name, _baseName)                                           \
	static constexpr const char * __className = #_name;                        \
	static constexpr const char * __baseClassName = #_baseName;                \
	static zn::ObjectType & sObjectType();                                     \
	const zn::ObjectType & objectType() const override {                       \
		return sObjectType();                                                  \
	}

// Put this declaration in public in a class inheriting from Object.
// Note: only single inheritance is supported.
// _name: class name with full namespace (must be unique !)
// _baseName: inherited class name with full namespace
#define ZN_OBJECT(_name, _baseName)                                            \
	_ZN_OBJECT(_name, _baseName)                                               \
	static zn::Object * instantiate() {                                        \
		return new _name();                                                    \
	}

// Variant for abstract types (the first one will not compile for them)
// TODO use std::is_abstract<T>
#define ZN_ABSTRACT_OBJECT(_name, _baseName)                                   \
	_ZN_OBJECT(_name, _baseName)                                               \
	static zn::Object * instantiate() {                                        \
		return nullptr;                                                        \
	}

// Put this in your .cpp in order to implement the type
#define ZN_OBJECT_IMPL(_className)                                             \
	zn::ObjectType & _className::sObjectType() {                               \
		static zn::ObjectType t(                                               \
			_className::__className,                                           \
			_className::__baseClassName);                                      \
		return t;                                                              \
	}

namespace zn
{

class Object
{
public:

	//ZN_ABSTRACT_OBJECT(zn::Object,)

	/// \brief base metaclass.
	static zn::ObjectType & sObjectType();

	/// \brief get the metaclass of the object.
	virtual const zn::ObjectType & objectType() const
	{
		return sObjectType();
	}

	/// \brief Tests if the object is an instance of the given type.
	/// It also works for inherited types, if they share the same reflection system.
	template <class Object_T>
	bool isInstanceOf()
	{
		ObjectType & expectedType = Object_T::sObjectType();
		return objectType().is(expectedType);
	}
};

} // namespace zn

#endif // HEADER_ZN_OBJECT_HPP_INCLUDED




