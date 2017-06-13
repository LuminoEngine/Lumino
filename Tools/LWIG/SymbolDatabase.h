
#pragma once
#include <memory>
#include <unordered_map>
#include "Common.h"

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
class ConstantInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;
using DocumentInfoPtr = std::shared_ptr<DocumentInfo>;
using ParameterDocumentInfoPtr = std::shared_ptr<ParameterDocumentInfo>;
using MetadataInfoPtr = std::shared_ptr<MetadataInfo>;
using MethodInfoPtr = std::shared_ptr<MethodInfo>;
using PropertyInfoPtr = std::shared_ptr<PropertyInfo>;
using ConstantInfoPtr = std::shared_ptr<ConstantInfo>;

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
	String							copydocMethodName;
	String							copydocSignature;

	bool IsCopyDoc() const { return !copydocMethodName.isEmpty(); }
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
	ConstantInfoPtr		defaultValue;

	String				typeRawName;
	Nullable<StringA>	rawDefaultValue;
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
	String	paramsRawSignature;		// 型名と引数名を抽出したもの (デフォルト引数は除く) e.g) "constVector3&minVec,constVector3&maxVec"

	bool IsOverloadChild() const { return overloadParent != nullptr; }
	bool IsRuntimeInitializer() const { return metadata->HasKey(_T("RuntimeInitializer")); }
	bool IsEventSetter() const { return metadata->HasKey(_T("Event")); }

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
	String				namePrefix;	// Is
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
	bool					isDelegate = false;
	List<FieldInfoPtr>		declaredFields;
	List<MethodInfoPtr>		declaredMethods;
	List<PropertyInfoPtr>	declaredProperties;
	List<ConstantInfoPtr>	declaredConstants;		// enum メンバ
	List<MethodInfoPtr>		declaredMethodsForDocument;	// LN_METHOD(Docuent)
	TypeInfoPtr				baseClass;

	String					baseClassRawName;

	TypeInfo() {}
	TypeInfo(StringRef name_) : name(name_) {}

	bool isValueType() const { return isStruct || isPrimitive || isEnum; }
	bool isStatic() const { return metadata->HasKey(_T("Static")); }
	bool IsClass() const { return !isValueType() && !isVoid; }

	void Link();

private:
	void MakeProperties();
	void LinkOverload();
	void ResolveCopyDoc();
};

class PredefinedTypes
{
public:
	static TypeInfoPtr	voidType;
	static TypeInfoPtr	nullptrType;
	static TypeInfoPtr	boolType;
	static TypeInfoPtr	intType;
	static TypeInfoPtr	uint32Type;
	static TypeInfoPtr	floatType;
	static TypeInfoPtr	stringType;
	static TypeInfoPtr	objectType;
	static TypeInfoPtr	EventConnectionType;
};

class SymbolDatabase
{
public:
	List<TypeInfoPtr>	predefineds;
	List<TypeInfoPtr>	structs;
	List<TypeInfoPtr>	classes;
	List<TypeInfoPtr>	enums;
	List<TypeInfoPtr>	delegates;

	void Link();

	tr::Enumerator<MethodInfoPtr> GetAllMethods();

	void FindEnumTypeAndValue(const String& typeName, const String& memberName, TypeInfoPtr* outEnum, ConstantInfoPtr* outMember);
	ConstantInfoPtr CreateConstantFromLiteralString(const String& valueStr);

public:
	void InitializePredefineds();
	TypeInfoPtr findTypeInfo(StringRef typeName);
};

