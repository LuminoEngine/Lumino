
#include "FlatCGenerator.hpp"





//==============================================================================
// FlatCCommon

ln::String FlatCCommon::makeEnumMemberName(GeneratorConfiguration* config, TypeSymbol* enumType, ConstantSymbol* member)
{
	return config->flatCOutputModuleName.toUpper() + u"_" + Generator::makeUpperSnakeName(enumType->shortName()) + u"_" + Generator::makeUpperSnakeName(member->name());
}

ln::String FlatCCommon::makeInstanceParamName(TypeSymbol* type)
{
	return type->shortName().toLower();
}

ln::String FlatCCommon::makeCreateDelegateObjectFuncHeader(TypeSymbol* delegateSymbol) const
{
    auto funcPtrType = makeDelegateCallbackFuncPtrName(delegateSymbol, FlatCharset::Unicode);
    auto flatClassName = makeFlatClassName(delegateSymbol);
    return ln::String::format(u"LN_FLAT_API LnResult {0}_Create({1} callback, LnHandle* outDelegate)", flatClassName, funcPtrType);
}

//==============================================================================
// FlatCHeaderGenerator

void FlatCHeaderGenerator::generate()
{
	// delegates
	OutputBuffer delegatesText;
	for (auto& delegateSymbol : db()->delegates()) {
		delegatesText.AppendLine(makeDelegateFuncPtrDecl(delegateSymbol));
	}
    delegatesText.NewLine();

    // delegateObjects
    for (auto& delegateSymbol : db()->delegateObjects()) {
		delegatesText.AppendLine(makeDelegateFuncPtrDecl(delegateSymbol));
        delegatesText.AppendLine(makeCreateDelegateObjectFuncHeader(delegateSymbol) + u";");
    }

	// structs
	OutputBuffer structsText;
	OutputBuffer structMemberFuncDeclsText;
	OutputBuffer structMemberFuncImplsText;
	for (auto& structInfo : db()->structs())
	{
		structsText.AppendLine(makeDocumentComment(structInfo->document()));
		structsText.AppendLine("struct {0}", makeFlatClassName(structInfo));
		structsText.AppendLine("{");
		structsText.IncreaseIndent();
		for (auto& fieldInfo : structInfo->fields())
		{
			structsText.AppendLine("{0} {1};", fieldInfo->type()->shortName(), fieldInfo->name());
		}
		structsText.DecreaseIndent();
		structsText.AppendLine("};");

		// function decls
		for (auto& methodInfo : structInfo->publicMethods())
		{
			// comment
			structMemberFuncDeclsText.AppendLine(makeMethodDocumentComment(methodInfo));

			// decl
			structMemberFuncDeclsText.AppendLine(makeFuncHeader(methodInfo, FlatCharset::Unicode) + u";").NewLine(2);
		}

		//// function impls
		//for (auto& methodInfo : structInfo->declaredMethods)
		//{
		//	structMemberFuncImplsText.AppendLines(makeFuncBody(structInfo, methodInfo)).NewLine();
		//}
	}

	// classes
	OutputBuffer classMemberFuncDeclsText;
	//OutputBuffer classMemberFuncImplsText;
	for (auto& classInfo : db()->classes())
	{
		classMemberFuncDeclsText.AppendLine("//==============================================================================");
		classMemberFuncDeclsText.AppendLine("// {0}", classInfo->fullName());
		classMemberFuncDeclsText.NewLine();

		// function decls
		for (auto& method : classInfo->publicMethods())
		{
			// comment
			classMemberFuncDeclsText.AppendLine(makeMethodDocumentComment(method));

			// decl
			classMemberFuncDeclsText.AppendLine(makeFuncHeader(method, FlatCharset::Unicode) + u";");

			if (method->hasStringDecl()) {
				classMemberFuncDeclsText.AppendLine(makeFuncHeader(method, FlatCharset::Ascii) + u";");
			}

			classMemberFuncDeclsText.NewLine();
		}

		// virtual callback
		if (!classInfo->virtualMethods().isEmpty()) {
			for (auto& method : classInfo->virtualMethods()) {
				// make params
				OutputBuffer params;
				for (auto& param : method->flatParameters()) {
					params.AppendCommad("{0} {1}", makeFlatCParamQualTypeName(method, param, FlatCharset::Unicode), param->name());
				}
				classMemberFuncDeclsText.AppendLine(u"typedef LnResult(*{0})({1});", makeFlatVirutalCallbackFuncPtrName(classInfo, method, FlatCharset::Unicode), params.toString());
				classMemberFuncDeclsText.AppendLine(makeFlatAPIDecl_SetOverrideCallback(classInfo, method, FlatCharset::Unicode) + u";");
				classMemberFuncDeclsText.AppendLine(makeFlatAPIDecl_CallOverrideBase(classInfo, method, FlatCharset::Unicode) + u";");
			}
			classMemberFuncDeclsText.NewLine();
		}
	}

	// save C API Header
	{
		ln::String src = ln::FileSystem::readAllText(makeTemplateFilePath(u"LuminoC.template.h"));
		src = src.replace(u"%%Structs%%", structsText.toString());
		src = src.replace(u"%%Enums%%", makeEnumDecls());
		src = src.replace(u"%%Delegates%%", delegatesText.toString());
		src = src.replace(u"%%StructMemberFuncDecls%%", structMemberFuncDeclsText.toString());
		src = src.replace(u"%%ClassMemberFuncDecls%%", classMemberFuncDeclsText.toString());

        auto filePath = makeFlatCHeaderOutputPath();
		ln::FileSystem::writeAllText(filePath, src, ln::TextEncoding::getEncoding(ln::EncodingType::UTF8));
	}
}

