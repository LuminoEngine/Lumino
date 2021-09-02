
#include "FlatCGenerator.hpp"





//==============================================================================
// FlatCCommon

ln::String FlatCCommon::makeInstanceParamName(TypeSymbol* type)
{
	return type->shortName().toLower();
}

ln::String FlatCCommon::makeCreateDelegateObjectFuncHeader(const TypeSymbol* delegateSymbol) const
{
    auto funcPtrType = makeDelegateCallbackFuncPtrName(delegateSymbol, FlatCharset::Unicode);
    auto flatClassName = makeFlatClassName(delegateSymbol);
    return ln::String::format(u"LN_FLAT_API LNResult {0}_Create({1} callback, LNHandle* outDelegate)", flatClassName, funcPtrType);
}

//==============================================================================
// FlatCHeaderGenerator

void FlatCHeaderGenerator::generate()
{
	// delegates
	OutputBuffer delegatesText;
	//for (auto& delegateSymbol : db()->delegates()) {
	//	delegatesText.AppendLine(makeDelegateFuncPtrDecl(delegateSymbol));
	//}
 //   delegatesText.NewLine();

	// structs
	OutputBuffer structsText;
	OutputBuffer structMemberFuncDeclsText;
	OutputBuffer structMemberFuncImplsText;
	for (auto& structInfo : db()->structs())
	{
		structsText.AppendLine(makeDocumentComment(structInfo->document()));
		structsText.AppendLine(u"struct {0}", makeFlatClassName(structInfo));
		structsText.AppendLine(u"{");
		structsText.IncreaseIndent();
		for (auto& fieldInfo : structInfo->fields())
		{
			structsText.AppendLine(u"{0} {1};", makeFlatTypeName2(fieldInfo->type()), fieldInfo->name());
		}
		structsText.DecreaseIndent();
		structsText.AppendLine(u"};");
        structsText.NewLine();

        structMemberFuncDeclsText.AppendLine(u"//==============================================================================");
        structMemberFuncDeclsText.AppendLine(u"// {0}", structInfo->fullName());
        structMemberFuncDeclsText.NewLine();

		// function decls
		for (auto& methodInfo : structInfo->publicMethods())
		{
            if (methodInfo->isFieldAccessor()) {
            }
            else {
                // comment
                structMemberFuncDeclsText.AppendLine(makeMethodDocumentComment(methodInfo));

                // decl
                structMemberFuncDeclsText.AppendLine(makeFuncHeader(methodInfo, FlatCharset::Unicode) + u";").NewLine(2);
            }
		}
	}

	// classes
	OutputBuffer classMemberFuncDeclsText;
	for (auto& classSymbol : db()->classes())
	{
		classMemberFuncDeclsText.AppendLine(u"//==============================================================================");
		classMemberFuncDeclsText.AppendLine(u"// {0}", classSymbol->fullName());
		classMemberFuncDeclsText.NewLine();

		if (classSymbol->typeClass() == TypeClass::DelegateObject) {
			delegatesText.AppendLine(makeDelegateFuncPtrDecl(classSymbol));
			classMemberFuncDeclsText.AppendLine(makeCreateDelegateObjectFuncHeader(classSymbol) + u";");
		}
		else {
			// function decls
			for (auto& method : classSymbol->publicMethods())
			{
				// comment
				classMemberFuncDeclsText.AppendLine(makeMethodDocumentComment(method));

				// decl
				classMemberFuncDeclsText.AppendLine(makeFuncHeader(method, FlatCharset::Unicode) + u";");

				if (method->hasStringDecl()) {
					classMemberFuncDeclsText.AppendLine(makeFuncHeader(method, FlatCharset::Ascii) + u";");
				}

				classMemberFuncDeclsText.NewLine();
			}

			// virtual callback
			if (!classSymbol->leafVirtualMethods().isEmpty()) {
				for (auto& method : classSymbol->leafVirtualMethods()) {
					// make params
					OutputBuffer params;
					for (auto& param : method->flatParameters()) {
						params.AppendCommad("{0} {1}", makeFlatCParamQualTypeName(method, param, FlatCharset::Unicode), param->name());
					}
					classMemberFuncDeclsText.AppendLine(u"typedef LNResult(*{0})({1});", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode), params.toString());
					classMemberFuncDeclsText.AppendLine(makeFlatAPIDecl_SetOverrideCallback(classSymbol, method, FlatCharset::Unicode) + u";");
					classMemberFuncDeclsText.AppendLine(makeFlatAPIDecl_CallOverrideBase(classSymbol, method, FlatCharset::Unicode) + u";");
					//classMemberFuncDeclsText.AppendLine(makeFlatAPIDecl_SetPrototype(classSymbol, method, FlatCharset::Unicode) + u";");
				}
				classMemberFuncDeclsText.NewLine();
			}

			// virtualPrototypeSetters
			for (auto& method : classSymbol->virtualPrototypeSetters()) {
				// comment
				classMemberFuncDeclsText.AppendLine(makeMethodDocumentComment(method));
				// decl
				classMemberFuncDeclsText.AppendLine(makeFuncHeader(method, FlatCharset::Unicode) + u";");
				classMemberFuncDeclsText.NewLine();
			}

			// type info
			if (!classSymbol->isStatic()) {
				classMemberFuncDeclsText.AppendLine(makeGetTypeInfoIdFuncHeader(classSymbol) + u";");
			}

			//for (auto& method : classSymbol->publicMethods()) {
			//	classMemberFuncDeclsText.AppendLine(u"extern \"C\" " + makeFuncHeader(method, FlatCharset::Unicode) + u";");
			//}

		}

		if (!classSymbol->isStatic()) {
			classMemberFuncDeclsText.AppendLine(makeFlatAPIDecl_SetManagedTypeInfoId(classSymbol) + u"; // deprecated");


			classMemberFuncDeclsText.AppendLines(makeSubClassRegistrationInfo(classSymbol));
		}

		classMemberFuncDeclsText.NewLine();
	}

	// save C API Header
	{
		ln::String src = ln::FileSystem::readAllText(makeTemplateFilePath(u"LuminoC.template.h"));
		src = src.replace(u"%%Structs%%", structsText.toString());
		src = src.replace(u"%%Enums%%", makeEnumDecls());
		src = src.replace(u"%%Delegates%%", delegatesText.toString());
		src = src.replace(u"%%StructMemberFuncDecls%%", structMemberFuncDeclsText.toString());
		src = src.replace(u"%%ClassMemberFuncDecls%%", classMemberFuncDeclsText.toString());

        auto filePath = makeFlatCHeaderOutputPath();
		ln::FileSystem::writeAllText(filePath, src, ln::TextEncoding::getEncoding(ln::EncodingType::UTF8));
	}
}

