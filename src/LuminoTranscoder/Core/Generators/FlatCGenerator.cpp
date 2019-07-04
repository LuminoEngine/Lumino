
#include "FlatCGenerator.hpp"


static const ln::String funcHeaderTemplate =
"LN_API LnResult %%FuncName%%(%%ParamList%%)";



//==============================================================================
// FlatCCommon

ln::String FlatCCommon::makeFuncName(GeneratorConfiguration* config, MethodSymbol* method)
{
	ln::String funcName;
	if (method->isConstructor()) {
		funcName = u"Init";
	}
	else {
		funcName = method->shortName().toTitleCase();
	}

	auto name = ln::String::format(_T("{0}{1}_{2}"), config->flatCOutputModuleName, method->ownerType()->shortName(), funcName);

	return name;

	//ln::String n = ln::String::format(_T("LN{0}_{1}"), owner->shortName(), funcName);
	//if (IsOverloadChild())
	//	n += metadata->getValue(MetadataSymbol::OverloadPostfixAttr);
	//return n;
}

ln::String FlatCCommon::makeTypeName(GeneratorConfiguration* config, TypeSymbol* type)
{
	if (type->isPrimitive()) {
		if (type == PredefinedTypes::boolType) return u"LnBool";
		return type->shortName();
	}
	else {
		return config->flatCOutputModuleName + type->shortName();
	}
}