ln::String FlatCHeaderGenerator::makeDocumentComment(DocumentInfo* doc) const
{
	OutputBuffer code;
    code.AppendLine(u"/**");
    code.IncreaseIndent();

	// @brief
    code.AppendLine(u"@brief {0}", doc->summary());

	// @param
	for (auto& param : doc->params())
	{
        code.AppendLine(u"@param[{0}] {1} : {2}", param->io(), param->name(), param->description());
	}

	// @return
	if (!doc->returns().isEmpty())
        code.AppendLine(u"@return {0}", doc->returns());

	// @details
	if (!doc->details().isEmpty())
        code.AppendLine(u"@details {0}", doc->details());

    code.DecreaseIndent();
    code.AppendLine(u"*/");

	return code.toString().trim();
}

ln::String FlatCHeaderGenerator::makeMethodDocumentComment(const MethodSymbol* method) const
{

	auto doc = method->document();

	OutputBuffer code;
    code.AppendLine(u"/**");
    code.IncreaseIndent();

	// @brief
    code.AppendLine(u"@brief {0}", doc->summary());

	// @param
	for (auto& param : doc->flatParams())
	{
		auto desc = param->description();
		auto paramInfo = method->findFlatParameter(param->name());
		if (paramInfo && paramInfo->qualType().strongReference) {
			desc += u" (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)";
		}

        code.AppendLine(u"@param[{0}] {1} : {2}", param->io(), param->name(), desc);
	}

	// @return
	if (!doc->returns().isEmpty())
        code.AppendLine(u"@return {0}", doc->returns());

	// @details
	if (!doc->details().isEmpty())
        code.AppendLine(u"@details {0}", doc->details());

    code.DecreaseIndent();
    code.AppendLine(u"*/");

	return code.toString().trim();
}

ln::String FlatCHeaderGenerator::makeDelegateFuncPtrDecl(const TypeSymbol* delegateSymbol) const
{
	// return
	auto returnType = (delegateSymbol->isDelegateObject()) ? u"LnResult" : u"void";

    // make params
    OutputBuffer params;
	if (delegateSymbol->isDelegateObject()) {
		params.AppendCommad(u"LnHandle");
	}
    for (auto& param : delegateSymbol->delegateDeclaration()->flatParameters()) {
        params.AppendCommad(u"{0} {1}", makeFlatCParamQualTypeName(nullptr, param, FlatCharset::Unicode), param->name());
    }
    return ln::String::format(u"typedef {0}(*{1})({2});", returnType, makeDelegateCallbackFuncPtrName(delegateSymbol, FlatCharset::Unicode), params.toString());
}

ln::String FlatCHeaderGenerator::makeEnumDecls() const
{
	OutputBuffer code;

	for (auto& symbol : db()->enums()) {
		auto typeName = makeFlatClassName(symbol);

		code.AppendLine(makeDocumentComment(symbol->document()));
		code.AppendLine(u"typedef enum tag" + typeName);
		code.AppendLine(u"{");
		code.IncreaseIndent();

		for (auto& member : symbol->constants()) {
			code.AppendLine(makeDocumentComment(member->document()));
			code.AppendLine(u"{0} = {1},", FlatCCommon::makeEnumMemberName(config(), symbol, member), member->value()->get<int>());
			code.NewLine();
		}

		code.DecreaseIndent();
		code.AppendLine(u"} " + typeName + u";");
		code.NewLine();
	}

	return code.toString().trim();
}

