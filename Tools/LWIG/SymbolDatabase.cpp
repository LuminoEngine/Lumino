
#include "Global.h"
#include "SymbolDatabase.h"

TypeInfoPtr	PredefinedTypes::voidType;
TypeInfoPtr	PredefinedTypes::nullptrType;
TypeInfoPtr	PredefinedTypes::boolType;
TypeInfoPtr	PredefinedTypes::intType;
TypeInfoPtr	PredefinedTypes::uint32Type;
TypeInfoPtr	PredefinedTypes::floatType;
TypeInfoPtr	PredefinedTypes::stringType;
TypeInfoPtr	PredefinedTypes::objectType;
TypeInfoPtr	PredefinedTypes::EventConnectionType;

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
void MethodInfo::LinkParameters()
{
	if (metadata->HasKey("Event"))
	{
		Console::writeLine("is event");
	}

	for (auto& paramInfo : parameters)
	{
		paramInfo->type = g_database.FindTypeInfo(paramInfo->typeRawName);

		// 今のところ、Class 型の 出力変数は許可しない
		if (paramInfo->isOut && paramInfo->type->IsClass())
		{
			paramInfo->isIn = true;
			paramInfo->isOut = false;
		}

		if (paramInfo->rawDefaultValue != nullptr)
		{
			String text = paramInfo->rawDefaultValue.get();
			if (text.contains("::"))
			{
				auto tokens = text.split("::");
				TypeInfoPtr dummy;
				g_database.FindEnumTypeAndValue(tokens[0], tokens[1], &dummy, &paramInfo->defaultValue);
			}
			else
			{
				paramInfo->defaultValue = g_database.CreateConstantFromLiteralString(text);
			}
		}
	}
}

void MethodInfo::ExpandCAPIParameters()
{
	// 第1引数
	{
		if (!isStatic)
		{
			if (owner->isStruct)
			{
				auto info = std::make_shared<ParameterInfo>();
				info->name = owner->name.toLower();
				info->type = g_database.FindTypeInfo(owner->name);
				info->isThis = true;
				capiParameters.add(info);
			}
			else if (isConstructor)
			{
			}
			else if (owner->isDelegate)
			{
				auto info = std::make_shared<ParameterInfo>();
				info->name = _T("sender");
				info->type = PredefinedTypes::objectType;
				info->isThis = true;
				capiParameters.add(info);
			}
			else
			{
				auto info = std::make_shared<ParameterInfo>();
				info->name = owner->name.toLower();
				info->type = g_database.FindTypeInfo(owner->name);
				info->isThis = true;
				capiParameters.add(info);
			}
		}
	}

	// params
	for (auto& paramInfo : parameters)
	{
		capiParameters.add(paramInfo);
		overloadSuffix += StringTraits::toUpper((TCHAR)paramInfo->name[0]);
	}

	// return value
	if (returnType->isVoid || returnType == PredefinedTypes::EventConnectionType)
	{
		// "void", "EventConnection" は戻り値扱いしない
	}
	else
	{
		auto info = std::make_shared<ParameterInfo>();
		info->name = "outReturn";
		info->type = returnType;
		info->isIn = false;
		info->isOut = true;
		info->isReturn = true;
		capiParameters.add(info);
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
			info->name = String::format(_T("out{0}"), owner->name);
			info->type = g_database.FindTypeInfo(owner->name);
			info->isReturn = true;
			capiParameters.add(info);
		}
	}
}

String MethodInfo::GetCAPIFuncName()
{
	String n = String::format(_T("LN{0}_{1}"), owner->name, name);
	if (IsOverloadChild())
		n += overloadSuffix;
	return n;
}

String MethodInfo::GetCApiSetOverrideCallbackFuncName()
{
	return GetCAPIFuncName() + "_SetOverrideCaller";
}

String MethodInfo::GetCApiSetOverrideCallbackTypeName()
{
	return String::format("LN{0}_{1}_OverrideCaller", owner->name, name);
}

