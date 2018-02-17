
#include "SymbolDatabase.h"

Ref<TypeSymbol>	PredefinedTypes::voidType;
Ref<TypeSymbol>	PredefinedTypes::nullptrType;
Ref<TypeSymbol>	PredefinedTypes::boolType;
Ref<TypeSymbol>	PredefinedTypes::intType;
Ref<TypeSymbol>	PredefinedTypes::int16Type;
Ref<TypeSymbol>	PredefinedTypes::uint32Type;
Ref<TypeSymbol>	PredefinedTypes::floatType;
Ref<TypeSymbol>	PredefinedTypes::stringType;
Ref<TypeSymbol>	PredefinedTypes::objectType;
Ref<TypeSymbol>	PredefinedTypes::EventConnectionType;

//==============================================================================
// MetadataSymbol
//==============================================================================
const String MetadataSymbol::OverloadPostfixAttr = _T("OverloadPostfix");

void MetadataSymbol::AddValue(const String& key, const String& value)
{
	values[key] = value;
}

String* MetadataSymbol::FindValue(const StringRef& key)
{
	auto itr = values.find(key);
	if (itr != values.end()) return &(itr->second);
	return nullptr;
}

String MetadataSymbol::getValue(const StringRef& key, const String& defaultValue)
{
	auto* s = FindValue(key);
	if (s)
		return *s;
	else
		return defaultValue;
}

bool MetadataSymbol::HasKey(const StringRef& key)
{
	auto itr = values.find(key);
	return (itr != values.end());
}

