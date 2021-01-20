
#ifndef __lumino__
#define __lumino__

#ifdef LUMINO_DEBUG
    #regcmd "_hsp3cmdinit@4","LuminoHSP3d.dll", 11
#else
    #regcmd "_hsp3cmdinit@4","LuminoHSP3.dll", 11
#endif

#const global LN_TRUE 1
#const global LN_FALSE 0
#const global LN_NULL_HANDLE 0
_ln_return_discard = 0

#const global LN_LOG_LEVEL_UNKNOWN 0
#const global LN_LOG_LEVEL_VERBOSE 1
#const global LN_LOG_LEVEL_DEBUG 2
#const global LN_LOG_LEVEL_INFO 3
#const global LN_LOG_LEVEL_WARNING 4
#const global LN_LOG_LEVEL_ERROR 5
#const global LN_LOG_LEVEL_FATAL 6
#const global LN_LOG_LEVEL_DISBLE 7
#const global LN_ENCODING_TYPE_UNKNOWN 0
#const global LN_ENCODING_TYPE_ASCII 1
#const global LN_ENCODING_TYPE_UTF8 2
#const global LN_ENCODING_TYPE_UTF8N 3
#const global LN_ENCODING_TYPE_SJIS 4
#const global LN_KEYS_UNKNOWN 0
#const global LN_KEYS_A 1
#const global LN_KEYS_B 2
#const global LN_KEYS_C 3
#const global LN_KEYS_D 4
#const global LN_KEYS_E 5
#const global LN_KEYS_F 6
#const global LN_KEYS_G 7
#const global LN_KEYS_H 8
#const global LN_KEYS_I 9
#const global LN_KEYS_J 10
#const global LN_KEYS_K 11
#const global LN_KEYS_L 12
#const global LN_KEYS_M 13
#const global LN_KEYS_N 14
#const global LN_KEYS_O 15
#const global LN_KEYS_P 16
#const global LN_KEYS_Q 17
#const global LN_KEYS_R 18
#const global LN_KEYS_S 19
#const global LN_KEYS_T 20
#const global LN_KEYS_U 21
#const global LN_KEYS_V 22
#const global LN_KEYS_W 23
#const global LN_KEYS_X 24
#const global LN_KEYS_Y 25
#const global LN_KEYS_Z 26
#const global LN_KEYS_D0 27
#const global LN_KEYS_D1 28
#const global LN_KEYS_D2 29
#const global LN_KEYS_D3 30
#const global LN_KEYS_D4 31
#const global LN_KEYS_D5 32
#const global LN_KEYS_D6 33
#const global LN_KEYS_D7 34
#const global LN_KEYS_D8 35
#const global LN_KEYS_D9 36
#const global LN_KEYS_F1 37
#const global LN_KEYS_F2 38
#const global LN_KEYS_F3 39
#const global LN_KEYS_F4 40
#const global LN_KEYS_F5 41
#const global LN_KEYS_F6 42
#const global LN_KEYS_F7 43
#const global LN_KEYS_F8 44
#const global LN_KEYS_F9 45
#const global LN_KEYS_F10 46
#const global LN_KEYS_F11 47
#const global LN_KEYS_F12 48
#const global LN_KEYS_SPACE 49
#const global LN_KEYS_ESCAPE 50
#const global LN_KEYS_UP 51
#const global LN_KEYS_DOWN 52
#const global LN_KEYS_LEFT 53
#const global LN_KEYS_RIGHT 54
#const global LN_KEYS_LSHIFT 55
#const global LN_KEYS_RSHIFT 56
#const global LN_KEYS_LCTRL 57
#const global LN_KEYS_RCTRL 58
#const global LN_KEYS_LALT 59
#const global LN_KEYS_RALT 60
#const global LN_KEYS_TAB 61
#const global LN_KEYS_ENTER 62
#const global LN_KEYS_BACK_SPACE 63
#const global LN_KEYS_INSERT 64
#const global LN_KEYS_DELETE 65
#const global LN_KEYS_PAGE_UP 66
#const global LN_KEYS_PAGE_DOWN 67
#const global LN_KEYS_HOME 68
#const global LN_KEYS_END 69
#const global LN_KEYS_COLON 70
#const global LN_KEYS_SEMICOLON 71
#const global LN_KEYS_COMMA 72
#const global LN_KEYS_PERIOD 73
#const global LN_KEYS_SLASH 74
#const global LN_KEYS_MINUS 75
#const global LN_KEYS_BACKSLASH 76
#const global LN_KEYS_YEN 77
#const global LN_KEYS_CARET 78
#const global LN_KEYS_LBRACKET 79
#const global LN_KEYS_RBRACKET 80
#const global LN_KEYS_ANDROID_PLUS 81
#const global LN_KEYS_TERMINATOR 82
#const global LN_MOUSE_BUTTONS_NONE 0
#const global LN_MOUSE_BUTTONS_LEFT 1
#const global LN_MOUSE_BUTTONS_RIGHT 2
#const global LN_MOUSE_BUTTONS_MIDDLE 3
#const global LN_MOUSE_BUTTONS_X1 4
#const global LN_MOUSE_BUTTONS_X2 5
#const global LN_SOUND_FADE_BEHAVIOR_CONTINUE 0
#const global LN_SOUND_FADE_BEHAVIOR_STOP 1
#const global LN_SOUND_FADE_BEHAVIOR_STOP_RESET 2
#const global LN_SOUND_FADE_BEHAVIOR_PAUSE 3
#const global LN_SOUND_FADE_BEHAVIOR_PAUSE_RESET 4
#const global LN_GRAPHICS_API_DEFAULT 0
#const global LN_GRAPHICS_API_OPEN_GL 1
#const global LN_GRAPHICS_API_VULKAN 2
#const global LN_GRAPHICS_API_DIRECT_X12 3
#const global LN_PIXEL_FORMAT_UNKNOWN 0
#const global LN_PIXEL_FORMAT_A8 1
#const global LN_PIXEL_FORMAT_RGBA8 2
#const global LN_PIXEL_FORMAT_RGB8 3
#const global LN_PIXEL_FORMAT_RGBA32F 4
#const global LN_PIXEL_FORMAT_R32S 5
#const global LN_TEXTURE_FORMAT_UNKNOWN 0
#const global LN_TEXTURE_FORMAT_RGBA8 1
#const global LN_TEXTURE_FORMAT_RGB8 2
#const global LN_TEXTURE_FORMAT_RGBA16F 3
#const global LN_TEXTURE_FORMAT_RGBA32F 4
#const global LN_TEXTURE_FORMAT_R16F 5
#const global LN_TEXTURE_FORMAT_R32F 6
#const global LN_TEXTURE_FORMAT_R32S 7
#const global LN_DEPTH_BUFFER_FORMAT_D24S8 0
#const global LN_SHADING_MODEL_DEFAULT 0
#const global LN_SHADING_MODEL_UNLIT 1
#const global LN_BLEND_MODE_NORMAL 0
#const global LN_BLEND_MODE_ALPHA 1
#const global LN_BLEND_MODE_ADD 2
#const global LN_BLEND_MODE_SUBTRACT 3
#const global LN_BLEND_MODE_MULTIPLY 4
#const global LN_SCENE_CLEAR_MODE_NONE 0
#const global LN_SCENE_CLEAR_MODE_COLOR_AND_DEPTH 1
#const global LN_SCENE_CLEAR_MODE_SKY 2
#const global LN_SCENE_CLEAR_MODE_SKY0 3
#const global LN_SCENE_CLEAR_MODE_SKY_DOME 4
#const global LN_ANIMATION_WRAP_MODE_ONCE 0
#const global LN_ANIMATION_WRAP_MODE_LOOP 1
#const global LN_ANIMATION_WRAP_MODE_ALTERNATE 2
#const global LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT 0
#const global LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT_Y 1
#const global LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION 2
#const global LN_HIERARCHICAL_ANIMATION_MODE_DISABLE_TRANSLATION 3
#const global LN_TANGENT_MODE_LINEAR 0
#const global LN_TANGENT_MODE_TANGENT 1
#const global LN_TANGENT_MODE_AUTO 2
#const global LN_TANGENT_MODE_CONSTANT 3
#const global LN_PARTICLE_EMITTER_SHAPE_TYPE_SPHERE 0
#const global LN_PARTICLE_EMITTER_SHAPE_TYPE_CONE 1
#const global LN_PARTICLE_EMITTER_SHAPE_TYPE_BOX 2
#const global LN_PARTICLE_GEOMETRY_DIRECTION_TO_VIEW 0
#const global LN_PARTICLE_GEOMETRY_DIRECTION_TOP 1
#const global LN_PARTICLE_GEOMETRY_DIRECTION_VERTICAL_BILLBOARD 2
#const global LN_PARTICLE_GEOMETRY_DIRECTION_HORIZONTAL_BILLBOARD 3
#const global LN_LEVEL_TRANSITION_EFFECT_MODE_NONE 0
#const global LN_LEVEL_TRANSITION_EFFECT_MODE_FADE_IN_OUT 1
#const global LN_LEVEL_TRANSITION_EFFECT_MODE_CROSS_FADE 2
#const global LN_UILAYOUT_ORIENTATION_HORIZONTAL 0
#const global LN_UILAYOUT_ORIENTATION_VERTICAL 1
#const global LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL 2
#const global LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL 3
#const global LN_UIVISIBILITY_VISIBLE 0
#const global LN_UIVISIBILITY_HIDDEN 1
#const global LN_UIVISIBILITY_COLLAPSED 2
#const global LN_UICOLOR_HUES_RED 0
#const global LN_UICOLOR_HUES_PINK 1
#const global LN_UICOLOR_HUES_PURPLE 2
#const global LN_UICOLOR_HUES_DEEP_PURPLE 4
#const global LN_UICOLOR_HUES_INDIGO 5
#const global LN_UICOLOR_HUES_BLUE 6
#const global LN_UICOLOR_HUES_LIGHT_BLUE 7
#const global LN_UICOLOR_HUES_CYAN 8
#const global LN_UICOLOR_HUES_TEAL 9
#const global LN_UICOLOR_HUES_GREEN 10
#const global LN_UICOLOR_HUES_LIGHT_GREEN 11
#const global LN_UICOLOR_HUES_LIME 12
#const global LN_UICOLOR_HUES_YELLOW 13
#const global LN_UICOLOR_HUES_AMBER 14
#const global LN_UICOLOR_HUES_ORANGE 15
#const global LN_UICOLOR_HUES_DEEP_ORANGE 16
#const global LN_UICOLOR_HUES_BROWN 17
#const global LN_UICOLOR_HUES_GREY 18
#const global LN_UICOLOR_HUES_BLACK 19
#const global LN_UICOLOR_HUES_WHITE 20
#const global LN_UIVALIGNMENT_TOP 0
#const global LN_UIVALIGNMENT_CENTER 1
#const global LN_UIVALIGNMENT_BOTTOM 2
#const global LN_UIVALIGNMENT_STRETCH 3
#const global LN_UIHALIGNMENT_LEFT 0
#const global LN_UIHALIGNMENT_CENTER 1
#const global LN_UIHALIGNMENT_RIGHT 2
#const global LN_UIHALIGNMENT_STRETCH 3
#const global LN_UIINLINE_PLACEMENT_TOP_LEFT 0
#const global LN_UIINLINE_PLACEMENT_TOP 1
#const global LN_UIINLINE_PLACEMENT_TOP_RIGHT 2
#const global LN_UIINLINE_PLACEMENT_LEFT 3
#const global LN_UIINLINE_PLACEMENT_CENTER 4
#const global LN_UIINLINE_PLACEMENT_RIGHT 5
#const global LN_UIINLINE_PLACEMENT_BOTTOM_LEFT 6
#const global LN_UIINLINE_PLACEMENT_BOTTOM 7
#const global LN_UIINLINE_PLACEMENT_BOTTOM_RIGHT 8
#const global LN_UILIST_SUBMIT_MODE_SINGLE 0
#const global LN_UILIST_SUBMIT_MODE_DOUBLE 1

#cmd LNVector3 $35
#cmd LNVector3_GetX $5AF
#cmd LNVector3_SetX $5B1
#cmd LNVector3_GetY $5B2
#cmd LNVector3_SetY $5B4
#cmd LNVector3_GetZ $5B5
#cmd LNVector3_SetZ $5B7
#cmd LNVector3_SetZeros $39
#cmd LNVector3_Set $3A
#cmd LNVector3_Get $3E
#cmd LNVector3_Length $42
#cmd LNVector3_LengthSquared $43
#cmd LNVector3_MutatingNormalize $44
#cmd LNVector3_NormalizeXYZ $45
#cmd LNVector3_Normalize $49
#cmd LNVector4 $4B
#cmd LNVector4_GetX $5CE
#cmd LNVector4_SetX $5D0
#cmd LNVector4_GetY $5D1
#cmd LNVector4_SetY $5D3
#cmd LNVector4_GetZ $5D4
#cmd LNVector4_SetZ $5D6
#cmd LNVector4_GetW $5D7
#cmd LNVector4_SetW $5D9
#cmd LNVector4_SetZeros $50
#cmd LNVector4_Set $51
#cmd LNQuaternion $56
#cmd LNQuaternion_GetX $5EC
#cmd LNQuaternion_SetX $5EE
#cmd LNQuaternion_GetY $5EF
#cmd LNQuaternion_SetY $5F1
#cmd LNQuaternion_GetZ $5F2
#cmd LNQuaternion_SetZ $5F4
#cmd LNQuaternion_GetW $5F5
#cmd LNQuaternion_SetW $5F7
#cmd LNQuaternion_SetZeros $5B
#cmd LNQuaternion_Set $5C
#cmd LNQuaternion_SetFromAxis $61
#cmd LNMatrix $64
#cmd LNMatrix_GetRow0 $60B
#cmd LNMatrix_SetRow0 $60D
#cmd LNMatrix_GetRow1 $60E
#cmd LNMatrix_SetRow1 $610
#cmd LNMatrix_GetRow2 $611
#cmd LNMatrix_SetRow2 $613
#cmd LNMatrix_GetRow3 $614
#cmd LNMatrix_SetRow3 $616
#cmd LNMatrix_SetZeros $69
#cmd LNMatrix_Set $6A
#cmd LNColor $18C
#cmd LNColor_GetR $6AA
#cmd LNColor_SetR $6AC
#cmd LNColor_GetG $6AD
#cmd LNColor_SetG $6AF
#cmd LNColor_GetB $6B0
#cmd LNColor_SetB $6B2
#cmd LNColor_GetA $6B3
#cmd LNColor_SetA $6B5
#cmd LNColor_SetZeros $191
#cmd LNColor_Set $192
#cmd LNColorTone $197
#cmd LNColorTone_GetR $6C9
#cmd LNColorTone_SetR $6CB
#cmd LNColorTone_GetG $6CC
#cmd LNColorTone_SetG $6CE
#cmd LNColorTone_GetB $6CF
#cmd LNColorTone_SetB $6D1
#cmd LNColorTone_GetS $6D2
#cmd LNColorTone_SetS $6D4
#cmd LNColorTone_SetZeros $19C
#cmd LNColorTone_Set $19D
#cmd LNPoint $1A2
#cmd LNPoint_GetX $6E7
#cmd LNPoint_SetX $6E9
#cmd LNPoint_GetY $6EA
#cmd LNPoint_SetY $6EC
#cmd LNPoint_SetZeros $1A5
#cmd LNPoint_Set $1A6
#cmd LNPoint_Get $1A9
#cmd LNSize $1AC
#cmd LNSize_GetWidth $6F8
#cmd LNSize_SetWidth $6FA
#cmd LNSize_GetHeight $6FB
#cmd LNSize_SetHeight $6FD
#cmd LNSize_SetZeros $1AF
#cmd LNSize_Set $1B0
#cmd LNRect $1B3
#cmd LNRect_GetX $708
#cmd LNRect_SetX $70A
#cmd LNRect_GetY $70B
#cmd LNRect_SetY $70D
#cmd LNRect_GetWidth $70E
#cmd LNRect_SetWidth $710
#cmd LNRect_GetHeight $711
#cmd LNRect_SetHeight $713
#cmd LNRect_SetZeros $1B8
#cmd LNRect_Set $1B9
#cmd LNRect_GetLeft $1BE
#cmd LNRect_SetSize $1BF
#cmd LNRect_GetSize $1C1
#cmd LNThickness $1C2
#cmd LNThickness_GetLeft $72C
#cmd LNThickness_SetLeft $72E
#cmd LNThickness_GetTop $72F
#cmd LNThickness_SetTop $731
#cmd LNThickness_GetRight $732
#cmd LNThickness_SetRight $734
#cmd LNThickness_GetBottom $735
#cmd LNThickness_SetBottom $737
#cmd LNThickness_SetZeros $1C7
#cmd LNThickness_Set $1C8
#cmd LNCornerRadius $1CD
#cmd LNCornerRadius_GetTopleft $74A
#cmd LNCornerRadius_SetTopleft $74C
#cmd LNCornerRadius_GetTopright $74D
#cmd LNCornerRadius_SetTopright $74F
#cmd LNCornerRadius_GetBottomright $750
#cmd LNCornerRadius_SetBottomright $752
#cmd LNCornerRadius_GetBottomleft $753
#cmd LNCornerRadius_SetBottomleft $755
#cmd LNCornerRadius_SetZeros $1D2
#cmd LNCornerRadius_Set $1D3

