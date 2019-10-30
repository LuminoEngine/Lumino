
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

protected:
	LN_SERIALIZE_CLASS_VERSION(1);
	void serialize(ln::Archive& ar)
	{
		ar & LN_NVP(name);
		ar & LN_NVP(io);
		ar & LN_NVP(description);
	}
};

// Doxygen コメント
class PIDocument : public ln::RefObject
{
public:
    ln::String summary;
    ln::List<Ref<PIParamDocument>> params;
    ln::String returns;
    ln::String details;
	ln::List<ln::String> copydocLocalSignature;

	static ln::String formatComment(const ln::String& comment);

protected:
	LN_SERIALIZE_CLASS_VERSION(1);
	void serialize(ln::Archive& ar)
	{
		ar & LN_NVP(summary);
		ar & LN_NVP(params);
		ar & LN_NVP(returns);
		ar & LN_NVP(details);
		ar & LN_NVP(copydocLocalSignature);
	}
};

// LN_METHOD() などの属性マクロの ( ) 内に記述されたパラメータ
class PIMetadata : public ln::RefObject
{
public:
	ln::String name;
	std::unordered_map<ln::String, ln::String> values;

protected:
	LN_SERIALIZE_CLASS_VERSION(1);
	void serialize(ln::Archive& ar)
	{
		ar & LN_NVP(name);
		ar & LN_NVP(values);
	}
};

// フィールド変数
class PIField : public ln::RefObject
{
public:
	Ref<PIDocument> document;
	Ref<PIMetadata> metadata;
	ln::String typeRawName;
	ln::String name;

protected:
	LN_SERIALIZE_CLASS_VERSION(1);
	void serialize(ln::Archive& ar)
	{
		ar & LN_NVP(document);
		ar & LN_NVP(metadata);
		ar & LN_NVP(typeRawName);
		ar & LN_NVP(name);
	}
};

// 定数 (enum メンバ)
class PIConstant : public ln::RefObject
{
public:
	Ref<PIDocument> document;
	Ref<PIMetadata> metadata;
	ln::String typeRawName;
	ln::String name;
	ln::Ref<ln::Variant> value;

protected:
	LN_SERIALIZE_CLASS_VERSION(1);
	void serialize(ln::Archive& ar)
	{
		ar & LN_NVP(document);
		ar & LN_NVP(metadata);
		ar & LN_NVP(typeRawName);
		ar & LN_NVP(name);
		ar & LN_NVP(value);
	}
};

// メソッドの引数情報
class PIMethodParameter : public ln::RefObject
{
public:
	ln::String typeRawName;
	ln::String name;
	bool isConst = false;
	bool isPointer = false;
	Ref<ln::Variant> defaultValue;

protected:
	LN_SERIALIZE_CLASS_VERSION(1);
	void serialize(ln::Archive& ar)
	{
		ar & LN_NVP(typeRawName);
		ar & LN_NVP(name);
		ar & LN_NVP(isConst);
		ar & LN_NVP(isPointer);
		ar & LN_NVP(defaultValue);
	}
};

// メソッド情報
class PIMethod : public ln::RefObject
{
public:
	//PITypeInfo* owner;
	Ref<PIDocument> document;
	Ref<PIMetadata> metadata;
	ln::String accessLevel;	// "public", "protected", "private"
	ln::String name;
	bool isConst = false;
	bool isStatic = false;
	bool isVirtual = false;
	bool isConstructor = false;
	ln::String returnTypeRawName;
	ln::List<Ref<PIMethodParameter>> parameters;

	AccessLevel accessLevelAsEnum() const;

protected:
	LN_SERIALIZE_CLASS_VERSION(1);
	void serialize(ln::Archive& ar)
	{
		ar & LN_NVP(document);
		ar & LN_NVP(metadata);
		ar & LN_NVP(accessLevel);
		ar & LN_NVP(name);
		ar & LN_NVP(isConst);
		ar & LN_NVP(isStatic);
		ar & LN_NVP(isVirtual);
		ar & LN_NVP(isConstructor);
		ar & LN_NVP(returnTypeRawName);
		ar & LN_NVP(parameters);
	}
};

// 型情報 (struct, class, enum)
class PITypeInfo : public ln::RefObject
{
public:
	//TypeKind kind;
	ln::String kind;
    ln::String rawFullName;
	ln::String baseClassRawName;
    Ref<PIDocument> document;
    Ref<PIMetadata> metadata;
	ln::List<Ref<PIField>> fields;
	ln::List<Ref<PIConstant>> constants;
	ln::List<Ref<PIMethod>> methods;

	TypeKind kindAsEnum() const;

protected:
	LN_SERIALIZE_CLASS_VERSION(1);
	void serialize(ln::Archive& ar)
	{
		ar & LN_NVP(kind);
		ar & LN_NVP(rawFullName);
		ar & LN_NVP(baseClassRawName);
		ar & LN_NVP(document);
		ar & LN_NVP(metadata);
		ar & LN_NVP(fields);
		ar & LN_NVP(constants);
		ar & LN_NVP(methods);
	}
};

// Type-alias で定義された関数オブジェクト
class PIDelegate : public ln::RefObject
{
public:
	Ref<PIDocument> document;
	ln::String name;
	ln::String returnTypeRawName;
	ln::List<Ref<PIMethodParameter>> parameters;

private:
	LN_SERIALIZE_CLASS_VERSION(1);
	void serialize(ln::Archive& ar)
	{
		ar & LN_NVP(document);
		ar & LN_NVP(name);
		ar & LN_NVP(returnTypeRawName);
		ar & LN_NVP(parameters);
	}
};

//class PIRelativeDocument : public ln::RefObject
//{
//public:
//	Ref<PIDocument> document;
//	ln::List<ln::String> localSigneture;
//};

// 型情報データベース
class PIDatabase : public ln::RefObject
{
public:
	ln::List<Ref<PITypeInfo>> types;
	ln::List<Ref<PIDelegate>> delegates;
	ln::List<Ref<PIDocument>> relativeDocuments;	// マクロ修飾されていないが、Doxygen comment を持っている関数の情報。copydoc で使う。
	//ln::List<Ref<PIRelativeDocument>> releativeDocuments;	// マクロ修飾されていないが、Doxygen comment を持っている関数の情報。copydoc で使う。

	PIDocument* findRelativeDocument(const ln::List<ln::String>& localSignature);

	void clear();
	void save(const ln::Path& filePath);
	void load(const ln::Path& filePath);

	static ln::List<ln::String> parseLocalSigneture(const ln::String& signeture, const ln::String& methodName = ln::String::Empty);

protected:
	LN_SERIALIZE_CLASS_VERSION(1);
	void serialize(ln::Archive& ar)
	{
		ar & LN_NVP(types);
		ar & LN_NVP(delegates);
		ar & LN_NVP(relativeDocuments);
	}
};