ln::String FlatCHeaderGenerator::makeDocumentComment(DocumentInfo* doc) const
{
	OutputBuffer code;
    code.AppendLine(u"/**");
    code.IncreaseIndent();

	// @brief
    code.AppendLine(u"@brief {0}", doc->summary());

	// @param
	for (auto& param : doc->params())
	{
        code.AppendLine(u"@param[{0}] {1} : {2}", param->io(), param->name(), param->description());
	}

	// @return
	if (!doc->returns().isEmpty())
        code.AppendLine(u"@return {0}", doc->returns());

	// @details
	if (!doc->details().isEmpty())
        code.AppendLine(u"@details {0}", doc->details());

    code.DecreaseIndent();
    code.AppendLine(u"*/");

	return code.toString().trim();
}

ln::String FlatCHeaderGenerator::makeMethodDocumentComment(const MethodSymbol* method) const
{

	auto doc = method->document();

	OutputBuffer code;
    code.AppendLine(u"/**");
    code.IncreaseIndent();

	// @brief
    code.AppendLine(u"@brief {0}", doc->summary());

	// @param
	for (auto& param : doc->flatParams())
	{
		auto desc = param->description();
		auto paramInfo = method->findFlatParameter(param->name());
		if (paramInfo && paramInfo->qualType().strongReference) {
			desc += u" (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)";
		}

        code.AppendLine(u"@param[{0}] {1} : {2}", param->io(), param->name(), desc);
	}

	// @return
	if (!doc->returns().isEmpty())
        code.AppendLine(u"@return {0}", doc->returns());

	// @details
	if (!doc->details().isEmpty())
        code.AppendLine(u"@details {0}", doc->details());

    code.DecreaseIndent();
    code.AppendLine(u"*/");

	return code.toString().trim();
}

ln::String FlatCHeaderGenerator::makeDelegateFuncPtrDecl(const TypeSymbol* delegateSymbol) const
{
	// return
	auto returnType = (delegateSymbol->isDelegateObject()) ? u"LNResult" : u"void";

    // make params
    OutputBuffer params;
	//if (delegateSymbol->isDelegateObject()) {
	//	params.AppendCommad(u"LNHandle");
	//}
    for (auto& param : delegateSymbol->delegateProtoType()->flatParameters()) {
        params.AppendCommad(u"{0} {1}", makeFlatCParamQualTypeName(nullptr, param, FlatCharset::Unicode), param->name());
    }
    return ln::String::format(u"typedef {0}(*{1})({2});", returnType, makeDelegateCallbackFuncPtrName(delegateSymbol, FlatCharset::Unicode), params.toString());
}

ln::String FlatCHeaderGenerator::makeEnumDecls() const
{
	OutputBuffer code;

	for (auto& symbol : db()->enums()) {
		auto typeName = makeFlatClassName(symbol);

		code.AppendLine(makeDocumentComment(symbol->document()));
		code.AppendLine(u"typedef enum tag" + typeName);
		code.AppendLine(u"{");
		code.IncreaseIndent();

		for (auto& member : symbol->constants()) {
			code.AppendLine(makeDocumentComment(member->document()));
			code.AppendLine(u"{0} = {1},", makeFlatEnumMemberName(symbol, member), member->value()->get<int>());
			code.NewLine();
		}

		code.DecreaseIndent();
		code.AppendLine(u"} " + typeName + u";");
		code.NewLine();
	}

	return code.toString().trim();
}

