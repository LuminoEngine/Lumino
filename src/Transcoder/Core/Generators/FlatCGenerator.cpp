
#include "FlatCGenerator.hpp"

//==============================================================================
// FlatCCommon

ln::String FlatCCommon::makeInstanceParamName(TypeSymbol* type) {
    return type->shortName().toLower();
}

ln::String FlatCCommon::makeCreateDelegateObjectFuncHeader(const TypeSymbol* delegateSymbol) const {
    auto funcPtrType = makeDelegateCallbackFuncPtrName(delegateSymbol, FlatCharset::Unicode);
    auto flatClassName = makeFlatClassName(delegateSymbol);
    return ln::format(U"LN_FLAT_API LNResult {0}_Create({1} callback, LNHandle* outDelegate)", flatClassName, funcPtrType);
}

//==============================================================================
// FlatCHeaderGenerator

void FlatCHeaderGenerator::generate() {
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
    for (auto& structInfo : db()->structs()) {
        structsText.AppendLine(makeDocumentComment(structInfo->document()));
        structsText.AppendLine(U"struct {0}", makeFlatClassName(structInfo));
        structsText.AppendLine(U"{");
        structsText.IncreaseIndent();
        for (auto& fieldInfo : structInfo->fields()) {
            structsText.AppendLine(U"{0} {1};", makeFlatTypeName2(fieldInfo->type()), fieldInfo->name());
        }
        structsText.DecreaseIndent();
        structsText.AppendLine(U"};");
        structsText.NewLine();

        structMemberFuncDeclsText.AppendLine(U"//==============================================================================");
        structMemberFuncDeclsText.AppendLine(U"// {0}", structInfo->fullName());
        structMemberFuncDeclsText.NewLine();

        // function decls
        for (auto& methodInfo : structInfo->publicMethods()) {
            if (methodInfo->isFieldAccessor()) {
            }
            else {
                // comment
                structMemberFuncDeclsText.AppendLine(makeMethodDocumentComment(methodInfo));

                // decl
                structMemberFuncDeclsText.AppendLine(makeFuncHeader(methodInfo, FlatCharset::Unicode) + U";").NewLine(2);
            }
        }
    }

    // classes
    OutputBuffer classMemberFuncDeclsText;
    for (auto& classSymbol : db()->classes()) {
        classMemberFuncDeclsText.AppendLine(U"//==============================================================================");
        classMemberFuncDeclsText.AppendLine(U"// {0}", classSymbol->fullName());
        classMemberFuncDeclsText.NewLine();

        if (classSymbol->typeClass() == TypeClass::DelegateObject) {
            delegatesText.AppendLine(makeDelegateFuncPtrDecl(classSymbol));
            classMemberFuncDeclsText.AppendLine(makeCreateDelegateObjectFuncHeader(classSymbol) + U";");
        }
        else {
            // function decls
            for (auto& method : classSymbol->publicMethods()) {
                // comment
                classMemberFuncDeclsText.AppendLine(makeMethodDocumentComment(method));

                // decl
                classMemberFuncDeclsText.AppendLine(makeFuncHeader(method, FlatCharset::Unicode) + U";");

                if (method->hasStringDecl()) {
                    classMemberFuncDeclsText.AppendLine(makeFuncHeader(method, FlatCharset::Ascii) + U";");
                }

                classMemberFuncDeclsText.NewLine();
            }

            // virtual callback
            if (!classSymbol->leafVirtualMethods().isEmpty()) {
                for (auto& method : classSymbol->leafVirtualMethods()) {
                    // make params
                    OutputBuffer params;
                    for (auto& param : method->flatParameters()) {
                        params.AppendCommad(U"{0} {1}", makeFlatCParamQualTypeName(method, param, FlatCharset::Unicode), param->name());
                    }
                    classMemberFuncDeclsText.AppendLine(U"typedef LNResult(*{0})({1});", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode), params.toString());
                    classMemberFuncDeclsText.AppendLine(makeFlatAPIDecl_SetOverrideCallback(classSymbol, method, FlatCharset::Unicode) + U";");
                    classMemberFuncDeclsText.AppendLine(makeFlatAPIDecl_CallOverrideBase(classSymbol, method, FlatCharset::Unicode) + U";");
                    //classMemberFuncDeclsText.AppendLine(makeFlatAPIDecl_SetPrototype(classSymbol, method, FlatCharset::Unicode) + U";");
                }
                classMemberFuncDeclsText.NewLine();
            }

            // virtualPrototypeSetters
            for (auto& method : classSymbol->virtualPrototypeSetters()) {
                // comment
                classMemberFuncDeclsText.AppendLine(makeMethodDocumentComment(method));
                // decl
                classMemberFuncDeclsText.AppendLine(makeFuncHeader(method, FlatCharset::Unicode) + U";");
                classMemberFuncDeclsText.NewLine();
            }

            // type info
            if (!classSymbol->isStatic()) {
                classMemberFuncDeclsText.AppendLine(makeGetTypeInfoIdFuncHeader(classSymbol) + U";");
            }

            //for (auto& method : classSymbol->publicMethods()) {
            //	classMemberFuncDeclsText.AppendLine(U"extern \"C\" " + makeFuncHeader(method, FlatCharset::Unicode) + U";");
            //}
        }

        if (!classSymbol->isStatic()) {
            classMemberFuncDeclsText.AppendLine(makeFlatAPIDecl_SetManagedTypeInfoId(classSymbol) + U"; // deprecated");

            classMemberFuncDeclsText.AppendLines(makeSubClassRegistrationInfo(classSymbol));
        }

        classMemberFuncDeclsText.NewLine();
    }

    // save C API Header
    {
        ln::String src = ln::FileSystem::readAllText(makeTemplateFilePath(U"LuminoC.template.h"));
        src = src.replace(U"%%Structs%%", structsText.toString());
        src = src.replace(U"%%Enums%%", makeEnumDecls());
        src = src.replace(U"%%Delegates%%", delegatesText.toString());
        src = src.replace(U"%%StructMemberFuncDecls%%", structMemberFuncDeclsText.toString());
        src = src.replace(U"%%ClassMemberFuncDecls%%", classMemberFuncDeclsText.toString());

        auto filePath = makeFlatCHeaderOutputPath();
        ln::FileSystem::writeAllText(filePath, src, ln::TextEncoding::getEncoding(ln::EncodingType::UTF8));
    }
}

