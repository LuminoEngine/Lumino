#include "LuminoHSP.h"

//-----------------------------------------------------------------------------
// struct LnVector3

static int g_LnVector3_typeid = 0;

int hspLnVector3_typeid()
{
    return g_LnVector3_typeid;
}

static int hspLnVector3_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LnVector3);
    return size;
}

static void hspLnVector3_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLnVector3_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LnVector3* fv = (LnVector3 *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLnVector3_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLnVector3_GetPtr(PVal* pval)
{
    return (PDAT*)(((LnVector3*)(pval->pt)) + pval->offset);
}

static int hspLnVector3_GetSize(const PDAT *pdatl)
{
    return sizeof(LnVector3);
}

static void hspLnVector3_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LnVector3*>(pdat) = *reinterpret_cast<const LnVector3*>(in);
}

static void hspLnVector3_Init(HspVarProc* p)
{
    p->Alloc = hspLnVector3_Alloc;
    p->Free = hspLnVector3_Free;
    
    p->GetPtr = hspLnVector3_GetPtr;
    p->GetSize = hspLnVector3_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLnVector3_Set;

    p->vartype_name = "LnVector3";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LnVector3);
    g_LnVector3_typeid = p->flag;
}

static void hspLnVector3_reffunc(int* typeRes, void** retValPtr)
{
    static LnVector3 returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.x = fetchVADouble_reffunc();
        returnValue.y = fetchVADouble_reffunc();
        returnValue.z = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLnVector3_typeid();
}


//-----------------------------------------------------------------------------
// struct LnVector4

static int g_LnVector4_typeid = 0;

int hspLnVector4_typeid()
{
    return g_LnVector4_typeid;
}

static int hspLnVector4_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LnVector4);
    return size;
}

static void hspLnVector4_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLnVector4_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LnVector4* fv = (LnVector4 *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLnVector4_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLnVector4_GetPtr(PVal* pval)
{
    return (PDAT*)(((LnVector4*)(pval->pt)) + pval->offset);
}

static int hspLnVector4_GetSize(const PDAT *pdatl)
{
    return sizeof(LnVector4);
}

static void hspLnVector4_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LnVector4*>(pdat) = *reinterpret_cast<const LnVector4*>(in);
}

static void hspLnVector4_Init(HspVarProc* p)
{
    p->Alloc = hspLnVector4_Alloc;
    p->Free = hspLnVector4_Free;
    
    p->GetPtr = hspLnVector4_GetPtr;
    p->GetSize = hspLnVector4_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLnVector4_Set;

    p->vartype_name = "LnVector4";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LnVector4);
    g_LnVector4_typeid = p->flag;
}

static void hspLnVector4_reffunc(int* typeRes, void** retValPtr)
{
    static LnVector4 returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.x = fetchVADouble_reffunc();
        returnValue.y = fetchVADouble_reffunc();
        returnValue.z = fetchVADouble_reffunc();
        returnValue.w = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLnVector4_typeid();
}


//-----------------------------------------------------------------------------
// struct LnQuaternion

static int g_LnQuaternion_typeid = 0;

int hspLnQuaternion_typeid()
{
    return g_LnQuaternion_typeid;
}

static int hspLnQuaternion_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LnQuaternion);
    return size;
}

static void hspLnQuaternion_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLnQuaternion_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LnQuaternion* fv = (LnQuaternion *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLnQuaternion_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLnQuaternion_GetPtr(PVal* pval)
{
    return (PDAT*)(((LnQuaternion*)(pval->pt)) + pval->offset);
}

static int hspLnQuaternion_GetSize(const PDAT *pdatl)
{
    return sizeof(LnQuaternion);
}

static void hspLnQuaternion_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LnQuaternion*>(pdat) = *reinterpret_cast<const LnQuaternion*>(in);
}

static void hspLnQuaternion_Init(HspVarProc* p)
{
    p->Alloc = hspLnQuaternion_Alloc;
    p->Free = hspLnQuaternion_Free;
    
    p->GetPtr = hspLnQuaternion_GetPtr;
    p->GetSize = hspLnQuaternion_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLnQuaternion_Set;

    p->vartype_name = "LnQuaternion";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LnQuaternion);
    g_LnQuaternion_typeid = p->flag;
}

static void hspLnQuaternion_reffunc(int* typeRes, void** retValPtr)
{
    static LnQuaternion returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.x = fetchVADouble_reffunc();
        returnValue.y = fetchVADouble_reffunc();
        returnValue.z = fetchVADouble_reffunc();
        returnValue.w = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLnQuaternion_typeid();
}


//-----------------------------------------------------------------------------
// struct LnMatrix

static int g_LnMatrix_typeid = 0;

int hspLnMatrix_typeid()
{
    return g_LnMatrix_typeid;
}

static int hspLnMatrix_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LnMatrix);
    return size;
}

static void hspLnMatrix_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLnMatrix_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LnMatrix* fv = (LnMatrix *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLnMatrix_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLnMatrix_GetPtr(PVal* pval)
{
    return (PDAT*)(((LnMatrix*)(pval->pt)) + pval->offset);
}

static int hspLnMatrix_GetSize(const PDAT *pdatl)
{
    return sizeof(LnMatrix);
}

static void hspLnMatrix_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LnMatrix*>(pdat) = *reinterpret_cast<const LnMatrix*>(in);
}

static void hspLnMatrix_Init(HspVarProc* p)
{
    p->Alloc = hspLnMatrix_Alloc;
    p->Free = hspLnMatrix_Free;
    
    p->GetPtr = hspLnMatrix_GetPtr;
    p->GetSize = hspLnMatrix_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLnMatrix_Set;

    p->vartype_name = "LnMatrix";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LnMatrix);
    g_LnMatrix_typeid = p->flag;
}

static void hspLnMatrix_reffunc(int* typeRes, void** retValPtr)
{
    static LnMatrix returnValue;


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



    *retValPtr = &returnValue;
    *typeRes = hspLnMatrix_typeid();
}


//-----------------------------------------------------------------------------
// struct LnColor

static int g_LnColor_typeid = 0;

int hspLnColor_typeid()
{
    return g_LnColor_typeid;
}

static int hspLnColor_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LnColor);
    return size;
}

static void hspLnColor_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLnColor_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LnColor* fv = (LnColor *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLnColor_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLnColor_GetPtr(PVal* pval)
{
    return (PDAT*)(((LnColor*)(pval->pt)) + pval->offset);
}

static int hspLnColor_GetSize(const PDAT *pdatl)
{
    return sizeof(LnColor);
}

static void hspLnColor_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LnColor*>(pdat) = *reinterpret_cast<const LnColor*>(in);
}

static void hspLnColor_Init(HspVarProc* p)
{
    p->Alloc = hspLnColor_Alloc;
    p->Free = hspLnColor_Free;
    
    p->GetPtr = hspLnColor_GetPtr;
    p->GetSize = hspLnColor_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLnColor_Set;

    p->vartype_name = "LnColor";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LnColor);
    g_LnColor_typeid = p->flag;
}

static void hspLnColor_reffunc(int* typeRes, void** retValPtr)
{
    static LnColor returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.r = fetchVADouble_reffunc();
        returnValue.g = fetchVADouble_reffunc();
        returnValue.b = fetchVADouble_reffunc();
        returnValue.a = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLnColor_typeid();
}


//-----------------------------------------------------------------------------
// struct LnPoint

static int g_LnPoint_typeid = 0;

int hspLnPoint_typeid()
{
    return g_LnPoint_typeid;
}

static int hspLnPoint_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LnPoint);
    return size;
}

static void hspLnPoint_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLnPoint_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LnPoint* fv = (LnPoint *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLnPoint_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLnPoint_GetPtr(PVal* pval)
{
    return (PDAT*)(((LnPoint*)(pval->pt)) + pval->offset);
}

static int hspLnPoint_GetSize(const PDAT *pdatl)
{
    return sizeof(LnPoint);
}

static void hspLnPoint_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LnPoint*>(pdat) = *reinterpret_cast<const LnPoint*>(in);
}

static void hspLnPoint_Init(HspVarProc* p)
{
    p->Alloc = hspLnPoint_Alloc;
    p->Free = hspLnPoint_Free;
    
    p->GetPtr = hspLnPoint_GetPtr;
    p->GetSize = hspLnPoint_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLnPoint_Set;

    p->vartype_name = "LnPoint";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LnPoint);
    g_LnPoint_typeid = p->flag;
}

static void hspLnPoint_reffunc(int* typeRes, void** retValPtr)
{
    static LnPoint returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.x = fetchVADouble_reffunc();
        returnValue.y = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLnPoint_typeid();
}


//-----------------------------------------------------------------------------
// struct LnSize

static int g_LnSize_typeid = 0;

int hspLnSize_typeid()
{
    return g_LnSize_typeid;
}

static int hspLnSize_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LnSize);
    return size;
}

static void hspLnSize_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLnSize_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LnSize* fv = (LnSize *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLnSize_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLnSize_GetPtr(PVal* pval)
{
    return (PDAT*)(((LnSize*)(pval->pt)) + pval->offset);
}

static int hspLnSize_GetSize(const PDAT *pdatl)
{
    return sizeof(LnSize);
}

static void hspLnSize_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LnSize*>(pdat) = *reinterpret_cast<const LnSize*>(in);
}

static void hspLnSize_Init(HspVarProc* p)
{
    p->Alloc = hspLnSize_Alloc;
    p->Free = hspLnSize_Free;
    
    p->GetPtr = hspLnSize_GetPtr;
    p->GetSize = hspLnSize_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLnSize_Set;

    p->vartype_name = "LnSize";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LnSize);
    g_LnSize_typeid = p->flag;
}

static void hspLnSize_reffunc(int* typeRes, void** retValPtr)
{
    static LnSize returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.width = fetchVADouble_reffunc();
        returnValue.height = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLnSize_typeid();
}


//-----------------------------------------------------------------------------
// struct LnRect

static int g_LnRect_typeid = 0;

int hspLnRect_typeid()
{
    return g_LnRect_typeid;
}

static int hspLnRect_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LnRect);
    return size;
}

