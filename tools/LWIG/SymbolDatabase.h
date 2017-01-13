
#pragma once
#include <memory>

//class Type
//{
//	Class		Generic
//	Struct
//	Enum
//	Delegate
//	Method
//};
//
//class Literal
//{
//};
//
//Module

class ParameterInfo
{
public:
	//DefaultValue
	//ParameterType
	String	name;
	bool	isIn = false;
	bool	isOut = false;
	Nullable<StringA>	defaultValue;

	String	typeRawName;
};
using ParameterInfoPtr = std::shared_ptr<ParameterInfo>;

class FieldInfo
{
public:
	String	name;

	String	typeRawName;
};
using FieldInfoPtr = std::shared_ptr<FieldInfo>;

class MethodInfo
{
public:
	String	name;

	//IsConstructor
	//IsStatic
	//IsVirtual
	List<FieldInfoPtr>	parameters;
};
using MethodInfoPtr = std::shared_ptr<MethodInfo>;

class TypeInfo
{
public:
	String	name;
	bool	isStruct = false;
	List<FieldInfoPtr>	declaredFields;
	List<MethodInfoPtr>	declaredMethods;
};

using TypeInfoPtr = std::shared_ptr<TypeInfo>;

class SymbolDatabase
{
public:
	List<TypeInfoPtr>	m_structs;
};

