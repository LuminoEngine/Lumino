
#include <windows.h>
#include "../hsp3plugin/hsp3plugin.h"
#include "../../../src/C_API/LuminoC.h"
#include "LuminoHSP.h"

//=============================================================================
// cmdfunc
//=============================================================================

bool Commands_cmdfunc(int cmd, int* retVal)
{
	switch (cmd)
	{
    case 0x0064:
    {
    
        stat = LNError_GetLastErrorCode();
    
        return;
    }
    case 0x0065:
    {
    
        stat = LNError_GetLastErrorMessage();
    
        return;
    }
    case 0x0066:
    {
        intptr_t p0 = CodeGetI(NULL);
    
        stat = LNObject_Release();
    
        return;
    }
    case 0x0067:
    {
        intptr_t p0 = CodeGetI(NULL);
    
        stat = LNObject_AddRef();
    
        return;
    }
    case 0x0068:
    {
        intptr_t p0 = CodeGetI(NULL);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNObject_GetRefCount();
    
        return;
    }
    case 0x0069:
    {
        LNBool p0 = CodeGetI(NULL);
    
        stat = LNConfig_SetApplicationLogEnabled();
    
        return;
    }
    case 0x006A:
    {
        LNBool p0 = CodeGetI(NULL);
    
        stat = LNConfig_SetConsoleEnabled();
    
        return;
    }
    case 0x006B:
    {
        str_p0 = CodeGetS(NULL);
        str_p1 = CodeGetS(NULL);
    
        stat = LNConfig_RegisterArchive();
    
        return;
    }
    case 0x006C:
    {
        int32_t p0 = CodeGetI(NULL);
    
        stat = LNConfig_SetFileAccessPriority();
    
        return;
    }
    case 0x006D:
    {
        void* p0 = (void*)CodeGetI(NULL);
    
        stat = LNConfig_SetUserWindowHandle();
    
        return;
    }
    case 0x006E:
    {
        int32_t p0 = CodeGetI(NULL);
        int32_t p1 = CodeGetI(NULL);
    
        stat = LNConfig_SetSoundCacheSize();
    
        return;
    }
    case 0x006F:
    {
        int32_t p0 = CodeGetI(NULL);
    
        stat = LNConfig_SetDirectMusicInitializeMode();
    
        return;
    }
    case 0x0070:
    {
        int32_t p0 = CodeGetI(NULL);
    
        stat = LNConfig_SetDirectMusicReverbLevel();
    
        return;
    }
    case 0x0071:
    {
    
        stat = LNApplication_InitializeAudio();
    
        return;
    }
    case 0x0072:
    {
    
        stat = LNApplication_Finalize();
    
        return;
    }
    case 0x0073:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector2);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNVector2_GetLength();
    
        return;
    }
    case 0x0074:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector2);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNVector2_GetSquareLength();
    
        return;
    }
    case 0x0075:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector2);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
    
        stat = LNVector2_Set();
    
        return;
    }
    case 0x0076:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector2);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNVector2_Normalize();
    
        return;
    }
    case 0x0077:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector2);
    
        stat = LNVector2_NormalizeV();
    
        return;
    }
    case 0x0078:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNVector3_GetLength();
    
        return;
    }
    case 0x0079:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNVector3_GetSquareLength();
    
        return;
    }
    case 0x007A:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
    
        stat = LNVector3_Set();
    
        return;
    }
    case 0x007B:
    {
        PVal* pval0;
        APTR aptr0 = code_getva(&pval0);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector2);
        float p2 = CodeGetD(NULL);
    
        stat = LNVector3_SetVZ();
    
        return;
    }
    case 0x007C:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNVector3_Normalize();
    
        return;
    }
    case 0x007D:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
    
        stat = LNVector3_NormalizeV();
    
        return;
    }
    case 0x007E:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
        PVal* pval2;
        APTR aptr2 = code_getva(&pval2);
    
        stat = LNVector3_Dot();
    
        return;
    }
    case 0x007F:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
        PVal* pval2;
        APTR aptr2 = code_getva(&pval2);
    
        stat = LNVector3_Cross();
    
        return;
    }
    case 0x0080:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
        PVal* pval2;
        APTR aptr2 = code_getva(&pval2);
    
        stat = LNVector3_Reflect();
    
        return;
    }
    case 0x0081:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
        PVal* pval2;
        APTR aptr2 = code_getva(&pval2);
    
        stat = LNVector3_Slide();
    
        return;
    }
    case 0x0082:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
        float p2 = CodeGetD(NULL);
        PVal* pval3;
        APTR aptr3 = code_getva(&pval3);
    
        stat = LNVector3_Lerp();
    
        return;
    }
    case 0x0083:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
        PVal* pval2; CodeGetVA_TypeChecked(&pval2, LNVector3);
        PVal* pval3; CodeGetVA_TypeChecked(&pval3, LNVector3);
        float p4 = CodeGetD(NULL);
        PVal* pval5;
        APTR aptr5 = code_getva(&pval5);
    
        stat = LNVector3_CatmullRom();
    
        return;
    }
    case 0x0084:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNMatrix);
        PVal* pval2;
        APTR aptr2 = code_getva(&pval2);
    
        stat = LNVector3_Transform();
    
        return;
    }
    case 0x0085:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNMatrix);
        PVal* pval2;
        APTR aptr2 = code_getva(&pval2);
    
        stat = LNVector3_TransformCoord();
    
        return;
    }
    case 0x0086:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector4);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
        float p4 = CodeGetD(NULL);
    
        stat = LNVector4_Set();
    
        return;
    }
    case 0x0087:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNMatrix_GetRight();
    
        return;
    }
    case 0x0088:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNMatrix_GetUp();
    
        return;
    }
    case 0x0089:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNMatrix_GetFront();
    
        return;
    }
    case 0x008A:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNMatrix_GetPosition();
    
        return;
    }
    case 0x008B:
    {
        PVal* pval0;
        APTR aptr0 = code_getva(&pval0);
    
        stat = LNMatrix_Identity();
    
        return;
    }
    case 0x008C:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
    
        stat = LNMatrix_Translate();
    
        return;
    }
    case 0x008D:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
    
        stat = LNMatrix_TranslateVec3();
    
        return;
    }
    case 0x008E:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        float p1 = CodeGetD(NULL);
    
        stat = LNMatrix_RotateX();
    
        return;
    }
    case 0x008F:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        float p1 = CodeGetD(NULL);
    
        stat = LNMatrix_RotateY();
    
        return;
    }
    case 0x0090:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        float p1 = CodeGetD(NULL);
    
        stat = LNMatrix_RotateZ();
    
        return;
    }
    case 0x0091:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
        int32_t p4 = CodeGetI(LN_ROTATIONORDER_XYZ);
    
        stat = LNMatrix_Rotate();
    
        return;
    }
    case 0x0092:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
        int32_t p2 = CodeGetI(LN_ROTATIONORDER_XYZ);
    
        stat = LNMatrix_RotateVec3();
    
        return;
    }
    case 0x0093:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
        float p2 = CodeGetD(NULL);
    
        stat = LNMatrix_RotateAxis();
    
        return;
    }
    case 0x0094:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNQuaternion);
    
        stat = LNMatrix_RotateQuaternion();
    
        return;
    }
    case 0x0095:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        float p1 = CodeGetD(NULL);
    
        stat = LNMatrix_Scale();
    
        return;
    }
    case 0x0096:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
    
        stat = LNMatrix_ScaleXYZ();
    
        return;
    }
    case 0x0097:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
    
        stat = LNMatrix_ScaleVec3();
    
        return;
    }
    case 0x0098:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNMatrix);
        PVal* pval2;
        APTR aptr2 = code_getva(&pval2);
    
        stat = LNMatrix_Multiply();
    
        return;
    }
    case 0x0099:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNMatrix_Inverse();
    
        return;
    }
    case 0x009A:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNMatrix_Transpose();
    
        return;
    }
    case 0x009B:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
        PVal* pval2; CodeGetVA_TypeChecked(&pval2, LNVector3);
        PVal* pval3;
        APTR aptr3 = code_getva(&pval3);
    
        stat = LNMatrix_ViewTransformLH();
    
        return;
    }
    case 0x009C:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
        PVal* pval2; CodeGetVA_TypeChecked(&pval2, LNVector3);
        PVal* pval3;
        APTR aptr3 = code_getva(&pval3);
    
        stat = LNMatrix_ViewTransformRH();
    
        return;
    }
    case 0x009D:
    {
        float p0 = CodeGetD(NULL);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
        PVal* pval4;
        APTR aptr4 = code_getva(&pval4);
    
        stat = LNMatrix_PerspectiveFovLH();
    
        return;
    }
    case 0x009E:
    {
        float p0 = CodeGetD(NULL);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
        PVal* pval4;
        APTR aptr4 = code_getva(&pval4);
    
        stat = LNMatrix_PerspectiveFovRH();
    
        return;
    }
    case 0x009F:
    {
        float p0 = CodeGetD(NULL);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
        PVal* pval4;
        APTR aptr4 = code_getva(&pval4);
    
        stat = LNMatrix_OrthoLH();
    
        return;
    }
    case 0x00A0:
    {
        float p0 = CodeGetD(NULL);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
        PVal* pval4;
        APTR aptr4 = code_getva(&pval4);
    
        stat = LNMatrix_OrthoRH();
    
        return;
    }
    case 0x00A1:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNMatrix_GetEulerAngles();
    
        return;
    }
    case 0x00A2:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
        PVal* pval2;
        APTR aptr2 = code_getva(&pval2);
        PVal* pval3;
        APTR aptr3 = code_getva(&pval3);
    
        stat = LNMatrix_Decompose();
    
        return;
    }
    case 0x00A3:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNQuaternion);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
        float p4 = CodeGetD(NULL);
    
        stat = LNQuaternion_Set();
    
        return;
    }
    case 0x00A4:
    {
        PVal* pval0;
        APTR aptr0 = code_getva(&pval0);
    
        stat = LNQuaternion_Identity();
    
        return;
    }
    case 0x00A5:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
        float p1 = CodeGetD(NULL);
        PVal* pval2;
        APTR aptr2 = code_getva(&pval2);
    
        stat = LNQuaternion_RotationAxis();
    
        return;
    }
    case 0x00A6:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNMatrix);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNQuaternion_RotationMatrix();
    
        return;
    }
    case 0x00A7:
    {
        float p0 = CodeGetD(NULL);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        PVal* pval3;
        APTR aptr3 = code_getva(&pval3);
    
        stat = LNQuaternion_RotationYawPitchRoll();
    
        return;
    }
    case 0x00A8:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNQuaternion);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNQuaternion_Normalize();
    
        return;
    }
    case 0x00A9:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNQuaternion);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNQuaternion_Conjugate();
    
        return;
    }
    case 0x00AA:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNQuaternion);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNQuaternion);
        PVal* pval2;
        APTR aptr2 = code_getva(&pval2);
    
        stat = LNQuaternion_Multiply();
    
        return;
    }
    case 0x00AB:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNQuaternion);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNQuaternion);
        float p2 = CodeGetD(NULL);
        PVal* pval3;
        APTR aptr3 = code_getva(&pval3);
    
        stat = LNQuaternion_Slerp();
    
        return;
    }
    case 0x00AC:
    {
        str_p0 = CodeGetS(NULL);
        int32_t p1 = CodeGetI(100);
        int32_t p2 = CodeGetI(100);
        double p3 = CodeGetD(0.0);
    
        stat = LNAudio_PlayBGM();
    
        return;
    }
    case 0x00AD:
    {
        intptr_t p0 = CodeGetI(NULL);
        int32_t p1 = CodeGetI(NULL);
        int32_t p2 = CodeGetI(100);
        int32_t p3 = CodeGetI(100);
        double p4 = CodeGetD(0.0);
    
        stat = LNAudio_PlayBGMMem();
    
        return;
    }
    case 0x00AE:
    {
        double p0 = CodeGetD(0.0);
    
        stat = LNAudio_StopBGM();
    
        return;
    }
    case 0x00AF:
    {
        str_p0 = CodeGetS(NULL);
        int32_t p1 = CodeGetI(100);
        int32_t p2 = CodeGetI(100);
        double p3 = CodeGetD(0.0);
    
        stat = LNAudio_PlayBGS();
    
        return;
    }
    case 0x00B0:
    {
        intptr_t p0 = CodeGetI(NULL);
        int32_t p1 = CodeGetI(NULL);
        int32_t p2 = CodeGetI(100);
        int32_t p3 = CodeGetI(100);
        double p4 = CodeGetD(0.0);
    
        stat = LNAudio_PlayBGSMem();
    
        return;
    }
    case 0x00B1:
    {
        double p0 = CodeGetD(0.0);
    
        stat = LNAudio_StopBGS();
    
        return;
    }
    case 0x00B2:
    {
        str_p0 = CodeGetS(NULL);
        int32_t p1 = CodeGetI(100);
        int32_t p2 = CodeGetI(100);
    
        stat = LNAudio_PlayME();
    
        return;
    }
    case 0x00B3:
    {
        intptr_t p0 = CodeGetI(NULL);
        int32_t p1 = CodeGetI(NULL);
        int32_t p2 = CodeGetI(100);
        int32_t p3 = CodeGetI(100);
    
        stat = LNAudio_PlayMEMem();
    
        return;
    }
    case 0x00B4:
    {
    
        stat = LNAudio_StopME();
    
        return;
    }
    case 0x00B5:
    {
        str_p0 = CodeGetS(NULL);
        int32_t p1 = CodeGetI(100);
        int32_t p2 = CodeGetI(100);
    
        stat = LNAudio_PlaySE();
    
        return;
    }
    case 0x00B6:
    {
        str_p0 = CodeGetS(NULL);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
        float p2 = CodeGetD(NULL);
        int32_t p3 = CodeGetI(100);
        int32_t p4 = CodeGetI(100);
    
        stat = LNAudio_PlaySE3D();
    
        return;
    }
    case 0x00B7:
    {
        str_p0 = CodeGetS(NULL);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
        float p4 = CodeGetD(NULL);
        int32_t p5 = CodeGetI(100);
        int32_t p6 = CodeGetI(100);
    
        stat = LNAudio_PlaySE3DXYZ();
    
        return;
    }
    case 0x00B8:
    {
        intptr_t p0 = CodeGetI(NULL);
        int32_t p1 = CodeGetI(NULL);
        int32_t p2 = CodeGetI(100);
        int32_t p3 = CodeGetI(100);
    
        stat = LNAudio_PlaySEMem();
    
        return;
    }
    case 0x00B9:
    {
        intptr_t p0 = CodeGetI(NULL);
        int32_t p1 = CodeGetI(NULL);
        PVal* pval2; CodeGetVA_TypeChecked(&pval2, LNVector3);
        float p3 = CodeGetD(NULL);
        int32_t p4 = CodeGetI(100);
        int32_t p5 = CodeGetI(100);
    
        stat = LNAudio_PlaySE3DMem();
    
        return;
    }
    case 0x00BA:
    {
        intptr_t p0 = CodeGetI(NULL);
        int32_t p1 = CodeGetI(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
        float p4 = CodeGetD(NULL);
        float p5 = CodeGetD(NULL);
        int32_t p6 = CodeGetI(100);
        int32_t p7 = CodeGetI(100);
    
        stat = LNAudio_PlaySE3DMemXYZ();
    
        return;
    }
    case 0x00BB:
    {
    
        stat = LNAudio_StopSE();
    
        return;
    }
    case 0x00BC:
    {
        int32_t p0 = CodeGetI(NULL);
        double p1 = CodeGetD(0.0);
    
        stat = LNAudio_SetBGMVolume();
    
        return;
    }
    case 0x00BD:
    {
        int32_t p0 = CodeGetI(NULL);
        double p1 = CodeGetD(0.0);
    
        stat = LNAudio_SetBGSVolume();
    
        return;
    }
    case 0x00BE:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
    
        stat = LNSoundListener_SetPosition();
    
        return;
    }
    case 0x00BF:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
    
        stat = LNSoundListener_SetDirection();
    
        return;
    }
    case 0x00C0:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
    
        stat = LNSoundListener_SetUpDirection();
    
        return;
    }
    case 0x00C1:
    {
        PVal* pval0; CodeGetVA_TypeChecked(&pval0, LNVector3);
    
        stat = LNSoundListener_SetVelocity();
    
        return;
    }
    case 0x00C2:
    {
        float p0 = CodeGetD(NULL);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
    
        stat = LNSoundListener_SetPositionXYZ();
    
        return;
    }
    case 0x00C3:
    {
        float p0 = CodeGetD(NULL);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
    
        stat = LNSoundListener_SetDirectionXYZ();
    
        return;
    }
    case 0x00C4:
    {
        float p0 = CodeGetD(NULL);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
    
        stat = LNSoundListener_SetUpDirectionXYZ();
    
        return;
    }
    case 0x00C5:
    {
        float p0 = CodeGetD(NULL);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
    
        stat = LNSoundListener_SetVelocityXYZ();
    
        return;
    }
    case 0x00C6:
    {
        intptr_t p0 = CodeGetI(NULL);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNSound_GetVolume();
    
        return;
    }
    case 0x00C7:
    {
        intptr_t p0 = CodeGetI(NULL);
        int32_t p1 = CodeGetI(NULL);
    
        stat = LNSound_SetVolume();
    
        return;
    }
    case 0x00C8:
    {
        intptr_t p0 = CodeGetI(NULL);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNSound_GetPitch();
    
        return;
    }
    case 0x00C9:
    {
        intptr_t p0 = CodeGetI(NULL);
        int32_t p1 = CodeGetI(NULL);
    
        stat = LNSound_SetPitch();
    
        return;
    }
    case 0x00CA:
    {
        intptr_t p0 = CodeGetI(NULL);
        LNBool p1 = CodeGetI(NULL);
    
        stat = LNSound_SetLoopEnabled();
    
        return;
    }
    case 0x00CB:
    {
        intptr_t p0 = CodeGetI(NULL);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNSound_IsLoopEnabled();
    
        return;
    }
    case 0x00CC:
    {
        intptr_t p0 = CodeGetI(NULL);
        int32_t p1 = CodeGetI(NULL);
        int32_t p2 = CodeGetI(NULL);
    
        stat = LNSound_SetLoopRange();
    
        return;
    }
    case 0x00CD:
    {
        intptr_t p0 = CodeGetI(NULL);
        LNBool p1 = CodeGetI(NULL);
    
        stat = LNSound_Set3DEnabled();
    
        return;
    }
    case 0x00CE:
    {
        intptr_t p0 = CodeGetI(NULL);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNSound_Is3DEnabled();
    
        return;
    }
    case 0x00CF:
    {
        intptr_t p0 = CodeGetI(NULL);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNSound_GetPlayingMode();
    
        return;
    }
    case 0x00D0:
    {
        intptr_t p0 = CodeGetI(NULL);
        int32_t p1 = CodeGetI(NULL);
    
        stat = LNSound_SetPlayingMode();
    
        return;
    }
    case 0x00D1:
    {
        intptr_t p0 = CodeGetI(NULL);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNSound_GetPlayingState();
    
        return;
    }
    case 0x00D2:
    {
        intptr_t p0 = CodeGetI(NULL);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNSound_GetPlayedSamples();
    
        return;
    }
    case 0x00D3:
    {
        intptr_t p0 = CodeGetI(NULL);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNSound_GetTotalSamples();
    
        return;
    }
    case 0x00D4:
    {
        intptr_t p0 = CodeGetI(NULL);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNSound_GetSamplingRate();
    
        return;
    }
    case 0x00D5:
    {
        intptr_t p0 = CodeGetI(NULL);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
    
        stat = LNSound_SetEmitterPosition();
    
        return;
    }
    case 0x00D6:
    {
        intptr_t p0 = CodeGetI(NULL);
        PVal* pval1; CodeGetVA_TypeChecked(&pval1, LNVector3);
    
        stat = LNSound_SetEmitterVelocity();
    
        return;
    }
    case 0x00D7:
    {
        intptr_t p0 = CodeGetI(NULL);
        float p1 = CodeGetD(NULL);
    
        stat = LNSound_SetEmitterMaxDistance();
    
        return;
    }
    case 0x00D8:
    {
        str_p0 = CodeGetS(NULL);
        PVal* pval1;
        APTR aptr1 = code_getva(&pval1);
    
        stat = LNSound_Create();
    
        return;
    }
    case 0x00D9:
    {
        intptr_t p0 = CodeGetI(NULL);
        int32_t p1 = CodeGetI(NULL);
        PVal* pval2;
        APTR aptr2 = code_getva(&pval2);
    
        stat = LNSound_CreateMem();
    
        return;
    }
    case 0x00DA:
    {
        intptr_t p0 = CodeGetI(NULL);
    
        stat = LNSound_Play();
    
        return;
    }
    case 0x00DB:
    {
        intptr_t p0 = CodeGetI(NULL);
    
        stat = LNSound_Stop();
    
        return;
    }
    case 0x00DC:
    {
        intptr_t p0 = CodeGetI(NULL);
    
        stat = LNSound_Pause();
    
        return;
    }
    case 0x00DD:
    {
        intptr_t p0 = CodeGetI(NULL);
    
        stat = LNSound_Resume();
    
        return;
    }
    case 0x00DE:
    {
        intptr_t p0 = CodeGetI(NULL);
        int32_t p1 = CodeGetI(NULL);
        double p2 = CodeGetD(NULL);
        int32_t p3 = CodeGetI(NULL);
    
        stat = LNSound_FadeVolume();
    
        return;
    }
    case 0x00DF:
    {
        intptr_t p0 = CodeGetI(NULL);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
    
        stat = LNSound_SetEmitterPositionXYZ();
    
        return;
    }
    case 0x00E0:
    {
        intptr_t p0 = CodeGetI(NULL);
        float p1 = CodeGetD(NULL);
        float p2 = CodeGetD(NULL);
        float p3 = CodeGetD(NULL);
    
        stat = LNSound_SetEmitterVelocityXYZ();
    
        return;
    }

	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}
	*retVal = RUNMODE_RUN;
}

