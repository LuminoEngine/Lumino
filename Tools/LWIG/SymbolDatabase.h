
#pragma once
#include <memory>
#include <unordered_map>
#include "Common.h"

class SymbolDatabase;
class TypeInfo;
class ConstantInfo;
class DocumentInfo;
class PropertyInfo;

enum class AccessLevel
{
	Public,
	Protected,
	Private,
	Internal,
};

class ParameterDocumentInfo : public RefObject
{
public:
	String name;
	String io;
	String description;
};

class DocumentInfo : public RefObject
{
public:
	String summary;
	List<Ref<ParameterDocumentInfo>> params;
	String returns;
	String details;
	String copydocMethodName;
	String copydocSignature;

	bool IsCopyDoc() const { return !copydocMethodName.isEmpty(); }
};

// 属性マクロの ( ) 内に記述されたパラメータ
class MetadataInfo : public RefObject
{
public:
	String name;
	std::unordered_map<String, String> values;

	void AddValue(const String& key, const String& value);
	String* FindValue(const StringRef& key);
	bool HasKey(const StringRef& key);
};

class ParameterInfo : public RefObject
{
public:
	String name;
	Ref<TypeInfo> type;
	bool isIn = false;
	bool isOut = false;
	bool isThis = false;
	bool isReturn = false;
	Ref<ConstantInfo> defaultValue;

	// parsing data (link source)
	String typeRawName;
	Nullable<String> rawDefaultValue;
};

class FieldInfo : public RefObject
{
public:
	Ref<DocumentInfo> document;
	Ref<TypeInfo> type;
	String name;

	// parsing data (link source)
	String	typeRawName;
};

class MethodInfo : public RefObject
{
public:
	// 
	Ref<TypeInfo> owner;
	Ref<MetadataInfo> metadata;
	Ref<DocumentInfo> document;
	AccessLevel accessLevel = AccessLevel::Public;
	String name;
	Ref<TypeInfo> returnType;
	//IsConstructor
	//IsStatic
	//IsVirtual
	bool isConst = false;		// const メンバ関数であるか
	bool isStatic = false;
	bool isVirtual = false;
	bool isConstructor = false;
	Ref<PropertyInfo> ownerProperty;		// このメソッドがプロパティに含まれていればそのプロパティを指す
	List<Ref<ParameterInfo>> parameters;

	String overloadSuffix;
	Ref<MethodInfo> overloadParent;			// このメソッドはどのメソッドをオーバーロードするか (基本的に一番最初に見つかった定義)
	List<Ref<MethodInfo>> overloadChildren;	// このメソッドはどのメソッドにオーバーロードされるか
	// 
	List<Ref<ParameterInfo>> capiParameters;

	String	returnTypeRawName;
	String	paramsRawSignature;		// 型名と引数名を抽出したもの (デフォルト引数は除く) e.g) "constVector3&minVec,constVector3&maxVec"

	bool IsOverloadChild() const { return overloadParent != nullptr; }
	bool IsRuntimeInitializer() const { return metadata->HasKey(_T("RuntimeInitializer")); }
	bool IsEventSetter() const { return metadata->HasKey(_T("Event")); }

	void LinkParameters(SymbolDatabase* db);
	void ExpandCAPIParameters(SymbolDatabase* db);
	String GetCAPIFuncName();
	String GetCApiSetOverrideCallbackFuncName();
	String GetCApiSetOverrideCallbackTypeName();

	static String GetAccessLevelName(AccessLevel accessLevel);
};

class PropertyInfo : public RefObject
{
public:
	Ref<TypeInfo>			owner;
	Ref<DocumentInfo>		document;
	String				name;
	String				namePrefix;	// Is
	Ref<TypeInfo>			type;
	Ref<MethodInfo>		getter;
	Ref<MethodInfo>		setter;

	void MakeDocument();
};

class ConstantInfo : public RefObject
{
public:
	Ref<DocumentInfo>		document;
	String				name;
	Ref<TypeInfo>			type;
	Variant			value;

	String				typeRawName;
};

class TypeInfo : public RefObject
{
public:
	Ref<MetadataInfo>			metadata;
	Ref<DocumentInfo>			document;
	String	name;
	bool	isStruct = false;
	bool			isVoid = false;
	bool				isPrimitive = false;
	bool					isEnum = false;
	bool					isDelegate = false;
	List<Ref<FieldInfo>>		declaredFields;
	List<Ref<MethodInfo>>		declaredMethods;
	List<Ref<PropertyInfo>>	declaredProperties;
	List<Ref<ConstantInfo>>	declaredConstants;		// enum メンバ
	List<Ref<MethodInfo>>		declaredMethodsForDocument;	// LN_METHOD(Docuent)
	Ref<TypeInfo>				baseClass;

	String					baseClassRawName;

	TypeInfo() {}
	TypeInfo(StringRef name_) : name(name_) {}

	bool isValueType() const { return isStruct || isPrimitive || isEnum; }
	bool isStatic() const { return metadata->HasKey(_T("Static")); }
	bool IsClass() const { return !isValueType() && !isVoid; }

	void Link(SymbolDatabase* db);

private:
	void MakeProperties();
	void LinkOverload();
	void ResolveCopyDoc();
};

class PredefinedTypes
{
public:
	static Ref<TypeInfo>	voidType;
	static Ref<TypeInfo>	nullptrType;
	static Ref<TypeInfo>	boolType;
	static Ref<TypeInfo>	intType;
	static Ref<TypeInfo>	uint32Type;
	static Ref<TypeInfo>	floatType;
	static Ref<TypeInfo>	stringType;
	static Ref<TypeInfo>	objectType;
	static Ref<TypeInfo>	EventConnectionType;
};

