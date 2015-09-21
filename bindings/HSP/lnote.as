//=============================================================================
// LNote
//=============================================================================

#include "user32.as"

#ifndef __lnote__
#define __lnote__

#uselib "LNote.dll"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#define LN_MUSICTIMEBASE 768
//#define LN_EXCEPTION_WIN32_MESSAGE 0xA000

//-----------------------------------------------------------------------------
// enum
//-----------------------------------------------------------------------------
// LNBool
#const LN_FALSE 0
#const LN_TRUE 1

// LNResult
#const LN_OK 0
#const LN_ERROR_UNKNOWN -1
#const LN_ERROR_OUT_OF_MEMORY -2
#const LN_ERROR_IO -3
#const LN_ERROR_END_OF_STREAM -4
#const LN_ERROR_ARGUMENT -5
#const LN_ERROR_INVALID_OPERATION -6
#const LN_ERROR_NOT_SUPPORTED -7
#const LN_ERROR_FILE_NOT_FOUND -8
#const LN_ERROR_DIRECTORY_NOT_FOUND -9
#const LN_ERROR_INVALID_FORMAT -10
#const LN_ERROR_NOT_IMPLEMENTED -11
#const LN_ERROR_RUNTIME -12
#const LN_ERROR_ENCODING_FALLBACK -13
#const LN_ERROR_WIN32 -14
#const LN_ERROR_COM -15

// LNVariantType
#const LN_VARIANTTYPE_UNKNOWN 0
#const LN_VARIANTTYPE_BOOL 1
#const LN_VARIANTTYPE_INT 2
#const LN_VARIANTTYPE_FLOAT 3
#const LN_VARIANTTYPE_STRING 4
#const LN_VARIANTTYPE_LIST 5
#const LN_VARIANTTYPE_OBJECT 6
#const LN_VARIANTTYPE_SIZE_F 7

// LNFileAccessPriority
#const LN_FILEACCESSPRIORITY_ARCHIVE_FIRST 0
#const LN_FILEACCESSPRIORITY_DIRECTORY_FIRST 1
#const LN_FILEACCESSPRIORITY_ARCHIVE_ONLY 2

// LNFileType
#const LN_FILETYPE_BINARY 0
#const LN_FILETYPE_TEXT 1

// LNFileMode
#const LN_FILEMODE_OPEN 0
#const LN_FILEMODE_CREATE 1
#const LN_FILEMODE_APPEND 2

// LNFileAccess
#const LN_FILEACCESS_READ 0
#const LN_FILEACCESS_READWRITE 1
#const LN_FILEACCESS_WRITE 2

// LNSeekOrigin
#const LN_SEEK_BEGIN 0
#const LN_SEEK_CURRENT 1
#const LN_SEEK_END 2

// LNAsyncIOState
#const LN_ASYNCIOSTATE_IDLE 0
#const LN_ASYNCIOSTATE_READY 1
#const LN_ASYNCIOSTATE_PROCESSING 2
#const LN_ASYNCIOSTATE_COMPLETED 3
#const LN_ASYNCIOSTATE_FAILED 4

// LNRotationOrder
#const LN_ROTATIONORDER_XYZ 0
#const LN_ROTATIONORDER_XZY 1
#const LN_ROTATIONORDER_YXZ 2
#const LN_ROTATIONORDER_YZX 3
#const LN_ROTATIONORDER_ZXY 4
#const LN_ROTATIONORDER_ZYX 5

// LNKeyCode
#const LN_KEY_UNKNOWN 0
#const LN_KEY_A 1
#const LN_KEY_B 2
#const LN_KEY_C 3
#const LN_KEY_D 4
#const LN_KEY_E 5
#const LN_KEY_F 6
#const LN_KEY_G 7
#const LN_KEY_H 8
#const LN_KEY_I 9
#const LN_KEY_J 10
#const LN_KEY_K 11
#const LN_KEY_L 12
#const LN_KEY_M 13
#const LN_KEY_N 14
#const LN_KEY_O 15
#const LN_KEY_P 16
#const LN_KEY_Q 17
#const LN_KEY_R 18
#const LN_KEY_S 19
#const LN_KEY_T 20
#const LN_KEY_U 21
#const LN_KEY_V 22
#const LN_KEY_W 23
#const LN_KEY_X 24
#const LN_KEY_Y 25
#const LN_KEY_Z 26
#const LN_KEY_F10 46
#const LN_KEY_F11 47
#const LN_KEY_F12 48
#const LN_KEY_SPACE 49
#const LN_KEY_ESCAPE 50
#const LN_KEY_DOWN 52
#const LN_KEY_LEFT 53
#const LN_KEY_RIGHT 54
#const LN_KEY_LSHIFT 55
#const LN_KEY_RSHIFT 56
#const LN_KEY_LCTRL 57
#const LN_KEY_RCTRL 58
#const LN_KEY_LALT 59
#const LN_KEY_RALT 60
#const LN_KEY_TAB 61
#const LN_KEY_ENTER 62
#const LN_KEY_BACKSPACE 63
#const LN_KEY_INSERT 64
#const LN_KEY_DELETE 65
#const LN_KEY_PAGEUP 66
#const LN_KEY_PAGEDOWN 67
#const LN_KEY_HOME 68
#const LN_KEY_END 69
#const LN_KEY_COLON 70
#const LN_KEY_SEMICOLON 71
#const LN_KEY_COMMA 72
#const LN_KEY_PERIOD 73
#const LN_KEY_SLASH 74
#const LN_KEY_MINUS 75
#const LN_KEY_BACKSLASH 76
#const LN_KEY_YEN 77
#const LN_KEY_CARET 78
#const LN_KEY_LBRACKET 79
#const LN_KEY_RBRACKET 80
#const LN_KEY_ANDROID_PLUS 81

