#pragma once
#include "../SymbolDatabase.h"
#include "../Generator.h"

class FlatCCommon
{
public:
	static String MakeCppTypeName(Ref<TypeInfo> typeInfo);
	static String MakeCApiParamTypeName(Ref<MethodInfo> methodInfo, Ref<ParameterInfo> paramInfo);
};
