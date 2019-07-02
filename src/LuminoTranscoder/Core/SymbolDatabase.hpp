
#pragma once
#include <memory>
#include <unordered_map>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include "Common.hpp"
#include "ParserIntermediates.hpp"

class SymbolDatabase;
class TypeSymbol;
class ConstantSymbol;
//class DocumentSymbol;
class MethodSymbol;
class PropertySymbol;


class ParameterDocumentInfo : public ln::RefObject
{
public:
	ln::Result init(PIParamDocument* pi);

	const ln::String& name() const { return m_pi->name; }
	const ln::String& io() const { return m_pi->io; }
	const ln::String& description() const { return m_pi->description; }

private:
	PIParamDocument* m_pi;
};

class DocumentInfo : public ln::RefObject
{
public:
	ln::Result init(PIDocument* pi);

	const ln::String& summary() const { return m_pi->summary; }
	const ln::String& returns() const { return m_pi->returns; }
	const ln::String& details() const { return m_pi->details; }
	const ln::List<Ref<ParameterDocumentInfo>>& params() const { return m_params; }

	//ln::String copydocMethodName;
	//ln::String copydocSignature;

	//bool IsCopyDoc() const { return !copydocMethodName.isEmpty(); }

private:
	PIDocument* m_pi = nullptr;
	ln::List<Ref<ParameterDocumentInfo>> m_params;
};

// 属性マクロの ( ) 内に記述されたパラメータ
class MetadataInfo : public ln::RefObject
{
public:
	static const ln::String OverloadPostfixAttr;

	ln::Result init(PIMetadata* pi);

	bool hasKey(const ln::StringRef& key) const;
	const ln::String* findValue(const ln::StringRef& key) const;
	ln::String getValue(const ln::StringRef& key, const ln::String& defaultValue = ln::String()) const;

private:
	PIMetadata* m_pi = nullptr;
	ln::String m_name;
	std::unordered_map<ln::String, ln::String> m_values;
};

// オーバーロードメソッドをまとめたもの
class MethodOverloadInfo : public ln::RefObject
{
public:
	MethodSymbol* representative() const { return m_methods[0]; }
	const ln::List<MethodSymbol*>& methods() const { return m_methods; }

private:
	ln::List<MethodSymbol*> m_methods;

	friend class TypeSymbol;
};

class Symbol : public ln::RefObject
{
public:
	SymbolDatabase* db() const { return m_db; }
	const Ref<DocumentInfo>& document() const { return m_document; }
	const Ref<MetadataInfo>& metadata() const { return m_metadata; }

protected:
	Symbol(SymbolDatabase* db);
	ln::Result init(PIDocument* document, PIMetadata* metadata);

private:
	SymbolDatabase* m_db;
	Ref<DocumentInfo> m_document;
	Ref<MetadataInfo> m_metadata;
};

//
//class ParameterDocumentSymbol : public ln::RefObject
//{
//public:
//	ln::String name;
//	ln::String io;
//	ln::String description;
//};
//



class FieldSymbol : public Symbol
{
public:
	//ln::Ref<DocumentSymbol> document;
	//ln::String name;

	//// parsing data (link source)
	//ln::String	typeRawName;

	TypeSymbol* type() const { return m_type; }
	const ln::String& name() const { return m_pi->name; }

public:
	FieldSymbol(SymbolDatabase* db);
	ln::Result init(PIField* pi);
	ln::Result link();

private:
	PIField* m_pi = nullptr;
	TypeSymbol* m_type = nullptr;
};

class ConstantSymbol : public Symbol
{
public:
	//ln::Ref<DocumentSymbol>		document;
	//ln::String				name;
	//ln::Ref<TypeSymbol>			type;
	//ln::Ref<ln::Variant>			value;

	//ln::String				typeRawName;

public:
	ConstantSymbol(SymbolDatabase* db);
	ln::Result init(PIConstant* pi);
	ln::Result link();

private:
	PIConstant* m_pi = nullptr;
};

class MethodParameterSymbol : public Symbol
{
public:
	struct SoueceData
	{
		ln::String typeRawName;
		ln::Optional<ln::String> rawDefaultValue;
	} src;

	ln::Ref<ConstantSymbol> defaultValue;

public:
	MethodParameterSymbol(SymbolDatabase* db);
	ln::Result init(PIMethodParameter* pi);
	ln::Result init(TypeSymbol* type, const ln::String& name);
	ln::Result link();

	TypeSymbol* type() const { return m_type; }
	const ln::String& name() const { return m_name; }

	bool isIn() const { return m_isIn; }
	bool isOut() const { return m_isOut; }
	bool isThis() const { return m_isThis; }		// for flat parameters
	bool isReturn() const { return m_isReturn; }	// for flat parameters

private:
	PIMethodParameter* m_pi = nullptr;
	TypeSymbol* m_type = nullptr;
	ln::String m_name;