ln::String FlatCHeaderGenerator::makeDocumentComment(DocumentInfo* doc) const {
    OutputBuffer code;
    code.AppendLine(U"/**");
    code.IncreaseIndent();

    // @brief
    code.AppendLine(U"@brief {0}", doc->summary());

    // @param
    for (auto& param : doc->params()) {
        code.AppendLine(U"@param[{0}] {1} : {2}", param->io(), param->name(), param->description());
    }

    // @return
    if (!doc->returns().isEmpty())
        code.AppendLine(U"@return {0}", doc->returns());

    // @details
    if (!doc->details().isEmpty())
        code.AppendLine(U"@details {0}", doc->details());

    code.DecreaseIndent();
    code.AppendLine(U"*/");

    return code.toString().trim();
}

ln::String FlatCHeaderGenerator::makeMethodDocumentComment(const MethodSymbol* method) const {

    auto doc = method->document();

    OutputBuffer code;
    code.AppendLine(U"/**");
    code.IncreaseIndent();

    // @brief
    code.AppendLine(U"@brief {0}", doc->summary());

    // @param
    for (auto& param : doc->flatParams()) {
        auto desc = param->description();
        auto paramInfo = method->findFlatParameter(param->name());
        if (paramInfo && paramInfo->qualType().strongReference) {
            desc += U" (このオブジェクトは不要になったら LNObject_Release で参照を開放する必要があります)";
        }

        code.AppendLine(U"@param[{0}] {1} : {2}", param->io(), param->name(), desc);
    }

    // @return
    if (!doc->returns().isEmpty())
        code.AppendLine(U"@return {0}", doc->returns());

    // @details
    if (!doc->details().isEmpty())
        code.AppendLine(U"@details {0}", doc->details());

    code.DecreaseIndent();
    code.AppendLine(U"*/");

    return code.toString().trim();
}

ln::String FlatCHeaderGenerator::makeDelegateFuncPtrDecl(const TypeSymbol* delegateSymbol) const {
    // return
    auto returnType = (delegateSymbol->isDelegateObject()) ? U"LNResult" : U"void";

    // make params
    OutputBuffer params;
    //if (delegateSymbol->isDelegateObject()) {
    //	params.AppendCommad(U"LNHandle");
    //}
    for (auto& param : delegateSymbol->delegateProtoType()->flatParameters()) {
        params.AppendCommad(U"{0} {1}", makeFlatCParamQualTypeName(nullptr, param, FlatCharset::Unicode), param->name());
    }
    return ln::format(U"typedef {0}(*{1})({2});", returnType, makeDelegateCallbackFuncPtrName(delegateSymbol, FlatCharset::Unicode), params.toString());
}

ln::String FlatCHeaderGenerator::makeEnumDecls() const {
    OutputBuffer code;

    for (auto& symbol : db()->enums()) {
        auto typeName = makeFlatClassName(symbol);

        code.AppendLine(makeDocumentComment(symbol->document()));
        code.AppendLine(U"typedef enum tag" + typeName);
        code.AppendLine(U"{");
        code.IncreaseIndent();

        for (auto& member : symbol->constants()) {
            code.AppendLine(makeDocumentComment(member->document()));
            code.AppendLine(U"{0} = {1},", makeFlatEnumMemberName(symbol, member), member->value()->get<int>());
            code.NewLine();
        }

        code.DecreaseIndent();
        code.AppendLine(U"} " + typeName + U";");
        code.NewLine();
    }

    return code.toString().trim();
}

