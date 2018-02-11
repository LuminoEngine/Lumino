
#include "SymbolDatabase.h"


Ref<TypeInfo>	PredefinedTypes::voidType;
Ref<TypeInfo>	PredefinedTypes::nullptrType;
Ref<TypeInfo>	PredefinedTypes::boolType;
Ref<TypeInfo>	PredefinedTypes::intType;
Ref<TypeInfo>	PredefinedTypes::uint32Type;
Ref<TypeInfo>	PredefinedTypes::floatType;
Ref<TypeInfo>	PredefinedTypes::stringType;
Ref<TypeInfo>	PredefinedTypes::objectType;
Ref<TypeInfo>	PredefinedTypes::EventConnectionType;

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
void MethodInfo::LinkParameters(SymbolDatabase* db)
{
	if (metadata->HasKey(_T("Event")))
	{
		Console::writeLine("is event");
	}

	for (auto& paramInfo : parameters)
	{
		paramInfo->type = db->findTypeInfo(paramInfo->typeRawName);

		// 今のところ、Class 型の 出力変数は許可しない
		if (paramInfo->isOut && paramInfo->type->IsClass())
		{
			paramInfo->isIn = true;
			paramInfo->isOut = false;
		}

		if (paramInfo->rawDefaultValue != nullptr)
		{
			String text = paramInfo->rawDefaultValue.get();
			if (text.contains(_T("::")))
			{
				auto tokens = text.split(_T("::"));
				Ref<TypeInfo> dummy;
				db->FindEnumTypeAndValue(tokens[0], tokens[1], &dummy, &paramInfo->defaultValue);
			}
			else
			{
				paramInfo->defaultValue = db->CreateConstantFromLiteralString(text);
			}
		}
	}
}

// capiParameters を構築する
void MethodInfo::ExpandCAPIParameters(SymbolDatabase* db)
{
	// 第1引数
	{
		if (!isStatic)
		{
			if (owner->isStruct)
			{
				auto info = Ref<ParameterInfo>::makeRef();
				info->name = owner->name.toLower();
				info->type = db->findTypeInfo(owner->name);
				info->isThis = true;
				capiParameters.add(info);
			}
			else if (isConstructor)
			{
			}
			else if (owner->isDelegate)
			{
				auto info = Ref<ParameterInfo>::makeRef();
				info->name = _T("sender");
				info->type = PredefinedTypes::objectType;
				info->isThis = true;
				capiParameters.add(info);
			}
			else
			{
				auto info = Ref<ParameterInfo>::makeRef();
				info->name = owner->name.toLower();
				info->type = db->findTypeInfo(owner->name);
				info->isThis = true;
				capiParameters.add(info);
			}
		}
	}

	// params
	for (auto& paramInfo : parameters)
	{
		capiParameters.add(paramInfo);
		overloadSuffix += StringTraits::toUpper((Char)paramInfo->name[0]);
	}

	// return value
	if (returnType->isVoid || returnType == PredefinedTypes::EventConnectionType)
	{
		// "void", "EventConnection" は戻り値扱いしない
	}
	else
	{
		auto info = Ref<ParameterInfo>::makeRef();
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
			auto info = Ref<ParameterInfo>::makeRef();
			info->name = String::format(_T("out{0}"), owner->name);
			info->type = db->findTypeInfo(owner->name);
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
	return String::format(_T("LN{0}_{1}_OverrideCaller"), owner->name, name);
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
	//case AccessLevel::Internal:
	//	return "internal";
	default:
		LN_UNREACHABLE();
		break;
	}
	return "";
}