static void hspLnRect_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLnRect_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LnRect* fv = (LnRect *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLnRect_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLnRect_GetPtr(PVal* pval)
{
    return (PDAT*)(((LnRect*)(pval->pt)) + pval->offset);
}

static int hspLnRect_GetSize(const PDAT *pdatl)
{
    return sizeof(LnRect);
}

static void hspLnRect_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LnRect*>(pdat) = *reinterpret_cast<const LnRect*>(in);
}

static void hspLnRect_Init(HspVarProc* p)
{
    p->Alloc = hspLnRect_Alloc;
    p->Free = hspLnRect_Free;
    
    p->GetPtr = hspLnRect_GetPtr;
    p->GetSize = hspLnRect_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLnRect_Set;

    p->vartype_name = "LnRect";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LnRect);
    g_LnRect_typeid = p->flag;
}

static void hspLnRect_reffunc(int* typeRes, void** retValPtr)
{
    static LnRect returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.x = fetchVADouble_reffunc();
        returnValue.y = fetchVADouble_reffunc();
        returnValue.width = fetchVADouble_reffunc();
        returnValue.height = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLnRect_typeid();
}


//-----------------------------------------------------------------------------
// struct LnThickness

static int g_LnThickness_typeid = 0;

int hspLnThickness_typeid()
{
    return g_LnThickness_typeid;
}

static int hspLnThickness_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LnThickness);
    return size;
}

static void hspLnThickness_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLnThickness_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LnThickness* fv = (LnThickness *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLnThickness_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLnThickness_GetPtr(PVal* pval)
{
    return (PDAT*)(((LnThickness*)(pval->pt)) + pval->offset);
}

static int hspLnThickness_GetSize(const PDAT *pdatl)
{
    return sizeof(LnThickness);
}

static void hspLnThickness_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LnThickness*>(pdat) = *reinterpret_cast<const LnThickness*>(in);
}

static void hspLnThickness_Init(HspVarProc* p)
{
    p->Alloc = hspLnThickness_Alloc;
    p->Free = hspLnThickness_Free;
    
    p->GetPtr = hspLnThickness_GetPtr;
    p->GetSize = hspLnThickness_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLnThickness_Set;

    p->vartype_name = "LnThickness";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LnThickness);
    g_LnThickness_typeid = p->flag;
}

static void hspLnThickness_reffunc(int* typeRes, void** retValPtr)
{
    static LnThickness returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.left = fetchVADouble_reffunc();
        returnValue.top = fetchVADouble_reffunc();
        returnValue.right = fetchVADouble_reffunc();
        returnValue.bottom = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLnThickness_typeid();
}


void RegisterStructTypes(HSP3TYPEINFO * info)
{
    registvar(-1, hspLnVector3_Init);
    registvar(-1, hspLnVector4_Init);
    registvar(-1, hspLnQuaternion_Init);
    registvar(-1, hspLnMatrix_Init);
    registvar(-1, hspLnColor_Init);
    registvar(-1, hspLnPoint_Init);
    registvar(-1, hspLnSize_Init);
    registvar(-1, hspLnRect_Init);
    registvar(-1, hspLnThickness_Init);
}

//==============================================================================
// ln::Object

struct HSPSubclass_LnObject
{
};


static void* HSPSubclass_LnObject_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnObject));
}

static void HSPSubclass_LnObject_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::EventConnection

struct HSPSubclass_LnEventConnection
{
};


static void* HSPSubclass_LnEventConnection_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnEventConnection));
}

static void HSPSubclass_LnEventConnection_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::PromiseFailureDelegate

struct HSPSubclass_LnPromiseFailureDelegate
{
    unsigned short* labelPointer = nullptr;
};


static void* HSPSubclass_LnPromiseFailureDelegate_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnPromiseFailureDelegate));
}

static void HSPSubclass_LnPromiseFailureDelegate_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


static LnResult HSPSubclass_LnPromiseFailureDelegate_DelegateLabelCaller(LnHandle promisefailuredelegate)
{
    auto* self = reinterpret_cast<HSPSubclass_LnPromiseFailureDelegate*>(LnPromiseFailureDelegate_GetSubinstanceId(promisefailuredelegate));
    stat = 0;
    code_call(self->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::ZVTestDelegate1

struct HSPSubclass_LnZVTestDelegate1
{
    unsigned short* labelPointer = nullptr;
};


static void* HSPSubclass_LnZVTestDelegate1_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnZVTestDelegate1));
}

static void HSPSubclass_LnZVTestDelegate1_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


static LnResult HSPSubclass_LnZVTestDelegate1_DelegateLabelCaller(LnHandle zvtestdelegate1, int p1)
{
    auto* self = reinterpret_cast<HSPSubclass_LnZVTestDelegate1*>(LnZVTestDelegate1_GetSubinstanceId(zvtestdelegate1));
    stat = 0;
    code_call(self->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::ZVTestDelegate2

struct HSPSubclass_LnZVTestDelegate2
{
    unsigned short* labelPointer = nullptr;
};


static void* HSPSubclass_LnZVTestDelegate2_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnZVTestDelegate2));
}

static void HSPSubclass_LnZVTestDelegate2_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


static LnResult HSPSubclass_LnZVTestDelegate2_DelegateLabelCaller(LnHandle zvtestdelegate2, int p1, int p2, int* outReturn)
{
    auto* self = reinterpret_cast<HSPSubclass_LnZVTestDelegate2*>(LnZVTestDelegate2_GetSubinstanceId(zvtestdelegate2));
    stat = 0;
    code_call(self->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::ZVTestDelegate3

struct HSPSubclass_LnZVTestDelegate3
{
    unsigned short* labelPointer = nullptr;
};


static void* HSPSubclass_LnZVTestDelegate3_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnZVTestDelegate3));
}

static void HSPSubclass_LnZVTestDelegate3_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


static LnResult HSPSubclass_LnZVTestDelegate3_DelegateLabelCaller(LnHandle zvtestdelegate3, LnHandle p1)
{
    auto* self = reinterpret_cast<HSPSubclass_LnZVTestDelegate3*>(LnZVTestDelegate3_GetSubinstanceId(zvtestdelegate3));
    stat = 0;
    code_call(self->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::ZVTestEventHandler1

struct HSPSubclass_LnZVTestEventHandler1
{
    unsigned short* labelPointer = nullptr;
};


static void* HSPSubclass_LnZVTestEventHandler1_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnZVTestEventHandler1));
}

static void HSPSubclass_LnZVTestEventHandler1_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


static LnResult HSPSubclass_LnZVTestEventHandler1_DelegateLabelCaller(LnHandle zvtesteventhandler1)
{
    auto* self = reinterpret_cast<HSPSubclass_LnZVTestEventHandler1*>(LnZVTestEventHandler1_GetSubinstanceId(zvtesteventhandler1));
    stat = 0;
    code_call(self->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::ZVTestEventHandler2

struct HSPSubclass_LnZVTestEventHandler2
{
    unsigned short* labelPointer = nullptr;
};


static void* HSPSubclass_LnZVTestEventHandler2_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnZVTestEventHandler2));
}

static void HSPSubclass_LnZVTestEventHandler2_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


static LnResult HSPSubclass_LnZVTestEventHandler2_DelegateLabelCaller(LnHandle zvtesteventhandler2, LnHandle p1)
{
    auto* self = reinterpret_cast<HSPSubclass_LnZVTestEventHandler2*>(LnZVTestEventHandler2_GetSubinstanceId(zvtesteventhandler2));
    stat = 0;
    code_call(self->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::ZVTestPromise1

struct HSPSubclass_LnZVTestPromise1
{
};


static void* HSPSubclass_LnZVTestPromise1_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnZVTestPromise1));
}

static void HSPSubclass_LnZVTestPromise1_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::ZVTestPromise2

struct HSPSubclass_LnZVTestPromise2
{
};


static void* HSPSubclass_LnZVTestPromise2_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnZVTestPromise2));
}

static void HSPSubclass_LnZVTestPromise2_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::ZVTestClass1

struct HSPSubclass_LnZVTestClass1
{
};


static void* HSPSubclass_LnZVTestClass1_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnZVTestClass1));
}

static void HSPSubclass_LnZVTestClass1_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::ZVTestEventArgs1

struct HSPSubclass_LnZVTestEventArgs1
{
};


static void* HSPSubclass_LnZVTestEventArgs1_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnZVTestEventArgs1));
}

static void HSPSubclass_LnZVTestEventArgs1_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::Serializer

struct HSPSubclass_LnSerializer
{
};


static void* HSPSubclass_LnSerializer_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnSerializer));
}

static void HSPSubclass_LnSerializer_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::Serializer2

struct HSPSubclass_LnSerializer2
{
};


static void* HSPSubclass_LnSerializer2_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnSerializer2));
}

static void HSPSubclass_LnSerializer2_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::AssetModel

struct HSPSubclass_LnAssetModel
{
};


static void* HSPSubclass_LnAssetModel_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnAssetModel));
}

static void HSPSubclass_LnAssetModel_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::Assets

struct HSPSubclass_LnAssets
{
};


static void* HSPSubclass_LnAssets_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnAssets));
}

static void HSPSubclass_LnAssets_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::GraphicsResource

struct HSPSubclass_LnGraphicsResource
{
};


static void* HSPSubclass_LnGraphicsResource_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnGraphicsResource));
}

static void HSPSubclass_LnGraphicsResource_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::Texture

struct HSPSubclass_LnTexture
{
};


static void* HSPSubclass_LnTexture_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnTexture));
}

static void HSPSubclass_LnTexture_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::Texture2D

struct HSPSubclass_LnTexture2D
{
};


static void* HSPSubclass_LnTexture2D_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnTexture2D));
}

static void HSPSubclass_LnTexture2D_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::RenderView

struct HSPSubclass_LnRenderView
{
};


static void* HSPSubclass_LnRenderView_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnRenderView));
}

static void HSPSubclass_LnRenderView_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::Component

struct HSPSubclass_LnComponent
{
};


static void* HSPSubclass_LnComponent_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnComponent));
}

static void HSPSubclass_LnComponent_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::VisualComponent

struct HSPSubclass_LnVisualComponent
{
};