//==============================================================================
// FlatCSourceGenerator

void FlatCSourceGenerator::generate()
{
	// structs
	OutputBuffer structsText;
	OutputBuffer structMemberFuncImplsText;
	for (auto& structInfo : db()->structs()) {
		for (auto& methodInfo : structInfo->publicMethods()) {
			structMemberFuncImplsText.AppendLines(makeFuncBody(structInfo, methodInfo, FlatCharset::Unicode)).NewLine();
		}
	}

    //for (auto& delegateSymbol : db()->delegateObjects()) {
    //    for (auto& methodInfo : structInfo->publicMethods()) {
    //        structMemberFuncImplsText.AppendLines(makeFuncBody(structInfo, methodInfo)).NewLine();
    //    }
    //}

	// classes
	OutputBuffer classMemberFuncImplsText;
	for (auto& classInfo : db()->classes()) {
		for (auto& methodInfo : classInfo->publicMethods()) {
			if (methodInfo->isEventConnector()) {
				classMemberFuncImplsText.AppendLines(makeEventConnectorFuncBody(classInfo, methodInfo)).NewLine();
			}
			else {
				classMemberFuncImplsText.AppendLines(makeFuncBody(classInfo, methodInfo, FlatCharset::Unicode)).NewLine();
				if (methodInfo->hasStringDecl()) {
					classMemberFuncImplsText.AppendLines(makeFuncBody(classInfo, methodInfo, FlatCharset::Ascii)).NewLine();
				}
			}
		}

		// TypeInfo id setter
		if (!classInfo->isStatic()) {
			classMemberFuncImplsText.AppendLine(makeFlatAPIDecl_SetManagedTypeInfoId(classInfo));
			classMemberFuncImplsText.AppendLine(u"{");
			classMemberFuncImplsText.IncreaseIndent();
			classMemberFuncImplsText.AppendLine(u"::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<{0}>(), id);", classInfo->fullName());
			classMemberFuncImplsText.DecreaseIndent();
			classMemberFuncImplsText.AppendLine(u"}");
			classMemberFuncImplsText.NewLine();
		}

		// virtual
		for (auto& method : classInfo->virtualMethods()) {
			OutputBuffer args;
			for (auto& param : method->parameters()) {
				args.AppendCommad(param->name());
			}

			OutputBuffer funcImpl;

			funcImpl.AppendLine(makeFlatAPIDecl_CallOverrideBase(classInfo, method, FlatCharset::Unicode));
			funcImpl.AppendLine(u"{");
			funcImpl.IncreaseIndent();
			funcImpl.AppendLine(u"LNI_FUNC_TRY_BEGIN;");
			funcImpl.AppendLine(u"(LNI_HANDLE_TO_OBJECT({0}, {1})->{2}_CallBase({3}));", makeWrapSubclassName(classInfo), method->flatParameters()[0]->name(), method->shortName(), makeNativeArgList(method));
			funcImpl.AppendLine(u"LNI_FUNC_TRY_END_RETURN;");
			funcImpl.DecreaseIndent();
			funcImpl.AppendLine(u"}");

			funcImpl.AppendLine(makeFlatAPIDecl_SetOverrideCallback(classInfo, method, FlatCharset::Unicode));
			funcImpl.AppendLine(u"{");
			funcImpl.IncreaseIndent();
			funcImpl.AppendLine(u"{0}::s_{1} = callback;", makeWrapSubclassName(classInfo), makeFlatVirutalCallbackFuncPtrName(classInfo, method, FlatCharset::Unicode));
			funcImpl.AppendLine(u"return LN_SUCCESS;");
			funcImpl.DecreaseIndent();
			funcImpl.AppendLine(u"}");

			classMemberFuncImplsText.AppendLine(funcImpl.toString());
		}
	}

	// save C API Source
	{
        auto includeDirective = ln::String::format(u"#include \"{0}.FlatC.generated.h\"", config()->moduleName);
        auto fileName = ln::String::format(u"{0}.FlatC.generated.cpp", config()->moduleName);

        if (!config()->flatCSourceOutputDirOverride.isEmpty())
            includeDirective = ln::String::format(u"#include <LuminoEngine/Runtime/{0}.FlatC.generated.h>", config()->moduleName);

        auto src = ln::FileSystem::readAllText(makeTemplateFilePath(u"Source.cpp.template"))
			.replace("%%IncludeDirective%%", includeDirective)
			.replace("%%HeaderString%%", config()->flatCHeaderString)
			.replace("%%WrapSubclassDecls%%", makeWrapSubclassDecls())
			.replace("%%StructMemberFuncImpls%%", structMemberFuncImplsText.toString())
			.replace("%%ClassMemberFuncImpls%%", classMemberFuncImplsText.toString());

        auto filePath = (config()->flatCSourceOutputDirOverride.isEmpty()) ? makeOutputFilePath("FlatC/src", fileName) : ln::Path::combine(config()->flatCSourceOutputDirOverride, fileName);
		ln::FileSystem::writeAllText(filePath, src);
	}
}

