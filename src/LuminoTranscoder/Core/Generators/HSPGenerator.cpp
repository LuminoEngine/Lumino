
#include "HSPGenerator.hpp"

//==============================================================================
// HSPGeneratorBase


//==============================================================================
// HSPHeaderGenerator

static const ln::String ASFileTemplate = uR"(
#ifndef __lumino__
#define __lumino__

#regcmd "_hsp3cmdinit@4","LuminoHSP.dll", 6

%%Contents%%

#endif // __lumino__
)";

void HSPHeaderGenerator::generate()
{
    OutputBuffer code;
    code.AppendLines(makeEnums());
    code.AppendLines(makeStructs());
    code.AppendLines(makeClasses());

    // save
    {
        auto outputDir = ln::Path(makeOutputFilePath(u"HSP", u""));
        ln::FileSystem::createDirectory(outputDir);

        ln::String fileName = ln::String::format("{0}.as", config()->moduleName);

        ln::String src = ASFileTemplate.replace(u"%%Contents%%", code.toString());

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

ln::String HSPHeaderGenerator::makeStructs() const
{
    OutputBuffer code;
    for (const auto& structSymbol : db()->structs()) {
        code.AppendLine("#cmd {0} ${1:X}", makeFlatTypeName2(structSymbol), structSymbol->symbolId());
    }
    return code.toString();
}

ln::String HSPHeaderGenerator::makeClasses() const
{
    OutputBuffer code;
    for (const auto& classSymbol : db()->classes()) {
        for (const auto& methodSymbol : classSymbol->publicMethods()) {
            code.AppendLine("#cmd {0} ${1:X}", makeFlatFullFuncName(methodSymbol, FlatCharset::Ascii), methodSymbol->symbolId());
        }
    }
    return code.toString();
}

//==============================================================================
// HSPCommandsGenerator

static const ln::String StructStorageCoreTemplate = uR"(
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



void HSPCommandsGenerator::generate()
{
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
    code.AppendLine(u"#include \"LuminoHSP.h\"");
    code.AppendLines(makeStructStorageCores());
    code.AppendLines(makeSubclassDefines());
    code.AppendLines(make_reffunc());
    code.AppendLines(make_cmdfunc());
    code.AppendLines(makeRegisterTypeFunc());

    
    // save
    {
        auto outputDir = ln::Path(makeOutputFilePath(u"HSP", u""));
        ln::FileSystem::createDirectory(outputDir);

        ln::String fileName = ln::String::format("{0}.HSPCommands.generated.cpp", config()->moduleName);

        ln::FileSystem::writeAllText(ln::Path(outputDir, fileName), code.toString());
    }
}

// Ruby のように、Allocator などを定義していく
ln::String HSPCommandsGenerator::makeStructStorageCores() const
{
    OutputBuffer code;

    for (const auto& structSymbol : db()->structs()) {

        OutputBuffer initializer;
        if (structSymbol->fullName() == u"ln::Matrix") {
            // フィールドに float を使うか Vector4 を使うかちょっと揺れてるので、直接書き込む
            static const ln::String MatrixInitializerTemplate = uR"(
if (checkAndFetchDefault()) {
    LnMatrix_SetZeros(&returnValue);
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
            initializer.AppendLine(u"if (checkAndFetchDefault()) {");
            initializer.IncreaseIndent();
            {
                //// 関数形式の初期化で、引数が省略されている場合
                //if (structSymbol->fullName() == u"ln::Matrix") {
                //    initializer.AppendLine(u"LnMatrix_SetZeros(&returnValue);");   // 行列の場合は単位行列にする TODO: Reset みたいな共通の名前の初期化関数作った方がいいかも
                //}
                //else {
                    initializer.AppendLine(u"memset(&returnValue, 0, sizeof(returnValue));");
                //}
            }
            initializer.DecreaseIndent();
            initializer.AppendLine(u"} else {");
            initializer.IncreaseIndent();
            {
                // 各メンバ代入式
                for (const auto& member : structSymbol->fields()) {
                    initializer.AppendLine(u"returnValue.{0} = {1};", member->name(), makeFetchVAExpr(member->type(), true));
                }
            }
            initializer.DecreaseIndent();
            initializer.AppendLine(u"}");
            initializer.DecreaseIndent();
        }

        code.AppendLines(StructStorageCoreTemplate
            .replace(u"%%Type%%", makeFlatTypeName2(structSymbol))
            .replace(u"%%MemeberInitializer%%", initializer.toString()));
    }
    
    return code.toString();
}

ln::String HSPCommandsGenerator::makeRegisterTypeFunc() const
{
    OutputBuffer code;
    code.AppendLine(u"void RegisterTypes(HSP3TYPEINFO * info)");
    code.AppendLine(u"{");
    code.IncreaseIndent();
    {
        for (const auto& structSymbol : db()->structs()) {
            code.AppendLine(u"registvar(-1, hsp{0}_Init);", makeFlatTypeName2(structSymbol));
        }

        for (const auto& classSymbol : db()->classes()) {
            if (classSymbol->isStatic()) {

            }
            else {
                code.AppendLine(u"{");
                code.IncreaseIndent();
                {
                    code.AppendLine(u"{0} info = {{}};", makeFlatAPIName_SubclassRegistrationInfo(classSymbol));
                    code.AppendLine(u"info.subinstanceAllocFunc = {0};", makeName_SubinstanceAlloc(classSymbol));
                    code.AppendLine(u"info.subinstanceFreeFunc = {0};", makeName_SubinstanceFree(classSymbol));

                    for (const auto& methodSymbol : classSymbol->virtualMethods()) {
                        code.AppendLine(u"//info.{0} = {1};", makeFlatAPIName_OverrideFunc(methodSymbol, FlatCharset::Ascii), u"????");
                    }

                    code.AppendLine(u"{0}(&info);", makeFlatAPIName_RegisterSubclassTypeInfo(classSymbol));
                }
                code.DecreaseIndent();
                code.AppendLine(u"}");
            }
        }
    }
    code.DecreaseIndent();
    code.AppendLine(u"}");
    return code.toString();
}

ln::String HSPCommandsGenerator::makeSubclassDefines() const
{
    static const ln::String SubclassCommonTemplate = uR"(
static LnSubinstanceId HSPSubclass_%%FlatClassName%%_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_%%FlatClassName%%)));
}

