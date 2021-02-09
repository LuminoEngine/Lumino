
#include "SymbolDatabase.hpp"

ln::Ref<TypeSymbol>	PredefinedTypes::voidType;
ln::Ref<TypeSymbol>	PredefinedTypes::nullptrType;
ln::Ref<TypeSymbol>	PredefinedTypes::boolType;
ln::Ref<TypeSymbol>	PredefinedTypes::intType;
ln::Ref<TypeSymbol>	PredefinedTypes::int16Type;
ln::Ref<TypeSymbol>	PredefinedTypes::uint32Type;
ln::Ref<TypeSymbol>	PredefinedTypes::intptrType;
ln::Ref<TypeSymbol>	PredefinedTypes::floatType;
ln::Ref<TypeSymbol>	PredefinedTypes::doubleType;
ln::Ref<TypeSymbol>	PredefinedTypes::stringType;
ln::Ref<TypeSymbol>	PredefinedTypes::stringRefType;
//ln::Ref<TypeSymbol>	PredefinedTypes::objectType;
//ln::Ref<TypeSymbol>	PredefinedTypes::EventConnectionType;

//==============================================================================
// DocumentInfo

ln::Result ParameterDocumentInfo::init(const ln::String& name, const ln::String& io, const ln::String& desc)
{
    m_name = name;
    m_io = io;
    m_description = desc;
    return true;
}

ln::Result ParameterDocumentInfo::init(PIParamDocument* pi)
{
	LN_CHECK(pi);
	m_pi = pi;
    m_name = m_pi->name;
    m_io = m_pi->io;
    m_description = m_pi->description;
	return true;
}

//==============================================================================
// DocumentInfo

ln::Result DocumentInfo::init(const PIDocument* pi)
{
	LN_CHECK(pi);
	m_pi = pi;
	m_summary = m_pi->summary;
	m_returns = m_pi->returns;
	m_details = m_pi->details;

	for (auto& i : m_pi->params) {
		auto& s = ln::makeRef<ParameterDocumentInfo>();
		if (!s->init(i)) return false;
		m_params.add(s);
	}

	return true;
}

//ln::Result DocumentInfo::makeFlatParameters()
//{
//    m_flatParams
//    return false;
//}

//==============================================================================
// MetadataInfo

const ln::String MetadataInfo::OverloadPostfixAttr = u"OverloadPostfix";
const ln::String MetadataInfo::EventAttr = u"Event";

ln::Result MetadataInfo::init(PIMetadata* pi)
{
	m_pi = pi;
	if (m_pi) {
		m_name = m_pi->name;
		m_values = m_pi->values;
	}
	return true;
}

bool MetadataInfo::hasKey(const ln::StringRef& key) const
{
	auto itr = m_values.find(key);
	return (itr != m_values.end());
}

const ln::String* MetadataInfo::findValue(const ln::StringRef& key) const
{
	auto itr = m_values.find(key);
	if (itr != m_values.end())
		return &(itr->second);
	else
		return nullptr;
}

ln::String MetadataInfo::getValue(const ln::StringRef& key, const ln::String& defaultValue) const
{
	auto* s = findValue(key);
	if (s)
		return *s;
	else
		return defaultValue;
}

void MetadataInfo::setValue(const ln::StringRef& key, const ln::String& value)
{
	m_values[key] = value;
}

//==============================================================================
// Symbol

Symbol::Symbol(SymbolDatabase* db)
	: m_db(db)
{
	m_document = ln::makeRef<DocumentInfo>();
	m_metadata = ln::makeRef<MetadataInfo>();
	m_symbolId = db->generateSymbolId();
}

ln::Result Symbol::init()
{
	return true;
}

ln::Result Symbol::init(const PIDocument* document, PIMetadata* metadata)
{
	m_document = ln::makeRef<DocumentInfo>();
	if (!m_document->init(document)) return false;

	m_metadata = ln::makeRef<MetadataInfo>();
	if (!m_metadata->init(metadata)) return false;

	return true;
}

//==============================================================================
// MetadataInfo
//==============================================================================
//
//void MetadataSymbol::AddValue(const ln::String& key, const ln::String& value)
//{
//	values[key] = value;
//}

//==============================================================================
// FieldSymbol

FieldSymbol::FieldSymbol(SymbolDatabase* db)
	: Symbol(db)
{
}

ln::Result FieldSymbol::init(PIField* pi)
{
	if (!Symbol::init(pi->document, pi->metadata)) return false;
	LN_CHECK(pi);
	m_pi = pi;
	return true;
}

ln::Result FieldSymbol::link()
{
	m_type = db()->getTypeSymbol(m_pi->typeRawName);
	if (!m_type) return false;

	return true;
}

//==============================================================================
// ConstantSymbol

