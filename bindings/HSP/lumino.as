
#ifndef __lumino__
#define __lumino__

#regcmd "_hsp3cmdinit@4","LuminoHSP.dll", 5

#const global LN_FALSE 0
#const global LN_TRUE 1

#const global LN_OK 0
#const global LN_ERROR_UNKNOWN -1
#const global LN_ERROR_VERIFY -2
#const global LN_ERROR_ARGUMENT -3
#const global LN_ERROR_INVALID_OPERATION -4
#const global LN_ERROR_NOT_IMPLEMENTED -5
#const global LN_ERROR_OUT_OF_MEMORY -6
#const global LN_ERROR_OUT_OF_RANGE -7
#const global LN_ERROR_KEY_NOT_FOUND -8
#const global LN_ERROR_OVERFLOW -9
#const global LN_ERROR_IO -10
#const global LN_ERROR_FILE_NOT_FOUND -11
#const global LN_ERROR_DIRECTORY_NOT_FOUND -12
#const global LN_ERROR_INVALID_FORMAT -13
#const global LN_ERROR_END_OF_STREAM -14
#const global LN_ERROR_ENCODING -15
#const global LN_ERROR_WIN32 -16
#const global LN_ERROR_COM -17

#const global LN_VARIANTTYPE_UNKNOWN 0
#const global LN_VARIANTTYPE_BOOL 1
#const global LN_VARIANTTYPE_INT 2
#const global LN_VARIANTTYPE_FLOAT 3
#const global LN_VARIANTTYPE_STRING 4
#const global LN_VARIANTTYPE_LIST 5
#const global LN_VARIANTTYPE_OBJECT 6
#const global LN_VARIANTTYPE_SIZE_F 7
#const global LN_VARIANTTYPE__TERMINATOR 8

#const global LN_FILEACCESSPRIORITY_ARCHIVE_FIRST 0
#const global LN_FILEACCESSPRIORITY_DIRECTORY_FIRST 1
#const global LN_FILEACCESSPRIORITY_ARCHIVE_ONLY 2
#const global LN_FILEACCESSPRIORITY__TERMINATOR 3

#const global LN_FILETYPE_BINARY 0
#const global LN_FILETYPE_TEXT 1
#const global LN_FILETYPE__TERMINATOR 2

#const global LN_FILEMODE_OPEN 0
#const global LN_FILEMODE_CREATE 1
#const global LN_FILEMODE_APPEND 2
#const global LN_FILEMODE__TERMINATOR 3

#const global LN_FILEACCESS_READ 0
#const global LN_FILEACCESS_READWRITE 1
#const global LN_FILEACCESS_WRITE 2
#const global LN_FILEACCESS__TERMINATOR 3

#const global LN_SEEK_BEGIN 0
#const global LN_SEEK_CURRENT 1
#const global LN_SEEK_END 2

#const global LN_ASYNCIOSTATE_IDLE 0
#const global LN_ASYNCIOSTATE_READY 1
#const global LN_ASYNCIOSTATE_PROCESSING 2
#const global LN_ASYNCIOSTATE_COMPLETED 3
#const global LN_ASYNCIOSTATE_FAILED 4
#const global LN_ASYNCIOSTATE__TERMINATOR 5

#const global LN_ROTATIONORDER_XYZ 0
#const global LN_ROTATIONORDER_XZY 1
#const global LN_ROTATIONORDER_YXZ 2
#const global LN_ROTATIONORDER_YZX 3
#const global LN_ROTATIONORDER_ZXY 4
#const global LN_ROTATIONORDER_ZYX 5
#const global LN_ROTATIONORDER__TERMINATOR 6