#cmd _LNObject_Release $84
#define LNObject_Release(%1) _LNObject_Release %1
#cmd _LNObject_Retain $85
#define LNObject_Retain(%1) _LNObject_Retain %1
#cmd _LNObject_GetReferenceCount $86
#define LNObject_GetReferenceCount(%1, %2=_ln_return_discard) _LNObject_GetReferenceCount %1, %2
#cmd LNObject_SetPrototype_OnSerialize $A28
#cmd LNEventConnection_SetPrototype_OnSerialize $A33
#cmd _LNPromiseFailureDelegate_Create $631
#define LNPromiseFailureDelegate_Create(%1, %2=_ln_return_discard) _LNPromiseFailureDelegate_Create %1, %2
#cmd _LNVariant_Create $8B
#define LNVariant_Create(%1=_ln_return_discard) _LNVariant_Create %1
#cmd _LNVariant_SetInt $8C
#define LNVariant_SetInt(%1, %2) _LNVariant_SetInt %1, %2
#cmd _LNVariant_GetInt $8E
#define LNVariant_GetInt(%1, %2=_ln_return_discard) _LNVariant_GetInt %1, %2
#cmd LNVariant_SetPrototype_OnSerialize $A3E
#cmd _LNZVTestDelegate1_Create $63A
#define LNZVTestDelegate1_Create(%1, %2=_ln_return_discard) _LNZVTestDelegate1_Create %1, %2
#cmd _LNZVTestDelegate2_Create $640
#define LNZVTestDelegate2_Create(%1, %2=_ln_return_discard) _LNZVTestDelegate2_Create %1, %2
#cmd _LNZVTestDelegate3_Create $645
#define LNZVTestDelegate3_Create(%1, %2=_ln_return_discard) _LNZVTestDelegate3_Create %1, %2
#cmd _LNZVTestEventHandler1_Create $64A
#define LNZVTestEventHandler1_Create(%1, %2=_ln_return_discard) _LNZVTestEventHandler1_Create %1, %2
#cmd _LNZVTestEventHandler2_Create $64F
#define LNZVTestEventHandler2_Create(%1, %2=_ln_return_discard) _LNZVTestEventHandler2_Create %1, %2
#cmd _LNZVTestPromise1_ThenWith $652
#define LNZVTestPromise1_ThenWith(%1, %2) _LNZVTestPromise1_ThenWith %1, %2
#cmd _LNZVTestPromise1_CatchWith $654
#define LNZVTestPromise1_CatchWith(%1, %2) _LNZVTestPromise1_CatchWith %1, %2
#cmd _LNZVTestPromise2_ThenWith $658
#define LNZVTestPromise2_ThenWith(%1, %2) _LNZVTestPromise2_ThenWith %1, %2
#cmd _LNZVTestPromise2_CatchWith $65A
#define LNZVTestPromise2_CatchWith(%1, %2) _LNZVTestPromise2_CatchWith %1, %2
#cmd _LNZVTestClass1_Create $B7
#define LNZVTestClass1_Create(%1=_ln_return_discard) _LNZVTestClass1_Create %1
#cmd _LNZVTestClass1_SetTestDelegate1 $A1
#define LNZVTestClass1_SetTestDelegate1(%1, %2) _LNZVTestClass1_SetTestDelegate1 %1, %2
#cmd _LNZVTestClass1_SetTestDelegate2 $A3
#define LNZVTestClass1_SetTestDelegate2(%1, %2) _LNZVTestClass1_SetTestDelegate2 %1, %2
#cmd _LNZVTestClass1_SetTestDelegate3 $A5
#define LNZVTestClass1_SetTestDelegate3(%1, %2) _LNZVTestClass1_SetTestDelegate3 %1, %2
#cmd _LNZVTestClass1_CallTestDelegate1 $A7
#define LNZVTestClass1_CallTestDelegate1(%1, %2) _LNZVTestClass1_CallTestDelegate1 %1, %2
#cmd _LNZVTestClass1_CallTestDelegate2 $A9
#define LNZVTestClass1_CallTestDelegate2(%1, %2, %3, %4=_ln_return_discard) _LNZVTestClass1_CallTestDelegate2 %1, %2, %3, %4
#cmd _LNZVTestClass1_CallTestDelegate3 $AC
#define LNZVTestClass1_CallTestDelegate3(%1) _LNZVTestClass1_CallTestDelegate3 %1
#cmd _LNZVTestClass1_LoadAsync $AD
#define LNZVTestClass1_LoadAsync(%1, %2=_ln_return_discard) _LNZVTestClass1_LoadAsync %1, %2
#cmd _LNZVTestClass1_ExecuteAsync $AF
#define LNZVTestClass1_ExecuteAsync(%1, %2=_ln_return_discard) _LNZVTestClass1_ExecuteAsync %1, %2
#cmd _LNZVTestClass1_GetFilePath $B0
#define LNZVTestClass1_GetFilePath(%1, %2=_ln_return_discard) _LNZVTestClass1_GetFilePath %1, %2
#cmd _LNZVTestClass1_ConnectOnEvent1 $B1
#define LNZVTestClass1_ConnectOnEvent1(%1, %2, %3=_ln_return_discard) _LNZVTestClass1_ConnectOnEvent1 %1, %2, %3
#cmd _LNZVTestClass1_RaiseEvent1 $B3
#define LNZVTestClass1_RaiseEvent1(%1) _LNZVTestClass1_RaiseEvent1 %1
#cmd _LNZVTestClass1_ConnectOnEvent2 $B4
#define LNZVTestClass1_ConnectOnEvent2(%1, %2, %3=_ln_return_discard) _LNZVTestClass1_ConnectOnEvent2 %1, %2, %3
#cmd _LNZVTestClass1_RaiseEvent2 $B6
#define LNZVTestClass1_RaiseEvent2(%1) _LNZVTestClass1_RaiseEvent2 %1
#cmd LNZVTestClass1_SetPrototype_OnSerialize $A49
#cmd _LNZVTestEventArgs1_Create $BA
#define LNZVTestEventArgs1_Create(%1=_ln_return_discard) _LNZVTestEventArgs1_Create %1
#cmd _LNZVTestEventArgs1_CreateWithValue $BB
#define LNZVTestEventArgs1_CreateWithValue(%1, %2=_ln_return_discard) _LNZVTestEventArgs1_CreateWithValue %1, %2
#cmd _LNZVTestEventArgs1_GetValue $B9
#define LNZVTestEventArgs1_GetValue(%1, %2=_ln_return_discard) _LNZVTestEventArgs1_GetValue %1, %2
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $A54
#cmd _LNLog_SetLevel $BE
#define LNLog_SetLevel(%1) _LNLog_SetLevel %1
#cmd _LNLog_AllocConsole $C0
#define LNLog_AllocConsole _LNLog_AllocConsole
#cmd _LNLog_Write $C1
#define LNLog_Write(%1, %2, %3) _LNLog_Write %1, %2, %3
#cmd LNSerializer2_SetPrototype_OnSerialize $A5F
#cmd LNAssetObject_SetPrototype_OnSerialize $A6A
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $A75
#cmd _LNAssetModel_Create $125
#define LNAssetModel_Create(%1, %2=_ln_return_discard) _LNAssetModel_Create %1, %2
#cmd _LNAssetModel_Target $124
#define LNAssetModel_Target(%1, %2=_ln_return_discard) _LNAssetModel_Target %1, %2
#cmd LNAssetModel_SetPrototype_OnSerialize $A80
#cmd _LNAssets_SaveAssetToLocalFile $128
#define LNAssets_SaveAssetToLocalFile(%1, %2) _LNAssets_SaveAssetToLocalFile %1, %2
#cmd _LNAssets_LoadAssetFromLocalFile $12B
#define LNAssets_LoadAssetFromLocalFile(%1, %2=_ln_return_discard) _LNAssets_LoadAssetFromLocalFile %1, %2
#cmd _LNAssets_LoadAsset $12D
#define LNAssets_LoadAsset(%1, %2=_ln_return_discard) _LNAssets_LoadAsset %1, %2
#cmd _LNAssets_ReloadAsset $12F
#define LNAssets_ReloadAsset(%1, %2) _LNAssets_ReloadAsset %1, %2
#cmd _LNAssets_ReadAllText $132
#define LNAssets_ReadAllText(%1, %2, %3=_ln_return_discard) _LNAssets_ReadAllText %1, %2, %3
#cmd _LNSound_SetVolume $13C
#define LNSound_SetVolume(%1, %2) _LNSound_SetVolume %1, %2
#cmd _LNSound_GetVolume $13E
#define LNSound_GetVolume(%1, %2=_ln_return_discard) _LNSound_GetVolume %1, %2
#cmd _LNSound_SetPitch $13F
#define LNSound_SetPitch(%1, %2) _LNSound_SetPitch %1, %2
#cmd _LNSound_GetPitch $141
#define LNSound_GetPitch(%1, %2=_ln_return_discard) _LNSound_GetPitch %1, %2
#cmd _LNSound_SetLoopEnabled $142
#define LNSound_SetLoopEnabled(%1, %2) _LNSound_SetLoopEnabled %1, %2
#cmd _LNSound_IsLoopEnabled $144
#define LNSound_IsLoopEnabled(%1, %2=_ln_return_discard) _LNSound_IsLoopEnabled %1, %2
#cmd _LNSound_SetLoopRange $145
#define LNSound_SetLoopRange(%1, %2, %3) _LNSound_SetLoopRange %1, %2, %3
#cmd _LNSound_Play $148
#define LNSound_Play(%1) _LNSound_Play %1
#cmd _LNSound_Stop $149
#define LNSound_Stop(%1) _LNSound_Stop %1
#cmd _LNSound_Pause $14A
#define LNSound_Pause(%1) _LNSound_Pause %1
#cmd _LNSound_Resume $14B
#define LNSound_Resume(%1) _LNSound_Resume %1
#cmd _LNSound_FadeVolume $14C
#define LNSound_FadeVolume(%1, %2, %3, %4) _LNSound_FadeVolume %1, %2, %3, %4
#cmd LNSound_SetPrototype_OnSerialize $A8B
#cmd _LNAudio_PlayBGM $151
#define LNAudio_PlayBGM(%1, %2, %3, %4) _LNAudio_PlayBGM %1, %2, %3, %4
#cmd _LNAudio_StopBGM $156
#define LNAudio_StopBGM(%1) _LNAudio_StopBGM %1
#cmd _LNAudio_PlayBGS $158
#define LNAudio_PlayBGS(%1, %2, %3, %4) _LNAudio_PlayBGS %1, %2, %3, %4
#cmd _LNAudio_StopBGS $15D
#define LNAudio_StopBGS(%1) _LNAudio_StopBGS %1
#cmd _LNAudio_PlayME $15F
#define LNAudio_PlayME(%1, %2, %3) _LNAudio_PlayME %1, %2, %3
#cmd _LNAudio_StopME $163
#define LNAudio_StopME _LNAudio_StopME
#cmd _LNAudio_PlaySE $164
#define LNAudio_PlaySE(%1, %2, %3) _LNAudio_PlaySE %1, %2, %3
#cmd _LNAudio_PlaySE3D $168
#define LNAudio_PlaySE3D(%1, %2, %3, %4, %5) _LNAudio_PlaySE3D %1, %2, %3, %4, %5
#cmd _LNAudio_StopSE $16E
#define LNAudio_StopSE _LNAudio_StopSE
#cmd _LNTexture2DDelegate_Create $6A0
#define LNTexture2DDelegate_Create(%1, %2=_ln_return_discard) _LNTexture2DDelegate_Create %1, %2
#cmd _LNTexture2DPromise_ThenWith $6A3
#define LNTexture2DPromise_ThenWith(%1, %2) _LNTexture2DPromise_ThenWith %1, %2
#cmd _LNTexture2DPromise_CatchWith $6A5
#define LNTexture2DPromise_CatchWith(%1, %2) _LNTexture2DPromise_CatchWith %1, %2
#cmd _LNGraphics_GetActiveGraphicsAPI $18B
#define LNGraphics_GetActiveGraphicsAPI(%1=_ln_return_discard) _LNGraphics_GetActiveGraphicsAPI %1
#cmd LNTexture_SetPrototype_OnSerialize $A96
#cmd _LNTexture2D_Create $1DE
#define LNTexture2D_Create(%1, %2, %3=_ln_return_discard) _LNTexture2D_Create %1, %2, %3
#cmd _LNTexture2D_CreateWithFormat $1E1
#define LNTexture2D_CreateWithFormat(%1, %2, %3, %4=_ln_return_discard) _LNTexture2D_CreateWithFormat %1, %2, %3, %4
#cmd _LNTexture2D_Load $1DA
#define LNTexture2D_Load(%1, %2=_ln_return_discard) _LNTexture2D_Load %1, %2
#cmd _LNTexture2D_LoadEmoji $1DC
#define LNTexture2D_LoadEmoji(%1, %2=_ln_return_discard) _LNTexture2D_LoadEmoji %1, %2
#cmd LNTexture2D_SetPrototype_OnSerialize $AA1
#cmd _LNShader_Load $1E6
#define LNShader_Load(%1, %2, %3=_ln_return_discard) _LNShader_Load %1, %2, %3
#cmd _LNShader_SetFloat $1E9
#define LNShader_SetFloat(%1, %2, %3) _LNShader_SetFloat %1, %2, %3
#cmd _LNShader_SetVector3 $1EC
#define LNShader_SetVector3(%1, %2, %3) _LNShader_SetVector3 %1, %2, %3
#cmd _LNShader_SetVector4 $1EF
#define LNShader_SetVector4(%1, %2, %3) _LNShader_SetVector4 %1, %2, %3
#cmd _LNShader_SetTexture $1F2
#define LNShader_SetTexture(%1, %2, %3) _LNShader_SetTexture %1, %2, %3
#cmd LNShader_SetPrototype_OnSerialize $AAC
#cmd LNRenderView_SetPrototype_OnSerialize $AB7
#cmd _LNMaterial_Create $217
#define LNMaterial_Create(%1=_ln_return_discard) _LNMaterial_Create %1
#cmd _LNMaterial_SetMainTexture $206
#define LNMaterial_SetMainTexture(%1, %2) _LNMaterial_SetMainTexture %1, %2
#cmd _LNMaterial_GetMainTexture $208
#define LNMaterial_GetMainTexture(%1, %2=_ln_return_discard) _LNMaterial_GetMainTexture %1, %2
#cmd _LNMaterial_SetColor $209
#define LNMaterial_SetColor(%1, %2) _LNMaterial_SetColor %1, %2
#cmd _LNMaterial_SetRoughness $20B
#define LNMaterial_SetRoughness(%1, %2) _LNMaterial_SetRoughness %1, %2
#cmd _LNMaterial_SetMetallic $20D
#define LNMaterial_SetMetallic(%1, %2) _LNMaterial_SetMetallic %1, %2
#cmd _LNMaterial_SetEmissive $20F
#define LNMaterial_SetEmissive(%1, %2) _LNMaterial_SetEmissive %1, %2
#cmd _LNMaterial_SetShadingModel $211
#define LNMaterial_SetShadingModel(%1, %2) _LNMaterial_SetShadingModel %1, %2
#cmd _LNMaterial_GetShadingModel $213
#define LNMaterial_GetShadingModel(%1, %2=_ln_return_discard) _LNMaterial_GetShadingModel %1, %2
#cmd _LNMaterial_SetShader $214
#define LNMaterial_SetShader(%1, %2) _LNMaterial_SetShader %1, %2
#cmd _LNMaterial_GetShader $216
#define LNMaterial_GetShader(%1, %2=_ln_return_discard) _LNMaterial_GetShader %1, %2
#cmd LNMaterial_SetPrototype_OnSerialize $AC2
#cmd _LNMeshNode_SetVisible $219
#define LNMeshNode_SetVisible(%1, %2) _LNMeshNode_SetVisible %1, %2
#cmd _LNMeshNode_IsVisible $21B
#define LNMeshNode_IsVisible(%1, %2=_ln_return_discard) _LNMeshNode_IsVisible %1, %2
#cmd LNMeshNode_SetPrototype_OnSerialize $ACD
#cmd _LNAnimationController_AddClip $21D
#define LNAnimationController_AddClip(%1, %2, %3=_ln_return_discard) _LNAnimationController_AddClip %1, %2, %3
#cmd _LNAnimationController_Play $21F
#define LNAnimationController_Play(%1, %2, %3) _LNAnimationController_Play %1, %2, %3
#cmd LNAnimationController_SetPrototype_OnSerialize $AD8
#cmd _LNMeshModel_Load $223
#define LNMeshModel_Load(%1, %2, %3=_ln_return_discard) _LNMeshModel_Load %1, %2, %3
#cmd _LNMeshModel_FindNode $226
#define LNMeshModel_FindNode(%1, %2, %3=_ln_return_discard) _LNMeshModel_FindNode %1, %2, %3
#cmd _LNMeshModel_FindMaterial $228
#define LNMeshModel_FindMaterial(%1, %2, %3=_ln_return_discard) _LNMeshModel_FindMaterial %1, %2, %3
#cmd _LNMeshModel_MaterialCount $22A
#define LNMeshModel_MaterialCount(%1, %2=_ln_return_discard) _LNMeshModel_MaterialCount %1, %2
#cmd _LNMeshModel_Material $22B
#define LNMeshModel_Material(%1, %2, %3=_ln_return_discard) _LNMeshModel_Material %1, %2, %3
#cmd _LNMeshModel_GetAnimationController $22D
#define LNMeshModel_GetAnimationController(%1, %2=_ln_return_discard) _LNMeshModel_GetAnimationController %1, %2
#cmd LNMeshModel_SetPrototype_OnSerialize $AE3
#cmd _LNMeshImportSettings_Create $22F
#define LNMeshImportSettings_Create(%1=_ln_return_discard) _LNMeshImportSettings_Create %1
#cmd LNMeshImportSettings_SetPrototype_OnSerialize $AEE
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $AF9
#cmd LNCollisionShape_SetPrototype_OnSerialize $B04
#cmd _LNBoxCollisionShape_Create $233
#define LNBoxCollisionShape_Create(%1, %2=_ln_return_discard) _LNBoxCollisionShape_Create %1, %2
#cmd _LNBoxCollisionShape_CreateWHD $235
#define LNBoxCollisionShape_CreateWHD(%1, %2, %3, %4=_ln_return_discard) _LNBoxCollisionShape_CreateWHD %1, %2, %3, %4
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $B0F
#cmd _LNAnimationCurve_Evaluate $243
#define LNAnimationCurve_Evaluate(%1, %2, %3=_ln_return_discard) _LNAnimationCurve_Evaluate %1, %2, %3
#cmd LNAnimationCurve_SetPrototype_OnSerialize $B1A
#cmd _LNKeyFrameAnimationCurve_Create $250
#define LNKeyFrameAnimationCurve_Create(%1=_ln_return_discard) _LNKeyFrameAnimationCurve_Create %1
#cmd _LNKeyFrameAnimationCurve_AddKeyFrame $24B
#define LNKeyFrameAnimationCurve_AddKeyFrame(%1, %2, %3, %4, %5) _LNKeyFrameAnimationCurve_AddKeyFrame %1, %2, %3, %4, %5
#cmd LNKeyFrameAnimationCurve_SetPrototype_OnSerialize $B25
#cmd _LNAnimationClip_Load $252
#define LNAnimationClip_Load(%1, %2=_ln_return_discard) _LNAnimationClip_Load %1, %2
#cmd _LNAnimationClip_SetWrapMode $254
#define LNAnimationClip_SetWrapMode(%1, %2) _LNAnimationClip_SetWrapMode %1, %2
#cmd _LNAnimationClip_GetWrapMode $256
#define LNAnimationClip_GetWrapMode(%1, %2=_ln_return_discard) _LNAnimationClip_GetWrapMode %1, %2
#cmd _LNAnimationClip_SetHierarchicalAnimationMode $257
#define LNAnimationClip_SetHierarchicalAnimationMode(%1, %2) _LNAnimationClip_SetHierarchicalAnimationMode %1, %2
#cmd _LNAnimationClip_GetHierarchicalAnimationMode $259
#define LNAnimationClip_GetHierarchicalAnimationMode(%1, %2=_ln_return_discard) _LNAnimationClip_GetHierarchicalAnimationMode %1, %2
#cmd LNAnimationClip_SetPrototype_OnSerialize $B30
#cmd LNAnimationState_SetPrototype_OnSerialize $B3B
#cmd LNEffectResource_SetPrototype_OnSerialize $B46
#cmd _LNParticleEmitterModel_Create $27E
#define LNParticleEmitterModel_Create(%1=_ln_return_discard) _LNParticleEmitterModel_Create %1
#cmd _LNParticleEmitterModel_SetMaxParticles $266
#define LNParticleEmitterModel_SetMaxParticles(%1, %2) _LNParticleEmitterModel_SetMaxParticles %1, %2
#cmd _LNParticleEmitterModel_SetSpawnRate $268
#define LNParticleEmitterModel_SetSpawnRate(%1, %2) _LNParticleEmitterModel_SetSpawnRate %1, %2
#cmd _LNParticleEmitterModel_SetLifeTime $26A
#define LNParticleEmitterModel_SetLifeTime(%1, %2) _LNParticleEmitterModel_SetLifeTime %1, %2
#cmd _LNParticleEmitterModel_SetupBoxShape $26C
#define LNParticleEmitterModel_SetupBoxShape(%1, %2) _LNParticleEmitterModel_SetupBoxShape %1, %2
#cmd _LNParticleEmitterModel_SetSize $26E
#define LNParticleEmitterModel_SetSize(%1, %2) _LNParticleEmitterModel_SetSize %1, %2
#cmd _LNParticleEmitterModel_SetSizeVelocity $270
#define LNParticleEmitterModel_SetSizeVelocity(%1, %2) _LNParticleEmitterModel_SetSizeVelocity %1, %2
#cmd _LNParticleEmitterModel_SetSizeAcceleration $272
#define LNParticleEmitterModel_SetSizeAcceleration(%1, %2) _LNParticleEmitterModel_SetSizeAcceleration %1, %2
#cmd _LNParticleEmitterModel_SetForwardVelocityMin $274
#define LNParticleEmitterModel_SetForwardVelocityMin(%1, %2) _LNParticleEmitterModel_SetForwardVelocityMin %1, %2
#cmd _LNParticleEmitterModel_SetForwardVelocityMax $276
#define LNParticleEmitterModel_SetForwardVelocityMax(%1, %2) _LNParticleEmitterModel_SetForwardVelocityMax %1, %2
#cmd _LNParticleEmitterModel_SetForwardScale $278
#define LNParticleEmitterModel_SetForwardScale(%1, %2) _LNParticleEmitterModel_SetForwardScale %1, %2
#cmd _LNParticleEmitterModel_SetGeometryDirection $27A
#define LNParticleEmitterModel_SetGeometryDirection(%1, %2) _LNParticleEmitterModel_SetGeometryDirection %1, %2
#cmd _LNParticleEmitterModel_SetupSpriteModule $27C
#define LNParticleEmitterModel_SetupSpriteModule(%1, %2) _LNParticleEmitterModel_SetupSpriteModule %1, %2
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $B51
#cmd _LNParticleModel_Create $285
#define LNParticleModel_Create(%1=_ln_return_discard) _LNParticleModel_Create %1
#cmd _LNParticleModel_SetLoop $280
#define LNParticleModel_SetLoop(%1, %2) _LNParticleModel_SetLoop %1, %2
#cmd _LNParticleModel_IsLoop $282
#define LNParticleModel_IsLoop(%1, %2=_ln_return_discard) _LNParticleModel_IsLoop %1, %2
#cmd _LNParticleModel_AddEmitter $283
#define LNParticleModel_AddEmitter(%1, %2) _LNParticleModel_AddEmitter %1, %2
#cmd LNParticleModel_SetPrototype_OnSerialize $B5C
#cmd LNComponent_SetPrototype_OnSerialize $B67
#cmd _LNVisualComponent_SetVisible $288
#define LNVisualComponent_SetVisible(%1, %2) _LNVisualComponent_SetVisible %1, %2
#cmd _LNVisualComponent_IsVisible $28A
#define LNVisualComponent_IsVisible(%1, %2=_ln_return_discard) _LNVisualComponent_IsVisible %1, %2
#cmd LNVisualComponent_SetPrototype_OnSerialize $B72
#cmd _LNSpriteComponent_SetTexture $28C
#define LNSpriteComponent_SetTexture(%1, %2) _LNSpriteComponent_SetTexture %1, %2
#cmd LNSpriteComponent_SetPrototype_OnSerialize $B7D
#cmd _LNCollisionEventHandler_Create $7D8
#define LNCollisionEventHandler_Create(%1, %2=_ln_return_discard) _LNCollisionEventHandler_Create %1, %2
#cmd _LNCharacterController_Create $2AC
#define LNCharacterController_Create(%1=_ln_return_discard) _LNCharacterController_Create %1
#cmd _LNCharacterController_SetWalkVelocity $296
#define LNCharacterController_SetWalkVelocity(%1, %2) _LNCharacterController_SetWalkVelocity %1, %2
#cmd _LNCharacterController_GetWalkVelocity $298
#define LNCharacterController_GetWalkVelocity(%1, %2=_ln_return_discard) _LNCharacterController_GetWalkVelocity %1, %2
#cmd _LNCharacterController_SetVelocity $299
#define LNCharacterController_SetVelocity(%1, %2) _LNCharacterController_SetVelocity %1, %2
#cmd _LNCharacterController_GetVelocity $29B
#define LNCharacterController_GetVelocity(%1, %2=_ln_return_discard) _LNCharacterController_GetVelocity %1, %2
#cmd _LNCharacterController_SetInputControlEnabled $29C
#define LNCharacterController_SetInputControlEnabled(%1, %2) _LNCharacterController_SetInputControlEnabled %1, %2
#cmd _LNCharacterController_SetCameraControlEnabled $29E
#define LNCharacterController_SetCameraControlEnabled(%1, %2) _LNCharacterController_SetCameraControlEnabled %1, %2
#cmd _LNCharacterController_SetHeight $2A0
#define LNCharacterController_SetHeight(%1, %2) _LNCharacterController_SetHeight %1, %2
#cmd _LNCharacterController_GetHeight $2A2
#define LNCharacterController_GetHeight(%1, %2=_ln_return_discard) _LNCharacterController_GetHeight %1, %2
#cmd _LNCharacterController_SetCameraRadius $2A3
#define LNCharacterController_SetCameraRadius(%1, %2) _LNCharacterController_SetCameraRadius %1, %2
#cmd _LNCharacterController_GetCameraRadius $2A5
#define LNCharacterController_GetCameraRadius(%1, %2=_ln_return_discard) _LNCharacterController_GetCameraRadius %1, %2
#cmd _LNCharacterController_SetCollisionEnter $2A6
#define LNCharacterController_SetCollisionEnter(%1, %2) _LNCharacterController_SetCollisionEnter %1, %2
#cmd _LNCharacterController_SetCollisionLeave $2A8
#define LNCharacterController_SetCollisionLeave(%1, %2) _LNCharacterController_SetCollisionLeave %1, %2
#cmd _LNCharacterController_SetCollisionStay $2AA
#define LNCharacterController_SetCollisionStay(%1, %2) _LNCharacterController_SetCollisionStay %1, %2
#cmd LNCharacterController_SetPrototype_OnSerialize $B88
#cmd _LNWorld_Add $2AE
#define LNWorld_Add(%1, %2) _LNWorld_Add %1, %2
#cmd LNWorld_SetPrototype_OnSerialize $B93
#cmd _LNComponentList_GetLength $7F8
#define LNComponentList_GetLength(%1, %2=_ln_return_discard) _LNComponentList_GetLength %1, %2
#cmd _LNComponentList_GetItem $7FA
#define LNComponentList_GetItem(%1, %2, %3=_ln_return_discard) _LNComponentList_GetItem %1, %2, %3
#cmd LNComponentList_SetPrototype_OnSerialize $B9E
#cmd _LNWorldObject_Create $2E5
#define LNWorldObject_Create(%1=_ln_return_discard) _LNWorldObject_Create %1
#cmd _LNWorldObject_SetPosition $2B2
#define LNWorldObject_SetPosition(%1, %2) _LNWorldObject_SetPosition %1, %2
#cmd _LNWorldObject_SetPositionXYZ $2B4
#define LNWorldObject_SetPositionXYZ(%1, %2, %3, %4) _LNWorldObject_SetPositionXYZ %1, %2, %3, %4
#cmd _LNWorldObject_GetPosition $2B8
#define LNWorldObject_GetPosition(%1, %2=_ln_return_discard) _LNWorldObject_GetPosition %1, %2
#cmd _LNWorldObject_SetRotationQuaternion $2B9
#define LNWorldObject_SetRotationQuaternion(%1, %2) _LNWorldObject_SetRotationQuaternion %1, %2
#cmd _LNWorldObject_SetRotation $2BB
#define LNWorldObject_SetRotation(%1, %2, %3, %4) _LNWorldObject_SetRotation %1, %2, %3, %4
#cmd _LNWorldObject_GetRotation $2BF
#define LNWorldObject_GetRotation(%1, %2=_ln_return_discard) _LNWorldObject_GetRotation %1, %2
#cmd _LNWorldObject_SetScale $2C0
#define LNWorldObject_SetScale(%1, %2) _LNWorldObject_SetScale %1, %2
#cmd _LNWorldObject_SetScaleS $2C2
#define LNWorldObject_SetScaleS(%1, %2) _LNWorldObject_SetScaleS %1, %2
#cmd _LNWorldObject_SetScaleXYZ $2C4
#define LNWorldObject_SetScaleXYZ(%1, %2, %3, %4) _LNWorldObject_SetScaleXYZ %1, %2, %3, %4
#cmd _LNWorldObject_GetScale $2C8
#define LNWorldObject_GetScale(%1, %2=_ln_return_discard) _LNWorldObject_GetScale %1, %2
#cmd _LNWorldObject_SetCenterPoint $2C9
#define LNWorldObject_SetCenterPoint(%1, %2) _LNWorldObject_SetCenterPoint %1, %2
#cmd _LNWorldObject_SetCenterPointXYZ $2CB
#define LNWorldObject_SetCenterPointXYZ(%1, %2, %3, %4) _LNWorldObject_SetCenterPointXYZ %1, %2, %3, %4
#cmd _LNWorldObject_GetCenterPoint $2CF
#define LNWorldObject_GetCenterPoint(%1, %2=_ln_return_discard) _LNWorldObject_GetCenterPoint %1, %2
#cmd _LNWorldObject_LookAt $2D0
#define LNWorldObject_LookAt(%1, %2) _LNWorldObject_LookAt %1, %2
#cmd _LNWorldObject_LookAtXYZ $2D2
#define LNWorldObject_LookAtXYZ(%1, %2, %3, %4) _LNWorldObject_LookAtXYZ %1, %2, %3, %4
#cmd _LNWorldObject_AddComponent $2D6
#define LNWorldObject_AddComponent(%1, %2) _LNWorldObject_AddComponent %1, %2
#cmd _LNWorldObject_RemoveComponent $2D8
#define LNWorldObject_RemoveComponent(%1, %2) _LNWorldObject_RemoveComponent %1, %2
#cmd _LNWorldObject_AddTag $2DA
#define LNWorldObject_AddTag(%1, %2) _LNWorldObject_AddTag %1, %2
#cmd _LNWorldObject_RemoveTag $2DC
#define LNWorldObject_RemoveTag(%1, %2) _LNWorldObject_RemoveTag %1, %2
#cmd _LNWorldObject_HasTag $2DE
#define LNWorldObject_HasTag(%1, %2, %3=_ln_return_discard) _LNWorldObject_HasTag %1, %2, %3
#cmd _LNWorldObject_Destroy $2E0
#define LNWorldObject_Destroy(%1) _LNWorldObject_Destroy %1
#cmd _LNWorldObject_GetComponents $2E1
#define LNWorldObject_GetComponents(%1, %2=_ln_return_discard) _LNWorldObject_GetComponents %1, %2
#cmd LNWorldObject_SetPrototype_OnSerialize $BA9
#cmd LNWorldObject_SetPrototype_OnPreUpdate $BB4
#cmd LNWorldObject_SetPrototype_OnUpdate $BBF
#cmd _LNVisualObject_SetVisible $2E7
#define LNVisualObject_SetVisible(%1, %2) _LNVisualObject_SetVisible %1, %2
#cmd _LNVisualObject_IsVisible $2E9
#define LNVisualObject_IsVisible(%1, %2=_ln_return_discard) _LNVisualObject_IsVisible %1, %2
#cmd _LNVisualObject_SetBlendMode2 $2EA
#define LNVisualObject_SetBlendMode2(%1, %2) _LNVisualObject_SetBlendMode2 %1, %2
#cmd _LNVisualObject_SetOpacity $2EC
#define LNVisualObject_SetOpacity(%1, %2) _LNVisualObject_SetOpacity %1, %2
#cmd _LNVisualObject_GetOpacity $2EE
#define LNVisualObject_GetOpacity(%1, %2=_ln_return_discard) _LNVisualObject_GetOpacity %1, %2
#cmd LNVisualObject_SetPrototype_OnSerialize $BCA
#cmd LNVisualObject_SetPrototype_OnPreUpdate $BD5
#cmd LNVisualObject_SetPrototype_OnUpdate $BE0
#cmd LNCamera_SetPrototype_OnSerialize $BEB
#cmd LNCamera_SetPrototype_OnPreUpdate $BF6
#cmd LNCamera_SetPrototype_OnUpdate $C01
#cmd _LNEnvironmentLight_SetEnabled $2F1
#define LNEnvironmentLight_SetEnabled(%1, %2) _LNEnvironmentLight_SetEnabled %1, %2
#cmd _LNEnvironmentLight_IsEnabled $2F3
#define LNEnvironmentLight_IsEnabled(%1, %2=_ln_return_discard) _LNEnvironmentLight_IsEnabled %1, %2
#cmd _LNEnvironmentLight_SetColor $2F4
#define LNEnvironmentLight_SetColor(%1, %2) _LNEnvironmentLight_SetColor %1, %2
#cmd _LNEnvironmentLight_GetColor $2F6
#define LNEnvironmentLight_GetColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetColor %1, %2
#cmd _LNEnvironmentLight_SetAmbientColor $2F7
#define LNEnvironmentLight_SetAmbientColor(%1, %2) _LNEnvironmentLight_SetAmbientColor %1, %2
#cmd _LNEnvironmentLight_GetAmbientColor $2F9
#define LNEnvironmentLight_GetAmbientColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetAmbientColor %1, %2
#cmd _LNEnvironmentLight_GetSkyColor $2FA
#define LNEnvironmentLight_GetSkyColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetSkyColor %1, %2
#cmd _LNEnvironmentLight_SetSkyColor $2FB
#define LNEnvironmentLight_SetSkyColor(%1, %2) _LNEnvironmentLight_SetSkyColor %1, %2
#cmd _LNEnvironmentLight_GetGroundColor $2FD
#define LNEnvironmentLight_GetGroundColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetGroundColor %1, %2
#cmd _LNEnvironmentLight_SetGroundColor $2FE
#define LNEnvironmentLight_SetGroundColor(%1, %2) _LNEnvironmentLight_SetGroundColor %1, %2
#cmd _LNEnvironmentLight_SetIntensity $300
#define LNEnvironmentLight_SetIntensity(%1, %2) _LNEnvironmentLight_SetIntensity %1, %2
#cmd _LNEnvironmentLight_GetIntensity $302
#define LNEnvironmentLight_GetIntensity(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetIntensity %1, %2
#cmd _LNEnvironmentLight_SetShadowEffectiveDistance $303
#define LNEnvironmentLight_SetShadowEffectiveDistance(%1, %2) _LNEnvironmentLight_SetShadowEffectiveDistance %1, %2
#cmd _LNEnvironmentLight_GetShadowEffectiveDistance $305
#define LNEnvironmentLight_GetShadowEffectiveDistance(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetShadowEffectiveDistance %1, %2
#cmd _LNEnvironmentLight_SetShadowEffectiveDepth $306
#define LNEnvironmentLight_SetShadowEffectiveDepth(%1, %2) _LNEnvironmentLight_SetShadowEffectiveDepth %1, %2
#cmd _LNEnvironmentLight_GetShadowEffectiveDepth $308
#define LNEnvironmentLight_GetShadowEffectiveDepth(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetShadowEffectiveDepth %1, %2
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $C0C
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $C17
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $C22
#cmd _LNDirectionalLight_Create $319
#define LNDirectionalLight_Create(%1=_ln_return_discard) _LNDirectionalLight_Create %1
#cmd _LNDirectionalLight_CreateWithColor $31A
#define LNDirectionalLight_CreateWithColor(%1, %2=_ln_return_discard) _LNDirectionalLight_CreateWithColor %1, %2
#cmd _LNDirectionalLight_SetEnabled $30A
#define LNDirectionalLight_SetEnabled(%1, %2) _LNDirectionalLight_SetEnabled %1, %2
#cmd _LNDirectionalLight_IsEnabled $30C
#define LNDirectionalLight_IsEnabled(%1, %2=_ln_return_discard) _LNDirectionalLight_IsEnabled %1, %2
#cmd _LNDirectionalLight_SetColor $30D
#define LNDirectionalLight_SetColor(%1, %2) _LNDirectionalLight_SetColor %1, %2
#cmd _LNDirectionalLight_GetColor $30F
#define LNDirectionalLight_GetColor(%1, %2=_ln_return_discard) _LNDirectionalLight_GetColor %1, %2
#cmd _LNDirectionalLight_SetIntensity $310
#define LNDirectionalLight_SetIntensity(%1, %2) _LNDirectionalLight_SetIntensity %1, %2
#cmd _LNDirectionalLight_GetIntensity $312
#define LNDirectionalLight_GetIntensity(%1, %2=_ln_return_discard) _LNDirectionalLight_GetIntensity %1, %2
#cmd _LNDirectionalLight_SetShadowEffectiveDistance $313
#define LNDirectionalLight_SetShadowEffectiveDistance(%1, %2) _LNDirectionalLight_SetShadowEffectiveDistance %1, %2
#cmd _LNDirectionalLight_GetShadowEffectiveDistance $315
#define LNDirectionalLight_GetShadowEffectiveDistance(%1, %2=_ln_return_discard) _LNDirectionalLight_GetShadowEffectiveDistance %1, %2
#cmd _LNDirectionalLight_SetShadowEffectiveDepth $316
#define LNDirectionalLight_SetShadowEffectiveDepth(%1, %2) _LNDirectionalLight_SetShadowEffectiveDepth %1, %2
#cmd _LNDirectionalLight_GetShadowEffectiveDepth $318
#define LNDirectionalLight_GetShadowEffectiveDepth(%1, %2=_ln_return_discard) _LNDirectionalLight_GetShadowEffectiveDepth %1, %2
#cmd LNDirectionalLight_SetPrototype_OnSerialize $C2D
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $C38
#cmd LNDirectionalLight_SetPrototype_OnUpdate $C43
#cmd _LNPointLight_Create $32C
#define LNPointLight_Create(%1=_ln_return_discard) _LNPointLight_Create %1
#cmd _LNPointLight_CreateWithColorAndRange $32D
#define LNPointLight_CreateWithColorAndRange(%1, %2, %3=_ln_return_discard) _LNPointLight_CreateWithColorAndRange %1, %2, %3
#cmd _LNPointLight_SetEnabled $31D
#define LNPointLight_SetEnabled(%1, %2) _LNPointLight_SetEnabled %1, %2
#cmd _LNPointLight_IsEnabled $31F
#define LNPointLight_IsEnabled(%1, %2=_ln_return_discard) _LNPointLight_IsEnabled %1, %2
#cmd _LNPointLight_SetColor $320
#define LNPointLight_SetColor(%1, %2) _LNPointLight_SetColor %1, %2
#cmd _LNPointLight_GetColor $322
#define LNPointLight_GetColor(%1, %2=_ln_return_discard) _LNPointLight_GetColor %1, %2
#cmd _LNPointLight_SetIntensity $323
#define LNPointLight_SetIntensity(%1, %2) _LNPointLight_SetIntensity %1, %2
#cmd _LNPointLight_GetIntensity $325
#define LNPointLight_GetIntensity(%1, %2=_ln_return_discard) _LNPointLight_GetIntensity %1, %2
#cmd _LNPointLight_SetRange $326
#define LNPointLight_SetRange(%1, %2) _LNPointLight_SetRange %1, %2
#cmd _LNPointLight_GetRange $328
#define LNPointLight_GetRange(%1, %2=_ln_return_discard) _LNPointLight_GetRange %1, %2
#cmd _LNPointLight_SetAttenuation $329
#define LNPointLight_SetAttenuation(%1, %2) _LNPointLight_SetAttenuation %1, %2
#cmd _LNPointLight_GetAttenuation $32B
#define LNPointLight_GetAttenuation(%1, %2=_ln_return_discard) _LNPointLight_GetAttenuation %1, %2
#cmd LNPointLight_SetPrototype_OnSerialize $C4E
#cmd LNPointLight_SetPrototype_OnPreUpdate $C59
#cmd LNPointLight_SetPrototype_OnUpdate $C64
#cmd _LNSpotLight_Create $346
#define LNSpotLight_Create(%1=_ln_return_discard) _LNSpotLight_Create %1
#cmd _LNSpotLight_CreateWithColorAndRange $347
#define LNSpotLight_CreateWithColorAndRange(%1, %2, %3, %4=_ln_return_discard) _LNSpotLight_CreateWithColorAndRange %1, %2, %3, %4
#cmd _LNSpotLight_SetEnabled $331
#define LNSpotLight_SetEnabled(%1, %2) _LNSpotLight_SetEnabled %1, %2
#cmd _LNSpotLight_IsEnabled $333
#define LNSpotLight_IsEnabled(%1, %2=_ln_return_discard) _LNSpotLight_IsEnabled %1, %2
#cmd _LNSpotLight_SetColor $334
#define LNSpotLight_SetColor(%1, %2) _LNSpotLight_SetColor %1, %2
#cmd _LNSpotLight_GetColor $336
#define LNSpotLight_GetColor(%1, %2=_ln_return_discard) _LNSpotLight_GetColor %1, %2
#cmd _LNSpotLight_SetIntensity $337
#define LNSpotLight_SetIntensity(%1, %2) _LNSpotLight_SetIntensity %1, %2
#cmd _LNSpotLight_GetIntensity $339
#define LNSpotLight_GetIntensity(%1, %2=_ln_return_discard) _LNSpotLight_GetIntensity %1, %2
#cmd _LNSpotLight_SetRange $33A
#define LNSpotLight_SetRange(%1, %2) _LNSpotLight_SetRange %1, %2
#cmd _LNSpotLight_GetRange $33C
#define LNSpotLight_GetRange(%1, %2=_ln_return_discard) _LNSpotLight_GetRange %1, %2
#cmd _LNSpotLight_SetAttenuation $33D
#define LNSpotLight_SetAttenuation(%1, %2) _LNSpotLight_SetAttenuation %1, %2
#cmd _LNSpotLight_GetAttenuation $33F
#define LNSpotLight_GetAttenuation(%1, %2=_ln_return_discard) _LNSpotLight_GetAttenuation %1, %2
#cmd _LNSpotLight_SetAngle $340
#define LNSpotLight_SetAngle(%1, %2) _LNSpotLight_SetAngle %1, %2
#cmd _LNSpotLight_GetAngle $342
#define LNSpotLight_GetAngle(%1, %2=_ln_return_discard) _LNSpotLight_GetAngle %1, %2
#cmd _LNSpotLight_SetPenumbra $343
#define LNSpotLight_SetPenumbra(%1, %2) _LNSpotLight_SetPenumbra %1, %2
#cmd _LNSpotLight_GetPenumbra $345
#define LNSpotLight_GetPenumbra(%1, %2=_ln_return_discard) _LNSpotLight_GetPenumbra %1, %2
#cmd LNSpotLight_SetPrototype_OnSerialize $C6F
#cmd LNSpotLight_SetPrototype_OnPreUpdate $C7A
#cmd LNSpotLight_SetPrototype_OnUpdate $C85
#cmd _LNTestDelegate_Create $8A6
#define LNTestDelegate_Create(%1, %2=_ln_return_discard) _LNTestDelegate_Create %1, %2
#cmd _LNSprite_Create $35D
#define LNSprite_Create(%1=_ln_return_discard) _LNSprite_Create %1
#cmd _LNSprite_CreateWithTexture $35E
#define LNSprite_CreateWithTexture(%1, %2=_ln_return_discard) _LNSprite_CreateWithTexture %1, %2
#cmd _LNSprite_CreateWithTextureAndSize $360
#define LNSprite_CreateWithTextureAndSize(%1, %2, %3, %4=_ln_return_discard) _LNSprite_CreateWithTextureAndSize %1, %2, %3, %4
#cmd _LNSprite_SetTexture $34F
#define LNSprite_SetTexture(%1, %2) _LNSprite_SetTexture %1, %2
#cmd _LNSprite_SetSize $351
#define LNSprite_SetSize(%1, %2) _LNSprite_SetSize %1, %2
#cmd _LNSprite_SetSizeWH $353
#define LNSprite_SetSizeWH(%1, %2, %3) _LNSprite_SetSizeWH %1, %2, %3
#cmd _LNSprite_SetSourceRectXYWH $356
#define LNSprite_SetSourceRectXYWH(%1, %2, %3, %4, %5) _LNSprite_SetSourceRectXYWH %1, %2, %3, %4, %5
#cmd _LNSprite_SetCallerTest $35B
#define LNSprite_SetCallerTest(%1, %2) _LNSprite_SetCallerTest %1, %2
#cmd LNSprite_SetPrototype_OnSerialize $C90
#cmd LNSprite_SetPrototype_OnPreUpdate $C9B
#cmd LNSprite_SetPrototype_OnUpdate $CA6
#cmd _LNCameraOrbitControlComponent_Create $365
#define LNCameraOrbitControlComponent_Create(%1=_ln_return_discard) _LNCameraOrbitControlComponent_Create %1
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $CB1
#cmd _LNRaycaster_FromScreen $367
#define LNRaycaster_FromScreen(%1, %2=_ln_return_discard) _LNRaycaster_FromScreen %1, %2
#cmd _LNRaycaster_IntersectPlane $369
#define LNRaycaster_IntersectPlane(%1, %2, %3, %4, %5=_ln_return_discard) _LNRaycaster_IntersectPlane %1, %2, %3, %4, %5
#cmd LNRaycaster_SetPrototype_OnSerialize $CBC
#cmd _LNRaycastResult_GetPoint $36E
#define LNRaycastResult_GetPoint(%1, %2=_ln_return_discard) _LNRaycastResult_GetPoint %1, %2
#cmd LNRaycastResult_SetPrototype_OnSerialize $CC7
#cmd _LNWorldRenderView_SetGuideGridEnabled $370
#define LNWorldRenderView_SetGuideGridEnabled(%1, %2) _LNWorldRenderView_SetGuideGridEnabled %1, %2
#cmd _LNWorldRenderView_GetGuideGridEnabled $372
#define LNWorldRenderView_GetGuideGridEnabled(%1, %2=_ln_return_discard) _LNWorldRenderView_GetGuideGridEnabled %1, %2
#cmd LNWorldRenderView_SetPrototype_OnSerialize $CD2
#cmd _LNBoxMesh_Create $374
#define LNBoxMesh_Create(%1=_ln_return_discard) _LNBoxMesh_Create %1
#cmd _LNBoxMesh_CreateXYZ $375
#define LNBoxMesh_CreateXYZ(%1, %2, %3, %4=_ln_return_discard) _LNBoxMesh_CreateXYZ %1, %2, %3, %4
#cmd LNBoxMesh_SetPrototype_OnSerialize $CDD
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $CE8
#cmd LNBoxMesh_SetPrototype_OnUpdate $CF3
#cmd _LNPlaneMesh_Create $37A
#define LNPlaneMesh_Create(%1=_ln_return_discard) _LNPlaneMesh_Create %1
#cmd LNPlaneMesh_SetPrototype_OnSerialize $CFE
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $D09
#cmd LNPlaneMesh_SetPrototype_OnUpdate $D14
#cmd _LNMeshComponent_Create $380
#define LNMeshComponent_Create(%1=_ln_return_discard) _LNMeshComponent_Create %1
#cmd _LNMeshComponent_SetModel $37C
#define LNMeshComponent_SetModel(%1, %2) _LNMeshComponent_SetModel %1, %2
#cmd _LNMeshComponent_MakeCollisionBody $37E
#define LNMeshComponent_MakeCollisionBody(%1, %2) _LNMeshComponent_MakeCollisionBody %1, %2
#cmd LNMeshComponent_SetPrototype_OnSerialize $D1F
#cmd _LNCollision_GetWorldObject $382
#define LNCollision_GetWorldObject(%1, %2=_ln_return_discard) _LNCollision_GetWorldObject %1, %2
#cmd LNCollision_SetPrototype_OnSerialize $D2A
#cmd _LNTriggerBodyComponent_Create $386
#define LNTriggerBodyComponent_Create(%1=_ln_return_discard) _LNTriggerBodyComponent_Create %1
#cmd _LNTriggerBodyComponent_AddCollisionShape $384
#define LNTriggerBodyComponent_AddCollisionShape(%1, %2) _LNTriggerBodyComponent_AddCollisionShape %1, %2
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $D35
#cmd _LNParticleEmitter_Create $388
#define LNParticleEmitter_Create(%1, %2=_ln_return_discard) _LNParticleEmitter_Create %1, %2
#cmd LNParticleEmitter_SetPrototype_OnSerialize $D40
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $D4B
#cmd LNParticleEmitter_SetPrototype_OnUpdate $D56
#cmd _LNScene_SetClearMode $38B
#define LNScene_SetClearMode(%1) _LNScene_SetClearMode %1
#cmd _LNScene_SetSkyColor $38D
#define LNScene_SetSkyColor(%1) _LNScene_SetSkyColor %1
#cmd _LNScene_SetSkyHorizonColor $38F
#define LNScene_SetSkyHorizonColor(%1) _LNScene_SetSkyHorizonColor %1
#cmd _LNScene_SetSkyCloudColor $391
#define LNScene_SetSkyCloudColor(%1) _LNScene_SetSkyCloudColor %1
#cmd _LNScene_SetSkyOverlayColor $393
#define LNScene_SetSkyOverlayColor(%1) _LNScene_SetSkyOverlayColor %1
#cmd _LNScene_GotoLevel $395
#define LNScene_GotoLevel(%1, %2) _LNScene_GotoLevel %1, %2
#cmd _LNScene_CallLevel $398
#define LNScene_CallLevel(%1, %2) _LNScene_CallLevel %1, %2
#cmd _LNScene_ReturnLevel $39B
#define LNScene_ReturnLevel(%1) _LNScene_ReturnLevel %1
#cmd _LNScene_ActiveLevel $39D
#define LNScene_ActiveLevel(%1=_ln_return_discard) _LNScene_ActiveLevel %1
#cmd _LNScene_IsTransitionEffectRunning $39E
#define LNScene_IsTransitionEffectRunning(%1=_ln_return_discard) _LNScene_IsTransitionEffectRunning %1
#cmd _LNScene_SetTransitionEffectMode $39F
#define LNScene_SetTransitionEffectMode(%1) _LNScene_SetTransitionEffectMode %1
#cmd _LNScene_TransitionEffectMode $3A1
#define LNScene_TransitionEffectMode(%1=_ln_return_discard) _LNScene_TransitionEffectMode %1
#cmd _LNScene_SetTransitionDuration $3A2
#define LNScene_SetTransitionDuration(%1) _LNScene_SetTransitionDuration %1
#cmd _LNScene_TransitionDuration $3A4
#define LNScene_TransitionDuration(%1=_ln_return_discard) _LNScene_TransitionDuration %1
#cmd _LNScene_SetTransitionEffectColor $3A5
#define LNScene_SetTransitionEffectColor(%1) _LNScene_SetTransitionEffectColor %1
#cmd _LNScene_TransitionEffectColor $3A7
#define LNScene_TransitionEffectColor(%1=_ln_return_discard) _LNScene_TransitionEffectColor %1
#cmd _LNScene_SetTransitionEffectMaskTexture $3A8
#define LNScene_SetTransitionEffectMaskTexture(%1) _LNScene_SetTransitionEffectMaskTexture %1
#cmd _LNScene_TransitionEffectMaskTexture $3AA
#define LNScene_TransitionEffectMaskTexture(%1=_ln_return_discard) _LNScene_TransitionEffectMaskTexture %1
#cmd _LNScene_SetTransitionEffectVague $3AB
#define LNScene_SetTransitionEffectVague(%1) _LNScene_SetTransitionEffectVague %1
#cmd _LNScene_TransitionEffectVague $3AD
#define LNScene_TransitionEffectVague(%1=_ln_return_discard) _LNScene_TransitionEffectVague %1
#cmd _LNScene_StartFadeOut $3AE
#define LNScene_StartFadeOut _LNScene_StartFadeOut
#cmd _LNScene_StartFadeIn $3AF
#define LNScene_StartFadeIn _LNScene_StartFadeIn
#cmd _LNScene_SetFogStartDistance $3B0
#define LNScene_SetFogStartDistance(%1) _LNScene_SetFogStartDistance %1
#cmd _LNScene_SetFogColor $3B2
#define LNScene_SetFogColor(%1) _LNScene_SetFogColor %1
#cmd _LNScene_SetFogDensity $3B4
#define LNScene_SetFogDensity(%1) _LNScene_SetFogDensity %1
#cmd _LNScene_SetFogHeightDensity $3B6
#define LNScene_SetFogHeightDensity(%1) _LNScene_SetFogHeightDensity %1
#cmd _LNScene_SetFogLowerHeight $3B8
#define LNScene_SetFogLowerHeight(%1) _LNScene_SetFogLowerHeight %1
#cmd _LNScene_SetFogUpperHeight $3BA
#define LNScene_SetFogUpperHeight(%1) _LNScene_SetFogUpperHeight %1
#cmd _LNScene_SetHDREnabled $3BC
#define LNScene_SetHDREnabled(%1) _LNScene_SetHDREnabled %1
#cmd _LNScene_IsHDREnabled $3BE
#define LNScene_IsHDREnabled(%1=_ln_return_discard) _LNScene_IsHDREnabled %1
#cmd _LNScene_SetScreenBlendColor $3BF
#define LNScene_SetScreenBlendColor(%1) _LNScene_SetScreenBlendColor %1
#cmd _LNScene_ScreenBlendColor $3C1
#define LNScene_ScreenBlendColor(%1=_ln_return_discard) _LNScene_ScreenBlendColor %1
#cmd _LNScene_SetColorTone $3C2
#define LNScene_SetColorTone(%1) _LNScene_SetColorTone %1
#cmd _LNScene_ColorTone $3C4
#define LNScene_ColorTone(%1=_ln_return_discard) _LNScene_ColorTone %1
#cmd _LNScene_SetAntialiasEnabled $3C5
#define LNScene_SetAntialiasEnabled(%1) _LNScene_SetAntialiasEnabled %1
#cmd _LNScene_IsAntialiasEnabled $3C7
#define LNScene_IsAntialiasEnabled(%1=_ln_return_discard) _LNScene_IsAntialiasEnabled %1
#cmd _LNScene_SetSSREnabled $3C8
#define LNScene_SetSSREnabled(%1) _LNScene_SetSSREnabled %1
#cmd _LNScene_IsSSREnabled $3CA
#define LNScene_IsSSREnabled(%1=_ln_return_discard) _LNScene_IsSSREnabled %1
#cmd _LNScene_SetSSAOEnabled $3CB
#define LNScene_SetSSAOEnabled(%1) _LNScene_SetSSAOEnabled %1
#cmd _LNScene_IsSSAOEnabled $3CD
#define LNScene_IsSSAOEnabled(%1=_ln_return_discard) _LNScene_IsSSAOEnabled %1
#cmd _LNScene_SetBloomEnabled $3CE
#define LNScene_SetBloomEnabled(%1) _LNScene_SetBloomEnabled %1
#cmd _LNScene_IsBloomEnabled $3D0
#define LNScene_IsBloomEnabled(%1=_ln_return_discard) _LNScene_IsBloomEnabled %1
#cmd _LNScene_SetDOFEnabled $3D1
#define LNScene_SetDOFEnabled(%1) _LNScene_SetDOFEnabled %1
#cmd _LNScene_IsDOFEnabled $3D3
#define LNScene_IsDOFEnabled(%1=_ln_return_discard) _LNScene_IsDOFEnabled %1
#cmd _LNScene_SetTonemapEnabled $3D4
#define LNScene_SetTonemapEnabled(%1) _LNScene_SetTonemapEnabled %1
#cmd _LNScene_IsTonemapEnabled $3D6
#define LNScene_IsTonemapEnabled(%1=_ln_return_discard) _LNScene_IsTonemapEnabled %1
#cmd _LNScene_SetVignetteEnabled $3D7
#define LNScene_SetVignetteEnabled(%1) _LNScene_SetVignetteEnabled %1
#cmd _LNScene_IsVignetteEnabled $3D9
#define LNScene_IsVignetteEnabled(%1=_ln_return_discard) _LNScene_IsVignetteEnabled %1
#cmd _LNScene_SetGammaEnabled $3DA
#define LNScene_SetGammaEnabled(%1) _LNScene_SetGammaEnabled %1
#cmd _LNScene_IsGammaEnabled $3DC
#define LNScene_IsGammaEnabled(%1=_ln_return_discard) _LNScene_IsGammaEnabled %1
#cmd _LNScene_SetTonemapExposure $3DD
#define LNScene_SetTonemapExposure(%1) _LNScene_SetTonemapExposure %1
#cmd _LNScene_SetTonemapLinearWhite $3DF
#define LNScene_SetTonemapLinearWhite(%1) _LNScene_SetTonemapLinearWhite %1
#cmd _LNScene_SetTonemapShoulderStrength $3E1
#define LNScene_SetTonemapShoulderStrength(%1) _LNScene_SetTonemapShoulderStrength %1
#cmd _LNScene_SetTonemapLinearStrength $3E3
#define LNScene_SetTonemapLinearStrength(%1) _LNScene_SetTonemapLinearStrength %1
#cmd _LNScene_SetTonemapLinearAngle $3E5
#define LNScene_SetTonemapLinearAngle(%1) _LNScene_SetTonemapLinearAngle %1
#cmd _LNScene_SetTonemapToeStrength $3E7
#define LNScene_SetTonemapToeStrength(%1) _LNScene_SetTonemapToeStrength %1
#cmd _LNScene_SetTonemapToeNumerator $3E9
#define LNScene_SetTonemapToeNumerator(%1) _LNScene_SetTonemapToeNumerator %1
#cmd _LNScene_SetTonemapToeDenominator $3EB
#define LNScene_SetTonemapToeDenominator(%1) _LNScene_SetTonemapToeDenominator %1
#cmd _LNLevel_Create $3FC
#define LNLevel_Create(%1=_ln_return_discard) _LNLevel_Create %1
#cmd _LNLevel_AddObject $3EE
#define LNLevel_AddObject(%1, %2) _LNLevel_AddObject %1, %2
#cmd _LNLevel_RemoveObject $3F0
#define LNLevel_RemoveObject(%1, %2) _LNLevel_RemoveObject %1, %2
#cmd _LNLevel_AddSubLevel $3F2
#define LNLevel_AddSubLevel(%1, %2) _LNLevel_AddSubLevel %1, %2
#cmd _LNLevel_RemoveSubLevel $3F4
#define LNLevel_RemoveSubLevel(%1, %2) _LNLevel_RemoveSubLevel %1, %2
#cmd _LNLevel_RemoveAllSubLevels $3F6
#define LNLevel_RemoveAllSubLevels(%1) _LNLevel_RemoveAllSubLevels %1
#cmd LNLevel_SetPrototype_OnSerialize $D61
#cmd LNLevel_SetPrototype_OnStart $D6C
#cmd LNLevel_SetPrototype_OnStop $D77
#cmd LNLevel_SetPrototype_OnPause $D82
#cmd LNLevel_SetPrototype_OnResume $D8D
#cmd LNLevel_SetPrototype_OnUpdate $D98
#cmd _LNUIColors_Red $41C
#define LNUIColors_Red(%1, %2=_ln_return_discard) _LNUIColors_Red %1, %2
#cmd _LNUIColors_Pink $41E
#define LNUIColors_Pink(%1, %2=_ln_return_discard) _LNUIColors_Pink %1, %2
#cmd _LNUIColors_Purple $420
#define LNUIColors_Purple(%1, %2=_ln_return_discard) _LNUIColors_Purple %1, %2
#cmd _LNUIColors_DeepPurple $422
#define LNUIColors_DeepPurple(%1, %2=_ln_return_discard) _LNUIColors_DeepPurple %1, %2
#cmd _LNUIColors_Indigo $424
#define LNUIColors_Indigo(%1, %2=_ln_return_discard) _LNUIColors_Indigo %1, %2
#cmd _LNUIColors_Blue $426
#define LNUIColors_Blue(%1, %2=_ln_return_discard) _LNUIColors_Blue %1, %2
#cmd _LNUIColors_LightBlue $428
#define LNUIColors_LightBlue(%1, %2=_ln_return_discard) _LNUIColors_LightBlue %1, %2
#cmd _LNUIColors_Cyan $42A
#define LNUIColors_Cyan(%1, %2=_ln_return_discard) _LNUIColors_Cyan %1, %2
#cmd _LNUIColors_Teal $42C
#define LNUIColors_Teal(%1, %2=_ln_return_discard) _LNUIColors_Teal %1, %2
#cmd _LNUIColors_Green $42E
#define LNUIColors_Green(%1, %2=_ln_return_discard) _LNUIColors_Green %1, %2
#cmd _LNUIColors_LightGreen $430
#define LNUIColors_LightGreen(%1, %2=_ln_return_discard) _LNUIColors_LightGreen %1, %2
#cmd _LNUIColors_Lime $432
#define LNUIColors_Lime(%1, %2=_ln_return_discard) _LNUIColors_Lime %1, %2
#cmd _LNUIColors_Yellow $434
#define LNUIColors_Yellow(%1, %2=_ln_return_discard) _LNUIColors_Yellow %1, %2
#cmd _LNUIColors_Amber $436
#define LNUIColors_Amber(%1, %2=_ln_return_discard) _LNUIColors_Amber %1, %2
#cmd _LNUIColors_Orange $438
#define LNUIColors_Orange(%1, %2=_ln_return_discard) _LNUIColors_Orange %1, %2
#cmd _LNUIColors_DeepOrange $43A
#define LNUIColors_DeepOrange(%1, %2=_ln_return_discard) _LNUIColors_DeepOrange %1, %2
#cmd _LNUIColors_Brown $43C
#define LNUIColors_Brown(%1, %2=_ln_return_discard) _LNUIColors_Brown %1, %2
#cmd _LNUIColors_Grey $43E
#define LNUIColors_Grey(%1, %2=_ln_return_discard) _LNUIColors_Grey %1, %2
#cmd _LNUIColors_Get $440
#define LNUIColors_Get(%1, %2, %3=_ln_return_discard) _LNUIColors_Get %1, %2, %3
#cmd _LNUIEventArgs_Sender $444
#define LNUIEventArgs_Sender(%1, %2=_ln_return_discard) _LNUIEventArgs_Sender %1, %2
#cmd LNUIEventArgs_SetPrototype_OnSerialize $DA3
#cmd _LNUIGeneralEventHandler_Create $925
#define LNUIGeneralEventHandler_Create(%1, %2=_ln_return_discard) _LNUIGeneralEventHandler_Create %1, %2
#cmd _LNUIEventHandler_Create $92A
#define LNUIEventHandler_Create(%1, %2=_ln_return_discard) _LNUIEventHandler_Create %1, %2
#cmd LNUILayoutElement_SetPrototype_OnSerialize $DAE
#cmd _LNUIElement_SetSize $456
#define LNUIElement_SetSize(%1, %2) _LNUIElement_SetSize %1, %2
#cmd _LNUIElement_SetSizeWH $458
#define LNUIElement_SetSizeWH(%1, %2, %3) _LNUIElement_SetSizeWH %1, %2, %3
#cmd _LNUIElement_SetWidth $45B
#define LNUIElement_SetWidth(%1, %2) _LNUIElement_SetWidth %1, %2
#cmd _LNUIElement_GetWidth $45D
#define LNUIElement_GetWidth(%1, %2=_ln_return_discard) _LNUIElement_GetWidth %1, %2
#cmd _LNUIElement_SetHeight $45E
#define LNUIElement_SetHeight(%1, %2) _LNUIElement_SetHeight %1, %2
#cmd _LNUIElement_GetHeight $460
#define LNUIElement_GetHeight(%1, %2=_ln_return_discard) _LNUIElement_GetHeight %1, %2
#cmd _LNUIElement_SetMargin $461
#define LNUIElement_SetMargin(%1, %2) _LNUIElement_SetMargin %1, %2
#cmd _LNUIElement_GetMargin $463
#define LNUIElement_GetMargin(%1, %2=_ln_return_discard) _LNUIElement_GetMargin %1, %2
#cmd _LNUIElement_SetPadding $464
#define LNUIElement_SetPadding(%1, %2) _LNUIElement_SetPadding %1, %2
#cmd _LNUIElement_GetPadding $466
#define LNUIElement_GetPadding(%1, %2=_ln_return_discard) _LNUIElement_GetPadding %1, %2
#cmd _LNUIElement_SetHAlignment $467
#define LNUIElement_SetHAlignment(%1, %2) _LNUIElement_SetHAlignment %1, %2
#cmd _LNUIElement_GetHAlignment $469
#define LNUIElement_GetHAlignment(%1, %2=_ln_return_discard) _LNUIElement_GetHAlignment %1, %2
#cmd _LNUIElement_SetVAlignment $46A
#define LNUIElement_SetVAlignment(%1, %2) _LNUIElement_SetVAlignment %1, %2
#cmd _LNUIElement_GetVAlignment $46C
#define LNUIElement_GetVAlignment(%1, %2=_ln_return_discard) _LNUIElement_GetVAlignment %1, %2
#cmd _LNUIElement_SetAlignments $46D
#define LNUIElement_SetAlignments(%1, %2, %3) _LNUIElement_SetAlignments %1, %2, %3
#cmd _LNUIElement_SetPosition $470
#define LNUIElement_SetPosition(%1, %2) _LNUIElement_SetPosition %1, %2
#cmd _LNUIElement_SetPositionXYZ $472
#define LNUIElement_SetPositionXYZ(%1, %2, %3, %4) _LNUIElement_SetPositionXYZ %1, %2, %3, %4
#cmd _LNUIElement_GetPosition $476
#define LNUIElement_GetPosition(%1, %2=_ln_return_discard) _LNUIElement_GetPosition %1, %2
#cmd _LNUIElement_SetRotation $477
#define LNUIElement_SetRotation(%1, %2) _LNUIElement_SetRotation %1, %2
#cmd _LNUIElement_SetEulerAngles $479
#define LNUIElement_SetEulerAngles(%1, %2, %3, %4) _LNUIElement_SetEulerAngles %1, %2, %3, %4
#cmd _LNUIElement_GetRotation $47D
#define LNUIElement_GetRotation(%1, %2=_ln_return_discard) _LNUIElement_GetRotation %1, %2
#cmd _LNUIElement_SetScale $47E
#define LNUIElement_SetScale(%1, %2) _LNUIElement_SetScale %1, %2
#cmd _LNUIElement_SetScaleS $480
#define LNUIElement_SetScaleS(%1, %2) _LNUIElement_SetScaleS %1, %2
#cmd _LNUIElement_SetScaleXY $482
#define LNUIElement_SetScaleXY(%1, %2, %3) _LNUIElement_SetScaleXY %1, %2, %3
#cmd _LNUIElement_GetScale $485
#define LNUIElement_GetScale(%1, %2=_ln_return_discard) _LNUIElement_GetScale %1, %2
#cmd _LNUIElement_SetCenterPoint $486
#define LNUIElement_SetCenterPoint(%1, %2) _LNUIElement_SetCenterPoint %1, %2
#cmd _LNUIElement_SetCenterPointXYZ $488
#define LNUIElement_SetCenterPointXYZ(%1, %2, %3, %4) _LNUIElement_SetCenterPointXYZ %1, %2, %3, %4
#cmd _LNUIElement_GetCenterPoint $48C
#define LNUIElement_GetCenterPoint(%1, %2=_ln_return_discard) _LNUIElement_GetCenterPoint %1, %2
#cmd _LNUIElement_SetEnabled $48D
#define LNUIElement_SetEnabled(%1, %2) _LNUIElement_SetEnabled %1, %2
#cmd _LNUIElement_IsEnabled $48F
#define LNUIElement_IsEnabled(%1, %2=_ln_return_discard) _LNUIElement_IsEnabled %1, %2
#cmd _LNUIElement_SetData $490
#define LNUIElement_SetData(%1, %2) _LNUIElement_SetData %1, %2
#cmd _LNUIElement_GetData $492
#define LNUIElement_GetData(%1, %2=_ln_return_discard) _LNUIElement_GetData %1, %2
#cmd _LNUIElement_SetBackgroundColor $493
#define LNUIElement_SetBackgroundColor(%1, %2) _LNUIElement_SetBackgroundColor %1, %2
#cmd _LNUIElement_GetBackgroundColor $495
#define LNUIElement_GetBackgroundColor(%1, %2=_ln_return_discard) _LNUIElement_GetBackgroundColor %1, %2
#cmd _LNUIElement_SetBorderThickness $496
#define LNUIElement_SetBorderThickness(%1, %2) _LNUIElement_SetBorderThickness %1, %2
#cmd _LNUIElement_GetBorderThickness $498
#define LNUIElement_GetBorderThickness(%1, %2=_ln_return_discard) _LNUIElement_GetBorderThickness %1, %2
#cmd _LNUIElement_SetBorderColor $499
#define LNUIElement_SetBorderColor(%1, %2) _LNUIElement_SetBorderColor %1, %2
#cmd _LNUIElement_GetBorderColor $49B
#define LNUIElement_GetBorderColor(%1, %2=_ln_return_discard) _LNUIElement_GetBorderColor %1, %2
#cmd _LNUIElement_SetCornerRadius $49C
#define LNUIElement_SetCornerRadius(%1, %2) _LNUIElement_SetCornerRadius %1, %2
#cmd _LNUIElement_GetCornerRadius $49E
#define LNUIElement_GetCornerRadius(%1, %2=_ln_return_discard) _LNUIElement_GetCornerRadius %1, %2
#cmd _LNUIElement_SetVisibility $49F
#define LNUIElement_SetVisibility(%1, %2) _LNUIElement_SetVisibility %1, %2
#cmd _LNUIElement_GetVisibility $4A1
#define LNUIElement_GetVisibility(%1, %2=_ln_return_discard) _LNUIElement_GetVisibility %1, %2
#cmd _LNUIElement_SetOpacity $4A2
#define LNUIElement_SetOpacity(%1, %2) _LNUIElement_SetOpacity %1, %2
#cmd _LNUIElement_GetOpacity $4A4
#define LNUIElement_GetOpacity(%1, %2=_ln_return_discard) _LNUIElement_GetOpacity %1, %2
#cmd _LNUIElement_AddChild $4A5
#define LNUIElement_AddChild(%1, %2) _LNUIElement_AddChild %1, %2
#cmd _LNUIElement_SetFocusable $4A7
#define LNUIElement_SetFocusable(%1, %2) _LNUIElement_SetFocusable %1, %2
#cmd _LNUIElement_GetFocusable $4A9
#define LNUIElement_GetFocusable(%1, %2=_ln_return_discard) _LNUIElement_GetFocusable %1, %2
#cmd LNUIElement_SetPrototype_OnSerialize $DB9
#cmd _LNUITextBlock_Create $4AE
#define LNUITextBlock_Create(%1=_ln_return_discard) _LNUITextBlock_Create %1
#cmd _LNUITextBlock_CreateWithText $4AF
#define LNUITextBlock_CreateWithText(%1, %2=_ln_return_discard) _LNUITextBlock_CreateWithText %1, %2
#cmd _LNUITextBlock_SetText $4AB
#define LNUITextBlock_SetText(%1, %2) _LNUITextBlock_SetText %1, %2
#cmd _LNUITextBlock_GetText $4AD
#define LNUITextBlock_GetText(%1, %2=_ln_return_discard) _LNUITextBlock_GetText %1, %2
#cmd LNUITextBlock_SetPrototype_OnSerialize $DC4
#cmd _LNUISprite_Create $4BE
#define LNUISprite_Create(%1=_ln_return_discard) _LNUISprite_Create %1
#cmd _LNUISprite_CreateWithTexture $4BF
#define LNUISprite_CreateWithTexture(%1, %2=_ln_return_discard) _LNUISprite_CreateWithTexture %1, %2
#cmd _LNUISprite_SetTexture $4B2
#define LNUISprite_SetTexture(%1, %2) _LNUISprite_SetTexture %1, %2
#cmd _LNUISprite_SetSourceRect $4B4
#define LNUISprite_SetSourceRect(%1, %2) _LNUISprite_SetSourceRect %1, %2
#cmd _LNUISprite_SetSourceRectXYWH $4B6
#define LNUISprite_SetSourceRectXYWH(%1, %2, %3, %4, %5) _LNUISprite_SetSourceRectXYWH %1, %2, %3, %4, %5
#cmd _LNUISprite_GetSourceRect $4BB
#define LNUISprite_GetSourceRect(%1, %2=_ln_return_discard) _LNUISprite_GetSourceRect %1, %2
#cmd _LNUISprite_SetShader $4BC
#define LNUISprite_SetShader(%1, %2) _LNUISprite_SetShader %1, %2
#cmd LNUISprite_SetPrototype_OnSerialize $DCF
#cmd _LNUIIcon_LoadFontIcon $4C2
#define LNUIIcon_LoadFontIcon(%1, %2=_ln_return_discard) _LNUIIcon_LoadFontIcon %1, %2
#cmd _LNUIIcon_LoadFontIconWithNameSize $4C4
#define LNUIIcon_LoadFontIconWithNameSize(%1, %2, %3=_ln_return_discard) _LNUIIcon_LoadFontIconWithNameSize %1, %2, %3
#cmd _LNUIIcon_LoadFontIconWithNameSizeColor $4C7
#define LNUIIcon_LoadFontIconWithNameSizeColor(%1, %2, %3, %4=_ln_return_discard) _LNUIIcon_LoadFontIconWithNameSizeColor %1, %2, %3, %4
#cmd LNUIIcon_SetPrototype_OnSerialize $DDA
#cmd _LNUIMessageTextArea_Create $4D0
#define LNUIMessageTextArea_Create(%1=_ln_return_discard) _LNUIMessageTextArea_Create %1
#cmd _LNUIMessageTextArea_SetText $4CC
#define LNUIMessageTextArea_SetText(%1, %2) _LNUIMessageTextArea_SetText %1, %2
#cmd _LNUIMessageTextArea_SetTypingSpeed $4CE
#define LNUIMessageTextArea_SetTypingSpeed(%1, %2) _LNUIMessageTextArea_SetTypingSpeed %1, %2
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $DE5
#cmd _LNUI_Add $4D2
#define LNUI_Add(%1) _LNUI_Add %1
#cmd _LNUI_Remove $4D4
#define LNUI_Remove(%1) _LNUI_Remove %1
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $DF0
#cmd _LNUIBoxLayout_Create $4DB
#define LNUIBoxLayout_Create(%1=_ln_return_discard) _LNUIBoxLayout_Create %1
#cmd _LNUIBoxLayout_SetOrientation $4D8
#define LNUIBoxLayout_SetOrientation(%1, %2) _LNUIBoxLayout_SetOrientation %1, %2
#cmd _LNUIBoxLayout_GetOrientation $4DA
#define LNUIBoxLayout_GetOrientation(%1, %2=_ln_return_discard) _LNUIBoxLayout_GetOrientation %1, %2
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $DFB
#cmd _LNUIStackLayout_Create $4E0
#define LNUIStackLayout_Create(%1=_ln_return_discard) _LNUIStackLayout_Create %1
#cmd _LNUIStackLayout_SetOrientation $4DD
#define LNUIStackLayout_SetOrientation(%1, %2) _LNUIStackLayout_SetOrientation %1, %2
#cmd _LNUIStackLayout_GetOrientation $4DF
#define LNUIStackLayout_GetOrientation(%1, %2=_ln_return_discard) _LNUIStackLayout_GetOrientation %1, %2
#cmd LNUIStackLayout_SetPrototype_OnSerialize $E06
#cmd _LNUIGridLayout_Create $4F0
#define LNUIGridLayout_Create(%1=_ln_return_discard) _LNUIGridLayout_Create %1
#cmd _LNUIGridLayout_SetColumnCount $4E2
#define LNUIGridLayout_SetColumnCount(%1, %2) _LNUIGridLayout_SetColumnCount %1, %2
#cmd _LNUIGridLayout_SetRow $4E4
#define LNUIGridLayout_SetRow(%1, %2) _LNUIGridLayout_SetRow %1, %2
#cmd _LNUIGridLayout_SetColumn $4E7
#define LNUIGridLayout_SetColumn(%1, %2) _LNUIGridLayout_SetColumn %1, %2
#cmd _LNUIGridLayout_SetPlacement $4EA
#define LNUIGridLayout_SetPlacement(%1, %2, %3, %4, %5) _LNUIGridLayout_SetPlacement %1, %2, %3, %4, %5
#cmd LNUIGridLayout_SetPrototype_OnSerialize $E11
#cmd _LNUIControl_Create $4FC
#define LNUIControl_Create(%1=_ln_return_discard) _LNUIControl_Create %1
#cmd _LNUIControl_AddInlineVisual $4FD
#define LNUIControl_AddInlineVisual(%1, %2, %3) _LNUIControl_AddInlineVisual %1, %2, %3
#cmd LNUIControl_SetPrototype_OnSerialize $E1C
#cmd _LNUIButtonBase_SetText $501
#define LNUIButtonBase_SetText(%1, %2) _LNUIButtonBase_SetText %1, %2
#cmd LNUIButtonBase_SetPrototype_OnSerialize $E27
#cmd _LNUIButton_Create $504
#define LNUIButton_Create(%1=_ln_return_discard) _LNUIButton_Create %1
#cmd _LNUIButton_CreateWithText $505
#define LNUIButton_CreateWithText(%1, %2=_ln_return_discard) _LNUIButton_CreateWithText %1, %2
#cmd _LNUIButton_ConnectOnClicked $507
#define LNUIButton_ConnectOnClicked(%1, %2, %3=_ln_return_discard) _LNUIButton_ConnectOnClicked %1, %2, %3
#cmd LNUIButton_SetPrototype_OnSerialize $E32
#cmd _LNUIWindow_Create $50A
#define LNUIWindow_Create(%1=_ln_return_discard) _LNUIWindow_Create %1
#cmd LNUIWindow_SetPrototype_OnSerialize $E3D
#cmd _LNUIListItem_ConnectOnSubmit $50F
#define LNUIListItem_ConnectOnSubmit(%1, %2, %3=_ln_return_discard) _LNUIListItem_ConnectOnSubmit %1, %2, %3
#cmd LNUIListItem_SetPrototype_OnSerialize $E48
#cmd _LNUIListItemsControl_SetItemsLayoutPanel $512
#define LNUIListItemsControl_SetItemsLayoutPanel(%1, %2) _LNUIListItemsControl_SetItemsLayoutPanel %1, %2
#cmd _LNUIListItemsControl_GetItemsLayoutPanel $514
#define LNUIListItemsControl_GetItemsLayoutPanel(%1, %2=_ln_return_discard) _LNUIListItemsControl_GetItemsLayoutPanel %1, %2
#cmd _LNUIListItemsControl_SetSubmitMode $515
#define LNUIListItemsControl_SetSubmitMode(%1, %2) _LNUIListItemsControl_SetSubmitMode %1, %2
#cmd _LNUIListItemsControl_GetSubmitMode $517
#define LNUIListItemsControl_GetSubmitMode(%1, %2=_ln_return_discard) _LNUIListItemsControl_GetSubmitMode %1, %2
#cmd _LNUIListItemsControl_ConnectOnSubmit $518
#define LNUIListItemsControl_ConnectOnSubmit(%1, %2, %3=_ln_return_discard) _LNUIListItemsControl_ConnectOnSubmit %1, %2, %3
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $E53
#cmd _LNUIListBoxItem_Create $51B
#define LNUIListBoxItem_Create(%1, %2=_ln_return_discard) _LNUIListBoxItem_Create %1, %2
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $E5E
#cmd _LNUIListBox_Create $520
#define LNUIListBox_Create(%1=_ln_return_discard) _LNUIListBox_Create %1
#cmd _LNUIListBox_AddItem $51E
#define LNUIListBox_AddItem(%1, %2, %3=_ln_return_discard) _LNUIListBox_AddItem %1, %2, %3
#cmd LNUIListBox_SetPrototype_OnSerialize $E69
#cmd LNInputGesture_SetPrototype_OnSerialize $E74
#cmd _LNKeyGesture_Create $523
#define LNKeyGesture_Create(%1, %2=_ln_return_discard) _LNKeyGesture_Create %1, %2
#cmd LNKeyGesture_SetPrototype_OnSerialize $E7F
#cmd _LNInput_IsPressed $526
#define LNInput_IsPressed(%1, %2=_ln_return_discard) _LNInput_IsPressed %1, %2
#cmd _LNInput_IsTriggered $528
#define LNInput_IsTriggered(%1, %2=_ln_return_discard) _LNInput_IsTriggered %1, %2
#cmd _LNInput_IsTriggeredOff $52A
#define LNInput_IsTriggeredOff(%1, %2=_ln_return_discard) _LNInput_IsTriggeredOff %1, %2
#cmd _LNInput_IsRepeated $52C
#define LNInput_IsRepeated(%1, %2=_ln_return_discard) _LNInput_IsRepeated %1, %2
#cmd _LNInput_GetAxisValue $52E
#define LNInput_GetAxisValue(%1, %2=_ln_return_discard) _LNInput_GetAxisValue %1, %2
#cmd _LNInput_AddBinding $530
#define LNInput_AddBinding(%1, %2) _LNInput_AddBinding %1, %2
#cmd _LNInput_RemoveBinding $533
#define LNInput_RemoveBinding(%1) _LNInput_RemoveBinding %1
#cmd _LNInput_ClearBindings $535
#define LNInput_ClearBindings(%1) _LNInput_ClearBindings %1
#cmd _LNInput_ClearAllBindings $537
#define LNInput_ClearAllBindings _LNInput_ClearAllBindings
#cmd _LNMouse_Pressed $539
#define LNMouse_Pressed(%1, %2=_ln_return_discard) _LNMouse_Pressed %1, %2
#cmd _LNMouse_Triggered $53B
#define LNMouse_Triggered(%1, %2=_ln_return_discard) _LNMouse_Triggered %1, %2
#cmd _LNMouse_TriggeredOff $53D
#define LNMouse_TriggeredOff(%1, %2=_ln_return_discard) _LNMouse_TriggeredOff %1, %2
#cmd _LNMouse_Repeated $53F
#define LNMouse_Repeated(%1, %2=_ln_return_discard) _LNMouse_Repeated %1, %2
#cmd _LNMouse_GetPosition $541
#define LNMouse_GetPosition(%1=_ln_return_discard) _LNMouse_GetPosition %1
#cmd _LNInterpreterCommand_Code $543
#define LNInterpreterCommand_Code(%1, %2=_ln_return_discard) _LNInterpreterCommand_Code %1, %2
#cmd _LNInterpreterCommand_ParamsCount $544
#define LNInterpreterCommand_ParamsCount(%1, %2=_ln_return_discard) _LNInterpreterCommand_ParamsCount %1, %2
#cmd _LNInterpreterCommand_Param $545
#define LNInterpreterCommand_Param(%1, %2, %3=_ln_return_discard) _LNInterpreterCommand_Param %1, %2, %3
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $E8A
#cmd _LNInterpreterCommandList_Create $55C
#define LNInterpreterCommandList_Create(%1=_ln_return_discard) _LNInterpreterCommandList_Create %1
#cmd _LNInterpreterCommandList_AddCommand $548
#define LNInterpreterCommandList_AddCommand(%1, %2) _LNInterpreterCommandList_AddCommand %1, %2
#cmd _LNInterpreterCommandList_AddCommand1 $54A
#define LNInterpreterCommandList_AddCommand1(%1, %2, %3) _LNInterpreterCommandList_AddCommand1 %1, %2, %3
#cmd _LNInterpreterCommandList_AddCommand2 $54D
#define LNInterpreterCommandList_AddCommand2(%1, %2, %3, %4) _LNInterpreterCommandList_AddCommand2 %1, %2, %3, %4
#cmd _LNInterpreterCommandList_AddCommand3 $551
#define LNInterpreterCommandList_AddCommand3(%1, %2, %3, %4, %5) _LNInterpreterCommandList_AddCommand3 %1, %2, %3, %4, %5
#cmd _LNInterpreterCommandList_AddCommand4 $556
#define LNInterpreterCommandList_AddCommand4(%1, %2, %3, %4, %5, %6) _LNInterpreterCommandList_AddCommand4 %1, %2, %3, %4, %5, %6
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $E95
#cmd _LNInterpreterCommandDelegate_Create $9F6
#define LNInterpreterCommandDelegate_Create(%1, %2=_ln_return_discard) _LNInterpreterCommandDelegate_Create %1, %2
#cmd _LNInterpreter_Create $570
#define LNInterpreter_Create(%1=_ln_return_discard) _LNInterpreter_Create %1
#cmd _LNInterpreter_Clear $561
#define LNInterpreter_Clear(%1) _LNInterpreter_Clear %1
#cmd _LNInterpreter_Run $562
#define LNInterpreter_Run(%1, %2) _LNInterpreter_Run %1, %2
#cmd _LNInterpreter_IsRunning $564
#define LNInterpreter_IsRunning(%1, %2=_ln_return_discard) _LNInterpreter_IsRunning %1, %2
#cmd _LNInterpreter_Update $565
#define LNInterpreter_Update(%1) _LNInterpreter_Update %1
#cmd _LNInterpreter_Terminate $566
#define LNInterpreter_Terminate(%1) _LNInterpreter_Terminate %1
#cmd _LNInterpreter_RegisterCommandHandler $567
#define LNInterpreter_RegisterCommandHandler(%1, %2, %3) _LNInterpreter_RegisterCommandHandler %1, %2, %3
#cmd _LNInterpreter_SetWaitMode $56A
#define LNInterpreter_SetWaitMode(%1, %2) _LNInterpreter_SetWaitMode %1, %2
#cmd _LNInterpreter_GetWaitMode $56C
#define LNInterpreter_GetWaitMode(%1, %2=_ln_return_discard) _LNInterpreter_GetWaitMode %1, %2
#cmd _LNInterpreter_SetWaitCount $56D
#define LNInterpreter_SetWaitCount(%1, %2) _LNInterpreter_SetWaitCount %1, %2
#cmd _LNInterpreter_GetWaitCount $56F
#define LNInterpreter_GetWaitCount(%1, %2=_ln_return_discard) _LNInterpreter_GetWaitCount %1, %2
#cmd LNInterpreter_SetPrototype_OnSerialize $EA0
#cmd LNInterpreter_SetPrototype_OnUpdateWait $EAC
#cmd _LNEngineSettings_SetMainWindowSize $573
#define LNEngineSettings_SetMainWindowSize(%1, %2) _LNEngineSettings_SetMainWindowSize %1, %2
#cmd _LNEngineSettings_SetMainWorldViewSize $576
#define LNEngineSettings_SetMainWorldViewSize(%1, %2) _LNEngineSettings_SetMainWorldViewSize %1, %2
#cmd _LNEngineSettings_SetMainWindowTitle $579
#define LNEngineSettings_SetMainWindowTitle(%1) _LNEngineSettings_SetMainWindowTitle %1
#cmd _LNEngineSettings_SetMainWindowResizable $57B
#define LNEngineSettings_SetMainWindowResizable(%1) _LNEngineSettings_SetMainWindowResizable %1
#cmd _LNEngineSettings_AddAssetDirectory $57D
#define LNEngineSettings_AddAssetDirectory(%1) _LNEngineSettings_AddAssetDirectory %1
#cmd _LNEngineSettings_AddAssetArchive $57F
#define LNEngineSettings_AddAssetArchive(%1, %2) _LNEngineSettings_AddAssetArchive %1, %2
#cmd _LNEngineSettings_SetFrameRate $582
#define LNEngineSettings_SetFrameRate(%1) _LNEngineSettings_SetFrameRate %1
#cmd _LNEngineSettings_SetUITheme $584
#define LNEngineSettings_SetUITheme(%1) _LNEngineSettings_SetUITheme %1
#cmd _LNEngineSettings_SetFontFile $586
#define LNEngineSettings_SetFontFile(%1) _LNEngineSettings_SetFontFile %1
#cmd _LNEngineSettings_SetDebugToolEnabled $588
#define LNEngineSettings_SetDebugToolEnabled(%1) _LNEngineSettings_SetDebugToolEnabled %1
#cmd _LNEngineSettings_SetEngineLogEnabled $58A
#define LNEngineSettings_SetEngineLogEnabled(%1) _LNEngineSettings_SetEngineLogEnabled %1
#cmd _LNEngineSettings_SetEngineLogFilePath $58C
#define LNEngineSettings_SetEngineLogFilePath(%1) _LNEngineSettings_SetEngineLogFilePath %1
#cmd _LNEngineSettings_SetDeveloperToolEnabled $58E
#define LNEngineSettings_SetDeveloperToolEnabled(%1) _LNEngineSettings_SetDeveloperToolEnabled %1
#cmd _LNEngineSettings_SetUserMainWindow $590
#define LNEngineSettings_SetUserMainWindow(%1) _LNEngineSettings_SetUserMainWindow %1
#cmd _LNEngine_Initialize $593
#define LNEngine_Initialize _LNEngine_Initialize
#cmd _LNEngine_Terminate $594
#define LNEngine_Terminate _LNEngine_Terminate
#cmd _LNEngine_Update $595
#define LNEngine_Update(%1=_ln_return_discard) _LNEngine_Update %1
#cmd _LNEngine_GetTime $596
#define LNEngine_GetTime(%1=_ln_return_discard) _LNEngine_GetTime %1
#cmd _LNEngine_GetWorld $597
#define LNEngine_GetWorld(%1=_ln_return_discard) _LNEngine_GetWorld %1
#cmd _LNEngine_GetCamera $598
#define LNEngine_GetCamera(%1=_ln_return_discard) _LNEngine_GetCamera %1
#cmd _LNEngine_GetMainLight $599
#define LNEngine_GetMainLight(%1=_ln_return_discard) _LNEngine_GetMainLight %1
#cmd _LNEngine_GetRenderView $59A
#define LNEngine_GetRenderView(%1=_ln_return_discard) _LNEngine_GetRenderView %1
#cmd _LNApplication_Create $5A0
#define LNApplication_Create(%1=_ln_return_discard) _LNApplication_Create %1
#cmd _LNApplication_OnInit $59C
#define LNApplication_OnInit(%1) _LNApplication_OnInit %1
#cmd _LNApplication_OnUpdate $59D
#define LNApplication_OnUpdate(%1) _LNApplication_OnUpdate %1
#cmd _LNApplication_World $59E
#define LNApplication_World(%1, %2=_ln_return_discard) _LNApplication_World %1, %2
#cmd _LNApplication_Run $59F
#define LNApplication_Run(%1) _LNApplication_Run %1
#cmd LNApplication_SetPrototype_OnSerialize $EB7
#cmd LNApplication_SetPrototype_OnInit $EC2
#cmd LNApplication_SetPrototype_OnUpdate $ECD
#cmd _LNDebug_SetGuideGridEnabled $5A2
#define LNDebug_SetGuideGridEnabled(%1) _LNDebug_SetGuideGridEnabled %1
#cmd _LNDebug_SetPhysicsDebugDrawEnabled $5A4
#define LNDebug_SetPhysicsDebugDrawEnabled(%1) _LNDebug_SetPhysicsDebugDrawEnabled %1
#cmd _LNDebug_Print $5A6
#define LNDebug_Print(%1) _LNDebug_Print %1
#cmd _LNDebug_PrintWithTime $5A8
#define LNDebug_PrintWithTime(%1, %2) _LNDebug_PrintWithTime %1, %2
#cmd _LNDebug_PrintWithTimeAndColor $5AB
#define LNDebug_PrintWithTimeAndColor(%1, %2, %3) _LNDebug_PrintWithTimeAndColor %1, %2, %3
#cmd _LNObjectSerializeHandler_Create $A26
#define LNObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNObjectSerializeHandler_Create %1, %2
#cmd _LNEventConnectionSerializeHandler_Create $A31
#define LNEventConnectionSerializeHandler_Create(%1, %2=_ln_return_discard) _LNEventConnectionSerializeHandler_Create %1, %2
#cmd _LNVariantSerializeHandler_Create $A3C
#define LNVariantSerializeHandler_Create(%1, %2=_ln_return_discard) _LNVariantSerializeHandler_Create %1, %2
#cmd _LNZVTestClass1SerializeHandler_Create $A47
#define LNZVTestClass1SerializeHandler_Create(%1, %2=_ln_return_discard) _LNZVTestClass1SerializeHandler_Create %1, %2
#cmd _LNZVTestEventArgs1SerializeHandler_Create $A52
#define LNZVTestEventArgs1SerializeHandler_Create(%1, %2=_ln_return_discard) _LNZVTestEventArgs1SerializeHandler_Create %1, %2
#cmd _LNSerializer2SerializeHandler_Create $A5D
#define LNSerializer2SerializeHandler_Create(%1, %2=_ln_return_discard) _LNSerializer2SerializeHandler_Create %1, %2
#cmd _LNAssetObjectSerializeHandler_Create $A68
#define LNAssetObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAssetObjectSerializeHandler_Create %1, %2
#cmd _LNAssetImportSettingsSerializeHandler_Create $A73
#define LNAssetImportSettingsSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAssetImportSettingsSerializeHandler_Create %1, %2
#cmd _LNAssetModelSerializeHandler_Create $A7E
#define LNAssetModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAssetModelSerializeHandler_Create %1, %2
#cmd _LNSoundSerializeHandler_Create $A89
#define LNSoundSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSoundSerializeHandler_Create %1, %2
#cmd _LNTextureSerializeHandler_Create $A94
#define LNTextureSerializeHandler_Create(%1, %2=_ln_return_discard) _LNTextureSerializeHandler_Create %1, %2
#cmd _LNTexture2DSerializeHandler_Create $A9F
#define LNTexture2DSerializeHandler_Create(%1, %2=_ln_return_discard) _LNTexture2DSerializeHandler_Create %1, %2
#cmd _LNShaderSerializeHandler_Create $AAA
#define LNShaderSerializeHandler_Create(%1, %2=_ln_return_discard) _LNShaderSerializeHandler_Create %1, %2
#cmd _LNRenderViewSerializeHandler_Create $AB5
#define LNRenderViewSerializeHandler_Create(%1, %2=_ln_return_discard) _LNRenderViewSerializeHandler_Create %1, %2
#cmd _LNMaterialSerializeHandler_Create $AC0
#define LNMaterialSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMaterialSerializeHandler_Create %1, %2
#cmd _LNMeshNodeSerializeHandler_Create $ACB
#define LNMeshNodeSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshNodeSerializeHandler_Create %1, %2
#cmd _LNAnimationControllerSerializeHandler_Create $AD6
#define LNAnimationControllerSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationControllerSerializeHandler_Create %1, %2
#cmd _LNMeshModelSerializeHandler_Create $AE1
#define LNMeshModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshModelSerializeHandler_Create %1, %2
#cmd _LNMeshImportSettingsSerializeHandler_Create $AEC
#define LNMeshImportSettingsSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshImportSettingsSerializeHandler_Create %1, %2
#cmd _LNSkinnedMeshModelSerializeHandler_Create $AF7
#define LNSkinnedMeshModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSkinnedMeshModelSerializeHandler_Create %1, %2
#cmd _LNCollisionShapeSerializeHandler_Create $B02
#define LNCollisionShapeSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCollisionShapeSerializeHandler_Create %1, %2
#cmd _LNBoxCollisionShapeSerializeHandler_Create $B0D
#define LNBoxCollisionShapeSerializeHandler_Create(%1, %2=_ln_return_discard) _LNBoxCollisionShapeSerializeHandler_Create %1, %2
#cmd _LNAnimationCurveSerializeHandler_Create $B18
#define LNAnimationCurveSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationCurveSerializeHandler_Create %1, %2
#cmd _LNKeyFrameAnimationCurveSerializeHandler_Create $B23
#define LNKeyFrameAnimationCurveSerializeHandler_Create(%1, %2=_ln_return_discard) _LNKeyFrameAnimationCurveSerializeHandler_Create %1, %2
#cmd _LNAnimationClipSerializeHandler_Create $B2E
#define LNAnimationClipSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationClipSerializeHandler_Create %1, %2
#cmd _LNAnimationStateSerializeHandler_Create $B39
#define LNAnimationStateSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationStateSerializeHandler_Create %1, %2
#cmd _LNEffectResourceSerializeHandler_Create $B44
#define LNEffectResourceSerializeHandler_Create(%1, %2=_ln_return_discard) _LNEffectResourceSerializeHandler_Create %1, %2
#cmd _LNParticleEmitterModelSerializeHandler_Create $B4F
#define LNParticleEmitterModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterModelSerializeHandler_Create %1, %2
#cmd _LNParticleModelSerializeHandler_Create $B5A
#define LNParticleModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNParticleModelSerializeHandler_Create %1, %2
#cmd _LNComponentSerializeHandler_Create $B65
#define LNComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNComponentSerializeHandler_Create %1, %2
#cmd _LNVisualComponentSerializeHandler_Create $B70
#define LNVisualComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNVisualComponentSerializeHandler_Create %1, %2
#cmd _LNSpriteComponentSerializeHandler_Create $B7B
#define LNSpriteComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSpriteComponentSerializeHandler_Create %1, %2
#cmd _LNCharacterControllerSerializeHandler_Create $B86
#define LNCharacterControllerSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCharacterControllerSerializeHandler_Create %1, %2
#cmd _LNWorldSerializeHandler_Create $B91
#define LNWorldSerializeHandler_Create(%1, %2=_ln_return_discard) _LNWorldSerializeHandler_Create %1, %2
#cmd _LNComponentListSerializeHandler_Create $B9C
#define LNComponentListSerializeHandler_Create(%1, %2=_ln_return_discard) _LNComponentListSerializeHandler_Create %1, %2
#cmd _LNWorldObjectSerializeHandler_Create $BA7
#define LNWorldObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNWorldObjectSerializeHandler_Create %1, %2
#cmd _LNWorldObjectPreUpdateHandler_Create $BB2
#define LNWorldObjectPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNWorldObjectPreUpdateHandler_Create %1, %2
#cmd _LNWorldObjectUpdateHandler_Create $BBD
#define LNWorldObjectUpdateHandler_Create(%1, %2=_ln_return_discard) _LNWorldObjectUpdateHandler_Create %1, %2
#cmd _LNVisualObjectSerializeHandler_Create $BC8
#define LNVisualObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNVisualObjectSerializeHandler_Create %1, %2
#cmd _LNVisualObjectPreUpdateHandler_Create $BD3
#define LNVisualObjectPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNVisualObjectPreUpdateHandler_Create %1, %2
#cmd _LNVisualObjectUpdateHandler_Create $BDE
#define LNVisualObjectUpdateHandler_Create(%1, %2=_ln_return_discard) _LNVisualObjectUpdateHandler_Create %1, %2
#cmd _LNCameraSerializeHandler_Create $BE9
#define LNCameraSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCameraSerializeHandler_Create %1, %2
#cmd _LNCameraPreUpdateHandler_Create $BF4
#define LNCameraPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNCameraPreUpdateHandler_Create %1, %2
#cmd _LNCameraUpdateHandler_Create $BFF
#define LNCameraUpdateHandler_Create(%1, %2=_ln_return_discard) _LNCameraUpdateHandler_Create %1, %2
#cmd _LNEnvironmentLightSerializeHandler_Create $C0A
#define LNEnvironmentLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNEnvironmentLightSerializeHandler_Create %1, %2
#cmd _LNEnvironmentLightPreUpdateHandler_Create $C15
#define LNEnvironmentLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNEnvironmentLightPreUpdateHandler_Create %1, %2
#cmd _LNEnvironmentLightUpdateHandler_Create $C20
#define LNEnvironmentLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNEnvironmentLightUpdateHandler_Create %1, %2
#cmd _LNDirectionalLightSerializeHandler_Create $C2B
#define LNDirectionalLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNDirectionalLightSerializeHandler_Create %1, %2
#cmd _LNDirectionalLightPreUpdateHandler_Create $C36
#define LNDirectionalLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNDirectionalLightPreUpdateHandler_Create %1, %2
#cmd _LNDirectionalLightUpdateHandler_Create $C41
#define LNDirectionalLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNDirectionalLightUpdateHandler_Create %1, %2
#cmd _LNPointLightSerializeHandler_Create $C4C
#define LNPointLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNPointLightSerializeHandler_Create %1, %2
#cmd _LNPointLightPreUpdateHandler_Create $C57
#define LNPointLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPointLightPreUpdateHandler_Create %1, %2
#cmd _LNPointLightUpdateHandler_Create $C62
#define LNPointLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPointLightUpdateHandler_Create %1, %2
#cmd _LNSpotLightSerializeHandler_Create $C6D
#define LNSpotLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSpotLightSerializeHandler_Create %1, %2
#cmd _LNSpotLightPreUpdateHandler_Create $C78
#define LNSpotLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpotLightPreUpdateHandler_Create %1, %2
#cmd _LNSpotLightUpdateHandler_Create $C83
#define LNSpotLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpotLightUpdateHandler_Create %1, %2
#cmd _LNSpriteSerializeHandler_Create $C8E
#define LNSpriteSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSpriteSerializeHandler_Create %1, %2
#cmd _LNSpritePreUpdateHandler_Create $C99
#define LNSpritePreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpritePreUpdateHandler_Create %1, %2
#cmd _LNSpriteUpdateHandler_Create $CA4
#define LNSpriteUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpriteUpdateHandler_Create %1, %2
#cmd _LNCameraOrbitControlComponentSerializeHandler_Create $CAF
#define LNCameraOrbitControlComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCameraOrbitControlComponentSerializeHandler_Create %1, %2
#cmd _LNRaycasterSerializeHandler_Create $CBA
#define LNRaycasterSerializeHandler_Create(%1, %2=_ln_return_discard) _LNRaycasterSerializeHandler_Create %1, %2
#cmd _LNRaycastResultSerializeHandler_Create $CC5
#define LNRaycastResultSerializeHandler_Create(%1, %2=_ln_return_discard) _LNRaycastResultSerializeHandler_Create %1, %2
#cmd _LNWorldRenderViewSerializeHandler_Create $CD0
#define LNWorldRenderViewSerializeHandler_Create(%1, %2=_ln_return_discard) _LNWorldRenderViewSerializeHandler_Create %1, %2
#cmd _LNBoxMeshSerializeHandler_Create $CDB
#define LNBoxMeshSerializeHandler_Create(%1, %2=_ln_return_discard) _LNBoxMeshSerializeHandler_Create %1, %2
#cmd _LNBoxMeshPreUpdateHandler_Create $CE6
#define LNBoxMeshPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNBoxMeshPreUpdateHandler_Create %1, %2
#cmd _LNBoxMeshUpdateHandler_Create $CF1
#define LNBoxMeshUpdateHandler_Create(%1, %2=_ln_return_discard) _LNBoxMeshUpdateHandler_Create %1, %2
#cmd _LNPlaneMeshSerializeHandler_Create $CFC
#define LNPlaneMeshSerializeHandler_Create(%1, %2=_ln_return_discard) _LNPlaneMeshSerializeHandler_Create %1, %2
#cmd _LNPlaneMeshPreUpdateHandler_Create $D07
#define LNPlaneMeshPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPlaneMeshPreUpdateHandler_Create %1, %2
#cmd _LNPlaneMeshUpdateHandler_Create $D12
#define LNPlaneMeshUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPlaneMeshUpdateHandler_Create %1, %2
#cmd _LNMeshComponentSerializeHandler_Create $D1D
#define LNMeshComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshComponentSerializeHandler_Create %1, %2
#cmd _LNCollisionSerializeHandler_Create $D28
#define LNCollisionSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCollisionSerializeHandler_Create %1, %2
#cmd _LNTriggerBodyComponentSerializeHandler_Create $D33
#define LNTriggerBodyComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNTriggerBodyComponentSerializeHandler_Create %1, %2
#cmd _LNParticleEmitterSerializeHandler_Create $D3E
#define LNParticleEmitterSerializeHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterSerializeHandler_Create %1, %2
#cmd _LNParticleEmitterPreUpdateHandler_Create $D49
#define LNParticleEmitterPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterPreUpdateHandler_Create %1, %2
#cmd _LNParticleEmitterUpdateHandler_Create $D54
#define LNParticleEmitterUpdateHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterUpdateHandler_Create %1, %2
#cmd _LNLevelSerializeHandler_Create $D5F
#define LNLevelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNLevelSerializeHandler_Create %1, %2
#cmd _LNLevelStartHandler_Create $D6A
#define LNLevelStartHandler_Create(%1, %2=_ln_return_discard) _LNLevelStartHandler_Create %1, %2
#cmd _LNLevelStopHandler_Create $D75
#define LNLevelStopHandler_Create(%1, %2=_ln_return_discard) _LNLevelStopHandler_Create %1, %2
#cmd _LNLevelPauseHandler_Create $D80
#define LNLevelPauseHandler_Create(%1, %2=_ln_return_discard) _LNLevelPauseHandler_Create %1, %2
#cmd _LNLevelResumeHandler_Create $D8B
#define LNLevelResumeHandler_Create(%1, %2=_ln_return_discard) _LNLevelResumeHandler_Create %1, %2
#cmd _LNLevelUpdateHandler_Create $D96
#define LNLevelUpdateHandler_Create(%1, %2=_ln_return_discard) _LNLevelUpdateHandler_Create %1, %2
#cmd _LNUIEventArgsSerializeHandler_Create $DA1
#define LNUIEventArgsSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIEventArgsSerializeHandler_Create %1, %2
#cmd _LNUILayoutElementSerializeHandler_Create $DAC
#define LNUILayoutElementSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUILayoutElementSerializeHandler_Create %1, %2
#cmd _LNUIElementSerializeHandler_Create $DB7
#define LNUIElementSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIElementSerializeHandler_Create %1, %2
#cmd _LNUITextBlockSerializeHandler_Create $DC2
#define LNUITextBlockSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUITextBlockSerializeHandler_Create %1, %2
#cmd _LNUISpriteSerializeHandler_Create $DCD
#define LNUISpriteSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUISpriteSerializeHandler_Create %1, %2
#cmd _LNUIIconSerializeHandler_Create $DD8
#define LNUIIconSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIIconSerializeHandler_Create %1, %2
#cmd _LNUIMessageTextAreaSerializeHandler_Create $DE3
#define LNUIMessageTextAreaSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIMessageTextAreaSerializeHandler_Create %1, %2
#cmd _LNUILayoutPanelSerializeHandler_Create $DEE
#define LNUILayoutPanelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUILayoutPanelSerializeHandler_Create %1, %2
#cmd _LNUIBoxLayoutSerializeHandler_Create $DF9
#define LNUIBoxLayoutSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIBoxLayoutSerializeHandler_Create %1, %2
#cmd _LNUIStackLayoutSerializeHandler_Create $E04
#define LNUIStackLayoutSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIStackLayoutSerializeHandler_Create %1, %2
#cmd _LNUIGridLayoutSerializeHandler_Create $E0F
#define LNUIGridLayoutSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIGridLayoutSerializeHandler_Create %1, %2
#cmd _LNUIControlSerializeHandler_Create $E1A
#define LNUIControlSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIControlSerializeHandler_Create %1, %2
#cmd _LNUIButtonBaseSerializeHandler_Create $E25
#define LNUIButtonBaseSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIButtonBaseSerializeHandler_Create %1, %2
#cmd _LNUIButtonSerializeHandler_Create $E30
#define LNUIButtonSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIButtonSerializeHandler_Create %1, %2
#cmd _LNUIWindowSerializeHandler_Create $E3B
#define LNUIWindowSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIWindowSerializeHandler_Create %1, %2
#cmd _LNUIListItemSerializeHandler_Create $E46
#define LNUIListItemSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListItemSerializeHandler_Create %1, %2
#cmd _LNUIListItemsControlSerializeHandler_Create $E51
#define LNUIListItemsControlSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListItemsControlSerializeHandler_Create %1, %2
#cmd _LNUIListBoxItemSerializeHandler_Create $E5C
#define LNUIListBoxItemSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListBoxItemSerializeHandler_Create %1, %2
#cmd _LNUIListBoxSerializeHandler_Create $E67
#define LNUIListBoxSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListBoxSerializeHandler_Create %1, %2
#cmd _LNInputGestureSerializeHandler_Create $E72
#define LNInputGestureSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInputGestureSerializeHandler_Create %1, %2
#cmd _LNKeyGestureSerializeHandler_Create $E7D
#define LNKeyGestureSerializeHandler_Create(%1, %2=_ln_return_discard) _LNKeyGestureSerializeHandler_Create %1, %2
#cmd _LNInterpreterCommandSerializeHandler_Create $E88
#define LNInterpreterCommandSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterCommandSerializeHandler_Create %1, %2
#cmd _LNInterpreterCommandListSerializeHandler_Create $E93
#define LNInterpreterCommandListSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterCommandListSerializeHandler_Create %1, %2
#cmd _LNInterpreterSerializeHandler_Create $E9E
#define LNInterpreterSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterSerializeHandler_Create %1, %2
#cmd _LNInterpreterUpdateWaitHandler_Create $EAA
#define LNInterpreterUpdateWaitHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterUpdateWaitHandler_Create %1, %2
#cmd _LNApplicationSerializeHandler_Create $EB5
#define LNApplicationSerializeHandler_Create(%1, %2=_ln_return_discard) _LNApplicationSerializeHandler_Create %1, %2
#cmd _LNApplicationInitHandler_Create $EC0
#define LNApplicationInitHandler_Create(%1, %2=_ln_return_discard) _LNApplicationInitHandler_Create %1, %2
#cmd _LNApplicationUpdateHandler_Create $ECB
#define LNApplicationUpdateHandler_Create(%1, %2=_ln_return_discard) _LNApplicationUpdateHandler_Create %1, %2



#cmd ln_args $1
#cmd ln_set_args $2

#define LUMINO_APP _ln_local_app = LN_NULL_HANDLE :\
    LNApplication_Create _ln_local_app :\
    LNApplication_SetPrototype_OnInit _ln_local_app, *on_init :\
    LNApplication_SetPrototype_OnUpdate _ln_local_app, *on_update :\
    LNApplication_Run _ln_local_app :\
    end

#endif // __lumino__
