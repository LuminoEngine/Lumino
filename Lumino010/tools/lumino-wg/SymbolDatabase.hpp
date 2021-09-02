
#pragma once
#include <memory>
#include <unordered_map>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include "Common.hpp"

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

class ParameterDocumentSymbol : public ln::RefObject
{
public:
	ln::String name;
	ln::String io;
	ln::String description;
};

class DocumentSymbol : public ln::RefObject
{
public:
	ln::String summary;
	ln::List<ln::Ref<ParameterDocumentSymbol>> params;
	ln::String returns;
	ln::String details;
	ln::String copydocMethodName;
	ln::String copydocSignature;

	bool IsCopyDoc() const { return !copydocMethodName.isEmpty(); }
};

// 属性マクロの ( ) 内に記述されたパラメータ
class MetadataSymbol : public ln::RefObject
{
public:
	static const ln::String OverloadPostfixAttr;

	ln::String name;
	std::unordered_map<ln::String, ln::String> values;

	void AddValue(const ln::String& key, const ln::String& value);
	ln::String* FindValue(const ln::StringRef& key);
	ln::String getValue(const ln::StringRef& key, const ln::String& defaultValue = ln::String());
	bool HasKey(const ln::StringRef& key);
};

class ParameterSymbol : public ln::RefObject
{
public:
	struct SoueceData
	{
		ln::String typeRawName;
		ln::Optional<ln::String> rawDefaultValue;
	} src;

	ln::String name;
	ln::Ref<TypeSymbol> type;
	bool isIn = false;
	bool isOut = false;
	bool isThis = false;
	bool isReturn = false;
	ln::Ref<ConstantSymbol> defaultValue;
};

class FieldSymbol : public ln::RefObject
{
public:
	ln::Ref<DocumentSymbol> document;
	ln::Ref<TypeSymbol> type;
	ln::String name;

	// parsing data (link source)
	ln::String	typeRawName;
};

class MethodSymbol : public ln::RefObject
{
public:
	// 
	ln::Ref<TypeSymbol> owner;
	ln::Ref<MetadataSymbol> metadata;
	ln::Ref<DocumentSymbol> document;
	AccessLevel accessLevel = AccessLevel::Public;
	ln::String name;
	ln::Ref<TypeSymbol> returnType;
	//IsConstructor
	//IsStatic
	//IsVirtual
	bool isConst = false;		// [set from Parser] const メンバ関数であるか
	bool isStatic = false;		// [set from Parser]
	bool isVirtual = false;		// [set from Parser]
	bool isConstructor = false;
	ln::Ref<PropertySymbol> ownerProperty;		// このメソッドがプロパティに含まれていればそのプロパティを指す
	ln::List<ln::Ref<ParameterSymbol>> parameters;

	ln::Ref<MethodSymbol> overloadParent;			// このメソッドはどのメソッドをオーバーロードするか (基本的に一番最初に見つかった定義)
	ln::List<ln::Ref<MethodSymbol>> overloadChildren;	// このメソッドはどのメソッドにオーバーロードされるか
	// 
	ln::List<ln::Ref<ParameterSymbol>> capiParameters;

	// parsing data
	ln::String	returnTypeRawName;
	ln::String	paramsRawSignature;		// 型名と引数名を抽出したもの (デフォルト引数は除く) e.g) "constVector3&minVec,constVector3&maxVec"

	bool IsOverloadChild() const { return overloadParent != nullptr; }
	bool IsRuntimeInitializer() const { return metadata->HasKey(_T("RuntimeInitializer")); }
	bool IsEventSetter() const { return metadata->HasKey(_T("Event")); }

	void LinkParameters(SymbolDatabase* db);
	ln::String GetCAPIFuncName();
	ln::String GetCApiSetOverrideCallbackFuncName();
	ln::String GetCApiSetOverrideCallbackTypeName();

	static ln::String GetAccessLevelName(AccessLevel accessLevel);

	void link(SymbolDatabase* db);

private:
	void ExpandCAPIParameters(SymbolDatabase* db);
};

class PropertySymbol : public ln::RefObject
{
public:
	ln::Ref<TypeSymbol>			owner;
	ln::Ref<DocumentSymbol>		document;
	ln::String				name;
	ln::String				namePrefix;	// Is
	ln::Ref<TypeSymbol>			type;
	ln::Ref<MethodSymbol>		getter;
	ln::Ref<MethodSymbol>		setter;

	void MakeDocument();
};

class ConstantSymbol : public ln::RefObject
{
public:
	ln::Ref<DocumentSymbol>		document;
	ln::String				name;
	ln::Ref<TypeSymbol>			type;
	ln::Ref<ln::Variant>			value;

	ln::String				typeRawName;
};

class TypeSymbol : public ln::RefObject
{
public:
	struct SoueceData
	{
		ln::String baseClassRawName;
		ln::String rawFullName;
	} src;

