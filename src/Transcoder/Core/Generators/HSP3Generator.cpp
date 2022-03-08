
#include "HSP3Generator.hpp"

// Delegate のハンドルを指定するところに直接ラベルを指定できるようにする (Delegate は使用不可能)
static const bool LabelSyntax = true;

//==============================================================================
// HSP3GeneratorBase

//ln::String HSP3GeneratorBase::makeFlatConstantValue(const ConstantSymbol* constant) const
//{
//    if (constant->type()->isEnum()) {
//        return ln::String::format(U"({0}){1}", makeFlatClassName(constant->type()), ln::String::fromNumber(constant->value()->get<int>()));
//    }
//    else if (constant->type() == PredefinedTypes::boolType) {
//        return constant->value()->get<bool>() ? U"LN_TRUE" : U"LN_FALSE";
//    }
//    else if (constant->type() == PredefinedTypes::floatType) {
//        return ln::String::fromNumber(constant->value()->get<float>());
//    }
//    else if (constant->type() == PredefinedTypes::nullptrType) {
//        return U"LN_NULL_HANDLE";
//    }
//    else {
//        return ln::String::fromNumber(constant->value()->get<int>());
//    }
//}

//==============================================================================
// HSP3HeaderGenerator

static const ln::String ASFileTemplate = UR"(
#ifndef __lumino__
#define __lumino__

#ifdef LUMINO_DEBUG
    #regcmd "_hsp3cmdinit@4","LuminoHSP3d.dll", %%varhpi%%
#else
    #regcmd "_hsp3cmdinit@4","LuminoHSP3.dll", %%varhpi%%
#endif

#const global LN_TRUE 1
#const global LN_FALSE 0
#const global LN_NULL_HANDLE 0
_ln_return_discard = 0

%%Contents%%

#cmd ln_args $1
#cmd ln_set_args $2

#define LUMINO_APP _ln_local_app = LN_NULL_HANDLE :\
    LNApplication_Create _ln_local_app :\
    LNApplication_SetPrototype_OnInit _ln_local_app, *on_init :\
    LNApplication_SetPrototype_OnUpdate _ln_local_app, *on_update :\
    LNApplication_Run _ln_local_app :\
    end

#endif // __lumino__
)";

void HSP3HeaderGenerator::generate() {
    OutputBuffer code;
    code.setNewLineCode(U"\r\n");
    code.AppendLines(makeEnums());
    code.AppendLines(makeStructs());
    code.AppendLines(makeClasses());

    // save
    {
        auto outputDir = ln::Path(makeOutputFilePath(U"HSP3", U""));
        ln::FileSystem::createDirectory(outputDir);

        ln::String fileName = ln::format(U"{0}.as", config()->moduleName).toLower();

        ln::String src = ASFileTemplate
                             .replace(U"%%Contents%%", code.toString())
                             .replace(U"%%varhpi%%", ln::String::fromNumber(db()->structs().length()));

        ln::FileSystem::writeAllText(ln::Path(outputDir, fileName), src);
    }
}

ln::String HSP3HeaderGenerator::makeEnums() const {
    OutputBuffer code;
    for (const auto& enumSymbol : db()->enums()) {
        for (const auto& member : enumSymbol->constants()) {
            code.AppendLine(U"#const global {0} {1}", makeFlatEnumMemberName(enumSymbol, member), member->value()->get<int>());
        }
    }
    return code.toString();
}

ln::String HSP3HeaderGenerator::makeStructs() const {
    OutputBuffer code;
    for (const auto& structSymbol : db()->structs()) {
        code.AppendLine(U"#cmd {0} ${1:X}", makeFlatTypeName2(structSymbol), getCommandId(structSymbol));

        for (const auto& methodSymbol : structSymbol->publicMethods()) {
            code.AppendLine(U"#cmd {0} ${1:X}", makeFlatFullFuncName(methodSymbol, FlatCharset::Unicode), getCommandId(methodSymbol));
        }
    }
    return code.toString();
}

ln::String HSP3HeaderGenerator::makeClasses() const {
    OutputBuffer code;
    for (const auto& classSymbol : db()->classes()) {
        for (const auto& methodSymbol : classSymbol->publicMethods()) {
            if (methodSymbol->isFieldAccessor()) {
                // 個別フィールドアクセスはひとまず見送り。HSP3 だと冗長になりすぎる。まとめて取得する Get だけで十分だろう。
            }
            else {
                const auto funcName = makeFlatFullFuncName(methodSymbol, FlatCharset::Unicode);
                code.AppendLine(U"#cmd _{0} ${1:X}", funcName, getCommandId(methodSymbol));

                // Engine::update やイベントの connect など、戻り値が不要な場合は省略できるようにしたい。
                // ただ #cmd では Val 型の省略ができないため、#define でラップすることで対策する。
                OutputBuffer paramsText;
                OutputBuffer argsText;
                const auto& flatParams = methodSymbol->flatParameters();
                for (int i = 0; i < flatParams.size(); i++) {
                    const auto& param = flatParams[i];
                    if (param->defaultValue())
                        paramsText.AppendCommad(U"%{0}={1}", i + 1, makeFlatConstantValue(param->defaultValue()));
                    else if (param->isReturn())
                        paramsText.AppendCommad(U"%{0}=_ln_return_discard", i + 1);
                    else
                        paramsText.AppendCommad(U"%{0}", i + 1);

                    argsText.AppendCommad(U"%{0}", i + 1);
                }

                if (paramsText.isEmpty())
                    code.AppendLine(U"#define {0} _{0}", funcName, paramsText.toString());
                else
                    code.AppendLine(U"#define {0}({1}) _{0} {2}", funcName, paramsText.toString(), argsText.toString());
            }
        }

        const auto virtualMethods = classSymbol->virtualPrototypeSetters();
        for (int i = 0; i < virtualMethods.size(); i++) {
            code.AppendLine(U"#cmd {0} ${1:X}", makeFlatFullFuncName(virtualMethods[i], FlatCharset::Unicode), getCommandId(virtualMethods[i]));
        }
    }
    return code.toString();
}

