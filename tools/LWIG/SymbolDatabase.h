
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
	TypeInfoPtr	type;
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
	String			name;
	TypeInfoPtr		returnType;
	//IsConstructor
	//IsStatic
	//IsVirtual
	bool			isConst = false;		// const ÉÅÉìÉoä÷êîÇ≈Ç†ÇÈÇ©
	bool			isStatic = false;
	bool			isConstructor = false;
	List<ParameterInfoPtr>	parameters;

	String	returnTypeRawName;
};
using MethodInfoPtr = std::shared_ptr<MethodInfo>;

class TypeInfo
{
public:
	String	name;
	bool	isStruct = false;
	bool			isVoid = false;
	bool				isPrimitive = false;
	List<FieldInfoPtr>	declaredFields;
	List<MethodInfoPtr>	declaredMethods;

	TypeInfo() {}
	TypeInfo(StringRef name_) : name(name_) {}

	bool IsValueType() const { return isStruct || isPrimitive; }
};


class SymbolDatabase
{
public:
	List<TypeInfoPtr>	predefineds;
	List<TypeInfoPtr>	structs;
	List<TypeInfoPtr>	classes;
	TypeInfoPtr			stringType;

	void Link();

private:
	void InitializePredefineds();
	TypeInfoPtr FindTypeInfo(StringRef typeName);
};