ln::String FlatCHeaderGenerator::makeSubClassRegistrationInfo(const TypeSymbol* classSymbol) const {
    OutputBuffer code;

    const auto& structName = makeFlatAPIName_SubclassRegistrationInfo(classSymbol);

    code.AppendLine(U"typedef struct tag" + structName, makeFlatTypeName2(classSymbol));
    code.AppendLine(U"{");
    code.IncreaseIndent();
    {
        code.AppendLine(U"int64_t subclassId;	// ManagedTypeInfoId");
        code.AppendLine(U"LNSubinstanceAllocFunc subinstanceAllocFunc;");
        code.AppendLine(U"LNSubinstanceFreeFunc subinstanceFreeFunc;");

        for (auto& method : classSymbol->leafVirtualMethods()) {
            //// make params
            //OutputBuffer params;
            //for (auto& param : method->flatParameters()) {
            //	params.AppendCommad("{0} {1}", makeFlatCParamQualTypeName(method, param, FlatCharset::Unicode), param->name());
            //}
            ////classMemberFuncDeclsText.AppendLine(U"typedef LNResult(*{0})({1});", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode), params.toString());
            //code.AppendLine(makeFlatAPIDecl_SetOverrideCallback(classSymbol, method, FlatCharset::Unicode) + U";");
            //code.AppendLine(makeFlatAPIDecl_CallOverrideBase(classSymbol, method, FlatCharset::Unicode) + U";");

            code.AppendLine(U"{0} {1};", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode), makeFlatAPIName_OverrideFunc(method, FlatCharset::Unicode));
        }

        code.NewLine();
    }
    code.DecreaseIndent();
    code.AppendLine(U"} " + structName + U";");

    code.NewLine();
    code.AppendLine(U"extern LN_FLAT_API void {0}(const {1}* info);", makeFlatAPIName_RegisterSubclassTypeInfo(classSymbol), structName);
    code.AppendLine(U"extern LN_FLAT_API LNSubinstanceId {0}(LNHandle handle);", makeFlatAPIName_GetSubinstanceId(classSymbol), structName);

    return code.toString().trim();
}

//==============================================================================
// FlatCSourceGenerator

