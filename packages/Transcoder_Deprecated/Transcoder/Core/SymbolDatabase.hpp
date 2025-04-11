
#pragma once
#include <memory>
#include <unordered_map>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include "Common.hpp"
#include "Project.hpp"
#include "../Parser/ParserIntermediates.hpp"

class SymbolDatabase;
class TypeSymbol;
class ConstantSymbol;
class MethodSymbol;
class PropertySymbol;

struct QualType {
    TypeSymbol* type = nullptr;
    bool strongReference = false;

    static bool equals(const QualType& hls, const QualType& rhs) {
        return hls.type == rhs.type &&
               hls.strongReference == rhs.strongReference;
    }
};

class ParameterDocumentInfo : public ln::RefObject {
public:
    ln::Result init(const ln::String& name, const ln::String& io, const ln::String& desc);
    ln::Result init(PIParamDocument* pi);

    const ln::String& name() const { return m_name; }
    const ln::String& io() const { return m_io; }
    const ln::String& description() const { return m_description; }

private:
    PIParamDocument* m_pi;
    ln::String m_name;
    ln::String m_io;
    ln::String m_description;
};

class DocumentInfo : public ln::RefObject {
public:
    ln::Result init(const PIDocument* pi);

    const ln::String& summary() const { return m_summary; }
    const ln::String& returns() const { return m_returns; }
    const ln::String& details() const { return m_details; }
    const ln::List<Ref<ParameterDocumentInfo>>& params() const { return m_params; }
    const ln::List<Ref<ParameterDocumentInfo>>& flatParams() const { return m_flatParams; }

    //ln::String copydocMethodName;
    //ln::String copydocSignature;

    //bool IsCopyDoc() const { return !copydocMethodName.isEmpty(); }

    void setSummary(const ln::String& value) { m_summary = value; }
    void setDetails(const ln::String& value) { m_details = value; }

private:
    //ln::Result makeFlatParameters();

    const PIDocument* m_pi = nullptr;
    ln::String m_summary;
    ln::String m_returns;
    ln::String m_details;
    ln::List<Ref<ParameterDocumentInfo>> m_params;
    ln::List<Ref<ParameterDocumentInfo>> m_flatParams;

    friend class MethodSymbol;
};

class PredefinedTypes {
public:
    static ln::Ref<TypeSymbol> voidType;
    static ln::Ref<TypeSymbol> nullptrType;
    static ln::Ref<TypeSymbol> boolType;
    static ln::Ref<TypeSymbol> intType;
    static ln::Ref<TypeSymbol> int16Type;
    static ln::Ref<TypeSymbol> uint32Type;
    static ln::Ref<TypeSymbol> intptrType;
    static ln::Ref<TypeSymbol> floatType;
    static ln::Ref<TypeSymbol> doubleType;
    static ln::Ref<TypeSymbol> stringType;
    static ln::Ref<TypeSymbol> StringViewType;
    //static ln::Ref<TypeSymbol>	objectType;
    //static ln::Ref<TypeSymbol>	EventConnectionType;
};

// 属性マクロの ( ) 内に記述されたパラメータ
class MetadataInfo : public ln::RefObject {
public:
    static const ln::String OverloadPostfixAttr;
    static const ln::String EventAttr;

    ln::Result init(PIMetadata* pi);

    bool hasKey(const ln::StringView& key) const;
    const ln::String* findValue(const ln::StringView& key) const;
    ln::String getValue(const ln::StringView& key, const ln::String& defaultValue = ln::String()) const;
    void setValue(const ln::StringView& key, const ln::String& value = ln::String());

private:
    PIMetadata* m_pi = nullptr;
    ln::String m_name;
    std::unordered_map<ln::String, ln::String> m_values;
};

// オーバーロードメソッドをまとめたもの
class MethodOverloadInfo : public ln::RefObject {
public:
    MethodSymbol* representative() const { return m_methods[m_representativeIndex]; }
    const ln::List<MethodSymbol*>& methods() const { return m_methods; }

private:
    int m_representativeIndex = -1;
    ln::List<MethodSymbol*> m_methods;

    friend class TypeSymbol;
};

