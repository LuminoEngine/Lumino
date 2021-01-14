
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
#cmd LNVector3_GetX $5AC
#cmd LNVector3_SetX $5AE
#cmd LNVector3_GetY $5AF
#cmd LNVector3_SetY $5B1
#cmd LNVector3_GetZ $5B2
#cmd LNVector3_SetZ $5B4
#cmd LNVector3_SetZeros $39
#cmd LNVector3_Set $3A
#cmd LNVector3_Get $3E
#cmd LNVector3_Length $42
#cmd LNVector3_LengthSquared $43
#cmd LNVector3_MutatingNormalize $44
#cmd LNVector3_NormalizeXYZ $45
#cmd LNVector3_Normalize $49
#cmd LNVector4 $4B
#cmd LNVector4_GetX $5CB
#cmd LNVector4_SetX $5CD
#cmd LNVector4_GetY $5CE
#cmd LNVector4_SetY $5D0
#cmd LNVector4_GetZ $5D1
#cmd LNVector4_SetZ $5D3
#cmd LNVector4_GetW $5D4
#cmd LNVector4_SetW $5D6
#cmd LNVector4_SetZeros $50
#cmd LNVector4_Set $51
#cmd LNQuaternion $56
#cmd LNQuaternion_GetX $5E9
#cmd LNQuaternion_SetX $5EB
#cmd LNQuaternion_GetY $5EC
#cmd LNQuaternion_SetY $5EE
#cmd LNQuaternion_GetZ $5EF
#cmd LNQuaternion_SetZ $5F1
#cmd LNQuaternion_GetW $5F2
#cmd LNQuaternion_SetW $5F4
#cmd LNQuaternion_SetZeros $5B
#cmd LNQuaternion_Set $5C
#cmd LNQuaternion_SetFromAxis $61
#cmd LNMatrix $64
#cmd LNMatrix_GetRow0 $608
#cmd LNMatrix_SetRow0 $60A
#cmd LNMatrix_GetRow1 $60B
#cmd LNMatrix_SetRow1 $60D
#cmd LNMatrix_GetRow2 $60E
#cmd LNMatrix_SetRow2 $610
#cmd LNMatrix_GetRow3 $611
#cmd LNMatrix_SetRow3 $613
#cmd LNMatrix_SetZeros $69
#cmd LNMatrix_Set $6A
#cmd LNColor $18B
#cmd LNColor_GetR $6A7
#cmd LNColor_SetR $6A9
#cmd LNColor_GetG $6AA
#cmd LNColor_SetG $6AC
#cmd LNColor_GetB $6AD
#cmd LNColor_SetB $6AF
#cmd LNColor_GetA $6B0
#cmd LNColor_SetA $6B2
#cmd LNColor_SetZeros $190
#cmd LNColor_Set $191
#cmd LNColorTone $196
#cmd LNColorTone_GetR $6C6
#cmd LNColorTone_SetR $6C8
#cmd LNColorTone_GetG $6C9
#cmd LNColorTone_SetG $6CB
#cmd LNColorTone_GetB $6CC
#cmd LNColorTone_SetB $6CE
#cmd LNColorTone_GetS $6CF
#cmd LNColorTone_SetS $6D1
#cmd LNColorTone_SetZeros $19B
#cmd LNColorTone_Set $19C
#cmd LNPoint $1A1
#cmd LNPoint_GetX $6E4
#cmd LNPoint_SetX $6E6
#cmd LNPoint_GetY $6E7
#cmd LNPoint_SetY $6E9
#cmd LNPoint_SetZeros $1A4
#cmd LNPoint_Set $1A5
#cmd LNSize $1A8
#cmd LNSize_GetWidth $6F4
#cmd LNSize_SetWidth $6F6
#cmd LNSize_GetHeight $6F7
#cmd LNSize_SetHeight $6F9
#cmd LNSize_SetZeros $1AB
#cmd LNSize_Set $1AC
#cmd LNRect $1AF
#cmd LNRect_GetX $704
#cmd LNRect_SetX $706
#cmd LNRect_GetY $707
#cmd LNRect_SetY $709
#cmd LNRect_GetWidth $70A
#cmd LNRect_SetWidth $70C
#cmd LNRect_GetHeight $70D
#cmd LNRect_SetHeight $70F
#cmd LNRect_SetZeros $1B4
#cmd LNRect_Set $1B5
#cmd LNRect_GetLeft $1BA
#cmd LNRect_SetSize $1BB
#cmd LNRect_GetSize $1BD
#cmd LNThickness $1BE
#cmd LNThickness_GetLeft $728
#cmd LNThickness_SetLeft $72A
#cmd LNThickness_GetTop $72B
#cmd LNThickness_SetTop $72D
#cmd LNThickness_GetRight $72E
#cmd LNThickness_SetRight $730
#cmd LNThickness_GetBottom $731
#cmd LNThickness_SetBottom $733
#cmd LNThickness_SetZeros $1C3
#cmd LNThickness_Set $1C4
#cmd LNCornerRadius $1C9
#cmd LNCornerRadius_GetTopleft $746
#cmd LNCornerRadius_SetTopleft $748
#cmd LNCornerRadius_GetTopright $749
#cmd LNCornerRadius_SetTopright $74B
#cmd LNCornerRadius_GetBottomright $74C
#cmd LNCornerRadius_SetBottomright $74E
#cmd LNCornerRadius_GetBottomleft $74F
#cmd LNCornerRadius_SetBottomleft $751
#cmd LNCornerRadius_SetZeros $1CE
#cmd LNCornerRadius_Set $1CF