void FlatCSourceGenerator::generate() {
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
				else*/
                {
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
                funcImpl.AppendLine(U"{");
                funcImpl.IncreaseIndent();
                funcImpl.AppendLine(U"LNI_FUNC_TRY_BEGIN;");
                funcImpl.AppendLine(U"(LNI_HANDLE_TO_OBJECT({0}, {1})->{2}_CallBase({3}));", makeWrapSubclassName(classSymbol), method->flatParameters()[0]->name(), method->shortName(), makeNativeArgList(method));
                funcImpl.AppendLine(U"LNI_FUNC_TRY_END_RETURN;");
                funcImpl.DecreaseIndent();
                funcImpl.AppendLine(U"}");

                funcImpl.AppendLine(makeFlatAPIDecl_SetOverrideCallback(classSymbol, method, FlatCharset::Unicode));
                funcImpl.AppendLine(U"{");
                funcImpl.IncreaseIndent();
                funcImpl.AppendLine(U"{0}::s_{1} = callback;", makeWrapSubclassName(classSymbol), makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode));
                funcImpl.AppendLine(U"return LN_OK;");
                funcImpl.DecreaseIndent();
                funcImpl.AppendLine(U"}");

                //funcImpl.AppendLine(makeFlatAPIDecl_SetPrototype(classSymbol, method, FlatCharset::Unicode));
                //funcImpl.AppendLine(U"{");
                //funcImpl.IncreaseIndent();
                //{
                //	funcImpl.AppendLine(U"LNI_HANDLE_TO_OBJECT({0}, {1})->acquireOverridePrototypes()->{2} = func;", makeWrapSubclassName(classSymbol), method->flatParameters()[0]->name(), makeFlatAPIName_OverrideFunc(method, FlatCharset::Unicode));
                //	funcImpl.AppendLine(U"return LN_OK;");
                //}
                //funcImpl.DecreaseIndent();
                //funcImpl.AppendLine(U"}");

                classMemberFuncImplsText.AppendLine(funcImpl.toString());
            }

            // virtual prototypes
            for (auto& method : classSymbol->virtualPrototypeSetters()) {

                classMemberFuncImplsText.AppendLine(makeFuncHeader(method, FlatCharset::Unicode));
                classMemberFuncImplsText.AppendLine(U"{");
                classMemberFuncImplsText.IncreaseIndent();
                {
                    classMemberFuncImplsText.AppendLine(
                        U"LNI_HANDLE_TO_OBJECT({0}, {1})->acquireOverridePrototypes()->{2} = LNI_HANDLE_TO_OBJECT({3}, callback);",
                        makeWrapSubclassName(classSymbol),
                        method->flatParameters()[0]->name(),
                        makeFlatAPIName_OverrideFunc(method->virtualPototypeSouceMethod(), FlatCharset::Unicode),
                        makeWrapSubclassName(method->virtualPototypeDelegate()));
                    classMemberFuncImplsText.AppendLine(U"return LN_OK;");
                }
                classMemberFuncImplsText.DecreaseIndent();
                classMemberFuncImplsText.AppendLine(U"}");
            }

            // type info
            if (!classSymbol->isStatic()) {
                classMemberFuncImplsText.AppendLine(makeGetTypeInfoIdFuncHeader(classSymbol));
                classMemberFuncImplsText.AppendLine(U"{");
                classMemberFuncImplsText.IncreaseIndent();
                classMemberFuncImplsText.AppendLine(U"return ln::TypeInfo::getTypeInfo<{0}>()->id();", classSymbol->fullName());
                classMemberFuncImplsText.DecreaseIndent();
                classMemberFuncImplsText.AppendLine(U"}");
                classMemberFuncImplsText.NewLine();
            }
        }

        // TypeInfo id setter
        if (!classSymbol->isStatic()) {
            classMemberFuncImplsText.AppendLine(makeFlatAPIDecl_SetManagedTypeInfoId(classSymbol));
            classMemberFuncImplsText.AppendLine(U"{");
            classMemberFuncImplsText.IncreaseIndent();
            classMemberFuncImplsText.AppendLine(U"::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<{0}>(), id);", classSymbol->fullName());
            classMemberFuncImplsText.DecreaseIndent();
            classMemberFuncImplsText.AppendLine(U"}");
            classMemberFuncImplsText.NewLine();

            // _RegisterSubclassTypeInfo
            classMemberFuncImplsText.AppendLine(U"void {0}(const {1}* info)", makeFlatAPIName_RegisterSubclassTypeInfo(classSymbol), makeFlatAPIName_SubclassRegistrationInfo(classSymbol));
            classMemberFuncImplsText.AppendLine(U"{");
            classMemberFuncImplsText.IncreaseIndent();
            {
                classMemberFuncImplsText.AppendLine(U"if (info) {");
                classMemberFuncImplsText.IncreaseIndent();
                {
                    classMemberFuncImplsText.AppendLine(U"::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<{0}>(), info->subclassId);", classSymbol->fullName());
                    classMemberFuncImplsText.AppendLine(U"*{0}::subclassInfo() = *info;", makeWrapSubclassName(classSymbol));
                }
                classMemberFuncImplsText.DecreaseIndent();
                classMemberFuncImplsText.AppendLine(U"}");
            }
            classMemberFuncImplsText.DecreaseIndent();
            classMemberFuncImplsText.AppendLine(U"}");
            classMemberFuncImplsText.NewLine();

            // GetSubinstanceId
            classMemberFuncImplsText.AppendLine(U"LNSubinstanceId {0}(LNHandle handle)", makeFlatAPIName_GetSubinstanceId(classSymbol));
            classMemberFuncImplsText.AppendLine(U"{");
            classMemberFuncImplsText.IncreaseIndent();
            {
                classMemberFuncImplsText.AppendLine(U"if (handle) {");
                classMemberFuncImplsText.IncreaseIndent();
                {
                    classMemberFuncImplsText.AppendLine(U"LNI_FUNC_TRY_BEGIN;");
                    classMemberFuncImplsText.AppendLine(U"return (LNI_HANDLE_TO_OBJECT({0}, handle))->m_subinstance;", makeWrapSubclassName(classSymbol));
                    classMemberFuncImplsText.AppendLine(U"LNI_FUNC_TRY_END_RETURN;");
                }
                classMemberFuncImplsText.DecreaseIndent();
                classMemberFuncImplsText.AppendLine(U"}");
                classMemberFuncImplsText.AppendLine(U"return 0;");
            }
            classMemberFuncImplsText.DecreaseIndent();
            classMemberFuncImplsText.AppendLine(U"}");
            classMemberFuncImplsText.NewLine();
        }
    }

    // save C API Source
    {
        auto includeDirective = ln::format(U"#include \"{0}.FlatC.generated.h\"", config()->moduleName);
        auto fileName = ln::format(U"{0}.FlatC.generated.cpp", config()->moduleName);

        if (!config()->flatCSourceOutputDirOverride.isEmpty())
            includeDirective = ln::format(U"#include <LuminoEngine/Runtime/{0}.FlatC.generated.h>", config()->moduleName);

        auto src = ln::FileSystem::readAllText(makeTemplateFilePath(U"Source.cpp.template"))
                       .replace(U"%%IncludeDirective%%", includeDirective)
                       .replace(U"%%HeaderString%%", config()->flatCHeaderString)
                       .replace(U"%%WrapSubclassDecls%%", generateWrapSubclassDecls())
                       .replace(U"%%StructMemberFuncImpls%%", structMemberFuncImplsText.toString())
                       .replace(U"%%ClassMemberFuncImpls%%", classMemberFuncImplsText.toString());

        auto filePath = (config()->flatCSourceOutputDirOverride.isEmpty()) ? makeOutputFilePath(U"FlatC/src", fileName) : ln::Path::combine(config()->flatCSourceOutputDirOverride, fileName);
        ln::FileSystem::writeAllText(filePath, src);
    }
}