class Symbol : public ln::RefObject {
public:
    SymbolDatabase* db() const { return m_db; }
    const Ref<DocumentInfo>& document() const { return m_document; }
    const Ref<MetadataInfo>& metadata() const { return m_metadata; }
    int symbolId() const { return m_symbolId; }

protected:
    Symbol(SymbolDatabase* db);
    ln::Result init();
    ln::Result init(const PIDocument* document, PIMetadata* metadata);

private:
    SymbolDatabase* m_db;
    Ref<DocumentInfo> m_document;
    Ref<MetadataInfo> m_metadata;
    int m_symbolId = 0;
};

class FieldSymbol : public Symbol {
public:
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

class ConstantSymbol : public Symbol {
public:
    //ln::Ref<DocumentSymbol>		document;

    TypeSymbol* type() const { return m_type; }
    const ln::String& name() const { return m_pi->name; }
    const Ref<ln::Variant>& value() const { return m_value; }

    //ln::Ref<TypeSymbol>			type;
    //ln::Ref<ln::Variant>			value;

    //ln::String				typeRawName;

public:
    ConstantSymbol(SymbolDatabase* db);
    ln::Result init(TypeSymbol* type, PIConstant* pi);
    ln::Result init(TypeSymbol* type, ln::Variant* value);
    ln::Result link();

private:
    PIConstant* m_pi = nullptr;
    TypeSymbol* m_type = nullptr;
    Ref<ln::Variant> m_value;
};

class MethodParameterSymbol : public Symbol {
    //public:
    //	struct SoueceData
    //	{
    //		ln::String typeRawName;
    //		ln::Optional<ln::String> rawDefaultValue;
    //	} src;
    //
    //
public:
    MethodParameterSymbol(SymbolDatabase* db);
    ln::Result init(PIMethodParameter* pi);
    ln::Result init(const QualType& qualType, const ln::String& name);
    ln::Result link();

    const TypeSymbol* type() const { return m_qualType.type; }
    const QualType& qualType() const { return m_qualType; }
    const ln::String& name() const { return m_name; }
    const ln::Ref<ConstantSymbol>& defaultValue() const { return m_defaultValue; }
    bool hasDefaultValue() const { return m_defaultValue != nullptr; }

    bool isIn() const { return m_isIn; }
    bool isOut() const { return m_isOut; }
    bool isThis() const { return m_isThis; }     // for flat parameters
    bool isReturn() const { return m_isReturn; } // for flat parameters

    void setIO(bool in, bool out) {
        m_isIn = in;
        m_isOut = out;
    }

    ln::String getFullQualTypeName() const;
    int flatParamIndex() const { return m_flatParamIndex; }

private:
    PIMethodParameter* m_pi = nullptr;
    //TypeSymbol* m_type = nullptr;
    QualType m_qualType;
    ln::String m_name;
    ln::Ref<ConstantSymbol> m_defaultValue;
    int m_flatParamIndex = -1; // m_flatParameters の index. インスタンスメソッドでは [0] が this, 戻り値があれば [last] が return になったりする。

    bool m_isIn = false;
    bool m_isOut = false;
    bool m_isThis = false;
    bool m_isReturn = false;

    friend class MethodSymbol;
};

class MethodSymbol : public Symbol {
public:
    //
    //ln::List<ln::Ref<MethodParameterSymbol>> capiParameters;

    // parsing data
    ln::String returnTypeRawName;
    ln::String paramsRawSignature; // 型名と引数名を抽出したもの (デフォルト引数は除く) e.g) "constVector3&minVec,constVector3&maxVec"

    //bool IsOverloadChild() const { return overloadParent != nullptr; }
    //bool IsRuntimeInitializer() const { return metadata->HasKey(_T("RuntimeInitializer")); }

    //void LinkParameters(SymbolDatabase* db);
    //ln::String GetCAPIFuncName();
    //ln::String GetCApiSetOverrideCallbackFuncName();
    //ln::String GetCApiSetOverrideCallbackTypeName();

    //static ln::String GetAccessLevelName(AccessLevel accessLevel);

public:
    MethodSymbol(SymbolDatabase* db);
    ln::Result init(PIMethod* pi, TypeSymbol* ownerType);
    ln::Result init(TypeSymbol* ownerType, const ln::String& shortName, const QualType& returnType, const ln::List<Ref<MethodParameterSymbol>>& params);
    ln::Result initAsVirtualPrototype(TypeSymbol* ownerType, MethodSymbol* sourceMethod, TypeSymbol* delegateType);
    ln::Result link();