// LNAcceleratorKey
#const LN_ACCELERATORKEY_TOGGLE_FULLSCREEN 0
#const LN_ACCELERATORKEY_SHOW_DEBUG_STATISTICS 1

// LNDirectMusicMode
#const LN_DIRECTMUSICMODE_NOT_USE 0
#const LN_DIRECTMUSICMODE_NORMAL 1
#const LN_DIRECTMUSICMODE_THREAD_WAIT 2
#const LN_DIRECTMUSICMODE_THREAD_REQUEST 3

// LNSoundPlayingMode
#const LN_SOUNDPLAYINGMODE_UNKNOWN 0
#const LN_SOUNDPLAYINGMODE_AUTO 1
#const LN_SOUNDPLAYINGMODE_ONMEMORY 2
#const LN_SOUNDPLAYINGMODE_STREAMING 3
#const LN_SOUNDPLAYINGMODE_MIDI 4

// LNSoundPlayingState
#const LN_SOUNDPLAYINGSTATE_STOPPED 0
#const LN_SOUNDPLAYINGSTATE_PLAYING 1
#const LN_SOUNDPLAYINGSTATE_PAUSING 2

// LNSoundFadeBehavior
#const LN_SOUNDFADEBEHAVIORBEHAVIOR_CONTINUE 0
#const LN_SOUNDFADEBEHAVIOR_STOP 1
#const LN_SOUNDFADEBEHAVIOR_STOP_RESET 2
#const LN_SOUNDFADEBEHAVIOR_PAUSE 3
#const LN_SOUNDFADEBEHAVIOR_PAUSE_RESET 4

// LNInternalGameSound
#const LN_INTERNALGAMESOUND_BGM 0
#const LN_INTERNALGAMESOUND_BGS 1
#const LN_INTERNALGAMESOUND_ME 2

// LNBackbufferResizeMode
#const LN_BACKBUFFERRESIZEMODE_SCALING 0
#const LN_BACKBUFFERRESIZEMODE_SCALING_WITH_LETTER_BOX 1
#const LN_BACKBUFFERRESIZEMODE_RESIZE 2

// LNTextureFormat
#const LN_FMT_UNKNOWN 0
#const LN_FMT_A8R8G8B8 1
#const LN_FMT_X8R8G8B8 2
#const LN_FMT_A16B16G16R16F 3
#const LN_FMT_A32B32G32R32F 4
#const LN_FMT_D24S8 5

// LNBlendMode
#const LN_BLEND_NORMAL 0
#const LN_BLEND_ALPHA 1
#const LN_BLEND_ADD 2
#const LN_BLEND_ADD_ALPHA_DISABLE 3
#const LN_BLEND_SUB 4
#const LN_BLEND_SUB_ALPHA_DISABLE 5
#const LN_BLEND_MUL 6
#const LN_BLEND_SCREEN 7
#const LN_BLEND_REVERSE 8

// LNCullingMode
#const LN_CULL_FRONT 0
#const LN_CULL_BACK 1
#const LN_CULL_DOUBLE 2

// LNTextAlign
#const LN_TEXTALIGN_LEFT 0
#const LN_TEXTALIGN_CENTER 1
#const LN_TEXTALIGN_RIGHT 2

// LNPlacementType
#const LN_PLACEMENTTYPE_NORMAL 0
#const LN_PLACEMENTTYPE_BILLBOARD 1
#const LN_PLACEMENTTYPE_Y_FIXED 2

// LNFrontAxis
#const LN_FRONTAXIS_X 0
#const LN_FRONTAXIS_Y 1
#const LN_FRONTAXIS_Z 2
#const LN_FRONTAXIS_RX 3
#const LN_FRONTAXIS_RY 4
#const LN_FRONTAXIS_RZ 5

// LNMeshCreateFlags
#const LN_MESHCREATEFLAGS_SOFTWARE_SKINNING 0x01
#const LN_MESHCREATEFLAGS_COLLISION_MESH 0x02
#const LN_MESHCREATEFLAGS_IGNORE_TEXTURE_NOT_FOUND 0x04

// LNCameraType
#const LN_CAMERATYPE_POSITION_AND_LOOKAT 0
#const LN_CAMERATYPE_POSITION_AND_ANGLE 1
#const LN_CAMERATYPE_TRANSFORM_MATRIX 2

// LNCoord
#const LN_COORD_SPACE_2D 0
#const LN_COORD_SPACE_3D 1

// LNObjectType
#const LN_OBJECTTYPE_SOUND 0



