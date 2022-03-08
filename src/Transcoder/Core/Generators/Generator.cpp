
#include "Generator.hpp"

const ln::String Generator::NewLine = U"\n";

ln::Path Generator::makeFlatCHeaderOutputPath() const {
    auto fileName = ln::format(U"{0}.FlatC.generated.h", config()->moduleName);
    return (config()->flatCHeaderOutputDirOverride.isEmpty()) ? makeOutputFilePath(U"FlatC/include", fileName) : ln::Path::combine(config()->flatCHeaderOutputDirOverride, fileName);
}

ln::String Generator::makeUpperSnakeName(const ln::String& name) {
    // まず大文字境界で分割
    ln::List<ln::String> tokens;
    for (int i = 0; i < name.length(); i++) {
        if (isupper(name[i])) {
            tokens.add(U"");
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
            output += U"_";
        }
        output += tokens[i];
    }

    return output.toUpper();
}

ln::String Generator::camelToPascal(const ln::String& name) {
    ln::String n = name;
    n[0] = toupper(n[0]);
    return n;
}

ln::String Generator::makeFlatClassName(const TypeSymbol* type) const {
    return config()->flatCOutputModuleName + type->shortName();
}

ln::String Generator::makeFlatTypeName2(const TypeSymbol* type) const {
    if (type->isString()) {
        return U"const char*";
    }
    else if (type->isPrimitive()) {
        if (type == PredefinedTypes::boolType) return U"LNBool";
        return type->shortName();
    }
    else if (type->isClass()) {
        return U"LNHandle";
    }
    else {
        return m_config->flatCOutputModuleName + type->shortName();
    }
    //return config()->flatCOutputModuleName + type->shortName();
}

ln::String Generator::makeFlatAPIName_SetManagedTypeInfoId(const TypeSymbol* type) const {
    return makeFlatClassName(type) + U"_SetManagedTypeInfoId";
}

ln::String Generator::makeFlatAPIDecl_SetManagedTypeInfoId(const TypeSymbol* type) const {
    return ln::format(U"LN_FLAT_API void {0}(int64_t id)", makeFlatAPIName_SetManagedTypeInfoId(type));
}

