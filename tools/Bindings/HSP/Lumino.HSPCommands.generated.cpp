#include "LuminoHSP.h"

//-----------------------------------------------------------------------------
// struct LNVector3

static int g_LNVector3_typeid = 0;

int hspLNVector3_typeid()
{
    return g_LNVector3_typeid;
}

static int hspLNVector3_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LNVector3);
    return size;
}

static void hspLNVector3_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLNVector3_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LNVector3* fv = (LNVector3 *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLNVector3_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLNVector3_GetPtr(PVal* pval)
{
    return (PDAT*)(((LNVector3*)(pval->pt)) + pval->offset);
}

static int hspLNVector3_GetSize(const PDAT *pdatl)
{
    return sizeof(LNVector3);
}

static void hspLNVector3_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LNVector3*>(pdat) = *reinterpret_cast<const LNVector3*>(in);
}

static void hspLNVector3_Init(HspVarProc* p)
{
    p->Alloc = hspLNVector3_Alloc;
    p->Free = hspLNVector3_Free;
    
    p->GetPtr = hspLNVector3_GetPtr;
    p->GetSize = hspLNVector3_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLNVector3_Set;

    p->vartype_name = "LNVector3";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LNVector3);
    g_LNVector3_typeid = p->flag;
}

static void hspLNVector3_reffunc(int* typeRes, void** retValPtr)
{
    static LNVector3 returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.x = fetchVADouble_reffunc();
        returnValue.y = fetchVADouble_reffunc();
        returnValue.z = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLNVector3_typeid();
}


//-----------------------------------------------------------------------------
// struct LNVector4

static int g_LNVector4_typeid = 0;

int hspLNVector4_typeid()
{
    return g_LNVector4_typeid;
}

static int hspLNVector4_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LNVector4);
    return size;
}

static void hspLNVector4_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLNVector4_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LNVector4* fv = (LNVector4 *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLNVector4_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLNVector4_GetPtr(PVal* pval)
{
    return (PDAT*)(((LNVector4*)(pval->pt)) + pval->offset);
}

static int hspLNVector4_GetSize(const PDAT *pdatl)
{
    return sizeof(LNVector4);
}

static void hspLNVector4_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LNVector4*>(pdat) = *reinterpret_cast<const LNVector4*>(in);
}

static void hspLNVector4_Init(HspVarProc* p)
{
    p->Alloc = hspLNVector4_Alloc;
    p->Free = hspLNVector4_Free;
    
    p->GetPtr = hspLNVector4_GetPtr;
    p->GetSize = hspLNVector4_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLNVector4_Set;

    p->vartype_name = "LNVector4";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LNVector4);
    g_LNVector4_typeid = p->flag;
}

static void hspLNVector4_reffunc(int* typeRes, void** retValPtr)
{
    static LNVector4 returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.x = fetchVADouble_reffunc();
        returnValue.y = fetchVADouble_reffunc();
        returnValue.z = fetchVADouble_reffunc();
        returnValue.w = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLNVector4_typeid();
}


//-----------------------------------------------------------------------------
// struct LNQuaternion

static int g_LNQuaternion_typeid = 0;

int hspLNQuaternion_typeid()
{
    return g_LNQuaternion_typeid;
}

static int hspLNQuaternion_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LNQuaternion);
    return size;
}

static void hspLNQuaternion_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLNQuaternion_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LNQuaternion* fv = (LNQuaternion *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLNQuaternion_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLNQuaternion_GetPtr(PVal* pval)
{
    return (PDAT*)(((LNQuaternion*)(pval->pt)) + pval->offset);
}

static int hspLNQuaternion_GetSize(const PDAT *pdatl)
{
    return sizeof(LNQuaternion);
}

static void hspLNQuaternion_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LNQuaternion*>(pdat) = *reinterpret_cast<const LNQuaternion*>(in);
}

static void hspLNQuaternion_Init(HspVarProc* p)
{
    p->Alloc = hspLNQuaternion_Alloc;
    p->Free = hspLNQuaternion_Free;
    
    p->GetPtr = hspLNQuaternion_GetPtr;
    p->GetSize = hspLNQuaternion_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLNQuaternion_Set;

    p->vartype_name = "LNQuaternion";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LNQuaternion);
    g_LNQuaternion_typeid = p->flag;
}

static void hspLNQuaternion_reffunc(int* typeRes, void** retValPtr)
{
    static LNQuaternion returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.x = fetchVADouble_reffunc();
        returnValue.y = fetchVADouble_reffunc();
        returnValue.z = fetchVADouble_reffunc();
        returnValue.w = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLNQuaternion_typeid();
}


//-----------------------------------------------------------------------------
// struct LNMatrix

static int g_LNMatrix_typeid = 0;

int hspLNMatrix_typeid()
{
    return g_LNMatrix_typeid;
}

static int hspLNMatrix_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LNMatrix);
    return size;
}

static void hspLNMatrix_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLNMatrix_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LNMatrix* fv = (LNMatrix *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLNMatrix_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLNMatrix_GetPtr(PVal* pval)
{
    return (PDAT*)(((LNMatrix*)(pval->pt)) + pval->offset);
}

static int hspLNMatrix_GetSize(const PDAT *pdatl)
{
    return sizeof(LNMatrix);
}

static void hspLNMatrix_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LNMatrix*>(pdat) = *reinterpret_cast<const LNMatrix*>(in);
}

static void hspLNMatrix_Init(HspVarProc* p)
{
    p->Alloc = hspLNMatrix_Alloc;
    p->Free = hspLNMatrix_Free;
    
    p->GetPtr = hspLNMatrix_GetPtr;
    p->GetSize = hspLNMatrix_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLNMatrix_Set;

    p->vartype_name = "LNMatrix";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LNMatrix);
    g_LNMatrix_typeid = p->flag;
}

static void hspLNMatrix_reffunc(int* typeRes, void** retValPtr)
{
    static LNMatrix returnValue;


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



    *retValPtr = &returnValue;
    *typeRes = hspLNMatrix_typeid();
}


//-----------------------------------------------------------------------------
// struct LNColor

static int g_LNColor_typeid = 0;

int hspLNColor_typeid()
{
    return g_LNColor_typeid;
}

static int hspLNColor_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LNColor);
    return size;
}

static void hspLNColor_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLNColor_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LNColor* fv = (LNColor *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLNColor_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLNColor_GetPtr(PVal* pval)
{
    return (PDAT*)(((LNColor*)(pval->pt)) + pval->offset);
}

static int hspLNColor_GetSize(const PDAT *pdatl)
{
    return sizeof(LNColor);
}

static void hspLNColor_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LNColor*>(pdat) = *reinterpret_cast<const LNColor*>(in);
}

static void hspLNColor_Init(HspVarProc* p)
{
    p->Alloc = hspLNColor_Alloc;
    p->Free = hspLNColor_Free;
    
    p->GetPtr = hspLNColor_GetPtr;
    p->GetSize = hspLNColor_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLNColor_Set;

    p->vartype_name = "LNColor";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LNColor);
    g_LNColor_typeid = p->flag;
}

static void hspLNColor_reffunc(int* typeRes, void** retValPtr)
{
    static LNColor returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.r = fetchVADouble_reffunc();
        returnValue.g = fetchVADouble_reffunc();
        returnValue.b = fetchVADouble_reffunc();
        returnValue.a = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLNColor_typeid();
}


//-----------------------------------------------------------------------------
// struct LNPoint

static int g_LNPoint_typeid = 0;

int hspLNPoint_typeid()
{
    return g_LNPoint_typeid;
}

static int hspLNPoint_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LNPoint);
    return size;
}

static void hspLNPoint_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLNPoint_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LNPoint* fv = (LNPoint *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLNPoint_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLNPoint_GetPtr(PVal* pval)
{
    return (PDAT*)(((LNPoint*)(pval->pt)) + pval->offset);
}

static int hspLNPoint_GetSize(const PDAT *pdatl)
{
    return sizeof(LNPoint);
}

static void hspLNPoint_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LNPoint*>(pdat) = *reinterpret_cast<const LNPoint*>(in);
}

static void hspLNPoint_Init(HspVarProc* p)
{
    p->Alloc = hspLNPoint_Alloc;
    p->Free = hspLNPoint_Free;
    
    p->GetPtr = hspLNPoint_GetPtr;
    p->GetSize = hspLNPoint_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLNPoint_Set;

    p->vartype_name = "LNPoint";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LNPoint);
    g_LNPoint_typeid = p->flag;
}

static void hspLNPoint_reffunc(int* typeRes, void** retValPtr)
{
    static LNPoint returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.x = fetchVADouble_reffunc();
        returnValue.y = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLNPoint_typeid();
}


//-----------------------------------------------------------------------------
// struct LNSize

static int g_LNSize_typeid = 0;

int hspLNSize_typeid()
{
    return g_LNSize_typeid;
}

static int hspLNSize_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LNSize);
    return size;
}

static void hspLNSize_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLNSize_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LNSize* fv = (LNSize *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLNSize_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLNSize_GetPtr(PVal* pval)
{
    return (PDAT*)(((LNSize*)(pval->pt)) + pval->offset);
}

static int hspLNSize_GetSize(const PDAT *pdatl)
{
    return sizeof(LNSize);
}

static void hspLNSize_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LNSize*>(pdat) = *reinterpret_cast<const LNSize*>(in);
}

static void hspLNSize_Init(HspVarProc* p)
{
    p->Alloc = hspLNSize_Alloc;
    p->Free = hspLNSize_Free;
    
    p->GetPtr = hspLNSize_GetPtr;
    p->GetSize = hspLNSize_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLNSize_Set;

    p->vartype_name = "LNSize";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LNSize);
    g_LNSize_typeid = p->flag;
}

static void hspLNSize_reffunc(int* typeRes, void** retValPtr)
{
    static LNSize returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.width = fetchVADouble_reffunc();
        returnValue.height = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLNSize_typeid();
}


//-----------------------------------------------------------------------------
// struct LNRect

static int g_LNRect_typeid = 0;

int hspLNRect_typeid()
{
    return g_LNRect_typeid;
}

static int hspLNRect_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LNRect);
    return size;
}

static void hspLNRect_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLNRect_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LNRect* fv = (LNRect *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLNRect_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLNRect_GetPtr(PVal* pval)
{
    return (PDAT*)(((LNRect*)(pval->pt)) + pval->offset);
}

static int hspLNRect_GetSize(const PDAT *pdatl)
{
    return sizeof(LNRect);
}

static void hspLNRect_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LNRect*>(pdat) = *reinterpret_cast<const LNRect*>(in);
}

static void hspLNRect_Init(HspVarProc* p)
{
    p->Alloc = hspLNRect_Alloc;
    p->Free = hspLNRect_Free;
    
    p->GetPtr = hspLNRect_GetPtr;
    p->GetSize = hspLNRect_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLNRect_Set;

    p->vartype_name = "LNRect";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LNRect);
    g_LNRect_typeid = p->flag;
}

static void hspLNRect_reffunc(int* typeRes, void** retValPtr)
{
    static LNRect returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.x = fetchVADouble_reffunc();
        returnValue.y = fetchVADouble_reffunc();
        returnValue.width = fetchVADouble_reffunc();
        returnValue.height = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLNRect_typeid();
}


//-----------------------------------------------------------------------------
// struct LNThickness

static int g_LNThickness_typeid = 0;

int hspLNThickness_typeid()
{
    return g_LNThickness_typeid;
}

static int hspLNThickness_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LNThickness);
    return size;
}

static void hspLNThickness_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLNThickness_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LNThickness* fv = (LNThickness *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLNThickness_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLNThickness_GetPtr(PVal* pval)
{
    return (PDAT*)(((LNThickness*)(pval->pt)) + pval->offset);
}

static int hspLNThickness_GetSize(const PDAT *pdatl)
{
    return sizeof(LNThickness);
}

static void hspLNThickness_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LNThickness*>(pdat) = *reinterpret_cast<const LNThickness*>(in);
}

static void hspLNThickness_Init(HspVarProc* p)
{
    p->Alloc = hspLNThickness_Alloc;
    p->Free = hspLNThickness_Free;
    
    p->GetPtr = hspLNThickness_GetPtr;
    p->GetSize = hspLNThickness_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLNThickness_Set;

    p->vartype_name = "LNThickness";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LNThickness);
    g_LNThickness_typeid = p->flag;
}

static void hspLNThickness_reffunc(int* typeRes, void** retValPtr)
{
    static LNThickness returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.left = fetchVADouble_reffunc();
        returnValue.top = fetchVADouble_reffunc();
        returnValue.right = fetchVADouble_reffunc();
        returnValue.bottom = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLNThickness_typeid();
}


//==============================================================================
// ln::Object

struct HSPSubclass_LNObject
{
};


static LNSubinstanceId HSPSubclass_LNObject_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNObject)));
}

static void HSPSubclass_LNObject_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::EventConnection

struct HSPSubclass_LNEventConnection
{
};


static LNSubinstanceId HSPSubclass_LNEventConnection_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNEventConnection)));
}

static void HSPSubclass_LNEventConnection_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::PromiseFailureDelegate

struct HSPSubclass_LNPromiseFailureDelegate
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNPromiseFailureDelegate_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNPromiseFailureDelegate)));
}

static void HSPSubclass_LNPromiseFailureDelegate_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNPromiseFailureDelegate_DelegateLabelCaller(LNHandle promisefailuredelegate)
{
    setCallbackArg(0, promisefailuredelegate);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNPromiseFailureDelegate*>(LNPromiseFailureDelegate_GetSubinstanceId(promisefailuredelegate));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ln::ZVTestDelegate1

struct HSPSubclass_LNZVTestDelegate1
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNZVTestDelegate1_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNZVTestDelegate1)));
}

static void HSPSubclass_LNZVTestDelegate1_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNZVTestDelegate1_DelegateLabelCaller(LNHandle zvtestdelegate1, int p1)
{
    setCallbackArg(0, zvtestdelegate1);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate1*>(LNZVTestDelegate1_GetSubinstanceId(zvtestdelegate1));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ln::ZVTestDelegate2

struct HSPSubclass_LNZVTestDelegate2
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNZVTestDelegate2_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNZVTestDelegate2)));
}

static void HSPSubclass_LNZVTestDelegate2_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNZVTestDelegate2_DelegateLabelCaller(LNHandle zvtestdelegate2, int p1, int p2, int* outReturn)
{
    setCallbackArg(0, zvtestdelegate2);
    setCallbackArg(1, p1);
    setCallbackArg(2, p2);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate2*>(LNZVTestDelegate2_GetSubinstanceId(zvtestdelegate2));
    stat = 0;
    code_call(localSelf->labelPointer);
    setCallbackOutput(3, outReturn);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ln::ZVTestDelegate3

struct HSPSubclass_LNZVTestDelegate3
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNZVTestDelegate3_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNZVTestDelegate3)));
}

static void HSPSubclass_LNZVTestDelegate3_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNZVTestDelegate3_DelegateLabelCaller(LNHandle zvtestdelegate3, LNHandle p1)
{
    setCallbackArg(0, zvtestdelegate3);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate3*>(LNZVTestDelegate3_GetSubinstanceId(zvtestdelegate3));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ln::ZVTestEventHandler1

struct HSPSubclass_LNZVTestEventHandler1
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNZVTestEventHandler1_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNZVTestEventHandler1)));
}

static void HSPSubclass_LNZVTestEventHandler1_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNZVTestEventHandler1_DelegateLabelCaller(LNHandle zvtesteventhandler1)
{
    setCallbackArg(0, zvtesteventhandler1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestEventHandler1*>(LNZVTestEventHandler1_GetSubinstanceId(zvtesteventhandler1));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ln::ZVTestEventHandler2

struct HSPSubclass_LNZVTestEventHandler2
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNZVTestEventHandler2_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNZVTestEventHandler2)));
}

static void HSPSubclass_LNZVTestEventHandler2_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNZVTestEventHandler2_DelegateLabelCaller(LNHandle zvtesteventhandler2, LNHandle p1)
{
    setCallbackArg(0, zvtesteventhandler2);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestEventHandler2*>(LNZVTestEventHandler2_GetSubinstanceId(zvtesteventhandler2));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ln::ZVTestPromise1

struct HSPSubclass_LNZVTestPromise1
{
};


static LNSubinstanceId HSPSubclass_LNZVTestPromise1_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNZVTestPromise1)));
}

static void HSPSubclass_LNZVTestPromise1_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::ZVTestPromise2

struct HSPSubclass_LNZVTestPromise2
{
};


static LNSubinstanceId HSPSubclass_LNZVTestPromise2_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNZVTestPromise2)));
}

static void HSPSubclass_LNZVTestPromise2_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::ZVTestClass1

struct HSPSubclass_LNZVTestClass1
{
};


static LNSubinstanceId HSPSubclass_LNZVTestClass1_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNZVTestClass1)));
}

static void HSPSubclass_LNZVTestClass1_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::ZVTestEventArgs1

struct HSPSubclass_LNZVTestEventArgs1
{
};


static LNSubinstanceId HSPSubclass_LNZVTestEventArgs1_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNZVTestEventArgs1)));
}

static void HSPSubclass_LNZVTestEventArgs1_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Serializer2

struct HSPSubclass_LNSerializer2
{
};


static LNSubinstanceId HSPSubclass_LNSerializer2_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSerializer2)));
}

static void HSPSubclass_LNSerializer2_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::AssetModel

struct HSPSubclass_LNAssetModel
{
};


static LNSubinstanceId HSPSubclass_LNAssetModel_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAssetModel)));
}

static void HSPSubclass_LNAssetModel_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Assets

struct HSPSubclass_LNAssets
{
};


static LNSubinstanceId HSPSubclass_LNAssets_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAssets)));
}

static void HSPSubclass_LNAssets_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Texture2DDelegate

struct HSPSubclass_LNTexture2DDelegate
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNTexture2DDelegate_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNTexture2DDelegate)));
}

static void HSPSubclass_LNTexture2DDelegate_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNTexture2DDelegate_DelegateLabelCaller(LNHandle texture2ddelegate, LNHandle p1)
{
    setCallbackArg(0, texture2ddelegate);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNTexture2DDelegate*>(LNTexture2DDelegate_GetSubinstanceId(texture2ddelegate));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ln::Texture2DPromise

struct HSPSubclass_LNTexture2DPromise
{
};


static LNSubinstanceId HSPSubclass_LNTexture2DPromise_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNTexture2DPromise)));
}

static void HSPSubclass_LNTexture2DPromise_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Texture

struct HSPSubclass_LNTexture
{
};


static LNSubinstanceId HSPSubclass_LNTexture_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNTexture)));
}

static void HSPSubclass_LNTexture_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Texture2D

struct HSPSubclass_LNTexture2D
{
};


static LNSubinstanceId HSPSubclass_LNTexture2D_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNTexture2D)));
}

static void HSPSubclass_LNTexture2D_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::RenderView

struct HSPSubclass_LNRenderView
{
};


static LNSubinstanceId HSPSubclass_LNRenderView_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNRenderView)));
}

static void HSPSubclass_LNRenderView_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::SkinnedMeshModel

struct HSPSubclass_LNSkinnedMeshModel
{
};


static LNSubinstanceId HSPSubclass_LNSkinnedMeshModel_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSkinnedMeshModel)));
}

static void HSPSubclass_LNSkinnedMeshModel_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::CollisionShape

struct HSPSubclass_LNCollisionShape
{
};


static LNSubinstanceId HSPSubclass_LNCollisionShape_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNCollisionShape)));
}

static void HSPSubclass_LNCollisionShape_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::BoxCollisionShape

struct HSPSubclass_LNBoxCollisionShape
{
};


static LNSubinstanceId HSPSubclass_LNBoxCollisionShape_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNBoxCollisionShape)));
}

static void HSPSubclass_LNBoxCollisionShape_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Component

struct HSPSubclass_LNComponent
{
};


static LNSubinstanceId HSPSubclass_LNComponent_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNComponent)));
}

static void HSPSubclass_LNComponent_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::VisualComponent

struct HSPSubclass_LNVisualComponent
{
};


static LNSubinstanceId HSPSubclass_LNVisualComponent_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNVisualComponent)));
}

static void HSPSubclass_LNVisualComponent_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::SpriteComponent

struct HSPSubclass_LNSpriteComponent
{
};


static LNSubinstanceId HSPSubclass_LNSpriteComponent_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSpriteComponent)));
}

static void HSPSubclass_LNSpriteComponent_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::CollisionEventHandler

struct HSPSubclass_LNCollisionEventHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNCollisionEventHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNCollisionEventHandler)));
}

static void HSPSubclass_LNCollisionEventHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNCollisionEventHandler_DelegateLabelCaller(LNHandle collisioneventhandler, LNHandle p1)
{
    setCallbackArg(0, collisioneventhandler);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionEventHandler*>(LNCollisionEventHandler_GetSubinstanceId(collisioneventhandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ln::CharacterController

struct HSPSubclass_LNCharacterController
{
};


static LNSubinstanceId HSPSubclass_LNCharacterController_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNCharacterController)));
}

static void HSPSubclass_LNCharacterController_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::World

struct HSPSubclass_LNWorld
{
};


static LNSubinstanceId HSPSubclass_LNWorld_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNWorld)));
}

static void HSPSubclass_LNWorld_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::ComponentList

struct HSPSubclass_LNComponentList
{
};


static LNSubinstanceId HSPSubclass_LNComponentList_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNComponentList)));
}

static void HSPSubclass_LNComponentList_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::WorldObject

struct HSPSubclass_LNWorldObject
{
};


static LNSubinstanceId HSPSubclass_LNWorldObject_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNWorldObject)));
}

static void HSPSubclass_LNWorldObject_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::VisualObject

struct HSPSubclass_LNVisualObject
{
};


static LNSubinstanceId HSPSubclass_LNVisualObject_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNVisualObject)));
}

static void HSPSubclass_LNVisualObject_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Camera

struct HSPSubclass_LNCamera
{
};


static LNSubinstanceId HSPSubclass_LNCamera_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNCamera)));
}

static void HSPSubclass_LNCamera_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::EnvironmentLight

struct HSPSubclass_LNEnvironmentLight
{
};


static LNSubinstanceId HSPSubclass_LNEnvironmentLight_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNEnvironmentLight)));
}

static void HSPSubclass_LNEnvironmentLight_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::DirectionalLight

struct HSPSubclass_LNDirectionalLight
{
};


static LNSubinstanceId HSPSubclass_LNDirectionalLight_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNDirectionalLight)));
}

static void HSPSubclass_LNDirectionalLight_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::PointLight

struct HSPSubclass_LNPointLight
{
};


static LNSubinstanceId HSPSubclass_LNPointLight_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNPointLight)));
}

static void HSPSubclass_LNPointLight_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::SpotLight

struct HSPSubclass_LNSpotLight
{
};


static LNSubinstanceId HSPSubclass_LNSpotLight_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSpotLight)));
}

static void HSPSubclass_LNSpotLight_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::TestDelegate

struct HSPSubclass_LNTestDelegate
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNTestDelegate_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNTestDelegate)));
}

static void HSPSubclass_LNTestDelegate_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNTestDelegate_DelegateLabelCaller(LNHandle testdelegate, int p1, int* outReturn)
{
    setCallbackArg(0, testdelegate);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNTestDelegate*>(LNTestDelegate_GetSubinstanceId(testdelegate));
    stat = 0;
    code_call(localSelf->labelPointer);
    setCallbackOutput(2, outReturn);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ln::Sprite

struct HSPSubclass_LNSprite
{
};


static LNSubinstanceId HSPSubclass_LNSprite_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSprite)));
}

static void HSPSubclass_LNSprite_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::CameraOrbitControlComponent

struct HSPSubclass_LNCameraOrbitControlComponent
{
};


static LNSubinstanceId HSPSubclass_LNCameraOrbitControlComponent_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNCameraOrbitControlComponent)));
}

static void HSPSubclass_LNCameraOrbitControlComponent_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Raycaster

struct HSPSubclass_LNRaycaster
{
};


static LNSubinstanceId HSPSubclass_LNRaycaster_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNRaycaster)));
}

static void HSPSubclass_LNRaycaster_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::RaycastResult

struct HSPSubclass_LNRaycastResult
{
};


static LNSubinstanceId HSPSubclass_LNRaycastResult_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNRaycastResult)));
}

static void HSPSubclass_LNRaycastResult_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::WorldRenderView

struct HSPSubclass_LNWorldRenderView
{
};


static LNSubinstanceId HSPSubclass_LNWorldRenderView_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNWorldRenderView)));
}

static void HSPSubclass_LNWorldRenderView_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::BoxMesh

struct HSPSubclass_LNBoxMesh
{
};


static LNSubinstanceId HSPSubclass_LNBoxMesh_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNBoxMesh)));
}

static void HSPSubclass_LNBoxMesh_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::PlaneMesh

struct HSPSubclass_LNPlaneMesh
{
};


static LNSubinstanceId HSPSubclass_LNPlaneMesh_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNPlaneMesh)));
}

static void HSPSubclass_LNPlaneMesh_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::StaticMesh

struct HSPSubclass_LNStaticMesh
{
};


static LNSubinstanceId HSPSubclass_LNStaticMesh_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNStaticMesh)));
}

static void HSPSubclass_LNStaticMesh_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::SkinnedMeshComponent

struct HSPSubclass_LNSkinnedMeshComponent
{
};


static LNSubinstanceId HSPSubclass_LNSkinnedMeshComponent_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSkinnedMeshComponent)));
}

static void HSPSubclass_LNSkinnedMeshComponent_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Collision

struct HSPSubclass_LNCollision
{
};


static LNSubinstanceId HSPSubclass_LNCollision_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNCollision)));
}

static void HSPSubclass_LNCollision_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::TriggerBodyComponent

struct HSPSubclass_LNTriggerBodyComponent
{
};


static LNSubinstanceId HSPSubclass_LNTriggerBodyComponent_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNTriggerBodyComponent)));
}

static void HSPSubclass_LNTriggerBodyComponent_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Scene

struct HSPSubclass_LNScene
{
};


static LNSubinstanceId HSPSubclass_LNScene_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNScene)));
}

static void HSPSubclass_LNScene_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Level

struct HSPSubclass_LNLevel
{
};


static LNSubinstanceId HSPSubclass_LNLevel_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNLevel)));
}

static void HSPSubclass_LNLevel_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIEventArgs

struct HSPSubclass_LNUIEventArgs
{
};


static LNSubinstanceId HSPSubclass_LNUIEventArgs_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIEventArgs)));
}

static void HSPSubclass_LNUIEventArgs_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIGeneralEventHandler

struct HSPSubclass_LNUIGeneralEventHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIGeneralEventHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIGeneralEventHandler)));
}

static void HSPSubclass_LNUIGeneralEventHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIGeneralEventHandler_DelegateLabelCaller(LNHandle uigeneraleventhandler, LNHandle p1)
{
    setCallbackArg(0, uigeneraleventhandler);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIGeneralEventHandler*>(LNUIGeneralEventHandler_GetSubinstanceId(uigeneraleventhandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ln::UIEventHandler

struct HSPSubclass_LNUIEventHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIEventHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIEventHandler)));
}

static void HSPSubclass_LNUIEventHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIEventHandler_DelegateLabelCaller(LNHandle uieventhandler)
{
    setCallbackArg(0, uieventhandler);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIEventHandler*>(LNUIEventHandler_GetSubinstanceId(uieventhandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ln::UILayoutElement

struct HSPSubclass_LNUILayoutElement
{
};


static LNSubinstanceId HSPSubclass_LNUILayoutElement_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUILayoutElement)));
}

static void HSPSubclass_LNUILayoutElement_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIElement

struct HSPSubclass_LNUIElement
{
};


static LNSubinstanceId HSPSubclass_LNUIElement_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIElement)));
}

static void HSPSubclass_LNUIElement_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UITextBlock

struct HSPSubclass_LNUITextBlock
{
};


static LNSubinstanceId HSPSubclass_LNUITextBlock_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUITextBlock)));
}

static void HSPSubclass_LNUITextBlock_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UISprite

struct HSPSubclass_LNUISprite
{
};


static LNSubinstanceId HSPSubclass_LNUISprite_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUISprite)));
}

static void HSPSubclass_LNUISprite_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIIcon

struct HSPSubclass_LNUIIcon
{
};


static LNSubinstanceId HSPSubclass_LNUIIcon_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIIcon)));
}

static void HSPSubclass_LNUIIcon_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UI

struct HSPSubclass_LNUI
{
};


static LNSubinstanceId HSPSubclass_LNUI_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUI)));
}

static void HSPSubclass_LNUI_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UILayoutPanel

struct HSPSubclass_LNUILayoutPanel
{
};


static LNSubinstanceId HSPSubclass_LNUILayoutPanel_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUILayoutPanel)));
}

static void HSPSubclass_LNUILayoutPanel_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIBoxLayout

struct HSPSubclass_LNUIBoxLayout
{
};


static LNSubinstanceId HSPSubclass_LNUIBoxLayout_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIBoxLayout)));
}

static void HSPSubclass_LNUIBoxLayout_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIGridLayout

struct HSPSubclass_LNUIGridLayout
{
};


static LNSubinstanceId HSPSubclass_LNUIGridLayout_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIGridLayout)));
}

static void HSPSubclass_LNUIGridLayout_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIControl

struct HSPSubclass_LNUIControl
{
};


static LNSubinstanceId HSPSubclass_LNUIControl_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIControl)));
}

static void HSPSubclass_LNUIControl_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIButtonBase

struct HSPSubclass_LNUIButtonBase
{
};


static LNSubinstanceId HSPSubclass_LNUIButtonBase_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIButtonBase)));
}

static void HSPSubclass_LNUIButtonBase_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIButton

struct HSPSubclass_LNUIButton
{
};


static LNSubinstanceId HSPSubclass_LNUIButton_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIButton)));
}

static void HSPSubclass_LNUIButton_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIWindow

struct HSPSubclass_LNUIWindow
{
};


static LNSubinstanceId HSPSubclass_LNUIWindow_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIWindow)));
}

static void HSPSubclass_LNUIWindow_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIListItem

struct HSPSubclass_LNUIListItem
{
};


static LNSubinstanceId HSPSubclass_LNUIListItem_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIListItem)));
}

static void HSPSubclass_LNUIListItem_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIListItemsControl

struct HSPSubclass_LNUIListItemsControl
{
};


static LNSubinstanceId HSPSubclass_LNUIListItemsControl_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIListItemsControl)));
}

static void HSPSubclass_LNUIListItemsControl_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIListBoxItem

struct HSPSubclass_LNUIListBoxItem
{
};


static LNSubinstanceId HSPSubclass_LNUIListBoxItem_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIListBoxItem)));
}

static void HSPSubclass_LNUIListBoxItem_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::UIListBox

struct HSPSubclass_LNUIListBox
{
};


static LNSubinstanceId HSPSubclass_LNUIListBox_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIListBox)));
}

static void HSPSubclass_LNUIListBox_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Input

struct HSPSubclass_LNInput
{
};


static LNSubinstanceId HSPSubclass_LNInput_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNInput)));
}

static void HSPSubclass_LNInput_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Mouse

struct HSPSubclass_LNMouse
{
};


static LNSubinstanceId HSPSubclass_LNMouse_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNMouse)));
}

static void HSPSubclass_LNMouse_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::InterpreterCommand

struct HSPSubclass_LNInterpreterCommand
{
};


static LNSubinstanceId HSPSubclass_LNInterpreterCommand_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNInterpreterCommand)));
}

static void HSPSubclass_LNInterpreterCommand_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::InterpreterCommandList

struct HSPSubclass_LNInterpreterCommandList
{
};


static LNSubinstanceId HSPSubclass_LNInterpreterCommandList_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNInterpreterCommandList)));
}

static void HSPSubclass_LNInterpreterCommandList_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::InterpreterCommandDelegate

struct HSPSubclass_LNInterpreterCommandDelegate
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNInterpreterCommandDelegate_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNInterpreterCommandDelegate)));
}

