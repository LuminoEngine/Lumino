
#include "Global.h"
#include "SymbolDatabase.h"

TypeInfoPtr	PrimitiveTypes::voidType;
TypeInfoPtr	PrimitiveTypes::boolType;
TypeInfoPtr	PrimitiveTypes::intType;
TypeInfoPtr	PrimitiveTypes::floatType;
TypeInfoPtr	PrimitiveTypes::stringType;

//==============================================================================
// MetadataInfo
//==============================================================================
void MetadataInfo::AddValue(const String& key, const String& value)
{
	values[key] = value;
}

String* MetadataInfo::FindValue(const StringRef& key)
{
	auto itr = values.find(key);
	if (itr != values.end()) return &(itr->second);
	return nullptr;
}

bool MetadataInfo::HasKey(const StringRef& key)
{
	auto itr = values.find(key);
	return (itr != values.end());
}

//==============================================================================
// MethodInfo
//==============================================================================
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
		overloadSuffix += StringTraits::ToUpper((TCHAR)paramInfo->name[0]);
	}

	// return value
	if (!returnType->isVoid)
	{
		auto info = std::make_shared<ParameterInfo>();
		info->name = "outReturn";
		info->type = returnType;
		info->isReturn = true;
		capiParameters.Add(info);
	}

	// constructor
	if (isConstructor)
	{
		if (owner->isStruct)
		{
		}
		else
		{
			auto info = std::make_shared<ParameterInfo>();
			info->name = String::Format("out{0}", owner->name);
			info->type = g_database.FindTypeInfo(owner->name);
			info->isReturn = true;
			capiParameters.Add(info);
		}
	}
}

String MethodInfo::GetCAPIFuncName()
{
	String n = String::Format("LN{0}_{1}", owner->name, name);
	if (IsOverloadChild())
		n += overloadSuffix;
	return n;
}


//==============================================================================
// TypeInfo
//==============================================================================
void TypeInfo::MakeProperties()
{
	for (auto& methodInfo : declaredMethods)
	{
		if (methodInfo->metadata->HasKey("Property"))
		{
			String name;
			bool isGetter = false;
			if (methodInfo->name.IndexOf("Get") == 0)
			{
				name = methodInfo->name.Mid(3);
				isGetter = true;
			}
			if (methodInfo->name.IndexOf("Is") == 0)
			{
				name = methodInfo->name.Mid(3);
				isGetter = true;
			}
			if (methodInfo->name.IndexOf("Set") == 0)
			{
				name = methodInfo->name.Mid(3);
				isGetter = false;
			}

			PropertyInfoPtr propInfo;
			{
				auto* ptr = declaredProperties.Find([name](PropertyInfoPtr info) {return info->name == name; });
				if (ptr == nullptr)
				{
					propInfo = std::make_shared<PropertyInfo>();
					propInfo->name = name;
					declaredProperties.Add(propInfo);
				}
				else
				{
					propInfo = *ptr;
				}
			}

			if (isGetter)
			{
				LN_THROW(propInfo->getter == nullptr, InvalidOperationException);
				propInfo->getter = methodInfo;
				if (propInfo->type == nullptr) propInfo->type = methodInfo->returnType;
			}
			else
			{
				LN_THROW(propInfo->setter == nullptr, InvalidOperationException);
				propInfo->setter = methodInfo;
				if (propInfo->type == nullptr) propInfo->type = methodInfo->parameters[0]->type;
			}

			methodInfo->ownerProperty = propInfo;
		}
	}

	// create document
	for (auto& propInfo : declaredProperties)
	{
		propInfo->MakeDocument();
	}
}

void TypeInfo::LinkOverload()
{
	for (auto& methodInfo1 : declaredMethods)
	{
		if (methodInfo1->IsOverloadChild()) continue;

		// find overload child
		for (auto& methodInfo2 : declaredMethods)
		{
			if (methodInfo2 == methodInfo1) continue;

			if (methodInfo1->name == methodInfo2->name)
			{
				methodInfo1->overloadChildren.Add(methodInfo2);
				methodInfo2->overloadParent = methodInfo1;
			}
		}
	}
}

//==============================================================================
// PropertyInfo
//==============================================================================
void PropertyInfo::MakeDocument()
{
	document = std::make_shared<DocumentInfo>();

	if (getter != nullptr)
	{
		document->summary += getter->document->summary;
		document->details += getter->document->details;
	}
	if (setter != nullptr)
	{
		if (!document->summary.IsEmpty()) document->summary += "\n";
		if (!document->details.IsEmpty()) document->details += "\n\n";

		document->summary += setter->document->summary;
		document->details += setter->document->details;
	}
}

//==============================================================================
// SymbolDatabase
//==============================================================================
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

		structInfo->MakeProperties();
		structInfo->LinkOverload();
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

		classInfo->MakeProperties();
		classInfo->LinkOverload();
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