ln::String FlatCHeaderGenerator::makeSubClassRegistrationInfo(const TypeSymbol* classSymbol) const
{
	OutputBuffer code;

	const auto& structName = makeFlatAPIName_SubclassRegistrationInfo(classSymbol);

	code.AppendLine(u"typedef struct tag" + structName, makeFlatTypeName2(classSymbol));
	code.AppendLine(u"{");
	code.IncreaseIndent();
	{
		code.AppendLine(u"int64_t subclassId;	// ManagedTypeInfoId");
		code.AppendLine(u"LNSubinstanceAllocFunc subinstanceAllocFunc;");
		code.AppendLine(u"LNSubinstanceFreeFunc subinstanceFreeFunc;");

		for (auto& method : classSymbol->leafVirtualMethods()) {
			//// make params
			//OutputBuffer params;
			//for (auto& param : method->flatParameters()) {
			//	params.AppendCommad("{0} {1}", makeFlatCParamQualTypeName(method, param, FlatCharset::Unicode), param->name());
			//}
			////classMemberFuncDeclsText.AppendLine(u"typedef LNResult(*{0})({1});", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode), params.toString());
			//code.AppendLine(makeFlatAPIDecl_SetOverrideCallback(classSymbol, method, FlatCharset::Unicode) + u";");
			//code.AppendLine(makeFlatAPIDecl_CallOverrideBase(classSymbol, method, FlatCharset::Unicode) + u";");

			code.AppendLine(u"{0} {1};",
				makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode),
				makeFlatAPIName_OverrideFunc(method, FlatCharset::Unicode));
		}

		code.NewLine();
	}
	code.DecreaseIndent();
	code.AppendLine(u"} " + structName + u";");

	code.NewLine();
	code.AppendLine(u"extern LN_FLAT_API void {0}(const {1}* info);", makeFlatAPIName_RegisterSubclassTypeInfo(classSymbol), structName);
	code.AppendLine(u"extern LN_FLAT_API LNSubinstanceId {0}(LNHandle handle);", makeFlatAPIName_GetSubinstanceId(classSymbol), structName);


	return code.toString().trim();
}

//==============================================================================
// FlatCSourceGenerator