class SymbolDatabase
{
public:
	List<Ref<TypeInfo>>	predefineds;
	List<Ref<TypeInfo>>	structs;
	List<Ref<TypeInfo>>	classes;
	List<Ref<TypeInfo>>	enums;
	List<Ref<TypeInfo>>	delegates;

	void Link();

	tr::Enumerator<Ref<MethodInfo>> GetAllMethods();

	void FindEnumTypeAndValue(const String& typeName, const String& memberName, Ref<TypeInfo>* outEnum, Ref<ConstantInfo>* outMember);
	Ref<ConstantInfo> CreateConstantFromLiteralString(const String& valueStr);

public:
	void InitializePredefineds();
	Ref<TypeInfo> findTypeInfo(StringRef typeName);
};


#if 0

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
using Ref<TypeInfo> = std::shared_ptr<TypeInfo>;
using Ref<DocumentInfo> = std::shared_ptr<DocumentInfo>;
using ParameterDocumentInfoPtr = std::shared_ptr<ParameterDocumentInfo>;
using Ref<MetadataInfo> = std::shared_ptr<MetadataInfo>;
using Ref<MethodInfo> = std::shared_ptr<MethodInfo>;
using Ref<PropertyInfo> = std::shared_ptr<PropertyInfo>;
using Ref<ConstantInfo> = std::shared_ptr<ConstantInfo>;

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
	Ref<TypeInfo>	type;
	bool	isIn = false;
	bool	isOut = false;
	bool	isThis = false;
	bool	isReturn = false;
	Ref<ConstantInfo>		defaultValue;

	String				typeRawName;
	Nullable<String>	rawDefaultValue;
};
using Ref<ParameterInfo> = std::shared_ptr<ParameterInfo>;

class FieldInfo
{
public:
	Ref<DocumentInfo>	document;
	Ref<TypeInfo>		type;
	String			name;

	String	typeRawName;
};
using Ref<FieldInfo> = std::shared_ptr<FieldInfo>;

class MethodInfo
{
public:
	// 
	Ref<TypeInfo>		owner;
	Ref<MetadataInfo>	metadata;
	Ref<DocumentInfo>	document;
	AccessLevel		accessLevel = AccessLevel::Public;
	String			name;
	Ref<TypeInfo>		returnType;
	//IsConstructor
	//IsStatic
	//IsVirtual
	bool			isConst = false;		// const メンバ関数であるか
	bool			isStatic = false;
	bool			isVirtual = false;
	bool			isConstructor = false;
	Ref<PropertyInfo>	ownerProperty;
	List<Ref<ParameterInfo>>	parameters;

	String					overloadSuffix;
	Ref<MethodInfo>			overloadParent;
	List<Ref<MethodInfo>>		overloadChildren;
	// 
	List<Ref<ParameterInfo>>	capiParameters;

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
	Ref<TypeInfo>			owner;
	Ref<DocumentInfo>		document;
	String				name;
	String				namePrefix;	// Is
	Ref<TypeInfo>			type;
	Ref<MethodInfo>		getter;
	Ref<MethodInfo>		setter;

	void MakeDocument();
};

class ConstantInfo
{
public:
	Ref<DocumentInfo>		document;
	String				name;
	Ref<TypeInfo>			type;
	//Variant			value;

	String				typeRawName;
};

class TypeInfo
{
public:
	Ref<MetadataInfo>			metadata;
	Ref<DocumentInfo>			document;
	String	name;
	bool	isStruct = false;
	bool			isVoid = false;
	bool				isPrimitive = false;
	bool					isEnum = false;
	bool					isDelegate = false;
	List<Ref<FieldInfo>>		declaredFields;
	List<Ref<MethodInfo>>		declaredMethods;
	List<Ref<PropertyInfo>>	declaredProperties;
	List<Ref<ConstantInfo>>	declaredConstants;		// enum メンバ
	List<Ref<MethodInfo>>		declaredMethodsForDocument;	// LN_METHOD(Docuent)
	Ref<TypeInfo>				baseClass;

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
	static Ref<TypeInfo>	voidType;
	static Ref<TypeInfo>	nullptrType;
	static Ref<TypeInfo>	boolType;
	static Ref<TypeInfo>	intType;
	static Ref<TypeInfo>	uint32Type;
	static Ref<TypeInfo>	floatType;
	static Ref<TypeInfo>	stringType;
	static Ref<TypeInfo>	objectType;
	static Ref<TypeInfo>	EventConnectionType;
};

class SymbolDatabase
{
public:
	List<Ref<TypeInfo>>	predefineds;
	List<Ref<TypeInfo>>	structs;
	List<Ref<TypeInfo>>	classes;
	List<Ref<TypeInfo>>	enums;
	List<Ref<TypeInfo>>	delegates;

	void Link();

	tr::Enumerator<Ref<MethodInfo>> GetAllMethods();

	void FindEnumTypeAndValue(const String& typeName, const String& memberName, Ref<TypeInfo>* outEnum, Ref<ConstantInfo>* outMember);
	Ref<ConstantInfo> CreateConstantFromLiteralString(const String& valueStr);

public:
	void InitializePredefineds();
	Ref<TypeInfo> findTypeInfo(StringRef typeName);
};

#endif