ConstantSymbol::ConstantSymbol(SymbolDatabase* db)
	: Symbol(db)
{
}

ln::Result ConstantSymbol::init(TypeSymbol* type, PIConstant* pi)
{
	if (!Symbol::init(pi->document, pi->metadata)) return false;
	LN_CHECK(pi);
	m_pi = pi;
	m_type = type;
	m_value = m_pi->value;
	return true;
}

ln::Result ConstantSymbol::init(TypeSymbol* type, ln::Variant* value)
{
	m_type = type;
	m_value = value;
	return true;
}

ln::Result ConstantSymbol::link()
{
	return true;
}

//==============================================================================
// MethodParameterSymbol

MethodParameterSymbol::MethodParameterSymbol(SymbolDatabase* db)
	: Symbol(db)
{
}

ln::Result MethodParameterSymbol::init(PIMethodParameter* pi)
{
	LN_CHECK(pi);
	m_pi = pi;
	m_name = m_pi->name;
	return true;
}

ln::Result MethodParameterSymbol::init(const QualType& qualType, const ln::String& name)
{
	LN_CHECK(qualType.type);
	LN_CHECK(!name.isEmpty());
	m_qualType = qualType;
	m_name = name;
	m_isIn = true;	// FIXME: いまのところ内部的に作られるのは in だけ
	return true;
}

ln::Result MethodParameterSymbol::link()
{
	if (m_pi) {
		m_qualType = db()->parseQualType(m_pi->typeRawName);
		if (!m_qualType.type) return false;

		// io direction
		if (type()->isObjectGroup()) {
			m_isIn = true;
		}
		else {
			// primitive, struct, enum ...
			if (!m_pi->isConst && m_pi->isPointer) {
				m_isOut = true;
			}
			else {
				m_isIn = true;
			}
		}

		// defaut value
		if (m_pi->defaultValue) {
			if (m_pi->defaultValue->type() == ln::VariantType::Bool) {
				m_defaultValue = ln::makeRef<ConstantSymbol>(db());
				if (!m_defaultValue->init(PredefinedTypes::boolType, m_pi->defaultValue)) return false;
			}
			else if (m_pi->defaultValue->isNumeric()) {
				if (type()->isEnum()) {
					m_defaultValue = ln::makeRef<ConstantSymbol>(db());
					if (!m_defaultValue->init(m_qualType.type, m_pi->defaultValue)) return false;
				}
				else if (m_pi->defaultValue->type() == ln::VariantType::Float ||
					m_pi->defaultValue->type() == ln::VariantType::Double) {
					m_defaultValue = ln::makeRef<ConstantSymbol>(db());
					if (!m_defaultValue->init(PredefinedTypes::floatType, m_pi->defaultValue)) return false;
				}
				else {
					m_defaultValue = ln::makeRef<ConstantSymbol>(db());
					if (!m_defaultValue->init(PredefinedTypes::intType, m_pi->defaultValue)) return false;
				}
			}
			else if (m_pi->defaultValue->type() == ln::VariantType::Null) {
				m_defaultValue = ln::makeRef<ConstantSymbol>(db());
				if (!m_defaultValue->init(PredefinedTypes::nullptrType, m_pi->defaultValue)) return false;
			}
			else {
				LN_NOTIMPLEMENTED();
				return false;
			}
		}
	}
	else {
		// 内部的に作成された Param は、あらかじめ型をセットしておく必要がある
		if (LN_REQUIRE(m_qualType.type)) return false;
	}
	return true;
}

ln::String MethodParameterSymbol::getFullQualTypeName() const
{
	ln::String name = m_qualType.type->fullName();
	if (m_qualType.type->isClass()) {
		name += u"*";
	}
	return name;
}

//==============================================================================
// MethodSymbol

MethodSymbol::MethodSymbol(SymbolDatabase* db)
	: Symbol(db)
{
}

ln::Result MethodSymbol::init(PIMethod* pi, TypeSymbol* ownerType)
{
	if (!Symbol::init(db()->resolveCopyDoc(pi->document), pi->metadata)) return false;
	LN_CHECK(pi);
	LN_CHECK(ownerType);
	m_ownerType = ownerType;
	m_pi = pi;
	m_accessLevel = accessLevelAsEnum(m_pi->accessLevel);
	m_shortName = m_pi->name;
	m_fullName = m_ownerType->fullName() + u"::" + m_shortName;

	for (auto& i : m_pi->parameters) {
		auto s = ln::makeRef<MethodParameterSymbol>(db());
		if (!s->init(i)) return false;
		m_parameters.add(s);
	}

    m_isConstructor = pi->isConstructor;

	m_isConst = m_pi->isConst;
	m_isStatic = m_pi->isStatic;
	m_isVirtual = m_pi->isVirtual;
	return true;
}