ln::String FlatCSourceGenerator::generateDelegateObjects() const
{
	OutputBuffer code;

	for (auto& delegateSymbol : db()->delegateObjects()) {
		auto className = makeWrapSubclassName(delegateSymbol);
		auto baseclassName = delegateSymbol->fullName();
		auto funcPtrType = makeDelegateCallbackFuncPtrName(delegateSymbol, FlatCharset::Unicode);
		auto nativeReturnType = delegateSymbol->delegateDeclaration()->returnType().type->fullName();
		auto flatClassName = makeFlatClassName(delegateSymbol);

		code.AppendLine(u"class {0} : public {1}", className, baseclassName);
		code.AppendLine(u"{");
		code.AppendLine(u"public:");
		code.IncreaseIndent();
		{
			code.AppendLine(u"{0} m_callback;", funcPtrType);
			code.NewLine();

			// constructor
			code.AppendLine(u"{0}() : {1}([this]({2}) -> {3}", className, baseclassName, makeNativeParamList(delegateSymbol->delegateDeclaration()), nativeReturnType);
			code.AppendLine(u"{");
			code.IncreaseIndent();
			{
				code.AppendLine(nativeReturnType + u" ret = {};");
				code.AppendLine(u"auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), {0}, &ret);", makeFlatArgList(delegateSymbol->delegateDeclaration()));
				code.AppendLine(u"if (r != LN_SUCCESS) {{ LN_ERROR(\"{0}\"); }}", funcPtrType);
				code.AppendLine(u"return ret;");
			}
			code.DecreaseIndent();
			code.AppendLine(u"})");
			code.AppendLine(u"{}");
			code.NewLine();

			// init()
			code.AppendLine(u"void init({0} callback)", funcPtrType);
			code.AppendLine(u"{");
			code.IncreaseIndent();
			{
				code.AppendLine(u"{0}::init();", delegateSymbol->fullName());
				code.AppendLine(u"m_callback = callback;");
			}
			code.DecreaseIndent();
			code.AppendLine(u"}");
		}
		code.DecreaseIndent();
		code.AppendLine(u"};");
		code.NewLine();

		// Create
        code.AppendLine(makeCreateDelegateObjectFuncHeader(delegateSymbol));
		code.AppendLine(u"{");
		code.IncreaseIndent();
		{
			code.AppendLine(u"LNI_FUNC_TRY_BEGIN;");
			code.AppendLine(u"LNI_CREATE_OBJECT(outDelegate, {0}, init, callback);", className);
			code.AppendLine(u"LNI_FUNC_TRY_END_RETURN;");
		}
		code.DecreaseIndent();
		code.AppendLine(u"}");
	}

	return code.toString();
}