void FlatCSourceGenerator::generate()
{
	// structs
	OutputBuffer structsText;
	OutputBuffer structMemberFuncImplsText;
	for (auto& structInfo : db()->structs()) {
        for (auto& methodInfo : structInfo->publicMethods()) {
            if (methodInfo->isFieldAccessor()) {
            }
            else {
                structMemberFuncImplsText.AppendLines(makeFuncBody(structInfo, methodInfo, FlatCharset::Unicode)).NewLine();
            }
        }
	}

	// classes
	OutputBuffer classMemberFuncImplsText;
	for (auto& classSymbol : db()->classes()) {
		if (classSymbol->isDelegateObject()) {
		}
		else {
			for (auto& methodInfo : classSymbol->publicMethods()) {
				if (methodInfo->isSpecialized()) {
					continue;
				}
				/*if (methodInfo->isEventConnector()) {
					classMemberFuncImplsText.AppendLines(makeEventConnectorFuncBody(classSymbol, methodInfo)).NewLine();
				}
				else*/ {
					classMemberFuncImplsText.AppendLines(makeFuncBody(classSymbol, methodInfo, FlatCharset::Unicode)).NewLine();
					if (methodInfo->hasStringDecl()) {
						classMemberFuncImplsText.AppendLines(makeFuncBody(classSymbol, methodInfo, FlatCharset::Ascii)).NewLine();
					}
				}
			}

			// virtual
			for (auto& method : classSymbol->leafVirtualMethods()) {
				OutputBuffer args;
				for (auto& param : method->parameters()) {
					args.AppendCommad(param->name());
				}

				OutputBuffer funcImpl;

				funcImpl.AppendLine(makeFlatAPIDecl_CallOverrideBase(classSymbol, method, FlatCharset::Unicode));
				funcImpl.AppendLine(u"{");
				funcImpl.IncreaseIndent();
				funcImpl.AppendLine(u"LNI_FUNC_TRY_BEGIN;");
				funcImpl.AppendLine(u"(LNI_HANDLE_TO_OBJECT({0}, {1})->{2}_CallBase({3}));", makeWrapSubclassName(classSymbol), method->flatParameters()[0]->name(), method->shortName(), makeNativeArgList(method));
				funcImpl.AppendLine(u"LNI_FUNC_TRY_END_RETURN;");
				funcImpl.DecreaseIndent();
				funcImpl.AppendLine(u"}");

				funcImpl.AppendLine(makeFlatAPIDecl_SetOverrideCallback(classSymbol, method, FlatCharset::Unicode));
				funcImpl.AppendLine(u"{");
				funcImpl.IncreaseIndent();
				funcImpl.AppendLine(u"{0}::s_{1} = callback;", makeWrapSubclassName(classSymbol), makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode));
				funcImpl.AppendLine(u"return LN_OK;");
				funcImpl.DecreaseIndent();
				funcImpl.AppendLine(u"}");

				//funcImpl.AppendLine(makeFlatAPIDecl_SetPrototype(classSymbol, method, FlatCharset::Unicode));
				//funcImpl.AppendLine(u"{");
				//funcImpl.IncreaseIndent();
				//{
				//	funcImpl.AppendLine(u"LNI_HANDLE_TO_OBJECT({0}, {1})->acquireOverridePrototypes()->{2} = func;", makeWrapSubclassName(classSymbol), method->flatParameters()[0]->name(), makeFlatAPIName_OverrideFunc(method, FlatCharset::Unicode));
				//	funcImpl.AppendLine(u"return LN_OK;");
				//}
				//funcImpl.DecreaseIndent();
				//funcImpl.AppendLine(u"}");

				classMemberFuncImplsText.AppendLine(funcImpl.toString());
			}

			// virtual prototypes
			for (auto& method : classSymbol->virtualPrototypeSetters()) {

				classMemberFuncImplsText.AppendLine(makeFuncHeader(method, FlatCharset::Unicode));
				classMemberFuncImplsText.AppendLine(u"{");
				classMemberFuncImplsText.IncreaseIndent();
				{
					classMemberFuncImplsText.AppendLine(
						u"LNI_HANDLE_TO_OBJECT({0}, {1})->acquireOverridePrototypes()->{2} = LNI_HANDLE_TO_OBJECT({3}, callback);", 
						makeWrapSubclassName(classSymbol), method->flatParameters()[0]->name(),
						makeFlatAPIName_OverrideFunc(method->virtualPototypeSouceMethod(), FlatCharset::Unicode),
						makeWrapSubclassName(method->virtualPototypeDelegate()));
					classMemberFuncImplsText.AppendLine(u"return LN_OK;");
				}
				classMemberFuncImplsText.DecreaseIndent();
				classMemberFuncImplsText.AppendLine(u"}");
			}

			// type info
			if (!classSymbol->isStatic()) {
				classMemberFuncImplsText.AppendLine(makeGetTypeInfoIdFuncHeader(classSymbol));
				classMemberFuncImplsText.AppendLine(u"{");
				classMemberFuncImplsText.IncreaseIndent();
				classMemberFuncImplsText.AppendLine(u"return ln::TypeInfo::getTypeInfo<{0}>()->id();", classSymbol->fullName());
				classMemberFuncImplsText.DecreaseIndent();
				classMemberFuncImplsText.AppendLine(u"}");
				classMemberFuncImplsText.NewLine();
			}
		}

		// TypeInfo id setter
		if (!classSymbol->isStatic()) {
			classMemberFuncImplsText.AppendLine(makeFlatAPIDecl_SetManagedTypeInfoId(classSymbol));
			classMemberFuncImplsText.AppendLine(u"{");
			classMemberFuncImplsText.IncreaseIndent();
			classMemberFuncImplsText.AppendLine(u"::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<{0}>(), id);", classSymbol->fullName());
			classMemberFuncImplsText.DecreaseIndent();
			classMemberFuncImplsText.AppendLine(u"}");
			classMemberFuncImplsText.NewLine();

			// _RegisterSubclassTypeInfo
			classMemberFuncImplsText.AppendLine(u"void {0}(const {1}* info)", makeFlatAPIName_RegisterSubclassTypeInfo(classSymbol), makeFlatAPIName_SubclassRegistrationInfo(classSymbol));
			classMemberFuncImplsText.AppendLine(u"{");
			classMemberFuncImplsText.IncreaseIndent();
			{
				classMemberFuncImplsText.AppendLine(u"if (info) {");
				classMemberFuncImplsText.IncreaseIndent();
				{
					classMemberFuncImplsText.AppendLine(u"::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<{0}>(), info->subclassId);", classSymbol->fullName());
					classMemberFuncImplsText.AppendLine(u"*{0}::subclassInfo() = *info;", makeWrapSubclassName(classSymbol));
				}
				classMemberFuncImplsText.DecreaseIndent();
				classMemberFuncImplsText.AppendLine(u"}");
			}
			classMemberFuncImplsText.DecreaseIndent();
			classMemberFuncImplsText.AppendLine(u"}");
			classMemberFuncImplsText.NewLine();

			// GetSubinstanceId
			classMemberFuncImplsText.AppendLine(u"LNSubinstanceId {0}(LNHandle handle)", makeFlatAPIName_GetSubinstanceId(classSymbol));
			classMemberFuncImplsText.AppendLine(u"{");
			classMemberFuncImplsText.IncreaseIndent();
			{
				classMemberFuncImplsText.AppendLine(u"if (handle) {");
				classMemberFuncImplsText.IncreaseIndent();
				{
					classMemberFuncImplsText.AppendLine(u"LNI_FUNC_TRY_BEGIN;");
					classMemberFuncImplsText.AppendLine(u"return (LNI_HANDLE_TO_OBJECT({0}, handle))->m_subinstance;", makeWrapSubclassName(classSymbol));
					classMemberFuncImplsText.AppendLine(u"LNI_FUNC_TRY_END_RETURN;");
				}
				classMemberFuncImplsText.DecreaseIndent();
				classMemberFuncImplsText.AppendLine(u"}");
				classMemberFuncImplsText.AppendLine(u"return 0;");
			}
			classMemberFuncImplsText.DecreaseIndent();
			classMemberFuncImplsText.AppendLine(u"}");
			classMemberFuncImplsText.NewLine();

		}
	}

	// save C API Source
	{
        auto includeDirective = ln::String::format(u"#include \"{0}.FlatC.generated.h\"", config()->moduleName);
        auto fileName = ln::String::format(u"{0}.FlatC.generated.cpp", config()->moduleName);

        if (!config()->flatCSourceOutputDirOverride.isEmpty())
            includeDirective = ln::String::format(u"#include <LuminoEngine/Runtime/{0}.FlatC.generated.h>", config()->moduleName);

        auto src = ln::FileSystem::readAllText(makeTemplateFilePath(u"Source.cpp.template"))
			.replace("%%IncludeDirective%%", includeDirective)
			.replace("%%HeaderString%%", config()->flatCHeaderString)
			.replace("%%WrapSubclassDecls%%", generateWrapSubclassDecls())
			.replace("%%StructMemberFuncImpls%%", structMemberFuncImplsText.toString())
			.replace("%%ClassMemberFuncImpls%%", classMemberFuncImplsText.toString());

        auto filePath = (config()->flatCSourceOutputDirOverride.isEmpty()) ? makeOutputFilePath("FlatC/src", fileName) : ln::Path::combine(config()->flatCSourceOutputDirOverride, fileName);
		ln::FileSystem::writeAllText(filePath, src);
	}
}

ln::String FlatCSourceGenerator::generateWrapSubclassDecls() const
{
	OutputBuffer code;

	// Delegate 宣言を先に作る
	for (auto& classSymbol : db()->classes()) {
		if (classSymbol->isVirtualHandlerDelegate()) {
			code.AppendLines(makeWrapSubclassDecl(classSymbol));
		}
	}

	for (auto& classSymbol : db()->classes()) {
		if (!classSymbol->isVirtualHandlerDelegate()) {
			code.AppendLines(makeWrapSubclassDecl(classSymbol));
		}
	}

	return code.toString();
}

