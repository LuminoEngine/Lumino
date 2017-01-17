
#include "Global.h"
#include "SymbolDatabase.h"

TypeInfoPtr	PrimitiveTypes::voidType;
TypeInfoPtr	PrimitiveTypes::boolType;
TypeInfoPtr	PrimitiveTypes::intType;
TypeInfoPtr	PrimitiveTypes::floatType;
TypeInfoPtr	PrimitiveTypes::stringType;

void MethodInfo::ExpandCAPIParameters()
{
	// ‘æ1ˆø”
	{
		if (!isStatic)
		{
			if (owner->isStruct)
			{
				auto info = std::make_shared<ParameterInfo>();
				info->name = owner->name.ToLower();
				info->type = g_database.FindTypeInfo(owner->name);
				info->isThis = true;
				capiParameters.Add(info);
			}
			else if (!isConstructor)
			{
				auto info = std::make_shared<ParameterInfo>();
				info->name = owner->name.ToLower();
				info->type = g_database.FindTypeInfo(owner->name);
				info->isThis = true;
				capiParameters.Add(info);
			}
		}
	}

	// params
	for (auto& paramInfo : parameters)
	{
		capiParameters.Add(paramInfo);
	}

	// return value
	if (!returnType->isVoid)
	{
		auto info = std::make_shared<ParameterInfo>();
		info->name = "outReturn";
		info->type = g_database.FindTypeInfo(owner->name);
		info->isReturn = true;
		capiParameters.Add(info);
	}

	// constructor
	if (isConstructor)
	{
		auto info = std::make_shared<ParameterInfo>();
		info->name = String::Format("out{0}", owner->name);
		info->type = g_database.FindTypeInfo(owner->name);
		info->isReturn = true;
		capiParameters.Add(info);
	}
}

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

			methodInfo->ExpandCAPIParameters();
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

			methodInfo->ExpandCAPIParameters();
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

tr::Enumerator<MethodInfoPtr> SymbolDatabase::GetAllMethods()
{
	List<MethodInfoPtr> dummy;
	auto e = tr::MakeEnumerator::from(dummy);

	for (auto& structInfo : structs)
	{
		e.Join(structInfo->declaredMethods);
	}
	for (auto& classInfo : classes)
	{
		e.Join(classInfo->declaredMethods);
	}

	return e;
}

void SymbolDatabase::InitializePredefineds()
{
	predefineds.Add(std::make_shared<TypeInfo>("void"));
	predefineds.GetLast()->isVoid = true;
	PrimitiveTypes::voidType = predefineds.GetLast();

	predefineds.Add(std::make_shared<TypeInfo>("bool"));
	predefineds.GetLast()->isPrimitive = true;
	PrimitiveTypes::boolType = predefineds.GetLast();

	predefineds.Add(std::make_shared<TypeInfo>("int"));
	predefineds.GetLast()->isPrimitive = true;
	PrimitiveTypes::intType = predefineds.GetLast();

	predefineds.Add(std::make_shared<TypeInfo>("float"));
	predefineds.GetLast()->isPrimitive = true;
	PrimitiveTypes::floatType = predefineds.GetLast();

	predefineds.Add(std::make_shared<TypeInfo>("String"));
	predefineds.GetLast()->isPrimitive = true;
	PrimitiveTypes::stringType = predefineds.GetLast();
}

TypeInfoPtr SymbolDatabase::FindTypeInfo(StringRef typeName)
{
	TypeInfoPtr* type;
	
	type = predefineds.Find([typeName](TypeInfoPtr type) { return type->name == typeName; });
	if (type != nullptr) return *type;

	type = structs.Find([typeName](TypeInfoPtr type) { return type->name == typeName; });
	if (type != nullptr) return *type;

	type = classes.Find([typeName](TypeInfoPtr type) { return type->name == typeName; });
	if (type != nullptr) return *type;

	if (typeName == "StringRef") return PrimitiveTypes::stringType;

	LN_UNREACHABLE();
	return nullptr;
}
