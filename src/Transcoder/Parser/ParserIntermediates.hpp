
#pragma once

template<class T>
using Ref = ln::Ref<T>;
class PITypeInfo;

// Doxygen コメント (@param)
class PIParamDocument : public ln::RefObject {
public:
    ln::String name;
    ln::String io;
    ln::String description;

protected:
    LN_SERIALIZE_CLASS_VERSION(1);
    void serialize(ln::Archive& ar) {
        ar& LN_NVP(name);
        ar& LN_NVP(io);
        ar& LN_NVP(description);
    }
};

// Doxygen コメント
class PIDocument : public ln::RefObject {
public:
    ln::String summary;
    ln::List<Ref<PIParamDocument>> params;
    ln::String returns;
    ln::String details;
    ln::List<ln::String> copydocLocalSignature;

    static ln::String formatComment(const ln::String& comment);
    static bool equalsLocalSigneture(const PIDocument* lhs, const PIDocument* rhs);

protected:
    LN_SERIALIZE_CLASS_VERSION(1);
    void serialize(ln::Archive& ar) {
        ar& LN_NVP(summary);
        ar& LN_NVP(params);
        ar& LN_NVP(returns);
        ar& LN_NVP(details);
        ar& LN_NVP(copydocLocalSignature);
    }
};

// LN_METHOD() などの属性マクロの ( ) 内に記述されたパラメータ
class PIMetadata : public ln::RefObject {
public:
    ln::String name;
    std::unordered_map<ln::String, ln::String> values;

protected:
    LN_SERIALIZE_CLASS_VERSION(1);
    void serialize(ln::Archive& ar) {
        ar& LN_NVP(name);
        ar& LN_NVP(values);
    }
};

// フィールド変数
class PIField : public ln::RefObject {
public:
    Ref<PIDocument> document;
    Ref<PIMetadata> metadata;
    ln::String typeRawName;
    ln::String name;

protected:
    LN_SERIALIZE_CLASS_VERSION(1);
    void serialize(ln::Archive& ar) {
        ar& LN_NVP(document);
        ar& LN_NVP(metadata);
        ar& LN_NVP(typeRawName);
        ar& LN_NVP(name);
    }
};

// 定数 (enum メンバ)
class PIConstant : public ln::RefObject {
public:
    Ref<PIDocument> document;
    Ref<PIMetadata> metadata;
    ln::String typeRawName;
    ln::String name;
    ln::Ref<ln::Variant> value;

protected:
    LN_SERIALIZE_CLASS_VERSION(1);
    void serialize(ln::Archive& ar) {
        ar& LN_NVP(document);
        ar& LN_NVP(metadata);
        ar& LN_NVP(typeRawName);
        ar& LN_NVP(name);
        ar& LN_NVP(value);
    }
};

// メソッドの引数情報
class PIMethodParameter : public ln::RefObject {
public:
    ln::String typeRawName;
    ln::String name;
    bool isConst = false;
    bool isPointer = false;
    Ref<ln::Variant> defaultValue;

protected:
    LN_SERIALIZE_CLASS_VERSION(1);
    void serialize(ln::Archive& ar) {
        ar& LN_NVP(typeRawName);
        ar& LN_NVP(name);
        ar& LN_NVP(isConst);
        ar& LN_NVP(isPointer);

        bool hasDefaultValue = (defaultValue != nullptr);
        ar& LN_NVP(hasDefaultValue);
        ar& LN_NVP(defaultValue);

        if (ar.isLoading()) {
            if (hasDefaultValue && !defaultValue) {
                defaultValue = ln::makeVariant();
            }
        }
    }
};