ln::String FlatCSourceGenerator::makeWrapSubclassDecl(const TypeSymbol* classSymbol) const
{
	OutputBuffer code;

	auto className = makeWrapSubclassName(classSymbol);
	auto baseclassName = classSymbol->fullName();

	// 内部的に作成されたものは Delegate の typedef を作っておく
	if (classSymbol->isVirtualHandlerDelegate()) {
		code.AppendLine(u"// Auto generated override handler");
		const auto& signatue = classSymbol->delegateProtoType();
		OutputBuffer params;
		for (const auto& param : signatue->parameters()) {
			params.AppendCommad(u"{0} {1}", param->getFullQualTypeName(), param->name());
		}
		code.AppendLine(u"using {0} = ln::Delegate<{1}({2})>;", baseclassName, signatue->returnType().type->fullName(), params.toString());
		code.NewLine();
	}

	code.AppendLine(u"class {0} : public {1}", className, baseclassName);
	code.AppendLine(u"{");
	code.AppendLine(u"public:");
	code.IncreaseIndent();
	if (!classSymbol->isStatic())
	{
		// stuct XXXX_OverridePrototypes
		code.AppendLine(u"// Override functions per instance for FlatAPI User.");
		code.AppendLine(makeOverridePrototypesStructDecl(classSymbol));
		code.AppendLine(u"std::unique_ptr<{0}> m_overridePrototypes = nullptr;", makeFlatAPIName_OverridePrototypesStruct(classSymbol));
		code.AppendLine(u"{0}* acquireOverridePrototypes() {{ if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<{0}>(); return m_overridePrototypes.get(); }}", makeFlatAPIName_OverridePrototypesStruct(classSymbol));
		code.NewLine();

		// subclassInfo
		code.AppendLine(u"static {0}* subclassInfo() {{ static {0} info; return &info; }}", makeFlatAPIName_SubclassRegistrationInfo(classSymbol));
		code.AppendLine(u"LNSubinstanceId m_subinstance = 0;").NewLine();

		if (classSymbol->isDelegateObject()) {
			const auto funcPtrType = makeDelegateCallbackFuncPtrName(classSymbol, FlatCharset::Unicode);
			code.AppendLine(u"{0} m_callback;", funcPtrType).NewLine();
		}

		//--------------------
		// Constructor
		code.AppendLine(u"{0}()", makeWrapSubclassName(classSymbol));
		if (classSymbol->isDelegateObject()) {
			// initializer
			const auto resutnType = classSymbol->delegateProtoType()->returnType().type;
			const auto nativeReturnType = resutnType->fullName();
			code.AppendLine(u"  : {0}([this]({1}) -> {2}", baseclassName, makeNativeParamList(classSymbol->delegateProtoType()), nativeReturnType);
			code.AppendLine(u"{");
			code.IncreaseIndent();
			{
				const auto funcPtrType = makeDelegateCallbackFuncPtrName(classSymbol, FlatCharset::Unicode);
				OutputBuffer args;
				args.AppendCommad(u"LNI_OBJECT_TO_HANDLE(this)");
				if (!classSymbol->delegateProtoType()->parameters().isEmpty())
					args.AppendCommad(makeFlatArgList(classSymbol->delegateProtoType()));

				if (resutnType == PredefinedTypes::voidType) {
					code.AppendLine(u"auto r = m_callback({0});", args.toString());
					code.AppendLine(u"if (r != LN_OK) {{ LN_ERROR(\"{0}\"); }}", funcPtrType);
				}
				else {
					args.AppendCommad(u"&ret");

					if (resutnType == PredefinedTypes::boolType)
						code.AppendLine(u"LNBool ret = {};");
					else
						code.AppendLine(nativeReturnType + u" ret = {};");

					code.AppendLine(u"auto r = m_callback({0});", args.toString());
					code.AppendLine(u"if (r != LN_OK) {{ LN_ERROR(\"{0}\"); }}", funcPtrType);

					if (resutnType == PredefinedTypes::boolType)
						code.AppendLine(u"return ret != LN_FALSE;");
					else
						code.AppendLine(u"return ret;");
				}
			}
			code.DecreaseIndent();
			code.AppendLine(u"})");
		}
		code.AppendLine(u"{");	// Constructor body
		code.IncreaseIndent();
		{
			code.AppendLine(makeSubinstanceAllocStmt());
		}
		code.DecreaseIndent();
		code.AppendLine(u"}").NewLine();

		//--------------------
		// Destructor
		code.AppendLine(u"~{0}()", makeWrapSubclassName(classSymbol));
		code.AppendLine(u"{");
		code.IncreaseIndent();
		{
			code.AppendLine(makeSubinstanceFreeStmt());
		}
		code.DecreaseIndent();
		code.AppendLine(u"}").NewLine();

		//--------------------
		// init()
		if (classSymbol->isDelegateObject()) {
			const auto funcPtrType = makeDelegateCallbackFuncPtrName(classSymbol, FlatCharset::Unicode);
			code.AppendLine(u"bool init({0} callback)", funcPtrType);
			code.AppendLine(u"{");
			code.IncreaseIndent();
			{
				code.AppendLine(u"if (!{0}::init()) return false;", classSymbol->fullName());
				code.AppendLine(u"m_callback = callback;");
				code.AppendLine(u"return true;");
			}
			code.DecreaseIndent();
			code.AppendLine(u"}").NewLine();
		}

		//--------------------
		// overrides
		code.AppendLine(u"// Overrides");
		for (auto& method : classSymbol->leafVirtualMethods()) {
			// field decl
			code.AppendLine(u"static {0} s_{0}; // deprecated", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode));

			// override method parameter list
			OutputBuffer paramList;
			for (auto& param : method->parameters()) {
				paramList.AppendCommad(u"{0} {1}", param->getFullQualTypeName(), param->name());
			}

			// override
			{
				code.AppendLine(u"virtual {0} {1}({2}) override", method->returnType().type->fullName(), method->shortName(), paramList.toString());
				code.AppendLine(u"{");
				code.IncreaseIndent();
				OutputBuffer nativeArgList;
				OutputBuffer flatArgList;
				nativeArgList.AppendCommad(u"this");
				flatArgList.AppendCommad(u"LNI_OBJECT_TO_HANDLE(this)");
				for (auto& param : method->parameters()) {
					if (param->type()->isClass())
						flatArgList.AppendCommad(u"LNI_OBJECT_TO_HANDLE({0})", param->name());
					else
						flatArgList.AppendCommad(param->name());
					nativeArgList.AppendCommad(param->name());
				}

				code.AppendLine(u"if (m_overridePrototypes) {");
				code.IncreaseIndent();
				{
					code.AppendLine(u"if (auto func = m_overridePrototypes->{0}) {{", makeFlatAPIName_OverrideFunc(method, FlatCharset::Unicode));
					code.IncreaseIndent();
					{
						if (method->returnType().type == PredefinedTypes::voidType) {
							code.AppendLine(u"func->call({0});", nativeArgList.toString());
							code.AppendLine(u"return;");
						}
						else {
							code.AppendLine(u"return func->call({0});", nativeArgList.toString());
						}
					}
					code.DecreaseIndent();
					code.AppendLine(u"}");
				}
				code.DecreaseIndent();
				code.AppendLine(u"}");


				if (method->returnType().type == PredefinedTypes::voidType)
					code.AppendLine(u"if (s_{0}) s_{0}({1});", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode), flatArgList.toString());
				// ↑のは deprecated なのでこれ以上対応しない

				code.AppendLine(makeBaseCallerExpr(classSymbol, method));

				code.DecreaseIndent();
				code.AppendLine(u"}");
			}

			// base caller
			{
				code.AppendLine(u"{0} {1}_CallBase({2})", method->returnType().type->fullName(), method->shortName(), paramList.toString());
				code.AppendLine(u"{");
				code.IncreaseIndent();
				code.AppendLine(makeBaseCallerExpr(classSymbol, method));
				code.DecreaseIndent();
				code.AppendLine(u"}");
				code.NewLine();
			}
		}

		//--------------------
		// TypeInfo
		if (!classSymbol->isStatic()) {
			ln::String baseClassName = u"Object";
			if (classSymbol->baseClass()) baseClassName = classSymbol->baseClass()->shortName();
			code.AppendLine(u"// TypeInfo");
			code.AppendLine(u"ln::TypeInfo* m_typeInfoOverride = nullptr;");
			code.AppendLine(u"virtual void setTypeInfoOverride(ln::TypeInfo* value) override");
			code.AppendLine(u"{");
			code.AppendLine(u"    m_typeInfoOverride = value;");
			code.AppendLine(u"}");
			code.AppendLine(u"virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override");
			code.AppendLine(u"{");
			code.AppendLine(u"    if (m_typeInfoOverride)");
			code.AppendLine(u"        return m_typeInfoOverride;");
			code.AppendLine(u"    else");
			code.AppendLine(u"        return ln::TypeInfo::getTypeInfo<{0}>();", baseClassName);
			code.AppendLine(u"}");
			code.NewLine();
		}
	}
	code.DecreaseIndent();
	code.AppendLine(u"};").NewLine();

	//--------------------
	// Virtuals
	for (auto& method : classSymbol->leafVirtualMethods()) {
		code.AppendLine(u"{0} {1}::s_{0} = nullptr;", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode), makeWrapSubclassName(classSymbol));
	}
	code.NewLine();

	//--------------------
	// xxxx_Create()
	if (classSymbol->isDelegateObject()) {
		code.AppendLine(makeCreateDelegateObjectFuncHeader(classSymbol));
		code.AppendLine(u"{");
		code.IncreaseIndent();
		{
			code.AppendLine(u"LNI_FUNC_TRY_BEGIN;");
			code.AppendLine(u"LNI_CREATE_OBJECT(outDelegate, {0}, init, callback);", className);
			code.AppendLine(u"LNI_FUNC_TRY_END_RETURN;");
		}
		code.DecreaseIndent();
		code.AppendLine(u"}").NewLine();
	}

	return code.toString();
}

