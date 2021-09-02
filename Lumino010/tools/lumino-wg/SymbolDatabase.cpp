
#include "SymbolDatabase.hpp"

ln::Ref<TypeSymbol>	PredefinedTypes::voidType;
ln::Ref<TypeSymbol>	PredefinedTypes::nullptrType;
ln::Ref<TypeSymbol>	PredefinedTypes::boolType;
ln::Ref<TypeSymbol>	PredefinedTypes::intType;
ln::Ref<TypeSymbol>	PredefinedTypes::int16Type;
ln::Ref<TypeSymbol>	PredefinedTypes::uint32Type;
ln::Ref<TypeSymbol>	PredefinedTypes::floatType;
ln::Ref<TypeSymbol>	PredefinedTypes::stringType;
ln::Ref<TypeSymbol>	PredefinedTypes::objectType;
ln::Ref<TypeSymbol>	PredefinedTypes::EventConnectionType;

//==============================================================================
// MetadataSymbol
//==============================================================================
const ln::String MetadataSymbol::OverloadPostfixAttr = _T("OverloadPostfix");

void MetadataSymbol::AddValue(const ln::String& key, const ln::String& value)
{
	values[key] = value;
}

ln::String* MetadataSymbol::FindValue(const ln::StringRef& key)
{
	auto itr = values.find(key);
	if (itr != values.end()) return &(itr->second);
	return nullptr;
}

ln::String MetadataSymbol::getValue(const ln::StringRef& key, const ln::String& defaultValue)
{
	auto* s = FindValue(key);
	if (s)
		return *s;
	else
		return defaultValue;
}

bool MetadataSymbol::HasKey(const ln::StringRef& key)
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
		printf("is event");	// TODO
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
			ln::String text = paramInfo->src.rawDefaultValue.value();
			if (text.contains(_T("::")))
			{
				auto tokens = text.split(_T("::"));
				ln::Ref<TypeSymbol> dummy;
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
				auto info = ln::makeRef<ParameterSymbol>();
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
				auto info = ln::makeRef<ParameterSymbol>();
				info->name = _T("sender");
				info->type = PredefinedTypes::objectType;
				info->isThis = true;
				capiParameters.add(info);
			}
			else
			{
				auto info = ln::makeRef<ParameterSymbol>();
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
		//overloadSuffix += ln::StringHelper::toUpper((Char)paramInfo->name[0]);
	}

	// return value
	if (returnType->isVoid || returnType == PredefinedTypes::EventConnectionType)
	{
		// "void", "EventConnection" は戻り値扱いしない
	}
	else
	{
		auto info = ln::makeRef<ParameterSymbol>();
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
			auto info = ln::makeRef<ParameterSymbol>();
			info->name = ln::String::format(_T("out{0}"), owner->shortName());
			info->type = db->findTypeInfo(owner->fullName());
			info->isReturn = true;
			capiParameters.add(info);
		}
	}
}

ln::String MethodSymbol::GetCAPIFuncName()
{
	ln::String funcName;
	if (isConstructor)
	{
		if (owner->isStruct)
		{
			if (isConstructor && parameters.isEmpty())
				funcName = _T("Clear");
			else
				funcName = _T("Set");
		}
		else
			funcName = _T("Create");
	}
	else
	{
		funcName = name;
		funcName[0] = ln::StringHelper::toUpper<ln::Char>(funcName[0]);	// 先頭大文字
	}

	ln::String n = ln::String::format(_T("LN{0}_{1}"), owner->shortName(), funcName);
	if (IsOverloadChild())
		n += metadata->getValue(MetadataSymbol::OverloadPostfixAttr);
	return n;
}

ln::String MethodSymbol::GetCApiSetOverrideCallbackFuncName()
{
	return GetCAPIFuncName() + "_SetOverrideCaller";
}

ln::String MethodSymbol::GetCApiSetOverrideCallbackTypeName()
{
	return ln::String::format(_T("LN{0}_{1}_OverrideCaller"), owner->shortName(), name);
}

ln::String MethodSymbol::GetAccessLevelName(AccessLevel accessLevel)
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
	if (!isStatic &&
		name == _T("initialize"))
	{
		isConstructor = true;
	}

	if (owner->isStruct &&
		owner->shortName() == name)
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
	// find base class
	if (!src.baseClassRawName.isEmpty())
	{
		baseClass = db->findTypeInfo(src.baseClassRawName);
	}

	for (auto methodInfo : declaredMethods)
	{
		methodInfo->link(db);
	}

	MakeProperties();
	LinkOverload(db);
	ResolveCopyDoc();

}