ln::Result MethodSymbol::init(TypeSymbol* ownerType, const ln::String& shortName, const QualType& returnType,/*TypeSymbol* returnType,*/ const ln::List<Ref<MethodParameterSymbol>>& params)
{
	if (!Symbol::init()) return false;
	m_ownerType = ownerType;
	m_accessLevel = AccessLevel::Public;
	m_shortName = shortName;
	m_fullName = m_ownerType->fullName() + u"::" + m_shortName;
	m_returnType = returnType;
	m_parameters = params;
	return true;
}

ln::Result MethodSymbol::initAsVirtualPrototype(TypeSymbol* ownerType, MethodSymbol* sourceMethod, TypeSymbol* delegateType)
{
	if (!Symbol::init()) return false;
	auto name = sourceMethod->shortName();
	name[0] = toupper(name[0]);

	m_ownerType = ownerType;	// LeafVirtuals からとっているので、sourceMethod->ownerType() は禁止
	m_accessLevel = AccessLevel::Public;
	m_shortName = u"setPrototype_" + name;
	m_fullName = m_ownerType->fullName() + u"::" + m_shortName;
	m_returnType = { PredefinedTypes::voidType, false };

	auto param = ln::makeRef<MethodParameterSymbol>(db());
	if (!param->init({ delegateType, false }, u"callback")) return false;
	m_parameters = { param };

	m_virtualPototypeSouceMethod = sourceMethod;
	m_virtualPototypeDelegate = delegateType;

	return link();
}

ln::Result MethodSymbol::link()
{
	if (!isStatic() && m_shortName == u"init") {
		m_isConstructor = true;
		m_accessLevel = AccessLevel::Public;
	}

	if (m_pi) {
		if (m_isConstructor) {
			m_returnType = QualType{ PredefinedTypes::voidType };
		}
		else {
			m_returnType = db()->parseQualType(m_pi->returnTypeRawName);
			if (!m_returnType.type) return false;
		}

		// std::function の alias などでは引数名が取れないことがあるため、名前を作っておく
		for (int i = 0; i < m_parameters.size(); i++) {
			if (m_parameters[i]->m_name.isEmpty()) {
				m_parameters[i]->m_name = ln::String::format(u"p{0}", i + 1);
			}
		}

		for (auto& p : m_parameters) {
			if (!p->link()) return false;
		}
	}

	if (!makeFlatParameters()) return false;

	//m_methodId = db()->generateMethodId();

	return true;
}

const MethodParameterSymbol* MethodSymbol::findFlatParameter(const ln::StringRef& name) const
{
	return m_flatParameters.findIf([&](auto& x) { return x->name() == name; }).valueOr(nullptr);
}

