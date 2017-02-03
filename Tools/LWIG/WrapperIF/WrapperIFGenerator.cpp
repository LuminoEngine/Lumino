
#include "../Global.h"
#include "../SymbolDatabase.h"
#include "WrapperIFGenerator.h"

const StringA funcTemplate =
	"LN_API LNResultCode %FuncName%(%ParamList%)\n"
	"{\n"
	"    LWIG_FUNC_TRY_BEGIN;\n"
	"    %FuncBody%\n"
	"    LWIG_FUNC_TRY_END_RETURN;\n"
	"}\n";

const StringA wrapperIFClassTemplate =
	"class LN%ClassName% : public %ClassName%\n"
	"{\n"
	"public:\n"
	"%OverrideFuncPtrs%\n"
	"\n"
	"%OverrideMethods%\n"
	"};\n"
	"%OverrideFuncPtrImpls%\n";

void WrapperIFGenerator::Generate(SymbolDatabase* database)
{
	m_database = database;

	// structs
	OutputBuffer buffer;
	for (auto& structInfo : m_database->structs)
	{
		buffer.AppendLine("struct LN{0}", structInfo->name);
		buffer.AppendLine("{");
		buffer.IncreaseIndent();
		for (auto& fieldInfo : structInfo->declaredFields)
		{
			buffer.AppendLine("{0} {1};", fieldInfo->type->name, fieldInfo->name);
		}
		buffer.DecreaseIndent();
		buffer.AppendLine("};");

		buffer.AppendLines(MakeMethods(structInfo));
	}

	// classes
	for (auto& classInfo : m_database->classes)
	{
		buffer.AppendLines(MakeMethods(classInfo));

		if (!classInfo->IsStatic())
		{
			static const String SetBindingTypeInfo =
				"LN_API void LN%ClassName%_SetBindingTypeInfo(void* data)\n"
				"{\n"
				"    tr::TypeInfo::GetTypeInfo<%ClassName%>()->SetBindingTypeInfo(data);\n"
				"}\n";
			buffer.AppendLines(SetBindingTypeInfo.Replace("%ClassName%", classInfo->name));
		}
	}

	// enums
	OutputBuffer enumsText;
	{
		for (auto& enumInfo : m_database->enums)
		{
			enumsText.AppendLine("/** {0} */", MakeDocumentComment(enumInfo->document));
			enumsText.AppendLine("typedef enum tagLN{0}", enumInfo->name);
			enumsText.AppendLine("{");
			enumsText.IncreaseIndent();
			for (auto& constantInfo : enumInfo->declaredConstants)
			{
				String name = (enumInfo->name + "_" + constantInfo->name).ToUpper();
				enumsText.AppendLine("LN_{0} = {1},", name, tr::Variant::Cast<int>(constantInfo->value));
			}
			enumsText.DecreaseIndent();
			enumsText.AppendLine("}} LN{0};", enumInfo->name);
		}
	}

	{
		String src = FileSystem::ReadAllText(PathName(g_templateDir, "WrapperIF/Templates/Source.cpp"));
		src = src.Replace("%Contents%", buffer.ToString());
		FileSystem::WriteAllText(LUMINO_ROOT_DIR"/bindings/Runtime/src/WrapperIF.generated.cpp", src);
	}
	{
		String src = FileSystem::ReadAllText(PathName(g_templateDir, "WrapperIF/Templates/WrapperIF.h"));
		src = src.Replace("%OverrideCallerFuncPtrs%", GenerateOverrideCallerFuncPtrs());
		src = src.Replace("%WrapperClasses%", GenerateWrapperIFClasses());
		FileSystem::WriteAllText(LUMINO_ROOT_DIR"/bindings/Runtime/src/WrapperIF.generated.h", src);
	}

	{
		String src = FileSystem::ReadAllText(PathName(g_templateDir, "WrapperIF/Templates/LuminoC.h"));
		src = src.Replace("%Enums%", enumsText.ToString());
		FileSystem::WriteAllText(LUMINO_ROOT_DIR"/bindings/Runtime/src/LuminoC.generated.h", src);
	}
}