static void HSPSubclass_LNInterpreterCommandDelegate_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNInterpreterCommandDelegate_DelegateLabelCaller(LNHandle interpretercommanddelegate, LNHandle p1, LNBool* outReturn)
{
    setCallbackArg(0, interpretercommanddelegate);
    setCallbackArg(1, p1);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterCommandDelegate*>(LNInterpreterCommandDelegate_GetSubinstanceId(interpretercommanddelegate));
    stat = 0;
    code_call(localSelf->labelPointer);
    setCallbackOutput(2, outReturn);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ln::Interpreter

struct HSPSubclass_LNInterpreter
{
};


static LNSubinstanceId HSPSubclass_LNInterpreter_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNInterpreter)));
}

static void HSPSubclass_LNInterpreter_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::EngineSettings

struct HSPSubclass_LNEngineSettings
{
};


static LNSubinstanceId HSPSubclass_LNEngineSettings_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNEngineSettings)));
}

static void HSPSubclass_LNEngineSettings_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Engine

struct HSPSubclass_LNEngine
{
};


static LNSubinstanceId HSPSubclass_LNEngine_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNEngine)));
}

static void HSPSubclass_LNEngine_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Application

struct HSPSubclass_LNApplication
{
};


static LNSubinstanceId HSPSubclass_LNApplication_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNApplication)));
}

static void HSPSubclass_LNApplication_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Debug

struct HSPSubclass_LNDebug
{
};


static LNSubinstanceId HSPSubclass_LNDebug_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNDebug)));
}

static void HSPSubclass_LNDebug_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ObjectSerializeHandler

struct HSPSubclass_LNObjectSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNObjectSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNObjectSerializeHandler)));
}

static void HSPSubclass_LNObjectSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNObjectSerializeHandler_DelegateLabelCaller(LNHandle objectserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, objectserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNObjectSerializeHandler*>(LNObjectSerializeHandler_GetSubinstanceId(objectserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// EventConnectionSerializeHandler

struct HSPSubclass_LNEventConnectionSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNEventConnectionSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNEventConnectionSerializeHandler)));
}

static void HSPSubclass_LNEventConnectionSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNEventConnectionSerializeHandler_DelegateLabelCaller(LNHandle eventconnectionserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, eventconnectionserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNEventConnectionSerializeHandler*>(LNEventConnectionSerializeHandler_GetSubinstanceId(eventconnectionserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ZVTestClass1SerializeHandler

struct HSPSubclass_LNZVTestClass1SerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNZVTestClass1SerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNZVTestClass1SerializeHandler)));
}

static void HSPSubclass_LNZVTestClass1SerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNZVTestClass1SerializeHandler_DelegateLabelCaller(LNHandle zvtestclass1serializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, zvtestclass1serializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestClass1SerializeHandler*>(LNZVTestClass1SerializeHandler_GetSubinstanceId(zvtestclass1serializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ZVTestEventArgs1SerializeHandler

struct HSPSubclass_LNZVTestEventArgs1SerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNZVTestEventArgs1SerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNZVTestEventArgs1SerializeHandler)));
}

static void HSPSubclass_LNZVTestEventArgs1SerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNZVTestEventArgs1SerializeHandler_DelegateLabelCaller(LNHandle zvtesteventargs1serializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, zvtesteventargs1serializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestEventArgs1SerializeHandler*>(LNZVTestEventArgs1SerializeHandler_GetSubinstanceId(zvtesteventargs1serializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// Serializer2SerializeHandler

struct HSPSubclass_LNSerializer2SerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNSerializer2SerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSerializer2SerializeHandler)));
}

static void HSPSubclass_LNSerializer2SerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNSerializer2SerializeHandler_DelegateLabelCaller(LNHandle serializer2serializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, serializer2serializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNSerializer2SerializeHandler*>(LNSerializer2SerializeHandler_GetSubinstanceId(serializer2serializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// AssetModelSerializeHandler

struct HSPSubclass_LNAssetModelSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNAssetModelSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAssetModelSerializeHandler)));
}

static void HSPSubclass_LNAssetModelSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNAssetModelSerializeHandler_DelegateLabelCaller(LNHandle assetmodelserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, assetmodelserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNAssetModelSerializeHandler*>(LNAssetModelSerializeHandler_GetSubinstanceId(assetmodelserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// TextureSerializeHandler

struct HSPSubclass_LNTextureSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNTextureSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNTextureSerializeHandler)));
}

static void HSPSubclass_LNTextureSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNTextureSerializeHandler_DelegateLabelCaller(LNHandle textureserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, textureserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNTextureSerializeHandler*>(LNTextureSerializeHandler_GetSubinstanceId(textureserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// Texture2DSerializeHandler

struct HSPSubclass_LNTexture2DSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNTexture2DSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNTexture2DSerializeHandler)));
}

static void HSPSubclass_LNTexture2DSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNTexture2DSerializeHandler_DelegateLabelCaller(LNHandle texture2dserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, texture2dserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNTexture2DSerializeHandler*>(LNTexture2DSerializeHandler_GetSubinstanceId(texture2dserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// RenderViewSerializeHandler

struct HSPSubclass_LNRenderViewSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNRenderViewSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNRenderViewSerializeHandler)));
}

static void HSPSubclass_LNRenderViewSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNRenderViewSerializeHandler_DelegateLabelCaller(LNHandle renderviewserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, renderviewserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNRenderViewSerializeHandler*>(LNRenderViewSerializeHandler_GetSubinstanceId(renderviewserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// SkinnedMeshModelSerializeHandler

struct HSPSubclass_LNSkinnedMeshModelSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNSkinnedMeshModelSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSkinnedMeshModelSerializeHandler)));
}

static void HSPSubclass_LNSkinnedMeshModelSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNSkinnedMeshModelSerializeHandler_DelegateLabelCaller(LNHandle skinnedmeshmodelserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, skinnedmeshmodelserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNSkinnedMeshModelSerializeHandler*>(LNSkinnedMeshModelSerializeHandler_GetSubinstanceId(skinnedmeshmodelserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// CollisionShapeSerializeHandler

struct HSPSubclass_LNCollisionShapeSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNCollisionShapeSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNCollisionShapeSerializeHandler)));
}

static void HSPSubclass_LNCollisionShapeSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNCollisionShapeSerializeHandler_DelegateLabelCaller(LNHandle collisionshapeserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, collisionshapeserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionShapeSerializeHandler*>(LNCollisionShapeSerializeHandler_GetSubinstanceId(collisionshapeserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// BoxCollisionShapeSerializeHandler

struct HSPSubclass_LNBoxCollisionShapeSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNBoxCollisionShapeSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNBoxCollisionShapeSerializeHandler)));
}

static void HSPSubclass_LNBoxCollisionShapeSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNBoxCollisionShapeSerializeHandler_DelegateLabelCaller(LNHandle boxcollisionshapeserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, boxcollisionshapeserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxCollisionShapeSerializeHandler*>(LNBoxCollisionShapeSerializeHandler_GetSubinstanceId(boxcollisionshapeserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ComponentSerializeHandler

struct HSPSubclass_LNComponentSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNComponentSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNComponentSerializeHandler)));
}

static void HSPSubclass_LNComponentSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNComponentSerializeHandler_DelegateLabelCaller(LNHandle componentserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, componentserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNComponentSerializeHandler*>(LNComponentSerializeHandler_GetSubinstanceId(componentserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// VisualComponentSerializeHandler

struct HSPSubclass_LNVisualComponentSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNVisualComponentSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNVisualComponentSerializeHandler)));
}

static void HSPSubclass_LNVisualComponentSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNVisualComponentSerializeHandler_DelegateLabelCaller(LNHandle visualcomponentserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, visualcomponentserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualComponentSerializeHandler*>(LNVisualComponentSerializeHandler_GetSubinstanceId(visualcomponentserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// SpriteComponentSerializeHandler

struct HSPSubclass_LNSpriteComponentSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNSpriteComponentSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSpriteComponentSerializeHandler)));
}

static void HSPSubclass_LNSpriteComponentSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNSpriteComponentSerializeHandler_DelegateLabelCaller(LNHandle spritecomponentserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, spritecomponentserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNSpriteComponentSerializeHandler*>(LNSpriteComponentSerializeHandler_GetSubinstanceId(spritecomponentserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// CharacterControllerSerializeHandler

struct HSPSubclass_LNCharacterControllerSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNCharacterControllerSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNCharacterControllerSerializeHandler)));
}

static void HSPSubclass_LNCharacterControllerSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNCharacterControllerSerializeHandler_DelegateLabelCaller(LNHandle charactercontrollerserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, charactercontrollerserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNCharacterControllerSerializeHandler*>(LNCharacterControllerSerializeHandler_GetSubinstanceId(charactercontrollerserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// WorldSerializeHandler

struct HSPSubclass_LNWorldSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNWorldSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNWorldSerializeHandler)));
}

static void HSPSubclass_LNWorldSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNWorldSerializeHandler_DelegateLabelCaller(LNHandle worldserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, worldserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldSerializeHandler*>(LNWorldSerializeHandler_GetSubinstanceId(worldserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ComponentListSerializeHandler

struct HSPSubclass_LNComponentListSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNComponentListSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNComponentListSerializeHandler)));
}

static void HSPSubclass_LNComponentListSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNComponentListSerializeHandler_DelegateLabelCaller(LNHandle componentlistserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, componentlistserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNComponentListSerializeHandler*>(LNComponentListSerializeHandler_GetSubinstanceId(componentlistserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// WorldObjectSerializeHandler

struct HSPSubclass_LNWorldObjectSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNWorldObjectSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNWorldObjectSerializeHandler)));
}

static void HSPSubclass_LNWorldObjectSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNWorldObjectSerializeHandler_DelegateLabelCaller(LNHandle worldobjectserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, worldobjectserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldObjectSerializeHandler*>(LNWorldObjectSerializeHandler_GetSubinstanceId(worldobjectserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// WorldObjectUpdateHandler

struct HSPSubclass_LNWorldObjectUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNWorldObjectUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNWorldObjectUpdateHandler)));
}

static void HSPSubclass_LNWorldObjectUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNWorldObjectUpdateHandler_DelegateLabelCaller(LNHandle worldobjectupdatehandler, LNHandle self, float elapsedSeconds)
{
    setCallbackArg(0, worldobjectupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldObjectUpdateHandler*>(LNWorldObjectUpdateHandler_GetSubinstanceId(worldobjectupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// VisualObjectSerializeHandler

struct HSPSubclass_LNVisualObjectSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNVisualObjectSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNVisualObjectSerializeHandler)));
}

static void HSPSubclass_LNVisualObjectSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNVisualObjectSerializeHandler_DelegateLabelCaller(LNHandle visualobjectserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, visualobjectserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualObjectSerializeHandler*>(LNVisualObjectSerializeHandler_GetSubinstanceId(visualobjectserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// VisualObjectUpdateHandler

struct HSPSubclass_LNVisualObjectUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNVisualObjectUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNVisualObjectUpdateHandler)));
}

static void HSPSubclass_LNVisualObjectUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNVisualObjectUpdateHandler_DelegateLabelCaller(LNHandle visualobjectupdatehandler, LNHandle self, float elapsedSeconds)
{
    setCallbackArg(0, visualobjectupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualObjectUpdateHandler*>(LNVisualObjectUpdateHandler_GetSubinstanceId(visualobjectupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// CameraSerializeHandler

struct HSPSubclass_LNCameraSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNCameraSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNCameraSerializeHandler)));
}

static void HSPSubclass_LNCameraSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNCameraSerializeHandler_DelegateLabelCaller(LNHandle cameraserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, cameraserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraSerializeHandler*>(LNCameraSerializeHandler_GetSubinstanceId(cameraserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// CameraUpdateHandler

struct HSPSubclass_LNCameraUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNCameraUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNCameraUpdateHandler)));
}

static void HSPSubclass_LNCameraUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNCameraUpdateHandler_DelegateLabelCaller(LNHandle cameraupdatehandler, LNHandle self, float elapsedSeconds)
{
    setCallbackArg(0, cameraupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraUpdateHandler*>(LNCameraUpdateHandler_GetSubinstanceId(cameraupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// EnvironmentLightSerializeHandler

struct HSPSubclass_LNEnvironmentLightSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNEnvironmentLightSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNEnvironmentLightSerializeHandler)));
}

static void HSPSubclass_LNEnvironmentLightSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNEnvironmentLightSerializeHandler_DelegateLabelCaller(LNHandle environmentlightserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, environmentlightserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNEnvironmentLightSerializeHandler*>(LNEnvironmentLightSerializeHandler_GetSubinstanceId(environmentlightserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// EnvironmentLightUpdateHandler

struct HSPSubclass_LNEnvironmentLightUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNEnvironmentLightUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNEnvironmentLightUpdateHandler)));
}

static void HSPSubclass_LNEnvironmentLightUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNEnvironmentLightUpdateHandler_DelegateLabelCaller(LNHandle environmentlightupdatehandler, LNHandle self, float elapsedSeconds)
{
    setCallbackArg(0, environmentlightupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNEnvironmentLightUpdateHandler*>(LNEnvironmentLightUpdateHandler_GetSubinstanceId(environmentlightupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// DirectionalLightSerializeHandler

struct HSPSubclass_LNDirectionalLightSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNDirectionalLightSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNDirectionalLightSerializeHandler)));
}

static void HSPSubclass_LNDirectionalLightSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNDirectionalLightSerializeHandler_DelegateLabelCaller(LNHandle directionallightserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, directionallightserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNDirectionalLightSerializeHandler*>(LNDirectionalLightSerializeHandler_GetSubinstanceId(directionallightserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// DirectionalLightUpdateHandler

struct HSPSubclass_LNDirectionalLightUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNDirectionalLightUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNDirectionalLightUpdateHandler)));
}

static void HSPSubclass_LNDirectionalLightUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNDirectionalLightUpdateHandler_DelegateLabelCaller(LNHandle directionallightupdatehandler, LNHandle self, float elapsedSeconds)
{
    setCallbackArg(0, directionallightupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNDirectionalLightUpdateHandler*>(LNDirectionalLightUpdateHandler_GetSubinstanceId(directionallightupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// PointLightSerializeHandler

struct HSPSubclass_LNPointLightSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNPointLightSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNPointLightSerializeHandler)));
}

static void HSPSubclass_LNPointLightSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNPointLightSerializeHandler_DelegateLabelCaller(LNHandle pointlightserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, pointlightserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNPointLightSerializeHandler*>(LNPointLightSerializeHandler_GetSubinstanceId(pointlightserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// PointLightUpdateHandler

struct HSPSubclass_LNPointLightUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNPointLightUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNPointLightUpdateHandler)));
}

static void HSPSubclass_LNPointLightUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNPointLightUpdateHandler_DelegateLabelCaller(LNHandle pointlightupdatehandler, LNHandle self, float elapsedSeconds)
{
    setCallbackArg(0, pointlightupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNPointLightUpdateHandler*>(LNPointLightUpdateHandler_GetSubinstanceId(pointlightupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// SpotLightSerializeHandler

struct HSPSubclass_LNSpotLightSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNSpotLightSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSpotLightSerializeHandler)));
}

static void HSPSubclass_LNSpotLightSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNSpotLightSerializeHandler_DelegateLabelCaller(LNHandle spotlightserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, spotlightserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNSpotLightSerializeHandler*>(LNSpotLightSerializeHandler_GetSubinstanceId(spotlightserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// SpotLightUpdateHandler

struct HSPSubclass_LNSpotLightUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNSpotLightUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSpotLightUpdateHandler)));
}

static void HSPSubclass_LNSpotLightUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNSpotLightUpdateHandler_DelegateLabelCaller(LNHandle spotlightupdatehandler, LNHandle self, float elapsedSeconds)
{
    setCallbackArg(0, spotlightupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNSpotLightUpdateHandler*>(LNSpotLightUpdateHandler_GetSubinstanceId(spotlightupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// SpriteSerializeHandler

struct HSPSubclass_LNSpriteSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNSpriteSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSpriteSerializeHandler)));
}

static void HSPSubclass_LNSpriteSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNSpriteSerializeHandler_DelegateLabelCaller(LNHandle spriteserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, spriteserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNSpriteSerializeHandler*>(LNSpriteSerializeHandler_GetSubinstanceId(spriteserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// SpriteUpdateHandler

struct HSPSubclass_LNSpriteUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNSpriteUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSpriteUpdateHandler)));
}

static void HSPSubclass_LNSpriteUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNSpriteUpdateHandler_DelegateLabelCaller(LNHandle spriteupdatehandler, LNHandle self, float elapsedSeconds)
{
    setCallbackArg(0, spriteupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNSpriteUpdateHandler*>(LNSpriteUpdateHandler_GetSubinstanceId(spriteupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// CameraOrbitControlComponentSerializeHandler

struct HSPSubclass_LNCameraOrbitControlComponentSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNCameraOrbitControlComponentSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNCameraOrbitControlComponentSerializeHandler)));
}

static void HSPSubclass_LNCameraOrbitControlComponentSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNCameraOrbitControlComponentSerializeHandler_DelegateLabelCaller(LNHandle cameraorbitcontrolcomponentserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, cameraorbitcontrolcomponentserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraOrbitControlComponentSerializeHandler*>(LNCameraOrbitControlComponentSerializeHandler_GetSubinstanceId(cameraorbitcontrolcomponentserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// RaycasterSerializeHandler

struct HSPSubclass_LNRaycasterSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNRaycasterSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNRaycasterSerializeHandler)));
}

static void HSPSubclass_LNRaycasterSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNRaycasterSerializeHandler_DelegateLabelCaller(LNHandle raycasterserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, raycasterserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNRaycasterSerializeHandler*>(LNRaycasterSerializeHandler_GetSubinstanceId(raycasterserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// RaycastResultSerializeHandler

struct HSPSubclass_LNRaycastResultSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNRaycastResultSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNRaycastResultSerializeHandler)));
}

static void HSPSubclass_LNRaycastResultSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNRaycastResultSerializeHandler_DelegateLabelCaller(LNHandle raycastresultserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, raycastresultserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNRaycastResultSerializeHandler*>(LNRaycastResultSerializeHandler_GetSubinstanceId(raycastresultserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// WorldRenderViewSerializeHandler

struct HSPSubclass_LNWorldRenderViewSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNWorldRenderViewSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNWorldRenderViewSerializeHandler)));
}

static void HSPSubclass_LNWorldRenderViewSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNWorldRenderViewSerializeHandler_DelegateLabelCaller(LNHandle worldrenderviewserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, worldrenderviewserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldRenderViewSerializeHandler*>(LNWorldRenderViewSerializeHandler_GetSubinstanceId(worldrenderviewserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// BoxMeshSerializeHandler

struct HSPSubclass_LNBoxMeshSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNBoxMeshSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNBoxMeshSerializeHandler)));
}

static void HSPSubclass_LNBoxMeshSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNBoxMeshSerializeHandler_DelegateLabelCaller(LNHandle boxmeshserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, boxmeshserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxMeshSerializeHandler*>(LNBoxMeshSerializeHandler_GetSubinstanceId(boxmeshserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// BoxMeshUpdateHandler

struct HSPSubclass_LNBoxMeshUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNBoxMeshUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNBoxMeshUpdateHandler)));
}

static void HSPSubclass_LNBoxMeshUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNBoxMeshUpdateHandler_DelegateLabelCaller(LNHandle boxmeshupdatehandler, LNHandle self, float elapsedSeconds)
{
    setCallbackArg(0, boxmeshupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxMeshUpdateHandler*>(LNBoxMeshUpdateHandler_GetSubinstanceId(boxmeshupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// PlaneMeshSerializeHandler

struct HSPSubclass_LNPlaneMeshSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNPlaneMeshSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNPlaneMeshSerializeHandler)));
}

static void HSPSubclass_LNPlaneMeshSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNPlaneMeshSerializeHandler_DelegateLabelCaller(LNHandle planemeshserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, planemeshserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNPlaneMeshSerializeHandler*>(LNPlaneMeshSerializeHandler_GetSubinstanceId(planemeshserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// PlaneMeshUpdateHandler

struct HSPSubclass_LNPlaneMeshUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNPlaneMeshUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNPlaneMeshUpdateHandler)));
}

static void HSPSubclass_LNPlaneMeshUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNPlaneMeshUpdateHandler_DelegateLabelCaller(LNHandle planemeshupdatehandler, LNHandle self, float elapsedSeconds)
{
    setCallbackArg(0, planemeshupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNPlaneMeshUpdateHandler*>(LNPlaneMeshUpdateHandler_GetSubinstanceId(planemeshupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// StaticMeshSerializeHandler

struct HSPSubclass_LNStaticMeshSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNStaticMeshSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNStaticMeshSerializeHandler)));
}

static void HSPSubclass_LNStaticMeshSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNStaticMeshSerializeHandler_DelegateLabelCaller(LNHandle staticmeshserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, staticmeshserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNStaticMeshSerializeHandler*>(LNStaticMeshSerializeHandler_GetSubinstanceId(staticmeshserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// StaticMeshUpdateHandler

struct HSPSubclass_LNStaticMeshUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNStaticMeshUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNStaticMeshUpdateHandler)));
}

static void HSPSubclass_LNStaticMeshUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNStaticMeshUpdateHandler_DelegateLabelCaller(LNHandle staticmeshupdatehandler, LNHandle self, float elapsedSeconds)
{
    setCallbackArg(0, staticmeshupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNStaticMeshUpdateHandler*>(LNStaticMeshUpdateHandler_GetSubinstanceId(staticmeshupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// SkinnedMeshComponentSerializeHandler

struct HSPSubclass_LNSkinnedMeshComponentSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNSkinnedMeshComponentSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSkinnedMeshComponentSerializeHandler)));
}

static void HSPSubclass_LNSkinnedMeshComponentSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNSkinnedMeshComponentSerializeHandler_DelegateLabelCaller(LNHandle skinnedmeshcomponentserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, skinnedmeshcomponentserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNSkinnedMeshComponentSerializeHandler*>(LNSkinnedMeshComponentSerializeHandler_GetSubinstanceId(skinnedmeshcomponentserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// CollisionSerializeHandler

struct HSPSubclass_LNCollisionSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNCollisionSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNCollisionSerializeHandler)));
}

static void HSPSubclass_LNCollisionSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNCollisionSerializeHandler_DelegateLabelCaller(LNHandle collisionserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, collisionserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionSerializeHandler*>(LNCollisionSerializeHandler_GetSubinstanceId(collisionserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// TriggerBodyComponentSerializeHandler

struct HSPSubclass_LNTriggerBodyComponentSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNTriggerBodyComponentSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNTriggerBodyComponentSerializeHandler)));
}

static void HSPSubclass_LNTriggerBodyComponentSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNTriggerBodyComponentSerializeHandler_DelegateLabelCaller(LNHandle triggerbodycomponentserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, triggerbodycomponentserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNTriggerBodyComponentSerializeHandler*>(LNTriggerBodyComponentSerializeHandler_GetSubinstanceId(triggerbodycomponentserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// LevelSerializeHandler

struct HSPSubclass_LNLevelSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNLevelSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNLevelSerializeHandler)));
}

static void HSPSubclass_LNLevelSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNLevelSerializeHandler_DelegateLabelCaller(LNHandle levelserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, levelserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelSerializeHandler*>(LNLevelSerializeHandler_GetSubinstanceId(levelserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// LevelStartHandler

struct HSPSubclass_LNLevelStartHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNLevelStartHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNLevelStartHandler)));
}

static void HSPSubclass_LNLevelStartHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNLevelStartHandler_DelegateLabelCaller(LNHandle levelstarthandler, LNHandle self)
{
    setCallbackArg(0, levelstarthandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelStartHandler*>(LNLevelStartHandler_GetSubinstanceId(levelstarthandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// LevelStopHandler

struct HSPSubclass_LNLevelStopHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNLevelStopHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNLevelStopHandler)));
}

static void HSPSubclass_LNLevelStopHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNLevelStopHandler_DelegateLabelCaller(LNHandle levelstophandler, LNHandle self)
{
    setCallbackArg(0, levelstophandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelStopHandler*>(LNLevelStopHandler_GetSubinstanceId(levelstophandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// LevelPauseHandler

struct HSPSubclass_LNLevelPauseHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNLevelPauseHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNLevelPauseHandler)));
}

static void HSPSubclass_LNLevelPauseHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNLevelPauseHandler_DelegateLabelCaller(LNHandle levelpausehandler, LNHandle self)
{
    setCallbackArg(0, levelpausehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelPauseHandler*>(LNLevelPauseHandler_GetSubinstanceId(levelpausehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// LevelResumeHandler

struct HSPSubclass_LNLevelResumeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNLevelResumeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNLevelResumeHandler)));
}

static void HSPSubclass_LNLevelResumeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNLevelResumeHandler_DelegateLabelCaller(LNHandle levelresumehandler, LNHandle self)
{
    setCallbackArg(0, levelresumehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelResumeHandler*>(LNLevelResumeHandler_GetSubinstanceId(levelresumehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// LevelUpdateHandler

struct HSPSubclass_LNLevelUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNLevelUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNLevelUpdateHandler)));
}

static void HSPSubclass_LNLevelUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNLevelUpdateHandler_DelegateLabelCaller(LNHandle levelupdatehandler, LNHandle self)
{
    setCallbackArg(0, levelupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelUpdateHandler*>(LNLevelUpdateHandler_GetSubinstanceId(levelupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UIEventArgsSerializeHandler

struct HSPSubclass_LNUIEventArgsSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIEventArgsSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIEventArgsSerializeHandler)));
}

static void HSPSubclass_LNUIEventArgsSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIEventArgsSerializeHandler_DelegateLabelCaller(LNHandle uieventargsserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uieventargsserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIEventArgsSerializeHandler*>(LNUIEventArgsSerializeHandler_GetSubinstanceId(uieventargsserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UILayoutElementSerializeHandler

struct HSPSubclass_LNUILayoutElementSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUILayoutElementSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUILayoutElementSerializeHandler)));
}

static void HSPSubclass_LNUILayoutElementSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUILayoutElementSerializeHandler_DelegateLabelCaller(LNHandle uilayoutelementserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uilayoutelementserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUILayoutElementSerializeHandler*>(LNUILayoutElementSerializeHandler_GetSubinstanceId(uilayoutelementserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UIElementSerializeHandler

struct HSPSubclass_LNUIElementSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIElementSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIElementSerializeHandler)));
}

static void HSPSubclass_LNUIElementSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIElementSerializeHandler_DelegateLabelCaller(LNHandle uielementserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uielementserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIElementSerializeHandler*>(LNUIElementSerializeHandler_GetSubinstanceId(uielementserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UITextBlockSerializeHandler

struct HSPSubclass_LNUITextBlockSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUITextBlockSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUITextBlockSerializeHandler)));
}

static void HSPSubclass_LNUITextBlockSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUITextBlockSerializeHandler_DelegateLabelCaller(LNHandle uitextblockserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uitextblockserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUITextBlockSerializeHandler*>(LNUITextBlockSerializeHandler_GetSubinstanceId(uitextblockserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UISpriteSerializeHandler

struct HSPSubclass_LNUISpriteSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUISpriteSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUISpriteSerializeHandler)));
}

static void HSPSubclass_LNUISpriteSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUISpriteSerializeHandler_DelegateLabelCaller(LNHandle uispriteserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uispriteserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUISpriteSerializeHandler*>(LNUISpriteSerializeHandler_GetSubinstanceId(uispriteserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UIIconSerializeHandler

struct HSPSubclass_LNUIIconSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIIconSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIIconSerializeHandler)));
}

static void HSPSubclass_LNUIIconSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIIconSerializeHandler_DelegateLabelCaller(LNHandle uiiconserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uiiconserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIIconSerializeHandler*>(LNUIIconSerializeHandler_GetSubinstanceId(uiiconserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UILayoutPanelSerializeHandler

struct HSPSubclass_LNUILayoutPanelSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUILayoutPanelSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUILayoutPanelSerializeHandler)));
}

static void HSPSubclass_LNUILayoutPanelSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUILayoutPanelSerializeHandler_DelegateLabelCaller(LNHandle uilayoutpanelserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uilayoutpanelserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUILayoutPanelSerializeHandler*>(LNUILayoutPanelSerializeHandler_GetSubinstanceId(uilayoutpanelserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UIBoxLayoutSerializeHandler

struct HSPSubclass_LNUIBoxLayoutSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIBoxLayoutSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIBoxLayoutSerializeHandler)));
}

static void HSPSubclass_LNUIBoxLayoutSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIBoxLayoutSerializeHandler_DelegateLabelCaller(LNHandle uiboxlayoutserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uiboxlayoutserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIBoxLayoutSerializeHandler*>(LNUIBoxLayoutSerializeHandler_GetSubinstanceId(uiboxlayoutserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UIGridLayoutSerializeHandler

struct HSPSubclass_LNUIGridLayoutSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIGridLayoutSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIGridLayoutSerializeHandler)));
}

static void HSPSubclass_LNUIGridLayoutSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIGridLayoutSerializeHandler_DelegateLabelCaller(LNHandle uigridlayoutserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uigridlayoutserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIGridLayoutSerializeHandler*>(LNUIGridLayoutSerializeHandler_GetSubinstanceId(uigridlayoutserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UIControlSerializeHandler

struct HSPSubclass_LNUIControlSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIControlSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIControlSerializeHandler)));
}

static void HSPSubclass_LNUIControlSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIControlSerializeHandler_DelegateLabelCaller(LNHandle uicontrolserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uicontrolserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIControlSerializeHandler*>(LNUIControlSerializeHandler_GetSubinstanceId(uicontrolserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UIButtonBaseSerializeHandler

struct HSPSubclass_LNUIButtonBaseSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIButtonBaseSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIButtonBaseSerializeHandler)));
}

static void HSPSubclass_LNUIButtonBaseSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIButtonBaseSerializeHandler_DelegateLabelCaller(LNHandle uibuttonbaseserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uibuttonbaseserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIButtonBaseSerializeHandler*>(LNUIButtonBaseSerializeHandler_GetSubinstanceId(uibuttonbaseserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UIButtonSerializeHandler

struct HSPSubclass_LNUIButtonSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIButtonSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIButtonSerializeHandler)));
}

static void HSPSubclass_LNUIButtonSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIButtonSerializeHandler_DelegateLabelCaller(LNHandle uibuttonserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uibuttonserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIButtonSerializeHandler*>(LNUIButtonSerializeHandler_GetSubinstanceId(uibuttonserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UIWindowSerializeHandler

struct HSPSubclass_LNUIWindowSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIWindowSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIWindowSerializeHandler)));
}

static void HSPSubclass_LNUIWindowSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIWindowSerializeHandler_DelegateLabelCaller(LNHandle uiwindowserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uiwindowserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIWindowSerializeHandler*>(LNUIWindowSerializeHandler_GetSubinstanceId(uiwindowserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UIListItemSerializeHandler

struct HSPSubclass_LNUIListItemSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIListItemSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIListItemSerializeHandler)));
}

static void HSPSubclass_LNUIListItemSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIListItemSerializeHandler_DelegateLabelCaller(LNHandle uilistitemserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uilistitemserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListItemSerializeHandler*>(LNUIListItemSerializeHandler_GetSubinstanceId(uilistitemserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UIListItemsControlSerializeHandler

struct HSPSubclass_LNUIListItemsControlSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIListItemsControlSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIListItemsControlSerializeHandler)));
}

static void HSPSubclass_LNUIListItemsControlSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIListItemsControlSerializeHandler_DelegateLabelCaller(LNHandle uilistitemscontrolserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uilistitemscontrolserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListItemsControlSerializeHandler*>(LNUIListItemsControlSerializeHandler_GetSubinstanceId(uilistitemscontrolserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UIListBoxItemSerializeHandler

struct HSPSubclass_LNUIListBoxItemSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIListBoxItemSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIListBoxItemSerializeHandler)));
}