static void HSPSubclass_%%FlatClassName%%_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}
)";

    OutputBuffer code;

    for (const auto& classSymbol : db()->classes()) {
        code.AppendLine(u"//==============================================================================");
        code.AppendLine(u"// " + classSymbol->fullName()).NewLine();

        code.AppendLine(u"struct HSPSubclass_" + makeFlatClassName(classSymbol));
        code.AppendLine(u"{");
        code.IncreaseIndent();
        {
            if (classSymbol->isDelegateObject()) {
                code.AppendLine(u"unsigned short* labelPointer = nullptr;");
            }
        }
        code.DecreaseIndent();
        code.AppendLine(u"};").NewLine();

        code.AppendLines(SubclassCommonTemplate.replace(u"%%FlatClassName%%", makeFlatClassName(classSymbol))).NewLine();

        // DelegateLabelCaller
        if (classSymbol->isDelegateObject()) {
            //OutputBuffer paramList;
            //for (const auto& param : classSymbol->delegateProtoType()->flatParameters()) {

            //}
            const auto selfParamName = classSymbol->delegateProtoType()->flatThisParam()->name();

            code.AppendLine(u"static LnResult {0}({1})", makeName_DelegateLabelCaller(classSymbol), makeFlatParamList(classSymbol->delegateProtoType(), FlatCharset::Ascii));
            code.AppendLine(u"{");
            code.IncreaseIndent();
            {
                code.AppendLine(u"auto* self = reinterpret_cast<{0}*>({1}({2}));", makeName_HSPSubclassType(classSymbol), makeFlatAPIName_GetSubinstanceId(classSymbol), selfParamName);
                code.AppendLine(u"stat = 0;");
                code.AppendLine(u"code_call(self->labelPointer);");
                code.AppendLine(u"return static_cast<LnResult>(stat);");
            }
            code.DecreaseIndent();
            code.AppendLine(u"}").NewLine();
        }
    }

    return code.toString().trim();
}

