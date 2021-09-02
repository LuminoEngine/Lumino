﻿#pragma once

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
	//Delegate,	// deprecated
	Function,	// raw function signature.
};

enum class TypeClass
{
	None,
	Collection,
	DelegateObject,
	Promise,
};

inline AccessLevel accessLevelAsEnum(const ln::String& accessLevel)
{
	if (ln::String::compare(accessLevel, u"Private", ln::CaseSensitivity::CaseInsensitive) == 0)
		return AccessLevel::Private;
	if (ln::String::compare(accessLevel, u"Protected", ln::CaseSensitivity::CaseInsensitive) == 0)
		return AccessLevel::Protected;
	if (ln::String::compare(accessLevel, u"Public", ln::CaseSensitivity::CaseInsensitive) == 0)
		return AccessLevel::Public;

	LN_UNREACHABLE();
	return AccessLevel::Private;
}

inline TypeKind kindAsEnum(const ln::String& kind)
{
	if (ln::String::compare(kind, u"Primitive", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeKind::Primitive;
	if (ln::String::compare(kind, u"Enum", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeKind::Enum;
	if (ln::String::compare(kind, u"Struct", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeKind::Struct;
	if (ln::String::compare(kind, u"Class", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeKind::Class;
	//if (ln::String::compare(kind, u"Delegate_deprecated", ln::CaseSensitivity::CaseInsensitive) == 0)
	//	return TypeKind::Delegate;
	if (ln::String::compare(kind, u"Collection", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeKind::Class;
    if (ln::String::compare(kind, u"DelegateObject", ln::CaseSensitivity::CaseInsensitive) == 0)
        return TypeKind::Class;
	if (ln::String::compare(kind, u"Promise", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeKind::Class;

	LN_UNREACHABLE();
	return TypeKind::Primitive;
}

inline TypeClass classAsEnum(const ln::String& kind)
{
	if (ln::String::compare(kind, u"Primitive", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeClass::None;
	if (ln::String::compare(kind, u"Enum", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeClass::None;
	if (ln::String::compare(kind, u"Struct", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeClass::None;
	if (ln::String::compare(kind, u"Class", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeClass::None;
	if (ln::String::compare(kind, u"Delegate", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeClass::None;
	if (ln::String::compare(kind, u"Collection", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeClass::Collection;
	if (ln::String::compare(kind, u"DelegateObject", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeClass::DelegateObject;
	if (ln::String::compare(kind, u"Promise", ln::CaseSensitivity::CaseInsensitive) == 0)
		return TypeClass::Promise;

	LN_UNREACHABLE();
	return TypeClass::None;
}