// メソッド情報
class PIMethod : public ln::RefObject {
public:
    Ref<PIDocument> document;
    Ref<PIMetadata> metadata;
    ln::String accessLevel; // "public", "protected", "private"
    ln::String name;
    bool isConst = false;
    bool isStatic = false;
    bool isVirtual = false;
    bool isConstructor = false;
    ln::String returnTypeRawName;
    ln::List<Ref<PIMethodParameter>> parameters;

protected:
    LN_SERIALIZE_CLASS_VERSION(1);
    void serialize(ln::Archive& ar) {
        ar& LN_NVP(document);
        ar& LN_NVP(metadata);
        ar& LN_NVP(accessLevel);
        ar& LN_NVP(name);
        ar& LN_NVP(isConst);
        ar& LN_NVP(isStatic);
        ar& LN_NVP(isVirtual);
        ar& LN_NVP(isConstructor);
        ar& LN_NVP(returnTypeRawName);
        ar& LN_NVP(parameters);
    }
};

class PITemplateArgument : public ln::RefObject {
public:
    ln::String paramName;
    ln::String typeRawName;
    bool isConst = false;
    bool isPointer = false;

protected:
    LN_SERIALIZE_CLASS_VERSION(1);
    void serialize(ln::Archive& ar) {
        ar& LN_NVP(paramName);
        ar& LN_NVP(typeRawName);
        ar& LN_NVP(isConst);
        ar& LN_NVP(isPointer);
    }
};

// 型情報 (struct, class, enum)
// template についてはインスタンス化されたもののみ扱う
class PITypeInfo : public ln::RefObject {
public:
    ln::String kind;
    ln::String rawFullName;
    ln::String baseClassRawName;
    Ref<PIDocument> document;
    Ref<PIMetadata> metadata;
    Ref<PIMethod> delegateProtoType;
    ln::List<Ref<PITemplateArgument>> templateArguments;
    ln::List<Ref<PIField>> fields;
    ln::List<Ref<PIConstant>> constants;
    ln::List<Ref<PIMethod>> methods;

protected:
    LN_SERIALIZE_CLASS_VERSION(1);
    void serialize(ln::Archive& ar) {
        ar& LN_NVP(kind);
        ar& LN_NVP(rawFullName);
        ar& LN_NVP(baseClassRawName);
        ar& LN_NVP(document);
        ar& LN_NVP(metadata);
        ar& LN_NVP(delegateProtoType);
        ar& LN_NVP(templateArguments);
        ar& LN_NVP(fields);
        ar& LN_NVP(constants);
        ar& LN_NVP(methods);
    }
};

// 型情報データベース
class PIDatabase : public ln::RefObject {
public:
    ln::List<Ref<PITypeInfo>> types;
    ln::Array<Ref<PIDocument>> relativeDocuments; // マクロ修飾されていないが、Doxygen comment を持っている関数の情報。copydoc で使う。
    // ln::List<Ref<PIRelativeDocument>> releativeDocuments;	// マクロ修飾されていないが、Doxygen comment を持っている関数の情報。copydoc で使う。

    PIDocument* findRelativeDocument(const ln::List<ln::String>& localSignature);

    void clear();
    void save(const ln::Path& filePath);
    void load(const ln::Path& filePath);
    void mergeFrom(const PIDatabase* src);

    static ln::List<ln::String> parseLocalSigneture(const ln::String& signeture, const ln::String& methodName = ln::String::Empty);

protected:
    LN_SERIALIZE_CLASS_VERSION(1);
    void serialize(ln::Archive& ar) {
        ar& LN_NVP(types);
        // ar & LN_NVP(delegates);
        ar& LN_NVP(relativeDocuments);
    }
};

struct CompilationDatabase {
    ln::String inputFile;
    ln::Array<ln::String> includeDirectories;
    ln::Array<ln::String> forceIncludeFiles;
    ln::String outputPIDB;

    void serialize(ln::Archive& ar) {
        ar& LN_NVP(inputFile);
        ar& LN_NVP(includeDirectories);
        ar& LN_NVP(forceIncludeFiles);
        ar& LN_NVP(outputPIDB);
    }

    void save(const ln::Path& cdbFilePath) {
        ln::JsonTextOutputArchive ar;
        ar.process(*this);
        ln::FileSystem::writeAllText(cdbFilePath, ar.toString());
    }

    void load(const ln::Path& cdbFilePath) {
        auto json = ln::FileSystem::readAllText(cdbFilePath);
        ln::JsonTextInputArchive ar(json);
        ar.process(*this);
    }
};