static void* HSPSubclass_LnVisualComponent_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnVisualComponent));
}

static void HSPSubclass_LnVisualComponent_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::SpriteComponent

struct HSPSubclass_LnSpriteComponent
{
};


static void* HSPSubclass_LnSpriteComponent_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnSpriteComponent));
}

static void HSPSubclass_LnSpriteComponent_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::World

struct HSPSubclass_LnWorld
{
};


static void* HSPSubclass_LnWorld_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnWorld));
}

static void HSPSubclass_LnWorld_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::ComponentList

struct HSPSubclass_LnComponentList
{
};


static void* HSPSubclass_LnComponentList_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnComponentList));
}

static void HSPSubclass_LnComponentList_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::WorldObject

struct HSPSubclass_LnWorldObject
{
};


static void* HSPSubclass_LnWorldObject_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnWorldObject));
}

static void HSPSubclass_LnWorldObject_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::VisualObject

struct HSPSubclass_LnVisualObject
{
};


static void* HSPSubclass_LnVisualObject_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnVisualObject));
}

static void HSPSubclass_LnVisualObject_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::Camera

struct HSPSubclass_LnCamera
{
};


static void* HSPSubclass_LnCamera_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnCamera));
}

static void HSPSubclass_LnCamera_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::DirectionalLight

struct HSPSubclass_LnDirectionalLight
{
};


static void* HSPSubclass_LnDirectionalLight_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnDirectionalLight));
}

static void HSPSubclass_LnDirectionalLight_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::PointLight

struct HSPSubclass_LnPointLight
{
};


static void* HSPSubclass_LnPointLight_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnPointLight));
}

static void HSPSubclass_LnPointLight_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::SpotLight

struct HSPSubclass_LnSpotLight
{
};


static void* HSPSubclass_LnSpotLight_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnSpotLight));
}

static void HSPSubclass_LnSpotLight_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::TestDelegate

struct HSPSubclass_LnTestDelegate
{
    unsigned short* labelPointer = nullptr;
};


static void* HSPSubclass_LnTestDelegate_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnTestDelegate));
}

static void HSPSubclass_LnTestDelegate_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


static LnResult HSPSubclass_LnTestDelegate_DelegateLabelCaller(LnHandle testdelegate, int p1, int* outReturn)
{
    auto* self = reinterpret_cast<HSPSubclass_LnTestDelegate*>(LnTestDelegate_GetSubinstanceId(testdelegate));
    stat = 0;
    code_call(self->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::Sprite

struct HSPSubclass_LnSprite
{
};


static void* HSPSubclass_LnSprite_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnSprite));
}

static void HSPSubclass_LnSprite_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::CameraOrbitControlComponent

struct HSPSubclass_LnCameraOrbitControlComponent
{
};


static void* HSPSubclass_LnCameraOrbitControlComponent_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnCameraOrbitControlComponent));
}

static void HSPSubclass_LnCameraOrbitControlComponent_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::Raycaster

struct HSPSubclass_LnRaycaster
{
};


static void* HSPSubclass_LnRaycaster_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnRaycaster));
}

static void HSPSubclass_LnRaycaster_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::RaycastResult

struct HSPSubclass_LnRaycastResult
{
};


static void* HSPSubclass_LnRaycastResult_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnRaycastResult));
}

static void HSPSubclass_LnRaycastResult_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::WorldRenderView

struct HSPSubclass_LnWorldRenderView
{
};


static void* HSPSubclass_LnWorldRenderView_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnWorldRenderView));
}

static void HSPSubclass_LnWorldRenderView_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::BoxMesh

struct HSPSubclass_LnBoxMesh
{
};


static void* HSPSubclass_LnBoxMesh_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnBoxMesh));
}

static void HSPSubclass_LnBoxMesh_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::PlaneMesh

struct HSPSubclass_LnPlaneMesh
{
};


static void* HSPSubclass_LnPlaneMesh_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnPlaneMesh));
}

static void HSPSubclass_LnPlaneMesh_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::UIEventArgs

struct HSPSubclass_LnUIEventArgs
{
};


static void* HSPSubclass_LnUIEventArgs_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnUIEventArgs));
}

static void HSPSubclass_LnUIEventArgs_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::UIGeneralEventHandler

struct HSPSubclass_LnUIGeneralEventHandler
{
    unsigned short* labelPointer = nullptr;
};


static void* HSPSubclass_LnUIGeneralEventHandler_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnUIGeneralEventHandler));
}

static void HSPSubclass_LnUIGeneralEventHandler_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


static LnResult HSPSubclass_LnUIGeneralEventHandler_DelegateLabelCaller(LnHandle uigeneraleventhandler, LnHandle p1)
{
    auto* self = reinterpret_cast<HSPSubclass_LnUIGeneralEventHandler*>(LnUIGeneralEventHandler_GetSubinstanceId(uigeneraleventhandler));
    stat = 0;
    code_call(self->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::UIEventHandler

struct HSPSubclass_LnUIEventHandler
{
    unsigned short* labelPointer = nullptr;
};


static void* HSPSubclass_LnUIEventHandler_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnUIEventHandler));
}

static void HSPSubclass_LnUIEventHandler_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


static LnResult HSPSubclass_LnUIEventHandler_DelegateLabelCaller(LnHandle uieventhandler)
{
    auto* self = reinterpret_cast<HSPSubclass_LnUIEventHandler*>(LnUIEventHandler_GetSubinstanceId(uieventhandler));
    stat = 0;
    code_call(self->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::UILayoutElement

struct HSPSubclass_LnUILayoutElement
{
};


static void* HSPSubclass_LnUILayoutElement_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnUILayoutElement));
}

static void HSPSubclass_LnUILayoutElement_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::UIElement

struct HSPSubclass_LnUIElement
{
};


static void* HSPSubclass_LnUIElement_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnUIElement));
}

static void HSPSubclass_LnUIElement_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::UIControl

struct HSPSubclass_LnUIControl
{
};


static void* HSPSubclass_LnUIControl_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnUIControl));
}

static void HSPSubclass_LnUIControl_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::UIButtonBase

struct HSPSubclass_LnUIButtonBase
{
};


static void* HSPSubclass_LnUIButtonBase_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnUIButtonBase));
}

static void HSPSubclass_LnUIButtonBase_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::UIButton

struct HSPSubclass_LnUIButton
{
};


static void* HSPSubclass_LnUIButton_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnUIButton));
}

static void HSPSubclass_LnUIButton_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::UITextBlock

struct HSPSubclass_LnUITextBlock
{
};


static void* HSPSubclass_LnUITextBlock_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnUITextBlock));
}

static void HSPSubclass_LnUITextBlock_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::UISprite

struct HSPSubclass_LnUISprite
{
};


static void* HSPSubclass_LnUISprite_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnUISprite));
}

static void HSPSubclass_LnUISprite_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::Input

struct HSPSubclass_LnInput
{
};


static void* HSPSubclass_LnInput_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnInput));
}

static void HSPSubclass_LnInput_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::Mouse

struct HSPSubclass_LnMouse
{
};


static void* HSPSubclass_LnMouse_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnMouse));
}

static void HSPSubclass_LnMouse_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::EngineSettings

struct HSPSubclass_LnEngineSettings
{
};


static void* HSPSubclass_LnEngineSettings_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnEngineSettings));
}

static void HSPSubclass_LnEngineSettings_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::Engine

struct HSPSubclass_LnEngine
{
};


static void* HSPSubclass_LnEngine_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnEngine));
}

static void HSPSubclass_LnEngine_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::Application

struct HSPSubclass_LnApplication
{
};


static void* HSPSubclass_LnApplication_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnApplication));
}

static void HSPSubclass_LnApplication_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}


//==============================================================================
// ln::Debug

struct HSPSubclass_LnDebug
{
};


static void* HSPSubclass_LnDebug_SubinstanceAlloc(LnHandle handle)
{
    return malloc(sizeof(HSPSubclass_LnDebug));
}