	bool m_isIn = false;
	bool m_isOut = false;
	bool m_isThis = false;
	bool m_isReturn = false;

	friend class MethodSymbol;
};

class MethodSymbol : public Symbol
{
public:
	// 
	//ln::Ref<MetadataSymbol> metadata;
	//ln::Ref<DocumentSymbol> document;
	AccessLevel accessLevel = AccessLevel::Public;
	//ln::String name;
	//IsConstructor
	//IsStatic
	//IsVirtual
	//bool isConstructor = false;
	ln::Ref<PropertySymbol> ownerProperty;		// このメソッドがプロパティに含まれていればそのプロパティを指す

	// 
	//ln::List<ln::Ref<MethodParameterSymbol>> capiParameters;

	// parsing data
	ln::String	returnTypeRawName;
	ln::String	paramsRawSignature;		// 型名と引数名を抽出したもの (デフォルト引数は除く) e.g) "constVector3&minVec,constVector3&maxVec"

	//bool IsOverloadChild() const { return overloadParent != nullptr; }
	//bool IsRuntimeInitializer() const { return metadata->HasKey(_T("RuntimeInitializer")); }
	//bool IsEventSetter() const { return metadata->HasKey(_T("Event")); }

	//void LinkParameters(SymbolDatabase* db);
	//ln::String GetCAPIFuncName();
	//ln::String GetCApiSetOverrideCallbackFuncName();
	//ln::String GetCApiSetOverrideCallbackTypeName();

	//static ln::String GetAccessLevelName(AccessLevel accessLevel);

public:
	MethodSymbol(SymbolDatabase* db);
	ln::Result init(PIMethod* pi, TypeSymbol* ownerType);
	ln::Result link();

	TypeSymbol* ownerType() const { return m_ownerType; }
	TypeSymbol* returnType() const { return m_returnType; }
	const ln::String& shortName() const { return m_shortName; }
	const ln::String& fullName() const { return m_fullName; }
	const ln::List<Ref<MethodParameterSymbol>>& parameters() const { return m_parameters; }
	const ln::List<Ref<MethodParameterSymbol>>& flatParameters() const { return m_flatParameters; }
	MethodOverloadInfo* overloadInfo() const { return m_overloadInfo; }
	//MethodSymbol* overloadParent() const { return m_overloadParent; }
	//const ln::List<MethodSymbol*>& overloadChildren() const { return m_overloadChildren; }

	bool isConst() const { return m_pi->isConst; }
	bool isStatic() const { return m_pi->isStatic; }
	bool isVirtual() const { return m_pi->isVirtual; }
	bool isConstructor() const { return m_pi->isConstructor; }

private:
	ln::Result makeFlatParameters();

	PIMethod* m_pi = nullptr;
	TypeSymbol* m_ownerType = nullptr;
	TypeSymbol* m_returnType = nullptr;
	ln::String m_shortName;
	ln::String m_fullName;
	ln::List<Ref<MethodParameterSymbol>> m_parameters;
	ln::List<Ref<MethodParameterSymbol>> m_flatParameters;	// FlatC-API としてのパラメータリスト。先頭が this だったり、末尾が return だったりする。
	MethodOverloadInfo* m_overloadInfo = nullptr;		// このメソッドが属するオーバーロードグループ
	//MethodSymbol* m_overloadParent;			// このメソッドはどのメソッドをオーバーロードするか (基本的に一番最初に見つかった定義)
	//ln::List<MethodSymbol*> m_overloadChildren;	// このメソッドはどのメソッドにオーバーロードされるか

	friend class TypeSymbol;
};

class PropertySymbol : public ln::RefObject
{
public:
	ln::Ref<TypeSymbol>			owner;
	//ln::Ref<DocumentSymbol>		document;
	ln::String				name;
	ln::String				namePrefix;	// Is
	ln::Ref<TypeSymbol>			type;
	ln::Ref<MethodSymbol>		getter;
	ln::Ref<MethodSymbol>		setter;

	void MakeDocument();
};

class TypeSymbol : public Symbol
{
public:
	TypeSymbol(SymbolDatabase* db);
	ln::Result init(PITypeInfo* piType);
	ln::Result init(const ln::String& primitveRawFullName);
	ln::Result link();

	TypeKind kind() const { return (m_piType) ? m_piType->kind : TypeKind::Primitive; };
	const ln::String& fullName() const { return m_fullName; }
	const ln::String& shortName() const { return m_shortName; }
	const ln::List<Ref<FieldSymbol>>& fields() const { return m_fields; }
	const ln::List<Ref<ConstantSymbol>>& constants() const { return m_constants; }
	const ln::List<Ref<MethodSymbol>>& methods() const { return m_methods; }
	const ln::List<Ref<MethodOverloadInfo>>& overloads() const { return m_overloads; }
	TypeSymbol* baseClass() const { return m_baseClass; }