#const global LN_KEY_UNKNOWN 0
#const global LN_KEY_A 1
#const global LN_KEY_B 2
#const global LN_KEY_C 3
#const global LN_KEY_D 4
#const global LN_KEY_E 5
#const global LN_KEY_F 6
#const global LN_KEY_G 7
#const global LN_KEY_H 8
#const global LN_KEY_I 9
#const global LN_KEY_J 10
#const global LN_KEY_K 11
#const global LN_KEY_L 12
#const global LN_KEY_M 13
#const global LN_KEY_N 14
#const global LN_KEY_O 15
#const global LN_KEY_P 16
#const global LN_KEY_Q 17
#const global LN_KEY_R 18
#const global LN_KEY_S 19
#const global LN_KEY_T 20
#const global LN_KEY_U 21
#const global LN_KEY_V 22
#const global LN_KEY_W 23
#const global LN_KEY_X 24
#const global LN_KEY_Y 25
#const global LN_KEY_Z 26
#const global LN_KEY_D0 27
#const global LN_KEY_D1 28
#const global LN_KEY_D2 29
#const global LN_KEY_D3 30
#const global LN_KEY_D4 31
#const global LN_KEY_D5 32
#const global LN_KEY_D6 33
#const global LN_KEY_D7 34
#const global LN_KEY_D8 35
#const global LN_KEY_D9 36
#const global LN_KEY_F1 37
#const global LN_KEY_F2 38
#const global LN_KEY_F3 39
#const global LN_KEY_F4 40
#const global LN_KEY_F5 41
#const global LN_KEY_F6 42
#const global LN_KEY_F7 43
#const global LN_KEY_F8 44
#const global LN_KEY_F9 45
#const global LN_KEY_F10 46
#const global LN_KEY_F11 47
#const global LN_KEY_F12 48
#const global LN_KEY_SPACE 49
#const global LN_KEY_ESCAPE 50
#const global LN_KEY_UP 51
#const global LN_KEY_DOWN 52
#const global LN_KEY_LEFT 53
#const global LN_KEY_RIGHT 54
#const global LN_KEY_LSHIFT 55
#const global LN_KEY_RSHIFT 56
#const global LN_KEY_LCTRL 57
#const global LN_KEY_RCTRL 58
#const global LN_KEY_LALT 59
#const global LN_KEY_RALT 60
#const global LN_KEY_TAB 61
#const global LN_KEY_ENTER 62
#const global LN_KEY_BACKSPACE 63
#const global LN_KEY_INSERT 64
#const global LN_KEY_DELETE 65
#const global LN_KEY_PAGEUP 66
#const global LN_KEY_PAGEDOWN 67
#const global LN_KEY_HOME 68
#const global LN_KEY_END 69
#const global LN_KEY_COLON 70
#const global LN_KEY_SEMICOLON 71
#const global LN_KEY_COMMA 72
#const global LN_KEY_PERIOD 73
#const global LN_KEY_SLASH 74
#const global LN_KEY_MINUS 75
#const global LN_KEY_BACKSLASH 76
#const global LN_KEY_YEN 77
#const global LN_KEY_CARET 78
#const global LN_KEY_LBRACKET 79
#const global LN_KEY_RBRACKET 80
#const global LN_KEY_ANDROID_PLUS 81
#const global LN_KEY__TERMINATOR 256

#const global LN_ACCELERATORKEY_TOGGLE_FULLSCREEN 0
#const global LN_ACCELERATORKEY_SHOW_DEBUG_STATISTICS 1
#const global LN_ACCELERATORKEY__TERMINATOR 2

#const global LN_DIRECTMUSICMODE_NOT_USE 0
#const global LN_DIRECTMUSICMODE_NORMAL 1
#const global LN_DIRECTMUSICMODE_THREAD_WAIT 2
#const global LN_DIRECTMUSICMODE_THREAD_REQUEST 3
#const global LN_DMINITMODE__TERMINATOR 4

#const global LN_SOUNDPLAYINGMODE_UNKNOWN 0
#const global LN_SOUNDPLAYINGMODE_AUTO 1
#const global LN_SOUNDPLAYINGMODE_ONMEMORY 2
#const global LN_SOUNDPLAYINGMODE_STREAMING 3
#const global LN_SOUNDPLAYINGMODE_MIDI 4
#const global LN_SOUNDPLAYINGTYPE__TERMINATOR 5

#const global LN_SOUNDPLAYINGSTATE_STOPPED 0
#const global LN_SOUNDPLAYINGSTATE_PLAYING 1
#const global LN_SOUNDPLAYINGSTATE_PAUSING 2
#const global LN_SOUNDPLAYINGSTATE__TERMINATOR 3

#const global LN_SOUNDFADEBEHAVIORBEHAVIOR_CONTINUE 0
#const global LN_SOUNDFADEBEHAVIOR_STOP 1
#const global LN_SOUNDFADEBEHAVIOR_STOP_RESET 2
#const global LN_SOUNDFADEBEHAVIOR_PAUSE 3
#const global LN_SOUNDFADEBEHAVIOR_PAUSE_RESET 4
#const global LN_SOUNDFADEBEHAVIOR__TERMINATOR 5

#const global LN_BACKBUFFERRESIZEMODE_SCALING 0
#const global LN_BACKBUFFERRESIZEMODE_SCALING_WITH_LETTER_BOX 1
#const global LN_BACKBUFFERRESIZEMODE_RESIZE 2
#const global LN_BACKBUFFERRESIZEMODE__TERMINATOR 3

#const global LN_FMT_UNKNOWN 0
#const global LN_FMT_A8R8G8B8 1
#const global LN_FMT_X8R8G8B8 2
#const global LN_FMT_A16B16G16R16F 3
#const global LN_FMT_A32B32G32R32F 4
#const global LN_FMT_D24S8 5
#const global LN_FMT__TERMINATOR 6