static void HSPSubclass_LNUIListBoxItemSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIListBoxItemSerializeHandler_DelegateLabelCaller(LNHandle uilistboxitemserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uilistboxitemserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListBoxItemSerializeHandler*>(LNUIListBoxItemSerializeHandler_GetSubinstanceId(uilistboxitemserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// UIListBoxSerializeHandler

struct HSPSubclass_LNUIListBoxSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIListBoxSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIListBoxSerializeHandler)));
}

static void HSPSubclass_LNUIListBoxSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIListBoxSerializeHandler_DelegateLabelCaller(LNHandle uilistboxserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uilistboxserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListBoxSerializeHandler*>(LNUIListBoxSerializeHandler_GetSubinstanceId(uilistboxserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// InterpreterCommandSerializeHandler

struct HSPSubclass_LNInterpreterCommandSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNInterpreterCommandSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNInterpreterCommandSerializeHandler)));
}

static void HSPSubclass_LNInterpreterCommandSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNInterpreterCommandSerializeHandler_DelegateLabelCaller(LNHandle interpretercommandserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, interpretercommandserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterCommandSerializeHandler*>(LNInterpreterCommandSerializeHandler_GetSubinstanceId(interpretercommandserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// InterpreterCommandListSerializeHandler

struct HSPSubclass_LNInterpreterCommandListSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNInterpreterCommandListSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNInterpreterCommandListSerializeHandler)));
}

static void HSPSubclass_LNInterpreterCommandListSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNInterpreterCommandListSerializeHandler_DelegateLabelCaller(LNHandle interpretercommandlistserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, interpretercommandlistserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterCommandListSerializeHandler*>(LNInterpreterCommandListSerializeHandler_GetSubinstanceId(interpretercommandlistserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// InterpreterSerializeHandler

struct HSPSubclass_LNInterpreterSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNInterpreterSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNInterpreterSerializeHandler)));
}

static void HSPSubclass_LNInterpreterSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNInterpreterSerializeHandler_DelegateLabelCaller(LNHandle interpreterserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, interpreterserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterSerializeHandler*>(LNInterpreterSerializeHandler_GetSubinstanceId(interpreterserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// InterpreterUpdateWaitHandler

struct HSPSubclass_LNInterpreterUpdateWaitHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNInterpreterUpdateWaitHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNInterpreterUpdateWaitHandler)));
}

static void HSPSubclass_LNInterpreterUpdateWaitHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNInterpreterUpdateWaitHandler_DelegateLabelCaller(LNHandle interpreterupdatewaithandler, LNHandle self, LNBool* outReturn)
{
    setCallbackArg(0, interpreterupdatewaithandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterUpdateWaitHandler*>(LNInterpreterUpdateWaitHandler_GetSubinstanceId(interpreterupdatewaithandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    setCallbackOutput(2, outReturn);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ApplicationSerializeHandler

struct HSPSubclass_LNApplicationSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNApplicationSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNApplicationSerializeHandler)));
}

static void HSPSubclass_LNApplicationSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNApplicationSerializeHandler_DelegateLabelCaller(LNHandle applicationserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, applicationserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNApplicationSerializeHandler*>(LNApplicationSerializeHandler_GetSubinstanceId(applicationserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ApplicationInitHandler

struct HSPSubclass_LNApplicationInitHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNApplicationInitHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNApplicationInitHandler)));
}

static void HSPSubclass_LNApplicationInitHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNApplicationInitHandler_DelegateLabelCaller(LNHandle applicationinithandler, LNHandle self)
{
    setCallbackArg(0, applicationinithandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNApplicationInitHandler*>(LNApplicationInitHandler_GetSubinstanceId(applicationinithandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ApplicationUpdateHandler

struct HSPSubclass_LNApplicationUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNApplicationUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNApplicationUpdateHandler)));
}