	bool isPrimitive() const { return kind() == TypeKind::Primitive; }
	bool isClass() const { return kind() == TypeKind::Class; }
	bool isStruct() const { return kind() == TypeKind::Struct; }
	bool isEnum() const { return kind() == TypeKind::Enum; }
	bool isStatic() const { return metadata()->hasKey(u"Static"); }	// static-class ?

private:
	void setFullName(const ln::String& value);
	ln::Result linkOverload();

	Ref<PITypeInfo> m_piType;
	ln::String m_fullName;
	ln::String m_shortName;
	ln::List<Ref<FieldSymbol>> m_fields;
	ln::List<Ref<ConstantSymbol>> m_constants;
	ln::List<Ref<MethodSymbol>> m_methods;
	ln::List<Ref<MethodOverloadInfo>> m_overloads;
	TypeSymbol* m_baseClass = nullptr;

//	struct SoueceData
//	{
//		ln::String baseClassRawName;
//		ln::String rawFullName;
//	} src;
//
//	ln::Ref<MetadataSymbol>			metadata;
//	ln::Ref<DocumentSymbol>			document;
//	bool	isStruct = false;
//	bool			isVoid = false;
//	bool				isPrimitive = false;
//	bool					isEnum = false;
//	bool					isDelegate = false;
//	ln::List<ln::Ref<FieldSymbol>>		declaredFields;
//	ln::List<ln::Ref<MethodSymbol>>		declaredMethods;
//	ln::List<ln::Ref<PropertySymbol>>	declaredProperties;
//	ln::List<ln::Ref<ConstantSymbol>>	declaredConstants;		// enum メンバ
//	ln::List<ln::Ref<MethodSymbol>>		declaredMethodsForDocument;	// LN_METHOD(Docuent)
//	ln::Ref<TypeSymbol>				baseClass;
//
//
//	TypeSymbol() {}
//	TypeSymbol(ln::StringRef rawFullName_) { setRawFullName(rawFullName_); }
//
//	const ln::String& fullName() const { return src.rawFullName; }
//	const ln::String& shortName() const { return m_shortName; }
//
//	bool isValueType() const { return isStruct || isPrimitive || isEnum; }
//	bool isStatic() const { return metadata->HasKey(_T("Static")); }
//	bool IsClass() const { return !isValueType() && !isVoid; }
//
//	void Link(SymbolDatabase* db);
//
//	void setRawFullName(const ln::String& value);
//
//private:
//	void MakeProperties();
//	void LinkOverload(SymbolDatabase* db);
//	void ResolveCopyDoc();
//
//	ln::String m_shortName;
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

class SymbolDatabase : public ln::RefObject
{
public:
	//ln::List<ln::Ref<TypeSymbol>>	structs;
	//ln::List<ln::Ref<TypeSymbol>>	classes;
	//ln::List<ln::Ref<TypeSymbol>>	enums;
	//ln::List<ln::Ref<TypeSymbol>>	delegates;

	SymbolDatabase(ln::DiagnosticsManager* diag);
	ln::DiagnosticsManager* diag() const { return m_diag; }
	ln::Result initTypes(PIDatabase* pidb);
	ln::Result linkTypes();

	//tr::Enumerator<ln::Ref<MethodSymbol>> GetAllMethods();

	//void FindEnumTypeAndValue(const ln::String& typeFullName, const ln::String& memberName, ln::Ref<TypeSymbol>* outEnum, ln::Ref<ConstantSymbol>* outMember);
	static Ref<ConstantSymbol> createConstantFromLiteralString(const ln::String& valueStr);

	//void verify(ln::DiagnosticsManager* diag);



	stream::Stream<Ref<TypeSymbol>> structs() const { return stream::MakeStream::from(m_allTypes) | stream::op::filter([](auto x) { return x->kind() == TypeKind::Struct; }); }
	stream::Stream<Ref<TypeSymbol>> classes() const { return stream::MakeStream::from(m_allTypes) | stream::op::filter([](auto x) { return x->kind() == TypeKind::Class; }); }

public:
	void initPredefineds();
	TypeSymbol* findTypeSymbol(const ln::String& typeFullName);

	// 型検索。見つからない場合はエラーをレポートして nullptr を返す。
	TypeSymbol* getTypeSymbol(const ln::String& typeFullName);


private:
	Ref<PIDatabase> m_pidb;
	ln::List<Ref<TypeSymbol>> m_allTypes;
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
