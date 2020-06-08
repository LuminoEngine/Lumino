
#include "HSPGenerator.hpp"

//==============================================================================
// HSPGeneratorBase


//==============================================================================
// HSPHeaderGenerator

void HSPHeaderGenerator::generate()
{
	OutputBuffer code;
	code.AppendLines(makeEnums());

	// save
	{
		auto outputDir = ln::Path(makeOutputFilePath(u"HSP", u""));
		ln::FileSystem::createDirectory(outputDir);

		ln::String fileName = ln::String::format("{0}.as", config()->moduleName);

		ln::String src = code.toString();

		ln::FileSystem::writeAllText(ln::Path(outputDir, fileName), src);
	}
}

ln::String HSPHeaderGenerator::makeEnums() const
{
	OutputBuffer code;
	for (const auto& enumSymbol : db()->enums()) {
		for (const auto& member : enumSymbol->constants()) {
			code.AppendLine("#const global {0} {1}", makeFlatEnumMemberName(enumSymbol, member), member->value()->get<int>());
		}
	}
	return code.toString();
}

//==============================================================================
// HSPCommandsGenerator

void HSPCommandsGenerator::generate()
{
	// Setup command map
	for (const auto& classSymbol : db()->classes()) {
		for (const auto& methodSymbol : classSymbol->publicMethods()) {
			auto itr = m_commandIdMap.find(methodSymbol);
			if (itr != m_commandIdMap.end()) {
				LN_ERROR();	// 重複？何かおかしい
				return;
			}
			else {
				m_commandIdMap.insert({ methodSymbol.get(), m_commandIdMap.size() + 0x0064 });
			}
		}
	}


	

	// save
	{
        auto outputDir = ln::Path(makeOutputFilePath(u"HSP", u""));
        ln::FileSystem::createDirectory(outputDir);

		ln::String fileName = ln::String::format("{0}.HSPCommands.generated.cpp", config()->moduleName);

		//ln::String src = ln::FileSystem::readAllText(makeTemplateFilePath(u"HSPCommands.template.cpp"))
		//	.replace("%%Commands_cmdfunc%%", moduleInitializer.toString());
		ln::String src = make_cmdfunc();

		ln::FileSystem::writeAllText(ln::Path(outputDir, fileName), src);
	}
}

ln::String HSPCommandsGenerator::make_cmdfunc() const
{
	OutputBuffer code;
	code.AppendLine(u"bool Commands_cmdfunc(int cmd, int* retVal)");
	code.AppendLine(u"{");
	code.IncreaseIndent();
	code.AppendLine(u"*retVal = RUNMODE_RUN;");
	code.AppendLine(u"switch (cmd) {");
	code.IncreaseIndent();
	
	for (const auto& classSymbol : db()->classes()) {
		for (const auto& methodSymbol : classSymbol->publicMethods()) {
			auto itr = m_commandIdMap.find(methodSymbol.get());
			if (itr == m_commandIdMap.end()) {
				LN_ERROR();	// ID が見つからないのはおかしい
				return ln::String::Empty;
			}

			code.AppendLine(u"// " + makeFlatFullFuncName(methodSymbol, FlatCharset::Ascii));
			code.AppendLine(u"case 0x{0:X} : {{", itr->second);
			code.IncreaseIndent();

			code.AppendLines(makeCallCommandBlock(methodSymbol));
			
			code.AppendLine(u"break;");
			code.DecreaseIndent();
			code.AppendLine(u"}");
		}
	}

	code.DecreaseIndent();
	code.AppendLine(u"}");
	code.DecreaseIndent();
	code.AppendLine(u"}");
	return code.toString();
}

ln::String HSPCommandsGenerator::makeCallCommandBlock(const MethodSymbol* methodSymbol) const
{
	OutputBuffer prologue;
	OutputBuffer args;
	OutputBuffer epilogue;

	for (const auto& param : methodSymbol->flatParameters()) {
		auto localVarName = u"local_" + param->name();
		prologue.AppendLine(u"// Fetch " + param->name());

		if (param->isOut() || param->isReturn())
		{
			prologue.AppendLine(u"const PVal* pval_{0};", param->name());
			prologue.AppendLine(u"const APTR aptr_{0} = code_getva(&pval_{0});", param->name());
			prologue.AppendLine(u"{0} {1};", makeFlatTypeName2(param->type()), localVarName);


			args.AppendCommad(u"&" + localVarName);
			epilogue.AppendLine(makeSetVAExpr(param));
		}
		else
		{
			prologue.AppendLine(makeGetVAExpr(param));
			args.AppendCommad(localVarName);
		}

	}

	OutputBuffer code;
	code.AppendLines(prologue.toString());
	ln::String callExpr;
	//if (method.FuncDecl.ReturnType != CLPrimitiveType.Void)
	//	callExp += "stat = ";
	callExpr = "stat = " + makeFlatFullFuncName(methodSymbol, FlatCharset::Ascii) + "(" + args.toString() + ");";

	code.AppendLine(callExpr);
	code.AppendLines(epilogue.toString().trim());
	return code.toString();
}

ln::String HSPCommandsGenerator::makeGetVAExpr(const MethodParameterSymbol* paramSymbol) const
{
	const auto localName = ln::String::format(u"local_" + paramSymbol->name());

	//static ln::Ref<TypeSymbol>	voidType;
	//static ln::Ref<TypeSymbol>	nullptrType;
	//static ln::Ref<TypeSymbol>	boolType;
	//static ln::Ref<TypeSymbol>	intType;
	//static ln::Ref<TypeSymbol>	int16Type;
	//static ln::Ref<TypeSymbol>	uint32Type;
	//static ln::Ref<TypeSymbol>	floatType;
	//static ln::Ref<TypeSymbol>	doubleType;
	//static ln::Ref<TypeSymbol>	stringType;
	//static ln::Ref<TypeSymbol>	stringRefType;
	if (paramSymbol->type() == PredefinedTypes::boolType ||
		paramSymbol->type() == PredefinedTypes::intType ||
		paramSymbol->type() == PredefinedTypes::int16Type ||
		paramSymbol->type() == PredefinedTypes::uint32Type ||
		paramSymbol->type()->isClass()) {
		return ln::String::format(u"const int {0} = fetchVAInt();", localName);
	}

	return u"??";
}

ln::String HSPCommandsGenerator::makeSetVAExpr(const MethodParameterSymbol* paramSymbol) const
{
	const auto pvalName = ln::String::format(u"pval_" + paramSymbol->name());
	const auto aptrName = ln::String::format(u"aptr_" + paramSymbol->name());
	const auto localName = ln::String::format(u"local_" + paramSymbol->name());

	if (paramSymbol->type() == PredefinedTypes::boolType ||
		paramSymbol->type()->isClass()) {
		return ln::String::format(u"code_setva({0}, {1}, HSPVAR_FLAG_INT, static_cast<int32_t*>(&{2}));", pvalName, aptrName, localName);
	}

	return u"??";
}