static void HSPSubclass_LNApplicationUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNApplicationUpdateHandler_DelegateLabelCaller(LNHandle applicationupdatehandler, LNHandle self)
{
    setCallbackArg(0, applicationupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNApplicationUpdateHandler*>(LNApplicationUpdateHandler_GetSubinstanceId(applicationupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}
bool Structs_reffunc(int cmd, int* typeRes, void** retValPtr)
{
    g_leadSupport = false;
    switch (cmd) {
        // LNVector3
        case 0x2B : {
            hspLNVector3_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNVector4
        case 0x3D : {
            hspLNVector4_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNQuaternion
        case 0x48 : {
            hspLNQuaternion_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNMatrix
        case 0x56 : {
            hspLNMatrix_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNColor
        case 0xD1 : {
            hspLNColor_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNPoint
        case 0xDC : {
            hspLNPoint_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNSize
        case 0xE3 : {
            hspLNSize_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNRect
        case 0xEA : {
            hspLNRect_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNThickness
        case 0xF9 : {
            hspLNThickness_reffunc(typeRes, retValPtr);
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
        // LNObject_Release
        case 0x70 : {
            // Fetch object
            const auto local_object = fetchVAInt();

            stat = LNObject_Release(local_object);


            return true;
        }
        // LNObject_Retain
        case 0x71 : {
            // Fetch object
            const auto local_object = fetchVAInt();

            stat = LNObject_Retain(local_object);


            return true;
        }
        // LNObject_GetReferenceCount
        case 0x72 : {
            // Fetch object
            const auto local_object = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LNObject_GetReferenceCount(local_object, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNObject_SetPrototype_OnSerialize
        case 0x66D : {
            // Fetch object
            const auto local_object = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNObjectSerializeHandler_Create(HSPSubclass_LNObjectSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNObjectSerializeHandler*>(LNObjectSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNObject_SetPrototype_OnSerialize(local_object, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNEventConnection_SetPrototype_OnSerialize
        case 0x678 : {
            // Fetch eventconnection
            const auto local_eventconnection = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNEventConnectionSerializeHandler_Create(HSPSubclass_LNEventConnectionSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNEventConnectionSerializeHandler*>(LNEventConnectionSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNEventConnection_SetPrototype_OnSerialize(local_eventconnection, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNPromiseFailureDelegate_Create
        case 0x3E5 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outPromiseFailureDelegate
            PVal* pval_outPromiseFailureDelegate;
            const APTR aptr_outPromiseFailureDelegate = code_getva(&pval_outPromiseFailureDelegate);
            LNHandle local_outPromiseFailureDelegate;

            stat = LNPromiseFailureDelegate_Create(HSPSubclass_LNPromiseFailureDelegate_DelegateLabelCaller, &local_outPromiseFailureDelegate);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNPromiseFailureDelegate*>(LNPromiseFailureDelegate_GetSubinstanceId(local_outPromiseFailureDelegate));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outPromiseFailureDelegate, aptr_outPromiseFailureDelegate, local_outPromiseFailureDelegate);

            return true;
        }
        // LNZVTestDelegate1_Create
        case 0x3EA : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestDelegate1
            PVal* pval_outZVTestDelegate1;
            const APTR aptr_outZVTestDelegate1 = code_getva(&pval_outZVTestDelegate1);
            LNHandle local_outZVTestDelegate1;

            stat = LNZVTestDelegate1_Create(HSPSubclass_LNZVTestDelegate1_DelegateLabelCaller, &local_outZVTestDelegate1);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate1*>(LNZVTestDelegate1_GetSubinstanceId(local_outZVTestDelegate1));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestDelegate1, aptr_outZVTestDelegate1, local_outZVTestDelegate1);

            return true;
        }
        // LNZVTestDelegate2_Create
        case 0x3F0 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestDelegate2
            PVal* pval_outZVTestDelegate2;
            const APTR aptr_outZVTestDelegate2 = code_getva(&pval_outZVTestDelegate2);
            LNHandle local_outZVTestDelegate2;

            stat = LNZVTestDelegate2_Create(HSPSubclass_LNZVTestDelegate2_DelegateLabelCaller, &local_outZVTestDelegate2);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate2*>(LNZVTestDelegate2_GetSubinstanceId(local_outZVTestDelegate2));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestDelegate2, aptr_outZVTestDelegate2, local_outZVTestDelegate2);

            return true;
        }
        // LNZVTestDelegate3_Create
        case 0x3F5 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestDelegate3
            PVal* pval_outZVTestDelegate3;
            const APTR aptr_outZVTestDelegate3 = code_getva(&pval_outZVTestDelegate3);
            LNHandle local_outZVTestDelegate3;

            stat = LNZVTestDelegate3_Create(HSPSubclass_LNZVTestDelegate3_DelegateLabelCaller, &local_outZVTestDelegate3);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate3*>(LNZVTestDelegate3_GetSubinstanceId(local_outZVTestDelegate3));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestDelegate3, aptr_outZVTestDelegate3, local_outZVTestDelegate3);

            return true;
        }
        // LNZVTestEventHandler1_Create
        case 0x3FA : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestEventHandler1
            PVal* pval_outZVTestEventHandler1;
            const APTR aptr_outZVTestEventHandler1 = code_getva(&pval_outZVTestEventHandler1);
            LNHandle local_outZVTestEventHandler1;

            stat = LNZVTestEventHandler1_Create(HSPSubclass_LNZVTestEventHandler1_DelegateLabelCaller, &local_outZVTestEventHandler1);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestEventHandler1*>(LNZVTestEventHandler1_GetSubinstanceId(local_outZVTestEventHandler1));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestEventHandler1, aptr_outZVTestEventHandler1, local_outZVTestEventHandler1);

            return true;
        }
        // LNZVTestEventHandler2_Create
        case 0x3FF : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestEventHandler2
            PVal* pval_outZVTestEventHandler2;
            const APTR aptr_outZVTestEventHandler2 = code_getva(&pval_outZVTestEventHandler2);
            LNHandle local_outZVTestEventHandler2;

            stat = LNZVTestEventHandler2_Create(HSPSubclass_LNZVTestEventHandler2_DelegateLabelCaller, &local_outZVTestEventHandler2);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestEventHandler2*>(LNZVTestEventHandler2_GetSubinstanceId(local_outZVTestEventHandler2));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestEventHandler2, aptr_outZVTestEventHandler2, local_outZVTestEventHandler2);

            return true;
        }
        // LNZVTestPromise1_ThenWith
        case 0x402 : {
            // Fetch zvtestpromise1
            const auto local_zvtestpromise1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNZVTestDelegate3_Create(HSPSubclass_LNZVTestDelegate3_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate3*>(LNZVTestDelegate3_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNZVTestPromise1_ThenWith(local_zvtestpromise1, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNZVTestPromise1_CatchWith
        case 0x404 : {
            // Fetch zvtestpromise1
            const auto local_zvtestpromise1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPromiseFailureDelegate_Create(HSPSubclass_LNPromiseFailureDelegate_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPromiseFailureDelegate*>(LNPromiseFailureDelegate_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNZVTestPromise1_CatchWith(local_zvtestpromise1, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNZVTestPromise2_ThenWith
        case 0x408 : {
            // Fetch zvtestpromise2
            const auto local_zvtestpromise2 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNZVTestDelegate1_Create(HSPSubclass_LNZVTestDelegate1_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate1*>(LNZVTestDelegate1_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNZVTestPromise2_ThenWith(local_zvtestpromise2, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNZVTestPromise2_CatchWith
        case 0x40A : {
            // Fetch zvtestpromise2
            const auto local_zvtestpromise2 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPromiseFailureDelegate_Create(HSPSubclass_LNPromiseFailureDelegate_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPromiseFailureDelegate*>(LNPromiseFailureDelegate_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNZVTestPromise2_CatchWith(local_zvtestpromise2, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNZVTestClass1_Create
        case 0x9E : {
            // Fetch outZVTestClass1
            PVal* pval_outZVTestClass1;
            const APTR aptr_outZVTestClass1 = code_getva(&pval_outZVTestClass1);
            LNHandle local_outZVTestClass1;

            stat = LNZVTestClass1_Create(&local_outZVTestClass1);
            setVAInt(pval_outZVTestClass1, aptr_outZVTestClass1, local_outZVTestClass1);

            return true;
        }
        // LNZVTestClass1_SetTestDelegate1
        case 0x88 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVALabelPointer();

            LNHandle localDelegate_value;
            {
                stat = LNZVTestDelegate1_Create(HSPSubclass_LNZVTestDelegate1_DelegateLabelCaller, &localDelegate_value);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate1*>(LNZVTestDelegate1_GetSubinstanceId(localDelegate_value));
                localSelf->labelPointer = local_value;
            }

            stat = LNZVTestClass1_SetTestDelegate1(local_zvtestclass1, localDelegate_value);
            LNObject_Release(localDelegate_value);

            return true;
        }
        // LNZVTestClass1_SetTestDelegate2
        case 0x8A : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVALabelPointer();

            LNHandle localDelegate_value;
            {
                stat = LNZVTestDelegate2_Create(HSPSubclass_LNZVTestDelegate2_DelegateLabelCaller, &localDelegate_value);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate2*>(LNZVTestDelegate2_GetSubinstanceId(localDelegate_value));
                localSelf->labelPointer = local_value;
            }

            stat = LNZVTestClass1_SetTestDelegate2(local_zvtestclass1, localDelegate_value);
            LNObject_Release(localDelegate_value);

            return true;
        }
        // LNZVTestClass1_SetTestDelegate3
        case 0x8C : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVALabelPointer();

            LNHandle localDelegate_value;
            {
                stat = LNZVTestDelegate3_Create(HSPSubclass_LNZVTestDelegate3_DelegateLabelCaller, &localDelegate_value);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate3*>(LNZVTestDelegate3_GetSubinstanceId(localDelegate_value));
                localSelf->labelPointer = local_value;
            }

            stat = LNZVTestClass1_SetTestDelegate3(local_zvtestclass1, localDelegate_value);
            LNObject_Release(localDelegate_value);

            return true;
        }
        // LNZVTestClass1_CallTestDelegate1
        case 0x8E : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch a
            const auto local_a = fetchVAInt();

            stat = LNZVTestClass1_CallTestDelegate1(local_zvtestclass1, local_a);


            return true;
        }
        // LNZVTestClass1_CallTestDelegate2
        case 0x90 : {
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

            stat = LNZVTestClass1_CallTestDelegate2(local_zvtestclass1, local_a, local_b, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestClass1_CallTestDelegate3
        case 0x93 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();

            stat = LNZVTestClass1_CallTestDelegate3(local_zvtestclass1);


            return true;
        }
        // LNZVTestClass1_LoadAsyncA
        case 0x94 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNZVTestClass1_LoadAsyncA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestClass1_ExecuteAsync
        case 0x96 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNZVTestClass1_ExecuteAsync(local_zvtestclass1, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestClass1_GetFilePathA
        case 0x97 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LNZVTestClass1_GetFilePathA(local_zvtestclass1, &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestClass1_ConnectOnEvent1
        case 0x98 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNZVTestEventHandler1_Create(HSPSubclass_LNZVTestEventHandler1_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestEventHandler1*>(LNZVTestEventHandler1_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNZVTestClass1_ConnectOnEvent1(local_zvtestclass1, localDelegate_handler, &local_outReturn);
            LNObject_Release(localDelegate_handler);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestClass1_RaiseEvent1
        case 0x9A : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();

            stat = LNZVTestClass1_RaiseEvent1(local_zvtestclass1);


            return true;
        }
        // LNZVTestClass1_ConnectOnEvent2
        case 0x9B : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNZVTestEventHandler2_Create(HSPSubclass_LNZVTestEventHandler2_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestEventHandler2*>(LNZVTestEventHandler2_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNZVTestClass1_ConnectOnEvent2(local_zvtestclass1, localDelegate_handler, &local_outReturn);
            LNObject_Release(localDelegate_handler);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestClass1_RaiseEvent2
        case 0x9D : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();

            stat = LNZVTestClass1_RaiseEvent2(local_zvtestclass1);


            return true;
        }
        // LNZVTestClass1_SetPrototype_OnSerialize
        case 0x683 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNZVTestClass1SerializeHandler_Create(HSPSubclass_LNZVTestClass1SerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestClass1SerializeHandler*>(LNZVTestClass1SerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNZVTestClass1_SetPrototype_OnSerialize(local_zvtestclass1, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNZVTestEventArgs1_Create
        case 0xA1 : {
            // Fetch outZVTestEventArgs1
            PVal* pval_outZVTestEventArgs1;
            const APTR aptr_outZVTestEventArgs1 = code_getva(&pval_outZVTestEventArgs1);
            LNHandle local_outZVTestEventArgs1;

            stat = LNZVTestEventArgs1_Create(&local_outZVTestEventArgs1);
            setVAInt(pval_outZVTestEventArgs1, aptr_outZVTestEventArgs1, local_outZVTestEventArgs1);

            return true;
        }
        // LNZVTestEventArgs1_CreateWithValue
        case 0xA2 : {
            // Fetch v
            const auto local_v = fetchVAInt();
            // Fetch outZVTestEventArgs1
            PVal* pval_outZVTestEventArgs1;
            const APTR aptr_outZVTestEventArgs1 = code_getva(&pval_outZVTestEventArgs1);
            LNHandle local_outZVTestEventArgs1;

            stat = LNZVTestEventArgs1_CreateWithValue(local_v, &local_outZVTestEventArgs1);
            setVAInt(pval_outZVTestEventArgs1, aptr_outZVTestEventArgs1, local_outZVTestEventArgs1);

            return true;
        }
        // LNZVTestEventArgs1_GetValue
        case 0xA0 : {
            // Fetch zvtesteventargs1
            const auto local_zvtesteventargs1 = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LNZVTestEventArgs1_GetValue(local_zvtesteventargs1, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestEventArgs1_SetPrototype_OnSerialize
        case 0x68E : {
            // Fetch zvtesteventargs1
            const auto local_zvtesteventargs1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNZVTestEventArgs1SerializeHandler_Create(HSPSubclass_LNZVTestEventArgs1SerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestEventArgs1SerializeHandler*>(LNZVTestEventArgs1SerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNZVTestEventArgs1_SetPrototype_OnSerialize(local_zvtesteventargs1, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSerializer2_SetPrototype_OnSerialize
        case 0x699 : {
            // Fetch serializer2
            const auto local_serializer2 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSerializer2SerializeHandler_Create(HSPSubclass_LNSerializer2SerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSerializer2SerializeHandler*>(LNSerializer2SerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSerializer2_SetPrototype_OnSerialize(local_serializer2, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNAssetModel_Create
        case 0xAE : {
            // Fetch target
            const auto local_target = fetchVAInt();
            // Fetch outAssetModel
            PVal* pval_outAssetModel;
            const APTR aptr_outAssetModel = code_getva(&pval_outAssetModel);
            LNHandle local_outAssetModel;

            stat = LNAssetModel_Create(local_target, &local_outAssetModel);
            setVAInt(pval_outAssetModel, aptr_outAssetModel, local_outAssetModel);

            return true;
        }
        // LNAssetModel_Target
        case 0xAD : {
            // Fetch assetmodel
            const auto local_assetmodel = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNAssetModel_Target(local_assetmodel, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNAssetModel_SetPrototype_OnSerialize
        case 0x6A4 : {
            // Fetch assetmodel
            const auto local_assetmodel = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNAssetModelSerializeHandler_Create(HSPSubclass_LNAssetModelSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNAssetModelSerializeHandler*>(LNAssetModelSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNAssetModel_SetPrototype_OnSerialize(local_assetmodel, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNAssets_SaveAssetToLocalFileA
        case 0xB1 : {
            // Fetch asset
            const auto local_asset = fetchVAInt();
            // Fetch filePath
            const auto local_filePath = fetchVAString();

            stat = LNAssets_SaveAssetToLocalFileA(local_asset, local_filePath);


            return true;
        }
        // LNAssets_LoadAssetFromLocalFileA
        case 0xB4 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNAssets_LoadAssetFromLocalFileA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNAssets_LoadAssetA
        case 0xB6 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNAssets_LoadAssetA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNAssets_ReloadAssetA
        case 0xB8 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch obj
            const auto local_obj = fetchVAInt();

            stat = LNAssets_ReloadAssetA(local_filePath, local_obj);


            return true;
        }
        // LNTexture2DDelegate_Create
        case 0x42E : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outTexture2DDelegate
            PVal* pval_outTexture2DDelegate;
            const APTR aptr_outTexture2DDelegate = code_getva(&pval_outTexture2DDelegate);
            LNHandle local_outTexture2DDelegate;

            stat = LNTexture2DDelegate_Create(HSPSubclass_LNTexture2DDelegate_DelegateLabelCaller, &local_outTexture2DDelegate);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNTexture2DDelegate*>(LNTexture2DDelegate_GetSubinstanceId(local_outTexture2DDelegate));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outTexture2DDelegate, aptr_outTexture2DDelegate, local_outTexture2DDelegate);

            return true;
        }
        // LNTexture2DPromise_ThenWith
        case 0x431 : {
            // Fetch texture2dpromise
            const auto local_texture2dpromise = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNTexture2DDelegate_Create(HSPSubclass_LNTexture2DDelegate_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNTexture2DDelegate*>(LNTexture2DDelegate_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNTexture2DPromise_ThenWith(local_texture2dpromise, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNTexture2DPromise_CatchWith
        case 0x433 : {
            // Fetch texture2dpromise
            const auto local_texture2dpromise = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPromiseFailureDelegate_Create(HSPSubclass_LNPromiseFailureDelegate_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPromiseFailureDelegate*>(LNPromiseFailureDelegate_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNTexture2DPromise_CatchWith(local_texture2dpromise, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNTexture_SetPrototype_OnSerialize
        case 0x6AF : {
            // Fetch texture
            const auto local_texture = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNTextureSerializeHandler_Create(HSPSubclass_LNTextureSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNTextureSerializeHandler*>(LNTextureSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNTexture_SetPrototype_OnSerialize(local_texture, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNTexture2D_Create
        case 0x10A : {
            // Fetch width
            const auto local_width = fetchVAInt();
            // Fetch height
            const auto local_height = fetchVAInt();
            // Fetch outTexture2D
            PVal* pval_outTexture2D;
            const APTR aptr_outTexture2D = code_getva(&pval_outTexture2D);
            LNHandle local_outTexture2D;

            stat = LNTexture2D_Create(local_width, local_height, &local_outTexture2D);
            setVAInt(pval_outTexture2D, aptr_outTexture2D, local_outTexture2D);

            return true;
        }
        // LNTexture2D_CreateWithFormat
        case 0x10D : {
            // Fetch width
            const auto local_width = fetchVAInt();
            // Fetch height
            const auto local_height = fetchVAInt();
            // Fetch format
            const auto local_format = static_cast<LNTextureFormat>(fetchVAInt());
            // Fetch outTexture2D
            PVal* pval_outTexture2D;
            const APTR aptr_outTexture2D = code_getva(&pval_outTexture2D);
            LNHandle local_outTexture2D;

            stat = LNTexture2D_CreateWithFormat(local_width, local_height, local_format, &local_outTexture2D);
            setVAInt(pval_outTexture2D, aptr_outTexture2D, local_outTexture2D);

            return true;
        }
        // LNTexture2D_LoadA
        case 0x106 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNTexture2D_LoadA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNTexture2D_LoadEmojiA
        case 0x108 : {
            // Fetch code
            const auto local_code = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNTexture2D_LoadEmojiA(local_code, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNTexture2D_SetPrototype_OnSerialize
        case 0x6BA : {
            // Fetch texture2d
            const auto local_texture2d = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNTexture2DSerializeHandler_Create(HSPSubclass_LNTexture2DSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNTexture2DSerializeHandler*>(LNTexture2DSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNTexture2D_SetPrototype_OnSerialize(local_texture2d, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNRenderView_SetPrototype_OnSerialize
        case 0x6C5 : {
            // Fetch renderview
            const auto local_renderview = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNRenderViewSerializeHandler_Create(HSPSubclass_LNRenderViewSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNRenderViewSerializeHandler*>(LNRenderViewSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNRenderView_SetPrototype_OnSerialize(local_renderview, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSkinnedMeshModel_LoadA
        case 0x119 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNSkinnedMeshModel_LoadA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSkinnedMeshModel_SetPrototype_OnSerialize
        case 0x6D0 : {
            // Fetch skinnedmeshmodel
            const auto local_skinnedmeshmodel = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSkinnedMeshModelSerializeHandler_Create(HSPSubclass_LNSkinnedMeshModelSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSkinnedMeshModelSerializeHandler*>(LNSkinnedMeshModelSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSkinnedMeshModel_SetPrototype_OnSerialize(local_skinnedmeshmodel, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNCollisionShape_SetPrototype_OnSerialize
        case 0x6DB : {
            // Fetch collisionshape
            const auto local_collisionshape = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNCollisionShapeSerializeHandler_Create(HSPSubclass_LNCollisionShapeSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionShapeSerializeHandler*>(LNCollisionShapeSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNCollisionShape_SetPrototype_OnSerialize(local_collisionshape, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNBoxCollisionShape_Create
        case 0x11D : {
            // Fetch size
            PVal* pval_size;
            CodeGetVA_TypeChecked(&pval_size, LNVector3);
            // Fetch outBoxCollisionShape
            PVal* pval_outBoxCollisionShape;
            const APTR aptr_outBoxCollisionShape = code_getva(&pval_outBoxCollisionShape);
            LNHandle local_outBoxCollisionShape;

            stat = LNBoxCollisionShape_Create(reinterpret_cast<const LNVector3*>(pval_size->pt), &local_outBoxCollisionShape);
            setVAInt(pval_outBoxCollisionShape, aptr_outBoxCollisionShape, local_outBoxCollisionShape);

            return true;
        }
        // LNBoxCollisionShape_CreateWHD
        case 0x11F : {
            // Fetch width
            const auto local_width = fetchVADouble();
            // Fetch height
            const auto local_height = fetchVADouble();
            // Fetch depth
            const auto local_depth = fetchVADouble();
            // Fetch outBoxCollisionShape
            PVal* pval_outBoxCollisionShape;
            const APTR aptr_outBoxCollisionShape = code_getva(&pval_outBoxCollisionShape);
            LNHandle local_outBoxCollisionShape;

            stat = LNBoxCollisionShape_CreateWHD(local_width, local_height, local_depth, &local_outBoxCollisionShape);
            setVAInt(pval_outBoxCollisionShape, aptr_outBoxCollisionShape, local_outBoxCollisionShape);

            return true;
        }
        // LNBoxCollisionShape_SetPrototype_OnSerialize
        case 0x6E6 : {
            // Fetch boxcollisionshape
            const auto local_boxcollisionshape = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNBoxCollisionShapeSerializeHandler_Create(HSPSubclass_LNBoxCollisionShapeSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxCollisionShapeSerializeHandler*>(LNBoxCollisionShapeSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNBoxCollisionShape_SetPrototype_OnSerialize(local_boxcollisionshape, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNComponent_SetPrototype_OnSerialize
        case 0x6F1 : {
            // Fetch component
            const auto local_component = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNComponentSerializeHandler_Create(HSPSubclass_LNComponentSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNComponentSerializeHandler*>(LNComponentSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNComponent_SetPrototype_OnSerialize(local_component, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNVisualComponent_SetVisible
        case 0x125 : {
            // Fetch visualcomponent
            const auto local_visualcomponent = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNVisualComponent_SetVisible(local_visualcomponent, local_value);


            return true;
        }
        // LNVisualComponent_IsVisible
        case 0x127 : {
            // Fetch visualcomponent
            const auto local_visualcomponent = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNVisualComponent_IsVisible(local_visualcomponent, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNVisualComponent_SetPrototype_OnSerialize
        case 0x6FC : {
            // Fetch visualcomponent
            const auto local_visualcomponent = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNVisualComponentSerializeHandler_Create(HSPSubclass_LNVisualComponentSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualComponentSerializeHandler*>(LNVisualComponentSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNVisualComponent_SetPrototype_OnSerialize(local_visualcomponent, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSpriteComponent_SetTexture
        case 0x129 : {
            // Fetch spritecomponent
            const auto local_spritecomponent = fetchVAInt();
            // Fetch texture
            const auto local_texture = fetchVAInt();

            stat = LNSpriteComponent_SetTexture(local_spritecomponent, local_texture);


            return true;
        }
        // LNSpriteComponent_SetPrototype_OnSerialize
        case 0x707 : {
            // Fetch spritecomponent
            const auto local_spritecomponent = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSpriteComponentSerializeHandler_Create(HSPSubclass_LNSpriteComponentSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSpriteComponentSerializeHandler*>(LNSpriteComponentSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSpriteComponent_SetPrototype_OnSerialize(local_spritecomponent, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNCollisionEventHandler_Create
        case 0x4C7 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outCollisionEventHandler
            PVal* pval_outCollisionEventHandler;
            const APTR aptr_outCollisionEventHandler = code_getva(&pval_outCollisionEventHandler);
            LNHandle local_outCollisionEventHandler;

            stat = LNCollisionEventHandler_Create(HSPSubclass_LNCollisionEventHandler_DelegateLabelCaller, &local_outCollisionEventHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionEventHandler*>(LNCollisionEventHandler_GetSubinstanceId(local_outCollisionEventHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outCollisionEventHandler, aptr_outCollisionEventHandler, local_outCollisionEventHandler);

            return true;
        }
        // LNCharacterController_Create
        case 0x13F : {
            // Fetch outCharacterController
            PVal* pval_outCharacterController;
            const APTR aptr_outCharacterController = code_getva(&pval_outCharacterController);
            LNHandle local_outCharacterController;

            stat = LNCharacterController_Create(&local_outCharacterController);
            setVAInt(pval_outCharacterController, aptr_outCharacterController, local_outCharacterController);

            return true;
        }
        // LNCharacterController_SetHeight
        case 0x133 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNCharacterController_SetHeight(local_charactercontroller, local_value);


            return true;
        }
        // LNCharacterController_GetHeight
        case 0x135 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNCharacterController_GetHeight(local_charactercontroller, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNCharacterController_SetCameraRadius
        case 0x136 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNCharacterController_SetCameraRadius(local_charactercontroller, local_value);


            return true;
        }
        // LNCharacterController_GetCameraRadius
        case 0x138 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNCharacterController_GetCameraRadius(local_charactercontroller, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNCharacterController_SetCollisionEnter
        case 0x139 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNCollisionEventHandler_Create(HSPSubclass_LNCollisionEventHandler_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionEventHandler*>(LNCollisionEventHandler_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }

            stat = LNCharacterController_SetCollisionEnter(local_charactercontroller, localDelegate_handler);
            LNObject_Release(localDelegate_handler);

            return true;
        }
        // LNCharacterController_SetCollisionLeave
        case 0x13B : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNCollisionEventHandler_Create(HSPSubclass_LNCollisionEventHandler_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionEventHandler*>(LNCollisionEventHandler_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }

            stat = LNCharacterController_SetCollisionLeave(local_charactercontroller, localDelegate_handler);
            LNObject_Release(localDelegate_handler);

            return true;
        }
        // LNCharacterController_SetCollisionStay
        case 0x13D : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNCollisionEventHandler_Create(HSPSubclass_LNCollisionEventHandler_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionEventHandler*>(LNCollisionEventHandler_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }

            stat = LNCharacterController_SetCollisionStay(local_charactercontroller, localDelegate_handler);
            LNObject_Release(localDelegate_handler);

            return true;
        }
        // LNCharacterController_SetPrototype_OnSerialize
        case 0x712 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNCharacterControllerSerializeHandler_Create(HSPSubclass_LNCharacterControllerSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCharacterControllerSerializeHandler*>(LNCharacterControllerSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNCharacterController_SetPrototype_OnSerialize(local_charactercontroller, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNWorld_Add
        case 0x141 : {
            // Fetch world
            const auto local_world = fetchVAInt();
            // Fetch obj
            const auto local_obj = fetchVAInt();

            stat = LNWorld_Add(local_world, local_obj);


            return true;
        }
        // LNWorld_SetPrototype_OnSerialize
        case 0x71D : {
            // Fetch world
            const auto local_world = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNWorldSerializeHandler_Create(HSPSubclass_LNWorldSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldSerializeHandler*>(LNWorldSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNWorld_SetPrototype_OnSerialize(local_world, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNComponentList_GetLength
        case 0x4D9 : {
            // Fetch componentlist
            const auto local_componentlist = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LNComponentList_GetLength(local_componentlist, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNComponentList_GetItem
        case 0x4DB : {
            // Fetch componentlist
            const auto local_componentlist = fetchVAInt();
            // Fetch index
            const auto local_index = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNComponentList_GetItem(local_componentlist, local_index, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNComponentList_SetPrototype_OnSerialize
        case 0x728 : {
            // Fetch componentlist
            const auto local_componentlist = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNComponentListSerializeHandler_Create(HSPSubclass_LNComponentListSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNComponentListSerializeHandler*>(LNComponentListSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNComponentList_SetPrototype_OnSerialize(local_componentlist, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNWorldObject_Create
        case 0x177 : {
            // Fetch outWorldObject
            PVal* pval_outWorldObject;
            const APTR aptr_outWorldObject = code_getva(&pval_outWorldObject);
            LNHandle local_outWorldObject;

            stat = LNWorldObject_Create(&local_outWorldObject);
            setVAInt(pval_outWorldObject, aptr_outWorldObject, local_outWorldObject);

            return true;
        }
        // LNWorldObject_SetPosition
        case 0x145 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch pos
            PVal* pval_pos;
            CodeGetVA_TypeChecked(&pval_pos, LNVector3);

            stat = LNWorldObject_SetPosition(local_worldobject, reinterpret_cast<const LNVector3*>(pval_pos->pt));


            return true;
        }
        // LNWorldObject_SetPositionXYZ
        case 0x147 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LNWorldObject_SetPositionXYZ(local_worldobject, local_x, local_y, local_z);


            return true;
        }
        // LNWorldObject_GetPosition
        case 0x14B : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNWorldObject_GetPosition(local_worldobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNWorldObject_SetRotationQuaternion
        case 0x14C : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch rot
            PVal* pval_rot;
            CodeGetVA_TypeChecked(&pval_rot, LNQuaternion);

            stat = LNWorldObject_SetRotationQuaternion(local_worldobject, reinterpret_cast<const LNQuaternion*>(pval_rot->pt));


            return true;
        }
        // LNWorldObject_SetRotation
        case 0x14E : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LNWorldObject_SetRotation(local_worldobject, local_x, local_y, local_z);


            return true;
        }
        // LNWorldObject_GetRotation
        case 0x152 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNQuaternion local_outReturn;

            stat = LNWorldObject_GetRotation(local_worldobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNQuaternion_typeid(), &local_outReturn);

            return true;
        }
        // LNWorldObject_SetScale
        case 0x153 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch scale
            PVal* pval_scale;
            CodeGetVA_TypeChecked(&pval_scale, LNVector3);

            stat = LNWorldObject_SetScale(local_worldobject, reinterpret_cast<const LNVector3*>(pval_scale->pt));


            return true;
        }
        // LNWorldObject_SetScaleS
        case 0x155 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch xyz
            const auto local_xyz = fetchVADouble();

            stat = LNWorldObject_SetScaleS(local_worldobject, local_xyz);


            return true;
        }
        // LNWorldObject_SetScaleXYZ
        case 0x157 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LNWorldObject_SetScaleXYZ(local_worldobject, local_x, local_y, local_z);


            return true;
        }
        // LNWorldObject_GetScale
        case 0x15B : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNWorldObject_GetScale(local_worldobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNWorldObject_SetCenterPoint
        case 0x15C : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNVector3);

            stat = LNWorldObject_SetCenterPoint(local_worldobject, reinterpret_cast<const LNVector3*>(pval_value->pt));


            return true;
        }
        // LNWorldObject_SetCenterPointXYZ
        case 0x15E : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LNWorldObject_SetCenterPointXYZ(local_worldobject, local_x, local_y, local_z);


            return true;
        }
        // LNWorldObject_GetCenterPoint
        case 0x162 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNWorldObject_GetCenterPoint(local_worldobject, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNWorldObject_LookAt
        case 0x163 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch target
            PVal* pval_target;
            CodeGetVA_TypeChecked(&pval_target, LNVector3);

            stat = LNWorldObject_LookAt(local_worldobject, reinterpret_cast<const LNVector3*>(pval_target->pt));


            return true;
        }
        // LNWorldObject_LookAtXYZ
        case 0x165 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LNWorldObject_LookAtXYZ(local_worldobject, local_x, local_y, local_z);


            return true;
        }
        // LNWorldObject_AddComponent
        case 0x169 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch component
            const auto local_component = fetchVAInt();

            stat = LNWorldObject_AddComponent(local_worldobject, local_component);


            return true;
        }
        // LNWorldObject_RemoveComponent
        case 0x16B : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch component
            const auto local_component = fetchVAInt();

            stat = LNWorldObject_RemoveComponent(local_worldobject, local_component);


            return true;
        }
        // LNWorldObject_AddTagA
        case 0x16D : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch tag
            const auto local_tag = fetchVAString();

            stat = LNWorldObject_AddTagA(local_worldobject, local_tag);


            return true;
        }
        // LNWorldObject_RemoveTagA
        case 0x16F : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch tag
            const auto local_tag = fetchVAString();

            stat = LNWorldObject_RemoveTagA(local_worldobject, local_tag);


            return true;
        }
        // LNWorldObject_HasTagA
        case 0x171 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch tag
            const auto local_tag = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNWorldObject_HasTagA(local_worldobject, local_tag, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNWorldObject_Destroy
        case 0x173 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();

            stat = LNWorldObject_Destroy(local_worldobject);


            return true;
        }
        // LNWorldObject_GetComponents
        case 0x174 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNWorldObject_GetComponents(local_worldobject, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNWorldObject_SetPrototype_OnSerialize
        case 0x733 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNWorldObjectSerializeHandler_Create(HSPSubclass_LNWorldObjectSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldObjectSerializeHandler*>(LNWorldObjectSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNWorldObject_SetPrototype_OnSerialize(local_worldobject, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNWorldObject_SetPrototype_OnUpdate
        case 0x73E : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNWorldObjectUpdateHandler_Create(HSPSubclass_LNWorldObjectUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldObjectUpdateHandler*>(LNWorldObjectUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNWorldObject_SetPrototype_OnUpdate(local_worldobject, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNVisualObject_SetVisible
        case 0x179 : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNVisualObject_SetVisible(local_visualobject, local_value);


            return true;
        }
        // LNVisualObject_IsVisible
        case 0x17B : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNVisualObject_IsVisible(local_visualobject, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNVisualObject_SetPrototype_OnSerialize
        case 0x749 : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNVisualObjectSerializeHandler_Create(HSPSubclass_LNVisualObjectSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualObjectSerializeHandler*>(LNVisualObjectSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNVisualObject_SetPrototype_OnSerialize(local_visualobject, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNVisualObject_SetPrototype_OnUpdate
        case 0x754 : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNVisualObjectUpdateHandler_Create(HSPSubclass_LNVisualObjectUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualObjectUpdateHandler*>(LNVisualObjectUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNVisualObject_SetPrototype_OnUpdate(local_visualobject, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNCamera_SetPrototype_OnSerialize
        case 0x75F : {
            // Fetch camera
            const auto local_camera = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNCameraSerializeHandler_Create(HSPSubclass_LNCameraSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraSerializeHandler*>(LNCameraSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNCamera_SetPrototype_OnSerialize(local_camera, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNCamera_SetPrototype_OnUpdate
        case 0x76A : {
            // Fetch camera
            const auto local_camera = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNCameraUpdateHandler_Create(HSPSubclass_LNCameraUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraUpdateHandler*>(LNCameraUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNCamera_SetPrototype_OnUpdate(local_camera, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNEnvironmentLight_SetEnabled
        case 0x17E : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNEnvironmentLight_SetEnabled(local_environmentlight, local_enabled);


            return true;
        }
        // LNEnvironmentLight_IsEnabled
        case 0x180 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNEnvironmentLight_IsEnabled(local_environmentlight, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetColor
        case 0x181 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNEnvironmentLight_SetColor(local_environmentlight, reinterpret_cast<const LNColor*>(pval_value->pt));


            return true;
        }
        // LNEnvironmentLight_GetColor
        case 0x183 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNEnvironmentLight_GetColor(local_environmentlight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetAmbientColor
        case 0x184 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNEnvironmentLight_SetAmbientColor(local_environmentlight, reinterpret_cast<const LNColor*>(pval_value->pt));


            return true;
        }
        // LNEnvironmentLight_GetAmbientColor
        case 0x186 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNEnvironmentLight_GetAmbientColor(local_environmentlight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNEnvironmentLight_GetSkyColor
        case 0x187 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNEnvironmentLight_GetSkyColor(local_environmentlight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetSkyColor
        case 0x188 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNEnvironmentLight_SetSkyColor(local_environmentlight, reinterpret_cast<const LNColor*>(pval_value->pt));


            return true;
        }
        // LNEnvironmentLight_GetGroundColor
        case 0x18A : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNEnvironmentLight_GetGroundColor(local_environmentlight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetGroundColor
        case 0x18B : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNEnvironmentLight_SetGroundColor(local_environmentlight, reinterpret_cast<const LNColor*>(pval_value->pt));


            return true;
        }
        // LNEnvironmentLight_SetIntensity
        case 0x18D : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LNEnvironmentLight_SetIntensity(local_environmentlight, local_intensity);


            return true;
        }
        // LNEnvironmentLight_GetIntensity
        case 0x18F : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNEnvironmentLight_GetIntensity(local_environmentlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetShadowEffectiveDistance
        case 0x190 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNEnvironmentLight_SetShadowEffectiveDistance(local_environmentlight, local_value);


            return true;
        }
        // LNEnvironmentLight_GetShadowEffectiveDistance
        case 0x192 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNEnvironmentLight_GetShadowEffectiveDistance(local_environmentlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetShadowEffectiveDepth
        case 0x193 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNEnvironmentLight_SetShadowEffectiveDepth(local_environmentlight, local_value);


            return true;
        }
        // LNEnvironmentLight_GetShadowEffectiveDepth
        case 0x195 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNEnvironmentLight_GetShadowEffectiveDepth(local_environmentlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetPrototype_OnSerialize
        case 0x775 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNEnvironmentLightSerializeHandler_Create(HSPSubclass_LNEnvironmentLightSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNEnvironmentLightSerializeHandler*>(LNEnvironmentLightSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNEnvironmentLight_SetPrototype_OnSerialize(local_environmentlight, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNEnvironmentLight_SetPrototype_OnUpdate
        case 0x780 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNEnvironmentLightUpdateHandler_Create(HSPSubclass_LNEnvironmentLightUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNEnvironmentLightUpdateHandler*>(LNEnvironmentLightUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNEnvironmentLight_SetPrototype_OnUpdate(local_environmentlight, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNDirectionalLight_Create
        case 0x1A6 : {
            // Fetch outDirectionalLight
            PVal* pval_outDirectionalLight;
            const APTR aptr_outDirectionalLight = code_getva(&pval_outDirectionalLight);
            LNHandle local_outDirectionalLight;

            stat = LNDirectionalLight_Create(&local_outDirectionalLight);
            setVAInt(pval_outDirectionalLight, aptr_outDirectionalLight, local_outDirectionalLight);

            return true;
        }
        // LNDirectionalLight_CreateWithColor
        case 0x1A7 : {
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);
            // Fetch outDirectionalLight
            PVal* pval_outDirectionalLight;
            const APTR aptr_outDirectionalLight = code_getva(&pval_outDirectionalLight);
            LNHandle local_outDirectionalLight;

            stat = LNDirectionalLight_CreateWithColor(reinterpret_cast<const LNColor*>(pval_color->pt), &local_outDirectionalLight);
            setVAInt(pval_outDirectionalLight, aptr_outDirectionalLight, local_outDirectionalLight);

            return true;
        }
        // LNDirectionalLight_SetEnabled
        case 0x197 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNDirectionalLight_SetEnabled(local_directionallight, local_enabled);


            return true;
        }
        // LNDirectionalLight_IsEnabled
        case 0x199 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNDirectionalLight_IsEnabled(local_directionallight, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNDirectionalLight_SetColor
        case 0x19A : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);

            stat = LNDirectionalLight_SetColor(local_directionallight, reinterpret_cast<const LNColor*>(pval_color->pt));


            return true;
        }
        // LNDirectionalLight_GetColor
        case 0x19C : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNDirectionalLight_GetColor(local_directionallight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNDirectionalLight_SetIntensity
        case 0x19D : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LNDirectionalLight_SetIntensity(local_directionallight, local_intensity);


            return true;
        }
        // LNDirectionalLight_GetIntensity
        case 0x19F : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNDirectionalLight_GetIntensity(local_directionallight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNDirectionalLight_SetShadowEffectiveDistance
        case 0x1A0 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNDirectionalLight_SetShadowEffectiveDistance(local_directionallight, local_value);


            return true;
        }
        // LNDirectionalLight_GetShadowEffectiveDistance
        case 0x1A2 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNDirectionalLight_GetShadowEffectiveDistance(local_directionallight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNDirectionalLight_SetShadowEffectiveDepth
        case 0x1A3 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNDirectionalLight_SetShadowEffectiveDepth(local_directionallight, local_value);


            return true;
        }
        // LNDirectionalLight_GetShadowEffectiveDepth
        case 0x1A5 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNDirectionalLight_GetShadowEffectiveDepth(local_directionallight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNDirectionalLight_SetPrototype_OnSerialize
        case 0x78B : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNDirectionalLightSerializeHandler_Create(HSPSubclass_LNDirectionalLightSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNDirectionalLightSerializeHandler*>(LNDirectionalLightSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNDirectionalLight_SetPrototype_OnSerialize(local_directionallight, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNDirectionalLight_SetPrototype_OnUpdate
        case 0x796 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNDirectionalLightUpdateHandler_Create(HSPSubclass_LNDirectionalLightUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNDirectionalLightUpdateHandler*>(LNDirectionalLightUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNDirectionalLight_SetPrototype_OnUpdate(local_directionallight, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNPointLight_Create
        case 0x1B9 : {
            // Fetch outPointLight
            PVal* pval_outPointLight;
            const APTR aptr_outPointLight = code_getva(&pval_outPointLight);
            LNHandle local_outPointLight;

            stat = LNPointLight_Create(&local_outPointLight);
            setVAInt(pval_outPointLight, aptr_outPointLight, local_outPointLight);

            return true;
        }
        // LNPointLight_CreateWithColorAndRange
        case 0x1BA : {
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);
            // Fetch range
            const auto local_range = fetchVADouble();
            // Fetch outPointLight
            PVal* pval_outPointLight;
            const APTR aptr_outPointLight = code_getva(&pval_outPointLight);
            LNHandle local_outPointLight;

            stat = LNPointLight_CreateWithColorAndRange(reinterpret_cast<const LNColor*>(pval_color->pt), local_range, &local_outPointLight);
            setVAInt(pval_outPointLight, aptr_outPointLight, local_outPointLight);

            return true;
        }
        // LNPointLight_SetEnabled
        case 0x1AA : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNPointLight_SetEnabled(local_pointlight, local_enabled);


            return true;
        }
        // LNPointLight_IsEnabled
        case 0x1AC : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNPointLight_IsEnabled(local_pointlight, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNPointLight_SetColor
        case 0x1AD : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);

            stat = LNPointLight_SetColor(local_pointlight, reinterpret_cast<const LNColor*>(pval_color->pt));


            return true;
        }
        // LNPointLight_GetColor
        case 0x1AF : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNPointLight_GetColor(local_pointlight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNPointLight_SetIntensity
        case 0x1B0 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LNPointLight_SetIntensity(local_pointlight, local_intensity);


            return true;
        }
        // LNPointLight_GetIntensity
        case 0x1B2 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNPointLight_GetIntensity(local_pointlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNPointLight_SetRange
        case 0x1B3 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch range
            const auto local_range = fetchVADouble();

            stat = LNPointLight_SetRange(local_pointlight, local_range);


            return true;
        }
        // LNPointLight_GetRange
        case 0x1B5 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNPointLight_GetRange(local_pointlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNPointLight_SetAttenuation
        case 0x1B6 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch attenuation
            const auto local_attenuation = fetchVADouble();

            stat = LNPointLight_SetAttenuation(local_pointlight, local_attenuation);


            return true;
        }
        // LNPointLight_GetAttenuation
        case 0x1B8 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNPointLight_GetAttenuation(local_pointlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNPointLight_SetPrototype_OnSerialize
        case 0x7A1 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPointLightSerializeHandler_Create(HSPSubclass_LNPointLightSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPointLightSerializeHandler*>(LNPointLightSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNPointLight_SetPrototype_OnSerialize(local_pointlight, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNPointLight_SetPrototype_OnUpdate
        case 0x7AC : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPointLightUpdateHandler_Create(HSPSubclass_LNPointLightUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPointLightUpdateHandler*>(LNPointLightUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNPointLight_SetPrototype_OnUpdate(local_pointlight, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSpotLight_Create
        case 0x1D3 : {
            // Fetch outSpotLight
            PVal* pval_outSpotLight;
            const APTR aptr_outSpotLight = code_getva(&pval_outSpotLight);
            LNHandle local_outSpotLight;

            stat = LNSpotLight_Create(&local_outSpotLight);
            setVAInt(pval_outSpotLight, aptr_outSpotLight, local_outSpotLight);

            return true;
        }
        // LNSpotLight_CreateWithColorAndRange
        case 0x1D4 : {
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);
            // Fetch range
            const auto local_range = fetchVADouble();
            // Fetch angle
            const auto local_angle = fetchVADouble();
            // Fetch outSpotLight
            PVal* pval_outSpotLight;
            const APTR aptr_outSpotLight = code_getva(&pval_outSpotLight);
            LNHandle local_outSpotLight;

            stat = LNSpotLight_CreateWithColorAndRange(reinterpret_cast<const LNColor*>(pval_color->pt), local_range, local_angle, &local_outSpotLight);
            setVAInt(pval_outSpotLight, aptr_outSpotLight, local_outSpotLight);

            return true;
        }
        // LNSpotLight_SetEnabled
        case 0x1BE : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNSpotLight_SetEnabled(local_spotlight, local_enabled);


            return true;
        }
        // LNSpotLight_IsEnabled
        case 0x1C0 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNSpotLight_IsEnabled(local_spotlight, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSpotLight_SetColor
        case 0x1C1 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);

            stat = LNSpotLight_SetColor(local_spotlight, reinterpret_cast<const LNColor*>(pval_color->pt));


            return true;
        }
        // LNSpotLight_GetColor
        case 0x1C3 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNSpotLight_GetColor(local_spotlight, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNSpotLight_SetIntensity
        case 0x1C4 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LNSpotLight_SetIntensity(local_spotlight, local_intensity);


            return true;
        }
        // LNSpotLight_GetIntensity
        case 0x1C6 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNSpotLight_GetIntensity(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSpotLight_SetRange
        case 0x1C7 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch range
            const auto local_range = fetchVADouble();

            stat = LNSpotLight_SetRange(local_spotlight, local_range);


            return true;
        }
        // LNSpotLight_GetRange
        case 0x1C9 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNSpotLight_GetRange(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSpotLight_SetAttenuation
        case 0x1CA : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch attenuation
            const auto local_attenuation = fetchVADouble();

            stat = LNSpotLight_SetAttenuation(local_spotlight, local_attenuation);


            return true;
        }
        // LNSpotLight_GetAttenuation
        case 0x1CC : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNSpotLight_GetAttenuation(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSpotLight_SetAngle
        case 0x1CD : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch angle
            const auto local_angle = fetchVADouble();

            stat = LNSpotLight_SetAngle(local_spotlight, local_angle);


            return true;
        }
        // LNSpotLight_GetAngle
        case 0x1CF : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNSpotLight_GetAngle(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSpotLight_SetPenumbra
        case 0x1D0 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch penumbra
            const auto local_penumbra = fetchVADouble();

            stat = LNSpotLight_SetPenumbra(local_spotlight, local_penumbra);


            return true;
        }
        // LNSpotLight_GetPenumbra
        case 0x1D2 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNSpotLight_GetPenumbra(local_spotlight, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSpotLight_SetPrototype_OnSerialize
        case 0x7B7 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSpotLightSerializeHandler_Create(HSPSubclass_LNSpotLightSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSpotLightSerializeHandler*>(LNSpotLightSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSpotLight_SetPrototype_OnSerialize(local_spotlight, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSpotLight_SetPrototype_OnUpdate
        case 0x7C2 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSpotLightUpdateHandler_Create(HSPSubclass_LNSpotLightUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSpotLightUpdateHandler*>(LNSpotLightUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSpotLight_SetPrototype_OnUpdate(local_spotlight, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNTestDelegate_Create
        case 0x57F : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outTestDelegate
            PVal* pval_outTestDelegate;
            const APTR aptr_outTestDelegate = code_getva(&pval_outTestDelegate);
            LNHandle local_outTestDelegate;

            stat = LNTestDelegate_Create(HSPSubclass_LNTestDelegate_DelegateLabelCaller, &local_outTestDelegate);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNTestDelegate*>(LNTestDelegate_GetSubinstanceId(local_outTestDelegate));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outTestDelegate, aptr_outTestDelegate, local_outTestDelegate);

            return true;
        }
        // LNSprite_Create
        case 0x1EA : {
            // Fetch outSprite
            PVal* pval_outSprite;
            const APTR aptr_outSprite = code_getva(&pval_outSprite);
            LNHandle local_outSprite;

            stat = LNSprite_Create(&local_outSprite);
            setVAInt(pval_outSprite, aptr_outSprite, local_outSprite);

            return true;
        }
        // LNSprite_CreateWithTexture
        case 0x1EB : {
            // Fetch texture
            const auto local_texture = fetchVAInt();
            // Fetch outSprite
            PVal* pval_outSprite;
            const APTR aptr_outSprite = code_getva(&pval_outSprite);
            LNHandle local_outSprite;

            stat = LNSprite_CreateWithTexture(local_texture, &local_outSprite);
            setVAInt(pval_outSprite, aptr_outSprite, local_outSprite);

            return true;
        }
        // LNSprite_CreateWithTextureAndSize
        case 0x1ED : {
            // Fetch texture
            const auto local_texture = fetchVAInt();
            // Fetch width
            const auto local_width = fetchVADouble();
            // Fetch height
            const auto local_height = fetchVADouble();
            // Fetch outSprite
            PVal* pval_outSprite;
            const APTR aptr_outSprite = code_getva(&pval_outSprite);
            LNHandle local_outSprite;

            stat = LNSprite_CreateWithTextureAndSize(local_texture, local_width, local_height, &local_outSprite);
            setVAInt(pval_outSprite, aptr_outSprite, local_outSprite);

            return true;
        }
        // LNSprite_SetTexture
        case 0x1DC : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LNSprite_SetTexture(local_sprite, local_value);


            return true;
        }
        // LNSprite_SetSize
        case 0x1DE : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNSize);

            stat = LNSprite_SetSize(local_sprite, reinterpret_cast<const LNSize*>(pval_value->pt));


            return true;
        }
        // LNSprite_SetSizeWH
        case 0x1E0 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch width
            const auto local_width = fetchVADouble();
            // Fetch height
            const auto local_height = fetchVADouble();

            stat = LNSprite_SetSizeWH(local_sprite, local_width, local_height);


            return true;
        }
        // LNSprite_SetSourceRectXYWH
        case 0x1E3 : {
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

            stat = LNSprite_SetSourceRectXYWH(local_sprite, local_x, local_y, local_width, local_height);


            return true;
        }
        // LNSprite_SetCallerTest
        case 0x1E8 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNTestDelegate_Create(HSPSubclass_LNTestDelegate_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNTestDelegate*>(LNTestDelegate_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSprite_SetCallerTest(local_sprite, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSprite_SetPrototype_OnSerialize
        case 0x7CD : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSpriteSerializeHandler_Create(HSPSubclass_LNSpriteSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSpriteSerializeHandler*>(LNSpriteSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSprite_SetPrototype_OnSerialize(local_sprite, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSprite_SetPrototype_OnUpdate
        case 0x7D8 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSpriteUpdateHandler_Create(HSPSubclass_LNSpriteUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSpriteUpdateHandler*>(LNSpriteUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSprite_SetPrototype_OnUpdate(local_sprite, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNCameraOrbitControlComponent_Create
        case 0x1F2 : {
            // Fetch outCameraOrbitControlComponent
            PVal* pval_outCameraOrbitControlComponent;
            const APTR aptr_outCameraOrbitControlComponent = code_getva(&pval_outCameraOrbitControlComponent);
            LNHandle local_outCameraOrbitControlComponent;

            stat = LNCameraOrbitControlComponent_Create(&local_outCameraOrbitControlComponent);
            setVAInt(pval_outCameraOrbitControlComponent, aptr_outCameraOrbitControlComponent, local_outCameraOrbitControlComponent);

            return true;
        }
        // LNCameraOrbitControlComponent_SetPrototype_OnSerialize
        case 0x7E3 : {
            // Fetch cameraorbitcontrolcomponent
            const auto local_cameraorbitcontrolcomponent = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNCameraOrbitControlComponentSerializeHandler_Create(HSPSubclass_LNCameraOrbitControlComponentSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraOrbitControlComponentSerializeHandler*>(LNCameraOrbitControlComponentSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNCameraOrbitControlComponent_SetPrototype_OnSerialize(local_cameraorbitcontrolcomponent, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNRaycaster_FromScreen
        case 0x1F4 : {
            // Fetch point
            PVal* pval_point;
            CodeGetVA_TypeChecked(&pval_point, LNPoint);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNRaycaster_FromScreen(reinterpret_cast<const LNPoint*>(pval_point->pt), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNRaycaster_IntersectPlane
        case 0x1F6 : {
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
            LNHandle local_outReturn;

            stat = LNRaycaster_IntersectPlane(local_raycaster, local_normalX, local_normalY, local_normalZ, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNRaycaster_SetPrototype_OnSerialize
        case 0x7EE : {
            // Fetch raycaster
            const auto local_raycaster = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNRaycasterSerializeHandler_Create(HSPSubclass_LNRaycasterSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNRaycasterSerializeHandler*>(LNRaycasterSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNRaycaster_SetPrototype_OnSerialize(local_raycaster, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNRaycastResult_GetPoint
        case 0x1FB : {
            // Fetch raycastresult
            const auto local_raycastresult = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNRaycastResult_GetPoint(local_raycastresult, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNRaycastResult_SetPrototype_OnSerialize
        case 0x7F9 : {
            // Fetch raycastresult
            const auto local_raycastresult = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNRaycastResultSerializeHandler_Create(HSPSubclass_LNRaycastResultSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNRaycastResultSerializeHandler*>(LNRaycastResultSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNRaycastResult_SetPrototype_OnSerialize(local_raycastresult, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNWorldRenderView_SetGuideGridEnabled
        case 0x1FD : {
            // Fetch worldrenderview
            const auto local_worldrenderview = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNWorldRenderView_SetGuideGridEnabled(local_worldrenderview, local_value);


            return true;
        }
        // LNWorldRenderView_GetGuideGridEnabled
        case 0x1FF : {
            // Fetch worldrenderview
            const auto local_worldrenderview = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNWorldRenderView_GetGuideGridEnabled(local_worldrenderview, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNWorldRenderView_SetPrototype_OnSerialize
        case 0x804 : {
            // Fetch worldrenderview
            const auto local_worldrenderview = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNWorldRenderViewSerializeHandler_Create(HSPSubclass_LNWorldRenderViewSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldRenderViewSerializeHandler*>(LNWorldRenderViewSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNWorldRenderView_SetPrototype_OnSerialize(local_worldrenderview, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNBoxMesh_Create
        case 0x201 : {
            // Fetch outBoxMesh
            PVal* pval_outBoxMesh;
            const APTR aptr_outBoxMesh = code_getva(&pval_outBoxMesh);
            LNHandle local_outBoxMesh;

            stat = LNBoxMesh_Create(&local_outBoxMesh);
            setVAInt(pval_outBoxMesh, aptr_outBoxMesh, local_outBoxMesh);

            return true;
        }
        // LNBoxMesh_SetPrototype_OnSerialize
        case 0x80F : {
            // Fetch boxmesh
            const auto local_boxmesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNBoxMeshSerializeHandler_Create(HSPSubclass_LNBoxMeshSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxMeshSerializeHandler*>(LNBoxMeshSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNBoxMesh_SetPrototype_OnSerialize(local_boxmesh, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNBoxMesh_SetPrototype_OnUpdate
        case 0x81A : {
            // Fetch boxmesh
            const auto local_boxmesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNBoxMeshUpdateHandler_Create(HSPSubclass_LNBoxMeshUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxMeshUpdateHandler*>(LNBoxMeshUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNBoxMesh_SetPrototype_OnUpdate(local_boxmesh, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNPlaneMesh_Create
        case 0x203 : {
            // Fetch outPlaneMesh
            PVal* pval_outPlaneMesh;
            const APTR aptr_outPlaneMesh = code_getva(&pval_outPlaneMesh);
            LNHandle local_outPlaneMesh;

            stat = LNPlaneMesh_Create(&local_outPlaneMesh);
            setVAInt(pval_outPlaneMesh, aptr_outPlaneMesh, local_outPlaneMesh);

            return true;
        }
        // LNPlaneMesh_SetPrototype_OnSerialize
        case 0x825 : {
            // Fetch planemesh
            const auto local_planemesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPlaneMeshSerializeHandler_Create(HSPSubclass_LNPlaneMeshSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPlaneMeshSerializeHandler*>(LNPlaneMeshSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNPlaneMesh_SetPrototype_OnSerialize(local_planemesh, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNPlaneMesh_SetPrototype_OnUpdate
        case 0x830 : {
            // Fetch planemesh
            const auto local_planemesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPlaneMeshUpdateHandler_Create(HSPSubclass_LNPlaneMeshUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPlaneMeshUpdateHandler*>(LNPlaneMeshUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNPlaneMesh_SetPrototype_OnUpdate(local_planemesh, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNStaticMesh_LoadA
        case 0x205 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNStaticMesh_LoadA(local_filePath, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNStaticMesh_MakeCollisionBodyA
        case 0x207 : {
            // Fetch staticmesh
            const auto local_staticmesh = fetchVAInt();
            // Fetch meshContainerName
            const auto local_meshContainerName = fetchVAString();

            stat = LNStaticMesh_MakeCollisionBodyA(local_staticmesh, local_meshContainerName);


            return true;
        }
        // LNStaticMesh_SetPrototype_OnSerialize
        case 0x83B : {
            // Fetch staticmesh
            const auto local_staticmesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNStaticMeshSerializeHandler_Create(HSPSubclass_LNStaticMeshSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNStaticMeshSerializeHandler*>(LNStaticMeshSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNStaticMesh_SetPrototype_OnSerialize(local_staticmesh, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNStaticMesh_SetPrototype_OnUpdate
        case 0x846 : {
            // Fetch staticmesh
            const auto local_staticmesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNStaticMeshUpdateHandler_Create(HSPSubclass_LNStaticMeshUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNStaticMeshUpdateHandler*>(LNStaticMeshUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNStaticMesh_SetPrototype_OnUpdate(local_staticmesh, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSkinnedMeshComponent_Create
        case 0x20C : {
            // Fetch outSkinnedMeshComponent
            PVal* pval_outSkinnedMeshComponent;
            const APTR aptr_outSkinnedMeshComponent = code_getva(&pval_outSkinnedMeshComponent);
            LNHandle local_outSkinnedMeshComponent;

            stat = LNSkinnedMeshComponent_Create(&local_outSkinnedMeshComponent);
            setVAInt(pval_outSkinnedMeshComponent, aptr_outSkinnedMeshComponent, local_outSkinnedMeshComponent);

            return true;
        }
        // LNSkinnedMeshComponent_SetModel
        case 0x20A : {
            // Fetch skinnedmeshcomponent
            const auto local_skinnedmeshcomponent = fetchVAInt();
            // Fetch model
            const auto local_model = fetchVAInt();

            stat = LNSkinnedMeshComponent_SetModel(local_skinnedmeshcomponent, local_model);


            return true;
        }
        // LNSkinnedMeshComponent_SetPrototype_OnSerialize
        case 0x851 : {
            // Fetch skinnedmeshcomponent
            const auto local_skinnedmeshcomponent = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSkinnedMeshComponentSerializeHandler_Create(HSPSubclass_LNSkinnedMeshComponentSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSkinnedMeshComponentSerializeHandler*>(LNSkinnedMeshComponentSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSkinnedMeshComponent_SetPrototype_OnSerialize(local_skinnedmeshcomponent, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNCollision_GetWorldObject
        case 0x20E : {
            // Fetch collision
            const auto local_collision = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNCollision_GetWorldObject(local_collision, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNCollision_SetPrototype_OnSerialize
        case 0x85C : {
            // Fetch collision
            const auto local_collision = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNCollisionSerializeHandler_Create(HSPSubclass_LNCollisionSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionSerializeHandler*>(LNCollisionSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNCollision_SetPrototype_OnSerialize(local_collision, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNTriggerBodyComponent_Create
        case 0x212 : {
            // Fetch outTriggerBodyComponent
            PVal* pval_outTriggerBodyComponent;
            const APTR aptr_outTriggerBodyComponent = code_getva(&pval_outTriggerBodyComponent);
            LNHandle local_outTriggerBodyComponent;

            stat = LNTriggerBodyComponent_Create(&local_outTriggerBodyComponent);
            setVAInt(pval_outTriggerBodyComponent, aptr_outTriggerBodyComponent, local_outTriggerBodyComponent);

            return true;
        }
        // LNTriggerBodyComponent_AddCollisionShape
        case 0x210 : {
            // Fetch triggerbodycomponent
            const auto local_triggerbodycomponent = fetchVAInt();
            // Fetch shape
            const auto local_shape = fetchVAInt();

            stat = LNTriggerBodyComponent_AddCollisionShape(local_triggerbodycomponent, local_shape);


            return true;
        }
        // LNTriggerBodyComponent_SetPrototype_OnSerialize
        case 0x867 : {
            // Fetch triggerbodycomponent
            const auto local_triggerbodycomponent = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNTriggerBodyComponentSerializeHandler_Create(HSPSubclass_LNTriggerBodyComponentSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNTriggerBodyComponentSerializeHandler*>(LNTriggerBodyComponentSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNTriggerBodyComponent_SetPrototype_OnSerialize(local_triggerbodycomponent, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNScene_SetClearMode
        case 0x214 : {
            // Fetch value
            const auto local_value = static_cast<LNSceneClearMode>(fetchVAInt());

            stat = LNScene_SetClearMode(local_value);


            return true;
        }
        // LNScene_SetSkyColor
        case 0x216 : {
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNScene_SetSkyColor(reinterpret_cast<const LNColor*>(pval_value->pt));


            return true;
        }
        // LNScene_SetSkyHorizonColor
        case 0x218 : {
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNScene_SetSkyHorizonColor(reinterpret_cast<const LNColor*>(pval_value->pt));


            return true;
        }
        // LNScene_SetSkyCloudColor
        case 0x21A : {
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNScene_SetSkyCloudColor(reinterpret_cast<const LNColor*>(pval_value->pt));


            return true;
        }
        // LNScene_SetSkyOverlayColor
        case 0x21C : {
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNScene_SetSkyOverlayColor(reinterpret_cast<const LNColor*>(pval_value->pt));


            return true;
        }
        // LNScene_GotoLevel
        case 0x21E : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch withEffect
            const auto local_withEffect = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_GotoLevel(local_level, local_withEffect);


            return true;
        }
        // LNScene_CallLevel
        case 0x221 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch withEffect
            const auto local_withEffect = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_CallLevel(local_level, local_withEffect);


            return true;
        }
        // LNScene_ReturnLevel
        case 0x224 : {
            // Fetch withEffect
            const auto local_withEffect = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_ReturnLevel(local_withEffect);


            return true;
        }
        // LNScene_ActiveLevel
        case 0x226 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNScene_ActiveLevel(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_IsTransitionEffectRunning
        case 0x227 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNScene_IsTransitionEffectRunning(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetTransitionEffectMode
        case 0x228 : {
            // Fetch value
            const auto local_value = static_cast<LNLevelTransitionEffectMode>(fetchVAInt());

            stat = LNScene_SetTransitionEffectMode(local_value);


            return true;
        }
        // LNScene_TransitionEffectMode
        case 0x22A : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNLevelTransitionEffectMode local_outReturn;

            stat = LNScene_TransitionEffectMode(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetTransitionDuration
        case 0x22B : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetTransitionDuration(local_value);


            return true;
        }
        // LNScene_TransitionDuration
        case 0x22D : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNScene_TransitionDuration(&local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetTransitionEffectColor
        case 0x22E : {
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNScene_SetTransitionEffectColor(reinterpret_cast<const LNColor*>(pval_value->pt));


            return true;
        }
        // LNScene_TransitionEffectColor
        case 0x230 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNScene_TransitionEffectColor(&local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNScene_SetTransitionEffectMaskTexture
        case 0x231 : {
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LNScene_SetTransitionEffectMaskTexture(local_value);


            return true;
        }
        // LNScene_TransitionEffectMaskTexture
        case 0x233 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNScene_TransitionEffectMaskTexture(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetTransitionEffectVague
        case 0x234 : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetTransitionEffectVague(local_value);


            return true;
        }
        // LNScene_TransitionEffectVague
        case 0x236 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNScene_TransitionEffectVague(&local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_StartFadeOut
        case 0x237 : {

            stat = LNScene_StartFadeOut();


            return true;
        }
        // LNScene_StartFadeIn
        case 0x238 : {

            stat = LNScene_StartFadeIn();


            return true;
        }
        // LNLevel_Create
        case 0x248 : {
            // Fetch outLevel
            PVal* pval_outLevel;
            const APTR aptr_outLevel = code_getva(&pval_outLevel);
            LNHandle local_outLevel;

            stat = LNLevel_Create(&local_outLevel);
            setVAInt(pval_outLevel, aptr_outLevel, local_outLevel);

            return true;
        }
        // LNLevel_AddObject
        case 0x23A : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch obj
            const auto local_obj = fetchVAInt();

            stat = LNLevel_AddObject(local_level, local_obj);


            return true;
        }
        // LNLevel_RemoveObject
        case 0x23C : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch obj
            const auto local_obj = fetchVAInt();

            stat = LNLevel_RemoveObject(local_level, local_obj);


            return true;
        }
        // LNLevel_AddSubLevel
        case 0x23E : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch sublevel
            const auto local_sublevel = fetchVAInt();

            stat = LNLevel_AddSubLevel(local_level, local_sublevel);


            return true;
        }
        // LNLevel_RemoveSubLevel
        case 0x240 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch sublevel
            const auto local_sublevel = fetchVAInt();

            stat = LNLevel_RemoveSubLevel(local_level, local_sublevel);


            return true;
        }
        // LNLevel_RemoveAllSubLevels
        case 0x242 : {
            // Fetch level
            const auto local_level = fetchVAInt();

            stat = LNLevel_RemoveAllSubLevels(local_level);


            return true;
        }
        // LNLevel_SetPrototype_OnSerialize
        case 0x872 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNLevelSerializeHandler_Create(HSPSubclass_LNLevelSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelSerializeHandler*>(LNLevelSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNLevel_SetPrototype_OnSerialize(local_level, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNLevel_SetPrototype_OnStart
        case 0x87D : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNLevelStartHandler_Create(HSPSubclass_LNLevelStartHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelStartHandler*>(LNLevelStartHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNLevel_SetPrototype_OnStart(local_level, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNLevel_SetPrototype_OnStop
        case 0x888 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNLevelStopHandler_Create(HSPSubclass_LNLevelStopHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelStopHandler*>(LNLevelStopHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNLevel_SetPrototype_OnStop(local_level, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNLevel_SetPrototype_OnPause
        case 0x893 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNLevelPauseHandler_Create(HSPSubclass_LNLevelPauseHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelPauseHandler*>(LNLevelPauseHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNLevel_SetPrototype_OnPause(local_level, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNLevel_SetPrototype_OnResume
        case 0x89E : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNLevelResumeHandler_Create(HSPSubclass_LNLevelResumeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelResumeHandler*>(LNLevelResumeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNLevel_SetPrototype_OnResume(local_level, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNLevel_SetPrototype_OnUpdate
        case 0x8A9 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNLevelUpdateHandler_Create(HSPSubclass_LNLevelUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelUpdateHandler*>(LNLevelUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNLevel_SetPrototype_OnUpdate(local_level, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIEventArgs_Sender
        case 0x24F : {
            // Fetch uieventargs
            const auto local_uieventargs = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIEventArgs_Sender(local_uieventargs, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIEventArgs_SetPrototype_OnSerialize
        case 0x8B4 : {
            // Fetch uieventargs
            const auto local_uieventargs = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIEventArgsSerializeHandler_Create(HSPSubclass_LNUIEventArgsSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIEventArgsSerializeHandler*>(LNUIEventArgsSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIEventArgs_SetPrototype_OnSerialize(local_uieventargs, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIGeneralEventHandler_Create
        case 0x5BC : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIGeneralEventHandler
            PVal* pval_outUIGeneralEventHandler;
            const APTR aptr_outUIGeneralEventHandler = code_getva(&pval_outUIGeneralEventHandler);
            LNHandle local_outUIGeneralEventHandler;

            stat = LNUIGeneralEventHandler_Create(HSPSubclass_LNUIGeneralEventHandler_DelegateLabelCaller, &local_outUIGeneralEventHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIGeneralEventHandler*>(LNUIGeneralEventHandler_GetSubinstanceId(local_outUIGeneralEventHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIGeneralEventHandler, aptr_outUIGeneralEventHandler, local_outUIGeneralEventHandler);

            return true;
        }
        // LNUIEventHandler_Create
        case 0x5C1 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIEventHandler
            PVal* pval_outUIEventHandler;
            const APTR aptr_outUIEventHandler = code_getva(&pval_outUIEventHandler);
            LNHandle local_outUIEventHandler;

            stat = LNUIEventHandler_Create(HSPSubclass_LNUIEventHandler_DelegateLabelCaller, &local_outUIEventHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIEventHandler*>(LNUIEventHandler_GetSubinstanceId(local_outUIEventHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIEventHandler, aptr_outUIEventHandler, local_outUIEventHandler);

            return true;
        }
        // LNUILayoutElement_SetPrototype_OnSerialize
        case 0x8BF : {
            // Fetch uilayoutelement
            const auto local_uilayoutelement = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUILayoutElementSerializeHandler_Create(HSPSubclass_LNUILayoutElementSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUILayoutElementSerializeHandler*>(LNUILayoutElementSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUILayoutElement_SetPrototype_OnSerialize(local_uilayoutelement, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIElement_SetSize
        case 0x261 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch size
            PVal* pval_size;
            CodeGetVA_TypeChecked(&pval_size, LNSize);

            stat = LNUIElement_SetSize(local_uielement, reinterpret_cast<const LNSize*>(pval_size->pt));


            return true;
        }
        // LNUIElement_SetSizeWH
        case 0x263 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch width
            const auto local_width = fetchVADouble();
            // Fetch height
            const auto local_height = fetchVADouble();

            stat = LNUIElement_SetSizeWH(local_uielement, local_width, local_height);


            return true;
        }
        // LNUIElement_SetMargin
        case 0x266 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch margin
            PVal* pval_margin;
            CodeGetVA_TypeChecked(&pval_margin, LNThickness);

            stat = LNUIElement_SetMargin(local_uielement, reinterpret_cast<const LNThickness*>(pval_margin->pt));


            return true;
        }
        // LNUIElement_GetMargin
        case 0x268 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNThickness local_outReturn;

            stat = LNUIElement_GetMargin(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNThickness_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetPadding
        case 0x269 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch padding
            PVal* pval_padding;
            CodeGetVA_TypeChecked(&pval_padding, LNThickness);

            stat = LNUIElement_SetPadding(local_uielement, reinterpret_cast<const LNThickness*>(pval_padding->pt));


            return true;
        }
        // LNUIElement_GetPadding
        case 0x26B : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNThickness local_outReturn;

            stat = LNUIElement_GetPadding(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNThickness_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetHAlignment
        case 0x26C : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNUIHAlignment>(fetchVAInt());

            stat = LNUIElement_SetHAlignment(local_uielement, local_value);


            return true;
        }
        // LNUIElement_GetHAlignment
        case 0x26E : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNUIHAlignment local_outReturn;

            stat = LNUIElement_GetHAlignment(local_uielement, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIElement_SetVAlignment
        case 0x26F : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNUIVAlignment>(fetchVAInt());

            stat = LNUIElement_SetVAlignment(local_uielement, local_value);


            return true;
        }
        // LNUIElement_GetVAlignment
        case 0x271 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNUIVAlignment local_outReturn;

            stat = LNUIElement_GetVAlignment(local_uielement, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIElement_SetAlignments
        case 0x272 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch halign
            const auto local_halign = static_cast<LNUIHAlignment>(fetchVAInt());
            // Fetch valign
            const auto local_valign = static_cast<LNUIVAlignment>(fetchVAInt());

            stat = LNUIElement_SetAlignments(local_uielement, local_halign, local_valign);


            return true;
        }
        // LNUIElement_SetPosition
        case 0x275 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch pos
            PVal* pval_pos;
            CodeGetVA_TypeChecked(&pval_pos, LNVector3);

            stat = LNUIElement_SetPosition(local_uielement, reinterpret_cast<const LNVector3*>(pval_pos->pt));


            return true;
        }
        // LNUIElement_SetPositionXYZ
        case 0x277 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LNUIElement_SetPositionXYZ(local_uielement, local_x, local_y, local_z);


            return true;
        }
        // LNUIElement_GetPosition
        case 0x27B : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNUIElement_GetPosition(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetRotation
        case 0x27C : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch rot
            PVal* pval_rot;
            CodeGetVA_TypeChecked(&pval_rot, LNQuaternion);

            stat = LNUIElement_SetRotation(local_uielement, reinterpret_cast<const LNQuaternion*>(pval_rot->pt));


            return true;
        }
        // LNUIElement_SetEulerAngles
        case 0x27E : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LNUIElement_SetEulerAngles(local_uielement, local_x, local_y, local_z);


            return true;
        }
        // LNUIElement_GetRotation
        case 0x282 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNQuaternion local_outReturn;

            stat = LNUIElement_GetRotation(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNQuaternion_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetScale
        case 0x283 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch scale
            PVal* pval_scale;
            CodeGetVA_TypeChecked(&pval_scale, LNVector3);

            stat = LNUIElement_SetScale(local_uielement, reinterpret_cast<const LNVector3*>(pval_scale->pt));


            return true;
        }
        // LNUIElement_SetScaleS
        case 0x285 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch xyz
            const auto local_xyz = fetchVADouble();

            stat = LNUIElement_SetScaleS(local_uielement, local_xyz);


            return true;
        }
        // LNUIElement_SetScaleXY
        case 0x287 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();

            stat = LNUIElement_SetScaleXY(local_uielement, local_x, local_y);


            return true;
        }
        // LNUIElement_GetScale
        case 0x28A : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNUIElement_GetScale(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetCenterPoint
        case 0x28B : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNVector3);

            stat = LNUIElement_SetCenterPoint(local_uielement, reinterpret_cast<const LNVector3*>(pval_value->pt));


            return true;
        }
        // LNUIElement_SetCenterPointXYZ
        case 0x28D : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LNUIElement_SetCenterPointXYZ(local_uielement, local_x, local_y, local_z);


            return true;
        }
        // LNUIElement_GetCenterPoint
        case 0x291 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNUIElement_GetCenterPoint(local_uielement, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetEnabled
        case 0x292 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNUIElement_SetEnabled(local_uielement, local_value);


            return true;
        }
        // LNUIElement_GetEnabled
        case 0x294 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNUIElement_GetEnabled(local_uielement, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIElement_AddChild
        case 0x295 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch child
            const auto local_child = fetchVAInt();

            stat = LNUIElement_AddChild(local_uielement, local_child);


            return true;
        }
        // LNUIElement_SetPrototype_OnSerialize
        case 0x8CA : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIElementSerializeHandler_Create(HSPSubclass_LNUIElementSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIElementSerializeHandler*>(LNUIElementSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIElement_SetPrototype_OnSerialize(local_uielement, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUITextBlock_Create
        case 0x298 : {
            // Fetch outUITextBlock
            PVal* pval_outUITextBlock;
            const APTR aptr_outUITextBlock = code_getva(&pval_outUITextBlock);
            LNHandle local_outUITextBlock;

            stat = LNUITextBlock_Create(&local_outUITextBlock);
            setVAInt(pval_outUITextBlock, aptr_outUITextBlock, local_outUITextBlock);

            return true;
        }
        // LNUITextBlock_CreateWithTextA
        case 0x299 : {
            // Fetch text
            const auto local_text = fetchVAString();
            // Fetch outUITextBlock
            PVal* pval_outUITextBlock;
            const APTR aptr_outUITextBlock = code_getva(&pval_outUITextBlock);
            LNHandle local_outUITextBlock;

            stat = LNUITextBlock_CreateWithTextA(local_text, &local_outUITextBlock);
            setVAInt(pval_outUITextBlock, aptr_outUITextBlock, local_outUITextBlock);

            return true;
        }
        // LNUITextBlock_SetPrototype_OnSerialize
        case 0x8D5 : {
            // Fetch uitextblock
            const auto local_uitextblock = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUITextBlockSerializeHandler_Create(HSPSubclass_LNUITextBlockSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUITextBlockSerializeHandler*>(LNUITextBlockSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUITextBlock_SetPrototype_OnSerialize(local_uitextblock, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUISprite_Create
        case 0x2A6 : {
            // Fetch outUISprite
            PVal* pval_outUISprite;
            const APTR aptr_outUISprite = code_getva(&pval_outUISprite);
            LNHandle local_outUISprite;

            stat = LNUISprite_Create(&local_outUISprite);
            setVAInt(pval_outUISprite, aptr_outUISprite, local_outUISprite);

            return true;
        }
        // LNUISprite_CreateWithTexture
        case 0x2A7 : {
            // Fetch texture
            const auto local_texture = fetchVAInt();
            // Fetch outUISprite
            PVal* pval_outUISprite;
            const APTR aptr_outUISprite = code_getva(&pval_outUISprite);
            LNHandle local_outUISprite;

            stat = LNUISprite_CreateWithTexture(local_texture, &local_outUISprite);
            setVAInt(pval_outUISprite, aptr_outUISprite, local_outUISprite);

            return true;
        }
        // LNUISprite_SetTexture
        case 0x29C : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch texture
            const auto local_texture = fetchVAInt();

            stat = LNUISprite_SetTexture(local_uisprite, local_texture);


            return true;
        }
        // LNUISprite_SetSourceRect
        case 0x29E : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch rect
            PVal* pval_rect;
            CodeGetVA_TypeChecked(&pval_rect, LNRect);

            stat = LNUISprite_SetSourceRect(local_uisprite, reinterpret_cast<const LNRect*>(pval_rect->pt));


            return true;
        }
        // LNUISprite_SetSourceRectXYWH
        case 0x2A0 : {
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

            stat = LNUISprite_SetSourceRectXYWH(local_uisprite, local_x, local_y, local_width, local_height);


            return true;
        }
        // LNUISprite_GetSourceRect
        case 0x2A5 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNRect local_outReturn;

            stat = LNUISprite_GetSourceRect(local_uisprite, &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNRect_typeid(), &local_outReturn);

            return true;
        }
        // LNUISprite_SetPrototype_OnSerialize
        case 0x8E0 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUISpriteSerializeHandler_Create(HSPSubclass_LNUISpriteSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUISpriteSerializeHandler*>(LNUISpriteSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUISprite_SetPrototype_OnSerialize(local_uisprite, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIIcon_LoadFontIconA
        case 0x2AA : {
            // Fetch iconName
            const auto local_iconName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIIcon_LoadFontIconA(local_iconName, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIIcon_LoadFontIconWithNameSizeA
        case 0x2AC : {
            // Fetch iconName
            const auto local_iconName = fetchVAString();
            // Fetch size
            const auto local_size = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIIcon_LoadFontIconWithNameSizeA(local_iconName, local_size, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIIcon_LoadFontIconWithNameSizeColorA
        case 0x2AF : {
            // Fetch iconName
            const auto local_iconName = fetchVAString();
            // Fetch size
            const auto local_size = fetchVAInt();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIIcon_LoadFontIconWithNameSizeColorA(local_iconName, local_size, reinterpret_cast<const LNColor*>(pval_color->pt), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIIcon_SetPrototype_OnSerialize
        case 0x8EB : {
            // Fetch uiicon
            const auto local_uiicon = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIIconSerializeHandler_Create(HSPSubclass_LNUIIconSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIIconSerializeHandler*>(LNUIIconSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIIcon_SetPrototype_OnSerialize(local_uiicon, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUI_Add
        case 0x2B4 : {
            // Fetch element
            const auto local_element = fetchVAInt();

            stat = LNUI_Add(local_element);


            return true;
        }
        // LNUILayoutPanel_SetPrototype_OnSerialize
        case 0x8F6 : {
            // Fetch uilayoutpanel
            const auto local_uilayoutpanel = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUILayoutPanelSerializeHandler_Create(HSPSubclass_LNUILayoutPanelSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUILayoutPanelSerializeHandler*>(LNUILayoutPanelSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUILayoutPanel_SetPrototype_OnSerialize(local_uilayoutpanel, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIBoxLayout_Create
        case 0x2BB : {
            // Fetch outUIBoxLayout
            PVal* pval_outUIBoxLayout;
            const APTR aptr_outUIBoxLayout = code_getva(&pval_outUIBoxLayout);
            LNHandle local_outUIBoxLayout;

            stat = LNUIBoxLayout_Create(&local_outUIBoxLayout);
            setVAInt(pval_outUIBoxLayout, aptr_outUIBoxLayout, local_outUIBoxLayout);

            return true;
        }
        // LNUIBoxLayout_SetOrientation
        case 0x2B8 : {
            // Fetch uiboxlayout
            const auto local_uiboxlayout = fetchVAInt();
            // Fetch orientation
            const auto local_orientation = static_cast<LNUILayoutOrientation>(fetchVAInt());

            stat = LNUIBoxLayout_SetOrientation(local_uiboxlayout, local_orientation);


            return true;
        }
        // LNUIBoxLayout_GetOrientation
        case 0x2BA : {
            // Fetch uiboxlayout
            const auto local_uiboxlayout = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNUILayoutOrientation local_outReturn;

            stat = LNUIBoxLayout_GetOrientation(local_uiboxlayout, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIBoxLayout_SetPrototype_OnSerialize
        case 0x901 : {
            // Fetch uiboxlayout
            const auto local_uiboxlayout = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIBoxLayoutSerializeHandler_Create(HSPSubclass_LNUIBoxLayoutSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIBoxLayoutSerializeHandler*>(LNUIBoxLayoutSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIBoxLayout_SetPrototype_OnSerialize(local_uiboxlayout, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIGridLayout_Create
        case 0x2CB : {
            // Fetch outUIGridLayout
            PVal* pval_outUIGridLayout;
            const APTR aptr_outUIGridLayout = code_getva(&pval_outUIGridLayout);
            LNHandle local_outUIGridLayout;

            stat = LNUIGridLayout_Create(&local_outUIGridLayout);
            setVAInt(pval_outUIGridLayout, aptr_outUIGridLayout, local_outUIGridLayout);

            return true;
        }
        // LNUIGridLayout_SetColumnCount
        case 0x2BD : {
            // Fetch uigridlayout
            const auto local_uigridlayout = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LNUIGridLayout_SetColumnCount(local_uigridlayout, local_value);


            return true;
        }
        // LNUIGridLayout_SetRow
        case 0x2BF : {
            // Fetch element
            const auto local_element = fetchVAInt();
            // Fetch row
            const auto local_row = fetchVAInt();

            stat = LNUIGridLayout_SetRow(local_element, local_row);


            return true;
        }
        // LNUIGridLayout_SetColumn
        case 0x2C2 : {
            // Fetch element
            const auto local_element = fetchVAInt();
            // Fetch column
            const auto local_column = fetchVAInt();

            stat = LNUIGridLayout_SetColumn(local_element, local_column);


            return true;
        }
        // LNUIGridLayout_SetPlacement
        case 0x2C5 : {
            // Fetch element
            const auto local_element = fetchVAInt();
            // Fetch row
            const auto local_row = fetchVAInt();
            // Fetch column
            const auto local_column = fetchVAInt();
            // Fetch rowSpan
            const auto local_rowSpan = fetchVAInt();
            // Fetch columnSpan
            const auto local_columnSpan = fetchVAInt();

            stat = LNUIGridLayout_SetPlacement(local_element, local_row, local_column, local_rowSpan, local_columnSpan);


            return true;
        }
        // LNUIGridLayout_SetPrototype_OnSerialize
        case 0x90C : {
            // Fetch uigridlayout
            const auto local_uigridlayout = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIGridLayoutSerializeHandler_Create(HSPSubclass_LNUIGridLayoutSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIGridLayoutSerializeHandler*>(LNUIGridLayoutSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIGridLayout_SetPrototype_OnSerialize(local_uigridlayout, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIControl_SetPrototype_OnSerialize
        case 0x917 : {
            // Fetch uicontrol
            const auto local_uicontrol = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIControlSerializeHandler_Create(HSPSubclass_LNUIControlSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIControlSerializeHandler*>(LNUIControlSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIControl_SetPrototype_OnSerialize(local_uicontrol, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIButtonBase_SetTextA
        case 0x2CE : {
            // Fetch uibuttonbase
            const auto local_uibuttonbase = fetchVAInt();
            // Fetch text
            const auto local_text = fetchVAString();

            stat = LNUIButtonBase_SetTextA(local_uibuttonbase, local_text);


            return true;
        }
        // LNUIButtonBase_SetPrototype_OnSerialize
        case 0x922 : {
            // Fetch uibuttonbase
            const auto local_uibuttonbase = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIButtonBaseSerializeHandler_Create(HSPSubclass_LNUIButtonBaseSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIButtonBaseSerializeHandler*>(LNUIButtonBaseSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIButtonBase_SetPrototype_OnSerialize(local_uibuttonbase, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIButton_Create
        case 0x2D1 : {
            // Fetch outUIButton
            PVal* pval_outUIButton;
            const APTR aptr_outUIButton = code_getva(&pval_outUIButton);
            LNHandle local_outUIButton;

            stat = LNUIButton_Create(&local_outUIButton);
            setVAInt(pval_outUIButton, aptr_outUIButton, local_outUIButton);

            return true;
        }
        // LNUIButton_CreateWithTextA
        case 0x2D2 : {
            // Fetch text
            const auto local_text = fetchVAString();
            // Fetch outUIButton
            PVal* pval_outUIButton;
            const APTR aptr_outUIButton = code_getva(&pval_outUIButton);
            LNHandle local_outUIButton;

            stat = LNUIButton_CreateWithTextA(local_text, &local_outUIButton);
            setVAInt(pval_outUIButton, aptr_outUIButton, local_outUIButton);

            return true;
        }
        // LNUIButton_ConnectOnClicked
        case 0x2D4 : {
            // Fetch uibutton
            const auto local_uibutton = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNUIEventHandler_Create(HSPSubclass_LNUIEventHandler_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIEventHandler*>(LNUIEventHandler_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIButton_ConnectOnClicked(local_uibutton, localDelegate_handler, &local_outReturn);
            LNObject_Release(localDelegate_handler);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIButton_SetPrototype_OnSerialize
        case 0x92D : {
            // Fetch uibutton
            const auto local_uibutton = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIButtonSerializeHandler_Create(HSPSubclass_LNUIButtonSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIButtonSerializeHandler*>(LNUIButtonSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIButton_SetPrototype_OnSerialize(local_uibutton, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIWindow_Create
        case 0x2D7 : {
            // Fetch outUIWindow
            PVal* pval_outUIWindow;
            const APTR aptr_outUIWindow = code_getva(&pval_outUIWindow);
            LNHandle local_outUIWindow;

            stat = LNUIWindow_Create(&local_outUIWindow);
            setVAInt(pval_outUIWindow, aptr_outUIWindow, local_outUIWindow);

            return true;
        }
        // LNUIWindow_SetPrototype_OnSerialize
        case 0x938 : {
            // Fetch uiwindow
            const auto local_uiwindow = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIWindowSerializeHandler_Create(HSPSubclass_LNUIWindowSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIWindowSerializeHandler*>(LNUIWindowSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIWindow_SetPrototype_OnSerialize(local_uiwindow, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIListItem_SetPrototype_OnSerialize
        case 0x943 : {
            // Fetch uilistitem
            const auto local_uilistitem = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIListItemSerializeHandler_Create(HSPSubclass_LNUIListItemSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListItemSerializeHandler*>(LNUIListItemSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIListItem_SetPrototype_OnSerialize(local_uilistitem, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIListItemsControl_SetItemsLayoutPanel
        case 0x2DD : {
            // Fetch uilistitemscontrol
            const auto local_uilistitemscontrol = fetchVAInt();
            // Fetch layout
            const auto local_layout = fetchVAInt();

            stat = LNUIListItemsControl_SetItemsLayoutPanel(local_uilistitemscontrol, local_layout);


            return true;
        }
        // LNUIListItemsControl_SetSubmitMode
        case 0x2DF : {
            // Fetch uilistitemscontrol
            const auto local_uilistitemscontrol = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNUIListSubmitMode>(fetchVAInt());

            stat = LNUIListItemsControl_SetSubmitMode(local_uilistitemscontrol, local_value);


            return true;
        }
        // LNUIListItemsControl_GetSubmitMode
        case 0x2E1 : {
            // Fetch uilistitemscontrol
            const auto local_uilistitemscontrol = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNUIListSubmitMode local_outReturn;

            stat = LNUIListItemsControl_GetSubmitMode(local_uilistitemscontrol, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIListItemsControl_SetPrototype_OnSerialize
        case 0x94E : {
            // Fetch uilistitemscontrol
            const auto local_uilistitemscontrol = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIListItemsControlSerializeHandler_Create(HSPSubclass_LNUIListItemsControlSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListItemsControlSerializeHandler*>(LNUIListItemsControlSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIListItemsControl_SetPrototype_OnSerialize(local_uilistitemscontrol, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIListBoxItem_Create
        case 0x2E3 : {
            // Fetch content
            const auto local_content = fetchVAInt();
            // Fetch outUIListBoxItem
            PVal* pval_outUIListBoxItem;
            const APTR aptr_outUIListBoxItem = code_getva(&pval_outUIListBoxItem);
            LNHandle local_outUIListBoxItem;

            stat = LNUIListBoxItem_Create(local_content, &local_outUIListBoxItem);
            setVAInt(pval_outUIListBoxItem, aptr_outUIListBoxItem, local_outUIListBoxItem);

            return true;
        }
        // LNUIListBoxItem_SetPrototype_OnSerialize
        case 0x959 : {
            // Fetch uilistboxitem
            const auto local_uilistboxitem = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIListBoxItemSerializeHandler_Create(HSPSubclass_LNUIListBoxItemSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListBoxItemSerializeHandler*>(LNUIListBoxItemSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIListBoxItem_SetPrototype_OnSerialize(local_uilistboxitem, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIListBox_Create
        case 0x2E8 : {
            // Fetch outUIListBox
            PVal* pval_outUIListBox;
            const APTR aptr_outUIListBox = code_getva(&pval_outUIListBox);
            LNHandle local_outUIListBox;

            stat = LNUIListBox_Create(&local_outUIListBox);
            setVAInt(pval_outUIListBox, aptr_outUIListBox, local_outUIListBox);

            return true;
        }
        // LNUIListBox_AddItem
        case 0x2E6 : {
            // Fetch uilistbox
            const auto local_uilistbox = fetchVAInt();
            // Fetch content
            const auto local_content = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIListBox_AddItem(local_uilistbox, local_content, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIListBox_SetPrototype_OnSerialize
        case 0x964 : {
            // Fetch uilistbox
            const auto local_uilistbox = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIListBoxSerializeHandler_Create(HSPSubclass_LNUIListBoxSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListBoxSerializeHandler*>(LNUIListBoxSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIListBox_SetPrototype_OnSerialize(local_uilistbox, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNInput_IsPressedA
        case 0x2EA : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNInput_IsPressedA(local_buttonName, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInput_IsTriggeredA
        case 0x2EC : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNInput_IsTriggeredA(local_buttonName, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInput_IsTriggeredOffA
        case 0x2EE : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNInput_IsTriggeredOffA(local_buttonName, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInput_IsRepeatedA
        case 0x2F0 : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNInput_IsRepeatedA(local_buttonName, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInput_GetAxisValueA
        case 0x2F2 : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNInput_GetAxisValueA(local_buttonName, &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInput_ClearAllBindings
        case 0x2F4 : {

            stat = LNInput_ClearAllBindings();


            return true;
        }
        // LNMouse_Pressed
        case 0x2F6 : {
            // Fetch button
            const auto local_button = static_cast<LNMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNMouse_Pressed(local_button, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMouse_Triggered
        case 0x2F8 : {
            // Fetch button
            const auto local_button = static_cast<LNMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNMouse_Triggered(local_button, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMouse_TriggeredOff
        case 0x2FA : {
            // Fetch button
            const auto local_button = static_cast<LNMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNMouse_TriggeredOff(local_button, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMouse_Repeated
        case 0x2FC : {
            // Fetch button
            const auto local_button = static_cast<LNMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNMouse_Repeated(local_button, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMouse_Position
        case 0x2FE : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNPoint local_outReturn;

            stat = LNMouse_Position(&local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNPoint_typeid(), &local_outReturn);

            return true;
        }
        // LNInterpreterCommand_CodeA
        case 0x300 : {
            // Fetch interpretercommand
            const auto local_interpretercommand = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LNInterpreterCommand_CodeA(local_interpretercommand, &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInterpreterCommand_ParamsCount
        case 0x301 : {
            // Fetch interpretercommand
            const auto local_interpretercommand = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LNInterpreterCommand_ParamsCount(local_interpretercommand, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInterpreterCommand_ParamA
        case 0x302 : {
            // Fetch interpretercommand
            const auto local_interpretercommand = fetchVAInt();
            // Fetch index
            const auto local_index = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LNInterpreterCommand_ParamA(local_interpretercommand, local_index, &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInterpreterCommand_SetPrototype_OnSerialize
        case 0x96F : {
            // Fetch interpretercommand
            const auto local_interpretercommand = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNInterpreterCommandSerializeHandler_Create(HSPSubclass_LNInterpreterCommandSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterCommandSerializeHandler*>(LNInterpreterCommandSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNInterpreterCommand_SetPrototype_OnSerialize(local_interpretercommand, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNInterpreterCommandList_Create
        case 0x319 : {
            // Fetch outInterpreterCommandList
            PVal* pval_outInterpreterCommandList;
            const APTR aptr_outInterpreterCommandList = code_getva(&pval_outInterpreterCommandList);
            LNHandle local_outInterpreterCommandList;

            stat = LNInterpreterCommandList_Create(&local_outInterpreterCommandList);
            setVAInt(pval_outInterpreterCommandList, aptr_outInterpreterCommandList, local_outInterpreterCommandList);

            return true;
        }
        // LNInterpreterCommandList_AddCommandA
        case 0x305 : {
            // Fetch interpretercommandlist
            const auto local_interpretercommandlist = fetchVAInt();
            // Fetch code
            const auto local_code = fetchVAString();

            stat = LNInterpreterCommandList_AddCommandA(local_interpretercommandlist, local_code);


            return true;
        }
        // LNInterpreterCommandList_AddCommand1A
        case 0x307 : {
            // Fetch interpretercommandlist
            const auto local_interpretercommandlist = fetchVAInt();
            // Fetch code
            const auto local_code = fetchVAString();
            // Fetch param0
            const auto local_param0 = fetchVAString();

            stat = LNInterpreterCommandList_AddCommand1A(local_interpretercommandlist, local_code, local_param0);


            return true;
        }
        // LNInterpreterCommandList_AddCommand2A
        case 0x30A : {
            // Fetch interpretercommandlist
            const auto local_interpretercommandlist = fetchVAInt();
            // Fetch code
            const auto local_code = fetchVAString();
            // Fetch param0
            const auto local_param0 = fetchVAString();
            // Fetch param1
            const auto local_param1 = fetchVAString();

            stat = LNInterpreterCommandList_AddCommand2A(local_interpretercommandlist, local_code, local_param0, local_param1);


            return true;
        }
        // LNInterpreterCommandList_AddCommand3A
        case 0x30E : {
            // Fetch interpretercommandlist
            const auto local_interpretercommandlist = fetchVAInt();
            // Fetch code
            const auto local_code = fetchVAString();
            // Fetch param0
            const auto local_param0 = fetchVAString();
            // Fetch param1
            const auto local_param1 = fetchVAString();
            // Fetch param2
            const auto local_param2 = fetchVAString();

            stat = LNInterpreterCommandList_AddCommand3A(local_interpretercommandlist, local_code, local_param0, local_param1, local_param2);


            return true;
        }
        // LNInterpreterCommandList_AddCommand4A
        case 0x313 : {
            // Fetch interpretercommandlist
            const auto local_interpretercommandlist = fetchVAInt();
            // Fetch code
            const auto local_code = fetchVAString();
            // Fetch param0
            const auto local_param0 = fetchVAString();
            // Fetch param1
            const auto local_param1 = fetchVAString();
            // Fetch param2
            const auto local_param2 = fetchVAString();
            // Fetch param3
            const auto local_param3 = fetchVAString();

            stat = LNInterpreterCommandList_AddCommand4A(local_interpretercommandlist, local_code, local_param0, local_param1, local_param2, local_param3);


            return true;
        }
        // LNInterpreterCommandList_SetPrototype_OnSerialize
        case 0x97A : {
            // Fetch interpretercommandlist
            const auto local_interpretercommandlist = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNInterpreterCommandListSerializeHandler_Create(HSPSubclass_LNInterpreterCommandListSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterCommandListSerializeHandler*>(LNInterpreterCommandListSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNInterpreterCommandList_SetPrototype_OnSerialize(local_interpretercommandlist, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNInterpreterCommandDelegate_Create
        case 0x63F : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outInterpreterCommandDelegate
            PVal* pval_outInterpreterCommandDelegate;
            const APTR aptr_outInterpreterCommandDelegate = code_getva(&pval_outInterpreterCommandDelegate);
            LNHandle local_outInterpreterCommandDelegate;

            stat = LNInterpreterCommandDelegate_Create(HSPSubclass_LNInterpreterCommandDelegate_DelegateLabelCaller, &local_outInterpreterCommandDelegate);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterCommandDelegate*>(LNInterpreterCommandDelegate_GetSubinstanceId(local_outInterpreterCommandDelegate));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outInterpreterCommandDelegate, aptr_outInterpreterCommandDelegate, local_outInterpreterCommandDelegate);

            return true;
        }
        // LNInterpreter_Create
        case 0x32D : {
            // Fetch outInterpreter
            PVal* pval_outInterpreter;
            const APTR aptr_outInterpreter = code_getva(&pval_outInterpreter);
            LNHandle local_outInterpreter;

            stat = LNInterpreter_Create(&local_outInterpreter);
            setVAInt(pval_outInterpreter, aptr_outInterpreter, local_outInterpreter);

            return true;
        }
        // LNInterpreter_Clear
        case 0x31E : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();

            stat = LNInterpreter_Clear(local_interpreter);


            return true;
        }
        // LNInterpreter_Run
        case 0x31F : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch commandList
            const auto local_commandList = fetchVAInt();

            stat = LNInterpreter_Run(local_interpreter, local_commandList);


            return true;
        }
        // LNInterpreter_IsRunning
        case 0x321 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNInterpreter_IsRunning(local_interpreter, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInterpreter_Update
        case 0x322 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();

            stat = LNInterpreter_Update(local_interpreter);


            return true;
        }
        // LNInterpreter_Terminate
        case 0x323 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();

            stat = LNInterpreter_Terminate(local_interpreter);


            return true;
        }
        // LNInterpreter_RegisterCommandHandlerA
        case 0x324 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNInterpreterCommandDelegate_Create(HSPSubclass_LNInterpreterCommandDelegate_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterCommandDelegate*>(LNInterpreterCommandDelegate_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }

            stat = LNInterpreter_RegisterCommandHandlerA(local_interpreter, local_name, localDelegate_handler);
            LNObject_Release(localDelegate_handler);

            return true;
        }
        // LNInterpreter_SetWaitModeA
        case 0x327 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch mode
            const auto local_mode = fetchVAString();

            stat = LNInterpreter_SetWaitModeA(local_interpreter, local_mode);


            return true;
        }
        // LNInterpreter_GetWaitModeA
        case 0x329 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LNInterpreter_GetWaitModeA(local_interpreter, &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInterpreter_SetWaitCount
        case 0x32A : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch count
            const auto local_count = fetchVAInt();

            stat = LNInterpreter_SetWaitCount(local_interpreter, local_count);


            return true;
        }
        // LNInterpreter_GetWaitCount
        case 0x32C : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LNInterpreter_GetWaitCount(local_interpreter, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInterpreter_SetPrototype_OnSerialize
        case 0x985 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNInterpreterSerializeHandler_Create(HSPSubclass_LNInterpreterSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterSerializeHandler*>(LNInterpreterSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNInterpreter_SetPrototype_OnSerialize(local_interpreter, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNInterpreter_SetPrototype_OnUpdateWait
        case 0x991 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNInterpreterUpdateWaitHandler_Create(HSPSubclass_LNInterpreterUpdateWaitHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterUpdateWaitHandler*>(LNInterpreterUpdateWaitHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNInterpreter_SetPrototype_OnUpdateWait(local_interpreter, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNEngineSettings_SetMainWindowSize
        case 0x330 : {
            // Fetch width
            const auto local_width = fetchVAInt();
            // Fetch height
            const auto local_height = fetchVAInt();

            stat = LNEngineSettings_SetMainWindowSize(local_width, local_height);


            return true;
        }
        // LNEngineSettings_SetMainWorldViewSize
        case 0x333 : {
            // Fetch width
            const auto local_width = fetchVAInt();
            // Fetch height
            const auto local_height = fetchVAInt();

            stat = LNEngineSettings_SetMainWorldViewSize(local_width, local_height);


            return true;
        }
        // LNEngineSettings_SetMainWindowTitleA
        case 0x336 : {
            // Fetch title
            const auto local_title = fetchVAString();

            stat = LNEngineSettings_SetMainWindowTitleA(local_title);


            return true;
        }
        // LNEngineSettings_AddAssetDirectoryA
        case 0x338 : {
            // Fetch path
            const auto local_path = fetchVAString();

            stat = LNEngineSettings_AddAssetDirectoryA(local_path);


            return true;
        }
        // LNEngineSettings_AddAssetArchiveA
        case 0x33A : {
            // Fetch fileFullPath
            const auto local_fileFullPath = fetchVAString();
            // Fetch password
            const auto local_password = fetchVAString();

            stat = LNEngineSettings_AddAssetArchiveA(local_fileFullPath, local_password);


            return true;
        }
        // LNEngineSettings_SetFrameRate
        case 0x33D : {
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LNEngineSettings_SetFrameRate(local_value);


            return true;
        }
        // LNEngineSettings_SetDefaultUIThemeA
        case 0x33F : {
            // Fetch value
            const auto local_value = fetchVAString();

            stat = LNEngineSettings_SetDefaultUIThemeA(local_value);


            return true;
        }
        // LNEngineSettings_SetDebugToolEnabled
        case 0x341 : {
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNEngineSettings_SetDebugToolEnabled(local_enabled);


            return true;
        }
        // LNEngineSettings_SetEngineLogEnabled
        case 0x343 : {
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNEngineSettings_SetEngineLogEnabled(local_enabled);


            return true;
        }
        // LNEngineSettings_SetEngineLogFilePathA
        case 0x345 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();

            stat = LNEngineSettings_SetEngineLogFilePathA(local_filePath);


            return true;
        }
        // LNEngine_Initialize
        case 0x348 : {

            stat = LNEngine_Initialize();


            return true;
        }
        // LNEngine_Finalize
        case 0x349 : {

            stat = LNEngine_Finalize();


            return true;
        }
        // LNEngine_Update
        case 0x34A : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNEngine_Update(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEngine_Run
        case 0x34B : {
            // Fetch app
            const auto local_app = fetchVAInt();

            stat = LNEngine_Run(local_app);


            return true;
        }
        // LNEngine_Time
        case 0x34D : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            double local_outReturn;

            stat = LNEngine_Time(&local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEngine_GetCamera
        case 0x34E : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNEngine_GetCamera(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEngine_GetMainLight
        case 0x34F : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNEngine_GetMainLight(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEngine_GetRenderView
        case 0x350 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNEngine_GetRenderView(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNApplication_Create
        case 0x355 : {
            // Fetch outApplication
            PVal* pval_outApplication;
            const APTR aptr_outApplication = code_getva(&pval_outApplication);
            LNHandle local_outApplication;

            stat = LNApplication_Create(&local_outApplication);
            setVAInt(pval_outApplication, aptr_outApplication, local_outApplication);

            return true;
        }
        // LNApplication_OnInit
        case 0x352 : {
            // Fetch application
            const auto local_application = fetchVAInt();

            stat = LNApplication_OnInit(local_application);


            return true;
        }
        // LNApplication_OnUpdate
        case 0x353 : {
            // Fetch application
            const auto local_application = fetchVAInt();

            stat = LNApplication_OnUpdate(local_application);


            return true;
        }
        // LNApplication_World
        case 0x354 : {
            // Fetch application
            const auto local_application = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNApplication_World(local_application, &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNApplication_SetPrototype_OnSerialize
        case 0x99C : {
            // Fetch application
            const auto local_application = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNApplicationSerializeHandler_Create(HSPSubclass_LNApplicationSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNApplicationSerializeHandler*>(LNApplicationSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNApplication_SetPrototype_OnSerialize(local_application, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNApplication_SetPrototype_OnInit
        case 0x9A7 : {
            // Fetch application
            const auto local_application = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNApplicationInitHandler_Create(HSPSubclass_LNApplicationInitHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNApplicationInitHandler*>(LNApplicationInitHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNApplication_SetPrototype_OnInit(local_application, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNApplication_SetPrototype_OnUpdate
        case 0x9B2 : {
            // Fetch application
            const auto local_application = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNApplicationUpdateHandler_Create(HSPSubclass_LNApplicationUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_SUCCESS) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNApplicationUpdateHandler*>(LNApplicationUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNApplication_SetPrototype_OnUpdate(local_application, localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNDebug_SetGuideGridEnabled
        case 0x357 : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNDebug_SetGuideGridEnabled(local_value);


            return true;
        }
        // LNDebug_SetPhysicsDebugDrawEnabled
        case 0x359 : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNDebug_SetPhysicsDebugDrawEnabled(local_value);


            return true;
        }
        // LNDebug_PrintA
        case 0x35B : {
            // Fetch str
            const auto local_str = fetchVAString();

            stat = LNDebug_PrintA(local_str);


            return true;
        }
        // LNDebug_PrintWithTimeA
        case 0x35D : {
            // Fetch time
            const auto local_time = fetchVADouble();
            // Fetch str
            const auto local_str = fetchVAString();

            stat = LNDebug_PrintWithTimeA(local_time, local_str);


            return true;
        }
        // LNDebug_PrintWithTimeAndColorA
        case 0x360 : {
            // Fetch time
            const auto local_time = fetchVADouble();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);
            // Fetch str
            const auto local_str = fetchVAString();

            stat = LNDebug_PrintWithTimeAndColorA(local_time, reinterpret_cast<const LNColor*>(pval_color->pt), local_str);


            return true;
        }
        // LNObjectSerializeHandler_Create
        case 0x66B : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outObjectSerializeHandler
            PVal* pval_outObjectSerializeHandler;
            const APTR aptr_outObjectSerializeHandler = code_getva(&pval_outObjectSerializeHandler);
            LNHandle local_outObjectSerializeHandler;

            stat = LNObjectSerializeHandler_Create(HSPSubclass_LNObjectSerializeHandler_DelegateLabelCaller, &local_outObjectSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNObjectSerializeHandler*>(LNObjectSerializeHandler_GetSubinstanceId(local_outObjectSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outObjectSerializeHandler, aptr_outObjectSerializeHandler, local_outObjectSerializeHandler);

            return true;
        }
        // LNEventConnectionSerializeHandler_Create
        case 0x676 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outEventConnectionSerializeHandler
            PVal* pval_outEventConnectionSerializeHandler;
            const APTR aptr_outEventConnectionSerializeHandler = code_getva(&pval_outEventConnectionSerializeHandler);
            LNHandle local_outEventConnectionSerializeHandler;

            stat = LNEventConnectionSerializeHandler_Create(HSPSubclass_LNEventConnectionSerializeHandler_DelegateLabelCaller, &local_outEventConnectionSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNEventConnectionSerializeHandler*>(LNEventConnectionSerializeHandler_GetSubinstanceId(local_outEventConnectionSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outEventConnectionSerializeHandler, aptr_outEventConnectionSerializeHandler, local_outEventConnectionSerializeHandler);

            return true;
        }
        // LNZVTestClass1SerializeHandler_Create
        case 0x681 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestClass1SerializeHandler
            PVal* pval_outZVTestClass1SerializeHandler;
            const APTR aptr_outZVTestClass1SerializeHandler = code_getva(&pval_outZVTestClass1SerializeHandler);
            LNHandle local_outZVTestClass1SerializeHandler;

            stat = LNZVTestClass1SerializeHandler_Create(HSPSubclass_LNZVTestClass1SerializeHandler_DelegateLabelCaller, &local_outZVTestClass1SerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestClass1SerializeHandler*>(LNZVTestClass1SerializeHandler_GetSubinstanceId(local_outZVTestClass1SerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestClass1SerializeHandler, aptr_outZVTestClass1SerializeHandler, local_outZVTestClass1SerializeHandler);

            return true;
        }
        // LNZVTestEventArgs1SerializeHandler_Create
        case 0x68C : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outZVTestEventArgs1SerializeHandler
            PVal* pval_outZVTestEventArgs1SerializeHandler;
            const APTR aptr_outZVTestEventArgs1SerializeHandler = code_getva(&pval_outZVTestEventArgs1SerializeHandler);
            LNHandle local_outZVTestEventArgs1SerializeHandler;

            stat = LNZVTestEventArgs1SerializeHandler_Create(HSPSubclass_LNZVTestEventArgs1SerializeHandler_DelegateLabelCaller, &local_outZVTestEventArgs1SerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestEventArgs1SerializeHandler*>(LNZVTestEventArgs1SerializeHandler_GetSubinstanceId(local_outZVTestEventArgs1SerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outZVTestEventArgs1SerializeHandler, aptr_outZVTestEventArgs1SerializeHandler, local_outZVTestEventArgs1SerializeHandler);

            return true;
        }
        // LNSerializer2SerializeHandler_Create
        case 0x697 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSerializer2SerializeHandler
            PVal* pval_outSerializer2SerializeHandler;
            const APTR aptr_outSerializer2SerializeHandler = code_getva(&pval_outSerializer2SerializeHandler);
            LNHandle local_outSerializer2SerializeHandler;

            stat = LNSerializer2SerializeHandler_Create(HSPSubclass_LNSerializer2SerializeHandler_DelegateLabelCaller, &local_outSerializer2SerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNSerializer2SerializeHandler*>(LNSerializer2SerializeHandler_GetSubinstanceId(local_outSerializer2SerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSerializer2SerializeHandler, aptr_outSerializer2SerializeHandler, local_outSerializer2SerializeHandler);

            return true;
        }
        // LNAssetModelSerializeHandler_Create
        case 0x6A2 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outAssetModelSerializeHandler
            PVal* pval_outAssetModelSerializeHandler;
            const APTR aptr_outAssetModelSerializeHandler = code_getva(&pval_outAssetModelSerializeHandler);
            LNHandle local_outAssetModelSerializeHandler;

            stat = LNAssetModelSerializeHandler_Create(HSPSubclass_LNAssetModelSerializeHandler_DelegateLabelCaller, &local_outAssetModelSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNAssetModelSerializeHandler*>(LNAssetModelSerializeHandler_GetSubinstanceId(local_outAssetModelSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outAssetModelSerializeHandler, aptr_outAssetModelSerializeHandler, local_outAssetModelSerializeHandler);

            return true;
        }
        // LNTextureSerializeHandler_Create
        case 0x6AD : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outTextureSerializeHandler
            PVal* pval_outTextureSerializeHandler;
            const APTR aptr_outTextureSerializeHandler = code_getva(&pval_outTextureSerializeHandler);
            LNHandle local_outTextureSerializeHandler;

            stat = LNTextureSerializeHandler_Create(HSPSubclass_LNTextureSerializeHandler_DelegateLabelCaller, &local_outTextureSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNTextureSerializeHandler*>(LNTextureSerializeHandler_GetSubinstanceId(local_outTextureSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outTextureSerializeHandler, aptr_outTextureSerializeHandler, local_outTextureSerializeHandler);

            return true;
        }
        // LNTexture2DSerializeHandler_Create
        case 0x6B8 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outTexture2DSerializeHandler
            PVal* pval_outTexture2DSerializeHandler;
            const APTR aptr_outTexture2DSerializeHandler = code_getva(&pval_outTexture2DSerializeHandler);
            LNHandle local_outTexture2DSerializeHandler;

            stat = LNTexture2DSerializeHandler_Create(HSPSubclass_LNTexture2DSerializeHandler_DelegateLabelCaller, &local_outTexture2DSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNTexture2DSerializeHandler*>(LNTexture2DSerializeHandler_GetSubinstanceId(local_outTexture2DSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outTexture2DSerializeHandler, aptr_outTexture2DSerializeHandler, local_outTexture2DSerializeHandler);

            return true;
        }
        // LNRenderViewSerializeHandler_Create
        case 0x6C3 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outRenderViewSerializeHandler
            PVal* pval_outRenderViewSerializeHandler;
            const APTR aptr_outRenderViewSerializeHandler = code_getva(&pval_outRenderViewSerializeHandler);
            LNHandle local_outRenderViewSerializeHandler;

            stat = LNRenderViewSerializeHandler_Create(HSPSubclass_LNRenderViewSerializeHandler_DelegateLabelCaller, &local_outRenderViewSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNRenderViewSerializeHandler*>(LNRenderViewSerializeHandler_GetSubinstanceId(local_outRenderViewSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outRenderViewSerializeHandler, aptr_outRenderViewSerializeHandler, local_outRenderViewSerializeHandler);

            return true;
        }
        // LNSkinnedMeshModelSerializeHandler_Create
        case 0x6CE : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSkinnedMeshModelSerializeHandler
            PVal* pval_outSkinnedMeshModelSerializeHandler;
            const APTR aptr_outSkinnedMeshModelSerializeHandler = code_getva(&pval_outSkinnedMeshModelSerializeHandler);
            LNHandle local_outSkinnedMeshModelSerializeHandler;

            stat = LNSkinnedMeshModelSerializeHandler_Create(HSPSubclass_LNSkinnedMeshModelSerializeHandler_DelegateLabelCaller, &local_outSkinnedMeshModelSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNSkinnedMeshModelSerializeHandler*>(LNSkinnedMeshModelSerializeHandler_GetSubinstanceId(local_outSkinnedMeshModelSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSkinnedMeshModelSerializeHandler, aptr_outSkinnedMeshModelSerializeHandler, local_outSkinnedMeshModelSerializeHandler);

            return true;
        }
        // LNCollisionShapeSerializeHandler_Create
        case 0x6D9 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outCollisionShapeSerializeHandler
            PVal* pval_outCollisionShapeSerializeHandler;
            const APTR aptr_outCollisionShapeSerializeHandler = code_getva(&pval_outCollisionShapeSerializeHandler);
            LNHandle local_outCollisionShapeSerializeHandler;

            stat = LNCollisionShapeSerializeHandler_Create(HSPSubclass_LNCollisionShapeSerializeHandler_DelegateLabelCaller, &local_outCollisionShapeSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionShapeSerializeHandler*>(LNCollisionShapeSerializeHandler_GetSubinstanceId(local_outCollisionShapeSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outCollisionShapeSerializeHandler, aptr_outCollisionShapeSerializeHandler, local_outCollisionShapeSerializeHandler);

            return true;
        }
        // LNBoxCollisionShapeSerializeHandler_Create
        case 0x6E4 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outBoxCollisionShapeSerializeHandler
            PVal* pval_outBoxCollisionShapeSerializeHandler;
            const APTR aptr_outBoxCollisionShapeSerializeHandler = code_getva(&pval_outBoxCollisionShapeSerializeHandler);
            LNHandle local_outBoxCollisionShapeSerializeHandler;

            stat = LNBoxCollisionShapeSerializeHandler_Create(HSPSubclass_LNBoxCollisionShapeSerializeHandler_DelegateLabelCaller, &local_outBoxCollisionShapeSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxCollisionShapeSerializeHandler*>(LNBoxCollisionShapeSerializeHandler_GetSubinstanceId(local_outBoxCollisionShapeSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outBoxCollisionShapeSerializeHandler, aptr_outBoxCollisionShapeSerializeHandler, local_outBoxCollisionShapeSerializeHandler);

            return true;
        }
        // LNComponentSerializeHandler_Create
        case 0x6EF : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outComponentSerializeHandler
            PVal* pval_outComponentSerializeHandler;
            const APTR aptr_outComponentSerializeHandler = code_getva(&pval_outComponentSerializeHandler);
            LNHandle local_outComponentSerializeHandler;

            stat = LNComponentSerializeHandler_Create(HSPSubclass_LNComponentSerializeHandler_DelegateLabelCaller, &local_outComponentSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNComponentSerializeHandler*>(LNComponentSerializeHandler_GetSubinstanceId(local_outComponentSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outComponentSerializeHandler, aptr_outComponentSerializeHandler, local_outComponentSerializeHandler);

            return true;
        }
        // LNVisualComponentSerializeHandler_Create
        case 0x6FA : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outVisualComponentSerializeHandler
            PVal* pval_outVisualComponentSerializeHandler;
            const APTR aptr_outVisualComponentSerializeHandler = code_getva(&pval_outVisualComponentSerializeHandler);
            LNHandle local_outVisualComponentSerializeHandler;

            stat = LNVisualComponentSerializeHandler_Create(HSPSubclass_LNVisualComponentSerializeHandler_DelegateLabelCaller, &local_outVisualComponentSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualComponentSerializeHandler*>(LNVisualComponentSerializeHandler_GetSubinstanceId(local_outVisualComponentSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outVisualComponentSerializeHandler, aptr_outVisualComponentSerializeHandler, local_outVisualComponentSerializeHandler);

            return true;
        }
        // LNSpriteComponentSerializeHandler_Create
        case 0x705 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSpriteComponentSerializeHandler
            PVal* pval_outSpriteComponentSerializeHandler;
            const APTR aptr_outSpriteComponentSerializeHandler = code_getva(&pval_outSpriteComponentSerializeHandler);
            LNHandle local_outSpriteComponentSerializeHandler;

            stat = LNSpriteComponentSerializeHandler_Create(HSPSubclass_LNSpriteComponentSerializeHandler_DelegateLabelCaller, &local_outSpriteComponentSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNSpriteComponentSerializeHandler*>(LNSpriteComponentSerializeHandler_GetSubinstanceId(local_outSpriteComponentSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSpriteComponentSerializeHandler, aptr_outSpriteComponentSerializeHandler, local_outSpriteComponentSerializeHandler);

            return true;
        }
        // LNCharacterControllerSerializeHandler_Create
        case 0x710 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outCharacterControllerSerializeHandler
            PVal* pval_outCharacterControllerSerializeHandler;
            const APTR aptr_outCharacterControllerSerializeHandler = code_getva(&pval_outCharacterControllerSerializeHandler);
            LNHandle local_outCharacterControllerSerializeHandler;

            stat = LNCharacterControllerSerializeHandler_Create(HSPSubclass_LNCharacterControllerSerializeHandler_DelegateLabelCaller, &local_outCharacterControllerSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNCharacterControllerSerializeHandler*>(LNCharacterControllerSerializeHandler_GetSubinstanceId(local_outCharacterControllerSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outCharacterControllerSerializeHandler, aptr_outCharacterControllerSerializeHandler, local_outCharacterControllerSerializeHandler);

            return true;
        }
        // LNWorldSerializeHandler_Create
        case 0x71B : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outWorldSerializeHandler
            PVal* pval_outWorldSerializeHandler;
            const APTR aptr_outWorldSerializeHandler = code_getva(&pval_outWorldSerializeHandler);
            LNHandle local_outWorldSerializeHandler;

            stat = LNWorldSerializeHandler_Create(HSPSubclass_LNWorldSerializeHandler_DelegateLabelCaller, &local_outWorldSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldSerializeHandler*>(LNWorldSerializeHandler_GetSubinstanceId(local_outWorldSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outWorldSerializeHandler, aptr_outWorldSerializeHandler, local_outWorldSerializeHandler);

            return true;
        }
        // LNComponentListSerializeHandler_Create
        case 0x726 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outComponentListSerializeHandler
            PVal* pval_outComponentListSerializeHandler;
            const APTR aptr_outComponentListSerializeHandler = code_getva(&pval_outComponentListSerializeHandler);
            LNHandle local_outComponentListSerializeHandler;

            stat = LNComponentListSerializeHandler_Create(HSPSubclass_LNComponentListSerializeHandler_DelegateLabelCaller, &local_outComponentListSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNComponentListSerializeHandler*>(LNComponentListSerializeHandler_GetSubinstanceId(local_outComponentListSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outComponentListSerializeHandler, aptr_outComponentListSerializeHandler, local_outComponentListSerializeHandler);

            return true;
        }
        // LNWorldObjectSerializeHandler_Create
        case 0x731 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outWorldObjectSerializeHandler
            PVal* pval_outWorldObjectSerializeHandler;
            const APTR aptr_outWorldObjectSerializeHandler = code_getva(&pval_outWorldObjectSerializeHandler);
            LNHandle local_outWorldObjectSerializeHandler;

            stat = LNWorldObjectSerializeHandler_Create(HSPSubclass_LNWorldObjectSerializeHandler_DelegateLabelCaller, &local_outWorldObjectSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldObjectSerializeHandler*>(LNWorldObjectSerializeHandler_GetSubinstanceId(local_outWorldObjectSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outWorldObjectSerializeHandler, aptr_outWorldObjectSerializeHandler, local_outWorldObjectSerializeHandler);

            return true;
        }
        // LNWorldObjectUpdateHandler_Create
        case 0x73C : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outWorldObjectUpdateHandler
            PVal* pval_outWorldObjectUpdateHandler;
            const APTR aptr_outWorldObjectUpdateHandler = code_getva(&pval_outWorldObjectUpdateHandler);
            LNHandle local_outWorldObjectUpdateHandler;

            stat = LNWorldObjectUpdateHandler_Create(HSPSubclass_LNWorldObjectUpdateHandler_DelegateLabelCaller, &local_outWorldObjectUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldObjectUpdateHandler*>(LNWorldObjectUpdateHandler_GetSubinstanceId(local_outWorldObjectUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outWorldObjectUpdateHandler, aptr_outWorldObjectUpdateHandler, local_outWorldObjectUpdateHandler);

            return true;
        }
        // LNVisualObjectSerializeHandler_Create
        case 0x747 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outVisualObjectSerializeHandler
            PVal* pval_outVisualObjectSerializeHandler;
            const APTR aptr_outVisualObjectSerializeHandler = code_getva(&pval_outVisualObjectSerializeHandler);
            LNHandle local_outVisualObjectSerializeHandler;

            stat = LNVisualObjectSerializeHandler_Create(HSPSubclass_LNVisualObjectSerializeHandler_DelegateLabelCaller, &local_outVisualObjectSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualObjectSerializeHandler*>(LNVisualObjectSerializeHandler_GetSubinstanceId(local_outVisualObjectSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outVisualObjectSerializeHandler, aptr_outVisualObjectSerializeHandler, local_outVisualObjectSerializeHandler);

            return true;
        }
        // LNVisualObjectUpdateHandler_Create
        case 0x752 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outVisualObjectUpdateHandler
            PVal* pval_outVisualObjectUpdateHandler;
            const APTR aptr_outVisualObjectUpdateHandler = code_getva(&pval_outVisualObjectUpdateHandler);
            LNHandle local_outVisualObjectUpdateHandler;

            stat = LNVisualObjectUpdateHandler_Create(HSPSubclass_LNVisualObjectUpdateHandler_DelegateLabelCaller, &local_outVisualObjectUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualObjectUpdateHandler*>(LNVisualObjectUpdateHandler_GetSubinstanceId(local_outVisualObjectUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outVisualObjectUpdateHandler, aptr_outVisualObjectUpdateHandler, local_outVisualObjectUpdateHandler);

            return true;
        }
        // LNCameraSerializeHandler_Create
        case 0x75D : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outCameraSerializeHandler
            PVal* pval_outCameraSerializeHandler;
            const APTR aptr_outCameraSerializeHandler = code_getva(&pval_outCameraSerializeHandler);
            LNHandle local_outCameraSerializeHandler;

            stat = LNCameraSerializeHandler_Create(HSPSubclass_LNCameraSerializeHandler_DelegateLabelCaller, &local_outCameraSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraSerializeHandler*>(LNCameraSerializeHandler_GetSubinstanceId(local_outCameraSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outCameraSerializeHandler, aptr_outCameraSerializeHandler, local_outCameraSerializeHandler);

            return true;
        }
        // LNCameraUpdateHandler_Create
        case 0x768 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outCameraUpdateHandler
            PVal* pval_outCameraUpdateHandler;
            const APTR aptr_outCameraUpdateHandler = code_getva(&pval_outCameraUpdateHandler);
            LNHandle local_outCameraUpdateHandler;

            stat = LNCameraUpdateHandler_Create(HSPSubclass_LNCameraUpdateHandler_DelegateLabelCaller, &local_outCameraUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraUpdateHandler*>(LNCameraUpdateHandler_GetSubinstanceId(local_outCameraUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outCameraUpdateHandler, aptr_outCameraUpdateHandler, local_outCameraUpdateHandler);

            return true;
        }
        // LNEnvironmentLightSerializeHandler_Create
        case 0x773 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outEnvironmentLightSerializeHandler
            PVal* pval_outEnvironmentLightSerializeHandler;
            const APTR aptr_outEnvironmentLightSerializeHandler = code_getva(&pval_outEnvironmentLightSerializeHandler);
            LNHandle local_outEnvironmentLightSerializeHandler;

            stat = LNEnvironmentLightSerializeHandler_Create(HSPSubclass_LNEnvironmentLightSerializeHandler_DelegateLabelCaller, &local_outEnvironmentLightSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNEnvironmentLightSerializeHandler*>(LNEnvironmentLightSerializeHandler_GetSubinstanceId(local_outEnvironmentLightSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outEnvironmentLightSerializeHandler, aptr_outEnvironmentLightSerializeHandler, local_outEnvironmentLightSerializeHandler);

            return true;
        }
        // LNEnvironmentLightUpdateHandler_Create
        case 0x77E : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outEnvironmentLightUpdateHandler
            PVal* pval_outEnvironmentLightUpdateHandler;
            const APTR aptr_outEnvironmentLightUpdateHandler = code_getva(&pval_outEnvironmentLightUpdateHandler);
            LNHandle local_outEnvironmentLightUpdateHandler;

            stat = LNEnvironmentLightUpdateHandler_Create(HSPSubclass_LNEnvironmentLightUpdateHandler_DelegateLabelCaller, &local_outEnvironmentLightUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNEnvironmentLightUpdateHandler*>(LNEnvironmentLightUpdateHandler_GetSubinstanceId(local_outEnvironmentLightUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outEnvironmentLightUpdateHandler, aptr_outEnvironmentLightUpdateHandler, local_outEnvironmentLightUpdateHandler);

            return true;
        }
        // LNDirectionalLightSerializeHandler_Create
        case 0x789 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outDirectionalLightSerializeHandler
            PVal* pval_outDirectionalLightSerializeHandler;
            const APTR aptr_outDirectionalLightSerializeHandler = code_getva(&pval_outDirectionalLightSerializeHandler);
            LNHandle local_outDirectionalLightSerializeHandler;

            stat = LNDirectionalLightSerializeHandler_Create(HSPSubclass_LNDirectionalLightSerializeHandler_DelegateLabelCaller, &local_outDirectionalLightSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNDirectionalLightSerializeHandler*>(LNDirectionalLightSerializeHandler_GetSubinstanceId(local_outDirectionalLightSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outDirectionalLightSerializeHandler, aptr_outDirectionalLightSerializeHandler, local_outDirectionalLightSerializeHandler);

            return true;
        }
        // LNDirectionalLightUpdateHandler_Create
        case 0x794 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outDirectionalLightUpdateHandler
            PVal* pval_outDirectionalLightUpdateHandler;
            const APTR aptr_outDirectionalLightUpdateHandler = code_getva(&pval_outDirectionalLightUpdateHandler);
            LNHandle local_outDirectionalLightUpdateHandler;

            stat = LNDirectionalLightUpdateHandler_Create(HSPSubclass_LNDirectionalLightUpdateHandler_DelegateLabelCaller, &local_outDirectionalLightUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNDirectionalLightUpdateHandler*>(LNDirectionalLightUpdateHandler_GetSubinstanceId(local_outDirectionalLightUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outDirectionalLightUpdateHandler, aptr_outDirectionalLightUpdateHandler, local_outDirectionalLightUpdateHandler);

            return true;
        }
        // LNPointLightSerializeHandler_Create
        case 0x79F : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outPointLightSerializeHandler
            PVal* pval_outPointLightSerializeHandler;
            const APTR aptr_outPointLightSerializeHandler = code_getva(&pval_outPointLightSerializeHandler);
            LNHandle local_outPointLightSerializeHandler;

            stat = LNPointLightSerializeHandler_Create(HSPSubclass_LNPointLightSerializeHandler_DelegateLabelCaller, &local_outPointLightSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNPointLightSerializeHandler*>(LNPointLightSerializeHandler_GetSubinstanceId(local_outPointLightSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outPointLightSerializeHandler, aptr_outPointLightSerializeHandler, local_outPointLightSerializeHandler);

            return true;
        }
        // LNPointLightUpdateHandler_Create
        case 0x7AA : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outPointLightUpdateHandler
            PVal* pval_outPointLightUpdateHandler;
            const APTR aptr_outPointLightUpdateHandler = code_getva(&pval_outPointLightUpdateHandler);
            LNHandle local_outPointLightUpdateHandler;

            stat = LNPointLightUpdateHandler_Create(HSPSubclass_LNPointLightUpdateHandler_DelegateLabelCaller, &local_outPointLightUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNPointLightUpdateHandler*>(LNPointLightUpdateHandler_GetSubinstanceId(local_outPointLightUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outPointLightUpdateHandler, aptr_outPointLightUpdateHandler, local_outPointLightUpdateHandler);

            return true;
        }
        // LNSpotLightSerializeHandler_Create
        case 0x7B5 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSpotLightSerializeHandler
            PVal* pval_outSpotLightSerializeHandler;
            const APTR aptr_outSpotLightSerializeHandler = code_getva(&pval_outSpotLightSerializeHandler);
            LNHandle local_outSpotLightSerializeHandler;

            stat = LNSpotLightSerializeHandler_Create(HSPSubclass_LNSpotLightSerializeHandler_DelegateLabelCaller, &local_outSpotLightSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNSpotLightSerializeHandler*>(LNSpotLightSerializeHandler_GetSubinstanceId(local_outSpotLightSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSpotLightSerializeHandler, aptr_outSpotLightSerializeHandler, local_outSpotLightSerializeHandler);

            return true;
        }
        // LNSpotLightUpdateHandler_Create
        case 0x7C0 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSpotLightUpdateHandler
            PVal* pval_outSpotLightUpdateHandler;
            const APTR aptr_outSpotLightUpdateHandler = code_getva(&pval_outSpotLightUpdateHandler);
            LNHandle local_outSpotLightUpdateHandler;

            stat = LNSpotLightUpdateHandler_Create(HSPSubclass_LNSpotLightUpdateHandler_DelegateLabelCaller, &local_outSpotLightUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNSpotLightUpdateHandler*>(LNSpotLightUpdateHandler_GetSubinstanceId(local_outSpotLightUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSpotLightUpdateHandler, aptr_outSpotLightUpdateHandler, local_outSpotLightUpdateHandler);

            return true;
        }
        // LNSpriteSerializeHandler_Create
        case 0x7CB : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSpriteSerializeHandler
            PVal* pval_outSpriteSerializeHandler;
            const APTR aptr_outSpriteSerializeHandler = code_getva(&pval_outSpriteSerializeHandler);
            LNHandle local_outSpriteSerializeHandler;

            stat = LNSpriteSerializeHandler_Create(HSPSubclass_LNSpriteSerializeHandler_DelegateLabelCaller, &local_outSpriteSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNSpriteSerializeHandler*>(LNSpriteSerializeHandler_GetSubinstanceId(local_outSpriteSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSpriteSerializeHandler, aptr_outSpriteSerializeHandler, local_outSpriteSerializeHandler);

            return true;
        }
        // LNSpriteUpdateHandler_Create
        case 0x7D6 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSpriteUpdateHandler
            PVal* pval_outSpriteUpdateHandler;
            const APTR aptr_outSpriteUpdateHandler = code_getva(&pval_outSpriteUpdateHandler);
            LNHandle local_outSpriteUpdateHandler;

            stat = LNSpriteUpdateHandler_Create(HSPSubclass_LNSpriteUpdateHandler_DelegateLabelCaller, &local_outSpriteUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNSpriteUpdateHandler*>(LNSpriteUpdateHandler_GetSubinstanceId(local_outSpriteUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSpriteUpdateHandler, aptr_outSpriteUpdateHandler, local_outSpriteUpdateHandler);

            return true;
        }
        // LNCameraOrbitControlComponentSerializeHandler_Create
        case 0x7E1 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outCameraOrbitControlComponentSerializeHandler
            PVal* pval_outCameraOrbitControlComponentSerializeHandler;
            const APTR aptr_outCameraOrbitControlComponentSerializeHandler = code_getva(&pval_outCameraOrbitControlComponentSerializeHandler);
            LNHandle local_outCameraOrbitControlComponentSerializeHandler;

            stat = LNCameraOrbitControlComponentSerializeHandler_Create(HSPSubclass_LNCameraOrbitControlComponentSerializeHandler_DelegateLabelCaller, &local_outCameraOrbitControlComponentSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraOrbitControlComponentSerializeHandler*>(LNCameraOrbitControlComponentSerializeHandler_GetSubinstanceId(local_outCameraOrbitControlComponentSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outCameraOrbitControlComponentSerializeHandler, aptr_outCameraOrbitControlComponentSerializeHandler, local_outCameraOrbitControlComponentSerializeHandler);

            return true;
        }
        // LNRaycasterSerializeHandler_Create
        case 0x7EC : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outRaycasterSerializeHandler
            PVal* pval_outRaycasterSerializeHandler;
            const APTR aptr_outRaycasterSerializeHandler = code_getva(&pval_outRaycasterSerializeHandler);
            LNHandle local_outRaycasterSerializeHandler;

            stat = LNRaycasterSerializeHandler_Create(HSPSubclass_LNRaycasterSerializeHandler_DelegateLabelCaller, &local_outRaycasterSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNRaycasterSerializeHandler*>(LNRaycasterSerializeHandler_GetSubinstanceId(local_outRaycasterSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outRaycasterSerializeHandler, aptr_outRaycasterSerializeHandler, local_outRaycasterSerializeHandler);

            return true;
        }
        // LNRaycastResultSerializeHandler_Create
        case 0x7F7 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outRaycastResultSerializeHandler
            PVal* pval_outRaycastResultSerializeHandler;
            const APTR aptr_outRaycastResultSerializeHandler = code_getva(&pval_outRaycastResultSerializeHandler);
            LNHandle local_outRaycastResultSerializeHandler;

            stat = LNRaycastResultSerializeHandler_Create(HSPSubclass_LNRaycastResultSerializeHandler_DelegateLabelCaller, &local_outRaycastResultSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNRaycastResultSerializeHandler*>(LNRaycastResultSerializeHandler_GetSubinstanceId(local_outRaycastResultSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outRaycastResultSerializeHandler, aptr_outRaycastResultSerializeHandler, local_outRaycastResultSerializeHandler);

            return true;
        }
        // LNWorldRenderViewSerializeHandler_Create
        case 0x802 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outWorldRenderViewSerializeHandler
            PVal* pval_outWorldRenderViewSerializeHandler;
            const APTR aptr_outWorldRenderViewSerializeHandler = code_getva(&pval_outWorldRenderViewSerializeHandler);
            LNHandle local_outWorldRenderViewSerializeHandler;

            stat = LNWorldRenderViewSerializeHandler_Create(HSPSubclass_LNWorldRenderViewSerializeHandler_DelegateLabelCaller, &local_outWorldRenderViewSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldRenderViewSerializeHandler*>(LNWorldRenderViewSerializeHandler_GetSubinstanceId(local_outWorldRenderViewSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outWorldRenderViewSerializeHandler, aptr_outWorldRenderViewSerializeHandler, local_outWorldRenderViewSerializeHandler);

            return true;
        }
        // LNBoxMeshSerializeHandler_Create
        case 0x80D : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outBoxMeshSerializeHandler
            PVal* pval_outBoxMeshSerializeHandler;
            const APTR aptr_outBoxMeshSerializeHandler = code_getva(&pval_outBoxMeshSerializeHandler);
            LNHandle local_outBoxMeshSerializeHandler;

            stat = LNBoxMeshSerializeHandler_Create(HSPSubclass_LNBoxMeshSerializeHandler_DelegateLabelCaller, &local_outBoxMeshSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxMeshSerializeHandler*>(LNBoxMeshSerializeHandler_GetSubinstanceId(local_outBoxMeshSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outBoxMeshSerializeHandler, aptr_outBoxMeshSerializeHandler, local_outBoxMeshSerializeHandler);

            return true;
        }
        // LNBoxMeshUpdateHandler_Create
        case 0x818 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outBoxMeshUpdateHandler
            PVal* pval_outBoxMeshUpdateHandler;
            const APTR aptr_outBoxMeshUpdateHandler = code_getva(&pval_outBoxMeshUpdateHandler);
            LNHandle local_outBoxMeshUpdateHandler;

            stat = LNBoxMeshUpdateHandler_Create(HSPSubclass_LNBoxMeshUpdateHandler_DelegateLabelCaller, &local_outBoxMeshUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxMeshUpdateHandler*>(LNBoxMeshUpdateHandler_GetSubinstanceId(local_outBoxMeshUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outBoxMeshUpdateHandler, aptr_outBoxMeshUpdateHandler, local_outBoxMeshUpdateHandler);

            return true;
        }
        // LNPlaneMeshSerializeHandler_Create
        case 0x823 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outPlaneMeshSerializeHandler
            PVal* pval_outPlaneMeshSerializeHandler;
            const APTR aptr_outPlaneMeshSerializeHandler = code_getva(&pval_outPlaneMeshSerializeHandler);
            LNHandle local_outPlaneMeshSerializeHandler;

            stat = LNPlaneMeshSerializeHandler_Create(HSPSubclass_LNPlaneMeshSerializeHandler_DelegateLabelCaller, &local_outPlaneMeshSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNPlaneMeshSerializeHandler*>(LNPlaneMeshSerializeHandler_GetSubinstanceId(local_outPlaneMeshSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outPlaneMeshSerializeHandler, aptr_outPlaneMeshSerializeHandler, local_outPlaneMeshSerializeHandler);

            return true;
        }
        // LNPlaneMeshUpdateHandler_Create
        case 0x82E : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outPlaneMeshUpdateHandler
            PVal* pval_outPlaneMeshUpdateHandler;
            const APTR aptr_outPlaneMeshUpdateHandler = code_getva(&pval_outPlaneMeshUpdateHandler);
            LNHandle local_outPlaneMeshUpdateHandler;

            stat = LNPlaneMeshUpdateHandler_Create(HSPSubclass_LNPlaneMeshUpdateHandler_DelegateLabelCaller, &local_outPlaneMeshUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNPlaneMeshUpdateHandler*>(LNPlaneMeshUpdateHandler_GetSubinstanceId(local_outPlaneMeshUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outPlaneMeshUpdateHandler, aptr_outPlaneMeshUpdateHandler, local_outPlaneMeshUpdateHandler);

            return true;
        }
        // LNStaticMeshSerializeHandler_Create
        case 0x839 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outStaticMeshSerializeHandler
            PVal* pval_outStaticMeshSerializeHandler;
            const APTR aptr_outStaticMeshSerializeHandler = code_getva(&pval_outStaticMeshSerializeHandler);
            LNHandle local_outStaticMeshSerializeHandler;

            stat = LNStaticMeshSerializeHandler_Create(HSPSubclass_LNStaticMeshSerializeHandler_DelegateLabelCaller, &local_outStaticMeshSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNStaticMeshSerializeHandler*>(LNStaticMeshSerializeHandler_GetSubinstanceId(local_outStaticMeshSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outStaticMeshSerializeHandler, aptr_outStaticMeshSerializeHandler, local_outStaticMeshSerializeHandler);

            return true;
        }
        // LNStaticMeshUpdateHandler_Create
        case 0x844 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outStaticMeshUpdateHandler
            PVal* pval_outStaticMeshUpdateHandler;
            const APTR aptr_outStaticMeshUpdateHandler = code_getva(&pval_outStaticMeshUpdateHandler);
            LNHandle local_outStaticMeshUpdateHandler;

            stat = LNStaticMeshUpdateHandler_Create(HSPSubclass_LNStaticMeshUpdateHandler_DelegateLabelCaller, &local_outStaticMeshUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNStaticMeshUpdateHandler*>(LNStaticMeshUpdateHandler_GetSubinstanceId(local_outStaticMeshUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outStaticMeshUpdateHandler, aptr_outStaticMeshUpdateHandler, local_outStaticMeshUpdateHandler);

            return true;
        }
        // LNSkinnedMeshComponentSerializeHandler_Create
        case 0x84F : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSkinnedMeshComponentSerializeHandler
            PVal* pval_outSkinnedMeshComponentSerializeHandler;
            const APTR aptr_outSkinnedMeshComponentSerializeHandler = code_getva(&pval_outSkinnedMeshComponentSerializeHandler);
            LNHandle local_outSkinnedMeshComponentSerializeHandler;

            stat = LNSkinnedMeshComponentSerializeHandler_Create(HSPSubclass_LNSkinnedMeshComponentSerializeHandler_DelegateLabelCaller, &local_outSkinnedMeshComponentSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNSkinnedMeshComponentSerializeHandler*>(LNSkinnedMeshComponentSerializeHandler_GetSubinstanceId(local_outSkinnedMeshComponentSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSkinnedMeshComponentSerializeHandler, aptr_outSkinnedMeshComponentSerializeHandler, local_outSkinnedMeshComponentSerializeHandler);

            return true;
        }
        // LNCollisionSerializeHandler_Create
        case 0x85A : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outCollisionSerializeHandler
            PVal* pval_outCollisionSerializeHandler;
            const APTR aptr_outCollisionSerializeHandler = code_getva(&pval_outCollisionSerializeHandler);
            LNHandle local_outCollisionSerializeHandler;

            stat = LNCollisionSerializeHandler_Create(HSPSubclass_LNCollisionSerializeHandler_DelegateLabelCaller, &local_outCollisionSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionSerializeHandler*>(LNCollisionSerializeHandler_GetSubinstanceId(local_outCollisionSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outCollisionSerializeHandler, aptr_outCollisionSerializeHandler, local_outCollisionSerializeHandler);

            return true;
        }
        // LNTriggerBodyComponentSerializeHandler_Create
        case 0x865 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outTriggerBodyComponentSerializeHandler
            PVal* pval_outTriggerBodyComponentSerializeHandler;
            const APTR aptr_outTriggerBodyComponentSerializeHandler = code_getva(&pval_outTriggerBodyComponentSerializeHandler);
            LNHandle local_outTriggerBodyComponentSerializeHandler;

            stat = LNTriggerBodyComponentSerializeHandler_Create(HSPSubclass_LNTriggerBodyComponentSerializeHandler_DelegateLabelCaller, &local_outTriggerBodyComponentSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNTriggerBodyComponentSerializeHandler*>(LNTriggerBodyComponentSerializeHandler_GetSubinstanceId(local_outTriggerBodyComponentSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outTriggerBodyComponentSerializeHandler, aptr_outTriggerBodyComponentSerializeHandler, local_outTriggerBodyComponentSerializeHandler);

            return true;
        }
        // LNLevelSerializeHandler_Create
        case 0x870 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outLevelSerializeHandler
            PVal* pval_outLevelSerializeHandler;
            const APTR aptr_outLevelSerializeHandler = code_getva(&pval_outLevelSerializeHandler);
            LNHandle local_outLevelSerializeHandler;

            stat = LNLevelSerializeHandler_Create(HSPSubclass_LNLevelSerializeHandler_DelegateLabelCaller, &local_outLevelSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelSerializeHandler*>(LNLevelSerializeHandler_GetSubinstanceId(local_outLevelSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outLevelSerializeHandler, aptr_outLevelSerializeHandler, local_outLevelSerializeHandler);

            return true;
        }
        // LNLevelStartHandler_Create
        case 0x87B : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outLevelStartHandler
            PVal* pval_outLevelStartHandler;
            const APTR aptr_outLevelStartHandler = code_getva(&pval_outLevelStartHandler);
            LNHandle local_outLevelStartHandler;

            stat = LNLevelStartHandler_Create(HSPSubclass_LNLevelStartHandler_DelegateLabelCaller, &local_outLevelStartHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelStartHandler*>(LNLevelStartHandler_GetSubinstanceId(local_outLevelStartHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outLevelStartHandler, aptr_outLevelStartHandler, local_outLevelStartHandler);

            return true;
        }
        // LNLevelStopHandler_Create
        case 0x886 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outLevelStopHandler
            PVal* pval_outLevelStopHandler;
            const APTR aptr_outLevelStopHandler = code_getva(&pval_outLevelStopHandler);
            LNHandle local_outLevelStopHandler;

            stat = LNLevelStopHandler_Create(HSPSubclass_LNLevelStopHandler_DelegateLabelCaller, &local_outLevelStopHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelStopHandler*>(LNLevelStopHandler_GetSubinstanceId(local_outLevelStopHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outLevelStopHandler, aptr_outLevelStopHandler, local_outLevelStopHandler);

            return true;
        }
        // LNLevelPauseHandler_Create
        case 0x891 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outLevelPauseHandler
            PVal* pval_outLevelPauseHandler;
            const APTR aptr_outLevelPauseHandler = code_getva(&pval_outLevelPauseHandler);
            LNHandle local_outLevelPauseHandler;

            stat = LNLevelPauseHandler_Create(HSPSubclass_LNLevelPauseHandler_DelegateLabelCaller, &local_outLevelPauseHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelPauseHandler*>(LNLevelPauseHandler_GetSubinstanceId(local_outLevelPauseHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outLevelPauseHandler, aptr_outLevelPauseHandler, local_outLevelPauseHandler);

            return true;
        }
        // LNLevelResumeHandler_Create
        case 0x89C : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outLevelResumeHandler
            PVal* pval_outLevelResumeHandler;
            const APTR aptr_outLevelResumeHandler = code_getva(&pval_outLevelResumeHandler);
            LNHandle local_outLevelResumeHandler;

            stat = LNLevelResumeHandler_Create(HSPSubclass_LNLevelResumeHandler_DelegateLabelCaller, &local_outLevelResumeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelResumeHandler*>(LNLevelResumeHandler_GetSubinstanceId(local_outLevelResumeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outLevelResumeHandler, aptr_outLevelResumeHandler, local_outLevelResumeHandler);

            return true;
        }
        // LNLevelUpdateHandler_Create
        case 0x8A7 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outLevelUpdateHandler
            PVal* pval_outLevelUpdateHandler;
            const APTR aptr_outLevelUpdateHandler = code_getva(&pval_outLevelUpdateHandler);
            LNHandle local_outLevelUpdateHandler;

            stat = LNLevelUpdateHandler_Create(HSPSubclass_LNLevelUpdateHandler_DelegateLabelCaller, &local_outLevelUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelUpdateHandler*>(LNLevelUpdateHandler_GetSubinstanceId(local_outLevelUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outLevelUpdateHandler, aptr_outLevelUpdateHandler, local_outLevelUpdateHandler);

            return true;
        }
        // LNUIEventArgsSerializeHandler_Create
        case 0x8B2 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIEventArgsSerializeHandler
            PVal* pval_outUIEventArgsSerializeHandler;
            const APTR aptr_outUIEventArgsSerializeHandler = code_getva(&pval_outUIEventArgsSerializeHandler);
            LNHandle local_outUIEventArgsSerializeHandler;

            stat = LNUIEventArgsSerializeHandler_Create(HSPSubclass_LNUIEventArgsSerializeHandler_DelegateLabelCaller, &local_outUIEventArgsSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIEventArgsSerializeHandler*>(LNUIEventArgsSerializeHandler_GetSubinstanceId(local_outUIEventArgsSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIEventArgsSerializeHandler, aptr_outUIEventArgsSerializeHandler, local_outUIEventArgsSerializeHandler);

            return true;
        }
        // LNUILayoutElementSerializeHandler_Create
        case 0x8BD : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUILayoutElementSerializeHandler
            PVal* pval_outUILayoutElementSerializeHandler;
            const APTR aptr_outUILayoutElementSerializeHandler = code_getva(&pval_outUILayoutElementSerializeHandler);
            LNHandle local_outUILayoutElementSerializeHandler;

            stat = LNUILayoutElementSerializeHandler_Create(HSPSubclass_LNUILayoutElementSerializeHandler_DelegateLabelCaller, &local_outUILayoutElementSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUILayoutElementSerializeHandler*>(LNUILayoutElementSerializeHandler_GetSubinstanceId(local_outUILayoutElementSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUILayoutElementSerializeHandler, aptr_outUILayoutElementSerializeHandler, local_outUILayoutElementSerializeHandler);

            return true;
        }
        // LNUIElementSerializeHandler_Create
        case 0x8C8 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIElementSerializeHandler
            PVal* pval_outUIElementSerializeHandler;
            const APTR aptr_outUIElementSerializeHandler = code_getva(&pval_outUIElementSerializeHandler);
            LNHandle local_outUIElementSerializeHandler;

            stat = LNUIElementSerializeHandler_Create(HSPSubclass_LNUIElementSerializeHandler_DelegateLabelCaller, &local_outUIElementSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIElementSerializeHandler*>(LNUIElementSerializeHandler_GetSubinstanceId(local_outUIElementSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIElementSerializeHandler, aptr_outUIElementSerializeHandler, local_outUIElementSerializeHandler);

            return true;
        }
        // LNUITextBlockSerializeHandler_Create
        case 0x8D3 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUITextBlockSerializeHandler
            PVal* pval_outUITextBlockSerializeHandler;
            const APTR aptr_outUITextBlockSerializeHandler = code_getva(&pval_outUITextBlockSerializeHandler);
            LNHandle local_outUITextBlockSerializeHandler;

            stat = LNUITextBlockSerializeHandler_Create(HSPSubclass_LNUITextBlockSerializeHandler_DelegateLabelCaller, &local_outUITextBlockSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUITextBlockSerializeHandler*>(LNUITextBlockSerializeHandler_GetSubinstanceId(local_outUITextBlockSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUITextBlockSerializeHandler, aptr_outUITextBlockSerializeHandler, local_outUITextBlockSerializeHandler);

            return true;
        }
        // LNUISpriteSerializeHandler_Create
        case 0x8DE : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUISpriteSerializeHandler
            PVal* pval_outUISpriteSerializeHandler;
            const APTR aptr_outUISpriteSerializeHandler = code_getva(&pval_outUISpriteSerializeHandler);
            LNHandle local_outUISpriteSerializeHandler;

            stat = LNUISpriteSerializeHandler_Create(HSPSubclass_LNUISpriteSerializeHandler_DelegateLabelCaller, &local_outUISpriteSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUISpriteSerializeHandler*>(LNUISpriteSerializeHandler_GetSubinstanceId(local_outUISpriteSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUISpriteSerializeHandler, aptr_outUISpriteSerializeHandler, local_outUISpriteSerializeHandler);

            return true;
        }
        // LNUIIconSerializeHandler_Create
        case 0x8E9 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIIconSerializeHandler
            PVal* pval_outUIIconSerializeHandler;
            const APTR aptr_outUIIconSerializeHandler = code_getva(&pval_outUIIconSerializeHandler);
            LNHandle local_outUIIconSerializeHandler;

            stat = LNUIIconSerializeHandler_Create(HSPSubclass_LNUIIconSerializeHandler_DelegateLabelCaller, &local_outUIIconSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIIconSerializeHandler*>(LNUIIconSerializeHandler_GetSubinstanceId(local_outUIIconSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIIconSerializeHandler, aptr_outUIIconSerializeHandler, local_outUIIconSerializeHandler);

            return true;
        }
        // LNUILayoutPanelSerializeHandler_Create
        case 0x8F4 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUILayoutPanelSerializeHandler
            PVal* pval_outUILayoutPanelSerializeHandler;
            const APTR aptr_outUILayoutPanelSerializeHandler = code_getva(&pval_outUILayoutPanelSerializeHandler);
            LNHandle local_outUILayoutPanelSerializeHandler;

            stat = LNUILayoutPanelSerializeHandler_Create(HSPSubclass_LNUILayoutPanelSerializeHandler_DelegateLabelCaller, &local_outUILayoutPanelSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUILayoutPanelSerializeHandler*>(LNUILayoutPanelSerializeHandler_GetSubinstanceId(local_outUILayoutPanelSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUILayoutPanelSerializeHandler, aptr_outUILayoutPanelSerializeHandler, local_outUILayoutPanelSerializeHandler);

            return true;
        }
        // LNUIBoxLayoutSerializeHandler_Create
        case 0x8FF : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIBoxLayoutSerializeHandler
            PVal* pval_outUIBoxLayoutSerializeHandler;
            const APTR aptr_outUIBoxLayoutSerializeHandler = code_getva(&pval_outUIBoxLayoutSerializeHandler);
            LNHandle local_outUIBoxLayoutSerializeHandler;

            stat = LNUIBoxLayoutSerializeHandler_Create(HSPSubclass_LNUIBoxLayoutSerializeHandler_DelegateLabelCaller, &local_outUIBoxLayoutSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIBoxLayoutSerializeHandler*>(LNUIBoxLayoutSerializeHandler_GetSubinstanceId(local_outUIBoxLayoutSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIBoxLayoutSerializeHandler, aptr_outUIBoxLayoutSerializeHandler, local_outUIBoxLayoutSerializeHandler);

            return true;
        }
        // LNUIGridLayoutSerializeHandler_Create
        case 0x90A : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIGridLayoutSerializeHandler
            PVal* pval_outUIGridLayoutSerializeHandler;
            const APTR aptr_outUIGridLayoutSerializeHandler = code_getva(&pval_outUIGridLayoutSerializeHandler);
            LNHandle local_outUIGridLayoutSerializeHandler;

            stat = LNUIGridLayoutSerializeHandler_Create(HSPSubclass_LNUIGridLayoutSerializeHandler_DelegateLabelCaller, &local_outUIGridLayoutSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIGridLayoutSerializeHandler*>(LNUIGridLayoutSerializeHandler_GetSubinstanceId(local_outUIGridLayoutSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIGridLayoutSerializeHandler, aptr_outUIGridLayoutSerializeHandler, local_outUIGridLayoutSerializeHandler);

            return true;
        }
        // LNUIControlSerializeHandler_Create
        case 0x915 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIControlSerializeHandler
            PVal* pval_outUIControlSerializeHandler;
            const APTR aptr_outUIControlSerializeHandler = code_getva(&pval_outUIControlSerializeHandler);
            LNHandle local_outUIControlSerializeHandler;

            stat = LNUIControlSerializeHandler_Create(HSPSubclass_LNUIControlSerializeHandler_DelegateLabelCaller, &local_outUIControlSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIControlSerializeHandler*>(LNUIControlSerializeHandler_GetSubinstanceId(local_outUIControlSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIControlSerializeHandler, aptr_outUIControlSerializeHandler, local_outUIControlSerializeHandler);

            return true;
        }
        // LNUIButtonBaseSerializeHandler_Create
        case 0x920 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIButtonBaseSerializeHandler
            PVal* pval_outUIButtonBaseSerializeHandler;
            const APTR aptr_outUIButtonBaseSerializeHandler = code_getva(&pval_outUIButtonBaseSerializeHandler);
            LNHandle local_outUIButtonBaseSerializeHandler;

            stat = LNUIButtonBaseSerializeHandler_Create(HSPSubclass_LNUIButtonBaseSerializeHandler_DelegateLabelCaller, &local_outUIButtonBaseSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIButtonBaseSerializeHandler*>(LNUIButtonBaseSerializeHandler_GetSubinstanceId(local_outUIButtonBaseSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIButtonBaseSerializeHandler, aptr_outUIButtonBaseSerializeHandler, local_outUIButtonBaseSerializeHandler);

            return true;
        }
        // LNUIButtonSerializeHandler_Create
        case 0x92B : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIButtonSerializeHandler
            PVal* pval_outUIButtonSerializeHandler;
            const APTR aptr_outUIButtonSerializeHandler = code_getva(&pval_outUIButtonSerializeHandler);
            LNHandle local_outUIButtonSerializeHandler;

            stat = LNUIButtonSerializeHandler_Create(HSPSubclass_LNUIButtonSerializeHandler_DelegateLabelCaller, &local_outUIButtonSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIButtonSerializeHandler*>(LNUIButtonSerializeHandler_GetSubinstanceId(local_outUIButtonSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIButtonSerializeHandler, aptr_outUIButtonSerializeHandler, local_outUIButtonSerializeHandler);

            return true;
        }
        // LNUIWindowSerializeHandler_Create
        case 0x936 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIWindowSerializeHandler
            PVal* pval_outUIWindowSerializeHandler;
            const APTR aptr_outUIWindowSerializeHandler = code_getva(&pval_outUIWindowSerializeHandler);
            LNHandle local_outUIWindowSerializeHandler;

            stat = LNUIWindowSerializeHandler_Create(HSPSubclass_LNUIWindowSerializeHandler_DelegateLabelCaller, &local_outUIWindowSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIWindowSerializeHandler*>(LNUIWindowSerializeHandler_GetSubinstanceId(local_outUIWindowSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIWindowSerializeHandler, aptr_outUIWindowSerializeHandler, local_outUIWindowSerializeHandler);

            return true;
        }
        // LNUIListItemSerializeHandler_Create
        case 0x941 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIListItemSerializeHandler
            PVal* pval_outUIListItemSerializeHandler;
            const APTR aptr_outUIListItemSerializeHandler = code_getva(&pval_outUIListItemSerializeHandler);
            LNHandle local_outUIListItemSerializeHandler;

            stat = LNUIListItemSerializeHandler_Create(HSPSubclass_LNUIListItemSerializeHandler_DelegateLabelCaller, &local_outUIListItemSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListItemSerializeHandler*>(LNUIListItemSerializeHandler_GetSubinstanceId(local_outUIListItemSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIListItemSerializeHandler, aptr_outUIListItemSerializeHandler, local_outUIListItemSerializeHandler);

            return true;
        }
        // LNUIListItemsControlSerializeHandler_Create
        case 0x94C : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIListItemsControlSerializeHandler
            PVal* pval_outUIListItemsControlSerializeHandler;
            const APTR aptr_outUIListItemsControlSerializeHandler = code_getva(&pval_outUIListItemsControlSerializeHandler);
            LNHandle local_outUIListItemsControlSerializeHandler;

            stat = LNUIListItemsControlSerializeHandler_Create(HSPSubclass_LNUIListItemsControlSerializeHandler_DelegateLabelCaller, &local_outUIListItemsControlSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListItemsControlSerializeHandler*>(LNUIListItemsControlSerializeHandler_GetSubinstanceId(local_outUIListItemsControlSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIListItemsControlSerializeHandler, aptr_outUIListItemsControlSerializeHandler, local_outUIListItemsControlSerializeHandler);

            return true;
        }
        // LNUIListBoxItemSerializeHandler_Create
        case 0x957 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIListBoxItemSerializeHandler
            PVal* pval_outUIListBoxItemSerializeHandler;
            const APTR aptr_outUIListBoxItemSerializeHandler = code_getva(&pval_outUIListBoxItemSerializeHandler);
            LNHandle local_outUIListBoxItemSerializeHandler;

            stat = LNUIListBoxItemSerializeHandler_Create(HSPSubclass_LNUIListBoxItemSerializeHandler_DelegateLabelCaller, &local_outUIListBoxItemSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListBoxItemSerializeHandler*>(LNUIListBoxItemSerializeHandler_GetSubinstanceId(local_outUIListBoxItemSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIListBoxItemSerializeHandler, aptr_outUIListBoxItemSerializeHandler, local_outUIListBoxItemSerializeHandler);

            return true;
        }
        // LNUIListBoxSerializeHandler_Create
        case 0x962 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIListBoxSerializeHandler
            PVal* pval_outUIListBoxSerializeHandler;
            const APTR aptr_outUIListBoxSerializeHandler = code_getva(&pval_outUIListBoxSerializeHandler);
            LNHandle local_outUIListBoxSerializeHandler;

            stat = LNUIListBoxSerializeHandler_Create(HSPSubclass_LNUIListBoxSerializeHandler_DelegateLabelCaller, &local_outUIListBoxSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListBoxSerializeHandler*>(LNUIListBoxSerializeHandler_GetSubinstanceId(local_outUIListBoxSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIListBoxSerializeHandler, aptr_outUIListBoxSerializeHandler, local_outUIListBoxSerializeHandler);

            return true;
        }
        // LNInterpreterCommandSerializeHandler_Create
        case 0x96D : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outInterpreterCommandSerializeHandler
            PVal* pval_outInterpreterCommandSerializeHandler;
            const APTR aptr_outInterpreterCommandSerializeHandler = code_getva(&pval_outInterpreterCommandSerializeHandler);
            LNHandle local_outInterpreterCommandSerializeHandler;

            stat = LNInterpreterCommandSerializeHandler_Create(HSPSubclass_LNInterpreterCommandSerializeHandler_DelegateLabelCaller, &local_outInterpreterCommandSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterCommandSerializeHandler*>(LNInterpreterCommandSerializeHandler_GetSubinstanceId(local_outInterpreterCommandSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outInterpreterCommandSerializeHandler, aptr_outInterpreterCommandSerializeHandler, local_outInterpreterCommandSerializeHandler);

            return true;
        }
        // LNInterpreterCommandListSerializeHandler_Create
        case 0x978 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outInterpreterCommandListSerializeHandler
            PVal* pval_outInterpreterCommandListSerializeHandler;
            const APTR aptr_outInterpreterCommandListSerializeHandler = code_getva(&pval_outInterpreterCommandListSerializeHandler);
            LNHandle local_outInterpreterCommandListSerializeHandler;

            stat = LNInterpreterCommandListSerializeHandler_Create(HSPSubclass_LNInterpreterCommandListSerializeHandler_DelegateLabelCaller, &local_outInterpreterCommandListSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterCommandListSerializeHandler*>(LNInterpreterCommandListSerializeHandler_GetSubinstanceId(local_outInterpreterCommandListSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outInterpreterCommandListSerializeHandler, aptr_outInterpreterCommandListSerializeHandler, local_outInterpreterCommandListSerializeHandler);

            return true;
        }
        // LNInterpreterSerializeHandler_Create
        case 0x983 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outInterpreterSerializeHandler
            PVal* pval_outInterpreterSerializeHandler;
            const APTR aptr_outInterpreterSerializeHandler = code_getva(&pval_outInterpreterSerializeHandler);
            LNHandle local_outInterpreterSerializeHandler;

            stat = LNInterpreterSerializeHandler_Create(HSPSubclass_LNInterpreterSerializeHandler_DelegateLabelCaller, &local_outInterpreterSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterSerializeHandler*>(LNInterpreterSerializeHandler_GetSubinstanceId(local_outInterpreterSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outInterpreterSerializeHandler, aptr_outInterpreterSerializeHandler, local_outInterpreterSerializeHandler);

            return true;
        }
        // LNInterpreterUpdateWaitHandler_Create
        case 0x98F : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outInterpreterUpdateWaitHandler
            PVal* pval_outInterpreterUpdateWaitHandler;
            const APTR aptr_outInterpreterUpdateWaitHandler = code_getva(&pval_outInterpreterUpdateWaitHandler);
            LNHandle local_outInterpreterUpdateWaitHandler;

            stat = LNInterpreterUpdateWaitHandler_Create(HSPSubclass_LNInterpreterUpdateWaitHandler_DelegateLabelCaller, &local_outInterpreterUpdateWaitHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterUpdateWaitHandler*>(LNInterpreterUpdateWaitHandler_GetSubinstanceId(local_outInterpreterUpdateWaitHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outInterpreterUpdateWaitHandler, aptr_outInterpreterUpdateWaitHandler, local_outInterpreterUpdateWaitHandler);

            return true;
        }
        // LNApplicationSerializeHandler_Create
        case 0x99A : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outApplicationSerializeHandler
            PVal* pval_outApplicationSerializeHandler;
            const APTR aptr_outApplicationSerializeHandler = code_getva(&pval_outApplicationSerializeHandler);
            LNHandle local_outApplicationSerializeHandler;

            stat = LNApplicationSerializeHandler_Create(HSPSubclass_LNApplicationSerializeHandler_DelegateLabelCaller, &local_outApplicationSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNApplicationSerializeHandler*>(LNApplicationSerializeHandler_GetSubinstanceId(local_outApplicationSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outApplicationSerializeHandler, aptr_outApplicationSerializeHandler, local_outApplicationSerializeHandler);

            return true;
        }
        // LNApplicationInitHandler_Create
        case 0x9A5 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outApplicationInitHandler
            PVal* pval_outApplicationInitHandler;
            const APTR aptr_outApplicationInitHandler = code_getva(&pval_outApplicationInitHandler);
            LNHandle local_outApplicationInitHandler;

            stat = LNApplicationInitHandler_Create(HSPSubclass_LNApplicationInitHandler_DelegateLabelCaller, &local_outApplicationInitHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNApplicationInitHandler*>(LNApplicationInitHandler_GetSubinstanceId(local_outApplicationInitHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outApplicationInitHandler, aptr_outApplicationInitHandler, local_outApplicationInitHandler);

            return true;
        }
        // LNApplicationUpdateHandler_Create
        case 0x9B0 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outApplicationUpdateHandler
            PVal* pval_outApplicationUpdateHandler;
            const APTR aptr_outApplicationUpdateHandler = code_getva(&pval_outApplicationUpdateHandler);
            LNHandle local_outApplicationUpdateHandler;

            stat = LNApplicationUpdateHandler_Create(HSPSubclass_LNApplicationUpdateHandler_DelegateLabelCaller, &local_outApplicationUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNApplicationUpdateHandler*>(LNApplicationUpdateHandler_GetSubinstanceId(local_outApplicationUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outApplicationUpdateHandler, aptr_outApplicationUpdateHandler, local_outApplicationUpdateHandler);

            return true;
        }
    }
    return false;
}

void RegisterTypes(HSP3TYPEINFO * info)
{
    registvar(-1, hspLNVector3_Init);
    registvar(-1, hspLNVector4_Init);
    registvar(-1, hspLNQuaternion_Init);
    registvar(-1, hspLNMatrix_Init);
    registvar(-1, hspLNColor_Init);
    registvar(-1, hspLNPoint_Init);
    registvar(-1, hspLNSize_Init);
    registvar(-1, hspLNRect_Init);
    registvar(-1, hspLNThickness_Init);
    {
        LNObject_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNObject_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNObject_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNObject_RegisterSubclassTypeInfo(&info);
    }
    {
        LNEventConnection_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNEventConnection_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNEventConnection_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNEventConnection_RegisterSubclassTypeInfo(&info);
    }
    {
        LNPromiseFailureDelegate_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNPromiseFailureDelegate_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNPromiseFailureDelegate_SubinstanceFree;
        LNPromiseFailureDelegate_RegisterSubclassTypeInfo(&info);
    }
    {
        LNZVTestDelegate1_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNZVTestDelegate1_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNZVTestDelegate1_SubinstanceFree;
        LNZVTestDelegate1_RegisterSubclassTypeInfo(&info);
    }
    {
        LNZVTestDelegate2_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNZVTestDelegate2_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNZVTestDelegate2_SubinstanceFree;
        LNZVTestDelegate2_RegisterSubclassTypeInfo(&info);
    }
    {
        LNZVTestDelegate3_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNZVTestDelegate3_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNZVTestDelegate3_SubinstanceFree;
        LNZVTestDelegate3_RegisterSubclassTypeInfo(&info);
    }
    {
        LNZVTestEventHandler1_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNZVTestEventHandler1_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNZVTestEventHandler1_SubinstanceFree;
        LNZVTestEventHandler1_RegisterSubclassTypeInfo(&info);
    }
    {
        LNZVTestEventHandler2_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNZVTestEventHandler2_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNZVTestEventHandler2_SubinstanceFree;
        LNZVTestEventHandler2_RegisterSubclassTypeInfo(&info);
    }
    {
        LNZVTestPromise1_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNZVTestPromise1_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNZVTestPromise1_SubinstanceFree;
        LNZVTestPromise1_RegisterSubclassTypeInfo(&info);
    }
    {
        LNZVTestPromise2_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNZVTestPromise2_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNZVTestPromise2_SubinstanceFree;
        LNZVTestPromise2_RegisterSubclassTypeInfo(&info);
    }
    {
        LNZVTestClass1_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNZVTestClass1_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNZVTestClass1_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNZVTestClass1_RegisterSubclassTypeInfo(&info);
    }
    {
        LNZVTestEventArgs1_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNZVTestEventArgs1_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNZVTestEventArgs1_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNZVTestEventArgs1_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSerializer2_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSerializer2_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSerializer2_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNSerializer2_RegisterSubclassTypeInfo(&info);
    }
    {
        LNAssetModel_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAssetModel_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAssetModel_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNAssetModel_RegisterSubclassTypeInfo(&info);
    }
    {
        LNTexture2DDelegate_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNTexture2DDelegate_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNTexture2DDelegate_SubinstanceFree;
        LNTexture2DDelegate_RegisterSubclassTypeInfo(&info);
    }
    {
        LNTexture2DPromise_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNTexture2DPromise_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNTexture2DPromise_SubinstanceFree;
        LNTexture2DPromise_RegisterSubclassTypeInfo(&info);
    }
    {
        LNTexture_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNTexture_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNTexture_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNTexture_RegisterSubclassTypeInfo(&info);
    }
    {
        LNTexture2D_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNTexture2D_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNTexture2D_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNTexture2D_RegisterSubclassTypeInfo(&info);
    }
    {
        LNRenderView_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNRenderView_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNRenderView_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNRenderView_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSkinnedMeshModel_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSkinnedMeshModel_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSkinnedMeshModel_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNSkinnedMeshModel_RegisterSubclassTypeInfo(&info);
    }
    {
        LNCollisionShape_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCollisionShape_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCollisionShape_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNCollisionShape_RegisterSubclassTypeInfo(&info);
    }
    {
        LNBoxCollisionShape_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNBoxCollisionShape_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNBoxCollisionShape_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNBoxCollisionShape_RegisterSubclassTypeInfo(&info);
    }
    {
        LNComponent_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNComponent_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNComponent_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNComponent_RegisterSubclassTypeInfo(&info);
    }
    {
        LNVisualComponent_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNVisualComponent_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNVisualComponent_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNVisualComponent_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSpriteComponent_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSpriteComponent_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSpriteComponent_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNSpriteComponent_RegisterSubclassTypeInfo(&info);
    }
    {
        LNCollisionEventHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCollisionEventHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCollisionEventHandler_SubinstanceFree;
        LNCollisionEventHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNCharacterController_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCharacterController_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCharacterController_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNCharacterController_RegisterSubclassTypeInfo(&info);
    }
    {
        LNWorld_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNWorld_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNWorld_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNWorld_RegisterSubclassTypeInfo(&info);
    }
    {
        LNComponentList_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNComponentList_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNComponentList_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNComponentList_RegisterSubclassTypeInfo(&info);
    }
    {
        LNWorldObject_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNWorldObject_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNWorldObject_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNWorldObject_RegisterSubclassTypeInfo(&info);
    }
    {
        LNVisualObject_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNVisualObject_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNVisualObject_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNVisualObject_RegisterSubclassTypeInfo(&info);
    }
    {
        LNCamera_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCamera_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCamera_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNCamera_RegisterSubclassTypeInfo(&info);
    }
    {
        LNEnvironmentLight_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNEnvironmentLight_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNEnvironmentLight_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNEnvironmentLight_RegisterSubclassTypeInfo(&info);
    }
    {
        LNDirectionalLight_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNDirectionalLight_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNDirectionalLight_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNDirectionalLight_RegisterSubclassTypeInfo(&info);
    }
    {
        LNPointLight_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNPointLight_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNPointLight_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNPointLight_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSpotLight_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSpotLight_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSpotLight_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNSpotLight_RegisterSubclassTypeInfo(&info);
    }
    {
        LNTestDelegate_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNTestDelegate_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNTestDelegate_SubinstanceFree;
        LNTestDelegate_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSprite_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSprite_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSprite_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNSprite_RegisterSubclassTypeInfo(&info);
    }
    {
        LNCameraOrbitControlComponent_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCameraOrbitControlComponent_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCameraOrbitControlComponent_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNCameraOrbitControlComponent_RegisterSubclassTypeInfo(&info);
    }
    {
        LNRaycaster_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNRaycaster_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNRaycaster_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNRaycaster_RegisterSubclassTypeInfo(&info);
    }
    {
        LNRaycastResult_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNRaycastResult_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNRaycastResult_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNRaycastResult_RegisterSubclassTypeInfo(&info);
    }
    {
        LNWorldRenderView_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNWorldRenderView_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNWorldRenderView_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNWorldRenderView_RegisterSubclassTypeInfo(&info);
    }
    {
        LNBoxMesh_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNBoxMesh_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNBoxMesh_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNBoxMesh_RegisterSubclassTypeInfo(&info);
    }
    {
        LNPlaneMesh_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNPlaneMesh_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNPlaneMesh_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNPlaneMesh_RegisterSubclassTypeInfo(&info);
    }
    {
        LNStaticMesh_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNStaticMesh_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNStaticMesh_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNStaticMesh_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSkinnedMeshComponent_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSkinnedMeshComponent_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSkinnedMeshComponent_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNSkinnedMeshComponent_RegisterSubclassTypeInfo(&info);
    }
    {
        LNCollision_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCollision_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCollision_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNCollision_RegisterSubclassTypeInfo(&info);
    }
    {
        LNTriggerBodyComponent_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNTriggerBodyComponent_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNTriggerBodyComponent_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNTriggerBodyComponent_RegisterSubclassTypeInfo(&info);
    }
    {
        LNLevel_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNLevel_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNLevel_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnStart_OverrideFunc = ????;
        //info.OnStop_OverrideFunc = ????;
        //info.OnPause_OverrideFunc = ????;
        //info.OnResume_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNLevel_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIEventArgs_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIEventArgs_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIEventArgs_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIEventArgs_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIGeneralEventHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIGeneralEventHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIGeneralEventHandler_SubinstanceFree;
        LNUIGeneralEventHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIEventHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIEventHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIEventHandler_SubinstanceFree;
        LNUIEventHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUILayoutElement_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUILayoutElement_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUILayoutElement_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUILayoutElement_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIElement_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIElement_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIElement_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIElement_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUITextBlock_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUITextBlock_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUITextBlock_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUITextBlock_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUISprite_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUISprite_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUISprite_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUISprite_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIIcon_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIIcon_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIIcon_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIIcon_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUILayoutPanel_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUILayoutPanel_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUILayoutPanel_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUILayoutPanel_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIBoxLayout_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIBoxLayout_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIBoxLayout_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIBoxLayout_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIGridLayout_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIGridLayout_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIGridLayout_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIGridLayout_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIControl_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIControl_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIControl_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIControl_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIButtonBase_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIButtonBase_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIButtonBase_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIButtonBase_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIButton_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIButton_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIButton_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIButton_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIWindow_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIWindow_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIWindow_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIWindow_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIListItem_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIListItem_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIListItem_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIListItem_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIListItemsControl_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIListItemsControl_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIListItemsControl_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIListItemsControl_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIListBoxItem_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIListBoxItem_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIListBoxItem_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIListBoxItem_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIListBox_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIListBox_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIListBox_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIListBox_RegisterSubclassTypeInfo(&info);
    }
    {
        LNInterpreterCommand_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNInterpreterCommand_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNInterpreterCommand_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNInterpreterCommand_RegisterSubclassTypeInfo(&info);
    }
    {
        LNInterpreterCommandList_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNInterpreterCommandList_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNInterpreterCommandList_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNInterpreterCommandList_RegisterSubclassTypeInfo(&info);
    }
    {
        LNInterpreterCommandDelegate_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNInterpreterCommandDelegate_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNInterpreterCommandDelegate_SubinstanceFree;
        LNInterpreterCommandDelegate_RegisterSubclassTypeInfo(&info);
    }
    {
        LNInterpreter_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNInterpreter_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNInterpreter_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnUpdateWait_OverrideFunc = ????;
        LNInterpreter_RegisterSubclassTypeInfo(&info);
    }
    {
        LNApplication_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNApplication_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNApplication_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnInit_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNApplication_RegisterSubclassTypeInfo(&info);
    }
    {
        LNObjectSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNObjectSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNObjectSerializeHandler_SubinstanceFree;
        LNObjectSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNEventConnectionSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNEventConnectionSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNEventConnectionSerializeHandler_SubinstanceFree;
        LNEventConnectionSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNZVTestClass1SerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNZVTestClass1SerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNZVTestClass1SerializeHandler_SubinstanceFree;
        LNZVTestClass1SerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNZVTestEventArgs1SerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNZVTestEventArgs1SerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNZVTestEventArgs1SerializeHandler_SubinstanceFree;
        LNZVTestEventArgs1SerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSerializer2SerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSerializer2SerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSerializer2SerializeHandler_SubinstanceFree;
        LNSerializer2SerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNAssetModelSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAssetModelSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAssetModelSerializeHandler_SubinstanceFree;
        LNAssetModelSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNTextureSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNTextureSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNTextureSerializeHandler_SubinstanceFree;
        LNTextureSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNTexture2DSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNTexture2DSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNTexture2DSerializeHandler_SubinstanceFree;
        LNTexture2DSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNRenderViewSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNRenderViewSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNRenderViewSerializeHandler_SubinstanceFree;
        LNRenderViewSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSkinnedMeshModelSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSkinnedMeshModelSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSkinnedMeshModelSerializeHandler_SubinstanceFree;
        LNSkinnedMeshModelSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNCollisionShapeSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCollisionShapeSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCollisionShapeSerializeHandler_SubinstanceFree;
        LNCollisionShapeSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNBoxCollisionShapeSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNBoxCollisionShapeSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNBoxCollisionShapeSerializeHandler_SubinstanceFree;
        LNBoxCollisionShapeSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNComponentSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNComponentSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNComponentSerializeHandler_SubinstanceFree;
        LNComponentSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNVisualComponentSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNVisualComponentSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNVisualComponentSerializeHandler_SubinstanceFree;
        LNVisualComponentSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSpriteComponentSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSpriteComponentSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSpriteComponentSerializeHandler_SubinstanceFree;
        LNSpriteComponentSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNCharacterControllerSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCharacterControllerSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCharacterControllerSerializeHandler_SubinstanceFree;
        LNCharacterControllerSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNWorldSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNWorldSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNWorldSerializeHandler_SubinstanceFree;
        LNWorldSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNComponentListSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNComponentListSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNComponentListSerializeHandler_SubinstanceFree;
        LNComponentListSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNWorldObjectSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNWorldObjectSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNWorldObjectSerializeHandler_SubinstanceFree;
        LNWorldObjectSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNWorldObjectUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNWorldObjectUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNWorldObjectUpdateHandler_SubinstanceFree;
        LNWorldObjectUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNVisualObjectSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNVisualObjectSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNVisualObjectSerializeHandler_SubinstanceFree;
        LNVisualObjectSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNVisualObjectUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNVisualObjectUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNVisualObjectUpdateHandler_SubinstanceFree;
        LNVisualObjectUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNCameraSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCameraSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCameraSerializeHandler_SubinstanceFree;
        LNCameraSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNCameraUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCameraUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCameraUpdateHandler_SubinstanceFree;
        LNCameraUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNEnvironmentLightSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNEnvironmentLightSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNEnvironmentLightSerializeHandler_SubinstanceFree;
        LNEnvironmentLightSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNEnvironmentLightUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNEnvironmentLightUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNEnvironmentLightUpdateHandler_SubinstanceFree;
        LNEnvironmentLightUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNDirectionalLightSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNDirectionalLightSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNDirectionalLightSerializeHandler_SubinstanceFree;
        LNDirectionalLightSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNDirectionalLightUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNDirectionalLightUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNDirectionalLightUpdateHandler_SubinstanceFree;
        LNDirectionalLightUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNPointLightSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNPointLightSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNPointLightSerializeHandler_SubinstanceFree;
        LNPointLightSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNPointLightUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNPointLightUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNPointLightUpdateHandler_SubinstanceFree;
        LNPointLightUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSpotLightSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSpotLightSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSpotLightSerializeHandler_SubinstanceFree;
        LNSpotLightSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSpotLightUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSpotLightUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSpotLightUpdateHandler_SubinstanceFree;
        LNSpotLightUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSpriteSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSpriteSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSpriteSerializeHandler_SubinstanceFree;
        LNSpriteSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSpriteUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSpriteUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSpriteUpdateHandler_SubinstanceFree;
        LNSpriteUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNCameraOrbitControlComponentSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCameraOrbitControlComponentSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCameraOrbitControlComponentSerializeHandler_SubinstanceFree;
        LNCameraOrbitControlComponentSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNRaycasterSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNRaycasterSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNRaycasterSerializeHandler_SubinstanceFree;
        LNRaycasterSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNRaycastResultSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNRaycastResultSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNRaycastResultSerializeHandler_SubinstanceFree;
        LNRaycastResultSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNWorldRenderViewSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNWorldRenderViewSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNWorldRenderViewSerializeHandler_SubinstanceFree;
        LNWorldRenderViewSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNBoxMeshSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNBoxMeshSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNBoxMeshSerializeHandler_SubinstanceFree;
        LNBoxMeshSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNBoxMeshUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNBoxMeshUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNBoxMeshUpdateHandler_SubinstanceFree;
        LNBoxMeshUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNPlaneMeshSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNPlaneMeshSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNPlaneMeshSerializeHandler_SubinstanceFree;
        LNPlaneMeshSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNPlaneMeshUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNPlaneMeshUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNPlaneMeshUpdateHandler_SubinstanceFree;
        LNPlaneMeshUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNStaticMeshSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNStaticMeshSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNStaticMeshSerializeHandler_SubinstanceFree;
        LNStaticMeshSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNStaticMeshUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNStaticMeshUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNStaticMeshUpdateHandler_SubinstanceFree;
        LNStaticMeshUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSkinnedMeshComponentSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSkinnedMeshComponentSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSkinnedMeshComponentSerializeHandler_SubinstanceFree;
        LNSkinnedMeshComponentSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNCollisionSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCollisionSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCollisionSerializeHandler_SubinstanceFree;
        LNCollisionSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNTriggerBodyComponentSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNTriggerBodyComponentSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNTriggerBodyComponentSerializeHandler_SubinstanceFree;
        LNTriggerBodyComponentSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNLevelSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNLevelSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNLevelSerializeHandler_SubinstanceFree;
        LNLevelSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNLevelStartHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNLevelStartHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNLevelStartHandler_SubinstanceFree;
        LNLevelStartHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNLevelStopHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNLevelStopHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNLevelStopHandler_SubinstanceFree;
        LNLevelStopHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNLevelPauseHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNLevelPauseHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNLevelPauseHandler_SubinstanceFree;
        LNLevelPauseHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNLevelResumeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNLevelResumeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNLevelResumeHandler_SubinstanceFree;
        LNLevelResumeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNLevelUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNLevelUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNLevelUpdateHandler_SubinstanceFree;
        LNLevelUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIEventArgsSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIEventArgsSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIEventArgsSerializeHandler_SubinstanceFree;
        LNUIEventArgsSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUILayoutElementSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUILayoutElementSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUILayoutElementSerializeHandler_SubinstanceFree;
        LNUILayoutElementSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIElementSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIElementSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIElementSerializeHandler_SubinstanceFree;
        LNUIElementSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUITextBlockSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUITextBlockSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUITextBlockSerializeHandler_SubinstanceFree;
        LNUITextBlockSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUISpriteSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUISpriteSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUISpriteSerializeHandler_SubinstanceFree;
        LNUISpriteSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIIconSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIIconSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIIconSerializeHandler_SubinstanceFree;
        LNUIIconSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUILayoutPanelSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUILayoutPanelSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUILayoutPanelSerializeHandler_SubinstanceFree;
        LNUILayoutPanelSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIBoxLayoutSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIBoxLayoutSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIBoxLayoutSerializeHandler_SubinstanceFree;
        LNUIBoxLayoutSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIGridLayoutSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIGridLayoutSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIGridLayoutSerializeHandler_SubinstanceFree;
        LNUIGridLayoutSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIControlSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIControlSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIControlSerializeHandler_SubinstanceFree;
        LNUIControlSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIButtonBaseSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIButtonBaseSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIButtonBaseSerializeHandler_SubinstanceFree;
        LNUIButtonBaseSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIButtonSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIButtonSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIButtonSerializeHandler_SubinstanceFree;
        LNUIButtonSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIWindowSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIWindowSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIWindowSerializeHandler_SubinstanceFree;
        LNUIWindowSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIListItemSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIListItemSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIListItemSerializeHandler_SubinstanceFree;
        LNUIListItemSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIListItemsControlSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIListItemsControlSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIListItemsControlSerializeHandler_SubinstanceFree;
        LNUIListItemsControlSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIListBoxItemSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIListBoxItemSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIListBoxItemSerializeHandler_SubinstanceFree;
        LNUIListBoxItemSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNUIListBoxSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIListBoxSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIListBoxSerializeHandler_SubinstanceFree;
        LNUIListBoxSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNInterpreterCommandSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNInterpreterCommandSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNInterpreterCommandSerializeHandler_SubinstanceFree;
        LNInterpreterCommandSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNInterpreterCommandListSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNInterpreterCommandListSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNInterpreterCommandListSerializeHandler_SubinstanceFree;
        LNInterpreterCommandListSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNInterpreterSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNInterpreterSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNInterpreterSerializeHandler_SubinstanceFree;
        LNInterpreterSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNInterpreterUpdateWaitHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNInterpreterUpdateWaitHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNInterpreterUpdateWaitHandler_SubinstanceFree;
        LNInterpreterUpdateWaitHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNApplicationSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNApplicationSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNApplicationSerializeHandler_SubinstanceFree;
        LNApplicationSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNApplicationInitHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNApplicationInitHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNApplicationInitHandler_SubinstanceFree;
        LNApplicationInitHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNApplicationUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNApplicationUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNApplicationUpdateHandler_SubinstanceFree;
        LNApplicationUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
}