ln::String FlatCSourceGenerator::generateWrapSubclassDecls() const {
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

ln::String FlatCSourceGenerator::makeWrapSubclassDecl(const TypeSymbol* classSymbol) const {
    OutputBuffer code;

    auto className = makeWrapSubclassName(classSymbol);
    auto baseclassName = classSymbol->fullName();

    // 内部的に作成されたものは Delegate の typedef を作っておく
    if (classSymbol->isVirtualHandlerDelegate()) {
        code.AppendLine(U"// Auto generated override handler");
        const auto& signatue = classSymbol->delegateProtoType();
        OutputBuffer params;
        for (const auto& param : signatue->parameters()) {
            params.AppendCommad(U"{0} {1}", param->getFullQualTypeName(), param->name());
        }
        code.AppendLine(U"using {0} = ln::Delegate<{1}({2})>;", baseclassName, signatue->returnType().type->fullName(), params.toString());
        code.NewLine();
    }

    code.AppendLine(U"class {0} : public {1}", className, baseclassName);
    code.AppendLine(U"{");
    code.AppendLine(U"public:");
    code.IncreaseIndent();
    if (!classSymbol->isStatic()) {
        // stuct XXXX_OverridePrototypes
        code.AppendLine(U"// Override functions per instance for FlatAPI User.");
        code.AppendLine(makeOverridePrototypesStructDecl(classSymbol));
        code.AppendLine(U"std::unique_ptr<{0}> m_overridePrototypes = nullptr;", makeFlatAPIName_OverridePrototypesStruct(classSymbol));
        code.AppendLine(U"{0}* acquireOverridePrototypes() {{ if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<{0}>(); return m_overridePrototypes.get(); }}", makeFlatAPIName_OverridePrototypesStruct(classSymbol));
        code.NewLine();

        // subclassInfo
        code.AppendLine(U"static {0}* subclassInfo() {{ static {0} info; return &info; }}", makeFlatAPIName_SubclassRegistrationInfo(classSymbol));
        code.AppendLine(U"LNSubinstanceId m_subinstance = 0;").NewLine();

        if (classSymbol->isDelegateObject()) {
            const auto funcPtrType = makeDelegateCallbackFuncPtrName(classSymbol, FlatCharset::Unicode);
            code.AppendLine(U"{0} m_callback;", funcPtrType).NewLine();
        }

        //--------------------
        // Constructor
        code.AppendLine(U"{0}()", makeWrapSubclassName(classSymbol));
        if (classSymbol->isDelegateObject()) {
            // initializer
            const auto resutnType = classSymbol->delegateProtoType()->returnType().type;
            const auto nativeReturnType = resutnType->fullName();
            code.AppendLine(U"  : {0}([this]({1}) -> {2}", baseclassName, makeNativeParamList(classSymbol->delegateProtoType()), nativeReturnType);
            code.AppendLine(U"{");
            code.IncreaseIndent();
            {
                const auto funcPtrType = makeDelegateCallbackFuncPtrName(classSymbol, FlatCharset::Unicode);
                OutputBuffer args;
                args.AppendCommad(U"LNI_OBJECT_TO_HANDLE(this)");
                if (!classSymbol->delegateProtoType()->parameters().isEmpty())
                    args.AppendCommad(makeFlatArgList(classSymbol->delegateProtoType()));

                if (resutnType == PredefinedTypes::voidType) {
                    code.AppendLine(U"auto r = m_callback({0});", args.toString());
                    code.AppendLine(U"if (r != LN_OK) {{ LN_ERROR(\"{0}\"); }}", funcPtrType);
                }
                else {
                    args.AppendCommad(U"&ret");

                    if (resutnType == PredefinedTypes::boolType)
                        code.AppendLine(U"LNBool ret = {};");
                    else
                        code.AppendLine(nativeReturnType + U" ret = {};");

                    code.AppendLine(U"auto r = m_callback({0});", args.toString());
                    code.AppendLine(U"if (r != LN_OK) {{ LN_ERROR(\"{0}\"); }}", funcPtrType);

                    if (resutnType == PredefinedTypes::boolType)
                        code.AppendLine(U"return ret != LN_FALSE;");
                    else
                        code.AppendLine(U"return ret;");
                }
            }
            code.DecreaseIndent();
            code.AppendLine(U"})");
        }
        code.AppendLine(U"{"); // Constructor body
        code.IncreaseIndent();
        {
            code.AppendLine(makeSubinstanceAllocStmt());
        }
        code.DecreaseIndent();
        code.AppendLine(U"}").NewLine();

        //--------------------
        // Destructor
        code.AppendLine(U"~{0}()", makeWrapSubclassName(classSymbol));
        code.AppendLine(U"{");
        code.IncreaseIndent();
        {
            code.AppendLine(makeSubinstanceFreeStmt());
        }
        code.DecreaseIndent();
        code.AppendLine(U"}").NewLine();

        //--------------------
        // init()
        if (classSymbol->isDelegateObject()) {
            const auto funcPtrType = makeDelegateCallbackFuncPtrName(classSymbol, FlatCharset::Unicode);
            code.AppendLine(U"bool init({0} callback)", funcPtrType);
            code.AppendLine(U"{");
            code.IncreaseIndent();
            {
                code.AppendLine(U"if (!{0}::init()) return false;", classSymbol->fullName());
                code.AppendLine(U"m_callback = callback;");
                code.AppendLine(U"return true;");
            }
            code.DecreaseIndent();
            code.AppendLine(U"}").NewLine();
        }

        //--------------------
        // overrides
        code.AppendLine(U"// Overrides");
        for (auto& method : classSymbol->leafVirtualMethods()) {
            // field decl
            code.AppendLine(U"static {0} s_{0}; // deprecated", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode));

            // override method parameter list
            OutputBuffer paramList;
            for (auto& param : method->parameters()) {
                paramList.AppendCommad(U"{0} {1}", param->getFullQualTypeName(), param->name());
            }

            // override
            {
                code.AppendLine(U"virtual {0} {1}({2}) override", method->returnType().type->fullName(), method->shortName(), paramList.toString());
                code.AppendLine(U"{");
                code.IncreaseIndent();
                OutputBuffer nativeArgList;
                OutputBuffer flatArgList;
                nativeArgList.AppendCommad(U"this");
                flatArgList.AppendCommad(U"LNI_OBJECT_TO_HANDLE(this)");
                for (auto& param : method->parameters()) {
                    if (param->type()->isClass())
                        flatArgList.AppendCommad(U"LNI_OBJECT_TO_HANDLE({0})", param->name());
                    else
                        flatArgList.AppendCommad(param->name());
                    nativeArgList.AppendCommad(param->name());
                }

                code.AppendLine(U"if (m_overridePrototypes) {");
                code.IncreaseIndent();
                {
                    code.AppendLine(U"if (auto func = m_overridePrototypes->{0}) {{", makeFlatAPIName_OverrideFunc(method, FlatCharset::Unicode));
                    code.IncreaseIndent();
                    {
                        if (method->returnType().type == PredefinedTypes::voidType) {
                            code.AppendLine(U"func->call({0});", nativeArgList.toString());
                            code.AppendLine(U"return;");
                        }
                        else {
                            code.AppendLine(U"return func->call({0});", nativeArgList.toString());
                        }
                    }
                    code.DecreaseIndent();
                    code.AppendLine(U"}");
                }
                code.DecreaseIndent();
                code.AppendLine(U"}");

                if (method->returnType().type == PredefinedTypes::voidType)
                    code.AppendLine(U"if (s_{0}) s_{0}({1});", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode), flatArgList.toString());
                // ↑のは deprecated なのでこれ以上対応しない

                code.AppendLine(makeBaseCallerExpr(classSymbol, method));

                code.DecreaseIndent();
                code.AppendLine(U"}");
            }

            // base caller
            {
                code.AppendLine(U"{0} {1}_CallBase({2})", method->returnType().type->fullName(), method->shortName(), paramList.toString());
                code.AppendLine(U"{");
                code.IncreaseIndent();
                code.AppendLine(makeBaseCallerExpr(classSymbol, method));
                code.DecreaseIndent();
                code.AppendLine(U"}");
                code.NewLine();
            }
        }

        //--------------------
        // TypeInfo
        if (!classSymbol->isStatic()) {
            ln::String baseClassName = U"Object";
            if (classSymbol->baseClass()) baseClassName = classSymbol->baseClass()->shortName();
            code.AppendLine(U"// TypeInfo");
            code.AppendLine(U"ln::TypeInfo* m_typeInfoOverride = nullptr;");
            code.AppendLine(U"virtual void setTypeInfoOverride(ln::TypeInfo* value) override");
            code.AppendLine(U"{");
            code.AppendLine(U"    m_typeInfoOverride = value;");
            code.AppendLine(U"}");
            code.AppendLine(U"virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override");
            code.AppendLine(U"{");
            code.AppendLine(U"    if (m_typeInfoOverride)");
            code.AppendLine(U"        return m_typeInfoOverride;");
            code.AppendLine(U"    else");
            code.AppendLine(U"        return ln::TypeInfo::getTypeInfo<{0}>();", baseClassName);
            code.AppendLine(U"}");
            code.NewLine();
        }
    }
    code.DecreaseIndent();
    code.AppendLine(U"};").NewLine();

    //--------------------
    // Virtuals
    for (auto& method : classSymbol->leafVirtualMethods()) {
        code.AppendLine(U"{0} {1}::s_{0} = nullptr;", makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode), makeWrapSubclassName(classSymbol));
    }
    code.NewLine();

    //--------------------
    // xxxx_Create()
    if (classSymbol->isDelegateObject()) {
        code.AppendLine(makeCreateDelegateObjectFuncHeader(classSymbol));
        code.AppendLine(U"{");
        code.IncreaseIndent();
        {
            code.AppendLine(U"LNI_FUNC_TRY_BEGIN;");
            code.AppendLine(U"LNI_CREATE_OBJECT(outDelegate, {0}, init, callback);", className);
            code.AppendLine(U"LNI_FUNC_TRY_END_RETURN;");
        }
        code.DecreaseIndent();
        code.AppendLine(U"}").NewLine();
    }

    return code.toString();
}