	ln::Ref<MetadataSymbol>			metadata;
	ln::Ref<DocumentSymbol>			document;
	bool	isStruct = false;
	bool			isVoid = false;
	bool				isPrimitive = false;
	bool					isEnum = false;
	bool					isDelegate = false;
	ln::List<ln::Ref<FieldSymbol>>		declaredFields;
	ln::List<ln::Ref<MethodSymbol>>		declaredMethods;
	ln::List<ln::Ref<PropertySymbol>>	declaredProperties;
	ln::List<ln::Ref<ConstantSymbol>>	declaredConstants;		// enum メンバ
	ln::List<ln::Ref<MethodSymbol>>		declaredMethodsForDocument;	// LN_METHOD(Docuent)
	ln::Ref<TypeSymbol>				baseClass;


	TypeSymbol() {}
	TypeSymbol(ln::StringRef rawFullName_) { setRawFullName(rawFullName_); }

	const ln::String& fullName() const { return src.rawFullName; }
	const ln::String& shortName() const { return m_shortName; }

	bool isValueType() const { return isStruct || isPrimitive || isEnum; }
	bool isStatic() const { return metadata->HasKey(_T("Static")); }
	bool IsClass() const { return !isValueType() && !isVoid; }

	void Link(SymbolDatabase* db);

	void setRawFullName(const ln::String& value);

private:
	void MakeProperties();
	void LinkOverload(SymbolDatabase* db);
	void ResolveCopyDoc();

	ln::String m_shortName;
};

class PredefinedTypes
{
public:
	static ln::Ref<TypeSymbol>	voidType;
	static ln::Ref<TypeSymbol>	nullptrType;
	static ln::Ref<TypeSymbol>	boolType;
	static ln::Ref<TypeSymbol>	intType;
	static ln::Ref<TypeSymbol>	int16Type;
	static ln::Ref<TypeSymbol>	uint32Type;
	static ln::Ref<TypeSymbol>	floatType;
	static ln::Ref<TypeSymbol>	stringType;
	static ln::Ref<TypeSymbol>	objectType;
	static ln::Ref<TypeSymbol>	EventConnectionType;
};

class SymbolDatabase
{
public:
	ln::List<ln::Ref<TypeSymbol>>	predefineds;
	ln::List<ln::Ref<TypeSymbol>>	structs;
	ln::List<ln::Ref<TypeSymbol>>	classes;
	ln::List<ln::Ref<TypeSymbol>>	enums;
	ln::List<ln::Ref<TypeSymbol>>	delegates;

	SymbolDatabase(ln::DiagnosticsManager* diag);
	ln::DiagnosticsManager* diag() const { return m_diag; }


	void Link();

	//tr::Enumerator<ln::Ref<MethodSymbol>> GetAllMethods();

	void FindEnumTypeAndValue(const ln::String& typeFullName, const ln::String& memberName, ln::Ref<TypeSymbol>* outEnum, ln::Ref<ConstantSymbol>* outMember);
	ln::Ref<ConstantSymbol> CreateConstantFromLiteralString(const ln::String& valueStr);

	void verify(ln::DiagnosticsManager* diag);

public:
	void InitializePredefineds();
	ln::Ref<TypeSymbol> findTypeInfo(ln::StringRef typeFullName);

private:
	ln::DiagnosticsManager* m_diag;
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
using ln::Ref<TypeSymbol> = std::shared_ptr<TypeSymbol>;
using ln::Ref<DocumentSymbol> = std::shared_ptr<DocumentSymbol>;
using ParameterDocumentInfoPtr = std::shared_ptr<ParameterDocumentSymbol>;
using ln::Ref<MetadataSymbol> = std::shared_ptr<MetadataSymbol>;
using ln::Ref<MethodSymbol> = std::shared_ptr<MethodSymbol>;
using ln::Ref<PropertySymbol> = std::shared_ptr<PropertySymbol>;
using ln::Ref<ConstantSymbol> = std::shared_ptr<ConstantSymbol>;

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
	ln::String	name;
	ln::String	io;
	ln::String	description;
};

class DocumentSymbol
{
public:
	ln::String							summary;
	ln::List<ParameterDocumentInfoPtr>	params;
	ln::String							returns;
	ln::String							details;
	ln::String							copydocMethodName;
	ln::String							copydocSignature;

	bool IsCopyDoc() const { return !copydocMethodName.isEmpty(); }
};

class MetadataSymbol
{
public:
	ln::String								name;
	std::unordered_map<ln::String, ln::String>	values;

	void AddValue(const ln::String& key, const ln::String& value);
	ln::String* FindValue(const ln::StringRef& key);
	bool HasKey(const ln::StringRef& key);
};


class ParameterSymbol
{
public:
	//DefaultValue
	//ParameterType
	ln::String	name;
	ln::Ref<TypeSymbol>	type;
	bool	isIn = false;
	bool	isOut = false;
	bool	isThis = false;
	bool	isReturn = false;
	ln::Ref<ConstantSymbol>		defaultValue;

	ln::String				typeRawName;
	Nullable<ln::String>	rawDefaultValue;
};
using ln::Ref<ParameterSymbol> = std::shared_ptr<ParameterSymbol>;