ln::Result MethodSymbol::makeFlatParameters()
{
    auto doc = document();
	doc->m_flatParams.clear();
	m_flatParameters.clear();

	// static でなければ、第1引数は this などになる
	if (!isStatic())
	{
        if (m_ownerType->kind() == TypeKind::Struct) {
            auto name = m_ownerType->shortName().toLower();
			auto s = ln::makeRef<MethodParameterSymbol>(db());
			if (!s->init(QualType{ m_ownerType }, name)) return false;
			s->m_isIn = true;
			s->m_isThis = true;
			s->m_flatParamIndex = doc->m_flatParams.size();
			m_flatParameters.add(s);

            // documetation
            auto param = ln::makeRef<ParameterDocumentInfo>();
			auto desc = ln::String::format(u"{0}", m_ownerType->shortName());
            if (!param->init(name, u"in", desc)) return false;
            doc->m_flatParams.add(param);
		}
		else if (isConstructor()) {
			// none
		}
        else if (m_ownerType->kind() == TypeKind::Class) {
            auto name = m_ownerType->shortName().toLower();
			auto s = ln::makeRef<MethodParameterSymbol>(db());
			if (!s->init(QualType{ m_ownerType }, name)) return false;
			s->m_isIn = true;
			s->m_isThis = true;
			s->m_flatParamIndex = doc->m_flatParams.size();
			m_flatParameters.add(s);

            // documetation
            auto param = ln::makeRef<ParameterDocumentInfo>();
			auto desc = ln::String::format(u"{0} のハンドル", m_ownerType->shortName());
            if (!param->init(name, u"in", desc)) return false;
            doc->m_flatParams.add(param);
		}
		else {
			LN_UNREACHABLE();
			return false;
		}
	}
	
	//if (m_ownerType->kind() == TypeKind::Delegate)
	//{
	//	auto s = ln::makeRef<MethodParameterSymbol>(db());
	//	if (!s->init(QualType{ db()->rootObjectClass() }, u"__eventOwner")) return false;
	//	m_flatParameters.add(s);

 //       // documetation
 //       auto param = ln::makeRef<ParameterDocumentInfo>();
 //       if (!param->init(u"eventOwner", u"in", "event owner")) return false;
 //       doc->m_flatParams.add(param);
	//}

	for (int iParam = 0; iParam < m_parameters.size(); iParam++) {
		auto param = m_parameters[iParam];
		m_flatParameters.add(param);

		auto paramDoc = doc->m_flatParams.findIf([param](auto& x) { return x->name() == param->name(); });
		if (paramDoc) {
			// 一致する DocComment が見つかったらインデックスを維持して追加
			param->m_flatParamIndex = doc->m_flatParams.size();
			doc->m_flatParams.add(*paramDoc);
		}
		else {
			// 見つからなかったらダミーを追加して続行
			auto paramDoc2 = ln::makeRef<ParameterDocumentInfo>();
			if (!paramDoc2->init(param->name(), u"", "")) return false;
			param->m_flatParamIndex = doc->m_flatParams.size();
			doc->m_flatParams.add(paramDoc2);
			std::cerr << "warning: " << param->name() << " parameter not commented." << std::endl;
		}
	}

	// return value
	if (m_returnType.type == PredefinedTypes::voidType/* || m_returnType.type == PredefinedTypes::EventConnectionType*/)
	{
		// "void", "EventConnection" は戻り値扱いしない
	}
	else
	{
		auto s = ln::makeRef<MethodParameterSymbol>(db());
		if (!s->init(m_returnType, u"outReturn")) return false;
		s->m_isIn = false;
		s->m_isOut = true;
		s->m_isReturn = true;
		s->m_flatParamIndex = doc->m_flatParams.size();
		m_flatParameters.add(s);

        // documetation
        auto param = ln::makeRef<ParameterDocumentInfo>();
		auto desc = ln::String::format(u"結果を格納する変数。", m_ownerType->shortName());
        if (!param->init(u"outReturn", u"out", desc)) return false;
        doc->m_flatParams.add(param);
	}

	// constructor
	if (isConstructor()) {
		if (m_ownerType->isStruct()) {
		}
		else {
            auto name = ln::String::format(u"out{0}", m_ownerType->shortName());
			auto s = ln::makeRef<MethodParameterSymbol>(db());
			if (!s->init(QualType{ m_ownerType }, name)) return false;
			s->m_isReturn = true;
			s->m_flatParamIndex = doc->m_flatParams.size();
			s->setIO(false, true);
			m_flatParameters.add(s);

            // documetation
            auto param = ln::makeRef<ParameterDocumentInfo>();
			auto desc = ln::String::format(u"作成された {0} のハンドルを格納する変数。", m_ownerType->shortName());
            if (!param->init(name, u"out", desc)) return false;
            doc->m_flatParams.add(param);
		}
	}

	// check string decl
	{
		m_hasStringDecl = m_flatParameters.containsIf([](auto& x) { return x->type()->isString(); });
	}

	return true;
}


#if 0
void MethodSymbol::LinkParameters(SymbolDatabase* db)
{
	if (metadata->HasKey(_T("Event")))
	{
		printf("is event");	// TODO
	}

	for (auto& paramInfo : parameters)
	{
		paramInfo->type = db->findSymbol(paramInfo->src.typeRawName);

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
				db->findEnumTypeAndValue(tokens[0], tokens[1], &dummy, &paramInfo->defaultValue);
			}
			else
			{
				paramInfo->defaultValue = db->createConstantFromLiteralString(text);
			}
		}
	}
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
#endif

//==============================================================================
// PropertySymbol

PropertySymbol::PropertySymbol(SymbolDatabase* db)
	: Symbol(db)
{
}

ln::Result PropertySymbol::init(const ln::String& shortName)
{
	Symbol::init();
	m_shortName = shortName;
	return true;
}

void PropertySymbol::buildDocument()
{
	ln::String summary;
	ln::String details;
	if (m_getter) {
		summary += m_getter->document()->summary();
		details += m_getter->document()->details();
	}
	if (m_setter) {
		if (!summary.isEmpty()) summary += u"\n";
		if (!details.isEmpty()) details += u"\n\n";

		summary += m_setter->document()->summary();
		details += m_setter->document()->details();
	}

	document()->setSummary(summary);
	document()->setDetails(details);
}

//==============================================================================
// TypeSymbol

TypeSymbol::TypeSymbol(SymbolDatabase* db)
	: Symbol(db)
{
}