//==============================================================================
// MethodSymbol
//==============================================================================
void MethodSymbol::LinkParameters(SymbolDatabase* db)
{
	if (metadata->HasKey(_T("Event")))
	{
		Console::writeLine("is event");
	}

	for (auto& paramInfo : parameters)
	{
		paramInfo->type = db->findTypeInfo(paramInfo->src.typeRawName);

		// 今のところ、Class 型の 出力変数は許可しない
		if (paramInfo->isOut && paramInfo->type->IsClass())
		{
			paramInfo->isIn = true;
			paramInfo->isOut = false;
		}

		if (paramInfo->src.rawDefaultValue != nullptr)
		{
			String text = paramInfo->src.rawDefaultValue.get();
			if (text.contains(_T("::")))
			{
				auto tokens = text.split(_T("::"));
				Ref<TypeSymbol> dummy;
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
void MethodSymbol::ExpandCAPIParameters(SymbolDatabase* db)
{
	// 第1引数 (種類によって this など)
	{
		if (!isStatic)
		{
			if (owner->isStruct)
			{
				auto info = Ref<ParameterSymbol>::makeRef();
				info->name = owner->shortName().toLower();
				info->type = db->findTypeInfo(owner->fullName());
				info->isThis = true;
				capiParameters.add(info);
			}
			else if (isConstructor)
			{
				// none
			}
			else if (owner->isDelegate)
			{
				auto info = Ref<ParameterSymbol>::makeRef();
				info->name = _T("sender");
				info->type = PredefinedTypes::objectType;
				info->isThis = true;
				capiParameters.add(info);
			}
			else
			{
				auto info = Ref<ParameterSymbol>::makeRef();
				info->name = owner->shortName().toLower();
				info->type = db->findTypeInfo(owner->fullName());
				info->isThis = true;
				capiParameters.add(info);
			}
		}
	}

	// params
	for (auto& paramInfo : parameters)
	{
		capiParameters.add(paramInfo);
		//overloadSuffix += StringTraits::toUpper((Char)paramInfo->name[0]);
	}

	// return value
	if (returnType->isVoid || returnType == PredefinedTypes::EventConnectionType)
	{
		// "void", "EventConnection" は戻り値扱いしない
	}
	else
	{
		auto info = Ref<ParameterSymbol>::makeRef();
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
			auto info = Ref<ParameterSymbol>::makeRef();
			info->name = String::format(_T("out{0}"), owner->shortName());
			info->type = db->findTypeInfo(owner->fullName());
			info->isReturn = true;
			capiParameters.add(info);
		}
	}
}

String MethodSymbol::GetCAPIFuncName()
{
	String funcName;
	if (isConstructor)
	{
		funcName = _T("Create");
	}
	else
	{
		funcName = name;
		funcName[0] = StringTraits::toUpper<Char>(funcName[0]);	// 先頭大文字
	}

	String n = String::format(_T("LN{0}_{1}"), owner->shortName(), funcName);
	if (IsOverloadChild())
		n += metadata->getValue(MetadataSymbol::OverloadPostfixAttr);
	return n;
}

String MethodSymbol::GetCApiSetOverrideCallbackFuncName()
{
	return GetCAPIFuncName() + "_SetOverrideCaller";
}

String MethodSymbol::GetCApiSetOverrideCallbackTypeName()
{
	return String::format(_T("LN{0}_{1}_OverrideCaller"), owner->shortName(), name);
}

String MethodSymbol::GetAccessLevelName(AccessLevel accessLevel)
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

void MethodSymbol::link(SymbolDatabase* db)
{
	if (name == _T("initialize"))
	{
		isConstructor = true;
	}

	ExpandCAPIParameters(db);
}

//==============================================================================
// TypeSymbol
//==============================================================================
void TypeSymbol::Link(SymbolDatabase* db)
{
	MakeProperties();
	LinkOverload();
	ResolveCopyDoc();

	// find base class
	if (!src.baseClassRawName.isEmpty())
	{
		baseClass = db->findTypeInfo(src.baseClassRawName);
	}

	for (auto methodInfo : declaredMethods)
	{
		methodInfo->link(db);
	}
}

void TypeSymbol::setRawFullName(const String& value)
{
	src.rawFullName = value;

	int c = value.lastIndexOf(':');
	if (c >= 0)
		m_shortName = value.substring(c + 1);
	else
		m_shortName = value;
}

void TypeSymbol::MakeProperties()
{
	for (auto& methodInfo : declaredMethods)
	{
		if (methodInfo->metadata->HasKey(_T("Property")))
		{
			String name;
			String namePrefix;
			bool isGetter = false;
			if (methodInfo->name.indexOf(_T("get"), 0, CaseSensitivity::CaseInsensitive) == 0)
			{
				name = methodInfo->name.substring(3);
				isGetter = true;
			}
			else if (methodInfo->name.indexOf(_T("is"), 0, CaseSensitivity::CaseInsensitive) == 0)
			{
				name = methodInfo->name.substring(2);
				namePrefix = _T("is");
				isGetter = true;
			}
			else if (methodInfo->name.indexOf(_T("set"), 0, CaseSensitivity::CaseInsensitive) == 0)
			{
				name = methodInfo->name.substring(3);
				isGetter = false;
			}
			else
			{
				// 上記以外 (longName() など名詞系) は getter とする
				name = methodInfo->name;
				isGetter = true;
			}

			Ref<PropertySymbol> propInfo;
			{
				auto* ptr = declaredProperties.find([name](Ref<PropertySymbol> info) {return info->name == name; });
				if (ptr == nullptr)
				{
					propInfo = Ref<PropertySymbol>::makeRef();
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
				if (propInfo->type == nullptr)	// return 型をプロパティの型とする
					propInfo->type = methodInfo->returnType;
			}
			else
			{
				LN_DCHECK(propInfo->setter == nullptr);
				propInfo->setter = methodInfo;
				if (propInfo->type == nullptr)	// 第1引数の型をプロパティの型とする
					propInfo->type = methodInfo->parameters[0]->type;
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

void TypeSymbol::LinkOverload()
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

void TypeSymbol::ResolveCopyDoc()
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
// PropertySymbol
//==============================================================================
void PropertySymbol::MakeDocument()
{
	document = Ref<DocumentSymbol>::makeRef();

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
		}

		classInfo->Link(this);
	}

	// enums
	for (auto enumInfo : enums)
	{
		enumInfo->isEnum = true;
	}

	// delegates
	for (auto classInfo : delegates)
	{
		for (auto methodInfo : classInfo->declaredMethods)
		{
			methodInfo->returnType = findTypeInfo(methodInfo->returnTypeRawName);

			methodInfo->LinkParameters(this);
		}

		classInfo->Link(this);
	}
}

tr::Enumerator<Ref<MethodSymbol>> SymbolDatabase::GetAllMethods()
{
	List<Ref<MethodSymbol>> dummy;
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

void SymbolDatabase::FindEnumTypeAndValue(const String& typeFullName, const String& memberName, Ref<TypeSymbol>* outEnum, Ref<ConstantSymbol>* outMember)
{
	for (auto& enumInfo : enums)
	{
		if (enumInfo->fullName() == typeFullName)
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

	LN_ENSURE(0, "Undefined enum: %s::%s", typeFullName.c_str(), memberName.c_str());
}

Ref<ConstantSymbol> SymbolDatabase::CreateConstantFromLiteralString(const String& valueStr)
{
	auto info = Ref<ConstantSymbol>::makeRef();
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
	predefineds.add(Ref<TypeSymbol>::makeRef(_T("void")));
	predefineds.getLast()->isVoid = true;
	PredefinedTypes::voidType = predefineds.getLast();

	predefineds.add(Ref<TypeSymbol>::makeRef(_T("nullptr")));
	predefineds.getLast()->isVoid = true;
	PredefinedTypes::nullptrType = predefineds.getLast();

	predefineds.add(Ref<TypeSymbol>::makeRef(_T("bool")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::boolType = predefineds.getLast();

	predefineds.add(Ref<TypeSymbol>::makeRef(_T("int")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::intType = predefineds.getLast();

	predefineds.add(Ref<TypeSymbol>::makeRef(_T("int16_t")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::int16Type = predefineds.getLast();

	predefineds.add(Ref<TypeSymbol>::makeRef(_T("uint32_t")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::uint32Type = predefineds.getLast();

	predefineds.add(Ref<TypeSymbol>::makeRef(_T("float")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::floatType = predefineds.getLast();

	predefineds.add(Ref<TypeSymbol>::makeRef(_T("ln::String")));
	predefineds.getLast()->isPrimitive = true;
	PredefinedTypes::stringType = predefineds.getLast();

	predefineds.add(Ref<TypeSymbol>::makeRef(_T("ln::Object")));
	PredefinedTypes::objectType = predefineds.getLast();

	predefineds.add(Ref<TypeSymbol>::makeRef(_T("ln::EventConnection")));
	PredefinedTypes::EventConnectionType = predefineds.getLast();
}

// typeFullName : const や &, * は除かれていること
Ref<TypeSymbol> SymbolDatabase::findTypeInfo(StringRef typeFullName)
{
	Ref<TypeSymbol>* type;
	
	type = predefineds.find([typeFullName](Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
	if (type != nullptr) return *type;

	type = structs.find([typeFullName](Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
	if (type != nullptr) return *type;

	type = classes.find([typeFullName](Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
	if (type != nullptr) return *type;

	type = enums.find([typeFullName](Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
	if (type != nullptr) return *type;

	type = delegates.find([typeFullName](Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
	if (type != nullptr)
		return *type;

	// aliases
	if (typeFullName == _T("short")) return PredefinedTypes::int16Type;
	if (typeFullName == _T("ln::StringRef")) return PredefinedTypes::stringType;
	if (typeFullName == _T("ln::EventConnection")) return PredefinedTypes::EventConnectionType;

	LN_ENSURE(0, _T("Undefined type: %s"), String(typeFullName).c_str());
	return nullptr;
}

void SymbolDatabase::verify(DiagManager* diag)
{
}