//==============================================================================
// TypeInfo
//==============================================================================
void TypeInfo::Link(SymbolDatabase* db)
{
	MakeProperties();
	LinkOverload();
	ResolveCopyDoc();

	// find base class
	if (!baseClassRawName.isEmpty())
	{
		baseClass = db->findTypeInfo(baseClassRawName);
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
				name = methodInfo->name.substring(3);
				isGetter = true;
			}
			if (methodInfo->name.indexOf(_T("Is")) == 0)
			{
				name = methodInfo->name.substring(2);
				namePrefix = _T("Is");
				isGetter = true;
			}
			if (methodInfo->name.indexOf(_T("Set")) == 0)
			{
				name = methodInfo->name.substring(3);
				isGetter = false;
			}

			Ref<PropertyInfo> propInfo;
			{
				auto* ptr = declaredProperties.find([name](Ref<PropertyInfo> info) {return info->name == name; });
				if (ptr == nullptr)
				{
					propInfo = Ref<PropertyInfo>::makeRef();
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
				LN_DCHECK(propInfo->getter == nullptr);
				propInfo->getter = methodInfo;
				if (propInfo->type == nullptr) propInfo->type = methodInfo->returnType;
			}
			else
			{
				LN_DCHECK(propInfo->setter == nullptr);
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
	document = Ref<DocumentInfo>::makeRef();

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
			fieldInfo->type = findTypeInfo(fieldInfo->typeRawName);
		}
		for (auto methodInfo : structInfo->declaredMethods)
		{
			methodInfo->returnType = findTypeInfo(methodInfo->returnTypeRawName);

			methodInfo->LinkParameters(this);
			methodInfo->ExpandCAPIParameters(this);
		}

		structInfo->Link(this);
	}

	// classes
	for (auto classInfo : classes)
	{
		for (auto methodInfo : classInfo->declaredMethods)
		{
			methodInfo->returnType = findTypeInfo(methodInfo->returnTypeRawName);

			methodInfo->LinkParameters(this);
			methodInfo->ExpandCAPIParameters(this);
		}

		classInfo->Link(this);
	}

	// enums
	//for (auto enumInfo : enums)
	//{
	//	for (auto constantInfo : enumInfo->declaredConstants)
	//	{
	//		constantInfo->type = findTypeInfo(constantInfo->typeRawName);
	//	}
	//}

	// delegates
	for (auto classInfo : delegates)
	{
		for (auto methodInfo : classInfo->declaredMethods)
		{
			methodInfo->returnType = findTypeInfo(methodInfo->returnTypeRawName);

			methodInfo->LinkParameters(this);
			methodInfo->ExpandCAPIParameters(this);
		}

		classInfo->Link(this);
	}
}

tr::Enumerator<Ref<MethodInfo>> SymbolDatabase::GetAllMethods()
{
	List<Ref<MethodInfo>> dummy;
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

void SymbolDatabase::FindEnumTypeAndValue(const String& typeName, const String& memberName, Ref<TypeInfo>* outEnum, Ref<ConstantInfo>* outMember)
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

	LN_ENSURE(0, "Undefined enum: %s::%s", typeName.c_str(), memberName.c_str());
}

Ref<ConstantInfo> SymbolDatabase::CreateConstantFromLiteralString(const String& valueStr)
{
	auto info = Ref<ConstantInfo>::makeRef();
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
	predefineds.add(Ref<TypeInfo>::makeRef(_T("void")));
	predefineds.getLast()->isVoid = true;
	PredefinedTypes::voidType = predefineds.getLast();

	predefineds.add(Ref<TypeInfo>::makeRef(_T("nullptr")));
	predefineds.getLast()->isVoid = true;
	PredefinedTypes::nullptrType = predefineds.getLast();

	predefineds.add(Ref<TypeInfo>::makeRef(_T("bool")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::boolType = predefineds.getLast();

	predefineds.add(Ref<TypeInfo>::makeRef(_T("int")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::intType = predefineds.getLast();

	predefineds.add(Ref<TypeInfo>::makeRef(_T("uint32_t")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::uint32Type = predefineds.getLast();

	predefineds.add(Ref<TypeInfo>::makeRef(_T("float")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::floatType = predefineds.getLast();

	predefineds.add(Ref<TypeInfo>::makeRef(_T("String")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::stringType = predefineds.getLast();

	predefineds.add(Ref<TypeInfo>::makeRef(_T("Object")));
	PredefinedTypes::objectType = predefineds.getLast();

	predefineds.add(Ref<TypeInfo>::makeRef(_T("EventConnection")));
	PredefinedTypes::EventConnectionType = predefineds.getLast();
}

Ref<TypeInfo> SymbolDatabase::findTypeInfo(StringRef typeName)
{
	Ref<TypeInfo>* type;
	
	type = predefineds.find([typeName](Ref<TypeInfo> type) { return type->name == typeName; });
	if (type != nullptr) return *type;

	type = structs.find([typeName](Ref<TypeInfo> type) { return type->name == typeName; });
	if (type != nullptr) return *type;

	type = classes.find([typeName](Ref<TypeInfo> type) { return type->name == typeName; });
	if (type != nullptr) return *type;

	type = enums.find([typeName](Ref<TypeInfo> type) { return type->name == typeName; });
	if (type != nullptr) return *type;

	type = delegates.find([typeName](Ref<TypeInfo> type) { return type->name == typeName; });
	if (type != nullptr)
		return *type;

	if (typeName == _T("StringRef")) return PredefinedTypes::stringType;
	if (typeName == _T("EventConnection")) return PredefinedTypes::EventConnectionType;

	LN_ENSURE(0, _T("Undefined type: %s"), String(typeName).c_str());
	return nullptr;
}