ln::Result TypeSymbol::init(PITypeInfo* piType)
{
	if (!Symbol::init(piType->document, piType->metadata)) return false;

	LN_CHECK(piType);
	m_piType = piType;
	m_kind = kindAsEnum(m_piType->kind);
	m_typeClass = classAsEnum(m_piType->kind);
	setFullName(m_piType->rawFullName);

	if (m_piType->delegateProtoType) {
		m_functionSignature = ln::makeRef<MethodSymbol>(db());
		if (!m_functionSignature->init(m_piType->delegateProtoType, this)) {
			return false;
		}
	}

	for (auto& i : m_piType->fields) {
		auto s = ln::makeRef<FieldSymbol>(db());
		if (!s->init(i)) {
			return false;
		}
		m_fields.add(s);
	}

	for (auto& i : m_piType->constants) {
		auto s = ln::makeRef<ConstantSymbol>(db());
		if (!s->init(this, i)) {
			return false;
		}
		m_constants.add(s);
	}

	for (auto& i : m_piType->methods) {
		auto s = ln::makeRef<MethodSymbol>(db());
		if (!s->init(i, this)) {
			return false;
		}
		m_declaredMethods.add(s);
	}

    if (isRootObjectClass()) {
        db()->setRootObjectClass(this);
    }

	return true;
}

ln::Result TypeSymbol::init(const ln::String& primitveRawFullName, TypeKind typeKind, TypeClass typeClass)
{
	m_kind = typeKind;
	m_typeClass = typeClass;
	setFullName(primitveRawFullName);
	return true;
}

ln::Result TypeSymbol::initAsFunctionType(const ln::String& fullName, MethodSymbol* signeture)
{
	if (LN_REQUIRE(signeture)) return false;
	m_kind = TypeKind::Function;
	m_typeClass = TypeClass::None;
	m_functionSignature = signeture;
	setFullName(fullName);
	return true;
}

ln::Result TypeSymbol::initAsVirtualDelegate(const ln::String& fullName, MethodSymbol* signeture, TypeSymbol* virtualMethodDefinedClass)
{
	if (LN_REQUIRE(signeture)) return false;
	m_kind = TypeKind::Class;
	m_typeClass = TypeClass::DelegateObject;
	m_virtualDelegateType = true;
	setFullName(fullName);
	//if (!addDelegateConstructorMethod()) {
	//	return false;
	//}

	auto param1 = ln::makeRef<MethodParameterSymbol>(db());
	if (!param1->init({ virtualMethodDefinedClass, false }, u"self")) return false;

	// 第1引数に this を受け取るようにして、新しい signature とする
	ln::List<Ref<MethodParameterSymbol>> params = { param1 };
	params.addRange(signeture->parameters());
	m_functionSignature = ln::makeRef<MethodSymbol>(db());
	if (!m_functionSignature->init(this, signeture->shortName(), signeture->returnType(), params)) return false;

	return true;
}

ln::Result TypeSymbol::link()
{
	// "init" を、順序を保ちつつ先頭に移動する
	ln::List<Ref<MethodSymbol>> initMethods;
	for (int i = m_declaredMethods.size() - 1; i >= 0; i--) {
		if (m_declaredMethods[i]->shortName().indexOf(u"init") == 0) {
			initMethods.insert(0, m_declaredMethods[i]);
			m_declaredMethods.removeAt(i);
		}
	}
	m_declaredMethods.insertRange(0, initMethods);


	if (m_functionSignature) {
		if (!m_functionSignature->link()) {
			return false;
		}
	}

	createSpecialSymbols();

    ln::List<Ref<MethodSymbol>> additionalMethods;
	for (auto& i : m_fields) {
		if (!i->link()) {
			return false;
		}

        // getter と setter を作る。
        // Ruby のようにスクリプトコードから直接フィールドにアクセスできない Binding 生成のために使用する。
        {
            auto fieldGetter = ln::makeRef<MethodSymbol>(db());
            if (!fieldGetter->init(this, u"get" + i->name().toTitleCase(), { i->type(), false }, {})) {
                return false;
            }
            fieldGetter->m_linkedField = i;
            fieldGetter->metadata()->setValue(u"Property");
            additionalMethods.add(fieldGetter);
        }
        {
            auto p = ln::makeRef<MethodParameterSymbol>(db());
            if (!p->init(QualType{ i->type() }, u"value")) return false;

            auto fieldSetter = ln::makeRef<MethodSymbol>(db());
            if (!fieldSetter->init(this, u"set" + i->name().toTitleCase(), { PredefinedTypes::voidType, false }, { p })) {
                return false;
            }
            fieldSetter->m_linkedField = i;
            fieldSetter->metadata()->setValue(u"Property");
            additionalMethods.add(fieldSetter);
        }
	}
    m_declaredMethods.insertRange(0, additionalMethods);

	for (auto& i : m_constants) {
		if (!i->link()) {
			return false;
		}
	}
	for (auto& i : m_declaredMethods) {
		if (!i->link()) {
			return false;
		}
		if (i->accessLevel() == AccessLevel::Public) {
			m_publicMethods.add(i);
		}
	}

	if (m_piType) {
        if (isRootObjectClass()) {
            // No base class required.
        }
		else if (isClass() && !isStatic()) {
			m_baseClass = db()->findTypeSymbol(m_piType->baseClassRawName);
			if (!m_baseClass) {
				m_baseClass = db()->rootObjectClass();
			}
			if (LN_REQUIRE(m_baseClass->kind() == TypeKind::Class)) return false;
		}
	}
	else {
		// predefined type.
	}

	if (!linkOverload()) return false;

	if (!linkProperties()) return false;

	if (typeClass() == TypeClass::None) {
		collectLeafVirtualMethods(&m_leafVirtualMethods);
	}

	for (auto& method : m_declaredMethods) {
		if (method->isVirtual()) {
			m_virtualMethods.add(method);
		}
	}

	return true;
}

