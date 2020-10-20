
#ifndef __lumino__
#define __lumino__

#ifdef LUMINO_DEBUG
    #regcmd "_hsp3cmdinit@4","LuminoHSPd.dll", 11
#else
    #regcmd "_hsp3cmdinit@4","LuminoHSP.dll", 11
#endif

#const global LN_TRUE 1
#const global LN_FALSE 0
#const global LN_NULL_HANDLE 0

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

#cmd LNVector3 $2B
#cmd LNVector3_GetX $54C
#cmd LNVector3_SetX $54E
#cmd LNVector3_GetY $54F
#cmd LNVector3_SetY $551
#cmd LNVector3_GetZ $552
#cmd LNVector3_SetZ $554
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $56B
#cmd LNVector4_SetX $56D
#cmd LNVector4_GetY $56E
#cmd LNVector4_SetY $570
#cmd LNVector4_GetZ $571
#cmd LNVector4_SetZ $573
#cmd LNVector4_GetW $574
#cmd LNVector4_SetW $576
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $589
#cmd LNQuaternion_SetX $58B
#cmd LNQuaternion_GetY $58C
#cmd LNQuaternion_SetY $58E
#cmd LNQuaternion_GetZ $58F
#cmd LNQuaternion_SetZ $591
#cmd LNQuaternion_GetW $592
#cmd LNQuaternion_SetW $594
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $5A8
#cmd LNMatrix_SetRow0 $5AA
#cmd LNMatrix_GetRow1 $5AB
#cmd LNMatrix_SetRow1 $5AD
#cmd LNMatrix_GetRow2 $5AE
#cmd LNMatrix_SetRow2 $5B0
#cmd LNMatrix_GetRow3 $5B1
#cmd LNMatrix_SetRow3 $5B3
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $173
#cmd LNColor_GetR $645
#cmd LNColor_SetR $647
#cmd LNColor_GetG $648
#cmd LNColor_SetG $64A
#cmd LNColor_GetB $64B
#cmd LNColor_SetB $64D
#cmd LNColor_GetA $64E
#cmd LNColor_SetA $650
#cmd LNColor_SetZeros $178
#cmd LNColor_Set $179
#cmd LNColorTone $17E
#cmd LNColorTone_GetR $664
#cmd LNColorTone_SetR $666
#cmd LNColorTone_GetG $667
#cmd LNColorTone_SetG $669
#cmd LNColorTone_GetB $66A
#cmd LNColorTone_SetB $66C
#cmd LNColorTone_GetS $66D
#cmd LNColorTone_SetS $66F
#cmd LNColorTone_SetZeros $183
#cmd LNColorTone_Set $184
#cmd LNPoint $189
#cmd LNPoint_GetX $682
#cmd LNPoint_SetX $684
#cmd LNPoint_GetY $685
#cmd LNPoint_SetY $687
#cmd LNPoint_SetZeros $18C
#cmd LNPoint_Set $18D
#cmd LNSize $190
#cmd LNSize_GetWidth $692
#cmd LNSize_SetWidth $694
#cmd LNSize_GetHeight $695
#cmd LNSize_SetHeight $697
#cmd LNSize_SetZeros $193
#cmd LNSize_Set $194
#cmd LNRect $197
#cmd LNRect_GetX $6A2
#cmd LNRect_SetX $6A4
#cmd LNRect_GetY $6A5
#cmd LNRect_SetY $6A7
#cmd LNRect_GetWidth $6A8
#cmd LNRect_SetWidth $6AA
#cmd LNRect_GetHeight $6AB
#cmd LNRect_SetHeight $6AD
#cmd LNRect_SetZeros $19C
#cmd LNRect_Set $19D
#cmd LNRect_GetLeft $1A2
#cmd LNRect_SetSize $1A3
#cmd LNRect_GetSize $1A5
#cmd LNThickness $1A6
#cmd LNThickness_GetLeft $6C6
#cmd LNThickness_SetLeft $6C8
#cmd LNThickness_GetTop $6C9
#cmd LNThickness_SetTop $6CB
#cmd LNThickness_GetRight $6CC
#cmd LNThickness_SetRight $6CE
#cmd LNThickness_GetBottom $6CF
#cmd LNThickness_SetBottom $6D1
#cmd LNThickness_SetZeros $1AB
#cmd LNThickness_Set $1AC
#cmd LNCornerRadius $1B1
#cmd LNCornerRadius_GetTopleft $6E4
#cmd LNCornerRadius_SetTopleft $6E6
#cmd LNCornerRadius_GetTopright $6E7
#cmd LNCornerRadius_SetTopright $6E9
#cmd LNCornerRadius_GetBottomright $6EA
#cmd LNCornerRadius_SetBottomright $6EC
#cmd LNCornerRadius_GetBottomleft $6ED
#cmd LNCornerRadius_SetBottomleft $6EF
#cmd LNCornerRadius_SetZeros $1B6
#cmd LNCornerRadius_Set $1B7