ln::String HSPCommandsGenerator::make_reffunc() const
{
    OutputBuffer code;
    code.AppendLine(u"bool Structs_reffunc(int cmd, int* typeRes, void** retValPtr)");
    code.AppendLine(u"{");
    code.IncreaseIndent();
    {
        code.AppendLine(u"g_leadSupport = false;");
        code.AppendLine(u"switch (cmd) {");
        code.IncreaseIndent();
        {
            for (const auto& structSymbol : db()->structs()) {
                code.AppendLine(u"// " + makeFlatTypeName2(structSymbol));
                code.AppendLine(u"case 0x{0:X} : {{", structSymbol->symbolId());
                code.IncreaseIndent();

                code.AppendLines(u"hsp{0}_reffunc(typeRes, retValPtr);", makeFlatTypeName2(structSymbol));

                code.AppendLine(u"return true;");
                code.DecreaseIndent();
                code.AppendLine(u"}");
            }
        }
        code.DecreaseIndent();
        code.AppendLine(u"}");
        code.AppendLine(u"return false;");
    }
    code.DecreaseIndent();
    code.AppendLine(u"}");
    return code.toString();
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
            code.AppendLine(u"// " + makeFlatFullFuncName(methodSymbol, FlatCharset::Ascii));
            code.AppendLine(u"case 0x{0:X} : {{", methodSymbol->symbolId());
            code.IncreaseIndent();

            code.AppendLines(makeCallCommandBlock(methodSymbol));
            
            code.AppendLine(u"return true;");
            code.DecreaseIndent();
            code.AppendLine(u"}");
        }
    }

    code.DecreaseIndent();
    code.AppendLine(u"}");
    code.AppendLine(u"return false;");
    code.DecreaseIndent();
    code.AppendLine(u"}");
    return code.toString();
}

ln::String HSPCommandsGenerator::makeCallCommandBlock(const MethodSymbol* methodSymbol) const
{
    OutputBuffer prologue;
    OutputBuffer args;
    OutputBuffer epilogue;

    const TypeSymbol* classSymbol = methodSymbol->ownerType();

    for (const auto& param : methodSymbol->flatParameters()) {
        auto localVarName = u"local_" + param->name();
        prologue.AppendLine(u"// Fetch " + param->name());

        if (classSymbol->isDelegateObject() && methodSymbol->isConstructor() && param->type()->isFunction() && param->isIn()) {

            prologue.AppendLine(makeGetVAExpr(param));
            args.AppendCommad(makeName_DelegateLabelCaller(classSymbol));
            epilogue.AppendLine(u"auto* self = reinterpret_cast<{0}*>({1}(local_{2}));", makeName_HSPSubclassType(classSymbol), makeFlatAPIName_GetSubinstanceId(classSymbol), methodSymbol->flatConstructorOutputThisParam()->name());
            epilogue.AppendLine(u"self->labelPointer = {0};", localVarName);
        }
        else if (param->isOut() || param->isReturn()) {
            prologue.AppendLine(u"PVal* pval_{0};", param->name());
            prologue.AppendLine(u"const APTR aptr_{0} = code_getva(&pval_{0});", param->name());
            prologue.AppendLine(u"{0} {1};", makeFlatTypeName2(param->type()), localVarName);


            args.AppendCommad(u"&" + localVarName);
            epilogue.AppendLine(makeSetVAExpr(param));
        }
        else if (param->type()->isStruct()) {
            prologue.AppendLine(u"PVal* pval_{0};", param->name());
            prologue.AppendLine(u"CodeGetVA_TypeChecked(&pval_{0}, {1});", param->name(), makeFlatTypeName2(param->type()));
            args.AppendCommad(u"reinterpret_cast<const {0}*>(pval_{1}->pt)", makeFlatTypeName2(param->type()), param->name());
        }
        else {
            prologue.AppendLine(makeGetVAExpr(param));
            args.AppendCommad(localVarName);
        }

    }

    OutputBuffer code;
    code.AppendLines(prologue.toString());

    ln::String callExpr;
    callExpr = "stat = " + makeFlatFullFuncName(methodSymbol, FlatCharset::Ascii) + "(" + args.toString() + ");";

    code.AppendLine(callExpr);
    code.AppendLines(epilogue.toString().trim());
    return code.toString();
}