ln::String FlatCSourceGenerator::makeNativeParamList(const MethodSymbol* method) const
{
	OutputBuffer params;
	for (auto& param : method->parameters()) {
		params.AppendCommad(u"{0} {1}", param->getFullQualTypeName(), param->name());
	}
	return params.toString();
}

// FlatC(Handleなど) -> Native(UIElement*など) への呼び出し実引数リストを作成する。
ln::String FlatCSourceGenerator::makeNativeArgList(const MethodSymbol* method) const
{
	OutputBuffer argList;
	for (auto& param : method->parameters()) {
		if (param->type()->isObjectGroup())
			argList.AppendCommad(u"LNI_HANDLE_TO_OBJECT({0}, {1})", param->type()->fullName(), param->name());
		else
			argList.AppendCommad(param->name());
	}
	return argList.toString();
}

ln::String FlatCSourceGenerator::makeOverridePrototypesStructDecl(const TypeSymbol* classSymbol) const
{
	OutputBuffer code;
	code.AppendLine(u"struct " + makeFlatAPIName_OverridePrototypesStruct(classSymbol));
	code.AppendLine(u"{");
	code.IncreaseIndent();
	{
		//for (auto& method : classSymbol->leafVirtualMethods()) {
		//	code.AppendLine(u"{0} {1};",
		//		makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode),
		//		makeFlatAPIName_OverrideFunc(method, FlatCharset::Unicode));

		//	//code.AppendLine(u"{0} {1};",
		//	//	makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode),
		//	//	makeFlatAPIName_OverrideFunc(method, FlatCharset::Unicode));
		//}


		for (auto& method : classSymbol->virtualPrototypeSetters()) {
			code.AppendLine(u"ln::Ref<{0}> {1};",
				makeWrapSubclassName(method->virtualPototypeDelegate()),
				makeFlatAPIName_OverrideFunc(method->virtualPototypeSouceMethod(), FlatCharset::Unicode));
		}
		code.NewLine();
	}
	code.DecreaseIndent();
	code.AppendLine(u"};");
	return code.toString().trim();
}

