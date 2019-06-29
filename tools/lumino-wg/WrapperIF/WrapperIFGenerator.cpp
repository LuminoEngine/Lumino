
#include "FlatCCommon.hpp"
#include "WrapperIFGenerator.hpp"

const ln::String funcHeaderTemplate =
"LN_API LNResultCode %%FuncName%%(%%ParamList%%)";

const ln::String funcBodyTemplate =
	"{\n"
	"    LWIG_FUNC_TRY_BEGIN;\n"
	"    %%FuncBody%%\n"
	"    LWIG_FUNC_TRY_END_RETURN;\n"
	"}\n";

WrapperIFGenerator::WrapperIFGenerator()
{
}

void WrapperIFGenerator::generate()
{
	// structs
	OutputBuffer structsText;
	OutputBuffer structMemberFuncDeclsText;
	OutputBuffer structMemberFuncImplsText;
	for (auto& structInfo : db()->structs)
	{
		structsText.AppendLine("struct LN{0}", structInfo->shortName());
		structsText.AppendLine("{");
		structsText.IncreaseIndent();
		for (auto& fieldInfo : structInfo->declaredFields)
		{
			structsText.AppendLine("{0} {1};", fieldInfo->type->shortName(), fieldInfo->name);
		}
		structsText.DecreaseIndent();
		structsText.AppendLine("};");

		// function decls
		for (auto& methodInfo : structInfo->declaredMethods)
		{
			// comment
			structMemberFuncDeclsText.AppendLines(MakeDocumentComment(methodInfo->document));

			// decl
			structMemberFuncDeclsText.AppendLines(makeFuncHeader(methodInfo)).append(";").NewLine(2);
		}

		// function impls
		for (auto& methodInfo : structInfo->declaredMethods)
		{
			structMemberFuncImplsText.AppendLines(makeFuncBody(structInfo, methodInfo)).NewLine();
		}
	}

	// classes
	OutputBuffer classMemberFuncDeclsText;
	OutputBuffer classMemberFuncImplsText;
	for (auto& classInfo : db()->classes)
	{
		classMemberFuncDeclsText.AppendLine("//==============================================================================");
		classMemberFuncDeclsText.AppendLine("// {0}", classInfo->fullName());
		classMemberFuncDeclsText.AppendLine("//==============================================================================").NewLine();

		// function decls
		for (auto& methodInfo : classInfo->declaredMethods)
		{
			// comment
			classMemberFuncDeclsText.AppendLines(MakeDocumentComment(methodInfo->document));

			// decl
			classMemberFuncDeclsText.AppendLines(makeFuncHeader(methodInfo)).append(";").NewLine(2);
		}

		// function impls
		for (auto& methodInfo : classInfo->declaredMethods)
		{
			classMemberFuncImplsText.AppendLines(makeFuncBody(classInfo, methodInfo)).NewLine();
		}

		if (!classInfo->isStatic())
		{
			static const ln::String setBindingTypeInfo =
				"LN_API void LN%ClassName%_SetBindingTypeInfo(void* data)\n"
				"{\n"
				"    tr::TypeInfo::getTypeInfo<%ClassName%>()->setBindingTypeInfo(data);\n"
				"}\n";
			classMemberFuncImplsText.AppendLines(setBindingTypeInfo.replace("%ClassName%", classInfo->shortName()));
		}
	}

	// enums
	OutputBuffer enumsText;
	{
		for (auto& enumInfo : db()->enums)
		{
			enumsText.AppendLines(MakeDocumentComment(enumInfo->document));
			enumsText.AppendLine("typedef enum tagLN{0}", enumInfo->shortName());
			enumsText.AppendLine("{");
			enumsText.IncreaseIndent();
			for (auto& constantInfo : enumInfo->declaredConstants)
			{
				// comment
				enumsText.AppendLines(MakeDocumentComment(constantInfo->document));

				// member
				ln::String name = (enumInfo->shortName() + "_" + constantInfo->name).toUpper();
				enumsText.AppendLine("LN_{0} = {1},", name, constantInfo->value->get<int>()).NewLine();
			}
			enumsText.DecreaseIndent();
			enumsText.AppendLine("}} LN{0};", enumInfo->shortName());
		}
	}

	// delegates
	OutputBuffer delegatesText;
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


	// save C API Header
	{
		ln::String src = ln::FileSystem::readAllText(makeTemplateFilePath(_T("LuminoC.h")));
		src = src.replace("%%Structs%%", structsText.toString());
		src = src.replace("%%Enums%%", enumsText.toString());
		src = src.replace("%%Delegates%%", delegatesText.toString());
		src = src.replace("%%StructMemberFuncDecls%%", structMemberFuncDeclsText.toString());
		src = src.replace("%%ClassMemberFuncDecls%%", classMemberFuncDeclsText.toString());

		ln::String fileName = ln::String::format("{0}.FlatC.generated.h", moduleFullName());
		ln::FileSystem::writeAllText(makeOutputFilePath(fileName).c_str(), src, ln::TextEncoding::utf8Encoding());
	}
	// save C API Source
	{
		ln::String src = ln::FileSystem::readAllText(makeTemplateFilePath(_T("Source.cpp")));
		src = src.replace("%%StructMemberFuncImpls%%", structMemberFuncImplsText.toString());
		src = src.replace("%%ClassMemberFuncImpls%%", classMemberFuncImplsText.toString());

		ln::String fileName = ln::String::format("{0}.FlatC.generated.cpp", moduleFullName());
		ln::FileSystem::writeAllText(makeOutputFilePath(fileName).c_str(), src);
	}
}


