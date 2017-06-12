
#include "../Global.h"
#include "../SymbolDatabase.h"
#include "DotNetCommon.h"
#include "CSClassLibGenerator.h"
#include "DotNetPInvokeLibGenerator.h"

static const String MethodBodyTemplate =
	_T("{\n")
	_T("    %InitStmt%\n")
	_T("    var result = API.%APIFuncName%(%CallArgList%);\n")
	_T("    if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);\n")
	_T("    %ReturnStmt%\n")
	_T("}\n");

void CSClassLibGenerator::Generate()
{
	OutputBuffer classesText(1);
	OutputBuffer typeInfoRegistersText(3);
	OutputBuffer typeInfoPInvolesText(2);
	for (auto& classInfo : g_database.classes)
	{
		// ランタイムへの TypeInfo 登録コード
		if (!classInfo->IsStatic())
			MakeTypeInfoRegisters(classInfo, &typeInfoRegistersText, &typeInfoPInvolesText);

		// XmlDocument
		classesText.AppendLines(DotNetCommon::MakeXmlDocument(classInfo->document));

		// class header
		String modifier = (classInfo->IsStatic()) ? "static" : "";
		classesText.Append("public {0} class {1}", modifier, classInfo->name).NewLine();
		if (classInfo->baseClass != nullptr) classesText.Append("    : " + classInfo->baseClass->name).NewLine();
		classesText.Append("{").NewLine();
		classesText.IncreaseIndent();

		// property
		for (auto& propInfo : classInfo->declaredProperties)
		{
			OutputBuffer prop;

			// xml document
			classesText.AppendLines(DotNetCommon::MakeXmlDocument(propInfo->document));

			// property header
			classesText.Append("public {0} {1}", DotNetCommon::MakeTypeName(propInfo->type), propInfo->namePrefix + propInfo->name).NewLine();
			classesText.Append("{").NewLine();
			classesText.IncreaseIndent();

			if (propInfo->getter != nullptr)
			{
				classesText.Append("get").NewLine();
				classesText.AppendLines(MakeMethodBody(propInfo->getter, true));
			}
			if (propInfo->setter != nullptr)
			{
				classesText.Append("set").NewLine();
				classesText.AppendLines(MakeMethodBody(propInfo->setter, true));
			}

			classesText.DecreaseIndent();
			classesText.Append("}").NewLine(2);
		}

		// methods
		for (auto& methodInfo : classInfo->declaredMethods)
		{
			if (methodInfo->ownerProperty != nullptr) continue;	// プロパティの場合はスキップ
			if (methodInfo->IsEventSetter()) continue;

			// xml document
			classesText.AppendLines(DotNetCommon::MakeXmlDocument(methodInfo->document));

			// method header
			String modifier = MethodInfo::GetAccessLevelName(methodInfo->accessLevel);
			if (methodInfo->isStatic) modifier += " static";
			if (methodInfo->isVirtual) modifier += " virtual";
			if (methodInfo->isConstructor)
			{
				classesText.Append("{0} {1}({2}) : base(_LNInternal.InternalBlock)", modifier, classInfo->name, MakeMethodHeaderParamList(methodInfo)).NewLine();
			}
			else
			{
				classesText.Append("{0} {1} {2}({3})", modifier, DotNetCommon::MakeTypeName(methodInfo->returnType), methodInfo->name, MakeMethodHeaderParamList(methodInfo)).NewLine();
			}

			// method body
			classesText.AppendLines(MakeMethodBody(methodInfo, false)).NewLine();

			// override callback
			if (methodInfo->isVirtual)
			{
				classesText.AppendLines(MakeOverrideCallbackMethodBody(methodInfo)).NewLine();
				typeInfoRegistersText.Append("API.LN{0}_{1}_SetOverrideCaller({0}.{1}_OverrideCallback);", classInfo->name, methodInfo->name);
			}
		}

		// events
		for (auto& methodInfo : classInfo->declaredMethods)
		{
			if (!methodInfo->IsEventSetter()) continue;

			// event
			m_eventsText.AppendLine("public event {0} {1};", methodInfo->parameters[0]->type->name, MakeEventName(methodInfo));
		
			// EventCallback
			auto invokeMethod = methodInfo->parameters[0]->type->declaredMethods[0];
			OutputBuffer args;
			for (auto param : invokeMethod->parameters)
			{
				if (param->type->IsClass())
					args.AppendCommad("InternalManager.GetWrapperObject<{0}>({1})", param->type->name, param->name);
				else
					args.AppendCommad(param->name);
			}
			m_eventsText.AppendLine("internal static void {0}({1})", MakeEventCallbackMethodName(methodInfo), DotNetPInvokeLibGenerator::MakePInvokeMethodDeclParamList(invokeMethod));
			m_eventsText.AppendLine("{");
			m_eventsText.AppendLine("    var this_ = InternalManager.GetWrapperObject<{0}>(sender);", classInfo->name);
			m_eventsText.AppendLine("    this_.{0}({1});", MakeEventName(methodInfo), args.ToString());
			m_eventsText.AppendLine("}");

			// PostInitialize
			m_posiInitializeText.AppendLine("API.{0}(Handle, {1});", methodInfo->GetCAPIFuncName(), MakeEventCallbackMethodName(methodInfo));
		}

		// internal constructor
		if (!classInfo->IsStatic())
		{
			classesText.Append("internal {0}(_LNInternal i) : base(i) {{}}", classInfo->name).NewLine(2);

			classesText.AppendLine("private void PostInitialize()");
			classesText.AppendLine("{");
			classesText.AppendLines(m_posiInitializeText.ToString());
			classesText.AppendLine("}");
		}

		classesText.Append(m_fieldsText.ToString()).NewLine();
		classesText.Append(m_eventsText.ToString()).NewLine();
		m_fieldsText.clear();
		m_eventsText.clear();

		classesText.DecreaseIndent();
		classesText.Append("}").NewLine();
	}

	// delegates
	for (auto& delegateInfo : g_database.delegates)
	{
		m_delegatesText.Append(
			"public delegate void {0}({1});",
			delegateInfo->name,
			MakeMethodHeaderParamList(delegateInfo->declaredMethods[0])).NewLine();
	}

	// output
	{
		String src = FileSystem::ReadAllText(PathName(g_templateDir, "DotNet/CSClasses.template.cs"));
		src = src.Replace("%Delegates%", m_delegatesText.ToString());
		src = src.Replace("%Classes%", classesText.ToString());
		src = src.Replace("%TypeInfoRegisters%", typeInfoRegistersText.ToString());
		src = src.Replace("%TypeInfoPInvoke%", typeInfoPInvolesText.ToString());
		FileSystem::WriteAllText(PathName(g_csOutputDir, "CSClasses.generated.cs"), src);
	}
}

