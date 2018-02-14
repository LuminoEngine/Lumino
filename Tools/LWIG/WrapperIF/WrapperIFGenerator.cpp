
#include "FlatCCommon.h"
#include "WrapperIFGenerator.h"

const String funcHeaderTemplate =
"LN_API LNResultCode %%FuncName%%(%%ParamList%%)";

const String funcBodyTemplate =
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
			structMemberFuncDeclsText.AppendLines(MakeMethodHeader(methodInfo)).append(";").NewLine(2);
		}

		// function impls
		for (auto& methodInfo : structInfo->declaredMethods)
		{
			structMemberFuncImplsText.AppendLines(MakeMethod(structInfo, methodInfo)).NewLine();
		}
	}

	//// classes
	//OutputBuffer buffer;
	//for (auto& classInfo : db()->classes)
	//{
	//	buffer.AppendLines(MakeMethods(classInfo));

	//	if (!classInfo->isStatic())
	//	{
	//		static const String setBindingTypeInfo =
	//			"LN_API void LN%ClassName%_SetBindingTypeInfo(void* data)\n"
	//			"{\n"
	//			"    tr::TypeInfo::GetTypeInfo<%ClassName%>()->SetBindingTypeInfo(data);\n"
	//			"}\n";
	//		buffer.AppendLines(setBindingTypeInfo.replace("%ClassName%", classInfo->shortName()));
	//	}
	//}

	// enums
	OutputBuffer enumsText;
	{
		for (auto& enumInfo : db()->enums)
		{
			enumsText.AppendLine("/** {0} */", MakeDocumentComment(enumInfo->document));
			enumsText.AppendLine("typedef enum tagLN{0}", enumInfo->shortName());
			enumsText.AppendLine("{");
			enumsText.IncreaseIndent();
			for (auto& constantInfo : enumInfo->declaredConstants)
			{
				String name = (enumInfo->shortName() + "_" + constantInfo->name).toUpper();
				enumsText.AppendLine("LN_{0} = {1},", name, constantInfo->value.get<int>());
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
				params.AppendCommad("{0} {1}", FlatCCommon::MakeCApiParamTypeName(involeMethod, paramInfo), paramInfo->name);
			}

			//enumsText.AppendLine("/** {0} */", MakeDocumentComment(delegateInfo->document));
			delegatesText.AppendLine("typedef void (*LN{0})({1});", delegateInfo->shortName(), params.toString());
		}
	}


	// save C API Header
	{
		String src = FileSystem::readAllText(makeTemplateFilePath(_T("LuminoC.h")));
		src = src.replace("%%Structs%%", structsText.toString());
		src = src.replace("%%Enums%%", enumsText.toString());
		src = src.replace("%%Delegates%%", delegatesText.toString());
		src = src.replace("%%StructMemberFuncDecls%%", structMemberFuncDeclsText.toString());

		String fileName = String::format("{0}.FlatC.generated.h", moduleFullName());
		FileSystem::writeAllText(makeOutputFilePath(fileName).c_str(), src, Encoding::getUTF8Encoding());
	}
	// save C API Source
	{
		String src = FileSystem::readAllText(makeTemplateFilePath(_T("Source.cpp")));
		src = src.replace("%Contents%", structMemberFuncImplsText.toString());

		String fileName = String::format("{0}.FlatC.generated.cpp", moduleFullName());
		FileSystem::writeAllText(makeOutputFilePath(fileName).c_str(), src);
	}
}


String WrapperIFGenerator::MakeInstanceParamName(Ref<TypeInfo> info)
{
	return info->shortName().toLower();
}