String WrapperIFGenerator::GenerateOverrideCallerFuncPtrs()
{
	OutputBuffer funcPtrDefs;
	for (auto& classInfo : m_database->classes)
	{
		if (!classInfo->IsStatic())
		{
			for (auto& methodInfo : classInfo->declaredMethods)
			{
				if (!methodInfo->isVirtual) continue;

				// make params
				OutputBuffer params;
				for (auto& paramInfo : methodInfo->capiParameters)
				{
					params.AppendCommad("{0} {1}", MakeCApiParamTypeName(methodInfo, paramInfo), paramInfo->name);
				}

				funcPtrDefs.Append("typedef LNResultCode (*{0}_{1}_OverrideCaller)({2});", classInfo->name, methodInfo->name, params.ToString()).NewLine();
				//funcPtrDefs.Append("{0}_OverrideCaller	m_{0}_OverrideCaller;", methodInfo->name).NewLine();
			}
		}
	}

	return funcPtrDefs.ToString();
}

String WrapperIFGenerator::GenerateWrapperIFClasses()
{
	OutputBuffer classes;
	for (auto& classInfo : m_database->classes)
	{
		if (!classInfo->IsStatic())
		{
			// 仮想関数の override
			OutputBuffer overrideCallersDecl(1);
			OutputBuffer overrideCallersImpl(0);
			OutputBuffer overrideMethods(1);
			for (auto& methodInfo : classInfo->declaredMethods)
			{
				if (!methodInfo->isVirtual) continue;

				// member variable decl
				String typeName = String::Format("{0}_{1}_OverrideCaller", classInfo->name, methodInfo->name);
				String varName = String::Format("m_{0}_OverrideCaller", methodInfo->name);
				overrideCallersDecl.Append("static {0} {1};", typeName, varName).NewLine();

				// member variable impl
				overrideCallersImpl.Append("{0} LN{1}::{2} = nullptr;", typeName, classInfo->name, varName).NewLine();

				// call args
				OutputBuffer args;
				for (auto& paramInfo : methodInfo->capiParameters)
				{
					if (paramInfo->isThis)
						args.AppendCommad("LWIG_TO_HANDLE(this)");
					else
						LN_NOTIMPLEMENTED();
				}

				// member method
				overrideMethods.Append("virtual {0} {1}() override", MakeCppTypeName(methodInfo->returnType), methodInfo->name).NewLine();
				overrideMethods.Append("{").NewLine();
				overrideMethods.IncreaseIndent();
				overrideMethods.Append("{0}({1});", varName, args.ToString()).NewLine();
				overrideMethods.DecreaseIndent();
				overrideMethods.Append("}").NewLine();

				// member method
				overrideMethods.Append("template<typename... TArgs>").NewLine();
				overrideMethods.Append("{0} {1}_CallBase(TArgs... args)", MakeCppTypeName(methodInfo->returnType), methodInfo->name).NewLine();
				overrideMethods.Append("{").NewLine();
				overrideMethods.IncreaseIndent();
				overrideMethods.Append("return {0}::{1}(args...);", methodInfo->owner->name, methodInfo->name).NewLine();
				overrideMethods.DecreaseIndent();
				overrideMethods.Append("}").NewLine();
			}

			classes.Append(wrapperIFClassTemplate
				.Replace("%ClassName%", classInfo->name)
				.Replace("%OverrideFuncPtrs%", overrideCallersDecl.ToString())
				.Replace("%OverrideMethods%", overrideMethods.ToString())
				.Replace("%OverrideFuncPtrImpls%", overrideCallersImpl.ToString()));
		}
	}

	return classes.ToString();
}

StringA WrapperIFGenerator::MakeInstanceParamName(TypeInfoPtr info)
{
	return info->name.ToLower();
}

StringA WrapperIFGenerator::MakeMethods(TypeInfoPtr typeInfo)
{
	OutputBuffer buffer;

	for (auto& methodInfo : typeInfo->declaredMethods)
	{
		buffer.AppendLines(MakeMethod(typeInfo, methodInfo, false));
		if (methodInfo->isVirtual)
		{
			// base caller
			buffer.AppendLines(MakeMethod(typeInfo, methodInfo, true));

			// override setter
			buffer.AppendLines(funcTemplate
				.Replace("%ClassName%", typeInfo->name)
				.Replace("%FuncName%", methodInfo->GetCAPIFuncName() + "_SetOverrideCaller")
				.Replace("%ParamList%", String::Format("{0}_{1}_OverrideCaller callback", typeInfo->name, methodInfo->name))
				.Replace("%FuncBody%", String::Format("LN{0}::m_{1}_OverrideCaller = callback;", typeInfo->name, methodInfo->name)));
		}
	}

	return buffer.ToString();
}

