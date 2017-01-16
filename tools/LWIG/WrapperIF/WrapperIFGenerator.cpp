
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

		buffer.AppendLines(MakeMethods(structInfo));
	}

	for (auto& classInfo : m_database->classes)
	{
		buffer.AppendLines(MakeMethods(classInfo));
	}

	FileSystem::WriteAllText("test.cpp", buffer.ToString());
}

StringA WrapperIFGenerator::MakeInstanceParamName(TypeInfoPtr info)
{
	return info->name;
}

StringA WrapperIFGenerator::MakeMethods(TypeInfoPtr typeInfo)
{
	OutputBuffer buffer;

	for (auto& methodInfo : typeInfo->declaredMethods)
	{
		// make params
		OutputBuffer params(1);
		{
			// ‘æ1ˆø”
			{
				if (!methodInfo->isStatic)
				{
					if (typeInfo->isStruct)
					{
						if (methodInfo->isConst)
							params.AppendCommad("const LN{0}* {1}", typeInfo->name, MakeInstanceParamName(typeInfo));
						else
							params.AppendCommad("LN{0}* {1}", typeInfo->name, typeInfo->name, MakeInstanceParamName(typeInfo));
					}
					else
					{
						params.AppendCommad("LNHandle {0}", MakeInstanceParamName(typeInfo));
					}
				}
			}

			// params
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


			if (methodInfo->isStatic)
			{
				body.Append("{0}::{1}({2});", typeInfo->name, methodInfo->name, args.ToString());
			}
			else
			{
				String castTo = typeInfo->name;
				if (methodInfo->isConst) castTo = "const " + castTo;

				body.Append("reinterpret_cast<{0}*>({1})->{2}({3});", castTo, MakeInstanceParamName(typeInfo), methodInfo->name, args.ToString());
			}
		}

		buffer.AppendLines(funcTemplate
			.Replace("%ClassName%", typeInfo->name)
			.Replace("%FuncName%", methodInfo->name)
			.Replace("%ParamList%", params.ToString())
			.Replace("%FuncBody%", body.ToString()));
	}

	return buffer.ToString();
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