static void HSPSubclass_LnDebug_SubinstanceFree(LnHandle handle, void* subinstance)
{
    free(subinstance);
}
bool Commands_cmdfunc(int cmd, int* retVal)
{
    *retVal = RUNMODE_RUN;
    switch (cmd) {
        // LnPromiseFailureDelegate_Create
        case 0x70 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outPromiseFailureDelegate
            PVal* pval_outPromiseFailureDelegate;
            const APTR aptr_outPromiseFailureDelegate = code_getva(&pval_outPromiseFailureDelegate);
            LnHandle local_outPromiseFailureDelegate;

            stat = LnPromiseFailureDelegate_Create(HSPSubclass_LnPromiseFailureDelegate_DelegateLabelCaller, &local_outPromiseFailureDelegate);
            auto* self = reinterpret_cast<HSPSubclass_LnPromiseFailureDelegate*>(LnPromiseFailureDelegate_GetSubinstanceId(local_outPromiseFailureDelegate));
            self->labelPointer = local_callback;
            setVAInt(pval_outPromiseFailureDelegate, aptr_outPromiseFailureDelegate, local_outPromiseFailureDelegate);

            break;
        }
        // LnZVTestDelegate1_Create
        case 0x72 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestDelegate1
            PVal* pval_outZVTestDelegate1;
            const APTR aptr_outZVTestDelegate1 = code_getva(&pval_outZVTestDelegate1);
            LnHandle local_outZVTestDelegate1;

            stat = LnZVTestDelegate1_Create(HSPSubclass_LnZVTestDelegate1_DelegateLabelCaller, &local_outZVTestDelegate1);
            auto* self = reinterpret_cast<HSPSubclass_LnZVTestDelegate1*>(LnZVTestDelegate1_GetSubinstanceId(local_outZVTestDelegate1));
            self->labelPointer = local_callback;
            setVAInt(pval_outZVTestDelegate1, aptr_outZVTestDelegate1, local_outZVTestDelegate1);

            break;
        }
        // LnZVTestDelegate2_Create
        case 0x74 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestDelegate2
            PVal* pval_outZVTestDelegate2;
            const APTR aptr_outZVTestDelegate2 = code_getva(&pval_outZVTestDelegate2);
            LnHandle local_outZVTestDelegate2;

            stat = LnZVTestDelegate2_Create(HSPSubclass_LnZVTestDelegate2_DelegateLabelCaller, &local_outZVTestDelegate2);
            auto* self = reinterpret_cast<HSPSubclass_LnZVTestDelegate2*>(LnZVTestDelegate2_GetSubinstanceId(local_outZVTestDelegate2));
            self->labelPointer = local_callback;
            setVAInt(pval_outZVTestDelegate2, aptr_outZVTestDelegate2, local_outZVTestDelegate2);

            break;
        }
        // LnZVTestDelegate3_Create
        case 0x76 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestDelegate3
            PVal* pval_outZVTestDelegate3;
            const APTR aptr_outZVTestDelegate3 = code_getva(&pval_outZVTestDelegate3);
            LnHandle local_outZVTestDelegate3;

            stat = LnZVTestDelegate3_Create(HSPSubclass_LnZVTestDelegate3_DelegateLabelCaller, &local_outZVTestDelegate3);
            auto* self = reinterpret_cast<HSPSubclass_LnZVTestDelegate3*>(LnZVTestDelegate3_GetSubinstanceId(local_outZVTestDelegate3));
            self->labelPointer = local_callback;
            setVAInt(pval_outZVTestDelegate3, aptr_outZVTestDelegate3, local_outZVTestDelegate3);

            break;
        }
        // LnZVTestEventHandler1_Create
        case 0x78 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestEventHandler1
            PVal* pval_outZVTestEventHandler1;
            const APTR aptr_outZVTestEventHandler1 = code_getva(&pval_outZVTestEventHandler1);
            LnHandle local_outZVTestEventHandler1;

            stat = LnZVTestEventHandler1_Create(HSPSubclass_LnZVTestEventHandler1_DelegateLabelCaller, &local_outZVTestEventHandler1);
            auto* self = reinterpret_cast<HSPSubclass_LnZVTestEventHandler1*>(LnZVTestEventHandler1_GetSubinstanceId(local_outZVTestEventHandler1));
            self->labelPointer = local_callback;
            setVAInt(pval_outZVTestEventHandler1, aptr_outZVTestEventHandler1, local_outZVTestEventHandler1);

            break;
        }
        // LnZVTestEventHandler2_Create
        case 0x7A : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestEventHandler2
            PVal* pval_outZVTestEventHandler2;
            const APTR aptr_outZVTestEventHandler2 = code_getva(&pval_outZVTestEventHandler2);
            LnHandle local_outZVTestEventHandler2;

            stat = LnZVTestEventHandler2_Create(HSPSubclass_LnZVTestEventHandler2_DelegateLabelCaller, &local_outZVTestEventHandler2);
            auto* self = reinterpret_cast<HSPSubclass_LnZVTestEventHandler2*>(LnZVTestEventHandler2_GetSubinstanceId(local_outZVTestEventHandler2));
            self->labelPointer = local_callback;
            setVAInt(pval_outZVTestEventHandler2, aptr_outZVTestEventHandler2, local_outZVTestEventHandler2);

            break;
        }
        // LnZVTestPromise1_ThenWith
        case 0x7B : {
            // Fetch zvtestpromise1
            const auto local_zvtestpromise1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnZVTestPromise1_ThenWith(local_zvtestpromise1, local_callback);


            break;
        }
        // LnZVTestPromise1_CatchWith
        case 0x7C : {
            // Fetch zvtestpromise1
            const auto local_zvtestpromise1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnZVTestPromise1_CatchWith(local_zvtestpromise1, local_callback);


            break;
        }
        // LnZVTestPromise2_ThenWith
        case 0x7D : {
            // Fetch zvtestpromise2
            const auto local_zvtestpromise2 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnZVTestPromise2_ThenWith(local_zvtestpromise2, local_callback);


            break;
        }
        // LnZVTestPromise2_CatchWith
        case 0x7E : {
            // Fetch zvtestpromise2
            const auto local_zvtestpromise2 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnZVTestPromise2_CatchWith(local_zvtestpromise2, local_callback);


            break;
        }
        // LnZVTestClass1_SetTestDelegate1
        case 0x7F : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LnZVTestClass1_SetTestDelegate1(local_zvtestclass1, local_value);


            break;
        }
        // LnZVTestClass1_SetTestDelegate2
        case 0x80 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LnZVTestClass1_SetTestDelegate2(local_zvtestclass1, local_value);


            break;
        }
        // LnZVTestClass1_SetTestDelegate3
        case 0x81 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LnZVTestClass1_SetTestDelegate3(local_zvtestclass1, local_value);


            break;
        }
        // LnZVTestClass1_CallTestDelegate1
        case 0x82 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch a
            const auto local_a = fetchVAInt();

            stat = LnZVTestClass1_CallTestDelegate1(local_zvtestclass1, local_a);


            break;
        }
        // LnZVTestClass1_CallTestDelegate2
        case 0x83 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch a
            const auto local_a = fetchVAInt();
            // Fetch b
            const auto local_b = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LnZVTestClass1_CallTestDelegate2(local_zvtestclass1, local_a, local_b, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnZVTestClass1_CallTestDelegate3
        case 0x84 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();

            stat = LnZVTestClass1_CallTestDelegate3(local_zvtestclass1);


            break;
        }
        // LnZVTestClass1_LoadAsyncA
        case 0x85 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnZVTestClass1_LoadAsyncA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnZVTestClass1_ExecuteAsync
        case 0x86 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnZVTestClass1_ExecuteAsync(local_zvtestclass1, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnZVTestClass1_GetFilePathA
        case 0x87 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LnZVTestClass1_GetFilePathA(local_zvtestclass1, &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnZVTestClass1_ConnectOnEvent1
        case 0x88 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnZVTestClass1_ConnectOnEvent1(local_zvtestclass1, local_handler, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnZVTestClass1_RaiseEvent1
        case 0x89 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();

            stat = LnZVTestClass1_RaiseEvent1(local_zvtestclass1);


            break;
        }
        // LnZVTestClass1_ConnectOnEvent2
        case 0x8A : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnZVTestClass1_ConnectOnEvent2(local_zvtestclass1, local_handler, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnZVTestClass1_RaiseEvent2
        case 0x8B : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();

            stat = LnZVTestClass1_RaiseEvent2(local_zvtestclass1);


            break;
        }
        // LnZVTestClass1_Create
        case 0x8C : {
            // Fetch outZVTestClass1
            PVal* pval_outZVTestClass1;
            const APTR aptr_outZVTestClass1 = code_getva(&pval_outZVTestClass1);
            LnHandle local_outZVTestClass1;

            stat = LnZVTestClass1_Create(&local_outZVTestClass1);
            setVAInt(pval_outZVTestClass1, aptr_outZVTestClass1, local_outZVTestClass1);

            break;
        }
        // LnZVTestEventArgs1_GetValue
        case 0x8D : {
            // Fetch zvtesteventargs1
            const auto local_zvtesteventargs1 = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LnZVTestEventArgs1_GetValue(local_zvtesteventargs1, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnZVTestEventArgs1_Create
        case 0x8E : {
            // Fetch outZVTestEventArgs1
            PVal* pval_outZVTestEventArgs1;
            const APTR aptr_outZVTestEventArgs1 = code_getva(&pval_outZVTestEventArgs1);
            LnHandle local_outZVTestEventArgs1;

            stat = LnZVTestEventArgs1_Create(&local_outZVTestEventArgs1);
            setVAInt(pval_outZVTestEventArgs1, aptr_outZVTestEventArgs1, local_outZVTestEventArgs1);

            break;
        }
        // LnZVTestEventArgs1_CreateWithValue
        case 0x8F : {
            // Fetch v
            const auto local_v = fetchVAInt();
            // Fetch outZVTestEventArgs1
            PVal* pval_outZVTestEventArgs1;
            const APTR aptr_outZVTestEventArgs1 = code_getva(&pval_outZVTestEventArgs1);
            LnHandle local_outZVTestEventArgs1;

            stat = LnZVTestEventArgs1_CreateWithValue(local_v, &local_outZVTestEventArgs1);
            setVAInt(pval_outZVTestEventArgs1, aptr_outZVTestEventArgs1, local_outZVTestEventArgs1);

            break;
        }
        // LnSerializer_WriteBoolA
        case 0x90 : {
            // Fetch serializer
            const auto local_serializer = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch value
            const auto local_value = static_cast<LnBool>(fetchVAInt());

            stat = LnSerializer_WriteBoolA(local_serializer, local_name, local_value);


            break;
        }
        // LnSerializer_WriteIntA
        case 0x91 : {
            // Fetch serializer
            const auto local_serializer = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LnSerializer_WriteIntA(local_serializer, local_name, local_value);


            break;
        }
        // LnSerializer_WriteFloatA
        case 0x92 : {
            // Fetch serializer
            const auto local_serializer = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LnSerializer_WriteFloatA(local_serializer, local_name, local_value);


            break;
        }
        // LnSerializer_WriteStringA
        case 0x93 : {
            // Fetch serializer
            const auto local_serializer = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch value
            const auto local_value = fetchVAString();

            stat = LnSerializer_WriteStringA(local_serializer, local_name, local_value);


            break;
        }
        // LnSerializer_WriteObjectA
        case 0x94 : {
            // Fetch serializer
            const auto local_serializer = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LnSerializer_WriteObjectA(local_serializer, local_name, local_value);


            break;
        }
        // LnSerializer_ReadBoolA
        case 0x95 : {
            // Fetch serializer
            const auto local_serializer = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnSerializer_ReadBoolA(local_serializer, local_name, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnSerializer_ReadIntA
        case 0x96 : {
            // Fetch serializer
            const auto local_serializer = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LnSerializer_ReadIntA(local_serializer, local_name, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnSerializer_ReadFloatA
        case 0x97 : {
            // Fetch serializer
            const auto local_serializer = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSerializer_ReadFloatA(local_serializer, local_name, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnSerializer_ReadStringA
        case 0x98 : {
            // Fetch serializer
            const auto local_serializer = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LnSerializer_ReadStringA(local_serializer, local_name, &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnSerializer_ReadObjectA
        case 0x99 : {
            // Fetch serializer
            const auto local_serializer = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnSerializer_ReadObjectA(local_serializer, local_name, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnSerializer_SerializeA
        case 0x9A : {
            // Fetch value
            const auto local_value = fetchVAInt();
            // Fetch basePath
            const auto local_basePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LnSerializer_SerializeA(local_value, local_basePath, &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnSerializer_DeserializeA
        case 0x9B : {
            // Fetch str
            const auto local_str = fetchVAString();
            // Fetch basePath
            const auto local_basePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnSerializer_DeserializeA(local_str, local_basePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnAssetModel_Target
        case 0x9C : {
            // Fetch assetmodel
            const auto local_assetmodel = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnAssetModel_Target(local_assetmodel, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnAssetModel_Create
        case 0x9D : {
            // Fetch target
            const auto local_target = fetchVAInt();
            // Fetch outAssetModel
            PVal* pval_outAssetModel;
            const APTR aptr_outAssetModel = code_getva(&pval_outAssetModel);
            LnHandle local_outAssetModel;

            stat = LnAssetModel_Create(local_target, &local_outAssetModel);
            setVAInt(pval_outAssetModel, aptr_outAssetModel, local_outAssetModel);

            break;
        }
        // LnAssets_SaveAssetToLocalFileA
        case 0x9E : {
            // Fetch asset
            const auto local_asset = fetchVAInt();
            // Fetch filePath
            const auto local_filePath = fetchVAString();

            stat = LnAssets_SaveAssetToLocalFileA(local_asset, local_filePath);


            break;
        }
        // LnAssets_LoadAssetFromLocalFileA
        case 0x9F : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnAssets_LoadAssetFromLocalFileA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnAssets_LoadAssetA
        case 0xA0 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnAssets_LoadAssetA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnTexture2D_LoadA
        case 0xCE : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnTexture2D_LoadA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnTexture2D_LoadEmojiA
        case 0xCF : {
            // Fetch code
            const auto local_code = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnTexture2D_LoadEmojiA(local_code, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnTexture2D_Create
        case 0xD0 : {
            // Fetch width
            const auto local_width = fetchVAInt();
            // Fetch height
            const auto local_height = fetchVAInt();
            // Fetch outTexture2D
            PVal* pval_outTexture2D;
            const APTR aptr_outTexture2D = code_getva(&pval_outTexture2D);
            LnHandle local_outTexture2D;

            stat = LnTexture2D_Create(local_width, local_height, &local_outTexture2D);
            setVAInt(pval_outTexture2D, aptr_outTexture2D, local_outTexture2D);

            break;
        }
        // LnTexture2D_CreateWithFormat
        case 0xD1 : {
            // Fetch width
            const auto local_width = fetchVAInt();
            // Fetch height
            const auto local_height = fetchVAInt();
            // Fetch format
            const auto local_format = static_cast<LnTextureFormat>(fetchVAInt());
            // Fetch outTexture2D
            PVal* pval_outTexture2D;
            const APTR aptr_outTexture2D = code_getva(&pval_outTexture2D);
            LnHandle local_outTexture2D;

            stat = LnTexture2D_CreateWithFormat(local_width, local_height, local_format, &local_outTexture2D);
            setVAInt(pval_outTexture2D, aptr_outTexture2D, local_outTexture2D);

            break;
        }
        // LnTexture2D_CreateFromFileA
        case 0xD2 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch format
            const auto local_format = static_cast<LnTextureFormat>(fetchVAInt());
            // Fetch outTexture2D
            PVal* pval_outTexture2D;
            const APTR aptr_outTexture2D = code_getva(&pval_outTexture2D);
            LnHandle local_outTexture2D;

            stat = LnTexture2D_CreateFromFileA(local_filePath, local_format, &local_outTexture2D);
            setVAInt(pval_outTexture2D, aptr_outTexture2D, local_outTexture2D);

            break;
        }
        // LnVisualComponent_SetVisible
        case 0xD3 : {
            // Fetch visualcomponent
            const auto local_visualcomponent = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LnBool>(fetchVAInt());

            stat = LnVisualComponent_SetVisible(local_visualcomponent, local_value);


            break;
        }
        // LnVisualComponent_IsVisible
        case 0xD4 : {
            // Fetch visualcomponent
            const auto local_visualcomponent = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnVisualComponent_IsVisible(local_visualcomponent, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnSpriteComponent_SetTexture
        case 0xD5 : {
            // Fetch spritecomponent
            const auto local_spritecomponent = fetchVAInt();
            // Fetch texture
            const auto local_texture = fetchVAInt();

            stat = LnSpriteComponent_SetTexture(local_spritecomponent, local_texture);


            break;
        }
        // LnWorld_Add
        case 0xD6 : {
            // Fetch world
            const auto local_world = fetchVAInt();
            // Fetch obj
            const auto local_obj = fetchVAInt();

            stat = LnWorld_Add(local_world, local_obj);


            break;
        }
        // LnComponentList_GetLength
        case 0xD7 : {
            // Fetch componentlist
            const auto local_componentlist = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LnComponentList_GetLength(local_componentlist, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnComponentList_GetItem
        case 0xD8 : {
            // Fetch componentlist
            const auto local_componentlist = fetchVAInt();
            // Fetch index
            const auto local_index = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnComponentList_GetItem(local_componentlist, local_index, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnWorldObject_SetPosition
        case 0xD9 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch pos
            PVal* pval_pos;
            CodeGetVA_TypeChecked(&pval_pos, LnVector3);

            stat = LnWorldObject_SetPosition(local_worldobject, reinterpret_cast<const LnVector3*>(pval_pos->pt));


            break;
        }
        // LnWorldObject_SetPositionXYZ
        case 0xDA : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnWorldObject_SetPositionXYZ(local_worldobject, local_x, local_y, local_z);


            break;
        }
        // LnWorldObject_GetPosition
        case 0xDB : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnWorldObject_GetPosition(local_worldobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            break;
        }
        // LnWorldObject_SetRotationQuaternion
        case 0xDC : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch rot
            PVal* pval_rot;
            CodeGetVA_TypeChecked(&pval_rot, LnQuaternion);

            stat = LnWorldObject_SetRotationQuaternion(local_worldobject, reinterpret_cast<const LnQuaternion*>(pval_rot->pt));


            break;
        }
        // LnWorldObject_SetRotation
        case 0xDD : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnWorldObject_SetRotation(local_worldobject, local_x, local_y, local_z);


            break;
        }
        // LnWorldObject_GetRotation
        case 0xDE : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnQuaternion local_outReturn;

            stat = LnWorldObject_GetRotation(local_worldobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnQuaternion_typeid(), &local_outReturn);

            break;
        }
        // LnWorldObject_SetScale
        case 0xDF : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch scale
            PVal* pval_scale;
            CodeGetVA_TypeChecked(&pval_scale, LnVector3);

            stat = LnWorldObject_SetScale(local_worldobject, reinterpret_cast<const LnVector3*>(pval_scale->pt));


            break;
        }
        // LnWorldObject_SetScaleS
        case 0xE0 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch xyz
            const auto local_xyz = fetchVADouble();

            stat = LnWorldObject_SetScaleS(local_worldobject, local_xyz);


            break;
        }
        // LnWorldObject_SetScaleXYZ
        case 0xE1 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnWorldObject_SetScaleXYZ(local_worldobject, local_x, local_y, local_z);


            break;
        }
        // LnWorldObject_GetScale
        case 0xE2 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnWorldObject_GetScale(local_worldobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            break;
        }
        // LnWorldObject_SetCenterPoint
        case 0xE3 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LnVector3);

            stat = LnWorldObject_SetCenterPoint(local_worldobject, reinterpret_cast<const LnVector3*>(pval_value->pt));


            break;
        }
        // LnWorldObject_SetCenterPointXYZ
        case 0xE4 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnWorldObject_SetCenterPointXYZ(local_worldobject, local_x, local_y, local_z);


            break;
        }
        // LnWorldObject_GetCenterPoint
        case 0xE5 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnWorldObject_GetCenterPoint(local_worldobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            break;
        }
        // LnWorldObject_LookAt
        case 0xE6 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch target
            PVal* pval_target;
            CodeGetVA_TypeChecked(&pval_target, LnVector3);

            stat = LnWorldObject_LookAt(local_worldobject, reinterpret_cast<const LnVector3*>(pval_target->pt));


            break;
        }
        // LnWorldObject_LookAtXYZ
        case 0xE7 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnWorldObject_LookAtXYZ(local_worldobject, local_x, local_y, local_z);


            break;
        }
        // LnWorldObject_AddComponent
        case 0xE8 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch component
            const auto local_component = fetchVAInt();

            stat = LnWorldObject_AddComponent(local_worldobject, local_component);


            break;
        }
        // LnWorldObject_GetComponents
        case 0xE9 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnWorldObject_GetComponents(local_worldobject, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnVisualObject_SetVisible
        case 0xEB : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LnBool>(fetchVAInt());

            stat = LnVisualObject_SetVisible(local_visualobject, local_value);


            break;
        }
        // LnVisualObject_IsVisible
        case 0xEC : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnVisualObject_IsVisible(local_visualobject, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnDirectionalLight_SetEnabled
        case 0xED : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LnBool>(fetchVAInt());

            stat = LnDirectionalLight_SetEnabled(local_directionallight, local_enabled);


            break;
        }
        // LnDirectionalLight_IsEnabled
        case 0xEE : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnDirectionalLight_IsEnabled(local_directionallight, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnDirectionalLight_SetColor
        case 0xEF : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LnColor);

            stat = LnDirectionalLight_SetColor(local_directionallight, reinterpret_cast<const LnColor*>(pval_color->pt));


            break;
        }
        // LnDirectionalLight_GetColor
        case 0xF0 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnColor local_outReturn;

            stat = LnDirectionalLight_GetColor(local_directionallight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnColor_typeid(), &local_outReturn);

            break;
        }
        // LnDirectionalLight_SetIntensity
        case 0xF1 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LnDirectionalLight_SetIntensity(local_directionallight, local_intensity);


            break;
        }
        // LnDirectionalLight_GetIntensity
        case 0xF2 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnDirectionalLight_GetIntensity(local_directionallight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnDirectionalLight_Create
        case 0xF3 : {
            // Fetch outDirectionalLight
            PVal* pval_outDirectionalLight;
            const APTR aptr_outDirectionalLight = code_getva(&pval_outDirectionalLight);
            LnHandle local_outDirectionalLight;

            stat = LnDirectionalLight_Create(&local_outDirectionalLight);
            setVAInt(pval_outDirectionalLight, aptr_outDirectionalLight, local_outDirectionalLight);

            break;
        }
        // LnDirectionalLight_CreateWithColor
        case 0xF4 : {
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LnColor);
            // Fetch outDirectionalLight
            PVal* pval_outDirectionalLight;
            const APTR aptr_outDirectionalLight = code_getva(&pval_outDirectionalLight);
            LnHandle local_outDirectionalLight;

            stat = LnDirectionalLight_CreateWithColor(reinterpret_cast<const LnColor*>(pval_color->pt), &local_outDirectionalLight);
            setVAInt(pval_outDirectionalLight, aptr_outDirectionalLight, local_outDirectionalLight);

            break;
        }
        // LnPointLight_SetEnabled
        case 0xF5 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LnBool>(fetchVAInt());

            stat = LnPointLight_SetEnabled(local_pointlight, local_enabled);


            break;
        }
        // LnPointLight_IsEnabled
        case 0xF6 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnPointLight_IsEnabled(local_pointlight, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnPointLight_SetColor
        case 0xF7 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LnColor);

            stat = LnPointLight_SetColor(local_pointlight, reinterpret_cast<const LnColor*>(pval_color->pt));


            break;
        }
        // LnPointLight_GetColor
        case 0xF8 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnColor local_outReturn;

            stat = LnPointLight_GetColor(local_pointlight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnColor_typeid(), &local_outReturn);

            break;
        }
        // LnPointLight_SetIntensity
        case 0xF9 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LnPointLight_SetIntensity(local_pointlight, local_intensity);


            break;
        }
        // LnPointLight_GetIntensity
        case 0xFA : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnPointLight_GetIntensity(local_pointlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnPointLight_SetRange
        case 0xFB : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch range
            const auto local_range = fetchVADouble();

            stat = LnPointLight_SetRange(local_pointlight, local_range);


            break;
        }
        // LnPointLight_GetRange
        case 0xFC : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnPointLight_GetRange(local_pointlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnPointLight_SetAttenuation
        case 0xFD : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch attenuation
            const auto local_attenuation = fetchVADouble();

            stat = LnPointLight_SetAttenuation(local_pointlight, local_attenuation);


            break;
        }
        // LnPointLight_GetAttenuation
        case 0xFE : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnPointLight_GetAttenuation(local_pointlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnPointLight_Create
        case 0xFF : {
            // Fetch outPointLight
            PVal* pval_outPointLight;
            const APTR aptr_outPointLight = code_getva(&pval_outPointLight);
            LnHandle local_outPointLight;

            stat = LnPointLight_Create(&local_outPointLight);
            setVAInt(pval_outPointLight, aptr_outPointLight, local_outPointLight);

            break;
        }
        // LnPointLight_CreateWithColorAndRange
        case 0x100 : {
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LnColor);
            // Fetch range
            const auto local_range = fetchVADouble();
            // Fetch outPointLight
            PVal* pval_outPointLight;
            const APTR aptr_outPointLight = code_getva(&pval_outPointLight);
            LnHandle local_outPointLight;

            stat = LnPointLight_CreateWithColorAndRange(reinterpret_cast<const LnColor*>(pval_color->pt), local_range, &local_outPointLight);
            setVAInt(pval_outPointLight, aptr_outPointLight, local_outPointLight);

            break;
        }
        // LnSpotLight_SetEnabled
        case 0x101 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LnBool>(fetchVAInt());

            stat = LnSpotLight_SetEnabled(local_spotlight, local_enabled);


            break;
        }
        // LnSpotLight_IsEnabled
        case 0x102 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnSpotLight_IsEnabled(local_spotlight, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnSpotLight_SetColor
        case 0x103 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LnColor);

            stat = LnSpotLight_SetColor(local_spotlight, reinterpret_cast<const LnColor*>(pval_color->pt));


            break;
        }
        // LnSpotLight_GetColor
        case 0x104 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnColor local_outReturn;

            stat = LnSpotLight_GetColor(local_spotlight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnColor_typeid(), &local_outReturn);

            break;
        }
        // LnSpotLight_SetIntensity
        case 0x105 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LnSpotLight_SetIntensity(local_spotlight, local_intensity);


            break;
        }
        // LnSpotLight_GetIntensity
        case 0x106 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetIntensity(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnSpotLight_SetRange
        case 0x107 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch range
            const auto local_range = fetchVADouble();

            stat = LnSpotLight_SetRange(local_spotlight, local_range);


            break;
        }
        // LnSpotLight_GetRange
        case 0x108 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetRange(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnSpotLight_SetAttenuation
        case 0x109 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch attenuation
            const auto local_attenuation = fetchVADouble();

            stat = LnSpotLight_SetAttenuation(local_spotlight, local_attenuation);


            break;
        }
        // LnSpotLight_GetAttenuation
        case 0x10A : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetAttenuation(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnSpotLight_SetAngle
        case 0x10B : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch angle
            const auto local_angle = fetchVADouble();

            stat = LnSpotLight_SetAngle(local_spotlight, local_angle);


            break;
        }
        // LnSpotLight_GetAngle
        case 0x10C : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetAngle(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnSpotLight_SetPenumbra
        case 0x10D : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch penumbra
            const auto local_penumbra = fetchVADouble();

            stat = LnSpotLight_SetPenumbra(local_spotlight, local_penumbra);


            break;
        }
        // LnSpotLight_GetPenumbra
        case 0x10E : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetPenumbra(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnSpotLight_Create
        case 0x10F : {
            // Fetch outSpotLight
            PVal* pval_outSpotLight;
            const APTR aptr_outSpotLight = code_getva(&pval_outSpotLight);
            LnHandle local_outSpotLight;

            stat = LnSpotLight_Create(&local_outSpotLight);
            setVAInt(pval_outSpotLight, aptr_outSpotLight, local_outSpotLight);

            break;
        }
        // LnSpotLight_CreateWithColorAndRange
        case 0x110 : {
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LnColor);
            // Fetch range
            const auto local_range = fetchVADouble();
            // Fetch angle
            const auto local_angle = fetchVADouble();
            // Fetch outSpotLight
            PVal* pval_outSpotLight;
            const APTR aptr_outSpotLight = code_getva(&pval_outSpotLight);
            LnHandle local_outSpotLight;

            stat = LnSpotLight_CreateWithColorAndRange(reinterpret_cast<const LnColor*>(pval_color->pt), local_range, local_angle, &local_outSpotLight);
            setVAInt(pval_outSpotLight, aptr_outSpotLight, local_outSpotLight);

            break;
        }
        // LnTestDelegate_Create
        case 0x112 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outTestDelegate
            PVal* pval_outTestDelegate;
            const APTR aptr_outTestDelegate = code_getva(&pval_outTestDelegate);
            LnHandle local_outTestDelegate;

            stat = LnTestDelegate_Create(HSPSubclass_LnTestDelegate_DelegateLabelCaller, &local_outTestDelegate);
            auto* self = reinterpret_cast<HSPSubclass_LnTestDelegate*>(LnTestDelegate_GetSubinstanceId(local_outTestDelegate));
            self->labelPointer = local_callback;
            setVAInt(pval_outTestDelegate, aptr_outTestDelegate, local_outTestDelegate);

            break;
        }
        // LnSprite_SetTexture
        case 0x113 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LnSprite_SetTexture(local_sprite, local_value);


            break;
        }
        // LnSprite_SetSize
        case 0x114 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LnSize);

            stat = LnSprite_SetSize(local_sprite, reinterpret_cast<const LnSize*>(pval_value->pt));


            break;
        }
        // LnSprite_SetSizeWH
        case 0x115 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch width
            const auto local_width = fetchVADouble();
            // Fetch height
            const auto local_height = fetchVADouble();

            stat = LnSprite_SetSizeWH(local_sprite, local_width, local_height);


            break;
        }
        // LnSprite_SetSourceRectXYWH
        case 0x116 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch width
            const auto local_width = fetchVADouble();
            // Fetch height
            const auto local_height = fetchVADouble();

            stat = LnSprite_SetSourceRectXYWH(local_sprite, local_x, local_y, local_width, local_height);


            break;
        }
        // LnSprite_SetCallerTest
        case 0x117 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnSprite_SetCallerTest(local_sprite, local_callback);


            break;
        }
        // LnSprite_Create
        case 0x118 : {
            // Fetch outSprite
            PVal* pval_outSprite;
            const APTR aptr_outSprite = code_getva(&pval_outSprite);
            LnHandle local_outSprite;

            stat = LnSprite_Create(&local_outSprite);
            setVAInt(pval_outSprite, aptr_outSprite, local_outSprite);

            break;
        }
        // LnSprite_CreateWithTexture
        case 0x119 : {
            // Fetch texture
            const auto local_texture = fetchVAInt();
            // Fetch outSprite
            PVal* pval_outSprite;
            const APTR aptr_outSprite = code_getva(&pval_outSprite);
            LnHandle local_outSprite;

            stat = LnSprite_CreateWithTexture(local_texture, &local_outSprite);
            setVAInt(pval_outSprite, aptr_outSprite, local_outSprite);

            break;
        }
        // LnSprite_CreateWithTextureAndSize
        case 0x11A : {
            // Fetch texture
            const auto local_texture = fetchVAInt();
            // Fetch width
            const auto local_width = fetchVADouble();
            // Fetch height
            const auto local_height = fetchVADouble();
            // Fetch outSprite
            PVal* pval_outSprite;
            const APTR aptr_outSprite = code_getva(&pval_outSprite);
            LnHandle local_outSprite;

            stat = LnSprite_CreateWithTextureAndSize(local_texture, local_width, local_height, &local_outSprite);
            setVAInt(pval_outSprite, aptr_outSprite, local_outSprite);

            break;
        }
        // LnCameraOrbitControlComponent_Create
        case 0x11B : {
            // Fetch outCameraOrbitControlComponent
            PVal* pval_outCameraOrbitControlComponent;
            const APTR aptr_outCameraOrbitControlComponent = code_getva(&pval_outCameraOrbitControlComponent);
            LnHandle local_outCameraOrbitControlComponent;

            stat = LnCameraOrbitControlComponent_Create(&local_outCameraOrbitControlComponent);
            setVAInt(pval_outCameraOrbitControlComponent, aptr_outCameraOrbitControlComponent, local_outCameraOrbitControlComponent);

            break;
        }
        // LnRaycaster_FromScreen
        case 0x11C : {
            // Fetch point
            PVal* pval_point;
            CodeGetVA_TypeChecked(&pval_point, LnPoint);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnRaycaster_FromScreen(reinterpret_cast<const LnPoint*>(pval_point->pt), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnRaycaster_IntersectPlane
        case 0x11D : {
            // Fetch raycaster
            const auto local_raycaster = fetchVAInt();
            // Fetch normalX
            const auto local_normalX = fetchVADouble();
            // Fetch normalY
            const auto local_normalY = fetchVADouble();
            // Fetch normalZ
            const auto local_normalZ = fetchVADouble();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnRaycaster_IntersectPlane(local_raycaster, local_normalX, local_normalY, local_normalZ, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnRaycastResult_GetPoint
        case 0x11E : {
            // Fetch raycastresult
            const auto local_raycastresult = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnRaycastResult_GetPoint(local_raycastresult, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            break;
        }
        // LnWorldRenderView_SetGuideGridEnabled
        case 0x11F : {
            // Fetch worldrenderview
            const auto local_worldrenderview = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LnBool>(fetchVAInt());

            stat = LnWorldRenderView_SetGuideGridEnabled(local_worldrenderview, local_value);


            break;
        }
        // LnWorldRenderView_GetGuideGridEnabled
        case 0x120 : {
            // Fetch worldrenderview
            const auto local_worldrenderview = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnWorldRenderView_GetGuideGridEnabled(local_worldrenderview, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnBoxMesh_Create
        case 0x121 : {
            // Fetch outBoxMesh
            PVal* pval_outBoxMesh;
            const APTR aptr_outBoxMesh = code_getva(&pval_outBoxMesh);
            LnHandle local_outBoxMesh;

            stat = LnBoxMesh_Create(&local_outBoxMesh);
            setVAInt(pval_outBoxMesh, aptr_outBoxMesh, local_outBoxMesh);

            break;
        }
        // LnPlaneMesh_Create
        case 0x122 : {
            // Fetch outPlaneMesh
            PVal* pval_outPlaneMesh;
            const APTR aptr_outPlaneMesh = code_getva(&pval_outPlaneMesh);
            LnHandle local_outPlaneMesh;

            stat = LnPlaneMesh_Create(&local_outPlaneMesh);
            setVAInt(pval_outPlaneMesh, aptr_outPlaneMesh, local_outPlaneMesh);

            break;
        }
        // LnUIEventArgs_Sender
        case 0x123 : {
            // Fetch uieventargs
            const auto local_uieventargs = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnUIEventArgs_Sender(local_uieventargs, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnUIGeneralEventHandler_Create
        case 0x125 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIGeneralEventHandler
            PVal* pval_outUIGeneralEventHandler;
            const APTR aptr_outUIGeneralEventHandler = code_getva(&pval_outUIGeneralEventHandler);
            LnHandle local_outUIGeneralEventHandler;

            stat = LnUIGeneralEventHandler_Create(HSPSubclass_LnUIGeneralEventHandler_DelegateLabelCaller, &local_outUIGeneralEventHandler);
            auto* self = reinterpret_cast<HSPSubclass_LnUIGeneralEventHandler*>(LnUIGeneralEventHandler_GetSubinstanceId(local_outUIGeneralEventHandler));
            self->labelPointer = local_callback;
            setVAInt(pval_outUIGeneralEventHandler, aptr_outUIGeneralEventHandler, local_outUIGeneralEventHandler);

            break;
        }
        // LnUIEventHandler_Create
        case 0x127 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIEventHandler
            PVal* pval_outUIEventHandler;
            const APTR aptr_outUIEventHandler = code_getva(&pval_outUIEventHandler);
            LnHandle local_outUIEventHandler;

            stat = LnUIEventHandler_Create(HSPSubclass_LnUIEventHandler_DelegateLabelCaller, &local_outUIEventHandler);
            auto* self = reinterpret_cast<HSPSubclass_LnUIEventHandler*>(LnUIEventHandler_GetSubinstanceId(local_outUIEventHandler));
            self->labelPointer = local_callback;
            setVAInt(pval_outUIEventHandler, aptr_outUIEventHandler, local_outUIEventHandler);

            break;
        }
        // LnUIElement_SetMargin
        case 0x128 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch margin
            PVal* pval_margin;
            CodeGetVA_TypeChecked(&pval_margin, LnThickness);

            stat = LnUIElement_SetMargin(local_uielement, reinterpret_cast<const LnThickness*>(pval_margin->pt));


            break;
        }
        // LnUIElement_GetMargin
        case 0x129 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnThickness local_outReturn;

            stat = LnUIElement_GetMargin(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnThickness_typeid(), &local_outReturn);

            break;
        }
        // LnUIElement_SetPadding
        case 0x12A : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch padding
            PVal* pval_padding;
            CodeGetVA_TypeChecked(&pval_padding, LnThickness);

            stat = LnUIElement_SetPadding(local_uielement, reinterpret_cast<const LnThickness*>(pval_padding->pt));


            break;
        }
        // LnUIElement_GetPadding
        case 0x12B : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnThickness local_outReturn;

            stat = LnUIElement_GetPadding(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnThickness_typeid(), &local_outReturn);

            break;
        }
        // LnUIElement_SetHAlignment
        case 0x12C : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LnHAlignment>(fetchVAInt());

            stat = LnUIElement_SetHAlignment(local_uielement, local_value);


            break;
        }
        // LnUIElement_GetHAlignment
        case 0x12D : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHAlignment local_outReturn;

            stat = LnUIElement_GetHAlignment(local_uielement, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnUIElement_SetVAlignment
        case 0x12E : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LnVAlignment>(fetchVAInt());

            stat = LnUIElement_SetVAlignment(local_uielement, local_value);


            break;
        }
        // LnUIElement_GetVAlignment
        case 0x12F : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVAlignment local_outReturn;

            stat = LnUIElement_GetVAlignment(local_uielement, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnUIElement_SetAlignments
        case 0x130 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch halign
            const auto local_halign = static_cast<LnHAlignment>(fetchVAInt());
            // Fetch valign
            const auto local_valign = static_cast<LnVAlignment>(fetchVAInt());

            stat = LnUIElement_SetAlignments(local_uielement, local_halign, local_valign);


            break;
        }
        // LnUIElement_SetPosition
        case 0x131 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch pos
            PVal* pval_pos;
            CodeGetVA_TypeChecked(&pval_pos, LnVector3);

            stat = LnUIElement_SetPosition(local_uielement, reinterpret_cast<const LnVector3*>(pval_pos->pt));


            break;
        }
        // LnUIElement_SetPositionXYZ
        case 0x132 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnUIElement_SetPositionXYZ(local_uielement, local_x, local_y, local_z);


            break;
        }
        // LnUIElement_GetPosition
        case 0x133 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnUIElement_GetPosition(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            break;
        }
        // LnUIElement_SetRotation
        case 0x134 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch rot
            PVal* pval_rot;
            CodeGetVA_TypeChecked(&pval_rot, LnQuaternion);

            stat = LnUIElement_SetRotation(local_uielement, reinterpret_cast<const LnQuaternion*>(pval_rot->pt));


            break;
        }
        // LnUIElement_SetEulerAngles
        case 0x135 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnUIElement_SetEulerAngles(local_uielement, local_x, local_y, local_z);


            break;
        }
        // LnUIElement_GetRotation
        case 0x136 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnQuaternion local_outReturn;

            stat = LnUIElement_GetRotation(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnQuaternion_typeid(), &local_outReturn);

            break;
        }
        // LnUIElement_SetScale
        case 0x137 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch scale
            PVal* pval_scale;
            CodeGetVA_TypeChecked(&pval_scale, LnVector3);

            stat = LnUIElement_SetScale(local_uielement, reinterpret_cast<const LnVector3*>(pval_scale->pt));


            break;
        }
        // LnUIElement_SetScaleS
        case 0x138 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch xyz
            const auto local_xyz = fetchVADouble();

            stat = LnUIElement_SetScaleS(local_uielement, local_xyz);


            break;
        }
        // LnUIElement_SetScaleXY
        case 0x139 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();

            stat = LnUIElement_SetScaleXY(local_uielement, local_x, local_y);


            break;
        }
        // LnUIElement_GetScale
        case 0x13A : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnUIElement_GetScale(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            break;
        }
        // LnUIElement_SetCenterPoint
        case 0x13B : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LnVector3);

            stat = LnUIElement_SetCenterPoint(local_uielement, reinterpret_cast<const LnVector3*>(pval_value->pt));


            break;
        }
        // LnUIElement_SetCenterPointXYZ
        case 0x13C : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnUIElement_SetCenterPointXYZ(local_uielement, local_x, local_y, local_z);


            break;
        }
        // LnUIElement_GetCenterPoint
        case 0x13D : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnUIElement_GetCenterPoint(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            break;
        }
        // LnUIElement_AddChild
        case 0x13E : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch child
            const auto local_child = fetchVAInt();

            stat = LnUIElement_AddChild(local_uielement, local_child);


            break;
        }
        // LnUIButtonBase_SetTextA
        case 0x13F : {
            // Fetch uibuttonbase
            const auto local_uibuttonbase = fetchVAInt();
            // Fetch text
            const auto local_text = fetchVAString();

            stat = LnUIButtonBase_SetTextA(local_uibuttonbase, local_text);


            break;
        }
        // LnUIButton_Create
        case 0x140 : {
            // Fetch outUIButton
            PVal* pval_outUIButton;
            const APTR aptr_outUIButton = code_getva(&pval_outUIButton);
            LnHandle local_outUIButton;

            stat = LnUIButton_Create(&local_outUIButton);
            setVAInt(pval_outUIButton, aptr_outUIButton, local_outUIButton);

            break;
        }
        // LnUIButton_CreateWithTextA
        case 0x141 : {
            // Fetch text
            const auto local_text = fetchVAString();
            // Fetch outUIButton
            PVal* pval_outUIButton;
            const APTR aptr_outUIButton = code_getva(&pval_outUIButton);
            LnHandle local_outUIButton;

            stat = LnUIButton_CreateWithTextA(local_text, &local_outUIButton);
            setVAInt(pval_outUIButton, aptr_outUIButton, local_outUIButton);

            break;
        }
        // LnUIButton_ConnectOnClicked
        case 0x142 : {
            // Fetch uibutton
            const auto local_uibutton = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnUIButton_ConnectOnClicked(local_uibutton, local_handler, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnUITextBlock_Create
        case 0x143 : {
            // Fetch outUITextBlock
            PVal* pval_outUITextBlock;
            const APTR aptr_outUITextBlock = code_getva(&pval_outUITextBlock);
            LnHandle local_outUITextBlock;

            stat = LnUITextBlock_Create(&local_outUITextBlock);
            setVAInt(pval_outUITextBlock, aptr_outUITextBlock, local_outUITextBlock);

            break;
        }
        // LnUITextBlock_CreateWithTextA
        case 0x144 : {
            // Fetch text
            const auto local_text = fetchVAString();
            // Fetch outUITextBlock
            PVal* pval_outUITextBlock;
            const APTR aptr_outUITextBlock = code_getva(&pval_outUITextBlock);
            LnHandle local_outUITextBlock;

            stat = LnUITextBlock_CreateWithTextA(local_text, &local_outUITextBlock);
            setVAInt(pval_outUITextBlock, aptr_outUITextBlock, local_outUITextBlock);

            break;
        }
        // LnUISprite_SetTexture
        case 0x145 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch texture
            const auto local_texture = fetchVAInt();

            stat = LnUISprite_SetTexture(local_uisprite, local_texture);


            break;
        }
        // LnUISprite_SetSourceRect
        case 0x146 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch rect
            PVal* pval_rect;
            CodeGetVA_TypeChecked(&pval_rect, LnRect);

            stat = LnUISprite_SetSourceRect(local_uisprite, reinterpret_cast<const LnRect*>(pval_rect->pt));


            break;
        }
        // LnUISprite_SetSourceRectXYWH
        case 0x147 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch width
            const auto local_width = fetchVADouble();
            // Fetch height
            const auto local_height = fetchVADouble();

            stat = LnUISprite_SetSourceRectXYWH(local_uisprite, local_x, local_y, local_width, local_height);


            break;
        }
        // LnUISprite_GetSourceRect
        case 0x148 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnRect local_outReturn;

            stat = LnUISprite_GetSourceRect(local_uisprite, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnRect_typeid(), &local_outReturn);

            break;
        }
        // LnUISprite_Create
        case 0x149 : {
            // Fetch outUISprite
            PVal* pval_outUISprite;
            const APTR aptr_outUISprite = code_getva(&pval_outUISprite);
            LnHandle local_outUISprite;

            stat = LnUISprite_Create(&local_outUISprite);
            setVAInt(pval_outUISprite, aptr_outUISprite, local_outUISprite);

            break;
        }
        // LnUISprite_CreateWithTexture
        case 0x14A : {
            // Fetch texture
            const auto local_texture = fetchVAInt();
            // Fetch outUISprite
            PVal* pval_outUISprite;
            const APTR aptr_outUISprite = code_getva(&pval_outUISprite);
            LnHandle local_outUISprite;

            stat = LnUISprite_CreateWithTexture(local_texture, &local_outUISprite);
            setVAInt(pval_outUISprite, aptr_outUISprite, local_outUISprite);

            break;
        }
        // LnInput_PressedA
        case 0x14B : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnInput_PressedA(local_buttonName, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnInput_TriggeredA
        case 0x14C : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnInput_TriggeredA(local_buttonName, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnInput_TriggeredOffA
        case 0x14D : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnInput_TriggeredOffA(local_buttonName, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnInput_RepeatedA
        case 0x14E : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnInput_RepeatedA(local_buttonName, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnInput_GetAxisValueA
        case 0x14F : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnInput_GetAxisValueA(local_buttonName, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnInput_ClearAllBindings
        case 0x150 : {

            stat = LnInput_ClearAllBindings();


            break;
        }
        // LnMouse_Pressed
        case 0x151 : {
            // Fetch button
            const auto local_button = static_cast<LnMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnMouse_Pressed(local_button, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnMouse_Triggered
        case 0x152 : {
            // Fetch button
            const auto local_button = static_cast<LnMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnMouse_Triggered(local_button, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnMouse_TriggeredOff
        case 0x153 : {
            // Fetch button
            const auto local_button = static_cast<LnMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnMouse_TriggeredOff(local_button, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnMouse_Repeated
        case 0x154 : {
            // Fetch button
            const auto local_button = static_cast<LnMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnMouse_Repeated(local_button, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnMouse_Position
        case 0x155 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnPoint local_outReturn;

            stat = LnMouse_Position(&local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnPoint_typeid(), &local_outReturn);

            break;
        }
        // LnEngineSettings_SetMainWindowSize
        case 0x156 : {
            // Fetch width
            const auto local_width = fetchVAInt();
            // Fetch height
            const auto local_height = fetchVAInt();

            stat = LnEngineSettings_SetMainWindowSize(local_width, local_height);


            break;
        }
        // LnEngineSettings_SetMainWorldViewSize
        case 0x157 : {
            // Fetch width
            const auto local_width = fetchVAInt();
            // Fetch height
            const auto local_height = fetchVAInt();

            stat = LnEngineSettings_SetMainWorldViewSize(local_width, local_height);


            break;
        }
        // LnEngineSettings_SetMainWindowTitleA
        case 0x158 : {
            // Fetch title
            const auto local_title = fetchVAString();

            stat = LnEngineSettings_SetMainWindowTitleA(local_title);


            break;
        }
        // LnEngineSettings_AddAssetDirectoryA
        case 0x159 : {
            // Fetch path
            const auto local_path = fetchVAString();

            stat = LnEngineSettings_AddAssetDirectoryA(local_path);


            break;
        }
        // LnEngineSettings_AddAssetArchiveA
        case 0x15A : {
            // Fetch fileFullPath
            const auto local_fileFullPath = fetchVAString();
            // Fetch password
            const auto local_password = fetchVAString();

            stat = LnEngineSettings_AddAssetArchiveA(local_fileFullPath, local_password);


            break;
        }
        // LnEngineSettings_SetFrameRate
        case 0x15B : {
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LnEngineSettings_SetFrameRate(local_value);


            break;
        }
        // LnEngineSettings_SetDebugToolEnabled
        case 0x15C : {
            // Fetch enabled
            const auto local_enabled = static_cast<LnBool>(fetchVAInt());

            stat = LnEngineSettings_SetDebugToolEnabled(local_enabled);


            break;
        }
        // LnEngineSettings_SetEngineLogEnabled
        case 0x15D : {
            // Fetch enabled
            const auto local_enabled = static_cast<LnBool>(fetchVAInt());

            stat = LnEngineSettings_SetEngineLogEnabled(local_enabled);


            break;
        }
        // LnEngineSettings_SetEngineLogFilePathA
        case 0x15E : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();

            stat = LnEngineSettings_SetEngineLogFilePathA(local_filePath);


            break;
        }
        // LnEngine_Initialize
        case 0x15F : {

            stat = LnEngine_Initialize();


            break;
        }
        // LnEngine_Finalize
        case 0x160 : {

            stat = LnEngine_Finalize();


            break;
        }
        // LnEngine_Update
        case 0x161 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnEngine_Update(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnEngine_Time
        case 0x162 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            double local_outReturn;

            stat = LnEngine_Time(&local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnEngine_GetCamera
        case 0x163 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnEngine_GetCamera(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnEngine_GetLight
        case 0x164 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnEngine_GetLight(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnEngine_GetRenderView
        case 0x165 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnEngine_GetRenderView(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnApplication_OnInit
        case 0x166 : {
            // Fetch application
            const auto local_application = fetchVAInt();

            stat = LnApplication_OnInit(local_application);


            break;
        }
        // LnApplication_OnUpdate
        case 0x167 : {
            // Fetch application
            const auto local_application = fetchVAInt();

            stat = LnApplication_OnUpdate(local_application);


            break;
        }
        // LnApplication_World
        case 0x168 : {
            // Fetch application
            const auto local_application = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnApplication_World(local_application, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            break;
        }
        // LnApplication_Create
        case 0x169 : {
            // Fetch outApplication
            PVal* pval_outApplication;
            const APTR aptr_outApplication = code_getva(&pval_outApplication);
            LnHandle local_outApplication;

            stat = LnApplication_Create(&local_outApplication);
            setVAInt(pval_outApplication, aptr_outApplication, local_outApplication);

            break;
        }
        // LnDebug_PrintA
        case 0x16A : {
            // Fetch str
            const auto local_str = fetchVAString();

            stat = LnDebug_PrintA(local_str);


            break;
        }
        // LnDebug_PrintWithTimeA
        case 0x16B : {
            // Fetch time
            const auto local_time = fetchVADouble();
            // Fetch str
            const auto local_str = fetchVAString();

            stat = LnDebug_PrintWithTimeA(local_time, local_str);


            break;
        }
        // LnDebug_PrintWithTimeAndColorA
        case 0x16C : {
            // Fetch time
            const auto local_time = fetchVADouble();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LnColor);
            // Fetch str
            const auto local_str = fetchVAString();

            stat = LnDebug_PrintWithTimeAndColorA(local_time, reinterpret_cast<const LnColor*>(pval_color->pt), local_str);


            break;
        }
    }
}