ln::String FlatCSourceGenerator::makeWrapSubclassDecls() const
{
	OutputBuffer code;

	code.AppendLines(generateDelegateObjects());

	for (auto& classSymbol : db()->classes()) {

		OutputBuffer overrideCallbackDecl;
		OutputBuffer overrideCallbackImpl;
		OutputBuffer overrideMethod;
		for (auto& method : classSymbol->virtualMethods()) {
			// field decl
			overrideCallbackDecl.AppendLine(u"static {0} s_{0};", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode));
			// field impl
			overrideCallbackImpl.AppendLine(u"{0} {1}::s_{0} = nullptr;", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode), makeWrapSubclassName(classSymbol));

			// override method parameter list
			OutputBuffer paramList;
			for (auto& param : method->parameters()) {
				paramList.AppendCommad(u"{0} {1}", param->getFullQualTypeName(), param->name());
			}

			// override
			{
				overrideMethod.AppendLine(u"virtual {0} {1}({2}) override", method->returnType().type->fullName(), method->shortName(), paramList.toString());
				overrideMethod.AppendLine(u"{");
				overrideMethod.IncreaseIndent();
				OutputBuffer argList;
				argList.AppendCommad(u"LNI_OBJECT_TO_HANDLE(this)");
				for (auto& param : method->parameters()) {
					if (param->type()->isClass())
						argList.AppendCommad(u"LNI_OBJECT_TO_HANDLE({0})", param->name());
					else
						argList.AppendCommad(param->name());
				}
				overrideMethod.AppendLine(u"if (s_{0}) s_{0}({1});", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode), argList.toString());
				overrideMethod.DecreaseIndent();
				overrideMethod.AppendLine(u"}");
				overrideMethod.NewLine();
			}

			// base caller
			{
				overrideMethod.AppendLine(u"{0} {1}_CallBase({2})", method->returnType().type->fullName(), method->shortName(), paramList.toString());
				overrideMethod.AppendLine(u"{");
				overrideMethod.IncreaseIndent();
				OutputBuffer argList;
				for (auto& param : method->parameters()) {
					argList.AppendCommad(param->name());
				}
				overrideMethod.AppendLine(u"{0}::{1}({2});", classSymbol->fullName(), method->shortName(), argList.toString());
				overrideMethod.DecreaseIndent();
				overrideMethod.AppendLine(u"}");
			}
		}

		code.AppendLine(u"class {0} : public {1}", makeWrapSubclassName(classSymbol), classSymbol->fullName());
		code.AppendLine(u"{");
		code.AppendLine(u"public:");
		code.IncreaseIndent();
		code.AppendLine(overrideCallbackDecl.toString());
		code.AppendLine(overrideMethod.toString());
		code.DecreaseIndent();
		code.AppendLine(u"};");
		code.AppendLine(overrideCallbackImpl.toString());
		code.NewLine();
	}
	return code.toString();
}

ln::String FlatCSourceGenerator::makeNativeParamList(const MethodSymbol* method) const
{
	OutputBuffer params;
	for (auto& param : method->parameters()) {
		params.AppendCommad(u"{0} {1}", param->getFullQualTypeName(), param->name());
	}
	return params.toString();
}

// FlatC(Handleなど) -> Native(UIElement*など) への呼び出し実引数リストを作成する。
ln::String FlatCSourceGenerator::makeNativeArgList(const MethodSymbol* method) const
{
	OutputBuffer argList;
	for (auto& param : method->parameters()) {
		if (param->type()->isObjectGroup())
			argList.AppendCommad(u"LNI_HANDLE_TO_OBJECT({0}, {1})", param->type()->fullName(), param->name());
		else
			argList.AppendCommad(param->name());
	}
	return argList.toString();
}

// Native -> FlatC への呼び出し実引数リストを作成する。Native のコールバックから、FlatC のコールバックを呼び出すときなどに使用する。 
ln::String FlatCSourceGenerator::makeFlatArgList(const MethodSymbol* method) const
{
	OutputBuffer argList;
	for (auto& param : method->parameters()) {
		if (param->type()->isObjectGroup())
			argList.AppendCommad(u"LNI_HANDLE_TO_OBJECT({0}, {1})", param->type()->fullName(), param->name());
		else
			argList.AppendCommad(param->name());
	}
	return argList.toString();
}

