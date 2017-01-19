
#include "../Global.h"
#include "../SymbolDatabase.h"
#include "DotNetCommon.h"
#include "DotNetPInvokeLibGenerator.h"

static const String FuncDeclTempalte =
	_T("[DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]\n")
	_T("public extern static ResultCode %FuncName%(%Params%);\n");

void DotNetPInvokeLibGenerator::Generate()
{
	// enums
	OutputBuffer enumsText(1);
	{
		for (auto& enumInfo : g_database.enums)
		{
			enumsText.AppendLines(DotNetCommon::MakeXmlDocument(enumInfo->document));
			enumsText.AppendLine("public enum {0}", enumInfo->name);
			enumsText.AppendLine("{");
			enumsText.IncreaseIndent();
			for (auto& constantInfo : enumInfo->declaredConstants)
			{
				enumsText.AppendLines(DotNetCommon::MakeXmlDocument(constantInfo->document));
				enumsText.AppendLine("{0} = {1},", constantInfo->name, tr::Variant::Cast<int>(constantInfo->value)).NewLine();
			}
			enumsText.DecreaseIndent();
			enumsText.AppendLine("}").NewLine();
		}
	}

	// funcs
	OutputBuffer funcsText(2);
	for (auto& methodInfo : g_database.GetAllMethods())
	{
		// DLLImport・型名・関数名
		String declText = FuncDeclTempalte;
		declText = declText.Replace("%FuncName%", methodInfo->GetCAPIFuncName());

		// params
		OutputBuffer params;
		for (auto& paramInfo : methodInfo->capiParameters)
		{
			params.AppendCommad("{0} {1}", MakeParamTypeName(paramInfo), paramInfo->name);
		}
		declText = declText.Replace("%Params%", params.ToString());

		funcsText.AppendLines(declText).NewLine();
	}

	// output
	{
		String src = FileSystem::ReadAllText(PathName(g_templateDir, "DotNet/DotNetPInvoke.template.cs"));
		src = src.Replace("%APIList%", funcsText.ToString());
		src = src.Replace("%EnumList%", enumsText.ToString());
		FileSystem::WriteAllText(PathName(g_csOutputDir, "DotNetPInvoke.generated.cs"), src);
	}
}

String DotNetPInvokeLibGenerator::MakeParamTypeName(ParameterInfoPtr paramInfo)
{
	String m;
	if (paramInfo->isOut || paramInfo->isReturn) m += "out ";
	else if (paramInfo->type->isStruct) m += "ref ";

	auto itr = DotNetCommon::primitiveTypesMap.find(paramInfo->type);
	if (itr != DotNetCommon::primitiveTypesMap.end()) return m + itr->second;

	if (paramInfo->type->isStruct) return m + paramInfo->type->name;

	return m + "IntPtr";//typeInfo->name;
}
