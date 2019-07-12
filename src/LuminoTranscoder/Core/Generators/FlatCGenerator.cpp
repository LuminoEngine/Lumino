
#include "FlatCGenerator.hpp"





//==============================================================================
// FlatCCommon




ln::String FlatCCommon::makeEnumMemberName(GeneratorConfiguration* config, TypeSymbol* enumType, ConstantSymbol* member)
{
	return config->flatCOutputModuleName.toUpper() + u"_" + Generator::makeUpperSnakeName(enumType->shortName()) + u"_" + Generator::makeUpperSnakeName(member->name());
}


#if 0
ln::String FlatCCommon::makeCppQualTypeName(TypeSymbol* typeInfo)
{
	if (typeInfo->IsClass())
	{
		return typeInfo->shortName() + _T("*");
	}

	return typeInfo->shortName();
}

ln::String FlatCCommon::makeFlatCQualTypeName(TypeSymbol* typeInfo)
{
	if (typeInfo->IsClass())
	{
		return typeInfo->shortName() + _T("*");
	}
	return typeInfo->shortName();
}


#endif

ln::String FlatCCommon::makeInstanceParamName(TypeSymbol* type)
{
	return type->shortName().toLower();
}

//==============================================================================
// FlatCHeaderGenerator

void FlatCHeaderGenerator::generate()
{
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
			structMemberFuncDeclsText.AppendLine(makeDocumentComment(methodInfo->document()));

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
			classMemberFuncDeclsText.AppendLine(makeDocumentComment(method->document()));

			// decl
			classMemberFuncDeclsText.AppendLine(makeFuncHeader(method, FlatCharset::Unicode) + u";");

			if (method->hasStringDecl()) {
				classMemberFuncDeclsText.AppendLine(makeFuncHeader(method, FlatCharset::Ascii) + u";");
			}

			classMemberFuncDeclsText.NewLine(2);
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

		//// function impls
		//for (auto& methodInfo : classInfo->declaredMethods)
		//{
		//	classMemberFuncImplsText.AppendLines(makeFuncBody(classInfo, methodInfo)).NewLine();
		//}
	}

	// delegates
	OutputBuffer delegatesText;
#if 0
	{
		for (auto& delegateInfo : db()->delegates)
		{
			auto& involeMethod = delegateInfo->declaredMethods[0];

			// make params
			OutputBuffer params;
			for (auto& paramInfo : involeMethod->capiParameters)
			{
				params.AppendCommad("{0} {1}", FlatCCommon::makeFlatCParamTypeName(involeMethod, paramInfo), paramInfo->name);
			}

			//enumsText.AppendLine("/** {0} */", MakeDocumentComment(delegateInfo->document));
			delegatesText.AppendLine("typedef void (*LN{0})({1});", delegateInfo->shortName(), params.toString());
		}
	}
#endif

	// save C API Header
	{
		ln::String src = ln::FileSystem::readAllText(makeTemplateFilePath(_T("LuminoC.h.template")));
		src = src.replace("%%Structs%%", structsText.toString());
		src = src.replace("%%Enums%%", makeEnumDecls());
		src = src.replace("%%Delegates%%", delegatesText.toString());
		src = src.replace("%%StructMemberFuncDecls%%", structMemberFuncDeclsText.toString());
		src = src.replace("%%ClassMemberFuncDecls%%", classMemberFuncDeclsText.toString());

		ln::String fileName = ln::String::format("{0}.FlatC.generated.h", config()->moduleName);
		ln::FileSystem::writeAllText(makeOutputFilePath("FlatC/include", fileName), src, ln::TextEncoding::utf8Encoding());
	}
}