String MethodInfo::GetAccessLevelName(AccessLevel accessLevel)
{
	switch (accessLevel)
	{
	case AccessLevel::Public:
		return "public";
	case AccessLevel::Protected:
		return "protected";
	case AccessLevel::Private:
		return "private";
	case AccessLevel::Internal:
		return "internal";
	}
	return "";
}

//==============================================================================
// TypeInfo
//==============================================================================
void TypeInfo::Link()
{
	MakeProperties();
	LinkOverload();
	ResolveCopyDoc();

	// find base class
	if (!baseClassRawName.isEmpty())
	{
		baseClass = g_database.FindTypeInfo(baseClassRawName);
	}
}

void TypeInfo::MakeProperties()
{
	for (auto& methodInfo : declaredMethods)
	{
		if (methodInfo->metadata->HasKey(_T("Property")))
		{
			String name;
			String namePrefix;
			bool isGetter = false;
			if (methodInfo->name.indexOf(_T("Get")) == 0)
			{
				name = methodInfo->name.mid(3);
				isGetter = true;
			}
			if (methodInfo->name.indexOf(_T("Is")) == 0)
			{
				name = methodInfo->name.mid(2);
				namePrefix = _T("Is");
				isGetter = true;
			}
			if (methodInfo->name.indexOf(_T("Set")) == 0)
			{
				name = methodInfo->name.mid(3);
				isGetter = false;
			}

			PropertyInfoPtr propInfo;
			{
				auto* ptr = declaredProperties.find([name](PropertyInfoPtr info) {return info->name == name; });
				if (ptr == nullptr)
				{
					propInfo = std::make_shared<PropertyInfo>();
					propInfo->name = name;
					declaredProperties.add(propInfo);
				}
				else
				{
					propInfo = *ptr;
				}
				propInfo->namePrefix = namePrefix;
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
				methodInfo1->overloadChildren.add(methodInfo2);
				methodInfo2->overloadParent = methodInfo1;
			}
		}
	}
}

