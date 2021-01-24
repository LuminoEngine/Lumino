
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
#cmd LNVector3_GetX $5B4
#cmd LNVector3_SetX $5B6
#cmd LNVector3_GetY $5B7
#cmd LNVector3_SetY $5B9
#cmd LNVector3_GetZ $5BA
#cmd LNVector3_SetZ $5BC
#cmd LNVector3_SetZeros $39
#cmd LNVector3_Set $3A
#cmd LNVector3_Get $3E
#cmd LNVector3_Length $42
#cmd LNVector3_LengthSquared $43
#cmd LNVector3_MutatingNormalize $44
#cmd LNVector3_NormalizeXYZ $45
#cmd LNVector3_Normalize $49
#cmd LNVector4 $4B
#cmd LNVector4_GetX $5D3
#cmd LNVector4_SetX $5D5
#cmd LNVector4_GetY $5D6
#cmd LNVector4_SetY $5D8
#cmd LNVector4_GetZ $5D9
#cmd LNVector4_SetZ $5DB
#cmd LNVector4_GetW $5DC
#cmd LNVector4_SetW $5DE
#cmd LNVector4_SetZeros $50
#cmd LNVector4_Set $51
#cmd LNQuaternion $56
#cmd LNQuaternion_GetX $5F1
#cmd LNQuaternion_SetX $5F3
#cmd LNQuaternion_GetY $5F4
#cmd LNQuaternion_SetY $5F6
#cmd LNQuaternion_GetZ $5F7
#cmd LNQuaternion_SetZ $5F9
#cmd LNQuaternion_GetW $5FA
#cmd LNQuaternion_SetW $5FC
#cmd LNQuaternion_SetZeros $5B
#cmd LNQuaternion_Set $5C
#cmd LNQuaternion_SetFromAxis $61
#cmd LNMatrix $64
#cmd LNMatrix_GetRow0 $610
#cmd LNMatrix_SetRow0 $612
#cmd LNMatrix_GetRow1 $613
#cmd LNMatrix_SetRow1 $615
#cmd LNMatrix_GetRow2 $616
#cmd LNMatrix_SetRow2 $618
#cmd LNMatrix_GetRow3 $619
#cmd LNMatrix_SetRow3 $61B
#cmd LNMatrix_SetZeros $69
#cmd LNMatrix_Set $6A
#cmd LNColor $18C
#cmd LNColor_GetR $6AF
#cmd LNColor_SetR $6B1
#cmd LNColor_GetG $6B2
#cmd LNColor_SetG $6B4
#cmd LNColor_GetB $6B5
#cmd LNColor_SetB $6B7
#cmd LNColor_GetA $6B8
#cmd LNColor_SetA $6BA
#cmd LNColor_SetZeros $191
#cmd LNColor_Set $192
#cmd LNColorTone $197
#cmd LNColorTone_GetR $6CE
#cmd LNColorTone_SetR $6D0
#cmd LNColorTone_GetG $6D1
#cmd LNColorTone_SetG $6D3
#cmd LNColorTone_GetB $6D4
#cmd LNColorTone_SetB $6D6
#cmd LNColorTone_GetS $6D7
#cmd LNColorTone_SetS $6D9
#cmd LNColorTone_SetZeros $19C
#cmd LNColorTone_Set $19D
#cmd LNPoint $1A2
#cmd LNPoint_GetX $6EC
#cmd LNPoint_SetX $6EE
#cmd LNPoint_GetY $6EF
#cmd LNPoint_SetY $6F1
#cmd LNPoint_SetZeros $1A5
#cmd LNPoint_Set $1A6
#cmd LNPoint_Get $1A9
#cmd LNSize $1AC
#cmd LNSize_GetWidth $6FD
#cmd LNSize_SetWidth $6FF
#cmd LNSize_GetHeight $700
#cmd LNSize_SetHeight $702
#cmd LNSize_SetZeros $1AF
#cmd LNSize_Set $1B0
#cmd LNRect $1B3
#cmd LNRect_GetX $70D
#cmd LNRect_SetX $70F
#cmd LNRect_GetY $710
#cmd LNRect_SetY $712
#cmd LNRect_GetWidth $713
#cmd LNRect_SetWidth $715
#cmd LNRect_GetHeight $716
#cmd LNRect_SetHeight $718
#cmd LNRect_SetZeros $1B8
#cmd LNRect_Set $1B9
#cmd LNRect_GetLeft $1BE
#cmd LNRect_SetSize $1BF
#cmd LNRect_GetSize $1C1
#cmd LNThickness $1C2
#cmd LNThickness_GetLeft $731
#cmd LNThickness_SetLeft $733
#cmd LNThickness_GetTop $734
#cmd LNThickness_SetTop $736
#cmd LNThickness_GetRight $737
#cmd LNThickness_SetRight $739
#cmd LNThickness_GetBottom $73A
#cmd LNThickness_SetBottom $73C
#cmd LNThickness_SetZeros $1C7
#cmd LNThickness_Set $1C8
#cmd LNCornerRadius $1CD
#cmd LNCornerRadius_GetTopleft $74F
#cmd LNCornerRadius_SetTopleft $751
#cmd LNCornerRadius_GetTopright $752
#cmd LNCornerRadius_SetTopright $754
#cmd LNCornerRadius_GetBottomright $755
#cmd LNCornerRadius_SetBottomright $757
#cmd LNCornerRadius_GetBottomleft $758
#cmd LNCornerRadius_SetBottomleft $75A
#cmd LNCornerRadius_SetZeros $1D2
#cmd LNCornerRadius_Set $1D3

