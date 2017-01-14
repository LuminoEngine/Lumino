
#include "SymbolDatabase.h"

void SymbolDatabase::Link()
{
	InitializePredefineds();

	for (auto structInfo : structs)
	{
		for (auto fieldInfo : structInfo->declaredFields)
		{
			fieldInfo->type = FindTypeInfo(fieldInfo->typeRawName);
		}
	}
}

void SymbolDatabase::InitializePredefineds()
{
	predefineds.Add(std::make_shared<TypeInfo>("void"));
	predefineds.Add(std::make_shared<TypeInfo>("int"));
	predefineds.Add(std::make_shared<TypeInfo>("float"));
}

TypeInfoPtr SymbolDatabase::FindTypeInfo(StringRef typeName)
{
	auto type = predefineds.Find([typeName](TypeInfoPtr type) { return type->name == typeName; });
	return *type;
}