void TypeInfo::ResolveCopyDoc()
{
	for (auto& methodInfo : declaredMethods)
	{
		if (methodInfo->document != nullptr && methodInfo->document->IsCopyDoc())
		{
			auto e = tr::MakeEnumerator::from(declaredMethods)
				.Join(declaredMethodsForDocument);
			for (auto& methodInfo2 : e)
			{
				if (methodInfo2->name == methodInfo->document->copydocMethodName &&
					methodInfo2->paramsRawSignature == methodInfo->document->copydocSignature)
				{
					methodInfo->document = methodInfo2->document;
					break;
				}
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
		if (!document->summary.isEmpty()) document->summary += _T("\n");
		if (!document->details.isEmpty()) document->details += _T("\n\n");

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

			methodInfo->LinkParameters();
			methodInfo->ExpandCAPIParameters();
		}

		structInfo->Link();
	}

	// classes
	for (auto classInfo : classes)
	{
		for (auto methodInfo : classInfo->declaredMethods)
		{
			methodInfo->returnType = FindTypeInfo(methodInfo->returnTypeRawName);

			methodInfo->LinkParameters();
			methodInfo->ExpandCAPIParameters();
		}

		classInfo->Link();
	}

	// enums
	//for (auto enumInfo : enums)
	//{
	//	for (auto constantInfo : enumInfo->declaredConstants)
	//	{
	//		constantInfo->type = FindTypeInfo(constantInfo->typeRawName);
	//	}
	//}

	// delegates
	for (auto classInfo : delegates)
	{
		for (auto methodInfo : classInfo->declaredMethods)
		{
			methodInfo->returnType = FindTypeInfo(methodInfo->returnTypeRawName);

			methodInfo->LinkParameters();
			methodInfo->ExpandCAPIParameters();
		}

		classInfo->Link();
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

void SymbolDatabase::FindEnumTypeAndValue(const String& typeName, const String& memberName, TypeInfoPtr* outEnum, ConstantInfoPtr* outMember)
{
	for (auto& enumInfo : enums)
	{
		if (enumInfo->name == typeName)
		{
			for (auto& constantInfo : enumInfo->declaredConstants)
			{
				if (constantInfo->name == memberName)
				{
					*outEnum = enumInfo;
					*outMember = constantInfo;
					return;
				}
			}
		}
	}

	LN_THROW(0, InvalidOperationException, "Undefined enum: %s::%s", typeName.c_str(), memberName.c_str());
}

ConstantInfoPtr SymbolDatabase::CreateConstantFromLiteralString(const String& valueStr)
{
	auto info = std::make_shared<ConstantInfo>();
	if (valueStr == "true")
	{
		info->type = PredefinedTypes::boolType;
		info->value = true;
	}
	else if (valueStr == "false")
	{
		info->type = PredefinedTypes::boolType;
		info->value = false;
	}
	else if (valueStr == "nullptr")
	{
		info->type = PredefinedTypes::nullptrType;
		info->value = nullptr;
	}
	else if (valueStr.contains('.'))
	{
		info->type = PredefinedTypes::floatType;
		info->value = StringTraits::toFloat(valueStr.c_str());
	}
	else
	{
		info->type = PredefinedTypes::intType;
		info->value = StringTraits::toInt32(valueStr.c_str());
	}

	return info;
}

void SymbolDatabase::InitializePredefineds()
{
	predefineds.add(std::make_shared<TypeInfo>(_T("void")));
	predefineds.getLast()->isVoid = true;
	PredefinedTypes::voidType = predefineds.getLast();

	predefineds.add(std::make_shared<TypeInfo>(_T("nullptr")));
	predefineds.getLast()->isVoid = true;
	PredefinedTypes::nullptrType = predefineds.getLast();

	predefineds.add(std::make_shared<TypeInfo>(_T("bool")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::boolType = predefineds.getLast();

	predefineds.add(std::make_shared<TypeInfo>(_T("int")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::intType = predefineds.getLast();

	predefineds.add(std::make_shared<TypeInfo>(_T("uint32_t")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::uint32Type = predefineds.getLast();

	predefineds.add(std::make_shared<TypeInfo>(_T("float")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::floatType = predefineds.getLast();

	predefineds.add(std::make_shared<TypeInfo>(_T("String")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::stringType = predefineds.getLast();

	predefineds.add(std::make_shared<TypeInfo>(_T("Object")));
	PredefinedTypes::objectType = predefineds.getLast();

	predefineds.add(std::make_shared<TypeInfo>(_T("EventConnection")));
	PredefinedTypes::EventConnectionType = predefineds.getLast();
}

TypeInfoPtr SymbolDatabase::FindTypeInfo(StringRef typeName)
{
	TypeInfoPtr* type;
	
	type = predefineds.find([typeName](TypeInfoPtr type) { return type->name == typeName; });
	if (type != nullptr) return *type;

	type = structs.find([typeName](TypeInfoPtr type) { return type->name == typeName; });
	if (type != nullptr) return *type;

	type = classes.find([typeName](TypeInfoPtr type) { return type->name == typeName; });
	if (type != nullptr) return *type;

	type = enums.find([typeName](TypeInfoPtr type) { return type->name == typeName; });
	if (type != nullptr) return *type;

	type = delegates.find([typeName](TypeInfoPtr type) { return type->name == typeName; });
	if (type != nullptr)
		return *type;

	if (typeName == _T("StringRef")) return PredefinedTypes::stringType;
	if (typeName == _T("EventConnection")) return PredefinedTypes::EventConnectionType;

	LN_THROW(0, InvalidOperationException, "Undefined type: %s", typeName.toString().c_str());
	return nullptr;
}