String CSClassLibGenerator::MakeMethodBody(MethodInfoPtr methodInfo, bool isProperty)
{
	OutputBuffer methodBody;

	// body
	OutputBuffer initStmt;
	OutputBuffer callArgs;
	String returnStmt;
	for (auto& paramInfo : methodInfo->capiParameters)
	{
		// init stmt
		if (paramInfo->isOut || paramInfo->isReturn)
		{
			if (paramInfo->type->isStruct)
				initStmt.Append("{0} {1} = new {0}();", DotNetCommon::MakePInvokeTypeName(paramInfo->type), paramInfo->name);
			else
				initStmt.Append("{0} {1};", DotNetCommon::MakePInvokeTypeName(paramInfo->type), paramInfo->name);
		}

		// call args
		String argName;
		if (paramInfo->isThis)
		{
			argName = "Handle";
		}
		else if(paramInfo->isOut || paramInfo->isReturn)
		{
			argName = "out " + paramInfo->name;
		}
		else
		{
			argName = ((isProperty) ? "value" : paramInfo->name);

			if (paramInfo->type->IsClass())
				argName = String::Format("({0} != null) ? {0}.Handle : IntPtr.Zero", argName);
			if (paramInfo->type->isStruct)
				argName = "ref " + argName;
		}
		callArgs.AppendCommad(argName);

		// return stmt
		if (paramInfo->isReturn)
		{
			if (methodInfo->isConstructor)	// クラスコンストラクタの場合は Manager 登録を行う
				returnStmt = String::Format("InternalManager.RegisterWrapperObject(this, {0}); API.LNObject_Release({0});", paramInfo->name);
			else if (methodInfo->returnType->IsClass())
			{
				returnStmt = String::Format("return InternalManager.ReturnObjectHelper<{0}>({1}, ref _{2});", methodInfo->returnType->name, paramInfo->name, methodInfo->name);

				// cache field
				String modifier = (methodInfo->isStatic) ? "static" : "";
				m_fieldsText.Append("private {0} {1} _{2};", modifier, methodInfo->returnType->name, methodInfo->name).NewLine();
			}
			else
				returnStmt = String::Format("return {0};", paramInfo->name);
		}

		if (methodInfo->isConstructor)
			returnStmt += String::Format("PostInitialize();", paramInfo->name);
	}

	// method body
	String suffix = (methodInfo->isVirtual) ? "_CallVirtualBase" : "";
	methodBody.AppendLines(MethodBodyTemplate
		.Replace("%InitStmt%", initStmt.ToString())
		.Replace("%APIFuncName%", methodInfo->GetCAPIFuncName() + suffix)
		.Replace("%CallArgList%", callArgs.ToString())
		.Replace("%ReturnStmt%", returnStmt));

	return methodBody.ToString();
}