//==============================================================================
// HSP3CommandsGenerator

static const ln::String StructStorageCoreTemplate = UR"(
//-----------------------------------------------------------------------------
// struct %%Type%%

static int g_%%Type%%_typeid = 0;

int hsp%%Type%%_typeid()
{
    return g_%%Type%%_typeid;
}

static int hsp%%Type%%_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(%%Type%%);
    return size;
}

static void hsp%%Type%%_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hsp%%Type%%_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    %%Type%%* fv = (%%Type%% *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hsp%%Type%%_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hsp%%Type%%_GetPtr(PVal* pval)
{
    return (PDAT*)(((%%Type%%*)(pval->pt)) + pval->offset);
}

static int hsp%%Type%%_GetSize(const PDAT *pdatl)
{
    return sizeof(%%Type%%);
}

static void hsp%%Type%%_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<%%Type%%*>(pdat) = *reinterpret_cast<const %%Type%%*>(in);
}

static void hsp%%Type%%_Init(HspVarProc* p)
{
    p->Alloc = hsp%%Type%%_Alloc;
    p->Free = hsp%%Type%%_Free;
    
    p->GetPtr = hsp%%Type%%_GetPtr;
    p->GetSize = hsp%%Type%%_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hsp%%Type%%_Set;

    p->vartype_name = "%%Type%%";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(%%Type%%);
    g_%%Type%%_typeid = p->flag;
}

static void hsp%%Type%%_reffunc(int* typeRes, void** retValPtr)
{
    static %%Type%% returnValue;

%%MemeberInitializer%%

    *retValPtr = &returnValue;
    *typeRes = hsp%%Type%%_typeid();
}
)";

void HSP3CommandsGenerator::generate() {
    // Setup command map
    //for (const auto& classSymbol : db()->classes()) {
    //    for (const auto& methodSymbol : classSymbol->publicMethods()) {
    //        auto itr = m_commandIdMap.find(methodSymbol);
    //        if (itr != m_commandIdMap.end()) {
    //            LN_ERROR();    // 重複？何かおかしい
    //            return;
    //        }
    //        else {
    //            m_commandIdMap.insert({ methodSymbol.get(), m_commandIdMap.size() + 0x0064 });
    //        }
    //    }
    //}

    OutputBuffer code;
    code.AppendLine(U"#include \"LuminoHSP.h\"");
    code.AppendLines(makeStructStorageCores());
    code.AppendLines(makeSubclassDefines());
    code.AppendLines(make_reffunc());
    code.AppendLines(make_cmdfunc());
    code.AppendLines(makeRegisterTypeFunc());

    // save
    {
        auto outputDir = ln::Path(makeOutputFilePath(U"HSP3", U""));
        ln::FileSystem::createDirectory(outputDir);

        ln::String fileName = ln::format(U"{0}.HSPCommands.generated.cpp", config()->moduleName);

        ln::FileSystem::writeAllText(ln::Path(outputDir, fileName), code.toString());
    }
}

// Ruby のように、Allocator などを定義していく
ln::String HSP3CommandsGenerator::makeStructStorageCores() const {
    OutputBuffer code;

    for (const auto& structSymbol : db()->structs()) {

        OutputBuffer initializer;
        if (structSymbol->fullName() == U"ln::Matrix") {
            // フィールドに float を使うか Vector4 を使うかちょっと揺れてるので、直接書き込む
            static const ln::String MatrixInitializerTemplate = UR"(
if (checkAndFetchDefault()) {
    LNMatrix_SetZeros(&returnValue);
}
else {
    returnValue.row0.x = fetchVADouble_reffunc();
    returnValue.row0.y = fetchVADouble_reffunc();
    returnValue.row0.z = fetchVADouble_reffunc();
    returnValue.row0.w = fetchVADouble_reffunc();
    returnValue.row1.x = fetchVADouble_reffunc();
    returnValue.row1.y = fetchVADouble_reffunc();
    returnValue.row1.z = fetchVADouble_reffunc();
    returnValue.row1.w = fetchVADouble_reffunc();
    returnValue.row2.x = fetchVADouble_reffunc();
    returnValue.row2.y = fetchVADouble_reffunc();
    returnValue.row2.z = fetchVADouble_reffunc();
    returnValue.row2.w = fetchVADouble_reffunc();
    returnValue.row3.x = fetchVADouble_reffunc();
    returnValue.row3.y = fetchVADouble_reffunc();
    returnValue.row3.z = fetchVADouble_reffunc();
    returnValue.row3.w = fetchVADouble_reffunc();
}
)";
            initializer.IncreaseIndent();
            initializer.AppendLines(MatrixInitializerTemplate);
            initializer.DecreaseIndent();
        }
        else {
            initializer.IncreaseIndent();
            initializer.AppendLine(U"if (checkAndFetchDefault()) {");
            initializer.IncreaseIndent();
            {
                //// 関数形式の初期化で、引数が省略されている場合
                //if (structSymbol->fullName() == U"ln::Matrix") {
                //    initializer.AppendLine(U"LNMatrix_SetZeros(&returnValue);");   // 行列の場合は単位行列にする TODO: Reset みたいな共通の名前の初期化関数作った方がいいかも
                //}
                //else {
                initializer.AppendLine(U"memset(&returnValue, 0, sizeof(returnValue));");
                //}
            }
            initializer.DecreaseIndent();
            initializer.AppendLine(U"} else {");
            initializer.IncreaseIndent();
            {
                // 各メンバ代入式
                for (const auto& member : structSymbol->fields()) {
                    initializer.AppendLine(U"returnValue.{0} = {1};", member->name(), makeFetchVAExpr(member->type(), true));
                }
            }
            initializer.DecreaseIndent();
            initializer.AppendLine(U"}");
            initializer.DecreaseIndent();
        }

        code.AppendLines(StructStorageCoreTemplate
                             .replace(U"%%Type%%", makeFlatTypeName2(structSymbol))
                             .replace(U"%%MemeberInitializer%%", initializer.toString()));
    }

    return code.toString();
}