ln::String FlatCSourceGenerator::makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo, FlatCharset charset)
{
	// function header
	ln::String funcHeader = makeFuncHeader(methodInfo, charset);

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

	{
		// make call args
		OutputBuffer args;
		for (auto& paramInfo : methodInfo->parameters())
		{
			auto* type = paramInfo->type();
			if (type->isObjectGroup())
			{
				args.AppendCommad(u"LNI_HANDLE_TO_OBJECT({0}, {1})", type->fullName(), paramInfo->name());
			}
			else if (type->isStruct())
			{
				ln::String castTo = type->fullName();
				if (paramInfo->isIn()) castTo = "const " + castTo;
				args.AppendCommad(u"*reinterpret_cast<{0}*>({1})", castTo, paramInfo->name());
			}
			else if (type->isEnum())
			{
				args.AppendCommad(u"static_cast<{0}>({1})", type->fullName(), paramInfo->name());
			}
			else if (type == PredefinedTypes::boolType)
			{
				args.AppendCommad(u"LNI_LNBOOL_TO_BOOL({0})", paramInfo->name());
			}
            else if (type->isString() && charset == FlatCharset::Ascii) {
                args.AppendCommad(u"LNI_ASTRPTR_TO_STRING({0})", paramInfo->name());
            }
			else
			{
				args.AppendCommad(paramInfo->name());
			}
		}

		// return output
		OutputBuffer returnAssignmentExpr;
		if (methodInfo->returnType().type != PredefinedTypes::voidType)
		{
			if (methodInfo->returnType().type->isStruct())
				returnAssignmentExpr.append("*outReturn = ln::detail::convertStructForced<{0}>", makeFlatClassName(methodInfo->returnType().type));
			else if (methodInfo->returnType().type->isEnum())
				returnAssignmentExpr.append("*outReturn = static_cast<{0}>", makeFlatClassName(methodInfo->returnType().type));
			else if (methodInfo->returnType().type->isClass())
				if (methodInfo->returnType().strongReference)
					returnAssignmentExpr.append("*outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE");
				else
					returnAssignmentExpr.append("*outReturn = LNI_OBJECT_TO_HANDLE");
			else if (methodInfo->returnType().type == PredefinedTypes::boolType)
				returnAssignmentExpr.append("*outReturn = LNI_BOOL_TO_LNBOOL");
			else if (methodInfo->returnType().type->isString())
                if (charset == FlatCharset::Unicode)
					returnAssignmentExpr.append("*outReturn = LNI_STRING_TO_STRPTR_UTF16");
                else
					returnAssignmentExpr.append("*outReturn = LNI_STRING_TO_STRPTR_A");
			else
				returnAssignmentExpr.append("*outReturn = ");
		}

		// call expression
		ln::String callExp;
		if (typeInfo->isStruct())
		{
			if (methodInfo->isConstructor()) {
				callExp = ln::String::format(u"new (reinterpret_cast<{0}*>({1})) {2}({3});", typeInfo->fullName(), typeInfo->shortName().toLower(), typeInfo->fullName(), args.toString());
			}
			else if (methodInfo->isStatic()) {
				//body.append("({0}::{1}({2}));", typeInfo->fullName(), methodInfo->shortName(), args.toString());
				callExp = ln::String::format(u"({0}::{1}({2}));", typeInfo->fullName(), methodInfo->shortName(), args.toString());
			}
			else {
				ln::String castTo = typeInfo->fullName();
				if (methodInfo->isConst()) castTo = u"const " + castTo;
				callExp = ln::String::format(u"(reinterpret_cast<{0}*>({1})->{2}({3}));", castTo, typeInfo->shortName().toLower(), methodInfo->shortName(), args.toString());
			}
		}
		else
		{
			// static 関数
			if (methodInfo->isStatic())
			{
				//body.append("({0}::{1}({2}));", typeInfo->fullName(), methodInfo->shortName(), args.toString());
				callExp = ln::String::format(u"({0}::{1}({2}));", typeInfo->fullName(), methodInfo->shortName(), args.toString());
			}
			// コンストラクタ 関数
			else if (methodInfo->isConstructor())
			{
				OutputBuffer macroArgs;
				macroArgs.AppendCommad(u"out" + typeInfo->shortName());	// 格納する変数名
				macroArgs.AppendCommad(makeWrapSubclassName(typeInfo));	// クラス名
				macroArgs.AppendCommad(methodInfo->shortName());		// 初期化関数名
				macroArgs.AppendCommad(args.toString());		// 引数
				callExp = ln::String::format(u"LNI_CREATE_OBJECT({0});", macroArgs.toString());
			}
			// 普通のインスタンス 関数
			else
			{
				auto name = methodInfo->shortName();
				if (methodInfo->isVirtual())
					name = (makeWrapSubclassName(typeInfo) + u"::" + name + u"_CallBase");
				callExp = ln::String::format(u"(LNI_HANDLE_TO_OBJECT({0}, {1})->{2}({3}));", makeWrapSubclassName(typeInfo), FlatCCommon::makeInstanceParamName(typeInfo), name, args.toString());
			}
		}

		if (!returnAssignmentExpr.isEmpty()) {
			body.AppendLine(u"if (outReturn) {");
			body.IncreaseIndent();
			body.AppendLine(returnAssignmentExpr.toString() + callExp);
			body.DecreaseIndent();
			body.AppendLine(u"}");
			body.AppendLine(u"else {");
			body.IncreaseIndent();
			body.AppendLine(callExp);
			body.DecreaseIndent();
			body.AppendLine(u"}");
		}
		else {
			body.append(callExp);
		}
	}



	OutputBuffer funcImpl;
	funcImpl.AppendLine(funcHeader);
	funcImpl.AppendLine(u"{");
	funcImpl.IncreaseIndent();
	funcImpl.AppendLine(u"LNI_FUNC_TRY_BEGIN;");
	funcImpl.AppendLine(body.toString());
	funcImpl.AppendLine(u"LNI_FUNC_TRY_END_RETURN;");
	funcImpl.DecreaseIndent();
	funcImpl.AppendLine(u"}");
	return funcImpl.toString();
}