//-----------------------------------------------------------------------------
// funcs
//-----------------------------------------------------------------------------
//LNObject
#func native_LNObject_Release "LNObject_Release" int
#define LNObject_Release(%1) native_LNObject_Release %1
#func native_LNObject_AddRef "LNObject_AddRef" int
#define LNObject_AddRef(%1) native_LNObject_AddRef %1
#func native_LNObject_GetRefCount "LNObject_GetRefCount" int, var
#define LNObject_GetRefCount(%1, %2) native_LNObject_GetRefCount %1, %2

//LNConfig
#func native_LNConfig_SetApplicationLogEnabled "LNConfig_SetApplicationLogEnabled" int
#define LNConfig_SetApplicationLogEnabled(%1) native_LNConfig_SetApplicationLogEnabled %1
#func native_LNConfig_SetConsoleEnabled "LNConfig_SetConsoleEnabled" int
#define LNConfig_SetConsoleEnabled(%1) native_LNConfig_SetConsoleEnabled %1
#func native_LNConfig_RegisterArchive "LNConfig_RegisterArchive" str, str
#define LNConfig_RegisterArchive(%1, %2) native_LNConfig_RegisterArchive %1, %2
#func native_LNConfig_SetFileAccessPriority "LNConfig_SetFileAccessPriority" int
#define LNConfig_SetFileAccessPriority(%1) native_LNConfig_SetFileAccessPriority %1
#func native_LNConfig_SetUserWindowHandle "LNConfig_SetUserWindowHandle" void*
#define LNConfig_SetUserWindowHandle(%1) native_LNConfig_SetUserWindowHandle %1
#func native_LNConfig_SetSoundCacheSize "LNConfig_SetSoundCacheSize" int, int
#define LNConfig_SetSoundCacheSize(%1, %2) native_LNConfig_SetSoundCacheSize %1, %2
#func native_LNConfig_SetDirectMusicInitializeMode "LNConfig_SetDirectMusicInitializeMode" int
#define LNConfig_SetDirectMusicInitializeMode(%1) native_LNConfig_SetDirectMusicInitializeMode %1
#func native_LNConfig_SetDirectMusicReverbLevel "LNConfig_SetDirectMusicReverbLevel" int
#define LNConfig_SetDirectMusicReverbLevel(%1) native_LNConfig_SetDirectMusicReverbLevel %1

//LNApplication
#func native_LNApplication_Initialize "LNApplication_Initialize" 
#define LNApplication_Initialize native_LNApplication_Initialize 
#func native_LNApplication_InitializeAudio "LNApplication_InitializeAudio" 
#define LNApplication_InitializeAudio native_LNApplication_InitializeAudio 
#func native_LNApplication_Update "LNApplication_Update" 
#define LNApplication_Update native_LNApplication_Update 
#func native_LNApplication_ResetFrameDelay "LNApplication_ResetFrameDelay" 
#define LNApplication_ResetFrameDelay native_LNApplication_ResetFrameDelay 
#func native_LNApplication_IsEndRequested "LNApplication_IsEndRequested" var
#define LNApplication_IsEndRequested(%1) native_LNApplication_IsEndRequested %1
#func native_LNApplication_Finalize "LNApplication_Finalize" 
#define LNApplication_Finalize native_LNApplication_Finalize 

