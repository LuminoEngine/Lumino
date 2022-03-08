#pragma once
#include "FlatCGenerator.hpp"
#include "Generator.hpp"

class HSP3GeneratorBase
    : public Generator {
protected:
    // システムとして必要な定義済みコマンドの分だけオフセットを付ける
    // Note: 上限はドキュメントには見当たらなかったが、0x2B020 を呼び出したりすると「サポートされていない機能を呼び出そうとしました」エラーになった
    // << 8 をやめたら呼び出せた。short max あたりが上限？
    int getCommandId(const Symbol* symbol) const { return symbol->symbolId() + 32; }
    //int getCommandId(const Symbol* symbol, int subIndex) const
    //{
    //    if (LN_REQUIRE(subIndex < 255)) return 0;
    //    return (symbol->symbolId() << 8) + 32 + subIndex;
    //}
};

// .as
class HSP3HeaderGenerator
    : public HSP3GeneratorBase {
public:
    void generate();

private:
    ln::String makeEnums() const;
    ln::String makeStructs() const;
    ln::String makeClasses() const;
};

// .cpp
class HSP3CommandsGenerator
    : public HSP3GeneratorBase {
public:
    void generate();

private:
    ln::String makeStructStorageCores() const;
    ln::String makeRegisterTypeFunc() const;
    ln::String makeSubclassDefines() const;
    ln::String make_reffunc() const;
    ln::String make_cmdfunc() const;
    ln::String makeCallCommandBlock(const MethodSymbol* methodSymbol) const;
    ln::String makeFetchVAExpr(const TypeSymbol* typeSymbol, bool reffunc, const ConstantSymbol* defaultValue = nullptr) const;
    ln::String makeGetVAExpr(const MethodParameterSymbol* paramSymbol) const;
    ln::String makeSetVAExpr(const MethodParameterSymbol* paramSymbol) const;

    //std::unordered_map<MethodSymbol*, int> m_commandIdMap;

    ln::String makeName_HSPSubclassType(const TypeSymbol* classSymbol) const { return U"HSPSubclass_" + makeFlatClassName(classSymbol); }
    ln::String makeName_DelegateLabelCaller(const TypeSymbol* classSymbol) const { return makeName_HSPSubclassType(classSymbol) + U"_DelegateLabelCaller"; }
    ln::String makeName_SubinstanceAlloc(const TypeSymbol* classSymbol) const { return makeName_HSPSubclassType(classSymbol) + U"_SubinstanceAlloc"; }
    ln::String makeName_SubinstanceFree(const TypeSymbol* classSymbol) const { return makeName_HSPSubclassType(classSymbol) + U"_SubinstanceFree"; }
};

// .hs
class HSP3HelpGenerator
    : public HSP3GeneratorBase {
public:
    void generate();

private:
    ln::String makeFuncDocument(const MethodSymbol* methodSymbol) const;
    ln::String makeIOName(const MethodParameterSymbol* paramSymbol) const;
    ln::String translateComment(const ln::String& text) const;
};
