
#include "../SymbolDatabase.h"
#include "WrapperIFGenerator.h"

const StringA funcTemplate =
	"LNResult LN_API LN%ClassName%_%FuncName%(%ParamList%)\n"
	"{\n"
	"    LN_FUNC_TRY_BEGIN;\n"
	"    %FuncBody%\n"
	"    LN_FUNC_TRY_END_RETURN;\n"
	"}\n";

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

		
		for (auto& methodInfo : structInfo->declaredMethods)
		{
			// make params
			OutputBuffer params(1);
			{
				if (methodInfo->isConst)
					params.AppendCommad("const LN{0}* _this", structInfo->name);
				else
					params.AppendCommad("LN{0}* _this", structInfo->name);

				for (auto& paramInfo : methodInfo->parameters)
				{
					params.AppendCommad("{0} {1}", paramInfo->type->name, paramInfo->name);
				}

				// return value
				if (!methodInfo->returnType->isVoid)
				{
					params.AppendCommad("{0} outReturn", MakeParamTypeName(methodInfo->returnType, true));
				}
			}

			// make func body
			OutputBuffer body;
			{
				OutputBuffer args;
				for (auto& paramInfo : methodInfo->parameters)
				{
					args.AppendCommad(paramInfo->name);
				}

				if (!methodInfo->returnType->isVoid)
					body.Append("*outReturn = ");

				String castTo = structInfo->name;
				if (methodInfo->isConst) castTo = "const " + castTo;

				body.Append("reinterpret_cast<{0}*>(_this)->{1}({2});", castTo, methodInfo->name, args.ToString());
			}

			buffer.AppendLines(funcTemplate
				.Replace("%ClassName%", structInfo->name)
				.Replace("%FuncName%", methodInfo->name)
				.Replace("%ParamList%", params.ToString())
				.Replace("%FuncBody%", body.ToString()));
		}
	}

	for (auto& classInfo : m_database->classes)
	{
		buffer.AppendLine("// {0}", classInfo->name);
	}

	FileSystem::WriteAllText("test.cpp", buffer.ToString());
}

StringA WrapperIFGenerator::MakeParamTypeName(TypeInfoPtr typeInfo, bool isOut)
{
	StringA name;

	if (isOut)
	{
		name = typeInfo->name;
		name += "*";
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
	return name;
}
