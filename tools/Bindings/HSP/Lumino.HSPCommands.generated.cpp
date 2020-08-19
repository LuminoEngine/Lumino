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


//==============================================================================
// ln::Object

struct HSPSubclass_LnObject
{
};


static LnSubinstanceId HSPSubclass_LnObject_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnObject)));
}

static void HSPSubclass_LnObject_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::EventConnection

struct HSPSubclass_LnEventConnection
{
};


static LnSubinstanceId HSPSubclass_LnEventConnection_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnEventConnection)));
}

static void HSPSubclass_LnEventConnection_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::PromiseFailureDelegate

struct HSPSubclass_LnPromiseFailureDelegate
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnPromiseFailureDelegate_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnPromiseFailureDelegate)));
}

static void HSPSubclass_LnPromiseFailureDelegate_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnPromiseFailureDelegate_DelegateLabelCaller(LnHandle promisefailuredelegate)
{
    setCallbackArg(0, promisefailuredelegate);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnPromiseFailureDelegate*>(LnPromiseFailureDelegate_GetSubinstanceId(promisefailuredelegate));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::ZVTestDelegate1

struct HSPSubclass_LnZVTestDelegate1
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnZVTestDelegate1_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnZVTestDelegate1)));
}

static void HSPSubclass_LnZVTestDelegate1_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnZVTestDelegate1_DelegateLabelCaller(LnHandle zvtestdelegate1, int p1)
{
    setCallbackArg(0, zvtestdelegate1);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestDelegate1*>(LnZVTestDelegate1_GetSubinstanceId(zvtestdelegate1));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::ZVTestDelegate2

struct HSPSubclass_LnZVTestDelegate2
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnZVTestDelegate2_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnZVTestDelegate2)));
}

static void HSPSubclass_LnZVTestDelegate2_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnZVTestDelegate2_DelegateLabelCaller(LnHandle zvtestdelegate2, int p1, int p2, int* outReturn)
{
    setCallbackArg(0, zvtestdelegate2);
    setCallbackArg(1, p1);
    setCallbackArg(2, p2);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestDelegate2*>(LnZVTestDelegate2_GetSubinstanceId(zvtestdelegate2));
    stat = 0;
    code_call(localSelf->labelPointer);
    setCallbackOutput(3, outReturn);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::ZVTestDelegate3

struct HSPSubclass_LnZVTestDelegate3
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnZVTestDelegate3_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnZVTestDelegate3)));
}

static void HSPSubclass_LnZVTestDelegate3_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnZVTestDelegate3_DelegateLabelCaller(LnHandle zvtestdelegate3, LnHandle p1)
{
    setCallbackArg(0, zvtestdelegate3);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestDelegate3*>(LnZVTestDelegate3_GetSubinstanceId(zvtestdelegate3));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::ZVTestEventHandler1

struct HSPSubclass_LnZVTestEventHandler1
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnZVTestEventHandler1_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnZVTestEventHandler1)));
}

static void HSPSubclass_LnZVTestEventHandler1_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnZVTestEventHandler1_DelegateLabelCaller(LnHandle zvtesteventhandler1)
{
    setCallbackArg(0, zvtesteventhandler1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestEventHandler1*>(LnZVTestEventHandler1_GetSubinstanceId(zvtesteventhandler1));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::ZVTestEventHandler2

struct HSPSubclass_LnZVTestEventHandler2
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnZVTestEventHandler2_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnZVTestEventHandler2)));
}

static void HSPSubclass_LnZVTestEventHandler2_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnZVTestEventHandler2_DelegateLabelCaller(LnHandle zvtesteventhandler2, LnHandle p1)
{
    setCallbackArg(0, zvtesteventhandler2);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestEventHandler2*>(LnZVTestEventHandler2_GetSubinstanceId(zvtesteventhandler2));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::ZVTestPromise1

struct HSPSubclass_LnZVTestPromise1
{
};


static LnSubinstanceId HSPSubclass_LnZVTestPromise1_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnZVTestPromise1)));
}

static void HSPSubclass_LnZVTestPromise1_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::ZVTestPromise2

struct HSPSubclass_LnZVTestPromise2
{
};


static LnSubinstanceId HSPSubclass_LnZVTestPromise2_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnZVTestPromise2)));
}

static void HSPSubclass_LnZVTestPromise2_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::ZVTestClass1

struct HSPSubclass_LnZVTestClass1
{
};


static LnSubinstanceId HSPSubclass_LnZVTestClass1_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnZVTestClass1)));
}

static void HSPSubclass_LnZVTestClass1_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::ZVTestEventArgs1

struct HSPSubclass_LnZVTestEventArgs1
{
};


static LnSubinstanceId HSPSubclass_LnZVTestEventArgs1_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnZVTestEventArgs1)));
}

static void HSPSubclass_LnZVTestEventArgs1_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Serializer2

struct HSPSubclass_LnSerializer2
{
};


static LnSubinstanceId HSPSubclass_LnSerializer2_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnSerializer2)));
}

static void HSPSubclass_LnSerializer2_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::AssetModel

struct HSPSubclass_LnAssetModel
{
};


static LnSubinstanceId HSPSubclass_LnAssetModel_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnAssetModel)));
}

static void HSPSubclass_LnAssetModel_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Assets

struct HSPSubclass_LnAssets
{
};


static LnSubinstanceId HSPSubclass_LnAssets_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnAssets)));
}

static void HSPSubclass_LnAssets_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Texture2DDelegate

struct HSPSubclass_LnTexture2DDelegate
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnTexture2DDelegate_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnTexture2DDelegate)));
}

static void HSPSubclass_LnTexture2DDelegate_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnTexture2DDelegate_DelegateLabelCaller(LnHandle texture2ddelegate, LnHandle p1)
{
    setCallbackArg(0, texture2ddelegate);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnTexture2DDelegate*>(LnTexture2DDelegate_GetSubinstanceId(texture2ddelegate));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::Texture2DPromise

struct HSPSubclass_LnTexture2DPromise
{
};


static LnSubinstanceId HSPSubclass_LnTexture2DPromise_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnTexture2DPromise)));
}

static void HSPSubclass_LnTexture2DPromise_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Texture

struct HSPSubclass_LnTexture
{
};


static LnSubinstanceId HSPSubclass_LnTexture_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnTexture)));
}

static void HSPSubclass_LnTexture_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Texture2D

struct HSPSubclass_LnTexture2D
{
};


static LnSubinstanceId HSPSubclass_LnTexture2D_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnTexture2D)));
}

static void HSPSubclass_LnTexture2D_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::RenderView

struct HSPSubclass_LnRenderView
{
};


static LnSubinstanceId HSPSubclass_LnRenderView_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnRenderView)));
}

static void HSPSubclass_LnRenderView_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Component

struct HSPSubclass_LnComponent
{
};


static LnSubinstanceId HSPSubclass_LnComponent_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnComponent)));
}

static void HSPSubclass_LnComponent_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::VisualComponent

struct HSPSubclass_LnVisualComponent
{
};


static LnSubinstanceId HSPSubclass_LnVisualComponent_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnVisualComponent)));
}

static void HSPSubclass_LnVisualComponent_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::SpriteComponent

struct HSPSubclass_LnSpriteComponent
{
};


static LnSubinstanceId HSPSubclass_LnSpriteComponent_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnSpriteComponent)));
}

static void HSPSubclass_LnSpriteComponent_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::World

struct HSPSubclass_LnWorld
{
};


static LnSubinstanceId HSPSubclass_LnWorld_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnWorld)));
}

static void HSPSubclass_LnWorld_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::ComponentList

struct HSPSubclass_LnComponentList
{
};


static LnSubinstanceId HSPSubclass_LnComponentList_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnComponentList)));
}

static void HSPSubclass_LnComponentList_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::WorldObject

struct HSPSubclass_LnWorldObject
{
};


static LnSubinstanceId HSPSubclass_LnWorldObject_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnWorldObject)));
}

static void HSPSubclass_LnWorldObject_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::VisualObject

struct HSPSubclass_LnVisualObject
{
};


static LnSubinstanceId HSPSubclass_LnVisualObject_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnVisualObject)));
}

static void HSPSubclass_LnVisualObject_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Camera

struct HSPSubclass_LnCamera
{
};


static LnSubinstanceId HSPSubclass_LnCamera_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnCamera)));
}

static void HSPSubclass_LnCamera_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::DirectionalLight

struct HSPSubclass_LnDirectionalLight
{
};


static LnSubinstanceId HSPSubclass_LnDirectionalLight_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnDirectionalLight)));
}

static void HSPSubclass_LnDirectionalLight_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::PointLight

struct HSPSubclass_LnPointLight
{
};


static LnSubinstanceId HSPSubclass_LnPointLight_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnPointLight)));
}

static void HSPSubclass_LnPointLight_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::SpotLight

struct HSPSubclass_LnSpotLight
{
};


static LnSubinstanceId HSPSubclass_LnSpotLight_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnSpotLight)));
}

static void HSPSubclass_LnSpotLight_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::TestDelegate

struct HSPSubclass_LnTestDelegate
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnTestDelegate_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnTestDelegate)));
}

static void HSPSubclass_LnTestDelegate_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnTestDelegate_DelegateLabelCaller(LnHandle testdelegate, int p1, int* outReturn)
{
    setCallbackArg(0, testdelegate);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnTestDelegate*>(LnTestDelegate_GetSubinstanceId(testdelegate));
    stat = 0;
    code_call(localSelf->labelPointer);
    setCallbackOutput(2, outReturn);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::Sprite

struct HSPSubclass_LnSprite
{
};


static LnSubinstanceId HSPSubclass_LnSprite_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnSprite)));
}

static void HSPSubclass_LnSprite_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::CameraOrbitControlComponent

struct HSPSubclass_LnCameraOrbitControlComponent
{
};


static LnSubinstanceId HSPSubclass_LnCameraOrbitControlComponent_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnCameraOrbitControlComponent)));
}

static void HSPSubclass_LnCameraOrbitControlComponent_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Raycaster

struct HSPSubclass_LnRaycaster
{
};


static LnSubinstanceId HSPSubclass_LnRaycaster_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnRaycaster)));
}

static void HSPSubclass_LnRaycaster_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::RaycastResult

struct HSPSubclass_LnRaycastResult
{
};


static LnSubinstanceId HSPSubclass_LnRaycastResult_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnRaycastResult)));
}

static void HSPSubclass_LnRaycastResult_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::WorldRenderView

struct HSPSubclass_LnWorldRenderView
{
};


static LnSubinstanceId HSPSubclass_LnWorldRenderView_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnWorldRenderView)));
}

static void HSPSubclass_LnWorldRenderView_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::BoxMesh

struct HSPSubclass_LnBoxMesh
{
};


static LnSubinstanceId HSPSubclass_LnBoxMesh_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnBoxMesh)));
}

static void HSPSubclass_LnBoxMesh_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::PlaneMesh

struct HSPSubclass_LnPlaneMesh
{
};


static LnSubinstanceId HSPSubclass_LnPlaneMesh_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnPlaneMesh)));
}

static void HSPSubclass_LnPlaneMesh_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIEventArgs

struct HSPSubclass_LnUIEventArgs
{
};


static LnSubinstanceId HSPSubclass_LnUIEventArgs_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnUIEventArgs)));
}

static void HSPSubclass_LnUIEventArgs_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIGeneralEventHandler

struct HSPSubclass_LnUIGeneralEventHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnUIGeneralEventHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnUIGeneralEventHandler)));
}

static void HSPSubclass_LnUIGeneralEventHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnUIGeneralEventHandler_DelegateLabelCaller(LnHandle uigeneraleventhandler, LnHandle p1)
{
    setCallbackArg(0, uigeneraleventhandler);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnUIGeneralEventHandler*>(LnUIGeneralEventHandler_GetSubinstanceId(uigeneraleventhandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::UIEventHandler

struct HSPSubclass_LnUIEventHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnUIEventHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnUIEventHandler)));
}

static void HSPSubclass_LnUIEventHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnUIEventHandler_DelegateLabelCaller(LnHandle uieventhandler)
{
    setCallbackArg(0, uieventhandler);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnUIEventHandler*>(LnUIEventHandler_GetSubinstanceId(uieventhandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ln::UILayoutElement

struct HSPSubclass_LnUILayoutElement
{
};


static LnSubinstanceId HSPSubclass_LnUILayoutElement_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnUILayoutElement)));
}

static void HSPSubclass_LnUILayoutElement_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIElement

struct HSPSubclass_LnUIElement
{
};


static LnSubinstanceId HSPSubclass_LnUIElement_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnUIElement)));
}

static void HSPSubclass_LnUIElement_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UITextBlock

struct HSPSubclass_LnUITextBlock
{
};


static LnSubinstanceId HSPSubclass_LnUITextBlock_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnUITextBlock)));
}

static void HSPSubclass_LnUITextBlock_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UISprite

struct HSPSubclass_LnUISprite
{
};


static LnSubinstanceId HSPSubclass_LnUISprite_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnUISprite)));
}

static void HSPSubclass_LnUISprite_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Input

struct HSPSubclass_LnInput
{
};


static LnSubinstanceId HSPSubclass_LnInput_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnInput)));
}

static void HSPSubclass_LnInput_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Mouse

struct HSPSubclass_LnMouse
{
};


static LnSubinstanceId HSPSubclass_LnMouse_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnMouse)));
}

static void HSPSubclass_LnMouse_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::EngineSettings

struct HSPSubclass_LnEngineSettings
{
};


static LnSubinstanceId HSPSubclass_LnEngineSettings_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnEngineSettings)));
}

static void HSPSubclass_LnEngineSettings_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Engine

struct HSPSubclass_LnEngine
{
};


static LnSubinstanceId HSPSubclass_LnEngine_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnEngine)));
}

static void HSPSubclass_LnEngine_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Application

struct HSPSubclass_LnApplication
{
};


static LnSubinstanceId HSPSubclass_LnApplication_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnApplication)));
}

static void HSPSubclass_LnApplication_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Debug

struct HSPSubclass_LnDebug
{
};


static LnSubinstanceId HSPSubclass_LnDebug_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnDebug)));
}

static void HSPSubclass_LnDebug_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ObjectSerializeHandler

struct HSPSubclass_LnObjectSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnObjectSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnObjectSerializeHandler)));
}

static void HSPSubclass_LnObjectSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnObjectSerializeHandler_DelegateLabelCaller(LnHandle objectserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, objectserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnObjectSerializeHandler*>(LnObjectSerializeHandler_GetSubinstanceId(objectserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// EventConnectionSerializeHandler

struct HSPSubclass_LnEventConnectionSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnEventConnectionSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnEventConnectionSerializeHandler)));
}

static void HSPSubclass_LnEventConnectionSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnEventConnectionSerializeHandler_DelegateLabelCaller(LnHandle eventconnectionserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, eventconnectionserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnEventConnectionSerializeHandler*>(LnEventConnectionSerializeHandler_GetSubinstanceId(eventconnectionserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ZVTestClass1SerializeHandler

struct HSPSubclass_LnZVTestClass1SerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnZVTestClass1SerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnZVTestClass1SerializeHandler)));
}

static void HSPSubclass_LnZVTestClass1SerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnZVTestClass1SerializeHandler_DelegateLabelCaller(LnHandle zvtestclass1serializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, zvtestclass1serializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestClass1SerializeHandler*>(LnZVTestClass1SerializeHandler_GetSubinstanceId(zvtestclass1serializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ZVTestEventArgs1SerializeHandler

struct HSPSubclass_LnZVTestEventArgs1SerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnZVTestEventArgs1SerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnZVTestEventArgs1SerializeHandler)));
}

static void HSPSubclass_LnZVTestEventArgs1SerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnZVTestEventArgs1SerializeHandler_DelegateLabelCaller(LnHandle zvtesteventargs1serializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, zvtesteventargs1serializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestEventArgs1SerializeHandler*>(LnZVTestEventArgs1SerializeHandler_GetSubinstanceId(zvtesteventargs1serializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// Serializer2SerializeHandler

struct HSPSubclass_LnSerializer2SerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnSerializer2SerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnSerializer2SerializeHandler)));
}

static void HSPSubclass_LnSerializer2SerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnSerializer2SerializeHandler_DelegateLabelCaller(LnHandle serializer2serializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, serializer2serializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnSerializer2SerializeHandler*>(LnSerializer2SerializeHandler_GetSubinstanceId(serializer2serializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// AssetModelSerializeHandler

struct HSPSubclass_LnAssetModelSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnAssetModelSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnAssetModelSerializeHandler)));
}

static void HSPSubclass_LnAssetModelSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnAssetModelSerializeHandler_DelegateLabelCaller(LnHandle assetmodelserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, assetmodelserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnAssetModelSerializeHandler*>(LnAssetModelSerializeHandler_GetSubinstanceId(assetmodelserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// TextureSerializeHandler

struct HSPSubclass_LnTextureSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnTextureSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnTextureSerializeHandler)));
}

static void HSPSubclass_LnTextureSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnTextureSerializeHandler_DelegateLabelCaller(LnHandle textureserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, textureserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnTextureSerializeHandler*>(LnTextureSerializeHandler_GetSubinstanceId(textureserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// Texture2DSerializeHandler

struct HSPSubclass_LnTexture2DSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnTexture2DSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnTexture2DSerializeHandler)));
}