#cmd _LNObject_Release $84
#define LNObject_Release(%1) _LNObject_Release %1
#cmd _LNObject_Retain $85
#define LNObject_Retain(%1) _LNObject_Retain %1
#cmd _LNObject_GetReferenceCount $86
#define LNObject_GetReferenceCount(%1, %2=_ln_return_discard) _LNObject_GetReferenceCount %1, %2
#cmd LNObject_SetPrototype_OnSerialize $A33
#cmd LNEventConnection_SetPrototype_OnSerialize $A3E
#cmd _LNPromiseFailureDelegate_Create $636
#define LNPromiseFailureDelegate_Create(%1, %2=_ln_return_discard) _LNPromiseFailureDelegate_Create %1, %2
#cmd _LNVariant_Create $8B
#define LNVariant_Create(%1=_ln_return_discard) _LNVariant_Create %1
#cmd _LNVariant_SetInt $8C
#define LNVariant_SetInt(%1, %2) _LNVariant_SetInt %1, %2
#cmd _LNVariant_GetInt $8E
#define LNVariant_GetInt(%1, %2=_ln_return_discard) _LNVariant_GetInt %1, %2
#cmd LNVariant_SetPrototype_OnSerialize $A49
#cmd _LNZVTestDelegate1_Create $63F
#define LNZVTestDelegate1_Create(%1, %2=_ln_return_discard) _LNZVTestDelegate1_Create %1, %2
#cmd _LNZVTestDelegate2_Create $645
#define LNZVTestDelegate2_Create(%1, %2=_ln_return_discard) _LNZVTestDelegate2_Create %1, %2
#cmd _LNZVTestDelegate3_Create $64A
#define LNZVTestDelegate3_Create(%1, %2=_ln_return_discard) _LNZVTestDelegate3_Create %1, %2
#cmd _LNZVTestEventHandler1_Create $64F
#define LNZVTestEventHandler1_Create(%1, %2=_ln_return_discard) _LNZVTestEventHandler1_Create %1, %2
#cmd _LNZVTestEventHandler2_Create $654
#define LNZVTestEventHandler2_Create(%1, %2=_ln_return_discard) _LNZVTestEventHandler2_Create %1, %2
#cmd _LNZVTestPromise1_ThenWith $657
#define LNZVTestPromise1_ThenWith(%1, %2) _LNZVTestPromise1_ThenWith %1, %2
#cmd _LNZVTestPromise1_CatchWith $659
#define LNZVTestPromise1_CatchWith(%1, %2) _LNZVTestPromise1_CatchWith %1, %2
#cmd _LNZVTestPromise2_ThenWith $65D
#define LNZVTestPromise2_ThenWith(%1, %2) _LNZVTestPromise2_ThenWith %1, %2
#cmd _LNZVTestPromise2_CatchWith $65F
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $A54
#cmd _LNZVTestEventArgs1_Create $BA
#define LNZVTestEventArgs1_Create(%1=_ln_return_discard) _LNZVTestEventArgs1_Create %1
#cmd _LNZVTestEventArgs1_CreateWithValue $BB
#define LNZVTestEventArgs1_CreateWithValue(%1, %2=_ln_return_discard) _LNZVTestEventArgs1_CreateWithValue %1, %2
#cmd _LNZVTestEventArgs1_GetValue $B9
#define LNZVTestEventArgs1_GetValue(%1, %2=_ln_return_discard) _LNZVTestEventArgs1_GetValue %1, %2
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $A5F
#cmd _LNLog_SetLevel $BE
#define LNLog_SetLevel(%1) _LNLog_SetLevel %1
#cmd _LNLog_AllocConsole $C0
#define LNLog_AllocConsole _LNLog_AllocConsole
#cmd _LNLog_Write $C1
#define LNLog_Write(%1, %2, %3) _LNLog_Write %1, %2, %3
#cmd LNSerializer2_SetPrototype_OnSerialize $A6A
#cmd LNAssetObject_SetPrototype_OnSerialize $A75
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $A80
#cmd _LNAssetModel_Create $125
#define LNAssetModel_Create(%1, %2=_ln_return_discard) _LNAssetModel_Create %1, %2
#cmd _LNAssetModel_Target $124
#define LNAssetModel_Target(%1, %2=_ln_return_discard) _LNAssetModel_Target %1, %2
#cmd LNAssetModel_SetPrototype_OnSerialize $A8B
#cmd _LNAssets_SaveAssetToLocalFile $128
#define LNAssets_SaveAssetToLocalFile(%1, %2) _LNAssets_SaveAssetToLocalFile %1, %2
#cmd _LNAssets_LoadAssetFromLocalFile $12B
#define LNAssets_LoadAssetFromLocalFile(%1, %2=_ln_return_discard) _LNAssets_LoadAssetFromLocalFile %1, %2
#cmd _LNAssets_LoadAsset $12D
#define LNAssets_LoadAsset(%1, %2=_ln_return_discard) _LNAssets_LoadAsset %1, %2
#cmd _LNAssets_ReloadAsset $12F
#define LNAssets_ReloadAsset(%1, %2) _LNAssets_ReloadAsset %1, %2
#cmd _LNAssets_ReadAllText $132
#define LNAssets_ReadAllText(%1, %2=LN_ENCODING_TYPE_UNKNOWN, %3=_ln_return_discard) _LNAssets_ReadAllText %1, %2, %3
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
#cmd LNSound_SetPrototype_OnSerialize $A96
#cmd _LNAudio_PlayBGM $151
#define LNAudio_PlayBGM(%1, %2=1.000000, %3=1.000000, %4=0.000000) _LNAudio_PlayBGM %1, %2, %3, %4
#cmd _LNAudio_StopBGM $156
#define LNAudio_StopBGM(%1=0.000000) _LNAudio_StopBGM %1
#cmd _LNAudio_PlayBGS $158
#define LNAudio_PlayBGS(%1, %2=1.000000, %3=1.000000, %4=0.000000) _LNAudio_PlayBGS %1, %2, %3, %4
#cmd _LNAudio_StopBGS $15D
#define LNAudio_StopBGS(%1=0.000000) _LNAudio_StopBGS %1
#cmd _LNAudio_PlayME $15F
#define LNAudio_PlayME(%1, %2=1.000000, %3=1.000000) _LNAudio_PlayME %1, %2, %3
#cmd _LNAudio_StopME $163
#define LNAudio_StopME _LNAudio_StopME
#cmd _LNAudio_PlaySE $164
#define LNAudio_PlaySE(%1, %2=1.000000, %3=1.000000) _LNAudio_PlaySE %1, %2, %3
#cmd _LNAudio_PlaySE3D $168
#define LNAudio_PlaySE3D(%1, %2, %3, %4=1.000000, %5=1.000000) _LNAudio_PlaySE3D %1, %2, %3, %4, %5
#cmd _LNAudio_StopSE $16E
#define LNAudio_StopSE _LNAudio_StopSE
#cmd _LNTexture2DDelegate_Create $6A5
#define LNTexture2DDelegate_Create(%1, %2=_ln_return_discard) _LNTexture2DDelegate_Create %1, %2
#cmd _LNTexture2DPromise_ThenWith $6A8
#define LNTexture2DPromise_ThenWith(%1, %2) _LNTexture2DPromise_ThenWith %1, %2
#cmd _LNTexture2DPromise_CatchWith $6AA
#define LNTexture2DPromise_CatchWith(%1, %2) _LNTexture2DPromise_CatchWith %1, %2
#cmd _LNGraphics_GetActiveGraphicsAPI $18B
#define LNGraphics_GetActiveGraphicsAPI(%1=_ln_return_discard) _LNGraphics_GetActiveGraphicsAPI %1
#cmd LNTexture_SetPrototype_OnSerialize $AA1
#cmd _LNTexture2D_Create $1DE
#define LNTexture2D_Create(%1, %2, %3=_ln_return_discard) _LNTexture2D_Create %1, %2, %3
#cmd _LNTexture2D_CreateWithFormat $1E1
#define LNTexture2D_CreateWithFormat(%1, %2, %3, %4=_ln_return_discard) _LNTexture2D_CreateWithFormat %1, %2, %3, %4
#cmd _LNTexture2D_Load $1DA
#define LNTexture2D_Load(%1, %2=_ln_return_discard) _LNTexture2D_Load %1, %2
#cmd _LNTexture2D_LoadEmoji $1DC
#define LNTexture2D_LoadEmoji(%1, %2=_ln_return_discard) _LNTexture2D_LoadEmoji %1, %2
#cmd LNTexture2D_SetPrototype_OnSerialize $AAC
#cmd _LNShader_Load $1E6
#define LNShader_Load(%1, %2=LN_NULL_HANDLE, %3=_ln_return_discard) _LNShader_Load %1, %2, %3
#cmd _LNShader_SetFloat $1E9
#define LNShader_SetFloat(%1, %2, %3) _LNShader_SetFloat %1, %2, %3
#cmd _LNShader_SetVector3 $1EC
#define LNShader_SetVector3(%1, %2, %3) _LNShader_SetVector3 %1, %2, %3
#cmd _LNShader_SetVector4 $1EF
#define LNShader_SetVector4(%1, %2, %3) _LNShader_SetVector4 %1, %2, %3
#cmd _LNShader_SetTexture $1F2
#define LNShader_SetTexture(%1, %2, %3) _LNShader_SetTexture %1, %2, %3
#cmd LNShader_SetPrototype_OnSerialize $AB7
#cmd LNRenderView_SetPrototype_OnSerialize $AC2
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
#cmd LNMaterial_SetPrototype_OnSerialize $ACD
#cmd _LNMeshNode_SetVisible $219
#define LNMeshNode_SetVisible(%1, %2) _LNMeshNode_SetVisible %1, %2
#cmd _LNMeshNode_IsVisible $21B
#define LNMeshNode_IsVisible(%1, %2=_ln_return_discard) _LNMeshNode_IsVisible %1, %2
#cmd LNMeshNode_SetPrototype_OnSerialize $AD8
#cmd _LNAnimationController_AddClip $21D
#define LNAnimationController_AddClip(%1, %2, %3=_ln_return_discard) _LNAnimationController_AddClip %1, %2, %3
#cmd _LNAnimationController_Play $21F
#define LNAnimationController_Play(%1, %2, %3=0.300000) _LNAnimationController_Play %1, %2, %3
#cmd LNAnimationController_SetPrototype_OnSerialize $AE3
#cmd _LNMeshModel_Load $223
#define LNMeshModel_Load(%1, %2=LN_NULL_HANDLE, %3=_ln_return_discard) _LNMeshModel_Load %1, %2, %3
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
#cmd LNMeshModel_SetPrototype_OnSerialize $AEE
#cmd _LNMeshImportSettings_Create $22F
#define LNMeshImportSettings_Create(%1=_ln_return_discard) _LNMeshImportSettings_Create %1
#cmd LNMeshImportSettings_SetPrototype_OnSerialize $AF9
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $B04
#cmd LNCollisionShape_SetPrototype_OnSerialize $B0F
#cmd _LNBoxCollisionShape_Create $233
#define LNBoxCollisionShape_Create(%1, %2=_ln_return_discard) _LNBoxCollisionShape_Create %1, %2
#cmd _LNBoxCollisionShape_CreateWHD $235
#define LNBoxCollisionShape_CreateWHD(%1, %2, %3, %4=_ln_return_discard) _LNBoxCollisionShape_CreateWHD %1, %2, %3, %4
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $B1A
#cmd _LNAnimationCurve_Evaluate $243
#define LNAnimationCurve_Evaluate(%1, %2, %3=_ln_return_discard) _LNAnimationCurve_Evaluate %1, %2, %3
#cmd LNAnimationCurve_SetPrototype_OnSerialize $B25
#cmd _LNKeyFrameAnimationCurve_Create $250
#define LNKeyFrameAnimationCurve_Create(%1=_ln_return_discard) _LNKeyFrameAnimationCurve_Create %1
#cmd _LNKeyFrameAnimationCurve_AddKeyFrame $24B
#define LNKeyFrameAnimationCurve_AddKeyFrame(%1, %2, %3, %4=LN_TANGENT_MODE_LINEAR, %5=0.000000) _LNKeyFrameAnimationCurve_AddKeyFrame %1, %2, %3, %4, %5
#cmd LNKeyFrameAnimationCurve_SetPrototype_OnSerialize $B30
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
#cmd LNAnimationClip_SetPrototype_OnSerialize $B3B
#cmd LNAnimationState_SetPrototype_OnSerialize $B46
#cmd LNEffectResource_SetPrototype_OnSerialize $B51
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
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $B5C
#cmd _LNParticleModel_Create $285
#define LNParticleModel_Create(%1=_ln_return_discard) _LNParticleModel_Create %1
#cmd _LNParticleModel_SetLoop $280
#define LNParticleModel_SetLoop(%1, %2) _LNParticleModel_SetLoop %1, %2
#cmd _LNParticleModel_IsLoop $282
#define LNParticleModel_IsLoop(%1, %2=_ln_return_discard) _LNParticleModel_IsLoop %1, %2
#cmd _LNParticleModel_AddEmitter $283
#define LNParticleModel_AddEmitter(%1, %2) _LNParticleModel_AddEmitter %1, %2
#cmd LNParticleModel_SetPrototype_OnSerialize $B67
#cmd LNComponent_SetPrototype_OnSerialize $B72
#cmd _LNVisualComponent_SetVisible $288
#define LNVisualComponent_SetVisible(%1, %2) _LNVisualComponent_SetVisible %1, %2
#cmd _LNVisualComponent_IsVisible $28A
#define LNVisualComponent_IsVisible(%1, %2=_ln_return_discard) _LNVisualComponent_IsVisible %1, %2
#cmd LNVisualComponent_SetPrototype_OnSerialize $B7D
#cmd _LNSpriteComponent_SetTexture $28C
#define LNSpriteComponent_SetTexture(%1, %2) _LNSpriteComponent_SetTexture %1, %2
#cmd LNSpriteComponent_SetPrototype_OnSerialize $B88
#cmd _LNCollisionEventHandler_Create $7DF
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
#cmd LNCharacterController_SetPrototype_OnSerialize $B93
#cmd _LNWorld_Add $2AE
#define LNWorld_Add(%1, %2) _LNWorld_Add %1, %2
#cmd LNWorld_SetPrototype_OnSerialize $B9E
#cmd _LNComponentList_GetLength $7FF
#define LNComponentList_GetLength(%1, %2=_ln_return_discard) _LNComponentList_GetLength %1, %2
#cmd _LNComponentList_GetItem $801
#define LNComponentList_GetItem(%1, %2, %3=_ln_return_discard) _LNComponentList_GetItem %1, %2, %3
#cmd LNComponentList_SetPrototype_OnSerialize $BA9
#cmd _LNWorldObject_Create $2E7
#define LNWorldObject_Create(%1=_ln_return_discard) _LNWorldObject_Create %1
#cmd _LNWorldObject_SetPosition $2B2
#define LNWorldObject_SetPosition(%1, %2) _LNWorldObject_SetPosition %1, %2
#cmd _LNWorldObject_SetPositionXYZ $2B4
#define LNWorldObject_SetPositionXYZ(%1, %2, %3, %4=0.000000) _LNWorldObject_SetPositionXYZ %1, %2, %3, %4
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
#define LNWorldObject_SetScaleXYZ(%1, %2, %3, %4=1.000000) _LNWorldObject_SetScaleXYZ %1, %2, %3, %4
#cmd _LNWorldObject_GetScale $2C8
#define LNWorldObject_GetScale(%1, %2=_ln_return_discard) _LNWorldObject_GetScale %1, %2
#cmd _LNWorldObject_SetCenterPoint $2C9
#define LNWorldObject_SetCenterPoint(%1, %2) _LNWorldObject_SetCenterPoint %1, %2
#cmd _LNWorldObject_SetCenterPointXYZ $2CB
#define LNWorldObject_SetCenterPointXYZ(%1, %2, %3, %4=0.000000) _LNWorldObject_SetCenterPointXYZ %1, %2, %3, %4
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
#cmd _LNWorldObject_AddInto $2E2
#define LNWorldObject_AddInto(%1, %2=LN_NULL_HANDLE) _LNWorldObject_AddInto %1, %2
#cmd LNWorldObject_SetPrototype_OnSerialize $BB4
#cmd LNWorldObject_SetPrototype_OnPreUpdate $BBF
#cmd LNWorldObject_SetPrototype_OnUpdate $BCA
#cmd _LNVisualObject_SetVisible $2E9
#define LNVisualObject_SetVisible(%1, %2) _LNVisualObject_SetVisible %1, %2
#cmd _LNVisualObject_IsVisible $2EB
#define LNVisualObject_IsVisible(%1, %2=_ln_return_discard) _LNVisualObject_IsVisible %1, %2
#cmd _LNVisualObject_SetBlendMode2 $2EC
#define LNVisualObject_SetBlendMode2(%1, %2) _LNVisualObject_SetBlendMode2 %1, %2
#cmd _LNVisualObject_SetOpacity $2EE
#define LNVisualObject_SetOpacity(%1, %2) _LNVisualObject_SetOpacity %1, %2
#cmd _LNVisualObject_GetOpacity $2F0
#define LNVisualObject_GetOpacity(%1, %2=_ln_return_discard) _LNVisualObject_GetOpacity %1, %2
#cmd LNVisualObject_SetPrototype_OnSerialize $BD5
#cmd LNVisualObject_SetPrototype_OnPreUpdate $BE0
#cmd LNVisualObject_SetPrototype_OnUpdate $BEB
#cmd LNCamera_SetPrototype_OnSerialize $BF6
#cmd LNCamera_SetPrototype_OnPreUpdate $C01
#cmd LNCamera_SetPrototype_OnUpdate $C0C
#cmd _LNEnvironmentLight_SetEnabled $2F3
#define LNEnvironmentLight_SetEnabled(%1, %2) _LNEnvironmentLight_SetEnabled %1, %2
#cmd _LNEnvironmentLight_IsEnabled $2F5
#define LNEnvironmentLight_IsEnabled(%1, %2=_ln_return_discard) _LNEnvironmentLight_IsEnabled %1, %2
#cmd _LNEnvironmentLight_SetColor $2F6
#define LNEnvironmentLight_SetColor(%1, %2) _LNEnvironmentLight_SetColor %1, %2
#cmd _LNEnvironmentLight_GetColor $2F8
#define LNEnvironmentLight_GetColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetColor %1, %2
#cmd _LNEnvironmentLight_SetAmbientColor $2F9
#define LNEnvironmentLight_SetAmbientColor(%1, %2) _LNEnvironmentLight_SetAmbientColor %1, %2
#cmd _LNEnvironmentLight_GetAmbientColor $2FB
#define LNEnvironmentLight_GetAmbientColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetAmbientColor %1, %2
#cmd _LNEnvironmentLight_GetSkyColor $2FC
#define LNEnvironmentLight_GetSkyColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetSkyColor %1, %2
#cmd _LNEnvironmentLight_SetSkyColor $2FD
#define LNEnvironmentLight_SetSkyColor(%1, %2) _LNEnvironmentLight_SetSkyColor %1, %2
#cmd _LNEnvironmentLight_GetGroundColor $2FF
#define LNEnvironmentLight_GetGroundColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetGroundColor %1, %2
#cmd _LNEnvironmentLight_SetGroundColor $300
#define LNEnvironmentLight_SetGroundColor(%1, %2) _LNEnvironmentLight_SetGroundColor %1, %2
#cmd _LNEnvironmentLight_SetIntensity $302
#define LNEnvironmentLight_SetIntensity(%1, %2) _LNEnvironmentLight_SetIntensity %1, %2
#cmd _LNEnvironmentLight_GetIntensity $304
#define LNEnvironmentLight_GetIntensity(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetIntensity %1, %2
#cmd _LNEnvironmentLight_SetShadowEffectiveDistance $305
#define LNEnvironmentLight_SetShadowEffectiveDistance(%1, %2) _LNEnvironmentLight_SetShadowEffectiveDistance %1, %2
#cmd _LNEnvironmentLight_GetShadowEffectiveDistance $307
#define LNEnvironmentLight_GetShadowEffectiveDistance(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetShadowEffectiveDistance %1, %2
#cmd _LNEnvironmentLight_SetShadowEffectiveDepth $308
#define LNEnvironmentLight_SetShadowEffectiveDepth(%1, %2) _LNEnvironmentLight_SetShadowEffectiveDepth %1, %2
#cmd _LNEnvironmentLight_GetShadowEffectiveDepth $30A
#define LNEnvironmentLight_GetShadowEffectiveDepth(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetShadowEffectiveDepth %1, %2
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $C17
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $C22
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $C2D
#cmd _LNDirectionalLight_Create $31B
#define LNDirectionalLight_Create(%1=_ln_return_discard) _LNDirectionalLight_Create %1
#cmd _LNDirectionalLight_CreateWithColor $31C
#define LNDirectionalLight_CreateWithColor(%1, %2=_ln_return_discard) _LNDirectionalLight_CreateWithColor %1, %2
#cmd _LNDirectionalLight_SetEnabled $30C
#define LNDirectionalLight_SetEnabled(%1, %2) _LNDirectionalLight_SetEnabled %1, %2
#cmd _LNDirectionalLight_IsEnabled $30E
#define LNDirectionalLight_IsEnabled(%1, %2=_ln_return_discard) _LNDirectionalLight_IsEnabled %1, %2
#cmd _LNDirectionalLight_SetColor $30F
#define LNDirectionalLight_SetColor(%1, %2) _LNDirectionalLight_SetColor %1, %2
#cmd _LNDirectionalLight_GetColor $311
#define LNDirectionalLight_GetColor(%1, %2=_ln_return_discard) _LNDirectionalLight_GetColor %1, %2
#cmd _LNDirectionalLight_SetIntensity $312
#define LNDirectionalLight_SetIntensity(%1, %2) _LNDirectionalLight_SetIntensity %1, %2
#cmd _LNDirectionalLight_GetIntensity $314
#define LNDirectionalLight_GetIntensity(%1, %2=_ln_return_discard) _LNDirectionalLight_GetIntensity %1, %2
#cmd _LNDirectionalLight_SetShadowEffectiveDistance $315
#define LNDirectionalLight_SetShadowEffectiveDistance(%1, %2) _LNDirectionalLight_SetShadowEffectiveDistance %1, %2
#cmd _LNDirectionalLight_GetShadowEffectiveDistance $317
#define LNDirectionalLight_GetShadowEffectiveDistance(%1, %2=_ln_return_discard) _LNDirectionalLight_GetShadowEffectiveDistance %1, %2
#cmd _LNDirectionalLight_SetShadowEffectiveDepth $318
#define LNDirectionalLight_SetShadowEffectiveDepth(%1, %2) _LNDirectionalLight_SetShadowEffectiveDepth %1, %2
#cmd _LNDirectionalLight_GetShadowEffectiveDepth $31A
#define LNDirectionalLight_GetShadowEffectiveDepth(%1, %2=_ln_return_discard) _LNDirectionalLight_GetShadowEffectiveDepth %1, %2
#cmd LNDirectionalLight_SetPrototype_OnSerialize $C38
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $C43
#cmd LNDirectionalLight_SetPrototype_OnUpdate $C4E
#cmd _LNPointLight_Create $32E
#define LNPointLight_Create(%1=_ln_return_discard) _LNPointLight_Create %1
#cmd _LNPointLight_CreateWithColorAndRange $32F
#define LNPointLight_CreateWithColorAndRange(%1, %2, %3=_ln_return_discard) _LNPointLight_CreateWithColorAndRange %1, %2, %3
#cmd _LNPointLight_SetEnabled $31F
#define LNPointLight_SetEnabled(%1, %2) _LNPointLight_SetEnabled %1, %2
#cmd _LNPointLight_IsEnabled $321
#define LNPointLight_IsEnabled(%1, %2=_ln_return_discard) _LNPointLight_IsEnabled %1, %2
#cmd _LNPointLight_SetColor $322
#define LNPointLight_SetColor(%1, %2) _LNPointLight_SetColor %1, %2
#cmd _LNPointLight_GetColor $324
#define LNPointLight_GetColor(%1, %2=_ln_return_discard) _LNPointLight_GetColor %1, %2
#cmd _LNPointLight_SetIntensity $325
#define LNPointLight_SetIntensity(%1, %2) _LNPointLight_SetIntensity %1, %2
#cmd _LNPointLight_GetIntensity $327
#define LNPointLight_GetIntensity(%1, %2=_ln_return_discard) _LNPointLight_GetIntensity %1, %2
#cmd _LNPointLight_SetRange $328
#define LNPointLight_SetRange(%1, %2) _LNPointLight_SetRange %1, %2
#cmd _LNPointLight_GetRange $32A
#define LNPointLight_GetRange(%1, %2=_ln_return_discard) _LNPointLight_GetRange %1, %2
#cmd _LNPointLight_SetAttenuation $32B
#define LNPointLight_SetAttenuation(%1, %2) _LNPointLight_SetAttenuation %1, %2
#cmd _LNPointLight_GetAttenuation $32D
#define LNPointLight_GetAttenuation(%1, %2=_ln_return_discard) _LNPointLight_GetAttenuation %1, %2
#cmd LNPointLight_SetPrototype_OnSerialize $C59
#cmd LNPointLight_SetPrototype_OnPreUpdate $C64
#cmd LNPointLight_SetPrototype_OnUpdate $C6F
#cmd _LNSpotLight_Create $348
#define LNSpotLight_Create(%1=_ln_return_discard) _LNSpotLight_Create %1
#cmd _LNSpotLight_CreateWithColorAndRange $349
#define LNSpotLight_CreateWithColorAndRange(%1, %2, %3, %4=_ln_return_discard) _LNSpotLight_CreateWithColorAndRange %1, %2, %3, %4
#cmd _LNSpotLight_SetEnabled $333
#define LNSpotLight_SetEnabled(%1, %2) _LNSpotLight_SetEnabled %1, %2
#cmd _LNSpotLight_IsEnabled $335
#define LNSpotLight_IsEnabled(%1, %2=_ln_return_discard) _LNSpotLight_IsEnabled %1, %2
#cmd _LNSpotLight_SetColor $336
#define LNSpotLight_SetColor(%1, %2) _LNSpotLight_SetColor %1, %2
#cmd _LNSpotLight_GetColor $338
#define LNSpotLight_GetColor(%1, %2=_ln_return_discard) _LNSpotLight_GetColor %1, %2
#cmd _LNSpotLight_SetIntensity $339
#define LNSpotLight_SetIntensity(%1, %2) _LNSpotLight_SetIntensity %1, %2
#cmd _LNSpotLight_GetIntensity $33B
#define LNSpotLight_GetIntensity(%1, %2=_ln_return_discard) _LNSpotLight_GetIntensity %1, %2
#cmd _LNSpotLight_SetRange $33C
#define LNSpotLight_SetRange(%1, %2) _LNSpotLight_SetRange %1, %2
#cmd _LNSpotLight_GetRange $33E
#define LNSpotLight_GetRange(%1, %2=_ln_return_discard) _LNSpotLight_GetRange %1, %2
#cmd _LNSpotLight_SetAttenuation $33F
#define LNSpotLight_SetAttenuation(%1, %2) _LNSpotLight_SetAttenuation %1, %2
#cmd _LNSpotLight_GetAttenuation $341
#define LNSpotLight_GetAttenuation(%1, %2=_ln_return_discard) _LNSpotLight_GetAttenuation %1, %2
#cmd _LNSpotLight_SetAngle $342
#define LNSpotLight_SetAngle(%1, %2) _LNSpotLight_SetAngle %1, %2
#cmd _LNSpotLight_GetAngle $344
#define LNSpotLight_GetAngle(%1, %2=_ln_return_discard) _LNSpotLight_GetAngle %1, %2
#cmd _LNSpotLight_SetPenumbra $345
#define LNSpotLight_SetPenumbra(%1, %2) _LNSpotLight_SetPenumbra %1, %2
#cmd _LNSpotLight_GetPenumbra $347
#define LNSpotLight_GetPenumbra(%1, %2=_ln_return_discard) _LNSpotLight_GetPenumbra %1, %2
#cmd LNSpotLight_SetPrototype_OnSerialize $C7A
#cmd LNSpotLight_SetPrototype_OnPreUpdate $C85
#cmd LNSpotLight_SetPrototype_OnUpdate $C90
#cmd _LNTestDelegate_Create $8AF
#define LNTestDelegate_Create(%1, %2=_ln_return_discard) _LNTestDelegate_Create %1, %2
#cmd _LNSprite_Create $35F
#define LNSprite_Create(%1=_ln_return_discard) _LNSprite_Create %1
#cmd _LNSprite_CreateWithTexture $360
#define LNSprite_CreateWithTexture(%1, %2=_ln_return_discard) _LNSprite_CreateWithTexture %1, %2
#cmd _LNSprite_CreateWithTextureAndSize $362
#define LNSprite_CreateWithTextureAndSize(%1, %2, %3, %4=_ln_return_discard) _LNSprite_CreateWithTextureAndSize %1, %2, %3, %4
#cmd _LNSprite_SetTexture $351
#define LNSprite_SetTexture(%1, %2) _LNSprite_SetTexture %1, %2
#cmd _LNSprite_SetSize $353
#define LNSprite_SetSize(%1, %2) _LNSprite_SetSize %1, %2
#cmd _LNSprite_SetSizeWH $355
#define LNSprite_SetSizeWH(%1, %2, %3) _LNSprite_SetSizeWH %1, %2, %3
#cmd _LNSprite_SetSourceRectXYWH $358
#define LNSprite_SetSourceRectXYWH(%1, %2, %3, %4, %5) _LNSprite_SetSourceRectXYWH %1, %2, %3, %4, %5
#cmd _LNSprite_SetCallerTest $35D
#define LNSprite_SetCallerTest(%1, %2) _LNSprite_SetCallerTest %1, %2
#cmd LNSprite_SetPrototype_OnSerialize $C9B
#cmd LNSprite_SetPrototype_OnPreUpdate $CA6
#cmd LNSprite_SetPrototype_OnUpdate $CB1
#cmd _LNCameraOrbitControlComponent_Create $367
#define LNCameraOrbitControlComponent_Create(%1=_ln_return_discard) _LNCameraOrbitControlComponent_Create %1
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $CBC
#cmd _LNRaycaster_FromScreen $369
#define LNRaycaster_FromScreen(%1, %2=_ln_return_discard) _LNRaycaster_FromScreen %1, %2
#cmd _LNRaycaster_IntersectPlane $36B
#define LNRaycaster_IntersectPlane(%1, %2, %3, %4, %5=_ln_return_discard) _LNRaycaster_IntersectPlane %1, %2, %3, %4, %5
#cmd LNRaycaster_SetPrototype_OnSerialize $CC7
#cmd _LNRaycastResult_GetPoint $370
#define LNRaycastResult_GetPoint(%1, %2=_ln_return_discard) _LNRaycastResult_GetPoint %1, %2
#cmd LNRaycastResult_SetPrototype_OnSerialize $CD2
#cmd _LNWorldRenderView_SetGuideGridEnabled $372
#define LNWorldRenderView_SetGuideGridEnabled(%1, %2) _LNWorldRenderView_SetGuideGridEnabled %1, %2
#cmd _LNWorldRenderView_GetGuideGridEnabled $374
#define LNWorldRenderView_GetGuideGridEnabled(%1, %2=_ln_return_discard) _LNWorldRenderView_GetGuideGridEnabled %1, %2
#cmd LNWorldRenderView_SetPrototype_OnSerialize $CDD
#cmd LNShapeObject_SetPrototype_OnSerialize $CE8
#cmd LNShapeObject_SetPrototype_OnPreUpdate $CF3
#cmd LNShapeObject_SetPrototype_OnUpdate $CFE
#cmd _LNPlaneMesh_Create $377
#define LNPlaneMesh_Create(%1=_ln_return_discard) _LNPlaneMesh_Create %1
#cmd LNPlaneMesh_SetPrototype_OnSerialize $D09
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $D14
#cmd LNPlaneMesh_SetPrototype_OnUpdate $D1F
#cmd _LNBoxMesh_Create $379
#define LNBoxMesh_Create(%1=_ln_return_discard) _LNBoxMesh_Create %1
#cmd _LNBoxMesh_CreateWithSize $37A
#define LNBoxMesh_CreateWithSize(%1, %2, %3, %4=_ln_return_discard) _LNBoxMesh_CreateWithSize %1, %2, %3, %4
#cmd LNBoxMesh_SetPrototype_OnSerialize $D2A
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $D35
#cmd LNBoxMesh_SetPrototype_OnUpdate $D40
#cmd _LNMeshComponent_Create $383
#define LNMeshComponent_Create(%1=_ln_return_discard) _LNMeshComponent_Create %1
#cmd _LNMeshComponent_SetModel $37F
#define LNMeshComponent_SetModel(%1, %2) _LNMeshComponent_SetModel %1, %2
#cmd _LNMeshComponent_MakeCollisionBody $381
#define LNMeshComponent_MakeCollisionBody(%1, %2) _LNMeshComponent_MakeCollisionBody %1, %2
#cmd LNMeshComponent_SetPrototype_OnSerialize $D4B
#cmd _LNCollision_GetWorldObject $385
#define LNCollision_GetWorldObject(%1, %2=_ln_return_discard) _LNCollision_GetWorldObject %1, %2
#cmd LNCollision_SetPrototype_OnSerialize $D56
#cmd _LNTriggerBodyComponent_Create $389
#define LNTriggerBodyComponent_Create(%1=_ln_return_discard) _LNTriggerBodyComponent_Create %1
#cmd _LNTriggerBodyComponent_AddCollisionShape $387
#define LNTriggerBodyComponent_AddCollisionShape(%1, %2) _LNTriggerBodyComponent_AddCollisionShape %1, %2
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $D61
#cmd _LNParticleEmitter_Create $38B
#define LNParticleEmitter_Create(%1, %2=_ln_return_discard) _LNParticleEmitter_Create %1, %2
#cmd LNParticleEmitter_SetPrototype_OnSerialize $D6C
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $D77
#cmd LNParticleEmitter_SetPrototype_OnUpdate $D82
#cmd _LNScene_SetClearMode $38E
#define LNScene_SetClearMode(%1) _LNScene_SetClearMode %1
#cmd _LNScene_SetSkyColor $390
#define LNScene_SetSkyColor(%1) _LNScene_SetSkyColor %1
#cmd _LNScene_SetSkyHorizonColor $392
#define LNScene_SetSkyHorizonColor(%1) _LNScene_SetSkyHorizonColor %1
#cmd _LNScene_SetSkyCloudColor $394
#define LNScene_SetSkyCloudColor(%1) _LNScene_SetSkyCloudColor %1
#cmd _LNScene_SetSkyOverlayColor $396
#define LNScene_SetSkyOverlayColor(%1) _LNScene_SetSkyOverlayColor %1
#cmd _LNScene_GotoLevel $398
#define LNScene_GotoLevel(%1, %2=LN_TRUE) _LNScene_GotoLevel %1, %2
#cmd _LNScene_CallLevel $39B
#define LNScene_CallLevel(%1, %2=LN_TRUE) _LNScene_CallLevel %1, %2
#cmd _LNScene_ReturnLevel $39E
#define LNScene_ReturnLevel(%1=LN_TRUE) _LNScene_ReturnLevel %1
#cmd _LNScene_ActiveLevel $3A0
#define LNScene_ActiveLevel(%1=_ln_return_discard) _LNScene_ActiveLevel %1
#cmd _LNScene_IsTransitionEffectRunning $3A1
#define LNScene_IsTransitionEffectRunning(%1=_ln_return_discard) _LNScene_IsTransitionEffectRunning %1
#cmd _LNScene_SetTransitionEffectMode $3A2
#define LNScene_SetTransitionEffectMode(%1) _LNScene_SetTransitionEffectMode %1
#cmd _LNScene_TransitionEffectMode $3A4
#define LNScene_TransitionEffectMode(%1=_ln_return_discard) _LNScene_TransitionEffectMode %1
#cmd _LNScene_SetTransitionDuration $3A5
#define LNScene_SetTransitionDuration(%1) _LNScene_SetTransitionDuration %1
#cmd _LNScene_TransitionDuration $3A7
#define LNScene_TransitionDuration(%1=_ln_return_discard) _LNScene_TransitionDuration %1
#cmd _LNScene_SetTransitionEffectColor $3A8
#define LNScene_SetTransitionEffectColor(%1) _LNScene_SetTransitionEffectColor %1
#cmd _LNScene_TransitionEffectColor $3AA
#define LNScene_TransitionEffectColor(%1=_ln_return_discard) _LNScene_TransitionEffectColor %1
#cmd _LNScene_SetTransitionEffectMaskTexture $3AB
#define LNScene_SetTransitionEffectMaskTexture(%1) _LNScene_SetTransitionEffectMaskTexture %1
#cmd _LNScene_TransitionEffectMaskTexture $3AD
#define LNScene_TransitionEffectMaskTexture(%1=_ln_return_discard) _LNScene_TransitionEffectMaskTexture %1
#cmd _LNScene_SetTransitionEffectVague $3AE
#define LNScene_SetTransitionEffectVague(%1) _LNScene_SetTransitionEffectVague %1
#cmd _LNScene_TransitionEffectVague $3B0
#define LNScene_TransitionEffectVague(%1=_ln_return_discard) _LNScene_TransitionEffectVague %1
#cmd _LNScene_StartFadeOut $3B1
#define LNScene_StartFadeOut _LNScene_StartFadeOut
#cmd _LNScene_StartFadeIn $3B2
#define LNScene_StartFadeIn _LNScene_StartFadeIn
#cmd _LNScene_SetFogStartDistance $3B3
#define LNScene_SetFogStartDistance(%1) _LNScene_SetFogStartDistance %1
#cmd _LNScene_SetFogColor $3B5
#define LNScene_SetFogColor(%1) _LNScene_SetFogColor %1
#cmd _LNScene_SetFogDensity $3B7
#define LNScene_SetFogDensity(%1) _LNScene_SetFogDensity %1
#cmd _LNScene_SetFogHeightDensity $3B9
#define LNScene_SetFogHeightDensity(%1) _LNScene_SetFogHeightDensity %1
#cmd _LNScene_SetFogLowerHeight $3BB
#define LNScene_SetFogLowerHeight(%1) _LNScene_SetFogLowerHeight %1
#cmd _LNScene_SetFogUpperHeight $3BD
#define LNScene_SetFogUpperHeight(%1) _LNScene_SetFogUpperHeight %1
#cmd _LNScene_SetHDREnabled $3BF
#define LNScene_SetHDREnabled(%1) _LNScene_SetHDREnabled %1
#cmd _LNScene_IsHDREnabled $3C1
#define LNScene_IsHDREnabled(%1=_ln_return_discard) _LNScene_IsHDREnabled %1
#cmd _LNScene_SetScreenBlendColor $3C2
#define LNScene_SetScreenBlendColor(%1) _LNScene_SetScreenBlendColor %1
#cmd _LNScene_ScreenBlendColor $3C4
#define LNScene_ScreenBlendColor(%1=_ln_return_discard) _LNScene_ScreenBlendColor %1
#cmd _LNScene_SetColorTone $3C5
#define LNScene_SetColorTone(%1) _LNScene_SetColorTone %1
#cmd _LNScene_ColorTone $3C7
#define LNScene_ColorTone(%1=_ln_return_discard) _LNScene_ColorTone %1
#cmd _LNScene_SetAntialiasEnabled $3C8
#define LNScene_SetAntialiasEnabled(%1) _LNScene_SetAntialiasEnabled %1
#cmd _LNScene_IsAntialiasEnabled $3CA
#define LNScene_IsAntialiasEnabled(%1=_ln_return_discard) _LNScene_IsAntialiasEnabled %1
#cmd _LNScene_SetSSREnabled $3CB
#define LNScene_SetSSREnabled(%1) _LNScene_SetSSREnabled %1
#cmd _LNScene_IsSSREnabled $3CD
#define LNScene_IsSSREnabled(%1=_ln_return_discard) _LNScene_IsSSREnabled %1
#cmd _LNScene_SetSSAOEnabled $3CE
#define LNScene_SetSSAOEnabled(%1) _LNScene_SetSSAOEnabled %1
#cmd _LNScene_IsSSAOEnabled $3D0
#define LNScene_IsSSAOEnabled(%1=_ln_return_discard) _LNScene_IsSSAOEnabled %1
#cmd _LNScene_SetBloomEnabled $3D1
#define LNScene_SetBloomEnabled(%1) _LNScene_SetBloomEnabled %1
#cmd _LNScene_IsBloomEnabled $3D3
#define LNScene_IsBloomEnabled(%1=_ln_return_discard) _LNScene_IsBloomEnabled %1
#cmd _LNScene_SetDOFEnabled $3D4
#define LNScene_SetDOFEnabled(%1) _LNScene_SetDOFEnabled %1
#cmd _LNScene_IsDOFEnabled $3D6
#define LNScene_IsDOFEnabled(%1=_ln_return_discard) _LNScene_IsDOFEnabled %1
#cmd _LNScene_SetTonemapEnabled $3D7
#define LNScene_SetTonemapEnabled(%1) _LNScene_SetTonemapEnabled %1
#cmd _LNScene_IsTonemapEnabled $3D9
#define LNScene_IsTonemapEnabled(%1=_ln_return_discard) _LNScene_IsTonemapEnabled %1
#cmd _LNScene_SetVignetteEnabled $3DA
#define LNScene_SetVignetteEnabled(%1) _LNScene_SetVignetteEnabled %1
#cmd _LNScene_IsVignetteEnabled $3DC
#define LNScene_IsVignetteEnabled(%1=_ln_return_discard) _LNScene_IsVignetteEnabled %1
#cmd _LNScene_SetGammaEnabled $3DD
#define LNScene_SetGammaEnabled(%1) _LNScene_SetGammaEnabled %1
#cmd _LNScene_IsGammaEnabled $3DF
#define LNScene_IsGammaEnabled(%1=_ln_return_discard) _LNScene_IsGammaEnabled %1
#cmd _LNScene_SetTonemapExposure $3E0
#define LNScene_SetTonemapExposure(%1) _LNScene_SetTonemapExposure %1
#cmd _LNScene_SetTonemapLinearWhite $3E2
#define LNScene_SetTonemapLinearWhite(%1) _LNScene_SetTonemapLinearWhite %1
#cmd _LNScene_SetTonemapShoulderStrength $3E4
#define LNScene_SetTonemapShoulderStrength(%1) _LNScene_SetTonemapShoulderStrength %1
#cmd _LNScene_SetTonemapLinearStrength $3E6
#define LNScene_SetTonemapLinearStrength(%1) _LNScene_SetTonemapLinearStrength %1
#cmd _LNScene_SetTonemapLinearAngle $3E8
#define LNScene_SetTonemapLinearAngle(%1) _LNScene_SetTonemapLinearAngle %1
#cmd _LNScene_SetTonemapToeStrength $3EA
#define LNScene_SetTonemapToeStrength(%1) _LNScene_SetTonemapToeStrength %1
#cmd _LNScene_SetTonemapToeNumerator $3EC
#define LNScene_SetTonemapToeNumerator(%1) _LNScene_SetTonemapToeNumerator %1
#cmd _LNScene_SetTonemapToeDenominator $3EE
#define LNScene_SetTonemapToeDenominator(%1) _LNScene_SetTonemapToeDenominator %1
#cmd _LNLevel_Create $3FF
#define LNLevel_Create(%1=_ln_return_discard) _LNLevel_Create %1
#cmd _LNLevel_AddObject $3F1
#define LNLevel_AddObject(%1, %2) _LNLevel_AddObject %1, %2
#cmd _LNLevel_RemoveObject $3F3
#define LNLevel_RemoveObject(%1, %2) _LNLevel_RemoveObject %1, %2
#cmd _LNLevel_AddSubLevel $3F5
#define LNLevel_AddSubLevel(%1, %2) _LNLevel_AddSubLevel %1, %2
#cmd _LNLevel_RemoveSubLevel $3F7
#define LNLevel_RemoveSubLevel(%1, %2) _LNLevel_RemoveSubLevel %1, %2
#cmd _LNLevel_RemoveAllSubLevels $3F9
#define LNLevel_RemoveAllSubLevels(%1) _LNLevel_RemoveAllSubLevels %1
#cmd LNLevel_SetPrototype_OnSerialize $D8D
#cmd LNLevel_SetPrototype_OnStart $D98
#cmd LNLevel_SetPrototype_OnStop $DA3
#cmd LNLevel_SetPrototype_OnPause $DAE
#cmd LNLevel_SetPrototype_OnResume $DB9
#cmd LNLevel_SetPrototype_OnUpdate $DC4
#cmd _LNUIColors_Red $41F
#define LNUIColors_Red(%1=5, %2=_ln_return_discard) _LNUIColors_Red %1, %2
#cmd _LNUIColors_Pink $421
#define LNUIColors_Pink(%1=5, %2=_ln_return_discard) _LNUIColors_Pink %1, %2
#cmd _LNUIColors_Purple $423
#define LNUIColors_Purple(%1=5, %2=_ln_return_discard) _LNUIColors_Purple %1, %2
#cmd _LNUIColors_DeepPurple $425
#define LNUIColors_DeepPurple(%1=5, %2=_ln_return_discard) _LNUIColors_DeepPurple %1, %2
#cmd _LNUIColors_Indigo $427
#define LNUIColors_Indigo(%1=5, %2=_ln_return_discard) _LNUIColors_Indigo %1, %2
#cmd _LNUIColors_Blue $429
#define LNUIColors_Blue(%1=5, %2=_ln_return_discard) _LNUIColors_Blue %1, %2
#cmd _LNUIColors_LightBlue $42B
#define LNUIColors_LightBlue(%1=5, %2=_ln_return_discard) _LNUIColors_LightBlue %1, %2
#cmd _LNUIColors_Cyan $42D
#define LNUIColors_Cyan(%1=5, %2=_ln_return_discard) _LNUIColors_Cyan %1, %2
#cmd _LNUIColors_Teal $42F
#define LNUIColors_Teal(%1=5, %2=_ln_return_discard) _LNUIColors_Teal %1, %2
#cmd _LNUIColors_Green $431
#define LNUIColors_Green(%1=5, %2=_ln_return_discard) _LNUIColors_Green %1, %2
#cmd _LNUIColors_LightGreen $433
#define LNUIColors_LightGreen(%1=5, %2=_ln_return_discard) _LNUIColors_LightGreen %1, %2
#cmd _LNUIColors_Lime $435
#define LNUIColors_Lime(%1=5, %2=_ln_return_discard) _LNUIColors_Lime %1, %2
#cmd _LNUIColors_Yellow $437
#define LNUIColors_Yellow(%1=5, %2=_ln_return_discard) _LNUIColors_Yellow %1, %2
#cmd _LNUIColors_Amber $439
#define LNUIColors_Amber(%1=5, %2=_ln_return_discard) _LNUIColors_Amber %1, %2
#cmd _LNUIColors_Orange $43B
#define LNUIColors_Orange(%1=5, %2=_ln_return_discard) _LNUIColors_Orange %1, %2
#cmd _LNUIColors_DeepOrange $43D
#define LNUIColors_DeepOrange(%1=5, %2=_ln_return_discard) _LNUIColors_DeepOrange %1, %2
#cmd _LNUIColors_Brown $43F
#define LNUIColors_Brown(%1=5, %2=_ln_return_discard) _LNUIColors_Brown %1, %2
#cmd _LNUIColors_Grey $441
#define LNUIColors_Grey(%1=5, %2=_ln_return_discard) _LNUIColors_Grey %1, %2
#cmd _LNUIColors_Get $443
#define LNUIColors_Get(%1, %2=5, %3=_ln_return_discard) _LNUIColors_Get %1, %2, %3
#cmd _LNUIEventArgs_Sender $447
#define LNUIEventArgs_Sender(%1, %2=_ln_return_discard) _LNUIEventArgs_Sender %1, %2
#cmd LNUIEventArgs_SetPrototype_OnSerialize $DCF
#cmd _LNUIGeneralEventHandler_Create $92E
#define LNUIGeneralEventHandler_Create(%1, %2=_ln_return_discard) _LNUIGeneralEventHandler_Create %1, %2
#cmd _LNUIEventHandler_Create $933
#define LNUIEventHandler_Create(%1, %2=_ln_return_discard) _LNUIEventHandler_Create %1, %2
#cmd LNUILayoutElement_SetPrototype_OnSerialize $DDA
#cmd _LNUIElement_SetSize $459
#define LNUIElement_SetSize(%1, %2) _LNUIElement_SetSize %1, %2
#cmd _LNUIElement_SetSizeWH $45B
#define LNUIElement_SetSizeWH(%1, %2, %3) _LNUIElement_SetSizeWH %1, %2, %3
#cmd _LNUIElement_SetWidth $45E
#define LNUIElement_SetWidth(%1, %2) _LNUIElement_SetWidth %1, %2
#cmd _LNUIElement_GetWidth $460
#define LNUIElement_GetWidth(%1, %2=_ln_return_discard) _LNUIElement_GetWidth %1, %2
#cmd _LNUIElement_SetHeight $461
#define LNUIElement_SetHeight(%1, %2) _LNUIElement_SetHeight %1, %2
#cmd _LNUIElement_GetHeight $463
#define LNUIElement_GetHeight(%1, %2=_ln_return_discard) _LNUIElement_GetHeight %1, %2
#cmd _LNUIElement_SetMargin $464
#define LNUIElement_SetMargin(%1, %2) _LNUIElement_SetMargin %1, %2
#cmd _LNUIElement_GetMargin $466
#define LNUIElement_GetMargin(%1, %2=_ln_return_discard) _LNUIElement_GetMargin %1, %2
#cmd _LNUIElement_SetPadding $467
#define LNUIElement_SetPadding(%1, %2) _LNUIElement_SetPadding %1, %2
#cmd _LNUIElement_GetPadding $469
#define LNUIElement_GetPadding(%1, %2=_ln_return_discard) _LNUIElement_GetPadding %1, %2
#cmd _LNUIElement_SetHAlignment $46A
#define LNUIElement_SetHAlignment(%1, %2) _LNUIElement_SetHAlignment %1, %2
#cmd _LNUIElement_GetHAlignment $46C
#define LNUIElement_GetHAlignment(%1, %2=_ln_return_discard) _LNUIElement_GetHAlignment %1, %2
#cmd _LNUIElement_SetVAlignment $46D
#define LNUIElement_SetVAlignment(%1, %2) _LNUIElement_SetVAlignment %1, %2
#cmd _LNUIElement_GetVAlignment $46F
#define LNUIElement_GetVAlignment(%1, %2=_ln_return_discard) _LNUIElement_GetVAlignment %1, %2
#cmd _LNUIElement_SetAlignments $470
#define LNUIElement_SetAlignments(%1, %2, %3) _LNUIElement_SetAlignments %1, %2, %3
#cmd _LNUIElement_SetPosition $473
#define LNUIElement_SetPosition(%1, %2) _LNUIElement_SetPosition %1, %2
#cmd _LNUIElement_SetPositionXYZ $475
#define LNUIElement_SetPositionXYZ(%1, %2, %3, %4=0.000000) _LNUIElement_SetPositionXYZ %1, %2, %3, %4
#cmd _LNUIElement_GetPosition $479
#define LNUIElement_GetPosition(%1, %2=_ln_return_discard) _LNUIElement_GetPosition %1, %2
#cmd _LNUIElement_SetRotation $47A
#define LNUIElement_SetRotation(%1, %2) _LNUIElement_SetRotation %1, %2
#cmd _LNUIElement_SetEulerAngles $47C
#define LNUIElement_SetEulerAngles(%1, %2, %3, %4) _LNUIElement_SetEulerAngles %1, %2, %3, %4
#cmd _LNUIElement_GetRotation $480
#define LNUIElement_GetRotation(%1, %2=_ln_return_discard) _LNUIElement_GetRotation %1, %2
#cmd _LNUIElement_SetScale $481
#define LNUIElement_SetScale(%1, %2) _LNUIElement_SetScale %1, %2
#cmd _LNUIElement_SetScaleS $483
#define LNUIElement_SetScaleS(%1, %2) _LNUIElement_SetScaleS %1, %2
#cmd _LNUIElement_SetScaleXY $485
#define LNUIElement_SetScaleXY(%1, %2, %3) _LNUIElement_SetScaleXY %1, %2, %3
#cmd _LNUIElement_GetScale $488
#define LNUIElement_GetScale(%1, %2=_ln_return_discard) _LNUIElement_GetScale %1, %2
#cmd _LNUIElement_SetCenterPoint $489
#define LNUIElement_SetCenterPoint(%1, %2) _LNUIElement_SetCenterPoint %1, %2
#cmd _LNUIElement_SetCenterPointXYZ $48B
#define LNUIElement_SetCenterPointXYZ(%1, %2, %3, %4=0.000000) _LNUIElement_SetCenterPointXYZ %1, %2, %3, %4
#cmd _LNUIElement_GetCenterPoint $48F
#define LNUIElement_GetCenterPoint(%1, %2=_ln_return_discard) _LNUIElement_GetCenterPoint %1, %2
#cmd _LNUIElement_SetEnabled $490
#define LNUIElement_SetEnabled(%1, %2) _LNUIElement_SetEnabled %1, %2
#cmd _LNUIElement_IsEnabled $492
#define LNUIElement_IsEnabled(%1, %2=_ln_return_discard) _LNUIElement_IsEnabled %1, %2
#cmd _LNUIElement_SetData $493
#define LNUIElement_SetData(%1, %2) _LNUIElement_SetData %1, %2
#cmd _LNUIElement_GetData $495
#define LNUIElement_GetData(%1, %2=_ln_return_discard) _LNUIElement_GetData %1, %2
#cmd _LNUIElement_SetBackgroundColor $496
#define LNUIElement_SetBackgroundColor(%1, %2) _LNUIElement_SetBackgroundColor %1, %2
#cmd _LNUIElement_GetBackgroundColor $498
#define LNUIElement_GetBackgroundColor(%1, %2=_ln_return_discard) _LNUIElement_GetBackgroundColor %1, %2
#cmd _LNUIElement_SetBorderThickness $499
#define LNUIElement_SetBorderThickness(%1, %2) _LNUIElement_SetBorderThickness %1, %2
#cmd _LNUIElement_GetBorderThickness $49B
#define LNUIElement_GetBorderThickness(%1, %2=_ln_return_discard) _LNUIElement_GetBorderThickness %1, %2
#cmd _LNUIElement_SetBorderColor $49C
#define LNUIElement_SetBorderColor(%1, %2) _LNUIElement_SetBorderColor %1, %2
#cmd _LNUIElement_GetBorderColor $49E
#define LNUIElement_GetBorderColor(%1, %2=_ln_return_discard) _LNUIElement_GetBorderColor %1, %2
#cmd _LNUIElement_SetCornerRadius $49F
#define LNUIElement_SetCornerRadius(%1, %2) _LNUIElement_SetCornerRadius %1, %2
#cmd _LNUIElement_GetCornerRadius $4A1
#define LNUIElement_GetCornerRadius(%1, %2=_ln_return_discard) _LNUIElement_GetCornerRadius %1, %2
#cmd _LNUIElement_SetVisibility $4A2
#define LNUIElement_SetVisibility(%1, %2) _LNUIElement_SetVisibility %1, %2
#cmd _LNUIElement_GetVisibility $4A4
#define LNUIElement_GetVisibility(%1, %2=_ln_return_discard) _LNUIElement_GetVisibility %1, %2
#cmd _LNUIElement_SetOpacity $4A5
#define LNUIElement_SetOpacity(%1, %2) _LNUIElement_SetOpacity %1, %2
#cmd _LNUIElement_GetOpacity $4A7
#define LNUIElement_GetOpacity(%1, %2=_ln_return_discard) _LNUIElement_GetOpacity %1, %2
#cmd _LNUIElement_AddChild $4A8
#define LNUIElement_AddChild(%1, %2) _LNUIElement_AddChild %1, %2
#cmd _LNUIElement_SetFocusable $4AA
#define LNUIElement_SetFocusable(%1, %2) _LNUIElement_SetFocusable %1, %2
#cmd _LNUIElement_GetFocusable $4AC
#define LNUIElement_GetFocusable(%1, %2=_ln_return_discard) _LNUIElement_GetFocusable %1, %2
#cmd _LNUIElement_AddInto $4AD
#define LNUIElement_AddInto(%1, %2=LN_NULL_HANDLE) _LNUIElement_AddInto %1, %2
#cmd LNUIElement_SetPrototype_OnSerialize $DE5
#cmd _LNUIText_Create $4B3
#define LNUIText_Create(%1=_ln_return_discard) _LNUIText_Create %1
#cmd _LNUIText_CreateWithText $4B4
#define LNUIText_CreateWithText(%1, %2=_ln_return_discard) _LNUIText_CreateWithText %1, %2
#cmd _LNUIText_SetText $4B0
#define LNUIText_SetText(%1, %2) _LNUIText_SetText %1, %2
#cmd _LNUIText_GetText $4B2
#define LNUIText_GetText(%1, %2=_ln_return_discard) _LNUIText_GetText %1, %2
#cmd LNUIText_SetPrototype_OnSerialize $DF0
#cmd _LNUISprite_Create $4C3
#define LNUISprite_Create(%1=_ln_return_discard) _LNUISprite_Create %1
#cmd _LNUISprite_CreateWithTexture $4C4
#define LNUISprite_CreateWithTexture(%1, %2=_ln_return_discard) _LNUISprite_CreateWithTexture %1, %2
#cmd _LNUISprite_SetTexture $4B7
#define LNUISprite_SetTexture(%1, %2) _LNUISprite_SetTexture %1, %2
#cmd _LNUISprite_SetSourceRect $4B9
#define LNUISprite_SetSourceRect(%1, %2) _LNUISprite_SetSourceRect %1, %2
#cmd _LNUISprite_SetSourceRectXYWH $4BB
#define LNUISprite_SetSourceRectXYWH(%1, %2, %3, %4, %5) _LNUISprite_SetSourceRectXYWH %1, %2, %3, %4, %5
#cmd _LNUISprite_GetSourceRect $4C0
#define LNUISprite_GetSourceRect(%1, %2=_ln_return_discard) _LNUISprite_GetSourceRect %1, %2
#cmd _LNUISprite_SetShader $4C1
#define LNUISprite_SetShader(%1, %2) _LNUISprite_SetShader %1, %2
#cmd LNUISprite_SetPrototype_OnSerialize $DFB
#cmd _LNUIIcon_LoadFontIcon $4C7
#define LNUIIcon_LoadFontIcon(%1, %2=_ln_return_discard) _LNUIIcon_LoadFontIcon %1, %2
#cmd _LNUIIcon_LoadFontIconWithNameSize $4C9
#define LNUIIcon_LoadFontIconWithNameSize(%1, %2, %3=_ln_return_discard) _LNUIIcon_LoadFontIconWithNameSize %1, %2, %3
#cmd _LNUIIcon_LoadFontIconWithNameSizeColor $4CC
#define LNUIIcon_LoadFontIconWithNameSizeColor(%1, %2, %3, %4=_ln_return_discard) _LNUIIcon_LoadFontIconWithNameSizeColor %1, %2, %3, %4
#cmd LNUIIcon_SetPrototype_OnSerialize $E06
#cmd _LNUIMessageTextArea_Create $4D5
#define LNUIMessageTextArea_Create(%1=_ln_return_discard) _LNUIMessageTextArea_Create %1
#cmd _LNUIMessageTextArea_SetText $4D1
#define LNUIMessageTextArea_SetText(%1, %2) _LNUIMessageTextArea_SetText %1, %2
#cmd _LNUIMessageTextArea_SetTypingSpeed $4D3
#define LNUIMessageTextArea_SetTypingSpeed(%1, %2) _LNUIMessageTextArea_SetTypingSpeed %1, %2
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $E11
#cmd _LNUI_Add $4D7
#define LNUI_Add(%1) _LNUI_Add %1
#cmd _LNUI_Remove $4D9
#define LNUI_Remove(%1) _LNUI_Remove %1
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $E1C
#cmd _LNUIBoxLayout_Create $4E0
#define LNUIBoxLayout_Create(%1=_ln_return_discard) _LNUIBoxLayout_Create %1
#cmd _LNUIBoxLayout_SetOrientation $4DD
#define LNUIBoxLayout_SetOrientation(%1, %2) _LNUIBoxLayout_SetOrientation %1, %2
#cmd _LNUIBoxLayout_GetOrientation $4DF
#define LNUIBoxLayout_GetOrientation(%1, %2=_ln_return_discard) _LNUIBoxLayout_GetOrientation %1, %2
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $E27
#cmd _LNUIStackLayout_Create $4E5
#define LNUIStackLayout_Create(%1=_ln_return_discard) _LNUIStackLayout_Create %1
#cmd _LNUIStackLayout_SetOrientation $4E2
#define LNUIStackLayout_SetOrientation(%1, %2) _LNUIStackLayout_SetOrientation %1, %2
#cmd _LNUIStackLayout_GetOrientation $4E4
#define LNUIStackLayout_GetOrientation(%1, %2=_ln_return_discard) _LNUIStackLayout_GetOrientation %1, %2
#cmd LNUIStackLayout_SetPrototype_OnSerialize $E32
#cmd _LNUIGridLayout_Create $4F5
#define LNUIGridLayout_Create(%1=_ln_return_discard) _LNUIGridLayout_Create %1
#cmd _LNUIGridLayout_SetColumnCount $4E7
#define LNUIGridLayout_SetColumnCount(%1, %2) _LNUIGridLayout_SetColumnCount %1, %2
#cmd _LNUIGridLayout_SetRow $4E9
#define LNUIGridLayout_SetRow(%1, %2) _LNUIGridLayout_SetRow %1, %2
#cmd _LNUIGridLayout_SetColumn $4EC
#define LNUIGridLayout_SetColumn(%1, %2) _LNUIGridLayout_SetColumn %1, %2
#cmd _LNUIGridLayout_SetPlacement $4EF
#define LNUIGridLayout_SetPlacement(%1, %2, %3, %4=1, %5=1) _LNUIGridLayout_SetPlacement %1, %2, %3, %4, %5
#cmd LNUIGridLayout_SetPrototype_OnSerialize $E3D
#cmd _LNUIControl_Create $501
#define LNUIControl_Create(%1=_ln_return_discard) _LNUIControl_Create %1
#cmd _LNUIControl_AddInlineVisual $502
#define LNUIControl_AddInlineVisual(%1, %2, %3) _LNUIControl_AddInlineVisual %1, %2, %3
#cmd LNUIControl_SetPrototype_OnSerialize $E48
#cmd _LNUIButtonBase_SetText $506
#define LNUIButtonBase_SetText(%1, %2) _LNUIButtonBase_SetText %1, %2
#cmd LNUIButtonBase_SetPrototype_OnSerialize $E53
#cmd _LNUIButton_Create $509
#define LNUIButton_Create(%1=_ln_return_discard) _LNUIButton_Create %1
#cmd _LNUIButton_CreateWithText $50A
#define LNUIButton_CreateWithText(%1, %2=_ln_return_discard) _LNUIButton_CreateWithText %1, %2
#cmd _LNUIButton_ConnectOnClicked $50C
#define LNUIButton_ConnectOnClicked(%1, %2, %3=_ln_return_discard) _LNUIButton_ConnectOnClicked %1, %2, %3
#cmd LNUIButton_SetPrototype_OnSerialize $E5E
#cmd _LNUIWindow_Create $50F
#define LNUIWindow_Create(%1=_ln_return_discard) _LNUIWindow_Create %1
#cmd LNUIWindow_SetPrototype_OnSerialize $E69
#cmd _LNUIListItem_ConnectOnSubmit $514
#define LNUIListItem_ConnectOnSubmit(%1, %2, %3=_ln_return_discard) _LNUIListItem_ConnectOnSubmit %1, %2, %3
#cmd LNUIListItem_SetPrototype_OnSerialize $E74
#cmd _LNUIListItemsControl_SetItemsLayoutPanel $517
#define LNUIListItemsControl_SetItemsLayoutPanel(%1, %2) _LNUIListItemsControl_SetItemsLayoutPanel %1, %2
#cmd _LNUIListItemsControl_GetItemsLayoutPanel $519
#define LNUIListItemsControl_GetItemsLayoutPanel(%1, %2=_ln_return_discard) _LNUIListItemsControl_GetItemsLayoutPanel %1, %2
#cmd _LNUIListItemsControl_SetSubmitMode $51A
#define LNUIListItemsControl_SetSubmitMode(%1, %2) _LNUIListItemsControl_SetSubmitMode %1, %2
#cmd _LNUIListItemsControl_GetSubmitMode $51C
#define LNUIListItemsControl_GetSubmitMode(%1, %2=_ln_return_discard) _LNUIListItemsControl_GetSubmitMode %1, %2
#cmd _LNUIListItemsControl_ConnectOnSubmit $51D
#define LNUIListItemsControl_ConnectOnSubmit(%1, %2, %3=_ln_return_discard) _LNUIListItemsControl_ConnectOnSubmit %1, %2, %3
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $E7F
#cmd _LNUIListBoxItem_Create $520
#define LNUIListBoxItem_Create(%1, %2=_ln_return_discard) _LNUIListBoxItem_Create %1, %2
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $E8A
#cmd _LNUIListBox_Create $525
#define LNUIListBox_Create(%1=_ln_return_discard) _LNUIListBox_Create %1
#cmd _LNUIListBox_AddItem $523
#define LNUIListBox_AddItem(%1, %2, %3=_ln_return_discard) _LNUIListBox_AddItem %1, %2, %3
#cmd LNUIListBox_SetPrototype_OnSerialize $E95
#cmd LNInputGesture_SetPrototype_OnSerialize $EA0
#cmd _LNKeyGesture_Create $528
#define LNKeyGesture_Create(%1, %2=_ln_return_discard) _LNKeyGesture_Create %1, %2
#cmd LNKeyGesture_SetPrototype_OnSerialize $EAB
#cmd _LNInput_IsPressed $52B
#define LNInput_IsPressed(%1, %2=_ln_return_discard) _LNInput_IsPressed %1, %2
#cmd _LNInput_IsTriggered $52D
#define LNInput_IsTriggered(%1, %2=_ln_return_discard) _LNInput_IsTriggered %1, %2
#cmd _LNInput_IsTriggeredOff $52F
#define LNInput_IsTriggeredOff(%1, %2=_ln_return_discard) _LNInput_IsTriggeredOff %1, %2
#cmd _LNInput_IsRepeated $531
#define LNInput_IsRepeated(%1, %2=_ln_return_discard) _LNInput_IsRepeated %1, %2
#cmd _LNInput_GetAxisValue $533
#define LNInput_GetAxisValue(%1, %2=_ln_return_discard) _LNInput_GetAxisValue %1, %2
#cmd _LNInput_AddBinding $535
#define LNInput_AddBinding(%1, %2) _LNInput_AddBinding %1, %2
#cmd _LNInput_RemoveBinding $538
#define LNInput_RemoveBinding(%1) _LNInput_RemoveBinding %1
#cmd _LNInput_ClearBindings $53A
#define LNInput_ClearBindings(%1) _LNInput_ClearBindings %1
#cmd _LNInput_ClearAllBindings $53C
#define LNInput_ClearAllBindings _LNInput_ClearAllBindings
#cmd _LNMouse_Pressed $53E
#define LNMouse_Pressed(%1, %2=_ln_return_discard) _LNMouse_Pressed %1, %2
#cmd _LNMouse_Triggered $540
#define LNMouse_Triggered(%1, %2=_ln_return_discard) _LNMouse_Triggered %1, %2
#cmd _LNMouse_TriggeredOff $542
#define LNMouse_TriggeredOff(%1, %2=_ln_return_discard) _LNMouse_TriggeredOff %1, %2
#cmd _LNMouse_Repeated $544
#define LNMouse_Repeated(%1, %2=_ln_return_discard) _LNMouse_Repeated %1, %2
#cmd _LNMouse_GetPosition $546
#define LNMouse_GetPosition(%1=_ln_return_discard) _LNMouse_GetPosition %1
#cmd _LNInterpreterCommand_Code $548
#define LNInterpreterCommand_Code(%1, %2=_ln_return_discard) _LNInterpreterCommand_Code %1, %2
#cmd _LNInterpreterCommand_ParamsCount $549
#define LNInterpreterCommand_ParamsCount(%1, %2=_ln_return_discard) _LNInterpreterCommand_ParamsCount %1, %2
#cmd _LNInterpreterCommand_Param $54A
#define LNInterpreterCommand_Param(%1, %2, %3=_ln_return_discard) _LNInterpreterCommand_Param %1, %2, %3
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $EB6
#cmd _LNInterpreterCommandList_Create $561
#define LNInterpreterCommandList_Create(%1=_ln_return_discard) _LNInterpreterCommandList_Create %1
#cmd _LNInterpreterCommandList_AddCommand $54D
#define LNInterpreterCommandList_AddCommand(%1, %2) _LNInterpreterCommandList_AddCommand %1, %2
#cmd _LNInterpreterCommandList_AddCommand1 $54F
#define LNInterpreterCommandList_AddCommand1(%1, %2, %3) _LNInterpreterCommandList_AddCommand1 %1, %2, %3
#cmd _LNInterpreterCommandList_AddCommand2 $552
#define LNInterpreterCommandList_AddCommand2(%1, %2, %3, %4) _LNInterpreterCommandList_AddCommand2 %1, %2, %3, %4
#cmd _LNInterpreterCommandList_AddCommand3 $556
#define LNInterpreterCommandList_AddCommand3(%1, %2, %3, %4, %5) _LNInterpreterCommandList_AddCommand3 %1, %2, %3, %4, %5
#cmd _LNInterpreterCommandList_AddCommand4 $55B
#define LNInterpreterCommandList_AddCommand4(%1, %2, %3, %4, %5, %6) _LNInterpreterCommandList_AddCommand4 %1, %2, %3, %4, %5, %6
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $EC1
#cmd _LNInterpreterCommandDelegate_Create $A01
#define LNInterpreterCommandDelegate_Create(%1, %2=_ln_return_discard) _LNInterpreterCommandDelegate_Create %1, %2
#cmd _LNInterpreter_Create $575
#define LNInterpreter_Create(%1=_ln_return_discard) _LNInterpreter_Create %1
#cmd _LNInterpreter_Clear $566
#define LNInterpreter_Clear(%1) _LNInterpreter_Clear %1
#cmd _LNInterpreter_Run $567
#define LNInterpreter_Run(%1, %2) _LNInterpreter_Run %1, %2
#cmd _LNInterpreter_IsRunning $569
#define LNInterpreter_IsRunning(%1, %2=_ln_return_discard) _LNInterpreter_IsRunning %1, %2
#cmd _LNInterpreter_Update $56A
#define LNInterpreter_Update(%1) _LNInterpreter_Update %1
#cmd _LNInterpreter_Terminate $56B
#define LNInterpreter_Terminate(%1) _LNInterpreter_Terminate %1
#cmd _LNInterpreter_RegisterCommandHandler $56C
#define LNInterpreter_RegisterCommandHandler(%1, %2, %3) _LNInterpreter_RegisterCommandHandler %1, %2, %3
#cmd _LNInterpreter_SetWaitMode $56F
#define LNInterpreter_SetWaitMode(%1, %2) _LNInterpreter_SetWaitMode %1, %2
#cmd _LNInterpreter_GetWaitMode $571
#define LNInterpreter_GetWaitMode(%1, %2=_ln_return_discard) _LNInterpreter_GetWaitMode %1, %2
#cmd _LNInterpreter_SetWaitCount $572
#define LNInterpreter_SetWaitCount(%1, %2) _LNInterpreter_SetWaitCount %1, %2
#cmd _LNInterpreter_GetWaitCount $574
#define LNInterpreter_GetWaitCount(%1, %2=_ln_return_discard) _LNInterpreter_GetWaitCount %1, %2
#cmd LNInterpreter_SetPrototype_OnSerialize $ECC
#cmd LNInterpreter_SetPrototype_OnUpdateWait $ED8
#cmd _LNEngineSettings_SetMainWindowSize $578
#define LNEngineSettings_SetMainWindowSize(%1, %2) _LNEngineSettings_SetMainWindowSize %1, %2
#cmd _LNEngineSettings_SetMainWorldViewSize $57B
#define LNEngineSettings_SetMainWorldViewSize(%1, %2) _LNEngineSettings_SetMainWorldViewSize %1, %2
#cmd _LNEngineSettings_SetMainWindowTitle $57E
#define LNEngineSettings_SetMainWindowTitle(%1) _LNEngineSettings_SetMainWindowTitle %1
#cmd _LNEngineSettings_SetMainWindowResizable $580
#define LNEngineSettings_SetMainWindowResizable(%1) _LNEngineSettings_SetMainWindowResizable %1
#cmd _LNEngineSettings_AddAssetDirectory $582
#define LNEngineSettings_AddAssetDirectory(%1) _LNEngineSettings_AddAssetDirectory %1
#cmd _LNEngineSettings_AddAssetArchive $584
#define LNEngineSettings_AddAssetArchive(%1, %2) _LNEngineSettings_AddAssetArchive %1, %2
#cmd _LNEngineSettings_SetFrameRate $587
#define LNEngineSettings_SetFrameRate(%1) _LNEngineSettings_SetFrameRate %1
#cmd _LNEngineSettings_SetUITheme $589
#define LNEngineSettings_SetUITheme(%1) _LNEngineSettings_SetUITheme %1
#cmd _LNEngineSettings_SetFontFile $58B
#define LNEngineSettings_SetFontFile(%1) _LNEngineSettings_SetFontFile %1
#cmd _LNEngineSettings_SetDebugToolEnabled $58D
#define LNEngineSettings_SetDebugToolEnabled(%1) _LNEngineSettings_SetDebugToolEnabled %1
#cmd _LNEngineSettings_SetEngineLogEnabled $58F
#define LNEngineSettings_SetEngineLogEnabled(%1) _LNEngineSettings_SetEngineLogEnabled %1
#cmd _LNEngineSettings_SetEngineLogFilePath $591
#define LNEngineSettings_SetEngineLogFilePath(%1) _LNEngineSettings_SetEngineLogFilePath %1
#cmd _LNEngineSettings_SetDeveloperToolEnabled $593
#define LNEngineSettings_SetDeveloperToolEnabled(%1) _LNEngineSettings_SetDeveloperToolEnabled %1
#cmd _LNEngineSettings_SetUserMainWindow $595
#define LNEngineSettings_SetUserMainWindow(%1) _LNEngineSettings_SetUserMainWindow %1
#cmd _LNEngine_Initialize $598
#define LNEngine_Initialize _LNEngine_Initialize
#cmd _LNEngine_Terminate $599
#define LNEngine_Terminate _LNEngine_Terminate
#cmd _LNEngine_Update $59A
#define LNEngine_Update(%1=_ln_return_discard) _LNEngine_Update %1
#cmd _LNEngine_GetTime $59B
#define LNEngine_GetTime(%1=_ln_return_discard) _LNEngine_GetTime %1
#cmd _LNEngine_GetWorld $59C
#define LNEngine_GetWorld(%1=_ln_return_discard) _LNEngine_GetWorld %1
#cmd _LNEngine_GetCamera $59D
#define LNEngine_GetCamera(%1=_ln_return_discard) _LNEngine_GetCamera %1
#cmd _LNEngine_GetMainLight $59E
#define LNEngine_GetMainLight(%1=_ln_return_discard) _LNEngine_GetMainLight %1
#cmd _LNEngine_GetRenderView $59F
#define LNEngine_GetRenderView(%1=_ln_return_discard) _LNEngine_GetRenderView %1
#cmd _LNApplication_Create $5A5
#define LNApplication_Create(%1=_ln_return_discard) _LNApplication_Create %1
#cmd _LNApplication_OnInit $5A1
#define LNApplication_OnInit(%1) _LNApplication_OnInit %1
#cmd _LNApplication_OnUpdate $5A2
#define LNApplication_OnUpdate(%1) _LNApplication_OnUpdate %1
#cmd _LNApplication_World $5A3
#define LNApplication_World(%1, %2=_ln_return_discard) _LNApplication_World %1, %2
#cmd _LNApplication_Run $5A4
#define LNApplication_Run(%1) _LNApplication_Run %1
#cmd LNApplication_SetPrototype_OnSerialize $EE3
#cmd LNApplication_SetPrototype_OnInit $EEE
#cmd LNApplication_SetPrototype_OnUpdate $EF9
#cmd _LNDebug_SetGuideGridEnabled $5A7
#define LNDebug_SetGuideGridEnabled(%1) _LNDebug_SetGuideGridEnabled %1
#cmd _LNDebug_SetPhysicsDebugDrawEnabled $5A9
#define LNDebug_SetPhysicsDebugDrawEnabled(%1) _LNDebug_SetPhysicsDebugDrawEnabled %1
#cmd _LNDebug_Print $5AB
#define LNDebug_Print(%1) _LNDebug_Print %1
#cmd _LNDebug_PrintWithTime $5AD
#define LNDebug_PrintWithTime(%1, %2) _LNDebug_PrintWithTime %1, %2
#cmd _LNDebug_PrintWithTimeAndColor $5B0
#define LNDebug_PrintWithTimeAndColor(%1, %2, %3) _LNDebug_PrintWithTimeAndColor %1, %2, %3
#cmd _LNObjectSerializeHandler_Create $A31
#define LNObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNObjectSerializeHandler_Create %1, %2
#cmd _LNEventConnectionSerializeHandler_Create $A3C
#define LNEventConnectionSerializeHandler_Create(%1, %2=_ln_return_discard) _LNEventConnectionSerializeHandler_Create %1, %2
#cmd _LNVariantSerializeHandler_Create $A47
#define LNVariantSerializeHandler_Create(%1, %2=_ln_return_discard) _LNVariantSerializeHandler_Create %1, %2
#cmd _LNZVTestClass1SerializeHandler_Create $A52
#define LNZVTestClass1SerializeHandler_Create(%1, %2=_ln_return_discard) _LNZVTestClass1SerializeHandler_Create %1, %2
#cmd _LNZVTestEventArgs1SerializeHandler_Create $A5D
#define LNZVTestEventArgs1SerializeHandler_Create(%1, %2=_ln_return_discard) _LNZVTestEventArgs1SerializeHandler_Create %1, %2
#cmd _LNSerializer2SerializeHandler_Create $A68
#define LNSerializer2SerializeHandler_Create(%1, %2=_ln_return_discard) _LNSerializer2SerializeHandler_Create %1, %2
#cmd _LNAssetObjectSerializeHandler_Create $A73
#define LNAssetObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAssetObjectSerializeHandler_Create %1, %2
#cmd _LNAssetImportSettingsSerializeHandler_Create $A7E
#define LNAssetImportSettingsSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAssetImportSettingsSerializeHandler_Create %1, %2
#cmd _LNAssetModelSerializeHandler_Create $A89
#define LNAssetModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAssetModelSerializeHandler_Create %1, %2
#cmd _LNSoundSerializeHandler_Create $A94
#define LNSoundSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSoundSerializeHandler_Create %1, %2
#cmd _LNTextureSerializeHandler_Create $A9F
#define LNTextureSerializeHandler_Create(%1, %2=_ln_return_discard) _LNTextureSerializeHandler_Create %1, %2
#cmd _LNTexture2DSerializeHandler_Create $AAA
#define LNTexture2DSerializeHandler_Create(%1, %2=_ln_return_discard) _LNTexture2DSerializeHandler_Create %1, %2
#cmd _LNShaderSerializeHandler_Create $AB5
#define LNShaderSerializeHandler_Create(%1, %2=_ln_return_discard) _LNShaderSerializeHandler_Create %1, %2
#cmd _LNRenderViewSerializeHandler_Create $AC0
#define LNRenderViewSerializeHandler_Create(%1, %2=_ln_return_discard) _LNRenderViewSerializeHandler_Create %1, %2
#cmd _LNMaterialSerializeHandler_Create $ACB
#define LNMaterialSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMaterialSerializeHandler_Create %1, %2
#cmd _LNMeshNodeSerializeHandler_Create $AD6
#define LNMeshNodeSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshNodeSerializeHandler_Create %1, %2
#cmd _LNAnimationControllerSerializeHandler_Create $AE1
#define LNAnimationControllerSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationControllerSerializeHandler_Create %1, %2
#cmd _LNMeshModelSerializeHandler_Create $AEC
#define LNMeshModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshModelSerializeHandler_Create %1, %2
#cmd _LNMeshImportSettingsSerializeHandler_Create $AF7
#define LNMeshImportSettingsSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshImportSettingsSerializeHandler_Create %1, %2
#cmd _LNSkinnedMeshModelSerializeHandler_Create $B02
#define LNSkinnedMeshModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSkinnedMeshModelSerializeHandler_Create %1, %2
#cmd _LNCollisionShapeSerializeHandler_Create $B0D
#define LNCollisionShapeSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCollisionShapeSerializeHandler_Create %1, %2
#cmd _LNBoxCollisionShapeSerializeHandler_Create $B18
#define LNBoxCollisionShapeSerializeHandler_Create(%1, %2=_ln_return_discard) _LNBoxCollisionShapeSerializeHandler_Create %1, %2
#cmd _LNAnimationCurveSerializeHandler_Create $B23
#define LNAnimationCurveSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationCurveSerializeHandler_Create %1, %2
#cmd _LNKeyFrameAnimationCurveSerializeHandler_Create $B2E
#define LNKeyFrameAnimationCurveSerializeHandler_Create(%1, %2=_ln_return_discard) _LNKeyFrameAnimationCurveSerializeHandler_Create %1, %2
#cmd _LNAnimationClipSerializeHandler_Create $B39
#define LNAnimationClipSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationClipSerializeHandler_Create %1, %2
#cmd _LNAnimationStateSerializeHandler_Create $B44
#define LNAnimationStateSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationStateSerializeHandler_Create %1, %2
#cmd _LNEffectResourceSerializeHandler_Create $B4F
#define LNEffectResourceSerializeHandler_Create(%1, %2=_ln_return_discard) _LNEffectResourceSerializeHandler_Create %1, %2
#cmd _LNParticleEmitterModelSerializeHandler_Create $B5A
#define LNParticleEmitterModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterModelSerializeHandler_Create %1, %2
#cmd _LNParticleModelSerializeHandler_Create $B65
#define LNParticleModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNParticleModelSerializeHandler_Create %1, %2
#cmd _LNComponentSerializeHandler_Create $B70
#define LNComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNComponentSerializeHandler_Create %1, %2
#cmd _LNVisualComponentSerializeHandler_Create $B7B
#define LNVisualComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNVisualComponentSerializeHandler_Create %1, %2
#cmd _LNSpriteComponentSerializeHandler_Create $B86
#define LNSpriteComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSpriteComponentSerializeHandler_Create %1, %2
#cmd _LNCharacterControllerSerializeHandler_Create $B91
#define LNCharacterControllerSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCharacterControllerSerializeHandler_Create %1, %2
#cmd _LNWorldSerializeHandler_Create $B9C
#define LNWorldSerializeHandler_Create(%1, %2=_ln_return_discard) _LNWorldSerializeHandler_Create %1, %2
#cmd _LNComponentListSerializeHandler_Create $BA7
#define LNComponentListSerializeHandler_Create(%1, %2=_ln_return_discard) _LNComponentListSerializeHandler_Create %1, %2
#cmd _LNWorldObjectSerializeHandler_Create $BB2
#define LNWorldObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNWorldObjectSerializeHandler_Create %1, %2
#cmd _LNWorldObjectPreUpdateHandler_Create $BBD
#define LNWorldObjectPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNWorldObjectPreUpdateHandler_Create %1, %2
#cmd _LNWorldObjectUpdateHandler_Create $BC8
#define LNWorldObjectUpdateHandler_Create(%1, %2=_ln_return_discard) _LNWorldObjectUpdateHandler_Create %1, %2
#cmd _LNVisualObjectSerializeHandler_Create $BD3
#define LNVisualObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNVisualObjectSerializeHandler_Create %1, %2
#cmd _LNVisualObjectPreUpdateHandler_Create $BDE
#define LNVisualObjectPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNVisualObjectPreUpdateHandler_Create %1, %2
#cmd _LNVisualObjectUpdateHandler_Create $BE9
#define LNVisualObjectUpdateHandler_Create(%1, %2=_ln_return_discard) _LNVisualObjectUpdateHandler_Create %1, %2
#cmd _LNCameraSerializeHandler_Create $BF4
#define LNCameraSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCameraSerializeHandler_Create %1, %2
#cmd _LNCameraPreUpdateHandler_Create $BFF
#define LNCameraPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNCameraPreUpdateHandler_Create %1, %2
#cmd _LNCameraUpdateHandler_Create $C0A
#define LNCameraUpdateHandler_Create(%1, %2=_ln_return_discard) _LNCameraUpdateHandler_Create %1, %2
#cmd _LNEnvironmentLightSerializeHandler_Create $C15
#define LNEnvironmentLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNEnvironmentLightSerializeHandler_Create %1, %2
#cmd _LNEnvironmentLightPreUpdateHandler_Create $C20
#define LNEnvironmentLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNEnvironmentLightPreUpdateHandler_Create %1, %2
#cmd _LNEnvironmentLightUpdateHandler_Create $C2B
#define LNEnvironmentLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNEnvironmentLightUpdateHandler_Create %1, %2
#cmd _LNDirectionalLightSerializeHandler_Create $C36
#define LNDirectionalLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNDirectionalLightSerializeHandler_Create %1, %2
#cmd _LNDirectionalLightPreUpdateHandler_Create $C41
#define LNDirectionalLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNDirectionalLightPreUpdateHandler_Create %1, %2
#cmd _LNDirectionalLightUpdateHandler_Create $C4C
#define LNDirectionalLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNDirectionalLightUpdateHandler_Create %1, %2
#cmd _LNPointLightSerializeHandler_Create $C57
#define LNPointLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNPointLightSerializeHandler_Create %1, %2
#cmd _LNPointLightPreUpdateHandler_Create $C62
#define LNPointLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPointLightPreUpdateHandler_Create %1, %2
#cmd _LNPointLightUpdateHandler_Create $C6D
#define LNPointLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPointLightUpdateHandler_Create %1, %2
#cmd _LNSpotLightSerializeHandler_Create $C78
#define LNSpotLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSpotLightSerializeHandler_Create %1, %2
#cmd _LNSpotLightPreUpdateHandler_Create $C83
#define LNSpotLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpotLightPreUpdateHandler_Create %1, %2
#cmd _LNSpotLightUpdateHandler_Create $C8E
#define LNSpotLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpotLightUpdateHandler_Create %1, %2
#cmd _LNSpriteSerializeHandler_Create $C99
#define LNSpriteSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSpriteSerializeHandler_Create %1, %2
#cmd _LNSpritePreUpdateHandler_Create $CA4
#define LNSpritePreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpritePreUpdateHandler_Create %1, %2
#cmd _LNSpriteUpdateHandler_Create $CAF
#define LNSpriteUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpriteUpdateHandler_Create %1, %2
#cmd _LNCameraOrbitControlComponentSerializeHandler_Create $CBA
#define LNCameraOrbitControlComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCameraOrbitControlComponentSerializeHandler_Create %1, %2
#cmd _LNRaycasterSerializeHandler_Create $CC5
#define LNRaycasterSerializeHandler_Create(%1, %2=_ln_return_discard) _LNRaycasterSerializeHandler_Create %1, %2
#cmd _LNRaycastResultSerializeHandler_Create $CD0
#define LNRaycastResultSerializeHandler_Create(%1, %2=_ln_return_discard) _LNRaycastResultSerializeHandler_Create %1, %2
#cmd _LNWorldRenderViewSerializeHandler_Create $CDB
#define LNWorldRenderViewSerializeHandler_Create(%1, %2=_ln_return_discard) _LNWorldRenderViewSerializeHandler_Create %1, %2
#cmd _LNShapeObjectSerializeHandler_Create $CE6
#define LNShapeObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNShapeObjectSerializeHandler_Create %1, %2
#cmd _LNShapeObjectPreUpdateHandler_Create $CF1
#define LNShapeObjectPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNShapeObjectPreUpdateHandler_Create %1, %2
#cmd _LNShapeObjectUpdateHandler_Create $CFC
#define LNShapeObjectUpdateHandler_Create(%1, %2=_ln_return_discard) _LNShapeObjectUpdateHandler_Create %1, %2
#cmd _LNPlaneMeshSerializeHandler_Create $D07
#define LNPlaneMeshSerializeHandler_Create(%1, %2=_ln_return_discard) _LNPlaneMeshSerializeHandler_Create %1, %2
#cmd _LNPlaneMeshPreUpdateHandler_Create $D12
#define LNPlaneMeshPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPlaneMeshPreUpdateHandler_Create %1, %2
#cmd _LNPlaneMeshUpdateHandler_Create $D1D
#define LNPlaneMeshUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPlaneMeshUpdateHandler_Create %1, %2
#cmd _LNBoxMeshSerializeHandler_Create $D28
#define LNBoxMeshSerializeHandler_Create(%1, %2=_ln_return_discard) _LNBoxMeshSerializeHandler_Create %1, %2
#cmd _LNBoxMeshPreUpdateHandler_Create $D33
#define LNBoxMeshPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNBoxMeshPreUpdateHandler_Create %1, %2
#cmd _LNBoxMeshUpdateHandler_Create $D3E
#define LNBoxMeshUpdateHandler_Create(%1, %2=_ln_return_discard) _LNBoxMeshUpdateHandler_Create %1, %2
#cmd _LNMeshComponentSerializeHandler_Create $D49
#define LNMeshComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshComponentSerializeHandler_Create %1, %2
#cmd _LNCollisionSerializeHandler_Create $D54
#define LNCollisionSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCollisionSerializeHandler_Create %1, %2
#cmd _LNTriggerBodyComponentSerializeHandler_Create $D5F
#define LNTriggerBodyComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNTriggerBodyComponentSerializeHandler_Create %1, %2
#cmd _LNParticleEmitterSerializeHandler_Create $D6A
#define LNParticleEmitterSerializeHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterSerializeHandler_Create %1, %2
#cmd _LNParticleEmitterPreUpdateHandler_Create $D75
#define LNParticleEmitterPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterPreUpdateHandler_Create %1, %2
#cmd _LNParticleEmitterUpdateHandler_Create $D80
#define LNParticleEmitterUpdateHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterUpdateHandler_Create %1, %2
#cmd _LNLevelSerializeHandler_Create $D8B
#define LNLevelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNLevelSerializeHandler_Create %1, %2
#cmd _LNLevelStartHandler_Create $D96
#define LNLevelStartHandler_Create(%1, %2=_ln_return_discard) _LNLevelStartHandler_Create %1, %2
#cmd _LNLevelStopHandler_Create $DA1
#define LNLevelStopHandler_Create(%1, %2=_ln_return_discard) _LNLevelStopHandler_Create %1, %2
#cmd _LNLevelPauseHandler_Create $DAC
#define LNLevelPauseHandler_Create(%1, %2=_ln_return_discard) _LNLevelPauseHandler_Create %1, %2
#cmd _LNLevelResumeHandler_Create $DB7
#define LNLevelResumeHandler_Create(%1, %2=_ln_return_discard) _LNLevelResumeHandler_Create %1, %2
#cmd _LNLevelUpdateHandler_Create $DC2
#define LNLevelUpdateHandler_Create(%1, %2=_ln_return_discard) _LNLevelUpdateHandler_Create %1, %2
#cmd _LNUIEventArgsSerializeHandler_Create $DCD
#define LNUIEventArgsSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIEventArgsSerializeHandler_Create %1, %2
#cmd _LNUILayoutElementSerializeHandler_Create $DD8
#define LNUILayoutElementSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUILayoutElementSerializeHandler_Create %1, %2
#cmd _LNUIElementSerializeHandler_Create $DE3
#define LNUIElementSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIElementSerializeHandler_Create %1, %2
#cmd _LNUITextSerializeHandler_Create $DEE
#define LNUITextSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUITextSerializeHandler_Create %1, %2
#cmd _LNUISpriteSerializeHandler_Create $DF9
#define LNUISpriteSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUISpriteSerializeHandler_Create %1, %2
#cmd _LNUIIconSerializeHandler_Create $E04
#define LNUIIconSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIIconSerializeHandler_Create %1, %2
#cmd _LNUIMessageTextAreaSerializeHandler_Create $E0F
#define LNUIMessageTextAreaSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIMessageTextAreaSerializeHandler_Create %1, %2
#cmd _LNUILayoutPanelSerializeHandler_Create $E1A
#define LNUILayoutPanelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUILayoutPanelSerializeHandler_Create %1, %2
#cmd _LNUIBoxLayoutSerializeHandler_Create $E25
#define LNUIBoxLayoutSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIBoxLayoutSerializeHandler_Create %1, %2
#cmd _LNUIStackLayoutSerializeHandler_Create $E30
#define LNUIStackLayoutSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIStackLayoutSerializeHandler_Create %1, %2
#cmd _LNUIGridLayoutSerializeHandler_Create $E3B
#define LNUIGridLayoutSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIGridLayoutSerializeHandler_Create %1, %2
#cmd _LNUIControlSerializeHandler_Create $E46
#define LNUIControlSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIControlSerializeHandler_Create %1, %2
#cmd _LNUIButtonBaseSerializeHandler_Create $E51
#define LNUIButtonBaseSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIButtonBaseSerializeHandler_Create %1, %2
#cmd _LNUIButtonSerializeHandler_Create $E5C
#define LNUIButtonSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIButtonSerializeHandler_Create %1, %2
#cmd _LNUIWindowSerializeHandler_Create $E67
#define LNUIWindowSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIWindowSerializeHandler_Create %1, %2
#cmd _LNUIListItemSerializeHandler_Create $E72
#define LNUIListItemSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListItemSerializeHandler_Create %1, %2
#cmd _LNUIListItemsControlSerializeHandler_Create $E7D
#define LNUIListItemsControlSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListItemsControlSerializeHandler_Create %1, %2
#cmd _LNUIListBoxItemSerializeHandler_Create $E88
#define LNUIListBoxItemSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListBoxItemSerializeHandler_Create %1, %2
#cmd _LNUIListBoxSerializeHandler_Create $E93
#define LNUIListBoxSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListBoxSerializeHandler_Create %1, %2
#cmd _LNInputGestureSerializeHandler_Create $E9E
#define LNInputGestureSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInputGestureSerializeHandler_Create %1, %2
#cmd _LNKeyGestureSerializeHandler_Create $EA9
#define LNKeyGestureSerializeHandler_Create(%1, %2=_ln_return_discard) _LNKeyGestureSerializeHandler_Create %1, %2
#cmd _LNInterpreterCommandSerializeHandler_Create $EB4
#define LNInterpreterCommandSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterCommandSerializeHandler_Create %1, %2
#cmd _LNInterpreterCommandListSerializeHandler_Create $EBF
#define LNInterpreterCommandListSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterCommandListSerializeHandler_Create %1, %2
#cmd _LNInterpreterSerializeHandler_Create $ECA
#define LNInterpreterSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterSerializeHandler_Create %1, %2
#cmd _LNInterpreterUpdateWaitHandler_Create $ED6
#define LNInterpreterUpdateWaitHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterUpdateWaitHandler_Create %1, %2
#cmd _LNApplicationSerializeHandler_Create $EE1
#define LNApplicationSerializeHandler_Create(%1, %2=_ln_return_discard) _LNApplicationSerializeHandler_Create %1, %2
#cmd _LNApplicationInitHandler_Create $EEC
#define LNApplicationInitHandler_Create(%1, %2=_ln_return_discard) _LNApplicationInitHandler_Create %1, %2
#cmd _LNApplicationUpdateHandler_Create $EF7
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