ln::String HSP3CommandsGenerator::makeRegisterTypeFunc() const {
    OutputBuffer code;
    code.AppendLine(U"void RegisterTypes(HSP3TYPEINFO * info)");
    code.AppendLine(U"{");
    code.IncreaseIndent();
    {
        for (const auto& structSymbol : db()->structs()) {
            code.AppendLine(U"registvar(-1, hsp{0}_Init);", makeFlatTypeName2(structSymbol));
        }

        for (const auto& classSymbol : db()->classes()) {
            if (classSymbol->isStatic()) {
            }
            else {
                code.AppendLine(U"{");
                code.IncreaseIndent();
                {
                    code.AppendLine(U"{0} info = {{}};", makeFlatAPIName_SubclassRegistrationInfo(classSymbol));
                    code.AppendLine(U"info.subinstanceAllocFunc = {0};", makeName_SubinstanceAlloc(classSymbol));
                    code.AppendLine(U"info.subinstanceFreeFunc = {0};", makeName_SubinstanceFree(classSymbol));

                    for (const auto& methodSymbol : classSymbol->leafVirtualMethods()) {
                        code.AppendLine(U"//info.{0} = {1};", makeFlatAPIName_OverrideFunc(methodSymbol, FlatCharset::Ascii), U"????");
                    }

                    code.AppendLine(U"{0}(&info);", makeFlatAPIName_RegisterSubclassTypeInfo(classSymbol));
                }
                code.DecreaseIndent();
                code.AppendLine(U"}");
            }
        }
    }
    code.DecreaseIndent();
    code.AppendLine(U"}");
    return code.toString();
}

ln::String HSP3CommandsGenerator::makeSubclassDefines() const {
    static const ln::String SubclassCommonTemplate = UR"(
static LNSubinstanceId HSPSubclass_%%FlatClassName%%_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_%%FlatClassName%%)));
}

static void HSPSubclass_%%FlatClassName%%_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}
)";

    OutputBuffer code;

    for (const auto& classSymbol : db()->classes()) {
        code.AppendLine(U"//==============================================================================");
        code.AppendLine(U"// " + classSymbol->fullName()).NewLine();

        code.AppendLine(U"struct HSPSubclass_" + makeFlatClassName(classSymbol));
        code.AppendLine(U"{");
        code.IncreaseIndent();
        {
            if (classSymbol->isDelegateObject()) {
                code.AppendLine(U"unsigned short* labelPointer = nullptr;");
            }
        }
        code.DecreaseIndent();
        code.AppendLine(U"};").NewLine();

        code.AppendLines(SubclassCommonTemplate.replace(U"%%FlatClassName%%", makeFlatClassName(classSymbol))).NewLine();

        // DelegateLabelCaller
        if (classSymbol->isDelegateObject()) {
            //OutputBuffer paramList;
            //for (const auto& param : classSymbol->delegateProtoType()->flatParameters()) {

            //}
            const auto selfParamName = classSymbol->delegateProtoType()->flatThisParam()->name();

            code.AppendLine(U"static LNResult {0}({1})", makeName_DelegateLabelCaller(classSymbol), makeFlatParamList(classSymbol->delegateProtoType(), FlatCharset::Ascii));
            code.AppendLine(U"{");
            code.IncreaseIndent();
            {
                const auto& params = classSymbol->delegateProtoType()->flatParameters();
                for (int i = 0; i < params.size(); i++) {
                    if (params[i]->isIn()) {
                        code.AppendLine(U"setCallbackArg({0}, {1});", i, params[i]->name());
                    }
                }

                code.AppendLine(U"auto* localSelf = reinterpret_cast<{0}*>({1}({2}));", makeName_HSPSubclassType(classSymbol), makeFlatAPIName_GetSubinstanceId(classSymbol), selfParamName);
                code.AppendLine(U"stat = 0;");
                code.AppendLine(U"code_call(localSelf->labelPointer);");

                for (int i = 0; i < params.size(); i++) {
                    if (params[i]->isOut()) {
                        code.AppendLine(U"setCallbackOutput({0}, {1});", i, params[i]->name());
                    }
                }

                code.AppendLine(U"return static_cast<LNResult>(stat);");
            }
            code.DecreaseIndent();
            code.AppendLine(U"}").NewLine();
        }
    }

    return code.toString().trim();
}

ln::String HSP3CommandsGenerator::make_reffunc() const {
    OutputBuffer code;
    code.AppendLine(U"bool Structs_reffunc(int cmd, int* typeRes, void** retValPtr)");
    code.AppendLine(U"{");
    code.IncreaseIndent();
    {
        code.AppendLine(U"g_leadSupport = false;");
        code.AppendLine(U"switch (cmd) {");
        code.IncreaseIndent();
        {
            for (const auto& structSymbol : db()->structs()) {
                code.AppendLine(U"// " + makeFlatTypeName2(structSymbol));
                code.AppendLine(U"case 0x{0:X} : {{", getCommandId(structSymbol));
                code.IncreaseIndent();

                code.AppendLines(U"hsp{0}_reffunc(typeRes, retValPtr);", makeFlatTypeName2(structSymbol));

                code.AppendLine(U"return true;");
                code.DecreaseIndent();
                code.AppendLine(U"}");
            }

            code.AppendLine(U"// ln_args");
            code.AppendLine(U"case 0x1 : {");
            code.IncreaseIndent();
            code.AppendLines(U"ln_args_reffunc(typeRes, retValPtr);");
            code.AppendLine(U"return true;");
            code.DecreaseIndent();
            code.AppendLine(U"}");
        }
        code.DecreaseIndent();
        code.AppendLine(U"}");
        code.AppendLine(U"return false;");
    }
    code.DecreaseIndent();
    code.AppendLine(U"}");
    return code.toString();
}