void TypeSymbol::setFullName(const ln::String& value)
{
	m_fullName = value;

	int c = value.lastIndexOf(':');
	if (c >= 0)
		m_shortName = value.substr(c + 1);
	else
		m_shortName = value;
}

ln::Result TypeSymbol::linkOverload()
{
	// Not required for delegates
	//if (isDelegate()) return true;

	for (auto& method1 : m_declaredMethods)
	{
		auto info = m_overloads.findIf([&method1](auto& x) { return x->m_methods[0]->shortName() == method1->shortName(); });
		if (info) {
			(*info)->m_methods.add(method1);
			method1->m_overloadInfo = (*info);
		}
		else {
			auto newInfo = ln::makeRef<MethodOverloadInfo>();
			newInfo->m_methods.add(method1);
			m_overloads.add(newInfo);
			method1->m_overloadInfo = newInfo;
		}
	}

	for (auto& overload : m_overloads) {
		for (int i = 0; i < overload->m_methods.size(); i++) {
			// OverloadPostfix を持っていないものを代表として採用する
			if (!overload->m_methods[i]->metadata()->hasKey(MetadataInfo::OverloadPostfixAttr)) {
				if (overload->m_representativeIndex >= 0) {
					// 複数の代表が見つかってしまった
					// verify: representative 以外は OverloadPostfix を持っていなければならない
					LN_NOTIMPLEMENTED();
					return false;
				}
				else {
					overload->m_representativeIndex = i;
				}
			}
		}
		if (overload->m_representativeIndex < 0) {
			// 代表が見つからなかったら 0 番を採用
			overload->m_representativeIndex = 0;
		}
	}

	return true;
}

ln::Result TypeSymbol::linkProperties()
{
	for (auto& methodInfo : m_declaredMethods)
	{
		if (methodInfo->metadata()->hasKey(u"Property"))
		{
			// プロパティの名前を推測する
			ln::String name;
			ln::String namePrefix;
			bool isGetter = false;
			if (methodInfo->shortName().indexOf(u"get", 0, ln::CaseSensitivity::CaseInsensitive) == 0) {
				name = methodInfo->shortName().substr(3);
				isGetter = true;
			}
			else if (methodInfo->shortName().indexOf(u"is", 0, ln::CaseSensitivity::CaseInsensitive) == 0) {
				name = methodInfo->shortName().substr(2);
				namePrefix = u"is";
				isGetter = true;
			}
			else if (methodInfo->shortName().indexOf(u"set", 0, ln::CaseSensitivity::CaseInsensitive) == 0) {
				name = methodInfo->shortName().substr(3);
				isGetter = false;
			}
			else {
				// 上記以外 (longName() など名詞系) は getter とする
				name = methodInfo->shortName();
				isGetter = true;
			}

			// 推測した名前で既存プロパティを検索する。なければ新しく作る
			ln::Ref<PropertySymbol> prop;
			{
				auto ptr = m_properties.findIf([name](auto& x) { return x->shortName() == name; });
				if (!ptr) {
					prop = ln::makeRef<PropertySymbol>(db());
					if (!prop->init(name)) return false;
					m_properties.add(prop);
				}
				else {
					prop = *ptr;
				}

				prop->m_namePrefix = namePrefix;
			}

			if (isGetter) {
				LN_DCHECK(!prop->m_getter);
				prop->m_getter = methodInfo;
				if (!prop->m_type)	// return 型をプロパティの型とする
					prop->m_type = methodInfo->returnType().type;
			}
			else {
				LN_DCHECK(!prop->m_setter);
				prop->m_setter = methodInfo;
				if (!prop->m_type)	// 第1引数の型をプロパティの型とする
					prop->m_type = methodInfo->parameters()[0]->qualType().type;
			}

			methodInfo->m_ownerProperty = prop;
		}
	}

	// create document
	for (auto& prop : m_properties) {
		prop->buildDocument();
	}

	return true;
}