ln::String WrapperIFGenerator::MakeInstanceParamName(ln::Ref<TypeSymbol> info)
{
	return info->shortName().toLower();
}

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
//				.replace("%FuncBody%", ln::String::format("LWIG_TO_OBJECT(LN{0}, self)->{1}.Connect(callback);", typeInfo->shortName(), MakeEventWrapperMemberVariableName(methodInfo))));
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

// 宣言文の作成。ドキュメンテーションコメントは含まない。
ln::String WrapperIFGenerator::makeFuncHeader(ln::Ref<MethodSymbol> methodInfo)
{
	// make params
	OutputBuffer params;
	for (auto& paramInfo : methodInfo->capiParameters)
	{
		params.AppendCommad("{0} {1}", FlatCCommon::makeFlatCParamTypeName(methodInfo, paramInfo), paramInfo->name);
	}

	ln::String suffix = (methodInfo->isVirtual) ? "_CallVirtualBase" : "";

	return funcHeaderTemplate
		.replace("%%ClassName%%", methodInfo->owner->shortName())
		.replace("%%FuncName%%", methodInfo->GetCAPIFuncName() + suffix)
		.replace("%%ParamList%%", params.toString());
}

ln::String WrapperIFGenerator::makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo)
{
	// function header
	ln::String funcHeader = makeFuncHeader(methodInfo);

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
	if (methodInfo->IsRuntimeInitializer())
	{
		body.AppendLine("LFManager::preInitialize();");
	}

	{
		// make call args
		OutputBuffer args;
		for (auto& paramInfo : methodInfo->parameters)
		{
			if (paramInfo->type->IsClass())
			{
				args.AppendCommad("LWIG_TO_OBJECT({0}, {1})", paramInfo->type->shortName(), paramInfo->name);
			}
			else if (paramInfo->type->isStruct)
			{
				ln::String castTo = paramInfo->type->shortName();
				if (paramInfo->isIn) castTo = "const " + castTo;
				args.AppendCommad("*reinterpret_cast<{0}*>({1})", castTo, paramInfo->name);
			}
			else if (paramInfo->type->isEnum)
			{
				args.AppendCommad("static_cast<{0}>({1})", paramInfo->type->shortName(), paramInfo->name);
			}
			else if (paramInfo->type == PredefinedTypes::boolType)
			{
				args.AppendCommad("LWIG_TO_BOOL({0})", paramInfo->name);
			}
			else
			{
				args.AppendCommad(paramInfo->name);
			}
		}

		// return output
		if (!methodInfo->returnType->isVoid)
		{
			if (methodInfo->returnType->isStruct)
				body.append("*outReturn = reinterpret_cast<const LN{0}&>", methodInfo->returnType->shortName());
			else if (methodInfo->returnType->IsClass())
				body.append("*outReturn = LWIG_TO_HANDLE");
			else if (methodInfo->returnType == PredefinedTypes::boolType)
				body.append("*outReturn = LWIG_TO_LNBOOL");
			else if (methodInfo->returnType == PredefinedTypes::stringType)
				body.append("*outReturn = LWIG_STRING_TO_STRPTR");
			else
				body.append("*outReturn = ");
		}

		// call expression
		ln::String callExp;
		if (typeInfo->isStruct)
		{
			if (methodInfo->isConstructor)
				callExp = ln::String::format("new (reinterpret_cast<{0}*>({1})) {2}({3});", typeInfo->shortName(), MakeInstanceParamName(typeInfo), methodInfo->name, args.toString());
			else if (methodInfo->isStatic)
				body.append("({0}::{1}({2}));", typeInfo->shortName(), methodInfo->name, args.toString());
			else
			{
				ln::String castTo = typeInfo->shortName();
				if (methodInfo->isConst) castTo = "const " + castTo;
				callExp = ln::String::format("(reinterpret_cast<{0}*>({1})->{2}({3}));", castTo, MakeInstanceParamName(typeInfo), methodInfo->name, args.toString());
			}
		}
		else
		{
			// static 関数
			if (methodInfo->isStatic)
			{
				body.append("({0}::{1}({2}));", typeInfo->shortName(), methodInfo->name, args.toString());
			}
			// コンストラクタ 関数
			else if (methodInfo->isConstructor)
			{
				OutputBuffer macroArgs;
				macroArgs.AppendCommad("out" + typeInfo->shortName());	// 格納する変数名
				macroArgs.AppendCommad("LN" + typeInfo->shortName());	// クラス名
				macroArgs.AppendCommad(methodInfo->name);		// 初期化関数名
				macroArgs.AppendCommad(args.toString());		// 引数
				callExp = ln::String::format("LWIG_CREATE_OBJECT({0});", macroArgs.toString());
			}
			// 普通のインスタンス 関数
			else
			{
				ln::String name = methodInfo->name;
				if (methodInfo->isVirtual)
					name = ("LN" + typeInfo->shortName() + "::" + name + "_CallBase");
				callExp = ln::String::format("(LWIG_TO_OBJECT(LN{0}, {1})->{2}({3}));", typeInfo->shortName(), MakeInstanceParamName(typeInfo), name, args.toString());
			}
		}

		body.AppendLine(callExp);
	}

	if (methodInfo->IsRuntimeInitializer())
	{
		body.AppendLine("LFManager::postInitialize();");
	}

	return funcHeader + NewLine + funcBodyTemplate
		.replace("%%FuncBody%%", body.toString());
}


ln::String WrapperIFGenerator::MakeDocumentComment(ln::Ref<DocumentSymbol> doc)
{
	OutputBuffer text;
	text.AppendLine("/**");
	text.IncreaseIndent();

	// @brief
	text.AppendLine("@brief {0}", doc->summary);

	// @param
	for (auto& param : doc->params)
	{
		text.AppendLine("@param[{0}] {1} : {2}", param->io, param->name, param->description);
	}

	// @return
	if (!doc->returns.isEmpty())
		text.AppendLine("@return {0}", doc->returns);

	// @details
	if (!doc->details.isEmpty())
		text.AppendLine("@details {0}", doc->details);

	text.DecreaseIndent();
	text.AppendLine("*/");

	return text.toString();
}