ln::String FlatCSourceGenerator::makeBaseCallerExpr(const TypeSymbol* classSymbol, const MethodSymbol* methodSymbol) const
{
	OutputBuffer argList;
	for (auto& param : methodSymbol->parameters()) {
		argList.AppendCommad(param->name());
	}
	return ln::String::format(u"{0}{1}::{2}({3});",
		(methodSymbol->returnType().type == PredefinedTypes::voidType) ? u"" : u"return ",
		classSymbol->fullName(), methodSymbol->shortName(), argList.toString());
}

//ln::String FlatCSourceGenerator::makeCharsetWrapperFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo, FlatCharset charset)
//{
//	OutputBuffer args;
//    ln::String returnDecl;
//    ln::String postCallExpr;
//	for (auto& param : methodInfo->flatParameters()) {
//        if (param->isReturn() && param->type()->isString()) {
//            returnDecl = u"const LNChar* _ret;";
//            args.AppendCommad(u"&_ret");
//            postCallExpr = u"";
//        }
//		if (!param->isReturn() &&   // 
//            param->type()->isString()) {
//			args.AppendCommad(u"ln::String::fromCString({0}, -1, ln::TextEncoding::utf8Encoding()).c_str()", param->name());
//		}
//		else {
//			args.AppendCommad(param->name());
//		}
//	}
//
//	auto callExpr = ln::String::format(u"LNResult result = {0}({1});", makeFlatFullFuncName(methodInfo, FlatCharset::Unicode), args.toString());
//
//	OutputBuffer code;
//	code.AppendLine(makeFuncHeader(methodInfo, charset));
//	code.AppendLine(u"{");
//	code.IncreaseIndent();
//	code.AppendLine(callExpr);
//	code.AppendLine(u"return result;");
//	code.DecreaseIndent();
//	code.AppendLine(u"}");
//	return code.toString();
//}

//ln::String FlatCSourceGenerator::makeEventConnectorFuncBody(const TypeSymbol* classInfo, const MethodSymbol* methodInfo) const
//{
//	auto delegateType = methodInfo->parameters().front()->type();
//	auto delegateDeclaration = delegateType->delegateProtoType();
//
//	// wrap callback
//	// Note: 今のところ引数ひとつしか想定しないので、引数名は e だけ.
//	OutputBuffer params;
//	OutputBuffer args;
//	args.AppendCommad(methodInfo->flatParameters().front()->name());
//	for (auto& param : delegateDeclaration->parameters()) {
//		params.AppendCommad(u"{0} e", param->getFullQualTypeName(), param->name());
//		if (param->type()->isClass()) {
//			args.AppendCommad(u"LNI_OBJECT_TO_HANDLE(e)");
//		}
//		else {
//			args.AppendCommad(u"e");
//		}
//	}
//	auto callbackLambda = ln::String::format(u"[=]({0}) {{ handler({1}); }}", params.toString(), args.toString());
//
//	OutputBuffer funcImpl;
//	funcImpl.AppendLine(makeFuncHeader(methodInfo, FlatCharset::Unicode));
//	funcImpl.AppendLine(u"{");
//	funcImpl.IncreaseIndent();
//	funcImpl.AppendLine(u"LNI_FUNC_TRY_BEGIN;");
//	funcImpl.AppendLine(u"(LNI_HANDLE_TO_OBJECT({0}, {1})->{2}({3}));", makeWrapSubclassName(classInfo), methodInfo->flatParameters()[0]->name(), methodInfo->shortName(), callbackLambda);
//	funcImpl.AppendLine(u"LNI_FUNC_TRY_END_RETURN;");
//	funcImpl.DecreaseIndent();
//	funcImpl.AppendLine(u"}");
//	return funcImpl.toString();
//}