#func native_LNVector2_GetLength "LNVector2_GetLengthD" var, var
#define LNVector2_GetLength(%1, %2) native_LNVector2_GetLength %1, %2
#func native_LNVector2_GetSquareLength "LNVector2_GetSquareLengthD" var, var
#define LNVector2_GetSquareLength(%1, %2) native_LNVector2_GetSquareLength %1, %2
#func native_LNVector2_Set "LNVector2_SetD" var, double, double
#define LNVector2_Set(%1, %2, %3) native_LNVector2_Set %1, %2, %3
#func native_LNVector2_Normalize "LNVector2_Normalize" var, var
#define LNVector2_Normalize(%1, %2) native_LNVector2_Normalize %1, %2
#func native_LNVector2_NormalizeV "LNVector2_NormalizeV" var
#define LNVector2_NormalizeV(%1) native_LNVector2_NormalizeV %1
#func native_LNVector3_GetLength "LNVector3_GetLengthD" var, var
#define LNVector3_GetLength(%1, %2) native_LNVector3_GetLength %1, %2
#func native_LNVector3_GetSquareLength "LNVector3_GetSquareLengthD" var, var
#define LNVector3_GetSquareLength(%1, %2) native_LNVector3_GetSquareLength %1, %2
#func native_LNVector3_Set "LNVector3_SetD" var, double, double, double
#define LNVector3_Set(%1, %2, %3, %4) native_LNVector3_Set %1, %2, %3, %4
#func native_LNVector3_SetVZ "LNVector3_SetVZD" var, var, double
#define LNVector3_SetVZ(%1, %2, %3) native_LNVector3_SetVZ %1, %2, %3
#func native_LNVector3_Normalize "LNVector3_Normalize" var, var
#define LNVector3_Normalize(%1, %2) native_LNVector3_Normalize %1, %2
#func native_LNVector3_NormalizeV "LNVector3_NormalizeV" var
#define LNVector3_NormalizeV(%1) native_LNVector3_NormalizeV %1
#func native_LNVector3_Dot "LNVector3_DotD" var, var, var
#define LNVector3_Dot(%1, %2, %3) native_LNVector3_Dot %1, %2, %3
#func native_LNVector3_Cross "LNVector3_Cross" var, var, var
#define LNVector3_Cross(%1, %2, %3) native_LNVector3_Cross %1, %2, %3
#func native_LNVector3_Reflect "LNVector3_Reflect" var, var, var
#define LNVector3_Reflect(%1, %2, %3) native_LNVector3_Reflect %1, %2, %3
#func native_LNVector3_Slide "LNVector3_Slide" var, var, var
#define LNVector3_Slide(%1, %2, %3) native_LNVector3_Slide %1, %2, %3
#func native_LNVector3_Lerp "LNVector3_LerpD" var, var, double, var
#define LNVector3_Lerp(%1, %2, %3, %4) native_LNVector3_Lerp %1, %2, %3, %4
#func native_LNVector3_CatmullRom "LNVector3_CatmullRomD" var, var, var, var, double, var
#define LNVector3_CatmullRom(%1, %2, %3, %4, %5, %6) native_LNVector3_CatmullRom %1, %2, %3, %4, %5, %6
#func native_LNVector3_Transform "LNVector3_Transform" var, var, var
#define LNVector3_Transform(%1, %2, %3) native_LNVector3_Transform %1, %2, %3
#func native_LNVector3_TransformCoord "LNVector3_TransformCoord" var, var, var
#define LNVector3_TransformCoord(%1, %2, %3) native_LNVector3_TransformCoord %1, %2, %3
#func native_LNVector4_Set "LNVector4_SetD" var, double, double, double, double
#define LNVector4_Set(%1, %2, %3, %4, %5) native_LNVector4_Set %1, %2, %3, %4, %5
#func native_LNMatrix_GetRight "LNMatrix_GetRight" var, var
#define LNMatrix_GetRight(%1, %2) native_LNMatrix_GetRight %1, %2
#func native_LNMatrix_GetUp "LNMatrix_GetUp" var, var
#define LNMatrix_GetUp(%1, %2) native_LNMatrix_GetUp %1, %2
#func native_LNMatrix_GetFront "LNMatrix_GetFront" var, var
#define LNMatrix_GetFront(%1, %2) native_LNMatrix_GetFront %1, %2
#func native_LNMatrix_GetPosition "LNMatrix_GetPosition" var, var
#define LNMatrix_GetPosition(%1, %2) native_LNMatrix_GetPosition %1, %2
#func native_LNMatrix_Identity "LNMatrix_Identity" var
#define LNMatrix_Identity(%1) native_LNMatrix_Identity %1
#func native_LNMatrix_Translate "LNMatrix_TranslateD" var, double, double, double
#define LNMatrix_Translate(%1, %2, %3, %4) native_LNMatrix_Translate %1, %2, %3, %4
#func native_LNMatrix_TranslateVec3 "LNMatrix_TranslateVec3" var, var
#define LNMatrix_TranslateVec3(%1, %2) native_LNMatrix_TranslateVec3 %1, %2
#func native_LNMatrix_RotateX "LNMatrix_RotateXD" var, double
#define LNMatrix_RotateX(%1, %2) native_LNMatrix_RotateX %1, %2
#func native_LNMatrix_RotateY "LNMatrix_RotateYD" var, double
#define LNMatrix_RotateY(%1, %2) native_LNMatrix_RotateY %1, %2
#func native_LNMatrix_RotateZ "LNMatrix_RotateZD" var, double
#define LNMatrix_RotateZ(%1, %2) native_LNMatrix_RotateZ %1, %2
#func native_LNMatrix_Rotate "LNMatrix_RotateD" var, double, double, double, int
#define LNMatrix_Rotate(%1, %2, %3, %4, %5=LN_ROTATIONORDER_XYZ) native_LNMatrix_Rotate %1, %2, %3, %4, %5
#func native_LNMatrix_RotateVec3 "LNMatrix_RotateVec3" var, var, int
#define LNMatrix_RotateVec3(%1, %2, %3=LN_ROTATIONORDER_XYZ) native_LNMatrix_RotateVec3 %1, %2, %3
#func native_LNMatrix_RotateAxis "LNMatrix_RotateAxisD" var, var, double
#define LNMatrix_RotateAxis(%1, %2, %3) native_LNMatrix_RotateAxis %1, %2, %3
#func native_LNMatrix_RotateQuaternion "LNMatrix_RotateQuaternion" var, var
#define LNMatrix_RotateQuaternion(%1, %2) native_LNMatrix_RotateQuaternion %1, %2
#func native_LNMatrix_Scale "LNMatrix_ScaleD" var, double
#define LNMatrix_Scale(%1, %2) native_LNMatrix_Scale %1, %2
#func native_LNMatrix_ScaleXYZ "LNMatrix_ScaleXYZD" var, double, double, double
#define LNMatrix_ScaleXYZ(%1, %2, %3, %4) native_LNMatrix_ScaleXYZ %1, %2, %3, %4
#func native_LNMatrix_ScaleVec3 "LNMatrix_ScaleVec3" var, var
#define LNMatrix_ScaleVec3(%1, %2) native_LNMatrix_ScaleVec3 %1, %2
#func native_LNMatrix_Multiply "LNMatrix_Multiply" var, var, var
#define LNMatrix_Multiply(%1, %2, %3) native_LNMatrix_Multiply %1, %2, %3
#func native_LNMatrix_Inverse "LNMatrix_Inverse" var, var
#define LNMatrix_Inverse(%1, %2) native_LNMatrix_Inverse %1, %2
#func native_LNMatrix_Transpose "LNMatrix_Transpose" var, var
#define LNMatrix_Transpose(%1, %2) native_LNMatrix_Transpose %1, %2
#func native_LNMatrix_ViewTransformLH "LNMatrix_ViewTransformLH" var, var, var, var
#define LNMatrix_ViewTransformLH(%1, %2, %3, %4) native_LNMatrix_ViewTransformLH %1, %2, %3, %4
#func native_LNMatrix_ViewTransformRH "LNMatrix_ViewTransformRH" var, var, var, var
#define LNMatrix_ViewTransformRH(%1, %2, %3, %4) native_LNMatrix_ViewTransformRH %1, %2, %3, %4
#func native_LNMatrix_PerspectiveFovLH "LNMatrix_PerspectiveFovLHD" double, double, double, double, var
#define LNMatrix_PerspectiveFovLH(%1, %2, %3, %4, %5) native_LNMatrix_PerspectiveFovLH %1, %2, %3, %4, %5
#func native_LNMatrix_PerspectiveFovRH "LNMatrix_PerspectiveFovRHD" double, double, double, double, var
#define LNMatrix_PerspectiveFovRH(%1, %2, %3, %4, %5) native_LNMatrix_PerspectiveFovRH %1, %2, %3, %4, %5
#func native_LNMatrix_OrthoLH "LNMatrix_OrthoLHD" double, double, double, double, var
#define LNMatrix_OrthoLH(%1, %2, %3, %4, %5) native_LNMatrix_OrthoLH %1, %2, %3, %4, %5
#func native_LNMatrix_OrthoRH "LNMatrix_OrthoRHD" double, double, double, double, var
#define LNMatrix_OrthoRH(%1, %2, %3, %4, %5) native_LNMatrix_OrthoRH %1, %2, %3, %4, %5
#func native_LNMatrix_GetEulerAngles "LNMatrix_GetEulerAngles" var, var
#define LNMatrix_GetEulerAngles(%1, %2) native_LNMatrix_GetEulerAngles %1, %2
#func native_LNMatrix_Decompose "LNMatrix_Decompose" var, var, var, var
#define LNMatrix_Decompose(%1, %2, %3, %4) native_LNMatrix_Decompose %1, %2, %3, %4
#func native_LNQuaternion_Set "LNQuaternion_SetD" var, double, double, double, double
#define LNQuaternion_Set(%1, %2, %3, %4, %5) native_LNQuaternion_Set %1, %2, %3, %4, %5
#func native_LNQuaternion_Identity "LNQuaternion_Identity" var
#define LNQuaternion_Identity(%1) native_LNQuaternion_Identity %1
#func native_LNQuaternion_RotationAxis "LNQuaternion_RotationAxisD" var, double, var
#define LNQuaternion_RotationAxis(%1, %2, %3) native_LNQuaternion_RotationAxis %1, %2, %3
#func native_LNQuaternion_RotationMatrix "LNQuaternion_RotationMatrix" var, var
#define LNQuaternion_RotationMatrix(%1, %2) native_LNQuaternion_RotationMatrix %1, %2
#func native_LNQuaternion_RotationYawPitchRoll "LNQuaternion_RotationYawPitchRollD" double, double, double, var
#define LNQuaternion_RotationYawPitchRoll(%1, %2, %3, %4) native_LNQuaternion_RotationYawPitchRoll %1, %2, %3, %4
#func native_LNQuaternion_Normalize "LNQuaternion_Normalize" var, var
#define LNQuaternion_Normalize(%1, %2) native_LNQuaternion_Normalize %1, %2
#func native_LNQuaternion_Conjugate "LNQuaternion_Conjugate" var, var
#define LNQuaternion_Conjugate(%1, %2) native_LNQuaternion_Conjugate %1, %2
#func native_LNQuaternion_Multiply "LNQuaternion_Multiply" var, var, var
#define LNQuaternion_Multiply(%1, %2, %3) native_LNQuaternion_Multiply %1, %2, %3
#func native_LNQuaternion_Slerp "LNQuaternion_SlerpD" var, var, double, var
#define LNQuaternion_Slerp(%1, %2, %3, %4) native_LNQuaternion_Slerp %1, %2, %3, %4
//LNAudio
#func native_LNAudio_PlayBGM "LNAudio_PlayBGM" str, int, int, double
#define LNAudio_PlayBGM(%1, %2=100, %3=100, %4=0.0) native_LNAudio_PlayBGM %1, %2, %3, %4
#func native_LNAudio_PlayBGMMem "LNAudio_PlayBGMMem" int, int, int, int, double
#define LNAudio_PlayBGMMem(%1, %2, %3=100, %4=100, %5=0.0) native_LNAudio_PlayBGMMem %1, %2, %3, %4, %5
#func native_LNAudio_StopBGM "LNAudio_StopBGM" double
#define LNAudio_StopBGM(%1=0.0) native_LNAudio_StopBGM %1
#func native_LNAudio_PlayBGS "LNAudio_PlayBGS" str, int, int, double
#define LNAudio_PlayBGS(%1, %2=100, %3=100, %4=0.0) native_LNAudio_PlayBGS %1, %2, %3, %4
#func native_LNAudio_PlayBGSMem "LNAudio_PlayBGSMem" int, int, int, int, double
#define LNAudio_PlayBGSMem(%1, %2, %3=100, %4=100, %5=0.0) native_LNAudio_PlayBGSMem %1, %2, %3, %4, %5
#func native_LNAudio_StopBGS "LNAudio_StopBGS" double
#define LNAudio_StopBGS(%1=0.0) native_LNAudio_StopBGS %1
#func native_LNAudio_PlayME "LNAudio_PlayME" str, int, int
#define LNAudio_PlayME(%1, %2=100, %3=100) native_LNAudio_PlayME %1, %2, %3
#func native_LNAudio_PlayMEMem "LNAudio_PlayMEMem" int, int, int, int
#define LNAudio_PlayMEMem(%1, %2, %3=100, %4=100) native_LNAudio_PlayMEMem %1, %2, %3, %4
#func native_LNAudio_StopME "LNAudio_StopME" 
#define LNAudio_StopME native_LNAudio_StopME 
#func native_LNAudio_PlaySE "LNAudio_PlaySE" str, int, int
#define LNAudio_PlaySE(%1, %2=100, %3=100) native_LNAudio_PlaySE %1, %2, %3
#func native_LNAudio_PlaySE3D "LNAudio_PlaySE3DD" str, var, double, int, int
#define LNAudio_PlaySE3D(%1, %2, %3, %4=100, %5=100) native_LNAudio_PlaySE3D %1, %2, %3, %4, %5
#func native_LNAudio_PlaySE3DXYZ "LNAudio_PlaySE3DXYZD" str, double, double, double, double, int, int
#define LNAudio_PlaySE3DXYZ(%1, %2, %3, %4, %5, %6=100, %7=100) native_LNAudio_PlaySE3DXYZ %1, %2, %3, %4, %5, %6, %7
#func native_LNAudio_PlaySEMem "LNAudio_PlaySEMem" int, int, int, int
#define LNAudio_PlaySEMem(%1, %2, %3=100, %4=100) native_LNAudio_PlaySEMem %1, %2, %3, %4
#func native_LNAudio_PlaySE3DMem "LNAudio_PlaySE3DMemD" int, int, var, double, int, int
#define LNAudio_PlaySE3DMem(%1, %2, %3, %4, %5=100, %6=100) native_LNAudio_PlaySE3DMem %1, %2, %3, %4, %5, %6
#func native_LNAudio_PlaySE3DMemXYZ "LNAudio_PlaySE3DMemXYZD" int, int, double, double, double, double, int, int
#define LNAudio_PlaySE3DMemXYZ(%1, %2, %3, %4, %5, %6, %7=100, %8=100) native_LNAudio_PlaySE3DMemXYZ %1, %2, %3, %4, %5, %6, %7, %8
#func native_LNAudio_StopSE "LNAudio_StopSE" 
#define LNAudio_StopSE native_LNAudio_StopSE 
#func native_LNAudio_SetBGMVolume "LNAudio_SetBGMVolume" int, double
#define LNAudio_SetBGMVolume(%1, %2=0.0) native_LNAudio_SetBGMVolume %1, %2
#func native_LNAudio_SetBGSVolume "LNAudio_SetBGSVolume" int, double
#define LNAudio_SetBGSVolume(%1, %2=0.0) native_LNAudio_SetBGSVolume %1, %2