String CSClassLibGenerator::MakeOverrideCallbackMethodBody(MethodInfoPtr methodInfo)
{
	static const String MethodTemplate =
		"internal static ResultCode %Method%_OverrideCallback(%CApiParams%)\n"
		"{\n"
		"	var obj = InternalManager.GetWrapperObject<%Class%>(gamescene);\n"
		"	try\n"
		"	{\n"
		"		obj.%Method%(%Args%);\n"
		"	}\n"
		"	catch (Exception)\n"
		"	{\n"
		"		return ResultCode.ErrorUnknown; \n"
		"	}\n"
		"	return ResultCode.OK;\n"
		"}\n";

	return MethodTemplate
		.Replace("%Method%", methodInfo->name)
		.Replace("%CApiParams%", DotNetPInvokeLibGenerator::MakePInvokeMethodDeclParamList(methodInfo))
		.Replace("%Class%", methodInfo->owner->name)
		.Replace("%Args%", "");

	//OutputBuffer method;
	//method.Append("private static ResultCode {0}_OverrideCallback({1})", methodInfo->name, DotNetPInvokeLibGenerator::MakePInvokeMethodDeclParamList(methodInfo)).NewLine();
	//method.Append("{").NewLine();
	//method.Append("}").NewLine();
	//return method.ToString();
}

void CSClassLibGenerator::MakeTypeInfoRegisters(TypeInfoPtr classInfo, OutputBuffer* typeInfoRegistersText, OutputBuffer* typeInfoPInvolesText)
{
	// ファクトリ
	static const String template1 =
		"var _%Class% = new TypeInfo(){ Factory = (handle) =>\n"
		"{\n"
		"    var obj = new %Class%(_LNInternal.InternalBlock);\n"
		//"    obj.SetHandle(handle);\n"
		"    return obj;\n"
		"}};\n"
		"_typeInfos.Add(_%Class%);\n"
		"LN%Class%_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));\n";
	typeInfoRegistersText->AppendLines(template1.Replace("%Class%", classInfo->name)).NewLine();

	// P/Invoke
	typeInfoPInvolesText->AppendLine("[DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]");
	typeInfoPInvolesText->AppendLine("private static extern void LN{0}_SetBindingTypeInfo(IntPtr data);", classInfo->name).NewLine();
}

String CSClassLibGenerator::MakeMethodHeaderParamList(MethodInfoPtr methodInfo)
{
	OutputBuffer params;
	for (auto& paramInfo : methodInfo->parameters)
	{
		params.AppendCommad("{0} {1}", DotNetCommon::MakeParamTypeName(paramInfo), paramInfo->name);

		// default value
		if (paramInfo->defaultValue != nullptr)
		{
			params.Append(" = {0}", DotNetCommon::MakeLiteral(paramInfo->defaultValue));
		}
	}
	return params.ToString();
}

String CSClassLibGenerator::MakeEventName(MethodInfoPtr eventConnectMethod)
{
	return eventConnectMethod->name.Replace("Connect", "").Replace("On", "");
}

String CSClassLibGenerator::MakeEventCallbackMethodName(MethodInfoPtr method)
{
	return method->name + "_EventCallback";
}