ln::String HSP3CommandsGenerator::make_cmdfunc() const {
    OutputBuffer code;
    code.AppendLine(U"bool Commands_cmdfunc(int cmd, int* retVal)");
    code.AppendLine(U"{");
    code.IncreaseIndent();
    code.AppendLine(U"*retVal = RUNMODE_RUN;");
    code.AppendLine(U"switch (cmd) {");
    code.IncreaseIndent();

    {
        code.AppendLine(U"// ln_set_args");
        code.AppendLine(U"case 0x2 : {");
        code.IncreaseIndent();
        code.AppendLines(U"ln_set_args_cmdfunc();");
        code.AppendLine(U"return true;");
        code.DecreaseIndent();
        code.AppendLine(U"}");
    }

    for (const auto& structSymbol : db()->structs()) {
        auto methods = structSymbol->publicMethods();
        for (const auto& methodSymbol : methods) {
            if (methodSymbol->isFieldAccessor()) {
                // 個別フィールドアクセスはひとまず見送り。HSP3 だと冗長になりすぎる。まとめて取得する Get だけで十分だろう。
            }
            else {
                code.AppendLine(U"// " + makeFlatFullFuncName(methodSymbol, FlatCharset::Ascii));
                code.AppendLine(U"case 0x{0:X} : {{", getCommandId(methodSymbol));
                code.IncreaseIndent();

                code.AppendLines(makeCallCommandBlock(methodSymbol));

                code.AppendLine(U"return true;");
                code.DecreaseIndent();
                code.AppendLine(U"}");
            }
        }
    }

    for (const auto& classSymbol : db()->classes()) {
        auto methods = classSymbol->publicMethods();
        methods.addRange(classSymbol->virtualPrototypeSetters());
        for (const auto& methodSymbol : methods) {
            code.AppendLine(U"// " + makeFlatFullFuncName(methodSymbol, FlatCharset::Ascii));
            code.AppendLine(U"case 0x{0:X} : {{", getCommandId(methodSymbol));
            code.IncreaseIndent();

            code.AppendLines(makeCallCommandBlock(methodSymbol));

            code.AppendLine(U"return true;");
            code.DecreaseIndent();
            code.AppendLine(U"}");
        }

        //const auto virtualMethods = classSymbol->virtualMethods();
        //for (int i = 0; i < virtualMethods.size(); i++) {
        //    const auto& methodSymbol = virtualMethods[i];
        //    code.AppendLine(U"// " + makeFlatAPIName_SetPrototype(classSymbol, virtualMethods[i], FlatCharset::Ascii));
        //    code.AppendLine(U"case 0x{0:X} : {{", getCommandId(methodSymbol, 1 + i));
        //    code.IncreaseIndent();

        //    code.AppendLine(U"return true;");
        //    code.DecreaseIndent();
        //    code.AppendLine(U"}");
        //}
    }

    code.DecreaseIndent();
    code.AppendLine(U"}");
    code.AppendLine(U"return false;");
    code.DecreaseIndent();
    code.AppendLine(U"}");
    return code.toString();
}