//LNSoundListener
#func native_LNSoundListener_SetPosition "LNSoundListener_SetPosition" var
#define LNSoundListener_SetPosition(%1) native_LNSoundListener_SetPosition %1
#func native_LNSoundListener_SetDirection "LNSoundListener_SetDirection" var
#define LNSoundListener_SetDirection(%1) native_LNSoundListener_SetDirection %1
#func native_LNSoundListener_SetUpDirection "LNSoundListener_SetUpDirection" var
#define LNSoundListener_SetUpDirection(%1) native_LNSoundListener_SetUpDirection %1
#func native_LNSoundListener_SetVelocity "LNSoundListener_SetVelocity" var
#define LNSoundListener_SetVelocity(%1) native_LNSoundListener_SetVelocity %1
#func native_LNSoundListener_SetPositionXYZ "LNSoundListener_SetPositionXYZD" double, double, double
#define LNSoundListener_SetPositionXYZ(%1, %2, %3) native_LNSoundListener_SetPositionXYZ %1, %2, %3
#func native_LNSoundListener_SetDirectionXYZ "LNSoundListener_SetDirectionXYZD" double, double, double
#define LNSoundListener_SetDirectionXYZ(%1, %2, %3) native_LNSoundListener_SetDirectionXYZ %1, %2, %3
#func native_LNSoundListener_SetUpDirectionXYZ "LNSoundListener_SetUpDirectionXYZD" double, double, double
#define LNSoundListener_SetUpDirectionXYZ(%1, %2, %3) native_LNSoundListener_SetUpDirectionXYZ %1, %2, %3
#func native_LNSoundListener_SetVelocityXYZ "LNSoundListener_SetVelocityXYZD" double, double, double
#define LNSoundListener_SetVelocityXYZ(%1, %2, %3) native_LNSoundListener_SetVelocityXYZ %1, %2, %3

