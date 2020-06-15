﻿
#include "Generator.hpp"

const ln::String Generator::NewLine = u"\n";

ln::Path Generator::makeFlatCHeaderOutputPath() const
{
    auto fileName = ln::String::format(u"{0}.FlatC.generated.h", config()->moduleName);
    return (config()->flatCHeaderOutputDirOverride.isEmpty()) ? makeOutputFilePath(u"FlatC/include", fileName) : ln::Path::combine(config()->flatCHeaderOutputDirOverride, fileName);
}

ln::String Generator::makeUpperSnakeName(const ln::String& name)
{
	// まず大文字境界で分割
	ln::List<ln::String> tokens;
	for (int i = 0; i < name.length(); i++) {
		if (isupper(name[i])) {
			tokens.add(u"");
		}
		tokens.back() += name[i];
	}
	// RGBA8 -> [A,G,B,A8]


	ln::String output = tokens[0];
	for (int i = 1; i < tokens.size(); i++) {
		auto last = tokens[i - 1][tokens[i - 1].length() - 1];
		if (isupper(last) || isdigit(last)) {
		}
		else {
			output += u"_";
		}
		output += tokens[i];
	}

	return output.toUpper();
}

ln::String Generator::camelToPascal(const ln::String& name)
{
	ln::String n = name;
	n[0] = toupper(n[0]);
	return n;
}

ln::String Generator::makeFlatClassName(const TypeSymbol* type) const
{
	return config()->flatCOutputModuleName + type->shortName();
}

ln::String Generator::makeFlatTypeName2(const TypeSymbol* type) const
{
    if (type->isString()) {
        return u"const char*";
    }
    else if (type->isPrimitive()) {
		if (type == PredefinedTypes::boolType) return u"LnBool";
		return type->shortName();
	}
    else if (type->isClass()) {
        return u"LnHandle";
    }
	else {
		return m_config->flatCOutputModuleName + type->shortName();
	}
	//return config()->flatCOutputModuleName + type->shortName();
}

ln::String Generator::makeFlatAPIName_SetManagedTypeInfoId(const TypeSymbol* type) const
{
	return makeFlatClassName(type) + u"_SetManagedTypeInfoId";
}

ln::String Generator::makeFlatAPIDecl_SetManagedTypeInfoId(const TypeSymbol* type) const
{
	return ln::String::format(u"LN_FLAT_API void {0}(int64_t id)", makeFlatAPIName_SetManagedTypeInfoId(type));
}

