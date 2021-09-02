
#include "../SymbolDatabase.hpp"
#include "FlatCCommon.hpp"

ln::String FlatCCommon::makeCppTypeName(TypeSymbol* typeInfo)
{
	if (typeInfo->IsClass())
	{
		return typeInfo->shortName() + _T("*");
	}

	return typeInfo->shortName();
}

ln::String FlatCCommon::makeFlatCTypeName(TypeSymbol* typeInfo)
{
	if (typeInfo->IsClass())
	{
		return typeInfo->shortName() + _T("*");
	}
	return typeInfo->shortName();
}

ln::String FlatCCommon::makeFlatCParamTypeName(ln::Ref<MethodSymbol> methodInfo, ln::Ref<ParameterSymbol> paramInfo)
{
	auto typeInfo = paramInfo->type;

	if (typeInfo->isStruct)
	{
		ln::String modifer;
		if (paramInfo->isThis && methodInfo->isConst)
			modifer = "const ";
		if (!paramInfo->isThis && !paramInfo->isOut)
			modifer = "const ";
		return ln::String::format("{0}LN{1}*", modifer, typeInfo->shortName());
	}

	if (typeInfo->isEnum)
	{
		return _T("LN") + typeInfo->shortName();
	}

	if (typeInfo == PredefinedTypes::stringType)
	{
		if (paramInfo->isOut || paramInfo->isReturn)
			return "const LNChar**";
		else
			return "const LNChar*";
	}

	ln::String name;
	if (typeInfo == PredefinedTypes::boolType)
		name = _T("LNBool");
	else if (typeInfo->IsClass())
		name = "LNHandle";
	else
		name = typeInfo->shortName();

	if (paramInfo->isOut || paramInfo->isReturn)
		name += "*";

	//if (typeInfo->isStruct && paramInfo->isThis)
	//{
	//	if (methodInfo->isConst)
	//		name = "const " + name;
	//	name += "*";
	//}

	return name;
	//	ln::String name;
	//
	//	if (isOut)
	//	{
	//		name = typeInfo->name;
	//		name += "*";
	//	}
	//	else
	//	{
	//		if (typeInfo == PredefinedTypes::stringType)
	//			name = "const LNChar*";
	//		else if (typeInfo->IsClass())
	//			name = "LNHandle";
	//		else
	//			name = typeInfo->name;
	//	}
	//	return name;
}