ln::String FlatCSourceGenerator::makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo, FlatCharset charset)
{
	// function header
	ln::String funcHeader = makeFuncHeader(methodInfo, charset);

	//{
	//	// 第1引数
	//	{
	//		if (!methodInfo->isStatic)
	//		{
	//			if (typeInfo->isStruct)
	//			{
	//				if (methodInfo->isConst)
	//					params.AppendCommad("const LN{0}* {1}", typeInfo->name, MakeInstanceParamName(typeInfo));
	//				else
	//					params.AppendCommad("LN{0}* {1}", typeInfo->name, MakeInstanceParamName(typeInfo));
	//			}
	//			else if (!methodInfo->isConstructor)
	//			{
	//				params.AppendCommad("LNHandle {0}", MakeInstanceParamName(typeInfo));
	//			}
	//		}
	//	}

	//	// params
	//	for (auto& paramInfo : methodInfo->parameters)
	//	{
	//		params.AppendCommad("{0} {1}", MakeParamTypeName(paramInfo->type, false), paramInfo->name);
	//	}

	//	// return value
	//	if (!methodInfo->returnType->isVoid)
	//	{
	//		params.AppendCommad("{0} outReturn", MakeParamTypeName(methodInfo->returnType, true));
	//	}

	//	// constructor
	//	if (methodInfo->isConstructor)
	//	{
	//		params.AppendCommad("LNHandle* out{0}", MakeInstanceParamName(typeInfo));
	//	}
	//}

	// make func body
	OutputBuffer body;

	if (methodInfo->isRuntimeInitializer()) {
		body.AppendLine("LnRuntime_Initialize();");
	}

	{
		// make call args
		OutputBuffer args;
		for (auto& paramInfo : methodInfo->parameters())
		{
			auto* type = paramInfo->type();
			if (type->isObjectGroup())
			{
				args.AppendCommad(u"LNI_HANDLE_TO_OBJECT({0}, {1})", type->fullName(), paramInfo->name());
			}
			else if (type->isStruct())
			{
				ln::String castTo = type->fullName();
				if (paramInfo->isIn()) castTo = "const " + castTo;
				args.AppendCommad(u"*reinterpret_cast<{0}*>({1})", castTo, paramInfo->name());
			}
			else if (type->isEnum())
			{
				args.AppendCommad(u"static_cast<{0}>({1})", type->fullName(), paramInfo->name());
			}
			else if (type == PredefinedTypes::boolType)
			{
				args.AppendCommad(u"LNI_LNBOOL_TO_BOOL({0})", paramInfo->name());
			}
            else if (type->isString() && charset == FlatCharset::Ascii) {
                args.AppendCommad(u"LNI_UTF8STRPTR_TO_STRING({0})", paramInfo->name());
            }
			else
			{
				args.AppendCommad(paramInfo->name());
			}
		}

		// return output
		if (methodInfo->returnType().type != PredefinedTypes::voidType)
		{
			if (methodInfo->returnType().type->isStruct())
				body.append("*outReturn = reinterpret_cast<const {0}&>", makeFlatClassName(methodInfo->returnType().type));
			else if (methodInfo->returnType().type->isClass())
				if (methodInfo->returnType().strongReference)
					body.append("*outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE");
				else
					body.append("*outReturn = LNI_OBJECT_TO_HANDLE");
			else if (methodInfo->returnType().type == PredefinedTypes::boolType)
				body.append("*outReturn = LNI_BOOL_TO_LNBOOL");
			else if (methodInfo->returnType().type->isString())
                if (charset == FlatCharset::Unicode)
				    body.append("*outReturn = LNI_STRING_TO_STRPTR_UTF16");
                else
                    body.append("*outReturn = LNI_STRING_TO_STRPTR_UTF8");
			else
				body.append("*outReturn = ");
		}

		// call expression
		ln::String callExp;
		if (typeInfo->isStruct())
		{
			if (methodInfo->isConstructor()) {
				callExp = ln::String::format("new (reinterpret_cast<{0}*>({1})) {2}({3});", typeInfo->fullName(), typeInfo->shortName().toLower(), methodInfo->shortName(), args.toString());
			}
			else if (methodInfo->isStatic()) {
				body.append("({0}::{1}({2}));", typeInfo->fullName(), methodInfo->shortName(), args.toString());
			}
			else {
				ln::String castTo = typeInfo->fullName();
				if (methodInfo->isConst()) castTo = "const " + castTo;
				callExp = ln::String::format("(reinterpret_cast<{0}*>({1})->{2}({3}));", castTo, typeInfo->shortName().toLower(), methodInfo->shortName(), args.toString());
			}
		}
		else
		{
			// static 関数
			if (methodInfo->isStatic())
			{
				body.append("({0}::{1}({2}));", typeInfo->fullName(), methodInfo->shortName(), args.toString());
			}
			// コンストラクタ 関数
			else if (methodInfo->isConstructor())
			{
				OutputBuffer macroArgs;
				macroArgs.AppendCommad("out" + typeInfo->shortName());	// 格納する変数名
				macroArgs.AppendCommad(makeWrapSubclassName(typeInfo));	// クラス名
				macroArgs.AppendCommad(methodInfo->shortName());		// 初期化関数名
				macroArgs.AppendCommad(args.toString());		// 引数
				callExp = ln::String::format("LNI_CREATE_OBJECT({0});", macroArgs.toString());
			}
			// 普通のインスタンス 関数
			else
			{
				auto name = methodInfo->shortName();
				if (methodInfo->isVirtual())
					name = (makeWrapSubclassName(typeInfo) + "::" + name + "_CallBase");
				callExp = ln::String::format("(LNI_HANDLE_TO_OBJECT({0}, {1})->{2}({3}));", makeWrapSubclassName(typeInfo), FlatCCommon::makeInstanceParamName(typeInfo), name, args.toString());
			}
		}

		body.append(callExp);
	}



	OutputBuffer funcImpl;
	funcImpl.AppendLine(funcHeader);
	funcImpl.AppendLine(u"{");
	funcImpl.IncreaseIndent();
	funcImpl.AppendLine(u"LNI_FUNC_TRY_BEGIN;");
	funcImpl.AppendLine(body.toString());
	funcImpl.AppendLine(u"LNI_FUNC_TRY_END_RETURN;");
	funcImpl.DecreaseIndent();
	funcImpl.AppendLine(u"}");
	return funcImpl.toString();
}