ln::String FlatCSourceGenerator::makeNativeParamList(const MethodSymbol* method) const {
    OutputBuffer params;
    for (auto& param : method->parameters()) {
        params.AppendCommad(U"{0} {1}", param->getFullQualTypeName(), param->name());
    }
    return params.toString();
}

// FlatC(Handleなど) -> Native(UIElement*など) への呼び出し実引数リストを作成する。
ln::String FlatCSourceGenerator::makeNativeArgList(const MethodSymbol* method) const {
    OutputBuffer argList;
    for (auto& param : method->parameters()) {
        if (param->type()->isObjectGroup())
            argList.AppendCommad(U"LNI_HANDLE_TO_OBJECT({0}, {1})", param->type()->fullName(), param->name());
        else
            argList.AppendCommad(param->name());
    }
    return argList.toString();
}

ln::String FlatCSourceGenerator::makeOverridePrototypesStructDecl(const TypeSymbol* classSymbol) const {
    OutputBuffer code;
    code.AppendLine(U"struct " + makeFlatAPIName_OverridePrototypesStruct(classSymbol));
    code.AppendLine(U"{");
    code.IncreaseIndent();
    {
        //for (auto& method : classSymbol->leafVirtualMethods()) {
        //	code.AppendLine(U"{0} {1};",
        //		makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode),
        //		makeFlatAPIName_OverrideFunc(method, FlatCharset::Unicode));

        //	//code.AppendLine(U"{0} {1};",
        //	//	makeFlatVirutalCallbackFuncPtrName(classSymbol, method, FlatCharset::Unicode),
        //	//	makeFlatAPIName_OverrideFunc(method, FlatCharset::Unicode));
        //}

        for (auto& method : classSymbol->virtualPrototypeSetters()) {
            code.AppendLine(U"ln::Ref<{0}> {1};", makeWrapSubclassName(method->virtualPototypeDelegate()), makeFlatAPIName_OverrideFunc(method->virtualPototypeSouceMethod(), FlatCharset::Unicode));
        }
        code.NewLine();
    }
    code.DecreaseIndent();
    code.AppendLine(U"};");
    return code.toString().trim();
}

