
#pragma once
#include <memory>
#include <unordered_map>
#include "Common.h"
#include "Diagnostics.h"

class SymbolDatabase;
class TypeSymbol;
class ConstantSymbol;
class DocumentSymbol;
class PropertySymbol;

enum class AccessLevel
{
	Public,
	Protected,
	Private,
};

class ParameterDocumentSymbol : public RefObject
{
public:
	String name;
	String io;
	String description;
};

class DocumentSymbol : public RefObject
{
public:
	String summary;
	List<Ref<ParameterDocumentSymbol>> params;
	String returns;
	String details;
	String copydocMethodName;
	String copydocSignature;

	bool IsCopyDoc() const { return !copydocMethodName.isEmpty(); }
};

// 属性マクロの ( ) 内に記述されたパラメータ
class MetadataSymbol : public RefObject
{
public:
	String name;
	std::unordered_map<String, String> values;

	void AddValue(const String& key, const String& value);
	String* FindValue(const StringRef& key);
	bool HasKey(const StringRef& key);
};

class ParameterSymbol : public RefObject
{
public:
	String name;
	Ref<TypeSymbol> type;
	bool isIn = false;
	bool isOut = false;
	bool isThis = false;
	bool isReturn = false;
	Ref<ConstantSymbol> defaultValue;

	// parsing data (link source)
	String typeRawName;
	Nullable<String> rawDefaultValue;
};

class FieldSymbol : public RefObject
{
public:
	Ref<DocumentSymbol> document;
	Ref<TypeSymbol> type;
	String name;

	// parsing data (link source)
	String	typeRawName;
};

class MethodSymbol : public RefObject
{
public:
	// 
	Ref<TypeSymbol> owner;
	Ref<MetadataSymbol> metadata;
	Ref<DocumentSymbol> document;
	AccessLevel accessLevel = AccessLevel::Public;
	String name;
	Ref<TypeSymbol> returnType;
	//IsConstructor
	//IsStatic
	//IsVirtual
	bool isConst = false;		// [set from Parser] const メンバ関数であるか
	bool isStatic = false;		// [set from Parser]
	bool isVirtual = false;		// [set from Parser]
	bool isConstructor = false;
	Ref<PropertySymbol> ownerProperty;		// このメソッドがプロパティに含まれていればそのプロパティを指す
	List<Ref<ParameterSymbol>> parameters;

	String overloadSuffix;
	Ref<MethodSymbol> overloadParent;			// このメソッドはどのメソッドをオーバーロードするか (基本的に一番最初に見つかった定義)
	List<Ref<MethodSymbol>> overloadChildren;	// このメソッドはどのメソッドにオーバーロードされるか
	// 
	List<Ref<ParameterSymbol>> capiParameters;

	String	returnTypeRawName;
	String	paramsRawSignature;		// 型名と引数名を抽出したもの (デフォルト引数は除く) e.g) "constVector3&minVec,constVector3&maxVec"

	bool IsOverloadChild() const { return overloadParent != nullptr; }
	bool IsRuntimeInitializer() const { return metadata->HasKey(_T("RuntimeInitializer")); }
	bool IsEventSetter() const { return metadata->HasKey(_T("Event")); }

	void LinkParameters(SymbolDatabase* db);
	String GetCAPIFuncName();
	String GetCApiSetOverrideCallbackFuncName();
	String GetCApiSetOverrideCallbackTypeName();

	static String GetAccessLevelName(AccessLevel accessLevel);

	void link(SymbolDatabase* db);

private:
	void ExpandCAPIParameters(SymbolDatabase* db);
};

class PropertySymbol : public RefObject
{
public:
	Ref<TypeSymbol>			owner;
	Ref<DocumentSymbol>		document;
	String				name;
	String				namePrefix;	// Is
	Ref<TypeSymbol>			type;
	Ref<MethodSymbol>		getter;
	Ref<MethodSymbol>		setter;

	void MakeDocument();
};

class ConstantSymbol : public RefObject
{
public:
	Ref<DocumentSymbol>		document;
	String				name;
	Ref<TypeSymbol>			type;
	Variant			value;

	String				typeRawName;
};