ln::Result TypeSymbol::createSpecialSymbols()
{
	if (isCollection()) {
		auto typeName = metadata()->getValue(u"Collection");
		m_collectionItemType = db()->getTypeSymbol(typeName);
		if (!m_collectionItemType) {
			db()->diag()->reportError(u"undefined symbol: " + typeName);
			return false;
		}

		{
			auto s = ln::makeRef<MethodSymbol>(db());
			if (!s->init(this, u"getLength", { PredefinedTypes::intType, false }, {})) return false;
			m_declaredMethods.add(s);
		}
		{
			auto p = ln::makeRef<MethodParameterSymbol>(db());
			if (!p->init({ PredefinedTypes::intType }, u"index")) return false;
			auto s = ln::makeRef<MethodSymbol>(db());
			if (!s->init(this, u"getItem", { m_collectionItemType, false }, { p })) return false;
			s->metadata()->setValue(u"Collection_GetItem");
			m_declaredMethods.add(s);
		}
	}

	if (isDelegateObject()) {
		addDelegateConstructorMethod();
	}

	if (isPromise()) {
		// thenWith()
		// Note: "then" だけだと言語によってはキーワードや組み込み関数と衝突する。例えば Ruby だと Object.then.
		{
			auto paramType = db()->parseQualType(m_piType->templateArguments[0]->typeRawName);
			auto delegateType = db()->findDelegateObjectFromSigneture({ PredefinedTypes::voidType, false }, { paramType });
			if (!delegateType) {
				db()->diag()->reportError(ln::String::format(u"Delegate required by Promise is not defined. ({0})", fullName()));
				return false;
			}

			auto p = ln::makeRef<MethodParameterSymbol>(db());
			if (!p->init({ delegateType, false }, u"callback")) return false;

			auto s = ln::makeRef<MethodSymbol>(db());
			if (!s->init(this, u"thenWith", { PredefinedTypes::voidType, false }, { p })) return false;
			m_declaredMethods.add(s);
		}
		// catchWith()
		{
			auto delegateType = db()->findTypeSymbol(u"ln::PromiseFailureDelegate");

			auto p = ln::makeRef<MethodParameterSymbol>(db());
			if (!p->init({ delegateType, false }, u"callback")) return false;

			auto s = ln::makeRef<MethodSymbol>(db());
			if (!s->init(this, u"catchWith", { PredefinedTypes::voidType, false }, { p })) return false;
			m_declaredMethods.add(s);
		}
	}

	return true;
}

void TypeSymbol::collectLeafVirtualMethods(ln::List<Ref<MethodSymbol>>* virtualMethods)
{
	if (m_baseClass) {
		m_baseClass->collectLeafVirtualMethods(virtualMethods);
	}

	for (auto& method : m_declaredMethods) {
		if (method->isVirtual()) {
			int index = virtualMethods->indexOfIf([&](auto& x) { return x->shortName() == method->shortName(); });
			if (index >= 0) {
				virtualMethods->at(index) = method;	// override
			}
			else {
				virtualMethods->add(method);
			}
		}
	}
}

bool TypeSymbol::addDelegateConstructorMethod()
{
	auto functonType = ln::makeRef<TypeSymbol>(db());
	if (!functonType->initAsFunctionType(fullName() + u"_Function", m_functionSignature)) return false;
	db()->registerTypeSymbol(functonType);

	auto param = ln::makeRef<MethodParameterSymbol>(db());
	if (!param->init({ functonType, false }, u"callback")) return false;

	auto s = ln::makeRef<MethodSymbol>(db());
	if (!s->init(this, u"init", { PredefinedTypes::voidType, false }, { param })) return false;
	m_declaredMethods.add(s);

	return true;
}

bool TypeSymbol::createOverridePrototype(MethodSymbol* sourceMethod, TypeSymbol* delegateType)
{
	if (LN_REQUIRE(sourceMethod)) return false;
	if (LN_REQUIRE(delegateType)) return false;

	auto method = ln::makeRef<MethodSymbol>(db());
	if (!method->initAsVirtualPrototype(this, sourceMethod, delegateType)) return false;

	//m_declaredMethods.add(method);
	m_vitualPrototypeSetters.add(method);

	return true;
}

#if 0
void TypeSymbol::Link(SymbolDatabase* db)
{
	// find base class
	if (!src.baseClassRawName.isEmpty())
	{
		baseClass = db->findSymbol(src.baseClassRawName);
	}

	for (auto methodInfo : declaredMethods)
	{
		methodInfo->link(db);
	}

	MakeProperties();
	LinkOverload(db);
	ResolveCopyDoc();

}