    AccessLevel accessLevel() const { return m_accessLevel; }
    TypeSymbol* ownerType() const { return m_ownerType; }
    const QualType& returnType() const { return m_returnType; }
    const ln::String& shortName() const { return m_shortName; }
    const ln::String& fullName() const { return m_fullName; }
    const ln::List<Ref<MethodParameterSymbol>>& parameters() const { return m_parameters; }
    const ln::List<Ref<MethodParameterSymbol>>& flatParameters() const { return m_flatParameters; }
    MethodParameterSymbol* flatThisParam() const { return !isStatic() ? m_flatParameters.front() : nullptr; }
    MethodParameterSymbol* flatReturnParam() const { return hasReturnType() ? m_flatParameters.back() : nullptr; }
    MethodParameterSymbol* flatConstructorOutputThisParam() const { return isConstructor() ? m_flatParameters.back() : nullptr; }
    const MethodParameterSymbol* findFlatParameter(const ln::StringView& name) const;
    MethodOverloadInfo* overloadInfo() const { return m_overloadInfo; }
    PropertySymbol* ownerProperty() const { return m_ownerProperty; }
    ln::String overloadPostfix() const { return metadata()->getValue(MetadataInfo::OverloadPostfixAttr, ln::String::Empty); }
    //MethodSymbol* overloadParent() const { return m_overloadParent; }
    //const ln::List<MethodSymbol*>& overloadChildren() const { return m_overloadChildren; }

    bool isConst() const { return m_isConst; }
    bool isStatic() const { return m_isStatic; }
    bool isVirtual() const { return m_isVirtual; }
    bool isConstructor() const { return m_isConstructor; } // 名前が init であるインスタンスメソッド
    bool isInstance() const { return !isStatic(); }        // instance method
    //bool isEventConnector() const { return metadata()->hasKey(MetadataInfo::EventAttr); }
    bool isPropertyGetter() const { return m_ownerProperty && m_parameters.isEmpty(); }
    bool isPropertySetter() const { return m_ownerProperty && !m_parameters.isEmpty(); }
    bool isCollectionGetItem() const { return metadata()->hasKey(U"Collection_GetItem"); } // AccessorCache を使うときにインデックス指定するものであるかどうか
    bool isRuntimeInitializer() const { return metadata()->hasKey(U"RuntimeInitializer"); }
    bool isSpecialized() const { return metadata()->hasKey(U"Specialized"); } // FlatAPI 定義を独自に作成しているかどうか。他言語側の BindingIF を作るとき、Object::release 等だけ特別扱いしたくないので用意したフラグ。

    bool hasReturnType() const { return m_returnType.type != PredefinedTypes::voidType; }
    bool hasStringDecl() const { return m_hasStringDecl; } // いずれかの引数、戻り値に文字列型が含まれているか

    bool isFieldAccessor() const { return m_linkedField != nullptr; }
    FieldSymbol* linkedField() const { return m_linkedField; }

    bool isVirtualPototypeSetter() const { return m_virtualPototypeDelegate != nullptr; }
    MethodSymbol* virtualPototypeSouceMethod() const { return m_virtualPototypeSouceMethod; }
    TypeSymbol* virtualPototypeDelegate() const { return m_virtualPototypeDelegate; }

private:
    ln::Result makeFlatParameters();

    PIMethod* m_pi = nullptr;
    AccessLevel m_accessLevel = AccessLevel::Public;
    TypeSymbol* m_ownerType = nullptr;
    //TypeSymbol* m_returnType = nullptr;
    QualType m_returnType;
    ln::String m_shortName;
    ln::String m_fullName;
    ln::List<Ref<MethodParameterSymbol>> m_parameters;
    ln::List<Ref<MethodParameterSymbol>> m_flatParameters; // FlatC-API としてのパラメータリスト。先頭が this だったり、末尾が return だったりする。
    MethodOverloadInfo* m_overloadInfo = nullptr;          // このメソッドが属するオーバーロードグループ
    PropertySymbol* m_ownerProperty = nullptr;
    FieldSymbol* m_linkedField = nullptr;
    MethodSymbol* m_virtualPototypeSouceMethod = nullptr;
    TypeSymbol* m_virtualPototypeDelegate = nullptr;