class TypeSymbol : public RefObject
{
public:
	Ref<MetadataSymbol>			metadata;
	Ref<DocumentSymbol>			document;
	bool	isStruct = false;
	bool			isVoid = false;
	bool				isPrimitive = false;
	bool					isEnum = false;
	bool					isDelegate = false;
	List<Ref<FieldSymbol>>		declaredFields;
	List<Ref<MethodSymbol>>		declaredMethods;
	List<Ref<PropertySymbol>>	declaredProperties;
	List<Ref<ConstantSymbol>>	declaredConstants;		// enum メンバ
	List<Ref<MethodSymbol>>		declaredMethodsForDocument;	// LN_METHOD(Docuent)
	Ref<TypeSymbol>				baseClass;

	String					baseClassRawName;
	String					rawFullName;

	TypeSymbol() {}
	TypeSymbol(StringRef rawFullName_) { setRawFullName(rawFullName_); }

	const String& fullName() const { return rawFullName; }
	const String& shortName() const { return m_shortName; }

	bool isValueType() const { return isStruct || isPrimitive || isEnum; }
	bool isStatic() const { return metadata->HasKey(_T("Static")); }
	bool IsClass() const { return !isValueType() && !isVoid; }

	void Link(SymbolDatabase* db);

	void setRawFullName(const String& value);

private:
	void MakeProperties();
	void LinkOverload();
	void ResolveCopyDoc();

	String m_shortName;
};

class PredefinedTypes
{
public:
	static Ref<TypeSymbol>	voidType;
	static Ref<TypeSymbol>	nullptrType;
	static Ref<TypeSymbol>	boolType;
	static Ref<TypeSymbol>	intType;
	static Ref<TypeSymbol>	uint32Type;
	static Ref<TypeSymbol>	floatType;
	static Ref<TypeSymbol>	stringType;
	static Ref<TypeSymbol>	objectType;
	static Ref<TypeSymbol>	EventConnectionType;
};

class SymbolDatabase
{
public:
	List<Ref<TypeSymbol>>	predefineds;
	List<Ref<TypeSymbol>>	structs;
	List<Ref<TypeSymbol>>	classes;
	List<Ref<TypeSymbol>>	enums;
	List<Ref<TypeSymbol>>	delegates;

	void Link();

	tr::Enumerator<Ref<MethodSymbol>> GetAllMethods();

	void FindEnumTypeAndValue(const String& typeFullName, const String& memberName, Ref<TypeSymbol>* outEnum, Ref<ConstantSymbol>* outMember);
	Ref<ConstantSymbol> CreateConstantFromLiteralString(const String& valueStr);

	void verify(DiagManager* diag);

public:
	void InitializePredefineds();
	Ref<TypeSymbol> findTypeInfo(StringRef typeFullName);
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



class TypeSymbol;
class DocumentSymbol;
class ParameterDocumentSymbol;
class MetadataSymbol;
class MethodSymbol;
class PropertySymbol;
class ConstantSymbol;
using Ref<TypeSymbol> = std::shared_ptr<TypeSymbol>;
using Ref<DocumentSymbol> = std::shared_ptr<DocumentSymbol>;
using ParameterDocumentInfoPtr = std::shared_ptr<ParameterDocumentSymbol>;
using Ref<MetadataSymbol> = std::shared_ptr<MetadataSymbol>;
using Ref<MethodSymbol> = std::shared_ptr<MethodSymbol>;
using Ref<PropertySymbol> = std::shared_ptr<PropertySymbol>;
using Ref<ConstantSymbol> = std::shared_ptr<ConstantSymbol>;

enum class AccessLevel
{
	Public,
	Protected,
	Private,
	Internal,
};

class ParameterDocumentSymbol
{
public:
	String	name;
	String	io;
	String	description;
};

class DocumentSymbol
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

class MetadataSymbol
{
public:
	String								name;
	std::unordered_map<String, String>	values;

	void AddValue(const String& key, const String& value);
	String* FindValue(const StringRef& key);
	bool HasKey(const StringRef& key);
};


class ParameterSymbol
{
public:
	//DefaultValue
	//ParameterType
	String	name;
	Ref<TypeSymbol>	type;
	bool	isIn = false;
	bool	isOut = false;
	bool	isThis = false;
	bool	isReturn = false;
	Ref<ConstantSymbol>		defaultValue;