//LNSound
#func native_LNSound_GetVolume "LNSound_GetVolume" int, var
#define LNSound_GetVolume(%1, %2) native_LNSound_GetVolume %1, %2
#func native_LNSound_SetVolume "LNSound_SetVolume" int, int
#define LNSound_SetVolume(%1, %2) native_LNSound_SetVolume %1, %2
#func native_LNSound_GetPitch "LNSound_GetPitch" int, var
#define LNSound_GetPitch(%1, %2) native_LNSound_GetPitch %1, %2
#func native_LNSound_SetPitch "LNSound_SetPitch" int, int
#define LNSound_SetPitch(%1, %2) native_LNSound_SetPitch %1, %2
#func native_LNSound_SetLoopEnabled "LNSound_SetLoopEnabled" int, int
#define LNSound_SetLoopEnabled(%1, %2) native_LNSound_SetLoopEnabled %1, %2
#func native_LNSound_IsLoopEnabled "LNSound_IsLoopEnabled" int, var
#define LNSound_IsLoopEnabled(%1, %2) native_LNSound_IsLoopEnabled %1, %2
#func native_LNSound_SetLoopRange "LNSound_SetLoopRange" int, int, int
#define LNSound_SetLoopRange(%1, %2, %3) native_LNSound_SetLoopRange %1, %2, %3
#func native_LNSound_Set3DEnabled "LNSound_Set3DEnabled" int, int
#define LNSound_Set3DEnabled(%1, %2) native_LNSound_Set3DEnabled %1, %2
#func native_LNSound_Is3DEnabled "LNSound_Is3DEnabled" int, var
#define LNSound_Is3DEnabled(%1, %2) native_LNSound_Is3DEnabled %1, %2
#func native_LNSound_GetPlayingMode "LNSound_GetPlayingMode" int, var
#define LNSound_GetPlayingMode(%1, %2) native_LNSound_GetPlayingMode %1, %2
#func native_LNSound_SetPlayingMode "LNSound_SetPlayingMode" int, int
#define LNSound_SetPlayingMode(%1, %2) native_LNSound_SetPlayingMode %1, %2
#func native_LNSound_GetPlayingState "LNSound_GetPlayingState" int, var
#define LNSound_GetPlayingState(%1, %2) native_LNSound_GetPlayingState %1, %2
#func native_LNSound_GetPlayedSamples "LNSound_GetPlayedSamples" int, var
#define LNSound_GetPlayedSamples(%1, %2) native_LNSound_GetPlayedSamples %1, %2
#func native_LNSound_GetTotalSamples "LNSound_GetTotalSamples" int, var
#define LNSound_GetTotalSamples(%1, %2) native_LNSound_GetTotalSamples %1, %2
#func native_LNSound_GetSamplingRate "LNSound_GetSamplingRate" int, var
#define LNSound_GetSamplingRate(%1, %2) native_LNSound_GetSamplingRate %1, %2
#func native_LNSound_SetEmitterPosition "LNSound_SetEmitterPosition" int, var
#define LNSound_SetEmitterPosition(%1, %2) native_LNSound_SetEmitterPosition %1, %2
#func native_LNSound_SetEmitterVelocity "LNSound_SetEmitterVelocity" int, var
#define LNSound_SetEmitterVelocity(%1, %2) native_LNSound_SetEmitterVelocity %1, %2
#func native_LNSound_SetEmitterMaxDistance "LNSound_SetEmitterMaxDistanceD" int, double
#define LNSound_SetEmitterMaxDistance(%1, %2) native_LNSound_SetEmitterMaxDistance %1, %2
#func native_LNSound_Create "LNSound_Create" str, var
#define LNSound_Create(%1, %2) native_LNSound_Create %1, %2
#func native_LNSound_CreateMem "LNSound_CreateMem" int, int, var
#define LNSound_CreateMem(%1, %2, %3) native_LNSound_CreateMem %1, %2, %3
#func native_LNSound_Play "LNSound_Play" int
#define LNSound_Play(%1) native_LNSound_Play %1
#func native_LNSound_Stop "LNSound_Stop" int
#define LNSound_Stop(%1) native_LNSound_Stop %1
#func native_LNSound_Pause "LNSound_Pause" int
#define LNSound_Pause(%1) native_LNSound_Pause %1
#func native_LNSound_Resume "LNSound_Resume" int
#define LNSound_Resume(%1) native_LNSound_Resume %1
#func native_LNSound_FadeVolume "LNSound_FadeVolume" int, int, double, int
#define LNSound_FadeVolume(%1, %2, %3, %4) native_LNSound_FadeVolume %1, %2, %3, %4
#func native_LNSound_SetEmitterPositionXYZ "LNSound_SetEmitterPositionXYZD" int, double, double, double
#define LNSound_SetEmitterPositionXYZ(%1, %2, %3, %4) native_LNSound_SetEmitterPositionXYZ %1, %2, %3, %4
#func native_LNSound_SetEmitterVelocityXYZ "LNSound_SetEmitterVelocityXYZD" int, double, double, double
#define LNSound_SetEmitterVelocityXYZ(%1, %2, %3, %4) native_LNSound_SetEmitterVelocityXYZ %1, %2, %3, %4



        const string ASFileFooter = @"
