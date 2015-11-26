
#include <windows.h>
#include "../hsp3plugin/hsp3plugin.h"
#include "../../../src/C_API/LuminoC.h"
#include "LuminoHSP.h"

//=============================================================================
// Common
//=============================================================================
#define sbAlloc hspmalloc
#define sbFree hspfree

static void* hspCommon_GetBlockSize(PVal *pval, PDAT *pdat, int *size)
{
	*size = pval->size - (((char *)pdat) - pval->pt);
	return (pdat);
}

static void hspCommon_AllocBlock(PVal *pval, PDAT *pdat, int size)
{
}

//=============================================================================
// Structs
//=============================================================================
//-----------------------------------------------------------------------------
// LNSize
//-----------------------------------------------------------------------------
#define GetPtr_LNSize(pval) ((LNSize*)pval)

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
	*GetPtr_LNSize(pdat) = *((LNSize*)(in));
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

//-----------------------------------------------------------------------------
// LNVector2
//-----------------------------------------------------------------------------
#define GetPtr_LNVector2(pval) ((LNVector2*)pval)

static int g_LNVector2_typeid = 0;

int hspLNVector2_typeid()
{
	return g_LNVector2_typeid;
}

static int hspLNVector2_GetVarSize(PVal *pval)
{
	int size;
	size = pval->len[1];
	if (pval->len[2]) size *= pval->len[2];
	if (pval->len[3]) size *= pval->len[3];
	if (pval->len[4]) size *= pval->len[4];
	size *= sizeof(LNVector2);
	return size;
}

static void hspLNVector2_Alloc(PVal *pval, const PVal *pval2)
{
	if (pval->len[1] < 1) pval->len[1] = 1;
	int size = hspLNVector2_GetVarSize(pval);
	pval->mode = HSPVAR_MODE_MALLOC;
	char* pt = sbAlloc(size);
	LNVector2* fv = (LNVector2 *)pt;
	memset(fv, 0, size);
	if (pval2 != NULL) {
		memcpy(pt, pval->pt, pval->size);
		sbFree(pval->pt);
	}
	pval->pt = pt;
	pval->size = size;
}

static void hspLNVector2_Free(PVal* pval)
{
	if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
	pval->pt = NULL;
	pval->mode = HSPVAR_MODE_NONE;
}

static PDAT* hspLNVector2_GetPtr(PVal* pval)
{
	return (PDAT*)(((LNVector2*)(pval->pt)) + pval->offset);
}

static int hspLNVector2_GetSize(const PDAT *pdatl)
{
	return sizeof(LNVector2);
}

static void hspLNVector2_Set(PVal* pval, PDAT* pdat, const void* in)
{
	*GetPtr_LNVector2(pdat) = *((LNVector2*)(in));
}

static void hspLNVector2_Init(HspVarProc* p)
{
	p->Alloc = hspLNVector2_Alloc;
	p->Free = hspLNVector2_Free;
	
	p->GetPtr = hspLNVector2_GetPtr;
	p->GetSize = hspLNVector2_GetSize;
	p->GetBlockSize = hspCommon_GetBlockSize;
	p->AllocBlock = hspCommon_AllocBlock;

	p->Set = hspLNVector2_Set;

	p->vartype_name = "LNVector2";
	p->version = 0x001;
	p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
	p->basesize = sizeof(LNVector2);
	g_LNVector2_typeid = p->flag;
}

//-----------------------------------------------------------------------------
// LNVector3
//-----------------------------------------------------------------------------
#define GetPtr_LNVector3(pval) ((LNVector3*)pval)

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
	*GetPtr_LNVector3(pdat) = *((LNVector3*)(in));
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

//-----------------------------------------------------------------------------
// LNVector4
//-----------------------------------------------------------------------------
#define GetPtr_LNVector4(pval) ((LNVector4*)pval)

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
	*GetPtr_LNVector4(pdat) = *((LNVector4*)(in));
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

//-----------------------------------------------------------------------------
// LNMatrix
//-----------------------------------------------------------------------------
#define GetPtr_LNMatrix(pval) ((LNMatrix*)pval)

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
	*GetPtr_LNMatrix(pdat) = *((LNMatrix*)(in));
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

//-----------------------------------------------------------------------------
// LNQuaternion
//-----------------------------------------------------------------------------
#define GetPtr_LNQuaternion(pval) ((LNQuaternion*)pval)

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
	*GetPtr_LNQuaternion(pdat) = *((LNQuaternion*)(in));
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