	String				typeRawName;
	Nullable<String>	rawDefaultValue;
};
using Ref<ParameterSymbol> = std::shared_ptr<ParameterSymbol>;

class FieldSymbol
{
public:
	Ref<DocumentSymbol>	document;
	Ref<TypeSymbol>		type;
	String			name;

	String	typeRawName;
};
using Ref<FieldSymbol> = std::shared_ptr<FieldSymbol>;

class MethodSymbol
{
public:
	// 
	Ref<TypeSymbol>		owner;
	Ref<MetadataSymbol>	metadata;
	Ref<DocumentSymbol>	document;
	AccessLevel		accessLevel = AccessLevel::Public;
	String			name;
	Ref<TypeSymbol>		returnType;
	//IsConstructor
	//IsStatic
	//IsVirtual
	bool			isConst = false;		// const メンバ関数であるか
	bool			isStatic = false;
	bool			isVirtual = false;
	bool			isConstructor = false;
	Ref<PropertySymbol>	ownerProperty;
	List<Ref<ParameterSymbol>>	parameters;

	String					overloadSuffix;
	Ref<MethodSymbol>			overloadParent;
	List<Ref<MethodSymbol>>		overloadChildren;
	// 
	List<Ref<ParameterSymbol>>	capiParameters;

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


class PropertySymbol
{
public:
	Ref<TypeSymbol>			owner;
	Ref<DocumentSymbol>		document;
	String				name;
	String				namePrefix;	// Is
	Ref<TypeSymbol>			type;
	Ref<MethodSymbol>		getter;
	Ref<MethodSymbol>		setter;

	void MakeDocument();
};

class ConstantSymbol
{
public:
	Ref<DocumentSymbol>		document;
	String				name;
	Ref<TypeSymbol>			type;
	//Variant			value;

	String				typeRawName;
};

class TypeSymbol
{
public:
	Ref<MetadataSymbol>			metadata;
	Ref<DocumentSymbol>			document;
	String	name;
	bool	isStruct = false;
	bool			isVoid = false;
	bool				isPrimitive = false;
	bool					isEnum = false;
	bool					isDelegate = false;
	List<Ref<FieldSymbol>>		declaredFields;
	List<Ref<MethodSymbol>>		declaredMethods;
	List<Ref<PropertySymbol>>	declaredProperties;
	List<Ref<ConstantSymbol>>	declaredConstants;		// enum メンバ
	List<Ref<MethodSymbol>>		declaredMethodsForDocument;	// LN_METHOD(Docuent)
	Ref<TypeSymbol>				baseClass;

	String					baseClassRawName;

	TypeSymbol() {}
	TypeSymbol(StringRef name_) : name(name_) {}

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
	static Ref<TypeSymbol>	voidType;
	static Ref<TypeSymbol>	nullptrType;
	static Ref<TypeSymbol>	boolType;
	static Ref<TypeSymbol>	intType;
	static Ref<TypeSymbol>	uint32Type;
	static Ref<TypeSymbol>	floatType;
	static Ref<TypeSymbol>	stringType;
	static Ref<TypeSymbol>	objectType;
	static Ref<TypeSymbol>	EventConnectionType;
};

class SymbolDatabase
{
public:
	List<Ref<TypeSymbol>>	predefineds;
	List<Ref<TypeSymbol>>	structs;
	List<Ref<TypeSymbol>>	classes;
	List<Ref<TypeSymbol>>	enums;
	List<Ref<TypeSymbol>>	delegates;

	void Link();

	tr::Enumerator<Ref<MethodSymbol>> GetAllMethods();

	void FindEnumTypeAndValue(const String& typeName, const String& memberName, Ref<TypeSymbol>* outEnum, Ref<ConstantSymbol>* outMember);
	Ref<ConstantSymbol> CreateConstantFromLiteralString(const String& valueStr);

public:
	void InitializePredefineds();
	Ref<TypeSymbol> findTypeInfo(StringRef typeName);
};

#endif
