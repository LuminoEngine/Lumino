
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

#cmd LNVector3 $2C
#cmd LNVector3_GetX $563
#cmd LNVector3_SetX $565
#cmd LNVector3_GetY $566
#cmd LNVector3_SetY $568
#cmd LNVector3_GetZ $569
#cmd LNVector3_SetZ $56B
#cmd LNVector3_SetZeros $30
#cmd LNVector3_Set $31
#cmd LNVector3_Get $35
#cmd LNVector3_Length $39
#cmd LNVector3_LengthSquared $3A
#cmd LNVector3_MutatingNormalize $3B
#cmd LNVector3_NormalizeXYZ $3C
#cmd LNVector3_Normalize $40
#cmd LNVector4 $42
#cmd LNVector4_GetX $582
#cmd LNVector4_SetX $584
#cmd LNVector4_GetY $585
#cmd LNVector4_SetY $587
#cmd LNVector4_GetZ $588
#cmd LNVector4_SetZ $58A
#cmd LNVector4_GetW $58B
#cmd LNVector4_SetW $58D
#cmd LNVector4_SetZeros $47
#cmd LNVector4_Set $48
#cmd LNQuaternion $4D
#cmd LNQuaternion_GetX $5A0
#cmd LNQuaternion_SetX $5A2
#cmd LNQuaternion_GetY $5A3
#cmd LNQuaternion_SetY $5A5
#cmd LNQuaternion_GetZ $5A6
#cmd LNQuaternion_SetZ $5A8
#cmd LNQuaternion_GetW $5A9
#cmd LNQuaternion_SetW $5AB
#cmd LNQuaternion_SetZeros $52
#cmd LNQuaternion_Set $53
#cmd LNQuaternion_SetFromAxis $58
#cmd LNMatrix $5B
#cmd LNMatrix_GetRow0 $5BF
#cmd LNMatrix_SetRow0 $5C1
#cmd LNMatrix_GetRow1 $5C2
#cmd LNMatrix_SetRow1 $5C4
#cmd LNMatrix_GetRow2 $5C5
#cmd LNMatrix_SetRow2 $5C7
#cmd LNMatrix_GetRow3 $5C8
#cmd LNMatrix_SetRow3 $5CA
#cmd LNMatrix_SetZeros $60
#cmd LNMatrix_Set $61
#cmd LNColor $17A
#cmd LNColor_GetR $65E
#cmd LNColor_SetR $660
#cmd LNColor_GetG $661
#cmd LNColor_SetG $663
#cmd LNColor_GetB $664
#cmd LNColor_SetB $666
#cmd LNColor_GetA $667
#cmd LNColor_SetA $669
#cmd LNColor_SetZeros $17F
#cmd LNColor_Set $180
#cmd LNColorTone $185
#cmd LNColorTone_GetR $67D
#cmd LNColorTone_SetR $67F
#cmd LNColorTone_GetG $680
#cmd LNColorTone_SetG $682
#cmd LNColorTone_GetB $683
#cmd LNColorTone_SetB $685
#cmd LNColorTone_GetS $686
#cmd LNColorTone_SetS $688
#cmd LNColorTone_SetZeros $18A
#cmd LNColorTone_Set $18B
#cmd LNPoint $190
#cmd LNPoint_GetX $69B
#cmd LNPoint_SetX $69D
#cmd LNPoint_GetY $69E
#cmd LNPoint_SetY $6A0
#cmd LNPoint_SetZeros $193
#cmd LNPoint_Set $194
#cmd LNSize $197
#cmd LNSize_GetWidth $6AB
#cmd LNSize_SetWidth $6AD
#cmd LNSize_GetHeight $6AE
#cmd LNSize_SetHeight $6B0
#cmd LNSize_SetZeros $19A
#cmd LNSize_Set $19B
#cmd LNRect $19E
#cmd LNRect_GetX $6BB
#cmd LNRect_SetX $6BD
#cmd LNRect_GetY $6BE
#cmd LNRect_SetY $6C0
#cmd LNRect_GetWidth $6C1
#cmd LNRect_SetWidth $6C3
#cmd LNRect_GetHeight $6C4
#cmd LNRect_SetHeight $6C6
#cmd LNRect_SetZeros $1A3
#cmd LNRect_Set $1A4
#cmd LNRect_GetLeft $1A9
#cmd LNRect_SetSize $1AA
#cmd LNRect_GetSize $1AC
#cmd LNThickness $1AD
#cmd LNThickness_GetLeft $6DF
#cmd LNThickness_SetLeft $6E1
#cmd LNThickness_GetTop $6E2
#cmd LNThickness_SetTop $6E4
#cmd LNThickness_GetRight $6E5
#cmd LNThickness_SetRight $6E7
#cmd LNThickness_GetBottom $6E8
#cmd LNThickness_SetBottom $6EA
#cmd LNThickness_SetZeros $1B2
#cmd LNThickness_Set $1B3
#cmd LNCornerRadius $1B8
#cmd LNCornerRadius_GetTopleft $6FD
#cmd LNCornerRadius_SetTopleft $6FF
#cmd LNCornerRadius_GetTopright $700
#cmd LNCornerRadius_SetTopright $702
#cmd LNCornerRadius_GetBottomright $703
#cmd LNCornerRadius_SetBottomright $705
#cmd LNCornerRadius_GetBottomleft $706
#cmd LNCornerRadius_SetBottomleft $708
#cmd LNCornerRadius_SetZeros $1BD
#cmd LNCornerRadius_Set $1BE

