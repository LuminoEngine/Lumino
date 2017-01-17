
#pragma once
#include <memory>
#include <Lumino/Base/Enumerable.h>

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

class DocumentInfo
{
public:
	String	summary;
};

using DocumentInfoPtr = std::shared_ptr<DocumentInfo>;

class ParameterInfo
{
public:
	//DefaultValue
	//ParameterType
	String	name;
	TypeInfoPtr	type;
	bool	isIn = false;
	bool	isOut = false;
	bool	isThis = false;
	bool	isReturn = false;
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
	// 
	TypeInfoPtr		owner;
	String			name;
	TypeInfoPtr		returnType;
	//IsConstructor
	//IsStatic
	//IsVirtual
	bool			isConst = false;		// const ÉÅÉìÉoä÷êîÇ≈Ç†ÇÈÇ©
	bool			isStatic = false;
	bool			isConstructor = false;
	List<ParameterInfoPtr>	parameters;

	// 
	List<ParameterInfoPtr>	capiParameters;

	String	returnTypeRawName;

	void ExpandCAPIParameters();
};
using MethodInfoPtr = std::shared_ptr<MethodInfo>;

class ConstantInfo
{
public:
	String			name;
	TypeInfoPtr		type;
	tr::Variant		value;

	String			typeRawName;
};
using ConstantInfoPtr = std::shared_ptr<ConstantInfo>;

class TypeInfo
{
public:
	String	name;
	bool	isStruct = false;
	bool			isVoid = false;
	bool				isPrimitive = false;
	bool					isEnum = false;
	List<FieldInfoPtr>		declaredFields;
	List<MethodInfoPtr>		declaredMethods;
	List<ConstantInfoPtr>	declaredConstants;
	DocumentInfoPtr			document;

	TypeInfo() {}
	TypeInfo(StringRef name_) : name(name_) {}

	bool IsValueType() const { return isStruct || isPrimitive; }
};

class PrimitiveTypes
{
public:
	static TypeInfoPtr	voidType;
	static TypeInfoPtr	boolType;
	static TypeInfoPtr	intType;
	static TypeInfoPtr	floatType;
	static TypeInfoPtr	stringType;
};

class SymbolDatabase
{
public:
	List<TypeInfoPtr>	predefineds;
	List<TypeInfoPtr>	structs;
	List<TypeInfoPtr>	classes;
	List<TypeInfoPtr>	enums;

	void Link();

	tr::Enumerator<MethodInfoPtr> GetAllMethods();

public:
	void InitializePredefineds();
	TypeInfoPtr FindTypeInfo(StringRef typeName);
};

