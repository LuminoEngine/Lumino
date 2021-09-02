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
// struct LNColorTone

static int g_LNColorTone_typeid = 0;

int hspLNColorTone_typeid()
{
    return g_LNColorTone_typeid;
}

static int hspLNColorTone_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LNColorTone);
    return size;
}

static void hspLNColorTone_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLNColorTone_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LNColorTone* fv = (LNColorTone *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLNColorTone_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLNColorTone_GetPtr(PVal* pval)
{
    return (PDAT*)(((LNColorTone*)(pval->pt)) + pval->offset);
}

static int hspLNColorTone_GetSize(const PDAT *pdatl)
{
    return sizeof(LNColorTone);
}

static void hspLNColorTone_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LNColorTone*>(pdat) = *reinterpret_cast<const LNColorTone*>(in);
}

static void hspLNColorTone_Init(HspVarProc* p)
{
    p->Alloc = hspLNColorTone_Alloc;
    p->Free = hspLNColorTone_Free;
    
    p->GetPtr = hspLNColorTone_GetPtr;
    p->GetSize = hspLNColorTone_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLNColorTone_Set;

    p->vartype_name = "LNColorTone";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LNColorTone);
    g_LNColorTone_typeid = p->flag;
}

static void hspLNColorTone_reffunc(int* typeRes, void** retValPtr)
{
    static LNColorTone returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.r = fetchVADouble_reffunc();
        returnValue.g = fetchVADouble_reffunc();
        returnValue.b = fetchVADouble_reffunc();
        returnValue.s = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLNColorTone_typeid();
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


//-----------------------------------------------------------------------------
// struct LNCornerRadius

static int g_LNCornerRadius_typeid = 0;

int hspLNCornerRadius_typeid()
{
    return g_LNCornerRadius_typeid;
}

static int hspLNCornerRadius_GetVarSize(PVal *pval)
{
    int size;
    size = pval->len[1];
    if (pval->len[2]) size *= pval->len[2];
    if (pval->len[3]) size *= pval->len[3];
    if (pval->len[4]) size *= pval->len[4];
    size *= sizeof(LNCornerRadius);
    return size;
}

static void hspLNCornerRadius_Alloc(PVal *pval, const PVal *pval2)
{
    if (pval->len[1] < 1) pval->len[1] = 1;
    int size = hspLNCornerRadius_GetVarSize(pval);
    pval->mode = HSPVAR_MODE_MALLOC;
    char* pt = sbAlloc(size);
    LNCornerRadius* fv = (LNCornerRadius *)pt;
    memset(fv, 0, size);
    if (pval2 != NULL) {
        memcpy(pt, pval->pt, pval->size);
        sbFree(pval->pt);
    }
    pval->pt = pt;
    pval->size = size;
}

static void hspLNCornerRadius_Free(PVal* pval)
{
    if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
    pval->pt = NULL;
    pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLNCornerRadius_GetPtr(PVal* pval)
{
    return (PDAT*)(((LNCornerRadius*)(pval->pt)) + pval->offset);
}

static int hspLNCornerRadius_GetSize(const PDAT *pdatl)
{
    return sizeof(LNCornerRadius);
}

static void hspLNCornerRadius_Set(PVal* pval, PDAT* pdat, const void* in)
{
    *reinterpret_cast<LNCornerRadius*>(pdat) = *reinterpret_cast<const LNCornerRadius*>(in);
}

static void hspLNCornerRadius_Init(HspVarProc* p)
{
    p->Alloc = hspLNCornerRadius_Alloc;
    p->Free = hspLNCornerRadius_Free;
    
    p->GetPtr = hspLNCornerRadius_GetPtr;
    p->GetSize = hspLNCornerRadius_GetSize;
    p->GetBlockSize = hspCommon_GetBlockSize;
    p->AllocBlock = hspCommon_AllocBlock;

    p->Set = hspLNCornerRadius_Set;

    p->vartype_name = "LNCornerRadius";
    p->version = 0x001;
    p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
    p->basesize = sizeof(LNCornerRadius);
    g_LNCornerRadius_typeid = p->flag;
}

static void hspLNCornerRadius_reffunc(int* typeRes, void** retValPtr)
{
    static LNCornerRadius returnValue;

    if (checkAndFetchDefault()) {
        memset(&returnValue, 0, sizeof(returnValue));
    } else {
        returnValue.topLeft = fetchVADouble_reffunc();
        returnValue.topRight = fetchVADouble_reffunc();
        returnValue.bottomRight = fetchVADouble_reffunc();
        returnValue.bottomLeft = fetchVADouble_reffunc();
    }


    *retValPtr = &returnValue;
    *typeRes = hspLNCornerRadius_typeid();
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
// ln::Variant

struct HSPSubclass_LNVariant
{
};


static LNSubinstanceId HSPSubclass_LNVariant_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNVariant)));
}

static void HSPSubclass_LNVariant_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
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
// ln::Log

struct HSPSubclass_LNLog
{
};


static LNSubinstanceId HSPSubclass_LNLog_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNLog)));
}

static void HSPSubclass_LNLog_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::AssetObject

struct HSPSubclass_LNAssetObject
{
};


static LNSubinstanceId HSPSubclass_LNAssetObject_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAssetObject)));
}

static void HSPSubclass_LNAssetObject_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::AssetImportSettings

struct HSPSubclass_LNAssetImportSettings
{
};


static LNSubinstanceId HSPSubclass_LNAssetImportSettings_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAssetImportSettings)));
}

static void HSPSubclass_LNAssetImportSettings_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::Sound

struct HSPSubclass_LNSound
{
};


static LNSubinstanceId HSPSubclass_LNSound_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSound)));
}

static void HSPSubclass_LNSound_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::Audio

struct HSPSubclass_LNAudio
{
};


static LNSubinstanceId HSPSubclass_LNAudio_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAudio)));
}

static void HSPSubclass_LNAudio_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::Graphics

struct HSPSubclass_LNGraphics
{
};


static LNSubinstanceId HSPSubclass_LNGraphics_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNGraphics)));
}

static void HSPSubclass_LNGraphics_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::Shader

struct HSPSubclass_LNShader
{
};


static LNSubinstanceId HSPSubclass_LNShader_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNShader)));
}

static void HSPSubclass_LNShader_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::Material

struct HSPSubclass_LNMaterial
{
};


static LNSubinstanceId HSPSubclass_LNMaterial_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNMaterial)));
}

static void HSPSubclass_LNMaterial_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::MeshNode

struct HSPSubclass_LNMeshNode
{
};


static LNSubinstanceId HSPSubclass_LNMeshNode_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNMeshNode)));
}

static void HSPSubclass_LNMeshNode_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::AnimationController

struct HSPSubclass_LNAnimationController
{
};


static LNSubinstanceId HSPSubclass_LNAnimationController_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAnimationController)));
}

static void HSPSubclass_LNAnimationController_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::MeshModel

struct HSPSubclass_LNMeshModel
{
};


static LNSubinstanceId HSPSubclass_LNMeshModel_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNMeshModel)));
}

static void HSPSubclass_LNMeshModel_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::MeshImportSettings

struct HSPSubclass_LNMeshImportSettings
{
};


static LNSubinstanceId HSPSubclass_LNMeshImportSettings_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNMeshImportSettings)));
}

static void HSPSubclass_LNMeshImportSettings_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::AnimationCurve

struct HSPSubclass_LNAnimationCurve
{
};


static LNSubinstanceId HSPSubclass_LNAnimationCurve_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAnimationCurve)));
}

static void HSPSubclass_LNAnimationCurve_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::KeyFrameAnimationCurve

struct HSPSubclass_LNKeyFrameAnimationCurve
{
};


static LNSubinstanceId HSPSubclass_LNKeyFrameAnimationCurve_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNKeyFrameAnimationCurve)));
}

static void HSPSubclass_LNKeyFrameAnimationCurve_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::AnimationClip

struct HSPSubclass_LNAnimationClip
{
};


static LNSubinstanceId HSPSubclass_LNAnimationClip_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAnimationClip)));
}

static void HSPSubclass_LNAnimationClip_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::AnimationState

struct HSPSubclass_LNAnimationState
{
};


static LNSubinstanceId HSPSubclass_LNAnimationState_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAnimationState)));
}

static void HSPSubclass_LNAnimationState_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::EffectResource

struct HSPSubclass_LNEffectResource
{
};


static LNSubinstanceId HSPSubclass_LNEffectResource_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNEffectResource)));
}

static void HSPSubclass_LNEffectResource_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::ParticleEmitterModel

struct HSPSubclass_LNParticleEmitterModel
{
};


static LNSubinstanceId HSPSubclass_LNParticleEmitterModel_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNParticleEmitterModel)));
}

static void HSPSubclass_LNParticleEmitterModel_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::ParticleModel

struct HSPSubclass_LNParticleModel
{
};


static LNSubinstanceId HSPSubclass_LNParticleModel_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNParticleModel)));
}

static void HSPSubclass_LNParticleModel_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::ShapeObject

struct HSPSubclass_LNShapeObject
{
};


static LNSubinstanceId HSPSubclass_LNShapeObject_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNShapeObject)));
}

static void HSPSubclass_LNShapeObject_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::MeshComponent

struct HSPSubclass_LNMeshComponent
{
};


static LNSubinstanceId HSPSubclass_LNMeshComponent_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNMeshComponent)));
}

static void HSPSubclass_LNMeshComponent_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::ParticleEmitter

struct HSPSubclass_LNParticleEmitter
{
};


static LNSubinstanceId HSPSubclass_LNParticleEmitter_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNParticleEmitter)));
}

static void HSPSubclass_LNParticleEmitter_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::UIColors

struct HSPSubclass_LNUIColors
{
};


static LNSubinstanceId HSPSubclass_LNUIColors_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIColors)));
}

static void HSPSubclass_LNUIColors_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::UIText

struct HSPSubclass_LNUIText
{
};


static LNSubinstanceId HSPSubclass_LNUIText_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIText)));
}

static void HSPSubclass_LNUIText_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::UIMessageTextArea

struct HSPSubclass_LNUIMessageTextArea
{
};


static LNSubinstanceId HSPSubclass_LNUIMessageTextArea_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIMessageTextArea)));
}

static void HSPSubclass_LNUIMessageTextArea_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::UIStackLayout

struct HSPSubclass_LNUIStackLayout
{
};


static LNSubinstanceId HSPSubclass_LNUIStackLayout_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIStackLayout)));
}

static void HSPSubclass_LNUIStackLayout_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// ln::InputGesture

struct HSPSubclass_LNInputGesture
{
};


static LNSubinstanceId HSPSubclass_LNInputGesture_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNInputGesture)));
}

static void HSPSubclass_LNInputGesture_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


//==============================================================================
// ln::KeyGesture

struct HSPSubclass_LNKeyGesture
{
};


static LNSubinstanceId HSPSubclass_LNKeyGesture_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNKeyGesture)));
}

static void HSPSubclass_LNKeyGesture_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
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
// VariantSerializeHandler

struct HSPSubclass_LNVariantSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNVariantSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNVariantSerializeHandler)));
}

static void HSPSubclass_LNVariantSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNVariantSerializeHandler_DelegateLabelCaller(LNHandle variantserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, variantserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNVariantSerializeHandler*>(LNVariantSerializeHandler_GetSubinstanceId(variantserializehandler));
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
// AssetObjectSerializeHandler

struct HSPSubclass_LNAssetObjectSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNAssetObjectSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAssetObjectSerializeHandler)));
}

static void HSPSubclass_LNAssetObjectSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNAssetObjectSerializeHandler_DelegateLabelCaller(LNHandle assetobjectserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, assetobjectserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNAssetObjectSerializeHandler*>(LNAssetObjectSerializeHandler_GetSubinstanceId(assetobjectserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// AssetImportSettingsSerializeHandler

struct HSPSubclass_LNAssetImportSettingsSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNAssetImportSettingsSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAssetImportSettingsSerializeHandler)));
}

static void HSPSubclass_LNAssetImportSettingsSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNAssetImportSettingsSerializeHandler_DelegateLabelCaller(LNHandle assetimportsettingsserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, assetimportsettingsserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNAssetImportSettingsSerializeHandler*>(LNAssetImportSettingsSerializeHandler_GetSubinstanceId(assetimportsettingsserializehandler));
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
// SoundSerializeHandler

struct HSPSubclass_LNSoundSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNSoundSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSoundSerializeHandler)));
}

static void HSPSubclass_LNSoundSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNSoundSerializeHandler_DelegateLabelCaller(LNHandle soundserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, soundserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNSoundSerializeHandler*>(LNSoundSerializeHandler_GetSubinstanceId(soundserializehandler));
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
// ShaderSerializeHandler

struct HSPSubclass_LNShaderSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNShaderSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNShaderSerializeHandler)));
}

static void HSPSubclass_LNShaderSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNShaderSerializeHandler_DelegateLabelCaller(LNHandle shaderserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, shaderserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNShaderSerializeHandler*>(LNShaderSerializeHandler_GetSubinstanceId(shaderserializehandler));
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
// MaterialSerializeHandler

struct HSPSubclass_LNMaterialSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNMaterialSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNMaterialSerializeHandler)));
}

static void HSPSubclass_LNMaterialSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNMaterialSerializeHandler_DelegateLabelCaller(LNHandle materialserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, materialserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNMaterialSerializeHandler*>(LNMaterialSerializeHandler_GetSubinstanceId(materialserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// MeshNodeSerializeHandler

struct HSPSubclass_LNMeshNodeSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNMeshNodeSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNMeshNodeSerializeHandler)));
}

static void HSPSubclass_LNMeshNodeSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNMeshNodeSerializeHandler_DelegateLabelCaller(LNHandle meshnodeserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, meshnodeserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNMeshNodeSerializeHandler*>(LNMeshNodeSerializeHandler_GetSubinstanceId(meshnodeserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// AnimationControllerSerializeHandler

struct HSPSubclass_LNAnimationControllerSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNAnimationControllerSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAnimationControllerSerializeHandler)));
}

static void HSPSubclass_LNAnimationControllerSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNAnimationControllerSerializeHandler_DelegateLabelCaller(LNHandle animationcontrollerserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, animationcontrollerserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNAnimationControllerSerializeHandler*>(LNAnimationControllerSerializeHandler_GetSubinstanceId(animationcontrollerserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// MeshModelSerializeHandler

struct HSPSubclass_LNMeshModelSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNMeshModelSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNMeshModelSerializeHandler)));
}

static void HSPSubclass_LNMeshModelSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNMeshModelSerializeHandler_DelegateLabelCaller(LNHandle meshmodelserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, meshmodelserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNMeshModelSerializeHandler*>(LNMeshModelSerializeHandler_GetSubinstanceId(meshmodelserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// MeshImportSettingsSerializeHandler

struct HSPSubclass_LNMeshImportSettingsSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNMeshImportSettingsSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNMeshImportSettingsSerializeHandler)));
}

static void HSPSubclass_LNMeshImportSettingsSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNMeshImportSettingsSerializeHandler_DelegateLabelCaller(LNHandle meshimportsettingsserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, meshimportsettingsserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNMeshImportSettingsSerializeHandler*>(LNMeshImportSettingsSerializeHandler_GetSubinstanceId(meshimportsettingsserializehandler));
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
// AnimationCurveSerializeHandler

struct HSPSubclass_LNAnimationCurveSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNAnimationCurveSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAnimationCurveSerializeHandler)));
}

static void HSPSubclass_LNAnimationCurveSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNAnimationCurveSerializeHandler_DelegateLabelCaller(LNHandle animationcurveserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, animationcurveserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNAnimationCurveSerializeHandler*>(LNAnimationCurveSerializeHandler_GetSubinstanceId(animationcurveserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// KeyFrameAnimationCurveSerializeHandler

struct HSPSubclass_LNKeyFrameAnimationCurveSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNKeyFrameAnimationCurveSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNKeyFrameAnimationCurveSerializeHandler)));
}

static void HSPSubclass_LNKeyFrameAnimationCurveSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNKeyFrameAnimationCurveSerializeHandler_DelegateLabelCaller(LNHandle keyframeanimationcurveserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, keyframeanimationcurveserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNKeyFrameAnimationCurveSerializeHandler*>(LNKeyFrameAnimationCurveSerializeHandler_GetSubinstanceId(keyframeanimationcurveserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// AnimationClipSerializeHandler

struct HSPSubclass_LNAnimationClipSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNAnimationClipSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAnimationClipSerializeHandler)));
}

static void HSPSubclass_LNAnimationClipSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNAnimationClipSerializeHandler_DelegateLabelCaller(LNHandle animationclipserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, animationclipserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNAnimationClipSerializeHandler*>(LNAnimationClipSerializeHandler_GetSubinstanceId(animationclipserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// AnimationStateSerializeHandler

struct HSPSubclass_LNAnimationStateSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNAnimationStateSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNAnimationStateSerializeHandler)));
}

static void HSPSubclass_LNAnimationStateSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNAnimationStateSerializeHandler_DelegateLabelCaller(LNHandle animationstateserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, animationstateserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNAnimationStateSerializeHandler*>(LNAnimationStateSerializeHandler_GetSubinstanceId(animationstateserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// EffectResourceSerializeHandler

struct HSPSubclass_LNEffectResourceSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNEffectResourceSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNEffectResourceSerializeHandler)));
}

static void HSPSubclass_LNEffectResourceSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNEffectResourceSerializeHandler_DelegateLabelCaller(LNHandle effectresourceserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, effectresourceserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNEffectResourceSerializeHandler*>(LNEffectResourceSerializeHandler_GetSubinstanceId(effectresourceserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ParticleEmitterModelSerializeHandler

struct HSPSubclass_LNParticleEmitterModelSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNParticleEmitterModelSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNParticleEmitterModelSerializeHandler)));
}

static void HSPSubclass_LNParticleEmitterModelSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNParticleEmitterModelSerializeHandler_DelegateLabelCaller(LNHandle particleemittermodelserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, particleemittermodelserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleEmitterModelSerializeHandler*>(LNParticleEmitterModelSerializeHandler_GetSubinstanceId(particleemittermodelserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ParticleModelSerializeHandler

struct HSPSubclass_LNParticleModelSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNParticleModelSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNParticleModelSerializeHandler)));
}

static void HSPSubclass_LNParticleModelSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNParticleModelSerializeHandler_DelegateLabelCaller(LNHandle particlemodelserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, particlemodelserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleModelSerializeHandler*>(LNParticleModelSerializeHandler_GetSubinstanceId(particlemodelserializehandler));
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
// WorldObjectPreUpdateHandler

struct HSPSubclass_LNWorldObjectPreUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNWorldObjectPreUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNWorldObjectPreUpdateHandler)));
}

static void HSPSubclass_LNWorldObjectPreUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNWorldObjectPreUpdateHandler_DelegateLabelCaller(LNHandle worldobjectpreupdatehandler, LNHandle self)
{
    setCallbackArg(0, worldobjectpreupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldObjectPreUpdateHandler*>(LNWorldObjectPreUpdateHandler_GetSubinstanceId(worldobjectpreupdatehandler));
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
// VisualObjectPreUpdateHandler

struct HSPSubclass_LNVisualObjectPreUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNVisualObjectPreUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNVisualObjectPreUpdateHandler)));
}

static void HSPSubclass_LNVisualObjectPreUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNVisualObjectPreUpdateHandler_DelegateLabelCaller(LNHandle visualobjectpreupdatehandler, LNHandle self)
{
    setCallbackArg(0, visualobjectpreupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualObjectPreUpdateHandler*>(LNVisualObjectPreUpdateHandler_GetSubinstanceId(visualobjectpreupdatehandler));
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
// CameraPreUpdateHandler

struct HSPSubclass_LNCameraPreUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNCameraPreUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNCameraPreUpdateHandler)));
}

static void HSPSubclass_LNCameraPreUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNCameraPreUpdateHandler_DelegateLabelCaller(LNHandle camerapreupdatehandler, LNHandle self)
{
    setCallbackArg(0, camerapreupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraPreUpdateHandler*>(LNCameraPreUpdateHandler_GetSubinstanceId(camerapreupdatehandler));
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
// EnvironmentLightPreUpdateHandler

struct HSPSubclass_LNEnvironmentLightPreUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNEnvironmentLightPreUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNEnvironmentLightPreUpdateHandler)));
}

static void HSPSubclass_LNEnvironmentLightPreUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNEnvironmentLightPreUpdateHandler_DelegateLabelCaller(LNHandle environmentlightpreupdatehandler, LNHandle self)
{
    setCallbackArg(0, environmentlightpreupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNEnvironmentLightPreUpdateHandler*>(LNEnvironmentLightPreUpdateHandler_GetSubinstanceId(environmentlightpreupdatehandler));
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
// DirectionalLightPreUpdateHandler

struct HSPSubclass_LNDirectionalLightPreUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNDirectionalLightPreUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNDirectionalLightPreUpdateHandler)));
}

static void HSPSubclass_LNDirectionalLightPreUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNDirectionalLightPreUpdateHandler_DelegateLabelCaller(LNHandle directionallightpreupdatehandler, LNHandle self)
{
    setCallbackArg(0, directionallightpreupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNDirectionalLightPreUpdateHandler*>(LNDirectionalLightPreUpdateHandler_GetSubinstanceId(directionallightpreupdatehandler));
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
// PointLightPreUpdateHandler

struct HSPSubclass_LNPointLightPreUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNPointLightPreUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNPointLightPreUpdateHandler)));
}

static void HSPSubclass_LNPointLightPreUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNPointLightPreUpdateHandler_DelegateLabelCaller(LNHandle pointlightpreupdatehandler, LNHandle self)
{
    setCallbackArg(0, pointlightpreupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNPointLightPreUpdateHandler*>(LNPointLightPreUpdateHandler_GetSubinstanceId(pointlightpreupdatehandler));
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
// SpotLightPreUpdateHandler

struct HSPSubclass_LNSpotLightPreUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNSpotLightPreUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSpotLightPreUpdateHandler)));
}

static void HSPSubclass_LNSpotLightPreUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNSpotLightPreUpdateHandler_DelegateLabelCaller(LNHandle spotlightpreupdatehandler, LNHandle self)
{
    setCallbackArg(0, spotlightpreupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNSpotLightPreUpdateHandler*>(LNSpotLightPreUpdateHandler_GetSubinstanceId(spotlightpreupdatehandler));
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
// SpritePreUpdateHandler

struct HSPSubclass_LNSpritePreUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNSpritePreUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNSpritePreUpdateHandler)));
}

static void HSPSubclass_LNSpritePreUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNSpritePreUpdateHandler_DelegateLabelCaller(LNHandle spritepreupdatehandler, LNHandle self)
{
    setCallbackArg(0, spritepreupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNSpritePreUpdateHandler*>(LNSpritePreUpdateHandler_GetSubinstanceId(spritepreupdatehandler));
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
// ShapeObjectSerializeHandler

struct HSPSubclass_LNShapeObjectSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNShapeObjectSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNShapeObjectSerializeHandler)));
}

static void HSPSubclass_LNShapeObjectSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNShapeObjectSerializeHandler_DelegateLabelCaller(LNHandle shapeobjectserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, shapeobjectserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNShapeObjectSerializeHandler*>(LNShapeObjectSerializeHandler_GetSubinstanceId(shapeobjectserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ShapeObjectPreUpdateHandler

struct HSPSubclass_LNShapeObjectPreUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNShapeObjectPreUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNShapeObjectPreUpdateHandler)));
}

static void HSPSubclass_LNShapeObjectPreUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNShapeObjectPreUpdateHandler_DelegateLabelCaller(LNHandle shapeobjectpreupdatehandler, LNHandle self)
{
    setCallbackArg(0, shapeobjectpreupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNShapeObjectPreUpdateHandler*>(LNShapeObjectPreUpdateHandler_GetSubinstanceId(shapeobjectpreupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ShapeObjectUpdateHandler

struct HSPSubclass_LNShapeObjectUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNShapeObjectUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNShapeObjectUpdateHandler)));
}

static void HSPSubclass_LNShapeObjectUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNShapeObjectUpdateHandler_DelegateLabelCaller(LNHandle shapeobjectupdatehandler, LNHandle self, float elapsedSeconds)
{
    setCallbackArg(0, shapeobjectupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNShapeObjectUpdateHandler*>(LNShapeObjectUpdateHandler_GetSubinstanceId(shapeobjectupdatehandler));
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
// PlaneMeshPreUpdateHandler

struct HSPSubclass_LNPlaneMeshPreUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNPlaneMeshPreUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNPlaneMeshPreUpdateHandler)));
}

static void HSPSubclass_LNPlaneMeshPreUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNPlaneMeshPreUpdateHandler_DelegateLabelCaller(LNHandle planemeshpreupdatehandler, LNHandle self)
{
    setCallbackArg(0, planemeshpreupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNPlaneMeshPreUpdateHandler*>(LNPlaneMeshPreUpdateHandler_GetSubinstanceId(planemeshpreupdatehandler));
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
// BoxMeshPreUpdateHandler

struct HSPSubclass_LNBoxMeshPreUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNBoxMeshPreUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNBoxMeshPreUpdateHandler)));
}

static void HSPSubclass_LNBoxMeshPreUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNBoxMeshPreUpdateHandler_DelegateLabelCaller(LNHandle boxmeshpreupdatehandler, LNHandle self)
{
    setCallbackArg(0, boxmeshpreupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxMeshPreUpdateHandler*>(LNBoxMeshPreUpdateHandler_GetSubinstanceId(boxmeshpreupdatehandler));
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
// MeshComponentSerializeHandler

struct HSPSubclass_LNMeshComponentSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNMeshComponentSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNMeshComponentSerializeHandler)));
}

static void HSPSubclass_LNMeshComponentSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNMeshComponentSerializeHandler_DelegateLabelCaller(LNHandle meshcomponentserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, meshcomponentserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNMeshComponentSerializeHandler*>(LNMeshComponentSerializeHandler_GetSubinstanceId(meshcomponentserializehandler));
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
// ParticleEmitterSerializeHandler

struct HSPSubclass_LNParticleEmitterSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNParticleEmitterSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNParticleEmitterSerializeHandler)));
}

static void HSPSubclass_LNParticleEmitterSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNParticleEmitterSerializeHandler_DelegateLabelCaller(LNHandle particleemitterserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, particleemitterserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleEmitterSerializeHandler*>(LNParticleEmitterSerializeHandler_GetSubinstanceId(particleemitterserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ParticleEmitterPreUpdateHandler

struct HSPSubclass_LNParticleEmitterPreUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNParticleEmitterPreUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNParticleEmitterPreUpdateHandler)));
}

static void HSPSubclass_LNParticleEmitterPreUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNParticleEmitterPreUpdateHandler_DelegateLabelCaller(LNHandle particleemitterpreupdatehandler, LNHandle self)
{
    setCallbackArg(0, particleemitterpreupdatehandler);
    setCallbackArg(1, self);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleEmitterPreUpdateHandler*>(LNParticleEmitterPreUpdateHandler_GetSubinstanceId(particleemitterpreupdatehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// ParticleEmitterUpdateHandler

struct HSPSubclass_LNParticleEmitterUpdateHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNParticleEmitterUpdateHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNParticleEmitterUpdateHandler)));
}

static void HSPSubclass_LNParticleEmitterUpdateHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNParticleEmitterUpdateHandler_DelegateLabelCaller(LNHandle particleemitterupdatehandler, LNHandle self, float elapsedSeconds)
{
    setCallbackArg(0, particleemitterupdatehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, elapsedSeconds);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleEmitterUpdateHandler*>(LNParticleEmitterUpdateHandler_GetSubinstanceId(particleemitterupdatehandler));
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
// UITextSerializeHandler

struct HSPSubclass_LNUITextSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUITextSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUITextSerializeHandler)));
}

static void HSPSubclass_LNUITextSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUITextSerializeHandler_DelegateLabelCaller(LNHandle uitextserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uitextserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUITextSerializeHandler*>(LNUITextSerializeHandler_GetSubinstanceId(uitextserializehandler));
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
// UIMessageTextAreaSerializeHandler

struct HSPSubclass_LNUIMessageTextAreaSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIMessageTextAreaSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIMessageTextAreaSerializeHandler)));
}

static void HSPSubclass_LNUIMessageTextAreaSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIMessageTextAreaSerializeHandler_DelegateLabelCaller(LNHandle uimessagetextareaserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uimessagetextareaserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIMessageTextAreaSerializeHandler*>(LNUIMessageTextAreaSerializeHandler_GetSubinstanceId(uimessagetextareaserializehandler));
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
// UIStackLayoutSerializeHandler

struct HSPSubclass_LNUIStackLayoutSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNUIStackLayoutSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNUIStackLayoutSerializeHandler)));
}

static void HSPSubclass_LNUIStackLayoutSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNUIStackLayoutSerializeHandler_DelegateLabelCaller(LNHandle uistacklayoutserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, uistacklayoutserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNUIStackLayoutSerializeHandler*>(LNUIStackLayoutSerializeHandler_GetSubinstanceId(uistacklayoutserializehandler));
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
// InputGestureSerializeHandler

struct HSPSubclass_LNInputGestureSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNInputGestureSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNInputGestureSerializeHandler)));
}

static void HSPSubclass_LNInputGestureSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNInputGestureSerializeHandler_DelegateLabelCaller(LNHandle inputgestureserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, inputgestureserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNInputGestureSerializeHandler*>(LNInputGestureSerializeHandler_GetSubinstanceId(inputgestureserializehandler));
    stat = 0;
    code_call(localSelf->labelPointer);
    return static_cast<LNResult>(stat);
}

//==============================================================================
// KeyGestureSerializeHandler

struct HSPSubclass_LNKeyGestureSerializeHandler
{
    unsigned short* labelPointer = nullptr;
};


static LNSubinstanceId HSPSubclass_LNKeyGestureSerializeHandler_SubinstanceAlloc(LNHandle handle)
{
    return reinterpret_cast<LNSubinstanceId>(malloc(sizeof(HSPSubclass_LNKeyGestureSerializeHandler)));
}

static void HSPSubclass_LNKeyGestureSerializeHandler_SubinstanceFree(LNHandle handle, LNSubinstanceId subinstance)
{
    free(reinterpret_cast<void*>(subinstance));
}