#const global LN_BLEND_NORMAL 0
#const global LN_BLEND_ALPHA 1
#const global LN_BLEND_ADD 2
#const global LN_BLEND_ADD_ALPHA_DISABLE 3
#const global LN_BLEND_SUB 4
#const global LN_BLEND_SUB_ALPHA_DISABLE 5
#const global LN_BLEND_MUL 6
#const global LN_BLEND_SCREEN 7
#const global LN_BLEND_REVERSE 8
#const global LN_BLEND__TERMINATOR 9

#const global LN_CULL_FRONT 0
#const global LN_CULL_BACK 1
#const global LN_CULL_DOUBLE 2
#const global LN_CULL__TERMINATOR 3

#const global LN_TEXTALIGN_LEFT 0
#const global LN_TEXTALIGN_CENTER 1
#const global LN_TEXTALIGN_RIGHT 2
#const global LN_TEXTALIGN__TERMINATOR 3

#const global LN_PLACEMENTTYPE_NORMAL 0
#const global LN_PLACEMENTTYPE_BILLBOARD 1
#const global LN_PLACEMENTTYPE_Y_FIXED 2
#const global LN_PLACEMENTTYPE__TERMINATOR 3

#const global LN_FRONTAXIS_X 0
#const global LN_FRONTAXIS_Y 1
#const global LN_FRONTAXIS_Z 2
#const global LN_FRONTAXIS_RX 3
#const global LN_FRONTAXIS_RY 4
#const global LN_FRONTAXIS_RZ 5
#const global LN_FRONTAXIS__TERMINATOR 6

#const global LN_MESHCREATEFLAGS_SOFTWARE_SKINNING 0x01
#const global LN_MESHCREATEFLAGS_COLLISION_MESH 0x02
#const global LN_MESHCREATEFLAGS_IGNORE_TEXTURE_NOT_FOUND 0x04
#const global LN_MESHCREATEFLAGS__TERMINATOR 5

#const global LN_CAMERATYPE_POSITION_AND_LOOKAT 0
#const global LN_CAMERATYPE_POSITION_AND_ANGLE 1
#const global LN_CAMERATYPE_TRANSFORM_MATRIX 2
#const global LN_CAMERATYPE__TERMINATOR 3

#const global LN_COORD_SPACE_2D 0
#const global LN_COORD_SPACE_3D 1
#const global LN_COORD__TERMINATOR 2

#const global LN_OBJECTTYPE_SOUND 0
#const global LN_OBJECTTYPE__TERMINATOR 1



#cmd LNVector2 $000A
#cmd LNVector3 $000B
#cmd LNVector4 $000C
#cmd LNMatrix $000D
#cmd LNQuaternion $000E