#cmd _LNObject_Release $7B
#define LNObject_Release(%1) _LNObject_Release %1
#cmd _LNObject_Retain $7C
#define LNObject_Retain(%1) _LNObject_Retain %1
#cmd _LNObject_GetReferenceCount $7D
#define LNObject_GetReferenceCount(%1, %2=_ln_return_discard) _LNObject_GetReferenceCount %1, %2
#cmd LNObject_SetPrototype_OnSerialize $9B4
#cmd LNEventConnection_SetPrototype_OnSerialize $9BF
#cmd _LNPromiseFailureDelegate_Create $5E5
#define LNPromiseFailureDelegate_Create(%1, %2=_ln_return_discard) _LNPromiseFailureDelegate_Create %1, %2
#cmd _LNVariant_Create $82
#define LNVariant_Create(%1=_ln_return_discard) _LNVariant_Create %1
#cmd _LNVariant_SetInt $83
#define LNVariant_SetInt(%1, %2) _LNVariant_SetInt %1, %2
#cmd _LNVariant_GetInt $85
#define LNVariant_GetInt(%1, %2=_ln_return_discard) _LNVariant_GetInt %1, %2
#cmd LNVariant_SetPrototype_OnSerialize $9CA
#cmd _LNZVTestDelegate1_Create $5EE
#define LNZVTestDelegate1_Create(%1, %2=_ln_return_discard) _LNZVTestDelegate1_Create %1, %2
#cmd _LNZVTestDelegate2_Create $5F4
#define LNZVTestDelegate2_Create(%1, %2=_ln_return_discard) _LNZVTestDelegate2_Create %1, %2
#cmd _LNZVTestDelegate3_Create $5F9
#define LNZVTestDelegate3_Create(%1, %2=_ln_return_discard) _LNZVTestDelegate3_Create %1, %2
#cmd _LNZVTestEventHandler1_Create $5FE
#define LNZVTestEventHandler1_Create(%1, %2=_ln_return_discard) _LNZVTestEventHandler1_Create %1, %2
#cmd _LNZVTestEventHandler2_Create $603
#define LNZVTestEventHandler2_Create(%1, %2=_ln_return_discard) _LNZVTestEventHandler2_Create %1, %2
#cmd _LNZVTestPromise1_ThenWith $606
#define LNZVTestPromise1_ThenWith(%1, %2) _LNZVTestPromise1_ThenWith %1, %2
#cmd _LNZVTestPromise1_CatchWith $608
#define LNZVTestPromise1_CatchWith(%1, %2) _LNZVTestPromise1_CatchWith %1, %2
#cmd _LNZVTestPromise2_ThenWith $60C
#define LNZVTestPromise2_ThenWith(%1, %2) _LNZVTestPromise2_ThenWith %1, %2
#cmd _LNZVTestPromise2_CatchWith $60E
#define LNZVTestPromise2_CatchWith(%1, %2) _LNZVTestPromise2_CatchWith %1, %2
#cmd _LNZVTestClass1_Create $AE
#define LNZVTestClass1_Create(%1=_ln_return_discard) _LNZVTestClass1_Create %1
#cmd _LNZVTestClass1_SetTestDelegate1 $98
#define LNZVTestClass1_SetTestDelegate1(%1, %2) _LNZVTestClass1_SetTestDelegate1 %1, %2
#cmd _LNZVTestClass1_SetTestDelegate2 $9A
#define LNZVTestClass1_SetTestDelegate2(%1, %2) _LNZVTestClass1_SetTestDelegate2 %1, %2
#cmd _LNZVTestClass1_SetTestDelegate3 $9C
#define LNZVTestClass1_SetTestDelegate3(%1, %2) _LNZVTestClass1_SetTestDelegate3 %1, %2
#cmd _LNZVTestClass1_CallTestDelegate1 $9E
#define LNZVTestClass1_CallTestDelegate1(%1, %2) _LNZVTestClass1_CallTestDelegate1 %1, %2
#cmd _LNZVTestClass1_CallTestDelegate2 $A0
#define LNZVTestClass1_CallTestDelegate2(%1, %2, %3, %4=_ln_return_discard) _LNZVTestClass1_CallTestDelegate2 %1, %2, %3, %4
#cmd _LNZVTestClass1_CallTestDelegate3 $A3
#define LNZVTestClass1_CallTestDelegate3(%1) _LNZVTestClass1_CallTestDelegate3 %1
#cmd _LNZVTestClass1_LoadAsync $A4
#define LNZVTestClass1_LoadAsync(%1, %2=_ln_return_discard) _LNZVTestClass1_LoadAsync %1, %2
#cmd _LNZVTestClass1_ExecuteAsync $A6
#define LNZVTestClass1_ExecuteAsync(%1, %2=_ln_return_discard) _LNZVTestClass1_ExecuteAsync %1, %2
#cmd _LNZVTestClass1_GetFilePath $A7
#define LNZVTestClass1_GetFilePath(%1, %2=_ln_return_discard) _LNZVTestClass1_GetFilePath %1, %2
#cmd _LNZVTestClass1_ConnectOnEvent1 $A8
#define LNZVTestClass1_ConnectOnEvent1(%1, %2, %3=_ln_return_discard) _LNZVTestClass1_ConnectOnEvent1 %1, %2, %3
#cmd _LNZVTestClass1_RaiseEvent1 $AA
#define LNZVTestClass1_RaiseEvent1(%1) _LNZVTestClass1_RaiseEvent1 %1
#cmd _LNZVTestClass1_ConnectOnEvent2 $AB
#define LNZVTestClass1_ConnectOnEvent2(%1, %2, %3=_ln_return_discard) _LNZVTestClass1_ConnectOnEvent2 %1, %2, %3
#cmd _LNZVTestClass1_RaiseEvent2 $AD
#define LNZVTestClass1_RaiseEvent2(%1) _LNZVTestClass1_RaiseEvent2 %1
#cmd LNZVTestClass1_SetPrototype_OnSerialize $9D5
#cmd _LNZVTestEventArgs1_Create $B1
#define LNZVTestEventArgs1_Create(%1=_ln_return_discard) _LNZVTestEventArgs1_Create %1
#cmd _LNZVTestEventArgs1_CreateWithValue $B2
#define LNZVTestEventArgs1_CreateWithValue(%1, %2=_ln_return_discard) _LNZVTestEventArgs1_CreateWithValue %1, %2
#cmd _LNZVTestEventArgs1_GetValue $B0
#define LNZVTestEventArgs1_GetValue(%1, %2=_ln_return_discard) _LNZVTestEventArgs1_GetValue %1, %2
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $9E0
#cmd LNSerializer2_SetPrototype_OnSerialize $9EB
#cmd LNAssetObject_SetPrototype_OnSerialize $9F6
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $A01
#cmd _LNAssetModel_Create $114
#define LNAssetModel_Create(%1, %2=_ln_return_discard) _LNAssetModel_Create %1, %2
#cmd _LNAssetModel_Target $113
#define LNAssetModel_Target(%1, %2=_ln_return_discard) _LNAssetModel_Target %1, %2
#cmd LNAssetModel_SetPrototype_OnSerialize $A0C
#cmd _LNAssets_SaveAssetToLocalFile $117
#define LNAssets_SaveAssetToLocalFile(%1, %2) _LNAssets_SaveAssetToLocalFile %1, %2
#cmd _LNAssets_LoadAssetFromLocalFile $11A
#define LNAssets_LoadAssetFromLocalFile(%1, %2=_ln_return_discard) _LNAssets_LoadAssetFromLocalFile %1, %2
#cmd _LNAssets_LoadAsset $11C
#define LNAssets_LoadAsset(%1, %2=_ln_return_discard) _LNAssets_LoadAsset %1, %2
#cmd _LNAssets_ReloadAsset $11E
#define LNAssets_ReloadAsset(%1, %2) _LNAssets_ReloadAsset %1, %2
#cmd _LNAssets_ReadAllText $121
#define LNAssets_ReadAllText(%1, %2, %3=_ln_return_discard) _LNAssets_ReadAllText %1, %2, %3
#cmd _LNSound_SetVolume $12B
#define LNSound_SetVolume(%1, %2) _LNSound_SetVolume %1, %2
#cmd _LNSound_GetVolume $12D
#define LNSound_GetVolume(%1, %2=_ln_return_discard) _LNSound_GetVolume %1, %2
#cmd _LNSound_SetPitch $12E
#define LNSound_SetPitch(%1, %2) _LNSound_SetPitch %1, %2
#cmd _LNSound_GetPitch $130
#define LNSound_GetPitch(%1, %2=_ln_return_discard) _LNSound_GetPitch %1, %2
#cmd _LNSound_SetLoopEnabled $131
#define LNSound_SetLoopEnabled(%1, %2) _LNSound_SetLoopEnabled %1, %2
#cmd _LNSound_IsLoopEnabled $133
#define LNSound_IsLoopEnabled(%1, %2=_ln_return_discard) _LNSound_IsLoopEnabled %1, %2
#cmd _LNSound_SetLoopRange $134
#define LNSound_SetLoopRange(%1, %2, %3) _LNSound_SetLoopRange %1, %2, %3
#cmd _LNSound_Play $137
#define LNSound_Play(%1) _LNSound_Play %1
#cmd _LNSound_Stop $138
#define LNSound_Stop(%1) _LNSound_Stop %1
#cmd _LNSound_Pause $139
#define LNSound_Pause(%1) _LNSound_Pause %1
#cmd _LNSound_Resume $13A
#define LNSound_Resume(%1) _LNSound_Resume %1
#cmd _LNSound_FadeVolume $13B
#define LNSound_FadeVolume(%1, %2, %3, %4) _LNSound_FadeVolume %1, %2, %3, %4
#cmd LNSound_SetPrototype_OnSerialize $A17
#cmd _LNAudio_PlayBGM $140
#define LNAudio_PlayBGM(%1, %2, %3, %4) _LNAudio_PlayBGM %1, %2, %3, %4
#cmd _LNAudio_StopBGM $145
#define LNAudio_StopBGM(%1) _LNAudio_StopBGM %1
#cmd _LNAudio_PlayBGS $147
#define LNAudio_PlayBGS(%1, %2, %3, %4) _LNAudio_PlayBGS %1, %2, %3, %4
#cmd _LNAudio_StopBGS $14C
#define LNAudio_StopBGS(%1) _LNAudio_StopBGS %1
#cmd _LNAudio_PlayME $14E
#define LNAudio_PlayME(%1, %2, %3) _LNAudio_PlayME %1, %2, %3
#cmd _LNAudio_StopME $152
#define LNAudio_StopME _LNAudio_StopME
#cmd _LNAudio_PlaySE $153
#define LNAudio_PlaySE(%1, %2, %3) _LNAudio_PlaySE %1, %2, %3
#cmd _LNAudio_PlaySE3D $157
#define LNAudio_PlaySE3D(%1, %2, %3, %4, %5) _LNAudio_PlaySE3D %1, %2, %3, %4, %5
#cmd _LNAudio_StopSE $15D
#define LNAudio_StopSE _LNAudio_StopSE
#cmd _LNTexture2DDelegate_Create $654
#define LNTexture2DDelegate_Create(%1, %2=_ln_return_discard) _LNTexture2DDelegate_Create %1, %2
#cmd _LNTexture2DPromise_ThenWith $657
#define LNTexture2DPromise_ThenWith(%1, %2) _LNTexture2DPromise_ThenWith %1, %2
#cmd _LNTexture2DPromise_CatchWith $659
#define LNTexture2DPromise_CatchWith(%1, %2) _LNTexture2DPromise_CatchWith %1, %2
#cmd _LNGraphics_GetActiveGraphicsAPI $179
#define LNGraphics_GetActiveGraphicsAPI(%1=_ln_return_discard) _LNGraphics_GetActiveGraphicsAPI %1
#cmd LNTexture_SetPrototype_OnSerialize $A22
#cmd _LNTexture2D_Create $1C9
#define LNTexture2D_Create(%1, %2, %3=_ln_return_discard) _LNTexture2D_Create %1, %2, %3
#cmd _LNTexture2D_CreateWithFormat $1CC
#define LNTexture2D_CreateWithFormat(%1, %2, %3, %4=_ln_return_discard) _LNTexture2D_CreateWithFormat %1, %2, %3, %4
#cmd _LNTexture2D_Load $1C5
#define LNTexture2D_Load(%1, %2=_ln_return_discard) _LNTexture2D_Load %1, %2
#cmd _LNTexture2D_LoadEmoji $1C7
#define LNTexture2D_LoadEmoji(%1, %2=_ln_return_discard) _LNTexture2D_LoadEmoji %1, %2
#cmd LNTexture2D_SetPrototype_OnSerialize $A2D
#cmd _LNShader_Load $1D1
#define LNShader_Load(%1, %2, %3=_ln_return_discard) _LNShader_Load %1, %2, %3
#cmd _LNShader_SetFloat $1D4
#define LNShader_SetFloat(%1, %2, %3) _LNShader_SetFloat %1, %2, %3
#cmd _LNShader_SetVector3 $1D7
#define LNShader_SetVector3(%1, %2, %3) _LNShader_SetVector3 %1, %2, %3
#cmd _LNShader_SetVector4 $1DA
#define LNShader_SetVector4(%1, %2, %3) _LNShader_SetVector4 %1, %2, %3
#cmd _LNShader_SetTexture $1DD
#define LNShader_SetTexture(%1, %2, %3) _LNShader_SetTexture %1, %2, %3
#cmd LNShader_SetPrototype_OnSerialize $A38
#cmd LNRenderView_SetPrototype_OnSerialize $A43
#cmd _LNMaterial_Create $202
#define LNMaterial_Create(%1=_ln_return_discard) _LNMaterial_Create %1
#cmd _LNMaterial_SetMainTexture $1F1
#define LNMaterial_SetMainTexture(%1, %2) _LNMaterial_SetMainTexture %1, %2
#cmd _LNMaterial_GetMainTexture $1F3
#define LNMaterial_GetMainTexture(%1, %2=_ln_return_discard) _LNMaterial_GetMainTexture %1, %2
#cmd _LNMaterial_SetColor $1F4
#define LNMaterial_SetColor(%1, %2) _LNMaterial_SetColor %1, %2
#cmd _LNMaterial_SetRoughness $1F6
#define LNMaterial_SetRoughness(%1, %2) _LNMaterial_SetRoughness %1, %2
#cmd _LNMaterial_SetMetallic $1F8
#define LNMaterial_SetMetallic(%1, %2) _LNMaterial_SetMetallic %1, %2
#cmd _LNMaterial_SetEmissive $1FA
#define LNMaterial_SetEmissive(%1, %2) _LNMaterial_SetEmissive %1, %2
#cmd _LNMaterial_SetShadingModel $1FC
#define LNMaterial_SetShadingModel(%1, %2) _LNMaterial_SetShadingModel %1, %2
#cmd _LNMaterial_GetShadingModel $1FE
#define LNMaterial_GetShadingModel(%1, %2=_ln_return_discard) _LNMaterial_GetShadingModel %1, %2
#cmd _LNMaterial_SetShader $1FF
#define LNMaterial_SetShader(%1, %2) _LNMaterial_SetShader %1, %2
#cmd _LNMaterial_GetShader $201
#define LNMaterial_GetShader(%1, %2=_ln_return_discard) _LNMaterial_GetShader %1, %2
#cmd LNMaterial_SetPrototype_OnSerialize $A4E
#cmd _LNMeshNode_SetVisible $204
#define LNMeshNode_SetVisible(%1, %2) _LNMeshNode_SetVisible %1, %2
#cmd _LNMeshNode_IsVisible $206
#define LNMeshNode_IsVisible(%1, %2=_ln_return_discard) _LNMeshNode_IsVisible %1, %2
#cmd LNMeshNode_SetPrototype_OnSerialize $A59
#cmd _LNAnimationController_AddClip $208
#define LNAnimationController_AddClip(%1, %2, %3=_ln_return_discard) _LNAnimationController_AddClip %1, %2, %3
#cmd _LNAnimationController_Play $20A
#define LNAnimationController_Play(%1, %2, %3) _LNAnimationController_Play %1, %2, %3
#cmd LNAnimationController_SetPrototype_OnSerialize $A64
#cmd _LNMeshModel_Load $20E
#define LNMeshModel_Load(%1, %2, %3=_ln_return_discard) _LNMeshModel_Load %1, %2, %3
#cmd _LNMeshModel_FindNode $211
#define LNMeshModel_FindNode(%1, %2, %3=_ln_return_discard) _LNMeshModel_FindNode %1, %2, %3
#cmd _LNMeshModel_FindMaterial $213
#define LNMeshModel_FindMaterial(%1, %2, %3=_ln_return_discard) _LNMeshModel_FindMaterial %1, %2, %3
#cmd _LNMeshModel_MaterialCount $215
#define LNMeshModel_MaterialCount(%1, %2=_ln_return_discard) _LNMeshModel_MaterialCount %1, %2
#cmd _LNMeshModel_Material $216
#define LNMeshModel_Material(%1, %2, %3=_ln_return_discard) _LNMeshModel_Material %1, %2, %3
#cmd _LNMeshModel_GetAnimationController $218
#define LNMeshModel_GetAnimationController(%1, %2=_ln_return_discard) _LNMeshModel_GetAnimationController %1, %2
#cmd LNMeshModel_SetPrototype_OnSerialize $A6F
#cmd _LNMeshImportSettings_Create $21A
#define LNMeshImportSettings_Create(%1=_ln_return_discard) _LNMeshImportSettings_Create %1
#cmd LNMeshImportSettings_SetPrototype_OnSerialize $A7A
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $A85
#cmd LNCollisionShape_SetPrototype_OnSerialize $A90
#cmd _LNBoxCollisionShape_Create $21E
#define LNBoxCollisionShape_Create(%1, %2=_ln_return_discard) _LNBoxCollisionShape_Create %1, %2
#cmd _LNBoxCollisionShape_CreateWHD $220
#define LNBoxCollisionShape_CreateWHD(%1, %2, %3, %4=_ln_return_discard) _LNBoxCollisionShape_CreateWHD %1, %2, %3, %4
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $A9B
#cmd _LNAnimationCurve_Evaluate $22E
#define LNAnimationCurve_Evaluate(%1, %2, %3=_ln_return_discard) _LNAnimationCurve_Evaluate %1, %2, %3
#cmd LNAnimationCurve_SetPrototype_OnSerialize $AA6
#cmd _LNKeyFrameAnimationCurve_Create $23B
#define LNKeyFrameAnimationCurve_Create(%1=_ln_return_discard) _LNKeyFrameAnimationCurve_Create %1
#cmd _LNKeyFrameAnimationCurve_AddKeyFrame $236
#define LNKeyFrameAnimationCurve_AddKeyFrame(%1, %2, %3, %4, %5) _LNKeyFrameAnimationCurve_AddKeyFrame %1, %2, %3, %4, %5
#cmd LNKeyFrameAnimationCurve_SetPrototype_OnSerialize $AB1
#cmd _LNAnimationClip_Load $23D
#define LNAnimationClip_Load(%1, %2=_ln_return_discard) _LNAnimationClip_Load %1, %2
#cmd _LNAnimationClip_SetWrapMode $23F
#define LNAnimationClip_SetWrapMode(%1, %2) _LNAnimationClip_SetWrapMode %1, %2
#cmd _LNAnimationClip_GetWrapMode $241
#define LNAnimationClip_GetWrapMode(%1, %2=_ln_return_discard) _LNAnimationClip_GetWrapMode %1, %2
#cmd _LNAnimationClip_SetHierarchicalAnimationMode $242
#define LNAnimationClip_SetHierarchicalAnimationMode(%1, %2) _LNAnimationClip_SetHierarchicalAnimationMode %1, %2
#cmd _LNAnimationClip_GetHierarchicalAnimationMode $244
#define LNAnimationClip_GetHierarchicalAnimationMode(%1, %2=_ln_return_discard) _LNAnimationClip_GetHierarchicalAnimationMode %1, %2
#cmd LNAnimationClip_SetPrototype_OnSerialize $ABC
#cmd LNAnimationState_SetPrototype_OnSerialize $AC7
#cmd LNEffectResource_SetPrototype_OnSerialize $AD2
#cmd _LNParticleEmitterModel_Create $269
#define LNParticleEmitterModel_Create(%1=_ln_return_discard) _LNParticleEmitterModel_Create %1
#cmd _LNParticleEmitterModel_SetMaxParticles $251
#define LNParticleEmitterModel_SetMaxParticles(%1, %2) _LNParticleEmitterModel_SetMaxParticles %1, %2
#cmd _LNParticleEmitterModel_SetSpawnRate $253
#define LNParticleEmitterModel_SetSpawnRate(%1, %2) _LNParticleEmitterModel_SetSpawnRate %1, %2
#cmd _LNParticleEmitterModel_SetLifeTime $255
#define LNParticleEmitterModel_SetLifeTime(%1, %2) _LNParticleEmitterModel_SetLifeTime %1, %2
#cmd _LNParticleEmitterModel_SetupBoxShape $257
#define LNParticleEmitterModel_SetupBoxShape(%1, %2) _LNParticleEmitterModel_SetupBoxShape %1, %2
#cmd _LNParticleEmitterModel_SetSize $259
#define LNParticleEmitterModel_SetSize(%1, %2) _LNParticleEmitterModel_SetSize %1, %2
#cmd _LNParticleEmitterModel_SetSizeVelocity $25B
#define LNParticleEmitterModel_SetSizeVelocity(%1, %2) _LNParticleEmitterModel_SetSizeVelocity %1, %2
#cmd _LNParticleEmitterModel_SetSizeAcceleration $25D
#define LNParticleEmitterModel_SetSizeAcceleration(%1, %2) _LNParticleEmitterModel_SetSizeAcceleration %1, %2
#cmd _LNParticleEmitterModel_SetForwardVelocityMin $25F
#define LNParticleEmitterModel_SetForwardVelocityMin(%1, %2) _LNParticleEmitterModel_SetForwardVelocityMin %1, %2
#cmd _LNParticleEmitterModel_SetForwardVelocityMax $261
#define LNParticleEmitterModel_SetForwardVelocityMax(%1, %2) _LNParticleEmitterModel_SetForwardVelocityMax %1, %2
#cmd _LNParticleEmitterModel_SetForwardScale $263
#define LNParticleEmitterModel_SetForwardScale(%1, %2) _LNParticleEmitterModel_SetForwardScale %1, %2
#cmd _LNParticleEmitterModel_SetGeometryDirection $265
#define LNParticleEmitterModel_SetGeometryDirection(%1, %2) _LNParticleEmitterModel_SetGeometryDirection %1, %2
#cmd _LNParticleEmitterModel_SetupSpriteModule $267
#define LNParticleEmitterModel_SetupSpriteModule(%1, %2) _LNParticleEmitterModel_SetupSpriteModule %1, %2
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $ADD
#cmd _LNParticleModel_Create $270
#define LNParticleModel_Create(%1=_ln_return_discard) _LNParticleModel_Create %1
#cmd _LNParticleModel_SetLoop $26B
#define LNParticleModel_SetLoop(%1, %2) _LNParticleModel_SetLoop %1, %2
#cmd _LNParticleModel_IsLoop $26D
#define LNParticleModel_IsLoop(%1, %2=_ln_return_discard) _LNParticleModel_IsLoop %1, %2
#cmd _LNParticleModel_AddEmitter $26E
#define LNParticleModel_AddEmitter(%1, %2) _LNParticleModel_AddEmitter %1, %2
#cmd LNParticleModel_SetPrototype_OnSerialize $AE8
#cmd LNComponent_SetPrototype_OnSerialize $AF3
#cmd _LNVisualComponent_SetVisible $273
#define LNVisualComponent_SetVisible(%1, %2) _LNVisualComponent_SetVisible %1, %2
#cmd _LNVisualComponent_IsVisible $275
#define LNVisualComponent_IsVisible(%1, %2=_ln_return_discard) _LNVisualComponent_IsVisible %1, %2
#cmd LNVisualComponent_SetPrototype_OnSerialize $AFE
#cmd _LNSpriteComponent_SetTexture $277
#define LNSpriteComponent_SetTexture(%1, %2) _LNSpriteComponent_SetTexture %1, %2
#cmd LNSpriteComponent_SetPrototype_OnSerialize $B09
#cmd _LNCollisionEventHandler_Create $78B
#define LNCollisionEventHandler_Create(%1, %2=_ln_return_discard) _LNCollisionEventHandler_Create %1, %2
#cmd _LNCharacterController_Create $297
#define LNCharacterController_Create(%1=_ln_return_discard) _LNCharacterController_Create %1
#cmd _LNCharacterController_SetWalkVelocity $281
#define LNCharacterController_SetWalkVelocity(%1, %2) _LNCharacterController_SetWalkVelocity %1, %2
#cmd _LNCharacterController_GetWalkVelocity $283
#define LNCharacterController_GetWalkVelocity(%1, %2=_ln_return_discard) _LNCharacterController_GetWalkVelocity %1, %2
#cmd _LNCharacterController_SetVelocity $284
#define LNCharacterController_SetVelocity(%1, %2) _LNCharacterController_SetVelocity %1, %2
#cmd _LNCharacterController_GetVelocity $286
#define LNCharacterController_GetVelocity(%1, %2=_ln_return_discard) _LNCharacterController_GetVelocity %1, %2
#cmd _LNCharacterController_SetInputControlEnabled $287
#define LNCharacterController_SetInputControlEnabled(%1, %2) _LNCharacterController_SetInputControlEnabled %1, %2
#cmd _LNCharacterController_SetCameraControlEnabled $289
#define LNCharacterController_SetCameraControlEnabled(%1, %2) _LNCharacterController_SetCameraControlEnabled %1, %2
#cmd _LNCharacterController_SetHeight $28B
#define LNCharacterController_SetHeight(%1, %2) _LNCharacterController_SetHeight %1, %2
#cmd _LNCharacterController_GetHeight $28D
#define LNCharacterController_GetHeight(%1, %2=_ln_return_discard) _LNCharacterController_GetHeight %1, %2
#cmd _LNCharacterController_SetCameraRadius $28E
#define LNCharacterController_SetCameraRadius(%1, %2) _LNCharacterController_SetCameraRadius %1, %2
#cmd _LNCharacterController_GetCameraRadius $290
#define LNCharacterController_GetCameraRadius(%1, %2=_ln_return_discard) _LNCharacterController_GetCameraRadius %1, %2
#cmd _LNCharacterController_SetCollisionEnter $291
#define LNCharacterController_SetCollisionEnter(%1, %2) _LNCharacterController_SetCollisionEnter %1, %2
#cmd _LNCharacterController_SetCollisionLeave $293
#define LNCharacterController_SetCollisionLeave(%1, %2) _LNCharacterController_SetCollisionLeave %1, %2
#cmd _LNCharacterController_SetCollisionStay $295
#define LNCharacterController_SetCollisionStay(%1, %2) _LNCharacterController_SetCollisionStay %1, %2
#cmd LNCharacterController_SetPrototype_OnSerialize $B14
#cmd _LNWorld_Add $299
#define LNWorld_Add(%1, %2) _LNWorld_Add %1, %2
#cmd LNWorld_SetPrototype_OnSerialize $B1F
#cmd _LNComponentList_GetLength $7AB
#define LNComponentList_GetLength(%1, %2=_ln_return_discard) _LNComponentList_GetLength %1, %2
#cmd _LNComponentList_GetItem $7AD
#define LNComponentList_GetItem(%1, %2, %3=_ln_return_discard) _LNComponentList_GetItem %1, %2, %3
#cmd LNComponentList_SetPrototype_OnSerialize $B2A
#cmd _LNWorldObject_Create $2D0
#define LNWorldObject_Create(%1=_ln_return_discard) _LNWorldObject_Create %1
#cmd _LNWorldObject_SetPosition $29D
#define LNWorldObject_SetPosition(%1, %2) _LNWorldObject_SetPosition %1, %2
#cmd _LNWorldObject_SetPositionXYZ $29F
#define LNWorldObject_SetPositionXYZ(%1, %2, %3, %4) _LNWorldObject_SetPositionXYZ %1, %2, %3, %4
#cmd _LNWorldObject_GetPosition $2A3
#define LNWorldObject_GetPosition(%1, %2=_ln_return_discard) _LNWorldObject_GetPosition %1, %2
#cmd _LNWorldObject_SetRotationQuaternion $2A4
#define LNWorldObject_SetRotationQuaternion(%1, %2) _LNWorldObject_SetRotationQuaternion %1, %2
#cmd _LNWorldObject_SetRotation $2A6
#define LNWorldObject_SetRotation(%1, %2, %3, %4) _LNWorldObject_SetRotation %1, %2, %3, %4
#cmd _LNWorldObject_GetRotation $2AA
#define LNWorldObject_GetRotation(%1, %2=_ln_return_discard) _LNWorldObject_GetRotation %1, %2
#cmd _LNWorldObject_SetScale $2AB
#define LNWorldObject_SetScale(%1, %2) _LNWorldObject_SetScale %1, %2
#cmd _LNWorldObject_SetScaleS $2AD
#define LNWorldObject_SetScaleS(%1, %2) _LNWorldObject_SetScaleS %1, %2
#cmd _LNWorldObject_SetScaleXYZ $2AF
#define LNWorldObject_SetScaleXYZ(%1, %2, %3, %4) _LNWorldObject_SetScaleXYZ %1, %2, %3, %4
#cmd _LNWorldObject_GetScale $2B3
#define LNWorldObject_GetScale(%1, %2=_ln_return_discard) _LNWorldObject_GetScale %1, %2
#cmd _LNWorldObject_SetCenterPoint $2B4
#define LNWorldObject_SetCenterPoint(%1, %2) _LNWorldObject_SetCenterPoint %1, %2
#cmd _LNWorldObject_SetCenterPointXYZ $2B6
#define LNWorldObject_SetCenterPointXYZ(%1, %2, %3, %4) _LNWorldObject_SetCenterPointXYZ %1, %2, %3, %4
#cmd _LNWorldObject_GetCenterPoint $2BA
#define LNWorldObject_GetCenterPoint(%1, %2=_ln_return_discard) _LNWorldObject_GetCenterPoint %1, %2
#cmd _LNWorldObject_LookAt $2BB
#define LNWorldObject_LookAt(%1, %2) _LNWorldObject_LookAt %1, %2
#cmd _LNWorldObject_LookAtXYZ $2BD
#define LNWorldObject_LookAtXYZ(%1, %2, %3, %4) _LNWorldObject_LookAtXYZ %1, %2, %3, %4
#cmd _LNWorldObject_AddComponent $2C1
#define LNWorldObject_AddComponent(%1, %2) _LNWorldObject_AddComponent %1, %2
#cmd _LNWorldObject_RemoveComponent $2C3
#define LNWorldObject_RemoveComponent(%1, %2) _LNWorldObject_RemoveComponent %1, %2
#cmd _LNWorldObject_AddTag $2C5
#define LNWorldObject_AddTag(%1, %2) _LNWorldObject_AddTag %1, %2
#cmd _LNWorldObject_RemoveTag $2C7
#define LNWorldObject_RemoveTag(%1, %2) _LNWorldObject_RemoveTag %1, %2
#cmd _LNWorldObject_HasTag $2C9
#define LNWorldObject_HasTag(%1, %2, %3=_ln_return_discard) _LNWorldObject_HasTag %1, %2, %3
#cmd _LNWorldObject_Destroy $2CB
#define LNWorldObject_Destroy(%1) _LNWorldObject_Destroy %1
#cmd _LNWorldObject_GetComponents $2CC
#define LNWorldObject_GetComponents(%1, %2=_ln_return_discard) _LNWorldObject_GetComponents %1, %2
#cmd LNWorldObject_SetPrototype_OnSerialize $B35
#cmd LNWorldObject_SetPrototype_OnPreUpdate $B40
#cmd LNWorldObject_SetPrototype_OnUpdate $B4B
#cmd _LNVisualObject_SetVisible $2D2
#define LNVisualObject_SetVisible(%1, %2) _LNVisualObject_SetVisible %1, %2
#cmd _LNVisualObject_IsVisible $2D4
#define LNVisualObject_IsVisible(%1, %2=_ln_return_discard) _LNVisualObject_IsVisible %1, %2
#cmd _LNVisualObject_SetBlendMode2 $2D5
#define LNVisualObject_SetBlendMode2(%1, %2) _LNVisualObject_SetBlendMode2 %1, %2
#cmd _LNVisualObject_SetOpacity $2D7
#define LNVisualObject_SetOpacity(%1, %2) _LNVisualObject_SetOpacity %1, %2
#cmd _LNVisualObject_GetOpacity $2D9
#define LNVisualObject_GetOpacity(%1, %2=_ln_return_discard) _LNVisualObject_GetOpacity %1, %2
#cmd LNVisualObject_SetPrototype_OnSerialize $B56
#cmd LNVisualObject_SetPrototype_OnPreUpdate $B61
#cmd LNVisualObject_SetPrototype_OnUpdate $B6C
#cmd LNCamera_SetPrototype_OnSerialize $B77
#cmd LNCamera_SetPrototype_OnPreUpdate $B82
#cmd LNCamera_SetPrototype_OnUpdate $B8D
#cmd _LNEnvironmentLight_SetEnabled $2DC
#define LNEnvironmentLight_SetEnabled(%1, %2) _LNEnvironmentLight_SetEnabled %1, %2
#cmd _LNEnvironmentLight_IsEnabled $2DE
#define LNEnvironmentLight_IsEnabled(%1, %2=_ln_return_discard) _LNEnvironmentLight_IsEnabled %1, %2
#cmd _LNEnvironmentLight_SetColor $2DF
#define LNEnvironmentLight_SetColor(%1, %2) _LNEnvironmentLight_SetColor %1, %2
#cmd _LNEnvironmentLight_GetColor $2E1
#define LNEnvironmentLight_GetColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetColor %1, %2
#cmd _LNEnvironmentLight_SetAmbientColor $2E2
#define LNEnvironmentLight_SetAmbientColor(%1, %2) _LNEnvironmentLight_SetAmbientColor %1, %2
#cmd _LNEnvironmentLight_GetAmbientColor $2E4
#define LNEnvironmentLight_GetAmbientColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetAmbientColor %1, %2
#cmd _LNEnvironmentLight_GetSkyColor $2E5
#define LNEnvironmentLight_GetSkyColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetSkyColor %1, %2
#cmd _LNEnvironmentLight_SetSkyColor $2E6
#define LNEnvironmentLight_SetSkyColor(%1, %2) _LNEnvironmentLight_SetSkyColor %1, %2
#cmd _LNEnvironmentLight_GetGroundColor $2E8
#define LNEnvironmentLight_GetGroundColor(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetGroundColor %1, %2
#cmd _LNEnvironmentLight_SetGroundColor $2E9
#define LNEnvironmentLight_SetGroundColor(%1, %2) _LNEnvironmentLight_SetGroundColor %1, %2
#cmd _LNEnvironmentLight_SetIntensity $2EB
#define LNEnvironmentLight_SetIntensity(%1, %2) _LNEnvironmentLight_SetIntensity %1, %2
#cmd _LNEnvironmentLight_GetIntensity $2ED
#define LNEnvironmentLight_GetIntensity(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetIntensity %1, %2
#cmd _LNEnvironmentLight_SetShadowEffectiveDistance $2EE
#define LNEnvironmentLight_SetShadowEffectiveDistance(%1, %2) _LNEnvironmentLight_SetShadowEffectiveDistance %1, %2
#cmd _LNEnvironmentLight_GetShadowEffectiveDistance $2F0
#define LNEnvironmentLight_GetShadowEffectiveDistance(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetShadowEffectiveDistance %1, %2
#cmd _LNEnvironmentLight_SetShadowEffectiveDepth $2F1
#define LNEnvironmentLight_SetShadowEffectiveDepth(%1, %2) _LNEnvironmentLight_SetShadowEffectiveDepth %1, %2
#cmd _LNEnvironmentLight_GetShadowEffectiveDepth $2F3
#define LNEnvironmentLight_GetShadowEffectiveDepth(%1, %2=_ln_return_discard) _LNEnvironmentLight_GetShadowEffectiveDepth %1, %2
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $B98
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $BA3
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $BAE
#cmd _LNDirectionalLight_Create $304
#define LNDirectionalLight_Create(%1=_ln_return_discard) _LNDirectionalLight_Create %1
#cmd _LNDirectionalLight_CreateWithColor $305
#define LNDirectionalLight_CreateWithColor(%1, %2=_ln_return_discard) _LNDirectionalLight_CreateWithColor %1, %2
#cmd _LNDirectionalLight_SetEnabled $2F5
#define LNDirectionalLight_SetEnabled(%1, %2) _LNDirectionalLight_SetEnabled %1, %2
#cmd _LNDirectionalLight_IsEnabled $2F7
#define LNDirectionalLight_IsEnabled(%1, %2=_ln_return_discard) _LNDirectionalLight_IsEnabled %1, %2
#cmd _LNDirectionalLight_SetColor $2F8
#define LNDirectionalLight_SetColor(%1, %2) _LNDirectionalLight_SetColor %1, %2
#cmd _LNDirectionalLight_GetColor $2FA
#define LNDirectionalLight_GetColor(%1, %2=_ln_return_discard) _LNDirectionalLight_GetColor %1, %2
#cmd _LNDirectionalLight_SetIntensity $2FB
#define LNDirectionalLight_SetIntensity(%1, %2) _LNDirectionalLight_SetIntensity %1, %2
#cmd _LNDirectionalLight_GetIntensity $2FD
#define LNDirectionalLight_GetIntensity(%1, %2=_ln_return_discard) _LNDirectionalLight_GetIntensity %1, %2
#cmd _LNDirectionalLight_SetShadowEffectiveDistance $2FE
#define LNDirectionalLight_SetShadowEffectiveDistance(%1, %2) _LNDirectionalLight_SetShadowEffectiveDistance %1, %2
#cmd _LNDirectionalLight_GetShadowEffectiveDistance $300
#define LNDirectionalLight_GetShadowEffectiveDistance(%1, %2=_ln_return_discard) _LNDirectionalLight_GetShadowEffectiveDistance %1, %2
#cmd _LNDirectionalLight_SetShadowEffectiveDepth $301
#define LNDirectionalLight_SetShadowEffectiveDepth(%1, %2) _LNDirectionalLight_SetShadowEffectiveDepth %1, %2
#cmd _LNDirectionalLight_GetShadowEffectiveDepth $303
#define LNDirectionalLight_GetShadowEffectiveDepth(%1, %2=_ln_return_discard) _LNDirectionalLight_GetShadowEffectiveDepth %1, %2
#cmd LNDirectionalLight_SetPrototype_OnSerialize $BB9
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $BC4
#cmd LNDirectionalLight_SetPrototype_OnUpdate $BCF
#cmd _LNPointLight_Create $317
#define LNPointLight_Create(%1=_ln_return_discard) _LNPointLight_Create %1
#cmd _LNPointLight_CreateWithColorAndRange $318
#define LNPointLight_CreateWithColorAndRange(%1, %2, %3=_ln_return_discard) _LNPointLight_CreateWithColorAndRange %1, %2, %3
#cmd _LNPointLight_SetEnabled $308
#define LNPointLight_SetEnabled(%1, %2) _LNPointLight_SetEnabled %1, %2
#cmd _LNPointLight_IsEnabled $30A
#define LNPointLight_IsEnabled(%1, %2=_ln_return_discard) _LNPointLight_IsEnabled %1, %2
#cmd _LNPointLight_SetColor $30B
#define LNPointLight_SetColor(%1, %2) _LNPointLight_SetColor %1, %2
#cmd _LNPointLight_GetColor $30D
#define LNPointLight_GetColor(%1, %2=_ln_return_discard) _LNPointLight_GetColor %1, %2
#cmd _LNPointLight_SetIntensity $30E
#define LNPointLight_SetIntensity(%1, %2) _LNPointLight_SetIntensity %1, %2
#cmd _LNPointLight_GetIntensity $310
#define LNPointLight_GetIntensity(%1, %2=_ln_return_discard) _LNPointLight_GetIntensity %1, %2
#cmd _LNPointLight_SetRange $311
#define LNPointLight_SetRange(%1, %2) _LNPointLight_SetRange %1, %2
#cmd _LNPointLight_GetRange $313
#define LNPointLight_GetRange(%1, %2=_ln_return_discard) _LNPointLight_GetRange %1, %2
#cmd _LNPointLight_SetAttenuation $314
#define LNPointLight_SetAttenuation(%1, %2) _LNPointLight_SetAttenuation %1, %2
#cmd _LNPointLight_GetAttenuation $316
#define LNPointLight_GetAttenuation(%1, %2=_ln_return_discard) _LNPointLight_GetAttenuation %1, %2
#cmd LNPointLight_SetPrototype_OnSerialize $BDA
#cmd LNPointLight_SetPrototype_OnPreUpdate $BE5
#cmd LNPointLight_SetPrototype_OnUpdate $BF0
#cmd _LNSpotLight_Create $331
#define LNSpotLight_Create(%1=_ln_return_discard) _LNSpotLight_Create %1
#cmd _LNSpotLight_CreateWithColorAndRange $332
#define LNSpotLight_CreateWithColorAndRange(%1, %2, %3, %4=_ln_return_discard) _LNSpotLight_CreateWithColorAndRange %1, %2, %3, %4
#cmd _LNSpotLight_SetEnabled $31C
#define LNSpotLight_SetEnabled(%1, %2) _LNSpotLight_SetEnabled %1, %2
#cmd _LNSpotLight_IsEnabled $31E
#define LNSpotLight_IsEnabled(%1, %2=_ln_return_discard) _LNSpotLight_IsEnabled %1, %2
#cmd _LNSpotLight_SetColor $31F
#define LNSpotLight_SetColor(%1, %2) _LNSpotLight_SetColor %1, %2
#cmd _LNSpotLight_GetColor $321
#define LNSpotLight_GetColor(%1, %2=_ln_return_discard) _LNSpotLight_GetColor %1, %2
#cmd _LNSpotLight_SetIntensity $322
#define LNSpotLight_SetIntensity(%1, %2) _LNSpotLight_SetIntensity %1, %2
#cmd _LNSpotLight_GetIntensity $324
#define LNSpotLight_GetIntensity(%1, %2=_ln_return_discard) _LNSpotLight_GetIntensity %1, %2
#cmd _LNSpotLight_SetRange $325
#define LNSpotLight_SetRange(%1, %2) _LNSpotLight_SetRange %1, %2
#cmd _LNSpotLight_GetRange $327
#define LNSpotLight_GetRange(%1, %2=_ln_return_discard) _LNSpotLight_GetRange %1, %2
#cmd _LNSpotLight_SetAttenuation $328
#define LNSpotLight_SetAttenuation(%1, %2) _LNSpotLight_SetAttenuation %1, %2
#cmd _LNSpotLight_GetAttenuation $32A
#define LNSpotLight_GetAttenuation(%1, %2=_ln_return_discard) _LNSpotLight_GetAttenuation %1, %2
#cmd _LNSpotLight_SetAngle $32B
#define LNSpotLight_SetAngle(%1, %2) _LNSpotLight_SetAngle %1, %2
#cmd _LNSpotLight_GetAngle $32D
#define LNSpotLight_GetAngle(%1, %2=_ln_return_discard) _LNSpotLight_GetAngle %1, %2
#cmd _LNSpotLight_SetPenumbra $32E
#define LNSpotLight_SetPenumbra(%1, %2) _LNSpotLight_SetPenumbra %1, %2
#cmd _LNSpotLight_GetPenumbra $330
#define LNSpotLight_GetPenumbra(%1, %2=_ln_return_discard) _LNSpotLight_GetPenumbra %1, %2
#cmd LNSpotLight_SetPrototype_OnSerialize $BFB
#cmd LNSpotLight_SetPrototype_OnPreUpdate $C06
#cmd LNSpotLight_SetPrototype_OnUpdate $C11
#cmd _LNTestDelegate_Create $859
#define LNTestDelegate_Create(%1, %2=_ln_return_discard) _LNTestDelegate_Create %1, %2
#cmd _LNSprite_Create $348
#define LNSprite_Create(%1=_ln_return_discard) _LNSprite_Create %1
#cmd _LNSprite_CreateWithTexture $349
#define LNSprite_CreateWithTexture(%1, %2=_ln_return_discard) _LNSprite_CreateWithTexture %1, %2
#cmd _LNSprite_CreateWithTextureAndSize $34B
#define LNSprite_CreateWithTextureAndSize(%1, %2, %3, %4=_ln_return_discard) _LNSprite_CreateWithTextureAndSize %1, %2, %3, %4
#cmd _LNSprite_SetTexture $33A
#define LNSprite_SetTexture(%1, %2) _LNSprite_SetTexture %1, %2
#cmd _LNSprite_SetSize $33C
#define LNSprite_SetSize(%1, %2) _LNSprite_SetSize %1, %2
#cmd _LNSprite_SetSizeWH $33E
#define LNSprite_SetSizeWH(%1, %2, %3) _LNSprite_SetSizeWH %1, %2, %3
#cmd _LNSprite_SetSourceRectXYWH $341
#define LNSprite_SetSourceRectXYWH(%1, %2, %3, %4, %5) _LNSprite_SetSourceRectXYWH %1, %2, %3, %4, %5
#cmd _LNSprite_SetCallerTest $346
#define LNSprite_SetCallerTest(%1, %2) _LNSprite_SetCallerTest %1, %2
#cmd LNSprite_SetPrototype_OnSerialize $C1C
#cmd LNSprite_SetPrototype_OnPreUpdate $C27
#cmd LNSprite_SetPrototype_OnUpdate $C32
#cmd _LNCameraOrbitControlComponent_Create $350
#define LNCameraOrbitControlComponent_Create(%1=_ln_return_discard) _LNCameraOrbitControlComponent_Create %1
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $C3D
#cmd _LNRaycaster_FromScreen $352
#define LNRaycaster_FromScreen(%1, %2=_ln_return_discard) _LNRaycaster_FromScreen %1, %2
#cmd _LNRaycaster_IntersectPlane $354
#define LNRaycaster_IntersectPlane(%1, %2, %3, %4, %5=_ln_return_discard) _LNRaycaster_IntersectPlane %1, %2, %3, %4, %5
#cmd LNRaycaster_SetPrototype_OnSerialize $C48
#cmd _LNRaycastResult_GetPoint $359
#define LNRaycastResult_GetPoint(%1, %2=_ln_return_discard) _LNRaycastResult_GetPoint %1, %2
#cmd LNRaycastResult_SetPrototype_OnSerialize $C53
#cmd _LNWorldRenderView_SetGuideGridEnabled $35B
#define LNWorldRenderView_SetGuideGridEnabled(%1, %2) _LNWorldRenderView_SetGuideGridEnabled %1, %2
#cmd _LNWorldRenderView_GetGuideGridEnabled $35D
#define LNWorldRenderView_GetGuideGridEnabled(%1, %2=_ln_return_discard) _LNWorldRenderView_GetGuideGridEnabled %1, %2
#cmd LNWorldRenderView_SetPrototype_OnSerialize $C5E
#cmd _LNBoxMesh_Create $35F
#define LNBoxMesh_Create(%1=_ln_return_discard) _LNBoxMesh_Create %1
#cmd _LNBoxMesh_CreateXYZ $360
#define LNBoxMesh_CreateXYZ(%1, %2, %3, %4=_ln_return_discard) _LNBoxMesh_CreateXYZ %1, %2, %3, %4
#cmd LNBoxMesh_SetPrototype_OnSerialize $C69
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $C74
#cmd LNBoxMesh_SetPrototype_OnUpdate $C7F
#cmd _LNPlaneMesh_Create $365
#define LNPlaneMesh_Create(%1=_ln_return_discard) _LNPlaneMesh_Create %1
#cmd LNPlaneMesh_SetPrototype_OnSerialize $C8A
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $C95
#cmd LNPlaneMesh_SetPrototype_OnUpdate $CA0
#cmd _LNStaticMesh_Load $367
#define LNStaticMesh_Load(%1, %2=_ln_return_discard) _LNStaticMesh_Load %1, %2
#cmd _LNStaticMesh_GetModel $369
#define LNStaticMesh_GetModel(%1, %2=_ln_return_discard) _LNStaticMesh_GetModel %1, %2
#cmd _LNStaticMesh_MakeCollisionBody $36A
#define LNStaticMesh_MakeCollisionBody(%1, %2) _LNStaticMesh_MakeCollisionBody %1, %2
#cmd LNStaticMesh_SetPrototype_OnSerialize $CAB
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $CB6
#cmd LNStaticMesh_SetPrototype_OnUpdate $CC1
#cmd _LNStaticMeshComponent_Create $371
#define LNStaticMeshComponent_Create(%1=_ln_return_discard) _LNStaticMeshComponent_Create %1
#cmd _LNStaticMeshComponent_SetModel $36D
#define LNStaticMeshComponent_SetModel(%1, %2) _LNStaticMeshComponent_SetModel %1, %2
#cmd _LNStaticMeshComponent_MakeCollisionBody $36F
#define LNStaticMeshComponent_MakeCollisionBody(%1, %2) _LNStaticMeshComponent_MakeCollisionBody %1, %2
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $CCC
#cmd _LNSkinnedMeshComponent_Create $373
#define LNSkinnedMeshComponent_Create(%1=_ln_return_discard) _LNSkinnedMeshComponent_Create %1
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $CD7
#cmd _LNCollision_GetWorldObject $375
#define LNCollision_GetWorldObject(%1, %2=_ln_return_discard) _LNCollision_GetWorldObject %1, %2
#cmd LNCollision_SetPrototype_OnSerialize $CE2
#cmd _LNTriggerBodyComponent_Create $379
#define LNTriggerBodyComponent_Create(%1=_ln_return_discard) _LNTriggerBodyComponent_Create %1
#cmd _LNTriggerBodyComponent_AddCollisionShape $377
#define LNTriggerBodyComponent_AddCollisionShape(%1, %2) _LNTriggerBodyComponent_AddCollisionShape %1, %2
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $CED
#cmd _LNParticleEmitter_Create $37B
#define LNParticleEmitter_Create(%1, %2=_ln_return_discard) _LNParticleEmitter_Create %1, %2
#cmd LNParticleEmitter_SetPrototype_OnSerialize $CF8
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $D03
#cmd LNParticleEmitter_SetPrototype_OnUpdate $D0E
#cmd _LNScene_SetClearMode $37E
#define LNScene_SetClearMode(%1) _LNScene_SetClearMode %1
#cmd _LNScene_SetSkyColor $380
#define LNScene_SetSkyColor(%1) _LNScene_SetSkyColor %1
#cmd _LNScene_SetSkyHorizonColor $382
#define LNScene_SetSkyHorizonColor(%1) _LNScene_SetSkyHorizonColor %1
#cmd _LNScene_SetSkyCloudColor $384
#define LNScene_SetSkyCloudColor(%1) _LNScene_SetSkyCloudColor %1
#cmd _LNScene_SetSkyOverlayColor $386
#define LNScene_SetSkyOverlayColor(%1) _LNScene_SetSkyOverlayColor %1
#cmd _LNScene_GotoLevel $388
#define LNScene_GotoLevel(%1, %2) _LNScene_GotoLevel %1, %2
#cmd _LNScene_CallLevel $38B
#define LNScene_CallLevel(%1, %2) _LNScene_CallLevel %1, %2
#cmd _LNScene_ReturnLevel $38E
#define LNScene_ReturnLevel(%1) _LNScene_ReturnLevel %1
#cmd _LNScene_ActiveLevel $390
#define LNScene_ActiveLevel(%1=_ln_return_discard) _LNScene_ActiveLevel %1
#cmd _LNScene_IsTransitionEffectRunning $391
#define LNScene_IsTransitionEffectRunning(%1=_ln_return_discard) _LNScene_IsTransitionEffectRunning %1
#cmd _LNScene_SetTransitionEffectMode $392
#define LNScene_SetTransitionEffectMode(%1) _LNScene_SetTransitionEffectMode %1
#cmd _LNScene_TransitionEffectMode $394
#define LNScene_TransitionEffectMode(%1=_ln_return_discard) _LNScene_TransitionEffectMode %1
#cmd _LNScene_SetTransitionDuration $395
#define LNScene_SetTransitionDuration(%1) _LNScene_SetTransitionDuration %1
#cmd _LNScene_TransitionDuration $397
#define LNScene_TransitionDuration(%1=_ln_return_discard) _LNScene_TransitionDuration %1
#cmd _LNScene_SetTransitionEffectColor $398
#define LNScene_SetTransitionEffectColor(%1) _LNScene_SetTransitionEffectColor %1
#cmd _LNScene_TransitionEffectColor $39A
#define LNScene_TransitionEffectColor(%1=_ln_return_discard) _LNScene_TransitionEffectColor %1
#cmd _LNScene_SetTransitionEffectMaskTexture $39B
#define LNScene_SetTransitionEffectMaskTexture(%1) _LNScene_SetTransitionEffectMaskTexture %1
#cmd _LNScene_TransitionEffectMaskTexture $39D
#define LNScene_TransitionEffectMaskTexture(%1=_ln_return_discard) _LNScene_TransitionEffectMaskTexture %1
#cmd _LNScene_SetTransitionEffectVague $39E
#define LNScene_SetTransitionEffectVague(%1) _LNScene_SetTransitionEffectVague %1
#cmd _LNScene_TransitionEffectVague $3A0
#define LNScene_TransitionEffectVague(%1=_ln_return_discard) _LNScene_TransitionEffectVague %1
#cmd _LNScene_StartFadeOut $3A1
#define LNScene_StartFadeOut _LNScene_StartFadeOut
#cmd _LNScene_StartFadeIn $3A2
#define LNScene_StartFadeIn _LNScene_StartFadeIn
#cmd _LNScene_SetFogStartDistance $3A3
#define LNScene_SetFogStartDistance(%1) _LNScene_SetFogStartDistance %1
#cmd _LNScene_SetFogColor $3A5
#define LNScene_SetFogColor(%1) _LNScene_SetFogColor %1
#cmd _LNScene_SetFogDensity $3A7
#define LNScene_SetFogDensity(%1) _LNScene_SetFogDensity %1
#cmd _LNScene_SetFogHeightDensity $3A9
#define LNScene_SetFogHeightDensity(%1) _LNScene_SetFogHeightDensity %1
#cmd _LNScene_SetFogLowerHeight $3AB
#define LNScene_SetFogLowerHeight(%1) _LNScene_SetFogLowerHeight %1
#cmd _LNScene_SetFogUpperHeight $3AD
#define LNScene_SetFogUpperHeight(%1) _LNScene_SetFogUpperHeight %1
#cmd _LNScene_SetHDREnabled $3AF
#define LNScene_SetHDREnabled(%1) _LNScene_SetHDREnabled %1
#cmd _LNScene_IsHDREnabled $3B1
#define LNScene_IsHDREnabled(%1=_ln_return_discard) _LNScene_IsHDREnabled %1
#cmd _LNScene_SetScreenBlendColor $3B2
#define LNScene_SetScreenBlendColor(%1) _LNScene_SetScreenBlendColor %1
#cmd _LNScene_ScreenBlendColor $3B4
#define LNScene_ScreenBlendColor(%1=_ln_return_discard) _LNScene_ScreenBlendColor %1
#cmd _LNScene_SetColorTone $3B5
#define LNScene_SetColorTone(%1) _LNScene_SetColorTone %1
#cmd _LNScene_ColorTone $3B7
#define LNScene_ColorTone(%1=_ln_return_discard) _LNScene_ColorTone %1
#cmd _LNScene_SetAntialiasEnabled $3B8
#define LNScene_SetAntialiasEnabled(%1) _LNScene_SetAntialiasEnabled %1
#cmd _LNScene_IsAntialiasEnabled $3BA
#define LNScene_IsAntialiasEnabled(%1=_ln_return_discard) _LNScene_IsAntialiasEnabled %1
#cmd _LNScene_SetSSREnabled $3BB
#define LNScene_SetSSREnabled(%1) _LNScene_SetSSREnabled %1
#cmd _LNScene_IsSSREnabled $3BD
#define LNScene_IsSSREnabled(%1=_ln_return_discard) _LNScene_IsSSREnabled %1
#cmd _LNScene_SetSSAOEnabled $3BE
#define LNScene_SetSSAOEnabled(%1) _LNScene_SetSSAOEnabled %1
#cmd _LNScene_IsSSAOEnabled $3C0
#define LNScene_IsSSAOEnabled(%1=_ln_return_discard) _LNScene_IsSSAOEnabled %1
#cmd _LNScene_SetBloomEnabled $3C1
#define LNScene_SetBloomEnabled(%1) _LNScene_SetBloomEnabled %1
#cmd _LNScene_IsBloomEnabled $3C3
#define LNScene_IsBloomEnabled(%1=_ln_return_discard) _LNScene_IsBloomEnabled %1
#cmd _LNScene_SetDOFEnabled $3C4
#define LNScene_SetDOFEnabled(%1) _LNScene_SetDOFEnabled %1
#cmd _LNScene_IsDOFEnabled $3C6
#define LNScene_IsDOFEnabled(%1=_ln_return_discard) _LNScene_IsDOFEnabled %1
#cmd _LNScene_SetTonemapEnabled $3C7
#define LNScene_SetTonemapEnabled(%1) _LNScene_SetTonemapEnabled %1
#cmd _LNScene_IsTonemapEnabled $3C9
#define LNScene_IsTonemapEnabled(%1=_ln_return_discard) _LNScene_IsTonemapEnabled %1
#cmd _LNScene_SetVignetteEnabled $3CA
#define LNScene_SetVignetteEnabled(%1) _LNScene_SetVignetteEnabled %1
#cmd _LNScene_IsVignetteEnabled $3CC
#define LNScene_IsVignetteEnabled(%1=_ln_return_discard) _LNScene_IsVignetteEnabled %1
#cmd _LNScene_SetGammaEnabled $3CD
#define LNScene_SetGammaEnabled(%1) _LNScene_SetGammaEnabled %1
#cmd _LNScene_IsGammaEnabled $3CF
#define LNScene_IsGammaEnabled(%1=_ln_return_discard) _LNScene_IsGammaEnabled %1
#cmd _LNScene_SetTonemapExposure $3D0
#define LNScene_SetTonemapExposure(%1) _LNScene_SetTonemapExposure %1
#cmd _LNScene_SetTonemapLinearWhite $3D2
#define LNScene_SetTonemapLinearWhite(%1) _LNScene_SetTonemapLinearWhite %1
#cmd _LNScene_SetTonemapShoulderStrength $3D4
#define LNScene_SetTonemapShoulderStrength(%1) _LNScene_SetTonemapShoulderStrength %1
#cmd _LNScene_SetTonemapLinearStrength $3D6
#define LNScene_SetTonemapLinearStrength(%1) _LNScene_SetTonemapLinearStrength %1
#cmd _LNScene_SetTonemapLinearAngle $3D8
#define LNScene_SetTonemapLinearAngle(%1) _LNScene_SetTonemapLinearAngle %1
#cmd _LNScene_SetTonemapToeStrength $3DA
#define LNScene_SetTonemapToeStrength(%1) _LNScene_SetTonemapToeStrength %1
#cmd _LNScene_SetTonemapToeNumerator $3DC
#define LNScene_SetTonemapToeNumerator(%1) _LNScene_SetTonemapToeNumerator %1
#cmd _LNScene_SetTonemapToeDenominator $3DE
#define LNScene_SetTonemapToeDenominator(%1) _LNScene_SetTonemapToeDenominator %1
#cmd _LNLevel_Create $3EF
#define LNLevel_Create(%1=_ln_return_discard) _LNLevel_Create %1
#cmd _LNLevel_AddObject $3E1
#define LNLevel_AddObject(%1, %2) _LNLevel_AddObject %1, %2
#cmd _LNLevel_RemoveObject $3E3
#define LNLevel_RemoveObject(%1, %2) _LNLevel_RemoveObject %1, %2
#cmd _LNLevel_AddSubLevel $3E5
#define LNLevel_AddSubLevel(%1, %2) _LNLevel_AddSubLevel %1, %2
#cmd _LNLevel_RemoveSubLevel $3E7
#define LNLevel_RemoveSubLevel(%1, %2) _LNLevel_RemoveSubLevel %1, %2
#cmd _LNLevel_RemoveAllSubLevels $3E9
#define LNLevel_RemoveAllSubLevels(%1) _LNLevel_RemoveAllSubLevels %1
#cmd LNLevel_SetPrototype_OnSerialize $D19
#cmd LNLevel_SetPrototype_OnStart $D24
#cmd LNLevel_SetPrototype_OnStop $D2F
#cmd LNLevel_SetPrototype_OnPause $D3A
#cmd LNLevel_SetPrototype_OnResume $D45
#cmd LNLevel_SetPrototype_OnUpdate $D50
#cmd _LNUIEventArgs_Sender $3FA
#define LNUIEventArgs_Sender(%1, %2=_ln_return_discard) _LNUIEventArgs_Sender %1, %2
#cmd LNUIEventArgs_SetPrototype_OnSerialize $D5B
#cmd _LNUIGeneralEventHandler_Create $8B8
#define LNUIGeneralEventHandler_Create(%1, %2=_ln_return_discard) _LNUIGeneralEventHandler_Create %1, %2
#cmd _LNUIEventHandler_Create $8BD
#define LNUIEventHandler_Create(%1, %2=_ln_return_discard) _LNUIEventHandler_Create %1, %2
#cmd LNUILayoutElement_SetPrototype_OnSerialize $D66
#cmd _LNUIElement_SetSize $40C
#define LNUIElement_SetSize(%1, %2) _LNUIElement_SetSize %1, %2
#cmd _LNUIElement_SetSizeWH $40E
#define LNUIElement_SetSizeWH(%1, %2, %3) _LNUIElement_SetSizeWH %1, %2, %3
#cmd _LNUIElement_SetWidth $411
#define LNUIElement_SetWidth(%1, %2) _LNUIElement_SetWidth %1, %2
#cmd _LNUIElement_GetWidth $413
#define LNUIElement_GetWidth(%1, %2=_ln_return_discard) _LNUIElement_GetWidth %1, %2
#cmd _LNUIElement_SetHeight $414
#define LNUIElement_SetHeight(%1, %2) _LNUIElement_SetHeight %1, %2
#cmd _LNUIElement_GetHeight $416
#define LNUIElement_GetHeight(%1, %2=_ln_return_discard) _LNUIElement_GetHeight %1, %2
#cmd _LNUIElement_SetMargin $417
#define LNUIElement_SetMargin(%1, %2) _LNUIElement_SetMargin %1, %2
#cmd _LNUIElement_GetMargin $419
#define LNUIElement_GetMargin(%1, %2=_ln_return_discard) _LNUIElement_GetMargin %1, %2
#cmd _LNUIElement_SetPadding $41A
#define LNUIElement_SetPadding(%1, %2) _LNUIElement_SetPadding %1, %2
#cmd _LNUIElement_GetPadding $41C
#define LNUIElement_GetPadding(%1, %2=_ln_return_discard) _LNUIElement_GetPadding %1, %2
#cmd _LNUIElement_SetHAlignment $41D
#define LNUIElement_SetHAlignment(%1, %2) _LNUIElement_SetHAlignment %1, %2
#cmd _LNUIElement_GetHAlignment $41F
#define LNUIElement_GetHAlignment(%1, %2=_ln_return_discard) _LNUIElement_GetHAlignment %1, %2
#cmd _LNUIElement_SetVAlignment $420
#define LNUIElement_SetVAlignment(%1, %2) _LNUIElement_SetVAlignment %1, %2
#cmd _LNUIElement_GetVAlignment $422
#define LNUIElement_GetVAlignment(%1, %2=_ln_return_discard) _LNUIElement_GetVAlignment %1, %2
#cmd _LNUIElement_SetAlignments $423
#define LNUIElement_SetAlignments(%1, %2, %3) _LNUIElement_SetAlignments %1, %2, %3
#cmd _LNUIElement_SetPosition $426
#define LNUIElement_SetPosition(%1, %2) _LNUIElement_SetPosition %1, %2
#cmd _LNUIElement_SetPositionXYZ $428
#define LNUIElement_SetPositionXYZ(%1, %2, %3, %4) _LNUIElement_SetPositionXYZ %1, %2, %3, %4
#cmd _LNUIElement_GetPosition $42C
#define LNUIElement_GetPosition(%1, %2=_ln_return_discard) _LNUIElement_GetPosition %1, %2
#cmd _LNUIElement_SetRotation $42D
#define LNUIElement_SetRotation(%1, %2) _LNUIElement_SetRotation %1, %2
#cmd _LNUIElement_SetEulerAngles $42F
#define LNUIElement_SetEulerAngles(%1, %2, %3, %4) _LNUIElement_SetEulerAngles %1, %2, %3, %4
#cmd _LNUIElement_GetRotation $433
#define LNUIElement_GetRotation(%1, %2=_ln_return_discard) _LNUIElement_GetRotation %1, %2
#cmd _LNUIElement_SetScale $434
#define LNUIElement_SetScale(%1, %2) _LNUIElement_SetScale %1, %2
#cmd _LNUIElement_SetScaleS $436
#define LNUIElement_SetScaleS(%1, %2) _LNUIElement_SetScaleS %1, %2
#cmd _LNUIElement_SetScaleXY $438
#define LNUIElement_SetScaleXY(%1, %2, %3) _LNUIElement_SetScaleXY %1, %2, %3
#cmd _LNUIElement_GetScale $43B
#define LNUIElement_GetScale(%1, %2=_ln_return_discard) _LNUIElement_GetScale %1, %2
#cmd _LNUIElement_SetCenterPoint $43C
#define LNUIElement_SetCenterPoint(%1, %2) _LNUIElement_SetCenterPoint %1, %2
#cmd _LNUIElement_SetCenterPointXYZ $43E
#define LNUIElement_SetCenterPointXYZ(%1, %2, %3, %4) _LNUIElement_SetCenterPointXYZ %1, %2, %3, %4
#cmd _LNUIElement_GetCenterPoint $442
#define LNUIElement_GetCenterPoint(%1, %2=_ln_return_discard) _LNUIElement_GetCenterPoint %1, %2
#cmd _LNUIElement_SetEnabled $443
#define LNUIElement_SetEnabled(%1, %2) _LNUIElement_SetEnabled %1, %2
#cmd _LNUIElement_IsEnabled $445
#define LNUIElement_IsEnabled(%1, %2=_ln_return_discard) _LNUIElement_IsEnabled %1, %2
#cmd _LNUIElement_SetData $446
#define LNUIElement_SetData(%1, %2) _LNUIElement_SetData %1, %2
#cmd _LNUIElement_GetData $448
#define LNUIElement_GetData(%1, %2=_ln_return_discard) _LNUIElement_GetData %1, %2
#cmd _LNUIElement_SetBackgroundColor $449
#define LNUIElement_SetBackgroundColor(%1, %2) _LNUIElement_SetBackgroundColor %1, %2
#cmd _LNUIElement_GetBackgroundColor $44B
#define LNUIElement_GetBackgroundColor(%1, %2=_ln_return_discard) _LNUIElement_GetBackgroundColor %1, %2
#cmd _LNUIElement_SetBorderThickness $44C
#define LNUIElement_SetBorderThickness(%1, %2) _LNUIElement_SetBorderThickness %1, %2
#cmd _LNUIElement_GetBorderThickness $44E
#define LNUIElement_GetBorderThickness(%1, %2=_ln_return_discard) _LNUIElement_GetBorderThickness %1, %2
#cmd _LNUIElement_SetBorderColor $44F
#define LNUIElement_SetBorderColor(%1, %2) _LNUIElement_SetBorderColor %1, %2
#cmd _LNUIElement_GetBorderColor $451
#define LNUIElement_GetBorderColor(%1, %2=_ln_return_discard) _LNUIElement_GetBorderColor %1, %2
#cmd _LNUIElement_SetCornerRadius $452
#define LNUIElement_SetCornerRadius(%1, %2) _LNUIElement_SetCornerRadius %1, %2
#cmd _LNUIElement_GetCornerRadius $454
#define LNUIElement_GetCornerRadius(%1, %2=_ln_return_discard) _LNUIElement_GetCornerRadius %1, %2
#cmd _LNUIElement_SetVisibility $455
#define LNUIElement_SetVisibility(%1, %2) _LNUIElement_SetVisibility %1, %2
#cmd _LNUIElement_GetVisibility $457
#define LNUIElement_GetVisibility(%1, %2=_ln_return_discard) _LNUIElement_GetVisibility %1, %2
#cmd _LNUIElement_SetOpacity $458
#define LNUIElement_SetOpacity(%1, %2) _LNUIElement_SetOpacity %1, %2
#cmd _LNUIElement_GetOpacity $45A
#define LNUIElement_GetOpacity(%1, %2=_ln_return_discard) _LNUIElement_GetOpacity %1, %2
#cmd _LNUIElement_AddChild $45B
#define LNUIElement_AddChild(%1, %2) _LNUIElement_AddChild %1, %2
#cmd _LNUIElement_SetFocusable $45D
#define LNUIElement_SetFocusable(%1, %2) _LNUIElement_SetFocusable %1, %2
#cmd _LNUIElement_GetFocusable $45F
#define LNUIElement_GetFocusable(%1, %2=_ln_return_discard) _LNUIElement_GetFocusable %1, %2
#cmd LNUIElement_SetPrototype_OnSerialize $D71
#cmd _LNUITextBlock_Create $464
#define LNUITextBlock_Create(%1=_ln_return_discard) _LNUITextBlock_Create %1
#cmd _LNUITextBlock_CreateWithText $465
#define LNUITextBlock_CreateWithText(%1, %2=_ln_return_discard) _LNUITextBlock_CreateWithText %1, %2
#cmd _LNUITextBlock_SetText $461
#define LNUITextBlock_SetText(%1, %2) _LNUITextBlock_SetText %1, %2
#cmd _LNUITextBlock_GetText $463
#define LNUITextBlock_GetText(%1, %2=_ln_return_discard) _LNUITextBlock_GetText %1, %2
#cmd LNUITextBlock_SetPrototype_OnSerialize $D7C
#cmd _LNUISprite_Create $474
#define LNUISprite_Create(%1=_ln_return_discard) _LNUISprite_Create %1
#cmd _LNUISprite_CreateWithTexture $475
#define LNUISprite_CreateWithTexture(%1, %2=_ln_return_discard) _LNUISprite_CreateWithTexture %1, %2
#cmd _LNUISprite_SetTexture $468
#define LNUISprite_SetTexture(%1, %2) _LNUISprite_SetTexture %1, %2
#cmd _LNUISprite_SetSourceRect $46A
#define LNUISprite_SetSourceRect(%1, %2) _LNUISprite_SetSourceRect %1, %2
#cmd _LNUISprite_SetSourceRectXYWH $46C
#define LNUISprite_SetSourceRectXYWH(%1, %2, %3, %4, %5) _LNUISprite_SetSourceRectXYWH %1, %2, %3, %4, %5
#cmd _LNUISprite_GetSourceRect $471
#define LNUISprite_GetSourceRect(%1, %2=_ln_return_discard) _LNUISprite_GetSourceRect %1, %2
#cmd _LNUISprite_SetShader $472
#define LNUISprite_SetShader(%1, %2) _LNUISprite_SetShader %1, %2
#cmd LNUISprite_SetPrototype_OnSerialize $D87
#cmd _LNUIIcon_LoadFontIcon $478
#define LNUIIcon_LoadFontIcon(%1, %2=_ln_return_discard) _LNUIIcon_LoadFontIcon %1, %2
#cmd _LNUIIcon_LoadFontIconWithNameSize $47A
#define LNUIIcon_LoadFontIconWithNameSize(%1, %2, %3=_ln_return_discard) _LNUIIcon_LoadFontIconWithNameSize %1, %2, %3
#cmd _LNUIIcon_LoadFontIconWithNameSizeColor $47D
#define LNUIIcon_LoadFontIconWithNameSizeColor(%1, %2, %3, %4=_ln_return_discard) _LNUIIcon_LoadFontIconWithNameSizeColor %1, %2, %3, %4
#cmd LNUIIcon_SetPrototype_OnSerialize $D92
#cmd _LNUIMessageTextArea_Create $486
#define LNUIMessageTextArea_Create(%1=_ln_return_discard) _LNUIMessageTextArea_Create %1
#cmd _LNUIMessageTextArea_SetText $482
#define LNUIMessageTextArea_SetText(%1, %2) _LNUIMessageTextArea_SetText %1, %2
#cmd _LNUIMessageTextArea_SetTypingSpeed $484
#define LNUIMessageTextArea_SetTypingSpeed(%1, %2) _LNUIMessageTextArea_SetTypingSpeed %1, %2
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $D9D
#cmd _LNUI_Add $488
#define LNUI_Add(%1) _LNUI_Add %1
#cmd _LNUI_Remove $48A
#define LNUI_Remove(%1) _LNUI_Remove %1
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $DA8
#cmd _LNUIBoxLayout_Create $491
#define LNUIBoxLayout_Create(%1=_ln_return_discard) _LNUIBoxLayout_Create %1
#cmd _LNUIBoxLayout_SetOrientation $48E
#define LNUIBoxLayout_SetOrientation(%1, %2) _LNUIBoxLayout_SetOrientation %1, %2
#cmd _LNUIBoxLayout_GetOrientation $490
#define LNUIBoxLayout_GetOrientation(%1, %2=_ln_return_discard) _LNUIBoxLayout_GetOrientation %1, %2
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $DB3
#cmd _LNUIStackLayout_Create $496
#define LNUIStackLayout_Create(%1=_ln_return_discard) _LNUIStackLayout_Create %1
#cmd _LNUIStackLayout_SetOrientation $493
#define LNUIStackLayout_SetOrientation(%1, %2) _LNUIStackLayout_SetOrientation %1, %2
#cmd _LNUIStackLayout_GetOrientation $495
#define LNUIStackLayout_GetOrientation(%1, %2=_ln_return_discard) _LNUIStackLayout_GetOrientation %1, %2
#cmd LNUIStackLayout_SetPrototype_OnSerialize $DBE
#cmd _LNUIGridLayout_Create $4A6
#define LNUIGridLayout_Create(%1=_ln_return_discard) _LNUIGridLayout_Create %1
#cmd _LNUIGridLayout_SetColumnCount $498
#define LNUIGridLayout_SetColumnCount(%1, %2) _LNUIGridLayout_SetColumnCount %1, %2
#cmd _LNUIGridLayout_SetRow $49A
#define LNUIGridLayout_SetRow(%1, %2) _LNUIGridLayout_SetRow %1, %2
#cmd _LNUIGridLayout_SetColumn $49D
#define LNUIGridLayout_SetColumn(%1, %2) _LNUIGridLayout_SetColumn %1, %2
#cmd _LNUIGridLayout_SetPlacement $4A0
#define LNUIGridLayout_SetPlacement(%1, %2, %3, %4, %5) _LNUIGridLayout_SetPlacement %1, %2, %3, %4, %5
#cmd LNUIGridLayout_SetPrototype_OnSerialize $DC9
#cmd _LNUIControl_Create $4B2
#define LNUIControl_Create(%1=_ln_return_discard) _LNUIControl_Create %1
#cmd _LNUIControl_AddInlineVisual $4B3
#define LNUIControl_AddInlineVisual(%1, %2, %3) _LNUIControl_AddInlineVisual %1, %2, %3
#cmd LNUIControl_SetPrototype_OnSerialize $DD4
#cmd _LNUIButtonBase_SetText $4B7
#define LNUIButtonBase_SetText(%1, %2) _LNUIButtonBase_SetText %1, %2
#cmd LNUIButtonBase_SetPrototype_OnSerialize $DDF
#cmd _LNUIButton_Create $4BA
#define LNUIButton_Create(%1=_ln_return_discard) _LNUIButton_Create %1
#cmd _LNUIButton_CreateWithText $4BB
#define LNUIButton_CreateWithText(%1, %2=_ln_return_discard) _LNUIButton_CreateWithText %1, %2
#cmd _LNUIButton_ConnectOnClicked $4BD
#define LNUIButton_ConnectOnClicked(%1, %2, %3=_ln_return_discard) _LNUIButton_ConnectOnClicked %1, %2, %3
#cmd LNUIButton_SetPrototype_OnSerialize $DEA
#cmd _LNUIWindow_Create $4C0
#define LNUIWindow_Create(%1=_ln_return_discard) _LNUIWindow_Create %1
#cmd LNUIWindow_SetPrototype_OnSerialize $DF5
#cmd _LNUIListItem_ConnectOnSubmit $4C5
#define LNUIListItem_ConnectOnSubmit(%1, %2, %3=_ln_return_discard) _LNUIListItem_ConnectOnSubmit %1, %2, %3
#cmd LNUIListItem_SetPrototype_OnSerialize $E00
#cmd _LNUIListItemsControl_SetItemsLayoutPanel $4C8
#define LNUIListItemsControl_SetItemsLayoutPanel(%1, %2) _LNUIListItemsControl_SetItemsLayoutPanel %1, %2
#cmd _LNUIListItemsControl_SetSubmitMode $4CA
#define LNUIListItemsControl_SetSubmitMode(%1, %2) _LNUIListItemsControl_SetSubmitMode %1, %2
#cmd _LNUIListItemsControl_GetSubmitMode $4CC
#define LNUIListItemsControl_GetSubmitMode(%1, %2=_ln_return_discard) _LNUIListItemsControl_GetSubmitMode %1, %2
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $E0B
#cmd _LNUIListBoxItem_Create $4CE
#define LNUIListBoxItem_Create(%1, %2=_ln_return_discard) _LNUIListBoxItem_Create %1, %2
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $E16
#cmd _LNUIListBox_Create $4D3
#define LNUIListBox_Create(%1=_ln_return_discard) _LNUIListBox_Create %1
#cmd _LNUIListBox_AddItem $4D1
#define LNUIListBox_AddItem(%1, %2, %3=_ln_return_discard) _LNUIListBox_AddItem %1, %2, %3
#cmd LNUIListBox_SetPrototype_OnSerialize $E21
#cmd LNInputGesture_SetPrototype_OnSerialize $E2C
#cmd _LNKeyGesture_Create $4D6
#define LNKeyGesture_Create(%1, %2=_ln_return_discard) _LNKeyGesture_Create %1, %2
#cmd LNKeyGesture_SetPrototype_OnSerialize $E37
#cmd _LNInput_IsPressed $4D9
#define LNInput_IsPressed(%1, %2=_ln_return_discard) _LNInput_IsPressed %1, %2
#cmd _LNInput_IsTriggered $4DB
#define LNInput_IsTriggered(%1, %2=_ln_return_discard) _LNInput_IsTriggered %1, %2
#cmd _LNInput_IsTriggeredOff $4DD
#define LNInput_IsTriggeredOff(%1, %2=_ln_return_discard) _LNInput_IsTriggeredOff %1, %2
#cmd _LNInput_IsRepeated $4DF
#define LNInput_IsRepeated(%1, %2=_ln_return_discard) _LNInput_IsRepeated %1, %2
#cmd _LNInput_GetAxisValue $4E1
#define LNInput_GetAxisValue(%1, %2=_ln_return_discard) _LNInput_GetAxisValue %1, %2
#cmd _LNInput_AddBinding $4E3
#define LNInput_AddBinding(%1, %2) _LNInput_AddBinding %1, %2
#cmd _LNInput_RemoveBinding $4E6
#define LNInput_RemoveBinding(%1) _LNInput_RemoveBinding %1
#cmd _LNInput_ClearBindings $4E8
#define LNInput_ClearBindings(%1) _LNInput_ClearBindings %1
#cmd _LNInput_ClearAllBindings $4EA
#define LNInput_ClearAllBindings _LNInput_ClearAllBindings
#cmd _LNMouse_Pressed $4EC
#define LNMouse_Pressed(%1, %2=_ln_return_discard) _LNMouse_Pressed %1, %2
#cmd _LNMouse_Triggered $4EE
#define LNMouse_Triggered(%1, %2=_ln_return_discard) _LNMouse_Triggered %1, %2
#cmd _LNMouse_TriggeredOff $4F0
#define LNMouse_TriggeredOff(%1, %2=_ln_return_discard) _LNMouse_TriggeredOff %1, %2
#cmd _LNMouse_Repeated $4F2
#define LNMouse_Repeated(%1, %2=_ln_return_discard) _LNMouse_Repeated %1, %2
#cmd _LNMouse_Position $4F4
#define LNMouse_Position(%1=_ln_return_discard) _LNMouse_Position %1
#cmd _LNInterpreterCommand_Code $4F6
#define LNInterpreterCommand_Code(%1, %2=_ln_return_discard) _LNInterpreterCommand_Code %1, %2
#cmd _LNInterpreterCommand_ParamsCount $4F7
#define LNInterpreterCommand_ParamsCount(%1, %2=_ln_return_discard) _LNInterpreterCommand_ParamsCount %1, %2
#cmd _LNInterpreterCommand_Param $4F8
#define LNInterpreterCommand_Param(%1, %2, %3=_ln_return_discard) _LNInterpreterCommand_Param %1, %2, %3
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $E42
#cmd _LNInterpreterCommandList_Create $50F
#define LNInterpreterCommandList_Create(%1=_ln_return_discard) _LNInterpreterCommandList_Create %1
#cmd _LNInterpreterCommandList_AddCommand $4FB
#define LNInterpreterCommandList_AddCommand(%1, %2) _LNInterpreterCommandList_AddCommand %1, %2
#cmd _LNInterpreterCommandList_AddCommand1 $4FD
#define LNInterpreterCommandList_AddCommand1(%1, %2, %3) _LNInterpreterCommandList_AddCommand1 %1, %2, %3
#cmd _LNInterpreterCommandList_AddCommand2 $500
#define LNInterpreterCommandList_AddCommand2(%1, %2, %3, %4) _LNInterpreterCommandList_AddCommand2 %1, %2, %3, %4
#cmd _LNInterpreterCommandList_AddCommand3 $504
#define LNInterpreterCommandList_AddCommand3(%1, %2, %3, %4, %5) _LNInterpreterCommandList_AddCommand3 %1, %2, %3, %4, %5
#cmd _LNInterpreterCommandList_AddCommand4 $509
#define LNInterpreterCommandList_AddCommand4(%1, %2, %3, %4, %5, %6) _LNInterpreterCommandList_AddCommand4 %1, %2, %3, %4, %5, %6
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $E4D
#cmd _LNInterpreterCommandDelegate_Create $983
#define LNInterpreterCommandDelegate_Create(%1, %2=_ln_return_discard) _LNInterpreterCommandDelegate_Create %1, %2
#cmd _LNInterpreter_Create $523
#define LNInterpreter_Create(%1=_ln_return_discard) _LNInterpreter_Create %1
#cmd _LNInterpreter_Clear $514
#define LNInterpreter_Clear(%1) _LNInterpreter_Clear %1
#cmd _LNInterpreter_Run $515
#define LNInterpreter_Run(%1, %2) _LNInterpreter_Run %1, %2
#cmd _LNInterpreter_IsRunning $517
#define LNInterpreter_IsRunning(%1, %2=_ln_return_discard) _LNInterpreter_IsRunning %1, %2
#cmd _LNInterpreter_Update $518
#define LNInterpreter_Update(%1) _LNInterpreter_Update %1
#cmd _LNInterpreter_Terminate $519
#define LNInterpreter_Terminate(%1) _LNInterpreter_Terminate %1
#cmd _LNInterpreter_RegisterCommandHandler $51A
#define LNInterpreter_RegisterCommandHandler(%1, %2, %3) _LNInterpreter_RegisterCommandHandler %1, %2, %3
#cmd _LNInterpreter_SetWaitMode $51D
#define LNInterpreter_SetWaitMode(%1, %2) _LNInterpreter_SetWaitMode %1, %2
#cmd _LNInterpreter_GetWaitMode $51F
#define LNInterpreter_GetWaitMode(%1, %2=_ln_return_discard) _LNInterpreter_GetWaitMode %1, %2
#cmd _LNInterpreter_SetWaitCount $520
#define LNInterpreter_SetWaitCount(%1, %2) _LNInterpreter_SetWaitCount %1, %2
#cmd _LNInterpreter_GetWaitCount $522
#define LNInterpreter_GetWaitCount(%1, %2=_ln_return_discard) _LNInterpreter_GetWaitCount %1, %2
#cmd LNInterpreter_SetPrototype_OnSerialize $E58
#cmd LNInterpreter_SetPrototype_OnUpdateWait $E64
#cmd _LNEngineSettings_SetMainWindowSize $526
#define LNEngineSettings_SetMainWindowSize(%1, %2) _LNEngineSettings_SetMainWindowSize %1, %2
#cmd _LNEngineSettings_SetMainWorldViewSize $529
#define LNEngineSettings_SetMainWorldViewSize(%1, %2) _LNEngineSettings_SetMainWorldViewSize %1, %2
#cmd _LNEngineSettings_SetMainWindowTitle $52C
#define LNEngineSettings_SetMainWindowTitle(%1) _LNEngineSettings_SetMainWindowTitle %1
#cmd _LNEngineSettings_SetMainWindowResizable $52E
#define LNEngineSettings_SetMainWindowResizable(%1) _LNEngineSettings_SetMainWindowResizable %1
#cmd _LNEngineSettings_AddAssetDirectory $530
#define LNEngineSettings_AddAssetDirectory(%1) _LNEngineSettings_AddAssetDirectory %1
#cmd _LNEngineSettings_AddAssetArchive $532
#define LNEngineSettings_AddAssetArchive(%1, %2) _LNEngineSettings_AddAssetArchive %1, %2
#cmd _LNEngineSettings_SetFrameRate $535
#define LNEngineSettings_SetFrameRate(%1) _LNEngineSettings_SetFrameRate %1
#cmd _LNEngineSettings_SetUITheme $537
#define LNEngineSettings_SetUITheme(%1) _LNEngineSettings_SetUITheme %1
#cmd _LNEngineSettings_SetFontFile $539
#define LNEngineSettings_SetFontFile(%1) _LNEngineSettings_SetFontFile %1
#cmd _LNEngineSettings_SetDebugToolEnabled $53B
#define LNEngineSettings_SetDebugToolEnabled(%1) _LNEngineSettings_SetDebugToolEnabled %1
#cmd _LNEngineSettings_SetEngineLogEnabled $53D
#define LNEngineSettings_SetEngineLogEnabled(%1) _LNEngineSettings_SetEngineLogEnabled %1
#cmd _LNEngineSettings_SetEngineLogFilePath $53F
#define LNEngineSettings_SetEngineLogFilePath(%1) _LNEngineSettings_SetEngineLogFilePath %1
#cmd _LNEngineSettings_SetDeveloperToolEnabled $541
#define LNEngineSettings_SetDeveloperToolEnabled(%1) _LNEngineSettings_SetDeveloperToolEnabled %1
#cmd _LNEngineSettings_SetUserMainWindow $543
#define LNEngineSettings_SetUserMainWindow(%1) _LNEngineSettings_SetUserMainWindow %1
#cmd _LNEngine_Init $546
#define LNEngine_Init _LNEngine_Init
#cmd _LNEngine_Finalize $547
#define LNEngine_Finalize _LNEngine_Finalize
#cmd _LNEngine_Update $548
#define LNEngine_Update(%1=_ln_return_discard) _LNEngine_Update %1
#cmd _LNEngine_Run $549
#define LNEngine_Run(%1) _LNEngine_Run %1
#cmd _LNEngine_GetTime $54B
#define LNEngine_GetTime(%1=_ln_return_discard) _LNEngine_GetTime %1
#cmd _LNEngine_GetWorld $54C
#define LNEngine_GetWorld(%1=_ln_return_discard) _LNEngine_GetWorld %1
#cmd _LNEngine_GetCamera $54D
#define LNEngine_GetCamera(%1=_ln_return_discard) _LNEngine_GetCamera %1
#cmd _LNEngine_GetMainLight $54E
#define LNEngine_GetMainLight(%1=_ln_return_discard) _LNEngine_GetMainLight %1
#cmd _LNEngine_GetRenderView $54F
#define LNEngine_GetRenderView(%1=_ln_return_discard) _LNEngine_GetRenderView %1
#cmd _LNApplication_Create $554
#define LNApplication_Create(%1=_ln_return_discard) _LNApplication_Create %1
#cmd _LNApplication_OnInit $551
#define LNApplication_OnInit(%1) _LNApplication_OnInit %1
#cmd _LNApplication_OnUpdate $552
#define LNApplication_OnUpdate(%1) _LNApplication_OnUpdate %1
#cmd _LNApplication_World $553
#define LNApplication_World(%1, %2=_ln_return_discard) _LNApplication_World %1, %2
#cmd LNApplication_SetPrototype_OnSerialize $E6F
#cmd LNApplication_SetPrototype_OnInit $E7A
#cmd LNApplication_SetPrototype_OnUpdate $E85
#cmd _LNDebug_SetGuideGridEnabled $556
#define LNDebug_SetGuideGridEnabled(%1) _LNDebug_SetGuideGridEnabled %1
#cmd _LNDebug_SetPhysicsDebugDrawEnabled $558
#define LNDebug_SetPhysicsDebugDrawEnabled(%1) _LNDebug_SetPhysicsDebugDrawEnabled %1
#cmd _LNDebug_Print $55A
#define LNDebug_Print(%1) _LNDebug_Print %1
#cmd _LNDebug_PrintWithTime $55C
#define LNDebug_PrintWithTime(%1, %2) _LNDebug_PrintWithTime %1, %2
#cmd _LNDebug_PrintWithTimeAndColor $55F
#define LNDebug_PrintWithTimeAndColor(%1, %2, %3) _LNDebug_PrintWithTimeAndColor %1, %2, %3
#cmd _LNObjectSerializeHandler_Create $9B2
#define LNObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNObjectSerializeHandler_Create %1, %2
#cmd _LNEventConnectionSerializeHandler_Create $9BD
#define LNEventConnectionSerializeHandler_Create(%1, %2=_ln_return_discard) _LNEventConnectionSerializeHandler_Create %1, %2
#cmd _LNVariantSerializeHandler_Create $9C8
#define LNVariantSerializeHandler_Create(%1, %2=_ln_return_discard) _LNVariantSerializeHandler_Create %1, %2
#cmd _LNZVTestClass1SerializeHandler_Create $9D3
#define LNZVTestClass1SerializeHandler_Create(%1, %2=_ln_return_discard) _LNZVTestClass1SerializeHandler_Create %1, %2
#cmd _LNZVTestEventArgs1SerializeHandler_Create $9DE
#define LNZVTestEventArgs1SerializeHandler_Create(%1, %2=_ln_return_discard) _LNZVTestEventArgs1SerializeHandler_Create %1, %2
#cmd _LNSerializer2SerializeHandler_Create $9E9
#define LNSerializer2SerializeHandler_Create(%1, %2=_ln_return_discard) _LNSerializer2SerializeHandler_Create %1, %2
#cmd _LNAssetObjectSerializeHandler_Create $9F4
#define LNAssetObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAssetObjectSerializeHandler_Create %1, %2
#cmd _LNAssetImportSettingsSerializeHandler_Create $9FF
#define LNAssetImportSettingsSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAssetImportSettingsSerializeHandler_Create %1, %2
#cmd _LNAssetModelSerializeHandler_Create $A0A
#define LNAssetModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAssetModelSerializeHandler_Create %1, %2
#cmd _LNSoundSerializeHandler_Create $A15
#define LNSoundSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSoundSerializeHandler_Create %1, %2
#cmd _LNTextureSerializeHandler_Create $A20
#define LNTextureSerializeHandler_Create(%1, %2=_ln_return_discard) _LNTextureSerializeHandler_Create %1, %2
#cmd _LNTexture2DSerializeHandler_Create $A2B
#define LNTexture2DSerializeHandler_Create(%1, %2=_ln_return_discard) _LNTexture2DSerializeHandler_Create %1, %2
#cmd _LNShaderSerializeHandler_Create $A36
#define LNShaderSerializeHandler_Create(%1, %2=_ln_return_discard) _LNShaderSerializeHandler_Create %1, %2
#cmd _LNRenderViewSerializeHandler_Create $A41
#define LNRenderViewSerializeHandler_Create(%1, %2=_ln_return_discard) _LNRenderViewSerializeHandler_Create %1, %2
#cmd _LNMaterialSerializeHandler_Create $A4C
#define LNMaterialSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMaterialSerializeHandler_Create %1, %2
#cmd _LNMeshNodeSerializeHandler_Create $A57
#define LNMeshNodeSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshNodeSerializeHandler_Create %1, %2
#cmd _LNAnimationControllerSerializeHandler_Create $A62
#define LNAnimationControllerSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationControllerSerializeHandler_Create %1, %2
#cmd _LNMeshModelSerializeHandler_Create $A6D
#define LNMeshModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshModelSerializeHandler_Create %1, %2
#cmd _LNMeshImportSettingsSerializeHandler_Create $A78
#define LNMeshImportSettingsSerializeHandler_Create(%1, %2=_ln_return_discard) _LNMeshImportSettingsSerializeHandler_Create %1, %2
#cmd _LNSkinnedMeshModelSerializeHandler_Create $A83
#define LNSkinnedMeshModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSkinnedMeshModelSerializeHandler_Create %1, %2
#cmd _LNCollisionShapeSerializeHandler_Create $A8E
#define LNCollisionShapeSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCollisionShapeSerializeHandler_Create %1, %2
#cmd _LNBoxCollisionShapeSerializeHandler_Create $A99
#define LNBoxCollisionShapeSerializeHandler_Create(%1, %2=_ln_return_discard) _LNBoxCollisionShapeSerializeHandler_Create %1, %2
#cmd _LNAnimationCurveSerializeHandler_Create $AA4
#define LNAnimationCurveSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationCurveSerializeHandler_Create %1, %2
#cmd _LNKeyFrameAnimationCurveSerializeHandler_Create $AAF
#define LNKeyFrameAnimationCurveSerializeHandler_Create(%1, %2=_ln_return_discard) _LNKeyFrameAnimationCurveSerializeHandler_Create %1, %2
#cmd _LNAnimationClipSerializeHandler_Create $ABA
#define LNAnimationClipSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationClipSerializeHandler_Create %1, %2
#cmd _LNAnimationStateSerializeHandler_Create $AC5
#define LNAnimationStateSerializeHandler_Create(%1, %2=_ln_return_discard) _LNAnimationStateSerializeHandler_Create %1, %2
#cmd _LNEffectResourceSerializeHandler_Create $AD0
#define LNEffectResourceSerializeHandler_Create(%1, %2=_ln_return_discard) _LNEffectResourceSerializeHandler_Create %1, %2
#cmd _LNParticleEmitterModelSerializeHandler_Create $ADB
#define LNParticleEmitterModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterModelSerializeHandler_Create %1, %2
#cmd _LNParticleModelSerializeHandler_Create $AE6
#define LNParticleModelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNParticleModelSerializeHandler_Create %1, %2
#cmd _LNComponentSerializeHandler_Create $AF1
#define LNComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNComponentSerializeHandler_Create %1, %2
#cmd _LNVisualComponentSerializeHandler_Create $AFC
#define LNVisualComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNVisualComponentSerializeHandler_Create %1, %2
#cmd _LNSpriteComponentSerializeHandler_Create $B07
#define LNSpriteComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSpriteComponentSerializeHandler_Create %1, %2
#cmd _LNCharacterControllerSerializeHandler_Create $B12
#define LNCharacterControllerSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCharacterControllerSerializeHandler_Create %1, %2
#cmd _LNWorldSerializeHandler_Create $B1D
#define LNWorldSerializeHandler_Create(%1, %2=_ln_return_discard) _LNWorldSerializeHandler_Create %1, %2
#cmd _LNComponentListSerializeHandler_Create $B28
#define LNComponentListSerializeHandler_Create(%1, %2=_ln_return_discard) _LNComponentListSerializeHandler_Create %1, %2
#cmd _LNWorldObjectSerializeHandler_Create $B33
#define LNWorldObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNWorldObjectSerializeHandler_Create %1, %2
#cmd _LNWorldObjectPreUpdateHandler_Create $B3E
#define LNWorldObjectPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNWorldObjectPreUpdateHandler_Create %1, %2
#cmd _LNWorldObjectUpdateHandler_Create $B49
#define LNWorldObjectUpdateHandler_Create(%1, %2=_ln_return_discard) _LNWorldObjectUpdateHandler_Create %1, %2
#cmd _LNVisualObjectSerializeHandler_Create $B54
#define LNVisualObjectSerializeHandler_Create(%1, %2=_ln_return_discard) _LNVisualObjectSerializeHandler_Create %1, %2
#cmd _LNVisualObjectPreUpdateHandler_Create $B5F
#define LNVisualObjectPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNVisualObjectPreUpdateHandler_Create %1, %2
#cmd _LNVisualObjectUpdateHandler_Create $B6A
#define LNVisualObjectUpdateHandler_Create(%1, %2=_ln_return_discard) _LNVisualObjectUpdateHandler_Create %1, %2
#cmd _LNCameraSerializeHandler_Create $B75
#define LNCameraSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCameraSerializeHandler_Create %1, %2
#cmd _LNCameraPreUpdateHandler_Create $B80
#define LNCameraPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNCameraPreUpdateHandler_Create %1, %2
#cmd _LNCameraUpdateHandler_Create $B8B
#define LNCameraUpdateHandler_Create(%1, %2=_ln_return_discard) _LNCameraUpdateHandler_Create %1, %2
#cmd _LNEnvironmentLightSerializeHandler_Create $B96
#define LNEnvironmentLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNEnvironmentLightSerializeHandler_Create %1, %2
#cmd _LNEnvironmentLightPreUpdateHandler_Create $BA1
#define LNEnvironmentLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNEnvironmentLightPreUpdateHandler_Create %1, %2
#cmd _LNEnvironmentLightUpdateHandler_Create $BAC
#define LNEnvironmentLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNEnvironmentLightUpdateHandler_Create %1, %2
#cmd _LNDirectionalLightSerializeHandler_Create $BB7
#define LNDirectionalLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNDirectionalLightSerializeHandler_Create %1, %2
#cmd _LNDirectionalLightPreUpdateHandler_Create $BC2
#define LNDirectionalLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNDirectionalLightPreUpdateHandler_Create %1, %2
#cmd _LNDirectionalLightUpdateHandler_Create $BCD
#define LNDirectionalLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNDirectionalLightUpdateHandler_Create %1, %2
#cmd _LNPointLightSerializeHandler_Create $BD8
#define LNPointLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNPointLightSerializeHandler_Create %1, %2
#cmd _LNPointLightPreUpdateHandler_Create $BE3
#define LNPointLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPointLightPreUpdateHandler_Create %1, %2
#cmd _LNPointLightUpdateHandler_Create $BEE
#define LNPointLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPointLightUpdateHandler_Create %1, %2
#cmd _LNSpotLightSerializeHandler_Create $BF9
#define LNSpotLightSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSpotLightSerializeHandler_Create %1, %2
#cmd _LNSpotLightPreUpdateHandler_Create $C04
#define LNSpotLightPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpotLightPreUpdateHandler_Create %1, %2
#cmd _LNSpotLightUpdateHandler_Create $C0F
#define LNSpotLightUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpotLightUpdateHandler_Create %1, %2
#cmd _LNSpriteSerializeHandler_Create $C1A
#define LNSpriteSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSpriteSerializeHandler_Create %1, %2
#cmd _LNSpritePreUpdateHandler_Create $C25
#define LNSpritePreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpritePreUpdateHandler_Create %1, %2
#cmd _LNSpriteUpdateHandler_Create $C30
#define LNSpriteUpdateHandler_Create(%1, %2=_ln_return_discard) _LNSpriteUpdateHandler_Create %1, %2
#cmd _LNCameraOrbitControlComponentSerializeHandler_Create $C3B
#define LNCameraOrbitControlComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCameraOrbitControlComponentSerializeHandler_Create %1, %2
#cmd _LNRaycasterSerializeHandler_Create $C46
#define LNRaycasterSerializeHandler_Create(%1, %2=_ln_return_discard) _LNRaycasterSerializeHandler_Create %1, %2
#cmd _LNRaycastResultSerializeHandler_Create $C51
#define LNRaycastResultSerializeHandler_Create(%1, %2=_ln_return_discard) _LNRaycastResultSerializeHandler_Create %1, %2
#cmd _LNWorldRenderViewSerializeHandler_Create $C5C
#define LNWorldRenderViewSerializeHandler_Create(%1, %2=_ln_return_discard) _LNWorldRenderViewSerializeHandler_Create %1, %2
#cmd _LNBoxMeshSerializeHandler_Create $C67
#define LNBoxMeshSerializeHandler_Create(%1, %2=_ln_return_discard) _LNBoxMeshSerializeHandler_Create %1, %2
#cmd _LNBoxMeshPreUpdateHandler_Create $C72
#define LNBoxMeshPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNBoxMeshPreUpdateHandler_Create %1, %2
#cmd _LNBoxMeshUpdateHandler_Create $C7D
#define LNBoxMeshUpdateHandler_Create(%1, %2=_ln_return_discard) _LNBoxMeshUpdateHandler_Create %1, %2
#cmd _LNPlaneMeshSerializeHandler_Create $C88
#define LNPlaneMeshSerializeHandler_Create(%1, %2=_ln_return_discard) _LNPlaneMeshSerializeHandler_Create %1, %2
#cmd _LNPlaneMeshPreUpdateHandler_Create $C93
#define LNPlaneMeshPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPlaneMeshPreUpdateHandler_Create %1, %2
#cmd _LNPlaneMeshUpdateHandler_Create $C9E
#define LNPlaneMeshUpdateHandler_Create(%1, %2=_ln_return_discard) _LNPlaneMeshUpdateHandler_Create %1, %2
#cmd _LNStaticMeshSerializeHandler_Create $CA9
#define LNStaticMeshSerializeHandler_Create(%1, %2=_ln_return_discard) _LNStaticMeshSerializeHandler_Create %1, %2
#cmd _LNStaticMeshPreUpdateHandler_Create $CB4
#define LNStaticMeshPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNStaticMeshPreUpdateHandler_Create %1, %2
#cmd _LNStaticMeshUpdateHandler_Create $CBF
#define LNStaticMeshUpdateHandler_Create(%1, %2=_ln_return_discard) _LNStaticMeshUpdateHandler_Create %1, %2
#cmd _LNStaticMeshComponentSerializeHandler_Create $CCA
#define LNStaticMeshComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNStaticMeshComponentSerializeHandler_Create %1, %2
#cmd _LNSkinnedMeshComponentSerializeHandler_Create $CD5
#define LNSkinnedMeshComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNSkinnedMeshComponentSerializeHandler_Create %1, %2
#cmd _LNCollisionSerializeHandler_Create $CE0
#define LNCollisionSerializeHandler_Create(%1, %2=_ln_return_discard) _LNCollisionSerializeHandler_Create %1, %2
#cmd _LNTriggerBodyComponentSerializeHandler_Create $CEB
#define LNTriggerBodyComponentSerializeHandler_Create(%1, %2=_ln_return_discard) _LNTriggerBodyComponentSerializeHandler_Create %1, %2
#cmd _LNParticleEmitterSerializeHandler_Create $CF6
#define LNParticleEmitterSerializeHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterSerializeHandler_Create %1, %2
#cmd _LNParticleEmitterPreUpdateHandler_Create $D01
#define LNParticleEmitterPreUpdateHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterPreUpdateHandler_Create %1, %2
#cmd _LNParticleEmitterUpdateHandler_Create $D0C
#define LNParticleEmitterUpdateHandler_Create(%1, %2=_ln_return_discard) _LNParticleEmitterUpdateHandler_Create %1, %2
#cmd _LNLevelSerializeHandler_Create $D17
#define LNLevelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNLevelSerializeHandler_Create %1, %2
#cmd _LNLevelStartHandler_Create $D22
#define LNLevelStartHandler_Create(%1, %2=_ln_return_discard) _LNLevelStartHandler_Create %1, %2
#cmd _LNLevelStopHandler_Create $D2D
#define LNLevelStopHandler_Create(%1, %2=_ln_return_discard) _LNLevelStopHandler_Create %1, %2
#cmd _LNLevelPauseHandler_Create $D38
#define LNLevelPauseHandler_Create(%1, %2=_ln_return_discard) _LNLevelPauseHandler_Create %1, %2
#cmd _LNLevelResumeHandler_Create $D43
#define LNLevelResumeHandler_Create(%1, %2=_ln_return_discard) _LNLevelResumeHandler_Create %1, %2
#cmd _LNLevelUpdateHandler_Create $D4E
#define LNLevelUpdateHandler_Create(%1, %2=_ln_return_discard) _LNLevelUpdateHandler_Create %1, %2
#cmd _LNUIEventArgsSerializeHandler_Create $D59
#define LNUIEventArgsSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIEventArgsSerializeHandler_Create %1, %2
#cmd _LNUILayoutElementSerializeHandler_Create $D64
#define LNUILayoutElementSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUILayoutElementSerializeHandler_Create %1, %2
#cmd _LNUIElementSerializeHandler_Create $D6F
#define LNUIElementSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIElementSerializeHandler_Create %1, %2
#cmd _LNUITextBlockSerializeHandler_Create $D7A
#define LNUITextBlockSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUITextBlockSerializeHandler_Create %1, %2
#cmd _LNUISpriteSerializeHandler_Create $D85
#define LNUISpriteSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUISpriteSerializeHandler_Create %1, %2
#cmd _LNUIIconSerializeHandler_Create $D90
#define LNUIIconSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIIconSerializeHandler_Create %1, %2
#cmd _LNUIMessageTextAreaSerializeHandler_Create $D9B
#define LNUIMessageTextAreaSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIMessageTextAreaSerializeHandler_Create %1, %2
#cmd _LNUILayoutPanelSerializeHandler_Create $DA6
#define LNUILayoutPanelSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUILayoutPanelSerializeHandler_Create %1, %2
#cmd _LNUIBoxLayoutSerializeHandler_Create $DB1
#define LNUIBoxLayoutSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIBoxLayoutSerializeHandler_Create %1, %2
#cmd _LNUIStackLayoutSerializeHandler_Create $DBC
#define LNUIStackLayoutSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIStackLayoutSerializeHandler_Create %1, %2
#cmd _LNUIGridLayoutSerializeHandler_Create $DC7
#define LNUIGridLayoutSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIGridLayoutSerializeHandler_Create %1, %2
#cmd _LNUIControlSerializeHandler_Create $DD2
#define LNUIControlSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIControlSerializeHandler_Create %1, %2
#cmd _LNUIButtonBaseSerializeHandler_Create $DDD
#define LNUIButtonBaseSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIButtonBaseSerializeHandler_Create %1, %2
#cmd _LNUIButtonSerializeHandler_Create $DE8
#define LNUIButtonSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIButtonSerializeHandler_Create %1, %2
#cmd _LNUIWindowSerializeHandler_Create $DF3
#define LNUIWindowSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIWindowSerializeHandler_Create %1, %2
#cmd _LNUIListItemSerializeHandler_Create $DFE
#define LNUIListItemSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListItemSerializeHandler_Create %1, %2
#cmd _LNUIListItemsControlSerializeHandler_Create $E09
#define LNUIListItemsControlSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListItemsControlSerializeHandler_Create %1, %2
#cmd _LNUIListBoxItemSerializeHandler_Create $E14
#define LNUIListBoxItemSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListBoxItemSerializeHandler_Create %1, %2
#cmd _LNUIListBoxSerializeHandler_Create $E1F
#define LNUIListBoxSerializeHandler_Create(%1, %2=_ln_return_discard) _LNUIListBoxSerializeHandler_Create %1, %2
#cmd _LNInputGestureSerializeHandler_Create $E2A
#define LNInputGestureSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInputGestureSerializeHandler_Create %1, %2
#cmd _LNKeyGestureSerializeHandler_Create $E35
#define LNKeyGestureSerializeHandler_Create(%1, %2=_ln_return_discard) _LNKeyGestureSerializeHandler_Create %1, %2
#cmd _LNInterpreterCommandSerializeHandler_Create $E40
#define LNInterpreterCommandSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterCommandSerializeHandler_Create %1, %2
#cmd _LNInterpreterCommandListSerializeHandler_Create $E4B
#define LNInterpreterCommandListSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterCommandListSerializeHandler_Create %1, %2
#cmd _LNInterpreterSerializeHandler_Create $E56
#define LNInterpreterSerializeHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterSerializeHandler_Create %1, %2
#cmd _LNInterpreterUpdateWaitHandler_Create $E62
#define LNInterpreterUpdateWaitHandler_Create(%1, %2=_ln_return_discard) _LNInterpreterUpdateWaitHandler_Create %1, %2
#cmd _LNApplicationSerializeHandler_Create $E6D
#define LNApplicationSerializeHandler_Create(%1, %2=_ln_return_discard) _LNApplicationSerializeHandler_Create %1, %2
#cmd _LNApplicationInitHandler_Create $E78
#define LNApplicationInitHandler_Create(%1, %2=_ln_return_discard) _LNApplicationInitHandler_Create %1, %2
#cmd _LNApplicationUpdateHandler_Create $E83
#define LNApplicationUpdateHandler_Create(%1, %2=_ln_return_discard) _LNApplicationUpdateHandler_Create %1, %2



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
