#include "../SymbolDatabase.hpp"
#include "../OutputBuffer.hpp"
#include "CSCommon.hpp"

std::unordered_map<TypeSymbol*, ln::String>	CSCommon::primitiveTypesMap;

void CSCommon::initialize()
{
	primitiveTypesMap[PredefinedTypes::voidType] = "void";
	primitiveTypesMap[PredefinedTypes::boolType] = "bool";
	primitiveTypesMap[PredefinedTypes::intType] = "int";
	primitiveTypesMap[PredefinedTypes::uint32Type] = "int";
	primitiveTypesMap[PredefinedTypes::floatType] = "float";
	primitiveTypesMap[PredefinedTypes::stringType] = "string";
}

ln::String CSCommon::MakePInvokeTypeName(ln::Ref<TypeSymbol> typeInfo)
{
	if (typeInfo->IsClass())
		return "IntPtr";

	return MakeTypeName(typeInfo);
}

ln::String CSCommon::MakeTypeName(ln::Ref<TypeSymbol> typeInfo)
{
	auto itr = primitiveTypesMap.find(typeInfo);
	if (itr != primitiveTypesMap.end())
		return itr->second;

	return typeInfo->shortName();
}

ln::String CSCommon::MakeParamTypeName(ln::Ref<ParameterSymbol> paramInfo)
{
	ln::String m;
	if (paramInfo->isOut) m += "out ";
	//else if (paramInfo->type->isStruct) m += "ref ";

	return m + MakeTypeName(paramInfo->type);
}

ln::String CSCommon::MakeXmlDocument(ln::Ref<DocumentSymbol> doc)
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

ln::String CSCommon::MakeLiteral(ln::Ref<ConstantSymbol> constantInfo)
{
	if (constantInfo->type == PredefinedTypes::boolType)
	{
		if (constantInfo->value->get<bool>())
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
		return constantInfo->type->shortName() + "." + constantInfo->name;
	}
	if (constantInfo->type == PredefinedTypes::intType)
	{
		return ln::String::format("{0}", constantInfo->value->get<int>());
	}
	if (constantInfo->type == PredefinedTypes::floatType)
	{
		return ln::String::format("{0}f", constantInfo->value->get<float>());
	}
	return "";
}