#func LException_SetEnableWrappedException ""LException_SetEnableWrappedException"" int
#func LHSPInternal_GetIntPtrStringLength ""LHSPInternal_GetIntPtrStringLength"" int, var
#func LHSPInternal_GetIntPtrString ""LHSPInternal_GetIntPtrString"" int, var

//------------------------------------------------------------------------------
// overrides
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
// LManager_Initialize
#deffunc _LManager_Initialize
	_LConfig_SetUserWindowHandle@ hwnd
	__LManager_Initialize@
	return stat
	
//--------------------------------------------------------------------------
// LManager_Update
#deffunc _LManager_Update
	__LManager_Update@
	await 0
	return stat
	
//--------------------------------------------------------------------------
// LFile_ExistDirectoryOrEXE(stat: 0=なし 1=ディレクトリに存在(優先) 2=EXEに存在)
#deffunc LFile_ExistDirectoryOrEXE str p1
	b = LN_FALSE@
	_LFile_Exists@ p1, b
	if b != LN_FALSE@ : return 1
	exist p1
	if strsize != -1 : return 2
	return 0

//--------------------------------------------------------------------------
// LManager_InitializeAudio
#deffunc _LManager_InitializeAudio
	_LConfig_SetUserWindowHandle@ hwnd
	__LManager_InitializeAudio@
	return

