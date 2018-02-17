#pragma once
#include "../SymbolDatabase.h"
#include "../Generator.h"

class FlatCCommon
{
public:
	static String makeCppTypeName(TypeSymbol* typeInfo);
	static String makeFlatCTypeName(TypeSymbol* typeInfo);
	static String makeFlatCParamTypeName(Ref<MethodSymbol> methodInfo, Ref<ParameterSymbol> paramInfo);
};
