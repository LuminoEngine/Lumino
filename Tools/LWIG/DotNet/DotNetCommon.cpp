#include "../OutputBuffer.h"
#include "DotNetCommon.h"

std::unordered_map<TypeInfoPtr, String>	DotNetCommon::primitiveTypesMap;

void DotNetCommon::initialize()
{
	primitiveTypesMap[PredefinedTypes::voidType] = "void";
	primitiveTypesMap[PredefinedTypes::boolType] = "bool";
	primitiveTypesMap[PredefinedTypes::intType] = "int";
	primitiveTypesMap[PredefinedTypes::uint32Type] = "int";
	primitiveTypesMap[PredefinedTypes::floatType] = "float";
	primitiveTypesMap[PredefinedTypes::stringType] = "string";
}

String DotNetCommon::MakePInvokeTypeName(TypeInfoPtr typeInfo)
{
	if (typeInfo->IsClass())
		return "IntPtr";

	return MakeTypeName(typeInfo);
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
	//else if (paramInfo->type->isStruct) m += "ref ";

	return m + MakeTypeName(paramInfo->type);
}

String DotNetCommon::MakeXmlDocument(DocumentInfoPtr doc)
{
	OutputBuffer buffer;
	if (doc != nullptr)
	{
		buffer.append("/// <summary>").NewLine();
		buffer.AppendLinesHeaderd(doc->summary, "/// ").NewLine();
		buffer.append("/// </summary>").NewLine();

		for (auto& param : doc->params)
		{
			buffer.append("/// <param name=\"{0}\">", param->name).NewLine();
			buffer.AppendLinesHeaderd(param->description, "/// ").NewLine();
			buffer.append("/// </param>").NewLine();
		}

		if (!doc->returns.isEmpty())
		{
			buffer.append("/// <returns>").NewLine();
			buffer.AppendLinesHeaderd(doc->returns, "/// ").NewLine();
			buffer.append("/// </returns>").NewLine();
		}

		if (!doc->details.isEmpty())
		{
			buffer.append("/// <remarks>").NewLine();
			buffer.AppendLinesHeaderd(doc->details, "/// ").NewLine();
			buffer.append("/// </remarks>").NewLine();
		}
	}
	return buffer.toString();
}

String DotNetCommon::MakeLiteral(ConstantInfoPtr constantInfo)
{
	if (constantInfo->type == PredefinedTypes::boolType)
	{
		if (tr::Variant::Cast<bool>(constantInfo->value))
			return "true";
		else
			return "false";
	}
	if (constantInfo->type == PredefinedTypes::nullptrType)
	{
		return "null";
	}
	if (constantInfo->type->isEnum)
	{
		return constantInfo->type->name + "." + constantInfo->name;
	}
	if (constantInfo->type == PredefinedTypes::intType)
	{
		return String::format("{0}", tr::Variant::Cast<int>(constantInfo->value));
	}
	if (constantInfo->type == PredefinedTypes::floatType)
	{
		return String::format("{0}f", tr::Variant::Cast<float>(constantInfo->value));
	}
	return "";
}
