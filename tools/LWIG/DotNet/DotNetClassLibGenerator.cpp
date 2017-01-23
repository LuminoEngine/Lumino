
#include "../Global.h"
#include "../SymbolDatabase.h"
#include "DotNetCommon.h"
#include "DotNetClassLibGenerator.h"

static const String MethodBodyTemplate =
	_T("{\n")
	_T("    %InitStmt%\n")
	_T("    var result = API.%APIFuncName%(%CallArgList%);\n")
	_T("    if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);\n")
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
			structText.Append("public {0} {1};", DotNetCommon::MakeTypeName(fieldInfo->type), fieldInfo->name.ToTitleCase()).NewLine(2);
		}

		// 各要素指定のコンストラクタを作る
		{
			// 仮引数リスト → (float x, float y) 等
			OutputBuffer params;
			for (auto& fieldInfo : structInfo->declaredFields)
				params.AppendCommad("{0} {1}", DotNetCommon::MakeTypeName(fieldInfo->type), fieldInfo->name);

			// フィールド格納 代入式 → X = x; Y = y; 等
			OutputBuffer body;
			for (auto& fieldInfo : structInfo->declaredFields)
				body.Append("{0} = {1};", fieldInfo->name.ToTitleCase(), fieldInfo->name).NewLine();

			// メソッド定義
			structText.Append("public {0}({1})", structInfo->name, params.ToString()).NewLine();
			structText.Append("{").NewLine();
			structText.IncreaseIndent();
			structText.AppendLines(body.ToString());
			structText.DecreaseIndent();
			structText.Append("}").NewLine(2);
		}

		// methods
		for (auto& methodInfo : structInfo->declaredMethods)
		{
			if (methodInfo->ownerProperty != nullptr) continue;								// プロパティの場合はスキップ
			if (methodInfo->isConstructor && methodInfo->parameters.IsEmpty()) continue;	// C# は構造体にデフォルトコンストラクタは定義できない
			if (CheckAllMemberSetConstructor(structInfo, methodInfo)) continue;				// 単に全メンバに set するだけのコンストラクタは別途定義される

			// params
			OutputBuffer params;
			for (auto& paramInfo : methodInfo->parameters)
			{
				params.AppendCommad("{0} {1}", DotNetCommon::MakeParamTypeName(paramInfo), paramInfo->name);
			}

			// xml document
			structText.AppendLines(DotNetCommon::MakeXmlDocument(methodInfo->document));

			// method header
			String modifier = MethodInfo::GetAccessLevelName(methodInfo->accessLevel);
			if (methodInfo->isStatic) modifier += " static";
			if (methodInfo->isVirtual) modifier += " virtual";
			if (methodInfo->isConstructor)
				structText.Append("{0} {1}({2})", modifier, methodInfo->name, params.ToString()).NewLine();
			else
				structText.Append("{0} {1} {2}({3})", modifier, DotNetCommon::MakeTypeName(methodInfo->returnType), methodInfo->name, params.ToString()).NewLine();

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
				structText.AppendLines(MakeMethodBody(propInfo->getter, false));
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
		FileSystem::WriteAllText(PathName(g_csOutputDir, "CSStructs.generated.cs"), src);
	}
}

String CSStructsGenerator::MakeMethodBody(MethodInfoPtr methodInfo, bool isSetProperty)
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
		String modifier = "";
		if (paramInfo->isThis || paramInfo->type->isStruct) modifier = "ref ";
		if (paramInfo->isOut || paramInfo->isReturn) modifier = "out ";
		String name = paramInfo->name;
		if (isSetProperty) name = "value";
		if (paramInfo->isThis) name = "this";
		callArgs.AppendCommad(modifier + name);

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

bool CSStructsGenerator::CheckAllMemberSetConstructor(TypeInfoPtr structInfo, MethodInfoPtr methodInfo)
{
	if (!methodInfo->isConstructor) return false;

	int count = methodInfo->parameters.GetCount();
	if (count != structInfo->declaredFields.GetCount()) return false;

	for (int i = 0; i < count; i++)
	{
		if (methodInfo->parameters[i]->type != structInfo->declaredFields[i]->type)
			return false;
	}

	return true;
}
