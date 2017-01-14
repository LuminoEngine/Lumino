
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
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

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
	TypeInfoPtr	type;
	String		name;

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
	List<ParameterInfoPtr>	parameters;
};
using MethodInfoPtr = std::shared_ptr<MethodInfo>;

class TypeInfo
{
public:
	String	name;
	bool	isStruct = false;
	List<FieldInfoPtr>	declaredFields;
	List<MethodInfoPtr>	declaredMethods;

	TypeInfo() {}
	TypeInfo(StringRef name_) : name(name_) {}
};


class SymbolDatabase
{
public:
	List<TypeInfoPtr>	predefineds;
	List<TypeInfoPtr>	structs;

	void Link();

private:
	void InitializePredefineds();
	TypeInfoPtr FindTypeInfo(StringRef typeName);
};