//--------------------------------------------------------------------------
// LAudio_PlayBGM
#deffunc _LAudio_PlayBGM str p1, int p2, int p3, int p4
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlayBGMMem@ buf, strsize, p2, p3, p4
	} else : if stat == 1 {
		__LAudio_PlayBGM@ p1, p2, p3, p4
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlayBGM""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlayBGS
#deffunc _LAudio_PlayBGS str p1, int p2, int p3, int p4
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlayBGSMem@ buf, strsize, p2, p3, p4
	} else : if stat == 1 {
		__LAudio_PlayBGS@ p1, p2, p3, p4
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlayBGS""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlayME
#deffunc _LAudio_PlayME str p1, int p2, int p3
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlayMEMem@ buf, strsize, p2, p3
	} else : if stat == 1 {
		__LAudio_PlayME@ p1, p2, p3
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlayME""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlaySE
#deffunc _LAudio_PlaySE str p1, int p2, int p3
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlaySEMem@ buf, strsize, p2, p3
	} else : if stat == 1 {
		__LAudio_PlaySE@ p1, p2, p3
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlaySE""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlaySE3D
#deffunc _LAudio_PlaySE3D str p1, var p2, double p3, int p4, int p5
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlaySE3DMem@ buf, strsize, p2, p3, p4, p5
	} else : if stat == 1 {
		__LAudio_PlaySE3D@ p1, p2, p3, p4, p5
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlaySE3D""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlaySE3D
#deffunc _LAudio_PlaySE3DXYZ str p1, double p2, double p3, double p4, double p5, int p6, int p7
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlaySE3DMemXYZ@ buf, strsize, p2, p3, p4, p5, p6, p7
	} else : if stat == 1 {
		__LAudio_PlaySE3DXYZ@ p1, p2, p3, p4, p5, p6, p7
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlaySE3D""
	}
	return

//--------------------------------------------------------------------------
// LMatrix_Identity
#deffunc _LMatrix_Identity var p1
	if (length(p1) < 16) : dim p1, 16
    __LMatrix_Identity@ p1
    return

//--------------------------------------------------------------------------
// LQuaternion_Identity
#deffunc _LQuaternion_Identity var p1
	if (length(p1) < 4) : dim p1, 4
    __LQuaternion_Identity@ p1
    return

//--------------------------------------------------------------------------
// LRandom_SetSeed
#deffunc _LRandom_SetSeed var p1, int p2
	if (length(p1) < 4) : dim p1, 4
    __LRandom_SetSeed@ p1, p2
    return

__INTERNAL_DEFILES__

#global

// 例外発生時にコールバックされるラベルの登録
#define LException_SetHandler(%1) oncmd gosub %1, LN_EXCEPTION_WIN32_MESSAGE

goto *lexception_default_label_dummy

// デフォルトの例外ジャンプ
*lexception_default_label
	LException_ProcDefault
	// HSP外部からの SendMessage() によるコールバック内では、
	// end ひとつだけだと終了できないみたい。
	// (ひとつめ:PostQuit → ふたつめ:Dispatch のように流れている？)
	end
	end
	return

*lexception_default_label_dummy

// デフォルト登録
//LException_SetHandler *lexception_default_label


#endif  // __lnote__