ln::String HSPCommandsGenerator::makeFetchVAExpr(const TypeSymbol* typeSymbol, bool reffunc) const
{
    const ln::Char* postfix = (reffunc) ? u"_reffunc" : u"";

    if (typeSymbol == PredefinedTypes::boolType) {
        return ln::String::format(u"static_cast<LnBool>(fetchVAInt{0}())", postfix);
    }
    if (typeSymbol == PredefinedTypes::boolType ||
        typeSymbol == PredefinedTypes::intType ||
        typeSymbol == PredefinedTypes::int16Type ||
        typeSymbol == PredefinedTypes::uint32Type ||
        typeSymbol->isClass()) {
        return ln::String::format(u"fetchVAInt{0}()", postfix);
    }
    if (typeSymbol->isEnum()) {
        return ln::String::format(u"static_cast<{0}>(fetchVAInt{1}())", makeFlatTypeName2(typeSymbol), postfix);
    }
    if (typeSymbol == PredefinedTypes::floatType ||
        typeSymbol == PredefinedTypes::doubleType) {
        return ln::String::format(u"fetchVADouble{0}()", postfix);
    }
    if (typeSymbol == PredefinedTypes::stringType ||
        typeSymbol == PredefinedTypes::stringRefType) {
        return ln::String::format(u"fetchVAString{0}()", postfix);
    }
    if (typeSymbol->isFunction()) {
        return ln::String::format(u"fetchVALabelPointer{0}()", postfix);
    }
    return u"????";
}

ln::String HSPCommandsGenerator::makeGetVAExpr(const MethodParameterSymbol* paramSymbol) const
{
    const auto localName = ln::String::format(u"local_" + paramSymbol->name());
    return ln::String::format(u"const auto {0} = {1};", localName, makeFetchVAExpr(paramSymbol->type(), false));
}

ln::String HSPCommandsGenerator::makeSetVAExpr(const MethodParameterSymbol* paramSymbol) const
{
    const TypeSymbol* typeSymbol = paramSymbol->type();
    const auto pvalName = ln::String::format(u"pval_" + paramSymbol->name());
    const auto aptrName = ln::String::format(u"aptr_" + paramSymbol->name());
    const auto localName = ln::String::format(u"local_" + paramSymbol->name());

    if (typeSymbol == PredefinedTypes::boolType ||
        typeSymbol == PredefinedTypes::intType ||
        typeSymbol == PredefinedTypes::int16Type ||
        typeSymbol == PredefinedTypes::uint32Type ||
        typeSymbol->isClass() ||
        typeSymbol->isEnum()) {
        return ln::String::format(u"setVAInt({0}, {1}, {2});", pvalName, aptrName, localName);
    }
    if (typeSymbol == PredefinedTypes::floatType ||
        typeSymbol == PredefinedTypes::doubleType) {
        return ln::String::format(u"setVADouble({0}, {1}, {2});", pvalName, aptrName, localName);
    }
    if (typeSymbol == PredefinedTypes::stringType ||
        typeSymbol == PredefinedTypes::stringRefType ||
        typeSymbol->isClass()) {
        return ln::String::format(u"setVAStr({0}, {1}, {2});", pvalName, aptrName, localName);
    }
    if (typeSymbol->isStruct()) {
        return ln::String::format(u"code_setva({0}, {1}, hsp{2}_typeid(), &{3});", pvalName, aptrName, makeFlatTypeName2(typeSymbol), localName);
    }

    return u"??";
}