ln::String HSP3CommandsGenerator::makeCallCommandBlock(const MethodSymbol* methodSymbol) const {
    OutputBuffer prologue;
    OutputBuffer args;
    OutputBuffer epilogue;

    const TypeSymbol* classSymbol = methodSymbol->ownerType();

    for (const auto& param : methodSymbol->flatParameters()) {
        auto localVarName = U"local_" + param->name();
        prologue.AppendLine(U"// Fetch " + param->name());

        if (LabelSyntax && !classSymbol->isDelegateObject() && param->type()->isDelegateObject() && param->isIn()) {
            prologue.AppendLine(makeGetVAExpr(param));

            // Create temp delegate
            {
                auto localDelegateName = U"localDelegate_" + param->name();
                prologue.NewLine();
                prologue.AppendLine(U"LNHandle {0};", localDelegateName);
                prologue.AppendLine(U"{");
                prologue.IncreaseIndent();
                prologue.AppendLine(U"stat = {0}({1}, &{2});", makeFlatFullFuncName(param->type()->publicMethods()[0], FlatCharset::Ascii), makeName_DelegateLabelCaller(param->type()), localDelegateName);
                prologue.AppendLine(U"if (stat != LN_OK) return true;");
                prologue.AppendLine(U"auto* localSelf = reinterpret_cast<{0}*>({1}({2}));", makeName_HSPSubclassType(param->type()), makeFlatAPIName_GetSubinstanceId(param->type()), localDelegateName);
                prologue.AppendLine(U"localSelf->labelPointer = {0};", localVarName);
                prologue.DecreaseIndent();
                prologue.AppendLine(U"}");

                args.AppendCommad(localDelegateName);

                epilogue.AppendLine(U"LNObject_Release({0});", localDelegateName);
            }
        }
        else if (classSymbol->isDelegateObject() && methodSymbol->isConstructor() && param->type()->isFunction() && param->isIn()) {
            prologue.AppendLine(makeGetVAExpr(param));
            args.AppendCommad(makeName_DelegateLabelCaller(classSymbol));
            epilogue.AppendLine(U"auto* localSelf = reinterpret_cast<{0}*>({1}(local_{2}));", makeName_HSPSubclassType(classSymbol), makeFlatAPIName_GetSubinstanceId(classSymbol), methodSymbol->flatConstructorOutputThisParam()->name());
            epilogue.AppendLine(U"localSelf->labelPointer = {0};", localVarName);
        }
        else if (param->isOut() || param->isReturn()) {
            prologue.AppendLine(U"PVal* pval_{0};", param->name());
            prologue.AppendLine(U"const APTR aptr_{0} = code_getva(&pval_{0});", param->name());
            prologue.AppendLine(U"{0} {1};", makeFlatTypeName2(param->type()), localVarName);

            args.AppendCommad(U"&" + localVarName);
            epilogue.AppendLine(makeSetVAExpr(param));
        }
        else if (param->type()->isStruct()) {
            prologue.AppendLine(U"PVal* pval_{0};", param->name());
            prologue.AppendLine(U"CodeGetVA_TypeChecked(&pval_{0}, {1});", param->name(), makeFlatTypeName2(param->type()));

            ln::String mod = methodSymbol->isConst() ? U"const " : U"";
            args.AppendCommad(U"reinterpret_cast<{0}{1}*>(pval_{2}->pt)", mod, makeFlatTypeName2(param->type()), param->name());
        }
        else {
            prologue.AppendLine(makeGetVAExpr(param));
            args.AppendCommad(U"static_cast<{0}>({1})", makeFlatCParamQualTypeName(methodSymbol, param, FlatCharset::Ascii), localVarName);
        }
    }

    OutputBuffer code;
    code.AppendLines(prologue.toString());

    ln::String callExpr;
    callExpr = U"stat = " + makeFlatFullFuncName(methodSymbol, FlatCharset::Ascii) + U"(" + args.toString() + U");";

    code.AppendLine(callExpr);
    code.AppendLines(epilogue.toString().trim());

    // LNEngine_Update は、await を実行して HSP3 ランタイムのメッセージ処理を行うようにする。
    // ※こうしておかないと、HSP3 が作ったメインウィンドウがクローズできない。
    //   ユーザープログラムで await してもらえばいいのだが、ちょっと煩わしい。hgimg のようにしておきたい。
    if (classSymbol->shortName() == U"Engine" && methodSymbol->shortName() == U"update") {
        code.AppendLine(U"ctx->waittick = 0;");
        code.AppendLine(U"*retVal = RUNMODE_AWAIT;");
    }

    return code.toString();
}

ln::String HSP3CommandsGenerator::makeFetchVAExpr(const TypeSymbol* typeSymbol, bool reffunc, const ConstantSymbol* defaultValue) const {
    const ln::Char* postfix = (reffunc) ? U"_reffunc" : U"";
    const auto defaultValueStr = (defaultValue) ? makeFlatConstantValue(defaultValue) : ln::String();

    if (LabelSyntax) {
        if (typeSymbol->isDelegateObject()) {
            return ln::format(U"fetchVALabelPointer{0}()", postfix);
        }
    }

    if (typeSymbol == PredefinedTypes::boolType) {
        return ln::format(U"static_cast<LNBool>(fetchVAInt{0}())", postfix);
    }
    if (typeSymbol == PredefinedTypes::boolType ||
        typeSymbol == PredefinedTypes::intType ||
        typeSymbol == PredefinedTypes::int16Type ||
        typeSymbol == PredefinedTypes::uint32Type ||
        typeSymbol == PredefinedTypes::intptrType ||
        typeSymbol->isClass()) {
        return ln::format(U"fetchVAInt{0}({1})", postfix, defaultValueStr);
    }
    if (typeSymbol->isEnum()) {
        return ln::format(U"static_cast<{0}>(fetchVAInt{1}({2}))", makeFlatTypeName2(typeSymbol), postfix, defaultValueStr);
    }
    if (typeSymbol == PredefinedTypes::floatType ||
        typeSymbol == PredefinedTypes::doubleType) {
        return ln::format(U"fetchVADouble{0}({1})", postfix, defaultValueStr);
    }
    if (typeSymbol == PredefinedTypes::stringType ||
        typeSymbol == PredefinedTypes::StringViewType) {
        return ln::format(U"fetchVAString{0}()", postfix);
    }
    if (typeSymbol->isFunction()) {
        return ln::format(U"fetchVALabelPointer{0}()", postfix);
    }
    return U"????";
}

ln::String HSP3CommandsGenerator::makeGetVAExpr(const MethodParameterSymbol* paramSymbol) const {
    const auto localName = ln::format(U"local_" + paramSymbol->name());
    return ln::format(U"const auto {0} = {1};", localName, makeFetchVAExpr(paramSymbol->type(), false, paramSymbol->defaultValue()));
}

ln::String HSP3CommandsGenerator::makeSetVAExpr(const MethodParameterSymbol* paramSymbol) const {
    const TypeSymbol* typeSymbol = paramSymbol->type();
    const auto pvalName = ln::format(U"pval_" + paramSymbol->name());
    const auto aptrName = ln::format(U"aptr_" + paramSymbol->name());
    const auto localName = ln::format(U"local_" + paramSymbol->name());

    if (typeSymbol == PredefinedTypes::boolType ||
        typeSymbol == PredefinedTypes::intType ||
        typeSymbol == PredefinedTypes::int16Type ||
        typeSymbol == PredefinedTypes::uint32Type ||
        typeSymbol->isClass() ||
        typeSymbol->isEnum()) {
        return ln::format(U"setVAInt({0}, {1}, {2});", pvalName, aptrName, localName);
    }
    if (typeSymbol == PredefinedTypes::floatType ||
        typeSymbol == PredefinedTypes::doubleType) {
        return ln::format(U"setVADouble({0}, {1}, {2});", pvalName, aptrName, localName);
    }
    if (typeSymbol == PredefinedTypes::stringType ||
        typeSymbol == PredefinedTypes::StringViewType ||
        typeSymbol->isClass()) {
        return ln::format(U"setVAStr({0}, {1}, {2});", pvalName, aptrName, localName);
    }
    if (typeSymbol->isStruct()) {
        return ln::format(U"code_setva({0}, {1}, hsp{2}_typeid(), &{3});", pvalName, aptrName, makeFlatTypeName2(typeSymbol), localName);
    }

    return U"??";
}