    bool m_isConst = false;
    bool m_isStatic = false;
    bool m_isVirtual = false;
    bool m_isConstructor = false;
    bool m_hasStringDecl = false;

    friend class TypeSymbol;
    friend class SymbolDatabase;
};

class PropertySymbol : public Symbol {
    //public:
    //	ln::Ref<TypeSymbol>			owner;
    //	//ln::Ref<DocumentSymbol>		document;
    //	ln::String				name;
    //	ln::String				namePrefix;	// Is
    //	ln::Ref<TypeSymbol>			type;
    //	ln::Ref<MethodSymbol>		getter;
    //	ln::Ref<MethodSymbol>		setter;
    //
    //	void MakeDocument();
public:
    PropertySymbol(SymbolDatabase* db);
    ln::Result init(const ln::String& shortName);

    const ln::String& shortName() const { return m_shortName; }
    const ln::Ref<TypeSymbol>& type() const { return m_type; }
    const ln::Ref<MethodSymbol>& getter() const { return m_getter; }
    const ln::Ref<MethodSymbol>& setter() const { return m_setter; }
    const ln::String namePrefix() const { return m_namePrefix; }

private:
    void buildDocument();

    ln::String m_shortName;
    ln::Ref<TypeSymbol> m_type;
    ln::Ref<MethodSymbol> m_getter;
    ln::Ref<MethodSymbol> m_setter;
    ln::String m_namePrefix; // Is

    friend class TypeSymbol;
};

class DependencyHelper {
public:
    static int calcDepth(TypeSymbol* type, int depth = 0);
};

class TypeSymbol : public Symbol {
public:
    TypeSymbol(SymbolDatabase* db);
    ln::Result init(Module* module, PITypeInfo* piType);
    ln::Result init(const ln::String& primitveRawFullName, TypeKind typeKind, TypeClass typeClass);
    ln::Result initAsFunctionType(const ln::String& fullName, MethodSymbol* signeture);                                           // delegate のコンストラクタに指定する関数ポインタ型を作るために使う
    ln::Result initAsVirtualDelegate(const ln::String& fullName, MethodSymbol* signeture, TypeSymbol* virtualMethodDefinedClass); // 仮想関数コールバック用の Delegate を内部生成する時に使う
    ln::Result link();

    TypeKind kind() const { return m_kind; } //{ return (m_piType) ? m_piType->kindAsEnum() : TypeKind::Primitive; };
    TypeClass typeClass() const { return m_typeClass; }
    const ln::String& fullName() const { return m_fullName; }
    const ln::String& shortName() const { return m_shortName; }
    const ln::List<Ref<FieldSymbol>>& fields() const { return m_fields; }
    const ln::List<Ref<ConstantSymbol>>& constants() const { return m_constants; }
    const ln::List<Ref<MethodSymbol>>& publicMethods() const { return m_publicMethods; } // クラス外から普通にコールできる public メソッド。virutal は含むが、protected virtual は含まない。
    const ln::List<Ref<MethodOverloadInfo>>& overloads() const { return m_overloads; }
    const ln::List<Ref<PropertySymbol>>& properties() const { return m_properties; }
    const ln::List<Ref<MethodSymbol>>& leafVirtualMethods() const { return m_leafVirtualMethods; } // ベースクラスも含めた、すべての末端レベル virtual method
    const ln::List<Ref<MethodSymbol>>& virtualMethods() const { return m_virtualMethods; }         // このクラスで定義されている仮想関数。isVirtual()==true であるもの
    const ln::List<Ref<MethodSymbol>>& virtualPrototypeSetters() const { return m_vitualPrototypeSetters; }
    //const ln::List<Ref<MethodSymbol>>& eventMethods() const { return m_eventMethods; }
    Module* module() const { return m_module; }
    TypeSymbol* baseClass() const { return m_baseClass; }
    TypeSymbol* collectionItemType() const { return m_collectionItemType; }
    MethodSymbol* delegateProtoType() const { return m_functionSignature; }
    int inheritanceDepth() const { return m_inheritanceDepth; }