static LNResult HSPSubclass_LNKeyGestureSerializeHandler_DelegateLabelCaller(LNHandle keygestureserializehandler, LNHandle self, LNHandle ar)
{
    setCallbackArg(0, keygestureserializehandler);
    setCallbackArg(1, self);
    setCallbackArg(2, ar);
    auto* localSelf = reinterpret_cast<HSPSubclass_LNKeyGestureSerializeHandler*>(LNKeyGestureSerializeHandler_GetSubinstanceId(keygestureserializehandler));
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
        case 0x35 : {
            hspLNVector3_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNVector4
        case 0x4B : {
            hspLNVector4_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNQuaternion
        case 0x56 : {
            hspLNQuaternion_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNMatrix
        case 0x64 : {
            hspLNMatrix_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNColor
        case 0x18C : {
            hspLNColor_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNColorTone
        case 0x197 : {
            hspLNColorTone_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNPoint
        case 0x1A2 : {
            hspLNPoint_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNSize
        case 0x1AC : {
            hspLNSize_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNRect
        case 0x1B3 : {
            hspLNRect_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNThickness
        case 0x1C2 : {
            hspLNThickness_reffunc(typeRes, retValPtr);
            return true;
        }
        // LNCornerRadius
        case 0x1CD : {
            hspLNCornerRadius_reffunc(typeRes, retValPtr);
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
        // ln_set_args
        case 0x2 : {
            ln_set_args_cmdfunc();
            return true;
        }
        // LNVector3_SetZeros
        case 0x39 : {
            // Fetch vector3
            PVal* pval_vector3;
            CodeGetVA_TypeChecked(&pval_vector3, LNVector3);

            stat = LNVector3_SetZeros(reinterpret_cast<LNVector3*>(pval_vector3->pt));


            return true;
        }
        // LNVector3_Set
        case 0x3A : {
            // Fetch vector3
            PVal* pval_vector3;
            CodeGetVA_TypeChecked(&pval_vector3, LNVector3);
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LNVector3_Set(reinterpret_cast<LNVector3*>(pval_vector3->pt), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_z));


            return true;
        }
        // LNVector3_Get
        case 0x3E : {
            // Fetch vector3
            PVal* pval_vector3;
            CodeGetVA_TypeChecked(&pval_vector3, LNVector3);
            // Fetch outX
            PVal* pval_outX;
            const APTR aptr_outX = code_getva(&pval_outX);
            float local_outX;
            // Fetch outY
            PVal* pval_outY;
            const APTR aptr_outY = code_getva(&pval_outY);
            float local_outY;
            // Fetch outZ
            PVal* pval_outZ;
            const APTR aptr_outZ = code_getva(&pval_outZ);
            float local_outZ;

            stat = LNVector3_Get(reinterpret_cast<const LNVector3*>(pval_vector3->pt), &local_outX, &local_outY, &local_outZ);
            setVADouble(pval_outX, aptr_outX, local_outX);
            setVADouble(pval_outY, aptr_outY, local_outY);
            setVADouble(pval_outZ, aptr_outZ, local_outZ);

            return true;
        }
        // LNVector3_Length
        case 0x42 : {
            // Fetch vector3
            PVal* pval_vector3;
            CodeGetVA_TypeChecked(&pval_vector3, LNVector3);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNVector3_Length(reinterpret_cast<const LNVector3*>(pval_vector3->pt), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNVector3_LengthSquared
        case 0x43 : {
            // Fetch vector3
            PVal* pval_vector3;
            CodeGetVA_TypeChecked(&pval_vector3, LNVector3);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNVector3_LengthSquared(reinterpret_cast<const LNVector3*>(pval_vector3->pt), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNVector3_MutatingNormalize
        case 0x44 : {
            // Fetch vector3
            PVal* pval_vector3;
            CodeGetVA_TypeChecked(&pval_vector3, LNVector3);

            stat = LNVector3_MutatingNormalize(reinterpret_cast<LNVector3*>(pval_vector3->pt));


            return true;
        }
        // LNVector3_NormalizeXYZ
        case 0x45 : {
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNVector3_NormalizeXYZ(static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_z), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNVector3_Normalize
        case 0x49 : {
            // Fetch vec
            PVal* pval_vec;
            CodeGetVA_TypeChecked(&pval_vec, LNVector3);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNVector3_Normalize(reinterpret_cast<LNVector3*>(pval_vec->pt), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNVector4_SetZeros
        case 0x50 : {
            // Fetch vector4
            PVal* pval_vector4;
            CodeGetVA_TypeChecked(&pval_vector4, LNVector4);

            stat = LNVector4_SetZeros(reinterpret_cast<LNVector4*>(pval_vector4->pt));


            return true;
        }
        // LNVector4_Set
        case 0x51 : {
            // Fetch vector4
            PVal* pval_vector4;
            CodeGetVA_TypeChecked(&pval_vector4, LNVector4);
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();
            // Fetch w
            const auto local_w = fetchVADouble();

            stat = LNVector4_Set(reinterpret_cast<LNVector4*>(pval_vector4->pt), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_z), static_cast<float>(local_w));


            return true;
        }
        // LNQuaternion_SetZeros
        case 0x5B : {
            // Fetch quaternion
            PVal* pval_quaternion;
            CodeGetVA_TypeChecked(&pval_quaternion, LNQuaternion);

            stat = LNQuaternion_SetZeros(reinterpret_cast<LNQuaternion*>(pval_quaternion->pt));


            return true;
        }
        // LNQuaternion_Set
        case 0x5C : {
            // Fetch quaternion
            PVal* pval_quaternion;
            CodeGetVA_TypeChecked(&pval_quaternion, LNQuaternion);
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();
            // Fetch w
            const auto local_w = fetchVADouble();

            stat = LNQuaternion_Set(reinterpret_cast<LNQuaternion*>(pval_quaternion->pt), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_z), static_cast<float>(local_w));


            return true;
        }
        // LNQuaternion_SetFromAxis
        case 0x61 : {
            // Fetch quaternion
            PVal* pval_quaternion;
            CodeGetVA_TypeChecked(&pval_quaternion, LNQuaternion);
            // Fetch axis
            PVal* pval_axis;
            CodeGetVA_TypeChecked(&pval_axis, LNVector3);
            // Fetch r
            const auto local_r = fetchVADouble();

            stat = LNQuaternion_SetFromAxis(reinterpret_cast<LNQuaternion*>(pval_quaternion->pt), reinterpret_cast<LNVector3*>(pval_axis->pt), static_cast<float>(local_r));


            return true;
        }
        // LNMatrix_SetZeros
        case 0x69 : {
            // Fetch matrix
            PVal* pval_matrix;
            CodeGetVA_TypeChecked(&pval_matrix, LNMatrix);

            stat = LNMatrix_SetZeros(reinterpret_cast<LNMatrix*>(pval_matrix->pt));


            return true;
        }
        // LNMatrix_Set
        case 0x6A : {
            // Fetch matrix
            PVal* pval_matrix;
            CodeGetVA_TypeChecked(&pval_matrix, LNMatrix);
            // Fetch m11
            const auto local_m11 = fetchVADouble();
            // Fetch m12
            const auto local_m12 = fetchVADouble();
            // Fetch m13
            const auto local_m13 = fetchVADouble();
            // Fetch m14
            const auto local_m14 = fetchVADouble();
            // Fetch m21
            const auto local_m21 = fetchVADouble();
            // Fetch m22
            const auto local_m22 = fetchVADouble();
            // Fetch m23
            const auto local_m23 = fetchVADouble();
            // Fetch m24
            const auto local_m24 = fetchVADouble();
            // Fetch m31
            const auto local_m31 = fetchVADouble();
            // Fetch m32
            const auto local_m32 = fetchVADouble();
            // Fetch m33
            const auto local_m33 = fetchVADouble();
            // Fetch m34
            const auto local_m34 = fetchVADouble();
            // Fetch m41
            const auto local_m41 = fetchVADouble();
            // Fetch m42
            const auto local_m42 = fetchVADouble();
            // Fetch m43
            const auto local_m43 = fetchVADouble();
            // Fetch m44
            const auto local_m44 = fetchVADouble();

            stat = LNMatrix_Set(reinterpret_cast<LNMatrix*>(pval_matrix->pt), static_cast<float>(local_m11), static_cast<float>(local_m12), static_cast<float>(local_m13), static_cast<float>(local_m14), static_cast<float>(local_m21), static_cast<float>(local_m22), static_cast<float>(local_m23), static_cast<float>(local_m24), static_cast<float>(local_m31), static_cast<float>(local_m32), static_cast<float>(local_m33), static_cast<float>(local_m34), static_cast<float>(local_m41), static_cast<float>(local_m42), static_cast<float>(local_m43), static_cast<float>(local_m44));


            return true;
        }
        // LNColor_SetZeros
        case 0x191 : {
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);

            stat = LNColor_SetZeros(reinterpret_cast<LNColor*>(pval_color->pt));


            return true;
        }
        // LNColor_Set
        case 0x192 : {
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);
            // Fetch r_
            const auto local_r_ = fetchVADouble();
            // Fetch g_
            const auto local_g_ = fetchVADouble();
            // Fetch b_
            const auto local_b_ = fetchVADouble();
            // Fetch a_
            const auto local_a_ = fetchVADouble(1.000000);

            stat = LNColor_Set(reinterpret_cast<LNColor*>(pval_color->pt), static_cast<float>(local_r_), static_cast<float>(local_g_), static_cast<float>(local_b_), static_cast<float>(local_a_));


            return true;
        }
        // LNColorTone_SetZeros
        case 0x19C : {
            // Fetch colortone
            PVal* pval_colortone;
            CodeGetVA_TypeChecked(&pval_colortone, LNColorTone);

            stat = LNColorTone_SetZeros(reinterpret_cast<LNColorTone*>(pval_colortone->pt));


            return true;
        }
        // LNColorTone_Set
        case 0x19D : {
            // Fetch colortone
            PVal* pval_colortone;
            CodeGetVA_TypeChecked(&pval_colortone, LNColorTone);
            // Fetch r_
            const auto local_r_ = fetchVADouble();
            // Fetch g_
            const auto local_g_ = fetchVADouble();
            // Fetch b_
            const auto local_b_ = fetchVADouble();
            // Fetch s_
            const auto local_s_ = fetchVADouble();

            stat = LNColorTone_Set(reinterpret_cast<LNColorTone*>(pval_colortone->pt), static_cast<float>(local_r_), static_cast<float>(local_g_), static_cast<float>(local_b_), static_cast<float>(local_s_));


            return true;
        }
        // LNPoint_SetZeros
        case 0x1A5 : {
            // Fetch point
            PVal* pval_point;
            CodeGetVA_TypeChecked(&pval_point, LNPoint);

            stat = LNPoint_SetZeros(reinterpret_cast<LNPoint*>(pval_point->pt));


            return true;
        }
        // LNPoint_Set
        case 0x1A6 : {
            // Fetch point
            PVal* pval_point;
            CodeGetVA_TypeChecked(&pval_point, LNPoint);
            // Fetch x_
            const auto local_x_ = fetchVADouble();
            // Fetch y_
            const auto local_y_ = fetchVADouble();

            stat = LNPoint_Set(reinterpret_cast<LNPoint*>(pval_point->pt), static_cast<float>(local_x_), static_cast<float>(local_y_));


            return true;
        }
        // LNPoint_Get
        case 0x1A9 : {
            // Fetch point
            PVal* pval_point;
            CodeGetVA_TypeChecked(&pval_point, LNPoint);
            // Fetch outX
            PVal* pval_outX;
            const APTR aptr_outX = code_getva(&pval_outX);
            float local_outX;
            // Fetch outY
            PVal* pval_outY;
            const APTR aptr_outY = code_getva(&pval_outY);
            float local_outY;

            stat = LNPoint_Get(reinterpret_cast<const LNPoint*>(pval_point->pt), &local_outX, &local_outY);
            setVADouble(pval_outX, aptr_outX, local_outX);
            setVADouble(pval_outY, aptr_outY, local_outY);

            return true;
        }
        // LNSize_SetZeros
        case 0x1AF : {
            // Fetch size
            PVal* pval_size;
            CodeGetVA_TypeChecked(&pval_size, LNSize);

            stat = LNSize_SetZeros(reinterpret_cast<LNSize*>(pval_size->pt));


            return true;
        }
        // LNSize_Set
        case 0x1B0 : {
            // Fetch size
            PVal* pval_size;
            CodeGetVA_TypeChecked(&pval_size, LNSize);
            // Fetch w
            const auto local_w = fetchVADouble();
            // Fetch h
            const auto local_h = fetchVADouble();

            stat = LNSize_Set(reinterpret_cast<LNSize*>(pval_size->pt), static_cast<float>(local_w), static_cast<float>(local_h));


            return true;
        }
        // LNRect_SetZeros
        case 0x1B8 : {
            // Fetch rect
            PVal* pval_rect;
            CodeGetVA_TypeChecked(&pval_rect, LNRect);

            stat = LNRect_SetZeros(reinterpret_cast<LNRect*>(pval_rect->pt));


            return true;
        }
        // LNRect_Set
        case 0x1B9 : {
            // Fetch rect
            PVal* pval_rect;
            CodeGetVA_TypeChecked(&pval_rect, LNRect);
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch width
            const auto local_width = fetchVADouble();
            // Fetch height
            const auto local_height = fetchVADouble();

            stat = LNRect_Set(reinterpret_cast<LNRect*>(pval_rect->pt), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_width), static_cast<float>(local_height));


            return true;
        }
        // LNRect_GetLeft
        case 0x1BE : {
            // Fetch rect
            PVal* pval_rect;
            CodeGetVA_TypeChecked(&pval_rect, LNRect);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNRect_GetLeft(reinterpret_cast<const LNRect*>(pval_rect->pt), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNRect_SetSize
        case 0x1BF : {
            // Fetch rect
            PVal* pval_rect;
            CodeGetVA_TypeChecked(&pval_rect, LNRect);
            // Fetch size
            PVal* pval_size;
            CodeGetVA_TypeChecked(&pval_size, LNSize);

            stat = LNRect_SetSize(reinterpret_cast<LNRect*>(pval_rect->pt), reinterpret_cast<LNSize*>(pval_size->pt));


            return true;
        }
        // LNRect_GetSize
        case 0x1C1 : {
            // Fetch rect
            PVal* pval_rect;
            CodeGetVA_TypeChecked(&pval_rect, LNRect);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNSize local_outReturn;

            stat = LNRect_GetSize(reinterpret_cast<const LNRect*>(pval_rect->pt), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNSize_typeid(), &local_outReturn);

            return true;
        }
        // LNThickness_SetZeros
        case 0x1C7 : {
            // Fetch thickness
            PVal* pval_thickness;
            CodeGetVA_TypeChecked(&pval_thickness, LNThickness);

            stat = LNThickness_SetZeros(reinterpret_cast<LNThickness*>(pval_thickness->pt));


            return true;
        }
        // LNThickness_Set
        case 0x1C8 : {
            // Fetch thickness
            PVal* pval_thickness;
            CodeGetVA_TypeChecked(&pval_thickness, LNThickness);
            // Fetch left_
            const auto local_left_ = fetchVADouble();
            // Fetch top_
            const auto local_top_ = fetchVADouble();
            // Fetch right_
            const auto local_right_ = fetchVADouble();
            // Fetch bottom_
            const auto local_bottom_ = fetchVADouble();

            stat = LNThickness_Set(reinterpret_cast<LNThickness*>(pval_thickness->pt), static_cast<float>(local_left_), static_cast<float>(local_top_), static_cast<float>(local_right_), static_cast<float>(local_bottom_));


            return true;
        }
        // LNCornerRadius_SetZeros
        case 0x1D2 : {
            // Fetch cornerradius
            PVal* pval_cornerradius;
            CodeGetVA_TypeChecked(&pval_cornerradius, LNCornerRadius);

            stat = LNCornerRadius_SetZeros(reinterpret_cast<LNCornerRadius*>(pval_cornerradius->pt));


            return true;
        }
        // LNCornerRadius_Set
        case 0x1D3 : {
            // Fetch cornerradius
            PVal* pval_cornerradius;
            CodeGetVA_TypeChecked(&pval_cornerradius, LNCornerRadius);
            // Fetch topLeft
            const auto local_topLeft = fetchVADouble();
            // Fetch topRight
            const auto local_topRight = fetchVADouble();
            // Fetch bottomRight
            const auto local_bottomRight = fetchVADouble();
            // Fetch bottomLeft
            const auto local_bottomLeft = fetchVADouble();

            stat = LNCornerRadius_Set(reinterpret_cast<LNCornerRadius*>(pval_cornerradius->pt), static_cast<float>(local_topLeft), static_cast<float>(local_topRight), static_cast<float>(local_bottomRight), static_cast<float>(local_bottomLeft));


            return true;
        }
        // LNObject_Release
        case 0x84 : {
            // Fetch object
            const auto local_object = fetchVAInt();

            stat = LNObject_Release(static_cast<LNHandle>(local_object));


            return true;
        }
        // LNObject_Retain
        case 0x85 : {
            // Fetch object
            const auto local_object = fetchVAInt();

            stat = LNObject_Retain(static_cast<LNHandle>(local_object));


            return true;
        }
        // LNObject_GetReferenceCount
        case 0x86 : {
            // Fetch object
            const auto local_object = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LNObject_GetReferenceCount(static_cast<LNHandle>(local_object), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNObject_SetPrototype_OnSerialize
        case 0xA35 : {
            // Fetch object
            const auto local_object = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNObjectSerializeHandler_Create(HSPSubclass_LNObjectSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNObjectSerializeHandler*>(LNObjectSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNObject_SetPrototype_OnSerialize(static_cast<LNHandle>(local_object), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNEventConnection_SetPrototype_OnSerialize
        case 0xA40 : {
            // Fetch eventconnection
            const auto local_eventconnection = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNEventConnectionSerializeHandler_Create(HSPSubclass_LNEventConnectionSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNEventConnectionSerializeHandler*>(LNEventConnectionSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNEventConnection_SetPrototype_OnSerialize(static_cast<LNHandle>(local_eventconnection), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNPromiseFailureDelegate_Create
        case 0x638 : {
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
        // LNVariant_Create
        case 0x8B : {
            // Fetch outVariant
            PVal* pval_outVariant;
            const APTR aptr_outVariant = code_getva(&pval_outVariant);
            LNHandle local_outVariant;

            stat = LNVariant_Create(&local_outVariant);
            setVAInt(pval_outVariant, aptr_outVariant, local_outVariant);

            return true;
        }
        // LNVariant_SetInt
        case 0x8C : {
            // Fetch variant
            const auto local_variant = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LNVariant_SetInt(static_cast<LNHandle>(local_variant), static_cast<int>(local_value));


            return true;
        }
        // LNVariant_GetInt
        case 0x8E : {
            // Fetch variant
            const auto local_variant = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LNVariant_GetInt(static_cast<LNHandle>(local_variant), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNVariant_SetPrototype_OnSerialize
        case 0xA4B : {
            // Fetch variant
            const auto local_variant = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNVariantSerializeHandler_Create(HSPSubclass_LNVariantSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNVariantSerializeHandler*>(LNVariantSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNVariant_SetPrototype_OnSerialize(static_cast<LNHandle>(local_variant), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNZVTestDelegate1_Create
        case 0x641 : {
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
        case 0x647 : {
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
        case 0x64C : {
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
        case 0x651 : {
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
        case 0x656 : {
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
        case 0x659 : {
            // Fetch zvtestpromise1
            const auto local_zvtestpromise1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNZVTestDelegate3_Create(HSPSubclass_LNZVTestDelegate3_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate3*>(LNZVTestDelegate3_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNZVTestPromise1_ThenWith(static_cast<LNHandle>(local_zvtestpromise1), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNZVTestPromise1_CatchWith
        case 0x65B : {
            // Fetch zvtestpromise1
            const auto local_zvtestpromise1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPromiseFailureDelegate_Create(HSPSubclass_LNPromiseFailureDelegate_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPromiseFailureDelegate*>(LNPromiseFailureDelegate_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNZVTestPromise1_CatchWith(static_cast<LNHandle>(local_zvtestpromise1), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNZVTestPromise2_ThenWith
        case 0x65F : {
            // Fetch zvtestpromise2
            const auto local_zvtestpromise2 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNZVTestDelegate1_Create(HSPSubclass_LNZVTestDelegate1_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate1*>(LNZVTestDelegate1_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNZVTestPromise2_ThenWith(static_cast<LNHandle>(local_zvtestpromise2), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNZVTestPromise2_CatchWith
        case 0x661 : {
            // Fetch zvtestpromise2
            const auto local_zvtestpromise2 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPromiseFailureDelegate_Create(HSPSubclass_LNPromiseFailureDelegate_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPromiseFailureDelegate*>(LNPromiseFailureDelegate_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNZVTestPromise2_CatchWith(static_cast<LNHandle>(local_zvtestpromise2), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNZVTestClass1_Create
        case 0xB7 : {
            // Fetch outZVTestClass1
            PVal* pval_outZVTestClass1;
            const APTR aptr_outZVTestClass1 = code_getva(&pval_outZVTestClass1);
            LNHandle local_outZVTestClass1;

            stat = LNZVTestClass1_Create(&local_outZVTestClass1);
            setVAInt(pval_outZVTestClass1, aptr_outZVTestClass1, local_outZVTestClass1);

            return true;
        }
        // LNZVTestClass1_SetTestDelegate1
        case 0xA1 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVALabelPointer();

            LNHandle localDelegate_value;
            {
                stat = LNZVTestDelegate1_Create(HSPSubclass_LNZVTestDelegate1_DelegateLabelCaller, &localDelegate_value);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate1*>(LNZVTestDelegate1_GetSubinstanceId(localDelegate_value));
                localSelf->labelPointer = local_value;
            }

            stat = LNZVTestClass1_SetTestDelegate1(static_cast<LNHandle>(local_zvtestclass1), localDelegate_value);
            LNObject_Release(localDelegate_value);

            return true;
        }
        // LNZVTestClass1_SetTestDelegate2
        case 0xA3 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVALabelPointer();

            LNHandle localDelegate_value;
            {
                stat = LNZVTestDelegate2_Create(HSPSubclass_LNZVTestDelegate2_DelegateLabelCaller, &localDelegate_value);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate2*>(LNZVTestDelegate2_GetSubinstanceId(localDelegate_value));
                localSelf->labelPointer = local_value;
            }

            stat = LNZVTestClass1_SetTestDelegate2(static_cast<LNHandle>(local_zvtestclass1), localDelegate_value);
            LNObject_Release(localDelegate_value);

            return true;
        }
        // LNZVTestClass1_SetTestDelegate3
        case 0xA5 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVALabelPointer();

            LNHandle localDelegate_value;
            {
                stat = LNZVTestDelegate3_Create(HSPSubclass_LNZVTestDelegate3_DelegateLabelCaller, &localDelegate_value);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestDelegate3*>(LNZVTestDelegate3_GetSubinstanceId(localDelegate_value));
                localSelf->labelPointer = local_value;
            }

            stat = LNZVTestClass1_SetTestDelegate3(static_cast<LNHandle>(local_zvtestclass1), localDelegate_value);
            LNObject_Release(localDelegate_value);

            return true;
        }
        // LNZVTestClass1_CallTestDelegate1
        case 0xA7 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch a
            const auto local_a = fetchVAInt();

            stat = LNZVTestClass1_CallTestDelegate1(static_cast<LNHandle>(local_zvtestclass1), static_cast<int>(local_a));


            return true;
        }
        // LNZVTestClass1_CallTestDelegate2
        case 0xA9 : {
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

            stat = LNZVTestClass1_CallTestDelegate2(static_cast<LNHandle>(local_zvtestclass1), static_cast<int>(local_a), static_cast<int>(local_b), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestClass1_CallTestDelegate3
        case 0xAC : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();

            stat = LNZVTestClass1_CallTestDelegate3(static_cast<LNHandle>(local_zvtestclass1));


            return true;
        }
        // LNZVTestClass1_LoadAsyncA
        case 0xAD : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNZVTestClass1_LoadAsyncA(static_cast<const char*>(local_filePath), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestClass1_ExecuteAsync
        case 0xAF : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNZVTestClass1_ExecuteAsync(static_cast<LNHandle>(local_zvtestclass1), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestClass1_GetFilePathA
        case 0xB0 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LNZVTestClass1_GetFilePathA(static_cast<LNHandle>(local_zvtestclass1), &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestClass1_ConnectOnEvent1
        case 0xB1 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNZVTestEventHandler1_Create(HSPSubclass_LNZVTestEventHandler1_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestEventHandler1*>(LNZVTestEventHandler1_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNZVTestClass1_ConnectOnEvent1(static_cast<LNHandle>(local_zvtestclass1), localDelegate_handler, &local_outReturn);
            LNObject_Release(localDelegate_handler);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestClass1_RaiseEvent1
        case 0xB3 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();

            stat = LNZVTestClass1_RaiseEvent1(static_cast<LNHandle>(local_zvtestclass1));


            return true;
        }
        // LNZVTestClass1_ConnectOnEvent2
        case 0xB4 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNZVTestEventHandler2_Create(HSPSubclass_LNZVTestEventHandler2_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestEventHandler2*>(LNZVTestEventHandler2_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNZVTestClass1_ConnectOnEvent2(static_cast<LNHandle>(local_zvtestclass1), localDelegate_handler, &local_outReturn);
            LNObject_Release(localDelegate_handler);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestClass1_RaiseEvent2
        case 0xB6 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();

            stat = LNZVTestClass1_RaiseEvent2(static_cast<LNHandle>(local_zvtestclass1));


            return true;
        }
        // LNZVTestClass1_SetPrototype_OnSerialize
        case 0xA56 : {
            // Fetch zvtestclass1
            const auto local_zvtestclass1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNZVTestClass1SerializeHandler_Create(HSPSubclass_LNZVTestClass1SerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestClass1SerializeHandler*>(LNZVTestClass1SerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNZVTestClass1_SetPrototype_OnSerialize(static_cast<LNHandle>(local_zvtestclass1), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNZVTestEventArgs1_Create
        case 0xBA : {
            // Fetch outZVTestEventArgs1
            PVal* pval_outZVTestEventArgs1;
            const APTR aptr_outZVTestEventArgs1 = code_getva(&pval_outZVTestEventArgs1);
            LNHandle local_outZVTestEventArgs1;

            stat = LNZVTestEventArgs1_Create(&local_outZVTestEventArgs1);
            setVAInt(pval_outZVTestEventArgs1, aptr_outZVTestEventArgs1, local_outZVTestEventArgs1);

            return true;
        }
        // LNZVTestEventArgs1_CreateWithValue
        case 0xBB : {
            // Fetch v
            const auto local_v = fetchVAInt();
            // Fetch outZVTestEventArgs1
            PVal* pval_outZVTestEventArgs1;
            const APTR aptr_outZVTestEventArgs1 = code_getva(&pval_outZVTestEventArgs1);
            LNHandle local_outZVTestEventArgs1;

            stat = LNZVTestEventArgs1_CreateWithValue(static_cast<int>(local_v), &local_outZVTestEventArgs1);
            setVAInt(pval_outZVTestEventArgs1, aptr_outZVTestEventArgs1, local_outZVTestEventArgs1);

            return true;
        }
        // LNZVTestEventArgs1_GetValue
        case 0xB9 : {
            // Fetch zvtesteventargs1
            const auto local_zvtesteventargs1 = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LNZVTestEventArgs1_GetValue(static_cast<LNHandle>(local_zvtesteventargs1), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNZVTestEventArgs1_SetPrototype_OnSerialize
        case 0xA61 : {
            // Fetch zvtesteventargs1
            const auto local_zvtesteventargs1 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNZVTestEventArgs1SerializeHandler_Create(HSPSubclass_LNZVTestEventArgs1SerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNZVTestEventArgs1SerializeHandler*>(LNZVTestEventArgs1SerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNZVTestEventArgs1_SetPrototype_OnSerialize(static_cast<LNHandle>(local_zvtesteventargs1), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNLog_SetLevel
        case 0xBE : {
            // Fetch level
            const auto local_level = static_cast<LNLogLevel>(fetchVAInt());

            stat = LNLog_SetLevel(static_cast<LNLogLevel>(local_level));


            return true;
        }
        // LNLog_AllocConsole
        case 0xC0 : {

            stat = LNLog_AllocConsole();


            return true;
        }
        // LNLog_WriteA
        case 0xC1 : {
            // Fetch level
            const auto local_level = static_cast<LNLogLevel>(fetchVAInt());
            // Fetch tag
            const auto local_tag = fetchVAString();
            // Fetch text
            const auto local_text = fetchVAString();

            stat = LNLog_WriteA(static_cast<LNLogLevel>(local_level), static_cast<const char*>(local_tag), static_cast<const char*>(local_text));


            return true;
        }
        // LNSerializer2_SetPrototype_OnSerialize
        case 0xA6C : {
            // Fetch serializer2
            const auto local_serializer2 = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSerializer2SerializeHandler_Create(HSPSubclass_LNSerializer2SerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSerializer2SerializeHandler*>(LNSerializer2SerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSerializer2_SetPrototype_OnSerialize(static_cast<LNHandle>(local_serializer2), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNAssetObject_SetPrototype_OnSerialize
        case 0xA77 : {
            // Fetch assetobject
            const auto local_assetobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNAssetObjectSerializeHandler_Create(HSPSubclass_LNAssetObjectSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNAssetObjectSerializeHandler*>(LNAssetObjectSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNAssetObject_SetPrototype_OnSerialize(static_cast<LNHandle>(local_assetobject), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNAssetImportSettings_SetPrototype_OnSerialize
        case 0xA82 : {
            // Fetch assetimportsettings
            const auto local_assetimportsettings = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNAssetImportSettingsSerializeHandler_Create(HSPSubclass_LNAssetImportSettingsSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNAssetImportSettingsSerializeHandler*>(LNAssetImportSettingsSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNAssetImportSettings_SetPrototype_OnSerialize(static_cast<LNHandle>(local_assetimportsettings), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNAssetModel_Create
        case 0x125 : {
            // Fetch target
            const auto local_target = fetchVAInt();
            // Fetch outAssetModel
            PVal* pval_outAssetModel;
            const APTR aptr_outAssetModel = code_getva(&pval_outAssetModel);
            LNHandle local_outAssetModel;

            stat = LNAssetModel_Create(static_cast<LNHandle>(local_target), &local_outAssetModel);
            setVAInt(pval_outAssetModel, aptr_outAssetModel, local_outAssetModel);

            return true;
        }
        // LNAssetModel_Target
        case 0x124 : {
            // Fetch assetmodel
            const auto local_assetmodel = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNAssetModel_Target(static_cast<LNHandle>(local_assetmodel), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNAssetModel_SetPrototype_OnSerialize
        case 0xA8D : {
            // Fetch assetmodel
            const auto local_assetmodel = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNAssetModelSerializeHandler_Create(HSPSubclass_LNAssetModelSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNAssetModelSerializeHandler*>(LNAssetModelSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNAssetModel_SetPrototype_OnSerialize(static_cast<LNHandle>(local_assetmodel), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNAssets_SaveAssetToLocalFileA
        case 0x128 : {
            // Fetch asset
            const auto local_asset = fetchVAInt();
            // Fetch filePath
            const auto local_filePath = fetchVAString();

            stat = LNAssets_SaveAssetToLocalFileA(static_cast<LNHandle>(local_asset), static_cast<const char*>(local_filePath));


            return true;
        }
        // LNAssets_LoadAssetFromLocalFileA
        case 0x12B : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNAssets_LoadAssetFromLocalFileA(static_cast<const char*>(local_filePath), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNAssets_LoadAssetA
        case 0x12D : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNAssets_LoadAssetA(static_cast<const char*>(local_filePath), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNAssets_ReloadAssetA
        case 0x12F : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch obj
            const auto local_obj = fetchVAInt();

            stat = LNAssets_ReloadAssetA(static_cast<const char*>(local_filePath), static_cast<LNHandle>(local_obj));


            return true;
        }
        // LNAssets_ReadAllTextA
        case 0x132 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch encoding
            const auto local_encoding = static_cast<LNEncodingType>(fetchVAInt(LN_ENCODING_TYPE_UNKNOWN));
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LNAssets_ReadAllTextA(static_cast<const char*>(local_filePath), static_cast<LNEncodingType>(local_encoding), &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSound_SetVolume
        case 0x13C : {
            // Fetch sound
            const auto local_sound = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNSound_SetVolume(static_cast<LNHandle>(local_sound), static_cast<float>(local_value));


            return true;
        }
        // LNSound_GetVolume
        case 0x13E : {
            // Fetch sound
            const auto local_sound = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNSound_GetVolume(static_cast<LNHandle>(local_sound), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSound_SetPitch
        case 0x13F : {
            // Fetch sound
            const auto local_sound = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNSound_SetPitch(static_cast<LNHandle>(local_sound), static_cast<float>(local_value));


            return true;
        }
        // LNSound_GetPitch
        case 0x141 : {
            // Fetch sound
            const auto local_sound = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNSound_GetPitch(static_cast<LNHandle>(local_sound), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSound_SetLoopEnabled
        case 0x142 : {
            // Fetch sound
            const auto local_sound = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNSound_SetLoopEnabled(static_cast<LNHandle>(local_sound), static_cast<LNBool>(local_enabled));


            return true;
        }
        // LNSound_IsLoopEnabled
        case 0x144 : {
            // Fetch sound
            const auto local_sound = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNSound_IsLoopEnabled(static_cast<LNHandle>(local_sound), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSound_SetLoopRange
        case 0x145 : {
            // Fetch sound
            const auto local_sound = fetchVAInt();
            // Fetch begin
            const auto local_begin = fetchVAInt();
            // Fetch length
            const auto local_length = fetchVAInt();

            stat = LNSound_SetLoopRange(static_cast<LNHandle>(local_sound), static_cast<uint32_t>(local_begin), static_cast<uint32_t>(local_length));


            return true;
        }
        // LNSound_Play
        case 0x148 : {
            // Fetch sound
            const auto local_sound = fetchVAInt();

            stat = LNSound_Play(static_cast<LNHandle>(local_sound));


            return true;
        }
        // LNSound_Stop
        case 0x149 : {
            // Fetch sound
            const auto local_sound = fetchVAInt();

            stat = LNSound_Stop(static_cast<LNHandle>(local_sound));


            return true;
        }
        // LNSound_Pause
        case 0x14A : {
            // Fetch sound
            const auto local_sound = fetchVAInt();

            stat = LNSound_Pause(static_cast<LNHandle>(local_sound));


            return true;
        }
        // LNSound_Resume
        case 0x14B : {
            // Fetch sound
            const auto local_sound = fetchVAInt();

            stat = LNSound_Resume(static_cast<LNHandle>(local_sound));


            return true;
        }
        // LNSound_FadeVolume
        case 0x14C : {
            // Fetch sound
            const auto local_sound = fetchVAInt();
            // Fetch targetVolume
            const auto local_targetVolume = fetchVADouble();
            // Fetch time
            const auto local_time = fetchVADouble();
            // Fetch behavior
            const auto local_behavior = static_cast<LNSoundFadeBehavior>(fetchVAInt());

            stat = LNSound_FadeVolume(static_cast<LNHandle>(local_sound), static_cast<float>(local_targetVolume), static_cast<float>(local_time), static_cast<LNSoundFadeBehavior>(local_behavior));


            return true;
        }
        // LNSound_SetPrototype_OnSerialize
        case 0xA98 : {
            // Fetch sound
            const auto local_sound = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSoundSerializeHandler_Create(HSPSubclass_LNSoundSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSoundSerializeHandler*>(LNSoundSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSound_SetPrototype_OnSerialize(static_cast<LNHandle>(local_sound), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNAudio_PlayBGMA
        case 0x151 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch volume
            const auto local_volume = fetchVADouble(1.000000);
            // Fetch pitch
            const auto local_pitch = fetchVADouble(1.000000);
            // Fetch fadeTime
            const auto local_fadeTime = fetchVADouble(0.000000);

            stat = LNAudio_PlayBGMA(static_cast<const char*>(local_filePath), static_cast<float>(local_volume), static_cast<float>(local_pitch), static_cast<double>(local_fadeTime));


            return true;
        }
        // LNAudio_StopBGM
        case 0x156 : {
            // Fetch fadeTime
            const auto local_fadeTime = fetchVADouble(0.000000);

            stat = LNAudio_StopBGM(static_cast<double>(local_fadeTime));


            return true;
        }
        // LNAudio_PlayBGSA
        case 0x158 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch volume
            const auto local_volume = fetchVADouble(1.000000);
            // Fetch pitch
            const auto local_pitch = fetchVADouble(1.000000);
            // Fetch fadeTime
            const auto local_fadeTime = fetchVADouble(0.000000);

            stat = LNAudio_PlayBGSA(static_cast<const char*>(local_filePath), static_cast<float>(local_volume), static_cast<float>(local_pitch), static_cast<double>(local_fadeTime));


            return true;
        }
        // LNAudio_StopBGS
        case 0x15D : {
            // Fetch fadeTime
            const auto local_fadeTime = fetchVADouble(0.000000);

            stat = LNAudio_StopBGS(static_cast<double>(local_fadeTime));


            return true;
        }
        // LNAudio_PlayMEA
        case 0x15F : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch volume
            const auto local_volume = fetchVADouble(1.000000);
            // Fetch pitch
            const auto local_pitch = fetchVADouble(1.000000);

            stat = LNAudio_PlayMEA(static_cast<const char*>(local_filePath), static_cast<float>(local_volume), static_cast<float>(local_pitch));


            return true;
        }
        // LNAudio_StopME
        case 0x163 : {

            stat = LNAudio_StopME();


            return true;
        }
        // LNAudio_PlaySEA
        case 0x164 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch volume
            const auto local_volume = fetchVADouble(1.000000);
            // Fetch pitch
            const auto local_pitch = fetchVADouble(1.000000);

            stat = LNAudio_PlaySEA(static_cast<const char*>(local_filePath), static_cast<float>(local_volume), static_cast<float>(local_pitch));


            return true;
        }
        // LNAudio_PlaySE3DA
        case 0x168 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch position
            PVal* pval_position;
            CodeGetVA_TypeChecked(&pval_position, LNVector3);
            // Fetch distance
            const auto local_distance = fetchVADouble();
            // Fetch volume
            const auto local_volume = fetchVADouble(1.000000);
            // Fetch pitch
            const auto local_pitch = fetchVADouble(1.000000);

            stat = LNAudio_PlaySE3DA(static_cast<const char*>(local_filePath), reinterpret_cast<LNVector3*>(pval_position->pt), static_cast<float>(local_distance), static_cast<float>(local_volume), static_cast<float>(local_pitch));


            return true;
        }
        // LNAudio_StopSE
        case 0x16E : {

            stat = LNAudio_StopSE();


            return true;
        }
        // LNTexture2DDelegate_Create
        case 0x6A7 : {
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
        case 0x6AA : {
            // Fetch texture2dpromise
            const auto local_texture2dpromise = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNTexture2DDelegate_Create(HSPSubclass_LNTexture2DDelegate_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNTexture2DDelegate*>(LNTexture2DDelegate_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNTexture2DPromise_ThenWith(static_cast<LNHandle>(local_texture2dpromise), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNTexture2DPromise_CatchWith
        case 0x6AC : {
            // Fetch texture2dpromise
            const auto local_texture2dpromise = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPromiseFailureDelegate_Create(HSPSubclass_LNPromiseFailureDelegate_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPromiseFailureDelegate*>(LNPromiseFailureDelegate_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNTexture2DPromise_CatchWith(static_cast<LNHandle>(local_texture2dpromise), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNGraphics_GetActiveGraphicsAPI
        case 0x18B : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNGraphicsAPI local_outReturn;

            stat = LNGraphics_GetActiveGraphicsAPI(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNTexture_SetPrototype_OnSerialize
        case 0xAA3 : {
            // Fetch texture
            const auto local_texture = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNTextureSerializeHandler_Create(HSPSubclass_LNTextureSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNTextureSerializeHandler*>(LNTextureSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNTexture_SetPrototype_OnSerialize(static_cast<LNHandle>(local_texture), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNTexture2D_Create
        case 0x1DE : {
            // Fetch width
            const auto local_width = fetchVAInt();
            // Fetch height
            const auto local_height = fetchVAInt();
            // Fetch outTexture2D
            PVal* pval_outTexture2D;
            const APTR aptr_outTexture2D = code_getva(&pval_outTexture2D);
            LNHandle local_outTexture2D;

            stat = LNTexture2D_Create(static_cast<int>(local_width), static_cast<int>(local_height), &local_outTexture2D);
            setVAInt(pval_outTexture2D, aptr_outTexture2D, local_outTexture2D);

            return true;
        }
        // LNTexture2D_CreateWithFormat
        case 0x1E1 : {
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

            stat = LNTexture2D_CreateWithFormat(static_cast<int>(local_width), static_cast<int>(local_height), static_cast<LNTextureFormat>(local_format), &local_outTexture2D);
            setVAInt(pval_outTexture2D, aptr_outTexture2D, local_outTexture2D);

            return true;
        }
        // LNTexture2D_LoadA
        case 0x1DA : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNTexture2D_LoadA(static_cast<const char*>(local_filePath), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNTexture2D_LoadEmojiA
        case 0x1DC : {
            // Fetch code
            const auto local_code = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNTexture2D_LoadEmojiA(static_cast<const char*>(local_code), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNTexture2D_SetPrototype_OnSerialize
        case 0xAAE : {
            // Fetch texture2d
            const auto local_texture2d = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNTexture2DSerializeHandler_Create(HSPSubclass_LNTexture2DSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNTexture2DSerializeHandler*>(LNTexture2DSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNTexture2D_SetPrototype_OnSerialize(static_cast<LNHandle>(local_texture2d), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNShader_LoadA
        case 0x1E6 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch settings
            const auto local_settings = fetchVAInt(LN_NULL_HANDLE);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNShader_LoadA(static_cast<const char*>(local_filePath), static_cast<LNHandle>(local_settings), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNShader_SetFloatA
        case 0x1E9 : {
            // Fetch shader
            const auto local_shader = fetchVAInt();
            // Fetch parameterName
            const auto local_parameterName = fetchVAString();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNShader_SetFloatA(static_cast<LNHandle>(local_shader), static_cast<const char*>(local_parameterName), static_cast<float>(local_value));


            return true;
        }
        // LNShader_SetVector3A
        case 0x1EC : {
            // Fetch shader
            const auto local_shader = fetchVAInt();
            // Fetch parameterName
            const auto local_parameterName = fetchVAString();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNVector3);

            stat = LNShader_SetVector3A(static_cast<LNHandle>(local_shader), static_cast<const char*>(local_parameterName), reinterpret_cast<LNVector3*>(pval_value->pt));


            return true;
        }
        // LNShader_SetVector4A
        case 0x1EF : {
            // Fetch shader
            const auto local_shader = fetchVAInt();
            // Fetch parameterName
            const auto local_parameterName = fetchVAString();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNVector4);

            stat = LNShader_SetVector4A(static_cast<LNHandle>(local_shader), static_cast<const char*>(local_parameterName), reinterpret_cast<LNVector4*>(pval_value->pt));


            return true;
        }
        // LNShader_SetTextureA
        case 0x1F2 : {
            // Fetch shader
            const auto local_shader = fetchVAInt();
            // Fetch parameterName
            const auto local_parameterName = fetchVAString();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LNShader_SetTextureA(static_cast<LNHandle>(local_shader), static_cast<const char*>(local_parameterName), static_cast<LNHandle>(local_value));


            return true;
        }
        // LNShader_SetPrototype_OnSerialize
        case 0xAB9 : {
            // Fetch shader
            const auto local_shader = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNShaderSerializeHandler_Create(HSPSubclass_LNShaderSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNShaderSerializeHandler*>(LNShaderSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNShader_SetPrototype_OnSerialize(static_cast<LNHandle>(local_shader), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNRenderView_SetPrototype_OnSerialize
        case 0xAC4 : {
            // Fetch renderview
            const auto local_renderview = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNRenderViewSerializeHandler_Create(HSPSubclass_LNRenderViewSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNRenderViewSerializeHandler*>(LNRenderViewSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNRenderView_SetPrototype_OnSerialize(static_cast<LNHandle>(local_renderview), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNMaterial_Create
        case 0x217 : {
            // Fetch outMaterial
            PVal* pval_outMaterial;
            const APTR aptr_outMaterial = code_getva(&pval_outMaterial);
            LNHandle local_outMaterial;

            stat = LNMaterial_Create(&local_outMaterial);
            setVAInt(pval_outMaterial, aptr_outMaterial, local_outMaterial);

            return true;
        }
        // LNMaterial_SetMainTexture
        case 0x206 : {
            // Fetch material
            const auto local_material = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LNMaterial_SetMainTexture(static_cast<LNHandle>(local_material), static_cast<LNHandle>(local_value));


            return true;
        }
        // LNMaterial_GetMainTexture
        case 0x208 : {
            // Fetch material
            const auto local_material = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNMaterial_GetMainTexture(static_cast<LNHandle>(local_material), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMaterial_SetColor
        case 0x209 : {
            // Fetch material
            const auto local_material = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNMaterial_SetColor(static_cast<LNHandle>(local_material), reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNMaterial_SetRoughness
        case 0x20B : {
            // Fetch material
            const auto local_material = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNMaterial_SetRoughness(static_cast<LNHandle>(local_material), static_cast<float>(local_value));


            return true;
        }
        // LNMaterial_SetMetallic
        case 0x20D : {
            // Fetch material
            const auto local_material = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNMaterial_SetMetallic(static_cast<LNHandle>(local_material), static_cast<float>(local_value));


            return true;
        }
        // LNMaterial_SetEmissive
        case 0x20F : {
            // Fetch material
            const auto local_material = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNMaterial_SetEmissive(static_cast<LNHandle>(local_material), reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNMaterial_SetShadingModel
        case 0x211 : {
            // Fetch material
            const auto local_material = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNShadingModel>(fetchVAInt());

            stat = LNMaterial_SetShadingModel(static_cast<LNHandle>(local_material), static_cast<LNShadingModel>(local_value));


            return true;
        }
        // LNMaterial_GetShadingModel
        case 0x213 : {
            // Fetch material
            const auto local_material = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNShadingModel local_outReturn;

            stat = LNMaterial_GetShadingModel(static_cast<LNHandle>(local_material), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMaterial_SetShader
        case 0x214 : {
            // Fetch material
            const auto local_material = fetchVAInt();
            // Fetch shader
            const auto local_shader = fetchVAInt();

            stat = LNMaterial_SetShader(static_cast<LNHandle>(local_material), static_cast<LNHandle>(local_shader));


            return true;
        }
        // LNMaterial_GetShader
        case 0x216 : {
            // Fetch material
            const auto local_material = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNMaterial_GetShader(static_cast<LNHandle>(local_material), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMaterial_SetPrototype_OnSerialize
        case 0xACF : {
            // Fetch material
            const auto local_material = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNMaterialSerializeHandler_Create(HSPSubclass_LNMaterialSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNMaterialSerializeHandler*>(LNMaterialSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNMaterial_SetPrototype_OnSerialize(static_cast<LNHandle>(local_material), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNMeshNode_SetVisible
        case 0x219 : {
            // Fetch meshnode
            const auto local_meshnode = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNMeshNode_SetVisible(static_cast<LNHandle>(local_meshnode), static_cast<LNBool>(local_value));


            return true;
        }
        // LNMeshNode_IsVisible
        case 0x21B : {
            // Fetch meshnode
            const auto local_meshnode = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNMeshNode_IsVisible(static_cast<LNHandle>(local_meshnode), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMeshNode_SetPrototype_OnSerialize
        case 0xADA : {
            // Fetch meshnode
            const auto local_meshnode = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNMeshNodeSerializeHandler_Create(HSPSubclass_LNMeshNodeSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNMeshNodeSerializeHandler*>(LNMeshNodeSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNMeshNode_SetPrototype_OnSerialize(static_cast<LNHandle>(local_meshnode), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNAnimationController_AddClip
        case 0x21D : {
            // Fetch animationcontroller
            const auto local_animationcontroller = fetchVAInt();
            // Fetch animationClip
            const auto local_animationClip = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNAnimationController_AddClip(static_cast<LNHandle>(local_animationcontroller), static_cast<LNHandle>(local_animationClip), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNAnimationController_Play
        case 0x21F : {
            // Fetch animationcontroller
            const auto local_animationcontroller = fetchVAInt();
            // Fetch state
            const auto local_state = fetchVAInt();
            // Fetch duration
            const auto local_duration = fetchVADouble(0.300000);

            stat = LNAnimationController_Play(static_cast<LNHandle>(local_animationcontroller), static_cast<LNHandle>(local_state), static_cast<float>(local_duration));


            return true;
        }
        // LNAnimationController_SetPrototype_OnSerialize
        case 0xAE5 : {
            // Fetch animationcontroller
            const auto local_animationcontroller = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNAnimationControllerSerializeHandler_Create(HSPSubclass_LNAnimationControllerSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNAnimationControllerSerializeHandler*>(LNAnimationControllerSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNAnimationController_SetPrototype_OnSerialize(static_cast<LNHandle>(local_animationcontroller), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNMeshModel_LoadA
        case 0x223 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch settings
            const auto local_settings = fetchVAInt(LN_NULL_HANDLE);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNMeshModel_LoadA(static_cast<const char*>(local_filePath), static_cast<LNHandle>(local_settings), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMeshModel_FindNodeA
        case 0x226 : {
            // Fetch meshmodel
            const auto local_meshmodel = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNMeshModel_FindNodeA(static_cast<LNHandle>(local_meshmodel), static_cast<const char*>(local_name), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMeshModel_FindMaterialA
        case 0x228 : {
            // Fetch meshmodel
            const auto local_meshmodel = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNMeshModel_FindMaterialA(static_cast<LNHandle>(local_meshmodel), static_cast<const char*>(local_name), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMeshModel_MaterialCount
        case 0x22A : {
            // Fetch meshmodel
            const auto local_meshmodel = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LNMeshModel_MaterialCount(static_cast<LNHandle>(local_meshmodel), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMeshModel_Material
        case 0x22B : {
            // Fetch meshmodel
            const auto local_meshmodel = fetchVAInt();
            // Fetch index
            const auto local_index = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNMeshModel_Material(static_cast<LNHandle>(local_meshmodel), static_cast<int>(local_index), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMeshModel_GetAnimationController
        case 0x22D : {
            // Fetch meshmodel
            const auto local_meshmodel = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNMeshModel_GetAnimationController(static_cast<LNHandle>(local_meshmodel), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMeshModel_SetPrototype_OnSerialize
        case 0xAF0 : {
            // Fetch meshmodel
            const auto local_meshmodel = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNMeshModelSerializeHandler_Create(HSPSubclass_LNMeshModelSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNMeshModelSerializeHandler*>(LNMeshModelSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNMeshModel_SetPrototype_OnSerialize(static_cast<LNHandle>(local_meshmodel), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNMeshImportSettings_Create
        case 0x22F : {
            // Fetch outMeshImportSettings
            PVal* pval_outMeshImportSettings;
            const APTR aptr_outMeshImportSettings = code_getva(&pval_outMeshImportSettings);
            LNHandle local_outMeshImportSettings;

            stat = LNMeshImportSettings_Create(&local_outMeshImportSettings);
            setVAInt(pval_outMeshImportSettings, aptr_outMeshImportSettings, local_outMeshImportSettings);

            return true;
        }
        // LNMeshImportSettings_SetPrototype_OnSerialize
        case 0xAFB : {
            // Fetch meshimportsettings
            const auto local_meshimportsettings = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNMeshImportSettingsSerializeHandler_Create(HSPSubclass_LNMeshImportSettingsSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNMeshImportSettingsSerializeHandler*>(LNMeshImportSettingsSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNMeshImportSettings_SetPrototype_OnSerialize(static_cast<LNHandle>(local_meshimportsettings), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSkinnedMeshModel_SetPrototype_OnSerialize
        case 0xB06 : {
            // Fetch skinnedmeshmodel
            const auto local_skinnedmeshmodel = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSkinnedMeshModelSerializeHandler_Create(HSPSubclass_LNSkinnedMeshModelSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSkinnedMeshModelSerializeHandler*>(LNSkinnedMeshModelSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSkinnedMeshModel_SetPrototype_OnSerialize(static_cast<LNHandle>(local_skinnedmeshmodel), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNCollisionShape_SetPrototype_OnSerialize
        case 0xB11 : {
            // Fetch collisionshape
            const auto local_collisionshape = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNCollisionShapeSerializeHandler_Create(HSPSubclass_LNCollisionShapeSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionShapeSerializeHandler*>(LNCollisionShapeSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNCollisionShape_SetPrototype_OnSerialize(static_cast<LNHandle>(local_collisionshape), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNBoxCollisionShape_Create
        case 0x233 : {
            // Fetch size
            PVal* pval_size;
            CodeGetVA_TypeChecked(&pval_size, LNVector3);
            // Fetch outBoxCollisionShape
            PVal* pval_outBoxCollisionShape;
            const APTR aptr_outBoxCollisionShape = code_getva(&pval_outBoxCollisionShape);
            LNHandle local_outBoxCollisionShape;

            stat = LNBoxCollisionShape_Create(reinterpret_cast<LNVector3*>(pval_size->pt), &local_outBoxCollisionShape);
            setVAInt(pval_outBoxCollisionShape, aptr_outBoxCollisionShape, local_outBoxCollisionShape);

            return true;
        }
        // LNBoxCollisionShape_CreateWHD
        case 0x235 : {
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

            stat = LNBoxCollisionShape_CreateWHD(static_cast<float>(local_width), static_cast<float>(local_height), static_cast<float>(local_depth), &local_outBoxCollisionShape);
            setVAInt(pval_outBoxCollisionShape, aptr_outBoxCollisionShape, local_outBoxCollisionShape);

            return true;
        }
        // LNBoxCollisionShape_SetPrototype_OnSerialize
        case 0xB1C : {
            // Fetch boxcollisionshape
            const auto local_boxcollisionshape = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNBoxCollisionShapeSerializeHandler_Create(HSPSubclass_LNBoxCollisionShapeSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxCollisionShapeSerializeHandler*>(LNBoxCollisionShapeSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNBoxCollisionShape_SetPrototype_OnSerialize(static_cast<LNHandle>(local_boxcollisionshape), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNAnimationCurve_Evaluate
        case 0x243 : {
            // Fetch animationcurve
            const auto local_animationcurve = fetchVAInt();
            // Fetch time
            const auto local_time = fetchVADouble();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNAnimationCurve_Evaluate(static_cast<LNHandle>(local_animationcurve), static_cast<float>(local_time), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNAnimationCurve_SetPrototype_OnSerialize
        case 0xB27 : {
            // Fetch animationcurve
            const auto local_animationcurve = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNAnimationCurveSerializeHandler_Create(HSPSubclass_LNAnimationCurveSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNAnimationCurveSerializeHandler*>(LNAnimationCurveSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNAnimationCurve_SetPrototype_OnSerialize(static_cast<LNHandle>(local_animationcurve), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNKeyFrameAnimationCurve_Create
        case 0x250 : {
            // Fetch outKeyFrameAnimationCurve
            PVal* pval_outKeyFrameAnimationCurve;
            const APTR aptr_outKeyFrameAnimationCurve = code_getva(&pval_outKeyFrameAnimationCurve);
            LNHandle local_outKeyFrameAnimationCurve;

            stat = LNKeyFrameAnimationCurve_Create(&local_outKeyFrameAnimationCurve);
            setVAInt(pval_outKeyFrameAnimationCurve, aptr_outKeyFrameAnimationCurve, local_outKeyFrameAnimationCurve);

            return true;
        }
        // LNKeyFrameAnimationCurve_AddKeyFrame
        case 0x24B : {
            // Fetch keyframeanimationcurve
            const auto local_keyframeanimationcurve = fetchVAInt();
            // Fetch time
            const auto local_time = fetchVADouble();
            // Fetch value
            const auto local_value = fetchVADouble();
            // Fetch rightTangentMode
            const auto local_rightTangentMode = static_cast<LNTangentMode>(fetchVAInt(LN_TANGENT_MODE_LINEAR));
            // Fetch tangent
            const auto local_tangent = fetchVADouble(0.000000);

            stat = LNKeyFrameAnimationCurve_AddKeyFrame(static_cast<LNHandle>(local_keyframeanimationcurve), static_cast<float>(local_time), static_cast<float>(local_value), static_cast<LNTangentMode>(local_rightTangentMode), static_cast<float>(local_tangent));


            return true;
        }
        // LNKeyFrameAnimationCurve_SetPrototype_OnSerialize
        case 0xB32 : {
            // Fetch keyframeanimationcurve
            const auto local_keyframeanimationcurve = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNKeyFrameAnimationCurveSerializeHandler_Create(HSPSubclass_LNKeyFrameAnimationCurveSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNKeyFrameAnimationCurveSerializeHandler*>(LNKeyFrameAnimationCurveSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNKeyFrameAnimationCurve_SetPrototype_OnSerialize(static_cast<LNHandle>(local_keyframeanimationcurve), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNAnimationClip_LoadA
        case 0x252 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNAnimationClip_LoadA(static_cast<const char*>(local_filePath), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNAnimationClip_SetWrapMode
        case 0x254 : {
            // Fetch animationclip
            const auto local_animationclip = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNAnimationWrapMode>(fetchVAInt());

            stat = LNAnimationClip_SetWrapMode(static_cast<LNHandle>(local_animationclip), static_cast<LNAnimationWrapMode>(local_value));


            return true;
        }
        // LNAnimationClip_GetWrapMode
        case 0x256 : {
            // Fetch animationclip
            const auto local_animationclip = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNAnimationWrapMode local_outReturn;

            stat = LNAnimationClip_GetWrapMode(static_cast<LNHandle>(local_animationclip), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNAnimationClip_SetHierarchicalAnimationMode
        case 0x257 : {
            // Fetch animationclip
            const auto local_animationclip = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNHierarchicalAnimationMode>(fetchVAInt());

            stat = LNAnimationClip_SetHierarchicalAnimationMode(static_cast<LNHandle>(local_animationclip), static_cast<LNHierarchicalAnimationMode>(local_value));


            return true;
        }
        // LNAnimationClip_GetHierarchicalAnimationMode
        case 0x259 : {
            // Fetch animationclip
            const auto local_animationclip = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHierarchicalAnimationMode local_outReturn;

            stat = LNAnimationClip_GetHierarchicalAnimationMode(static_cast<LNHandle>(local_animationclip), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNAnimationClip_SetPrototype_OnSerialize
        case 0xB3D : {
            // Fetch animationclip
            const auto local_animationclip = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNAnimationClipSerializeHandler_Create(HSPSubclass_LNAnimationClipSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNAnimationClipSerializeHandler*>(LNAnimationClipSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNAnimationClip_SetPrototype_OnSerialize(static_cast<LNHandle>(local_animationclip), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNAnimationState_SetPrototype_OnSerialize
        case 0xB48 : {
            // Fetch animationstate
            const auto local_animationstate = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNAnimationStateSerializeHandler_Create(HSPSubclass_LNAnimationStateSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNAnimationStateSerializeHandler*>(LNAnimationStateSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNAnimationState_SetPrototype_OnSerialize(static_cast<LNHandle>(local_animationstate), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNEffectResource_SetPrototype_OnSerialize
        case 0xB53 : {
            // Fetch effectresource
            const auto local_effectresource = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNEffectResourceSerializeHandler_Create(HSPSubclass_LNEffectResourceSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNEffectResourceSerializeHandler*>(LNEffectResourceSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNEffectResource_SetPrototype_OnSerialize(static_cast<LNHandle>(local_effectresource), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNParticleEmitterModel_Create
        case 0x27E : {
            // Fetch outParticleEmitterModel
            PVal* pval_outParticleEmitterModel;
            const APTR aptr_outParticleEmitterModel = code_getva(&pval_outParticleEmitterModel);
            LNHandle local_outParticleEmitterModel;

            stat = LNParticleEmitterModel_Create(&local_outParticleEmitterModel);
            setVAInt(pval_outParticleEmitterModel, aptr_outParticleEmitterModel, local_outParticleEmitterModel);

            return true;
        }
        // LNParticleEmitterModel_SetMaxParticles
        case 0x266 : {
            // Fetch particleemittermodel
            const auto local_particleemittermodel = fetchVAInt();
            // Fetch count
            const auto local_count = fetchVAInt();

            stat = LNParticleEmitterModel_SetMaxParticles(static_cast<LNHandle>(local_particleemittermodel), static_cast<int>(local_count));


            return true;
        }
        // LNParticleEmitterModel_SetSpawnRate
        case 0x268 : {
            // Fetch particleemittermodel
            const auto local_particleemittermodel = fetchVAInt();
            // Fetch rate
            const auto local_rate = fetchVADouble();

            stat = LNParticleEmitterModel_SetSpawnRate(static_cast<LNHandle>(local_particleemittermodel), static_cast<float>(local_rate));


            return true;
        }
        // LNParticleEmitterModel_SetLifeTime
        case 0x26A : {
            // Fetch particleemittermodel
            const auto local_particleemittermodel = fetchVAInt();
            // Fetch time
            const auto local_time = fetchVADouble();

            stat = LNParticleEmitterModel_SetLifeTime(static_cast<LNHandle>(local_particleemittermodel), static_cast<float>(local_time));


            return true;
        }
        // LNParticleEmitterModel_SetupBoxShape
        case 0x26C : {
            // Fetch particleemittermodel
            const auto local_particleemittermodel = fetchVAInt();
            // Fetch size
            PVal* pval_size;
            CodeGetVA_TypeChecked(&pval_size, LNVector3);

            stat = LNParticleEmitterModel_SetupBoxShape(static_cast<LNHandle>(local_particleemittermodel), reinterpret_cast<LNVector3*>(pval_size->pt));


            return true;
        }
        // LNParticleEmitterModel_SetSize
        case 0x26E : {
            // Fetch particleemittermodel
            const auto local_particleemittermodel = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNParticleEmitterModel_SetSize(static_cast<LNHandle>(local_particleemittermodel), static_cast<float>(local_value));


            return true;
        }
        // LNParticleEmitterModel_SetSizeVelocity
        case 0x270 : {
            // Fetch particleemittermodel
            const auto local_particleemittermodel = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNParticleEmitterModel_SetSizeVelocity(static_cast<LNHandle>(local_particleemittermodel), static_cast<float>(local_value));


            return true;
        }
        // LNParticleEmitterModel_SetSizeAcceleration
        case 0x272 : {
            // Fetch particleemittermodel
            const auto local_particleemittermodel = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNParticleEmitterModel_SetSizeAcceleration(static_cast<LNHandle>(local_particleemittermodel), static_cast<float>(local_value));


            return true;
        }
        // LNParticleEmitterModel_SetForwardVelocityMin
        case 0x274 : {
            // Fetch particleemittermodel
            const auto local_particleemittermodel = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNParticleEmitterModel_SetForwardVelocityMin(static_cast<LNHandle>(local_particleemittermodel), static_cast<float>(local_value));


            return true;
        }
        // LNParticleEmitterModel_SetForwardVelocityMax
        case 0x276 : {
            // Fetch particleemittermodel
            const auto local_particleemittermodel = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNParticleEmitterModel_SetForwardVelocityMax(static_cast<LNHandle>(local_particleemittermodel), static_cast<float>(local_value));


            return true;
        }
        // LNParticleEmitterModel_SetForwardScale
        case 0x278 : {
            // Fetch particleemittermodel
            const auto local_particleemittermodel = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNParticleEmitterModel_SetForwardScale(static_cast<LNHandle>(local_particleemittermodel), static_cast<float>(local_value));


            return true;
        }
        // LNParticleEmitterModel_SetGeometryDirection
        case 0x27A : {
            // Fetch particleemittermodel
            const auto local_particleemittermodel = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNParticleGeometryDirection>(fetchVAInt());

            stat = LNParticleEmitterModel_SetGeometryDirection(static_cast<LNHandle>(local_particleemittermodel), static_cast<LNParticleGeometryDirection>(local_value));


            return true;
        }
        // LNParticleEmitterModel_SetupSpriteModule
        case 0x27C : {
            // Fetch particleemittermodel
            const auto local_particleemittermodel = fetchVAInt();
            // Fetch material
            const auto local_material = fetchVAInt();

            stat = LNParticleEmitterModel_SetupSpriteModule(static_cast<LNHandle>(local_particleemittermodel), static_cast<LNHandle>(local_material));


            return true;
        }
        // LNParticleEmitterModel_SetPrototype_OnSerialize
        case 0xB5E : {
            // Fetch particleemittermodel
            const auto local_particleemittermodel = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNParticleEmitterModelSerializeHandler_Create(HSPSubclass_LNParticleEmitterModelSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleEmitterModelSerializeHandler*>(LNParticleEmitterModelSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNParticleEmitterModel_SetPrototype_OnSerialize(static_cast<LNHandle>(local_particleemittermodel), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNParticleModel_Create
        case 0x285 : {
            // Fetch outParticleModel
            PVal* pval_outParticleModel;
            const APTR aptr_outParticleModel = code_getva(&pval_outParticleModel);
            LNHandle local_outParticleModel;

            stat = LNParticleModel_Create(&local_outParticleModel);
            setVAInt(pval_outParticleModel, aptr_outParticleModel, local_outParticleModel);

            return true;
        }
        // LNParticleModel_SetLoop
        case 0x280 : {
            // Fetch particlemodel
            const auto local_particlemodel = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNParticleModel_SetLoop(static_cast<LNHandle>(local_particlemodel), static_cast<LNBool>(local_value));


            return true;
        }
        // LNParticleModel_IsLoop
        case 0x282 : {
            // Fetch particlemodel
            const auto local_particlemodel = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNParticleModel_IsLoop(static_cast<LNHandle>(local_particlemodel), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNParticleModel_AddEmitter
        case 0x283 : {
            // Fetch particlemodel
            const auto local_particlemodel = fetchVAInt();
            // Fetch emitter
            const auto local_emitter = fetchVAInt();

            stat = LNParticleModel_AddEmitter(static_cast<LNHandle>(local_particlemodel), static_cast<LNHandle>(local_emitter));


            return true;
        }
        // LNParticleModel_SetPrototype_OnSerialize
        case 0xB69 : {
            // Fetch particlemodel
            const auto local_particlemodel = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNParticleModelSerializeHandler_Create(HSPSubclass_LNParticleModelSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleModelSerializeHandler*>(LNParticleModelSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNParticleModel_SetPrototype_OnSerialize(static_cast<LNHandle>(local_particlemodel), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNComponent_SetPrototype_OnSerialize
        case 0xB74 : {
            // Fetch component
            const auto local_component = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNComponentSerializeHandler_Create(HSPSubclass_LNComponentSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNComponentSerializeHandler*>(LNComponentSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNComponent_SetPrototype_OnSerialize(static_cast<LNHandle>(local_component), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNVisualComponent_SetVisible
        case 0x288 : {
            // Fetch visualcomponent
            const auto local_visualcomponent = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNVisualComponent_SetVisible(static_cast<LNHandle>(local_visualcomponent), static_cast<LNBool>(local_value));


            return true;
        }
        // LNVisualComponent_IsVisible
        case 0x28A : {
            // Fetch visualcomponent
            const auto local_visualcomponent = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNVisualComponent_IsVisible(static_cast<LNHandle>(local_visualcomponent), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNVisualComponent_SetPrototype_OnSerialize
        case 0xB7F : {
            // Fetch visualcomponent
            const auto local_visualcomponent = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNVisualComponentSerializeHandler_Create(HSPSubclass_LNVisualComponentSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualComponentSerializeHandler*>(LNVisualComponentSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNVisualComponent_SetPrototype_OnSerialize(static_cast<LNHandle>(local_visualcomponent), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSpriteComponent_SetTexture
        case 0x28C : {
            // Fetch spritecomponent
            const auto local_spritecomponent = fetchVAInt();
            // Fetch texture
            const auto local_texture = fetchVAInt();

            stat = LNSpriteComponent_SetTexture(static_cast<LNHandle>(local_spritecomponent), static_cast<LNHandle>(local_texture));


            return true;
        }
        // LNSpriteComponent_SetPrototype_OnSerialize
        case 0xB8A : {
            // Fetch spritecomponent
            const auto local_spritecomponent = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSpriteComponentSerializeHandler_Create(HSPSubclass_LNSpriteComponentSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSpriteComponentSerializeHandler*>(LNSpriteComponentSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSpriteComponent_SetPrototype_OnSerialize(static_cast<LNHandle>(local_spritecomponent), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNCollisionEventHandler_Create
        case 0x7E1 : {
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
        case 0x2AC : {
            // Fetch outCharacterController
            PVal* pval_outCharacterController;
            const APTR aptr_outCharacterController = code_getva(&pval_outCharacterController);
            LNHandle local_outCharacterController;

            stat = LNCharacterController_Create(&local_outCharacterController);
            setVAInt(pval_outCharacterController, aptr_outCharacterController, local_outCharacterController);

            return true;
        }
        // LNCharacterController_SetWalkVelocity
        case 0x296 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNCharacterController_SetWalkVelocity(static_cast<LNHandle>(local_charactercontroller), static_cast<float>(local_value));


            return true;
        }
        // LNCharacterController_GetWalkVelocity
        case 0x298 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNCharacterController_GetWalkVelocity(static_cast<LNHandle>(local_charactercontroller), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNCharacterController_SetVelocity
        case 0x299 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNVector3);

            stat = LNCharacterController_SetVelocity(static_cast<LNHandle>(local_charactercontroller), reinterpret_cast<LNVector3*>(pval_value->pt));


            return true;
        }
        // LNCharacterController_GetVelocity
        case 0x29B : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNCharacterController_GetVelocity(static_cast<LNHandle>(local_charactercontroller), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNCharacterController_SetInputControlEnabled
        case 0x29C : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNCharacterController_SetInputControlEnabled(static_cast<LNHandle>(local_charactercontroller), static_cast<LNBool>(local_value));


            return true;
        }
        // LNCharacterController_SetCameraControlEnabled
        case 0x29E : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNCharacterController_SetCameraControlEnabled(static_cast<LNHandle>(local_charactercontroller), static_cast<LNBool>(local_value));


            return true;
        }
        // LNCharacterController_SetHeight
        case 0x2A0 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNCharacterController_SetHeight(static_cast<LNHandle>(local_charactercontroller), static_cast<float>(local_value));


            return true;
        }
        // LNCharacterController_GetHeight
        case 0x2A2 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNCharacterController_GetHeight(static_cast<LNHandle>(local_charactercontroller), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNCharacterController_SetCameraRadius
        case 0x2A3 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNCharacterController_SetCameraRadius(static_cast<LNHandle>(local_charactercontroller), static_cast<float>(local_value));


            return true;
        }
        // LNCharacterController_GetCameraRadius
        case 0x2A5 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNCharacterController_GetCameraRadius(static_cast<LNHandle>(local_charactercontroller), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNCharacterController_SetCollisionEnter
        case 0x2A6 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNCollisionEventHandler_Create(HSPSubclass_LNCollisionEventHandler_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionEventHandler*>(LNCollisionEventHandler_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }

            stat = LNCharacterController_SetCollisionEnter(static_cast<LNHandle>(local_charactercontroller), localDelegate_handler);
            LNObject_Release(localDelegate_handler);

            return true;
        }
        // LNCharacterController_SetCollisionLeave
        case 0x2A8 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNCollisionEventHandler_Create(HSPSubclass_LNCollisionEventHandler_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionEventHandler*>(LNCollisionEventHandler_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }

            stat = LNCharacterController_SetCollisionLeave(static_cast<LNHandle>(local_charactercontroller), localDelegate_handler);
            LNObject_Release(localDelegate_handler);

            return true;
        }
        // LNCharacterController_SetCollisionStay
        case 0x2AA : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNCollisionEventHandler_Create(HSPSubclass_LNCollisionEventHandler_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionEventHandler*>(LNCollisionEventHandler_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }

            stat = LNCharacterController_SetCollisionStay(static_cast<LNHandle>(local_charactercontroller), localDelegate_handler);
            LNObject_Release(localDelegate_handler);

            return true;
        }
        // LNCharacterController_SetPrototype_OnSerialize
        case 0xB95 : {
            // Fetch charactercontroller
            const auto local_charactercontroller = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNCharacterControllerSerializeHandler_Create(HSPSubclass_LNCharacterControllerSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCharacterControllerSerializeHandler*>(LNCharacterControllerSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNCharacterController_SetPrototype_OnSerialize(static_cast<LNHandle>(local_charactercontroller), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNWorld_Add
        case 0x2AE : {
            // Fetch world
            const auto local_world = fetchVAInt();
            // Fetch obj
            const auto local_obj = fetchVAInt();

            stat = LNWorld_Add(static_cast<LNHandle>(local_world), static_cast<LNHandle>(local_obj));


            return true;
        }
        // LNWorld_SetPrototype_OnSerialize
        case 0xBA0 : {
            // Fetch world
            const auto local_world = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNWorldSerializeHandler_Create(HSPSubclass_LNWorldSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldSerializeHandler*>(LNWorldSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNWorld_SetPrototype_OnSerialize(static_cast<LNHandle>(local_world), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNComponentList_GetLength
        case 0x801 : {
            // Fetch componentlist
            const auto local_componentlist = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LNComponentList_GetLength(static_cast<LNHandle>(local_componentlist), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNComponentList_GetItem
        case 0x803 : {
            // Fetch componentlist
            const auto local_componentlist = fetchVAInt();
            // Fetch index
            const auto local_index = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNComponentList_GetItem(static_cast<LNHandle>(local_componentlist), static_cast<int>(local_index), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNComponentList_SetPrototype_OnSerialize
        case 0xBAB : {
            // Fetch componentlist
            const auto local_componentlist = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNComponentListSerializeHandler_Create(HSPSubclass_LNComponentListSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNComponentListSerializeHandler*>(LNComponentListSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNComponentList_SetPrototype_OnSerialize(static_cast<LNHandle>(local_componentlist), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNWorldObject_Create
        case 0x2E7 : {
            // Fetch outWorldObject
            PVal* pval_outWorldObject;
            const APTR aptr_outWorldObject = code_getva(&pval_outWorldObject);
            LNHandle local_outWorldObject;

            stat = LNWorldObject_Create(&local_outWorldObject);
            setVAInt(pval_outWorldObject, aptr_outWorldObject, local_outWorldObject);

            return true;
        }
        // LNWorldObject_SetPosition
        case 0x2B2 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch pos
            PVal* pval_pos;
            CodeGetVA_TypeChecked(&pval_pos, LNVector3);

            stat = LNWorldObject_SetPosition(static_cast<LNHandle>(local_worldobject), reinterpret_cast<LNVector3*>(pval_pos->pt));


            return true;
        }
        // LNWorldObject_SetPositionXYZ
        case 0x2B4 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble(0.000000);

            stat = LNWorldObject_SetPositionXYZ(static_cast<LNHandle>(local_worldobject), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_z));


            return true;
        }
        // LNWorldObject_GetPosition
        case 0x2B8 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNWorldObject_GetPosition(static_cast<LNHandle>(local_worldobject), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNWorldObject_SetRotation
        case 0x2B9 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch rot
            PVal* pval_rot;
            CodeGetVA_TypeChecked(&pval_rot, LNQuaternion);

            stat = LNWorldObject_SetRotation(static_cast<LNHandle>(local_worldobject), reinterpret_cast<LNQuaternion*>(pval_rot->pt));


            return true;
        }
        // LNWorldObject_SetRotationXYZ
        case 0x2BB : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LNWorldObject_SetRotationXYZ(static_cast<LNHandle>(local_worldobject), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_z));


            return true;
        }
        // LNWorldObject_GetRotation
        case 0x2BF : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNQuaternion local_outReturn;

            stat = LNWorldObject_GetRotation(static_cast<LNHandle>(local_worldobject), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNQuaternion_typeid(), &local_outReturn);

            return true;
        }
        // LNWorldObject_SetScale
        case 0x2C0 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch scale
            PVal* pval_scale;
            CodeGetVA_TypeChecked(&pval_scale, LNVector3);

            stat = LNWorldObject_SetScale(static_cast<LNHandle>(local_worldobject), reinterpret_cast<LNVector3*>(pval_scale->pt));


            return true;
        }
        // LNWorldObject_SetScaleS
        case 0x2C2 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch xyz
            const auto local_xyz = fetchVADouble();

            stat = LNWorldObject_SetScaleS(static_cast<LNHandle>(local_worldobject), static_cast<float>(local_xyz));


            return true;
        }
        // LNWorldObject_SetScaleXYZ
        case 0x2C4 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble(1.000000);

            stat = LNWorldObject_SetScaleXYZ(static_cast<LNHandle>(local_worldobject), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_z));


            return true;
        }
        // LNWorldObject_GetScale
        case 0x2C8 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNWorldObject_GetScale(static_cast<LNHandle>(local_worldobject), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNWorldObject_SetCenterPoint
        case 0x2C9 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNVector3);

            stat = LNWorldObject_SetCenterPoint(static_cast<LNHandle>(local_worldobject), reinterpret_cast<LNVector3*>(pval_value->pt));


            return true;
        }
        // LNWorldObject_SetCenterPointXYZ
        case 0x2CB : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble(0.000000);

            stat = LNWorldObject_SetCenterPointXYZ(static_cast<LNHandle>(local_worldobject), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_z));


            return true;
        }
        // LNWorldObject_GetCenterPoint
        case 0x2CF : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNWorldObject_GetCenterPoint(static_cast<LNHandle>(local_worldobject), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNWorldObject_LookAt
        case 0x2D0 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch target
            PVal* pval_target;
            CodeGetVA_TypeChecked(&pval_target, LNVector3);

            stat = LNWorldObject_LookAt(static_cast<LNHandle>(local_worldobject), reinterpret_cast<LNVector3*>(pval_target->pt));


            return true;
        }
        // LNWorldObject_LookAtXYZ
        case 0x2D2 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LNWorldObject_LookAtXYZ(static_cast<LNHandle>(local_worldobject), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_z));


            return true;
        }
        // LNWorldObject_AddComponent
        case 0x2D6 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch component
            const auto local_component = fetchVAInt();

            stat = LNWorldObject_AddComponent(static_cast<LNHandle>(local_worldobject), static_cast<LNHandle>(local_component));


            return true;
        }
        // LNWorldObject_RemoveComponent
        case 0x2D8 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch component
            const auto local_component = fetchVAInt();

            stat = LNWorldObject_RemoveComponent(static_cast<LNHandle>(local_worldobject), static_cast<LNHandle>(local_component));


            return true;
        }
        // LNWorldObject_AddTagA
        case 0x2DA : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch tag
            const auto local_tag = fetchVAString();

            stat = LNWorldObject_AddTagA(static_cast<LNHandle>(local_worldobject), static_cast<const char*>(local_tag));


            return true;
        }
        // LNWorldObject_RemoveTagA
        case 0x2DC : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch tag
            const auto local_tag = fetchVAString();

            stat = LNWorldObject_RemoveTagA(static_cast<LNHandle>(local_worldobject), static_cast<const char*>(local_tag));


            return true;
        }
        // LNWorldObject_HasTagA
        case 0x2DE : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch tag
            const auto local_tag = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNWorldObject_HasTagA(static_cast<LNHandle>(local_worldobject), static_cast<const char*>(local_tag), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNWorldObject_Destroy
        case 0x2E0 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();

            stat = LNWorldObject_Destroy(static_cast<LNHandle>(local_worldobject));


            return true;
        }
        // LNWorldObject_GetComponents
        case 0x2E1 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNWorldObject_GetComponents(static_cast<LNHandle>(local_worldobject), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNWorldObject_AddInto
        case 0x2E2 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch world
            const auto local_world = fetchVAInt(LN_NULL_HANDLE);

            stat = LNWorldObject_AddInto(static_cast<LNHandle>(local_worldobject), static_cast<LNHandle>(local_world));


            return true;
        }
        // LNWorldObject_SetPrototype_OnSerialize
        case 0xBB6 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNWorldObjectSerializeHandler_Create(HSPSubclass_LNWorldObjectSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldObjectSerializeHandler*>(LNWorldObjectSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNWorldObject_SetPrototype_OnSerialize(static_cast<LNHandle>(local_worldobject), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNWorldObject_SetPrototype_OnPreUpdate
        case 0xBC1 : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNWorldObjectPreUpdateHandler_Create(HSPSubclass_LNWorldObjectPreUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldObjectPreUpdateHandler*>(LNWorldObjectPreUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNWorldObject_SetPrototype_OnPreUpdate(static_cast<LNHandle>(local_worldobject), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNWorldObject_SetPrototype_OnUpdate
        case 0xBCC : {
            // Fetch worldobject
            const auto local_worldobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNWorldObjectUpdateHandler_Create(HSPSubclass_LNWorldObjectUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldObjectUpdateHandler*>(LNWorldObjectUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNWorldObject_SetPrototype_OnUpdate(static_cast<LNHandle>(local_worldobject), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNVisualObject_SetVisible
        case 0x2E9 : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNVisualObject_SetVisible(static_cast<LNHandle>(local_visualobject), static_cast<LNBool>(local_value));


            return true;
        }
        // LNVisualObject_IsVisible
        case 0x2EB : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNVisualObject_IsVisible(static_cast<LNHandle>(local_visualobject), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNVisualObject_SetBlendMode2
        case 0x2EC : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBlendMode>(fetchVAInt());

            stat = LNVisualObject_SetBlendMode2(static_cast<LNHandle>(local_visualobject), static_cast<LNBlendMode>(local_value));


            return true;
        }
        // LNVisualObject_SetOpacity
        case 0x2EE : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNVisualObject_SetOpacity(static_cast<LNHandle>(local_visualobject), static_cast<float>(local_value));


            return true;
        }
        // LNVisualObject_GetOpacity
        case 0x2F0 : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNVisualObject_GetOpacity(static_cast<LNHandle>(local_visualobject), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNVisualObject_SetPrototype_OnSerialize
        case 0xBD7 : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNVisualObjectSerializeHandler_Create(HSPSubclass_LNVisualObjectSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualObjectSerializeHandler*>(LNVisualObjectSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNVisualObject_SetPrototype_OnSerialize(static_cast<LNHandle>(local_visualobject), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNVisualObject_SetPrototype_OnPreUpdate
        case 0xBE2 : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNVisualObjectPreUpdateHandler_Create(HSPSubclass_LNVisualObjectPreUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualObjectPreUpdateHandler*>(LNVisualObjectPreUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNVisualObject_SetPrototype_OnPreUpdate(static_cast<LNHandle>(local_visualobject), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNVisualObject_SetPrototype_OnUpdate
        case 0xBED : {
            // Fetch visualobject
            const auto local_visualobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNVisualObjectUpdateHandler_Create(HSPSubclass_LNVisualObjectUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualObjectUpdateHandler*>(LNVisualObjectUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNVisualObject_SetPrototype_OnUpdate(static_cast<LNHandle>(local_visualobject), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNCamera_SetPrototype_OnSerialize
        case 0xBF8 : {
            // Fetch camera
            const auto local_camera = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNCameraSerializeHandler_Create(HSPSubclass_LNCameraSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraSerializeHandler*>(LNCameraSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNCamera_SetPrototype_OnSerialize(static_cast<LNHandle>(local_camera), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNCamera_SetPrototype_OnPreUpdate
        case 0xC03 : {
            // Fetch camera
            const auto local_camera = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNCameraPreUpdateHandler_Create(HSPSubclass_LNCameraPreUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraPreUpdateHandler*>(LNCameraPreUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNCamera_SetPrototype_OnPreUpdate(static_cast<LNHandle>(local_camera), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNCamera_SetPrototype_OnUpdate
        case 0xC0E : {
            // Fetch camera
            const auto local_camera = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNCameraUpdateHandler_Create(HSPSubclass_LNCameraUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraUpdateHandler*>(LNCameraUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNCamera_SetPrototype_OnUpdate(static_cast<LNHandle>(local_camera), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNEnvironmentLight_SetEnabled
        case 0x2F3 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNEnvironmentLight_SetEnabled(static_cast<LNHandle>(local_environmentlight), static_cast<LNBool>(local_enabled));


            return true;
        }
        // LNEnvironmentLight_IsEnabled
        case 0x2F5 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNEnvironmentLight_IsEnabled(static_cast<LNHandle>(local_environmentlight), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetColor
        case 0x2F6 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNEnvironmentLight_SetColor(static_cast<LNHandle>(local_environmentlight), reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNEnvironmentLight_GetColor
        case 0x2F8 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNEnvironmentLight_GetColor(static_cast<LNHandle>(local_environmentlight), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetAmbientColor
        case 0x2F9 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNEnvironmentLight_SetAmbientColor(static_cast<LNHandle>(local_environmentlight), reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNEnvironmentLight_GetAmbientColor
        case 0x2FB : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNEnvironmentLight_GetAmbientColor(static_cast<LNHandle>(local_environmentlight), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNEnvironmentLight_GetSkyColor
        case 0x2FC : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNEnvironmentLight_GetSkyColor(static_cast<LNHandle>(local_environmentlight), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetSkyColor
        case 0x2FD : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNEnvironmentLight_SetSkyColor(static_cast<LNHandle>(local_environmentlight), reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNEnvironmentLight_GetGroundColor
        case 0x2FF : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNEnvironmentLight_GetGroundColor(static_cast<LNHandle>(local_environmentlight), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetGroundColor
        case 0x300 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNEnvironmentLight_SetGroundColor(static_cast<LNHandle>(local_environmentlight), reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNEnvironmentLight_SetIntensity
        case 0x302 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LNEnvironmentLight_SetIntensity(static_cast<LNHandle>(local_environmentlight), static_cast<float>(local_intensity));


            return true;
        }
        // LNEnvironmentLight_GetIntensity
        case 0x304 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNEnvironmentLight_GetIntensity(static_cast<LNHandle>(local_environmentlight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetShadowEffectiveDistance
        case 0x305 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNEnvironmentLight_SetShadowEffectiveDistance(static_cast<LNHandle>(local_environmentlight), static_cast<float>(local_value));


            return true;
        }
        // LNEnvironmentLight_GetShadowEffectiveDistance
        case 0x307 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNEnvironmentLight_GetShadowEffectiveDistance(static_cast<LNHandle>(local_environmentlight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetShadowEffectiveDepth
        case 0x308 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNEnvironmentLight_SetShadowEffectiveDepth(static_cast<LNHandle>(local_environmentlight), static_cast<float>(local_value));


            return true;
        }
        // LNEnvironmentLight_GetShadowEffectiveDepth
        case 0x30A : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNEnvironmentLight_GetShadowEffectiveDepth(static_cast<LNHandle>(local_environmentlight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEnvironmentLight_SetPrototype_OnSerialize
        case 0xC19 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNEnvironmentLightSerializeHandler_Create(HSPSubclass_LNEnvironmentLightSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNEnvironmentLightSerializeHandler*>(LNEnvironmentLightSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNEnvironmentLight_SetPrototype_OnSerialize(static_cast<LNHandle>(local_environmentlight), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNEnvironmentLight_SetPrototype_OnPreUpdate
        case 0xC24 : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNEnvironmentLightPreUpdateHandler_Create(HSPSubclass_LNEnvironmentLightPreUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNEnvironmentLightPreUpdateHandler*>(LNEnvironmentLightPreUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNEnvironmentLight_SetPrototype_OnPreUpdate(static_cast<LNHandle>(local_environmentlight), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNEnvironmentLight_SetPrototype_OnUpdate
        case 0xC2F : {
            // Fetch environmentlight
            const auto local_environmentlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNEnvironmentLightUpdateHandler_Create(HSPSubclass_LNEnvironmentLightUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNEnvironmentLightUpdateHandler*>(LNEnvironmentLightUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNEnvironmentLight_SetPrototype_OnUpdate(static_cast<LNHandle>(local_environmentlight), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNDirectionalLight_Create
        case 0x31B : {
            // Fetch outDirectionalLight
            PVal* pval_outDirectionalLight;
            const APTR aptr_outDirectionalLight = code_getva(&pval_outDirectionalLight);
            LNHandle local_outDirectionalLight;

            stat = LNDirectionalLight_Create(&local_outDirectionalLight);
            setVAInt(pval_outDirectionalLight, aptr_outDirectionalLight, local_outDirectionalLight);

            return true;
        }
        // LNDirectionalLight_CreateWithColor
        case 0x31C : {
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);
            // Fetch outDirectionalLight
            PVal* pval_outDirectionalLight;
            const APTR aptr_outDirectionalLight = code_getva(&pval_outDirectionalLight);
            LNHandle local_outDirectionalLight;

            stat = LNDirectionalLight_CreateWithColor(reinterpret_cast<LNColor*>(pval_color->pt), &local_outDirectionalLight);
            setVAInt(pval_outDirectionalLight, aptr_outDirectionalLight, local_outDirectionalLight);

            return true;
        }
        // LNDirectionalLight_SetEnabled
        case 0x30C : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNDirectionalLight_SetEnabled(static_cast<LNHandle>(local_directionallight), static_cast<LNBool>(local_enabled));


            return true;
        }
        // LNDirectionalLight_IsEnabled
        case 0x30E : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNDirectionalLight_IsEnabled(static_cast<LNHandle>(local_directionallight), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNDirectionalLight_SetColor
        case 0x30F : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);

            stat = LNDirectionalLight_SetColor(static_cast<LNHandle>(local_directionallight), reinterpret_cast<LNColor*>(pval_color->pt));


            return true;
        }
        // LNDirectionalLight_GetColor
        case 0x311 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNDirectionalLight_GetColor(static_cast<LNHandle>(local_directionallight), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNDirectionalLight_SetIntensity
        case 0x312 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LNDirectionalLight_SetIntensity(static_cast<LNHandle>(local_directionallight), static_cast<float>(local_intensity));


            return true;
        }
        // LNDirectionalLight_GetIntensity
        case 0x314 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNDirectionalLight_GetIntensity(static_cast<LNHandle>(local_directionallight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNDirectionalLight_SetShadowEffectiveDistance
        case 0x315 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNDirectionalLight_SetShadowEffectiveDistance(static_cast<LNHandle>(local_directionallight), static_cast<float>(local_value));


            return true;
        }
        // LNDirectionalLight_GetShadowEffectiveDistance
        case 0x317 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNDirectionalLight_GetShadowEffectiveDistance(static_cast<LNHandle>(local_directionallight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNDirectionalLight_SetShadowEffectiveDepth
        case 0x318 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNDirectionalLight_SetShadowEffectiveDepth(static_cast<LNHandle>(local_directionallight), static_cast<float>(local_value));


            return true;
        }
        // LNDirectionalLight_GetShadowEffectiveDepth
        case 0x31A : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNDirectionalLight_GetShadowEffectiveDepth(static_cast<LNHandle>(local_directionallight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNDirectionalLight_SetPrototype_OnSerialize
        case 0xC3A : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNDirectionalLightSerializeHandler_Create(HSPSubclass_LNDirectionalLightSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNDirectionalLightSerializeHandler*>(LNDirectionalLightSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNDirectionalLight_SetPrototype_OnSerialize(static_cast<LNHandle>(local_directionallight), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNDirectionalLight_SetPrototype_OnPreUpdate
        case 0xC45 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNDirectionalLightPreUpdateHandler_Create(HSPSubclass_LNDirectionalLightPreUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNDirectionalLightPreUpdateHandler*>(LNDirectionalLightPreUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNDirectionalLight_SetPrototype_OnPreUpdate(static_cast<LNHandle>(local_directionallight), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNDirectionalLight_SetPrototype_OnUpdate
        case 0xC50 : {
            // Fetch directionallight
            const auto local_directionallight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNDirectionalLightUpdateHandler_Create(HSPSubclass_LNDirectionalLightUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNDirectionalLightUpdateHandler*>(LNDirectionalLightUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNDirectionalLight_SetPrototype_OnUpdate(static_cast<LNHandle>(local_directionallight), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNPointLight_Create
        case 0x32E : {
            // Fetch outPointLight
            PVal* pval_outPointLight;
            const APTR aptr_outPointLight = code_getva(&pval_outPointLight);
            LNHandle local_outPointLight;

            stat = LNPointLight_Create(&local_outPointLight);
            setVAInt(pval_outPointLight, aptr_outPointLight, local_outPointLight);

            return true;
        }
        // LNPointLight_CreateWithColorAndRange
        case 0x32F : {
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);
            // Fetch range
            const auto local_range = fetchVADouble();
            // Fetch outPointLight
            PVal* pval_outPointLight;
            const APTR aptr_outPointLight = code_getva(&pval_outPointLight);
            LNHandle local_outPointLight;

            stat = LNPointLight_CreateWithColorAndRange(reinterpret_cast<LNColor*>(pval_color->pt), static_cast<float>(local_range), &local_outPointLight);
            setVAInt(pval_outPointLight, aptr_outPointLight, local_outPointLight);

            return true;
        }
        // LNPointLight_SetEnabled
        case 0x31F : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNPointLight_SetEnabled(static_cast<LNHandle>(local_pointlight), static_cast<LNBool>(local_enabled));


            return true;
        }
        // LNPointLight_IsEnabled
        case 0x321 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNPointLight_IsEnabled(static_cast<LNHandle>(local_pointlight), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNPointLight_SetColor
        case 0x322 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);

            stat = LNPointLight_SetColor(static_cast<LNHandle>(local_pointlight), reinterpret_cast<LNColor*>(pval_color->pt));


            return true;
        }
        // LNPointLight_GetColor
        case 0x324 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNPointLight_GetColor(static_cast<LNHandle>(local_pointlight), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNPointLight_SetIntensity
        case 0x325 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LNPointLight_SetIntensity(static_cast<LNHandle>(local_pointlight), static_cast<float>(local_intensity));


            return true;
        }
        // LNPointLight_GetIntensity
        case 0x327 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNPointLight_GetIntensity(static_cast<LNHandle>(local_pointlight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNPointLight_SetRange
        case 0x328 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch range
            const auto local_range = fetchVADouble();

            stat = LNPointLight_SetRange(static_cast<LNHandle>(local_pointlight), static_cast<float>(local_range));


            return true;
        }
        // LNPointLight_GetRange
        case 0x32A : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNPointLight_GetRange(static_cast<LNHandle>(local_pointlight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNPointLight_SetAttenuation
        case 0x32B : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch attenuation
            const auto local_attenuation = fetchVADouble();

            stat = LNPointLight_SetAttenuation(static_cast<LNHandle>(local_pointlight), static_cast<float>(local_attenuation));


            return true;
        }
        // LNPointLight_GetAttenuation
        case 0x32D : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNPointLight_GetAttenuation(static_cast<LNHandle>(local_pointlight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNPointLight_SetPrototype_OnSerialize
        case 0xC5B : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPointLightSerializeHandler_Create(HSPSubclass_LNPointLightSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPointLightSerializeHandler*>(LNPointLightSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNPointLight_SetPrototype_OnSerialize(static_cast<LNHandle>(local_pointlight), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNPointLight_SetPrototype_OnPreUpdate
        case 0xC66 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPointLightPreUpdateHandler_Create(HSPSubclass_LNPointLightPreUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPointLightPreUpdateHandler*>(LNPointLightPreUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNPointLight_SetPrototype_OnPreUpdate(static_cast<LNHandle>(local_pointlight), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNPointLight_SetPrototype_OnUpdate
        case 0xC71 : {
            // Fetch pointlight
            const auto local_pointlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPointLightUpdateHandler_Create(HSPSubclass_LNPointLightUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPointLightUpdateHandler*>(LNPointLightUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNPointLight_SetPrototype_OnUpdate(static_cast<LNHandle>(local_pointlight), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSpotLight_Create
        case 0x348 : {
            // Fetch outSpotLight
            PVal* pval_outSpotLight;
            const APTR aptr_outSpotLight = code_getva(&pval_outSpotLight);
            LNHandle local_outSpotLight;

            stat = LNSpotLight_Create(&local_outSpotLight);
            setVAInt(pval_outSpotLight, aptr_outSpotLight, local_outSpotLight);

            return true;
        }
        // LNSpotLight_CreateWithColorAndRange
        case 0x349 : {
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

            stat = LNSpotLight_CreateWithColorAndRange(reinterpret_cast<LNColor*>(pval_color->pt), static_cast<float>(local_range), static_cast<float>(local_angle), &local_outSpotLight);
            setVAInt(pval_outSpotLight, aptr_outSpotLight, local_outSpotLight);

            return true;
        }
        // LNSpotLight_SetEnabled
        case 0x333 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNSpotLight_SetEnabled(static_cast<LNHandle>(local_spotlight), static_cast<LNBool>(local_enabled));


            return true;
        }
        // LNSpotLight_IsEnabled
        case 0x335 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNSpotLight_IsEnabled(static_cast<LNHandle>(local_spotlight), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSpotLight_SetColor
        case 0x336 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);

            stat = LNSpotLight_SetColor(static_cast<LNHandle>(local_spotlight), reinterpret_cast<LNColor*>(pval_color->pt));


            return true;
        }
        // LNSpotLight_GetColor
        case 0x338 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNSpotLight_GetColor(static_cast<LNHandle>(local_spotlight), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNSpotLight_SetIntensity
        case 0x339 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch intensity
            const auto local_intensity = fetchVADouble();

            stat = LNSpotLight_SetIntensity(static_cast<LNHandle>(local_spotlight), static_cast<float>(local_intensity));


            return true;
        }
        // LNSpotLight_GetIntensity
        case 0x33B : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNSpotLight_GetIntensity(static_cast<LNHandle>(local_spotlight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSpotLight_SetRange
        case 0x33C : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch range
            const auto local_range = fetchVADouble();

            stat = LNSpotLight_SetRange(static_cast<LNHandle>(local_spotlight), static_cast<float>(local_range));


            return true;
        }
        // LNSpotLight_GetRange
        case 0x33E : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNSpotLight_GetRange(static_cast<LNHandle>(local_spotlight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSpotLight_SetAttenuation
        case 0x33F : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch attenuation
            const auto local_attenuation = fetchVADouble();

            stat = LNSpotLight_SetAttenuation(static_cast<LNHandle>(local_spotlight), static_cast<float>(local_attenuation));


            return true;
        }
        // LNSpotLight_GetAttenuation
        case 0x341 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNSpotLight_GetAttenuation(static_cast<LNHandle>(local_spotlight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSpotLight_SetAngle
        case 0x342 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch angle
            const auto local_angle = fetchVADouble();

            stat = LNSpotLight_SetAngle(static_cast<LNHandle>(local_spotlight), static_cast<float>(local_angle));


            return true;
        }
        // LNSpotLight_GetAngle
        case 0x344 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNSpotLight_GetAngle(static_cast<LNHandle>(local_spotlight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSpotLight_SetPenumbra
        case 0x345 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch penumbra
            const auto local_penumbra = fetchVADouble();

            stat = LNSpotLight_SetPenumbra(static_cast<LNHandle>(local_spotlight), static_cast<float>(local_penumbra));


            return true;
        }
        // LNSpotLight_GetPenumbra
        case 0x347 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNSpotLight_GetPenumbra(static_cast<LNHandle>(local_spotlight), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNSpotLight_SetPrototype_OnSerialize
        case 0xC7C : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSpotLightSerializeHandler_Create(HSPSubclass_LNSpotLightSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSpotLightSerializeHandler*>(LNSpotLightSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSpotLight_SetPrototype_OnSerialize(static_cast<LNHandle>(local_spotlight), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSpotLight_SetPrototype_OnPreUpdate
        case 0xC87 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSpotLightPreUpdateHandler_Create(HSPSubclass_LNSpotLightPreUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSpotLightPreUpdateHandler*>(LNSpotLightPreUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSpotLight_SetPrototype_OnPreUpdate(static_cast<LNHandle>(local_spotlight), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSpotLight_SetPrototype_OnUpdate
        case 0xC92 : {
            // Fetch spotlight
            const auto local_spotlight = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSpotLightUpdateHandler_Create(HSPSubclass_LNSpotLightUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSpotLightUpdateHandler*>(LNSpotLightUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSpotLight_SetPrototype_OnUpdate(static_cast<LNHandle>(local_spotlight), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNTestDelegate_Create
        case 0x8B1 : {
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
        case 0x35F : {
            // Fetch outSprite
            PVal* pval_outSprite;
            const APTR aptr_outSprite = code_getva(&pval_outSprite);
            LNHandle local_outSprite;

            stat = LNSprite_Create(&local_outSprite);
            setVAInt(pval_outSprite, aptr_outSprite, local_outSprite);

            return true;
        }
        // LNSprite_CreateWithTexture
        case 0x360 : {
            // Fetch texture
            const auto local_texture = fetchVAInt();
            // Fetch outSprite
            PVal* pval_outSprite;
            const APTR aptr_outSprite = code_getva(&pval_outSprite);
            LNHandle local_outSprite;

            stat = LNSprite_CreateWithTexture(static_cast<LNHandle>(local_texture), &local_outSprite);
            setVAInt(pval_outSprite, aptr_outSprite, local_outSprite);

            return true;
        }
        // LNSprite_CreateWithTextureAndSize
        case 0x362 : {
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

            stat = LNSprite_CreateWithTextureAndSize(static_cast<LNHandle>(local_texture), static_cast<float>(local_width), static_cast<float>(local_height), &local_outSprite);
            setVAInt(pval_outSprite, aptr_outSprite, local_outSprite);

            return true;
        }
        // LNSprite_SetTexture
        case 0x351 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LNSprite_SetTexture(static_cast<LNHandle>(local_sprite), static_cast<LNHandle>(local_value));


            return true;
        }
        // LNSprite_SetSize
        case 0x353 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNSize);

            stat = LNSprite_SetSize(static_cast<LNHandle>(local_sprite), reinterpret_cast<LNSize*>(pval_value->pt));


            return true;
        }
        // LNSprite_SetSizeWH
        case 0x355 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch width
            const auto local_width = fetchVADouble();
            // Fetch height
            const auto local_height = fetchVADouble();

            stat = LNSprite_SetSizeWH(static_cast<LNHandle>(local_sprite), static_cast<float>(local_width), static_cast<float>(local_height));


            return true;
        }
        // LNSprite_SetSourceRectXYWH
        case 0x358 : {
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

            stat = LNSprite_SetSourceRectXYWH(static_cast<LNHandle>(local_sprite), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_width), static_cast<float>(local_height));


            return true;
        }
        // LNSprite_SetCallerTest
        case 0x35D : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNTestDelegate_Create(HSPSubclass_LNTestDelegate_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNTestDelegate*>(LNTestDelegate_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSprite_SetCallerTest(static_cast<LNHandle>(local_sprite), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSprite_SetPrototype_OnSerialize
        case 0xC9D : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSpriteSerializeHandler_Create(HSPSubclass_LNSpriteSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSpriteSerializeHandler*>(LNSpriteSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSprite_SetPrototype_OnSerialize(static_cast<LNHandle>(local_sprite), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSprite_SetPrototype_OnPreUpdate
        case 0xCA8 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSpritePreUpdateHandler_Create(HSPSubclass_LNSpritePreUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSpritePreUpdateHandler*>(LNSpritePreUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSprite_SetPrototype_OnPreUpdate(static_cast<LNHandle>(local_sprite), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNSprite_SetPrototype_OnUpdate
        case 0xCB3 : {
            // Fetch sprite
            const auto local_sprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNSpriteUpdateHandler_Create(HSPSubclass_LNSpriteUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNSpriteUpdateHandler*>(LNSpriteUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNSprite_SetPrototype_OnUpdate(static_cast<LNHandle>(local_sprite), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNCameraOrbitControlComponent_Create
        case 0x367 : {
            // Fetch outCameraOrbitControlComponent
            PVal* pval_outCameraOrbitControlComponent;
            const APTR aptr_outCameraOrbitControlComponent = code_getva(&pval_outCameraOrbitControlComponent);
            LNHandle local_outCameraOrbitControlComponent;

            stat = LNCameraOrbitControlComponent_Create(&local_outCameraOrbitControlComponent);
            setVAInt(pval_outCameraOrbitControlComponent, aptr_outCameraOrbitControlComponent, local_outCameraOrbitControlComponent);

            return true;
        }
        // LNCameraOrbitControlComponent_SetPrototype_OnSerialize
        case 0xCBE : {
            // Fetch cameraorbitcontrolcomponent
            const auto local_cameraorbitcontrolcomponent = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNCameraOrbitControlComponentSerializeHandler_Create(HSPSubclass_LNCameraOrbitControlComponentSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraOrbitControlComponentSerializeHandler*>(LNCameraOrbitControlComponentSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNCameraOrbitControlComponent_SetPrototype_OnSerialize(static_cast<LNHandle>(local_cameraorbitcontrolcomponent), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNRaycaster_FromScreen
        case 0x369 : {
            // Fetch point
            PVal* pval_point;
            CodeGetVA_TypeChecked(&pval_point, LNPoint);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNRaycaster_FromScreen(reinterpret_cast<LNPoint*>(pval_point->pt), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNRaycaster_IntersectPlane
        case 0x36B : {
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

            stat = LNRaycaster_IntersectPlane(static_cast<LNHandle>(local_raycaster), static_cast<float>(local_normalX), static_cast<float>(local_normalY), static_cast<float>(local_normalZ), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNRaycaster_SetPrototype_OnSerialize
        case 0xCC9 : {
            // Fetch raycaster
            const auto local_raycaster = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNRaycasterSerializeHandler_Create(HSPSubclass_LNRaycasterSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNRaycasterSerializeHandler*>(LNRaycasterSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNRaycaster_SetPrototype_OnSerialize(static_cast<LNHandle>(local_raycaster), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNRaycastResult_GetPoint
        case 0x370 : {
            // Fetch raycastresult
            const auto local_raycastresult = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNRaycastResult_GetPoint(static_cast<LNHandle>(local_raycastresult), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNRaycastResult_SetPrototype_OnSerialize
        case 0xCD4 : {
            // Fetch raycastresult
            const auto local_raycastresult = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNRaycastResultSerializeHandler_Create(HSPSubclass_LNRaycastResultSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNRaycastResultSerializeHandler*>(LNRaycastResultSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNRaycastResult_SetPrototype_OnSerialize(static_cast<LNHandle>(local_raycastresult), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNWorldRenderView_SetGuideGridEnabled
        case 0x372 : {
            // Fetch worldrenderview
            const auto local_worldrenderview = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNWorldRenderView_SetGuideGridEnabled(static_cast<LNHandle>(local_worldrenderview), static_cast<LNBool>(local_value));


            return true;
        }
        // LNWorldRenderView_GetGuideGridEnabled
        case 0x374 : {
            // Fetch worldrenderview
            const auto local_worldrenderview = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNWorldRenderView_GetGuideGridEnabled(static_cast<LNHandle>(local_worldrenderview), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNWorldRenderView_SetPrototype_OnSerialize
        case 0xCDF : {
            // Fetch worldrenderview
            const auto local_worldrenderview = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNWorldRenderViewSerializeHandler_Create(HSPSubclass_LNWorldRenderViewSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldRenderViewSerializeHandler*>(LNWorldRenderViewSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNWorldRenderView_SetPrototype_OnSerialize(static_cast<LNHandle>(local_worldrenderview), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNShapeObject_SetPrototype_OnSerialize
        case 0xCEA : {
            // Fetch shapeobject
            const auto local_shapeobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNShapeObjectSerializeHandler_Create(HSPSubclass_LNShapeObjectSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNShapeObjectSerializeHandler*>(LNShapeObjectSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNShapeObject_SetPrototype_OnSerialize(static_cast<LNHandle>(local_shapeobject), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNShapeObject_SetPrototype_OnPreUpdate
        case 0xCF5 : {
            // Fetch shapeobject
            const auto local_shapeobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNShapeObjectPreUpdateHandler_Create(HSPSubclass_LNShapeObjectPreUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNShapeObjectPreUpdateHandler*>(LNShapeObjectPreUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNShapeObject_SetPrototype_OnPreUpdate(static_cast<LNHandle>(local_shapeobject), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNShapeObject_SetPrototype_OnUpdate
        case 0xD00 : {
            // Fetch shapeobject
            const auto local_shapeobject = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNShapeObjectUpdateHandler_Create(HSPSubclass_LNShapeObjectUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNShapeObjectUpdateHandler*>(LNShapeObjectUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNShapeObject_SetPrototype_OnUpdate(static_cast<LNHandle>(local_shapeobject), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNPlaneMesh_Create
        case 0x377 : {
            // Fetch outPlaneMesh
            PVal* pval_outPlaneMesh;
            const APTR aptr_outPlaneMesh = code_getva(&pval_outPlaneMesh);
            LNHandle local_outPlaneMesh;

            stat = LNPlaneMesh_Create(&local_outPlaneMesh);
            setVAInt(pval_outPlaneMesh, aptr_outPlaneMesh, local_outPlaneMesh);

            return true;
        }
        // LNPlaneMesh_SetPrototype_OnSerialize
        case 0xD0B : {
            // Fetch planemesh
            const auto local_planemesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPlaneMeshSerializeHandler_Create(HSPSubclass_LNPlaneMeshSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPlaneMeshSerializeHandler*>(LNPlaneMeshSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNPlaneMesh_SetPrototype_OnSerialize(static_cast<LNHandle>(local_planemesh), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNPlaneMesh_SetPrototype_OnPreUpdate
        case 0xD16 : {
            // Fetch planemesh
            const auto local_planemesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPlaneMeshPreUpdateHandler_Create(HSPSubclass_LNPlaneMeshPreUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPlaneMeshPreUpdateHandler*>(LNPlaneMeshPreUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNPlaneMesh_SetPrototype_OnPreUpdate(static_cast<LNHandle>(local_planemesh), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNPlaneMesh_SetPrototype_OnUpdate
        case 0xD21 : {
            // Fetch planemesh
            const auto local_planemesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNPlaneMeshUpdateHandler_Create(HSPSubclass_LNPlaneMeshUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNPlaneMeshUpdateHandler*>(LNPlaneMeshUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNPlaneMesh_SetPrototype_OnUpdate(static_cast<LNHandle>(local_planemesh), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNBoxMesh_Create
        case 0x379 : {
            // Fetch outBoxMesh
            PVal* pval_outBoxMesh;
            const APTR aptr_outBoxMesh = code_getva(&pval_outBoxMesh);
            LNHandle local_outBoxMesh;

            stat = LNBoxMesh_Create(&local_outBoxMesh);
            setVAInt(pval_outBoxMesh, aptr_outBoxMesh, local_outBoxMesh);

            return true;
        }
        // LNBoxMesh_CreateWithSize
        case 0x37A : {
            // Fetch width
            const auto local_width = fetchVADouble();
            // Fetch height
            const auto local_height = fetchVADouble();
            // Fetch depth
            const auto local_depth = fetchVADouble();
            // Fetch outBoxMesh
            PVal* pval_outBoxMesh;
            const APTR aptr_outBoxMesh = code_getva(&pval_outBoxMesh);
            LNHandle local_outBoxMesh;

            stat = LNBoxMesh_CreateWithSize(static_cast<float>(local_width), static_cast<float>(local_height), static_cast<float>(local_depth), &local_outBoxMesh);
            setVAInt(pval_outBoxMesh, aptr_outBoxMesh, local_outBoxMesh);

            return true;
        }
        // LNBoxMesh_SetPrototype_OnSerialize
        case 0xD2C : {
            // Fetch boxmesh
            const auto local_boxmesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNBoxMeshSerializeHandler_Create(HSPSubclass_LNBoxMeshSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxMeshSerializeHandler*>(LNBoxMeshSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNBoxMesh_SetPrototype_OnSerialize(static_cast<LNHandle>(local_boxmesh), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNBoxMesh_SetPrototype_OnPreUpdate
        case 0xD37 : {
            // Fetch boxmesh
            const auto local_boxmesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNBoxMeshPreUpdateHandler_Create(HSPSubclass_LNBoxMeshPreUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxMeshPreUpdateHandler*>(LNBoxMeshPreUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNBoxMesh_SetPrototype_OnPreUpdate(static_cast<LNHandle>(local_boxmesh), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNBoxMesh_SetPrototype_OnUpdate
        case 0xD42 : {
            // Fetch boxmesh
            const auto local_boxmesh = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNBoxMeshUpdateHandler_Create(HSPSubclass_LNBoxMeshUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxMeshUpdateHandler*>(LNBoxMeshUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNBoxMesh_SetPrototype_OnUpdate(static_cast<LNHandle>(local_boxmesh), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNMeshComponent_Create
        case 0x383 : {
            // Fetch outMeshComponent
            PVal* pval_outMeshComponent;
            const APTR aptr_outMeshComponent = code_getva(&pval_outMeshComponent);
            LNHandle local_outMeshComponent;

            stat = LNMeshComponent_Create(&local_outMeshComponent);
            setVAInt(pval_outMeshComponent, aptr_outMeshComponent, local_outMeshComponent);

            return true;
        }
        // LNMeshComponent_SetModel
        case 0x37F : {
            // Fetch meshcomponent
            const auto local_meshcomponent = fetchVAInt();
            // Fetch model
            const auto local_model = fetchVAInt();

            stat = LNMeshComponent_SetModel(static_cast<LNHandle>(local_meshcomponent), static_cast<LNHandle>(local_model));


            return true;
        }
        // LNMeshComponent_MakeCollisionBodyA
        case 0x381 : {
            // Fetch meshcomponent
            const auto local_meshcomponent = fetchVAInt();
            // Fetch meshContainerName
            const auto local_meshContainerName = fetchVAString();

            stat = LNMeshComponent_MakeCollisionBodyA(static_cast<LNHandle>(local_meshcomponent), static_cast<const char*>(local_meshContainerName));


            return true;
        }
        // LNMeshComponent_SetPrototype_OnSerialize
        case 0xD4D : {
            // Fetch meshcomponent
            const auto local_meshcomponent = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNMeshComponentSerializeHandler_Create(HSPSubclass_LNMeshComponentSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNMeshComponentSerializeHandler*>(LNMeshComponentSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNMeshComponent_SetPrototype_OnSerialize(static_cast<LNHandle>(local_meshcomponent), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNCollision_GetWorldObject
        case 0x385 : {
            // Fetch collision
            const auto local_collision = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNCollision_GetWorldObject(static_cast<LNHandle>(local_collision), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNCollision_SetPrototype_OnSerialize
        case 0xD58 : {
            // Fetch collision
            const auto local_collision = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNCollisionSerializeHandler_Create(HSPSubclass_LNCollisionSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNCollisionSerializeHandler*>(LNCollisionSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNCollision_SetPrototype_OnSerialize(static_cast<LNHandle>(local_collision), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNTriggerBodyComponent_Create
        case 0x389 : {
            // Fetch outTriggerBodyComponent
            PVal* pval_outTriggerBodyComponent;
            const APTR aptr_outTriggerBodyComponent = code_getva(&pval_outTriggerBodyComponent);
            LNHandle local_outTriggerBodyComponent;

            stat = LNTriggerBodyComponent_Create(&local_outTriggerBodyComponent);
            setVAInt(pval_outTriggerBodyComponent, aptr_outTriggerBodyComponent, local_outTriggerBodyComponent);

            return true;
        }
        // LNTriggerBodyComponent_AddCollisionShape
        case 0x387 : {
            // Fetch triggerbodycomponent
            const auto local_triggerbodycomponent = fetchVAInt();
            // Fetch shape
            const auto local_shape = fetchVAInt();

            stat = LNTriggerBodyComponent_AddCollisionShape(static_cast<LNHandle>(local_triggerbodycomponent), static_cast<LNHandle>(local_shape));


            return true;
        }
        // LNTriggerBodyComponent_SetPrototype_OnSerialize
        case 0xD63 : {
            // Fetch triggerbodycomponent
            const auto local_triggerbodycomponent = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNTriggerBodyComponentSerializeHandler_Create(HSPSubclass_LNTriggerBodyComponentSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNTriggerBodyComponentSerializeHandler*>(LNTriggerBodyComponentSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNTriggerBodyComponent_SetPrototype_OnSerialize(static_cast<LNHandle>(local_triggerbodycomponent), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNParticleEmitter_Create
        case 0x38B : {
            // Fetch model
            const auto local_model = fetchVAInt();
            // Fetch outParticleEmitter
            PVal* pval_outParticleEmitter;
            const APTR aptr_outParticleEmitter = code_getva(&pval_outParticleEmitter);
            LNHandle local_outParticleEmitter;

            stat = LNParticleEmitter_Create(static_cast<LNHandle>(local_model), &local_outParticleEmitter);
            setVAInt(pval_outParticleEmitter, aptr_outParticleEmitter, local_outParticleEmitter);

            return true;
        }
        // LNParticleEmitter_SetPrototype_OnSerialize
        case 0xD6E : {
            // Fetch particleemitter
            const auto local_particleemitter = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNParticleEmitterSerializeHandler_Create(HSPSubclass_LNParticleEmitterSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleEmitterSerializeHandler*>(LNParticleEmitterSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNParticleEmitter_SetPrototype_OnSerialize(static_cast<LNHandle>(local_particleemitter), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNParticleEmitter_SetPrototype_OnPreUpdate
        case 0xD79 : {
            // Fetch particleemitter
            const auto local_particleemitter = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNParticleEmitterPreUpdateHandler_Create(HSPSubclass_LNParticleEmitterPreUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleEmitterPreUpdateHandler*>(LNParticleEmitterPreUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNParticleEmitter_SetPrototype_OnPreUpdate(static_cast<LNHandle>(local_particleemitter), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNParticleEmitter_SetPrototype_OnUpdate
        case 0xD84 : {
            // Fetch particleemitter
            const auto local_particleemitter = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNParticleEmitterUpdateHandler_Create(HSPSubclass_LNParticleEmitterUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleEmitterUpdateHandler*>(LNParticleEmitterUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNParticleEmitter_SetPrototype_OnUpdate(static_cast<LNHandle>(local_particleemitter), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNScene_SetClearMode
        case 0x38E : {
            // Fetch value
            const auto local_value = static_cast<LNSceneClearMode>(fetchVAInt());

            stat = LNScene_SetClearMode(static_cast<LNSceneClearMode>(local_value));


            return true;
        }
        // LNScene_SetSkyColor
        case 0x390 : {
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNScene_SetSkyColor(reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNScene_SetSkyHorizonColor
        case 0x392 : {
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNScene_SetSkyHorizonColor(reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNScene_SetSkyCloudColor
        case 0x394 : {
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNScene_SetSkyCloudColor(reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNScene_SetSkyOverlayColor
        case 0x396 : {
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNScene_SetSkyOverlayColor(reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNScene_GotoLevel
        case 0x398 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch withEffect
            const auto local_withEffect = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_GotoLevel(static_cast<LNHandle>(local_level), static_cast<LNBool>(local_withEffect));


            return true;
        }
        // LNScene_CallLevel
        case 0x39B : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch withEffect
            const auto local_withEffect = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_CallLevel(static_cast<LNHandle>(local_level), static_cast<LNBool>(local_withEffect));


            return true;
        }
        // LNScene_ReturnLevel
        case 0x39E : {
            // Fetch withEffect
            const auto local_withEffect = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_ReturnLevel(static_cast<LNBool>(local_withEffect));


            return true;
        }
        // LNScene_ActiveLevel
        case 0x3A0 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNScene_ActiveLevel(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_IsTransitionEffectRunning
        case 0x3A1 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNScene_IsTransitionEffectRunning(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetTransitionEffectMode
        case 0x3A2 : {
            // Fetch value
            const auto local_value = static_cast<LNLevelTransitionEffectMode>(fetchVAInt());

            stat = LNScene_SetTransitionEffectMode(static_cast<LNLevelTransitionEffectMode>(local_value));


            return true;
        }
        // LNScene_TransitionEffectMode
        case 0x3A4 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNLevelTransitionEffectMode local_outReturn;

            stat = LNScene_TransitionEffectMode(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetTransitionDuration
        case 0x3A5 : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetTransitionDuration(static_cast<float>(local_value));


            return true;
        }
        // LNScene_TransitionDuration
        case 0x3A7 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNScene_TransitionDuration(&local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetTransitionEffectColor
        case 0x3A8 : {
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNScene_SetTransitionEffectColor(reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNScene_TransitionEffectColor
        case 0x3AA : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNScene_TransitionEffectColor(&local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNScene_SetTransitionEffectMaskTexture
        case 0x3AB : {
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LNScene_SetTransitionEffectMaskTexture(static_cast<LNHandle>(local_value));


            return true;
        }
        // LNScene_TransitionEffectMaskTexture
        case 0x3AD : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNScene_TransitionEffectMaskTexture(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetTransitionEffectVague
        case 0x3AE : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetTransitionEffectVague(static_cast<float>(local_value));


            return true;
        }
        // LNScene_TransitionEffectVague
        case 0x3B0 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNScene_TransitionEffectVague(&local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_StartFadeOut
        case 0x3B1 : {

            stat = LNScene_StartFadeOut();


            return true;
        }
        // LNScene_StartFadeIn
        case 0x3B2 : {

            stat = LNScene_StartFadeIn();


            return true;
        }
        // LNScene_SetFogStartDistance
        case 0x3B3 : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetFogStartDistance(static_cast<float>(local_value));


            return true;
        }
        // LNScene_SetFogColor
        case 0x3B5 : {
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNScene_SetFogColor(reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNScene_SetFogDensity
        case 0x3B7 : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetFogDensity(static_cast<float>(local_value));


            return true;
        }
        // LNScene_SetFogHeightDensity
        case 0x3B9 : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetFogHeightDensity(static_cast<float>(local_value));


            return true;
        }
        // LNScene_SetFogLowerHeight
        case 0x3BB : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetFogLowerHeight(static_cast<float>(local_value));


            return true;
        }
        // LNScene_SetFogUpperHeight
        case 0x3BD : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetFogUpperHeight(static_cast<float>(local_value));


            return true;
        }
        // LNScene_SetHDREnabled
        case 0x3BF : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_SetHDREnabled(static_cast<LNBool>(local_value));


            return true;
        }
        // LNScene_IsHDREnabled
        case 0x3C1 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNScene_IsHDREnabled(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetScreenBlendColor
        case 0x3C2 : {
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNScene_SetScreenBlendColor(reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNScene_ScreenBlendColor
        case 0x3C4 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNScene_ScreenBlendColor(&local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNScene_SetColorTone
        case 0x3C5 : {
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColorTone);

            stat = LNScene_SetColorTone(reinterpret_cast<LNColorTone*>(pval_value->pt));


            return true;
        }
        // LNScene_ColorTone
        case 0x3C7 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColorTone local_outReturn;

            stat = LNScene_ColorTone(&local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColorTone_typeid(), &local_outReturn);

            return true;
        }
        // LNScene_SetAntialiasEnabled
        case 0x3C8 : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_SetAntialiasEnabled(static_cast<LNBool>(local_value));


            return true;
        }
        // LNScene_IsAntialiasEnabled
        case 0x3CA : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNScene_IsAntialiasEnabled(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetSSREnabled
        case 0x3CB : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_SetSSREnabled(static_cast<LNBool>(local_value));


            return true;
        }
        // LNScene_IsSSREnabled
        case 0x3CD : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNScene_IsSSREnabled(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetSSAOEnabled
        case 0x3CE : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_SetSSAOEnabled(static_cast<LNBool>(local_value));


            return true;
        }
        // LNScene_IsSSAOEnabled
        case 0x3D0 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNScene_IsSSAOEnabled(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetBloomEnabled
        case 0x3D1 : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_SetBloomEnabled(static_cast<LNBool>(local_value));


            return true;
        }
        // LNScene_IsBloomEnabled
        case 0x3D3 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNScene_IsBloomEnabled(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetDOFEnabled
        case 0x3D4 : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_SetDOFEnabled(static_cast<LNBool>(local_value));


            return true;
        }
        // LNScene_IsDOFEnabled
        case 0x3D6 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNScene_IsDOFEnabled(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetTonemapEnabled
        case 0x3D7 : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_SetTonemapEnabled(static_cast<LNBool>(local_value));


            return true;
        }
        // LNScene_IsTonemapEnabled
        case 0x3D9 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNScene_IsTonemapEnabled(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetVignetteEnabled
        case 0x3DA : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_SetVignetteEnabled(static_cast<LNBool>(local_value));


            return true;
        }
        // LNScene_IsVignetteEnabled
        case 0x3DC : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNScene_IsVignetteEnabled(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetGammaEnabled
        case 0x3DD : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNScene_SetGammaEnabled(static_cast<LNBool>(local_value));


            return true;
        }
        // LNScene_IsGammaEnabled
        case 0x3DF : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNScene_IsGammaEnabled(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNScene_SetTonemapExposure
        case 0x3E0 : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetTonemapExposure(static_cast<float>(local_value));


            return true;
        }
        // LNScene_SetTonemapLinearWhite
        case 0x3E2 : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetTonemapLinearWhite(static_cast<float>(local_value));


            return true;
        }
        // LNScene_SetTonemapShoulderStrength
        case 0x3E4 : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetTonemapShoulderStrength(static_cast<float>(local_value));


            return true;
        }
        // LNScene_SetTonemapLinearStrength
        case 0x3E6 : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetTonemapLinearStrength(static_cast<float>(local_value));


            return true;
        }
        // LNScene_SetTonemapLinearAngle
        case 0x3E8 : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetTonemapLinearAngle(static_cast<float>(local_value));


            return true;
        }
        // LNScene_SetTonemapToeStrength
        case 0x3EA : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetTonemapToeStrength(static_cast<float>(local_value));


            return true;
        }
        // LNScene_SetTonemapToeNumerator
        case 0x3EC : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetTonemapToeNumerator(static_cast<float>(local_value));


            return true;
        }
        // LNScene_SetTonemapToeDenominator
        case 0x3EE : {
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNScene_SetTonemapToeDenominator(static_cast<float>(local_value));


            return true;
        }
        // LNLevel_Create
        case 0x3FF : {
            // Fetch outLevel
            PVal* pval_outLevel;
            const APTR aptr_outLevel = code_getva(&pval_outLevel);
            LNHandle local_outLevel;

            stat = LNLevel_Create(&local_outLevel);
            setVAInt(pval_outLevel, aptr_outLevel, local_outLevel);

            return true;
        }
        // LNLevel_AddObject
        case 0x3F1 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch obj
            const auto local_obj = fetchVAInt();

            stat = LNLevel_AddObject(static_cast<LNHandle>(local_level), static_cast<LNHandle>(local_obj));


            return true;
        }
        // LNLevel_RemoveObject
        case 0x3F3 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch obj
            const auto local_obj = fetchVAInt();

            stat = LNLevel_RemoveObject(static_cast<LNHandle>(local_level), static_cast<LNHandle>(local_obj));


            return true;
        }
        // LNLevel_AddSubLevel
        case 0x3F5 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch sublevel
            const auto local_sublevel = fetchVAInt();

            stat = LNLevel_AddSubLevel(static_cast<LNHandle>(local_level), static_cast<LNHandle>(local_sublevel));


            return true;
        }
        // LNLevel_RemoveSubLevel
        case 0x3F7 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch sublevel
            const auto local_sublevel = fetchVAInt();

            stat = LNLevel_RemoveSubLevel(static_cast<LNHandle>(local_level), static_cast<LNHandle>(local_sublevel));


            return true;
        }
        // LNLevel_RemoveAllSubLevels
        case 0x3F9 : {
            // Fetch level
            const auto local_level = fetchVAInt();

            stat = LNLevel_RemoveAllSubLevels(static_cast<LNHandle>(local_level));


            return true;
        }
        // LNLevel_SetPrototype_OnSerialize
        case 0xD8F : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNLevelSerializeHandler_Create(HSPSubclass_LNLevelSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelSerializeHandler*>(LNLevelSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNLevel_SetPrototype_OnSerialize(static_cast<LNHandle>(local_level), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNLevel_SetPrototype_OnStart
        case 0xD9A : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNLevelStartHandler_Create(HSPSubclass_LNLevelStartHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelStartHandler*>(LNLevelStartHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNLevel_SetPrototype_OnStart(static_cast<LNHandle>(local_level), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNLevel_SetPrototype_OnStop
        case 0xDA5 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNLevelStopHandler_Create(HSPSubclass_LNLevelStopHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelStopHandler*>(LNLevelStopHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNLevel_SetPrototype_OnStop(static_cast<LNHandle>(local_level), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNLevel_SetPrototype_OnPause
        case 0xDB0 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNLevelPauseHandler_Create(HSPSubclass_LNLevelPauseHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelPauseHandler*>(LNLevelPauseHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNLevel_SetPrototype_OnPause(static_cast<LNHandle>(local_level), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNLevel_SetPrototype_OnResume
        case 0xDBB : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNLevelResumeHandler_Create(HSPSubclass_LNLevelResumeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelResumeHandler*>(LNLevelResumeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNLevel_SetPrototype_OnResume(static_cast<LNHandle>(local_level), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNLevel_SetPrototype_OnUpdate
        case 0xDC6 : {
            // Fetch level
            const auto local_level = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNLevelUpdateHandler_Create(HSPSubclass_LNLevelUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNLevelUpdateHandler*>(LNLevelUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNLevel_SetPrototype_OnUpdate(static_cast<LNHandle>(local_level), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIColors_Red
        case 0x41F : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Red(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Pink
        case 0x421 : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Pink(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Purple
        case 0x423 : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Purple(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_DeepPurple
        case 0x425 : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_DeepPurple(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Indigo
        case 0x427 : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Indigo(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Blue
        case 0x429 : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Blue(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_LightBlue
        case 0x42B : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_LightBlue(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Cyan
        case 0x42D : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Cyan(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Teal
        case 0x42F : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Teal(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Green
        case 0x431 : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Green(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_LightGreen
        case 0x433 : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_LightGreen(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Lime
        case 0x435 : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Lime(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Yellow
        case 0x437 : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Yellow(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Amber
        case 0x439 : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Amber(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Orange
        case 0x43B : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Orange(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_DeepOrange
        case 0x43D : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_DeepOrange(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Brown
        case 0x43F : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Brown(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Grey
        case 0x441 : {
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Grey(static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIColors_Get
        case 0x443 : {
            // Fetch hue
            const auto local_hue = static_cast<LNUIColorHues>(fetchVAInt());
            // Fetch shades
            const auto local_shades = fetchVAInt(5);
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIColors_Get(static_cast<LNUIColorHues>(local_hue), static_cast<int>(local_shades), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIEventArgs_Sender
        case 0x447 : {
            // Fetch uieventargs
            const auto local_uieventargs = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIEventArgs_Sender(static_cast<LNHandle>(local_uieventargs), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIEventArgs_SetPrototype_OnSerialize
        case 0xDD1 : {
            // Fetch uieventargs
            const auto local_uieventargs = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIEventArgsSerializeHandler_Create(HSPSubclass_LNUIEventArgsSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIEventArgsSerializeHandler*>(LNUIEventArgsSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIEventArgs_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uieventargs), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIGeneralEventHandler_Create
        case 0x930 : {
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
        case 0x935 : {
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
        case 0xDDC : {
            // Fetch uilayoutelement
            const auto local_uilayoutelement = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUILayoutElementSerializeHandler_Create(HSPSubclass_LNUILayoutElementSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUILayoutElementSerializeHandler*>(LNUILayoutElementSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUILayoutElement_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uilayoutelement), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIElement_SetSize
        case 0x459 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch size
            PVal* pval_size;
            CodeGetVA_TypeChecked(&pval_size, LNSize);

            stat = LNUIElement_SetSize(static_cast<LNHandle>(local_uielement), reinterpret_cast<LNSize*>(pval_size->pt));


            return true;
        }
        // LNUIElement_SetSizeWH
        case 0x45B : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch width
            const auto local_width = fetchVADouble();
            // Fetch height
            const auto local_height = fetchVADouble();

            stat = LNUIElement_SetSizeWH(static_cast<LNHandle>(local_uielement), static_cast<float>(local_width), static_cast<float>(local_height));


            return true;
        }
        // LNUIElement_SetWidth
        case 0x45E : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNUIElement_SetWidth(static_cast<LNHandle>(local_uielement), static_cast<float>(local_value));


            return true;
        }
        // LNUIElement_GetWidth
        case 0x460 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNUIElement_GetWidth(static_cast<LNHandle>(local_uielement), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIElement_SetHeight
        case 0x461 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNUIElement_SetHeight(static_cast<LNHandle>(local_uielement), static_cast<float>(local_value));


            return true;
        }
        // LNUIElement_GetHeight
        case 0x463 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNUIElement_GetHeight(static_cast<LNHandle>(local_uielement), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIElement_SetMargin
        case 0x464 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch margin
            PVal* pval_margin;
            CodeGetVA_TypeChecked(&pval_margin, LNThickness);

            stat = LNUIElement_SetMargin(static_cast<LNHandle>(local_uielement), reinterpret_cast<LNThickness*>(pval_margin->pt));


            return true;
        }
        // LNUIElement_GetMargin
        case 0x466 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNThickness local_outReturn;

            stat = LNUIElement_GetMargin(static_cast<LNHandle>(local_uielement), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNThickness_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetPadding
        case 0x467 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch padding
            PVal* pval_padding;
            CodeGetVA_TypeChecked(&pval_padding, LNThickness);

            stat = LNUIElement_SetPadding(static_cast<LNHandle>(local_uielement), reinterpret_cast<LNThickness*>(pval_padding->pt));


            return true;
        }
        // LNUIElement_GetPadding
        case 0x469 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNThickness local_outReturn;

            stat = LNUIElement_GetPadding(static_cast<LNHandle>(local_uielement), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNThickness_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetHAlignment
        case 0x46A : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNUIHAlignment>(fetchVAInt());

            stat = LNUIElement_SetHAlignment(static_cast<LNHandle>(local_uielement), static_cast<LNUIHAlignment>(local_value));


            return true;
        }
        // LNUIElement_GetHAlignment
        case 0x46C : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNUIHAlignment local_outReturn;

            stat = LNUIElement_GetHAlignment(static_cast<LNHandle>(local_uielement), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIElement_SetVAlignment
        case 0x46D : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNUIVAlignment>(fetchVAInt());

            stat = LNUIElement_SetVAlignment(static_cast<LNHandle>(local_uielement), static_cast<LNUIVAlignment>(local_value));


            return true;
        }
        // LNUIElement_GetVAlignment
        case 0x46F : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNUIVAlignment local_outReturn;

            stat = LNUIElement_GetVAlignment(static_cast<LNHandle>(local_uielement), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIElement_SetAlignments
        case 0x470 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch halign
            const auto local_halign = static_cast<LNUIHAlignment>(fetchVAInt());
            // Fetch valign
            const auto local_valign = static_cast<LNUIVAlignment>(fetchVAInt());

            stat = LNUIElement_SetAlignments(static_cast<LNHandle>(local_uielement), static_cast<LNUIHAlignment>(local_halign), static_cast<LNUIVAlignment>(local_valign));


            return true;
        }
        // LNUIElement_SetPosition
        case 0x473 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch pos
            PVal* pval_pos;
            CodeGetVA_TypeChecked(&pval_pos, LNVector3);

            stat = LNUIElement_SetPosition(static_cast<LNHandle>(local_uielement), reinterpret_cast<LNVector3*>(pval_pos->pt));


            return true;
        }
        // LNUIElement_SetPositionXYZ
        case 0x475 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble(0.000000);

            stat = LNUIElement_SetPositionXYZ(static_cast<LNHandle>(local_uielement), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_z));


            return true;
        }
        // LNUIElement_GetPosition
        case 0x479 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNUIElement_GetPosition(static_cast<LNHandle>(local_uielement), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetRotation
        case 0x47A : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch rot
            PVal* pval_rot;
            CodeGetVA_TypeChecked(&pval_rot, LNQuaternion);

            stat = LNUIElement_SetRotation(static_cast<LNHandle>(local_uielement), reinterpret_cast<LNQuaternion*>(pval_rot->pt));


            return true;
        }
        // LNUIElement_SetEulerAngles
        case 0x47C : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble();

            stat = LNUIElement_SetEulerAngles(static_cast<LNHandle>(local_uielement), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_z));


            return true;
        }
        // LNUIElement_GetRotation
        case 0x480 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNQuaternion local_outReturn;

            stat = LNUIElement_GetRotation(static_cast<LNHandle>(local_uielement), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNQuaternion_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetScale
        case 0x481 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch scale
            PVal* pval_scale;
            CodeGetVA_TypeChecked(&pval_scale, LNVector3);

            stat = LNUIElement_SetScale(static_cast<LNHandle>(local_uielement), reinterpret_cast<LNVector3*>(pval_scale->pt));


            return true;
        }
        // LNUIElement_SetScaleS
        case 0x483 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch xyz
            const auto local_xyz = fetchVADouble();

            stat = LNUIElement_SetScaleS(static_cast<LNHandle>(local_uielement), static_cast<float>(local_xyz));


            return true;
        }
        // LNUIElement_SetScaleXY
        case 0x485 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();

            stat = LNUIElement_SetScaleXY(static_cast<LNHandle>(local_uielement), static_cast<float>(local_x), static_cast<float>(local_y));


            return true;
        }
        // LNUIElement_GetScale
        case 0x488 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNUIElement_GetScale(static_cast<LNHandle>(local_uielement), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetCenterPoint
        case 0x489 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNVector3);

            stat = LNUIElement_SetCenterPoint(static_cast<LNHandle>(local_uielement), reinterpret_cast<LNVector3*>(pval_value->pt));


            return true;
        }
        // LNUIElement_SetCenterPointXYZ
        case 0x48B : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch x
            const auto local_x = fetchVADouble();
            // Fetch y
            const auto local_y = fetchVADouble();
            // Fetch z
            const auto local_z = fetchVADouble(0.000000);

            stat = LNUIElement_SetCenterPointXYZ(static_cast<LNHandle>(local_uielement), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_z));


            return true;
        }
        // LNUIElement_GetCenterPoint
        case 0x48F : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNVector3 local_outReturn;

            stat = LNUIElement_GetCenterPoint(static_cast<LNHandle>(local_uielement), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNVector3_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetEnabled
        case 0x490 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNUIElement_SetEnabled(static_cast<LNHandle>(local_uielement), static_cast<LNBool>(local_value));


            return true;
        }
        // LNUIElement_IsEnabled
        case 0x492 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNUIElement_IsEnabled(static_cast<LNHandle>(local_uielement), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIElement_SetData
        case 0x493 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LNUIElement_SetData(static_cast<LNHandle>(local_uielement), static_cast<LNHandle>(local_value));


            return true;
        }
        // LNUIElement_GetData
        case 0x495 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIElement_GetData(static_cast<LNHandle>(local_uielement), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIElement_SetBackgroundColor
        case 0x496 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNUIElement_SetBackgroundColor(static_cast<LNHandle>(local_uielement), reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNUIElement_GetBackgroundColor
        case 0x498 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIElement_GetBackgroundColor(static_cast<LNHandle>(local_uielement), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetBorderThickness
        case 0x499 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNThickness);

            stat = LNUIElement_SetBorderThickness(static_cast<LNHandle>(local_uielement), reinterpret_cast<LNThickness*>(pval_value->pt));


            return true;
        }
        // LNUIElement_GetBorderThickness
        case 0x49B : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNThickness local_outReturn;

            stat = LNUIElement_GetBorderThickness(static_cast<LNHandle>(local_uielement), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNThickness_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetBorderColor
        case 0x49C : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNColor);

            stat = LNUIElement_SetBorderColor(static_cast<LNHandle>(local_uielement), reinterpret_cast<LNColor*>(pval_value->pt));


            return true;
        }
        // LNUIElement_GetBorderColor
        case 0x49E : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNColor local_outReturn;

            stat = LNUIElement_GetBorderColor(static_cast<LNHandle>(local_uielement), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNColor_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetCornerRadius
        case 0x49F : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            PVal* pval_value;
            CodeGetVA_TypeChecked(&pval_value, LNCornerRadius);

            stat = LNUIElement_SetCornerRadius(static_cast<LNHandle>(local_uielement), reinterpret_cast<LNCornerRadius*>(pval_value->pt));


            return true;
        }
        // LNUIElement_GetCornerRadius
        case 0x4A1 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNCornerRadius local_outReturn;

            stat = LNUIElement_GetCornerRadius(static_cast<LNHandle>(local_uielement), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNCornerRadius_typeid(), &local_outReturn);

            return true;
        }
        // LNUIElement_SetVisibility
        case 0x4A2 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNUIVisibility>(fetchVAInt());

            stat = LNUIElement_SetVisibility(static_cast<LNHandle>(local_uielement), static_cast<LNUIVisibility>(local_value));


            return true;
        }
        // LNUIElement_GetVisibility
        case 0x4A4 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNUIVisibility local_outReturn;

            stat = LNUIElement_GetVisibility(static_cast<LNHandle>(local_uielement), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIElement_SetOpacity
        case 0x4A5 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNUIElement_SetOpacity(static_cast<LNHandle>(local_uielement), static_cast<float>(local_value));


            return true;
        }
        // LNUIElement_GetOpacity
        case 0x4A7 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNUIElement_GetOpacity(static_cast<LNHandle>(local_uielement), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIElement_AddChild
        case 0x4A8 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch child
            const auto local_child = fetchVAInt();

            stat = LNUIElement_AddChild(static_cast<LNHandle>(local_uielement), static_cast<LNHandle>(local_child));


            return true;
        }
        // LNUIElement_SetFocusable
        case 0x4AA : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNUIElement_SetFocusable(static_cast<LNHandle>(local_uielement), static_cast<LNBool>(local_value));


            return true;
        }
        // LNUIElement_GetFocusable
        case 0x4AC : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNUIElement_GetFocusable(static_cast<LNHandle>(local_uielement), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIElement_AddInto
        case 0x4AD : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch parent
            const auto local_parent = fetchVAInt(LN_NULL_HANDLE);

            stat = LNUIElement_AddInto(static_cast<LNHandle>(local_uielement), static_cast<LNHandle>(local_parent));


            return true;
        }
        // LNUIElement_SetPrototype_OnSerialize
        case 0xDE7 : {
            // Fetch uielement
            const auto local_uielement = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIElementSerializeHandler_Create(HSPSubclass_LNUIElementSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIElementSerializeHandler*>(LNUIElementSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIElement_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uielement), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIText_Create
        case 0x4B3 : {
            // Fetch outUIText
            PVal* pval_outUIText;
            const APTR aptr_outUIText = code_getva(&pval_outUIText);
            LNHandle local_outUIText;

            stat = LNUIText_Create(&local_outUIText);
            setVAInt(pval_outUIText, aptr_outUIText, local_outUIText);

            return true;
        }
        // LNUIText_CreateWithTextA
        case 0x4B4 : {
            // Fetch text
            const auto local_text = fetchVAString();
            // Fetch outUIText
            PVal* pval_outUIText;
            const APTR aptr_outUIText = code_getva(&pval_outUIText);
            LNHandle local_outUIText;

            stat = LNUIText_CreateWithTextA(static_cast<const char*>(local_text), &local_outUIText);
            setVAInt(pval_outUIText, aptr_outUIText, local_outUIText);

            return true;
        }
        // LNUIText_SetTextA
        case 0x4B0 : {
            // Fetch uitext
            const auto local_uitext = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAString();

            stat = LNUIText_SetTextA(static_cast<LNHandle>(local_uitext), static_cast<const char*>(local_value));


            return true;
        }
        // LNUIText_GetTextA
        case 0x4B2 : {
            // Fetch uitext
            const auto local_uitext = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LNUIText_GetTextA(static_cast<LNHandle>(local_uitext), &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIText_SetPrototype_OnSerialize
        case 0xDF2 : {
            // Fetch uitext
            const auto local_uitext = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUITextSerializeHandler_Create(HSPSubclass_LNUITextSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUITextSerializeHandler*>(LNUITextSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIText_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uitext), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUISprite_Create
        case 0x4C3 : {
            // Fetch outUISprite
            PVal* pval_outUISprite;
            const APTR aptr_outUISprite = code_getva(&pval_outUISprite);
            LNHandle local_outUISprite;

            stat = LNUISprite_Create(&local_outUISprite);
            setVAInt(pval_outUISprite, aptr_outUISprite, local_outUISprite);

            return true;
        }
        // LNUISprite_CreateWithTexture
        case 0x4C4 : {
            // Fetch texture
            const auto local_texture = fetchVAInt();
            // Fetch outUISprite
            PVal* pval_outUISprite;
            const APTR aptr_outUISprite = code_getva(&pval_outUISprite);
            LNHandle local_outUISprite;

            stat = LNUISprite_CreateWithTexture(static_cast<LNHandle>(local_texture), &local_outUISprite);
            setVAInt(pval_outUISprite, aptr_outUISprite, local_outUISprite);

            return true;
        }
        // LNUISprite_SetTexture
        case 0x4B7 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch texture
            const auto local_texture = fetchVAInt();

            stat = LNUISprite_SetTexture(static_cast<LNHandle>(local_uisprite), static_cast<LNHandle>(local_texture));


            return true;
        }
        // LNUISprite_SetSourceRect
        case 0x4B9 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch rect
            PVal* pval_rect;
            CodeGetVA_TypeChecked(&pval_rect, LNRect);

            stat = LNUISprite_SetSourceRect(static_cast<LNHandle>(local_uisprite), reinterpret_cast<LNRect*>(pval_rect->pt));


            return true;
        }
        // LNUISprite_SetSourceRectXYWH
        case 0x4BB : {
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

            stat = LNUISprite_SetSourceRectXYWH(static_cast<LNHandle>(local_uisprite), static_cast<float>(local_x), static_cast<float>(local_y), static_cast<float>(local_width), static_cast<float>(local_height));


            return true;
        }
        // LNUISprite_GetSourceRect
        case 0x4C0 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNRect local_outReturn;

            stat = LNUISprite_GetSourceRect(static_cast<LNHandle>(local_uisprite), &local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNRect_typeid(), &local_outReturn);

            return true;
        }
        // LNUISprite_SetShader
        case 0x4C1 : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch shader
            const auto local_shader = fetchVAInt();

            stat = LNUISprite_SetShader(static_cast<LNHandle>(local_uisprite), static_cast<LNHandle>(local_shader));


            return true;
        }
        // LNUISprite_SetPrototype_OnSerialize
        case 0xDFD : {
            // Fetch uisprite
            const auto local_uisprite = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUISpriteSerializeHandler_Create(HSPSubclass_LNUISpriteSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUISpriteSerializeHandler*>(LNUISpriteSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUISprite_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uisprite), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIIcon_LoadFontIconA
        case 0x4C7 : {
            // Fetch iconName
            const auto local_iconName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIIcon_LoadFontIconA(static_cast<const char*>(local_iconName), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIIcon_LoadFontIconWithNameSizeA
        case 0x4C9 : {
            // Fetch iconName
            const auto local_iconName = fetchVAString();
            // Fetch size
            const auto local_size = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIIcon_LoadFontIconWithNameSizeA(static_cast<const char*>(local_iconName), static_cast<int>(local_size), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIIcon_LoadFontIconWithNameSizeColorA
        case 0x4CC : {
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

            stat = LNUIIcon_LoadFontIconWithNameSizeColorA(static_cast<const char*>(local_iconName), static_cast<int>(local_size), reinterpret_cast<LNColor*>(pval_color->pt), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIIcon_SetPrototype_OnSerialize
        case 0xE08 : {
            // Fetch uiicon
            const auto local_uiicon = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIIconSerializeHandler_Create(HSPSubclass_LNUIIconSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIIconSerializeHandler*>(LNUIIconSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIIcon_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uiicon), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIMessageTextArea_Create
        case 0x4D5 : {
            // Fetch outUIMessageTextArea
            PVal* pval_outUIMessageTextArea;
            const APTR aptr_outUIMessageTextArea = code_getva(&pval_outUIMessageTextArea);
            LNHandle local_outUIMessageTextArea;

            stat = LNUIMessageTextArea_Create(&local_outUIMessageTextArea);
            setVAInt(pval_outUIMessageTextArea, aptr_outUIMessageTextArea, local_outUIMessageTextArea);

            return true;
        }
        // LNUIMessageTextArea_SetTextA
        case 0x4D1 : {
            // Fetch uimessagetextarea
            const auto local_uimessagetextarea = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAString();

            stat = LNUIMessageTextArea_SetTextA(static_cast<LNHandle>(local_uimessagetextarea), static_cast<const char*>(local_value));


            return true;
        }
        // LNUIMessageTextArea_SetTypingSpeed
        case 0x4D3 : {
            // Fetch uimessagetextarea
            const auto local_uimessagetextarea = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVADouble();

            stat = LNUIMessageTextArea_SetTypingSpeed(static_cast<LNHandle>(local_uimessagetextarea), static_cast<float>(local_value));


            return true;
        }
        // LNUIMessageTextArea_SetPrototype_OnSerialize
        case 0xE13 : {
            // Fetch uimessagetextarea
            const auto local_uimessagetextarea = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIMessageTextAreaSerializeHandler_Create(HSPSubclass_LNUIMessageTextAreaSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIMessageTextAreaSerializeHandler*>(LNUIMessageTextAreaSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIMessageTextArea_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uimessagetextarea), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUI_Add
        case 0x4D7 : {
            // Fetch element
            const auto local_element = fetchVAInt();

            stat = LNUI_Add(static_cast<LNHandle>(local_element));


            return true;
        }
        // LNUI_Remove
        case 0x4D9 : {
            // Fetch element
            const auto local_element = fetchVAInt();

            stat = LNUI_Remove(static_cast<LNHandle>(local_element));


            return true;
        }
        // LNUILayoutPanel_SetPrototype_OnSerialize
        case 0xE1E : {
            // Fetch uilayoutpanel
            const auto local_uilayoutpanel = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUILayoutPanelSerializeHandler_Create(HSPSubclass_LNUILayoutPanelSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUILayoutPanelSerializeHandler*>(LNUILayoutPanelSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUILayoutPanel_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uilayoutpanel), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIBoxLayout_Create
        case 0x4E0 : {
            // Fetch outUIBoxLayout
            PVal* pval_outUIBoxLayout;
            const APTR aptr_outUIBoxLayout = code_getva(&pval_outUIBoxLayout);
            LNHandle local_outUIBoxLayout;

            stat = LNUIBoxLayout_Create(&local_outUIBoxLayout);
            setVAInt(pval_outUIBoxLayout, aptr_outUIBoxLayout, local_outUIBoxLayout);

            return true;
        }
        // LNUIBoxLayout_SetOrientation
        case 0x4DD : {
            // Fetch uiboxlayout
            const auto local_uiboxlayout = fetchVAInt();
            // Fetch orientation
            const auto local_orientation = static_cast<LNUILayoutOrientation>(fetchVAInt());

            stat = LNUIBoxLayout_SetOrientation(static_cast<LNHandle>(local_uiboxlayout), static_cast<LNUILayoutOrientation>(local_orientation));


            return true;
        }
        // LNUIBoxLayout_GetOrientation
        case 0x4DF : {
            // Fetch uiboxlayout
            const auto local_uiboxlayout = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNUILayoutOrientation local_outReturn;

            stat = LNUIBoxLayout_GetOrientation(static_cast<LNHandle>(local_uiboxlayout), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIBoxLayout_SetPrototype_OnSerialize
        case 0xE29 : {
            // Fetch uiboxlayout
            const auto local_uiboxlayout = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIBoxLayoutSerializeHandler_Create(HSPSubclass_LNUIBoxLayoutSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIBoxLayoutSerializeHandler*>(LNUIBoxLayoutSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIBoxLayout_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uiboxlayout), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIStackLayout_Create
        case 0x4E5 : {
            // Fetch outUIStackLayout
            PVal* pval_outUIStackLayout;
            const APTR aptr_outUIStackLayout = code_getva(&pval_outUIStackLayout);
            LNHandle local_outUIStackLayout;

            stat = LNUIStackLayout_Create(&local_outUIStackLayout);
            setVAInt(pval_outUIStackLayout, aptr_outUIStackLayout, local_outUIStackLayout);

            return true;
        }
        // LNUIStackLayout_SetOrientation
        case 0x4E2 : {
            // Fetch uistacklayout
            const auto local_uistacklayout = fetchVAInt();
            // Fetch orientation
            const auto local_orientation = static_cast<LNUILayoutOrientation>(fetchVAInt());

            stat = LNUIStackLayout_SetOrientation(static_cast<LNHandle>(local_uistacklayout), static_cast<LNUILayoutOrientation>(local_orientation));


            return true;
        }
        // LNUIStackLayout_GetOrientation
        case 0x4E4 : {
            // Fetch uistacklayout
            const auto local_uistacklayout = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNUILayoutOrientation local_outReturn;

            stat = LNUIStackLayout_GetOrientation(static_cast<LNHandle>(local_uistacklayout), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIStackLayout_SetPrototype_OnSerialize
        case 0xE34 : {
            // Fetch uistacklayout
            const auto local_uistacklayout = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIStackLayoutSerializeHandler_Create(HSPSubclass_LNUIStackLayoutSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIStackLayoutSerializeHandler*>(LNUIStackLayoutSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIStackLayout_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uistacklayout), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIGridLayout_Create
        case 0x4F5 : {
            // Fetch outUIGridLayout
            PVal* pval_outUIGridLayout;
            const APTR aptr_outUIGridLayout = code_getva(&pval_outUIGridLayout);
            LNHandle local_outUIGridLayout;

            stat = LNUIGridLayout_Create(&local_outUIGridLayout);
            setVAInt(pval_outUIGridLayout, aptr_outUIGridLayout, local_outUIGridLayout);

            return true;
        }
        // LNUIGridLayout_SetColumnCount
        case 0x4E7 : {
            // Fetch uigridlayout
            const auto local_uigridlayout = fetchVAInt();
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LNUIGridLayout_SetColumnCount(static_cast<LNHandle>(local_uigridlayout), static_cast<int>(local_value));


            return true;
        }
        // LNUIGridLayout_SetRow
        case 0x4E9 : {
            // Fetch element
            const auto local_element = fetchVAInt();
            // Fetch row
            const auto local_row = fetchVAInt();

            stat = LNUIGridLayout_SetRow(static_cast<LNHandle>(local_element), static_cast<int>(local_row));


            return true;
        }
        // LNUIGridLayout_SetColumn
        case 0x4EC : {
            // Fetch element
            const auto local_element = fetchVAInt();
            // Fetch column
            const auto local_column = fetchVAInt();

            stat = LNUIGridLayout_SetColumn(static_cast<LNHandle>(local_element), static_cast<int>(local_column));


            return true;
        }
        // LNUIGridLayout_SetPlacement
        case 0x4EF : {
            // Fetch element
            const auto local_element = fetchVAInt();
            // Fetch row
            const auto local_row = fetchVAInt();
            // Fetch column
            const auto local_column = fetchVAInt();
            // Fetch rowSpan
            const auto local_rowSpan = fetchVAInt(1);
            // Fetch columnSpan
            const auto local_columnSpan = fetchVAInt(1);

            stat = LNUIGridLayout_SetPlacement(static_cast<LNHandle>(local_element), static_cast<int>(local_row), static_cast<int>(local_column), static_cast<int>(local_rowSpan), static_cast<int>(local_columnSpan));


            return true;
        }
        // LNUIGridLayout_SetPrototype_OnSerialize
        case 0xE3F : {
            // Fetch uigridlayout
            const auto local_uigridlayout = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIGridLayoutSerializeHandler_Create(HSPSubclass_LNUIGridLayoutSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIGridLayoutSerializeHandler*>(LNUIGridLayoutSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIGridLayout_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uigridlayout), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIControl_Create
        case 0x501 : {
            // Fetch outUIControl
            PVal* pval_outUIControl;
            const APTR aptr_outUIControl = code_getva(&pval_outUIControl);
            LNHandle local_outUIControl;

            stat = LNUIControl_Create(&local_outUIControl);
            setVAInt(pval_outUIControl, aptr_outUIControl, local_outUIControl);

            return true;
        }
        // LNUIControl_AddInlineVisual
        case 0x502 : {
            // Fetch uicontrol
            const auto local_uicontrol = fetchVAInt();
            // Fetch element
            const auto local_element = fetchVAInt();
            // Fetch layout
            const auto local_layout = static_cast<LNUIInlinePlacement>(fetchVAInt());

            stat = LNUIControl_AddInlineVisual(static_cast<LNHandle>(local_uicontrol), static_cast<LNHandle>(local_element), static_cast<LNUIInlinePlacement>(local_layout));


            return true;
        }
        // LNUIControl_SetPrototype_OnSerialize
        case 0xE4A : {
            // Fetch uicontrol
            const auto local_uicontrol = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIControlSerializeHandler_Create(HSPSubclass_LNUIControlSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIControlSerializeHandler*>(LNUIControlSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIControl_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uicontrol), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIButtonBase_SetTextA
        case 0x506 : {
            // Fetch uibuttonbase
            const auto local_uibuttonbase = fetchVAInt();
            // Fetch text
            const auto local_text = fetchVAString();

            stat = LNUIButtonBase_SetTextA(static_cast<LNHandle>(local_uibuttonbase), static_cast<const char*>(local_text));


            return true;
        }
        // LNUIButtonBase_SetPrototype_OnSerialize
        case 0xE55 : {
            // Fetch uibuttonbase
            const auto local_uibuttonbase = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIButtonBaseSerializeHandler_Create(HSPSubclass_LNUIButtonBaseSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIButtonBaseSerializeHandler*>(LNUIButtonBaseSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIButtonBase_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uibuttonbase), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIButton_Create
        case 0x509 : {
            // Fetch outUIButton
            PVal* pval_outUIButton;
            const APTR aptr_outUIButton = code_getva(&pval_outUIButton);
            LNHandle local_outUIButton;

            stat = LNUIButton_Create(&local_outUIButton);
            setVAInt(pval_outUIButton, aptr_outUIButton, local_outUIButton);

            return true;
        }
        // LNUIButton_CreateWithTextA
        case 0x50A : {
            // Fetch text
            const auto local_text = fetchVAString();
            // Fetch outUIButton
            PVal* pval_outUIButton;
            const APTR aptr_outUIButton = code_getva(&pval_outUIButton);
            LNHandle local_outUIButton;

            stat = LNUIButton_CreateWithTextA(static_cast<const char*>(local_text), &local_outUIButton);
            setVAInt(pval_outUIButton, aptr_outUIButton, local_outUIButton);

            return true;
        }
        // LNUIButton_ConnectOnClicked
        case 0x50C : {
            // Fetch uibutton
            const auto local_uibutton = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNUIEventHandler_Create(HSPSubclass_LNUIEventHandler_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIEventHandler*>(LNUIEventHandler_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIButton_ConnectOnClicked(static_cast<LNHandle>(local_uibutton), localDelegate_handler, &local_outReturn);
            LNObject_Release(localDelegate_handler);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIButton_SetPrototype_OnSerialize
        case 0xE60 : {
            // Fetch uibutton
            const auto local_uibutton = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIButtonSerializeHandler_Create(HSPSubclass_LNUIButtonSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIButtonSerializeHandler*>(LNUIButtonSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIButton_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uibutton), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIWindow_Create
        case 0x50F : {
            // Fetch outUIWindow
            PVal* pval_outUIWindow;
            const APTR aptr_outUIWindow = code_getva(&pval_outUIWindow);
            LNHandle local_outUIWindow;

            stat = LNUIWindow_Create(&local_outUIWindow);
            setVAInt(pval_outUIWindow, aptr_outUIWindow, local_outUIWindow);

            return true;
        }
        // LNUIWindow_SetPrototype_OnSerialize
        case 0xE6B : {
            // Fetch uiwindow
            const auto local_uiwindow = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIWindowSerializeHandler_Create(HSPSubclass_LNUIWindowSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIWindowSerializeHandler*>(LNUIWindowSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIWindow_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uiwindow), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIListItem_ConnectOnSubmit
        case 0x514 : {
            // Fetch uilistitem
            const auto local_uilistitem = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNUIGeneralEventHandler_Create(HSPSubclass_LNUIGeneralEventHandler_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIGeneralEventHandler*>(LNUIGeneralEventHandler_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIListItem_ConnectOnSubmit(static_cast<LNHandle>(local_uilistitem), localDelegate_handler, &local_outReturn);
            LNObject_Release(localDelegate_handler);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIListItem_SetPrototype_OnSerialize
        case 0xE76 : {
            // Fetch uilistitem
            const auto local_uilistitem = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIListItemSerializeHandler_Create(HSPSubclass_LNUIListItemSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListItemSerializeHandler*>(LNUIListItemSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIListItem_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uilistitem), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIListItemsControl_SetItemsLayoutPanel
        case 0x517 : {
            // Fetch uilistitemscontrol
            const auto local_uilistitemscontrol = fetchVAInt();
            // Fetch layout
            const auto local_layout = fetchVAInt();

            stat = LNUIListItemsControl_SetItemsLayoutPanel(static_cast<LNHandle>(local_uilistitemscontrol), static_cast<LNHandle>(local_layout));


            return true;
        }
        // LNUIListItemsControl_GetItemsLayoutPanel
        case 0x519 : {
            // Fetch uilistitemscontrol
            const auto local_uilistitemscontrol = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIListItemsControl_GetItemsLayoutPanel(static_cast<LNHandle>(local_uilistitemscontrol), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIListItemsControl_SetSubmitMode
        case 0x51A : {
            // Fetch uilistitemscontrol
            const auto local_uilistitemscontrol = fetchVAInt();
            // Fetch value
            const auto local_value = static_cast<LNUIListSubmitMode>(fetchVAInt());

            stat = LNUIListItemsControl_SetSubmitMode(static_cast<LNHandle>(local_uilistitemscontrol), static_cast<LNUIListSubmitMode>(local_value));


            return true;
        }
        // LNUIListItemsControl_GetSubmitMode
        case 0x51C : {
            // Fetch uilistitemscontrol
            const auto local_uilistitemscontrol = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNUIListSubmitMode local_outReturn;

            stat = LNUIListItemsControl_GetSubmitMode(static_cast<LNHandle>(local_uilistitemscontrol), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIListItemsControl_ConnectOnSubmit
        case 0x51D : {
            // Fetch uilistitemscontrol
            const auto local_uilistitemscontrol = fetchVAInt();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNUIGeneralEventHandler_Create(HSPSubclass_LNUIGeneralEventHandler_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIGeneralEventHandler*>(LNUIGeneralEventHandler_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIListItemsControl_ConnectOnSubmit(static_cast<LNHandle>(local_uilistitemscontrol), localDelegate_handler, &local_outReturn);
            LNObject_Release(localDelegate_handler);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIListItemsControl_SetPrototype_OnSerialize
        case 0xE81 : {
            // Fetch uilistitemscontrol
            const auto local_uilistitemscontrol = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIListItemsControlSerializeHandler_Create(HSPSubclass_LNUIListItemsControlSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListItemsControlSerializeHandler*>(LNUIListItemsControlSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIListItemsControl_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uilistitemscontrol), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIListBoxItem_Create
        case 0x520 : {
            // Fetch content
            const auto local_content = fetchVAInt();
            // Fetch outUIListBoxItem
            PVal* pval_outUIListBoxItem;
            const APTR aptr_outUIListBoxItem = code_getva(&pval_outUIListBoxItem);
            LNHandle local_outUIListBoxItem;

            stat = LNUIListBoxItem_Create(static_cast<LNHandle>(local_content), &local_outUIListBoxItem);
            setVAInt(pval_outUIListBoxItem, aptr_outUIListBoxItem, local_outUIListBoxItem);

            return true;
        }
        // LNUIListBoxItem_SetPrototype_OnSerialize
        case 0xE8C : {
            // Fetch uilistboxitem
            const auto local_uilistboxitem = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIListBoxItemSerializeHandler_Create(HSPSubclass_LNUIListBoxItemSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListBoxItemSerializeHandler*>(LNUIListBoxItemSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIListBoxItem_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uilistboxitem), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNUIListBox_Create
        case 0x525 : {
            // Fetch outUIListBox
            PVal* pval_outUIListBox;
            const APTR aptr_outUIListBox = code_getva(&pval_outUIListBox);
            LNHandle local_outUIListBox;

            stat = LNUIListBox_Create(&local_outUIListBox);
            setVAInt(pval_outUIListBox, aptr_outUIListBox, local_outUIListBox);

            return true;
        }
        // LNUIListBox_AddItem
        case 0x523 : {
            // Fetch uilistbox
            const auto local_uilistbox = fetchVAInt();
            // Fetch content
            const auto local_content = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNUIListBox_AddItem(static_cast<LNHandle>(local_uilistbox), static_cast<LNHandle>(local_content), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNUIListBox_SetPrototype_OnSerialize
        case 0xE97 : {
            // Fetch uilistbox
            const auto local_uilistbox = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNUIListBoxSerializeHandler_Create(HSPSubclass_LNUIListBoxSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNUIListBoxSerializeHandler*>(LNUIListBoxSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNUIListBox_SetPrototype_OnSerialize(static_cast<LNHandle>(local_uilistbox), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNInputGesture_SetPrototype_OnSerialize
        case 0xEA2 : {
            // Fetch inputgesture
            const auto local_inputgesture = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNInputGestureSerializeHandler_Create(HSPSubclass_LNInputGestureSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNInputGestureSerializeHandler*>(LNInputGestureSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNInputGesture_SetPrototype_OnSerialize(static_cast<LNHandle>(local_inputgesture), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNKeyGesture_Create
        case 0x528 : {
            // Fetch key
            const auto local_key = static_cast<LNKeys>(fetchVAInt());
            // Fetch outKeyGesture
            PVal* pval_outKeyGesture;
            const APTR aptr_outKeyGesture = code_getva(&pval_outKeyGesture);
            LNHandle local_outKeyGesture;

            stat = LNKeyGesture_Create(static_cast<LNKeys>(local_key), &local_outKeyGesture);
            setVAInt(pval_outKeyGesture, aptr_outKeyGesture, local_outKeyGesture);

            return true;
        }
        // LNKeyGesture_SetPrototype_OnSerialize
        case 0xEAD : {
            // Fetch keygesture
            const auto local_keygesture = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNKeyGestureSerializeHandler_Create(HSPSubclass_LNKeyGestureSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNKeyGestureSerializeHandler*>(LNKeyGestureSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNKeyGesture_SetPrototype_OnSerialize(static_cast<LNHandle>(local_keygesture), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNInput_IsPressedA
        case 0x52B : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNInput_IsPressedA(static_cast<const char*>(local_buttonName), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInput_IsTriggeredA
        case 0x52D : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNInput_IsTriggeredA(static_cast<const char*>(local_buttonName), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInput_IsTriggeredOffA
        case 0x52F : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNInput_IsTriggeredOffA(static_cast<const char*>(local_buttonName), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInput_IsRepeatedA
        case 0x531 : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNInput_IsRepeatedA(static_cast<const char*>(local_buttonName), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInput_GetAxisValueA
        case 0x533 : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            float local_outReturn;

            stat = LNInput_GetAxisValueA(static_cast<const char*>(local_buttonName), &local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInput_AddBindingA
        case 0x535 : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();
            // Fetch gesture
            const auto local_gesture = fetchVAInt();

            stat = LNInput_AddBindingA(static_cast<const char*>(local_buttonName), static_cast<LNHandle>(local_gesture));


            return true;
        }
        // LNInput_RemoveBinding
        case 0x538 : {
            // Fetch gesture
            const auto local_gesture = fetchVAInt();

            stat = LNInput_RemoveBinding(static_cast<LNHandle>(local_gesture));


            return true;
        }
        // LNInput_ClearBindingsA
        case 0x53A : {
            // Fetch buttonName
            const auto local_buttonName = fetchVAString();

            stat = LNInput_ClearBindingsA(static_cast<const char*>(local_buttonName));


            return true;
        }
        // LNInput_ClearAllBindings
        case 0x53C : {

            stat = LNInput_ClearAllBindings();


            return true;
        }
        // LNMouse_Pressed
        case 0x53E : {
            // Fetch button
            const auto local_button = static_cast<LNMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNMouse_Pressed(static_cast<LNMouseButtons>(local_button), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMouse_Triggered
        case 0x540 : {
            // Fetch button
            const auto local_button = static_cast<LNMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNMouse_Triggered(static_cast<LNMouseButtons>(local_button), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMouse_TriggeredOff
        case 0x542 : {
            // Fetch button
            const auto local_button = static_cast<LNMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNMouse_TriggeredOff(static_cast<LNMouseButtons>(local_button), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMouse_Repeated
        case 0x544 : {
            // Fetch button
            const auto local_button = static_cast<LNMouseButtons>(fetchVAInt());
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNMouse_Repeated(static_cast<LNMouseButtons>(local_button), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNMouse_GetPosition
        case 0x546 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNPoint local_outReturn;

            stat = LNMouse_GetPosition(&local_outReturn);
            code_setva(pval_outReturn, aptr_outReturn, hspLNPoint_typeid(), &local_outReturn);

            return true;
        }
        // LNInterpreterCommand_CodeA
        case 0x548 : {
            // Fetch interpretercommand
            const auto local_interpretercommand = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LNInterpreterCommand_CodeA(static_cast<LNHandle>(local_interpretercommand), &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInterpreterCommand_ParamsCount
        case 0x549 : {
            // Fetch interpretercommand
            const auto local_interpretercommand = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LNInterpreterCommand_ParamsCount(static_cast<LNHandle>(local_interpretercommand), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInterpreterCommand_ParamA
        case 0x54A : {
            // Fetch interpretercommand
            const auto local_interpretercommand = fetchVAInt();
            // Fetch index
            const auto local_index = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LNInterpreterCommand_ParamA(static_cast<LNHandle>(local_interpretercommand), static_cast<int>(local_index), &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInterpreterCommand_SetPrototype_OnSerialize
        case 0xEB8 : {
            // Fetch interpretercommand
            const auto local_interpretercommand = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNInterpreterCommandSerializeHandler_Create(HSPSubclass_LNInterpreterCommandSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterCommandSerializeHandler*>(LNInterpreterCommandSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNInterpreterCommand_SetPrototype_OnSerialize(static_cast<LNHandle>(local_interpretercommand), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNInterpreterCommandList_Create
        case 0x561 : {
            // Fetch outInterpreterCommandList
            PVal* pval_outInterpreterCommandList;
            const APTR aptr_outInterpreterCommandList = code_getva(&pval_outInterpreterCommandList);
            LNHandle local_outInterpreterCommandList;

            stat = LNInterpreterCommandList_Create(&local_outInterpreterCommandList);
            setVAInt(pval_outInterpreterCommandList, aptr_outInterpreterCommandList, local_outInterpreterCommandList);

            return true;
        }
        // LNInterpreterCommandList_AddCommandA
        case 0x54D : {
            // Fetch interpretercommandlist
            const auto local_interpretercommandlist = fetchVAInt();
            // Fetch code
            const auto local_code = fetchVAString();

            stat = LNInterpreterCommandList_AddCommandA(static_cast<LNHandle>(local_interpretercommandlist), static_cast<const char*>(local_code));


            return true;
        }
        // LNInterpreterCommandList_AddCommand1A
        case 0x54F : {
            // Fetch interpretercommandlist
            const auto local_interpretercommandlist = fetchVAInt();
            // Fetch code
            const auto local_code = fetchVAString();
            // Fetch param0
            const auto local_param0 = fetchVAString();

            stat = LNInterpreterCommandList_AddCommand1A(static_cast<LNHandle>(local_interpretercommandlist), static_cast<const char*>(local_code), static_cast<const char*>(local_param0));


            return true;
        }
        // LNInterpreterCommandList_AddCommand2A
        case 0x552 : {
            // Fetch interpretercommandlist
            const auto local_interpretercommandlist = fetchVAInt();
            // Fetch code
            const auto local_code = fetchVAString();
            // Fetch param0
            const auto local_param0 = fetchVAString();
            // Fetch param1
            const auto local_param1 = fetchVAString();

            stat = LNInterpreterCommandList_AddCommand2A(static_cast<LNHandle>(local_interpretercommandlist), static_cast<const char*>(local_code), static_cast<const char*>(local_param0), static_cast<const char*>(local_param1));


            return true;
        }
        // LNInterpreterCommandList_AddCommand3A
        case 0x556 : {
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

            stat = LNInterpreterCommandList_AddCommand3A(static_cast<LNHandle>(local_interpretercommandlist), static_cast<const char*>(local_code), static_cast<const char*>(local_param0), static_cast<const char*>(local_param1), static_cast<const char*>(local_param2));


            return true;
        }
        // LNInterpreterCommandList_AddCommand4A
        case 0x55B : {
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

            stat = LNInterpreterCommandList_AddCommand4A(static_cast<LNHandle>(local_interpretercommandlist), static_cast<const char*>(local_code), static_cast<const char*>(local_param0), static_cast<const char*>(local_param1), static_cast<const char*>(local_param2), static_cast<const char*>(local_param3));


            return true;
        }
        // LNInterpreterCommandList_SetPrototype_OnSerialize
        case 0xEC3 : {
            // Fetch interpretercommandlist
            const auto local_interpretercommandlist = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNInterpreterCommandListSerializeHandler_Create(HSPSubclass_LNInterpreterCommandListSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterCommandListSerializeHandler*>(LNInterpreterCommandListSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNInterpreterCommandList_SetPrototype_OnSerialize(static_cast<LNHandle>(local_interpretercommandlist), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNInterpreterCommandDelegate_Create
        case 0xA03 : {
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
        case 0x575 : {
            // Fetch outInterpreter
            PVal* pval_outInterpreter;
            const APTR aptr_outInterpreter = code_getva(&pval_outInterpreter);
            LNHandle local_outInterpreter;

            stat = LNInterpreter_Create(&local_outInterpreter);
            setVAInt(pval_outInterpreter, aptr_outInterpreter, local_outInterpreter);

            return true;
        }
        // LNInterpreter_Clear
        case 0x566 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();

            stat = LNInterpreter_Clear(static_cast<LNHandle>(local_interpreter));


            return true;
        }
        // LNInterpreter_Run
        case 0x567 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch commandList
            const auto local_commandList = fetchVAInt();

            stat = LNInterpreter_Run(static_cast<LNHandle>(local_interpreter), static_cast<LNHandle>(local_commandList));


            return true;
        }
        // LNInterpreter_IsRunning
        case 0x569 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNInterpreter_IsRunning(static_cast<LNHandle>(local_interpreter), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInterpreter_Update
        case 0x56A : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();

            stat = LNInterpreter_Update(static_cast<LNHandle>(local_interpreter));


            return true;
        }
        // LNInterpreter_Terminate
        case 0x56B : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();

            stat = LNInterpreter_Terminate(static_cast<LNHandle>(local_interpreter));


            return true;
        }
        // LNInterpreter_RegisterCommandHandlerA
        case 0x56C : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch name
            const auto local_name = fetchVAString();
            // Fetch handler
            const auto local_handler = fetchVALabelPointer();

            LNHandle localDelegate_handler;
            {
                stat = LNInterpreterCommandDelegate_Create(HSPSubclass_LNInterpreterCommandDelegate_DelegateLabelCaller, &localDelegate_handler);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterCommandDelegate*>(LNInterpreterCommandDelegate_GetSubinstanceId(localDelegate_handler));
                localSelf->labelPointer = local_handler;
            }

            stat = LNInterpreter_RegisterCommandHandlerA(static_cast<LNHandle>(local_interpreter), static_cast<const char*>(local_name), localDelegate_handler);
            LNObject_Release(localDelegate_handler);

            return true;
        }
        // LNInterpreter_SetWaitModeA
        case 0x56F : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch mode
            const auto local_mode = fetchVAString();

            stat = LNInterpreter_SetWaitModeA(static_cast<LNHandle>(local_interpreter), static_cast<const char*>(local_mode));


            return true;
        }
        // LNInterpreter_GetWaitModeA
        case 0x571 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            const char* local_outReturn;

            stat = LNInterpreter_GetWaitModeA(static_cast<LNHandle>(local_interpreter), &local_outReturn);
            setVAStr(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInterpreter_SetWaitCount
        case 0x572 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch count
            const auto local_count = fetchVAInt();

            stat = LNInterpreter_SetWaitCount(static_cast<LNHandle>(local_interpreter), static_cast<int>(local_count));


            return true;
        }
        // LNInterpreter_GetWaitCount
        case 0x574 : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            int local_outReturn;

            stat = LNInterpreter_GetWaitCount(static_cast<LNHandle>(local_interpreter), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNInterpreter_SetPrototype_OnSerialize
        case 0xECE : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNInterpreterSerializeHandler_Create(HSPSubclass_LNInterpreterSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterSerializeHandler*>(LNInterpreterSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNInterpreter_SetPrototype_OnSerialize(static_cast<LNHandle>(local_interpreter), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNInterpreter_SetPrototype_OnUpdateWait
        case 0xEDA : {
            // Fetch interpreter
            const auto local_interpreter = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNInterpreterUpdateWaitHandler_Create(HSPSubclass_LNInterpreterUpdateWaitHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNInterpreterUpdateWaitHandler*>(LNInterpreterUpdateWaitHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNInterpreter_SetPrototype_OnUpdateWait(static_cast<LNHandle>(local_interpreter), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNEngineSettings_SetMainWindowTitleA
        case 0x578 : {
            // Fetch title
            const auto local_title = fetchVAString();

            stat = LNEngineSettings_SetMainWindowTitleA(static_cast<const char*>(local_title));


            return true;
        }
        // LNEngineSettings_SetMainWindowSize
        case 0x57A : {
            // Fetch width
            const auto local_width = fetchVAInt();
            // Fetch height
            const auto local_height = fetchVAInt();

            stat = LNEngineSettings_SetMainWindowSize(static_cast<int>(local_width), static_cast<int>(local_height));


            return true;
        }
        // LNEngineSettings_SetMainWorldViewSize
        case 0x57D : {
            // Fetch width
            const auto local_width = fetchVAInt();
            // Fetch height
            const auto local_height = fetchVAInt();

            stat = LNEngineSettings_SetMainWorldViewSize(static_cast<int>(local_width), static_cast<int>(local_height));


            return true;
        }
        // LNEngineSettings_SetMainWindowResizable
        case 0x580 : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNEngineSettings_SetMainWindowResizable(static_cast<LNBool>(local_value));


            return true;
        }
        // LNEngineSettings_AddAssetDirectoryA
        case 0x582 : {
            // Fetch path
            const auto local_path = fetchVAString();

            stat = LNEngineSettings_AddAssetDirectoryA(static_cast<const char*>(local_path));


            return true;
        }
        // LNEngineSettings_AddAssetArchiveA
        case 0x584 : {
            // Fetch fileFullPath
            const auto local_fileFullPath = fetchVAString();
            // Fetch password
            const auto local_password = fetchVAString();

            stat = LNEngineSettings_AddAssetArchiveA(static_cast<const char*>(local_fileFullPath), static_cast<const char*>(local_password));


            return true;
        }
        // LNEngineSettings_SetFrameRate
        case 0x587 : {
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LNEngineSettings_SetFrameRate(static_cast<int>(local_value));


            return true;
        }
        // LNEngineSettings_SetUIThemeA
        case 0x589 : {
            // Fetch value
            const auto local_value = fetchVAString();

            stat = LNEngineSettings_SetUIThemeA(static_cast<const char*>(local_value));


            return true;
        }
        // LNEngineSettings_SetFontFileA
        case 0x58B : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();

            stat = LNEngineSettings_SetFontFileA(static_cast<const char*>(local_filePath));


            return true;
        }
        // LNEngineSettings_SetDebugToolEnabled
        case 0x58D : {
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNEngineSettings_SetDebugToolEnabled(static_cast<LNBool>(local_enabled));


            return true;
        }
        // LNEngineSettings_SetDebugMode
        case 0x58F : {
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNEngineSettings_SetDebugMode(static_cast<LNBool>(local_enabled));


            return true;
        }
        // LNEngineSettings_SetEngineLogFilePathA
        case 0x591 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();

            stat = LNEngineSettings_SetEngineLogFilePathA(static_cast<const char*>(local_filePath));


            return true;
        }
        // LNEngineSettings_SetPriorityGPUNameA
        case 0x593 : {
            // Fetch filePath
            const auto local_filePath = fetchVAString();

            stat = LNEngineSettings_SetPriorityGPUNameA(static_cast<const char*>(local_filePath));


            return true;
        }
        // LNEngineSettings_SetDeveloperToolEnabled
        case 0x595 : {
            // Fetch enabled
            const auto local_enabled = static_cast<LNBool>(fetchVAInt());

            stat = LNEngineSettings_SetDeveloperToolEnabled(static_cast<LNBool>(local_enabled));


            return true;
        }
        // LNEngineSettings_SetUserMainWindow
        case 0x597 : {
            // Fetch value
            const auto local_value = fetchVAInt();

            stat = LNEngineSettings_SetUserMainWindow(static_cast<intptr_t>(local_value));


            return true;
        }
        // LNEngine_Initialize
        case 0x59A : {

            stat = LNEngine_Initialize();


            return true;
        }
        // LNEngine_Terminate
        case 0x59B : {

            stat = LNEngine_Terminate();


            return true;
        }
        // LNEngine_Update
        case 0x59C : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNBool local_outReturn;

            stat = LNEngine_Update(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);
            ctx->waittick = 0;
            *retVal = RUNMODE_AWAIT;

            return true;
        }
        // LNEngine_GetTime
        case 0x59D : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            double local_outReturn;

            stat = LNEngine_GetTime(&local_outReturn);
            setVADouble(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEngine_GetWorld
        case 0x59E : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNEngine_GetWorld(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEngine_GetMainCamera
        case 0x59F : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNEngine_GetMainCamera(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEngine_GetMainLight
        case 0x5A0 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNEngine_GetMainLight(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNEngine_GetRenderView
        case 0x5A1 : {
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNEngine_GetRenderView(&local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNApplication_Create
        case 0x5A7 : {
            // Fetch outApplication
            PVal* pval_outApplication;
            const APTR aptr_outApplication = code_getva(&pval_outApplication);
            LNHandle local_outApplication;

            stat = LNApplication_Create(&local_outApplication);
            setVAInt(pval_outApplication, aptr_outApplication, local_outApplication);

            return true;
        }
        // LNApplication_OnInit
        case 0x5A3 : {
            // Fetch application
            const auto local_application = fetchVAInt();

            stat = LNApplication_OnInit(static_cast<LNHandle>(local_application));


            return true;
        }
        // LNApplication_OnUpdate
        case 0x5A4 : {
            // Fetch application
            const auto local_application = fetchVAInt();

            stat = LNApplication_OnUpdate(static_cast<LNHandle>(local_application));


            return true;
        }
        // LNApplication_World
        case 0x5A5 : {
            // Fetch application
            const auto local_application = fetchVAInt();
            // Fetch outReturn
            PVal* pval_outReturn;
            const APTR aptr_outReturn = code_getva(&pval_outReturn);
            LNHandle local_outReturn;

            stat = LNApplication_World(static_cast<LNHandle>(local_application), &local_outReturn);
            setVAInt(pval_outReturn, aptr_outReturn, local_outReturn);

            return true;
        }
        // LNApplication_Run
        case 0x5A6 : {
            // Fetch application
            const auto local_application = fetchVAInt();

            stat = LNApplication_Run(static_cast<LNHandle>(local_application));


            return true;
        }
        // LNApplication_SetPrototype_OnSerialize
        case 0xEE5 : {
            // Fetch application
            const auto local_application = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNApplicationSerializeHandler_Create(HSPSubclass_LNApplicationSerializeHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNApplicationSerializeHandler*>(LNApplicationSerializeHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNApplication_SetPrototype_OnSerialize(static_cast<LNHandle>(local_application), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNApplication_SetPrototype_OnInit
        case 0xEF0 : {
            // Fetch application
            const auto local_application = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNApplicationInitHandler_Create(HSPSubclass_LNApplicationInitHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNApplicationInitHandler*>(LNApplicationInitHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNApplication_SetPrototype_OnInit(static_cast<LNHandle>(local_application), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNApplication_SetPrototype_OnUpdate
        case 0xEFB : {
            // Fetch application
            const auto local_application = fetchVAInt();
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();

            LNHandle localDelegate_callback;
            {
                stat = LNApplicationUpdateHandler_Create(HSPSubclass_LNApplicationUpdateHandler_DelegateLabelCaller, &localDelegate_callback);
                if (stat != LN_OK) return true;
                auto* localSelf = reinterpret_cast<HSPSubclass_LNApplicationUpdateHandler*>(LNApplicationUpdateHandler_GetSubinstanceId(localDelegate_callback));
                localSelf->labelPointer = local_callback;
            }

            stat = LNApplication_SetPrototype_OnUpdate(static_cast<LNHandle>(local_application), localDelegate_callback);
            LNObject_Release(localDelegate_callback);

            return true;
        }
        // LNDebug_SetGuideGridEnabled
        case 0x5A9 : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNDebug_SetGuideGridEnabled(static_cast<LNBool>(local_value));


            return true;
        }
        // LNDebug_SetPhysicsDebugDrawEnabled
        case 0x5AB : {
            // Fetch value
            const auto local_value = static_cast<LNBool>(fetchVAInt());

            stat = LNDebug_SetPhysicsDebugDrawEnabled(static_cast<LNBool>(local_value));


            return true;
        }
        // LNDebug_PrintA
        case 0x5AD : {
            // Fetch str
            const auto local_str = fetchVAString();

            stat = LNDebug_PrintA(static_cast<const char*>(local_str));


            return true;
        }
        // LNDebug_PrintWithTimeA
        case 0x5AF : {
            // Fetch time
            const auto local_time = fetchVADouble();
            // Fetch str
            const auto local_str = fetchVAString();

            stat = LNDebug_PrintWithTimeA(static_cast<float>(local_time), static_cast<const char*>(local_str));


            return true;
        }
        // LNDebug_PrintWithTimeAndColorA
        case 0x5B2 : {
            // Fetch time
            const auto local_time = fetchVADouble();
            // Fetch color
            PVal* pval_color;
            CodeGetVA_TypeChecked(&pval_color, LNColor);
            // Fetch str
            const auto local_str = fetchVAString();

            stat = LNDebug_PrintWithTimeAndColorA(static_cast<float>(local_time), reinterpret_cast<LNColor*>(pval_color->pt), static_cast<const char*>(local_str));


            return true;
        }
        // LNObjectSerializeHandler_Create
        case 0xA33 : {
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
        case 0xA3E : {
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
        // LNVariantSerializeHandler_Create
        case 0xA49 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outVariantSerializeHandler
            PVal* pval_outVariantSerializeHandler;
            const APTR aptr_outVariantSerializeHandler = code_getva(&pval_outVariantSerializeHandler);
            LNHandle local_outVariantSerializeHandler;

            stat = LNVariantSerializeHandler_Create(HSPSubclass_LNVariantSerializeHandler_DelegateLabelCaller, &local_outVariantSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNVariantSerializeHandler*>(LNVariantSerializeHandler_GetSubinstanceId(local_outVariantSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outVariantSerializeHandler, aptr_outVariantSerializeHandler, local_outVariantSerializeHandler);

            return true;
        }
        // LNZVTestClass1SerializeHandler_Create
        case 0xA54 : {
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
        case 0xA5F : {
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
        case 0xA6A : {
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
        // LNAssetObjectSerializeHandler_Create
        case 0xA75 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outAssetObjectSerializeHandler
            PVal* pval_outAssetObjectSerializeHandler;
            const APTR aptr_outAssetObjectSerializeHandler = code_getva(&pval_outAssetObjectSerializeHandler);
            LNHandle local_outAssetObjectSerializeHandler;

            stat = LNAssetObjectSerializeHandler_Create(HSPSubclass_LNAssetObjectSerializeHandler_DelegateLabelCaller, &local_outAssetObjectSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNAssetObjectSerializeHandler*>(LNAssetObjectSerializeHandler_GetSubinstanceId(local_outAssetObjectSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outAssetObjectSerializeHandler, aptr_outAssetObjectSerializeHandler, local_outAssetObjectSerializeHandler);

            return true;
        }
        // LNAssetImportSettingsSerializeHandler_Create
        case 0xA80 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outAssetImportSettingsSerializeHandler
            PVal* pval_outAssetImportSettingsSerializeHandler;
            const APTR aptr_outAssetImportSettingsSerializeHandler = code_getva(&pval_outAssetImportSettingsSerializeHandler);
            LNHandle local_outAssetImportSettingsSerializeHandler;

            stat = LNAssetImportSettingsSerializeHandler_Create(HSPSubclass_LNAssetImportSettingsSerializeHandler_DelegateLabelCaller, &local_outAssetImportSettingsSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNAssetImportSettingsSerializeHandler*>(LNAssetImportSettingsSerializeHandler_GetSubinstanceId(local_outAssetImportSettingsSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outAssetImportSettingsSerializeHandler, aptr_outAssetImportSettingsSerializeHandler, local_outAssetImportSettingsSerializeHandler);

            return true;
        }
        // LNAssetModelSerializeHandler_Create
        case 0xA8B : {
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
        // LNSoundSerializeHandler_Create
        case 0xA96 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSoundSerializeHandler
            PVal* pval_outSoundSerializeHandler;
            const APTR aptr_outSoundSerializeHandler = code_getva(&pval_outSoundSerializeHandler);
            LNHandle local_outSoundSerializeHandler;

            stat = LNSoundSerializeHandler_Create(HSPSubclass_LNSoundSerializeHandler_DelegateLabelCaller, &local_outSoundSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNSoundSerializeHandler*>(LNSoundSerializeHandler_GetSubinstanceId(local_outSoundSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSoundSerializeHandler, aptr_outSoundSerializeHandler, local_outSoundSerializeHandler);

            return true;
        }
        // LNTextureSerializeHandler_Create
        case 0xAA1 : {
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
        case 0xAAC : {
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
        // LNShaderSerializeHandler_Create
        case 0xAB7 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outShaderSerializeHandler
            PVal* pval_outShaderSerializeHandler;
            const APTR aptr_outShaderSerializeHandler = code_getva(&pval_outShaderSerializeHandler);
            LNHandle local_outShaderSerializeHandler;

            stat = LNShaderSerializeHandler_Create(HSPSubclass_LNShaderSerializeHandler_DelegateLabelCaller, &local_outShaderSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNShaderSerializeHandler*>(LNShaderSerializeHandler_GetSubinstanceId(local_outShaderSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outShaderSerializeHandler, aptr_outShaderSerializeHandler, local_outShaderSerializeHandler);

            return true;
        }
        // LNRenderViewSerializeHandler_Create
        case 0xAC2 : {
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
        // LNMaterialSerializeHandler_Create
        case 0xACD : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outMaterialSerializeHandler
            PVal* pval_outMaterialSerializeHandler;
            const APTR aptr_outMaterialSerializeHandler = code_getva(&pval_outMaterialSerializeHandler);
            LNHandle local_outMaterialSerializeHandler;

            stat = LNMaterialSerializeHandler_Create(HSPSubclass_LNMaterialSerializeHandler_DelegateLabelCaller, &local_outMaterialSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNMaterialSerializeHandler*>(LNMaterialSerializeHandler_GetSubinstanceId(local_outMaterialSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outMaterialSerializeHandler, aptr_outMaterialSerializeHandler, local_outMaterialSerializeHandler);

            return true;
        }
        // LNMeshNodeSerializeHandler_Create
        case 0xAD8 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outMeshNodeSerializeHandler
            PVal* pval_outMeshNodeSerializeHandler;
            const APTR aptr_outMeshNodeSerializeHandler = code_getva(&pval_outMeshNodeSerializeHandler);
            LNHandle local_outMeshNodeSerializeHandler;

            stat = LNMeshNodeSerializeHandler_Create(HSPSubclass_LNMeshNodeSerializeHandler_DelegateLabelCaller, &local_outMeshNodeSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNMeshNodeSerializeHandler*>(LNMeshNodeSerializeHandler_GetSubinstanceId(local_outMeshNodeSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outMeshNodeSerializeHandler, aptr_outMeshNodeSerializeHandler, local_outMeshNodeSerializeHandler);

            return true;
        }
        // LNAnimationControllerSerializeHandler_Create
        case 0xAE3 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outAnimationControllerSerializeHandler
            PVal* pval_outAnimationControllerSerializeHandler;
            const APTR aptr_outAnimationControllerSerializeHandler = code_getva(&pval_outAnimationControllerSerializeHandler);
            LNHandle local_outAnimationControllerSerializeHandler;

            stat = LNAnimationControllerSerializeHandler_Create(HSPSubclass_LNAnimationControllerSerializeHandler_DelegateLabelCaller, &local_outAnimationControllerSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNAnimationControllerSerializeHandler*>(LNAnimationControllerSerializeHandler_GetSubinstanceId(local_outAnimationControllerSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outAnimationControllerSerializeHandler, aptr_outAnimationControllerSerializeHandler, local_outAnimationControllerSerializeHandler);

            return true;
        }
        // LNMeshModelSerializeHandler_Create
        case 0xAEE : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outMeshModelSerializeHandler
            PVal* pval_outMeshModelSerializeHandler;
            const APTR aptr_outMeshModelSerializeHandler = code_getva(&pval_outMeshModelSerializeHandler);
            LNHandle local_outMeshModelSerializeHandler;

            stat = LNMeshModelSerializeHandler_Create(HSPSubclass_LNMeshModelSerializeHandler_DelegateLabelCaller, &local_outMeshModelSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNMeshModelSerializeHandler*>(LNMeshModelSerializeHandler_GetSubinstanceId(local_outMeshModelSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outMeshModelSerializeHandler, aptr_outMeshModelSerializeHandler, local_outMeshModelSerializeHandler);

            return true;
        }
        // LNMeshImportSettingsSerializeHandler_Create
        case 0xAF9 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outMeshImportSettingsSerializeHandler
            PVal* pval_outMeshImportSettingsSerializeHandler;
            const APTR aptr_outMeshImportSettingsSerializeHandler = code_getva(&pval_outMeshImportSettingsSerializeHandler);
            LNHandle local_outMeshImportSettingsSerializeHandler;

            stat = LNMeshImportSettingsSerializeHandler_Create(HSPSubclass_LNMeshImportSettingsSerializeHandler_DelegateLabelCaller, &local_outMeshImportSettingsSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNMeshImportSettingsSerializeHandler*>(LNMeshImportSettingsSerializeHandler_GetSubinstanceId(local_outMeshImportSettingsSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outMeshImportSettingsSerializeHandler, aptr_outMeshImportSettingsSerializeHandler, local_outMeshImportSettingsSerializeHandler);

            return true;
        }
        // LNSkinnedMeshModelSerializeHandler_Create
        case 0xB04 : {
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
        case 0xB0F : {
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
        case 0xB1A : {
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
        // LNAnimationCurveSerializeHandler_Create
        case 0xB25 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outAnimationCurveSerializeHandler
            PVal* pval_outAnimationCurveSerializeHandler;
            const APTR aptr_outAnimationCurveSerializeHandler = code_getva(&pval_outAnimationCurveSerializeHandler);
            LNHandle local_outAnimationCurveSerializeHandler;

            stat = LNAnimationCurveSerializeHandler_Create(HSPSubclass_LNAnimationCurveSerializeHandler_DelegateLabelCaller, &local_outAnimationCurveSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNAnimationCurveSerializeHandler*>(LNAnimationCurveSerializeHandler_GetSubinstanceId(local_outAnimationCurveSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outAnimationCurveSerializeHandler, aptr_outAnimationCurveSerializeHandler, local_outAnimationCurveSerializeHandler);

            return true;
        }
        // LNKeyFrameAnimationCurveSerializeHandler_Create
        case 0xB30 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outKeyFrameAnimationCurveSerializeHandler
            PVal* pval_outKeyFrameAnimationCurveSerializeHandler;
            const APTR aptr_outKeyFrameAnimationCurveSerializeHandler = code_getva(&pval_outKeyFrameAnimationCurveSerializeHandler);
            LNHandle local_outKeyFrameAnimationCurveSerializeHandler;

            stat = LNKeyFrameAnimationCurveSerializeHandler_Create(HSPSubclass_LNKeyFrameAnimationCurveSerializeHandler_DelegateLabelCaller, &local_outKeyFrameAnimationCurveSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNKeyFrameAnimationCurveSerializeHandler*>(LNKeyFrameAnimationCurveSerializeHandler_GetSubinstanceId(local_outKeyFrameAnimationCurveSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outKeyFrameAnimationCurveSerializeHandler, aptr_outKeyFrameAnimationCurveSerializeHandler, local_outKeyFrameAnimationCurveSerializeHandler);

            return true;
        }
        // LNAnimationClipSerializeHandler_Create
        case 0xB3B : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outAnimationClipSerializeHandler
            PVal* pval_outAnimationClipSerializeHandler;
            const APTR aptr_outAnimationClipSerializeHandler = code_getva(&pval_outAnimationClipSerializeHandler);
            LNHandle local_outAnimationClipSerializeHandler;

            stat = LNAnimationClipSerializeHandler_Create(HSPSubclass_LNAnimationClipSerializeHandler_DelegateLabelCaller, &local_outAnimationClipSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNAnimationClipSerializeHandler*>(LNAnimationClipSerializeHandler_GetSubinstanceId(local_outAnimationClipSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outAnimationClipSerializeHandler, aptr_outAnimationClipSerializeHandler, local_outAnimationClipSerializeHandler);

            return true;
        }
        // LNAnimationStateSerializeHandler_Create
        case 0xB46 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outAnimationStateSerializeHandler
            PVal* pval_outAnimationStateSerializeHandler;
            const APTR aptr_outAnimationStateSerializeHandler = code_getva(&pval_outAnimationStateSerializeHandler);
            LNHandle local_outAnimationStateSerializeHandler;

            stat = LNAnimationStateSerializeHandler_Create(HSPSubclass_LNAnimationStateSerializeHandler_DelegateLabelCaller, &local_outAnimationStateSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNAnimationStateSerializeHandler*>(LNAnimationStateSerializeHandler_GetSubinstanceId(local_outAnimationStateSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outAnimationStateSerializeHandler, aptr_outAnimationStateSerializeHandler, local_outAnimationStateSerializeHandler);

            return true;
        }
        // LNEffectResourceSerializeHandler_Create
        case 0xB51 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outEffectResourceSerializeHandler
            PVal* pval_outEffectResourceSerializeHandler;
            const APTR aptr_outEffectResourceSerializeHandler = code_getva(&pval_outEffectResourceSerializeHandler);
            LNHandle local_outEffectResourceSerializeHandler;

            stat = LNEffectResourceSerializeHandler_Create(HSPSubclass_LNEffectResourceSerializeHandler_DelegateLabelCaller, &local_outEffectResourceSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNEffectResourceSerializeHandler*>(LNEffectResourceSerializeHandler_GetSubinstanceId(local_outEffectResourceSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outEffectResourceSerializeHandler, aptr_outEffectResourceSerializeHandler, local_outEffectResourceSerializeHandler);

            return true;
        }
        // LNParticleEmitterModelSerializeHandler_Create
        case 0xB5C : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outParticleEmitterModelSerializeHandler
            PVal* pval_outParticleEmitterModelSerializeHandler;
            const APTR aptr_outParticleEmitterModelSerializeHandler = code_getva(&pval_outParticleEmitterModelSerializeHandler);
            LNHandle local_outParticleEmitterModelSerializeHandler;

            stat = LNParticleEmitterModelSerializeHandler_Create(HSPSubclass_LNParticleEmitterModelSerializeHandler_DelegateLabelCaller, &local_outParticleEmitterModelSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleEmitterModelSerializeHandler*>(LNParticleEmitterModelSerializeHandler_GetSubinstanceId(local_outParticleEmitterModelSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outParticleEmitterModelSerializeHandler, aptr_outParticleEmitterModelSerializeHandler, local_outParticleEmitterModelSerializeHandler);

            return true;
        }
        // LNParticleModelSerializeHandler_Create
        case 0xB67 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outParticleModelSerializeHandler
            PVal* pval_outParticleModelSerializeHandler;
            const APTR aptr_outParticleModelSerializeHandler = code_getva(&pval_outParticleModelSerializeHandler);
            LNHandle local_outParticleModelSerializeHandler;

            stat = LNParticleModelSerializeHandler_Create(HSPSubclass_LNParticleModelSerializeHandler_DelegateLabelCaller, &local_outParticleModelSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleModelSerializeHandler*>(LNParticleModelSerializeHandler_GetSubinstanceId(local_outParticleModelSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outParticleModelSerializeHandler, aptr_outParticleModelSerializeHandler, local_outParticleModelSerializeHandler);

            return true;
        }
        // LNComponentSerializeHandler_Create
        case 0xB72 : {
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
        case 0xB7D : {
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
        case 0xB88 : {
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
        case 0xB93 : {
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
        case 0xB9E : {
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
        case 0xBA9 : {
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
        case 0xBB4 : {
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
        // LNWorldObjectPreUpdateHandler_Create
        case 0xBBF : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outWorldObjectPreUpdateHandler
            PVal* pval_outWorldObjectPreUpdateHandler;
            const APTR aptr_outWorldObjectPreUpdateHandler = code_getva(&pval_outWorldObjectPreUpdateHandler);
            LNHandle local_outWorldObjectPreUpdateHandler;

            stat = LNWorldObjectPreUpdateHandler_Create(HSPSubclass_LNWorldObjectPreUpdateHandler_DelegateLabelCaller, &local_outWorldObjectPreUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNWorldObjectPreUpdateHandler*>(LNWorldObjectPreUpdateHandler_GetSubinstanceId(local_outWorldObjectPreUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outWorldObjectPreUpdateHandler, aptr_outWorldObjectPreUpdateHandler, local_outWorldObjectPreUpdateHandler);

            return true;
        }
        // LNWorldObjectUpdateHandler_Create
        case 0xBCA : {
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
        case 0xBD5 : {
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
        // LNVisualObjectPreUpdateHandler_Create
        case 0xBE0 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outVisualObjectPreUpdateHandler
            PVal* pval_outVisualObjectPreUpdateHandler;
            const APTR aptr_outVisualObjectPreUpdateHandler = code_getva(&pval_outVisualObjectPreUpdateHandler);
            LNHandle local_outVisualObjectPreUpdateHandler;

            stat = LNVisualObjectPreUpdateHandler_Create(HSPSubclass_LNVisualObjectPreUpdateHandler_DelegateLabelCaller, &local_outVisualObjectPreUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNVisualObjectPreUpdateHandler*>(LNVisualObjectPreUpdateHandler_GetSubinstanceId(local_outVisualObjectPreUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outVisualObjectPreUpdateHandler, aptr_outVisualObjectPreUpdateHandler, local_outVisualObjectPreUpdateHandler);

            return true;
        }
        // LNVisualObjectUpdateHandler_Create
        case 0xBEB : {
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
        case 0xBF6 : {
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
        // LNCameraPreUpdateHandler_Create
        case 0xC01 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outCameraPreUpdateHandler
            PVal* pval_outCameraPreUpdateHandler;
            const APTR aptr_outCameraPreUpdateHandler = code_getva(&pval_outCameraPreUpdateHandler);
            LNHandle local_outCameraPreUpdateHandler;

            stat = LNCameraPreUpdateHandler_Create(HSPSubclass_LNCameraPreUpdateHandler_DelegateLabelCaller, &local_outCameraPreUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNCameraPreUpdateHandler*>(LNCameraPreUpdateHandler_GetSubinstanceId(local_outCameraPreUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outCameraPreUpdateHandler, aptr_outCameraPreUpdateHandler, local_outCameraPreUpdateHandler);

            return true;
        }
        // LNCameraUpdateHandler_Create
        case 0xC0C : {
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
        case 0xC17 : {
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
        // LNEnvironmentLightPreUpdateHandler_Create
        case 0xC22 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outEnvironmentLightPreUpdateHandler
            PVal* pval_outEnvironmentLightPreUpdateHandler;
            const APTR aptr_outEnvironmentLightPreUpdateHandler = code_getva(&pval_outEnvironmentLightPreUpdateHandler);
            LNHandle local_outEnvironmentLightPreUpdateHandler;

            stat = LNEnvironmentLightPreUpdateHandler_Create(HSPSubclass_LNEnvironmentLightPreUpdateHandler_DelegateLabelCaller, &local_outEnvironmentLightPreUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNEnvironmentLightPreUpdateHandler*>(LNEnvironmentLightPreUpdateHandler_GetSubinstanceId(local_outEnvironmentLightPreUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outEnvironmentLightPreUpdateHandler, aptr_outEnvironmentLightPreUpdateHandler, local_outEnvironmentLightPreUpdateHandler);

            return true;
        }
        // LNEnvironmentLightUpdateHandler_Create
        case 0xC2D : {
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
        case 0xC38 : {
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
        // LNDirectionalLightPreUpdateHandler_Create
        case 0xC43 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outDirectionalLightPreUpdateHandler
            PVal* pval_outDirectionalLightPreUpdateHandler;
            const APTR aptr_outDirectionalLightPreUpdateHandler = code_getva(&pval_outDirectionalLightPreUpdateHandler);
            LNHandle local_outDirectionalLightPreUpdateHandler;

            stat = LNDirectionalLightPreUpdateHandler_Create(HSPSubclass_LNDirectionalLightPreUpdateHandler_DelegateLabelCaller, &local_outDirectionalLightPreUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNDirectionalLightPreUpdateHandler*>(LNDirectionalLightPreUpdateHandler_GetSubinstanceId(local_outDirectionalLightPreUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outDirectionalLightPreUpdateHandler, aptr_outDirectionalLightPreUpdateHandler, local_outDirectionalLightPreUpdateHandler);

            return true;
        }
        // LNDirectionalLightUpdateHandler_Create
        case 0xC4E : {
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
        case 0xC59 : {
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
        // LNPointLightPreUpdateHandler_Create
        case 0xC64 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outPointLightPreUpdateHandler
            PVal* pval_outPointLightPreUpdateHandler;
            const APTR aptr_outPointLightPreUpdateHandler = code_getva(&pval_outPointLightPreUpdateHandler);
            LNHandle local_outPointLightPreUpdateHandler;

            stat = LNPointLightPreUpdateHandler_Create(HSPSubclass_LNPointLightPreUpdateHandler_DelegateLabelCaller, &local_outPointLightPreUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNPointLightPreUpdateHandler*>(LNPointLightPreUpdateHandler_GetSubinstanceId(local_outPointLightPreUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outPointLightPreUpdateHandler, aptr_outPointLightPreUpdateHandler, local_outPointLightPreUpdateHandler);

            return true;
        }
        // LNPointLightUpdateHandler_Create
        case 0xC6F : {
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
        case 0xC7A : {
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
        // LNSpotLightPreUpdateHandler_Create
        case 0xC85 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSpotLightPreUpdateHandler
            PVal* pval_outSpotLightPreUpdateHandler;
            const APTR aptr_outSpotLightPreUpdateHandler = code_getva(&pval_outSpotLightPreUpdateHandler);
            LNHandle local_outSpotLightPreUpdateHandler;

            stat = LNSpotLightPreUpdateHandler_Create(HSPSubclass_LNSpotLightPreUpdateHandler_DelegateLabelCaller, &local_outSpotLightPreUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNSpotLightPreUpdateHandler*>(LNSpotLightPreUpdateHandler_GetSubinstanceId(local_outSpotLightPreUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSpotLightPreUpdateHandler, aptr_outSpotLightPreUpdateHandler, local_outSpotLightPreUpdateHandler);

            return true;
        }
        // LNSpotLightUpdateHandler_Create
        case 0xC90 : {
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
        case 0xC9B : {
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
        // LNSpritePreUpdateHandler_Create
        case 0xCA6 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outSpritePreUpdateHandler
            PVal* pval_outSpritePreUpdateHandler;
            const APTR aptr_outSpritePreUpdateHandler = code_getva(&pval_outSpritePreUpdateHandler);
            LNHandle local_outSpritePreUpdateHandler;

            stat = LNSpritePreUpdateHandler_Create(HSPSubclass_LNSpritePreUpdateHandler_DelegateLabelCaller, &local_outSpritePreUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNSpritePreUpdateHandler*>(LNSpritePreUpdateHandler_GetSubinstanceId(local_outSpritePreUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outSpritePreUpdateHandler, aptr_outSpritePreUpdateHandler, local_outSpritePreUpdateHandler);

            return true;
        }
        // LNSpriteUpdateHandler_Create
        case 0xCB1 : {
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
        case 0xCBC : {
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
        case 0xCC7 : {
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
        case 0xCD2 : {
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
        case 0xCDD : {
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
        // LNShapeObjectSerializeHandler_Create
        case 0xCE8 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outShapeObjectSerializeHandler
            PVal* pval_outShapeObjectSerializeHandler;
            const APTR aptr_outShapeObjectSerializeHandler = code_getva(&pval_outShapeObjectSerializeHandler);
            LNHandle local_outShapeObjectSerializeHandler;

            stat = LNShapeObjectSerializeHandler_Create(HSPSubclass_LNShapeObjectSerializeHandler_DelegateLabelCaller, &local_outShapeObjectSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNShapeObjectSerializeHandler*>(LNShapeObjectSerializeHandler_GetSubinstanceId(local_outShapeObjectSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outShapeObjectSerializeHandler, aptr_outShapeObjectSerializeHandler, local_outShapeObjectSerializeHandler);

            return true;
        }
        // LNShapeObjectPreUpdateHandler_Create
        case 0xCF3 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outShapeObjectPreUpdateHandler
            PVal* pval_outShapeObjectPreUpdateHandler;
            const APTR aptr_outShapeObjectPreUpdateHandler = code_getva(&pval_outShapeObjectPreUpdateHandler);
            LNHandle local_outShapeObjectPreUpdateHandler;

            stat = LNShapeObjectPreUpdateHandler_Create(HSPSubclass_LNShapeObjectPreUpdateHandler_DelegateLabelCaller, &local_outShapeObjectPreUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNShapeObjectPreUpdateHandler*>(LNShapeObjectPreUpdateHandler_GetSubinstanceId(local_outShapeObjectPreUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outShapeObjectPreUpdateHandler, aptr_outShapeObjectPreUpdateHandler, local_outShapeObjectPreUpdateHandler);

            return true;
        }
        // LNShapeObjectUpdateHandler_Create
        case 0xCFE : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outShapeObjectUpdateHandler
            PVal* pval_outShapeObjectUpdateHandler;
            const APTR aptr_outShapeObjectUpdateHandler = code_getva(&pval_outShapeObjectUpdateHandler);
            LNHandle local_outShapeObjectUpdateHandler;

            stat = LNShapeObjectUpdateHandler_Create(HSPSubclass_LNShapeObjectUpdateHandler_DelegateLabelCaller, &local_outShapeObjectUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNShapeObjectUpdateHandler*>(LNShapeObjectUpdateHandler_GetSubinstanceId(local_outShapeObjectUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outShapeObjectUpdateHandler, aptr_outShapeObjectUpdateHandler, local_outShapeObjectUpdateHandler);

            return true;
        }
        // LNPlaneMeshSerializeHandler_Create
        case 0xD09 : {
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
        // LNPlaneMeshPreUpdateHandler_Create
        case 0xD14 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outPlaneMeshPreUpdateHandler
            PVal* pval_outPlaneMeshPreUpdateHandler;
            const APTR aptr_outPlaneMeshPreUpdateHandler = code_getva(&pval_outPlaneMeshPreUpdateHandler);
            LNHandle local_outPlaneMeshPreUpdateHandler;

            stat = LNPlaneMeshPreUpdateHandler_Create(HSPSubclass_LNPlaneMeshPreUpdateHandler_DelegateLabelCaller, &local_outPlaneMeshPreUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNPlaneMeshPreUpdateHandler*>(LNPlaneMeshPreUpdateHandler_GetSubinstanceId(local_outPlaneMeshPreUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outPlaneMeshPreUpdateHandler, aptr_outPlaneMeshPreUpdateHandler, local_outPlaneMeshPreUpdateHandler);

            return true;
        }
        // LNPlaneMeshUpdateHandler_Create
        case 0xD1F : {
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
        // LNBoxMeshSerializeHandler_Create
        case 0xD2A : {
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
        // LNBoxMeshPreUpdateHandler_Create
        case 0xD35 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outBoxMeshPreUpdateHandler
            PVal* pval_outBoxMeshPreUpdateHandler;
            const APTR aptr_outBoxMeshPreUpdateHandler = code_getva(&pval_outBoxMeshPreUpdateHandler);
            LNHandle local_outBoxMeshPreUpdateHandler;

            stat = LNBoxMeshPreUpdateHandler_Create(HSPSubclass_LNBoxMeshPreUpdateHandler_DelegateLabelCaller, &local_outBoxMeshPreUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNBoxMeshPreUpdateHandler*>(LNBoxMeshPreUpdateHandler_GetSubinstanceId(local_outBoxMeshPreUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outBoxMeshPreUpdateHandler, aptr_outBoxMeshPreUpdateHandler, local_outBoxMeshPreUpdateHandler);

            return true;
        }
        // LNBoxMeshUpdateHandler_Create
        case 0xD40 : {
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
        // LNMeshComponentSerializeHandler_Create
        case 0xD4B : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outMeshComponentSerializeHandler
            PVal* pval_outMeshComponentSerializeHandler;
            const APTR aptr_outMeshComponentSerializeHandler = code_getva(&pval_outMeshComponentSerializeHandler);
            LNHandle local_outMeshComponentSerializeHandler;

            stat = LNMeshComponentSerializeHandler_Create(HSPSubclass_LNMeshComponentSerializeHandler_DelegateLabelCaller, &local_outMeshComponentSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNMeshComponentSerializeHandler*>(LNMeshComponentSerializeHandler_GetSubinstanceId(local_outMeshComponentSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outMeshComponentSerializeHandler, aptr_outMeshComponentSerializeHandler, local_outMeshComponentSerializeHandler);

            return true;
        }
        // LNCollisionSerializeHandler_Create
        case 0xD56 : {
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
        case 0xD61 : {
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
        // LNParticleEmitterSerializeHandler_Create
        case 0xD6C : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outParticleEmitterSerializeHandler
            PVal* pval_outParticleEmitterSerializeHandler;
            const APTR aptr_outParticleEmitterSerializeHandler = code_getva(&pval_outParticleEmitterSerializeHandler);
            LNHandle local_outParticleEmitterSerializeHandler;

            stat = LNParticleEmitterSerializeHandler_Create(HSPSubclass_LNParticleEmitterSerializeHandler_DelegateLabelCaller, &local_outParticleEmitterSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleEmitterSerializeHandler*>(LNParticleEmitterSerializeHandler_GetSubinstanceId(local_outParticleEmitterSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outParticleEmitterSerializeHandler, aptr_outParticleEmitterSerializeHandler, local_outParticleEmitterSerializeHandler);

            return true;
        }
        // LNParticleEmitterPreUpdateHandler_Create
        case 0xD77 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outParticleEmitterPreUpdateHandler
            PVal* pval_outParticleEmitterPreUpdateHandler;
            const APTR aptr_outParticleEmitterPreUpdateHandler = code_getva(&pval_outParticleEmitterPreUpdateHandler);
            LNHandle local_outParticleEmitterPreUpdateHandler;

            stat = LNParticleEmitterPreUpdateHandler_Create(HSPSubclass_LNParticleEmitterPreUpdateHandler_DelegateLabelCaller, &local_outParticleEmitterPreUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleEmitterPreUpdateHandler*>(LNParticleEmitterPreUpdateHandler_GetSubinstanceId(local_outParticleEmitterPreUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outParticleEmitterPreUpdateHandler, aptr_outParticleEmitterPreUpdateHandler, local_outParticleEmitterPreUpdateHandler);

            return true;
        }
        // LNParticleEmitterUpdateHandler_Create
        case 0xD82 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outParticleEmitterUpdateHandler
            PVal* pval_outParticleEmitterUpdateHandler;
            const APTR aptr_outParticleEmitterUpdateHandler = code_getva(&pval_outParticleEmitterUpdateHandler);
            LNHandle local_outParticleEmitterUpdateHandler;

            stat = LNParticleEmitterUpdateHandler_Create(HSPSubclass_LNParticleEmitterUpdateHandler_DelegateLabelCaller, &local_outParticleEmitterUpdateHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNParticleEmitterUpdateHandler*>(LNParticleEmitterUpdateHandler_GetSubinstanceId(local_outParticleEmitterUpdateHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outParticleEmitterUpdateHandler, aptr_outParticleEmitterUpdateHandler, local_outParticleEmitterUpdateHandler);

            return true;
        }
        // LNLevelSerializeHandler_Create
        case 0xD8D : {
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
        case 0xD98 : {
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
        case 0xDA3 : {
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
        case 0xDAE : {
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
        case 0xDB9 : {
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
        case 0xDC4 : {
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
        case 0xDCF : {
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
        case 0xDDA : {
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
        case 0xDE5 : {
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
        // LNUITextSerializeHandler_Create
        case 0xDF0 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUITextSerializeHandler
            PVal* pval_outUITextSerializeHandler;
            const APTR aptr_outUITextSerializeHandler = code_getva(&pval_outUITextSerializeHandler);
            LNHandle local_outUITextSerializeHandler;

            stat = LNUITextSerializeHandler_Create(HSPSubclass_LNUITextSerializeHandler_DelegateLabelCaller, &local_outUITextSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUITextSerializeHandler*>(LNUITextSerializeHandler_GetSubinstanceId(local_outUITextSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUITextSerializeHandler, aptr_outUITextSerializeHandler, local_outUITextSerializeHandler);

            return true;
        }
        // LNUISpriteSerializeHandler_Create
        case 0xDFB : {
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
        case 0xE06 : {
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
        // LNUIMessageTextAreaSerializeHandler_Create
        case 0xE11 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIMessageTextAreaSerializeHandler
            PVal* pval_outUIMessageTextAreaSerializeHandler;
            const APTR aptr_outUIMessageTextAreaSerializeHandler = code_getva(&pval_outUIMessageTextAreaSerializeHandler);
            LNHandle local_outUIMessageTextAreaSerializeHandler;

            stat = LNUIMessageTextAreaSerializeHandler_Create(HSPSubclass_LNUIMessageTextAreaSerializeHandler_DelegateLabelCaller, &local_outUIMessageTextAreaSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIMessageTextAreaSerializeHandler*>(LNUIMessageTextAreaSerializeHandler_GetSubinstanceId(local_outUIMessageTextAreaSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIMessageTextAreaSerializeHandler, aptr_outUIMessageTextAreaSerializeHandler, local_outUIMessageTextAreaSerializeHandler);

            return true;
        }
        // LNUILayoutPanelSerializeHandler_Create
        case 0xE1C : {
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
        case 0xE27 : {
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
        // LNUIStackLayoutSerializeHandler_Create
        case 0xE32 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outUIStackLayoutSerializeHandler
            PVal* pval_outUIStackLayoutSerializeHandler;
            const APTR aptr_outUIStackLayoutSerializeHandler = code_getva(&pval_outUIStackLayoutSerializeHandler);
            LNHandle local_outUIStackLayoutSerializeHandler;

            stat = LNUIStackLayoutSerializeHandler_Create(HSPSubclass_LNUIStackLayoutSerializeHandler_DelegateLabelCaller, &local_outUIStackLayoutSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNUIStackLayoutSerializeHandler*>(LNUIStackLayoutSerializeHandler_GetSubinstanceId(local_outUIStackLayoutSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outUIStackLayoutSerializeHandler, aptr_outUIStackLayoutSerializeHandler, local_outUIStackLayoutSerializeHandler);

            return true;
        }
        // LNUIGridLayoutSerializeHandler_Create
        case 0xE3D : {
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
        case 0xE48 : {
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
        case 0xE53 : {
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
        case 0xE5E : {
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
        case 0xE69 : {
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
        case 0xE74 : {
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
        case 0xE7F : {
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
        case 0xE8A : {
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
        case 0xE95 : {
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
        // LNInputGestureSerializeHandler_Create
        case 0xEA0 : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outInputGestureSerializeHandler
            PVal* pval_outInputGestureSerializeHandler;
            const APTR aptr_outInputGestureSerializeHandler = code_getva(&pval_outInputGestureSerializeHandler);
            LNHandle local_outInputGestureSerializeHandler;

            stat = LNInputGestureSerializeHandler_Create(HSPSubclass_LNInputGestureSerializeHandler_DelegateLabelCaller, &local_outInputGestureSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNInputGestureSerializeHandler*>(LNInputGestureSerializeHandler_GetSubinstanceId(local_outInputGestureSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outInputGestureSerializeHandler, aptr_outInputGestureSerializeHandler, local_outInputGestureSerializeHandler);

            return true;
        }
        // LNKeyGestureSerializeHandler_Create
        case 0xEAB : {
            // Fetch callback
            const auto local_callback = fetchVALabelPointer();
            // Fetch outKeyGestureSerializeHandler
            PVal* pval_outKeyGestureSerializeHandler;
            const APTR aptr_outKeyGestureSerializeHandler = code_getva(&pval_outKeyGestureSerializeHandler);
            LNHandle local_outKeyGestureSerializeHandler;

            stat = LNKeyGestureSerializeHandler_Create(HSPSubclass_LNKeyGestureSerializeHandler_DelegateLabelCaller, &local_outKeyGestureSerializeHandler);
            auto* localSelf = reinterpret_cast<HSPSubclass_LNKeyGestureSerializeHandler*>(LNKeyGestureSerializeHandler_GetSubinstanceId(local_outKeyGestureSerializeHandler));
            localSelf->labelPointer = local_callback;
            setVAInt(pval_outKeyGestureSerializeHandler, aptr_outKeyGestureSerializeHandler, local_outKeyGestureSerializeHandler);

            return true;
        }
        // LNInterpreterCommandSerializeHandler_Create
        case 0xEB6 : {
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
        case 0xEC1 : {
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
        case 0xECC : {
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
        case 0xED8 : {
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
        case 0xEE3 : {
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
        case 0xEEE : {
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
        case 0xEF9 : {
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
    registvar(-1, hspLNColorTone_Init);
    registvar(-1, hspLNPoint_Init);
    registvar(-1, hspLNSize_Init);
    registvar(-1, hspLNRect_Init);
    registvar(-1, hspLNThickness_Init);
    registvar(-1, hspLNCornerRadius_Init);
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
        LNVariant_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNVariant_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNVariant_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNVariant_RegisterSubclassTypeInfo(&info);
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
        LNAssetObject_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAssetObject_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAssetObject_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNAssetObject_RegisterSubclassTypeInfo(&info);
    }
    {
        LNAssetImportSettings_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAssetImportSettings_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAssetImportSettings_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNAssetImportSettings_RegisterSubclassTypeInfo(&info);
    }
    {
        LNAssetModel_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAssetModel_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAssetModel_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNAssetModel_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSound_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSound_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSound_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNSound_RegisterSubclassTypeInfo(&info);
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
        LNShader_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNShader_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNShader_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNShader_RegisterSubclassTypeInfo(&info);
    }
    {
        LNRenderView_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNRenderView_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNRenderView_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNRenderView_RegisterSubclassTypeInfo(&info);
    }
    {
        LNMaterial_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNMaterial_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNMaterial_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNMaterial_RegisterSubclassTypeInfo(&info);
    }
    {
        LNMeshNode_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNMeshNode_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNMeshNode_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNMeshNode_RegisterSubclassTypeInfo(&info);
    }
    {
        LNAnimationController_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAnimationController_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAnimationController_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNAnimationController_RegisterSubclassTypeInfo(&info);
    }
    {
        LNMeshModel_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNMeshModel_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNMeshModel_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNMeshModel_RegisterSubclassTypeInfo(&info);
    }
    {
        LNMeshImportSettings_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNMeshImportSettings_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNMeshImportSettings_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNMeshImportSettings_RegisterSubclassTypeInfo(&info);
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
        LNAnimationCurve_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAnimationCurve_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAnimationCurve_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNAnimationCurve_RegisterSubclassTypeInfo(&info);
    }
    {
        LNKeyFrameAnimationCurve_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNKeyFrameAnimationCurve_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNKeyFrameAnimationCurve_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNKeyFrameAnimationCurve_RegisterSubclassTypeInfo(&info);
    }
    {
        LNAnimationClip_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAnimationClip_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAnimationClip_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNAnimationClip_RegisterSubclassTypeInfo(&info);
    }
    {
        LNAnimationState_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAnimationState_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAnimationState_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNAnimationState_RegisterSubclassTypeInfo(&info);
    }
    {
        LNEffectResource_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNEffectResource_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNEffectResource_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNEffectResource_RegisterSubclassTypeInfo(&info);
    }
    {
        LNParticleEmitterModel_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNParticleEmitterModel_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNParticleEmitterModel_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNParticleEmitterModel_RegisterSubclassTypeInfo(&info);
    }
    {
        LNParticleModel_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNParticleModel_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNParticleModel_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNParticleModel_RegisterSubclassTypeInfo(&info);
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
        //info.OnPreUpdate_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNWorldObject_RegisterSubclassTypeInfo(&info);
    }
    {
        LNVisualObject_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNVisualObject_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNVisualObject_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnPreUpdate_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNVisualObject_RegisterSubclassTypeInfo(&info);
    }
    {
        LNCamera_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCamera_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCamera_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnPreUpdate_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNCamera_RegisterSubclassTypeInfo(&info);
    }
    {
        LNEnvironmentLight_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNEnvironmentLight_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNEnvironmentLight_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnPreUpdate_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNEnvironmentLight_RegisterSubclassTypeInfo(&info);
    }
    {
        LNDirectionalLight_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNDirectionalLight_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNDirectionalLight_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnPreUpdate_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNDirectionalLight_RegisterSubclassTypeInfo(&info);
    }
    {
        LNPointLight_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNPointLight_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNPointLight_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnPreUpdate_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNPointLight_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSpotLight_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSpotLight_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSpotLight_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnPreUpdate_OverrideFunc = ????;
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
        //info.OnPreUpdate_OverrideFunc = ????;
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
        LNShapeObject_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNShapeObject_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNShapeObject_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnPreUpdate_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNShapeObject_RegisterSubclassTypeInfo(&info);
    }
    {
        LNPlaneMesh_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNPlaneMesh_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNPlaneMesh_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnPreUpdate_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNPlaneMesh_RegisterSubclassTypeInfo(&info);
    }
    {
        LNBoxMesh_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNBoxMesh_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNBoxMesh_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnPreUpdate_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNBoxMesh_RegisterSubclassTypeInfo(&info);
    }
    {
        LNMeshComponent_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNMeshComponent_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNMeshComponent_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNMeshComponent_RegisterSubclassTypeInfo(&info);
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
        LNParticleEmitter_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNParticleEmitter_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNParticleEmitter_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        //info.OnPreUpdate_OverrideFunc = ????;
        //info.OnUpdate_OverrideFunc = ????;
        LNParticleEmitter_RegisterSubclassTypeInfo(&info);
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
        LNUIText_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIText_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIText_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIText_RegisterSubclassTypeInfo(&info);
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
        LNUIMessageTextArea_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIMessageTextArea_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIMessageTextArea_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIMessageTextArea_RegisterSubclassTypeInfo(&info);
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
        LNUIStackLayout_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIStackLayout_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIStackLayout_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNUIStackLayout_RegisterSubclassTypeInfo(&info);
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
        LNInputGesture_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNInputGesture_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNInputGesture_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNInputGesture_RegisterSubclassTypeInfo(&info);
    }
    {
        LNKeyGesture_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNKeyGesture_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNKeyGesture_SubinstanceFree;
        //info.OnSerialize_OverrideFunc = ????;
        LNKeyGesture_RegisterSubclassTypeInfo(&info);
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
        LNVariantSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNVariantSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNVariantSerializeHandler_SubinstanceFree;
        LNVariantSerializeHandler_RegisterSubclassTypeInfo(&info);
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
        LNAssetObjectSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAssetObjectSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAssetObjectSerializeHandler_SubinstanceFree;
        LNAssetObjectSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNAssetImportSettingsSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAssetImportSettingsSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAssetImportSettingsSerializeHandler_SubinstanceFree;
        LNAssetImportSettingsSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNAssetModelSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAssetModelSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAssetModelSerializeHandler_SubinstanceFree;
        LNAssetModelSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNSoundSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSoundSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSoundSerializeHandler_SubinstanceFree;
        LNSoundSerializeHandler_RegisterSubclassTypeInfo(&info);
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
        LNShaderSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNShaderSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNShaderSerializeHandler_SubinstanceFree;
        LNShaderSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNRenderViewSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNRenderViewSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNRenderViewSerializeHandler_SubinstanceFree;
        LNRenderViewSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNMaterialSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNMaterialSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNMaterialSerializeHandler_SubinstanceFree;
        LNMaterialSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNMeshNodeSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNMeshNodeSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNMeshNodeSerializeHandler_SubinstanceFree;
        LNMeshNodeSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNAnimationControllerSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAnimationControllerSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAnimationControllerSerializeHandler_SubinstanceFree;
        LNAnimationControllerSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNMeshModelSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNMeshModelSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNMeshModelSerializeHandler_SubinstanceFree;
        LNMeshModelSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNMeshImportSettingsSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNMeshImportSettingsSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNMeshImportSettingsSerializeHandler_SubinstanceFree;
        LNMeshImportSettingsSerializeHandler_RegisterSubclassTypeInfo(&info);
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
        LNAnimationCurveSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAnimationCurveSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAnimationCurveSerializeHandler_SubinstanceFree;
        LNAnimationCurveSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNKeyFrameAnimationCurveSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNKeyFrameAnimationCurveSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNKeyFrameAnimationCurveSerializeHandler_SubinstanceFree;
        LNKeyFrameAnimationCurveSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNAnimationClipSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAnimationClipSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAnimationClipSerializeHandler_SubinstanceFree;
        LNAnimationClipSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNAnimationStateSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNAnimationStateSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNAnimationStateSerializeHandler_SubinstanceFree;
        LNAnimationStateSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNEffectResourceSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNEffectResourceSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNEffectResourceSerializeHandler_SubinstanceFree;
        LNEffectResourceSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNParticleEmitterModelSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNParticleEmitterModelSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNParticleEmitterModelSerializeHandler_SubinstanceFree;
        LNParticleEmitterModelSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNParticleModelSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNParticleModelSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNParticleModelSerializeHandler_SubinstanceFree;
        LNParticleModelSerializeHandler_RegisterSubclassTypeInfo(&info);
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
        LNWorldObjectPreUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNWorldObjectPreUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNWorldObjectPreUpdateHandler_SubinstanceFree;
        LNWorldObjectPreUpdateHandler_RegisterSubclassTypeInfo(&info);
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
        LNVisualObjectPreUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNVisualObjectPreUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNVisualObjectPreUpdateHandler_SubinstanceFree;
        LNVisualObjectPreUpdateHandler_RegisterSubclassTypeInfo(&info);
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
        LNCameraPreUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNCameraPreUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNCameraPreUpdateHandler_SubinstanceFree;
        LNCameraPreUpdateHandler_RegisterSubclassTypeInfo(&info);
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
        LNEnvironmentLightPreUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNEnvironmentLightPreUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNEnvironmentLightPreUpdateHandler_SubinstanceFree;
        LNEnvironmentLightPreUpdateHandler_RegisterSubclassTypeInfo(&info);
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
        LNDirectionalLightPreUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNDirectionalLightPreUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNDirectionalLightPreUpdateHandler_SubinstanceFree;
        LNDirectionalLightPreUpdateHandler_RegisterSubclassTypeInfo(&info);
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
        LNPointLightPreUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNPointLightPreUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNPointLightPreUpdateHandler_SubinstanceFree;
        LNPointLightPreUpdateHandler_RegisterSubclassTypeInfo(&info);
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
        LNSpotLightPreUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSpotLightPreUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSpotLightPreUpdateHandler_SubinstanceFree;
        LNSpotLightPreUpdateHandler_RegisterSubclassTypeInfo(&info);
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
        LNSpritePreUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNSpritePreUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNSpritePreUpdateHandler_SubinstanceFree;
        LNSpritePreUpdateHandler_RegisterSubclassTypeInfo(&info);
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
        LNShapeObjectSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNShapeObjectSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNShapeObjectSerializeHandler_SubinstanceFree;
        LNShapeObjectSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNShapeObjectPreUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNShapeObjectPreUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNShapeObjectPreUpdateHandler_SubinstanceFree;
        LNShapeObjectPreUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNShapeObjectUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNShapeObjectUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNShapeObjectUpdateHandler_SubinstanceFree;
        LNShapeObjectUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNPlaneMeshSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNPlaneMeshSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNPlaneMeshSerializeHandler_SubinstanceFree;
        LNPlaneMeshSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNPlaneMeshPreUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNPlaneMeshPreUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNPlaneMeshPreUpdateHandler_SubinstanceFree;
        LNPlaneMeshPreUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNPlaneMeshUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNPlaneMeshUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNPlaneMeshUpdateHandler_SubinstanceFree;
        LNPlaneMeshUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNBoxMeshSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNBoxMeshSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNBoxMeshSerializeHandler_SubinstanceFree;
        LNBoxMeshSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNBoxMeshPreUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNBoxMeshPreUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNBoxMeshPreUpdateHandler_SubinstanceFree;
        LNBoxMeshPreUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNBoxMeshUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNBoxMeshUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNBoxMeshUpdateHandler_SubinstanceFree;
        LNBoxMeshUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNMeshComponentSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNMeshComponentSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNMeshComponentSerializeHandler_SubinstanceFree;
        LNMeshComponentSerializeHandler_RegisterSubclassTypeInfo(&info);
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
        LNParticleEmitterSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNParticleEmitterSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNParticleEmitterSerializeHandler_SubinstanceFree;
        LNParticleEmitterSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNParticleEmitterPreUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNParticleEmitterPreUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNParticleEmitterPreUpdateHandler_SubinstanceFree;
        LNParticleEmitterPreUpdateHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNParticleEmitterUpdateHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNParticleEmitterUpdateHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNParticleEmitterUpdateHandler_SubinstanceFree;
        LNParticleEmitterUpdateHandler_RegisterSubclassTypeInfo(&info);
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
        LNUITextSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUITextSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUITextSerializeHandler_SubinstanceFree;
        LNUITextSerializeHandler_RegisterSubclassTypeInfo(&info);
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
        LNUIMessageTextAreaSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIMessageTextAreaSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIMessageTextAreaSerializeHandler_SubinstanceFree;
        LNUIMessageTextAreaSerializeHandler_RegisterSubclassTypeInfo(&info);
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
        LNUIStackLayoutSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNUIStackLayoutSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNUIStackLayoutSerializeHandler_SubinstanceFree;
        LNUIStackLayoutSerializeHandler_RegisterSubclassTypeInfo(&info);
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
        LNInputGestureSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNInputGestureSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNInputGestureSerializeHandler_SubinstanceFree;
        LNInputGestureSerializeHandler_RegisterSubclassTypeInfo(&info);
    }
    {
        LNKeyGestureSerializeHandler_SubclassRegistrationInfo info = {};
        info.subinstanceAllocFunc = HSPSubclass_LNKeyGestureSerializeHandler_SubinstanceAlloc;
        info.subinstanceFreeFunc = HSPSubclass_LNKeyGestureSerializeHandler_SubinstanceFree;
        LNKeyGestureSerializeHandler_RegisterSubclassTypeInfo(&info);
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