StringA WrapperIFGenerator::MakeMethod(TypeInfoPtr typeInfo, MethodInfoPtr methodInfo, bool virtualBase)
{
	// make params
	OutputBuffer params;
	for (auto& paramInfo : methodInfo->capiParameters)
	{
		params.AppendCommad("{0} {1}", MakeCApiParamTypeName(methodInfo, paramInfo), paramInfo->name);
	}
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
		body.Append("LFManager::PreInitialize();");
	{
		// make call args
		OutputBuffer args;
		for (auto& paramInfo : methodInfo->parameters)
		{
			if (paramInfo->type->IsClass())
			{
				args.AppendCommad("LWIG_TO_OBJECT({0}, {1})", paramInfo->type->name, paramInfo->name);
			}
			else if (paramInfo->type->isStruct)
			{
				String castTo = paramInfo->type->name;
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
				body.Append("*outReturn = reinterpret_cast<const LN{0}&>", methodInfo->returnType->name);
			else if (methodInfo->returnType->IsClass())
				body.Append("*outReturn = LWIG_TO_HANDLE");
			else
				body.Append("*outReturn = ");
		}

		// call expression
		String callExp;
		if (typeInfo->isStruct)
		{
			if (methodInfo->isConstructor)
				callExp = String::Format("new (reinterpret_cast<{0}*>({1})) {2}({3});", typeInfo->name, MakeInstanceParamName(typeInfo), methodInfo->name, args.ToString());
			else if (methodInfo->isStatic)
				body.Append("({0}::{1}({2}));", typeInfo->name, methodInfo->name, args.ToString());
			else
			{
				String castTo = typeInfo->name;
				if (methodInfo->isConst) castTo = "const " + castTo;
				callExp = String::Format("(reinterpret_cast<{0}*>({1})->{2}({3}));", castTo, MakeInstanceParamName(typeInfo), methodInfo->name, args.ToString());
			}
		}
		else
		{
			// static 関数
			if (methodInfo->isStatic)
			{
				body.Append("({0}::{1}({2}));", typeInfo->name, methodInfo->name, args.ToString());
			}
			// コンストラクタ 関数
			else if (methodInfo->isConstructor)
			{
				OutputBuffer macroArgs;
				macroArgs.AppendCommad("out" + typeInfo->name);	// 格納する変数名
				macroArgs.AppendCommad("LN" + typeInfo->name);	// クラス名
				macroArgs.AppendCommad(methodInfo->name);		// 初期化関数名
				macroArgs.AppendCommad(args.ToString());		// 引数
				callExp = String::Format("LWIG_CREATE_OBJECT({0});", macroArgs.ToString());
			}
			// 普通のインスタンス 関数
			else
			{
				String name = methodInfo->name;
				if (virtualBase) 
					name = ("LN" + typeInfo->name + "::" + name + "_CallBase");
				callExp = String::Format("(LWIG_TO_OBJECT(LN{0}, {1})->{2}({3}));", typeInfo->name, MakeInstanceParamName(typeInfo), name, args.ToString());
			}
		}

		body.Append(callExp);
	}

	if (methodInfo->IsRuntimeInitializer())
		body.Append("LFManager::PostInitialize();");

	String suffix = (virtualBase) ? "_CallVirtualBase" : "";

	return funcTemplate
		.Replace("%ClassName%", typeInfo->name)
		.Replace("%FuncName%", methodInfo->GetCAPIFuncName() + suffix)
		.Replace("%ParamList%", params.ToString())
		.Replace("%FuncBody%", body.ToString());
}

StringA WrapperIFGenerator::MakeCApiParamTypeName(MethodInfoPtr methodInfo, ParameterInfoPtr paramInfo)
{
	auto typeInfo = paramInfo->type;

	if (typeInfo->isStruct)
	{
		String modifer;
		if (paramInfo->isThis && methodInfo->isConst)
			modifer = "const ";
		if (!paramInfo->isThis && !paramInfo->isOut)
			modifer = "const ";
		return String::Format("{0}LN{1}*", modifer, typeInfo->name);
	}

	StringA name;
	if (typeInfo == PredefinedTypes::stringType)
		name = "const LNChar*";
	else if (typeInfo->IsClass())
		name = "LNHandle";
	else
		name = typeInfo->name;

	if (paramInfo->isOut || paramInfo->isReturn)
		name += "*";

	//if (typeInfo->isStruct && paramInfo->isThis)
	//{
	//	if (methodInfo->isConst)
	//		name = "const " + name;
	//	name += "*";
	//}

	return name;
//	StringA name;
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

StringA WrapperIFGenerator::MakeCppTypeName(TypeInfoPtr typeInfo)
{
	//if (paramInfo->type->IsClass())
	return typeInfo->name;
}

String WrapperIFGenerator::MakeDocumentComment(DocumentInfoPtr doc)
{
	return doc->summary;
}

