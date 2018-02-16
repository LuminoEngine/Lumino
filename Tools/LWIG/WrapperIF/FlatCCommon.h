#pragma once
#include "../SymbolDatabase.h"
#include "../Generator.h"

class FlatCCommon
{
public:
	static String MakeCppTypeName(Ref<TypeSymbol> typeInfo);
	static String MakeCApiParamTypeName(Ref<MethodSymbol> methodInfo, Ref<ParameterSymbol> paramInfo);
};