ln::String FlatCHeaderGenerator::makeDocumentComment(DocumentInfo* doc) const
{
	OutputBuffer text;
	text.AppendLine("/**");
	text.IncreaseIndent();

	// @brief
	text.AppendLine("@brief {0}", doc->summary());

	// @param
	for (auto& param : doc->params())
	{
		text.AppendLine("@param[{0}] {1} : {2}", param->io(), param->name(), param->description());
	}

	// @return
	if (!doc->returns().isEmpty())
		text.AppendLine("@return {0}", doc->returns());

	// @details
	if (!doc->details().isEmpty())
		text.AppendLine("@details {0}", doc->details());

	text.DecreaseIndent();
	text.AppendLine("*/");

	return text.toString().trim();
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

#if 0

//ln::String WrapperIFGenerator::MakeMethods(Ref<TypeSymbol> typeInfo)
//{
//	OutputBuffer buffer;
//
//	for (auto& methodInfo : typeInfo->declaredMethods)
//	{
//		// event
//		if (methodInfo->IsEventSetter())
//		{
//			// ※Event は引数1つが前提
//			auto delegateClass = methodInfo->parameters[0]->type;
//
//			// event setter
//			buffer.AppendLines(funcBodyTemplate
//				.replace("%ClassName%", typeInfo->shortName())
//				.replace("%FuncName%", methodInfo->GetCAPIFuncName())
//				.replace("%ParamList%", ln::String::format("LNHandle self, LN{0} callback", delegateClass->shortName()))
//				.replace("%FuncBody%", ln::String::format("LNI_TO_OBJECT(LN{0}, self)->{1}.Connect(callback);", typeInfo->shortName(), MakeEventWrapperMemberVariableName(methodInfo))));
//		}
//		else
//		{
//			buffer.AppendLines(makeFuncBody(typeInfo, methodInfo, false));
//
//			// override
//			if (methodInfo->isVirtual)
//			{
//				// base caller
//				buffer.AppendLines(makeFuncBody(typeInfo, methodInfo, true));
//
//				// override setter
//				buffer.AppendLines(funcBodyTemplate
//					.replace("%ClassName%", typeInfo->shortName())
//					.replace("%FuncName%", methodInfo->GetCAPIFuncName() + "_SetOverrideCaller")
//					.replace("%ParamList%", ln::String::format("{0}_{1}_OverrideCaller callback", typeInfo->shortName(), methodInfo->name))
//					.replace("%FuncBody%", ln::String::format("LN{0}::m_{1}_OverrideCaller = callback;", typeInfo->shortName(), methodInfo->name)));
//			}
//		}
//	}
//
//	return buffer.toString();
//}



#endif

//==============================================================================
// FlatCSourceGenerator

void FlatCSourceGenerator::generate()
{
	// structs
	OutputBuffer structsText;
	OutputBuffer structMemberFuncImplsText;
	for (auto& structInfo : db()->structs()) {
		for (auto& methodInfo : structInfo->publicMethods()) {
			structMemberFuncImplsText.AppendLines(makeFuncBody(structInfo, methodInfo)).NewLine();
		}
	}

	// classes
	OutputBuffer classMemberFuncImplsText;
	for (auto& classInfo : db()->classes()) {
		for (auto& methodInfo : classInfo->publicMethods()) {
			classMemberFuncImplsText.AppendLines(makeFuncBody(classInfo, methodInfo)).NewLine();
			if (methodInfo->hasStringDecl()) {
				classMemberFuncImplsText.AppendLines(makeCharsetWrapperFuncBody(classInfo, methodInfo, FlatCharset::Ascii)).NewLine();
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
			funcImpl.AppendLine(u"(LNI_HANDLE_TO_OBJECT({0}, {1})->{2}_CallBase({3}));", makeWrapSubclassName(classInfo), method->flatParameters()[0]->name(), method->shortName(), args.toString());
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
		ln::String headerName = ln::String::format("{0}.FlatC.generated.h", config()->moduleName);
		ln::String fileName = ln::String::format("{0}.FlatC.generated.cpp", config()->moduleName);

		ln::String src = ln::FileSystem::readAllText(makeTemplateFilePath(_T("Source.cpp.template")))
			.replace("%%HeaderName%%", headerName)
			.replace("%%HeaderString%%", config()->flatCHeaderString)
			.replace("%%WrapSubclassDecls%%", makeWrapSubclassDecls())
			.replace("%%StructMemberFuncImpls%%", structMemberFuncImplsText.toString())
			.replace("%%ClassMemberFuncImpls%%", classMemberFuncImplsText.toString());

		ln::FileSystem::writeAllText(makeOutputFilePath("FlatC/src", fileName), src);
	}
}

ln::String FlatCSourceGenerator::makeWrapSubclassDecls() const
{
	OutputBuffer code;
	for (auto& classSymbol : db()->classes()) {

		OutputBuffer overrideCallbackDecl;
		OutputBuffer overrideCallbackImpl;
		OutputBuffer overrideMethod;
		for (auto& method : classSymbol->virtualMethods()) {
			// field decl
			overrideCallbackDecl.AppendLine(u"static {0} s_{0};", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode));
			// field impl
			overrideCallbackImpl.AppendLine(u"{0} {1}::s_{0} = nullptr;", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode), makeWrapSubclassName(classSymbol));


			OutputBuffer paramList;
			for (auto& param : method->parameters()) {
				paramList.AppendCommad(u"{0} {1}", param->type()->fullName(), param->name());
			}

			// override
			{
				overrideMethod.AppendLine(u"virtual {0} {1}({2}) override", method->returnType()->fullName(), method->shortName(), paramList.toString());
				overrideMethod.AppendLine(u"{");
				overrideMethod.IncreaseIndent();
				OutputBuffer argList;
				argList.AppendCommad(u"LNI_OBJECT_TO_HANDLE(this)");
				for (auto& param : method->parameters()) {
					argList.AppendCommad(param->name());
				}
				overrideMethod.AppendLine(u"if (s_{0}) s_{0}({1});", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode), argList.toString());
				overrideMethod.DecreaseIndent();
				overrideMethod.AppendLine(u"}");
				overrideMethod.NewLine();
			}

			// base caller
			{
				overrideMethod.AppendLine(u"{0} {1}_CallBase({2})", method->returnType()->fullName(), method->shortName(), paramList.toString());
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

ln::String FlatCSourceGenerator::makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo)
{
	// function header
	ln::String funcHeader = makeFuncHeader(methodInfo, FlatCharset::Unicode);

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
	//if (methodInfo->IsRuntimeInitializer())
	//{
	//	body.AppendLine("LFManager::preInitialize();");
	//}

	{
		// make call args
		OutputBuffer args;
		for (auto& paramInfo : methodInfo->parameters())
		{
			auto* type = paramInfo->type();
			if (type->isClass())
			{
				args.AppendCommad("LNI_HANDLE_TO_OBJECT({0}, {1})", type->fullName(), paramInfo->name());
			}
			else if (type->isStruct())
			{
				ln::String castTo = type->fullName();
				if (paramInfo->isIn()) castTo = "const " + castTo;
				args.AppendCommad("*reinterpret_cast<{0}*>({1})", castTo, paramInfo->name());
			}
			else if (type->isEnum())
			{
				args.AppendCommad("static_cast<{0}>({1})", type->fullName(), paramInfo->name());
			}
			else if (type == PredefinedTypes::boolType)
			{
				args.AppendCommad("LNI_LNBOOL_TO_BOOL({0})", paramInfo->name());
			}
			else
			{
				args.AppendCommad(paramInfo->name());
			}
		}

		// return output
		if (methodInfo->returnType() != PredefinedTypes::voidType)
		{
			if (methodInfo->returnType()->isStruct())
				body.append("*outReturn = reinterpret_cast<const {0}&>", makeFlatClassName(methodInfo->returnType()));
			else if (methodInfo->returnType()->isClass())
				body.append("*outReturn = LNI_TO_HANDLE");
			else if (methodInfo->returnType() == PredefinedTypes::boolType)
				body.append("*outReturn = LNI_BOOL_TO_LNBOOL");
			else if (methodInfo->returnType()->isString())
				body.append("*outReturn = LNI_STRING_TO_STRPTR");
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
					name = ("LN" + typeInfo->shortName() + "::" + name + "_CallBase");
				callExp = ln::String::format("(LNI_HANDLE_TO_OBJECT({0}, {1})->{2}({3}));", typeInfo->fullName(), FlatCCommon::makeInstanceParamName(typeInfo), name, args.toString());
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

ln::String FlatCSourceGenerator::makeCharsetWrapperFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo, FlatCharset charset)
{
	OutputBuffer args;
	for (auto& param : methodInfo->flatParameters()) {
		if (param->type()->isString()) {
			args.AppendCommad(u"ln::String::fromCString({0}, -1, ln::TextEncoding::utf8Encoding()).c_str()", param->name());
		}
		else {
			args.AppendCommad(param->name());
		}
	}

	auto callExpr = ln::String::format(u"LnResult result = {0}({1});", makeFuncName(methodInfo, FlatCharset::Unicode), args.toString());

	OutputBuffer code;
	code.AppendLine(makeFuncHeader(methodInfo, charset));
	code.AppendLine(u"{");
	code.IncreaseIndent();
	code.AppendLine(callExpr);
	code.AppendLine(u"return result;");
	code.DecreaseIndent();
	code.AppendLine(u"}");
	return code.toString();
}