#cmd LNObject_Release $7A
#cmd LNObject_Retain $7B
#cmd LNObject_GetReferenceCount $7C
#cmd LNObject_SetPrototype_OnSerialize $98F
#cmd LNEventConnection_SetPrototype_OnSerialize $99A
#cmd LNPromiseFailureDelegate_Create $5CE
#cmd LNVariant_Create $81
#cmd LNVariant_SetInt $82
#cmd LNVariant_GetInt $84
#cmd LNVariant_SetPrototype_OnSerialize $9A5
#cmd LNZVTestDelegate1_Create $5D7
#cmd LNZVTestDelegate2_Create $5DD
#cmd LNZVTestDelegate3_Create $5E2
#cmd LNZVTestEventHandler1_Create $5E7
#cmd LNZVTestEventHandler2_Create $5EC
#cmd LNZVTestPromise1_ThenWith $5EF
#cmd LNZVTestPromise1_CatchWith $5F1
#cmd LNZVTestPromise2_ThenWith $5F5
#cmd LNZVTestPromise2_CatchWith $5F7
#cmd LNZVTestClass1_Create $AD
#cmd LNZVTestClass1_SetTestDelegate1 $97
#cmd LNZVTestClass1_SetTestDelegate2 $99
#cmd LNZVTestClass1_SetTestDelegate3 $9B
#cmd LNZVTestClass1_CallTestDelegate1 $9D
#cmd LNZVTestClass1_CallTestDelegate2 $9F
#cmd LNZVTestClass1_CallTestDelegate3 $A2
#cmd LNZVTestClass1_LoadAsync $A3
#cmd LNZVTestClass1_ExecuteAsync $A5
#cmd LNZVTestClass1_GetFilePath $A6
#cmd LNZVTestClass1_ConnectOnEvent1 $A7
#cmd LNZVTestClass1_RaiseEvent1 $A9
#cmd LNZVTestClass1_ConnectOnEvent2 $AA
#cmd LNZVTestClass1_RaiseEvent2 $AC
#cmd LNZVTestClass1_SetPrototype_OnSerialize $9B0
#cmd LNZVTestEventArgs1_Create $B0
#cmd LNZVTestEventArgs1_CreateWithValue $B1
#cmd LNZVTestEventArgs1_GetValue $AF
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $9BB
#cmd LNSerializer2_SetPrototype_OnSerialize $9C6
#cmd LNAssetObject_SetPrototype_OnSerialize $9D1
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $9DC
#cmd LNAssetModel_Create $113
#cmd LNAssetModel_Target $112
#cmd LNAssetModel_SetPrototype_OnSerialize $9E7
#cmd LNAssets_SaveAssetToLocalFile $116
#cmd LNAssets_LoadAssetFromLocalFile $119
#cmd LNAssets_LoadAsset $11B
#cmd LNAssets_ReloadAsset $11D
#cmd LNAssets_ReadAllText $120
#cmd LNSound_SetVolume $12A
#cmd LNSound_GetVolume $12C
#cmd LNSound_SetPitch $12D
#cmd LNSound_GetPitch $12F
#cmd LNSound_SetLoopEnabled $130
#cmd LNSound_IsLoopEnabled $132
#cmd LNSound_SetLoopRange $133
#cmd LNSound_Play $136
#cmd LNSound_Stop $137
#cmd LNSound_Pause $138
#cmd LNSound_Resume $139
#cmd LNSound_FadeVolume $13A
#cmd LNSound_SetPrototype_OnSerialize $9F2
#cmd LNAudio_PlayBGM $13F
#cmd LNAudio_StopBGM $144
#cmd LNAudio_PlayBGS $146
#cmd LNAudio_StopBGS $14B
#cmd LNAudio_PlayME $14D
#cmd LNAudio_StopME $151
#cmd LNAudio_PlaySE $152
#cmd LNAudio_PlaySE3D $156
#cmd LNAudio_StopSE $15C
#cmd LNTexture2DDelegate_Create $63D
#cmd LNTexture2DPromise_ThenWith $640
#cmd LNTexture2DPromise_CatchWith $642
#cmd LNTexture_SetPrototype_OnSerialize $9FD
#cmd LNTexture2D_Create $1C2
#cmd LNTexture2D_CreateWithFormat $1C5
#cmd LNTexture2D_Load $1BE
#cmd LNTexture2D_LoadEmoji $1C0
#cmd LNTexture2D_SetPrototype_OnSerialize $A08
#cmd LNShader_Load $1CA
#cmd LNShader_SetFloat $1CD
#cmd LNShader_SetVector3 $1D0
#cmd LNShader_SetVector4 $1D3
#cmd LNShader_SetPrototype_OnSerialize $A13
#cmd LNRenderView_SetPrototype_OnSerialize $A1E
#cmd LNMaterial_Create $1F8
#cmd LNMaterial_SetMainTexture $1E7
#cmd LNMaterial_GetMainTexture $1E9
#cmd LNMaterial_SetColor $1EA
#cmd LNMaterial_SetRoughness $1EC
#cmd LNMaterial_SetMetallic $1EE
#cmd LNMaterial_SetEmissive $1F0
#cmd LNMaterial_SetShadingModel $1F2
#cmd LNMaterial_GetShadingModel $1F4
#cmd LNMaterial_SetShader $1F5
#cmd LNMaterial_GetShader $1F7
#cmd LNMaterial_SetPrototype_OnSerialize $A29
#cmd LNMeshNode_SetVisible $1FA
#cmd LNMeshNode_IsVisible $1FC
#cmd LNMeshNode_SetPrototype_OnSerialize $A34
#cmd LNStaticMeshModel_Load $1FE
#cmd LNStaticMeshModel_FindNode $200
#cmd LNStaticMeshModel_FindMaterial $202
#cmd LNStaticMeshModel_MaterialCount $204
#cmd LNStaticMeshModel_Material $205
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $A3F
#cmd LNSkinnedMeshModel_Load $208
#cmd LNSkinnedMeshModel_GetAnimationController $20A
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $A4A
#cmd LNAnimationController_AddClip $20C
#cmd LNAnimationController_Play $20E
#cmd LNAnimationController_SetPrototype_OnSerialize $A55
#cmd LNCollisionShape_SetPrototype_OnSerialize $A60
#cmd LNBoxCollisionShape_Create $213
#cmd LNBoxCollisionShape_CreateWHD $215
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $A6B
#cmd LNAnimationCurve_Evaluate $223
#cmd LNAnimationCurve_SetPrototype_OnSerialize $A76
#cmd LNKeyFrameAnimationCurve_Create $230
#cmd LNKeyFrameAnimationCurve_AddKeyFrame $22B
#cmd LNKeyFrameAnimationCurve_SetPrototype_OnSerialize $A81
#cmd LNAnimationClip_Load $232
#cmd LNAnimationClip_SetWrapMode $234
#cmd LNAnimationClip_GetWrapMode $236
#cmd LNAnimationClip_SetHierarchicalAnimationMode $237
#cmd LNAnimationClip_GetHierarchicalAnimationMode $239
#cmd LNAnimationClip_SetPrototype_OnSerialize $A8C
#cmd LNAnimationState_SetPrototype_OnSerialize $A97
#cmd LNEffectResource_SetPrototype_OnSerialize $AA2
#cmd LNParticleEmitterModel_Create $25E
#cmd LNParticleEmitterModel_SetMaxParticles $246
#cmd LNParticleEmitterModel_SetSpawnRate $248
#cmd LNParticleEmitterModel_SetLifeTime $24A
#cmd LNParticleEmitterModel_SetupBoxShape $24C
#cmd LNParticleEmitterModel_SetSize $24E
#cmd LNParticleEmitterModel_SetSizeVelocity $250
#cmd LNParticleEmitterModel_SetSizeAcceleration $252
#cmd LNParticleEmitterModel_SetForwardVelocityMin $254
#cmd LNParticleEmitterModel_SetForwardVelocityMax $256
#cmd LNParticleEmitterModel_SetForwardScale $258
#cmd LNParticleEmitterModel_SetGeometryDirection $25A
#cmd LNParticleEmitterModel_SetupSpriteModule $25C
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $AAD
#cmd LNParticleModel_Create $265
#cmd LNParticleModel_SetLoop $260
#cmd LNParticleModel_IsLoop $262
#cmd LNParticleModel_AddEmitter $263
#cmd LNParticleModel_SetPrototype_OnSerialize $AB8
#cmd LNComponent_SetPrototype_OnSerialize $AC3
#cmd LNVisualComponent_SetVisible $268
#cmd LNVisualComponent_IsVisible $26A
#cmd LNVisualComponent_SetPrototype_OnSerialize $ACE
#cmd LNSpriteComponent_SetTexture $26C
#cmd LNSpriteComponent_SetPrototype_OnSerialize $AD9
#cmd LNCollisionEventHandler_Create $771
#cmd LNCharacterController_Create $28C
#cmd LNCharacterController_SetWalkVelocity $276
#cmd LNCharacterController_GetWalkVelocity $278
#cmd LNCharacterController_SetVelocity $279
#cmd LNCharacterController_GetVelocity $27B
#cmd LNCharacterController_SetInputControlEnabled $27C
#cmd LNCharacterController_SetCameraControlEnabled $27E
#cmd LNCharacterController_SetHeight $280
#cmd LNCharacterController_GetHeight $282
#cmd LNCharacterController_SetCameraRadius $283
#cmd LNCharacterController_GetCameraRadius $285
#cmd LNCharacterController_SetCollisionEnter $286
#cmd LNCharacterController_SetCollisionLeave $288
#cmd LNCharacterController_SetCollisionStay $28A
#cmd LNCharacterController_SetPrototype_OnSerialize $AE4
#cmd LNWorld_Add $28E
#cmd LNWorld_SetPrototype_OnSerialize $AEF
#cmd LNComponentList_GetLength $791
#cmd LNComponentList_GetItem $793
#cmd LNComponentList_SetPrototype_OnSerialize $AFA
#cmd LNWorldObject_Create $2C5
#cmd LNWorldObject_SetPosition $292
#cmd LNWorldObject_SetPositionXYZ $294
#cmd LNWorldObject_GetPosition $298
#cmd LNWorldObject_SetRotationQuaternion $299
#cmd LNWorldObject_SetRotation $29B
#cmd LNWorldObject_GetRotation $29F
#cmd LNWorldObject_SetScale $2A0
#cmd LNWorldObject_SetScaleS $2A2
#cmd LNWorldObject_SetScaleXYZ $2A4
#cmd LNWorldObject_GetScale $2A8
#cmd LNWorldObject_SetCenterPoint $2A9
#cmd LNWorldObject_SetCenterPointXYZ $2AB
#cmd LNWorldObject_GetCenterPoint $2AF
#cmd LNWorldObject_LookAt $2B0
#cmd LNWorldObject_LookAtXYZ $2B2
#cmd LNWorldObject_AddComponent $2B6
#cmd LNWorldObject_RemoveComponent $2B8
#cmd LNWorldObject_AddTag $2BA
#cmd LNWorldObject_RemoveTag $2BC
#cmd LNWorldObject_HasTag $2BE
#cmd LNWorldObject_Destroy $2C0
#cmd LNWorldObject_GetComponents $2C1
#cmd LNWorldObject_SetPrototype_OnSerialize $B05
#cmd LNWorldObject_SetPrototype_OnPreUpdate $B10
#cmd LNWorldObject_SetPrototype_OnUpdate $B1B
#cmd LNVisualObject_SetVisible $2C7
#cmd LNVisualObject_IsVisible $2C9
#cmd LNVisualObject_SetBlendMode2 $2CA
#cmd LNVisualObject_SetOpacity $2CC
#cmd LNVisualObject_GetOpacity $2CE
#cmd LNVisualObject_SetPrototype_OnSerialize $B26
#cmd LNVisualObject_SetPrototype_OnPreUpdate $B31
#cmd LNVisualObject_SetPrototype_OnUpdate $B3C
#cmd LNCamera_SetPrototype_OnSerialize $B47
#cmd LNCamera_SetPrototype_OnPreUpdate $B52
#cmd LNCamera_SetPrototype_OnUpdate $B5D
#cmd LNEnvironmentLight_SetEnabled $2D1
#cmd LNEnvironmentLight_IsEnabled $2D3
#cmd LNEnvironmentLight_SetColor $2D4
#cmd LNEnvironmentLight_GetColor $2D6
#cmd LNEnvironmentLight_SetAmbientColor $2D7
#cmd LNEnvironmentLight_GetAmbientColor $2D9
#cmd LNEnvironmentLight_GetSkyColor $2DA
#cmd LNEnvironmentLight_SetSkyColor $2DB
#cmd LNEnvironmentLight_GetGroundColor $2DD
#cmd LNEnvironmentLight_SetGroundColor $2DE
#cmd LNEnvironmentLight_SetIntensity $2E0
#cmd LNEnvironmentLight_GetIntensity $2E2
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $2E3
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $2E5
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $2E6
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $2E8
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $B68
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $B73
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $B7E
#cmd LNDirectionalLight_Create $2F9
#cmd LNDirectionalLight_CreateWithColor $2FA
#cmd LNDirectionalLight_SetEnabled $2EA
#cmd LNDirectionalLight_IsEnabled $2EC
#cmd LNDirectionalLight_SetColor $2ED
#cmd LNDirectionalLight_GetColor $2EF
#cmd LNDirectionalLight_SetIntensity $2F0
#cmd LNDirectionalLight_GetIntensity $2F2
#cmd LNDirectionalLight_SetShadowEffectiveDistance $2F3
#cmd LNDirectionalLight_GetShadowEffectiveDistance $2F5
#cmd LNDirectionalLight_SetShadowEffectiveDepth $2F6
#cmd LNDirectionalLight_GetShadowEffectiveDepth $2F8
#cmd LNDirectionalLight_SetPrototype_OnSerialize $B89
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $B94
#cmd LNDirectionalLight_SetPrototype_OnUpdate $B9F
#cmd LNPointLight_Create $30C
#cmd LNPointLight_CreateWithColorAndRange $30D
#cmd LNPointLight_SetEnabled $2FD
#cmd LNPointLight_IsEnabled $2FF
#cmd LNPointLight_SetColor $300
#cmd LNPointLight_GetColor $302
#cmd LNPointLight_SetIntensity $303
#cmd LNPointLight_GetIntensity $305
#cmd LNPointLight_SetRange $306
#cmd LNPointLight_GetRange $308
#cmd LNPointLight_SetAttenuation $309
#cmd LNPointLight_GetAttenuation $30B
#cmd LNPointLight_SetPrototype_OnSerialize $BAA
#cmd LNPointLight_SetPrototype_OnPreUpdate $BB5
#cmd LNPointLight_SetPrototype_OnUpdate $BC0
#cmd LNSpotLight_Create $326
#cmd LNSpotLight_CreateWithColorAndRange $327
#cmd LNSpotLight_SetEnabled $311
#cmd LNSpotLight_IsEnabled $313
#cmd LNSpotLight_SetColor $314
#cmd LNSpotLight_GetColor $316
#cmd LNSpotLight_SetIntensity $317
#cmd LNSpotLight_GetIntensity $319
#cmd LNSpotLight_SetRange $31A
#cmd LNSpotLight_GetRange $31C
#cmd LNSpotLight_SetAttenuation $31D
#cmd LNSpotLight_GetAttenuation $31F
#cmd LNSpotLight_SetAngle $320
#cmd LNSpotLight_GetAngle $322
#cmd LNSpotLight_SetPenumbra $323
#cmd LNSpotLight_GetPenumbra $325
#cmd LNSpotLight_SetPrototype_OnSerialize $BCB
#cmd LNSpotLight_SetPrototype_OnPreUpdate $BD6
#cmd LNSpotLight_SetPrototype_OnUpdate $BE1
#cmd LNTestDelegate_Create $83F
#cmd LNSprite_Create $33D
#cmd LNSprite_CreateWithTexture $33E
#cmd LNSprite_CreateWithTextureAndSize $340
#cmd LNSprite_SetTexture $32F
#cmd LNSprite_SetSize $331
#cmd LNSprite_SetSizeWH $333
#cmd LNSprite_SetSourceRectXYWH $336
#cmd LNSprite_SetCallerTest $33B
#cmd LNSprite_SetPrototype_OnSerialize $BEC
#cmd LNSprite_SetPrototype_OnPreUpdate $BF7
#cmd LNSprite_SetPrototype_OnUpdate $C02
#cmd LNCameraOrbitControlComponent_Create $345
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $C0D
#cmd LNRaycaster_FromScreen $347
#cmd LNRaycaster_IntersectPlane $349
#cmd LNRaycaster_SetPrototype_OnSerialize $C18
#cmd LNRaycastResult_GetPoint $34E
#cmd LNRaycastResult_SetPrototype_OnSerialize $C23
#cmd LNWorldRenderView_SetGuideGridEnabled $350
#cmd LNWorldRenderView_GetGuideGridEnabled $352
#cmd LNWorldRenderView_SetPrototype_OnSerialize $C2E
#cmd LNBoxMesh_Create $354
#cmd LNBoxMesh_SetPrototype_OnSerialize $C39
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $C44
#cmd LNBoxMesh_SetPrototype_OnUpdate $C4F
#cmd LNPlaneMesh_Create $356
#cmd LNPlaneMesh_SetPrototype_OnSerialize $C5A
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $C65
#cmd LNPlaneMesh_SetPrototype_OnUpdate $C70
#cmd LNStaticMesh_Load $358
#cmd LNStaticMesh_GetModel $35A
#cmd LNStaticMesh_MakeCollisionBody $35B
#cmd LNStaticMesh_SetPrototype_OnSerialize $C7B
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $C86
#cmd LNStaticMesh_SetPrototype_OnUpdate $C91
#cmd LNStaticMeshComponent_Create $362
#cmd LNStaticMeshComponent_SetModel $35E
#cmd LNStaticMeshComponent_MakeCollisionBody $360
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $C9C
#cmd LNSkinnedMeshComponent_Create $366
#cmd LNSkinnedMeshComponent_SetModel $364
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $CA7
#cmd LNCollision_GetWorldObject $368
#cmd LNCollision_SetPrototype_OnSerialize $CB2
#cmd LNTriggerBodyComponent_Create $36C
#cmd LNTriggerBodyComponent_AddCollisionShape $36A
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $CBD
#cmd LNParticleEmitter_Create $36E
#cmd LNParticleEmitter_SetPrototype_OnSerialize $CC8
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $CD3
#cmd LNParticleEmitter_SetPrototype_OnUpdate $CDE
#cmd LNScene_SetClearMode $371
#cmd LNScene_SetSkyColor $373
#cmd LNScene_SetSkyHorizonColor $375
#cmd LNScene_SetSkyCloudColor $377
#cmd LNScene_SetSkyOverlayColor $379
#cmd LNScene_GotoLevel $37B
#cmd LNScene_CallLevel $37E
#cmd LNScene_ReturnLevel $381
#cmd LNScene_ActiveLevel $383
#cmd LNScene_IsTransitionEffectRunning $384
#cmd LNScene_SetTransitionEffectMode $385
#cmd LNScene_TransitionEffectMode $387
#cmd LNScene_SetTransitionDuration $388
#cmd LNScene_TransitionDuration $38A
#cmd LNScene_SetTransitionEffectColor $38B
#cmd LNScene_TransitionEffectColor $38D
#cmd LNScene_SetTransitionEffectMaskTexture $38E
#cmd LNScene_TransitionEffectMaskTexture $390
#cmd LNScene_SetTransitionEffectVague $391
#cmd LNScene_TransitionEffectVague $393
#cmd LNScene_StartFadeOut $394
#cmd LNScene_StartFadeIn $395
#cmd LNScene_SetFogStartDistance $396
#cmd LNScene_SetFogColor $398
#cmd LNScene_SetFogDensity $39A
#cmd LNScene_SetFogHeightDensity $39C
#cmd LNScene_SetFogLowerHeight $39E
#cmd LNScene_SetFogUpperHeight $3A0
#cmd LNScene_SetHDREnabled $3A2
#cmd LNScene_IsHDREnabled $3A4
#cmd LNScene_SetScreenBlendColor $3A5
#cmd LNScene_ScreenBlendColor $3A7
#cmd LNScene_SetColorTone $3A8
#cmd LNScene_ColorTone $3AA
#cmd LNScene_SetAntialiasEnabled $3AB
#cmd LNScene_IsAntialiasEnabled $3AD
#cmd LNScene_SetSSREnabled $3AE
#cmd LNScene_IsSSREnabled $3B0
#cmd LNScene_SetSSAOEnabled $3B1
#cmd LNScene_IsSSAOEnabled $3B3
#cmd LNScene_SetBloomEnabled $3B4
#cmd LNScene_IsBloomEnabled $3B6
#cmd LNScene_SetDOFEnabled $3B7
#cmd LNScene_IsDOFEnabled $3B9
#cmd LNScene_SetTonemapEnabled $3BA
#cmd LNScene_IsTonemapEnabled $3BC
#cmd LNScene_SetVignetteEnabled $3BD
#cmd LNScene_IsVignetteEnabled $3BF
#cmd LNScene_SetGammaEnabled $3C0
#cmd LNScene_IsGammaEnabled $3C2
#cmd LNScene_SetTonemapExposure $3C3
#cmd LNScene_SetTonemapLinearWhite $3C5
#cmd LNScene_SetTonemapShoulderStrength $3C7
#cmd LNScene_SetTonemapLinearStrength $3C9
#cmd LNScene_SetTonemapLinearAngle $3CB
#cmd LNScene_SetTonemapToeStrength $3CD
#cmd LNScene_SetTonemapToeNumerator $3CF
#cmd LNScene_SetTonemapToeDenominator $3D1
#cmd LNLevel_Create $3E2
#cmd LNLevel_AddObject $3D4
#cmd LNLevel_RemoveObject $3D6
#cmd LNLevel_AddSubLevel $3D8
#cmd LNLevel_RemoveSubLevel $3DA
#cmd LNLevel_RemoveAllSubLevels $3DC
#cmd LNLevel_SetPrototype_OnSerialize $CE9
#cmd LNLevel_SetPrototype_OnStart $CF4
#cmd LNLevel_SetPrototype_OnStop $CFF
#cmd LNLevel_SetPrototype_OnPause $D0A
#cmd LNLevel_SetPrototype_OnResume $D15
#cmd LNLevel_SetPrototype_OnUpdate $D20
#cmd LNUIEventArgs_Sender $3ED
#cmd LNUIEventArgs_SetPrototype_OnSerialize $D2B
#cmd LNUIGeneralEventHandler_Create $89E
#cmd LNUIEventHandler_Create $8A3
#cmd LNUILayoutElement_SetPrototype_OnSerialize $D36
#cmd LNUIElement_SetSize $3FF
#cmd LNUIElement_SetSizeWH $401
#cmd LNUIElement_SetWidth $404
#cmd LNUIElement_GetWidth $406
#cmd LNUIElement_SetHeight $407
#cmd LNUIElement_GetHeight $409
#cmd LNUIElement_SetMargin $40A
#cmd LNUIElement_GetMargin $40C
#cmd LNUIElement_SetPadding $40D
#cmd LNUIElement_GetPadding $40F
#cmd LNUIElement_SetHAlignment $410
#cmd LNUIElement_GetHAlignment $412
#cmd LNUIElement_SetVAlignment $413
#cmd LNUIElement_GetVAlignment $415
#cmd LNUIElement_SetAlignments $416
#cmd LNUIElement_SetPosition $419
#cmd LNUIElement_SetPositionXYZ $41B
#cmd LNUIElement_GetPosition $41F
#cmd LNUIElement_SetRotation $420
#cmd LNUIElement_SetEulerAngles $422
#cmd LNUIElement_GetRotation $426
#cmd LNUIElement_SetScale $427
#cmd LNUIElement_SetScaleS $429
#cmd LNUIElement_SetScaleXY $42B
#cmd LNUIElement_GetScale $42E
#cmd LNUIElement_SetCenterPoint $42F
#cmd LNUIElement_SetCenterPointXYZ $431
#cmd LNUIElement_GetCenterPoint $435
#cmd LNUIElement_SetEnabled $436
#cmd LNUIElement_IsEnabled $438
#cmd LNUIElement_SetData $439
#cmd LNUIElement_GetData $43B
#cmd LNUIElement_SetBackgroundColor $43C
#cmd LNUIElement_GetBackgroundColor $43E
#cmd LNUIElement_SetBorderThickness $43F
#cmd LNUIElement_GetBorderThickness $441
#cmd LNUIElement_SetBorderColor $442
#cmd LNUIElement_GetBorderColor $444
#cmd LNUIElement_SetCornerRadius $445
#cmd LNUIElement_GetCornerRadius $447
#cmd LNUIElement_SetVisibility $448
#cmd LNUIElement_GetVisibility $44A
#cmd LNUIElement_SetOpacity $44B
#cmd LNUIElement_GetOpacity $44D
#cmd LNUIElement_AddChild $44E
#cmd LNUIElement_SetPrototype_OnSerialize $D41
#cmd LNUITextBlock_Create $454
#cmd LNUITextBlock_CreateWithText $455
#cmd LNUITextBlock_SetText $451
#cmd LNUITextBlock_GetText $453
#cmd LNUITextBlock_SetPrototype_OnSerialize $D4C
#cmd LNUISprite_Create $462
#cmd LNUISprite_CreateWithTexture $463
#cmd LNUISprite_SetTexture $458
#cmd LNUISprite_SetSourceRect $45A
#cmd LNUISprite_SetSourceRectXYWH $45C
#cmd LNUISprite_GetSourceRect $461
#cmd LNUISprite_SetPrototype_OnSerialize $D57
#cmd LNUIIcon_LoadFontIcon $466
#cmd LNUIIcon_LoadFontIconWithNameSize $468
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $46B
#cmd LNUIIcon_SetPrototype_OnSerialize $D62
#cmd LNUIMessageTextArea_Create $474
#cmd LNUIMessageTextArea_SetText $470
#cmd LNUIMessageTextArea_SetTypingSpeed $472
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $D6D
#cmd LNUI_Add $476
#cmd LNUI_Remove $478
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $D78
#cmd LNUIBoxLayout_Create $47F
#cmd LNUIBoxLayout_SetOrientation $47C
#cmd LNUIBoxLayout_GetOrientation $47E
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $D83
#cmd LNUIStackLayout_Create $484
#cmd LNUIStackLayout_SetOrientation $481
#cmd LNUIStackLayout_GetOrientation $483
#cmd LNUIStackLayout_SetPrototype_OnSerialize $D8E
#cmd LNUIGridLayout_Create $494
#cmd LNUIGridLayout_SetColumnCount $486
#cmd LNUIGridLayout_SetRow $488
#cmd LNUIGridLayout_SetColumn $48B
#cmd LNUIGridLayout_SetPlacement $48E
#cmd LNUIGridLayout_SetPrototype_OnSerialize $D99
#cmd LNUIControl_Create $4A0
#cmd LNUIControl_AddInlineVisual $4A1
#cmd LNUIControl_SetPrototype_OnSerialize $DA4
#cmd LNUIButtonBase_SetText $4A5
#cmd LNUIButtonBase_SetPrototype_OnSerialize $DAF
#cmd LNUIButton_Create $4A8
#cmd LNUIButton_CreateWithText $4A9
#cmd LNUIButton_ConnectOnClicked $4AB
#cmd LNUIButton_SetPrototype_OnSerialize $DBA
#cmd LNUIWindow_Create $4AE
#cmd LNUIWindow_SetPrototype_OnSerialize $DC5
#cmd LNUIListItem_ConnectOnSubmit $4B3
#cmd LNUIListItem_SetPrototype_OnSerialize $DD0
#cmd LNUIListItemsControl_SetItemsLayoutPanel $4B6
#cmd LNUIListItemsControl_SetSubmitMode $4B8
#cmd LNUIListItemsControl_GetSubmitMode $4BA
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $DDB
#cmd LNUIListBoxItem_Create $4BC
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $DE6
#cmd LNUIListBox_Create $4C1
#cmd LNUIListBox_AddItem $4BF
#cmd LNUIListBox_SetPrototype_OnSerialize $DF1
#cmd LNInputGesture_SetPrototype_OnSerialize $DFC
#cmd LNKeyGesture_Create $4C4
#cmd LNKeyGesture_SetPrototype_OnSerialize $E07
#cmd LNInput_IsPressed $4C7
#cmd LNInput_IsTriggered $4C9
#cmd LNInput_IsTriggeredOff $4CB
#cmd LNInput_IsRepeated $4CD
#cmd LNInput_GetAxisValue $4CF
#cmd LNInput_AddBinding $4D1
#cmd LNInput_RemoveBinding $4D4
#cmd LNInput_ClearBindings $4D6
#cmd LNInput_ClearAllBindings $4D8
#cmd LNMouse_Pressed $4DA
#cmd LNMouse_Triggered $4DC
#cmd LNMouse_TriggeredOff $4DE
#cmd LNMouse_Repeated $4E0
#cmd LNMouse_Position $4E2
#cmd LNInterpreterCommand_Code $4E4
#cmd LNInterpreterCommand_ParamsCount $4E5
#cmd LNInterpreterCommand_Param $4E6
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $E12
#cmd LNInterpreterCommandList_Create $4FD
#cmd LNInterpreterCommandList_AddCommand $4E9
#cmd LNInterpreterCommandList_AddCommand1 $4EB
#cmd LNInterpreterCommandList_AddCommand2 $4EE
#cmd LNInterpreterCommandList_AddCommand3 $4F2
#cmd LNInterpreterCommandList_AddCommand4 $4F7
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $E1D
#cmd LNInterpreterCommandDelegate_Create $961
#cmd LNInterpreter_Create $511
#cmd LNInterpreter_Clear $502
#cmd LNInterpreter_Run $503
#cmd LNInterpreter_IsRunning $505
#cmd LNInterpreter_Update $506
#cmd LNInterpreter_Terminate $507
#cmd LNInterpreter_RegisterCommandHandler $508
#cmd LNInterpreter_SetWaitMode $50B
#cmd LNInterpreter_GetWaitMode $50D
#cmd LNInterpreter_SetWaitCount $50E
#cmd LNInterpreter_GetWaitCount $510
#cmd LNInterpreter_SetPrototype_OnSerialize $E28
#cmd LNInterpreter_SetPrototype_OnUpdateWait $E34
#cmd LNEngineSettings_SetMainWindowSize $514
#cmd LNEngineSettings_SetMainWorldViewSize $517
#cmd LNEngineSettings_SetMainWindowTitle $51A
#cmd LNEngineSettings_AddAssetDirectory $51C
#cmd LNEngineSettings_AddAssetArchive $51E
#cmd LNEngineSettings_SetFrameRate $521
#cmd LNEngineSettings_SetUITheme $523
#cmd LNEngineSettings_SetFontFile $525
#cmd LNEngineSettings_SetDebugToolEnabled $527
#cmd LNEngineSettings_SetEngineLogEnabled $529
#cmd LNEngineSettings_SetEngineLogFilePath $52B
#cmd LNEngineSettings_SetDeveloperToolEnabled $52D
#cmd LNEngine_Initialize $530
#cmd LNEngine_Finalize $531
#cmd LNEngine_Update $532
#cmd LNEngine_Run $533
#cmd LNEngine_Time $535
#cmd LNEngine_GetCamera $536
#cmd LNEngine_GetMainLight $537
#cmd LNEngine_GetRenderView $538
#cmd LNApplication_Create $53D
#cmd LNApplication_OnInit $53A
#cmd LNApplication_OnUpdate $53B
#cmd LNApplication_World $53C
#cmd LNApplication_SetPrototype_OnSerialize $E3F
#cmd LNApplication_SetPrototype_OnInit $E4A
#cmd LNApplication_SetPrototype_OnUpdate $E55
#cmd LNDebug_SetGuideGridEnabled $53F
#cmd LNDebug_SetPhysicsDebugDrawEnabled $541
#cmd LNDebug_Print $543
#cmd LNDebug_PrintWithTime $545
#cmd LNDebug_PrintWithTimeAndColor $548
#cmd LNObjectSerializeHandler_Create $98D
#cmd LNEventConnectionSerializeHandler_Create $998
#cmd LNVariantSerializeHandler_Create $9A3
#cmd LNZVTestClass1SerializeHandler_Create $9AE
#cmd LNZVTestEventArgs1SerializeHandler_Create $9B9
#cmd LNSerializer2SerializeHandler_Create $9C4
#cmd LNAssetObjectSerializeHandler_Create $9CF
#cmd LNAssetImportSettingsSerializeHandler_Create $9DA
#cmd LNAssetModelSerializeHandler_Create $9E5
#cmd LNSoundSerializeHandler_Create $9F0
#cmd LNTextureSerializeHandler_Create $9FB
#cmd LNTexture2DSerializeHandler_Create $A06
#cmd LNShaderSerializeHandler_Create $A11
#cmd LNRenderViewSerializeHandler_Create $A1C
#cmd LNMaterialSerializeHandler_Create $A27
#cmd LNMeshNodeSerializeHandler_Create $A32
#cmd LNStaticMeshModelSerializeHandler_Create $A3D
#cmd LNSkinnedMeshModelSerializeHandler_Create $A48
#cmd LNAnimationControllerSerializeHandler_Create $A53
#cmd LNCollisionShapeSerializeHandler_Create $A5E
#cmd LNBoxCollisionShapeSerializeHandler_Create $A69
#cmd LNAnimationCurveSerializeHandler_Create $A74
#cmd LNKeyFrameAnimationCurveSerializeHandler_Create $A7F
#cmd LNAnimationClipSerializeHandler_Create $A8A
#cmd LNAnimationStateSerializeHandler_Create $A95
#cmd LNEffectResourceSerializeHandler_Create $AA0
#cmd LNParticleEmitterModelSerializeHandler_Create $AAB
#cmd LNParticleModelSerializeHandler_Create $AB6
#cmd LNComponentSerializeHandler_Create $AC1
#cmd LNVisualComponentSerializeHandler_Create $ACC
#cmd LNSpriteComponentSerializeHandler_Create $AD7
#cmd LNCharacterControllerSerializeHandler_Create $AE2
#cmd LNWorldSerializeHandler_Create $AED
#cmd LNComponentListSerializeHandler_Create $AF8
#cmd LNWorldObjectSerializeHandler_Create $B03
#cmd LNWorldObjectPreUpdateHandler_Create $B0E
#cmd LNWorldObjectUpdateHandler_Create $B19
#cmd LNVisualObjectSerializeHandler_Create $B24
#cmd LNVisualObjectPreUpdateHandler_Create $B2F
#cmd LNVisualObjectUpdateHandler_Create $B3A
#cmd LNCameraSerializeHandler_Create $B45
#cmd LNCameraPreUpdateHandler_Create $B50
#cmd LNCameraUpdateHandler_Create $B5B
#cmd LNEnvironmentLightSerializeHandler_Create $B66
#cmd LNEnvironmentLightPreUpdateHandler_Create $B71
#cmd LNEnvironmentLightUpdateHandler_Create $B7C
#cmd LNDirectionalLightSerializeHandler_Create $B87
#cmd LNDirectionalLightPreUpdateHandler_Create $B92
#cmd LNDirectionalLightUpdateHandler_Create $B9D
#cmd LNPointLightSerializeHandler_Create $BA8
#cmd LNPointLightPreUpdateHandler_Create $BB3
#cmd LNPointLightUpdateHandler_Create $BBE
#cmd LNSpotLightSerializeHandler_Create $BC9
#cmd LNSpotLightPreUpdateHandler_Create $BD4
#cmd LNSpotLightUpdateHandler_Create $BDF
#cmd LNSpriteSerializeHandler_Create $BEA
#cmd LNSpritePreUpdateHandler_Create $BF5
#cmd LNSpriteUpdateHandler_Create $C00
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $C0B
#cmd LNRaycasterSerializeHandler_Create $C16
#cmd LNRaycastResultSerializeHandler_Create $C21
#cmd LNWorldRenderViewSerializeHandler_Create $C2C
#cmd LNBoxMeshSerializeHandler_Create $C37
#cmd LNBoxMeshPreUpdateHandler_Create $C42
#cmd LNBoxMeshUpdateHandler_Create $C4D
#cmd LNPlaneMeshSerializeHandler_Create $C58
#cmd LNPlaneMeshPreUpdateHandler_Create $C63
#cmd LNPlaneMeshUpdateHandler_Create $C6E
#cmd LNStaticMeshSerializeHandler_Create $C79
#cmd LNStaticMeshPreUpdateHandler_Create $C84
#cmd LNStaticMeshUpdateHandler_Create $C8F
#cmd LNStaticMeshComponentSerializeHandler_Create $C9A
#cmd LNSkinnedMeshComponentSerializeHandler_Create $CA5
#cmd LNCollisionSerializeHandler_Create $CB0
#cmd LNTriggerBodyComponentSerializeHandler_Create $CBB
#cmd LNParticleEmitterSerializeHandler_Create $CC6
#cmd LNParticleEmitterPreUpdateHandler_Create $CD1
#cmd LNParticleEmitterUpdateHandler_Create $CDC
#cmd LNLevelSerializeHandler_Create $CE7
#cmd LNLevelStartHandler_Create $CF2
#cmd LNLevelStopHandler_Create $CFD
#cmd LNLevelPauseHandler_Create $D08
#cmd LNLevelResumeHandler_Create $D13
#cmd LNLevelUpdateHandler_Create $D1E
#cmd LNUIEventArgsSerializeHandler_Create $D29
#cmd LNUILayoutElementSerializeHandler_Create $D34
#cmd LNUIElementSerializeHandler_Create $D3F
#cmd LNUITextBlockSerializeHandler_Create $D4A
#cmd LNUISpriteSerializeHandler_Create $D55
#cmd LNUIIconSerializeHandler_Create $D60
#cmd LNUIMessageTextAreaSerializeHandler_Create $D6B
#cmd LNUILayoutPanelSerializeHandler_Create $D76
#cmd LNUIBoxLayoutSerializeHandler_Create $D81
#cmd LNUIStackLayoutSerializeHandler_Create $D8C
#cmd LNUIGridLayoutSerializeHandler_Create $D97
#cmd LNUIControlSerializeHandler_Create $DA2
#cmd LNUIButtonBaseSerializeHandler_Create $DAD
#cmd LNUIButtonSerializeHandler_Create $DB8
#cmd LNUIWindowSerializeHandler_Create $DC3
#cmd LNUIListItemSerializeHandler_Create $DCE
#cmd LNUIListItemsControlSerializeHandler_Create $DD9
#cmd LNUIListBoxItemSerializeHandler_Create $DE4
#cmd LNUIListBoxSerializeHandler_Create $DEF
#cmd LNInputGestureSerializeHandler_Create $DFA
#cmd LNKeyGestureSerializeHandler_Create $E05
#cmd LNInterpreterCommandSerializeHandler_Create $E10
#cmd LNInterpreterCommandListSerializeHandler_Create $E1B
#cmd LNInterpreterSerializeHandler_Create $E26
#cmd LNInterpreterUpdateWaitHandler_Create $E32
#cmd LNApplicationSerializeHandler_Create $E3D
#cmd LNApplicationInitHandler_Create $E48
#cmd LNApplicationUpdateHandler_Create $E53



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