//=============================================================================
// reffunc
//=============================================================================
bool g_leadSupport = false;

bool CheclDefault()
{
	int r = code_getprm();
	g_leadSupport = true;
	return (r == PARAM_ENDSPLIT);	// ')'
}

double GetParamDouble()
{
	if (g_leadSupport) {
		g_leadSupport = false;
		return *((double*)mpval->pt);
	}
	else {
		return code_getd();
	}
}

bool Structs_reffunc(int cmd, int* typeRes, void** retValPtr)
{
	g_leadSupport = false;
	
	switch (cmd)
	{
    case 0x000A:
    {
        static LNSize returnValue;
        if (CheclDefault()) {
            memset(&returnValue, 0, sizeof(returnValue));
        }
        else {
            returnValue.Width = GetParamDouble();
            returnValue.Height = GetParamDouble();
        }
        *retValPtr = &returnValue;
        *typeRes = hspLNSize_typeid();
        return true;
    }
    case 0x000B:
    {
        static LNVector2 returnValue;
        if (CheclDefault()) {
            memset(&returnValue, 0, sizeof(returnValue));
        }
        else {
            returnValue.X = GetParamDouble();
            returnValue.Y = GetParamDouble();
        }
        *retValPtr = &returnValue;
        *typeRes = hspLNVector2_typeid();
        return true;
    }
    case 0x000C:
    {
        static LNVector3 returnValue;
        if (CheclDefault()) {
            memset(&returnValue, 0, sizeof(returnValue));
        }
        else {
            returnValue.X = GetParamDouble();
            returnValue.Y = GetParamDouble();
            returnValue.Z = GetParamDouble();
        }
        *retValPtr = &returnValue;
        *typeRes = hspLNVector3_typeid();
        return true;
    }
    case 0x000D:
    {
        static LNVector4 returnValue;
        if (CheclDefault()) {
            memset(&returnValue, 0, sizeof(returnValue));
        }
        else {
            returnValue.X = GetParamDouble();
            returnValue.Y = GetParamDouble();
            returnValue.Z = GetParamDouble();
            returnValue.W = GetParamDouble();
        }
        *retValPtr = &returnValue;
        *typeRes = hspLNVector4_typeid();
        return true;
    }
    case 0x000E:
    {
        static LNMatrix returnValue;
        if (CheclDefault()) {
            LNMatrix_Identity(&returnValue);
        }
        else {
            returnValue.M11 = GetParamDouble();
            returnValue.M12 = GetParamDouble();
            returnValue.M13 = GetParamDouble();
            returnValue.M14 = GetParamDouble();
            returnValue.M21 = GetParamDouble();
            returnValue.M22 = GetParamDouble();
            returnValue.M23 = GetParamDouble();
            returnValue.M24 = GetParamDouble();
            returnValue.M31 = GetParamDouble();
            returnValue.M32 = GetParamDouble();
            returnValue.M33 = GetParamDouble();
            returnValue.M34 = GetParamDouble();
            returnValue.M41 = GetParamDouble();
            returnValue.M42 = GetParamDouble();
            returnValue.M43 = GetParamDouble();
            returnValue.M44 = GetParamDouble();
        }
        *retValPtr = &returnValue;
        *typeRes = hspLNMatrix_typeid();
        return true;
    }
    case 0x000F:
    {
        static LNQuaternion returnValue;
        if (CheclDefault()) {
            memset(&returnValue, 0, sizeof(returnValue));
        }
        else {
            returnValue.X = GetParamDouble();
            returnValue.Y = GetParamDouble();
            returnValue.Z = GetParamDouble();
            returnValue.W = GetParamDouble();
        }
        *retValPtr = &returnValue;
        *typeRes = hspLNQuaternion_typeid();
        return true;
    }

	}
	return false;
}

//=============================================================================
// Register
//=============================================================================
void RegisterStructTypes(HSP3TYPEINFO *info)
{
    registvar(-1, hspLNSize_Init);
    registvar(-1, hspLNVector2_Init);
    registvar(-1, hspLNVector3_Init);
    registvar(-1, hspLNVector4_Init);
    registvar(-1, hspLNMatrix_Init);
    registvar(-1, hspLNQuaternion_Init);

}

