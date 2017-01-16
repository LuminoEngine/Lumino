
#include "../SymbolDatabase.h"
#include "WrapperIFGenerator.h"

const StringA funcTemplate =
	"LNResult LN_API LN%ClassName%_%FuncName%(%ParamList%)\n"
	"{\n"
	"    LWIG_FUNC_TRY_BEGIN;\n"
	"    %FuncBody%\n"
	"    LWIG_FUNC_TRY_END_RETURN;\n"
	"}\n";

const StringA wrapperIFClassTemplate =
	"class LN%ClassName% : public %ClassName%\n"
	"{\n"
	"public:\n"
	"};\n";

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
	OutputBuffer wrapper;
	for (auto& classInfo : m_database->classes)
	{
		buffer.AppendLines(MakeMethods(classInfo));

		wrapper.AppendLines(wrapperIFClassTemplate.Replace("%ClassName%", classInfo->name));
	}

	FileSystem::WriteAllText(LUMINO_ROOT_DIR"/test.cpp", buffer.ToString());
	FileSystem::WriteAllText(LUMINO_ROOT_DIR"/testIF.h", wrapper.ToString());
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
			// 第1引数
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
					else if (!methodInfo->isConstructor)
					{
						params.AppendCommad("LNHandle {0}", MakeInstanceParamName(typeInfo));
					}
				}
			}

			// params
			for (auto& paramInfo : methodInfo->parameters)
			{
				params.AppendCommad("{0} {1}", MakeParamTypeName(paramInfo->type, false), paramInfo->name);
			}

			// return value
			if (!methodInfo->returnType->isVoid)
			{
				params.AppendCommad("{0} outReturn", MakeParamTypeName(methodInfo->returnType, true));
			}
			
			// constructor
			if (methodInfo->isConstructor)
			{
				params.AppendCommad("LNHandle* out{0}", MakeInstanceParamName(typeInfo));
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


			if (typeInfo->isStruct)
			{
				String castTo = typeInfo->name;
				if (methodInfo->isConst) castTo = "const " + castTo;

				body.Append("reinterpret_cast<{0}*>({1})->{2}({3});", castTo, MakeInstanceParamName(typeInfo), methodInfo->name, args.ToString());
			}
			else
			{
				// static 関数
				if (methodInfo->isStatic)
				{
					body.Append("{0}::{1}({2});", typeInfo->name, methodInfo->name, args.ToString());
				}
				// コンストラクタ 関数
				else if (methodInfo->isConstructor)
				{
					OutputBuffer macroArgs;
					macroArgs.AppendCommad("out" + MakeInstanceParamName(typeInfo));	// 格納する変数名
					macroArgs.AppendCommad("LN" + typeInfo->name);						// クラス名
					macroArgs.AppendCommad(methodInfo->name);							// 初期化関数名
					macroArgs.AppendCommad(args.ToString());							// 引数
					body.Append("LWIG_CREATE_OBJECT({0});", macroArgs.ToString());
				}
				// 普通のインスタンス 関数
				else
				{
					body.Append("LWIG_TO_OBJECT({0})->{1}({2});", MakeInstanceParamName(typeInfo), methodInfo->name, args.ToString());
				}
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
		if (typeInfo == m_database->stringType)
			name = "const LNChar*";
		else
			name = typeInfo->name;
	}
	return name;
}