ln::String FlatCSourceGenerator::makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo, FlatCharset charset) {
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
        for (auto& paramInfo : methodInfo->parameters()) {
            auto* type = paramInfo->type();
            if (type->isObjectGroup()) {
                args.AppendCommad(U"LNI_HANDLE_TO_OBJECT({0}, {1})", type->fullName(), paramInfo->name());
            }
            else if (type->isStruct()) {
                ln::String castTo = type->fullName();
                if (paramInfo->isIn()) castTo = U"const " + castTo;
                args.AppendCommad(U"*reinterpret_cast<{0}*>({1})", castTo, paramInfo->name());
            }
            else if (type->isEnum()) {
                args.AppendCommad(U"static_cast<{0}>({1})", type->fullName(), paramInfo->name());
            }
            else if (type == PredefinedTypes::boolType) {
                args.AppendCommad(U"LNI_LNBOOL_TO_BOOL({0})", paramInfo->name());
            }
            else if (type->isString() && charset == FlatCharset::Ascii) {
                args.AppendCommad(U"LNI_ASTRPTR_TO_STRING({0})", paramInfo->name());
            }
            else {
                args.AppendCommad(paramInfo->name());
            }
        }

        // return output
        OutputBuffer returnAssignmentExpr;
        if (methodInfo->returnType().type != PredefinedTypes::voidType) {
            if (methodInfo->returnType().type->isStruct())
                returnAssignmentExpr.append(U"*outReturn = ln::detail::convertStructForced<{0}>", makeFlatClassName(methodInfo->returnType().type));
            else if (methodInfo->returnType().type->isEnum())
                returnAssignmentExpr.append(U"*outReturn = static_cast<{0}>", makeFlatClassName(methodInfo->returnType().type));
            else if (methodInfo->returnType().type->isClass())
                if (methodInfo->returnType().strongReference)
                    returnAssignmentExpr.append(U"*outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE");
                else
                    returnAssignmentExpr.append(U"*outReturn = LNI_OBJECT_TO_HANDLE");
            else if (methodInfo->returnType().type == PredefinedTypes::boolType)
                returnAssignmentExpr.append(U"*outReturn = LNI_BOOL_TO_LNBOOL");
            else if (methodInfo->returnType().type->isString())
                if (charset == FlatCharset::Unicode)
                    returnAssignmentExpr.append(U"*outReturn = LNI_STRING_TO_STRPTR_UTF16");
                else
                    returnAssignmentExpr.append(U"*outReturn = LNI_STRING_TO_STRPTR_A");
            else
                returnAssignmentExpr.append(U"*outReturn = ");
        }

        // call expression
        ln::String callExp;
        if (typeInfo->isStruct()) {
            if (methodInfo->isConstructor()) {
                callExp = ln::format(U"new (reinterpret_cast<{0}*>({1})) {2}({3});", typeInfo->fullName(), typeInfo->shortName().toLower(), typeInfo->fullName(), args.toString());
            }
            else if (methodInfo->isStatic()) {
                //body.append("({0}::{1}({2}));", typeInfo->fullName(), methodInfo->shortName(), args.toString());
                callExp = ln::format(U"({0}::{1}({2}));", typeInfo->fullName(), methodInfo->shortName(), args.toString());
            }
            else {
                ln::String castTo = typeInfo->fullName();
                if (methodInfo->isConst()) castTo = U"const " + castTo;
                callExp = ln::format(U"(reinterpret_cast<{0}*>({1})->{2}({3}));", castTo, typeInfo->shortName().toLower(), methodInfo->shortName(), args.toString());
            }
        }
        else {
            // static 関数
            if (methodInfo->isStatic()) {
                //body.append("({0}::{1}({2}));", typeInfo->fullName(), methodInfo->shortName(), args.toString());
                callExp = ln::format(U"({0}::{1}({2}));", typeInfo->fullName(), methodInfo->shortName(), args.toString());
            }
            // コンストラクタ 関数
            else if (methodInfo->isConstructor()) {
                OutputBuffer macroArgs;
                macroArgs.AppendCommad(U"out" + typeInfo->shortName()); // 格納する変数名
                macroArgs.AppendCommad(makeWrapSubclassName(typeInfo)); // クラス名
                macroArgs.AppendCommad(methodInfo->shortName());        // 初期化関数名
                macroArgs.AppendCommad(args.toString());                // 引数
                callExp = ln::format(U"LNI_CREATE_OBJECT({0});", macroArgs.toString());
            }
            // 普通のインスタンス 関数
            else {
                auto name = methodInfo->shortName();
                if (methodInfo->isVirtual())
                    name = (makeWrapSubclassName(typeInfo) + U"::" + name + U"_CallBase");
                callExp = ln::format(U"(LNI_HANDLE_TO_OBJECT({0}, {1})->{2}({3}));", makeWrapSubclassName(typeInfo), FlatCCommon::makeInstanceParamName(typeInfo), name, args.toString());
            }
        }

        if (!returnAssignmentExpr.isEmpty()) {
            body.AppendLine(U"if (outReturn) {");
            body.IncreaseIndent();
            body.AppendLine(returnAssignmentExpr.toString() + callExp);
            body.DecreaseIndent();
            body.AppendLine(U"}");
            body.AppendLine(U"else {");
            body.IncreaseIndent();
            body.AppendLine(callExp);
            body.DecreaseIndent();
            body.AppendLine(U"}");
        }
        else {
            body.append(callExp);
        }
    }

    OutputBuffer funcImpl;
    funcImpl.AppendLine(funcHeader);
    funcImpl.AppendLine(U"{");
    funcImpl.IncreaseIndent();
    funcImpl.AppendLine(U"LNI_FUNC_TRY_BEGIN;");
    funcImpl.AppendLine(body.toString());
    funcImpl.AppendLine(U"LNI_FUNC_TRY_END_RETURN;");
    funcImpl.DecreaseIndent();
    funcImpl.AppendLine(U"}");
    return funcImpl.toString();
}

