
#include "../Global.h"
#include "../SymbolDatabase.h"
#include "DotNetCommon.h"
#include "DotNetPInvokeLibGenerator.h"

static const String FuncDeclTempalte =
	_T("[DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]\n")
	_T("public extern static ResultCode %FuncName%(%Params%);\n");

void DotNetPInvokeLibGenerator::generate()
{
	// delegates
	OutputBuffer delegatesText(1);
	{
		for (auto& methodInfo : g_database.GetAllMethods())
		{
			if (methodInfo->isVirtual)
			{
				delegatesText.append("[UnmanagedFunctionPointer(CallingConvention.Cdecl)]").NewLine();
				delegatesText.append("public delegate ResultCode {0}({1});", methodInfo->GetCApiSetOverrideCallbackTypeName(), MakePInvokeMethodDeclParamList(methodInfo)).NewLine();
			}
		}

		for (auto& delegateInfo : g_database.delegates)
		{
			delegatesText.append("[UnmanagedFunctionPointer(CallingConvention.Cdecl)]").NewLine();
			delegatesText.append("public delegate void LN{0}({1});", delegateInfo->name, MakePInvokeMethodDeclParamList(delegateInfo->declaredMethods[0])).NewLine();
		}
	}

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
				enumsText.AppendLine("{0} = {1},", constantInfo->name, tr::Variant::cast<int>(constantInfo->value)).NewLine();
			}
			enumsText.DecreaseIndent();
			enumsText.AppendLine("}").NewLine();
		}
	}

	// funcs
	OutputBuffer funcsText(2);
	for (auto& methodInfo : g_database.GetAllMethods())
	{
		funcsText.AppendLines(MakePInvokeMethodDecl(methodInfo, false)).NewLine();
		if (methodInfo->isVirtual)
		{
			funcsText.AppendLines(MakePInvokeMethodDecl(methodInfo, true)).NewLine();
			funcsText.AppendLines(MakePInvokeMethodDeclSetOverrideCallback(methodInfo)).NewLine();
		}
	}

	// output
	{
		String src = FileSystem::readAllText(PathName(g_templateDir, "DotNet/DotNetPInvoke.template.cs"));
		src = src.replace("%DelegateList%", delegatesText.toString());
		src = src.replace("%EnumList%", enumsText.toString());
		src = src.replace("%APIList%", funcsText.toString());
		FileSystem::writeAllText(PathName(g_csOutputDir, "DotNetPInvoke.generated.cs"), src);
	}
}

String DotNetPInvokeLibGenerator::MakePInvokeMethodDecl(MethodInfoPtr methodInfo, bool virtualBase)
{
	String suffix = (virtualBase) ? "_CallVirtualBase" : "";

	// DLLImport・型名・関数名
	String declText = FuncDeclTempalte;
	declText = declText.replace("%FuncName%", methodInfo->GetCAPIFuncName() + suffix);

	// params
	return declText.replace("%Params%", MakePInvokeMethodDeclParamList(methodInfo));
}

String DotNetPInvokeLibGenerator::MakePInvokeMethodDeclSetOverrideCallback(MethodInfoPtr methodInfo)
{
	return FuncDeclTempalte
		.replace("%FuncName%", methodInfo->GetCApiSetOverrideCallbackFuncName())
		.replace("%Params%", methodInfo->GetCApiSetOverrideCallbackTypeName() + " func");
}

String DotNetPInvokeLibGenerator::MakePInvokeMethodDeclParamList(MethodInfoPtr methodInfo)
{
	OutputBuffer params;
	for (auto& paramInfo : methodInfo->capiParameters)
	{
		params.AppendCommad("{0} {1}", MakeParamTypeName(paramInfo), paramInfo->name);
	}
	return params.toString();
}

String DotNetPInvokeLibGenerator::MakeParamTypeName(ParameterInfoPtr paramInfo)
{
	String m;
	if (paramInfo->isOut || paramInfo->isReturn) m += "out ";
	else if (paramInfo->type->isStruct) m += "ref ";

	auto itr = DotNetCommon::primitiveTypesMap.find(paramInfo->type);
	if (itr != DotNetCommon::primitiveTypesMap.end()) return m + itr->second;

	if (paramInfo->type->isStruct) return m + paramInfo->type->name;
	if (paramInfo->type->isEnum) return m + paramInfo->type->name;
	if (paramInfo->type->isDelegate) return m + "LN" + paramInfo->type->name;

	return m + "IntPtr";//typeInfo->name;
}