ln::String Generator::makeFlatAPIName_SetOverrideCallback(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const {
    return ln::format(U"{0}_{1}_SetOverrideCallback", makeFlatClassName(leafClass), makeFlatShortFuncName(method, charset));
}

ln::String Generator::makeFlatAPIDecl_SetOverrideCallback(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const {
    return ln::format(U"LN_FLAT_API LNResult {0}({1} callback)", makeFlatAPIName_SetOverrideCallback(leafClass, method, charset), makeFlatVirutalCallbackFuncPtrName(leafClass, method, charset));
}

ln::String Generator::makeFlatAPIName_CallOverrideBase(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const {
    return ln::format(U"{0}_{1}_CallOverrideBase", makeFlatClassName(leafClass), makeFlatShortFuncName(method, charset));
}

ln::String Generator::makeFlatAPIDecl_CallOverrideBase(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const {
    // make params
    OutputBuffer params;
    for (auto& paramInfo : method->flatParameters()) {
        params.AppendCommad(U"{0} {1}", makeFlatCParamQualTypeName(method, paramInfo, charset), paramInfo->name());
    }

    return ln::format(U"LN_FLAT_API LNResult {0}({1})", makeFlatAPIName_CallOverrideBase(leafClass, method, charset), params.toString());
}

ln::String Generator::makeFlatAPIName_OverrideFunc(const MethodSymbol* method, FlatCharset charset) const {
    return ln::format(U"{0}_OverrideFunc", makeFlatShortFuncName(method, charset));
}

ln::String Generator::makeFlatAPIName_SubclassRegistrationInfo(const TypeSymbol* classSymbol) const {
    return makeFlatClassName(classSymbol) + U"_SubclassRegistrationInfo";
}

ln::String Generator::makeFlatAPIName_RegisterSubclassTypeInfo(const TypeSymbol* classSymbol) const {
    return makeFlatClassName(classSymbol) + U"_RegisterSubclassTypeInfo";
}

ln::String Generator::makeFlatAPIName_GetSubinstanceId(const TypeSymbol* classSymbol) const {
    return makeFlatClassName(classSymbol) + U"_GetSubinstanceId";
}

ln::String Generator::makeFlatAPIName_OverridePrototypesStruct(const TypeSymbol* classSymbol) const {
    return makeFlatClassName(classSymbol) + U"_OverridePrototypes";
}

//// deprecated
//ln::String Generator::makeFlatAPIName_SetPrototype(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const
//{
//	return ln::String::format(U"{0}_SetPrototype_{1}", makeFlatClassName(leafClass), makeFlatShortFuncName(method, charset));
//}
//
//// deprecated
//ln::String Generator::makeFlatAPIDecl_SetPrototype(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const
//{
//	return ln::String::format(U"LN_FLAT_API LNResult {0}(LNHandle {1}, {2} func)",
//		makeFlatAPIName_SetPrototype(leafClass, method, charset),
//		method->flatParameters()[0]->name(),
//		makeFlatVirutalCallbackFuncPtrName(leafClass, method, FlatCharset::Unicode));
//}

ln::String Generator::makeFlatVirutalCallbackFuncPtrName(const TypeSymbol* leafClass, const MethodSymbol* method, FlatCharset charset) const {
    return ln::format(U"{0}_{1}_OverrideCallback", makeFlatClassName(leafClass), makeFlatShortFuncName(method, charset));
}

ln::String Generator::makeDelegateCallbackFuncPtrName(const TypeSymbol* delegateSymbol, FlatCharset charset) const {
    auto funcName = delegateSymbol->shortName();
    funcName[0] = toupper(funcName[0]);
    return ln::format(U"{0}{1}Callback", config()->flatCOutputModuleName, funcName);
}

ln::String Generator::makeFlatShortFuncName(const MethodSymbol* method, FlatCharset charset) const {
    ln::String funcName;
    if (method->isConstructor()) {
        if (method->ownerType()->isStruct()) {
            funcName = U"Set";
        }
        else {
            funcName = U"Create";
        }
    }
    else if (method->isPropertyGetter()) {
        // prefix を整える.
        //		components -> GetComponents
        //		isVisible -> isVisible
        auto prop = method->ownerProperty();
        auto prefix = prop->namePrefix().isEmpty() ? U"Get" : prop->namePrefix();
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
            funcName += U"A";
        }
    }
    return funcName;
}

ln::String Generator::makeFlatFullFuncName(const MethodSymbol* method, FlatCharset charset) const {
    return makeFlatFullFuncName(method->ownerType(), method, charset);
}

ln::String Generator::makeFlatFullFuncName(const TypeSymbol* classSymbol, const MethodSymbol* method, FlatCharset charset) const {
    return ln::format(U"{0}{1}_{2}", m_config->flatCOutputModuleName, classSymbol->shortName(), makeFlatShortFuncName(method, charset));
}

// 宣言文の作成。ドキュメンテーションコメントは含まない。
ln::String Generator::makeFuncHeader(const MethodSymbol* methodInfo, FlatCharset charset) const {
    static const ln::String funcHeaderTemplate =
        U"LN_FLAT_API LNResult %%FuncName%%(%%ParamList%%)";

    return funcHeaderTemplate
        .replace(U"%%FuncName%%", makeFlatFullFuncName(methodInfo, charset))
        .replace(U"%%ParamList%%", makeFlatParamList(methodInfo, charset));
}

ln::String Generator::makeFlatCParamQualTypeName(const MethodSymbol* methodInfo, const MethodParameterSymbol* paramInfo, FlatCharset charset) const {
    bool methodIsConst = (methodInfo && methodInfo->isConst());
    auto typeInfo = paramInfo->type();

    if (typeInfo->kind() == TypeKind::Struct) {
        ln::String modifer;
        if (paramInfo->isThis() && methodIsConst)
            modifer = U"const ";
        if (!paramInfo->isThis() && !paramInfo->isOut() && !paramInfo->isReturn())
            modifer = U"const ";
        return ln::format(U"{0}{1}{2}*", modifer, m_config->flatCOutputModuleName, typeInfo->shortName());
    }
    else if (typeInfo->kind() == TypeKind::Enum) {
        if (paramInfo->isReturn())
            return ln::format(U"{0}*", makeFlatTypeName2(typeInfo));
        else
            return makeFlatTypeName2(typeInfo);
    }
    else if (typeInfo->isString()) {
        if (paramInfo->isOut() || paramInfo->isReturn())
            return ln::format(U"const {0}**", makeFlatCharTypeName(charset));
        else
            return ln::format(U"const {0}*", makeFlatCharTypeName(charset));
    }
    //else if (typeInfo->isDelegate())
    //{
    //	return makeDelegateCallbackFuncPtrName(typeInfo, FlatCharset::Unicode);
    //}
    else {
        ln::String name;
        if (typeInfo == PredefinedTypes::boolType)
            name = U"LNBool";
        else if (typeInfo->isObjectGroup())
            name = U"LNHandle";
        else
            name = typeInfo->shortName();

        if (paramInfo->isOut() || paramInfo->isReturn())
            name += U"*";

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

ln::String Generator::makeFlatCharTypeName(FlatCharset charset) const {
    if (charset == FlatCharset::Unicode)
        return U"LNChar";
    else
        return U"char";
}

ln::String Generator::makeFlatParamList(const MethodSymbol* method, FlatCharset charset) const {
    // make params
    OutputBuffer params;
    for (auto& paramInfo : method->flatParameters()) {
        params.AppendCommad(U"{0} {1}", makeFlatCParamQualTypeName(method, paramInfo, charset), paramInfo->name());
    }
    return params.toString();
}

// Native -> FlatC への呼び出し実引数リストを作成する。Native のコールバックから、FlatC のコールバックを呼び出すときなどに使用する。
ln::String Generator::makeFlatArgList(const MethodSymbol* method) const {
    OutputBuffer argList;
    for (auto& param : method->parameters()) {
        if (param->type()->isObjectGroup())
            if (param->qualType().strongReference)
                argList.AppendCommad(U"LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE({0})", param->name());
            else
                argList.AppendCommad(U"LNI_OBJECT_TO_HANDLE({0})", param->name());
        else
            argList.AppendCommad(param->name());
    }
    return argList.toString();
}

ln::String Generator::makeFlatConstantValue(const ConstantSymbol* constant) const {
    if (constant->type()->isEnum()) {
        return makeFlatEnumMemberNameFromValue(constant);
        //return ln::String::format(U"({0}){1}", makeFlatClassName(constant->type()), ln::String::fromNumber(constant->value()->get<int>()));
    }
    else if (constant->type() == PredefinedTypes::boolType) {
        return constant->value()->get<bool>() ? U"LN_TRUE" : U"LN_FALSE";
    }
    else if (constant->type() == PredefinedTypes::floatType) {
        return ln::String::fromNumber(constant->value()->get<float>());
    }
    else if (constant->type() == PredefinedTypes::nullptrType) {
        return U"LN_NULL_HANDLE";
    }
    else {
        return ln::String::fromNumber(constant->value()->get<int>());
    }
}

ln::String Generator::makeFlatEnumMemberName(const TypeSymbol* enumType, const ConstantSymbol* member) const {
    return config()->flatCOutputModuleName.toUpper() + U"_" + Generator::makeUpperSnakeName(enumType->shortName()) + U"_" + Generator::makeUpperSnakeName(member->name());
}

ln::String Generator::makeFlatEnumMemberNameFromValue(const ConstantSymbol* member) const {
    const auto& type = member->type();
    if (LN_REQUIRE(type->isEnum())) return U"";

    auto value = type->constants().findIf([&](const Ref<ConstantSymbol>& x) { return x->value()->get<int>() == member->value()->get<int>(); });
    if (value) {
        return makeFlatEnumMemberName(type, (*value));
    }
    else {
        LN_ERROR();
        return U"";
    }
}