//==============================================================================
// HSP3HelpGenerator

static const ln::String HeaderTemplate = UR"(
;============================================================
; Lumino ヘルプファイル
;============================================================

%dll
Lumino

%ver
_LIBRARY_VERSION_

%date
_BUILD_TIME_

%author
lriki

%note
lumino.as をインクルードしてください

%type
拡張命令

%url
http ://nnmy.sakura.ne.jp/

)";

static const ln::String FuncTemplate = UR"(
;------------------------------------------------------------
;
;------------------------------------------------------------
%index
_NAME_
_BRIEF_
%group
_GROUP_
%prm
_PRM_LIST_
_PRM_DETAIL_
%inst
_INST_
%href
_HREF_
)";

void HSP3HelpGenerator::generate() {
    OutputBuffer code;
    code.setNewLineCode(U"\r\n");

    // Header
    {
        std::time_t t = std::time(0); // get time now
        std::tm* now = std::localtime(&t);
        auto text = HeaderTemplate
                        .replace(U"_LIBRARY_VERSION_", config()->versionString)
                        .replace(U"_BUILD_TIME_", ln::format(U"{0}/{1}/{2}", (now->tm_year + 1900), (now->tm_mon + 1), now->tm_mday));
        code.AppendLines(text);
    }

    // Structs
    for (const auto& structSymbol : db()->structs()) {
        for (auto& methodSymbol : structSymbol->publicMethods()) {
            if (methodSymbol->isFieldAccessor()) {
                // 個別フィールドアクセスはひとまず見送り。HSP3 だと冗長になりすぎる。まとめて取得する Get だけで十分だろう。
            }
            else {
                code.AppendLines(makeFuncDocument(methodSymbol));
            }
        }
    }

    // Classes
    for (const auto& classSymbol : db()->classes()) {
        for (auto& methodSymbol : classSymbol->publicMethods()) {
            code.AppendLines(makeFuncDocument(methodSymbol));
        }
    }

    // save
    {
        auto outputDir = ln::Path(makeOutputFilePath(U"HSP3", U""));
        ln::FileSystem::createDirectory(outputDir);

        ln::String fileName = ln::format(U"{0}.hs", config()->moduleName).toLower();

        ln::FileSystem::writeAllText(ln::Path(outputDir, fileName), code.toString(), ln::TextEncoding::getEncoding(ln::EncodingType::SJIS));
    }
}

ln::String HSP3HelpGenerator::makeFuncDocument(const MethodSymbol* methodSymbol) const {
    // 引数リスト
    OutputBuffer params;
    for (const auto& param : methodSymbol->flatParameters()) {
        params.AppendCommad(param->name());
    }

    // インデント量の計算
    // [out] pp   : 説明
    // [in]  name : 説明
    // ^^^^^        … ioColumnWidth
    //       ^^^^   … nameColumnWidth
    int ioColumnWidth = 0;
    int nameColumnWidth = 0;
    for (const auto& param : methodSymbol->flatParameters()) {
        // 名前部分はデフォルト引数も含んだ長さで考える
        int nameLen = param->name().length();
        if (param->hasDefaultValue())
            nameLen += makeFlatConstantValue(param->defaultValue()).length() + 2; // +2 は () の分

        ioColumnWidth = std::max(ioColumnWidth, makeIOName(param).length());
        nameColumnWidth = std::max(nameColumnWidth, nameLen);
    }
    ioColumnWidth += 2; // 前後の [ ] の分

    // 引数の1行説明
    OutputBuffer paramsDetailText;
    for (const auto& param : methodSymbol->flatParameters()) {
        auto name = param->name();

        // デフォルト値がある場合は () を付けて表現
        // また outReturn も省略可能とする
        if (param->hasDefaultValue())
            name += U"(" + makeFlatConstantValue(param->defaultValue()) + U")";
        else if (param->isReturn()) {
            name += U"(0)";
        }

        paramsDetailText.append(ln::format(U"{0,-" + ln::String::fromNumber(ioColumnWidth) + U"}", U"[" + makeIOName(param) + U"]"));
        paramsDetailText.append(ln::format(U" {0,-" + ln::String::fromNumber(nameColumnWidth) + U"}", name));
        paramsDetailText.append(U" : ");
        if (param->type()->isStruct()) {
            paramsDetailText.append(U"({0} 型の値) ", param->type()->shortName());
        }
        paramsDetailText.append(translateComment(methodSymbol->document()->flatParams()[param->flatParamIndex()]->description()));
        paramsDetailText.NewLine();

        // enum 型の場合は候補値も追加しておく
        const auto& enumType = param->type();
        if (enumType->isEnum()) {
            const auto indent = ln::format(U" {0,-" + ln::String::fromNumber(ioColumnWidth + 3 + nameColumnWidth + 3) + U"}", U" ");

            for (const auto& member : enumType->constants()) {
                paramsDetailText.AppendLine(indent + makeFlatEnumMemberName(enumType, member));
                paramsDetailText.AppendLine(indent + member->document()->summary());
            }
        }
    }

    paramsDetailText.NewLine();
    paramsDetailText.AppendLine(U"stat : 0=成功, 負値=失敗");

    OutputBuffer detailText;
    detailText.AppendLines(methodSymbol->document()->details());
    if (TypeSymbol* baseClass = methodSymbol->ownerType()->baseClass()) {
        detailText.NewLine();
        detailText.AppendLine(U"備考");
        detailText.AppendLine(U"--------------------");

        TypeSymbol* thisClass = methodSymbol->ownerType();
        while (baseClass) {
            detailText.AppendLine(U"{0} は {1} のサブクラスです。{0} ハンドルは {1} ハンドルとして扱うことができ、 {2}_ から始まる命令等で使用できます。", thisClass->shortName(), baseClass->shortName(), makeFlatClassName(baseClass));
            baseClass = baseClass->baseClass();
        }
    }

    return FuncTemplate
        .replace(U"_NAME_", makeFlatFullFuncName(methodSymbol, FlatCharset::Unicode)) // FlatCharset::Unicode を指定して、"A" をつけないようにする
        .replace(U"_BRIEF_", translateComment(methodSymbol->document()->summary()))
        .replace(U"_INST_", translateComment(detailText.toString()))
        .replace(U"_HREF_", U"")
        .replace(U"_GROUP_", makeFlatClassName(methodSymbol->ownerType()))
        .replace(U"_PRM_LIST_", params.toString())
        .replace(U"_PRM_DETAIL_", paramsDetailText.toString());

    // サンプルコード
    //TestCode sampleCode;
    //if (_sampleCodeMap.TryGetValue(func.CPPName, out sampleCode) &&
    //    !sampleCode.IsSkipTest)
    //{
    //    funcText += "\n%sample\n" + sampleCode.ExtractedCode + "\n";
    //}
}