void TypeSymbol::setRawFullName(const ln::String& value)
{
	src.rawFullName = value;

	int c = value.lastIndexOf(':');
	if (c >= 0)
		m_shortName = value.substr(c + 1);
	else
		m_shortName = value;
}

void TypeSymbol::MakeProperties()
{
	for (auto& methodInfo : declaredMethods)
	{
		if (methodInfo->metadata->HasKey(_T("Property")))
		{
			ln::String name;
			ln::String namePrefix;
			bool isGetter = false;
			if (methodInfo->name.indexOf(_T("get"), 0, ln::CaseSensitivity::CaseInsensitive) == 0)
			{
				name = methodInfo->name.substr(3);
				isGetter = true;
			}
			else if (methodInfo->name.indexOf(_T("is"), 0, ln::CaseSensitivity::CaseInsensitive) == 0)
			{
				name = methodInfo->name.substr(2);
				namePrefix = _T("is");
				isGetter = true;
			}
			else if (methodInfo->name.indexOf(_T("set"), 0, ln::CaseSensitivity::CaseInsensitive) == 0)
			{
				name = methodInfo->name.substr(3);
				isGetter = false;
			}
			else
			{
				// 上記以外 (longName() など名詞系) は getter とする
				name = methodInfo->name;
				isGetter = true;
			}

			ln::Ref<PropertySymbol> propInfo;
			{
				auto ptr = declaredProperties.findIf([name](auto info) { return info->name == name; });
				if (ptr)
				{
					propInfo = ln::makeRef<PropertySymbol>();
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

void TypeSymbol::LinkOverload(SymbolDatabase* db)
{
	for (auto& methodInfo1 : declaredMethods)
	{
		if (!methodInfo1->overloadChildren.isEmpty() || methodInfo1->overloadParent)
			continue;	// processed

		// find same names
		ln::List<MethodSymbol*> sameNames;
		for (auto& methodInfo2 : declaredMethods)
		{
			if (isStruct && methodInfo2->isConstructor && methodInfo2->parameters.isEmpty())
			{
				// 構造体の引数無しコンストラクタは Clear という別名にしたい。
				continue;
			}

			if (methodInfo1->name == methodInfo2->name)
			{
				sameNames.add(methodInfo2);
			}
		}

		// select parent
		auto f = sameNames.findIf([](MethodSymbol* m) { return !m->metadata->HasKey(MetadataSymbol::OverloadPostfixAttr); });
		if (!f)
		{
			LN_NOTIMPLEMENTED();
		}
		MethodSymbol* parent = *f;

		// link
		for (auto& method2 : sameNames)
		{
			if (method2 == parent) continue;
			
			if (!method2->metadata->HasKey(MetadataSymbol::OverloadPostfixAttr))
			{
				//db->diag();
				LN_NOTIMPLEMENTED();
			}

			parent->overloadChildren.add(method2);
			method2->overloadParent = parent;
		}
	}
}

void TypeSymbol::ResolveCopyDoc()
{
	for (auto& methodInfo : declaredMethods)
	{
		if (methodInfo->document != nullptr && methodInfo->document->IsCopyDoc())
		{
			auto e = stream::MakeStream::from(declaredMethods)
				| stream::op::concat(stream::MakeStream::from(declaredMethodsForDocument));
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
	document = ln::makeRef<DocumentSymbol>();

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
SymbolDatabase::SymbolDatabase(ln::DiagnosticsManager* diag)
	: m_diag(diag)
{
}

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

		// ドキュメントとしてみるときのために、コンストラクタを先頭に出すようにする
		std::stable_sort(classInfo->declaredMethods.begin(), classInfo->declaredMethods.end(), [](MethodSymbol* lhs, MethodSymbol* rhs)
		{
			int lv = lhs->isConstructor ? 0 : 1;
			int rv = rhs->isConstructor ? 0 : 1;
			return lv < rv;
		});
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

//tr::Enumerator<ln::Ref<MethodSymbol>> SymbolDatabase::GetAllMethods()
//{
//	ln::List<ln::Ref<MethodSymbol>> dummy;
//	auto e = tr::MakeEnumerator::from(dummy);
//
//	for (auto& structInfo : structs)
//	{
//		e.Join(structInfo->declaredMethods);
//	}
//	for (auto& classInfo : classes)
//	{
//		e.Join(classInfo->declaredMethods);
//	}
//
//	return e;
//}

void SymbolDatabase::FindEnumTypeAndValue(const ln::String& typeFullName, const ln::String& memberName, ln::Ref<TypeSymbol>* outEnum, ln::Ref<ConstantSymbol>* outMember)
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

ln::Ref<ConstantSymbol> SymbolDatabase::CreateConstantFromLiteralString(const ln::String& valueStr)
{
	auto info = ln::makeRef<ConstantSymbol>();
	if (valueStr == "true")
	{
		info->type = PredefinedTypes::boolType;
		info->value = ln::makeVariant(true);
	}
	else if (valueStr == "false")
	{
		info->type = PredefinedTypes::boolType;
		info->value = ln::makeVariant(false);
	}
	else if (valueStr == "nullptr")
	{
		info->type = PredefinedTypes::nullptrType;
		info->value = ln::makeVariant(nullptr);
	}
	else if (valueStr.contains('.'))
	{
		info->type = PredefinedTypes::floatType;
		info->value = ln::makeVariant(ln::StringHelper::toFloat(valueStr.c_str()));
	}
	else
	{
		info->type = PredefinedTypes::intType;
		info->value = ln::makeVariant(ln::StringHelper::toInt32(valueStr.c_str()));
	}

	return info;
}

void SymbolDatabase::InitializePredefineds()
{
	predefineds.add(ln::makeRef<TypeSymbol>(_T("void")));
	predefineds.back()->isVoid = true;
	PredefinedTypes::voidType = predefineds.back();

	predefineds.add(ln::makeRef<TypeSymbol>(_T("nullptr")));
	predefineds.back()->isVoid = true;
	PredefinedTypes::nullptrType = predefineds.back();

	predefineds.add(ln::makeRef<TypeSymbol>(_T("bool")));
	predefineds.back()->isPrimitive = true;
	PredefinedTypes::boolType = predefineds.back();

	predefineds.add(ln::makeRef<TypeSymbol>(_T("int")));
	predefineds.back()->isPrimitive = true;
	PredefinedTypes::intType = predefineds.back();

	predefineds.add(ln::makeRef<TypeSymbol>(_T("int16_t")));
	predefineds.back()->isPrimitive = true;
	PredefinedTypes::int16Type = predefineds.back();

	predefineds.add(ln::makeRef<TypeSymbol>(_T("uint32_t")));
	predefineds.back()->isPrimitive = true;
	PredefinedTypes::uint32Type = predefineds.back();

	predefineds.add(ln::makeRef<TypeSymbol>(_T("float")));
	predefineds.back()->isPrimitive = true;
	PredefinedTypes::floatType = predefineds.back();

	predefineds.add(ln::makeRef<TypeSymbol>(_T("ln::String")));
	predefineds.back()->isPrimitive = true;
	PredefinedTypes::stringType = predefineds.back();

	predefineds.add(ln::makeRef<TypeSymbol>(_T("ln::Object")));
	PredefinedTypes::objectType = predefineds.back();

	predefineds.add(ln::makeRef<TypeSymbol>(_T("ln::EventConnection")));
	PredefinedTypes::EventConnectionType = predefineds.back();
}

// typeFullName : const や &, * は除かれていること
ln::Ref<TypeSymbol> SymbolDatabase::findTypeInfo(ln::StringRef typeFullName)
{
	ln::Optional<ln::Ref<TypeSymbol>> type;
	
	type = predefineds.findIf([typeFullName](ln::Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
	if (type != nullptr) return *type;

	type = structs.findIf([typeFullName](ln::Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
	if (type != nullptr) return *type;

	type = classes.findIf([typeFullName](ln::Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
	if (type != nullptr) return *type;

	type = enums.findIf([typeFullName](ln::Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
	if (type != nullptr) return *type;

	type = delegates.findIf([typeFullName](ln::Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
	if (type)
		return *type;

	// aliases
	if (typeFullName == _T("short")) return PredefinedTypes::int16Type;
	if (typeFullName == _T("ln::ln::StringRef")) return PredefinedTypes::stringType;
	if (typeFullName == _T("ln::EventConnection")) return PredefinedTypes::EventConnectionType;

	LN_ENSURE(0, "Undefined type: %s", ln::String(typeFullName).c_str());
	return nullptr;
}

//void SymbolDatabase::verify(DiagManager* diag)
//{
//}


