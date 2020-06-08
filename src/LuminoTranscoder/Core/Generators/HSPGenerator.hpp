#pragma once
#include "FlatCGenerator.hpp"
#include "Generator.hpp"

class HSPGeneratorBase
	: public Generator
{
protected:
};

// .as
class HSPHeaderGenerator
    : public HSPGeneratorBase
{
public:
    void generate();

private:
    ln::String makeEnums() const;
};

class HSPCommandsGenerator
    : public HSPGeneratorBase
{
public:
    void generate();

private:
    ln::String make_cmdfunc() const;
    ln::String makeCallCommandBlock(const MethodSymbol* methodSymbol) const;
    ln::String makeGetVAExpr(const MethodParameterSymbol* paramSymbol) const;
    ln::String makeSetVAExpr(const MethodParameterSymbol* paramSymbol) const;

    std::unordered_map<MethodSymbol*, int> m_commandIdMap;
};
