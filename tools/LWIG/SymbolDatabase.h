
#pragma once
#include <memory>
#include <unordered_map>
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
class DocumentInfo;
class ParameterDocumentInfo;
class MetadataInfo;
class MethodInfo;
class PropertyInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;
using DocumentInfoPtr = std::shared_ptr<DocumentInfo>;
using ParameterDocumentInfoPtr = std::shared_ptr<ParameterDocumentInfo>;
using MetadataInfoPtr = std::shared_ptr<MetadataInfo>;
using MethodInfoPtr = std::shared_ptr<MethodInfo>;
using PropertyInfoPtr = std::shared_ptr<PropertyInfo>;

enum class AccessLevel
{
	Public,
	Protected,
	Private,
	Internal,
};

class ParameterDocumentInfo
{
public:
	String	name;
	String	io;
	String	description;
};

class DocumentInfo
{
public:
	String							summary;
	List<ParameterDocumentInfoPtr>	params;
	String							returns;
	String							details;
};

class MetadataInfo
{
public:
	String								name;
	std::unordered_map<String, String>	values;

	void AddValue(const String& key, const String& value);
	String* FindValue(const StringRef& key);
	bool HasKey(const StringRef& key);
};


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
	DocumentInfoPtr	document;
	TypeInfoPtr		type;
	String			name;

	String	typeRawName;
};
using FieldInfoPtr = std::shared_ptr<FieldInfo>;

class MethodInfo
{
public:
	// 
	TypeInfoPtr		owner;
	MetadataInfoPtr	metadata;
	DocumentInfoPtr	document;
	AccessLevel		accessLevel = AccessLevel::Public;
	String			name;
	TypeInfoPtr		returnType;
	//IsConstructor
	//IsStatic
	//IsVirtual
	bool			isConst = false;		// const メンバ関数であるか
	bool			isStatic = false;
	bool			isVirtual = false;
	bool			isConstructor = false;
	PropertyInfoPtr	ownerProperty;
	List<ParameterInfoPtr>	parameters;

	String					overloadSuffix;
	MethodInfoPtr			overloadParent;
	List<MethodInfoPtr>		overloadChildren;
	// 
	List<ParameterInfoPtr>	capiParameters;

	String	returnTypeRawName;

	bool IsOverloadChild() const { return overloadParent != nullptr; }
	bool IsRuntimeInitializer() const { return metadata->HasKey(_T("RuntimeInitializer")); }

	void LinkParameters();
	void ExpandCAPIParameters();
	String GetCAPIFuncName();
	String GetCApiSetOverrideCallbackFuncName();
	String GetCApiSetOverrideCallbackTypeName();

	static String GetAccessLevelName(AccessLevel accessLevel);
};


class PropertyInfo
{
public:
	TypeInfoPtr			owner;
	DocumentInfoPtr		document;
	String				name;
	TypeInfoPtr			type;
	MethodInfoPtr		getter;
	MethodInfoPtr		setter;

	void MakeDocument();
};

class ConstantInfo
{
public:
	DocumentInfoPtr		document;
	String				name;
	TypeInfoPtr			type;
	tr::Variant			value;

	String				typeRawName;
};
using ConstantInfoPtr = std::shared_ptr<ConstantInfo>;

class TypeInfo
{
public:
	MetadataInfoPtr			metadata;
	DocumentInfoPtr			document;
	String	name;
	bool	isStruct = false;
	bool			isVoid = false;
	bool				isPrimitive = false;
	bool					isEnum = false;
	List<FieldInfoPtr>		declaredFields;
	List<MethodInfoPtr>		declaredMethods;
	List<PropertyInfoPtr>	declaredProperties;
	List<ConstantInfoPtr>	declaredConstants;
	TypeInfoPtr				baseClass;

	String					baseClassRawName;

	TypeInfo() {}
	TypeInfo(StringRef name_) : name(name_) {}

	bool IsValueType() const { return isStruct || isPrimitive; }
	bool IsStatic() const { return metadata->HasKey(_T("Static")); }
	bool IsClass() const { return !IsValueType(); }

	void Link();

private:
	void MakeProperties();
	void LinkOverload();
};

class PredefinedTypes
{
public:
	static TypeInfoPtr	voidType;
	static TypeInfoPtr	boolType;
	static TypeInfoPtr	intType;
	static TypeInfoPtr	uint32Type;
	static TypeInfoPtr	floatType;
	static TypeInfoPtr	stringType;
	static TypeInfoPtr	objectType;
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