//ln::String FlatCSourceGenerator::makeCharsetWrapperFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo, FlatCharset charset)
//{
//	OutputBuffer args;
//    ln::String returnDecl;
//    ln::String postCallExpr;
//	for (auto& param : methodInfo->flatParameters()) {
//        if (param->isReturn() && param->type()->isString()) {
//            returnDecl = u"const LnChar* _ret;";
//            args.AppendCommad(u"&_ret");
//            postCallExpr = u"";
//        }
//		if (!param->isReturn() &&   // 
//            param->type()->isString()) {
//			args.AppendCommad(u"ln::String::fromCString({0}, -1, ln::TextEncoding::utf8Encoding()).c_str()", param->name());
//		}
//		else {
//			args.AppendCommad(param->name());
//		}
//	}
//
//	auto callExpr = ln::String::format(u"LnResult result = {0}({1});", makeFlatFullFuncName(methodInfo, FlatCharset::Unicode), args.toString());
//
//	OutputBuffer code;
//	code.AppendLine(makeFuncHeader(methodInfo, charset));
//	code.AppendLine(u"{");
//	code.IncreaseIndent();
//	code.AppendLine(callExpr);
//	code.AppendLine(u"return result;");
//	code.DecreaseIndent();
//	code.AppendLine(u"}");
//	return code.toString();
//}

ln::String FlatCSourceGenerator::makeEventConnectorFuncBody(const TypeSymbol* classInfo, const MethodSymbol* methodInfo) const
{
	auto delegateType = methodInfo->parameters().front()->type();
	auto delegateDeclaration = delegateType->delegateDeclaration();

	// wrap callback
	// Note: 今のところ引数ひとつしか想定しないので、引数名は e だけ.
	OutputBuffer params;
	OutputBuffer args;
	args.AppendCommad(methodInfo->flatParameters().front()->name());
	for (auto& param : delegateDeclaration->parameters()) {
		params.AppendCommad(u"{0} e", param->getFullQualTypeName(), param->name());
		if (param->type()->isClass()) {
			args.AppendCommad(u"LNI_OBJECT_TO_HANDLE(e)");
		}
		else {
			args.AppendCommad(u"e");
		}
	}
	auto callbackLambda = ln::String::format(u"[=]({0}) {{ handler({1}); }}", params.toString(), args.toString());

	OutputBuffer funcImpl;
	funcImpl.AppendLine(makeFuncHeader(methodInfo, FlatCharset::Unicode));
	funcImpl.AppendLine(u"{");
	funcImpl.IncreaseIndent();
	funcImpl.AppendLine(u"LNI_FUNC_TRY_BEGIN;");
	funcImpl.AppendLine(u"(LNI_HANDLE_TO_OBJECT({0}, {1})->{2}({3}));", makeWrapSubclassName(classInfo), methodInfo->flatParameters()[0]->name(), methodInfo->shortName(), callbackLambda);
	funcImpl.AppendLine(u"LNI_FUNC_TRY_END_RETURN;");
	funcImpl.DecreaseIndent();
	funcImpl.AppendLine(u"}");
	return funcImpl.toString();
}
