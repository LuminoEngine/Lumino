
#pragma once
#include "Common.hpp"

class PITypeInfo;

// Doxygen コメント (@param)
class PIParamDocument : public ln::RefObject
{
public:
    ln::String name;
    ln::String io;
    ln::String description;
};

// Doxygen コメント
class PIDocument : public ln::RefObject
{
public:
    ln::String summary;
    ln::List<Ref<PIParamDocument>> params;
    ln::String returns;
    ln::String details;
    ln::String copydocMethodName;
    ln::String copydocSignature;
};

// LN_METHOD() などの属性マクロの ( ) 内に記述されたパラメータ
class PIMetadata : public ln::RefObject
{
public:
	ln::String name;
	std::unordered_map<ln::String, ln::String> values;
};

// フィールド変数
class PIField : public ln::RefObject
{
public:
	Ref<PIDocument> document;
	Ref<PIMetadata> metadata;
	ln::String typeRawName;
	ln::String name;
};

// 定数 (enum メンバやデフォルト引数)
class PIConstant : public ln::RefObject
{
public:
	Ref<PIDocument> document;
	Ref<PIMetadata> metadata;
	ln::String typeRawName;
	ln::String name;
	ln::Ref<ln::Variant> value;
};

// メソッドの引数情報
class PIMethodParameter : public ln::RefObject
{
public:
	ln::String typeRawName;
	ln::String name;
	bool isIn = false;
	bool isOut = false;
	bool isThis = false;
	bool isReturn = false;
};

// メソッド情報
class PIMethod : public ln::RefObject
{
public:
	PITypeInfo* owner;
	Ref<PIDocument> document;
	Ref<PIMetadata> metadata;
	AccessLevel accessLevel = AccessLevel::Private;
	ln::String name;
	bool isConst = false;
	bool isStatic = false;
	bool isVirtual = false;
	bool isConstructor = false;
	ln::String returnTypeRawName;
	ln::List<Ref<PIMethodParameter>> parameters;
};

// 型情報 (struct, class, enum)
class PITypeInfo : public ln::RefObject
{
public:
	TypeKind kind;
    ln::String rawFullName;
	ln::String baseClassRawName;
    Ref<PIDocument> document;
    Ref<PIMetadata> metadata;
	ln::List<Ref<PIField>> fields;
	ln::List<Ref<PIConstant>> constants;
	ln::List<Ref<PIMethod>> methods;
};

// 型情報データベース
class PIDatabase : public ln::RefObject
{
public:
	ln::List<Ref<PITypeInfo>> types;
};