#cmd LNError_GetLastErrorCode $0064
#cmd LNError_GetLastErrorMessage $0065
#cmd LNObject_Release $0066
#cmd LNObject_AddRef $0067
#cmd LNObject_GetRefCount $0068
#cmd LNConfig_SetApplicationLogEnabled $0069
#cmd LNConfig_SetConsoleEnabled $006A
#cmd LNConfig_RegisterArchive $006B
#cmd LNConfig_SetFileAccessPriority $006C
#cmd LNConfig_SetUserWindowHandle $006D
#cmd LNConfig_SetSoundCacheSize $006E
#cmd LNConfig_SetDirectMusicInitializeMode $006F
#cmd LNConfig_SetDirectMusicReverbLevel $0070
#cmd LNApplication_InitializeAudio $0071
#cmd LNApplication_Terminate $0072
#cmd LNVersion_GetMajor $0073
#cmd LNVersion_GetMinor $0074
#cmd LNVersion_GetRevision $0075
#cmd LNVersion_GetString $0076
#cmd LNVersion_IsAtLeast $0077
#cmd LNVector2_GetLength $0078
#cmd LNVector2_GetSquareLength $0079
#cmd LNVector2_Set $007A
#cmd LNVector2_Normalize $007B
#cmd LNVector2_NormalizeV $007C
#cmd LNVector3_GetLength $007D
#cmd LNVector3_GetSquareLength $007E
#cmd LNVector3_Set $007F
#cmd LNVector3_SetVZ $0080
#cmd LNVector3_Normalize $0081
#cmd LNVector3_NormalizeV $0082
#cmd LNVector3_Dot $0083
#cmd LNVector3_Cross $0084
#cmd LNVector3_Reflect $0085
#cmd LNVector3_Slide $0086
#cmd LNVector3_Lerp $0087
#cmd LNVector3_CatmullRom $0088
#cmd LNVector3_Transform $0089
#cmd LNVector3_TransformCoord $008A
#cmd LNVector4_Set $008B
#cmd LNMatrix_GetRight $008C
#cmd LNMatrix_GetUp $008D
#cmd LNMatrix_GetFront $008E
#cmd LNMatrix_GetPosition $008F
#cmd LNMatrix_Identity $0090
#cmd LNMatrix_Translate $0091
#cmd LNMatrix_TranslateVec3 $0092
#cmd LNMatrix_RotateX $0093
#cmd LNMatrix_RotateY $0094
#cmd LNMatrix_RotateZ $0095
#cmd LNMatrix_Rotate $0096
#cmd LNMatrix_RotateVec3 $0097
#cmd LNMatrix_RotateAxis $0098
#cmd LNMatrix_RotateQuaternion $0099
#cmd LNMatrix_Scale $009A
#cmd LNMatrix_ScaleXYZ $009B
#cmd LNMatrix_ScaleVec3 $009C
#cmd LNMatrix_Multiply $009D
#cmd LNMatrix_Inverse $009E
#cmd LNMatrix_Transpose $009F
#cmd LNMatrix_ViewTransformLH $00A0
#cmd LNMatrix_ViewTransformRH $00A1
#cmd LNMatrix_PerspectiveFovLH $00A2
#cmd LNMatrix_PerspectiveFovRH $00A3
#cmd LNMatrix_OrthoLH $00A4
#cmd LNMatrix_OrthoRH $00A5
#cmd LNMatrix_GetEulerAngles $00A6
#cmd LNMatrix_Decompose $00A7
#cmd LNQuaternion_Set $00A8
#cmd LNQuaternion_Identity $00A9
#cmd LNQuaternion_RotationAxis $00AA
#cmd LNQuaternion_RotationMatrix $00AB
#cmd LNQuaternion_RotationYawPitchRoll $00AC
#cmd LNQuaternion_Normalize $00AD
#cmd LNQuaternion_Conjugate $00AE
#cmd LNQuaternion_Multiply $00AF
#cmd LNQuaternion_Slerp $00B0
#cmd LNAudio_PlayBGM $00B1
#cmd LNAudio_PlayBGMMem $00B2
#cmd LNAudio_StopBGM $00B3
#cmd LNAudio_PlayBGS $00B4
#cmd LNAudio_PlayBGSMem $00B5
#cmd LNAudio_StopBGS $00B6
#cmd LNAudio_PlayME $00B7
#cmd LNAudio_PlayMEMem $00B8
#cmd LNAudio_StopME $00B9
#cmd LNAudio_PlaySE $00BA
#cmd LNAudio_PlaySE3D $00BB
#cmd LNAudio_PlaySE3DXYZ $00BC
#cmd LNAudio_PlaySEMem $00BD
#cmd LNAudio_PlaySE3DMem $00BE
#cmd LNAudio_PlaySE3DMemXYZ $00BF
#cmd LNAudio_StopSE $00C0
#cmd LNAudio_SetBGMVolume $00C1
#cmd LNAudio_SetBGSVolume $00C2
#cmd LNSoundListener_SetPosition $00C3
#cmd LNSoundListener_SetDirection $00C4
#cmd LNSoundListener_SetUpDirection $00C5
#cmd LNSoundListener_SetVelocity $00C6
#cmd LNSoundListener_SetPositionXYZ $00C7
#cmd LNSoundListener_SetDirectionXYZ $00C8
#cmd LNSoundListener_SetUpDirectionXYZ $00C9
#cmd LNSoundListener_SetVelocityXYZ $00CA
#cmd LNSound_Create $00CB
#cmd LNSound_CreateMem $00CC
#cmd LNSound_GetVolume $00CD
#cmd LNSound_SetVolume $00CE
#cmd LNSound_GetPitch $00CF
#cmd LNSound_SetPitch $00D0
#cmd LNSound_IsLoopEnabled $00D1
#cmd LNSound_SetLoopEnabled $00D2
#cmd LNSound_Is3DEnabled $00D3
#cmd LNSound_Set3DEnabled $00D4
#cmd LNSound_GetPlayingMode $00D5
#cmd LNSound_SetPlayingMode $00D6
#cmd LNSound_GetPlayingState $00D7
#cmd LNSound_GetPlayedSamples $00D8
#cmd LNSound_GetTotalSamples $00D9
#cmd LNSound_GetSamplingRate $00DA
#cmd LNSound_SetEmitterPosition $00DB
#cmd LNSound_SetEmitterVelocity $00DC
#cmd LNSound_SetEmitterMaxDistance $00DD
#cmd LNSound_SetLoopRange $00DE
#cmd LNSound_Play $00DF
#cmd LNSound_Stop $00E0
#cmd LNSound_Pause $00E1
#cmd LNSound_Resume $00E2
#cmd LNSound_FadeVolume $00E3
#cmd LNSound_SetEmitterPositionXYZ $00E4
#cmd LNSound_SetEmitterVelocityXYZ $00E5


#endif // __lumino__

