
#include "SymbolDatabase.h"

void SymbolDatabase::Link()
{
	InitializePredefineds();

	// structs
	for (auto structInfo : structs)
	{
		for (auto fieldInfo : structInfo->declaredFields)
		{
			fieldInfo->type = FindTypeInfo(fieldInfo->typeRawName);
		}
		for (auto methodInfo : structInfo->declaredMethods)
		{
			methodInfo->returnType = FindTypeInfo(methodInfo->returnTypeRawName);

			for (auto& paramInfo : methodInfo->parameters)
			{
				paramInfo->type = FindTypeInfo(paramInfo->typeRawName);
			}
		}
	}

	// classes
	for (auto classInfo : classes)
	{
		for (auto methodInfo : classInfo->declaredMethods)
		{
			methodInfo->returnType = FindTypeInfo(methodInfo->returnTypeRawName);

			for (auto& paramInfo : methodInfo->parameters)
			{
				paramInfo->type = FindTypeInfo(paramInfo->typeRawName);
			}
		}
	}

	// enums
	for (auto enumInfo : enums)
	{
		for (auto constantInfo : enumInfo->declaredConstants)
		{
			constantInfo->type = FindTypeInfo(constantInfo->typeRawName);
		}
	}
}

void SymbolDatabase::InitializePredefineds()
{
	predefineds.Add(std::make_shared<TypeInfo>("void")); predefineds.GetLast()->isVoid = true;
	predefineds.Add(std::make_shared<TypeInfo>("bool")); predefineds.GetLast()->isPrimitive = true;
	predefineds.Add(std::make_shared<TypeInfo>("int")); predefineds.GetLast()->isPrimitive = true;
	predefineds.Add(std::make_shared<TypeInfo>("float")); predefineds.GetLast()->isPrimitive = true;
	predefineds.Add(std::make_shared<TypeInfo>("String")); predefineds.GetLast()->isPrimitive = true; stringType = predefineds.GetLast();
}

TypeInfoPtr SymbolDatabase::FindTypeInfo(StringRef typeName)
{
	TypeInfoPtr* type;
	
	type = predefineds.Find([typeName](TypeInfoPtr type) { return type->name == typeName; });
	if (type != nullptr) return *type;

	type = structs.Find([typeName](TypeInfoPtr type) { return type->name == typeName; });
	if (type != nullptr) return *type;

	if (typeName == "StringRef") return stringType;

	LN_UNREACHABLE();
	return nullptr;
}