ln::String HSP3HelpGenerator::makeIOName(const MethodParameterSymbol* paramSymbol) const {
    if (paramSymbol->isOut())
        return U"out";
    else
        return U"in";
}

ln::String HSP3HelpGenerator::translateComment(const ln::String& text) const {
    auto result = text.replace(U"関数", U"命令")
                      .replace(U"のポインタ", U"");
    //.replace("クラス", "モジュール")

    return result;

    //string doc = "";
    //string[] lines = text.Replace("\r", "").Split('\n');
    //foreach(var line in lines)
    //    doc += line.Trim() + OutputBuffer.NewLineCode;

    //return doc;
}

#if 0

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    /// <summary>
    /// .hs 生成
    /// </summary>
    class HSPHelpBuilder : Builder
    {
/// <summary>
/// プロパティ 通知
/// </summary>
/// <param name="enumType"></param>
protected override void OnPropertyLooked(CLProperty prop)
{
    // プロパティとして割り振られたメソッドは OnMethodLooked() ではなくこの OnPropertyLooked() にくる。
    // やりたいことは OnMethodLooked() と同じなのでそちらにまわす。
    if (prop.Getter != null) OnMethodLooked(prop.Getter);
    if (prop.Setter != null) OnMethodLooked(prop.Setter);
}

/// <summary>
/// メソッド 通知
/// </summary>
/// <param name="enumType"></param>
protected override void OnMethodLooked(CLMethod method)
{
    if (!Context.CheckEnabled(method)) return;

    // 関数
    string allFuncText = "";

    // 基本
    string funcText = FuncTemplate.TrimStart();
    funcText = funcText.Replace("_NAME_", method.FuncDecl.OriginalFullName);
    funcText = funcText.Replace("_BRIEF_", TranslateComment(method.Document.OriginalBriefText));
    funcText = funcText.Replace("_INST_", TranslateComment(method.Document.OriginalDetailsText));
    funcText = funcText.Replace("_HREF_", "");
    funcText = funcText.Replace("_GROUP_", method.OwnerClass.OwnerModule.Document.OriginalGroup);

    // 引数リスト
    var paramsText = new OutputBuffer();
    foreach(var param in method.FuncDecl.Params)
    {
        paramsText.AppendCommad(param.Name);
    }
    if (paramsText.IsEmpty) paramsText.Append("　"); // 空なら全角スペースでごまかす
    funcText = funcText.Replace("_PRM_LIST_", paramsText.ToString());

    // インデント量の計算
    // [out] pp   : 説明
    // [in]  name : 説明
    // ^^^^^        … ioColumnWidth
    //       ^^^^   … nameColumnWidth
    int ioColumnWidth = 0;
    int nameColumnWidth = 0;
    foreach(var param in method.FuncDecl.Params)
    {
        // 名前部分はデフォルト引数も含んだ長さで考える
        int nameLen = param.Name.Length;
        if (!string.IsNullOrEmpty(param.OriginalDefaultValue))
            nameLen += param.OriginalDefaultValue.Length + 2;   // +2 は () の分

        ioColumnWidth = Math.Max(ioColumnWidth, GetIOName(param.IOModifier).Length);
        nameColumnWidth = Math.Max(nameColumnWidth, nameLen);
    }
    ioColumnWidth += 2; // 前後の [ ] の分

    // 引数の1行説明
    string detailText = "";
    foreach(var param in method.FuncDecl.Params)
    {
        string name = param.Name;

        // デフォルト値がある場合は () を付けて表現
        if (!string.IsNullOrEmpty(param.OriginalDefaultValue))
            name += "(" + param.OriginalDefaultValue + ")";

        detailText += string.Format("{0,-" + ioColumnWidth + "}", "[" + GetIOName(param.IOModifier) + "]");
        detailText += string.Format(" {0,-" + nameColumnWidth + "}", name);
        detailText += " : ";
        detailText += TranslateComment(param.Document.OriginalText);
        //detailText += "\n";

        // enum 型の場合は候補値も追加しておく
        CLEnum enumType = param.Type as CLEnum;
        if (enumType != null)
        {
            string indent = string.Format(" {0,-" + (ioColumnWidth + 3 + nameColumnWidth + 3) + "}", " ");

            foreach(var member in enumType.Members)
            {
                if (member.IsTerminator) continue;  // ターミネータは追加しない
                detailText += indent;
                detailText += member.OriginalName + OutputBuffer.NewLineCode;
                detailText += indent + "    ";
                detailText += member.Comment + OutputBuffer.NewLineCode;
            }
        }
    }

    // 戻り値 (全て stat 扱い)
    if (method.FuncDecl.ReturnType != null && method.FuncDecl.ReturnType.IsResultCodeType)
    {
        if (method.FuncDecl.OriginalFullName == "LNError_GetLastErrorCode")
        {
            detailText += OutputBuffer.NewLineCode;
            detailText += "stat : エラーコード" + OutputBuffer.NewLineCode;

            CLEnum enumType = (CLEnum)method.FuncDecl.ReturnType;
            foreach(var member in enumType.Members)
            {
                if (member.IsTerminator) continue;  // ターミネータは追加しない
                detailText += "    ";
                detailText += member.OriginalName + OutputBuffer.NewLineCode;
                detailText += "        ";
                detailText += member.Comment + OutputBuffer.NewLineCode;
            }

            detailText += OutputBuffer.NewLineCode;
        }
        else
        {
            detailText += OutputBuffer.NewLineCode;
            detailText += "stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)";
            detailText += OutputBuffer.NewLineCode;
        }
    }

    funcText = funcText.Replace("_PRM_DETAIL_", detailText);

    // サンプルコード
    //TestCode sampleCode;
    //if (_sampleCodeMap.TryGetValue(func.CPPName, out sampleCode) &&
    //    !sampleCode.IsSkipTest)
    //{
    //    funcText += "\n%sample\n" + sampleCode.ExtractedCode + "\n";
    //}

    allFuncText += funcText;
    _output.AppendWithIndent(allFuncText);
}

/// <summary>
/// ファイルに出力するための最終文字列を生成する
/// </summary>
protected override string OnMakeOutoutFileText()
{
    return _output.ToString();
}

/// <summary>
/// 出力ファイルのエンコーディング
/// </summary>
protected override Encoding GetOutputEncoding() { return Encoding.GetEncoding(932); }




}


