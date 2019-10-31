#pragma once

template<class T>
using Ref = ln::Ref<T>;

enum class AccessLevel
{
    Public,
    Protected,
    Private,
};

enum class TypeKind
{
	Primitive,
	Struct,
	Class,
	Enum,
	Delegate,
};