static void HSPSubclass_LnTexture2DSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnTexture2DSerializeHandler_DelegateLabelCaller(LnHandle texture2dserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, texture2dserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnTexture2DSerializeHandler*>(LnTexture2DSerializeHandler_GetSubinstanceId(texture2dserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// RenderViewSerializeHandler

struct HSPSubclass_LnRenderViewSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnRenderViewSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnRenderViewSerializeHandler)));
}

static void HSPSubclass_LnRenderViewSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnRenderViewSerializeHandler_DelegateLabelCaller(LnHandle renderviewserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, renderviewserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnRenderViewSerializeHandler*>(LnRenderViewSerializeHandler_GetSubinstanceId(renderviewserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ComponentSerializeHandler

struct HSPSubclass_LnComponentSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnComponentSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnComponentSerializeHandler)));
}

static void HSPSubclass_LnComponentSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnComponentSerializeHandler_DelegateLabelCaller(LnHandle componentserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, componentserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnComponentSerializeHandler*>(LnComponentSerializeHandler_GetSubinstanceId(componentserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// VisualComponentSerializeHandler

struct HSPSubclass_LnVisualComponentSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnVisualComponentSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnVisualComponentSerializeHandler)));
}

static void HSPSubclass_LnVisualComponentSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnVisualComponentSerializeHandler_DelegateLabelCaller(LnHandle visualcomponentserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, visualcomponentserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnVisualComponentSerializeHandler*>(LnVisualComponentSerializeHandler_GetSubinstanceId(visualcomponentserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// SpriteComponentSerializeHandler

struct HSPSubclass_LnSpriteComponentSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnSpriteComponentSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnSpriteComponentSerializeHandler)));
}

static void HSPSubclass_LnSpriteComponentSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnSpriteComponentSerializeHandler_DelegateLabelCaller(LnHandle spritecomponentserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, spritecomponentserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnSpriteComponentSerializeHandler*>(LnSpriteComponentSerializeHandler_GetSubinstanceId(spritecomponentserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// WorldSerializeHandler

struct HSPSubclass_LnWorldSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnWorldSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnWorldSerializeHandler)));
}

static void HSPSubclass_LnWorldSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnWorldSerializeHandler_DelegateLabelCaller(LnHandle worldserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, worldserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnWorldSerializeHandler*>(LnWorldSerializeHandler_GetSubinstanceId(worldserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ComponentListSerializeHandler

struct HSPSubclass_LnComponentListSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnComponentListSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnComponentListSerializeHandler)));
}

static void HSPSubclass_LnComponentListSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnComponentListSerializeHandler_DelegateLabelCaller(LnHandle componentlistserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, componentlistserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnComponentListSerializeHandler*>(LnComponentListSerializeHandler_GetSubinstanceId(componentlistserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// WorldObjectSerializeHandler

struct HSPSubclass_LnWorldObjectSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnWorldObjectSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnWorldObjectSerializeHandler)));
}

static void HSPSubclass_LnWorldObjectSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnWorldObjectSerializeHandler_DelegateLabelCaller(LnHandle worldobjectserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, worldobjectserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnWorldObjectSerializeHandler*>(LnWorldObjectSerializeHandler_GetSubinstanceId(worldobjectserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// WorldObjectUpdateHandler

struct HSPSubclass_LnWorldObjectUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnWorldObjectUpdateHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnWorldObjectUpdateHandler)));
}

static void HSPSubclass_LnWorldObjectUpdateHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnWorldObjectUpdateHandler_DelegateLabelCaller(LnHandle worldobjectupdatehandler, LnHandle self, float elapsedSeconds)
{
    setCallbackArg(0, worldobjectupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnWorldObjectUpdateHandler*>(LnWorldObjectUpdateHandler_GetSubinstanceId(worldobjectupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// VisualObjectSerializeHandler

struct HSPSubclass_LnVisualObjectSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnVisualObjectSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnVisualObjectSerializeHandler)));
}

static void HSPSubclass_LnVisualObjectSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnVisualObjectSerializeHandler_DelegateLabelCaller(LnHandle visualobjectserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, visualobjectserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnVisualObjectSerializeHandler*>(LnVisualObjectSerializeHandler_GetSubinstanceId(visualobjectserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// VisualObjectUpdateHandler

struct HSPSubclass_LnVisualObjectUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnVisualObjectUpdateHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnVisualObjectUpdateHandler)));
}

static void HSPSubclass_LnVisualObjectUpdateHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnVisualObjectUpdateHandler_DelegateLabelCaller(LnHandle visualobjectupdatehandler, LnHandle self, float elapsedSeconds)
{
    setCallbackArg(0, visualobjectupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnVisualObjectUpdateHandler*>(LnVisualObjectUpdateHandler_GetSubinstanceId(visualobjectupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// CameraSerializeHandler

struct HSPSubclass_LnCameraSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnCameraSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnCameraSerializeHandler)));
}

static void HSPSubclass_LnCameraSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnCameraSerializeHandler_DelegateLabelCaller(LnHandle cameraserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, cameraserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnCameraSerializeHandler*>(LnCameraSerializeHandler_GetSubinstanceId(cameraserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// CameraUpdateHandler

struct HSPSubclass_LnCameraUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnCameraUpdateHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnCameraUpdateHandler)));
}

static void HSPSubclass_LnCameraUpdateHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnCameraUpdateHandler_DelegateLabelCaller(LnHandle cameraupdatehandler, LnHandle self, float elapsedSeconds)
{
    setCallbackArg(0, cameraupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnCameraUpdateHandler*>(LnCameraUpdateHandler_GetSubinstanceId(cameraupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// DirectionalLightSerializeHandler

struct HSPSubclass_LnDirectionalLightSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnDirectionalLightSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnDirectionalLightSerializeHandler)));
}

static void HSPSubclass_LnDirectionalLightSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnDirectionalLightSerializeHandler_DelegateLabelCaller(LnHandle directionallightserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, directionallightserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnDirectionalLightSerializeHandler*>(LnDirectionalLightSerializeHandler_GetSubinstanceId(directionallightserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// DirectionalLightUpdateHandler

struct HSPSubclass_LnDirectionalLightUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnDirectionalLightUpdateHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnDirectionalLightUpdateHandler)));
}

static void HSPSubclass_LnDirectionalLightUpdateHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnDirectionalLightUpdateHandler_DelegateLabelCaller(LnHandle directionallightupdatehandler, LnHandle self, float elapsedSeconds)
{
    setCallbackArg(0, directionallightupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnDirectionalLightUpdateHandler*>(LnDirectionalLightUpdateHandler_GetSubinstanceId(directionallightupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// PointLightSerializeHandler

struct HSPSubclass_LnPointLightSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnPointLightSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnPointLightSerializeHandler)));
}

static void HSPSubclass_LnPointLightSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnPointLightSerializeHandler_DelegateLabelCaller(LnHandle pointlightserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, pointlightserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnPointLightSerializeHandler*>(LnPointLightSerializeHandler_GetSubinstanceId(pointlightserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// PointLightUpdateHandler

struct HSPSubclass_LnPointLightUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnPointLightUpdateHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnPointLightUpdateHandler)));
}

static void HSPSubclass_LnPointLightUpdateHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnPointLightUpdateHandler_DelegateLabelCaller(LnHandle pointlightupdatehandler, LnHandle self, float elapsedSeconds)
{
    setCallbackArg(0, pointlightupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnPointLightUpdateHandler*>(LnPointLightUpdateHandler_GetSubinstanceId(pointlightupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// SpotLightSerializeHandler

struct HSPSubclass_LnSpotLightSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnSpotLightSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnSpotLightSerializeHandler)));
}

static void HSPSubclass_LnSpotLightSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnSpotLightSerializeHandler_DelegateLabelCaller(LnHandle spotlightserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, spotlightserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnSpotLightSerializeHandler*>(LnSpotLightSerializeHandler_GetSubinstanceId(spotlightserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// SpotLightUpdateHandler

struct HSPSubclass_LnSpotLightUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnSpotLightUpdateHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnSpotLightUpdateHandler)));
}

static void HSPSubclass_LnSpotLightUpdateHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnSpotLightUpdateHandler_DelegateLabelCaller(LnHandle spotlightupdatehandler, LnHandle self, float elapsedSeconds)
{
    setCallbackArg(0, spotlightupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnSpotLightUpdateHandler*>(LnSpotLightUpdateHandler_GetSubinstanceId(spotlightupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// SpriteSerializeHandler

struct HSPSubclass_LnSpriteSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnSpriteSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnSpriteSerializeHandler)));
}

static void HSPSubclass_LnSpriteSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnSpriteSerializeHandler_DelegateLabelCaller(LnHandle spriteserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, spriteserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnSpriteSerializeHandler*>(LnSpriteSerializeHandler_GetSubinstanceId(spriteserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// SpriteUpdateHandler

struct HSPSubclass_LnSpriteUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnSpriteUpdateHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnSpriteUpdateHandler)));
}

static void HSPSubclass_LnSpriteUpdateHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnSpriteUpdateHandler_DelegateLabelCaller(LnHandle spriteupdatehandler, LnHandle self, float elapsedSeconds)
{
    setCallbackArg(0, spriteupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnSpriteUpdateHandler*>(LnSpriteUpdateHandler_GetSubinstanceId(spriteupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// CameraOrbitControlComponentSerializeHandler

struct HSPSubclass_LnCameraOrbitControlComponentSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnCameraOrbitControlComponentSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnCameraOrbitControlComponentSerializeHandler)));
}

static void HSPSubclass_LnCameraOrbitControlComponentSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnCameraOrbitControlComponentSerializeHandler_DelegateLabelCaller(LnHandle cameraorbitcontrolcomponentserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, cameraorbitcontrolcomponentserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnCameraOrbitControlComponentSerializeHandler*>(LnCameraOrbitControlComponentSerializeHandler_GetSubinstanceId(cameraorbitcontrolcomponentserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// RaycasterSerializeHandler

struct HSPSubclass_LnRaycasterSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnRaycasterSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnRaycasterSerializeHandler)));
}

static void HSPSubclass_LnRaycasterSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnRaycasterSerializeHandler_DelegateLabelCaller(LnHandle raycasterserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, raycasterserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnRaycasterSerializeHandler*>(LnRaycasterSerializeHandler_GetSubinstanceId(raycasterserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// RaycastResultSerializeHandler

struct HSPSubclass_LnRaycastResultSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnRaycastResultSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnRaycastResultSerializeHandler)));
}

static void HSPSubclass_LnRaycastResultSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnRaycastResultSerializeHandler_DelegateLabelCaller(LnHandle raycastresultserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, raycastresultserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnRaycastResultSerializeHandler*>(LnRaycastResultSerializeHandler_GetSubinstanceId(raycastresultserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// WorldRenderViewSerializeHandler

struct HSPSubclass_LnWorldRenderViewSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnWorldRenderViewSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnWorldRenderViewSerializeHandler)));
}

static void HSPSubclass_LnWorldRenderViewSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnWorldRenderViewSerializeHandler_DelegateLabelCaller(LnHandle worldrenderviewserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, worldrenderviewserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnWorldRenderViewSerializeHandler*>(LnWorldRenderViewSerializeHandler_GetSubinstanceId(worldrenderviewserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// BoxMeshSerializeHandler

struct HSPSubclass_LnBoxMeshSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnBoxMeshSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnBoxMeshSerializeHandler)));
}

static void HSPSubclass_LnBoxMeshSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnBoxMeshSerializeHandler_DelegateLabelCaller(LnHandle boxmeshserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, boxmeshserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnBoxMeshSerializeHandler*>(LnBoxMeshSerializeHandler_GetSubinstanceId(boxmeshserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// BoxMeshUpdateHandler

struct HSPSubclass_LnBoxMeshUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnBoxMeshUpdateHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnBoxMeshUpdateHandler)));
}

static void HSPSubclass_LnBoxMeshUpdateHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnBoxMeshUpdateHandler_DelegateLabelCaller(LnHandle boxmeshupdatehandler, LnHandle self, float elapsedSeconds)
{
    setCallbackArg(0, boxmeshupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnBoxMeshUpdateHandler*>(LnBoxMeshUpdateHandler_GetSubinstanceId(boxmeshupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// PlaneMeshSerializeHandler

struct HSPSubclass_LnPlaneMeshSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnPlaneMeshSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnPlaneMeshSerializeHandler)));
}

static void HSPSubclass_LnPlaneMeshSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnPlaneMeshSerializeHandler_DelegateLabelCaller(LnHandle planemeshserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, planemeshserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnPlaneMeshSerializeHandler*>(LnPlaneMeshSerializeHandler_GetSubinstanceId(planemeshserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// PlaneMeshUpdateHandler

struct HSPSubclass_LnPlaneMeshUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnPlaneMeshUpdateHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnPlaneMeshUpdateHandler)));
}

static void HSPSubclass_LnPlaneMeshUpdateHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnPlaneMeshUpdateHandler_DelegateLabelCaller(LnHandle planemeshupdatehandler, LnHandle self, float elapsedSeconds)
{
    setCallbackArg(0, planemeshupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnPlaneMeshUpdateHandler*>(LnPlaneMeshUpdateHandler_GetSubinstanceId(planemeshupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// UIEventArgsSerializeHandler

struct HSPSubclass_LnUIEventArgsSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnUIEventArgsSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnUIEventArgsSerializeHandler)));
}

static void HSPSubclass_LnUIEventArgsSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnUIEventArgsSerializeHandler_DelegateLabelCaller(LnHandle uieventargsserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, uieventargsserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnUIEventArgsSerializeHandler*>(LnUIEventArgsSerializeHandler_GetSubinstanceId(uieventargsserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// UILayoutElementSerializeHandler

struct HSPSubclass_LnUILayoutElementSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnUILayoutElementSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnUILayoutElementSerializeHandler)));
}

static void HSPSubclass_LnUILayoutElementSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnUILayoutElementSerializeHandler_DelegateLabelCaller(LnHandle uilayoutelementserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, uilayoutelementserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnUILayoutElementSerializeHandler*>(LnUILayoutElementSerializeHandler_GetSubinstanceId(uilayoutelementserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// UIElementSerializeHandler

struct HSPSubclass_LnUIElementSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnUIElementSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnUIElementSerializeHandler)));
}

static void HSPSubclass_LnUIElementSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnUIElementSerializeHandler_DelegateLabelCaller(LnHandle uielementserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, uielementserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnUIElementSerializeHandler*>(LnUIElementSerializeHandler_GetSubinstanceId(uielementserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// UITextBlockSerializeHandler

struct HSPSubclass_LnUITextBlockSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnUITextBlockSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnUITextBlockSerializeHandler)));
}

static void HSPSubclass_LnUITextBlockSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnUITextBlockSerializeHandler_DelegateLabelCaller(LnHandle uitextblockserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, uitextblockserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnUITextBlockSerializeHandler*>(LnUITextBlockSerializeHandler_GetSubinstanceId(uitextblockserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// UISpriteSerializeHandler

struct HSPSubclass_LnUISpriteSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnUISpriteSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnUISpriteSerializeHandler)));
}

static void HSPSubclass_LnUISpriteSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnUISpriteSerializeHandler_DelegateLabelCaller(LnHandle uispriteserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, uispriteserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnUISpriteSerializeHandler*>(LnUISpriteSerializeHandler_GetSubinstanceId(uispriteserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ApplicationSerializeHandler

struct HSPSubclass_LnApplicationSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnApplicationSerializeHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnApplicationSerializeHandler)));
}

static void HSPSubclass_LnApplicationSerializeHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnApplicationSerializeHandler_DelegateLabelCaller(LnHandle applicationserializehandler, LnHandle self, LnHandle ar)
{
    setCallbackArg(0, applicationserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnApplicationSerializeHandler*>(LnApplicationSerializeHandler_GetSubinstanceId(applicationserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ApplicationInitHandler

struct HSPSubclass_LnApplicationInitHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnApplicationInitHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnApplicationInitHandler)));
}