/// <summary>
/// 命令一覧
/// </summary>
class HSPFuncListBuilder : Builder
{
    private struct DocLine
    {
        public string Name;
        public string Brief;
    }

    private List<DocLine> _oneClass;
    private OutputBuffer _output = new OutputBuffer();

    /// <summary>
    /// ビルド開始前(初期化)通知
    /// </summary>
    /// <param name="enumType"></param>
    protected override void OnInitialize()
    {
        _output.AppendLine("===============================================================================");
        _output.AppendLine(" Lumino HSP 命令一覧");
        _output.AppendLine("-------------------------------------------------------------------------------");
        _output.AppendLine("  このファイルは、本ライブラリがどのような機能を持っているかを示す概要です。");
        _output.AppendLine("  個々の命令の詳しい説明は、同梱の lumino.hs を HSP インストールフォルダ内の");
        _output.AppendLine("  「hsphelp」フォルダにコピーし、エディタから F1 キーヘルプにて参照してください。");
        _output.AppendLine("===============================================================================");
    }

    /// <summary>
    /// クラスor構造体 通知 (開始)
    /// </summary>
    /// <param name="classType"></param>
    /// <returns>false の場合このクラスの出力を無視する</returns>
    protected override bool OnClassLookedStart(CLClass classType)
    {
        _oneClass = new List<DocLine>();

        if (classType.IsStruct)
        {
            _oneClass.Add(
                new DocLine()
                {
                    Name = classType.OriginalName + "()",
                    Brief = classType.OriginalName + "型変数の作成" + OutputBuffer.NewLineCode
                });
        }

        return true;
    }

    /// <summary>
    /// クラスor構造体 通知 (終了)
    /// </summary>
    /// <param name="enumType"></param>
    protected override void OnClassLookedEnd(CLClass classType)
    {
        _output.AppendLine("-------------------------------------------------------------------------------");
        _output.AppendLine(" ■ [{0}]  {1}", classType.OriginalName, HSPHelpBuilder.TranslateComment(classType.Document.OriginalBriefText));

        // 関数名カラムの最大文字数を求める
        int col1Len = 0;
        foreach(var line in _oneClass)
        {
            col1Len = Math.Max(col1Len, line.Name.Length);
        }

        // インデントを考慮し、テキストを作る
        foreach(var line in _oneClass)
        {
            _output.Append(string.Format("{0,-" + col1Len + "}  ", line.Name));
            _output.Append(line.Brief);
        }
        _output.NewLine();
    }

    /// <summary>
    /// プロパティ 通知
    /// </summary>
    /// <param name="enumType"></param>
    protected override void OnPropertyLooked(CLProperty prop)
    {
        if (prop.Setter != null) OnMethodLooked(prop.Setter);
        if (prop.Getter != null) OnMethodLooked(prop.Getter);
    }

    /// <summary>
    /// メソッド 通知 (プロパティや internal は通知されない)
    /// </summary>
    /// <param name="enumType"></param>
    protected override void OnMethodLooked(CLMethod method)
    {
        // あとでインデント量を調整するため、まずはテキストを覚えておく
        _oneClass.Add(
            new DocLine()
            {
                Name = method.FuncDecl.OriginalFullName,
                Brief = HSPHelpBuilder.TranslateComment(method.Document.OriginalBriefText)
            });
    }

    /// <summary>
    /// ファイルに出力するための最終文字列を生成する
    /// </summary>
    protected override string OnMakeOutoutFileText()
    {
        return _output.ToString();
    }
}
}

#endif