class FieldSymbol
{
public:
	ln::Ref<DocumentSymbol>	document;
	ln::Ref<TypeSymbol>		type;
	ln::String			name;

	ln::String	typeRawName;
};
using ln::Ref<FieldSymbol> = std::shared_ptr<FieldSymbol>;

class MethodSymbol
{
public:
	// 
	ln::Ref<TypeSymbol>		owner;
	ln::Ref<MetadataSymbol>	metadata;
	ln::Ref<DocumentSymbol>	document;
	AccessLevel		accessLevel = AccessLevel::Public;
	ln::String			name;
	ln::Ref<TypeSymbol>		returnType;
	//IsConstructor
	//IsStatic
	//IsVirtual
	bool			isConst = false;		// const メンバ関数であるか
	bool			isStatic = false;
	bool			isVirtual = false;
	bool			isConstructor = false;
	ln::Ref<PropertySymbol>	ownerProperty;
	ln::List<ln::Ref<ParameterSymbol>>	parameters;

	ln::String					overloadSuffix;
	ln::Ref<MethodSymbol>			overloadParent;
	ln::List<ln::Ref<MethodSymbol>>		overloadChildren;
	// 
	ln::List<ln::Ref<ParameterSymbol>>	capiParameters;

	ln::String	returnTypeRawName;
	ln::String	paramsRawSignature;		// 型名と引数名を抽出したもの (デフォルト引数は除く) e.g) "constVector3&minVec,constVector3&maxVec"

	bool IsOverloadChild() const { return overloadParent != nullptr; }
	bool IsRuntimeInitializer() const { return metadata->HasKey(_T("RuntimeInitializer")); }
	bool IsEventSetter() const { return metadata->HasKey(_T("Event")); }

	void LinkParameters();
	void ExpandCAPIParameters();
	ln::String GetCAPIFuncName();
	ln::String GetCApiSetOverrideCallbackFuncName();
	ln::String GetCApiSetOverrideCallbackTypeName();

	static ln::String GetAccessLevelName(AccessLevel accessLevel);
};


class PropertySymbol
{
public:
	ln::Ref<TypeSymbol>			owner;
	ln::Ref<DocumentSymbol>		document;
	ln::String				name;
	ln::String				namePrefix;	// Is
	ln::Ref<TypeSymbol>			type;
	ln::Ref<MethodSymbol>		getter;
	ln::Ref<MethodSymbol>		setter;

	void MakeDocument();
};

class ConstantSymbol
{
public:
	ln::Ref<DocumentSymbol>		document;
	ln::String				name;
	ln::Ref<TypeSymbol>			type;
	//Variant			value;

	ln::String				typeRawName;
};

class TypeSymbol
{
public:
	ln::Ref<MetadataSymbol>			metadata;
	ln::Ref<DocumentSymbol>			document;
	ln::String	name;
	bool	isStruct = false;
	bool			isVoid = false;
	bool				isPrimitive = false;
	bool					isEnum = false;
	bool					isDelegate = false;
	ln::List<ln::Ref<FieldSymbol>>		declaredFields;
	ln::List<ln::Ref<MethodSymbol>>		declaredMethods;
	ln::List<ln::Ref<PropertySymbol>>	declaredProperties;
	ln::List<ln::Ref<ConstantSymbol>>	declaredConstants;		// enum メンバ
	ln::List<ln::Ref<MethodSymbol>>		declaredMethodsForDocument;	// LN_METHOD(Docuent)
	ln::Ref<TypeSymbol>				baseClass;

	ln::String					baseClassRawName;

	TypeSymbol() {}
	TypeSymbol(ln::StringRef name_) : name(name_) {}

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
	static ln::Ref<TypeSymbol>	voidType;
	static ln::Ref<TypeSymbol>	nullptrType;
	static ln::Ref<TypeSymbol>	boolType;
	static ln::Ref<TypeSymbol>	intType;
	static ln::Ref<TypeSymbol>	uint32Type;
	static ln::Ref<TypeSymbol>	floatType;
	static ln::Ref<TypeSymbol>	stringType;
	static ln::Ref<TypeSymbol>	objectType;
	static ln::Ref<TypeSymbol>	EventConnectionType;
};

class SymbolDatabase
{
public:
	ln::List<ln::Ref<TypeSymbol>>	predefineds;
	ln::List<ln::Ref<TypeSymbol>>	structs;
	ln::List<ln::Ref<TypeSymbol>>	classes;
	ln::List<ln::Ref<TypeSymbol>>	enums;
	ln::List<ln::Ref<TypeSymbol>>	delegates;

	void Link();

	tr::Enumerator<ln::Ref<MethodSymbol>> GetAllMethods();

	void FindEnumTypeAndValue(const ln::String& typeName, const ln::String& memberName, ln::Ref<TypeSymbol>* outEnum, ln::Ref<ConstantSymbol>* outMember);
	ln::Ref<ConstantSymbol> CreateConstantFromLiteralString(const ln::String& valueStr);

public:
	void InitializePredefineds();
	ln::Ref<TypeSymbol> findTypeInfo(ln::StringRef typeName);
};

#endif
