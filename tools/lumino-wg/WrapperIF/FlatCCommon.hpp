#pragma once
#include "../SymbolDatabase.hpp"
#include "../Generator.hpp"

class FlatCCommon
{
public:
	static ln::String makeCppTypeName(TypeSymbol* typeInfo);
	static ln::String makeFlatCTypeName(TypeSymbol* typeInfo);
	static ln::String makeFlatCParamTypeName(ln::Ref<MethodSymbol> methodInfo, ln::Ref<ParameterSymbol> paramInfo);
};