    bool isPrimitive() const { return kind() == TypeKind::Primitive; }
    bool isClass() const { return kind() == TypeKind::Class; }
    bool isStruct() const { return kind() == TypeKind::Struct; }
    bool isEnum() const { return kind() == TypeKind::Enum; }
    bool isFunction() const { return kind() == TypeKind::Function; }
    //bool isDelegate() const { return kind() == TypeKind::Delegate && !isDelegateObject(); }	// deprecated
    bool isStatic() const { return metadata() ? metadata()->hasKey(U"Static") : false; } // static-class

    bool isRootObjectClass() const { return m_fullName == U"ln::Object"; }
    bool isString() const { return this == PredefinedTypes::stringType || this == PredefinedTypes::StringViewType; }
    bool isCollection() const { return metadata()->hasKey(U"Collection"); }
    bool isFlags() const { return metadata()->hasKey(U"Flags"); }
    bool isDelegateObject() const { return typeClass() == TypeClass::DelegateObject; }
    bool isPromise() const { return typeClass() == TypeClass::Promise; }

    // LNHandle として扱うものかどうか
    bool isObjectGroup() const { return isClass() || isDelegateObject(); }
    bool isVirtualHandlerDelegate() const { return m_virtualDelegateType; }

    bool createOverridePrototype(MethodSymbol* sourceMethod, TypeSymbol* delegateType);

    int m_inheritanceDepth = 0;
private:
    void setFullName(const ln::String& value);
    ln::Result linkOverload();
    ln::Result linkProperties();
    ln::Result createSpecialSymbols();
    void collectLeafVirtualMethods(ln::List<Ref<MethodSymbol>>* virtualMethods);
    bool addDelegateConstructorMethod();

    Ref<PITypeInfo> m_piType;
    TypeKind m_kind = TypeKind::Primitive;
    TypeClass m_typeClass = TypeClass::None;
    ln::String m_fullName;
    ln::String m_shortName;
    Ref<MethodSymbol> m_functionSignature; // Delegate<> の型引数部分やFunction 型のシグネチャを表す。 e.g.) "void(ZVTestEventArgs1* e)"
    ln::List<Ref<FieldSymbol>> m_fields;
    ln::List<Ref<ConstantSymbol>> m_constants;
    ln::List<Ref<MethodSymbol>> m_publicMethods;
    ln::List<Ref<MethodSymbol>> m_declaredMethods; // このクラス内で宣言されたすべてメソッド。ベースクラスは含まない。
    ln::List<Ref<MethodOverloadInfo>> m_overloads;
    ln::List<Ref<PropertySymbol>> m_properties;
    ln::List<Ref<MethodSymbol>> m_leafVirtualMethods;
    ln::List<Ref<MethodSymbol>> m_virtualMethods;
    ln::List<Ref<MethodSymbol>> m_vitualPrototypeSetters;

    //ln::List<Ref<MethodSymbol>> m_eventMethods;
    Module* m_module;
    TypeSymbol* m_baseClass = nullptr;
    TypeSymbol* m_collectionItemType = nullptr;

    bool m_virtualDelegateType = false;
    // Note: prototype は delegate にする必要ある？生の関数ポインタでいいのでは？
    //
    // Delegate を使うのは、Lumino 内で Delegate が要求されているから。
    // Lumino 内では、std::function のように static 関数とメンバ関数の区別なくコールバックを実装するために使っている。
    // それだけなら std::function でもよいのだが、Binding に公開するインターフェイスとしては「参照カウントを持った std::function」が無いと都合が悪い。
    // 代表的なところだと、すべてが動的な Ruby の関数オブジェクトをラップして、Lumino 無いから呼び出せるようにするのに使う。
    // もし Delegate が無い場合、std::function と Proc を対応付ける他の仕組みが必要になるが、
    // 関連するオブジェクトがかなり離れたところで管理されることになるので、プログラムが複雑になる。
    // 寿命管理として正しく動作するのかも未検証。
    //
    // それで prototype に話を戻すと、こちらは static 関数のみでかまわない。
    //
    // もし Lumino が対応していない言語で使いたい、となった時は、FlatAPI を直叩きするか、ラップしたクラスライブラリを使うことになる。
    // で、FlatAPI を直叩きのときでも Delegate はそのままでは使えず、Delegate のコンストラクタに static なコールバックを登録し、
    // そこから Managed 側のメンバ関数などをコールする仕組みを作らなければならない。
    //
    // また、porototype はオブジェクト志向の仕組みを持たない言語でにおいて、仮想関数のオーバーライドを実現する仕組みである。
    // そのため C# などオブジェクト指向の仕組みを持つ言語向けのクラスライブラリを作るときは、SetPrototype ではなくて SetOverrideCallback を使うべき。
    // そもそも prototype は overrideCallback よりもメモリ消費が大きい。
    //
    // で、オブジェクト指向の仕組みを持たない言語の多くは「メンバ関数」という考え方もあんまり無くて、
    // そういう意味で全部 static 関数と考えてもほとんど問題ない。
    // ちなみに、対応しようとしてるそんな感じの言語は C と HSP.
    //
    // ...と思ったけど、やっぱり Delegate が必要そうだった。
    // HSP で実装するとき、結局 FlatAPI に登録する関数ポインタと、Labelポインタの対応を覚えておかないとならない。
    // Labelポインタは prototype 関数単位で覚えておく必要があるので、Delegate と全く同じ仕組みが必要になる。

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
    //	TypeSymbol(ln::StringView rawFullName_) { setRawFullName(rawFullName_); }
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

class SymbolDatabase : public ln::RefObject {
public:
    //ln::List<ln::Ref<TypeSymbol>>	structs;
    //ln::List<ln::Ref<TypeSymbol>>	classes;
    //ln::List<ln::Ref<TypeSymbol>>	enums;
    //ln::List<ln::Ref<TypeSymbol>>	delegates;

