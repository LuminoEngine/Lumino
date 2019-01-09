#pragma once
#include <unordered_map>

class CSCommon
{
public:
	static void initialize();
	static ln::String MakePInvokeTypeName(ln::Ref<TypeSymbol> typeInfo);
	static ln::String MakeTypeName(ln::Ref<TypeSymbol> typeInfo);
	static ln::String MakeParamTypeName(ln::Ref<ParameterSymbol> paramInfo);
	static ln::String MakeXmlDocument(ln::Ref<DocumentSymbol> doc);
	static ln::String MakeLiteral(ln::Ref<ConstantSymbol> constantInfo);

	static std::unordered_map<TypeSymbol*, ln::String>	primitiveTypesMap;

};
