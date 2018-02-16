#pragma once
#include <unordered_map>

class CSCommon
{
public:
	static void initialize();
	static String MakePInvokeTypeName(Ref<TypeSymbol> typeInfo);
	static String MakeTypeName(Ref<TypeSymbol> typeInfo);
	static String MakeParamTypeName(Ref<ParameterSymbol> paramInfo);
	static String MakeXmlDocument(Ref<DocumentSymbol> doc);
	static String MakeLiteral(Ref<ConstantSymbol> constantInfo);

	static std::unordered_map<TypeSymbol*, String>	primitiveTypesMap;

};