    SymbolDatabase(ln::DiagnosticsManager* diag);
    ln::DiagnosticsManager* diag() const { return m_diag; }
    ln::Result initTypes(Project* project, PIDatabase* pidb);
    ln::Result linkTypes();

    //tr::Enumerator<ln::Ref<MethodSymbol>> GetAllMethods();

    //void FindEnumTypeAndValue(const ln::String& typeFullName, const ln::String& memberName, ln::Ref<TypeSymbol>* outEnum, ln::Ref<ConstantSymbol>* outMember);
    static Ref<ConstantSymbol> createConstantFromLiteralString(const ln::String& valueStr);

    //void verify(ln::DiagnosticsManager* diag);

    TypeSymbol* rootObjectClass() const { return m_rootObjectClass; }
    void setRootObjectClass(TypeSymbol* value) { m_rootObjectClass = value; }

    ln::Array<Ref<TypeSymbol>> enums() const {
        return m_allTypes.filter([](auto x) { return x->kind() == TypeKind::Enum; });
    }
    ln::Array<Ref<TypeSymbol>> structs() const {
        return m_allTypes.filter([](auto x) { return x->kind() == TypeKind::Struct; });
    }
    ln::Array<Ref<TypeSymbol>> classes() const {
        return m_allTypes.filter([](auto x) { return x->kind() == TypeKind::Class /* && (x->isRootObjectClass())*/; });
    }
    ln::Array<Ref<TypeSymbol>> delegateObjects() const {
        return m_allTypes.filter([](auto x) { return x->isDelegateObject(); });
    }

    const Ref<PIDatabase>& pidb() const { return m_pidb; }
    const PIDocument* resolveCopyDoc(const PIDocument* pi) const;
    int generateSymbolId() { return m_nextSymbolId++; }

public:
    void initPredefineds();
    TypeSymbol* findTypeSymbol(const ln::String& typeFullName) const;
    TypeSymbol* findDelegateObjectFromSigneture(const QualType& returnType, const ln::List<QualType>& paramTypes) const;

    // 型検索。見つからない場合はエラーをレポートして nullptr を返す。
    TypeSymbol* getTypeSymbol(const ln::String& typeFullName) const;

    QualType parseQualType(const ln::String& rawTypeName) const;

    void registerTypeSymbol(TypeSymbol* type);

private:
    Ref<PIDatabase> m_pidb;
    ln::Array<Ref<TypeSymbol>> m_allTypes;
    ln::DiagnosticsManager* m_diag;
    TypeSymbol* m_rootObjectClass = nullptr;
    int m_nextSymbolId = 1;
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
	ln::String* FindValue(const ln::StringView& key);
	bool HasKey(const ln::StringView& key);
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
	TypeSymbol(ln::StringView name_) : name(name_) {}

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
	ln::Ref<TypeSymbol> findTypeInfo(ln::StringView typeName);
};

#endif