//ln::String FlatCCommon::makeFunkParamList(MethodSymbol* method)
//{
//	return
//}
//
ln::String FlatCCommon::makeFlatCParamQualTypeName(GeneratorConfiguration* config, ln::Ref<MethodSymbol> methodInfo, ln::Ref<MethodParameterSymbol> paramInfo)
{
	auto typeInfo = paramInfo->type();

	if (typeInfo->kind() == TypeKind::Struct)
	{
		ln::String modifer;
		if (paramInfo->isThis() && methodInfo->isConst())
			modifer = "const ";
		if (!paramInfo->isThis() && !paramInfo->isOut())
			modifer = "const ";
		return ln::String::format("{0}{1}{2}*", modifer, config->flatCOutputModuleName, typeInfo->shortName());
	}

	if (typeInfo->kind() == TypeKind::Enum)
	{
		return makeTypeName(config, typeInfo);
	}

	if (typeInfo == PredefinedTypes::stringType)
	{
		if (paramInfo->isOut() || paramInfo->isReturn())
			return "const LnChar**";
		else
			return "const LnChar*";
	}

	ln::String name;
	if (typeInfo == PredefinedTypes::boolType)
		name = _T("LnBool");
	else if (typeInfo->kind() == TypeKind::Class)
		name = "LnHandle";
	else
		name = typeInfo->shortName();

	if (paramInfo->isOut() || paramInfo->isReturn())
		name += "*";

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

// 宣言文の作成。ドキュメンテーションコメントは含まない。
ln::String FlatCCommon::makeFuncHeader(GeneratorConfiguration* config, MethodSymbol* methodInfo)
{
	// make params
	OutputBuffer params;
	for (auto& paramInfo : methodInfo->flatParameters())
	{
		params.AppendCommad("{0} {1}", FlatCCommon::makeFlatCParamQualTypeName(config, methodInfo, paramInfo), paramInfo->name());
	}

	//ln::String suffix = (methodInfo->isVirtual) ? "_CallVirtualBase" : "";

	return funcHeaderTemplate
		.replace("%%FuncName%%", FlatCCommon::makeFuncName(config, methodInfo))// + suffix)
		.replace("%%ParamList%%", params.toString());
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
		structsText.AppendLine("struct {0}", FlatCCommon::makeTypeName(config(), structInfo));
		structsText.AppendLine("{");
		structsText.IncreaseIndent();
		for (auto& fieldInfo : structInfo->fields())
		{
			structsText.AppendLine("{0} {1};", fieldInfo->type()->shortName(), fieldInfo->name());
		}
		structsText.DecreaseIndent();
		structsText.AppendLine("};");

		// function decls
		for (auto& methodInfo : structInfo->methods())
		{
			// comment
			structMemberFuncDeclsText.AppendLine(makeDocumentComment(methodInfo->document()));

			// decl
			structMemberFuncDeclsText.AppendLine(FlatCCommon::makeFuncHeader(config(), methodInfo) + u";").NewLine(2);
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
		for (auto& methodInfo : classInfo->methods())
		{
			// comment
			classMemberFuncDeclsText.AppendLine(makeDocumentComment(methodInfo->document()));

			// decl
			classMemberFuncDeclsText.AppendLine(FlatCCommon::makeFuncHeader(config(), methodInfo)).append(";").NewLine(2);
		}

		//// function impls
		//for (auto& methodInfo : classInfo->declaredMethods)
		//{
		//	classMemberFuncImplsText.AppendLines(makeFuncBody(classInfo, methodInfo)).NewLine();
		//}

		//if (!classInfo->isStatic())
		//{
		//	static const ln::String setBindingTypeInfo =
		//		"LN_API void LN%ClassName%_SetBindingTypeInfo(void* data)\n"
		//		"{\n"
		//		"    tr::TypeInfo::getTypeInfo<%ClassName%>()->setBindingTypeInfo(data);\n"
		//		"}\n";
		//	classMemberFuncImplsText.AppendLines(setBindingTypeInfo.replace("%ClassName%", classInfo->shortName()));
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
		ln::FileSystem::writeAllText(makeOutputFilePath("FlatC", fileName), src, ln::TextEncoding::utf8Encoding());
	}
	//// save C API Source
	//{
	//	ln::String src = ln::FileSystem::readAllText(makeTemplateFilePath(_T("Source.cpp.template")));
	//	src = src.replace("%%StructMemberFuncImpls%%", structMemberFuncImplsText.toString());
	//	//src = src.replace("%%ClassMemberFuncImpls%%", classMemberFuncImplsText.toString());

	//	ln::String fileName = ln::String::format("{0}.FlatC.generated.cpp", config()->moduleName);
	//	ln::FileSystem::writeAllText(makeOutputFilePath(fileName).c_str(), src);
	//}
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
		auto typeName = FlatCCommon::makeTypeName(config(), symbol);

		code.AppendLine(makeDocumentComment(symbol->document()));
		code.AppendLine(u"typedef enum tag" + typeName);
		code.AppendLine(u"{");
		code.IncreaseIndent();

		for (auto& member : symbol->constants()) {
			code.AppendLine(makeDocumentComment(member->document()));
			code.AppendLine(u"{0} = {1},", makeUpperSnakeName(member->name()), member->value()->get<int>());
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
		for (auto& methodInfo : structInfo->methods()) {
			structMemberFuncImplsText.AppendLines(makeFuncBody(structInfo, methodInfo)).NewLine();
		}
	}

	// classes
	OutputBuffer classMemberFuncImplsText;
	for (auto& structInfo : db()->classes()) {
		for (auto& methodInfo : structInfo->methods()) {
			classMemberFuncImplsText.AppendLines(makeFuncBody(structInfo, methodInfo)).NewLine();
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

		ln::FileSystem::writeAllText(makeOutputFilePath("FlatC", fileName), src);
	}
}

ln::String FlatCSourceGenerator::makeWrapSubclassDecls() const
{
	OutputBuffer code;
	for (auto& classSymbol : db()->classes()) {
		code.AppendLine(u"class {0} : public {1}", makeWrapSubclassName(classSymbol), classSymbol->fullName());
		code.AppendLine(u"{");
		code.IncreaseIndent();

		// TODO:

		code.DecreaseIndent();
		code.AppendLine(u"};");
		code.NewLine();
	}
	return code.toString();
}

ln::String FlatCSourceGenerator::makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo)
{
	// function header
	ln::String funcHeader = FlatCCommon::makeFuncHeader(config(), methodInfo);

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
				args.AppendCommad("LNI_TO_OBJECT({0}, {1})", type->shortName(), paramInfo->name());
			}
			else if (type->isStruct())
			{
				ln::String castTo = type->shortName();
				if (paramInfo->isIn()) castTo = "const " + castTo;
				args.AppendCommad("*reinterpret_cast<{0}*>({1})", castTo, paramInfo->name());
			}
			else if (type->isEnum())
			{
				args.AppendCommad("static_cast<{0}>({1})", type->shortName(), paramInfo->name());
			}
			else if (type == PredefinedTypes::boolType)
			{
				args.AppendCommad("LNI_TO_BOOL({0})", paramInfo->name());
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
				body.append("*outReturn = reinterpret_cast<const LN{0}&>", methodInfo->returnType()->shortName());
			else if (methodInfo->returnType()->isClass())
				body.append("*outReturn = LNI_TO_HANDLE");
			else if (methodInfo->returnType() == PredefinedTypes::boolType)
				body.append("*outReturn = LNI_BOOL_TO_LNBOOL");
			else if (methodInfo->returnType() == PredefinedTypes::stringType)
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
				macroArgs.AppendCommad("LN" + typeInfo->shortName());	// クラス名
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
				callExp = ln::String::format("(LNI_TO_OBJECT(LN{0}, {1})->{2}({3}));", typeInfo->shortName(), FlatCCommon::makeInstanceParamName(typeInfo), name, args.toString());
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

