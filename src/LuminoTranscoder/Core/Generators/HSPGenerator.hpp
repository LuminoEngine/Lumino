#pragma once
#include "FlatCGenerator.hpp"
#include "Generator.hpp"

class HSPGeneratorBase
	: public Generator
{
protected:
    // システムとして必要な定義済みコマンドの分だけオフセットを付ける
    int getCommandId(const Symbol* symbol) const { return 32 + symbol->symbolId(); }
};

// .as
class HSPHeaderGenerator
    : public HSPGeneratorBase
{
public:
    void generate();

private:
    ln::String makeEnums() const;
    ln::String makeStructs() const;
    ln::String makeClasses() const;
};

class HSPCommandsGenerator
    : public HSPGeneratorBase
{
public:
    void generate();

private:
    ln::String makeStructStorageCores() const;
    ln::String makeRegisterTypeFunc() const;
    ln::String makeSubclassDefines() const;
    ln::String make_reffunc() const;
    ln::String make_cmdfunc() const;
    ln::String makeCallCommandBlock(const MethodSymbol* methodSymbol) const;
    ln::String makeFetchVAExpr(const TypeSymbol* typeSymbol, bool reffunc) const;
    ln::String makeGetVAExpr(const MethodParameterSymbol* paramSymbol) const;
    ln::String makeSetVAExpr(const MethodParameterSymbol* paramSymbol) const;

    //std::unordered_map<MethodSymbol*, int> m_commandIdMap;

    ln::String makeName_HSPSubclassType(const TypeSymbol* classSymbol) const { return u"HSPSubclass_" + makeFlatClassName(classSymbol); }
    ln::String makeName_DelegateLabelCaller(const TypeSymbol* classSymbol) const { return makeName_HSPSubclassType(classSymbol) + u"_DelegateLabelCaller"; }
    ln::String makeName_SubinstanceAlloc(const TypeSymbol* classSymbol) const { return makeName_HSPSubclassType(classSymbol) + u"_SubinstanceAlloc"; }
    ln::String makeName_SubinstanceFree(const TypeSymbol* classSymbol) const { return makeName_HSPSubclassType(classSymbol) + u"_SubinstanceFree"; }
};
