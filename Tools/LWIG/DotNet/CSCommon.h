#pragma once
#include <unordered_map>

class CSCommon
{
public:
	static void initialize();
	static String MakePInvokeTypeName(Ref<TypeInfo> typeInfo);
	static String MakeTypeName(Ref<TypeInfo> typeInfo);
	static String MakeParamTypeName(Ref<ParameterInfo> paramInfo);
	static String MakeXmlDocument(Ref<DocumentInfo> doc);
	static String MakeLiteral(Ref<ConstantInfo> constantInfo);

	static std::unordered_map<TypeInfo*, String>	primitiveTypesMap;

};