static void HSPSubclass_LnApplicationInitHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnApplicationInitHandler_DelegateLabelCaller(LnHandle applicationinithandler, LnHandle self)
{
    setCallbackArg(0, applicationinithandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnApplicationInitHandler*>(LnApplicationInitHandler_GetSubinstanceId(applicationinithandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}

//==============================================================================
// ApplicationUpdateHandler

struct HSPSubclass_LnApplicationUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LnSubinstanceId HSPSubclass_LnApplicationUpdateHandler_SubinstanceAlloc(LnHandle handle)
{
    return reinterpret_cast<LnSubinstanceId>(malloc(sizeof(HSPSubclass_LnApplicationUpdateHandler)));
}

static void HSPSubclass_LnApplicationUpdateHandler_SubinstanceFree(LnHandle handle, LnSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LnResult HSPSubclass_LnApplicationUpdateHandler_DelegateLabelCaller(LnHandle applicationupdatehandler, LnHandle self)
{
    setCallbackArg(0, applicationupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LnApplicationUpdateHandler*>(LnApplicationUpdateHandler_GetSubinstanceId(applicationupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LnResult>(stat);
}
bool Structs_reffunc(int cmd, int* typeRes, void** retValPtr)
{
    g_leadSupport = false;
    switch (cmd) {
        // LnVector3
        case 0xB20 : {
            hspLnVector3_reffunc(typeRes, retValPtr);
            return true;
        }
        // LnVector4
        case 0x1D20 : {
            hspLnVector4_reffunc(typeRes, retValPtr);
            return true;
        }
        // LnQuaternion
        case 0x2820 : {
            hspLnQuaternion_reffunc(typeRes, retValPtr);
            return true;
        }
        // LnMatrix
        case 0x3620 : {
            hspLnMatrix_reffunc(typeRes, retValPtr);
            return true;
        }
        // LnColor
        case 0xAE20 : {
            hspLnColor_reffunc(typeRes, retValPtr);
            return true;
        }
        // LnPoint
        case 0xB920 : {
            hspLnPoint_reffunc(typeRes, retValPtr);
            return true;
        }
        // LnSize
        case 0xC020 : {
            hspLnSize_reffunc(typeRes, retValPtr);
            return true;
        }
        // LnRect
        case 0xC720 : {
            hspLnRect_reffunc(typeRes, retValPtr);
            return true;
        }
        // LnThickness
        case 0xD620 : {
            hspLnThickness_reffunc(typeRes, retValPtr);
            return true;
        }
        // ln_args
        case 0x1 : {
            ln_args_reffunc(typeRes, retValPtr);
            return true;
        }
    }
    return false;
}

bool Commands_cmdfunc(int cmd, int* retVal)
{
    *retVal = RUNMODE_RUN;
    switch (cmd) {
        // LnObject_SetPrototype_OnSerialize
        case 0x48720 : {
            // Fetch object
            const auto local_object = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnObject_SetPrototype_OnSerialize(local_object, local_callback);


            return true;
        }
        // LnEventConnection_SetPrototype_OnSerialize
        case 0x49220 : {
            // Fetch eventconnection
            const auto local_eventconnection = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnEventConnection_SetPrototype_OnSerialize(local_eventconnection, local_callback);


            return true;
        }
        // LnPromiseFailureDelegate_Create
        case 0x2AB20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outPromiseFailureDelegate
            PVal* pval_outPromiseFailureDelegate;
            const APTR aptr_outPromiseFailureDelegate = code_getva(&pval_outPromiseFailureDelegate);
            LnHandle local_outPromiseFailureDelegate;

            stat = LnPromiseFailureDelegate_Create(HSPSubclass_LnPromiseFailureDelegate_DelegateLabelCaller, &local_outPromiseFailureDelegate);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnPromiseFailureDelegate*>(LnPromiseFailureDelegate_GetSubinstanceId(local_outPromiseFailureDelegate));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outPromiseFailureDelegate, aptr_outPromiseFailureDelegate, local_outPromiseFailureDelegate);

            return true;
        }
        // LnZVTestDelegate1_Create
        case 0x2B020 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestDelegate1
            PVal* pval_outZVTestDelegate1;
            const APTR aptr_outZVTestDelegate1 = code_getva(&pval_outZVTestDelegate1);
            LnHandle local_outZVTestDelegate1;

            stat = LnZVTestDelegate1_Create(HSPSubclass_LnZVTestDelegate1_DelegateLabelCaller, &local_outZVTestDelegate1);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestDelegate1*>(LnZVTestDelegate1_GetSubinstanceId(local_outZVTestDelegate1));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestDelegate1, aptr_outZVTestDelegate1, local_outZVTestDelegate1);

            return true;
        }
        // LnZVTestDelegate2_Create
        case 0x2B620 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestDelegate2
            PVal* pval_outZVTestDelegate2;
            const APTR aptr_outZVTestDelegate2 = code_getva(&pval_outZVTestDelegate2);
            LnHandle local_outZVTestDelegate2;

            stat = LnZVTestDelegate2_Create(HSPSubclass_LnZVTestDelegate2_DelegateLabelCaller, &local_outZVTestDelegate2);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestDelegate2*>(LnZVTestDelegate2_GetSubinstanceId(local_outZVTestDelegate2));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestDelegate2, aptr_outZVTestDelegate2, local_outZVTestDelegate2);

            return true;
        }
        // LnZVTestDelegate3_Create
        case 0x2BB20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestDelegate3
            PVal* pval_outZVTestDelegate3;
            const APTR aptr_outZVTestDelegate3 = code_getva(&pval_outZVTestDelegate3);
            LnHandle local_outZVTestDelegate3;

            stat = LnZVTestDelegate3_Create(HSPSubclass_LnZVTestDelegate3_DelegateLabelCaller, &local_outZVTestDelegate3);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestDelegate3*>(LnZVTestDelegate3_GetSubinstanceId(local_outZVTestDelegate3));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestDelegate3, aptr_outZVTestDelegate3, local_outZVTestDelegate3);

            return true;
        }
        // LnZVTestEventHandler1_Create
        case 0x2C020 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestEventHandler1
            PVal* pval_outZVTestEventHandler1;
            const APTR aptr_outZVTestEventHandler1 = code_getva(&pval_outZVTestEventHandler1);
            LnHandle local_outZVTestEventHandler1;

            stat = LnZVTestEventHandler1_Create(HSPSubclass_LnZVTestEventHandler1_DelegateLabelCaller, &local_outZVTestEventHandler1);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestEventHandler1*>(LnZVTestEventHandler1_GetSubinstanceId(local_outZVTestEventHandler1));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestEventHandler1, aptr_outZVTestEventHandler1, local_outZVTestEventHandler1);

            return true;
        }
        // LnZVTestEventHandler2_Create
        case 0x2C520 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestEventHandler2
            PVal* pval_outZVTestEventHandler2;
            const APTR aptr_outZVTestEventHandler2 = code_getva(&pval_outZVTestEventHandler2);
            LnHandle local_outZVTestEventHandler2;

            stat = LnZVTestEventHandler2_Create(HSPSubclass_LnZVTestEventHandler2_DelegateLabelCaller, &local_outZVTestEventHandler2);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestEventHandler2*>(LnZVTestEventHandler2_GetSubinstanceId(local_outZVTestEventHandler2));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestEventHandler2, aptr_outZVTestEventHandler2, local_outZVTestEventHandler2);

            return true;
        }
        // LnZVTestPromise1_ThenWith
        case 0x2C820 : {
            // Fetch zvtestpromise1
            const auto local_zvtestpromise1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnZVTestPromise1_ThenWith(local_zvtestpromise1, local_callback);


            return true;
        }
        // LnZVTestPromise1_CatchWith
        case 0x2CA20 : {
            // Fetch zvtestpromise1
            const auto local_zvtestpromise1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnZVTestPromise1_CatchWith(local_zvtestpromise1, local_callback);


            return true;
        }
        // LnZVTestPromise2_ThenWith
        case 0x2CE20 : {
            // Fetch zvtestpromise2
            const auto local_zvtestpromise2 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnZVTestPromise2_ThenWith(local_zvtestpromise2, local_callback);


            return true;
        }
        // LnZVTestPromise2_CatchWith
        case 0x2D020 : {
            // Fetch zvtestpromise2
            const auto local_zvtestpromise2 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnZVTestPromise2_CatchWith(local_zvtestpromise2, local_callback);


            return true;
        }
        // LnZVTestClass1_Create
        case 0x7B20 : {
            // Fetch outZVTestClass1
            PVal* pval_outZVTestClass1;
            const APTR aptr_outZVTestClass1 = code_getva(&pval_outZVTestClass1);
            LnHandle local_outZVTestClass1;

            stat = LnZVTestClass1_Create(&local_outZVTestClass1);
            setVAInt(pval_outZVTestClass1, aptr_outZVTestClass1, local_outZVTestClass1);

            return true;
        }
        // LnZVTestClass1_SetTestDelegate1
        case 0x6520 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LnZVTestClass1_SetTestDelegate1(local_zvtestclass1, local_value);


            return true;
        }
        // LnZVTestClass1_SetTestDelegate2
        case 0x6720 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LnZVTestClass1_SetTestDelegate2(local_zvtestclass1, local_value);


            return true;
        }
        // LnZVTestClass1_SetTestDelegate3
        case 0x6920 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LnZVTestClass1_SetTestDelegate3(local_zvtestclass1, local_value);


            return true;
        }
        // LnZVTestClass1_CallTestDelegate1
        case 0x6B20 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch a
            const auto local_a = fetchVAInt();

            stat = LnZVTestClass1_CallTestDelegate1(local_zvtestclass1, local_a);


            return true;
        }
        // LnZVTestClass1_CallTestDelegate2
        case 0x6D20 : {
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

            return true;
        }
        // LnZVTestClass1_CallTestDelegate3
        case 0x7020 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();

            stat = LnZVTestClass1_CallTestDelegate3(local_zvtestclass1);


            return true;
        }
        // LnZVTestClass1_LoadAsyncA
        case 0x7120 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnZVTestClass1_LoadAsyncA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnZVTestClass1_ExecuteAsync
        case 0x7320 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnZVTestClass1_ExecuteAsync(local_zvtestclass1, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnZVTestClass1_GetFilePathA
        case 0x7420 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LnZVTestClass1_GetFilePathA(local_zvtestclass1, &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnZVTestClass1_ConnectOnEvent1
        case 0x7520 : {
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

            return true;
        }
        // LnZVTestClass1_RaiseEvent1
        case 0x7720 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();

            stat = LnZVTestClass1_RaiseEvent1(local_zvtestclass1);


            return true;
        }
        // LnZVTestClass1_ConnectOnEvent2
        case 0x7820 : {
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

            return true;
        }
        // LnZVTestClass1_RaiseEvent2
        case 0x7A20 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();

            stat = LnZVTestClass1_RaiseEvent2(local_zvtestclass1);


            return true;
        }
        // LnZVTestClass1_SetPrototype_OnSerialize
        case 0x49D20 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnZVTestClass1_SetPrototype_OnSerialize(local_zvtestclass1, local_callback);


            return true;
        }
        // LnZVTestEventArgs1_Create
        case 0x7E20 : {
            // Fetch outZVTestEventArgs1
            PVal* pval_outZVTestEventArgs1;
            const APTR aptr_outZVTestEventArgs1 = code_getva(&pval_outZVTestEventArgs1);
            LnHandle local_outZVTestEventArgs1;

            stat = LnZVTestEventArgs1_Create(&local_outZVTestEventArgs1);
            setVAInt(pval_outZVTestEventArgs1, aptr_outZVTestEventArgs1, local_outZVTestEventArgs1);

            return true;
        }
        // LnZVTestEventArgs1_CreateWithValue
        case 0x7F20 : {
            // Fetch v
            const auto local_v = fetchVAInt();
            // Fetch outZVTestEventArgs1
            PVal* pval_outZVTestEventArgs1;
            const APTR aptr_outZVTestEventArgs1 = code_getva(&pval_outZVTestEventArgs1);
            LnHandle local_outZVTestEventArgs1;

            stat = LnZVTestEventArgs1_CreateWithValue(local_v, &local_outZVTestEventArgs1);
            setVAInt(pval_outZVTestEventArgs1, aptr_outZVTestEventArgs1, local_outZVTestEventArgs1);

            return true;
        }
        // LnZVTestEventArgs1_GetValue
        case 0x7D20 : {
            // Fetch zvtesteventargs1
            const auto local_zvtesteventargs1 = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LnZVTestEventArgs1_GetValue(local_zvtesteventargs1, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnZVTestEventArgs1_SetPrototype_OnSerialize
        case 0x4A820 : {
            // Fetch zvtesteventargs1
            const auto local_zvtesteventargs1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnZVTestEventArgs1_SetPrototype_OnSerialize(local_zvtesteventargs1, local_callback);


            return true;
        }
        // LnSerializer2_SetPrototype_OnSerialize
        case 0x4B320 : {
            // Fetch serializer2
            const auto local_serializer2 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnSerializer2_SetPrototype_OnSerialize(local_serializer2, local_callback);


            return true;
        }
        // LnAssetModel_Create
        case 0x8B20 : {
            // Fetch target
            const auto local_target = fetchVAInt();
            // Fetch outAssetModel
            PVal* pval_outAssetModel;
            const APTR aptr_outAssetModel = code_getva(&pval_outAssetModel);
            LnHandle local_outAssetModel;

            stat = LnAssetModel_Create(local_target, &local_outAssetModel);
            setVAInt(pval_outAssetModel, aptr_outAssetModel, local_outAssetModel);

            return true;
        }
        // LnAssetModel_Target
        case 0x8A20 : {
            // Fetch assetmodel
            const auto local_assetmodel = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnAssetModel_Target(local_assetmodel, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnAssetModel_SetPrototype_OnSerialize
        case 0x4BE20 : {
            // Fetch assetmodel
            const auto local_assetmodel = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnAssetModel_SetPrototype_OnSerialize(local_assetmodel, local_callback);


            return true;
        }
        // LnAssets_SaveAssetToLocalFileA
        case 0x8E20 : {
            // Fetch asset
            const auto local_asset = fetchVAInt();
            // Fetch filePath
            const auto local_filePath = fetchVAString();

            stat = LnAssets_SaveAssetToLocalFileA(local_asset, local_filePath);


            return true;
        }
        // LnAssets_LoadAssetFromLocalFileA
        case 0x9120 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnAssets_LoadAssetFromLocalFileA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnAssets_LoadAssetA
        case 0x9320 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnAssets_LoadAssetA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnAssets_ReloadAssetA
        case 0x9520 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch obj
            const auto local_obj = fetchVAInt();

            stat = LnAssets_ReloadAssetA(local_filePath, local_obj);


            return true;
        }
        // LnTexture2DDelegate_Create
        case 0x2F420 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outTexture2DDelegate
            PVal* pval_outTexture2DDelegate;
            const APTR aptr_outTexture2DDelegate = code_getva(&pval_outTexture2DDelegate);
            LnHandle local_outTexture2DDelegate;

            stat = LnTexture2DDelegate_Create(HSPSubclass_LnTexture2DDelegate_DelegateLabelCaller, &local_outTexture2DDelegate);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnTexture2DDelegate*>(LnTexture2DDelegate_GetSubinstanceId(local_outTexture2DDelegate));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outTexture2DDelegate, aptr_outTexture2DDelegate, local_outTexture2DDelegate);

            return true;
        }
        // LnTexture2DPromise_ThenWith
        case 0x2F720 : {
            // Fetch texture2dpromise
            const auto local_texture2dpromise = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnTexture2DPromise_ThenWith(local_texture2dpromise, local_callback);


            return true;
        }
        // LnTexture2DPromise_CatchWith
        case 0x2F920 : {
            // Fetch texture2dpromise
            const auto local_texture2dpromise = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnTexture2DPromise_CatchWith(local_texture2dpromise, local_callback);


            return true;
        }
        // LnTexture_SetPrototype_OnSerialize
        case 0x4C920 : {
            // Fetch texture
            const auto local_texture = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnTexture_SetPrototype_OnSerialize(local_texture, local_callback);


            return true;
        }
        // LnTexture2D_Create
        case 0xE720 : {
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

            return true;
        }
        // LnTexture2D_CreateWithFormat
        case 0xEA20 : {
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

            return true;
        }
        // LnTexture2D_LoadA
        case 0xE320 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnTexture2D_LoadA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnTexture2D_LoadEmojiA
        case 0xE520 : {
            // Fetch code
            const auto local_code = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnTexture2D_LoadEmojiA(local_code, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnTexture2D_SetPrototype_OnSerialize
        case 0x4D420 : {
            // Fetch texture2d
            const auto local_texture2d = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnTexture2D_SetPrototype_OnSerialize(local_texture2d, local_callback);


            return true;
        }
        // LnRenderView_SetPrototype_OnSerialize
        case 0x4DF20 : {
            // Fetch renderview
            const auto local_renderview = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnRenderView_SetPrototype_OnSerialize(local_renderview, local_callback);


            return true;
        }
        // LnComponent_SetPrototype_OnSerialize
        case 0x4EA20 : {
            // Fetch component
            const auto local_component = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnComponent_SetPrototype_OnSerialize(local_component, local_callback);


            return true;
        }
        // LnVisualComponent_SetVisible
        case 0xF120 : {
            // Fetch visualcomponent
            const auto local_visualcomponent = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LnBool>(fetchVAInt());

            stat = LnVisualComponent_SetVisible(local_visualcomponent, local_value);


            return true;
        }
        // LnVisualComponent_IsVisible
        case 0xF320 : {
            // Fetch visualcomponent
            const auto local_visualcomponent = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnVisualComponent_IsVisible(local_visualcomponent, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnVisualComponent_SetPrototype_OnSerialize
        case 0x4F520 : {
            // Fetch visualcomponent
            const auto local_visualcomponent = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnVisualComponent_SetPrototype_OnSerialize(local_visualcomponent, local_callback);


            return true;
        }
        // LnSpriteComponent_SetTexture
        case 0xF520 : {
            // Fetch spritecomponent
            const auto local_spritecomponent = fetchVAInt();
            // Fetch texture
            const auto local_texture = fetchVAInt();

            stat = LnSpriteComponent_SetTexture(local_spritecomponent, local_texture);


            return true;
        }
        // LnSpriteComponent_SetPrototype_OnSerialize
        case 0x50020 : {
            // Fetch spritecomponent
            const auto local_spritecomponent = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnSpriteComponent_SetPrototype_OnSerialize(local_spritecomponent, local_callback);


            return true;
        }
        // LnWorld_Add
        case 0xF820 : {
            // Fetch world
            const auto local_world = fetchVAInt();
            // Fetch obj
            const auto local_obj = fetchVAInt();

            stat = LnWorld_Add(local_world, local_obj);


            return true;
        }
        // LnWorld_SetPrototype_OnSerialize
        case 0x50B20 : {
            // Fetch world
            const auto local_world = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnWorld_SetPrototype_OnSerialize(local_world, local_callback);


            return true;
        }
        // LnComponentList_GetLength
        case 0x38820 : {
            // Fetch componentlist
            const auto local_componentlist = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LnComponentList_GetLength(local_componentlist, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnComponentList_GetItem
        case 0x38A20 : {
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

            return true;
        }
        // LnComponentList_SetPrototype_OnSerialize
        case 0x51620 : {
            // Fetch componentlist
            const auto local_componentlist = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnComponentList_SetPrototype_OnSerialize(local_componentlist, local_callback);


            return true;
        }
        // LnWorldObject_SetPosition
        case 0xFC20 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch pos
            PVal* pval_pos;
            CodeGetVA_TypeChecked(&pval_pos, LnVector3);

            stat = LnWorldObject_SetPosition(local_worldobject, reinterpret_cast<const LnVector3*>(pval_pos->pt));


            return true;
        }
        // LnWorldObject_SetPositionXYZ
        case 0xFE20 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnWorldObject_SetPositionXYZ(local_worldobject, local_x, local_y, local_z);


            return true;
        }
        // LnWorldObject_GetPosition
        case 0x10220 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnWorldObject_GetPosition(local_worldobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            return true;
        }
        // LnWorldObject_SetRotationQuaternion
        case 0x10320 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch rot
            PVal* pval_rot;
            CodeGetVA_TypeChecked(&pval_rot, LnQuaternion);

            stat = LnWorldObject_SetRotationQuaternion(local_worldobject, reinterpret_cast<const LnQuaternion*>(pval_rot->pt));


            return true;
        }
        // LnWorldObject_SetRotation
        case 0x10520 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnWorldObject_SetRotation(local_worldobject, local_x, local_y, local_z);


            return true;
        }
        // LnWorldObject_GetRotation
        case 0x10920 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnQuaternion local_outReturn;

            stat = LnWorldObject_GetRotation(local_worldobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnQuaternion_typeid(), &local_outReturn);

            return true;
        }
        // LnWorldObject_SetScale
        case 0x10A20 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch scale
            PVal* pval_scale;
            CodeGetVA_TypeChecked(&pval_scale, LnVector3);

            stat = LnWorldObject_SetScale(local_worldobject, reinterpret_cast<const LnVector3*>(pval_scale->pt));


            return true;
        }
        // LnWorldObject_SetScaleS
        case 0x10C20 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch xyz
            const auto local_xyz = fetchVADouble();

            stat = LnWorldObject_SetScaleS(local_worldobject, local_xyz);


            return true;
        }
        // LnWorldObject_SetScaleXYZ
        case 0x10E20 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnWorldObject_SetScaleXYZ(local_worldobject, local_x, local_y, local_z);


            return true;
        }
        // LnWorldObject_GetScale
        case 0x11220 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnWorldObject_GetScale(local_worldobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            return true;
        }
        // LnWorldObject_SetCenterPoint
        case 0x11320 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LnVector3);

            stat = LnWorldObject_SetCenterPoint(local_worldobject, reinterpret_cast<const LnVector3*>(pval_value->pt));


            return true;
        }
        // LnWorldObject_SetCenterPointXYZ
        case 0x11520 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnWorldObject_SetCenterPointXYZ(local_worldobject, local_x, local_y, local_z);


            return true;
        }
        // LnWorldObject_GetCenterPoint
        case 0x11920 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnWorldObject_GetCenterPoint(local_worldobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            return true;
        }
        // LnWorldObject_LookAt
        case 0x11A20 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch target
            PVal* pval_target;
            CodeGetVA_TypeChecked(&pval_target, LnVector3);

            stat = LnWorldObject_LookAt(local_worldobject, reinterpret_cast<const LnVector3*>(pval_target->pt));


            return true;
        }
        // LnWorldObject_LookAtXYZ
        case 0x11C20 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnWorldObject_LookAtXYZ(local_worldobject, local_x, local_y, local_z);


            return true;
        }
        // LnWorldObject_AddComponent
        case 0x12020 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch component
            const auto local_component = fetchVAInt();

            stat = LnWorldObject_AddComponent(local_worldobject, local_component);


            return true;
        }
        // LnWorldObject_GetComponents
        case 0x12220 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnWorldObject_GetComponents(local_worldobject, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnWorldObject_SetPrototype_OnSerialize
        case 0x52120 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnWorldObject_SetPrototype_OnSerialize(local_worldobject, local_callback);


            return true;
        }
        // LnWorldObject_SetPrototype_OnUpdate
        case 0x52C20 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnWorldObject_SetPrototype_OnUpdate(local_worldobject, local_callback);


            return true;
        }
        // LnVisualObject_SetVisible
        case 0x12620 : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LnBool>(fetchVAInt());

            stat = LnVisualObject_SetVisible(local_visualobject, local_value);


            return true;
        }
        // LnVisualObject_IsVisible
        case 0x12820 : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnVisualObject_IsVisible(local_visualobject, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnVisualObject_SetPrototype_OnSerialize
        case 0x53720 : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnVisualObject_SetPrototype_OnSerialize(local_visualobject, local_callback);


            return true;
        }
        // LnVisualObject_SetPrototype_OnUpdate
        case 0x54220 : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnVisualObject_SetPrototype_OnUpdate(local_visualobject, local_callback);


            return true;
        }
        // LnCamera_SetPrototype_OnSerialize
        case 0x54D20 : {
            // Fetch camera
            const auto local_camera = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnCamera_SetPrototype_OnSerialize(local_camera, local_callback);


            return true;
        }
        // LnCamera_SetPrototype_OnUpdate
        case 0x55820 : {
            // Fetch camera
            const auto local_camera = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnCamera_SetPrototype_OnUpdate(local_camera, local_callback);


            return true;
        }
        // LnDirectionalLight_Create
        case 0x13A20 : {
            // Fetch outDirectionalLight
            PVal* pval_outDirectionalLight;
            const APTR aptr_outDirectionalLight = code_getva(&pval_outDirectionalLight);
            LnHandle local_outDirectionalLight;

            stat = LnDirectionalLight_Create(&local_outDirectionalLight);
            setVAInt(pval_outDirectionalLight, aptr_outDirectionalLight, local_outDirectionalLight);

            return true;
        }
        // LnDirectionalLight_CreateWithColor
        case 0x13B20 : {
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LnColor);
            // Fetch outDirectionalLight
            PVal* pval_outDirectionalLight;
            const APTR aptr_outDirectionalLight = code_getva(&pval_outDirectionalLight);
            LnHandle local_outDirectionalLight;

            stat = LnDirectionalLight_CreateWithColor(reinterpret_cast<const LnColor*>(pval_color->pt), &local_outDirectionalLight);
            setVAInt(pval_outDirectionalLight, aptr_outDirectionalLight, local_outDirectionalLight);

            return true;
        }
        // LnDirectionalLight_SetEnabled
        case 0x12B20 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LnBool>(fetchVAInt());

            stat = LnDirectionalLight_SetEnabled(local_directionallight, local_enabled);


            return true;
        }
        // LnDirectionalLight_IsEnabled
        case 0x12D20 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnDirectionalLight_IsEnabled(local_directionallight, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnDirectionalLight_SetColor
        case 0x12E20 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LnColor);

            stat = LnDirectionalLight_SetColor(local_directionallight, reinterpret_cast<const LnColor*>(pval_color->pt));


            return true;
        }
        // LnDirectionalLight_GetColor
        case 0x13020 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnColor local_outReturn;

            stat = LnDirectionalLight_GetColor(local_directionallight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnColor_typeid(), &local_outReturn);

            return true;
        }
        // LnDirectionalLight_SetIntensity
        case 0x13120 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LnDirectionalLight_SetIntensity(local_directionallight, local_intensity);


            return true;
        }
        // LnDirectionalLight_GetIntensity
        case 0x13320 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnDirectionalLight_GetIntensity(local_directionallight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnDirectionalLight_SetShadowEffectiveDistance
        case 0x13420 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LnDirectionalLight_SetShadowEffectiveDistance(local_directionallight, local_value);


            return true;
        }
        // LnDirectionalLight_GetShadowEffectiveDistance
        case 0x13620 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnDirectionalLight_GetShadowEffectiveDistance(local_directionallight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnDirectionalLight_SetShadowEffectiveDepth
        case 0x13720 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LnDirectionalLight_SetShadowEffectiveDepth(local_directionallight, local_value);


            return true;
        }
        // LnDirectionalLight_GetShadowEffectiveDepth
        case 0x13920 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnDirectionalLight_GetShadowEffectiveDepth(local_directionallight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnDirectionalLight_SetPrototype_OnSerialize
        case 0x56320 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnDirectionalLight_SetPrototype_OnSerialize(local_directionallight, local_callback);


            return true;
        }
        // LnDirectionalLight_SetPrototype_OnUpdate
        case 0x56E20 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnDirectionalLight_SetPrototype_OnUpdate(local_directionallight, local_callback);


            return true;
        }
        // LnPointLight_Create
        case 0x14D20 : {
            // Fetch outPointLight
            PVal* pval_outPointLight;
            const APTR aptr_outPointLight = code_getva(&pval_outPointLight);
            LnHandle local_outPointLight;

            stat = LnPointLight_Create(&local_outPointLight);
            setVAInt(pval_outPointLight, aptr_outPointLight, local_outPointLight);

            return true;
        }
        // LnPointLight_CreateWithColorAndRange
        case 0x14E20 : {
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

            return true;
        }
        // LnPointLight_SetEnabled
        case 0x13E20 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LnBool>(fetchVAInt());

            stat = LnPointLight_SetEnabled(local_pointlight, local_enabled);


            return true;
        }
        // LnPointLight_IsEnabled
        case 0x14020 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnPointLight_IsEnabled(local_pointlight, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnPointLight_SetColor
        case 0x14120 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LnColor);

            stat = LnPointLight_SetColor(local_pointlight, reinterpret_cast<const LnColor*>(pval_color->pt));


            return true;
        }
        // LnPointLight_GetColor
        case 0x14320 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnColor local_outReturn;

            stat = LnPointLight_GetColor(local_pointlight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnColor_typeid(), &local_outReturn);

            return true;
        }
        // LnPointLight_SetIntensity
        case 0x14420 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LnPointLight_SetIntensity(local_pointlight, local_intensity);


            return true;
        }
        // LnPointLight_GetIntensity
        case 0x14620 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnPointLight_GetIntensity(local_pointlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnPointLight_SetRange
        case 0x14720 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch range
            const auto local_range = fetchVADouble();

            stat = LnPointLight_SetRange(local_pointlight, local_range);


            return true;
        }
        // LnPointLight_GetRange
        case 0x14920 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnPointLight_GetRange(local_pointlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnPointLight_SetAttenuation
        case 0x14A20 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch attenuation
            const auto local_attenuation = fetchVADouble();

            stat = LnPointLight_SetAttenuation(local_pointlight, local_attenuation);


            return true;
        }
        // LnPointLight_GetAttenuation
        case 0x14C20 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnPointLight_GetAttenuation(local_pointlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnPointLight_SetPrototype_OnSerialize
        case 0x57920 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnPointLight_SetPrototype_OnSerialize(local_pointlight, local_callback);


            return true;
        }
        // LnPointLight_SetPrototype_OnUpdate
        case 0x58420 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnPointLight_SetPrototype_OnUpdate(local_pointlight, local_callback);


            return true;
        }
        // LnSpotLight_Create
        case 0x16720 : {
            // Fetch outSpotLight
            PVal* pval_outSpotLight;
            const APTR aptr_outSpotLight = code_getva(&pval_outSpotLight);
            LnHandle local_outSpotLight;

            stat = LnSpotLight_Create(&local_outSpotLight);
            setVAInt(pval_outSpotLight, aptr_outSpotLight, local_outSpotLight);

            return true;
        }
        // LnSpotLight_CreateWithColorAndRange
        case 0x16820 : {
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

            return true;
        }
        // LnSpotLight_SetEnabled
        case 0x15220 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LnBool>(fetchVAInt());

            stat = LnSpotLight_SetEnabled(local_spotlight, local_enabled);


            return true;
        }
        // LnSpotLight_IsEnabled
        case 0x15420 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnSpotLight_IsEnabled(local_spotlight, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnSpotLight_SetColor
        case 0x15520 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LnColor);

            stat = LnSpotLight_SetColor(local_spotlight, reinterpret_cast<const LnColor*>(pval_color->pt));


            return true;
        }
        // LnSpotLight_GetColor
        case 0x15720 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnColor local_outReturn;

            stat = LnSpotLight_GetColor(local_spotlight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnColor_typeid(), &local_outReturn);

            return true;
        }
        // LnSpotLight_SetIntensity
        case 0x15820 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LnSpotLight_SetIntensity(local_spotlight, local_intensity);


            return true;
        }
        // LnSpotLight_GetIntensity
        case 0x15A20 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetIntensity(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnSpotLight_SetRange
        case 0x15B20 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch range
            const auto local_range = fetchVADouble();

            stat = LnSpotLight_SetRange(local_spotlight, local_range);


            return true;
        }
        // LnSpotLight_GetRange
        case 0x15D20 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetRange(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnSpotLight_SetAttenuation
        case 0x15E20 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch attenuation
            const auto local_attenuation = fetchVADouble();

            stat = LnSpotLight_SetAttenuation(local_spotlight, local_attenuation);


            return true;
        }
        // LnSpotLight_GetAttenuation
        case 0x16020 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetAttenuation(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnSpotLight_SetAngle
        case 0x16120 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch angle
            const auto local_angle = fetchVADouble();

            stat = LnSpotLight_SetAngle(local_spotlight, local_angle);


            return true;
        }
        // LnSpotLight_GetAngle
        case 0x16320 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetAngle(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnSpotLight_SetPenumbra
        case 0x16420 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch penumbra
            const auto local_penumbra = fetchVADouble();

            stat = LnSpotLight_SetPenumbra(local_spotlight, local_penumbra);


            return true;
        }
        // LnSpotLight_GetPenumbra
        case 0x16620 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnSpotLight_GetPenumbra(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnSpotLight_SetPrototype_OnSerialize
        case 0x58F20 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnSpotLight_SetPrototype_OnSerialize(local_spotlight, local_callback);


            return true;
        }
        // LnSpotLight_SetPrototype_OnUpdate
        case 0x59A20 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnSpotLight_SetPrototype_OnUpdate(local_spotlight, local_callback);


            return true;
        }
        // LnTestDelegate_Create
        case 0x40520 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outTestDelegate
            PVal* pval_outTestDelegate;
            const APTR aptr_outTestDelegate = code_getva(&pval_outTestDelegate);
            LnHandle local_outTestDelegate;

            stat = LnTestDelegate_Create(HSPSubclass_LnTestDelegate_DelegateLabelCaller, &local_outTestDelegate);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnTestDelegate*>(LnTestDelegate_GetSubinstanceId(local_outTestDelegate));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outTestDelegate, aptr_outTestDelegate, local_outTestDelegate);

            return true;
        }
        // LnSprite_Create
        case 0x17E20 : {
            // Fetch outSprite
            PVal* pval_outSprite;
            const APTR aptr_outSprite = code_getva(&pval_outSprite);
            LnHandle local_outSprite;

            stat = LnSprite_Create(&local_outSprite);
            setVAInt(pval_outSprite, aptr_outSprite, local_outSprite);

            return true;
        }
        // LnSprite_CreateWithTexture
        case 0x17F20 : {
            // Fetch texture
            const auto local_texture = fetchVAInt();
            // Fetch outSprite
            PVal* pval_outSprite;
            const APTR aptr_outSprite = code_getva(&pval_outSprite);
            LnHandle local_outSprite;

            stat = LnSprite_CreateWithTexture(local_texture, &local_outSprite);
            setVAInt(pval_outSprite, aptr_outSprite, local_outSprite);

            return true;
        }
        // LnSprite_CreateWithTextureAndSize
        case 0x18120 : {
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

            return true;
        }
        // LnSprite_SetTexture
        case 0x17020 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LnSprite_SetTexture(local_sprite, local_value);


            return true;
        }
        // LnSprite_SetSize
        case 0x17220 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LnSize);

            stat = LnSprite_SetSize(local_sprite, reinterpret_cast<const LnSize*>(pval_value->pt));


            return true;
        }
        // LnSprite_SetSizeWH
        case 0x17420 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch width
            const auto local_width = fetchVADouble();
            // Fetch height
            const auto local_height = fetchVADouble();

            stat = LnSprite_SetSizeWH(local_sprite, local_width, local_height);


            return true;
        }
        // LnSprite_SetSourceRectXYWH
        case 0x17720 : {
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


            return true;
        }
        // LnSprite_SetCallerTest
        case 0x17C20 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnSprite_SetCallerTest(local_sprite, local_callback);


            return true;
        }
        // LnSprite_SetPrototype_OnSerialize
        case 0x5A520 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnSprite_SetPrototype_OnSerialize(local_sprite, local_callback);


            return true;
        }
        // LnSprite_SetPrototype_OnUpdate
        case 0x5B020 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnSprite_SetPrototype_OnUpdate(local_sprite, local_callback);


            return true;
        }
        // LnCameraOrbitControlComponent_Create
        case 0x18620 : {
            // Fetch outCameraOrbitControlComponent
            PVal* pval_outCameraOrbitControlComponent;
            const APTR aptr_outCameraOrbitControlComponent = code_getva(&pval_outCameraOrbitControlComponent);
            LnHandle local_outCameraOrbitControlComponent;

            stat = LnCameraOrbitControlComponent_Create(&local_outCameraOrbitControlComponent);
            setVAInt(pval_outCameraOrbitControlComponent, aptr_outCameraOrbitControlComponent, local_outCameraOrbitControlComponent);

            return true;
        }
        // LnCameraOrbitControlComponent_SetPrototype_OnSerialize
        case 0x5BB20 : {
            // Fetch cameraorbitcontrolcomponent
            const auto local_cameraorbitcontrolcomponent = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnCameraOrbitControlComponent_SetPrototype_OnSerialize(local_cameraorbitcontrolcomponent, local_callback);


            return true;
        }
        // LnRaycaster_FromScreen
        case 0x18820 : {
            // Fetch point
            PVal* pval_point;
            CodeGetVA_TypeChecked(&pval_point, LnPoint);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnRaycaster_FromScreen(reinterpret_cast<const LnPoint*>(pval_point->pt), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnRaycaster_IntersectPlane
        case 0x18A20 : {
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

            return true;
        }
        // LnRaycaster_SetPrototype_OnSerialize
        case 0x5C620 : {
            // Fetch raycaster
            const auto local_raycaster = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnRaycaster_SetPrototype_OnSerialize(local_raycaster, local_callback);


            return true;
        }
        // LnRaycastResult_GetPoint
        case 0x18F20 : {
            // Fetch raycastresult
            const auto local_raycastresult = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnRaycastResult_GetPoint(local_raycastresult, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            return true;
        }
        // LnRaycastResult_SetPrototype_OnSerialize
        case 0x5D120 : {
            // Fetch raycastresult
            const auto local_raycastresult = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnRaycastResult_SetPrototype_OnSerialize(local_raycastresult, local_callback);


            return true;
        }
        // LnWorldRenderView_SetGuideGridEnabled
        case 0x19120 : {
            // Fetch worldrenderview
            const auto local_worldrenderview = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LnBool>(fetchVAInt());

            stat = LnWorldRenderView_SetGuideGridEnabled(local_worldrenderview, local_value);


            return true;
        }
        // LnWorldRenderView_GetGuideGridEnabled
        case 0x19320 : {
            // Fetch worldrenderview
            const auto local_worldrenderview = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnWorldRenderView_GetGuideGridEnabled(local_worldrenderview, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnWorldRenderView_SetPrototype_OnSerialize
        case 0x5DC20 : {
            // Fetch worldrenderview
            const auto local_worldrenderview = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnWorldRenderView_SetPrototype_OnSerialize(local_worldrenderview, local_callback);


            return true;
        }
        // LnBoxMesh_Create
        case 0x19520 : {
            // Fetch outBoxMesh
            PVal* pval_outBoxMesh;
            const APTR aptr_outBoxMesh = code_getva(&pval_outBoxMesh);
            LnHandle local_outBoxMesh;

            stat = LnBoxMesh_Create(&local_outBoxMesh);
            setVAInt(pval_outBoxMesh, aptr_outBoxMesh, local_outBoxMesh);

            return true;
        }
        // LnBoxMesh_SetPrototype_OnSerialize
        case 0x5E720 : {
            // Fetch boxmesh
            const auto local_boxmesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnBoxMesh_SetPrototype_OnSerialize(local_boxmesh, local_callback);


            return true;
        }
        // LnBoxMesh_SetPrototype_OnUpdate
        case 0x5F220 : {
            // Fetch boxmesh
            const auto local_boxmesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnBoxMesh_SetPrototype_OnUpdate(local_boxmesh, local_callback);


            return true;
        }
        // LnPlaneMesh_Create
        case 0x19720 : {
            // Fetch outPlaneMesh
            PVal* pval_outPlaneMesh;
            const APTR aptr_outPlaneMesh = code_getva(&pval_outPlaneMesh);
            LnHandle local_outPlaneMesh;

            stat = LnPlaneMesh_Create(&local_outPlaneMesh);
            setVAInt(pval_outPlaneMesh, aptr_outPlaneMesh, local_outPlaneMesh);

            return true;
        }
        // LnPlaneMesh_SetPrototype_OnSerialize
        case 0x5FD20 : {
            // Fetch planemesh
            const auto local_planemesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnPlaneMesh_SetPrototype_OnSerialize(local_planemesh, local_callback);


            return true;
        }
        // LnPlaneMesh_SetPrototype_OnUpdate
        case 0x60820 : {
            // Fetch planemesh
            const auto local_planemesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnPlaneMesh_SetPrototype_OnUpdate(local_planemesh, local_callback);


            return true;
        }
        // LnUIEventArgs_Sender
        case 0x19920 : {
            // Fetch uieventargs
            const auto local_uieventargs = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnUIEventArgs_Sender(local_uieventargs, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnUIEventArgs_SetPrototype_OnSerialize
        case 0x61320 : {
            // Fetch uieventargs
            const auto local_uieventargs = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnUIEventArgs_SetPrototype_OnSerialize(local_uieventargs, local_callback);


            return true;
        }
        // LnUIGeneralEventHandler_Create
        case 0x42420 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIGeneralEventHandler
            PVal* pval_outUIGeneralEventHandler;
            const APTR aptr_outUIGeneralEventHandler = code_getva(&pval_outUIGeneralEventHandler);
            LnHandle local_outUIGeneralEventHandler;

            stat = LnUIGeneralEventHandler_Create(HSPSubclass_LnUIGeneralEventHandler_DelegateLabelCaller, &local_outUIGeneralEventHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnUIGeneralEventHandler*>(LnUIGeneralEventHandler_GetSubinstanceId(local_outUIGeneralEventHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIGeneralEventHandler, aptr_outUIGeneralEventHandler, local_outUIGeneralEventHandler);

            return true;
        }
        // LnUIEventHandler_Create
        case 0x42920 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIEventHandler
            PVal* pval_outUIEventHandler;
            const APTR aptr_outUIEventHandler = code_getva(&pval_outUIEventHandler);
            LnHandle local_outUIEventHandler;

            stat = LnUIEventHandler_Create(HSPSubclass_LnUIEventHandler_DelegateLabelCaller, &local_outUIEventHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnUIEventHandler*>(LnUIEventHandler_GetSubinstanceId(local_outUIEventHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIEventHandler, aptr_outUIEventHandler, local_outUIEventHandler);

            return true;
        }
        // LnUILayoutElement_SetPrototype_OnSerialize
        case 0x61E20 : {
            // Fetch uilayoutelement
            const auto local_uilayoutelement = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnUILayoutElement_SetPrototype_OnSerialize(local_uilayoutelement, local_callback);


            return true;
        }
        // LnUIElement_SetMargin
        case 0x1AB20 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch margin
            PVal* pval_margin;
            CodeGetVA_TypeChecked(&pval_margin, LnThickness);

            stat = LnUIElement_SetMargin(local_uielement, reinterpret_cast<const LnThickness*>(pval_margin->pt));


            return true;
        }
        // LnUIElement_GetMargin
        case 0x1AD20 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnThickness local_outReturn;

            stat = LnUIElement_GetMargin(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnThickness_typeid(), &local_outReturn);

            return true;
        }
        // LnUIElement_SetPadding
        case 0x1AE20 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch padding
            PVal* pval_padding;
            CodeGetVA_TypeChecked(&pval_padding, LnThickness);

            stat = LnUIElement_SetPadding(local_uielement, reinterpret_cast<const LnThickness*>(pval_padding->pt));


            return true;
        }
        // LnUIElement_GetPadding
        case 0x1B020 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnThickness local_outReturn;

            stat = LnUIElement_GetPadding(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnThickness_typeid(), &local_outReturn);

            return true;
        }
        // LnUIElement_SetHAlignment
        case 0x1B120 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LnHAlignment>(fetchVAInt());

            stat = LnUIElement_SetHAlignment(local_uielement, local_value);


            return true;
        }
        // LnUIElement_GetHAlignment
        case 0x1B320 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHAlignment local_outReturn;

            stat = LnUIElement_GetHAlignment(local_uielement, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnUIElement_SetVAlignment
        case 0x1B420 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LnVAlignment>(fetchVAInt());

            stat = LnUIElement_SetVAlignment(local_uielement, local_value);


            return true;
        }
        // LnUIElement_GetVAlignment
        case 0x1B620 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVAlignment local_outReturn;

            stat = LnUIElement_GetVAlignment(local_uielement, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnUIElement_SetAlignments
        case 0x1B720 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch halign
            const auto local_halign = static_cast<LnHAlignment>(fetchVAInt());
            // Fetch valign
            const auto local_valign = static_cast<LnVAlignment>(fetchVAInt());

            stat = LnUIElement_SetAlignments(local_uielement, local_halign, local_valign);


            return true;
        }
        // LnUIElement_SetPosition
        case 0x1BA20 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch pos
            PVal* pval_pos;
            CodeGetVA_TypeChecked(&pval_pos, LnVector3);

            stat = LnUIElement_SetPosition(local_uielement, reinterpret_cast<const LnVector3*>(pval_pos->pt));


            return true;
        }
        // LnUIElement_SetPositionXYZ
        case 0x1BC20 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnUIElement_SetPositionXYZ(local_uielement, local_x, local_y, local_z);


            return true;
        }
        // LnUIElement_GetPosition
        case 0x1C020 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnUIElement_GetPosition(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            return true;
        }
        // LnUIElement_SetRotation
        case 0x1C120 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch rot
            PVal* pval_rot;
            CodeGetVA_TypeChecked(&pval_rot, LnQuaternion);

            stat = LnUIElement_SetRotation(local_uielement, reinterpret_cast<const LnQuaternion*>(pval_rot->pt));


            return true;
        }
        // LnUIElement_SetEulerAngles
        case 0x1C320 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnUIElement_SetEulerAngles(local_uielement, local_x, local_y, local_z);


            return true;
        }
        // LnUIElement_GetRotation
        case 0x1C720 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnQuaternion local_outReturn;

            stat = LnUIElement_GetRotation(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnQuaternion_typeid(), &local_outReturn);

            return true;
        }
        // LnUIElement_SetScale
        case 0x1C820 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch scale
            PVal* pval_scale;
            CodeGetVA_TypeChecked(&pval_scale, LnVector3);

            stat = LnUIElement_SetScale(local_uielement, reinterpret_cast<const LnVector3*>(pval_scale->pt));


            return true;
        }
        // LnUIElement_SetScaleS
        case 0x1CA20 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch xyz
            const auto local_xyz = fetchVADouble();

            stat = LnUIElement_SetScaleS(local_uielement, local_xyz);


            return true;
        }
        // LnUIElement_SetScaleXY
        case 0x1CC20 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();

            stat = LnUIElement_SetScaleXY(local_uielement, local_x, local_y);


            return true;
        }
        // LnUIElement_GetScale
        case 0x1CF20 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnUIElement_GetScale(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            return true;
        }
        // LnUIElement_SetCenterPoint
        case 0x1D020 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LnVector3);

            stat = LnUIElement_SetCenterPoint(local_uielement, reinterpret_cast<const LnVector3*>(pval_value->pt));


            return true;
        }
        // LnUIElement_SetCenterPointXYZ
        case 0x1D220 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LnUIElement_SetCenterPointXYZ(local_uielement, local_x, local_y, local_z);


            return true;
        }
        // LnUIElement_GetCenterPoint
        case 0x1D620 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnVector3 local_outReturn;

            stat = LnUIElement_GetCenterPoint(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnVector3_typeid(), &local_outReturn);

            return true;
        }
        // LnUIElement_AddChild
        case 0x1D720 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch child
            const auto local_child = fetchVAInt();

            stat = LnUIElement_AddChild(local_uielement, local_child);


            return true;
        }
        // LnUIElement_SetPrototype_OnSerialize
        case 0x62920 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnUIElement_SetPrototype_OnSerialize(local_uielement, local_callback);


            return true;
        }
        // LnUITextBlock_Create
        case 0x1DA20 : {
            // Fetch outUITextBlock
            PVal* pval_outUITextBlock;
            const APTR aptr_outUITextBlock = code_getva(&pval_outUITextBlock);
            LnHandle local_outUITextBlock;

            stat = LnUITextBlock_Create(&local_outUITextBlock);
            setVAInt(pval_outUITextBlock, aptr_outUITextBlock, local_outUITextBlock);

            return true;
        }
        // LnUITextBlock_CreateWithTextA
        case 0x1DB20 : {
            // Fetch text
            const auto local_text = fetchVAString();
            // Fetch outUITextBlock
            PVal* pval_outUITextBlock;
            const APTR aptr_outUITextBlock = code_getva(&pval_outUITextBlock);
            LnHandle local_outUITextBlock;

            stat = LnUITextBlock_CreateWithTextA(local_text, &local_outUITextBlock);
            setVAInt(pval_outUITextBlock, aptr_outUITextBlock, local_outUITextBlock);

            return true;
        }
        // LnUITextBlock_SetPrototype_OnSerialize
        case 0x63420 : {
            // Fetch uitextblock
            const auto local_uitextblock = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnUITextBlock_SetPrototype_OnSerialize(local_uitextblock, local_callback);


            return true;
        }
        // LnUISprite_Create
        case 0x1E820 : {
            // Fetch outUISprite
            PVal* pval_outUISprite;
            const APTR aptr_outUISprite = code_getva(&pval_outUISprite);
            LnHandle local_outUISprite;

            stat = LnUISprite_Create(&local_outUISprite);
            setVAInt(pval_outUISprite, aptr_outUISprite, local_outUISprite);

            return true;
        }
        // LnUISprite_CreateWithTexture
        case 0x1E920 : {
            // Fetch texture
            const auto local_texture = fetchVAInt();
            // Fetch outUISprite
            PVal* pval_outUISprite;
            const APTR aptr_outUISprite = code_getva(&pval_outUISprite);
            LnHandle local_outUISprite;

            stat = LnUISprite_CreateWithTexture(local_texture, &local_outUISprite);
            setVAInt(pval_outUISprite, aptr_outUISprite, local_outUISprite);

            return true;
        }
        // LnUISprite_SetTexture
        case 0x1DE20 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch texture
            const auto local_texture = fetchVAInt();

            stat = LnUISprite_SetTexture(local_uisprite, local_texture);


            return true;
        }
        // LnUISprite_SetSourceRect
        case 0x1E020 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch rect
            PVal* pval_rect;
            CodeGetVA_TypeChecked(&pval_rect, LnRect);

            stat = LnUISprite_SetSourceRect(local_uisprite, reinterpret_cast<const LnRect*>(pval_rect->pt));


            return true;
        }
        // LnUISprite_SetSourceRectXYWH
        case 0x1E220 : {
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


            return true;
        }
        // LnUISprite_GetSourceRect
        case 0x1E720 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnRect local_outReturn;

            stat = LnUISprite_GetSourceRect(local_uisprite, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnRect_typeid(), &local_outReturn);

            return true;
        }
        // LnUISprite_SetPrototype_OnSerialize
        case 0x63F20 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnUISprite_SetPrototype_OnSerialize(local_uisprite, local_callback);


            return true;
        }
        // LnInput_PressedA
        case 0x1EC20 : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnInput_PressedA(local_buttonName, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnInput_TriggeredA
        case 0x1EE20 : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnInput_TriggeredA(local_buttonName, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnInput_TriggeredOffA
        case 0x1F020 : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnInput_TriggeredOffA(local_buttonName, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnInput_RepeatedA
        case 0x1F220 : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnInput_RepeatedA(local_buttonName, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnInput_GetAxisValueA
        case 0x1F420 : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LnInput_GetAxisValueA(local_buttonName, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnInput_ClearAllBindings
        case 0x1F620 : {

            stat = LnInput_ClearAllBindings();


            return true;
        }
        // LnMouse_Pressed
        case 0x1F820 : {
            // Fetch button
            const auto local_button = static_cast<LnMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnMouse_Pressed(local_button, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnMouse_Triggered
        case 0x1FA20 : {
            // Fetch button
            const auto local_button = static_cast<LnMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnMouse_Triggered(local_button, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnMouse_TriggeredOff
        case 0x1FC20 : {
            // Fetch button
            const auto local_button = static_cast<LnMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnMouse_TriggeredOff(local_button, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnMouse_Repeated
        case 0x1FE20 : {
            // Fetch button
            const auto local_button = static_cast<LnMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnMouse_Repeated(local_button, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnMouse_Position
        case 0x20020 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnPoint local_outReturn;

            stat = LnMouse_Position(&local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLnPoint_typeid(), &local_outReturn);

            return true;
        }
        // LnEngineSettings_SetMainWindowSize
        case 0x20220 : {
            // Fetch width
            const auto local_width = fetchVAInt();
            // Fetch height
            const auto local_height = fetchVAInt();

            stat = LnEngineSettings_SetMainWindowSize(local_width, local_height);


            return true;
        }
        // LnEngineSettings_SetMainWorldViewSize
        case 0x20520 : {
            // Fetch width
            const auto local_width = fetchVAInt();
            // Fetch height
            const auto local_height = fetchVAInt();

            stat = LnEngineSettings_SetMainWorldViewSize(local_width, local_height);


            return true;
        }
        // LnEngineSettings_SetMainWindowTitleA
        case 0x20820 : {
            // Fetch title
            const auto local_title = fetchVAString();

            stat = LnEngineSettings_SetMainWindowTitleA(local_title);


            return true;
        }
        // LnEngineSettings_AddAssetDirectoryA
        case 0x20A20 : {
            // Fetch path
            const auto local_path = fetchVAString();

            stat = LnEngineSettings_AddAssetDirectoryA(local_path);


            return true;
        }
        // LnEngineSettings_AddAssetArchiveA
        case 0x20C20 : {
            // Fetch fileFullPath
            const auto local_fileFullPath = fetchVAString();
            // Fetch password
            const auto local_password = fetchVAString();

            stat = LnEngineSettings_AddAssetArchiveA(local_fileFullPath, local_password);


            return true;
        }
        // LnEngineSettings_SetFrameRate
        case 0x20F20 : {
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LnEngineSettings_SetFrameRate(local_value);


            return true;
        }
        // LnEngineSettings_SetDebugToolEnabled
        case 0x21120 : {
            // Fetch enabled
            const auto local_enabled = static_cast<LnBool>(fetchVAInt());

            stat = LnEngineSettings_SetDebugToolEnabled(local_enabled);


            return true;
        }
        // LnEngineSettings_SetEngineLogEnabled
        case 0x21320 : {
            // Fetch enabled
            const auto local_enabled = static_cast<LnBool>(fetchVAInt());

            stat = LnEngineSettings_SetEngineLogEnabled(local_enabled);


            return true;
        }
        // LnEngineSettings_SetEngineLogFilePathA
        case 0x21520 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();

            stat = LnEngineSettings_SetEngineLogFilePathA(local_filePath);


            return true;
        }
        // LnEngine_Initialize
        case 0x21820 : {

            stat = LnEngine_Initialize();


            return true;
        }
        // LnEngine_Finalize
        case 0x21920 : {

            stat = LnEngine_Finalize();


            return true;
        }
        // LnEngine_Update
        case 0x21A20 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnBool local_outReturn;

            stat = LnEngine_Update(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnEngine_Time
        case 0x21B20 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            double local_outReturn;

            stat = LnEngine_Time(&local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnEngine_GetCamera
        case 0x21C20 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnEngine_GetCamera(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnEngine_GetLight
        case 0x21D20 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnEngine_GetLight(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnEngine_GetRenderView
        case 0x21E20 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnEngine_GetRenderView(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnApplication_Create
        case 0x22320 : {
            // Fetch outApplication
            PVal* pval_outApplication;
            const APTR aptr_outApplication = code_getva(&pval_outApplication);
            LnHandle local_outApplication;

            stat = LnApplication_Create(&local_outApplication);
            setVAInt(pval_outApplication, aptr_outApplication, local_outApplication);

            return true;
        }
        // LnApplication_OnInit
        case 0x22020 : {
            // Fetch application
            const auto local_application = fetchVAInt();

            stat = LnApplication_OnInit(local_application);


            return true;
        }
        // LnApplication_OnUpdate
        case 0x22120 : {
            // Fetch application
            const auto local_application = fetchVAInt();

            stat = LnApplication_OnUpdate(local_application);


            return true;
        }
        // LnApplication_World
        case 0x22220 : {
            // Fetch application
            const auto local_application = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LnHandle local_outReturn;

            stat = LnApplication_World(local_application, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LnApplication_SetPrototype_OnSerialize
        case 0x64A20 : {
            // Fetch application
            const auto local_application = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnApplication_SetPrototype_OnSerialize(local_application, local_callback);


            return true;
        }
        // LnApplication_SetPrototype_OnInit
        case 0x65520 : {
            // Fetch application
            const auto local_application = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnApplication_SetPrototype_OnInit(local_application, local_callback);


            return true;
        }
        // LnApplication_SetPrototype_OnUpdate
        case 0x66020 : {
            // Fetch application
            const auto local_application = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVAInt();

            stat = LnApplication_SetPrototype_OnUpdate(local_application, local_callback);


            return true;
        }
        // LnDebug_PrintA
        case 0x22520 : {
            // Fetch str
            const auto local_str = fetchVAString();

            stat = LnDebug_PrintA(local_str);


            return true;
        }
        // LnDebug_PrintWithTimeA
        case 0x22720 : {
            // Fetch time
            const auto local_time = fetchVADouble();
            // Fetch str
            const auto local_str = fetchVAString();

            stat = LnDebug_PrintWithTimeA(local_time, local_str);


            return true;
        }
        // LnDebug_PrintWithTimeAndColorA
        case 0x22A20 : {
            // Fetch time
            const auto local_time = fetchVADouble();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LnColor);
            // Fetch str
            const auto local_str = fetchVAString();

            stat = LnDebug_PrintWithTimeAndColorA(local_time, reinterpret_cast<const LnColor*>(pval_color->pt), local_str);


            return true;
        }
        // LnObjectSerializeHandler_Create
        case 0x48520 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outObjectSerializeHandler
            PVal* pval_outObjectSerializeHandler;
            const APTR aptr_outObjectSerializeHandler = code_getva(&pval_outObjectSerializeHandler);
            LnHandle local_outObjectSerializeHandler;

            stat = LnObjectSerializeHandler_Create(HSPSubclass_LnObjectSerializeHandler_DelegateLabelCaller, &local_outObjectSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnObjectSerializeHandler*>(LnObjectSerializeHandler_GetSubinstanceId(local_outObjectSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outObjectSerializeHandler, aptr_outObjectSerializeHandler, local_outObjectSerializeHandler);

            return true;
        }
        // LnEventConnectionSerializeHandler_Create
        case 0x49020 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outEventConnectionSerializeHandler
            PVal* pval_outEventConnectionSerializeHandler;
            const APTR aptr_outEventConnectionSerializeHandler = code_getva(&pval_outEventConnectionSerializeHandler);
            LnHandle local_outEventConnectionSerializeHandler;

            stat = LnEventConnectionSerializeHandler_Create(HSPSubclass_LnEventConnectionSerializeHandler_DelegateLabelCaller, &local_outEventConnectionSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnEventConnectionSerializeHandler*>(LnEventConnectionSerializeHandler_GetSubinstanceId(local_outEventConnectionSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outEventConnectionSerializeHandler, aptr_outEventConnectionSerializeHandler, local_outEventConnectionSerializeHandler);

            return true;
        }
        // LnZVTestClass1SerializeHandler_Create
        case 0x49B20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestClass1SerializeHandler
            PVal* pval_outZVTestClass1SerializeHandler;
            const APTR aptr_outZVTestClass1SerializeHandler = code_getva(&pval_outZVTestClass1SerializeHandler);
            LnHandle local_outZVTestClass1SerializeHandler;

            stat = LnZVTestClass1SerializeHandler_Create(HSPSubclass_LnZVTestClass1SerializeHandler_DelegateLabelCaller, &local_outZVTestClass1SerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestClass1SerializeHandler*>(LnZVTestClass1SerializeHandler_GetSubinstanceId(local_outZVTestClass1SerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestClass1SerializeHandler, aptr_outZVTestClass1SerializeHandler, local_outZVTestClass1SerializeHandler);

            return true;
        }
        // LnZVTestEventArgs1SerializeHandler_Create
        case 0x4A620 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestEventArgs1SerializeHandler
            PVal* pval_outZVTestEventArgs1SerializeHandler;
            const APTR aptr_outZVTestEventArgs1SerializeHandler = code_getva(&pval_outZVTestEventArgs1SerializeHandler);
            LnHandle local_outZVTestEventArgs1SerializeHandler;

            stat = LnZVTestEventArgs1SerializeHandler_Create(HSPSubclass_LnZVTestEventArgs1SerializeHandler_DelegateLabelCaller, &local_outZVTestEventArgs1SerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnZVTestEventArgs1SerializeHandler*>(LnZVTestEventArgs1SerializeHandler_GetSubinstanceId(local_outZVTestEventArgs1SerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestEventArgs1SerializeHandler, aptr_outZVTestEventArgs1SerializeHandler, local_outZVTestEventArgs1SerializeHandler);

            return true;
        }
        // LnSerializer2SerializeHandler_Create
        case 0x4B120 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSerializer2SerializeHandler
            PVal* pval_outSerializer2SerializeHandler;
            const APTR aptr_outSerializer2SerializeHandler = code_getva(&pval_outSerializer2SerializeHandler);
            LnHandle local_outSerializer2SerializeHandler;

            stat = LnSerializer2SerializeHandler_Create(HSPSubclass_LnSerializer2SerializeHandler_DelegateLabelCaller, &local_outSerializer2SerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnSerializer2SerializeHandler*>(LnSerializer2SerializeHandler_GetSubinstanceId(local_outSerializer2SerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSerializer2SerializeHandler, aptr_outSerializer2SerializeHandler, local_outSerializer2SerializeHandler);

            return true;
        }
        // LnAssetModelSerializeHandler_Create
        case 0x4BC20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outAssetModelSerializeHandler
            PVal* pval_outAssetModelSerializeHandler;
            const APTR aptr_outAssetModelSerializeHandler = code_getva(&pval_outAssetModelSerializeHandler);
            LnHandle local_outAssetModelSerializeHandler;

            stat = LnAssetModelSerializeHandler_Create(HSPSubclass_LnAssetModelSerializeHandler_DelegateLabelCaller, &local_outAssetModelSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnAssetModelSerializeHandler*>(LnAssetModelSerializeHandler_GetSubinstanceId(local_outAssetModelSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outAssetModelSerializeHandler, aptr_outAssetModelSerializeHandler, local_outAssetModelSerializeHandler);

            return true;
        }
        // LnTextureSerializeHandler_Create
        case 0x4C720 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outTextureSerializeHandler
            PVal* pval_outTextureSerializeHandler;
            const APTR aptr_outTextureSerializeHandler = code_getva(&pval_outTextureSerializeHandler);
            LnHandle local_outTextureSerializeHandler;

            stat = LnTextureSerializeHandler_Create(HSPSubclass_LnTextureSerializeHandler_DelegateLabelCaller, &local_outTextureSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnTextureSerializeHandler*>(LnTextureSerializeHandler_GetSubinstanceId(local_outTextureSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outTextureSerializeHandler, aptr_outTextureSerializeHandler, local_outTextureSerializeHandler);

            return true;
        }
        // LnTexture2DSerializeHandler_Create
        case 0x4D220 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outTexture2DSerializeHandler
            PVal* pval_outTexture2DSerializeHandler;
            const APTR aptr_outTexture2DSerializeHandler = code_getva(&pval_outTexture2DSerializeHandler);
            LnHandle local_outTexture2DSerializeHandler;

            stat = LnTexture2DSerializeHandler_Create(HSPSubclass_LnTexture2DSerializeHandler_DelegateLabelCaller, &local_outTexture2DSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnTexture2DSerializeHandler*>(LnTexture2DSerializeHandler_GetSubinstanceId(local_outTexture2DSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outTexture2DSerializeHandler, aptr_outTexture2DSerializeHandler, local_outTexture2DSerializeHandler);

            return true;
        }
        // LnRenderViewSerializeHandler_Create
        case 0x4DD20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outRenderViewSerializeHandler
            PVal* pval_outRenderViewSerializeHandler;
            const APTR aptr_outRenderViewSerializeHandler = code_getva(&pval_outRenderViewSerializeHandler);
            LnHandle local_outRenderViewSerializeHandler;

            stat = LnRenderViewSerializeHandler_Create(HSPSubclass_LnRenderViewSerializeHandler_DelegateLabelCaller, &local_outRenderViewSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnRenderViewSerializeHandler*>(LnRenderViewSerializeHandler_GetSubinstanceId(local_outRenderViewSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outRenderViewSerializeHandler, aptr_outRenderViewSerializeHandler, local_outRenderViewSerializeHandler);

            return true;
        }
        // LnComponentSerializeHandler_Create
        case 0x4E820 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outComponentSerializeHandler
            PVal* pval_outComponentSerializeHandler;
            const APTR aptr_outComponentSerializeHandler = code_getva(&pval_outComponentSerializeHandler);
            LnHandle local_outComponentSerializeHandler;

            stat = LnComponentSerializeHandler_Create(HSPSubclass_LnComponentSerializeHandler_DelegateLabelCaller, &local_outComponentSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnComponentSerializeHandler*>(LnComponentSerializeHandler_GetSubinstanceId(local_outComponentSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outComponentSerializeHandler, aptr_outComponentSerializeHandler, local_outComponentSerializeHandler);

            return true;
        }
        // LnVisualComponentSerializeHandler_Create
        case 0x4F320 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outVisualComponentSerializeHandler
            PVal* pval_outVisualComponentSerializeHandler;
            const APTR aptr_outVisualComponentSerializeHandler = code_getva(&pval_outVisualComponentSerializeHandler);
            LnHandle local_outVisualComponentSerializeHandler;

            stat = LnVisualComponentSerializeHandler_Create(HSPSubclass_LnVisualComponentSerializeHandler_DelegateLabelCaller, &local_outVisualComponentSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnVisualComponentSerializeHandler*>(LnVisualComponentSerializeHandler_GetSubinstanceId(local_outVisualComponentSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outVisualComponentSerializeHandler, aptr_outVisualComponentSerializeHandler, local_outVisualComponentSerializeHandler);

            return true;
        }
        // LnSpriteComponentSerializeHandler_Create
        case 0x4FE20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSpriteComponentSerializeHandler
            PVal* pval_outSpriteComponentSerializeHandler;
            const APTR aptr_outSpriteComponentSerializeHandler = code_getva(&pval_outSpriteComponentSerializeHandler);
            LnHandle local_outSpriteComponentSerializeHandler;

            stat = LnSpriteComponentSerializeHandler_Create(HSPSubclass_LnSpriteComponentSerializeHandler_DelegateLabelCaller, &local_outSpriteComponentSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnSpriteComponentSerializeHandler*>(LnSpriteComponentSerializeHandler_GetSubinstanceId(local_outSpriteComponentSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSpriteComponentSerializeHandler, aptr_outSpriteComponentSerializeHandler, local_outSpriteComponentSerializeHandler);

            return true;
        }
        // LnWorldSerializeHandler_Create
        case 0x50920 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outWorldSerializeHandler
            PVal* pval_outWorldSerializeHandler;
            const APTR aptr_outWorldSerializeHandler = code_getva(&pval_outWorldSerializeHandler);
            LnHandle local_outWorldSerializeHandler;

            stat = LnWorldSerializeHandler_Create(HSPSubclass_LnWorldSerializeHandler_DelegateLabelCaller, &local_outWorldSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnWorldSerializeHandler*>(LnWorldSerializeHandler_GetSubinstanceId(local_outWorldSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outWorldSerializeHandler, aptr_outWorldSerializeHandler, local_outWorldSerializeHandler);

            return true;
        }
        // LnComponentListSerializeHandler_Create
        case 0x51420 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outComponentListSerializeHandler
            PVal* pval_outComponentListSerializeHandler;
            const APTR aptr_outComponentListSerializeHandler = code_getva(&pval_outComponentListSerializeHandler);
            LnHandle local_outComponentListSerializeHandler;

            stat = LnComponentListSerializeHandler_Create(HSPSubclass_LnComponentListSerializeHandler_DelegateLabelCaller, &local_outComponentListSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnComponentListSerializeHandler*>(LnComponentListSerializeHandler_GetSubinstanceId(local_outComponentListSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outComponentListSerializeHandler, aptr_outComponentListSerializeHandler, local_outComponentListSerializeHandler);

            return true;
        }
        // LnWorldObjectSerializeHandler_Create
        case 0x51F20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outWorldObjectSerializeHandler
            PVal* pval_outWorldObjectSerializeHandler;
            const APTR aptr_outWorldObjectSerializeHandler = code_getva(&pval_outWorldObjectSerializeHandler);
            LnHandle local_outWorldObjectSerializeHandler;

            stat = LnWorldObjectSerializeHandler_Create(HSPSubclass_LnWorldObjectSerializeHandler_DelegateLabelCaller, &local_outWorldObjectSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnWorldObjectSerializeHandler*>(LnWorldObjectSerializeHandler_GetSubinstanceId(local_outWorldObjectSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outWorldObjectSerializeHandler, aptr_outWorldObjectSerializeHandler, local_outWorldObjectSerializeHandler);

            return true;
        }
        // LnWorldObjectUpdateHandler_Create
        case 0x52A20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outWorldObjectUpdateHandler
            PVal* pval_outWorldObjectUpdateHandler;
            const APTR aptr_outWorldObjectUpdateHandler = code_getva(&pval_outWorldObjectUpdateHandler);
            LnHandle local_outWorldObjectUpdateHandler;

            stat = LnWorldObjectUpdateHandler_Create(HSPSubclass_LnWorldObjectUpdateHandler_DelegateLabelCaller, &local_outWorldObjectUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnWorldObjectUpdateHandler*>(LnWorldObjectUpdateHandler_GetSubinstanceId(local_outWorldObjectUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outWorldObjectUpdateHandler, aptr_outWorldObjectUpdateHandler, local_outWorldObjectUpdateHandler);

            return true;
        }
        // LnVisualObjectSerializeHandler_Create
        case 0x53520 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outVisualObjectSerializeHandler
            PVal* pval_outVisualObjectSerializeHandler;
            const APTR aptr_outVisualObjectSerializeHandler = code_getva(&pval_outVisualObjectSerializeHandler);
            LnHandle local_outVisualObjectSerializeHandler;

            stat = LnVisualObjectSerializeHandler_Create(HSPSubclass_LnVisualObjectSerializeHandler_DelegateLabelCaller, &local_outVisualObjectSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnVisualObjectSerializeHandler*>(LnVisualObjectSerializeHandler_GetSubinstanceId(local_outVisualObjectSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outVisualObjectSerializeHandler, aptr_outVisualObjectSerializeHandler, local_outVisualObjectSerializeHandler);

            return true;
        }
        // LnVisualObjectUpdateHandler_Create
        case 0x54020 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outVisualObjectUpdateHandler
            PVal* pval_outVisualObjectUpdateHandler;
            const APTR aptr_outVisualObjectUpdateHandler = code_getva(&pval_outVisualObjectUpdateHandler);
            LnHandle local_outVisualObjectUpdateHandler;

            stat = LnVisualObjectUpdateHandler_Create(HSPSubclass_LnVisualObjectUpdateHandler_DelegateLabelCaller, &local_outVisualObjectUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnVisualObjectUpdateHandler*>(LnVisualObjectUpdateHandler_GetSubinstanceId(local_outVisualObjectUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outVisualObjectUpdateHandler, aptr_outVisualObjectUpdateHandler, local_outVisualObjectUpdateHandler);

            return true;
        }
        // LnCameraSerializeHandler_Create
        case 0x54B20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outCameraSerializeHandler
            PVal* pval_outCameraSerializeHandler;
            const APTR aptr_outCameraSerializeHandler = code_getva(&pval_outCameraSerializeHandler);
            LnHandle local_outCameraSerializeHandler;

            stat = LnCameraSerializeHandler_Create(HSPSubclass_LnCameraSerializeHandler_DelegateLabelCaller, &local_outCameraSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnCameraSerializeHandler*>(LnCameraSerializeHandler_GetSubinstanceId(local_outCameraSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outCameraSerializeHandler, aptr_outCameraSerializeHandler, local_outCameraSerializeHandler);

            return true;
        }
        // LnCameraUpdateHandler_Create
        case 0x55620 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outCameraUpdateHandler
            PVal* pval_outCameraUpdateHandler;
            const APTR aptr_outCameraUpdateHandler = code_getva(&pval_outCameraUpdateHandler);
            LnHandle local_outCameraUpdateHandler;

            stat = LnCameraUpdateHandler_Create(HSPSubclass_LnCameraUpdateHandler_DelegateLabelCaller, &local_outCameraUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnCameraUpdateHandler*>(LnCameraUpdateHandler_GetSubinstanceId(local_outCameraUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outCameraUpdateHandler, aptr_outCameraUpdateHandler, local_outCameraUpdateHandler);

            return true;
        }
        // LnDirectionalLightSerializeHandler_Create
        case 0x56120 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outDirectionalLightSerializeHandler
            PVal* pval_outDirectionalLightSerializeHandler;
            const APTR aptr_outDirectionalLightSerializeHandler = code_getva(&pval_outDirectionalLightSerializeHandler);
            LnHandle local_outDirectionalLightSerializeHandler;

            stat = LnDirectionalLightSerializeHandler_Create(HSPSubclass_LnDirectionalLightSerializeHandler_DelegateLabelCaller, &local_outDirectionalLightSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnDirectionalLightSerializeHandler*>(LnDirectionalLightSerializeHandler_GetSubinstanceId(local_outDirectionalLightSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outDirectionalLightSerializeHandler, aptr_outDirectionalLightSerializeHandler, local_outDirectionalLightSerializeHandler);

            return true;
        }
        // LnDirectionalLightUpdateHandler_Create
        case 0x56C20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outDirectionalLightUpdateHandler
            PVal* pval_outDirectionalLightUpdateHandler;
            const APTR aptr_outDirectionalLightUpdateHandler = code_getva(&pval_outDirectionalLightUpdateHandler);
            LnHandle local_outDirectionalLightUpdateHandler;

            stat = LnDirectionalLightUpdateHandler_Create(HSPSubclass_LnDirectionalLightUpdateHandler_DelegateLabelCaller, &local_outDirectionalLightUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnDirectionalLightUpdateHandler*>(LnDirectionalLightUpdateHandler_GetSubinstanceId(local_outDirectionalLightUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outDirectionalLightUpdateHandler, aptr_outDirectionalLightUpdateHandler, local_outDirectionalLightUpdateHandler);

            return true;
        }
        // LnPointLightSerializeHandler_Create
        case 0x57720 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outPointLightSerializeHandler
            PVal* pval_outPointLightSerializeHandler;
            const APTR aptr_outPointLightSerializeHandler = code_getva(&pval_outPointLightSerializeHandler);
            LnHandle local_outPointLightSerializeHandler;

            stat = LnPointLightSerializeHandler_Create(HSPSubclass_LnPointLightSerializeHandler_DelegateLabelCaller, &local_outPointLightSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnPointLightSerializeHandler*>(LnPointLightSerializeHandler_GetSubinstanceId(local_outPointLightSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outPointLightSerializeHandler, aptr_outPointLightSerializeHandler, local_outPointLightSerializeHandler);

            return true;
        }
        // LnPointLightUpdateHandler_Create
        case 0x58220 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outPointLightUpdateHandler
            PVal* pval_outPointLightUpdateHandler;
            const APTR aptr_outPointLightUpdateHandler = code_getva(&pval_outPointLightUpdateHandler);
            LnHandle local_outPointLightUpdateHandler;

            stat = LnPointLightUpdateHandler_Create(HSPSubclass_LnPointLightUpdateHandler_DelegateLabelCaller, &local_outPointLightUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnPointLightUpdateHandler*>(LnPointLightUpdateHandler_GetSubinstanceId(local_outPointLightUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outPointLightUpdateHandler, aptr_outPointLightUpdateHandler, local_outPointLightUpdateHandler);

            return true;
        }
        // LnSpotLightSerializeHandler_Create
        case 0x58D20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSpotLightSerializeHandler
            PVal* pval_outSpotLightSerializeHandler;
            const APTR aptr_outSpotLightSerializeHandler = code_getva(&pval_outSpotLightSerializeHandler);
            LnHandle local_outSpotLightSerializeHandler;

            stat = LnSpotLightSerializeHandler_Create(HSPSubclass_LnSpotLightSerializeHandler_DelegateLabelCaller, &local_outSpotLightSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnSpotLightSerializeHandler*>(LnSpotLightSerializeHandler_GetSubinstanceId(local_outSpotLightSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSpotLightSerializeHandler, aptr_outSpotLightSerializeHandler, local_outSpotLightSerializeHandler);

            return true;
        }
        // LnSpotLightUpdateHandler_Create
        case 0x59820 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSpotLightUpdateHandler
            PVal* pval_outSpotLightUpdateHandler;
            const APTR aptr_outSpotLightUpdateHandler = code_getva(&pval_outSpotLightUpdateHandler);
            LnHandle local_outSpotLightUpdateHandler;

            stat = LnSpotLightUpdateHandler_Create(HSPSubclass_LnSpotLightUpdateHandler_DelegateLabelCaller, &local_outSpotLightUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnSpotLightUpdateHandler*>(LnSpotLightUpdateHandler_GetSubinstanceId(local_outSpotLightUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSpotLightUpdateHandler, aptr_outSpotLightUpdateHandler, local_outSpotLightUpdateHandler);

            return true;
        }
        // LnSpriteSerializeHandler_Create
        case 0x5A320 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSpriteSerializeHandler
            PVal* pval_outSpriteSerializeHandler;
            const APTR aptr_outSpriteSerializeHandler = code_getva(&pval_outSpriteSerializeHandler);
            LnHandle local_outSpriteSerializeHandler;

            stat = LnSpriteSerializeHandler_Create(HSPSubclass_LnSpriteSerializeHandler_DelegateLabelCaller, &local_outSpriteSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnSpriteSerializeHandler*>(LnSpriteSerializeHandler_GetSubinstanceId(local_outSpriteSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSpriteSerializeHandler, aptr_outSpriteSerializeHandler, local_outSpriteSerializeHandler);

            return true;
        }
        // LnSpriteUpdateHandler_Create
        case 0x5AE20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSpriteUpdateHandler
            PVal* pval_outSpriteUpdateHandler;
            const APTR aptr_outSpriteUpdateHandler = code_getva(&pval_outSpriteUpdateHandler);
            LnHandle local_outSpriteUpdateHandler;

            stat = LnSpriteUpdateHandler_Create(HSPSubclass_LnSpriteUpdateHandler_DelegateLabelCaller, &local_outSpriteUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnSpriteUpdateHandler*>(LnSpriteUpdateHandler_GetSubinstanceId(local_outSpriteUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSpriteUpdateHandler, aptr_outSpriteUpdateHandler, local_outSpriteUpdateHandler);

            return true;
        }
        // LnCameraOrbitControlComponentSerializeHandler_Create
        case 0x5B920 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outCameraOrbitControlComponentSerializeHandler
            PVal* pval_outCameraOrbitControlComponentSerializeHandler;
            const APTR aptr_outCameraOrbitControlComponentSerializeHandler = code_getva(&pval_outCameraOrbitControlComponentSerializeHandler);
            LnHandle local_outCameraOrbitControlComponentSerializeHandler;

            stat = LnCameraOrbitControlComponentSerializeHandler_Create(HSPSubclass_LnCameraOrbitControlComponentSerializeHandler_DelegateLabelCaller, &local_outCameraOrbitControlComponentSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnCameraOrbitControlComponentSerializeHandler*>(LnCameraOrbitControlComponentSerializeHandler_GetSubinstanceId(local_outCameraOrbitControlComponentSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outCameraOrbitControlComponentSerializeHandler, aptr_outCameraOrbitControlComponentSerializeHandler, local_outCameraOrbitControlComponentSerializeHandler);

            return true;
        }
        // LnRaycasterSerializeHandler_Create
        case 0x5C420 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outRaycasterSerializeHandler
            PVal* pval_outRaycasterSerializeHandler;
            const APTR aptr_outRaycasterSerializeHandler = code_getva(&pval_outRaycasterSerializeHandler);
            LnHandle local_outRaycasterSerializeHandler;

            stat = LnRaycasterSerializeHandler_Create(HSPSubclass_LnRaycasterSerializeHandler_DelegateLabelCaller, &local_outRaycasterSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnRaycasterSerializeHandler*>(LnRaycasterSerializeHandler_GetSubinstanceId(local_outRaycasterSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outRaycasterSerializeHandler, aptr_outRaycasterSerializeHandler, local_outRaycasterSerializeHandler);

            return true;
        }
        // LnRaycastResultSerializeHandler_Create
        case 0x5CF20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outRaycastResultSerializeHandler
            PVal* pval_outRaycastResultSerializeHandler;
            const APTR aptr_outRaycastResultSerializeHandler = code_getva(&pval_outRaycastResultSerializeHandler);
            LnHandle local_outRaycastResultSerializeHandler;

            stat = LnRaycastResultSerializeHandler_Create(HSPSubclass_LnRaycastResultSerializeHandler_DelegateLabelCaller, &local_outRaycastResultSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnRaycastResultSerializeHandler*>(LnRaycastResultSerializeHandler_GetSubinstanceId(local_outRaycastResultSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outRaycastResultSerializeHandler, aptr_outRaycastResultSerializeHandler, local_outRaycastResultSerializeHandler);

            return true;
        }
        // LnWorldRenderViewSerializeHandler_Create
        case 0x5DA20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outWorldRenderViewSerializeHandler
            PVal* pval_outWorldRenderViewSerializeHandler;
            const APTR aptr_outWorldRenderViewSerializeHandler = code_getva(&pval_outWorldRenderViewSerializeHandler);
            LnHandle local_outWorldRenderViewSerializeHandler;

            stat = LnWorldRenderViewSerializeHandler_Create(HSPSubclass_LnWorldRenderViewSerializeHandler_DelegateLabelCaller, &local_outWorldRenderViewSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnWorldRenderViewSerializeHandler*>(LnWorldRenderViewSerializeHandler_GetSubinstanceId(local_outWorldRenderViewSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outWorldRenderViewSerializeHandler, aptr_outWorldRenderViewSerializeHandler, local_outWorldRenderViewSerializeHandler);

            return true;
        }
        // LnBoxMeshSerializeHandler_Create
        case 0x5E520 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outBoxMeshSerializeHandler
            PVal* pval_outBoxMeshSerializeHandler;
            const APTR aptr_outBoxMeshSerializeHandler = code_getva(&pval_outBoxMeshSerializeHandler);
            LnHandle local_outBoxMeshSerializeHandler;

            stat = LnBoxMeshSerializeHandler_Create(HSPSubclass_LnBoxMeshSerializeHandler_DelegateLabelCaller, &local_outBoxMeshSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnBoxMeshSerializeHandler*>(LnBoxMeshSerializeHandler_GetSubinstanceId(local_outBoxMeshSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outBoxMeshSerializeHandler, aptr_outBoxMeshSerializeHandler, local_outBoxMeshSerializeHandler);

            return true;
        }
        // LnBoxMeshUpdateHandler_Create
        case 0x5F020 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outBoxMeshUpdateHandler
            PVal* pval_outBoxMeshUpdateHandler;
            const APTR aptr_outBoxMeshUpdateHandler = code_getva(&pval_outBoxMeshUpdateHandler);
            LnHandle local_outBoxMeshUpdateHandler;

            stat = LnBoxMeshUpdateHandler_Create(HSPSubclass_LnBoxMeshUpdateHandler_DelegateLabelCaller, &local_outBoxMeshUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnBoxMeshUpdateHandler*>(LnBoxMeshUpdateHandler_GetSubinstanceId(local_outBoxMeshUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outBoxMeshUpdateHandler, aptr_outBoxMeshUpdateHandler, local_outBoxMeshUpdateHandler);

            return true;
        }
        // LnPlaneMeshSerializeHandler_Create
        case 0x5FB20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outPlaneMeshSerializeHandler
            PVal* pval_outPlaneMeshSerializeHandler;
            const APTR aptr_outPlaneMeshSerializeHandler = code_getva(&pval_outPlaneMeshSerializeHandler);
            LnHandle local_outPlaneMeshSerializeHandler;

            stat = LnPlaneMeshSerializeHandler_Create(HSPSubclass_LnPlaneMeshSerializeHandler_DelegateLabelCaller, &local_outPlaneMeshSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnPlaneMeshSerializeHandler*>(LnPlaneMeshSerializeHandler_GetSubinstanceId(local_outPlaneMeshSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outPlaneMeshSerializeHandler, aptr_outPlaneMeshSerializeHandler, local_outPlaneMeshSerializeHandler);

            return true;
        }
        // LnPlaneMeshUpdateHandler_Create
        case 0x60620 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outPlaneMeshUpdateHandler
            PVal* pval_outPlaneMeshUpdateHandler;
            const APTR aptr_outPlaneMeshUpdateHandler = code_getva(&pval_outPlaneMeshUpdateHandler);
            LnHandle local_outPlaneMeshUpdateHandler;

            stat = LnPlaneMeshUpdateHandler_Create(HSPSubclass_LnPlaneMeshUpdateHandler_DelegateLabelCaller, &local_outPlaneMeshUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnPlaneMeshUpdateHandler*>(LnPlaneMeshUpdateHandler_GetSubinstanceId(local_outPlaneMeshUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outPlaneMeshUpdateHandler, aptr_outPlaneMeshUpdateHandler, local_outPlaneMeshUpdateHandler);

            return true;
        }
        // LnUIEventArgsSerializeHandler_Create
        case 0x61120 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIEventArgsSerializeHandler
            PVal* pval_outUIEventArgsSerializeHandler;
            const APTR aptr_outUIEventArgsSerializeHandler = code_getva(&pval_outUIEventArgsSerializeHandler);
            LnHandle local_outUIEventArgsSerializeHandler;

            stat = LnUIEventArgsSerializeHandler_Create(HSPSubclass_LnUIEventArgsSerializeHandler_DelegateLabelCaller, &local_outUIEventArgsSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnUIEventArgsSerializeHandler*>(LnUIEventArgsSerializeHandler_GetSubinstanceId(local_outUIEventArgsSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIEventArgsSerializeHandler, aptr_outUIEventArgsSerializeHandler, local_outUIEventArgsSerializeHandler);

            return true;
        }
        // LnUILayoutElementSerializeHandler_Create
        case 0x61C20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUILayoutElementSerializeHandler
            PVal* pval_outUILayoutElementSerializeHandler;
            const APTR aptr_outUILayoutElementSerializeHandler = code_getva(&pval_outUILayoutElementSerializeHandler);
            LnHandle local_outUILayoutElementSerializeHandler;

            stat = LnUILayoutElementSerializeHandler_Create(HSPSubclass_LnUILayoutElementSerializeHandler_DelegateLabelCaller, &local_outUILayoutElementSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnUILayoutElementSerializeHandler*>(LnUILayoutElementSerializeHandler_GetSubinstanceId(local_outUILayoutElementSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUILayoutElementSerializeHandler, aptr_outUILayoutElementSerializeHandler, local_outUILayoutElementSerializeHandler);

            return true;
        }
        // LnUIElementSerializeHandler_Create
        case 0x62720 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIElementSerializeHandler
            PVal* pval_outUIElementSerializeHandler;
            const APTR aptr_outUIElementSerializeHandler = code_getva(&pval_outUIElementSerializeHandler);
            LnHandle local_outUIElementSerializeHandler;

            stat = LnUIElementSerializeHandler_Create(HSPSubclass_LnUIElementSerializeHandler_DelegateLabelCaller, &local_outUIElementSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnUIElementSerializeHandler*>(LnUIElementSerializeHandler_GetSubinstanceId(local_outUIElementSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIElementSerializeHandler, aptr_outUIElementSerializeHandler, local_outUIElementSerializeHandler);

            return true;
        }
        // LnUITextBlockSerializeHandler_Create
        case 0x63220 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUITextBlockSerializeHandler
            PVal* pval_outUITextBlockSerializeHandler;
            const APTR aptr_outUITextBlockSerializeHandler = code_getva(&pval_outUITextBlockSerializeHandler);
            LnHandle local_outUITextBlockSerializeHandler;

            stat = LnUITextBlockSerializeHandler_Create(HSPSubclass_LnUITextBlockSerializeHandler_DelegateLabelCaller, &local_outUITextBlockSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnUITextBlockSerializeHandler*>(LnUITextBlockSerializeHandler_GetSubinstanceId(local_outUITextBlockSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUITextBlockSerializeHandler, aptr_outUITextBlockSerializeHandler, local_outUITextBlockSerializeHandler);

            return true;
        }
        // LnUISpriteSerializeHandler_Create
        case 0x63D20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUISpriteSerializeHandler
            PVal* pval_outUISpriteSerializeHandler;
            const APTR aptr_outUISpriteSerializeHandler = code_getva(&pval_outUISpriteSerializeHandler);
            LnHandle local_outUISpriteSerializeHandler;

            stat = LnUISpriteSerializeHandler_Create(HSPSubclass_LnUISpriteSerializeHandler_DelegateLabelCaller, &local_outUISpriteSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnUISpriteSerializeHandler*>(LnUISpriteSerializeHandler_GetSubinstanceId(local_outUISpriteSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUISpriteSerializeHandler, aptr_outUISpriteSerializeHandler, local_outUISpriteSerializeHandler);

            return true;
        }
        // LnApplicationSerializeHandler_Create
        case 0x64820 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outApplicationSerializeHandler
            PVal* pval_outApplicationSerializeHandler;
            const APTR aptr_outApplicationSerializeHandler = code_getva(&pval_outApplicationSerializeHandler);
            LnHandle local_outApplicationSerializeHandler;

            stat = LnApplicationSerializeHandler_Create(HSPSubclass_LnApplicationSerializeHandler_DelegateLabelCaller, &local_outApplicationSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnApplicationSerializeHandler*>(LnApplicationSerializeHandler_GetSubinstanceId(local_outApplicationSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outApplicationSerializeHandler, aptr_outApplicationSerializeHandler, local_outApplicationSerializeHandler);

            return true;
        }
        // LnApplicationInitHandler_Create
        case 0x65320 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outApplicationInitHandler
            PVal* pval_outApplicationInitHandler;
            const APTR aptr_outApplicationInitHandler = code_getva(&pval_outApplicationInitHandler);
            LnHandle local_outApplicationInitHandler;

            stat = LnApplicationInitHandler_Create(HSPSubclass_LnApplicationInitHandler_DelegateLabelCaller, &local_outApplicationInitHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnApplicationInitHandler*>(LnApplicationInitHandler_GetSubinstanceId(local_outApplicationInitHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outApplicationInitHandler, aptr_outApplicationInitHandler, local_outApplicationInitHandler);

            return true;
        }
        // LnApplicationUpdateHandler_Create
        case 0x65E20 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outApplicationUpdateHandler
            PVal* pval_outApplicationUpdateHandler;
            const APTR aptr_outApplicationUpdateHandler = code_getva(&pval_outApplicationUpdateHandler);
            LnHandle local_outApplicationUpdateHandler;

            stat = LnApplicationUpdateHandler_Create(HSPSubclass_LnApplicationUpdateHandler_DelegateLabelCaller, &local_outApplicationUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LnApplicationUpdateHandler*>(LnApplicationUpdateHandler_GetSubinstanceId(local_outApplicationUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outApplicationUpdateHandler, aptr_outApplicationUpdateHandler, local_outApplicationUpdateHandler);

            return true;
        }
    }
    return false;
}

void RegisterTypes(HSP3TYPEINFO * info)
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
    {
        LnObject_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnObject_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnObject_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnObject_RegisterSubclassTypeInfo(&info);
    }
    {
        LnEventConnection_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnEventConnection_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnEventConnection_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnEventConnection_RegisterSubclassTypeInfo(&info);
    }
    {
        LnPromiseFailureDelegate_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnPromiseFailureDelegate_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnPromiseFailureDelegate_SubinstanceFree;
        LnPromiseFailureDelegate_RegisterSubclassTypeInfo(&info);
    }
    {
        LnZVTestDelegate1_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnZVTestDelegate1_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnZVTestDelegate1_SubinstanceFree;
        LnZVTestDelegate1_RegisterSubclassTypeInfo(&info);
    }
    {
        LnZVTestDelegate2_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnZVTestDelegate2_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnZVTestDelegate2_SubinstanceFree;
        LnZVTestDelegate2_RegisterSubclassTypeInfo(&info);
    }
    {
        LnZVTestDelegate3_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnZVTestDelegate3_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnZVTestDelegate3_SubinstanceFree;
        LnZVTestDelegate3_RegisterSubclassTypeInfo(&info);
    }
    {
        LnZVTestEventHandler1_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnZVTestEventHandler1_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnZVTestEventHandler1_SubinstanceFree;
        LnZVTestEventHandler1_RegisterSubclassTypeInfo(&info);
    }
    {
        LnZVTestEventHandler2_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnZVTestEventHandler2_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnZVTestEventHandler2_SubinstanceFree;
        LnZVTestEventHandler2_RegisterSubclassTypeInfo(&info);
    }
    {
        LnZVTestPromise1_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnZVTestPromise1_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnZVTestPromise1_SubinstanceFree;
        LnZVTestPromise1_RegisterSubclassTypeInfo(&info);
    }
    {
        LnZVTestPromise2_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnZVTestPromise2_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnZVTestPromise2_SubinstanceFree;
        LnZVTestPromise2_RegisterSubclassTypeInfo(&info);
    }
    {
        LnZVTestClass1_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnZVTestClass1_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnZVTestClass1_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnZVTestClass1_RegisterSubclassTypeInfo(&info);
    }
    {
        LnZVTestEventArgs1_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnZVTestEventArgs1_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnZVTestEventArgs1_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnZVTestEventArgs1_RegisterSubclassTypeInfo(&info);
    }
    {
        LnSerializer2_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnSerializer2_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnSerializer2_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnSerializer2_RegisterSubclassTypeInfo(&info);
    }
    {
        LnAssetModel_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnAssetModel_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnAssetModel_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnAssetModel_RegisterSubclassTypeInfo(&info);
    }
    {
        LnTexture2DDelegate_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnTexture2DDelegate_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnTexture2DDelegate_SubinstanceFree;
        LnTexture2DDelegate_RegisterSubclassTypeInfo(&info);
    }
    {
        LnTexture2DPromise_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnTexture2DPromise_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnTexture2DPromise_SubinstanceFree;
        LnTexture2DPromise_RegisterSubclassTypeInfo(&info);
    }
    {
        LnTexture_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnTexture_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnTexture_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnTexture_RegisterSubclassTypeInfo(&info);
    }
    {
        LnTexture2D_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnTexture2D_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnTexture2D_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnTexture2D_RegisterSubclassTypeInfo(&info);
    }
    {
        LnRenderView_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnRenderView_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnRenderView_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnRenderView_RegisterSubclassTypeInfo(&info);
    }
    {
        LnComponent_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnComponent_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnComponent_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnComponent_RegisterSubclassTypeInfo(&info);
    }
    {
        LnVisualComponent_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnVisualComponent_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnVisualComponent_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnVisualComponent_RegisterSubclassTypeInfo(&info);
    }
    {
        LnSpriteComponent_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnSpriteComponent_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnSpriteComponent_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnSpriteComponent_RegisterSubclassTypeInfo(&info);
    }
    {
        LnWorld_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnWorld_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnWorld_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnWorld_RegisterSubclassTypeInfo(&info);
    }
    {
        LnComponentList_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnComponentList_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnComponentList_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnComponentList_RegisterSubclassTypeInfo(&info);
    }
    {
        LnWorldObject_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnWorldObject_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnWorldObject_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LnWorldObject_RegisterSubclassTypeInfo(&info);
    }
    {
        LnVisualObject_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnVisualObject_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnVisualObject_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LnVisualObject_RegisterSubclassTypeInfo(&info);
    }
    {
        LnCamera_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnCamera_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnCamera_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LnCamera_RegisterSubclassTypeInfo(&info);
    }
    {
        LnDirectionalLight_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnDirectionalLight_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnDirectionalLight_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LnDirectionalLight_RegisterSubclassTypeInfo(&info);
    }
    {
        LnPointLight_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnPointLight_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnPointLight_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LnPointLight_RegisterSubclassTypeInfo(&info);
    }
    {
        LnSpotLight_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnSpotLight_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnSpotLight_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LnSpotLight_RegisterSubclassTypeInfo(&info);
    }
    {
        LnTestDelegate_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnTestDelegate_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnTestDelegate_SubinstanceFree;
        LnTestDelegate_RegisterSubclassTypeInfo(&info);
    }
    {
        LnSprite_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnSprite_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnSprite_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LnSprite_RegisterSubclassTypeInfo(&info);
    }
    {
        LnCameraOrbitControlComponent_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnCameraOrbitControlComponent_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnCameraOrbitControlComponent_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnCameraOrbitControlComponent_RegisterSubclassTypeInfo(&info);
    }
    {
        LnRaycaster_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnRaycaster_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnRaycaster_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnRaycaster_RegisterSubclassTypeInfo(&info);
    }
    {
        LnRaycastResult_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnRaycastResult_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnRaycastResult_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnRaycastResult_RegisterSubclassTypeInfo(&info);
    }
    {
        LnWorldRenderView_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnWorldRenderView_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnWorldRenderView_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnWorldRenderView_RegisterSubclassTypeInfo(&info);
    }
    {
        LnBoxMesh_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnBoxMesh_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnBoxMesh_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LnBoxMesh_RegisterSubclassTypeInfo(&info);
    }
    {
        LnPlaneMesh_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnPlaneMesh_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnPlaneMesh_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LnPlaneMesh_RegisterSubclassTypeInfo(&info);
    }
    {
        LnUIEventArgs_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnUIEventArgs_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnUIEventArgs_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnUIEventArgs_RegisterSubclassTypeInfo(&info);
    }
    {
        LnUIGeneralEventHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnUIGeneralEventHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnUIGeneralEventHandler_SubinstanceFree;
        LnUIGeneralEventHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnUIEventHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnUIEventHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnUIEventHandler_SubinstanceFree;
        LnUIEventHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnUILayoutElement_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnUILayoutElement_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnUILayoutElement_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnUILayoutElement_RegisterSubclassTypeInfo(&info);
    }
    {
        LnUIElement_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnUIElement_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnUIElement_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnUIElement_RegisterSubclassTypeInfo(&info);
    }
    {
        LnUITextBlock_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnUITextBlock_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnUITextBlock_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnUITextBlock_RegisterSubclassTypeInfo(&info);
    }
    {
        LnUISprite_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnUISprite_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnUISprite_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LnUISprite_RegisterSubclassTypeInfo(&info);
    }
    {
        LnApplication_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnApplication_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnApplication_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnInit_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LnApplication_RegisterSubclassTypeInfo(&info);
    }
    {
        LnObjectSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnObjectSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnObjectSerializeHandler_SubinstanceFree;
        LnObjectSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnEventConnectionSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnEventConnectionSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnEventConnectionSerializeHandler_SubinstanceFree;
        LnEventConnectionSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnZVTestClass1SerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnZVTestClass1SerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnZVTestClass1SerializeHandler_SubinstanceFree;
        LnZVTestClass1SerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnZVTestEventArgs1SerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnZVTestEventArgs1SerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnZVTestEventArgs1SerializeHandler_SubinstanceFree;
        LnZVTestEventArgs1SerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnSerializer2SerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnSerializer2SerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnSerializer2SerializeHandler_SubinstanceFree;
        LnSerializer2SerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnAssetModelSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnAssetModelSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnAssetModelSerializeHandler_SubinstanceFree;
        LnAssetModelSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnTextureSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnTextureSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnTextureSerializeHandler_SubinstanceFree;
        LnTextureSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnTexture2DSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnTexture2DSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnTexture2DSerializeHandler_SubinstanceFree;
        LnTexture2DSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnRenderViewSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnRenderViewSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnRenderViewSerializeHandler_SubinstanceFree;
        LnRenderViewSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnComponentSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnComponentSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnComponentSerializeHandler_SubinstanceFree;
        LnComponentSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnVisualComponentSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnVisualComponentSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnVisualComponentSerializeHandler_SubinstanceFree;
        LnVisualComponentSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnSpriteComponentSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnSpriteComponentSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnSpriteComponentSerializeHandler_SubinstanceFree;
        LnSpriteComponentSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnWorldSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnWorldSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnWorldSerializeHandler_SubinstanceFree;
        LnWorldSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnComponentListSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnComponentListSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnComponentListSerializeHandler_SubinstanceFree;
        LnComponentListSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnWorldObjectSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnWorldObjectSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnWorldObjectSerializeHandler_SubinstanceFree;
        LnWorldObjectSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnWorldObjectUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnWorldObjectUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnWorldObjectUpdateHandler_SubinstanceFree;
        LnWorldObjectUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnVisualObjectSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnVisualObjectSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnVisualObjectSerializeHandler_SubinstanceFree;
        LnVisualObjectSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnVisualObjectUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnVisualObjectUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnVisualObjectUpdateHandler_SubinstanceFree;
        LnVisualObjectUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnCameraSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnCameraSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnCameraSerializeHandler_SubinstanceFree;
        LnCameraSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnCameraUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnCameraUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnCameraUpdateHandler_SubinstanceFree;
        LnCameraUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnDirectionalLightSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnDirectionalLightSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnDirectionalLightSerializeHandler_SubinstanceFree;
        LnDirectionalLightSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnDirectionalLightUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnDirectionalLightUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnDirectionalLightUpdateHandler_SubinstanceFree;
        LnDirectionalLightUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnPointLightSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnPointLightSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnPointLightSerializeHandler_SubinstanceFree;
        LnPointLightSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnPointLightUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnPointLightUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnPointLightUpdateHandler_SubinstanceFree;
        LnPointLightUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnSpotLightSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnSpotLightSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnSpotLightSerializeHandler_SubinstanceFree;
        LnSpotLightSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnSpotLightUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnSpotLightUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnSpotLightUpdateHandler_SubinstanceFree;
        LnSpotLightUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnSpriteSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnSpriteSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnSpriteSerializeHandler_SubinstanceFree;
        LnSpriteSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnSpriteUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnSpriteUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnSpriteUpdateHandler_SubinstanceFree;
        LnSpriteUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnCameraOrbitControlComponentSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnCameraOrbitControlComponentSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnCameraOrbitControlComponentSerializeHandler_SubinstanceFree;
        LnCameraOrbitControlComponentSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnRaycasterSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnRaycasterSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnRaycasterSerializeHandler_SubinstanceFree;
        LnRaycasterSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnRaycastResultSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnRaycastResultSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnRaycastResultSerializeHandler_SubinstanceFree;
        LnRaycastResultSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnWorldRenderViewSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnWorldRenderViewSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnWorldRenderViewSerializeHandler_SubinstanceFree;
        LnWorldRenderViewSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnBoxMeshSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnBoxMeshSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnBoxMeshSerializeHandler_SubinstanceFree;
        LnBoxMeshSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnBoxMeshUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnBoxMeshUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnBoxMeshUpdateHandler_SubinstanceFree;
        LnBoxMeshUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnPlaneMeshSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnPlaneMeshSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnPlaneMeshSerializeHandler_SubinstanceFree;
        LnPlaneMeshSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnPlaneMeshUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnPlaneMeshUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnPlaneMeshUpdateHandler_SubinstanceFree;
        LnPlaneMeshUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnUIEventArgsSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnUIEventArgsSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnUIEventArgsSerializeHandler_SubinstanceFree;
        LnUIEventArgsSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnUILayoutElementSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnUILayoutElementSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnUILayoutElementSerializeHandler_SubinstanceFree;
        LnUILayoutElementSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnUIElementSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnUIElementSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnUIElementSerializeHandler_SubinstanceFree;
        LnUIElementSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnUITextBlockSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnUITextBlockSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnUITextBlockSerializeHandler_SubinstanceFree;
        LnUITextBlockSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnUISpriteSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnUISpriteSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnUISpriteSerializeHandler_SubinstanceFree;
        LnUISpriteSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnApplicationSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnApplicationSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnApplicationSerializeHandler_SubinstanceFree;
        LnApplicationSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnApplicationInitHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnApplicationInitHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnApplicationInitHandler_SubinstanceFree;
        LnApplicationInitHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LnApplicationUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LnApplicationUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LnApplicationUpdateHandler_SubinstanceFree;
        LnApplicationUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
}

