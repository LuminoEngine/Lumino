#include "../OutputBuffer.h"
#include "DotNetCommon.h"

std::unordered_map<TypeInfoPtr, String>	DotNetCommon::primitiveTypesMap;

void DotNetCommon::Initialize()
{
	primitiveTypesMap[PrimitiveTypes::voidType] = "void";
	primitiveTypesMap[PrimitiveTypes::boolType] = "bool";
	primitiveTypesMap[PrimitiveTypes::intType] = "int";
	primitiveTypesMap[PrimitiveTypes::floatType] = "float";
	primitiveTypesMap[PrimitiveTypes::stringType] = "string";
}

String DotNetCommon::MakeTypeName(TypeInfoPtr typeInfo)
{
	auto itr = primitiveTypesMap.find(typeInfo);
	if (itr != primitiveTypesMap.end())
		return itr->second;

	return typeInfo->name;
}

String DotNetCommon::MakeParamTypeName(ParameterInfoPtr paramInfo)
{
	String m;
	if (paramInfo->isOut) m += "out ";
	else if (paramInfo->type->isStruct) m += "ref ";

	return m + MakeTypeName(paramInfo->type);
}

String DotNetCommon::MakeXmlDocument(DocumentInfoPtr doc)
{
	OutputBuffer buffer;
	if (doc != nullptr)
	{
		buffer.Append("/// <summary>").NewLine();
		buffer.AppendLinesHeaderd(doc->summary, "/// ").NewLine();
		buffer.Append("/// </summary>").NewLine();

		for (auto& param : doc->params)
		{
			buffer.Append("/// <param name=\"{0}\">", param->name).NewLine();
			buffer.AppendLinesHeaderd(param->description, "/// ").NewLine();
			buffer.Append("/// </param>").NewLine();
		}

		if (!doc->returns.IsEmpty())
		{
			buffer.Append("/// <returns>").NewLine();
			buffer.AppendLinesHeaderd(doc->returns, "/// ").NewLine();
			buffer.Append("/// </returns>").NewLine();
		}

		if (!doc->details.IsEmpty())
		{
			buffer.Append("/// <remarks>").NewLine();
			buffer.AppendLinesHeaderd(doc->details, "/// ").NewLine();
			buffer.Append("/// </remarks>").NewLine();
		}
	}
	return buffer.ToString();
}
