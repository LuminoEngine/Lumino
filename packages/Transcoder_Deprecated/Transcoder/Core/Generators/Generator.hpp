#pragma once
#include "../SymbolDatabase.hpp"
#include "OutputBuffer.hpp"

class SymbolDatabase;

enum class FlatCharset {
    Unicode,
    Ascii,
    // Wide,
};

struct FlatCConfiguration {
    ln::Path outputHeaderFile;
    ln::Path outputSourceFile;
};

struct RubyConfiguration {
    ln::Path outputSourceFile;
};

class GeneratorConfiguration : public ln::RefObject {
public:
    // 出力ファイル名などに使用される、モジュールの正式名称
    // e.g.) "Lumino"
    ln::String moduleName;

    // 出力フォルダ
    ln::Path outputDir;

    // テンプレートフォルダ
    ln::Path templateDir;

    // 出力する C++ 名前空間の名前。
    // e.g.) "ln", "ln::local"
    ln::String targetNamespace;

    // targetModuleName に対する、出力モジュール名。
    // 関数名、型名などの Prefix となる。定数 Prefix となる場合は大文字化される。
    // e.g.) "LN"
    ln::String flatCOutputModuleName;

    // LNResult などが定義された C 用の共通ヘッダ。
    // ターゲットのフォルダにコピーしたり、ソースコードに埋め込んだりする。
    ln::Path flatCCommonHeader;

    // FlatC の実装コード (.cpp) の先頭に埋め込まれる文字列。ターゲットモジュールの #include 等を指定する。
    ln::String flatCHeaderString;

    // ライブラリバージョン
    // e.g.) 0.9.0
    ln::String versionString;

    //ln::String flatCHeaderOutputDirOverride;
    //ln::String flatCSourceOutputDirOverride;

    FlatCConfiguration flatC;
    RubyConfiguration ruby;
};

class Generator {
public:
    static const ln::String NewLine;

    void setup(SymbolDatabase* database, GeneratorConfiguration* config) {
        m_database = database;
        m_config = config;
        // m_templateDir = templateDir;
        // m_outputDir = outputDir;
        // m_moduleFullName = moduleFullName;
        // m_moduleShortName = moduleShortName;
    }

    SymbolDatabase* db() const { return m_database; }
    GeneratorConfiguration* config() const { return m_config; }
    ln::Path makeOutputFilePath(const ln::String& lang, const ln::Path& localPath) const { return ln::Path::combine(m_config->outputDir, lang, localPath); }
    ln::Path makeTemplateFilePath(const ln::Path& localPath) const { return ln::Path(m_config->templateDir, localPath); }

    // Utilities
    static ln::String makeUpperSnakeName(const ln::String& name); // PixelFormatA8 -> PIXEL_FORMAT_A8
    static ln::String camelToPascal(const ln::String& name);      // worldObject -> WorldObject
    ln::String makeFlatClassName(const TypeSymbol* type) const;   // e.g) -> "LNTexture2D" ※クラス名のみ対応
    ln::String makeFlatTypeName2(const TypeSymbol* type) const;   // e.g) -> "LNHandle", "LNBool", "int", "LNVector3"...

    ln::String makeFlatAPIName_SetManagedTypeInfoId(const TypeSymbol* type) const; // e.g) -> "LNTexture2D_SetManagedTypeInfoId"
    ln::String makeFlatAPIDecl_SetManagedTypeInfoId(const TypeSymbol* type) const; // e.g) -> "LN_FLAT_API void LNTexture2D_SetManagedTypeInfoId(int64_t id)"
    ln::String makeFlatAPIName_SetOverrideCallback(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const;
    ln::String makeFlatAPIDecl_SetOverrideCallback(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const;
    ln::String makeFlatAPIName_CallOverrideBase(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const;
    ln::String makeFlatAPIDecl_CallOverrideBase(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const;
    ln::String makeFlatAPIName_OverrideFunc(const MethodSymbol* method, FlatCharset charset) const;
    ln::String makeFlatAPIName_SubclassRegistrationInfo(const TypeSymbol* classSymbol) const;
    ln::String makeFlatAPIName_RegisterSubclassTypeInfo(const TypeSymbol* classSymbol) const;
    ln::String makeFlatAPIName_GetSubinstanceId(const TypeSymbol* classSymbol) const;
    ln::String makeFlatAPIName_OverridePrototypesStruct(const TypeSymbol* classSymbol) const;
    // ln::String makeFlatAPIName_SetPrototype(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const;
    // ln::String makeFlatAPIDecl_SetPrototype(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const;

    ln::String makeFlatVirutalCallbackFuncPtrName(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const;
    ln::String makeDelegateCallbackFuncPtrName(const TypeSymbol* delegateSymbol, FlatCharset charset) const;
    ln::String makeFlatShortFuncName(const MethodSymbol* method, FlatCharset charset) const;                               // e.g( -> "SetTexture"
    ln::String makeFlatFullFuncName(const MethodSymbol* method, FlatCharset charset) const;                                // e.g( -> "LNSprite_SetTexture"
    ln::String makeFlatFullFuncName(const TypeSymbol* classSymbol, const MethodSymbol* method, FlatCharset charset) const; // トップレベルの Visual 関数の名前を作るの使用する
    ln::String makeFuncHeader(const MethodSymbol* methodInfo, FlatCharset charset) const;                                  // e.g) -> "LN_FLAT_API LNResult LNWorldObject_Seteulerangles(LNHandle worldobject, float x, float y, float z)"
    ln::String makeFlatCParamQualTypeName(const MethodSymbol* methodInfo, const MethodParameterSymbol* paramInfo, FlatCharset charset) const;
    ln::String makeFlatCharTypeName(FlatCharset charset) const;
    ln::String makeFlatParamList(const MethodSymbol* method, FlatCharset charset) const; // e.g) -> "int p1, int p2, int* outReturn"
    ln::String makeFlatArgList(const MethodSymbol* method) const;
    ln::String makeFlatConstantValue(const ConstantSymbol* constant) const;

    // e.g) -> LN_PIXEL_FORMAT_A8
    ln::String makeFlatEnumMemberName(const TypeSymbol* enumType, const ConstantSymbol* member) const;
    ln::String makeFlatEnumMemberNameFromValue(const ConstantSymbol* member) const;

private:
    SymbolDatabase* m_database;
    Ref<GeneratorConfiguration> m_config;
    // ln::Path m_templateDir;
    // ln::Path m_outputDir;
    // ln::String m_moduleFullName;
    // ln::String m_moduleShortName;
};