ln::String FlatCSourceGenerator::makeBaseCallerExpr(const TypeSymbol* classSymbol, const MethodSymbol* methodSymbol) const {
    OutputBuffer argList;
    for (auto& param : methodSymbol->parameters()) {
        argList.AppendCommad(param->name());
    }
    return ln::format(U"{0}{1}::{2}({3});", (methodSymbol->returnType().type == PredefinedTypes::voidType) ? U"" : U"return ", classSymbol->fullName(), methodSymbol->shortName(), argList.toString());
}

//ln::String FlatCSourceGenerator::makeCharsetWrapperFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo, FlatCharset charset)
//{
//	OutputBuffer args;
//    ln::String returnDecl;
//    ln::String postCallExpr;
//	for (auto& param : methodInfo->flatParameters()) {
//        if (param->isReturn() && param->type()->isString()) {
//            returnDecl = U"const LNChar* _ret;";
//            args.AppendCommad(U"&_ret");
//            postCallExpr = U"";
//        }
//		if (!param->isReturn() &&   //
//            param->type()->isString()) {
//			args.AppendCommad(U"ln::String::fromCString({0}, -1, ln::TextEncoding::utf8Encoding()).c_str()", param->name());
//		}
//		else {
//			args.AppendCommad(param->name());
//		}
//	}
//
//	auto callExpr = ln::String::format(U"LNResult result = {0}({1});", makeFlatFullFuncName(methodInfo, FlatCharset::Unicode), args.toString());
//
//	OutputBuffer code;
//	code.AppendLine(makeFuncHeader(methodInfo, charset));
//	code.AppendLine(U"{");
//	code.IncreaseIndent();
//	code.AppendLine(callExpr);
//	code.AppendLine(U"return result;");
//	code.DecreaseIndent();
//	code.AppendLine(U"}");
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
//		params.AppendCommad(U"{0} e", param->getFullQualTypeName(), param->name());
//		if (param->type()->isClass()) {
//			args.AppendCommad(U"LNI_OBJECT_TO_HANDLE(e)");
//		}
//		else {
//			args.AppendCommad(U"e");
//		}
//	}
//	auto callbackLambda = ln::String::format(U"[=]({0}) {{ handler({1}); }}", params.toString(), args.toString());
//
//	OutputBuffer funcImpl;
//	funcImpl.AppendLine(makeFuncHeader(methodInfo, FlatCharset::Unicode));
//	funcImpl.AppendLine(U"{");
//	funcImpl.IncreaseIndent();
//	funcImpl.AppendLine(U"LNI_FUNC_TRY_BEGIN;");
//	funcImpl.AppendLine(U"(LNI_HANDLE_TO_OBJECT({0}, {1})->{2}({3}));", makeWrapSubclassName(classInfo), methodInfo->flatParameters()[0]->name(), methodInfo->shortName(), callbackLambda);
//	funcImpl.AppendLine(U"LNI_FUNC_TRY_END_RETURN;");
//	funcImpl.DecreaseIndent();
//	funcImpl.AppendLine(U"}");
//	return funcImpl.toString();
//}