void TypeSymbol::MakeProperties()
{
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
#endif


//==============================================================================
// SymbolDatabase

SymbolDatabase::SymbolDatabase(ln::DiagnosticsManager* diag)
	: m_diag(diag)
{
}

ln::Result SymbolDatabase::initTypes(PIDatabase* pidb)
{
	LN_CHECK(pidb);
	m_pidb = pidb;
	initPredefineds();

	for (auto& t : m_pidb->types) {
		auto symbol = ln::makeRef<TypeSymbol>(this);
		if (!symbol->init(t)) {
			return false;
		}
		m_allTypes.add(symbol);
	}

	return true;
}

ln::Result SymbolDatabase::linkTypes()
{
	int count = m_allTypes.size();
	for (int i = 0; i < count; i++) {	// iterate 中に m_allTypes へ新しい Type が add されることもある
		if (!m_allTypes[i]->link()) return false;
	}

	// prototype で仮想関数をオーバーライドする場合、Delegate によってコールバックを登録したい。
	// その Delegate を自動生成する。
	for (int i = 0; i < count; i++) {
		for (auto& method : m_allTypes[i]->leafVirtualMethods()) {
			auto className = method->shortName();
			if (className.indexOf(u"on") == 0) {
				className = className.substr(2);	// onUpdate のような先頭の "on" を除外
			}
			className = m_allTypes[i]->shortName() + className + u"Handler";

			auto d = ln::makeRef<TypeSymbol>(this);
			if (!d->initAsVirtualDelegate(className, method, m_allTypes[i])) return false;
			if (!d->link()) return false;
			m_allTypes.add(d);

			m_allTypes[i]->createOverridePrototype(method, d);
		}
	}

	return true;
}

const PIDocument* SymbolDatabase::resolveCopyDoc(const PIDocument* pi) const
{
	if (pi->copydocLocalSignature.isEmpty())
		return pi;
	else
		return m_pidb->findRelativeDocument(pi->copydocLocalSignature);
}

void SymbolDatabase::initPredefineds()
{
	auto addPredefined = [this](const ln::String& name, TypeKind kind = TypeKind::Primitive) {
		auto t = ln::makeRef<TypeSymbol>(this);
		t->init(name, kind, TypeClass::None);
		m_allTypes.add(t);
		return t;
	};
		
	PredefinedTypes::voidType = addPredefined(u"void");
	
	PredefinedTypes::nullptrType = addPredefined(u"nullptr_t");

	PredefinedTypes::boolType = addPredefined(u"bool");

	PredefinedTypes::intType = addPredefined(u"int");

	PredefinedTypes::int16Type = addPredefined(u"int16_t");

	PredefinedTypes::uint32Type = addPredefined(u"uint32_t");

	PredefinedTypes::intptrType = addPredefined(u"intptr_t");

	PredefinedTypes::floatType = addPredefined(u"float");

	PredefinedTypes::doubleType = addPredefined(u"double");

	PredefinedTypes::stringType = addPredefined(u"ln::String");

	PredefinedTypes::stringRefType = addPredefined(u"ln::StringRef");

	//PredefinedTypes::objectType = addPredefined(u"ln::Object", TypeKind::Class);

	//PredefinedTypes::EventConnectionType = addPredefined(u"ln::EventConnection");
}

TypeSymbol* SymbolDatabase::findTypeSymbol(const ln::String& typeFullName) const
{
	auto type = m_allTypes.findIf([typeFullName](ln::Ref<TypeSymbol> type) { return type->fullName() == typeFullName; });
	if (type)
		return *type;
	else
		return nullptr;
}

TypeSymbol* SymbolDatabase::findDelegateObjectFromSigneture(const QualType& returnType, const ln::List<QualType>& paramTypes) const
{
	for (auto& d : delegateObjects()) {
		if (QualType::equals(d->delegateProtoType()->returnType(), returnType) &&
			d->delegateProtoType()->parameters().size() == paramTypes.size()) {
			bool matched = true;
			for (int i = 0; i < paramTypes.size(); i++) {
				if (!QualType::equals(d->delegateProtoType()->parameters()[i]->qualType(), paramTypes[i])) {
					matched = false;
					break;
				}
			}
			if (matched) {
				return d;
			}
		}
	}
	return nullptr;
}

TypeSymbol* SymbolDatabase::getTypeSymbol(const ln::String& typeFullName) const
{
	auto type = findTypeSymbol(typeFullName);
	if (!type)
		m_diag->reportError(u"Undefined type : " + typeFullName);
	return type;
}

QualType SymbolDatabase::parseQualType(const ln::String& rawTypeName) const
{
	QualType qt;

	if (rawTypeName.indexOf(u"Ref<") == 0) {
		auto name = rawTypeName.substr(4, rawTypeName.length() - 5);
		qt.type = getTypeSymbol(name);
		qt.strongReference = true;
	}
	else {
		qt.type = getTypeSymbol(rawTypeName);
		qt.strongReference = false;
	}

	return qt;
}

void SymbolDatabase::registerTypeSymbol(TypeSymbol* type)
{
	m_allTypes.add(type);
}
