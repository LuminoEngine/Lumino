
#include <windows.h>
#include <string>
#include "../hsp3plugin/hsp3plugin.h"
#include "../../../C_API/include/LuminoC.h"
#include "LuminoHSP.h"

extern int hspLNSize_typeid();
extern int hspLNVector2_typeid();
extern int hspLNVector3_typeid();
extern int hspLNVector4_typeid();
extern int hspLNMatrix_typeid();
extern int hspLNQuaternion_typeid();


//=============================================================================
// Common
//=============================================================================
int CodeGetI()
{
	return code_geti();
}
int CodeGetI(int defaultValue)
{
	return code_getdi(defaultValue);
}
double CodeGetD()
{
	return code_getd();
}
double CodeGetD(double defaultValue)
{
	return code_getdd(defaultValue);
}
const char* CodeGetS()
{
	return code_gets();
}
const char* CodeGetS(const char* defaultValue)
{
	return code_getds(defaultValue);
}

#define CodeGetVA_TypeChecked(ppval, type) \
	code_getva(ppval); \
	if ((*ppval)->flag != hsp##type##_typeid()) { throw HSPVAR_ERROR_TYPEMISS; }

std::string str_p0;
std::string str_p1;
std::string str_p2;
std::string str_p3;
std::string str_p4;
std::string str_p5;
std::string str_p6;
std::string str_p7;
std::string str_p8;

//=============================================================================
// cmdfunc
//=============================================================================

bool Commands_cmdfunc(int cmd, int* retVal)
{
	*retVal = RUNMODE_RUN;
	switch (cmd)
	{
    case 0x0064:
    {
        LNBool p0 = (LNBool)CodeGetI();
        LNConfig_SetApplicationLogEnabled(p0);
    
        return true;
    }
    case 0x0065:
    {
        LNBool p0 = (LNBool)CodeGetI();
        LNConfig_SetConsoleEnabled(p0);
    
        return true;
    }
    case 0x0066:
    {
        str_p0 = CodeGetS();
        str_p1 = CodeGetS();
        LNConfig_RegisterArchive(str_p0.c_str(), str_p1.c_str());
    
        return true;
    }
    case 0x0067:
    {
        int p0 = CodeGetI();
        LNConfig_SetFileAccessPriority((LNFileAccessPriority)p0);
    
        return true;
    }
    case 0x0068:
    {
        intptr_t p0 = CodeGetI();
        LNConfig_SetUserWindowHandle(p0);
    
        return true;
    }
    case 0x0069:
    {
        int p0 = CodeGetI();
        int p1 = CodeGetI();
        LNConfig_SetSoundCacheSize(p0, p1);
    
        return true;
    }
    case 0x006A:
    {
        int p0 = CodeGetI();
        LNConfig_SetDirectMusicMode((LNDirectMusicMode)p0);
    
        return true;
    }
    case 0x006B:
    {
        float p0 = CodeGetD();
        LNConfig_SetDirectMusicReverbLevel(p0);
    
        return true;
    }
    case 0x006C:
    {
    
        stat = LNApplication_Initialize();
    
        return true;
    }
    case 0x006D:
    {
    
        stat = LNApplication_InitializeAudio();
    
        return true;
    }
    case 0x006E:
    {
    
        stat = LNApplication_UpdateFrame();
    
        return true;
    }
    case 0x006F:
    {
        PVal* pval_p0;
        APTR aptr_p0 = code_getva(&pval_p0);
        LNBool p0;
        stat = LNApplication_IsEndRequested(&p0);
        int rp0 = p0; code_setva(pval_p0, aptr_p0, HSPVAR_FLAG_INT, &rp0);
        return true;
    }
    case 0x0070:
    {
    
        LNApplication_Terminate();
    
        return true;
    }
    case 0x0071:
    {
        PVal* pval_p0;
        APTR aptr_p0 = code_getva(&pval_p0);
        int p0;
        LNVersion_GetMajor(&p0);
        int rp0 = p0; code_setva(pval_p0, aptr_p0, HSPVAR_FLAG_INT, &rp0);
        return true;
    }
    case 0x0072:
    {
        PVal* pval_p0;
        APTR aptr_p0 = code_getva(&pval_p0);
        int p0;
        LNVersion_GetMinor(&p0);
        int rp0 = p0; code_setva(pval_p0, aptr_p0, HSPVAR_FLAG_INT, &rp0);
        return true;
    }
    case 0x0073:
    {
        PVal* pval_p0;
        APTR aptr_p0 = code_getva(&pval_p0);
        int p0;
        LNVersion_GetRevision(&p0);
        int rp0 = p0; code_setva(pval_p0, aptr_p0, HSPVAR_FLAG_INT, &rp0);
        return true;
    }
    case 0x0074:
    {
        PVal* pval_p0;
        APTR aptr_p0 = code_getva(&pval_p0);
        int p0;
        LNVersion_GetBuild(&p0);
        int rp0 = p0; code_setva(pval_p0, aptr_p0, HSPVAR_FLAG_INT, &rp0);
        return true;
    }
    case 0x0075:
    {
        PVal* pval_p0;
        APTR aptr_p0 = code_getva(&pval_p0);
        const LNChar* p0;
        LNVersion_GetString(&p0);
        code_setva(pval_p0, aptr_p0, HSPVAR_FLAG_STR, p0);
        return true;
    }
    case 0x0076:
    {
        int p0 = CodeGetI();
        int p1 = CodeGetI();
        int p2 = CodeGetI();
        PVal* pval_p3;
        APTR aptr_p3 = code_getva(&pval_p3);
        LNBool p3;
        LNVersion_IsAtLeast(p0, p1, p2, &p3);
        int rp3 = p3; code_setva(pval_p3, aptr_p3, HSPVAR_FLAG_INT, &rp3);
        return true;
    }
    case 0x0077:
    {
    
        stat = LNError_GetLastErrorCode();
    
        return true;
    }
    case 0x0078:
    {
        PVal* pval_p0;
        APTR aptr_p0 = code_getva(&pval_p0);
        const LNChar* p0;
        LNError_GetLastErrorMessage(&p0);
        code_setva(pval_p0, aptr_p0, HSPVAR_FLAG_STR, p0);
        return true;
    }
    case 0x0079:
    {
        intptr_t p0 = CodeGetI();
        stat = LNObject_Release(p0);
    
        return true;
    }
    case 0x007A:
    {
        intptr_t p0 = CodeGetI();
        stat = LNObject_AddRef(p0);
    
        return true;
    }
    case 0x007B:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        int p1;
        stat = LNObject_GetRefCount(p0, &p1);
        int rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_INT, &rp1);
        return true;
    }
    case 0x007C:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector2);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        float p1;
        stat = LNVector2_GetLength((LNVector2*)pval_p0->pt, &p1);
        double rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_DOUBLE, &rp1);
        return true;
    }
    case 0x007D:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector2);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        float p1;
        stat = LNVector2_GetSquareLength((LNVector2*)pval_p0->pt, &p1);
        double rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_DOUBLE, &rp1);
        return true;
    }
    case 0x007E:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector2);
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        stat = LNVector2_Set((LNVector2*)pval_p0->pt, p1, p2);
    
        return true;
    }
    case 0x007F:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector2);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNVector2 p1;
        stat = LNVector2_Normalize((LNVector2*)pval_p0->pt, &p1);
        code_setva(pval_p1, aptr_p1, hspLNVector2_typeid(), &p1);
        return true;
    }
    case 0x0080:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector2);
        stat = LNVector2_NormalizeV((LNVector2*)pval_p0->pt);
    
        return true;
    }
    case 0x0081:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        float p1;
        stat = LNVector3_GetLength((LNVector3*)pval_p0->pt, &p1);
        double rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_DOUBLE, &rp1);
        return true;
    }
    case 0x0082:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        float p1;
        stat = LNVector3_GetSquareLength((LNVector3*)pval_p0->pt, &p1);
        double rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_DOUBLE, &rp1);
        return true;
    }
    case 0x0083:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        stat = LNVector3_Set((LNVector3*)pval_p0->pt, p1, p2, p3);
    
        return true;
    }
    case 0x0084:
    {
        PVal* pval_p0;
        APTR aptr_p0 = code_getva(&pval_p0);
        LNVector3 p0;
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector2);
        float p2 = CodeGetD();
        stat = LNVector3_SetVZ(&p0, (LNVector2*)pval_p1->pt, p2);
        code_setva(pval_p0, aptr_p0, hspLNVector3_typeid(), &p0);
        return true;
    }
    case 0x0085:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNVector3 p1;
        stat = LNVector3_Normalize((LNVector3*)pval_p0->pt, &p1);
        code_setva(pval_p1, aptr_p1, hspLNVector3_typeid(), &p1);
        return true;
    }
    case 0x0086:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        stat = LNVector3_NormalizeV((LNVector3*)pval_p0->pt);
    
        return true;
    }
    case 0x0087:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        PVal* pval_p2;
        APTR aptr_p2 = code_getva(&pval_p2);
        float p2;
        stat = LNVector3_Dot((LNVector3*)pval_p0->pt, (LNVector3*)pval_p1->pt, &p2);
        double rp2 = p2; code_setva(pval_p2, aptr_p2, HSPVAR_FLAG_DOUBLE, &rp2);
        return true;
    }
    case 0x0088:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        PVal* pval_p2;
        APTR aptr_p2 = code_getva(&pval_p2);
        LNVector3 p2;
        stat = LNVector3_Cross((LNVector3*)pval_p0->pt, (LNVector3*)pval_p1->pt, &p2);
        code_setva(pval_p2, aptr_p2, hspLNVector3_typeid(), &p2);
        return true;
    }
    case 0x0089:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        PVal* pval_p2;
        APTR aptr_p2 = code_getva(&pval_p2);
        LNVector3 p2;
        stat = LNVector3_Reflect((LNVector3*)pval_p0->pt, (LNVector3*)pval_p1->pt, &p2);
        code_setva(pval_p2, aptr_p2, hspLNVector3_typeid(), &p2);
        return true;
    }
    case 0x008A:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        PVal* pval_p2;
        APTR aptr_p2 = code_getva(&pval_p2);
        LNVector3 p2;
        stat = LNVector3_Slide((LNVector3*)pval_p0->pt, (LNVector3*)pval_p1->pt, &p2);
        code_setva(pval_p2, aptr_p2, hspLNVector3_typeid(), &p2);
        return true;
    }
    case 0x008B:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        float p2 = CodeGetD();
        PVal* pval_p3;
        APTR aptr_p3 = code_getva(&pval_p3);
        LNVector3 p3;
        stat = LNVector3_Lerp((LNVector3*)pval_p0->pt, (LNVector3*)pval_p1->pt, p2, &p3);
        code_setva(pval_p3, aptr_p3, hspLNVector3_typeid(), &p3);
        return true;
    }
    case 0x008C:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        PVal* pval_p2; CodeGetVA_TypeChecked(&pval_p2, LNVector3);
        PVal* pval_p3; CodeGetVA_TypeChecked(&pval_p3, LNVector3);
        float p4 = CodeGetD();
        PVal* pval_p5;
        APTR aptr_p5 = code_getva(&pval_p5);
        LNVector3 p5;
        stat = LNVector3_CatmullRom((LNVector3*)pval_p0->pt, (LNVector3*)pval_p1->pt, (LNVector3*)pval_p2->pt, (LNVector3*)pval_p3->pt, p4, &p5);
        code_setva(pval_p5, aptr_p5, hspLNVector3_typeid(), &p5);
        return true;
    }
    case 0x008D:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNMatrix);
        PVal* pval_p2;
        APTR aptr_p2 = code_getva(&pval_p2);
        LNVector4 p2;
        stat = LNVector3_Transform((LNVector3*)pval_p0->pt, (LNMatrix*)pval_p1->pt, &p2);
        code_setva(pval_p2, aptr_p2, hspLNVector4_typeid(), &p2);
        return true;
    }
    case 0x008E:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNMatrix);
        PVal* pval_p2;
        APTR aptr_p2 = code_getva(&pval_p2);
        LNVector3 p2;
        stat = LNVector3_TransformCoord((LNVector3*)pval_p0->pt, (LNMatrix*)pval_p1->pt, &p2);
        code_setva(pval_p2, aptr_p2, hspLNVector3_typeid(), &p2);
        return true;
    }
    case 0x008F:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector4);
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        float p4 = CodeGetD();
        stat = LNVector4_Set((LNVector4*)pval_p0->pt, p1, p2, p3, p4);
    
        return true;
    }
    case 0x0090:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNVector3 p1;
        stat = LNMatrix_GetRight((LNMatrix*)pval_p0->pt, &p1);
        code_setva(pval_p1, aptr_p1, hspLNVector3_typeid(), &p1);
        return true;
    }
    case 0x0091:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNVector3 p1;
        stat = LNMatrix_GetUp((LNMatrix*)pval_p0->pt, &p1);
        code_setva(pval_p1, aptr_p1, hspLNVector3_typeid(), &p1);
        return true;
    }
    case 0x0092:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNVector3 p1;
        stat = LNMatrix_GetFront((LNMatrix*)pval_p0->pt, &p1);
        code_setva(pval_p1, aptr_p1, hspLNVector3_typeid(), &p1);
        return true;
    }
    case 0x0093:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNVector3 p1;
        stat = LNMatrix_GetPosition((LNMatrix*)pval_p0->pt, &p1);
        code_setva(pval_p1, aptr_p1, hspLNVector3_typeid(), &p1);
        return true;
    }
    case 0x0094:
    {
        PVal* pval_p0;
        APTR aptr_p0 = code_getva(&pval_p0);
        LNMatrix p0;
        stat = LNMatrix_Identity(&p0);
        code_setva(pval_p0, aptr_p0, hspLNMatrix_typeid(), &p0);
        return true;
    }
    case 0x0095:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        stat = LNMatrix_Translate((LNMatrix*)pval_p0->pt, p1, p2, p3);
    
        return true;
    }
    case 0x0096:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        stat = LNMatrix_TranslateVec3((LNMatrix*)pval_p0->pt, (LNVector3*)pval_p1->pt);
    
        return true;
    }
    case 0x0097:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        float p1 = CodeGetD();
        stat = LNMatrix_RotateX((LNMatrix*)pval_p0->pt, p1);
    
        return true;
    }
    case 0x0098:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        float p1 = CodeGetD();
        stat = LNMatrix_RotateY((LNMatrix*)pval_p0->pt, p1);
    
        return true;
    }
    case 0x0099:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        float p1 = CodeGetD();
        stat = LNMatrix_RotateZ((LNMatrix*)pval_p0->pt, p1);
    
        return true;
    }
    case 0x009A:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        int p4 = CodeGetI(LN_ROTATIONORDER_XYZ);
        stat = LNMatrix_Rotate((LNMatrix*)pval_p0->pt, p1, p2, p3, (LNRotationOrder)p4);
    
        return true;
    }
    case 0x009B:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        int p2 = CodeGetI(LN_ROTATIONORDER_XYZ);
        stat = LNMatrix_RotateVec3((LNMatrix*)pval_p0->pt, (LNVector3*)pval_p1->pt, (LNRotationOrder)p2);
    
        return true;
    }
    case 0x009C:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        float p2 = CodeGetD();
        stat = LNMatrix_RotateAxis((LNMatrix*)pval_p0->pt, (LNVector3*)pval_p1->pt, p2);
    
        return true;
    }
    case 0x009D:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNQuaternion);
        stat = LNMatrix_RotateQuaternion((LNMatrix*)pval_p0->pt, (LNQuaternion*)pval_p1->pt);
    
        return true;
    }
    case 0x009E:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        float p1 = CodeGetD();
        stat = LNMatrix_Scale((LNMatrix*)pval_p0->pt, p1);
    
        return true;
    }
    case 0x009F:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        stat = LNMatrix_ScaleXYZ((LNMatrix*)pval_p0->pt, p1, p2, p3);
    
        return true;
    }
    case 0x00A0:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        stat = LNMatrix_ScaleVec3((LNMatrix*)pval_p0->pt, (LNVector3*)pval_p1->pt);
    
        return true;
    }
    case 0x00A1:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNMatrix);
        PVal* pval_p2;
        APTR aptr_p2 = code_getva(&pval_p2);
        LNMatrix p2;
        stat = LNMatrix_Multiply((LNMatrix*)pval_p0->pt, (LNMatrix*)pval_p1->pt, &p2);
        code_setva(pval_p2, aptr_p2, hspLNMatrix_typeid(), &p2);
        return true;
    }
    case 0x00A2:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNMatrix p1;
        stat = LNMatrix_Inverse((LNMatrix*)pval_p0->pt, &p1);
        code_setva(pval_p1, aptr_p1, hspLNMatrix_typeid(), &p1);
        return true;
    }
    case 0x00A3:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNMatrix p1;
        stat = LNMatrix_Transpose((LNMatrix*)pval_p0->pt, &p1);
        code_setva(pval_p1, aptr_p1, hspLNMatrix_typeid(), &p1);
        return true;
    }
    case 0x00A4:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        PVal* pval_p2; CodeGetVA_TypeChecked(&pval_p2, LNVector3);
        PVal* pval_p3;
        APTR aptr_p3 = code_getva(&pval_p3);
        LNMatrix p3;
        stat = LNMatrix_ViewTransformLH((LNVector3*)pval_p0->pt, (LNVector3*)pval_p1->pt, (LNVector3*)pval_p2->pt, &p3);
        code_setva(pval_p3, aptr_p3, hspLNMatrix_typeid(), &p3);
        return true;
    }
    case 0x00A5:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        PVal* pval_p2; CodeGetVA_TypeChecked(&pval_p2, LNVector3);
        PVal* pval_p3;
        APTR aptr_p3 = code_getva(&pval_p3);
        LNMatrix p3;
        stat = LNMatrix_ViewTransformRH((LNVector3*)pval_p0->pt, (LNVector3*)pval_p1->pt, (LNVector3*)pval_p2->pt, &p3);
        code_setva(pval_p3, aptr_p3, hspLNMatrix_typeid(), &p3);
        return true;
    }
    case 0x00A6:
    {
        float p0 = CodeGetD();
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        PVal* pval_p4;
        APTR aptr_p4 = code_getva(&pval_p4);
        LNMatrix p4;
        stat = LNMatrix_PerspectiveFovLH(p0, p1, p2, p3, &p4);
        code_setva(pval_p4, aptr_p4, hspLNMatrix_typeid(), &p4);
        return true;
    }
    case 0x00A7:
    {
        float p0 = CodeGetD();
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        PVal* pval_p4;
        APTR aptr_p4 = code_getva(&pval_p4);
        LNMatrix p4;
        stat = LNMatrix_PerspectiveFovRH(p0, p1, p2, p3, &p4);
        code_setva(pval_p4, aptr_p4, hspLNMatrix_typeid(), &p4);
        return true;
    }
    case 0x00A8:
    {
        float p0 = CodeGetD();
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        PVal* pval_p4;
        APTR aptr_p4 = code_getva(&pval_p4);
        LNMatrix p4;
        stat = LNMatrix_OrthoLH(p0, p1, p2, p3, &p4);
        code_setva(pval_p4, aptr_p4, hspLNMatrix_typeid(), &p4);
        return true;
    }
    case 0x00A9:
    {
        float p0 = CodeGetD();
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        PVal* pval_p4;
        APTR aptr_p4 = code_getva(&pval_p4);
        LNMatrix p4;
        stat = LNMatrix_OrthoRH(p0, p1, p2, p3, &p4);
        code_setva(pval_p4, aptr_p4, hspLNMatrix_typeid(), &p4);
        return true;
    }
    case 0x00AA:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNVector3 p1;
        stat = LNMatrix_GetEulerAngles((LNMatrix*)pval_p0->pt, &p1);
        code_setva(pval_p1, aptr_p1, hspLNVector3_typeid(), &p1);
        return true;
    }
    case 0x00AB:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNVector3 p1;
        PVal* pval_p2;
        APTR aptr_p2 = code_getva(&pval_p2);
        LNQuaternion p2;
        PVal* pval_p3;
        APTR aptr_p3 = code_getva(&pval_p3);
        LNVector3 p3;
        stat = LNMatrix_Decompose((LNMatrix*)pval_p0->pt, &p1, &p2, &p3);
        code_setva(pval_p1, aptr_p1, hspLNVector3_typeid(), &p1);
        code_setva(pval_p2, aptr_p2, hspLNQuaternion_typeid(), &p2);
        code_setva(pval_p3, aptr_p3, hspLNVector3_typeid(), &p3);
        return true;
    }
    case 0x00AC:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNQuaternion);
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        float p4 = CodeGetD();
        stat = LNQuaternion_Set((LNQuaternion*)pval_p0->pt, p1, p2, p3, p4);
    
        return true;
    }
    case 0x00AD:
    {
        PVal* pval_p0;
        APTR aptr_p0 = code_getva(&pval_p0);
        LNQuaternion p0;
        stat = LNQuaternion_Identity(&p0);
        code_setva(pval_p0, aptr_p0, hspLNQuaternion_typeid(), &p0);
        return true;
    }
    case 0x00AE:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        float p1 = CodeGetD();
        PVal* pval_p2;
        APTR aptr_p2 = code_getva(&pval_p2);
        LNQuaternion p2;
        stat = LNQuaternion_RotationAxis((LNVector3*)pval_p0->pt, p1, &p2);
        code_setva(pval_p2, aptr_p2, hspLNQuaternion_typeid(), &p2);
        return true;
    }
    case 0x00AF:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNMatrix);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNQuaternion p1;
        stat = LNQuaternion_RotationMatrix((LNMatrix*)pval_p0->pt, &p1);
        code_setva(pval_p1, aptr_p1, hspLNQuaternion_typeid(), &p1);
        return true;
    }
    case 0x00B0:
    {
        float p0 = CodeGetD();
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        PVal* pval_p3;
        APTR aptr_p3 = code_getva(&pval_p3);
        LNQuaternion p3;
        stat = LNQuaternion_RotationYawPitchRoll(p0, p1, p2, &p3);
        code_setva(pval_p3, aptr_p3, hspLNQuaternion_typeid(), &p3);
        return true;
    }
    case 0x00B1:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNQuaternion);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNQuaternion p1;
        stat = LNQuaternion_Normalize((LNQuaternion*)pval_p0->pt, &p1);
        code_setva(pval_p1, aptr_p1, hspLNQuaternion_typeid(), &p1);
        return true;
    }
    case 0x00B2:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNQuaternion);
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNQuaternion p1;
        stat = LNQuaternion_Conjugate((LNQuaternion*)pval_p0->pt, &p1);
        code_setva(pval_p1, aptr_p1, hspLNQuaternion_typeid(), &p1);
        return true;
    }
    case 0x00B3:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNQuaternion);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNQuaternion);
        PVal* pval_p2;
        APTR aptr_p2 = code_getva(&pval_p2);
        LNQuaternion p2;
        stat = LNQuaternion_Multiply((LNQuaternion*)pval_p0->pt, (LNQuaternion*)pval_p1->pt, &p2);
        code_setva(pval_p2, aptr_p2, hspLNQuaternion_typeid(), &p2);
        return true;
    }
    case 0x00B4:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNQuaternion);
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNQuaternion);
        float p2 = CodeGetD();
        PVal* pval_p3;
        APTR aptr_p3 = code_getva(&pval_p3);
        LNQuaternion p3;
        stat = LNQuaternion_Slerp((LNQuaternion*)pval_p0->pt, (LNQuaternion*)pval_p1->pt, p2, &p3);
        code_setva(pval_p3, aptr_p3, hspLNQuaternion_typeid(), &p3);
        return true;
    }
    case 0x00B5:
    {
        str_p0 = CodeGetS();
        float p1 = CodeGetD(1.0f);
        float p2 = CodeGetD(1.0f);
        double p3 = CodeGetD(0.0);
        stat = LNGameAudio_PlayBGM(str_p0.c_str(), p1, p2, p3);
    
        return true;
    }
    case 0x00B6:
    {
        void* p0 = (void*)CodeGetS();
        int p1 = CodeGetI();
        float p2 = CodeGetD(1.0f);
        float p3 = CodeGetD(1.0f);
        double p4 = CodeGetD(0.0);
        stat = LNGameAudio_PlayBGMMem(p0, p1, p2, p3, p4);
    
        return true;
    }
    case 0x00B7:
    {
        double p0 = CodeGetD(0.0);
        stat = LNGameAudio_StopBGM(p0);
    
        return true;
    }
    case 0x00B8:
    {
        str_p0 = CodeGetS();
        float p1 = CodeGetD(1.0f);
        float p2 = CodeGetD(1.0f);
        double p3 = CodeGetD(0.0);
        stat = LNGameAudio_PlayBGS(str_p0.c_str(), p1, p2, p3);
    
        return true;
    }
    case 0x00B9:
    {
        void* p0 = (void*)CodeGetS();
        int p1 = CodeGetI();
        float p2 = CodeGetD(1.0f);
        float p3 = CodeGetD(1.0f);
        double p4 = CodeGetD(0.0);
        stat = LNGameAudio_PlayBGSMem(p0, p1, p2, p3, p4);
    
        return true;
    }
    case 0x00BA:
    {
        double p0 = CodeGetD(0.0);
        stat = LNGameAudio_StopBGS(p0);
    
        return true;
    }
    case 0x00BB:
    {
        str_p0 = CodeGetS();
        float p1 = CodeGetD(1.0f);
        float p2 = CodeGetD(1.0f);
        stat = LNGameAudio_PlayME(str_p0.c_str(), p1, p2);
    
        return true;
    }
    case 0x00BC:
    {
        void* p0 = (void*)CodeGetS();
        int p1 = CodeGetI();
        float p2 = CodeGetD(1.0f);
        float p3 = CodeGetD(1.0f);
        stat = LNGameAudio_PlayMEMem(p0, p1, p2, p3);
    
        return true;
    }
    case 0x00BD:
    {
    
        stat = LNGameAudio_StopME();
    
        return true;
    }
    case 0x00BE:
    {
        str_p0 = CodeGetS();
        float p1 = CodeGetD(1.0f);
        float p2 = CodeGetD(1.0f);
        stat = LNGameAudio_PlaySE(str_p0.c_str(), p1, p2);
    
        return true;
    }
    case 0x00BF:
    {
        str_p0 = CodeGetS();
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        float p2 = CodeGetD();
        float p3 = CodeGetD(1.0f);
        float p4 = CodeGetD(1.0f);
        stat = LNGameAudio_PlaySE3D(str_p0.c_str(), (LNVector3*)pval_p1->pt, p2, p3, p4);
    
        return true;
    }
    case 0x00C0:
    {
        str_p0 = CodeGetS();
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        float p4 = CodeGetD();
        float p5 = CodeGetD(1.0f);
        float p6 = CodeGetD(1.0f);
        stat = LNGameAudio_PlaySE3DXYZ(str_p0.c_str(), p1, p2, p3, p4, p5, p6);
    
        return true;
    }
    case 0x00C1:
    {
        void* p0 = (void*)CodeGetS();
        int p1 = CodeGetI();
        float p2 = CodeGetD(1.0f);
        float p3 = CodeGetD(1.0f);
        stat = LNGameAudio_PlaySEMem(p0, p1, p2, p3);
    
        return true;
    }
    case 0x00C2:
    {
        void* p0 = (void*)CodeGetS();
        int p1 = CodeGetI();
        PVal* pval_p2; CodeGetVA_TypeChecked(&pval_p2, LNVector3);
        float p3 = CodeGetD();
        float p4 = CodeGetD(1.0f);
        float p5 = CodeGetD(1.0f);
        stat = LNGameAudio_PlaySE3DMem(p0, p1, (LNVector3*)pval_p2->pt, p3, p4, p5);
    
        return true;
    }
    case 0x00C3:
    {
        void* p0 = (void*)CodeGetS();
        int p1 = CodeGetI();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        float p4 = CodeGetD();
        float p5 = CodeGetD();
        float p6 = CodeGetD(1.0f);
        float p7 = CodeGetD(1.0f);
        stat = LNGameAudio_PlaySE3DMemXYZ(p0, p1, p2, p3, p4, p5, p6, p7);
    
        return true;
    }
    case 0x00C4:
    {
    
        stat = LNGameAudio_StopSE();
    
        return true;
    }
    case 0x00C5:
    {
        float p0 = CodeGetD();
        double p1 = CodeGetD(0.0);
        stat = LNGameAudio_SetBGMVolume(p0, p1);
    
        return true;
    }
    case 0x00C6:
    {
        float p0 = CodeGetD();
        double p1 = CodeGetD(0.0);
        stat = LNGameAudio_SetBGSVolume(p0, p1);
    
        return true;
    }
    case 0x00C7:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        stat = LNSoundListener_SetPosition((LNVector3*)pval_p0->pt);
    
        return true;
    }
    case 0x00C8:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        stat = LNSoundListener_SetDirection((LNVector3*)pval_p0->pt);
    
        return true;
    }
    case 0x00C9:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        stat = LNSoundListener_SetUpDirection((LNVector3*)pval_p0->pt);
    
        return true;
    }
    case 0x00CA:
    {
        PVal* pval_p0; CodeGetVA_TypeChecked(&pval_p0, LNVector3);
        stat = LNSoundListener_SetVelocity((LNVector3*)pval_p0->pt);
    
        return true;
    }
    case 0x00CB:
    {
        float p0 = CodeGetD();
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        stat = LNSoundListener_SetPositionXYZ(p0, p1, p2);
    
        return true;
    }
    case 0x00CC:
    {
        float p0 = CodeGetD();
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        stat = LNSoundListener_SetDirectionXYZ(p0, p1, p2);
    
        return true;
    }
    case 0x00CD:
    {
        float p0 = CodeGetD();
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        stat = LNSoundListener_SetUpDirectionXYZ(p0, p1, p2);
    
        return true;
    }
    case 0x00CE:
    {
        float p0 = CodeGetD();
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        stat = LNSoundListener_SetVelocityXYZ(p0, p1, p2);
    
        return true;
    }
    case 0x00CF:
    {
        str_p0 = CodeGetS();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        intptr_t p1;
        stat = LNSound_Create(str_p0.c_str(), &p1);
        int rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_INT, &rp1);
        return true;
    }
    case 0x00D0:
    {
        void* p0 = (void*)CodeGetS();
        int p1 = CodeGetI();
        PVal* pval_p2;
        APTR aptr_p2 = code_getva(&pval_p2);
        intptr_t p2;
        stat = LNSound_CreateMem(p0, p1, &p2);
        int rp2 = p2; code_setva(pval_p2, aptr_p2, HSPVAR_FLAG_INT, &rp2);
        return true;
    }
    case 0x00D1:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        float p1;
        stat = LNSound_GetVolume(p0, &p1);
        double rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_DOUBLE, &rp1);
        return true;
    }
    case 0x00D2:
    {
        intptr_t p0 = CodeGetI();
        float p1 = CodeGetD();
        stat = LNSound_SetVolume(p0, p1);
    
        return true;
    }
    case 0x00D3:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        float p1;
        stat = LNSound_GetPitch(p0, &p1);
        double rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_DOUBLE, &rp1);
        return true;
    }
    case 0x00D4:
    {
        intptr_t p0 = CodeGetI();
        float p1 = CodeGetD();
        stat = LNSound_SetPitch(p0, p1);
    
        return true;
    }
    case 0x00D5:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNBool p1;
        stat = LNSound_IsLoopEnabled(p0, &p1);
        int rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_INT, &rp1);
        return true;
    }
    case 0x00D6:
    {
        intptr_t p0 = CodeGetI();
        LNBool p1 = (LNBool)CodeGetI();
        stat = LNSound_SetLoopEnabled(p0, p1);
    
        return true;
    }
    case 0x00D7:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNBool p1;
        stat = LNSound_Is3DEnabled(p0, &p1);
        int rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_INT, &rp1);
        return true;
    }
    case 0x00D8:
    {
        intptr_t p0 = CodeGetI();
        LNBool p1 = (LNBool)CodeGetI();
        stat = LNSound_Set3DEnabled(p0, p1);
    
        return true;
    }
    case 0x00D9:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNSoundPlayingMode p1;
        stat = LNSound_GetPlayingMode(p0, &p1);
        int rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_INT, &rp1);
        return true;
    }
    case 0x00DA:
    {
        intptr_t p0 = CodeGetI();
        int p1 = CodeGetI();
        stat = LNSound_SetPlayingMode(p0, (LNSoundPlayingMode)p1);
    
        return true;
    }
    case 0x00DB:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNSoundPlayingState p1;
        stat = LNSound_GetPlayingState(p0, &p1);
        int rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_INT, &rp1);
        return true;
    }
    case 0x00DC:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        int64_t p1;
        stat = LNSound_GetPlayedSamples(p0, &p1);
        int rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_INT, &rp1);
        return true;
    }
    case 0x00DD:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        int64_t p1;
        stat = LNSound_GetTotalSamples(p0, &p1);
        int rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_INT, &rp1);
        return true;
    }
    case 0x00DE:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        int p1;
        stat = LNSound_GetSamplingRate(p0, &p1);
        int rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_INT, &rp1);
        return true;
    }
    case 0x00DF:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        stat = LNSound_SetEmitterPosition(p0, (LNVector3*)pval_p1->pt);
    
        return true;
    }
    case 0x00E0:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        stat = LNSound_SetEmitterVelocity(p0, (LNVector3*)pval_p1->pt);
    
        return true;
    }
    case 0x00E1:
    {
        intptr_t p0 = CodeGetI();
        float p1 = CodeGetD();
        stat = LNSound_SetEmitterMaxDistance(p0, p1);
    
        return true;
    }
    case 0x00E2:
    {
        intptr_t p0 = CodeGetI();
        int p1 = CodeGetI();
        int p2 = CodeGetI();
        stat = LNSound_SetLoopRange(p0, p1, p2);
    
        return true;
    }
    case 0x00E3:
    {
        intptr_t p0 = CodeGetI();
        stat = LNSound_Play(p0);
    
        return true;
    }
    case 0x00E4:
    {
        intptr_t p0 = CodeGetI();
        stat = LNSound_Stop(p0);
    
        return true;
    }
    case 0x00E5:
    {
        intptr_t p0 = CodeGetI();
        stat = LNSound_Pause(p0);
    
        return true;
    }
    case 0x00E6:
    {
        intptr_t p0 = CodeGetI();
        stat = LNSound_Resume(p0);
    
        return true;
    }
    case 0x00E7:
    {
        intptr_t p0 = CodeGetI();
        float p1 = CodeGetD();
        double p2 = CodeGetD();
        int p3 = CodeGetI();
        stat = LNSound_FadeVolume(p0, p1, p2, (LNSoundFadeBehavior)p3);
    
        return true;
    }
    case 0x00E8:
    {
        intptr_t p0 = CodeGetI();
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        stat = LNSound_SetEmitterPositionXYZ(p0, p1, p2, p3);
    
        return true;
    }
    case 0x00E9:
    {
        intptr_t p0 = CodeGetI();
        float p1 = CodeGetD();
        float p2 = CodeGetD();
        float p3 = CodeGetD();
        stat = LNSound_SetEmitterVelocityXYZ(p0, p1, p2, p3);
    
        return true;
    }
    case 0x00EA:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        LNSize p1;
        stat = LNTexture_GetSize(p0, &p1);
        code_setva(pval_p1, aptr_p1, hspLNSize_typeid(), &p1);
        return true;
    }
    case 0x00EB:
    {
        int p0 = CodeGetI();
        int p1 = CodeGetI();
        int p2 = CodeGetI(LN_FMT_A8R8G8B8);
        LNBool p3 = (LNBool)CodeGetI(LN_FALSE);
        PVal* pval_p4;
        APTR aptr_p4 = code_getva(&pval_p4);
        intptr_t p4;
        stat = LNTexture2D_Create(p0, p1, (LNTextureFormat)p2, p3, &p4);
        int rp4 = p4; code_setva(pval_p4, aptr_p4, HSPVAR_FLAG_INT, &rp4);
        return true;
    }
    case 0x00EC:
    {
        str_p0 = CodeGetS();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        intptr_t p1;
        stat = LNTexture2D_CreateFromFile(str_p0.c_str(), &p1);
        int rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_INT, &rp1);
        return true;
    }
    case 0x00ED:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1; CodeGetVA_TypeChecked(&pval_p1, LNVector3);
        stat = LNSceneNode_SetPosition(p0, (LNVector3*)pval_p1->pt);
    
        return true;
    }
    case 0x00EE:
    {
        intptr_t p0 = CodeGetI();
        intptr_t p1 = CodeGetI();
        stat = LNSprite_SetTexture(p0, p1);
    
        return true;
    }
    case 0x00EF:
    {
        intptr_t p0 = CodeGetI();
        PVal* pval_p1;
        APTR aptr_p1 = code_getva(&pval_p1);
        intptr_t p1;
        stat = LNSprite2D_Create(p0, &p1);
        int rp1 = p1; code_setva(pval_p1, aptr_p1, HSPVAR_FLAG_INT, &rp1);
        return true;
    }

	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}
	return false;
}

