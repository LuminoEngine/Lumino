
#include "../Global.h"
#include "../SymbolDatabase.h"
#include "DotNetCommon.h"
#include "DotNetClassLibGenerator.h"

static const String MethodBodyTemplate =
	_T("{\n")
	_T("    %InitStmt%\n")
	_T("    var result = API.%APIFuncName%(%CallArgList%);\n")
	_T("    if (result != ResultCode.Ok) throw LuminoException.MakeExceptionFromLastError(result);\n")
	_T("    %ReturnStmt%\n")
	_T("}\n");

void CSStructsGenerator::Generate()
{
	OutputBuffer structText(1);
	for (auto& structInfo : g_database.structs)
	{
		structText.AppendLines(DotNetCommon::MakeXmlDocument(structInfo->document));
		structText.Append("[StructLayout(LayoutKind.Sequential)]").NewLine();
		structText.Append("public struct {0}", structInfo->name).NewLine();
		structText.Append("{").NewLine();
		structText.IncreaseIndent();

		// fields
		for (auto& fieldInfo : structInfo->declaredFields)
		{
			structText.AppendLines(DotNetCommon::MakeXmlDocument(fieldInfo->document));
			structText.Append("public {0} {1};", DotNetCommon::MakeTypeName(fieldInfo->type), fieldInfo->name).NewLine(2);
		}

		// methods
		for (auto& methodInfo : structInfo->declaredMethods)
		{
			// プロパティの場合はスキップ
			if (methodInfo->ownerProperty != nullptr) continue;

			// params
			OutputBuffer params;
			for (auto& paramInfo : methodInfo->parameters)
			{
				params.AppendCommad("{0} {1}", DotNetCommon::MakeParamTypeName(paramInfo), paramInfo->name);
			}

			// xml document
			structText.AppendLines(DotNetCommon::MakeXmlDocument(methodInfo->document));

			// method header
			if (methodInfo->isConstructor)
				structText.Append("public {0}({1})", methodInfo->name, params.ToString()).NewLine();
			else
				structText.Append("public {0} {1}({2})", DotNetCommon::MakeTypeName(methodInfo->returnType), methodInfo->name, params.ToString()).NewLine();

			// method body
			structText.AppendLines(MakeMethodBody(methodInfo, false)).NewLine();
		}

		// property
		for (auto& propInfo : structInfo->declaredProperties)
		{
			OutputBuffer prop;

			// xml document
			structText.AppendLines(DotNetCommon::MakeXmlDocument(propInfo->document));

			// property header
			structText.Append("public {0} {1}", DotNetCommon::MakeTypeName(propInfo->type), propInfo->name).NewLine();
			structText.Append("{").NewLine();
			structText.IncreaseIndent();

			if (propInfo->getter != nullptr)
			{
				structText.Append("get").NewLine();
				structText.AppendLines(MakeMethodBody(propInfo->getter, true));
			}
			if (propInfo->setter != nullptr)
			{
				structText.Append("set").NewLine();
				structText.AppendLines(MakeMethodBody(propInfo->setter, true));
			}

			structText.DecreaseIndent();
			structText.Append("}").NewLine(2);
		}

		structText.DecreaseIndent();
		structText.Append("}").NewLine();
	}

	// output
	{
		String src = FileSystem::ReadAllText(PathName(g_templateDir, "DotNet/CSStructs.template.cs"));
		src = src.Replace("%Structs%", structText.ToString());
		FileSystem::WriteAllText(LUMINO_ROOT_DIR"/CSStructs.generated.cs", src);
	}
}

String CSStructsGenerator::MakeMethodBody(MethodInfoPtr methodInfo, bool isProperty)
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
				initStmt.Append("{0} {1} = new {0}();", DotNetCommon::MakeTypeName(paramInfo->type), paramInfo->name);
			else
				initStmt.Append("{0} {1};", DotNetCommon::MakeTypeName(paramInfo->type), paramInfo->name);
		}

		// call args
		if (paramInfo->isThis)
			callArgs.AppendCommad("ref this");
		else if (paramInfo->isOut || paramInfo->isReturn)
			callArgs.AppendCommad("out " + paramInfo->name);
		else
			callArgs.AppendCommad((isProperty) ? "value" : paramInfo->name);

		// return stmt
		if (paramInfo->isReturn)
			returnStmt = String::Format("return {0};", paramInfo->name);
	}

	// method body
	methodBody.AppendLines(MethodBodyTemplate
		.Replace("%InitStmt%", initStmt.ToString())
		.Replace("%APIFuncName%", methodInfo->GetCAPIFuncName())
		.Replace("%CallArgList%", callArgs.ToString())
		.Replace("%ReturnStmt%", returnStmt));

	return methodBody.ToString();
}
