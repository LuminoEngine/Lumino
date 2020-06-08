
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
    *GetPtr_%%Type%%(pdat) = *((%%Type%%*)(in));
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

static void hsp%%Type%%_reffunc(HspVarProc* p)
{
    static %%Type%% returnValue;

    %%MemeberInitializer%%

    *retValPtr = &returnValue;
    *typeRes = hsp%%Type%%_typeid();
    return true;
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
    code.AppendLines(makeStructStorageCores());
    code.AppendLines(make_cmdfunc());

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

        initializer.AppendLine(u"if (CheclDefault()) {");
        initializer.IncreaseIndent();

        initializer.DecreaseIndent();
        initializer.AppendLine(u"} else {");
        initializer.IncreaseIndent();

        initializer.DecreaseIndent();
        initializer.AppendLine(u"}");


        if (CheclDefault()) {
            memset(&returnValue, 0, sizeof(returnValue));
        }
        else {
        }
        // デフォルトの場合の初期化式
        string defaultExp = "memset(&returnValue, 0, sizeof(returnValue));";
        if (originalName == "LNMatrix")
            defaultExp = "LNMatrix_Identity(&returnValue);";   // 行列の場合は単位行列にする

        // 各メンバ代入式
        OutputBuffer initExp = new OutputBuffer(2);
        foreach(var member in classType.StructData.Members)
        {
            initExp.AppendLine("returnValue.{0} = {1};", member.Name, "GetParamDouble()");
        }


        code.AppendLines(StructStorageCoreTemplate.replace(u"%%Type%%", makeFlatTypeName2(structSymbol)));
    }
    
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
            //auto itr = m_commandIdMap.find(methodSymbol.get());
            //if (itr == m_commandIdMap.end()) {
            //    LN_ERROR();    // ID が見つからないのはおかしい
            //    return ln::String::Empty;
            //}

            code.AppendLine(u"// " + makeFlatFullFuncName(methodSymbol, FlatCharset::Ascii));
            code.AppendLine(u"case 0x{0:X} : {{", getMethodId(methodSymbol));
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
    callExpr = "stat = " + makeFlatFullFuncName(methodSymbol, FlatCharset::Ascii) + "(" + args.toString() + ");";

    code.AppendLine(callExpr);
    code.AppendLines(epilogue.toString().trim());
    return code.toString();
}

ln::String HSPCommandsGenerator::makeGetVAExpr(const MethodParameterSymbol* paramSymbol) const
{
    const auto localName = ln::String::format(u"local_" + paramSymbol->name());

    //static ln::Ref<TypeSymbol>    voidType;
    //static ln::Ref<TypeSymbol>    nullptrType;
    //static ln::Ref<TypeSymbol>    boolType;
    //static ln::Ref<TypeSymbol>    intType;
    //static ln::Ref<TypeSymbol>    int16Type;
    //static ln::Ref<TypeSymbol>    uint32Type;
    //static ln::Ref<TypeSymbol>    floatType;
    //static ln::Ref<TypeSymbol>    doubleType;
    //static ln::Ref<TypeSymbol>    stringType;
    //static ln::Ref<TypeSymbol>    stringRefType;
    if (paramSymbol->type() == PredefinedTypes::boolType ||
        paramSymbol->type() == PredefinedTypes::intType ||
        paramSymbol->type() == PredefinedTypes::int16Type ||
        paramSymbol->type() == PredefinedTypes::uint32Type ||
        paramSymbol->type()->isClass()) {
        return ln::String::format(u"const int {0} = fetchVAInt();", localName);
    }
    if (paramSymbol->type() == PredefinedTypes::floatType ||
        paramSymbol->type() == PredefinedTypes::doubleType ||
        paramSymbol->type()->isClass()) {
        return ln::String::format(u"const double {0} = fetchVADouble();", localName);
    }
    if (paramSymbol->type() == PredefinedTypes::stringType ||
        paramSymbol->type() == PredefinedTypes::stringRefType ||
        paramSymbol->type()->isClass()) {
        return ln::String::format(u"const std::string {0} = fetchVAString();", localName);
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
    if (paramSymbol->type() == PredefinedTypes::stringType ||
        paramSymbol->type() == PredefinedTypes::stringRefType ||
        paramSymbol->type()->isClass()) {
        return ln::String::format(u"code_setva({0}, {1}, HSPVAR_FLAG_STR, {2}.c_str());", pvalName, aptrName, localName);
    }

    return u"??";
}