//String WrapperIFGenerator::MakeMethods(Ref<TypeInfo> typeInfo)
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
//				.replace("%ParamList%", String::format("LNHandle self, LN{0} callback", delegateClass->shortName()))
//				.replace("%FuncBody%", String::format("LWIG_TO_OBJECT(LN{0}, self)->{1}.Connect(callback);", typeInfo->shortName(), MakeEventWrapperMemberVariableName(methodInfo))));
//		}
//		else
//		{
//			buffer.AppendLines(MakeMethod(typeInfo, methodInfo, false));
//
//			// override
//			if (methodInfo->isVirtual)
//			{
//				// base caller
//				buffer.AppendLines(MakeMethod(typeInfo, methodInfo, true));
//
//				// override setter
//				buffer.AppendLines(funcBodyTemplate
//					.replace("%ClassName%", typeInfo->shortName())
//					.replace("%FuncName%", methodInfo->GetCAPIFuncName() + "_SetOverrideCaller")
//					.replace("%ParamList%", String::format("{0}_{1}_OverrideCaller callback", typeInfo->shortName(), methodInfo->name))
//					.replace("%FuncBody%", String::format("LN{0}::m_{1}_OverrideCaller = callback;", typeInfo->shortName(), methodInfo->name)));
//			}
//		}
//	}
//
//	return buffer.toString();
//}

// 宣言文の作成。ドキュメンテーションコメントは含まない。
String WrapperIFGenerator::MakeMethodHeader(Ref<MethodInfo> methodInfo)
{
	// make params
	OutputBuffer params;
	for (auto& paramInfo : methodInfo->capiParameters)
	{
		params.AppendCommad("{0} {1}", FlatCCommon::MakeCApiParamTypeName(methodInfo, paramInfo), paramInfo->name);
	}

	String suffix = (methodInfo->isVirtual) ? "_CallVirtualBase" : "";

	return funcHeaderTemplate
		.replace("%%ClassName%%", methodInfo->owner->shortName())
		.replace("%%FuncName%%", methodInfo->GetCAPIFuncName() + suffix)
		.replace("%%ParamList%%", params.toString());
}

String WrapperIFGenerator::MakeMethod(Ref<TypeInfo> typeInfo, Ref<MethodInfo> methodInfo)
{
	// function header
	String funcHeader = MakeMethodHeader(methodInfo);

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
		body.append("LFManager::PreInitialize();");
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
				String castTo = paramInfo->type->shortName();
				if (paramInfo->isIn) castTo = "const " + castTo;
				args.AppendCommad("*reinterpret_cast<{0}*>({1})", castTo, paramInfo->name);
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
			else
				body.append("*outReturn = ");
		}

		// call expression
		String callExp;
		if (typeInfo->isStruct)
		{
			if (methodInfo->isConstructor)
				callExp = String::format("new (reinterpret_cast<{0}*>({1})) {2}({3});", typeInfo->shortName(), MakeInstanceParamName(typeInfo), methodInfo->name, args.toString());
			else if (methodInfo->isStatic)
				body.append("({0}::{1}({2}));", typeInfo->shortName(), methodInfo->name, args.toString());
			else
			{
				String castTo = typeInfo->shortName();
				if (methodInfo->isConst) castTo = "const " + castTo;
				callExp = String::format("(reinterpret_cast<{0}*>({1})->{2}({3}));", castTo, MakeInstanceParamName(typeInfo), methodInfo->name, args.toString());
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
				callExp = String::format("LWIG_CREATE_OBJECT({0});", macroArgs.toString());
			}
			// 普通のインスタンス 関数
			else
			{
				String name = methodInfo->name;
				if (methodInfo->isVirtual)
					name = ("LN" + typeInfo->shortName() + "::" + name + "_CallBase");
				callExp = String::format("(LWIG_TO_OBJECT(LN{0}, {1})->{2}({3}));", typeInfo->shortName(), MakeInstanceParamName(typeInfo), name, args.toString());
			}
		}

		body.append(callExp);
	}

	if (methodInfo->IsRuntimeInitializer())
		body.append("LFManager::PostInitialize();");

	return funcHeader + NewLine + funcBodyTemplate
		.replace("%%FuncBody%%", body.toString());
}


String WrapperIFGenerator::MakeDocumentComment(Ref<DocumentInfo> doc)
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