ln::String Generator::makeFlatAPIName_SetOverrideCallback(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const
{
	return ln::String::format(u"{0}_{1}_SetOverrideCallback", makeFlatClassName(leafClass), makeFlatShortFuncName(method, charset));
}

ln::String Generator::makeFlatAPIDecl_SetOverrideCallback(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const
{
	return ln::String::format(u"LN_FLAT_API LnResult {0}({1} callback)", makeFlatAPIName_SetOverrideCallback(leafClass, method, charset), makeFlatVirutalCallbackFuncPtrName(leafClass, method, charset));
}

ln::String Generator::makeFlatAPIName_CallOverrideBase(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const
{
	return ln::String::format(u"{0}_{1}_CallOverrideBase", makeFlatClassName(leafClass), makeFlatShortFuncName(method, charset));
}

ln::String Generator::makeFlatAPIDecl_CallOverrideBase(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const
{
	// make params
	OutputBuffer params;
	for (auto& paramInfo : method->flatParameters()) {
		params.AppendCommad("{0} {1}", makeFlatCParamQualTypeName(method, paramInfo, charset), paramInfo->name());
	}

	return ln::String::format(u"LN_FLAT_API LnResult {0}({1})", makeFlatAPIName_CallOverrideBase(leafClass, method, charset), params.toString());
}

ln::String Generator::makeFlatAPIName_OverrideFunc(const MethodSymbol* method, FlatCharset charset) const
{
	return ln::String::format(u"{0}_OverrideFunc", makeFlatShortFuncName(method, charset));
}

ln::String Generator::makeFlatAPIName_SubclassRegistrationInfo(const TypeSymbol* classSymbol) const
{
	return makeFlatClassName(classSymbol) + u"_SubclassRegistrationInfo";
}

ln::String Generator::makeFlatAPIName_RegisterSubclassTypeInfo(const TypeSymbol* classSymbol) const
{
	return makeFlatClassName(classSymbol) + u"_RegisterSubclassTypeInfo";
}

ln::String Generator::makeFlatAPIName_GetSubinstanceId(const TypeSymbol* classSymbol) const
{
	return makeFlatClassName(classSymbol) + u"_GetSubinstanceId";
}

ln::String Generator::makeFlatVirutalCallbackFuncPtrName(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const
{
	return ln::String::format(u"{0}_{1}_OverrideCallback", makeFlatClassName(leafClass), makeFlatShortFuncName(method, charset));
}

ln::String Generator::makeDelegateCallbackFuncPtrName(const TypeSymbol* delegateSymbol, FlatCharset charset) const
{
	auto funcName = delegateSymbol->shortName();
	funcName[0] = toupper(funcName[0]);
	return ln::String::format(u"{0}{1}Callback", config()->flatCOutputModuleName, funcName);
}

ln::String Generator::makeFlatShortFuncName(const MethodSymbol* method, FlatCharset charset) const
{
	ln::String funcName;
	if (method->isConstructor()) {
        if (method->ownerType()->isStruct()) {
            funcName = u"Set";
        }
        else {
            funcName = u"Create";
        }
	}
	else if (method->isPropertyGetter()) {
		// prefix を整える.
		//		components -> GetComponents
		//		isVisible -> isVisible
		auto prop = method->ownerProperty();
		auto prefix = prop->namePrefix().isEmpty() ? u"Get" : prop->namePrefix();
		if (method->shortName().indexOf(prefix, 0, ln::CaseSensitivity::CaseInsensitive) == 0)
			funcName = camelToPascal(method->shortName());
		else
			funcName = prefix + camelToPascal(method->shortName());
	}
	else {
		funcName = camelToPascal(method->shortName());
	}

	funcName += method->overloadPostfix();

	if (method->hasStringDecl()) {
		if (charset == FlatCharset::Ascii) {
			funcName += u"A";
		}
	}
	return funcName;
}

ln::String Generator::makeFlatFullFuncName(const MethodSymbol* method, FlatCharset charset) const
{
	return makeFlatFullFuncName(method->ownerType(), method, charset);
}

ln::String Generator::makeFlatFullFuncName(const TypeSymbol* classSymbol, const MethodSymbol* method, FlatCharset charset) const
{
	return ln::String::format(u"{0}{1}_{2}", m_config->flatCOutputModuleName, classSymbol->shortName(), makeFlatShortFuncName(method, charset));
}

// 宣言文の作成。ドキュメンテーションコメントは含まない。
ln::String Generator::makeFuncHeader(const MethodSymbol* methodInfo, FlatCharset charset) const
{
	static const ln::String funcHeaderTemplate =
		u"LN_FLAT_API LnResult %%FuncName%%(%%ParamList%%)";

	return funcHeaderTemplate
		.replace(u"%%FuncName%%", makeFlatFullFuncName(methodInfo, charset))
		.replace(u"%%ParamList%%", makeFlatParamList(methodInfo, charset));
}

ln::String Generator::makeFlatCParamQualTypeName(const MethodSymbol* methodInfo, const MethodParameterSymbol* paramInfo, FlatCharset charset) const
{
	bool methodIsConst = (methodInfo && methodInfo->isConst());
	auto typeInfo = paramInfo->type();

	if (typeInfo->kind() == TypeKind::Struct)
	{
		ln::String modifer;
		if (paramInfo->isThis() && methodIsConst)
			modifer = "const ";
		if (!paramInfo->isThis() && !paramInfo->isOut() && !paramInfo->isReturn())
			modifer = "const ";
		return ln::String::format("{0}{1}{2}*", modifer, m_config->flatCOutputModuleName, typeInfo->shortName());
	}
	else if (typeInfo->kind() == TypeKind::Enum)
	{
		if (paramInfo->isReturn())
			return ln::String::format(u"{0}*", makeFlatTypeName2(typeInfo));
		else
			return makeFlatTypeName2(typeInfo);
	}
	else if (typeInfo->isString())
	{
		if (paramInfo->isOut() || paramInfo->isReturn())
			return ln::String::format(u"const {0}**", makeFlatCharTypeName(charset));
		else
			return ln::String::format(u"const {0}*", makeFlatCharTypeName(charset));
	}
	else if (typeInfo->isDelegate())
	{
		return makeDelegateCallbackFuncPtrName(typeInfo, FlatCharset::Unicode);
	}
	else
	{
		ln::String name;
		if (typeInfo == PredefinedTypes::boolType)
			name = u"LnBool";
		else if (typeInfo->isObjectGroup())
			name = u"LnHandle";
		else
			name = typeInfo->shortName();

		if (paramInfo->isOut() || paramInfo->isReturn())
			name += u"*";

		//if (typeInfo->isStruct && paramInfo->isThis)
		//{
		//	if (methodInfo->isConst)
		//		name = "const " + name;
		//	name += "*";
		//}

		return name;
		//	ln::String name;
		//
		//	if (isOut)
		//	{
		//		name = typeInfo->name;
		//		name += "*";
		//	}
		//	else
		//	{
		//		if (typeInfo == PredefinedTypes::stringType)
		//			name = "const LNChar*";
		//		else if (typeInfo->IsClass())
		//			name = "LNHandle";
		//		else
		//			name = typeInfo->name;
		//	}
		//	return name;
	}
}

ln::String Generator::makeFlatCharTypeName(FlatCharset charset) const
{
	if (charset == FlatCharset::Unicode)
		return u"LnChar";
	else
		return u"char";
}

ln::String Generator::makeFlatParamList(const MethodSymbol* method, FlatCharset charset) const
{
	// make params
	OutputBuffer params;
	for (auto& paramInfo : method->flatParameters()) {
		params.AppendCommad(u"{0} {1}", makeFlatCParamQualTypeName(method, paramInfo, charset), paramInfo->name());
	}
	return params.toString();
}

// Native -> FlatC への呼び出し実引数リストを作成する。Native のコールバックから、FlatC のコールバックを呼び出すときなどに使用する。 
ln::String Generator::makeFlatArgList(const MethodSymbol* method) const
{
	OutputBuffer argList;
	for (auto& param : method->parameters()) {
		if (param->type()->isObjectGroup())
			if (param->qualType().strongReference)
				argList.AppendCommad(u"LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE({0})", param->name());
			else
				argList.AppendCommad(u"LNI_OBJECT_TO_HANDLE({0})", param->name());
		else
			argList.AppendCommad(param->name());
	}
	return argList.toString();
}

ln::String Generator::makeFlatEnumMemberName(TypeSymbol* enumType, ConstantSymbol* member) const
{
	return config()->flatCOutputModuleName.toUpper() + u"_" + Generator::makeUpperSnakeName(enumType->shortName()) + u"_" + Generator::makeUpperSnakeName(member->name());
}