#cmd _LNObject_Release $84
#define LNObject_Release(%1) _LNObject_Release %1
#cmd _LNObject_Retain $85
#define LNObject_Retain(%1) _LNObject_Retain %1
#cmd _LNObject_GetReferenceCount $86
#define LNObject_GetReferenceCount(%1, %2=_ln_return_discard) _LNObject_GetReferenceCount %1, %2
#cmd LNObject_SetPrototype_OnSerialize $A22
#cmd LNEventConnection_SetPrototype_OnSerialize $A2D
#cmd _LNPromiseFailureDelegate_Create $62E
#define LNPromiseFailureDelegate_Create(%1, %2=_ln_return_discard) _LNPromiseFailureDelegate_Create %1, %2
#cmd _LNVariant_Create $8B
#define LNVariant_Create(%1=_ln_return_discard) _LNVariant_Create %1
#cmd _LNVariant_SetInt $8C
#define LNVariant_SetInt(%1, %2) _LNVariant_SetInt %1, %2
#cmd _LNVariant_GetInt $8E
#define LNVariant_GetInt(%1, %2=_ln_return_discard) _LNVariant_GetInt %1, %2
#cmd LNVariant_SetPrototype_OnSerialize $A38
#cmd _LNZVTestDelegate1_Create $637
#define LNZVTestDelegate1_Create(%1, %2=_ln_return_discard) _LNZVTestDelegate1_Create %1, %2
#cmd _LNZVTestDelegate2_Create $63D
#define LNZVTestDelegate2_Create(%1, %2=_ln_return_discard) _LNZVTestDelegate2_Create %1, %2
#cmd _LNZVTestDelegate3_Create $642
#define LNZVTestDelegate3_Create(%1, %2=_ln_return_discard) _LNZVTestDelegate3_Create %1, %2
#cmd _LNZVTestEventHandler1_Create $647
#define LNZVTestEventHandler1_Create(%1, %2=_ln_return_discard) _LNZVTestEventHandler1_Create %1, %2
#cmd _LNZVTestEventHandler2_Create $64C
#define LNZVTestEventHandler2_Create(%1, %2=_ln_return_discard) _LNZVTestEventHandler2_Create %1, %2
#cmd _LNZVTestPromise1_ThenWith $64F
#define LNZVTestPromise1_ThenWith(%1, %2) _LNZVTestPromise1_ThenWith %1, %2
#cmd _LNZVTestPromise1_CatchWith $651
#define LNZVTestPromise1_CatchWith(%1, %2) _LNZVTestPromise1_CatchWith %1, %2
#cmd _LNZVTestPromise2_ThenWith $655
#define LNZVTestPromise2_ThenWith(%1, %2) _LNZVTestPromise2_ThenWith %1, %2
#cmd _LNZVTestPromise2_CatchWith $657
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $A43
#cmd _LNZVTestEventArgs1_Create $BA
#define LNZVTestEventArgs1_Create(%1=_ln_return_discard) _LNZVTestEventArgs1_Create %1
#cmd _LNZVTestEventArgs1_CreateWithValue $BB
#define LNZVTestEventArgs1_CreateWithValue(%1, %2=_ln_return_discard) _LNZVTestEventArgs1_CreateWithValue %1, %2
#cmd _LNZVTestEventArgs1_GetValue $B9
#define LNZVTestEventArgs1_GetValue(%1, %2=_ln_return_discard) _LNZVTestEventArgs1_GetValue %1, %2
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $A4E
#cmd _LNLog_SetLevel $BE
#define LNLog_SetLevel(%1) _LNLog_SetLevel %1
#cmd _LNLog_AllocConsole $C0
#define LNLog_AllocConsole _LNLog_AllocConsole
#cmd _LNLog_Write $C1
#define LNLog_Write(%1, %2, %3) _LNLog_Write %1, %2, %3
#cmd LNSerializer2_SetPrototype_OnSerialize $A59
#cmd LNAssetObject_SetPrototype_OnSerialize $A64
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $A6F
#cmd _LNAssetModel_Create $125
#define LNAssetModel_Create(%1, %2=_ln_return_discard) _LNAssetModel_Create %1, %2
#cmd _LNAssetModel_Target $124
#define LNAssetModel_Target(%1, %2=_ln_return_discard) _LNAssetModel_Target %1, %2
#cmd LNAssetModel_SetPrototype_OnSerialize $A7A
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
#cmd LNSound_SetPrototype_OnSerialize $A85
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
#cmd _LNTexture2DDelegate_Create $69D
#define LNTexture2DDelegate_Create(%1, %2=_ln_return_discard) _LNTexture2DDelegate_Create %1, %2
#cmd _LNTexture2DPromise_ThenWith $6A0
#define LNTexture2DPromise_ThenWith(%1, %2) _LNTexture2DPromise_ThenWith %1, %2
#cmd _LNTexture2DPromise_CatchWith $6A2
#define LNTexture2DPromise_CatchWith(%1, %2) _LNTexture2DPromise_CatchWith %1, %2
#cmd _LNGraphics_GetActiveGraphicsAPI $18A
#define LNGraphics_GetActiveGraphicsAPI(%1=_ln_return_discard) _LNGraphics_GetActiveGraphicsAPI %1
#cmd LNTexture_SetPrototype_OnSerialize $A90
#cmd _LNTexture2D_Create $1DA
#define LNTexture2D_Create(%1, %2, %3=_ln_return_discard) _LNTexture2D_Create %1, %2, %3
#cmd _LNTexture2D_CreateWithFormat $1DD
#define LNTexture2D_CreateWithFormat(%1, %2, %3, %4=_ln_return_discard) _LNTexture2D_CreateWithFormat %1, %2, %3, %4
#cmd _LNTexture2D_Load $1D6
#define LNTexture2D_Load(%1, %2=_ln_return_discard) _LNTexture2D_Load %1, %2
#cmd _LNTexture2D_LoadEmoji $1D8
#define LNTexture2D_LoadEmoji(%1, %2=_ln_return_discard) _LNTexture2D_LoadEmoji %1, %2
#cmd LNTexture2D_SetPrototype_OnSerialize $A9B
#cmd _LNShader_Load $1E2
#define LNShader_Load(%1, %2, %3=_ln_return_discard) _LNShader_Load %1, %2, %3
#cmd _LNShader_SetFloat $1E5
#define LNShader_SetFloat(%1, %2, %3) _LNShader_SetFloat %1, %2, %3
#cmd _LNShader_SetVector3 $1E8
#define LNShader_SetVector3(%1, %2, %3) _LNShader_SetVector3 %1, %2, %3
#cmd _LNShader_SetVector4 $1EB
#define LNShader_SetVector4(%1, %2, %3) _LNShader_SetVector4 %1, %2, %3
#cmd _LNShader_SetTexture $1EE
#define LNShader_SetTexture(%1, %2, %3) _LNShader_SetTexture %1, %2, %3
#cmd LNShader_SetPrototype_OnSerialize $AA6
#cmd LNRenderView_SetPrototype_OnSerialize $AB1
#cmd _LNMaterial_Create $213
#define LNMaterial_Create(%1=_ln_return_discard) _LNMaterial_Create %1
#cmd _LNMaterial_SetMainTexture $202
#define LNMaterial_SetMainTexture(%1, %2) _LNMaterial_SetMainTexture %1, %2
#cmd _LNMaterial_GetMainTexture $204
#define LNMaterial_GetMainTexture(%1, %2=_ln_return_discard) _LNMaterial_GetMainTexture %1, %2
#cmd _LNMaterial_SetColor $205
#define LNMaterial_SetColor(%1, %2) _LNMaterial_SetColor %1, %2
#cmd _LNMaterial_SetRoughness $207
#define LNMaterial_SetRoughness(%1, %2) _LNMaterial_SetRoughness %1, %2
#cmd _LNMaterial_SetMetallic $209
#define LNMaterial_SetMetallic(%1, %2) _LNMaterial_SetMetallic %1, %2
#cmd _LNMaterial_SetEmissive $20B
#define LNMaterial_SetEmissive(%1, %2) _LNMaterial_SetEmissive %1, %2
#cmd _LNMaterial_SetShadingModel $20D
#define LNMaterial_SetShadingModel(%1, %2) _LNMaterial_SetShadingModel %1, %2
#cmd _LNMaterial_GetShadingModel $20F
#define LNMaterial_GetShadingModel(%1, %2=_ln_return_discard) _LNMaterial_GetShadingModel %1, %2
#cmd _LNMaterial_SetShader $210
#define LNMaterial_SetShader(%1, %2) _LNMaterial_SetShader %1, %2
#cmd _LNMaterial_GetShader $212
#define LNMaterial_GetShader(%1, %2=_ln_return_discard) _LNMaterial_GetShader %1, %2
#cmd LNMaterial_SetPrototype_OnSerialize $ABC
#cmd _LNMeshNode_SetVisible $215
#define LNMeshNode_SetVisible(%1, %2) _LNMeshNode_SetVisible %1, %2
#cmd _LNMeshNode_IsVisible $217
#define LNMeshNode_IsVisible(%1, %2=_ln_return_discard) _LNMeshNode_IsVisible %1, %2
#cmd LNMeshNode_SetPrototype_OnSerialize $AC7
#cmd _LNAnimationController_AddClip $219
#define LNAnimationController_AddClip(%1, %2, %3=_ln_return_discard) _LNAnimationController_AddClip %1, %2, %3
#cmd _LNAnimationController_Play $21B
#define LNAnimationController_Play(%1, %2, %3) _LNAnimationController_Play %1, %2, %3
#cmd LNAnimationController_SetPrototype_OnSerialize $AD2
#cmd _LNMeshModel_Load $21F
#define LNMeshModel_Load(%1, %2, %3=_ln_return_discard) _LNMeshModel_Load %1, %2, %3
#cmd _LNMeshModel_FindNode $222
#define LNMeshModel_FindNode(%1, %2, %3=_ln_return_discard) _LNMeshModel_FindNode %1, %2, %3
#cmd _LNMeshModel_FindMaterial $224
#define LNMeshModel_FindMaterial(%1, %2, %3=_ln_return_discard) _LNMeshModel_FindMaterial %1, %2, %3
#cmd _LNMeshModel_MaterialCount $226
#define LNMeshModel_MaterialCount(%1, %2=_ln_return_discard) _LNMeshModel_MaterialCount %1, %2
#cmd _LNMeshModel_Material $227
#define LNMeshModel_Material(%1, %2, %3=_ln_return_discard) _LNMeshModel_Material %1, %2, %3
#cmd _LNMeshModel_GetAnimationController $229
#define LNMeshModel_GetAnimationController(%1, %2=_ln_return_discard) _LNMeshModel_GetAnimationController %1, %2
#cmd LNMeshModel_SetPrototype_OnSerialize $ADD
#cmd _LNMeshImportSettings_Create $22B
#define LNMeshImportSettings_Create(%1=_ln_return_discard) _LNMeshImportSettings_Create %1
#cmd LNMeshImportSettings_SetPrototype_OnSerialize $AE8
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $AF3
#cmd LNCollisionShape_SetPrototype_OnSerialize $AFE
#cmd _LNBoxCollisionShape_Create $22F
#define LNBoxCollisionShape_Create(%1, %2=_ln_return_discard) _LNBoxCollisionShape_Create %1, %2
#cmd _LNBoxCollisionShape_CreateWHD $231
#define LNBoxCollisionShape_CreateWHD(%1, %2, %3, %4=_ln_return_discard) _LNBoxCollisionShape_CreateWHD %1, %2, %3, %4
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $B09
#cmd _LNAnimationCurve_Evaluate $23F
#define LNAnimationCurve_Evaluate(%1, %2, %3=_ln_return_discard) _LNAnimationCurve_Evaluate %1, %2, %3
#cmd LNAnimationCurve_SetPrototype_OnSerialize $B14
#cmd _LNKeyFrameAnimationCurve_Create $24C
#define LNKeyFrameAnimationCurve_Create(%1=_ln_return_discard) _LNKeyFrameAnimationCurve_Create %1
#cmd _LNKeyFrameAnimationCurve_AddKeyFrame $247
#define LNKeyFrameAnimationCurve_AddKeyFrame(%1, %2, %3, %4, %5) _LNKeyFrameAnimationCurve_AddKeyFrame %1, %2, %3, %4, %5
#cmd LNKeyFrameAnimationCurve_SetPrototype_OnSerialize $B1F
#cmd _LNAnimationClip_Load $24E
#define LNAnimationClip_Load(%1, %2=_ln_return_discard) _LNAnimationClip_Load %1, %2
#cmd _LNAnimationClip_SetWrapMode $250
#define LNAnimationClip_SetWrapMode(%1, %2) _LNAnimationClip_SetWrapMode %1, %2
#cmd _LNAnimationClip_GetWrapMode $252
#define LNAnimationClip_GetWrapMode(%1, %2=_ln_return_discard) _LNAnimationClip_GetWrapMode %1, %2
#cmd _LNAnimationClip_SetHierarchicalAnimationMode $253
#define LNAnimationClip_SetHierarchicalAnimationMode(%1, %2) _LNAnimationClip_SetHierarchicalAnimationMode %1, %2
#cmd _LNAnimationClip_GetHierarchicalAnimationMode $255
#define LNAnimationClip_GetHierarchicalAnimationMode(%1, %2=_ln_return_discard) _LNAnimationClip_GetHierarchicalAnimationMode %1, %2
#cmd LNAnimationClip_SetPrototype_OnSerialize $B2A
#cmd LNAnimationState_SetPrototype_OnSerialize $B35
#cmd LNEffectResource_SetPrototype_OnSerialize $B40
#cmd _LNParticleEmitterModel_Create $27A
#define LNParticleEmitterModel_Create(%1=_ln_return_discard) _LNParticleEmitterModel_Create %1
#cmd _LNParticleEmitterModel_SetMaxParticles $262
#define LNParticleEmitterModel_SetMaxParticles(%1, %2) _LNParticleEmitterModel_SetMaxParticles %1, %2
#cmd _LNParticleEmitterModel_SetSpawnRate $264
#define LNParticleEmitterModel_SetSpawnRate(%1, %2) _LNParticleEmitterModel_SetSpawnRate %1, %2
#cmd _LNParticleEmitterModel_SetLifeTime $266
#define LNParticleEmitterModel_SetLifeTime(%1, %2) _LNParticleEmitterModel_SetLifeTime %1, %2
#cmd _LNParticleEmitterModel_SetupBoxShape $268
#define LNParticleEmitterModel_SetupBoxShape(%1, %2) _LNParticleEmitterModel_SetupBoxShape %1, %2
#cmd _LNParticleEmitterModel_SetSize $26A
#define LNParticleEmitterModel_SetSize(%1, %2) _LNParticleEmitterModel_SetSize %1, %2
#cmd _LNParticleEmitterModel_SetSizeVelocity $26C
#define LNParticleEmitterModel_SetSizeVelocity(%1, %2) _LNParticleEmitterModel_SetSizeVelocity %1, %2
#cmd _LNParticleEmitterModel_SetSizeAcceleration $26E
#define LNParticleEmitterModel_SetSizeAcceleration(%1, %2) _LNParticleEmitterModel_SetSizeAcceleration %1, %2
#cmd _LNParticleEmitterModel_SetForwardVelocityMin $270
#define LNParticleEmitterModel_SetForwardVelocityMin(%1, %2) _LNParticleEmitterModel_SetForwardVelocityMin %1, %2
#cmd _LNParticleEmitterModel_SetForwardVelocityMax $272
#define LNParticleEmitterModel_SetForwardVelocityMax(%1, %2) _LNParticleEmitterModel_SetForwardVelocityMax %1, %2
#cmd _LNParticleEmitterModel_SetForwardScale $274
#define LNParticleEmitterModel_SetForwardScale(%1, %2) _LNParticleEmitterModel_SetForwardScale %1, %2
#cmd _LNParticleEmitterModel_SetGeometryDirection $276
#define LNParticleEmitterModel_SetGeometryDirection(%1, %2) _LNParticleEmitterModel_SetGeometryDirection %1, %2
#cmd _LNParticleEmitterModel_SetupSpriteModule $278
#define LNParticleEmitterModel_SetupSpriteModule(%1, %2) _LNParticleEmitterModel_SetupSpriteModule %1, %2
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $B4B
#cmd _LNParticleModel_Create $281
#define LNParticleModel_Create(%1=_ln_return_discard) _LNParticleModel_Create %1
#cmd _LNParticleModel_SetLoop $27C
#define LNParticleModel_SetLoop(%1, %2) _LNParticleModel_SetLoop %1, %2
#cmd _LNParticleModel_IsLoop $27E
#define LNParticleModel_IsLoop(%1, %2=_ln_return_discard) _LNParticleModel_IsLoop %1, %2
#cmd _LNParticleModel_AddEmitter $27F
#define LNParticleModel_AddEmitter(%1, %2) _LNParticleModel_AddEmitter %1, %2
#cmd LNParticleModel_SetPrototype_OnSerialize $B56
#cmd LNComponent_SetPrototype_OnSerialize $B61
#cmd _LNVisualComponent_SetVisible $284
#define LNVisualComponent_SetVisible(%1, %2) _LNVisualComponent_SetVisible %1, %2
#cmd _LNVisualComponent_IsVisible $286
#define LNVisualComponent_IsVisible(%1, %2=_ln_return_discard) _LNVisualComponent_IsVisible %1, %2
#cmd LNVisualComponent_SetPrototype_OnSerialize $B6C
#cmd _LNSpriteComponent_SetTexture $288
#define LNSpriteComponent_SetTexture(%1, %2) _LNSpriteComponent_SetTexture %1, %2
#cmd LNSpriteComponent_SetPrototype_OnSerialize $B77
#cmd _LNCollisionEventHandler_Create $7D4
#define LNCollisionEventHandler_Create(%1, %2=_ln_return_discard) _LNCollisionEventHandler_Create %1, %2
#cmd _LNCharacterController_Create $2A8
#define LNCharacterController_Create(%1=_ln_return_discard) _LNCharacterController_Create %1
#cmd _LNCharacterController_SetWalkVelocity $292
#define LNCharacterController_SetWalkVelocity(%1, %2) _LNCharacterController_SetWalkVelocity %1, %2
#cmd _LNCharacterController_GetWalkVelocity $294
#define LNCharacterController_GetWalkVelocity(%1, %2=_ln_return_discard) _LNCharacterController_GetWalkVelocity %1, %2
#cmd _LNCharacterController_SetVelocity $295
#define LNCharacterController_SetVelocity(%1, %2) _LNCharacterController_SetVelocity %1, %2
#cmd _LNCharacterController_GetVelocity $297
#define LNCharacterController_GetVelocity(%1, %2=_ln_return_discard) _LNCharacterController_GetVelocity %1, %2
#cmd _LNCharacterController_SetInputControlEnabled $298
#define LNCharacterController_SetInputControlEnabled(%1, %2) _LNCharacterController_SetInputControlEnabled %1, %2
#cmd _LNCharacterController_SetCameraControlEnabled $29A
#define LNCharacterController_SetCameraControlEnabled(%1, %2) _LNCharacterController_SetCameraControlEnabled %1, %2
#cmd _LNCharacterController_SetHeight $29C
#define LNCharacterController_SetHeight(%1, %2) _LNCharacterController_SetHeight %1, %2
#cmd _LNCharacterController_GetHeight $29E
#define LNCharacterController_GetHeight(%1, %2=_ln_return_discard) _LNCharacterController_GetHeight %1, %2
#cmd _LNCharacterController_SetCameraRadius $29F
#define LNCharacterController_SetCameraRadius(%1, %2) _LNCharacterController_SetCameraRadius %1, %2
#cmd _LNCharacterController_GetCameraRadius $2A1
#define LNCharacterController_GetCameraRadius(%1, %2=_ln_return_discard) _LNCharacterController_GetCameraRadius %1, %2
#cmd _LNCharacterController_SetCollisionEnter $2A2
#define LNCharacterController_SetCollisionEnter(%1, %2) _LNCharacterController_SetCollisionEnter %1, %2
#cmd _LNCharacterController_SetCollisionLeave $2A4
#define LNCharacterController_SetCollisionLeave(%1, %2) _LNCharacterController_SetCollisionLeave %1, %2
#cmd _LNCharacterController_SetCollisionStay $2A6
#define LNCharacterController_SetCollisionStay(%1, %2) _LNCharacterController_SetCollisionStay %1, %2
#cmd LNCharacterController_SetPrototype_OnSerialize $B82
#cmd _LNWorld_Add $2AA
#define LNWorld_Add(%1, %2) _LNWorld_Add %1, %2
#cmd LNWorld_SetPrototype_OnSerialize $B8D
#cmd _LNComponentList_GetLength $7F4
#define LNComponentList_GetLength(%1, %2=_ln_return_discard) _LNComponentList_GetLength %1, %2
#cmd _LNComponentList_GetItem $7F6
#define LNComponentList_GetItem(%1, %2, %3=_ln_return_discard) _LNComponentList_GetItem %1, %2, %3
#cmd LNComponentList_SetPrototype_OnSerialize $B98
#cmd _LNWorldObject_Create $2E1
#define LNWorldObject_Create(%1=_ln_return_discard) _LNWorldObject_Create %1
#cmd _LNWorldObject_SetPosition $2AE
#define LNWorldObject_SetPosition(%1, %2) _LNWorldObject_SetPosition %1, %2
#cmd _LNWorldObject_SetPositionXYZ $2B0
#define LNWorldObject_SetPositionXYZ(%1, %2, %3, %4) _LNWorldObject_SetPositionXYZ %1, %2, %3, %4
#cmd _LNWorldObject_GetPosition $2B4
#define LNWorldObject_GetPosition(%1, %2=_ln_return_discard) _LNWorldObject_GetPosition %1, %2
#cmd _LNWorldObject_SetRotationQuaternion $2B5
#define LNWorldObject_SetRotationQuaternion(%1, %2) _LNWorldObject_SetRotationQuaternion %1, %2
#cmd _LNWorldObject_SetRotation $2B7
#define LNWorldObject_SetRotation(%1, %2, %3, %4) _LNWorldObject_SetRotation %1, %2, %3, %4
#cmd _LNWorldObject_GetRotation $2BB
#define LNWorldObject_GetRotation(%1, %2=_ln_return_discard) _LNWorldObject_GetRotation %1, %2
#cmd _LNWorldObject_SetScale $2BC
#define LNWorldObject_SetScale(%1, %2) _LNWorldObject_SetScale %1, %2
#cmd _LNWorldObject_SetScaleS $2BE
#define LNWorldObject_SetScaleS(%1, %2) _LNWorldObject_SetScaleS %1, %2
#cmd _LNWorldObject_SetScaleXYZ $2C0
#define LNWorldObject_SetScaleXYZ(%1, %2, %3, %4) _LNWorldObject_SetScaleXYZ %1, %2, %3, %4
#cmd _LNWorldObject_GetScale $2C4
#define LNWorldObject_GetScale(%1, %2=_ln_return_discard) _LNWorldObject_GetScale %1, %2
#cmd _LNWorldObject_SetCenterPoint $2C5
#define LNWorldObject_SetCenterPoint(%1, %2) _LNWorldObject_SetCenterPoint %1, %2
#cmd _LNWorldObject_SetCenterPointXYZ $2C7
#define LNWorldObject_SetCenterPointXYZ(%1, %2, %3, %4) _LNWorldObject_SetCenterPointXYZ %1, %2, %3, %4
#cmd _LNWorldObject_GetCenterPoint $2CB
#define LNWorldObject_GetCenterPoint(%1, %2=_ln_return_discard) _LNWorldObject_GetCenterPoint %1, %2
#cmd _LNWorldObject_LookAt $2CC
#define LNWorldObject_LookAt(%1, %2) _LNWorldObject_LookAt %1, %2
#cmd _LNWorldObject_LookAtXYZ $2CE
#define LNWorldObject_LookAtXYZ(%1, %2, %3, %4) _LNWorldObject_LookAtXYZ %1, %2, %3, %4
#cmd _LNWorldObject_AddComponent $2D2
#define LNWorldObject_AddComponent(%1, %2) _LNWorldObject_AddComponent %1, %2
#cmd _LNWorldObject_RemoveComponent $2D4
#define LNWorldObject_RemoveComponent(%1, %2) _LNWorldObject_RemoveComponent %1, %2
#cmd _LNWorldObject_AddTag $2D6
#define LNWorldObject_AddTag(%1, %2) _LNWorldObject_AddTag %1, %2
#cmd _LNWorldObject_RemoveTag $2D8
#define LNWorldObject_RemoveTag(%1, %2) _LNWorldObject_RemoveTag %1, %2
#cmd _LNWorldObject_HasTag $2DA
#define LNWorldObject_HasTag(%1, %2, %3=_ln_return_discard) _LNWorldObject_HasTag %1, %2, %3
#cmd _LNWorldObject_Destroy $2DC
#define LNWorldObject_Destroy(%1) _LNWorldObject_Destroy %1
#cmd _LNWorldObject_GetComponents $2DD
#define LNWorldObject_GetComponents(%1, %2=_ln_return_discard) _LNWorldObject_GetComponents %1, %2
#cmd LNWorldObject_SetPrototype_OnSerialize $BA3
#cmd LNWorldObject_SetPrototype_OnPreUpdate $BAE
#cmd LNWorldObject_SetPrototype_OnUpdate $BB9
#cmd _LNVisualObject_SetVisible $2E3
#define LNVisualObject_SetVisible(%1, %2) _LNVisualObject_SetVisible %1, %2
#cmd _LNVisualObject_IsVisible $2E5
#define LNVisualObject_IsVisible(%1, %2=_ln_return_discard) _LNVisualObject_IsVisible %1, %2
#cmd _LNVisualObject_SetBlendMode2 $2E6
#define LNVisualObject_SetBlendMode2(%1, %2) _LNVisualObject_SetBlendMode2 %1, %2
#cmd _LNVisualObject_SetOpacity $2E8
#define LNVisualObject_SetOpacity(%1, %2) _LNVisualObject_SetOpacity %1, %2
#cmd _LNVisualObject_GetOpacity $2EA
#define LNVisualObject_GetOpacity(%1, %2=_ln_return_discard) _LNVisualObject_GetOpacity %1, %2
#cmd LNVisualObject_SetPrototype_OnSerialize $BC4
#cmd LNVisualObject_SetPrototype_OnPreUpdate $BCF
#cmd LNVisualObject_SetPrototype_OnUpdate $BDA
#cmd LNCamera_SetPrototype_OnSerialize $BE5
#cmd LNCamera_SetPrototype_OnPreUpdate $BF0
#cmd LNCamera_SetPrototype_OnUpdate $BFB
#cmd _LNEnvironmentLight_SetEnabled $2ED
#define LNEnvironmentLight_SetEnabled(%1, %2) _LNEnvironmentLight_SetEnabled %1, %2
#cmd _LNEnvironmentLight_IsEnabled $2EF
#define LNEnvironmentLight_IsEnabled(%1, %2=_ln_return_discard) _LNEnvironmentLight_IsEnabled %1, %2
#cmd _LNEnvironmentLight_SetColor $2F0
#define LNEnvironmentLight_SetColor(%1, %2) _LNEnvironmentLight_SetColor %1, %2
#cmd _LNEnvironmentLight_GetColor $2F2
#define LNEnvironmentLight_GetColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetColor %1, %2
#cmd _LNEnvironmentLight_SetAmbientColor $2F3
#define LNEnvironmentLight_SetAmbientColor(%1, %2) _LNEnvironmentLight_SetAmbientColor %1, %2
#cmd _LNEnvironmentLight_GetAmbientColor $2F5
#define LNEnvironmentLight_GetAmbientColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetAmbientColor %1, %2
#cmd _LNEnvironmentLight_GetSkyColor $2F6
#define LNEnvironmentLight_GetSkyColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetSkyColor %1, %2
#cmd _LNEnvironmentLight_SetSkyColor $2F7
#define LNEnvironmentLight_SetSkyColor(%1, %2) _LNEnvironmentLight_SetSkyColor %1, %2
#cmd _LNEnvironmentLight_GetGroundColor $2F9
#define LNEnvironmentLight_GetGroundColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetGroundColor %1, %2
#cmd _LNEnvironmentLight_SetGroundColor $2FA
#define LNEnvironmentLight_SetGroundColor(%1, %2) _LNEnvironmentLight_SetGroundColor %1, %2
#cmd _LNEnvironmentLight_SetIntensity $2FC
#define LNEnvironmentLight_SetIntensity(%1, %2) _LNEnvironmentLight_SetIntensity %1, %2
#cmd _LNEnvironmentLight_GetIntensity $2FE
#define LNEnvironmentLight_GetIntensity(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetIntensity %1, %2
#cmd _LNEnvironmentLight_SetShadowEffectiveDistance $2FF
#define LNEnvironmentLight_SetShadowEffectiveDistance(%1, %2) _LNEnvironmentLight_SetShadowEffectiveDistance %1, %2
#cmd _LNEnvironmentLight_GetShadowEffectiveDistance $301
#define LNEnvironmentLight_GetShadowEffectiveDistance(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetShadowEffectiveDistance %1, %2
#cmd _LNEnvironmentLight_SetShadowEffectiveDepth $302
#define LNEnvironmentLight_SetShadowEffectiveDepth(%1, %2) _LNEnvironmentLight_SetShadowEffectiveDepth %1, %2
#cmd _LNEnvironmentLight_GetShadowEffectiveDepth $304
#define LNEnvironmentLight_GetShadowEffectiveDepth(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetShadowEffectiveDepth %1, %2
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $C06
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $C11
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $C1C
#cmd _LNDirectionalLight_Create $315
#define LNDirectionalLight_Create(%1=_ln_return_discard) _LNDirectionalLight_Create %1
#cmd _LNDirectionalLight_CreateWithColor $316
#define LNDirectionalLight_CreateWithColor(%1, %2=_ln_return_discard) _LNDirectionalLight_CreateWithColor %1, %2
#cmd _LNDirectionalLight_SetEnabled $306
#define LNDirectionalLight_SetEnabled(%1, %2) _LNDirectionalLight_SetEnabled %1, %2
#cmd _LNDirectionalLight_IsEnabled $308
#define LNDirectionalLight_IsEnabled(%1, %2=_ln_return_discard) _LNDirectionalLight_IsEnabled %1, %2
#cmd _LNDirectionalLight_SetColor $309
#define LNDirectionalLight_SetColor(%1, %2) _LNDirectionalLight_SetColor %1, %2
#cmd _LNDirectionalLight_GetColor $30B
#define LNDirectionalLight_GetColor(%1, %2=_ln_return_discard) _LNDirectionalLight_GetColor %1, %2
#cmd _LNDirectionalLight_SetIntensity $30C
#define LNDirectionalLight_SetIntensity(%1, %2) _LNDirectionalLight_SetIntensity %1, %2
#cmd _LNDirectionalLight_GetIntensity $30E
#define LNDirectionalLight_GetIntensity(%1, %2=_ln_return_discard) _LNDirectionalLight_GetIntensity %1, %2
#cmd _LNDirectionalLight_SetShadowEffectiveDistance $30F
#define LNDirectionalLight_SetShadowEffectiveDistance(%1, %2) _LNDirectionalLight_SetShadowEffectiveDistance %1, %2
#cmd _LNDirectionalLight_GetShadowEffectiveDistance $311
#define LNDirectionalLight_GetShadowEffectiveDistance(%1, %2=_ln_return_discard) _LNDirectionalLight_GetShadowEffectiveDistance %1, %2
#cmd _LNDirectionalLight_SetShadowEffectiveDepth $312
#define LNDirectionalLight_SetShadowEffectiveDepth(%1, %2) _LNDirectionalLight_SetShadowEffectiveDepth %1, %2
#cmd _LNDirectionalLight_GetShadowEffectiveDepth $314
#define LNDirectionalLight_GetShadowEffectiveDepth(%1, %2=_ln_return_discard) _LNDirectionalLight_GetShadowEffectiveDepth %1, %2
#cmd LNDirectionalLight_SetPrototype_OnSerialize $C27
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $C32
#cmd LNDirectionalLight_SetPrototype_OnUpdate $C3D
#cmd _LNPointLight_Create $328
#define LNPointLight_Create(%1=_ln_return_discard) _LNPointLight_Create %1
#cmd _LNPointLight_CreateWithColorAndRange $329
#define LNPointLight_CreateWithColorAndRange(%1, %2, %3=_ln_return_discard) _LNPointLight_CreateWithColorAndRange %1, %2, %3
#cmd _LNPointLight_SetEnabled $319
#define LNPointLight_SetEnabled(%1, %2) _LNPointLight_SetEnabled %1, %2
#cmd _LNPointLight_IsEnabled $31B
#define LNPointLight_IsEnabled(%1, %2=_ln_return_discard) _LNPointLight_IsEnabled %1, %2
#cmd _LNPointLight_SetColor $31C
#define LNPointLight_SetColor(%1, %2) _LNPointLight_SetColor %1, %2
#cmd _LNPointLight_GetColor $31E
#define LNPointLight_GetColor(%1, %2=_ln_return_discard) _LNPointLight_GetColor %1, %2
#cmd _LNPointLight_SetIntensity $31F
#define LNPointLight_SetIntensity(%1, %2) _LNPointLight_SetIntensity %1, %2
#cmd _LNPointLight_GetIntensity $321
#define LNPointLight_GetIntensity(%1, %2=_ln_return_discard) _LNPointLight_GetIntensity %1, %2
#cmd _LNPointLight_SetRange $322
#define LNPointLight_SetRange(%1, %2) _LNPointLight_SetRange %1, %2
#cmd _LNPointLight_GetRange $324
#define LNPointLight_GetRange(%1, %2=_ln_return_discard) _LNPointLight_GetRange %1, %2
#cmd _LNPointLight_SetAttenuation $325
#define LNPointLight_SetAttenuation(%1, %2) _LNPointLight_SetAttenuation %1, %2
#cmd _LNPointLight_GetAttenuation $327
#define LNPointLight_GetAttenuation(%1, %2=_ln_return_discard) _LNPointLight_GetAttenuation %1, %2
#cmd LNPointLight_SetPrototype_OnSerialize $C48
#cmd LNPointLight_SetPrototype_OnPreUpdate $C53
#cmd LNPointLight_SetPrototype_OnUpdate $C5E
#cmd _LNSpotLight_Create $342
#define LNSpotLight_Create(%1=_ln_return_discard) _LNSpotLight_Create %1
#cmd _LNSpotLight_CreateWithColorAndRange $343
#define LNSpotLight_CreateWithColorAndRange(%1, %2, %3, %4=_ln_return_discard) _LNSpotLight_CreateWithColorAndRange %1, %2, %3, %4
#cmd _LNSpotLight_SetEnabled $32D
#define LNSpotLight_SetEnabled(%1, %2) _LNSpotLight_SetEnabled %1, %2
#cmd _LNSpotLight_IsEnabled $32F
#define LNSpotLight_IsEnabled(%1, %2=_ln_return_discard) _LNSpotLight_IsEnabled %1, %2
#cmd _LNSpotLight_SetColor $330
#define LNSpotLight_SetColor(%1, %2) _LNSpotLight_SetColor %1, %2
#cmd _LNSpotLight_GetColor $332
#define LNSpotLight_GetColor(%1, %2=_ln_return_discard) _LNSpotLight_GetColor %1, %2
#cmd _LNSpotLight_SetIntensity $333
#define LNSpotLight_SetIntensity(%1, %2) _LNSpotLight_SetIntensity %1, %2
#cmd _LNSpotLight_GetIntensity $335
#define LNSpotLight_GetIntensity(%1, %2=_ln_return_discard) _LNSpotLight_GetIntensity %1, %2
#cmd _LNSpotLight_SetRange $336
#define LNSpotLight_SetRange(%1, %2) _LNSpotLight_SetRange %1, %2
#cmd _LNSpotLight_GetRange $338
#define LNSpotLight_GetRange(%1, %2=_ln_return_discard) _LNSpotLight_GetRange %1, %2
#cmd _LNSpotLight_SetAttenuation $339
#define LNSpotLight_SetAttenuation(%1, %2) _LNSpotLight_SetAttenuation %1, %2
#cmd _LNSpotLight_GetAttenuation $33B
#define LNSpotLight_GetAttenuation(%1, %2=_ln_return_discard) _LNSpotLight_GetAttenuation %1, %2
#cmd _LNSpotLight_SetAngle $33C
#define LNSpotLight_SetAngle(%1, %2) _LNSpotLight_SetAngle %1, %2
#cmd _LNSpotLight_GetAngle $33E
#define LNSpotLight_GetAngle(%1, %2=_ln_return_discard) _LNSpotLight_GetAngle %1, %2
#cmd _LNSpotLight_SetPenumbra $33F
#define LNSpotLight_SetPenumbra(%1, %2) _LNSpotLight_SetPenumbra %1, %2
#cmd _LNSpotLight_GetPenumbra $341
#define LNSpotLight_GetPenumbra(%1, %2=_ln_return_discard) _LNSpotLight_GetPenumbra %1, %2
#cmd LNSpotLight_SetPrototype_OnSerialize $C69
#cmd LNSpotLight_SetPrototype_OnPreUpdate $C74
#cmd LNSpotLight_SetPrototype_OnUpdate $C7F
#cmd _LNTestDelegate_Create $8A2
#define LNTestDelegate_Create(%1, %2=_ln_return_discard) _LNTestDelegate_Create %1, %2
#cmd _LNSprite_Create $359
#define LNSprite_Create(%1=_ln_return_discard) _LNSprite_Create %1
#cmd _LNSprite_CreateWithTexture $35A
#define LNSprite_CreateWithTexture(%1, %2=_ln_return_discard) _LNSprite_CreateWithTexture %1, %2
#cmd _LNSprite_CreateWithTextureAndSize $35C
#define LNSprite_CreateWithTextureAndSize(%1, %2, %3, %4=_ln_return_discard) _LNSprite_CreateWithTextureAndSize %1, %2, %3, %4
#cmd _LNSprite_SetTexture $34B
#define LNSprite_SetTexture(%1, %2) _LNSprite_SetTexture %1, %2
#cmd _LNSprite_SetSize $34D
#define LNSprite_SetSize(%1, %2) _LNSprite_SetSize %1, %2
#cmd _LNSprite_SetSizeWH $34F
#define LNSprite_SetSizeWH(%1, %2, %3) _LNSprite_SetSizeWH %1, %2, %3
#cmd _LNSprite_SetSourceRectXYWH $352
#define LNSprite_SetSourceRectXYWH(%1, %2, %3, %4, %5) _LNSprite_SetSourceRectXYWH %1, %2, %3, %4, %5
#cmd _LNSprite_SetCallerTest $357
#define LNSprite_SetCallerTest(%1, %2) _LNSprite_SetCallerTest %1, %2
#cmd LNSprite_SetPrototype_OnSerialize $C8A
#cmd LNSprite_SetPrototype_OnPreUpdate $C95
#cmd LNSprite_SetPrototype_OnUpdate $CA0
#cmd _LNCameraOrbitControlComponent_Create $361
#define LNCameraOrbitControlComponent_Create(%1=_ln_return_discard) _LNCameraOrbitControlComponent_Create %1
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $CAB
#cmd _LNRaycaster_FromScreen $363
#define LNRaycaster_FromScreen(%1, %2=_ln_return_discard) _LNRaycaster_FromScreen %1, %2
#cmd _LNRaycaster_IntersectPlane $365
#define LNRaycaster_IntersectPlane(%1, %2, %3, %4, %5=_ln_return_discard) _LNRaycaster_IntersectPlane %1, %2, %3, %4, %5
#cmd LNRaycaster_SetPrototype_OnSerialize $CB6
#cmd _LNRaycastResult_GetPoint $36A
#define LNRaycastResult_GetPoint(%1, %2=_ln_return_discard) _LNRaycastResult_GetPoint %1, %2
#cmd LNRaycastResult_SetPrototype_OnSerialize $CC1
#cmd _LNWorldRenderView_SetGuideGridEnabled $36C
#define LNWorldRenderView_SetGuideGridEnabled(%1, %2) _LNWorldRenderView_SetGuideGridEnabled %1, %2
#cmd _LNWorldRenderView_GetGuideGridEnabled $36E
#define LNWorldRenderView_GetGuideGridEnabled(%1, %2=_ln_return_discard) _LNWorldRenderView_GetGuideGridEnabled %1, %2
#cmd LNWorldRenderView_SetPrototype_OnSerialize $CCC
#cmd _LNBoxMesh_Create $370
#define LNBoxMesh_Create(%1=_ln_return_discard) _LNBoxMesh_Create %1
#cmd _LNBoxMesh_CreateXYZ $371
#define LNBoxMesh_CreateXYZ(%1, %2, %3, %4=_ln_return_discard) _LNBoxMesh_CreateXYZ %1, %2, %3, %4
#cmd LNBoxMesh_SetPrototype_OnSerialize $CD7
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $CE2
#cmd LNBoxMesh_SetPrototype_OnUpdate $CED
#cmd _LNPlaneMesh_Create $376
#define LNPlaneMesh_Create(%1=_ln_return_discard) _LNPlaneMesh_Create %1
#cmd LNPlaneMesh_SetPrototype_OnSerialize $CF8
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $D03
#cmd LNPlaneMesh_SetPrototype_OnUpdate $D0E
#cmd _LNMeshComponent_Create $37C
#define LNMeshComponent_Create(%1=_ln_return_discard) _LNMeshComponent_Create %1
#cmd _LNMeshComponent_SetModel $378
#define LNMeshComponent_SetModel(%1, %2) _LNMeshComponent_SetModel %1, %2
#cmd _LNMeshComponent_MakeCollisionBody $37A
#define LNMeshComponent_MakeCollisionBody(%1, %2) _LNMeshComponent_MakeCollisionBody %1, %2
#cmd LNMeshComponent_SetPrototype_OnSerialize $D19
#cmd _LNCollision_GetWorldObject $37E
#define LNCollision_GetWorldObject(%1, %2=_ln_return_discard) _LNCollision_GetWorldObject %1, %2
#cmd LNCollision_SetPrototype_OnSerialize $D24
#cmd _LNTriggerBodyComponent_Create $382
#define LNTriggerBodyComponent_Create(%1=_ln_return_discard) _LNTriggerBodyComponent_Create %1
#cmd _LNTriggerBodyComponent_AddCollisionShape $380
#define LNTriggerBodyComponent_AddCollisionShape(%1, %2) _LNTriggerBodyComponent_AddCollisionShape %1, %2
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $D2F
#cmd _LNParticleEmitter_Create $384
#define LNParticleEmitter_Create(%1, %2=_ln_return_discard) _LNParticleEmitter_Create %1, %2
#cmd LNParticleEmitter_SetPrototype_OnSerialize $D3A
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $D45
#cmd LNParticleEmitter_SetPrototype_OnUpdate $D50
#cmd _LNScene_SetClearMode $387
#define LNScene_SetClearMode(%1) _LNScene_SetClearMode %1
#cmd _LNScene_SetSkyColor $389
#define LNScene_SetSkyColor(%1) _LNScene_SetSkyColor %1
#cmd _LNScene_SetSkyHorizonColor $38B
#define LNScene_SetSkyHorizonColor(%1) _LNScene_SetSkyHorizonColor %1
#cmd _LNScene_SetSkyCloudColor $38D
#define LNScene_SetSkyCloudColor(%1) _LNScene_SetSkyCloudColor %1
#cmd _LNScene_SetSkyOverlayColor $38F
#define LNScene_SetSkyOverlayColor(%1) _LNScene_SetSkyOverlayColor %1
#cmd _LNScene_GotoLevel $391
#define LNScene_GotoLevel(%1, %2) _LNScene_GotoLevel %1, %2
#cmd _LNScene_CallLevel $394
#define LNScene_CallLevel(%1, %2) _LNScene_CallLevel %1, %2
#cmd _LNScene_ReturnLevel $397
#define LNScene_ReturnLevel(%1) _LNScene_ReturnLevel %1
#cmd _LNScene_ActiveLevel $399
#define LNScene_ActiveLevel(%1=_ln_return_discard) _LNScene_ActiveLevel %1
#cmd _LNScene_IsTransitionEffectRunning $39A
#define LNScene_IsTransitionEffectRunning(%1=_ln_return_discard) _LNScene_IsTransitionEffectRunning %1
#cmd _LNScene_SetTransitionEffectMode $39B
#define LNScene_SetTransitionEffectMode(%1) _LNScene_SetTransitionEffectMode %1
#cmd _LNScene_TransitionEffectMode $39D
#define LNScene_TransitionEffectMode(%1=_ln_return_discard) _LNScene_TransitionEffectMode %1
#cmd _LNScene_SetTransitionDuration $39E
#define LNScene_SetTransitionDuration(%1) _LNScene_SetTransitionDuration %1
#cmd _LNScene_TransitionDuration $3A0
#define LNScene_TransitionDuration(%1=_ln_return_discard) _LNScene_TransitionDuration %1
#cmd _LNScene_SetTransitionEffectColor $3A1
#define LNScene_SetTransitionEffectColor(%1) _LNScene_SetTransitionEffectColor %1
#cmd _LNScene_TransitionEffectColor $3A3
#define LNScene_TransitionEffectColor(%1=_ln_return_discard) _LNScene_TransitionEffectColor %1
#cmd _LNScene_SetTransitionEffectMaskTexture $3A4
#define LNScene_SetTransitionEffectMaskTexture(%1) _LNScene_SetTransitionEffectMaskTexture %1
#cmd _LNScene_TransitionEffectMaskTexture $3A6
#define LNScene_TransitionEffectMaskTexture(%1=_ln_return_discard) _LNScene_TransitionEffectMaskTexture %1
#cmd _LNScene_SetTransitionEffectVague $3A7
#define LNScene_SetTransitionEffectVague(%1) _LNScene_SetTransitionEffectVague %1
#cmd _LNScene_TransitionEffectVague $3A9
#define LNScene_TransitionEffectVague(%1=_ln_return_discard) _LNScene_TransitionEffectVague %1
#cmd _LNScene_StartFadeOut $3AA
#define LNScene_StartFadeOut _LNScene_StartFadeOut
#cmd _LNScene_StartFadeIn $3AB
#define LNScene_StartFadeIn _LNScene_StartFadeIn
#cmd _LNScene_SetFogStartDistance $3AC
#define LNScene_SetFogStartDistance(%1) _LNScene_SetFogStartDistance %1
#cmd _LNScene_SetFogColor $3AE
#define LNScene_SetFogColor(%1) _LNScene_SetFogColor %1
#cmd _LNScene_SetFogDensity $3B0
#define LNScene_SetFogDensity(%1) _LNScene_SetFogDensity %1
#cmd _LNScene_SetFogHeightDensity $3B2
#define LNScene_SetFogHeightDensity(%1) _LNScene_SetFogHeightDensity %1
#cmd _LNScene_SetFogLowerHeight $3B4
#define LNScene_SetFogLowerHeight(%1) _LNScene_SetFogLowerHeight %1
#cmd _LNScene_SetFogUpperHeight $3B6
#define LNScene_SetFogUpperHeight(%1) _LNScene_SetFogUpperHeight %1
#cmd _LNScene_SetHDREnabled $3B8
#define LNScene_SetHDREnabled(%1) _LNScene_SetHDREnabled %1
#cmd _LNScene_IsHDREnabled $3BA
#define LNScene_IsHDREnabled(%1=_ln_return_discard) _LNScene_IsHDREnabled %1
#cmd _LNScene_SetScreenBlendColor $3BB
#define LNScene_SetScreenBlendColor(%1) _LNScene_SetScreenBlendColor %1
#cmd _LNScene_ScreenBlendColor $3BD
#define LNScene_ScreenBlendColor(%1=_ln_return_discard) _LNScene_ScreenBlendColor %1
#cmd _LNScene_SetColorTone $3BE
#define LNScene_SetColorTone(%1) _LNScene_SetColorTone %1
#cmd _LNScene_ColorTone $3C0
#define LNScene_ColorTone(%1=_ln_return_discard) _LNScene_ColorTone %1
#cmd _LNScene_SetAntialiasEnabled $3C1
#define LNScene_SetAntialiasEnabled(%1) _LNScene_SetAntialiasEnabled %1
#cmd _LNScene_IsAntialiasEnabled $3C3
#define LNScene_IsAntialiasEnabled(%1=_ln_return_discard) _LNScene_IsAntialiasEnabled %1
#cmd _LNScene_SetSSREnabled $3C4
#define LNScene_SetSSREnabled(%1) _LNScene_SetSSREnabled %1
#cmd _LNScene_IsSSREnabled $3C6
#define LNScene_IsSSREnabled(%1=_ln_return_discard) _LNScene_IsSSREnabled %1
#cmd _LNScene_SetSSAOEnabled $3C7
#define LNScene_SetSSAOEnabled(%1) _LNScene_SetSSAOEnabled %1
#cmd _LNScene_IsSSAOEnabled $3C9
#define LNScene_IsSSAOEnabled(%1=_ln_return_discard) _LNScene_IsSSAOEnabled %1
#cmd _LNScene_SetBloomEnabled $3CA
#define LNScene_SetBloomEnabled(%1) _LNScene_SetBloomEnabled %1
#cmd _LNScene_IsBloomEnabled $3CC
#define LNScene_IsBloomEnabled(%1=_ln_return_discard) _LNScene_IsBloomEnabled %1
#cmd _LNScene_SetDOFEnabled $3CD
#define LNScene_SetDOFEnabled(%1) _LNScene_SetDOFEnabled %1
#cmd _LNScene_IsDOFEnabled $3CF
#define LNScene_IsDOFEnabled(%1=_ln_return_discard) _LNScene_IsDOFEnabled %1
#cmd _LNScene_SetTonemapEnabled $3D0
#define LNScene_SetTonemapEnabled(%1) _LNScene_SetTonemapEnabled %1
#cmd _LNScene_IsTonemapEnabled $3D2
#define LNScene_IsTonemapEnabled(%1=_ln_return_discard) _LNScene_IsTonemapEnabled %1
#cmd _LNScene_SetVignetteEnabled $3D3
#define LNScene_SetVignetteEnabled(%1) _LNScene_SetVignetteEnabled %1
#cmd _LNScene_IsVignetteEnabled $3D5
#define LNScene_IsVignetteEnabled(%1=_ln_return_discard) _LNScene_IsVignetteEnabled %1
#cmd _LNScene_SetGammaEnabled $3D6
#define LNScene_SetGammaEnabled(%1) _LNScene_SetGammaEnabled %1
#cmd _LNScene_IsGammaEnabled $3D8
#define LNScene_IsGammaEnabled(%1=_ln_return_discard) _LNScene_IsGammaEnabled %1
#cmd _LNScene_SetTonemapExposure $3D9
#define LNScene_SetTonemapExposure(%1) _LNScene_SetTonemapExposure %1
#cmd _LNScene_SetTonemapLinearWhite $3DB
#define LNScene_SetTonemapLinearWhite(%1) _LNScene_SetTonemapLinearWhite %1
#cmd _LNScene_SetTonemapShoulderStrength $3DD
#define LNScene_SetTonemapShoulderStrength(%1) _LNScene_SetTonemapShoulderStrength %1
#cmd _LNScene_SetTonemapLinearStrength $3DF
#define LNScene_SetTonemapLinearStrength(%1) _LNScene_SetTonemapLinearStrength %1
#cmd _LNScene_SetTonemapLinearAngle $3E1
#define LNScene_SetTonemapLinearAngle(%1) _LNScene_SetTonemapLinearAngle %1
#cmd _LNScene_SetTonemapToeStrength $3E3
#define LNScene_SetTonemapToeStrength(%1) _LNScene_SetTonemapToeStrength %1
#cmd _LNScene_SetTonemapToeNumerator $3E5
#define LNScene_SetTonemapToeNumerator(%1) _LNScene_SetTonemapToeNumerator %1
#cmd _LNScene_SetTonemapToeDenominator $3E7
#define LNScene_SetTonemapToeDenominator(%1) _LNScene_SetTonemapToeDenominator %1
#cmd _LNLevel_Create $3F8
#define LNLevel_Create(%1=_ln_return_discard) _LNLevel_Create %1
#cmd _LNLevel_AddObject $3EA
#define LNLevel_AddObject(%1, %2) _LNLevel_AddObject %1, %2
#cmd _LNLevel_RemoveObject $3EC
#define LNLevel_RemoveObject(%1, %2) _LNLevel_RemoveObject %1, %2
#cmd _LNLevel_AddSubLevel $3EE
#define LNLevel_AddSubLevel(%1, %2) _LNLevel_AddSubLevel %1, %2
#cmd _LNLevel_RemoveSubLevel $3F0
#define LNLevel_RemoveSubLevel(%1, %2) _LNLevel_RemoveSubLevel %1, %2
#cmd _LNLevel_RemoveAllSubLevels $3F2
#define LNLevel_RemoveAllSubLevels(%1) _LNLevel_RemoveAllSubLevels %1
#cmd LNLevel_SetPrototype_OnSerialize $D5B
#cmd LNLevel_SetPrototype_OnStart $D66
#cmd LNLevel_SetPrototype_OnStop $D71
#cmd LNLevel_SetPrototype_OnPause $D7C
#cmd LNLevel_SetPrototype_OnResume $D87
#cmd LNLevel_SetPrototype_OnUpdate $D92
#cmd _LNUIColors_Red $418
#define LNUIColors_Red(%1, %2=_ln_return_discard) _LNUIColors_Red %1, %2
#cmd _LNUIColors_Pink $41A
#define LNUIColors_Pink(%1, %2=_ln_return_discard) _LNUIColors_Pink %1, %2
#cmd _LNUIColors_Purple $41C
#define LNUIColors_Purple(%1, %2=_ln_return_discard) _LNUIColors_Purple %1, %2
#cmd _LNUIColors_DeepPurple $41E
#define LNUIColors_DeepPurple(%1, %2=_ln_return_discard) _LNUIColors_DeepPurple %1, %2
#cmd _LNUIColors_Indigo $420
#define LNUIColors_Indigo(%1, %2=_ln_return_discard) _LNUIColors_Indigo %1, %2
#cmd _LNUIColors_Blue $422
#define LNUIColors_Blue(%1, %2=_ln_return_discard) _LNUIColors_Blue %1, %2
#cmd _LNUIColors_LightBlue $424
#define LNUIColors_LightBlue(%1, %2=_ln_return_discard) _LNUIColors_LightBlue %1, %2
#cmd _LNUIColors_Cyan $426
#define LNUIColors_Cyan(%1, %2=_ln_return_discard) _LNUIColors_Cyan %1, %2
#cmd _LNUIColors_Teal $428
#define LNUIColors_Teal(%1, %2=_ln_return_discard) _LNUIColors_Teal %1, %2
#cmd _LNUIColors_Green $42A
#define LNUIColors_Green(%1, %2=_ln_return_discard) _LNUIColors_Green %1, %2
#cmd _LNUIColors_LightGreen $42C
#define LNUIColors_LightGreen(%1, %2=_ln_return_discard) _LNUIColors_LightGreen %1, %2
#cmd _LNUIColors_Lime $42E
#define LNUIColors_Lime(%1, %2=_ln_return_discard) _LNUIColors_Lime %1, %2
#cmd _LNUIColors_Yellow $430
#define LNUIColors_Yellow(%1, %2=_ln_return_discard) _LNUIColors_Yellow %1, %2
#cmd _LNUIColors_Amber $432
#define LNUIColors_Amber(%1, %2=_ln_return_discard) _LNUIColors_Amber %1, %2
#cmd _LNUIColors_Orange $434
#define LNUIColors_Orange(%1, %2=_ln_return_discard) _LNUIColors_Orange %1, %2
#cmd _LNUIColors_DeepOrange $436
#define LNUIColors_DeepOrange(%1, %2=_ln_return_discard) _LNUIColors_DeepOrange %1, %2
#cmd _LNUIColors_Brown $438
#define LNUIColors_Brown(%1, %2=_ln_return_discard) _LNUIColors_Brown %1, %2
#cmd _LNUIColors_Grey $43A
#define LNUIColors_Grey(%1, %2=_ln_return_discard) _LNUIColors_Grey %1, %2
#cmd _LNUIColors_Get $43C
#define LNUIColors_Get(%1, %2, %3=_ln_return_discard) _LNUIColors_Get %1, %2, %3
#cmd _LNUIEventArgs_Sender $440
#define LNUIEventArgs_Sender(%1, %2=_ln_return_discard) _LNUIEventArgs_Sender %1, %2
#cmd LNUIEventArgs_SetPrototype_OnSerialize $D9D
#cmd _LNUIGeneralEventHandler_Create $921
#define LNUIGeneralEventHandler_Create(%1, %2=_ln_return_discard) _LNUIGeneralEventHandler_Create %1, %2
#cmd _LNUIEventHandler_Create $926
#define LNUIEventHandler_Create(%1, %2=_ln_return_discard) _LNUIEventHandler_Create %1, %2
#cmd LNUILayoutElement_SetPrototype_OnSerialize $DA8
#cmd _LNUIElement_SetSize $452
#define LNUIElement_SetSize(%1, %2) _LNUIElement_SetSize %1, %2
#cmd _LNUIElement_SetSizeWH $454
#define LNUIElement_SetSizeWH(%1, %2, %3) _LNUIElement_SetSizeWH %1, %2, %3
#cmd _LNUIElement_SetWidth $457
#define LNUIElement_SetWidth(%1, %2) _LNUIElement_SetWidth %1, %2
#cmd _LNUIElement_GetWidth $459
#define LNUIElement_GetWidth(%1, %2=_ln_return_discard) _LNUIElement_GetWidth %1, %2
#cmd _LNUIElement_SetHeight $45A
#define LNUIElement_SetHeight(%1, %2) _LNUIElement_SetHeight %1, %2
#cmd _LNUIElement_GetHeight $45C
#define LNUIElement_GetHeight(%1, %2=_ln_return_discard) _LNUIElement_GetHeight %1, %2
#cmd _LNUIElement_SetMargin $45D
#define LNUIElement_SetMargin(%1, %2) _LNUIElement_SetMargin %1, %2
#cmd _LNUIElement_GetMargin $45F
#define LNUIElement_GetMargin(%1, %2=_ln_return_discard) _LNUIElement_GetMargin %1, %2
#cmd _LNUIElement_SetPadding $460
#define LNUIElement_SetPadding(%1, %2) _LNUIElement_SetPadding %1, %2
#cmd _LNUIElement_GetPadding $462
#define LNUIElement_GetPadding(%1, %2=_ln_return_discard) _LNUIElement_GetPadding %1, %2
#cmd _LNUIElement_SetHAlignment $463
#define LNUIElement_SetHAlignment(%1, %2) _LNUIElement_SetHAlignment %1, %2
#cmd _LNUIElement_GetHAlignment $465
#define LNUIElement_GetHAlignment(%1, %2=_ln_return_discard) _LNUIElement_GetHAlignment %1, %2
#cmd _LNUIElement_SetVAlignment $466
#define LNUIElement_SetVAlignment(%1, %2) _LNUIElement_SetVAlignment %1, %2
#cmd _LNUIElement_GetVAlignment $468
#define LNUIElement_GetVAlignment(%1, %2=_ln_return_discard) _LNUIElement_GetVAlignment %1, %2
#cmd _LNUIElement_SetAlignments $469
#define LNUIElement_SetAlignments(%1, %2, %3) _LNUIElement_SetAlignments %1, %2, %3
#cmd _LNUIElement_SetPosition $46C
#define LNUIElement_SetPosition(%1, %2) _LNUIElement_SetPosition %1, %2
#cmd _LNUIElement_SetPositionXYZ $46E
#define LNUIElement_SetPositionXYZ(%1, %2, %3, %4) _LNUIElement_SetPositionXYZ %1, %2, %3, %4
#cmd _LNUIElement_GetPosition $472
#define LNUIElement_GetPosition(%1, %2=_ln_return_discard) _LNUIElement_GetPosition %1, %2
#cmd _LNUIElement_SetRotation $473
#define LNUIElement_SetRotation(%1, %2) _LNUIElement_SetRotation %1, %2
#cmd _LNUIElement_SetEulerAngles $475
#define LNUIElement_SetEulerAngles(%1, %2, %3, %4) _LNUIElement_SetEulerAngles %1, %2, %3, %4
#cmd _LNUIElement_GetRotation $479
#define LNUIElement_GetRotation(%1, %2=_ln_return_discard) _LNUIElement_GetRotation %1, %2
#cmd _LNUIElement_SetScale $47A
#define LNUIElement_SetScale(%1, %2) _LNUIElement_SetScale %1, %2
#cmd _LNUIElement_SetScaleS $47C
#define LNUIElement_SetScaleS(%1, %2) _LNUIElement_SetScaleS %1, %2
#cmd _LNUIElement_SetScaleXY $47E
#define LNUIElement_SetScaleXY(%1, %2, %3) _LNUIElement_SetScaleXY %1, %2, %3
#cmd _LNUIElement_GetScale $481
#define LNUIElement_GetScale(%1, %2=_ln_return_discard) _LNUIElement_GetScale %1, %2
#cmd _LNUIElement_SetCenterPoint $482
#define LNUIElement_SetCenterPoint(%1, %2) _LNUIElement_SetCenterPoint %1, %2
#cmd _LNUIElement_SetCenterPointXYZ $484
#define LNUIElement_SetCenterPointXYZ(%1, %2, %3, %4) _LNUIElement_SetCenterPointXYZ %1, %2, %3, %4
#cmd _LNUIElement_GetCenterPoint $488
#define LNUIElement_GetCenterPoint(%1, %2=_ln_return_discard) _LNUIElement_GetCenterPoint %1, %2
#cmd _LNUIElement_SetEnabled $489
#define LNUIElement_SetEnabled(%1, %2) _LNUIElement_SetEnabled %1, %2
#cmd _LNUIElement_IsEnabled $48B
#define LNUIElement_IsEnabled(%1, %2=_ln_return_discard) _LNUIElement_IsEnabled %1, %2
#cmd _LNUIElement_SetData $48C
#define LNUIElement_SetData(%1, %2) _LNUIElement_SetData %1, %2
#cmd _LNUIElement_GetData $48E
#define LNUIElement_GetData(%1, %2=_ln_return_discard) _LNUIElement_GetData %1, %2
#cmd _LNUIElement_SetBackgroundColor $48F
#define LNUIElement_SetBackgroundColor(%1, %2) _LNUIElement_SetBackgroundColor %1, %2
#cmd _LNUIElement_GetBackgroundColor $491
#define LNUIElement_GetBackgroundColor(%1, %2=_ln_return_discard) _LNUIElement_GetBackgroundColor %1, %2
#cmd _LNUIElement_SetBorderThickness $492
#define LNUIElement_SetBorderThickness(%1, %2) _LNUIElement_SetBorderThickness %1, %2
#cmd _LNUIElement_GetBorderThickness $494
#define LNUIElement_GetBorderThickness(%1, %2=_ln_return_discard) _LNUIElement_GetBorderThickness %1, %2
#cmd _LNUIElement_SetBorderColor $495
#define LNUIElement_SetBorderColor(%1, %2) _LNUIElement_SetBorderColor %1, %2
#cmd _LNUIElement_GetBorderColor $497
#define LNUIElement_GetBorderColor(%1, %2=_ln_return_discard) _LNUIElement_GetBorderColor %1, %2
#cmd _LNUIElement_SetCornerRadius $498
#define LNUIElement_SetCornerRadius(%1, %2) _LNUIElement_SetCornerRadius %1, %2
#cmd _LNUIElement_GetCornerRadius $49A
#define LNUIElement_GetCornerRadius(%1, %2=_ln_return_discard) _LNUIElement_GetCornerRadius %1, %2
#cmd _LNUIElement_SetVisibility $49B
#define LNUIElement_SetVisibility(%1, %2) _LNUIElement_SetVisibility %1, %2
#cmd _LNUIElement_GetVisibility $49D
#define LNUIElement_GetVisibility(%1, %2=_ln_return_discard) _LNUIElement_GetVisibility %1, %2
#cmd _LNUIElement_SetOpacity $49E
#define LNUIElement_SetOpacity(%1, %2) _LNUIElement_SetOpacity %1, %2
#cmd _LNUIElement_GetOpacity $4A0
#define LNUIElement_GetOpacity(%1, %2=_ln_return_discard) _LNUIElement_GetOpacity %1, %2
#cmd _LNUIElement_AddChild $4A1
#define LNUIElement_AddChild(%1, %2) _LNUIElement_AddChild %1, %2
#cmd _LNUIElement_SetFocusable $4A3
#define LNUIElement_SetFocusable(%1, %2) _LNUIElement_SetFocusable %1, %2
#cmd _LNUIElement_GetFocusable $4A5
#define LNUIElement_GetFocusable(%1, %2=_ln_return_discard) _LNUIElement_GetFocusable %1, %2
#cmd LNUIElement_SetPrototype_OnSerialize $DB3
#cmd _LNUITextBlock_Create $4AA
#define LNUITextBlock_Create(%1=_ln_return_discard) _LNUITextBlock_Create %1
#cmd _LNUITextBlock_CreateWithText $4AB
#define LNUITextBlock_CreateWithText(%1, %2=_ln_return_discard) _LNUITextBlock_CreateWithText %1, %2
#cmd _LNUITextBlock_SetText $4A7
#define LNUITextBlock_SetText(%1, %2) _LNUITextBlock_SetText %1, %2
#cmd _LNUITextBlock_GetText $4A9
#define LNUITextBlock_GetText(%1, %2=_ln_return_discard) _LNUITextBlock_GetText %1, %2
#cmd LNUITextBlock_SetPrototype_OnSerialize $DBE
#cmd _LNUISprite_Create $4BA
#define LNUISprite_Create(%1=_ln_return_discard) _LNUISprite_Create %1
#cmd _LNUISprite_CreateWithTexture $4BB
#define LNUISprite_CreateWithTexture(%1, %2=_ln_return_discard) _LNUISprite_CreateWithTexture %1, %2
#cmd _LNUISprite_SetTexture $4AE
#define LNUISprite_SetTexture(%1, %2) _LNUISprite_SetTexture %1, %2
#cmd _LNUISprite_SetSourceRect $4B0
#define LNUISprite_SetSourceRect(%1, %2) _LNUISprite_SetSourceRect %1, %2
#cmd _LNUISprite_SetSourceRectXYWH $4B2
#define LNUISprite_SetSourceRectXYWH(%1, %2, %3, %4, %5) _LNUISprite_SetSourceRectXYWH %1, %2, %3, %4, %5
#cmd _LNUISprite_GetSourceRect $4B7
#define LNUISprite_GetSourceRect(%1, %2=_ln_return_discard) _LNUISprite_GetSourceRect %1, %2
#cmd _LNUISprite_SetShader $4B8
#define LNUISprite_SetShader(%1, %2) _LNUISprite_SetShader %1, %2
#cmd LNUISprite_SetPrototype_OnSerialize $DC9
#cmd _LNUIIcon_LoadFontIcon $4BE
#define LNUIIcon_LoadFontIcon(%1, %2=_ln_return_discard) _LNUIIcon_LoadFontIcon %1, %2
#cmd _LNUIIcon_LoadFontIconWithNameSize $4C0
#define LNUIIcon_LoadFontIconWithNameSize(%1, %2, %3=_ln_return_discard) _LNUIIcon_LoadFontIconWithNameSize %1, %2, %3
#cmd _LNUIIcon_LoadFontIconWithNameSizeColor $4C3
#define LNUIIcon_LoadFontIconWithNameSizeColor(%1, %2, %3, %4=_ln_return_discard) _LNUIIcon_LoadFontIconWithNameSizeColor %1, %2, %3, %4
#cmd LNUIIcon_SetPrototype_OnSerialize $DD4
#cmd _LNUIMessageTextArea_Create $4CC
#define LNUIMessageTextArea_Create(%1=_ln_return_discard) _LNUIMessageTextArea_Create %1
#cmd _LNUIMessageTextArea_SetText $4C8
#define LNUIMessageTextArea_SetText(%1, %2) _LNUIMessageTextArea_SetText %1, %2
#cmd _LNUIMessageTextArea_SetTypingSpeed $4CA
#define LNUIMessageTextArea_SetTypingSpeed(%1, %2) _LNUIMessageTextArea_SetTypingSpeed %1, %2
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $DDF
#cmd _LNUI_Add $4CE
#define LNUI_Add(%1) _LNUI_Add %1
#cmd _LNUI_Remove $4D0
#define LNUI_Remove(%1) _LNUI_Remove %1
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $DEA
#cmd _LNUIBoxLayout_Create $4D7
#define LNUIBoxLayout_Create(%1=_ln_return_discard) _LNUIBoxLayout_Create %1
#cmd _LNUIBoxLayout_SetOrientation $4D4
#define LNUIBoxLayout_SetOrientation(%1, %2) _LNUIBoxLayout_SetOrientation %1, %2
#cmd _LNUIBoxLayout_GetOrientation $4D6
#define LNUIBoxLayout_GetOrientation(%1, %2=_ln_return_discard) _LNUIBoxLayout_GetOrientation %1, %2
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $DF5
#cmd _LNUIStackLayout_Create $4DC
#define LNUIStackLayout_Create(%1=_ln_return_discard) _LNUIStackLayout_Create %1
#cmd _LNUIStackLayout_SetOrientation $4D9
#define LNUIStackLayout_SetOrientation(%1, %2) _LNUIStackLayout_SetOrientation %1, %2
#cmd _LNUIStackLayout_GetOrientation $4DB
#define LNUIStackLayout_GetOrientation(%1, %2=_ln_return_discard) _LNUIStackLayout_GetOrientation %1, %2
#cmd LNUIStackLayout_SetPrototype_OnSerialize $E00
#cmd _LNUIGridLayout_Create $4EC
#define LNUIGridLayout_Create(%1=_ln_return_discard) _LNUIGridLayout_Create %1
#cmd _LNUIGridLayout_SetColumnCount $4DE
#define LNUIGridLayout_SetColumnCount(%1, %2) _LNUIGridLayout_SetColumnCount %1, %2
#cmd _LNUIGridLayout_SetRow $4E0
#define LNUIGridLayout_SetRow(%1, %2) _LNUIGridLayout_SetRow %1, %2
#cmd _LNUIGridLayout_SetColumn $4E3
#define LNUIGridLayout_SetColumn(%1, %2) _LNUIGridLayout_SetColumn %1, %2
#cmd _LNUIGridLayout_SetPlacement $4E6
#define LNUIGridLayout_SetPlacement(%1, %2, %3, %4, %5) _LNUIGridLayout_SetPlacement %1, %2, %3, %4, %5
#cmd LNUIGridLayout_SetPrototype_OnSerialize $E0B
#cmd _LNUIControl_Create $4F8
#define LNUIControl_Create(%1=_ln_return_discard) _LNUIControl_Create %1
#cmd _LNUIControl_AddInlineVisual $4F9
#define LNUIControl_AddInlineVisual(%1, %2, %3) _LNUIControl_AddInlineVisual %1, %2, %3
#cmd LNUIControl_SetPrototype_OnSerialize $E16
#cmd _LNUIButtonBase_SetText $4FD
#define LNUIButtonBase_SetText(%1, %2) _LNUIButtonBase_SetText %1, %2
#cmd LNUIButtonBase_SetPrototype_OnSerialize $E21
#cmd _LNUIButton_Create $500
#define LNUIButton_Create(%1=_ln_return_discard) _LNUIButton_Create %1
#cmd _LNUIButton_CreateWithText $501
#define LNUIButton_CreateWithText(%1, %2=_ln_return_discard) _LNUIButton_CreateWithText %1, %2
#cmd _LNUIButton_ConnectOnClicked $503
#define LNUIButton_ConnectOnClicked(%1, %2, %3=_ln_return_discard) _LNUIButton_ConnectOnClicked %1, %2, %3
#cmd LNUIButton_SetPrototype_OnSerialize $E2C
#cmd _LNUIWindow_Create $506
#define LNUIWindow_Create(%1=_ln_return_discard) _LNUIWindow_Create %1
#cmd LNUIWindow_SetPrototype_OnSerialize $E37
#cmd _LNUIListItem_ConnectOnSubmit $50B
#define LNUIListItem_ConnectOnSubmit(%1, %2, %3=_ln_return_discard) _LNUIListItem_ConnectOnSubmit %1, %2, %3
#cmd LNUIListItem_SetPrototype_OnSerialize $E42
#cmd _LNUIListItemsControl_SetItemsLayoutPanel $50E
#define LNUIListItemsControl_SetItemsLayoutPanel(%1, %2) _LNUIListItemsControl_SetItemsLayoutPanel %1, %2
#cmd _LNUIListItemsControl_GetItemsLayoutPanel $510
#define LNUIListItemsControl_GetItemsLayoutPanel(%1, %2=_ln_return_discard) _LNUIListItemsControl_GetItemsLayoutPanel %1, %2
#cmd _LNUIListItemsControl_SetSubmitMode $511
#define LNUIListItemsControl_SetSubmitMode(%1, %2) _LNUIListItemsControl_SetSubmitMode %1, %2
#cmd _LNUIListItemsControl_GetSubmitMode $513
#define LNUIListItemsControl_GetSubmitMode(%1, %2=_ln_return_discard) _LNUIListItemsControl_GetSubmitMode %1, %2
#cmd _LNUIListItemsControl_ConnectOnSubmit $514
#define LNUIListItemsControl_ConnectOnSubmit(%1, %2, %3=_ln_return_discard) _LNUIListItemsControl_ConnectOnSubmit %1, %2, %3
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $E4D
#cmd _LNUIListBoxItem_Create $517
#define LNUIListBoxItem_Create(%1, %2=_ln_return_discard) _LNUIListBoxItem_Create %1, %2
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $E58
#cmd _LNUIListBox_Create $51C
#define LNUIListBox_Create(%1=_ln_return_discard) _LNUIListBox_Create %1
#cmd _LNUIListBox_AddItem $51A
#define LNUIListBox_AddItem(%1, %2, %3=_ln_return_discard) _LNUIListBox_AddItem %1, %2, %3
#cmd LNUIListBox_SetPrototype_OnSerialize $E63
#cmd LNInputGesture_SetPrototype_OnSerialize $E6E
#cmd _LNKeyGesture_Create $51F
#define LNKeyGesture_Create(%1, %2=_ln_return_discard) _LNKeyGesture_Create %1, %2
#cmd LNKeyGesture_SetPrototype_OnSerialize $E79
#cmd _LNInput_IsPressed $522
#define LNInput_IsPressed(%1, %2=_ln_return_discard) _LNInput_IsPressed %1, %2
#cmd _LNInput_IsTriggered $524
#define LNInput_IsTriggered(%1, %2=_ln_return_discard) _LNInput_IsTriggered %1, %2
#cmd _LNInput_IsTriggeredOff $526
#define LNInput_IsTriggeredOff(%1, %2=_ln_return_discard) _LNInput_IsTriggeredOff %1, %2
#cmd _LNInput_IsRepeated $528
#define LNInput_IsRepeated(%1, %2=_ln_return_discard) _LNInput_IsRepeated %1, %2
#cmd _LNInput_GetAxisValue $52A
#define LNInput_GetAxisValue(%1, %2=_ln_return_discard) _LNInput_GetAxisValue %1, %2
#cmd _LNInput_AddBinding $52C
#define LNInput_AddBinding(%1, %2) _LNInput_AddBinding %1, %2
#cmd _LNInput_RemoveBinding $52F
#define LNInput_RemoveBinding(%1) _LNInput_RemoveBinding %1
#cmd _LNInput_ClearBindings $531
#define LNInput_ClearBindings(%1) _LNInput_ClearBindings %1
#cmd _LNInput_ClearAllBindings $533
#define LNInput_ClearAllBindings _LNInput_ClearAllBindings
#cmd _LNMouse_Pressed $535
#define LNMouse_Pressed(%1, %2=_ln_return_discard) _LNMouse_Pressed %1, %2
#cmd _LNMouse_Triggered $537
#define LNMouse_Triggered(%1, %2=_ln_return_discard) _LNMouse_Triggered %1, %2
#cmd _LNMouse_TriggeredOff $539
#define LNMouse_TriggeredOff(%1, %2=_ln_return_discard) _LNMouse_TriggeredOff %1, %2
#cmd _LNMouse_Repeated $53B
#define LNMouse_Repeated(%1, %2=_ln_return_discard) _LNMouse_Repeated %1, %2
#cmd _LNMouse_Position $53D
#define LNMouse_Position(%1=_ln_return_discard) _LNMouse_Position %1
#cmd _LNInterpreterCommand_Code $53F
#define LNInterpreterCommand_Code(%1, %2=_ln_return_discard) _LNInterpreterCommand_Code %1, %2
#cmd _LNInterpreterCommand_ParamsCount $540
#define LNInterpreterCommand_ParamsCount(%1, %2=_ln_return_discard) _LNInterpreterCommand_ParamsCount %1, %2
#cmd _LNInterpreterCommand_Param $541
#define LNInterpreterCommand_Param(%1, %2, %3=_ln_return_discard) _LNInterpreterCommand_Param %1, %2, %3
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $E84
#cmd _LNInterpreterCommandList_Create $558
#define LNInterpreterCommandList_Create(%1=_ln_return_discard) _LNInterpreterCommandList_Create %1
#cmd _LNInterpreterCommandList_AddCommand $544
#define LNInterpreterCommandList_AddCommand(%1, %2) _LNInterpreterCommandList_AddCommand %1, %2
#cmd _LNInterpreterCommandList_AddCommand1 $546
#define LNInterpreterCommandList_AddCommand1(%1, %2, %3) _LNInterpreterCommandList_AddCommand1 %1, %2, %3
#cmd _LNInterpreterCommandList_AddCommand2 $549
#define LNInterpreterCommandList_AddCommand2(%1, %2, %3, %4) _LNInterpreterCommandList_AddCommand2 %1, %2, %3, %4
#cmd _LNInterpreterCommandList_AddCommand3 $54D
#define LNInterpreterCommandList_AddCommand3(%1, %2, %3, %4, %5) _LNInterpreterCommandList_AddCommand3 %1, %2, %3, %4, %5
#cmd _LNInterpreterCommandList_AddCommand4 $552
#define LNInterpreterCommandList_AddCommand4(%1, %2, %3, %4, %5, %6) _LNInterpreterCommandList_AddCommand4 %1, %2, %3, %4, %5, %6
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $E8F
#cmd _LNInterpreterCommandDelegate_Create $9F1
#define LNInterpreterCommandDelegate_Create(%1, %2=_ln_return_discard) _LNInterpreterCommandDelegate_Create %1, %2
#cmd _LNInterpreter_Create $56C
#define LNInterpreter_Create(%1=_ln_return_discard) _LNInterpreter_Create %1
#cmd _LNInterpreter_Clear $55D
#define LNInterpreter_Clear(%1) _LNInterpreter_Clear %1
#cmd _LNInterpreter_Run $55E
#define LNInterpreter_Run(%1, %2) _LNInterpreter_Run %1, %2
#cmd _LNInterpreter_IsRunning $560
#define LNInterpreter_IsRunning(%1, %2=_ln_return_discard) _LNInterpreter_IsRunning %1, %2
#cmd _LNInterpreter_Update $561
#define LNInterpreter_Update(%1) _LNInterpreter_Update %1
#cmd _LNInterpreter_Terminate $562
#define LNInterpreter_Terminate(%1) _LNInterpreter_Terminate %1
#cmd _LNInterpreter_RegisterCommandHandler $563
#define LNInterpreter_RegisterCommandHandler(%1, %2, %3) _LNInterpreter_RegisterCommandHandler %1, %2, %3
#cmd _LNInterpreter_SetWaitMode $566
#define LNInterpreter_SetWaitMode(%1, %2) _LNInterpreter_SetWaitMode %1, %2
#cmd _LNInterpreter_GetWaitMode $568
#define LNInterpreter_GetWaitMode(%1, %2=_ln_return_discard) _LNInterpreter_GetWaitMode %1, %2
#cmd _LNInterpreter_SetWaitCount $569
#define LNInterpreter_SetWaitCount(%1, %2) _LNInterpreter_SetWaitCount %1, %2
#cmd _LNInterpreter_GetWaitCount $56B
#define LNInterpreter_GetWaitCount(%1, %2=_ln_return_discard) _LNInterpreter_GetWaitCount %1, %2
#cmd LNInterpreter_SetPrototype_OnSerialize $E9A
#cmd LNInterpreter_SetPrototype_OnUpdateWait $EA6
#cmd _LNEngineSettings_SetMainWindowSize $56F
#define LNEngineSettings_SetMainWindowSize(%1, %2) _LNEngineSettings_SetMainWindowSize %1, %2
#cmd _LNEngineSettings_SetMainWorldViewSize $572
#define LNEngineSettings_SetMainWorldViewSize(%1, %2) _LNEngineSettings_SetMainWorldViewSize %1, %2
#cmd _LNEngineSettings_SetMainWindowTitle $575
#define LNEngineSettings_SetMainWindowTitle(%1) _LNEngineSettings_SetMainWindowTitle %1
#cmd _LNEngineSettings_SetMainWindowResizable $577
#define LNEngineSettings_SetMainWindowResizable(%1) _LNEngineSettings_SetMainWindowResizable %1
#cmd _LNEngineSettings_AddAssetDirectory $579
#define LNEngineSettings_AddAssetDirectory(%1) _LNEngineSettings_AddAssetDirectory %1
#cmd _LNEngineSettings_AddAssetArchive $57B
#define LNEngineSettings_AddAssetArchive(%1, %2) _LNEngineSettings_AddAssetArchive %1, %2
#cmd _LNEngineSettings_SetFrameRate $57E
#define LNEngineSettings_SetFrameRate(%1) _LNEngineSettings_SetFrameRate %1
#cmd _LNEngineSettings_SetUITheme $580
#define LNEngineSettings_SetUITheme(%1) _LNEngineSettings_SetUITheme %1
#cmd _LNEngineSettings_SetFontFile $582
#define LNEngineSettings_SetFontFile(%1) _LNEngineSettings_SetFontFile %1
#cmd _LNEngineSettings_SetDebugToolEnabled $584
#define LNEngineSettings_SetDebugToolEnabled(%1) _LNEngineSettings_SetDebugToolEnabled %1
#cmd _LNEngineSettings_SetEngineLogEnabled $586
#define LNEngineSettings_SetEngineLogEnabled(%1) _LNEngineSettings_SetEngineLogEnabled %1
#cmd _LNEngineSettings_SetEngineLogFilePath $588
#define LNEngineSettings_SetEngineLogFilePath(%1) _LNEngineSettings_SetEngineLogFilePath %1
#cmd _LNEngineSettings_SetDeveloperToolEnabled $58A
#define LNEngineSettings_SetDeveloperToolEnabled(%1) _LNEngineSettings_SetDeveloperToolEnabled %1
#cmd _LNEngineSettings_SetUserMainWindow $58C
#define LNEngineSettings_SetUserMainWindow(%1) _LNEngineSettings_SetUserMainWindow %1
#cmd _LNEngine_Initialize $58F
#define LNEngine_Initialize _LNEngine_Initialize
#cmd _LNEngine_Terminate $590
#define LNEngine_Terminate _LNEngine_Terminate
#cmd _LNEngine_Update $591
#define LNEngine_Update(%1=_ln_return_discard) _LNEngine_Update %1
#cmd _LNEngine_Run $592
#define LNEngine_Run(%1) _LNEngine_Run %1
#cmd _LNEngine_GetTime $594
#define LNEngine_GetTime(%1=_ln_return_discard) _LNEngine_GetTime %1
#cmd _LNEngine_GetWorld $595
#define LNEngine_GetWorld(%1=_ln_return_discard) _LNEngine_GetWorld %1
#cmd _LNEngine_GetCamera $596
#define LNEngine_GetCamera(%1=_ln_return_discard) _LNEngine_GetCamera %1
#cmd _LNEngine_GetMainLight $597
#define LNEngine_GetMainLight(%1=_ln_return_discard) _LNEngine_GetMainLight %1
#cmd _LNEngine_GetRenderView $598
#define LNEngine_GetRenderView(%1=_ln_return_discard) _LNEngine_GetRenderView %1
#cmd _LNApplication_Create $59D
#define LNApplication_Create(%1=_ln_return_discard) _LNApplication_Create %1
#cmd _LNApplication_OnInit $59A
#define LNApplication_OnInit(%1) _LNApplication_OnInit %1
#cmd _LNApplication_OnUpdate $59B
#define LNApplication_OnUpdate(%1) _LNApplication_OnUpdate %1
#cmd _LNApplication_World $59C
#define LNApplication_World(%1, %2=_ln_return_discard) _LNApplication_World %1, %2
#cmd LNApplication_SetPrototype_OnSerialize $EB1
#cmd LNApplication_SetPrototype_OnInit $EBC
#cmd LNApplication_SetPrototype_OnUpdate $EC7
#cmd _LNDebug_SetGuideGridEnabled $59F
#define LNDebug_SetGuideGridEnabled(%1) _LNDebug_SetGuideGridEnabled %1
#cmd _LNDebug_SetPhysicsDebugDrawEnabled $5A1
#define LNDebug_SetPhysicsDebugDrawEnabled(%1) _LNDebug_SetPhysicsDebugDrawEnabled %1
#cmd _LNDebug_Print $5A3
#define LNDebug_Print(%1) _LNDebug_Print %1
#cmd _LNDebug_PrintWithTime $5A5
#define LNDebug_PrintWithTime(%1, %2) _LNDebug_PrintWithTime %1, %2
#cmd _LNDebug_PrintWithTimeAndColor $5A8
#define LNDebug_PrintWithTimeAndColor(%1, %2, %3) _LNDebug_PrintWithTimeAndColor %1, %2, %3
#cmd _LNObjectSerializeHandler_Create $A20
#define LNObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNObjectSerializeHandler_Create %1, %2
#cmd _LNEventConnectionSerializeHandler_Create $A2B
#define LNEventConnectionSerializeHandler_Create(%1, %2=_ln_return_discard) _LNEventConnectionSerializeHandler_Create %1, %2
#cmd _LNVariantSerializeHandler_Create $A36
#define LNVariantSerializeHandler_Create(%1, %2=_ln_return_discard) _LNVariantSerializeHandler_Create %1, %2
#cmd _LNZVTestClass1SerializeHandler_Create $A41
#define LNZVTestClass1SerializeHandler_Create(%1, %2=_ln_return_discard) _LNZVTestClass1SerializeHandler_Create %1, %2
#cmd _LNZVTestEventArgs1SerializeHandler_Create $A4C
#define LNZVTestEventArgs1SerializeHandler_Create(%1, %2=_ln_return_discard) _LNZVTestEventArgs1SerializeHandler_Create %1, %2
#cmd _LNSerializer2SerializeHandler_Create $A57
#define LNSerializer2SerializeHandler_Create(%1, %2=_ln_return_discard) _LNSerializer2SerializeHandler_Create %1, %2
#cmd _LNAssetObjectSerializeHandler_Create $A62
#define LNAssetObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAssetObjectSerializeHandler_Create %1, %2
#cmd _LNAssetImportSettingsSerializeHandler_Create $A6D
#define LNAssetImportSettingsSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAssetImportSettingsSerializeHandler_Create %1, %2
#cmd _LNAssetModelSerializeHandler_Create $A78
#define LNAssetModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAssetModelSerializeHandler_Create %1, %2
#cmd _LNSoundSerializeHandler_Create $A83
#define LNSoundSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSoundSerializeHandler_Create %1, %2
#cmd _LNTextureSerializeHandler_Create $A8E
#define LNTextureSerializeHandler_Create(%1, %2=_ln_return_discard) _LNTextureSerializeHandler_Create %1, %2
#cmd _LNTexture2DSerializeHandler_Create $A99
#define LNTexture2DSerializeHandler_Create(%1, %2=_ln_return_discard) _LNTexture2DSerializeHandler_Create %1, %2
#cmd _LNShaderSerializeHandler_Create $AA4
#define LNShaderSerializeHandler_Create(%1, %2=_ln_return_discard) _LNShaderSerializeHandler_Create %1, %2
#cmd _LNRenderViewSerializeHandler_Create $AAF
#define LNRenderViewSerializeHandler_Create(%1, %2=_ln_return_discard) _LNRenderViewSerializeHandler_Create %1, %2
#cmd _LNMaterialSerializeHandler_Create $ABA
#define LNMaterialSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMaterialSerializeHandler_Create %1, %2
#cmd _LNMeshNodeSerializeHandler_Create $AC5
#define LNMeshNodeSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshNodeSerializeHandler_Create %1, %2
#cmd _LNAnimationControllerSerializeHandler_Create $AD0
#define LNAnimationControllerSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationControllerSerializeHandler_Create %1, %2
#cmd _LNMeshModelSerializeHandler_Create $ADB
#define LNMeshModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshModelSerializeHandler_Create %1, %2
#cmd _LNMeshImportSettingsSerializeHandler_Create $AE6
#define LNMeshImportSettingsSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshImportSettingsSerializeHandler_Create %1, %2
#cmd _LNSkinnedMeshModelSerializeHandler_Create $AF1
#define LNSkinnedMeshModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSkinnedMeshModelSerializeHandler_Create %1, %2
#cmd _LNCollisionShapeSerializeHandler_Create $AFC
#define LNCollisionShapeSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCollisionShapeSerializeHandler_Create %1, %2
#cmd _LNBoxCollisionShapeSerializeHandler_Create $B07
#define LNBoxCollisionShapeSerializeHandler_Create(%1, %2=_ln_return_discard) _LNBoxCollisionShapeSerializeHandler_Create %1, %2
#cmd _LNAnimationCurveSerializeHandler_Create $B12
#define LNAnimationCurveSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationCurveSerializeHandler_Create %1, %2
#cmd _LNKeyFrameAnimationCurveSerializeHandler_Create $B1D
#define LNKeyFrameAnimationCurveSerializeHandler_Create(%1, %2=_ln_return_discard) _LNKeyFrameAnimationCurveSerializeHandler_Create %1, %2
#cmd _LNAnimationClipSerializeHandler_Create $B28
#define LNAnimationClipSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationClipSerializeHandler_Create %1, %2
#cmd _LNAnimationStateSerializeHandler_Create $B33
#define LNAnimationStateSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationStateSerializeHandler_Create %1, %2
#cmd _LNEffectResourceSerializeHandler_Create $B3E
#define LNEffectResourceSerializeHandler_Create(%1, %2=_ln_return_discard) _LNEffectResourceSerializeHandler_Create %1, %2
#cmd _LNParticleEmitterModelSerializeHandler_Create $B49
#define LNParticleEmitterModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterModelSerializeHandler_Create %1, %2
#cmd _LNParticleModelSerializeHandler_Create $B54
#define LNParticleModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNParticleModelSerializeHandler_Create %1, %2
#cmd _LNComponentSerializeHandler_Create $B5F
#define LNComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNComponentSerializeHandler_Create %1, %2
#cmd _LNVisualComponentSerializeHandler_Create $B6A
#define LNVisualComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNVisualComponentSerializeHandler_Create %1, %2
#cmd _LNSpriteComponentSerializeHandler_Create $B75
#define LNSpriteComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSpriteComponentSerializeHandler_Create %1, %2
#cmd _LNCharacterControllerSerializeHandler_Create $B80
#define LNCharacterControllerSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCharacterControllerSerializeHandler_Create %1, %2
#cmd _LNWorldSerializeHandler_Create $B8B
#define LNWorldSerializeHandler_Create(%1, %2=_ln_return_discard) _LNWorldSerializeHandler_Create %1, %2
#cmd _LNComponentListSerializeHandler_Create $B96
#define LNComponentListSerializeHandler_Create(%1, %2=_ln_return_discard) _LNComponentListSerializeHandler_Create %1, %2
#cmd _LNWorldObjectSerializeHandler_Create $BA1
#define LNWorldObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNWorldObjectSerializeHandler_Create %1, %2
#cmd _LNWorldObjectPreUpdateHandler_Create $BAC
#define LNWorldObjectPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNWorldObjectPreUpdateHandler_Create %1, %2
#cmd _LNWorldObjectUpdateHandler_Create $BB7
#define LNWorldObjectUpdateHandler_Create(%1, %2=_ln_return_discard) _LNWorldObjectUpdateHandler_Create %1, %2
#cmd _LNVisualObjectSerializeHandler_Create $BC2
#define LNVisualObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNVisualObjectSerializeHandler_Create %1, %2
#cmd _LNVisualObjectPreUpdateHandler_Create $BCD
#define LNVisualObjectPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNVisualObjectPreUpdateHandler_Create %1, %2
#cmd _LNVisualObjectUpdateHandler_Create $BD8
#define LNVisualObjectUpdateHandler_Create(%1, %2=_ln_return_discard) _LNVisualObjectUpdateHandler_Create %1, %2
#cmd _LNCameraSerializeHandler_Create $BE3
#define LNCameraSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCameraSerializeHandler_Create %1, %2
#cmd _LNCameraPreUpdateHandler_Create $BEE
#define LNCameraPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNCameraPreUpdateHandler_Create %1, %2
#cmd _LNCameraUpdateHandler_Create $BF9
#define LNCameraUpdateHandler_Create(%1, %2=_ln_return_discard) _LNCameraUpdateHandler_Create %1, %2
#cmd _LNEnvironmentLightSerializeHandler_Create $C04
#define LNEnvironmentLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNEnvironmentLightSerializeHandler_Create %1, %2
#cmd _LNEnvironmentLightPreUpdateHandler_Create $C0F
#define LNEnvironmentLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNEnvironmentLightPreUpdateHandler_Create %1, %2
#cmd _LNEnvironmentLightUpdateHandler_Create $C1A
#define LNEnvironmentLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNEnvironmentLightUpdateHandler_Create %1, %2
#cmd _LNDirectionalLightSerializeHandler_Create $C25
#define LNDirectionalLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNDirectionalLightSerializeHandler_Create %1, %2
#cmd _LNDirectionalLightPreUpdateHandler_Create $C30
#define LNDirectionalLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNDirectionalLightPreUpdateHandler_Create %1, %2
#cmd _LNDirectionalLightUpdateHandler_Create $C3B
#define LNDirectionalLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNDirectionalLightUpdateHandler_Create %1, %2
#cmd _LNPointLightSerializeHandler_Create $C46
#define LNPointLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNPointLightSerializeHandler_Create %1, %2
#cmd _LNPointLightPreUpdateHandler_Create $C51
#define LNPointLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPointLightPreUpdateHandler_Create %1, %2
#cmd _LNPointLightUpdateHandler_Create $C5C
#define LNPointLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPointLightUpdateHandler_Create %1, %2
#cmd _LNSpotLightSerializeHandler_Create $C67
#define LNSpotLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSpotLightSerializeHandler_Create %1, %2
#cmd _LNSpotLightPreUpdateHandler_Create $C72
#define LNSpotLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpotLightPreUpdateHandler_Create %1, %2
#cmd _LNSpotLightUpdateHandler_Create $C7D
#define LNSpotLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpotLightUpdateHandler_Create %1, %2
#cmd _LNSpriteSerializeHandler_Create $C88
#define LNSpriteSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSpriteSerializeHandler_Create %1, %2
#cmd _LNSpritePreUpdateHandler_Create $C93
#define LNSpritePreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpritePreUpdateHandler_Create %1, %2
#cmd _LNSpriteUpdateHandler_Create $C9E
#define LNSpriteUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpriteUpdateHandler_Create %1, %2
#cmd _LNCameraOrbitControlComponentSerializeHandler_Create $CA9
#define LNCameraOrbitControlComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCameraOrbitControlComponentSerializeHandler_Create %1, %2
#cmd _LNRaycasterSerializeHandler_Create $CB4
#define LNRaycasterSerializeHandler_Create(%1, %2=_ln_return_discard) _LNRaycasterSerializeHandler_Create %1, %2
#cmd _LNRaycastResultSerializeHandler_Create $CBF
#define LNRaycastResultSerializeHandler_Create(%1, %2=_ln_return_discard) _LNRaycastResultSerializeHandler_Create %1, %2
#cmd _LNWorldRenderViewSerializeHandler_Create $CCA
#define LNWorldRenderViewSerializeHandler_Create(%1, %2=_ln_return_discard) _LNWorldRenderViewSerializeHandler_Create %1, %2
#cmd _LNBoxMeshSerializeHandler_Create $CD5
#define LNBoxMeshSerializeHandler_Create(%1, %2=_ln_return_discard) _LNBoxMeshSerializeHandler_Create %1, %2
#cmd _LNBoxMeshPreUpdateHandler_Create $CE0
#define LNBoxMeshPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNBoxMeshPreUpdateHandler_Create %1, %2
#cmd _LNBoxMeshUpdateHandler_Create $CEB
#define LNBoxMeshUpdateHandler_Create(%1, %2=_ln_return_discard) _LNBoxMeshUpdateHandler_Create %1, %2
#cmd _LNPlaneMeshSerializeHandler_Create $CF6
#define LNPlaneMeshSerializeHandler_Create(%1, %2=_ln_return_discard) _LNPlaneMeshSerializeHandler_Create %1, %2
#cmd _LNPlaneMeshPreUpdateHandler_Create $D01
#define LNPlaneMeshPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPlaneMeshPreUpdateHandler_Create %1, %2
#cmd _LNPlaneMeshUpdateHandler_Create $D0C
#define LNPlaneMeshUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPlaneMeshUpdateHandler_Create %1, %2
#cmd _LNMeshComponentSerializeHandler_Create $D17
#define LNMeshComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshComponentSerializeHandler_Create %1, %2
#cmd _LNCollisionSerializeHandler_Create $D22
#define LNCollisionSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCollisionSerializeHandler_Create %1, %2
#cmd _LNTriggerBodyComponentSerializeHandler_Create $D2D
#define LNTriggerBodyComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNTriggerBodyComponentSerializeHandler_Create %1, %2
#cmd _LNParticleEmitterSerializeHandler_Create $D38
#define LNParticleEmitterSerializeHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterSerializeHandler_Create %1, %2
#cmd _LNParticleEmitterPreUpdateHandler_Create $D43
#define LNParticleEmitterPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterPreUpdateHandler_Create %1, %2
#cmd _LNParticleEmitterUpdateHandler_Create $D4E
#define LNParticleEmitterUpdateHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterUpdateHandler_Create %1, %2
#cmd _LNLevelSerializeHandler_Create $D59
#define LNLevelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNLevelSerializeHandler_Create %1, %2
#cmd _LNLevelStartHandler_Create $D64
#define LNLevelStartHandler_Create(%1, %2=_ln_return_discard) _LNLevelStartHandler_Create %1, %2
#cmd _LNLevelStopHandler_Create $D6F
#define LNLevelStopHandler_Create(%1, %2=_ln_return_discard) _LNLevelStopHandler_Create %1, %2
#cmd _LNLevelPauseHandler_Create $D7A
#define LNLevelPauseHandler_Create(%1, %2=_ln_return_discard) _LNLevelPauseHandler_Create %1, %2
#cmd _LNLevelResumeHandler_Create $D85
#define LNLevelResumeHandler_Create(%1, %2=_ln_return_discard) _LNLevelResumeHandler_Create %1, %2
#cmd _LNLevelUpdateHandler_Create $D90
#define LNLevelUpdateHandler_Create(%1, %2=_ln_return_discard) _LNLevelUpdateHandler_Create %1, %2
#cmd _LNUIEventArgsSerializeHandler_Create $D9B
#define LNUIEventArgsSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIEventArgsSerializeHandler_Create %1, %2
#cmd _LNUILayoutElementSerializeHandler_Create $DA6
#define LNUILayoutElementSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUILayoutElementSerializeHandler_Create %1, %2
#cmd _LNUIElementSerializeHandler_Create $DB1
#define LNUIElementSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIElementSerializeHandler_Create %1, %2
#cmd _LNUITextBlockSerializeHandler_Create $DBC
#define LNUITextBlockSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUITextBlockSerializeHandler_Create %1, %2
#cmd _LNUISpriteSerializeHandler_Create $DC7
#define LNUISpriteSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUISpriteSerializeHandler_Create %1, %2
#cmd _LNUIIconSerializeHandler_Create $DD2
#define LNUIIconSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIIconSerializeHandler_Create %1, %2
#cmd _LNUIMessageTextAreaSerializeHandler_Create $DDD
#define LNUIMessageTextAreaSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIMessageTextAreaSerializeHandler_Create %1, %2
#cmd _LNUILayoutPanelSerializeHandler_Create $DE8
#define LNUILayoutPanelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUILayoutPanelSerializeHandler_Create %1, %2
#cmd _LNUIBoxLayoutSerializeHandler_Create $DF3
#define LNUIBoxLayoutSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIBoxLayoutSerializeHandler_Create %1, %2
#cmd _LNUIStackLayoutSerializeHandler_Create $DFE
#define LNUIStackLayoutSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIStackLayoutSerializeHandler_Create %1, %2
#cmd _LNUIGridLayoutSerializeHandler_Create $E09
#define LNUIGridLayoutSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIGridLayoutSerializeHandler_Create %1, %2
#cmd _LNUIControlSerializeHandler_Create $E14
#define LNUIControlSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIControlSerializeHandler_Create %1, %2
#cmd _LNUIButtonBaseSerializeHandler_Create $E1F
#define LNUIButtonBaseSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIButtonBaseSerializeHandler_Create %1, %2
#cmd _LNUIButtonSerializeHandler_Create $E2A
#define LNUIButtonSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIButtonSerializeHandler_Create %1, %2
#cmd _LNUIWindowSerializeHandler_Create $E35
#define LNUIWindowSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIWindowSerializeHandler_Create %1, %2
#cmd _LNUIListItemSerializeHandler_Create $E40
#define LNUIListItemSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListItemSerializeHandler_Create %1, %2
#cmd _LNUIListItemsControlSerializeHandler_Create $E4B
#define LNUIListItemsControlSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListItemsControlSerializeHandler_Create %1, %2
#cmd _LNUIListBoxItemSerializeHandler_Create $E56
#define LNUIListBoxItemSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListBoxItemSerializeHandler_Create %1, %2
#cmd _LNUIListBoxSerializeHandler_Create $E61
#define LNUIListBoxSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListBoxSerializeHandler_Create %1, %2
#cmd _LNInputGestureSerializeHandler_Create $E6C
#define LNInputGestureSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInputGestureSerializeHandler_Create %1, %2
#cmd _LNKeyGestureSerializeHandler_Create $E77
#define LNKeyGestureSerializeHandler_Create(%1, %2=_ln_return_discard) _LNKeyGestureSerializeHandler_Create %1, %2
#cmd _LNInterpreterCommandSerializeHandler_Create $E82
#define LNInterpreterCommandSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterCommandSerializeHandler_Create %1, %2
#cmd _LNInterpreterCommandListSerializeHandler_Create $E8D
#define LNInterpreterCommandListSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterCommandListSerializeHandler_Create %1, %2
#cmd _LNInterpreterSerializeHandler_Create $E98
#define LNInterpreterSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterSerializeHandler_Create %1, %2
#cmd _LNInterpreterUpdateWaitHandler_Create $EA4
#define LNInterpreterUpdateWaitHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterUpdateWaitHandler_Create %1, %2
#cmd _LNApplicationSerializeHandler_Create $EAF
#define LNApplicationSerializeHandler_Create(%1, %2=_ln_return_discard) _LNApplicationSerializeHandler_Create %1, %2
#cmd _LNApplicationInitHandler_Create $EBA
#define LNApplicationInitHandler_Create(%1, %2=_ln_return_discard) _LNApplicationInitHandler_Create %1, %2
#cmd _LNApplicationUpdateHandler_Create $EC5
#define LNApplicationUpdateHandler_Create(%1, %2=_ln_return_discard) _LNApplicationUpdateHandler_Create %1, %2



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
