
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
#const global LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT 0
#const global LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT_Y 1
#const global LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION 2
#const global LN_HIERARCHICAL_ANIMATION_MODE_DISABLE_TRANSLATION 3
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
#cmd LNVector3_GetX $531
#cmd LNVector3_SetX $533
#cmd LNVector3_GetY $534
#cmd LNVector3_SetY $536
#cmd LNVector3_GetZ $537
#cmd LNVector3_SetZ $539
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $550
#cmd LNVector4_SetX $552
#cmd LNVector4_GetY $553
#cmd LNVector4_SetY $555
#cmd LNVector4_GetZ $556
#cmd LNVector4_SetZ $558
#cmd LNVector4_GetW $559
#cmd LNVector4_SetW $55B
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $56E
#cmd LNQuaternion_SetX $570
#cmd LNQuaternion_GetY $571
#cmd LNQuaternion_SetY $573
#cmd LNQuaternion_GetZ $574
#cmd LNQuaternion_SetZ $576
#cmd LNQuaternion_GetW $577
#cmd LNQuaternion_SetW $579
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $58D
#cmd LNMatrix_SetRow0 $58F
#cmd LNMatrix_GetRow1 $590
#cmd LNMatrix_SetRow1 $592
#cmd LNMatrix_GetRow2 $593
#cmd LNMatrix_SetRow2 $595
#cmd LNMatrix_GetRow3 $596
#cmd LNMatrix_SetRow3 $598
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $173
#cmd LNColor_GetR $62A
#cmd LNColor_SetR $62C
#cmd LNColor_GetG $62D
#cmd LNColor_SetG $62F
#cmd LNColor_GetB $630
#cmd LNColor_SetB $632
#cmd LNColor_GetA $633
#cmd LNColor_SetA $635
#cmd LNColor_SetZeros $178
#cmd LNColor_Set $179
#cmd LNColorTone $17E
#cmd LNColorTone_GetR $649
#cmd LNColorTone_SetR $64B
#cmd LNColorTone_GetG $64C
#cmd LNColorTone_SetG $64E
#cmd LNColorTone_GetB $64F
#cmd LNColorTone_SetB $651
#cmd LNColorTone_GetS $652
#cmd LNColorTone_SetS $654
#cmd LNColorTone_SetZeros $183
#cmd LNColorTone_Set $184
#cmd LNPoint $189
#cmd LNPoint_GetX $667
#cmd LNPoint_SetX $669
#cmd LNPoint_GetY $66A
#cmd LNPoint_SetY $66C
#cmd LNPoint_SetZeros $18C
#cmd LNPoint_Set $18D
#cmd LNSize $190
#cmd LNSize_GetWidth $677
#cmd LNSize_SetWidth $679
#cmd LNSize_GetHeight $67A
#cmd LNSize_SetHeight $67C
#cmd LNSize_SetZeros $193
#cmd LNSize_Set $194
#cmd LNRect $197
#cmd LNRect_GetX $687
#cmd LNRect_SetX $689
#cmd LNRect_GetY $68A
#cmd LNRect_SetY $68C
#cmd LNRect_GetWidth $68D
#cmd LNRect_SetWidth $68F
#cmd LNRect_GetHeight $690
#cmd LNRect_SetHeight $692
#cmd LNRect_SetZeros $19C
#cmd LNRect_Set $19D
#cmd LNRect_GetLeft $1A2
#cmd LNRect_SetSize $1A3
#cmd LNRect_GetSize $1A5
#cmd LNThickness $1A6
#cmd LNThickness_GetLeft $6AB
#cmd LNThickness_SetLeft $6AD
#cmd LNThickness_GetTop $6AE
#cmd LNThickness_SetTop $6B0
#cmd LNThickness_GetRight $6B1
#cmd LNThickness_SetRight $6B3
#cmd LNThickness_GetBottom $6B4
#cmd LNThickness_SetBottom $6B6
#cmd LNThickness_SetZeros $1AB
#cmd LNThickness_Set $1AC
#cmd LNCornerRadius $1B1
#cmd LNCornerRadius_GetTopleft $6C9
#cmd LNCornerRadius_SetTopleft $6CB
#cmd LNCornerRadius_GetTopright $6CC
#cmd LNCornerRadius_SetTopright $6CE
#cmd LNCornerRadius_GetBottomright $6CF
#cmd LNCornerRadius_SetBottomright $6D1
#cmd LNCornerRadius_GetBottomleft $6D2
#cmd LNCornerRadius_SetBottomleft $6D4
#cmd LNCornerRadius_SetZeros $1B6
#cmd LNCornerRadius_Set $1B7

#cmd LNObject_Release $7A
#cmd LNObject_Retain $7B
#cmd LNObject_GetReferenceCount $7C
#cmd LNObject_SetPrototype_OnSerialize $962
#cmd LNEventConnection_SetPrototype_OnSerialize $96D
#cmd LNPromiseFailureDelegate_Create $5B3
#cmd LNVariant_Create $81
#cmd LNVariant_SetInt $82
#cmd LNVariant_GetInt $84
#cmd LNVariant_SetPrototype_OnSerialize $978
#cmd LNZVTestDelegate1_Create $5BC
#cmd LNZVTestDelegate2_Create $5C2
#cmd LNZVTestDelegate3_Create $5C7
#cmd LNZVTestEventHandler1_Create $5CC
#cmd LNZVTestEventHandler2_Create $5D1
#cmd LNZVTestPromise1_ThenWith $5D4
#cmd LNZVTestPromise1_CatchWith $5D6
#cmd LNZVTestPromise2_ThenWith $5DA
#cmd LNZVTestPromise2_CatchWith $5DC
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $983
#cmd LNZVTestEventArgs1_Create $B0
#cmd LNZVTestEventArgs1_CreateWithValue $B1
#cmd LNZVTestEventArgs1_GetValue $AF
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $98E
#cmd LNSerializer2_SetPrototype_OnSerialize $999
#cmd LNAssetObject_SetPrototype_OnSerialize $9A4
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $9AF
#cmd LNAssetModel_Create $113
#cmd LNAssetModel_Target $112
#cmd LNAssetModel_SetPrototype_OnSerialize $9BA
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
#cmd LNSound_SetPrototype_OnSerialize $9C5
#cmd LNAudio_PlayBGM $13F
#cmd LNAudio_StopBGM $144
#cmd LNAudio_PlayBGS $146
#cmd LNAudio_StopBGS $14B
#cmd LNAudio_PlayME $14D
#cmd LNAudio_StopME $151
#cmd LNAudio_PlaySE $152
#cmd LNAudio_PlaySE3D $156
#cmd LNAudio_StopSE $15C
#cmd LNTexture2DDelegate_Create $622
#cmd LNTexture2DPromise_ThenWith $625
#cmd LNTexture2DPromise_CatchWith $627
#cmd LNTexture_SetPrototype_OnSerialize $9D0
#cmd LNTexture2D_Create $1C2
#cmd LNTexture2D_CreateWithFormat $1C5
#cmd LNTexture2D_Load $1BE
#cmd LNTexture2D_LoadEmoji $1C0
#cmd LNTexture2D_SetPrototype_OnSerialize $9DB
#cmd LNShader_Load $1CA
#cmd LNShader_SetFloat $1CD
#cmd LNShader_SetVector3 $1D0
#cmd LNShader_SetVector4 $1D3
#cmd LNShader_SetPrototype_OnSerialize $9E6
#cmd LNRenderView_SetPrototype_OnSerialize $9F1
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
#cmd LNMaterial_SetPrototype_OnSerialize $9FC
#cmd LNMeshNode_SetVisible $1FA
#cmd LNMeshNode_IsVisible $1FC
#cmd LNMeshNode_SetPrototype_OnSerialize $A07
#cmd LNStaticMeshModel_Load $1FE
#cmd LNStaticMeshModel_FindNode $200
#cmd LNStaticMeshModel_FindMaterial $202
#cmd LNStaticMeshModel_MaterialCount $204
#cmd LNStaticMeshModel_Material $205
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $A12
#cmd LNSkinnedMeshModel_Load $208
#cmd LNSkinnedMeshModel_GetAnimationController $20A
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $A1D
#cmd LNAnimationController_AddClip $20C
#cmd LNAnimationController_Play $20E
#cmd LNAnimationController_SetPrototype_OnSerialize $A28
#cmd LNCollisionShape_SetPrototype_OnSerialize $A33
#cmd LNBoxCollisionShape_Create $213
#cmd LNBoxCollisionShape_CreateWHD $215
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $A3E
#cmd LNAnimationClip_Load $21F
#cmd LNAnimationClip_SetHierarchicalAnimationMode $221
#cmd LNAnimationClip_HierarchicalAnimationMode $223
#cmd LNAnimationClip_SetPrototype_OnSerialize $A49
#cmd LNAnimationState_SetPrototype_OnSerialize $A54
#cmd LNEffectResource_SetPrototype_OnSerialize $A5F
#cmd LNParticleEmitterModel_Create $248
#cmd LNParticleEmitterModel_SetMaxParticles $230
#cmd LNParticleEmitterModel_SetSpawnRate $232
#cmd LNParticleEmitterModel_SetLifeTime $234
#cmd LNParticleEmitterModel_SetupBoxShape $236
#cmd LNParticleEmitterModel_SetSize $238
#cmd LNParticleEmitterModel_SetSizeVelocity $23A
#cmd LNParticleEmitterModel_SetSizeAcceleration $23C
#cmd LNParticleEmitterModel_SetForwardVelocityMin $23E
#cmd LNParticleEmitterModel_SetForwardVelocityMax $240
#cmd LNParticleEmitterModel_SetForwardScale $242
#cmd LNParticleEmitterModel_SetGeometryDirection $244
#cmd LNParticleEmitterModel_SetupSpriteModule $246
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $A6A
#cmd LNParticleModel_Create $24F
#cmd LNParticleModel_SetLoop $24A
#cmd LNParticleModel_IsLoop $24C
#cmd LNParticleModel_AddEmitter $24D
#cmd LNParticleModel_SetPrototype_OnSerialize $A75
#cmd LNComponent_SetPrototype_OnSerialize $A80
#cmd LNVisualComponent_SetVisible $252
#cmd LNVisualComponent_IsVisible $254
#cmd LNVisualComponent_SetPrototype_OnSerialize $A8B
#cmd LNSpriteComponent_SetTexture $256
#cmd LNSpriteComponent_SetPrototype_OnSerialize $A96
#cmd LNCollisionEventHandler_Create $749
#cmd LNCharacterController_Create $276
#cmd LNCharacterController_SetWalkVelocity $260
#cmd LNCharacterController_GetWalkVelocity $262
#cmd LNCharacterController_SetVelocity $263
#cmd LNCharacterController_GetVelocity $265
#cmd LNCharacterController_SetInputControlEnabled $266
#cmd LNCharacterController_SetCameraControlEnabled $268
#cmd LNCharacterController_SetHeight $26A
#cmd LNCharacterController_GetHeight $26C
#cmd LNCharacterController_SetCameraRadius $26D
#cmd LNCharacterController_GetCameraRadius $26F
#cmd LNCharacterController_SetCollisionEnter $270
#cmd LNCharacterController_SetCollisionLeave $272
#cmd LNCharacterController_SetCollisionStay $274
#cmd LNCharacterController_SetPrototype_OnSerialize $AA1
#cmd LNWorld_Add $278
#cmd LNWorld_SetPrototype_OnSerialize $AAC
#cmd LNComponentList_GetLength $769
#cmd LNComponentList_GetItem $76B
#cmd LNComponentList_SetPrototype_OnSerialize $AB7
#cmd LNWorldObject_Create $2AF
#cmd LNWorldObject_SetPosition $27C
#cmd LNWorldObject_SetPositionXYZ $27E
#cmd LNWorldObject_GetPosition $282
#cmd LNWorldObject_SetRotationQuaternion $283
#cmd LNWorldObject_SetRotation $285
#cmd LNWorldObject_GetRotation $289
#cmd LNWorldObject_SetScale $28A
#cmd LNWorldObject_SetScaleS $28C
#cmd LNWorldObject_SetScaleXYZ $28E
#cmd LNWorldObject_GetScale $292
#cmd LNWorldObject_SetCenterPoint $293
#cmd LNWorldObject_SetCenterPointXYZ $295
#cmd LNWorldObject_GetCenterPoint $299
#cmd LNWorldObject_LookAt $29A
#cmd LNWorldObject_LookAtXYZ $29C
#cmd LNWorldObject_AddComponent $2A0
#cmd LNWorldObject_RemoveComponent $2A2
#cmd LNWorldObject_AddTag $2A4
#cmd LNWorldObject_RemoveTag $2A6
#cmd LNWorldObject_HasTag $2A8
#cmd LNWorldObject_Destroy $2AA
#cmd LNWorldObject_GetComponents $2AB
#cmd LNWorldObject_SetPrototype_OnSerialize $AC2
#cmd LNWorldObject_SetPrototype_OnPreUpdate $ACD
#cmd LNWorldObject_SetPrototype_OnUpdate $AD8
#cmd LNVisualObject_SetVisible $2B1
#cmd LNVisualObject_IsVisible $2B3
#cmd LNVisualObject_SetBlendMode2 $2B4
#cmd LNVisualObject_SetOpacity $2B6
#cmd LNVisualObject_GetOpacity $2B8
#cmd LNVisualObject_SetPrototype_OnSerialize $AE3
#cmd LNVisualObject_SetPrototype_OnPreUpdate $AEE
#cmd LNVisualObject_SetPrototype_OnUpdate $AF9
#cmd LNCamera_SetPrototype_OnSerialize $B04
#cmd LNCamera_SetPrototype_OnPreUpdate $B0F
#cmd LNCamera_SetPrototype_OnUpdate $B1A
#cmd LNEnvironmentLight_SetEnabled $2BB
#cmd LNEnvironmentLight_IsEnabled $2BD
#cmd LNEnvironmentLight_SetColor $2BE
#cmd LNEnvironmentLight_GetColor $2C0
#cmd LNEnvironmentLight_SetAmbientColor $2C1
#cmd LNEnvironmentLight_GetAmbientColor $2C3
#cmd LNEnvironmentLight_GetSkyColor $2C4
#cmd LNEnvironmentLight_SetSkyColor $2C5
#cmd LNEnvironmentLight_GetGroundColor $2C7
#cmd LNEnvironmentLight_SetGroundColor $2C8
#cmd LNEnvironmentLight_SetIntensity $2CA
#cmd LNEnvironmentLight_GetIntensity $2CC
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $2CD
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $2CF
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $2D0
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $2D2
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $B25
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $B30
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $B3B
#cmd LNDirectionalLight_Create $2E3
#cmd LNDirectionalLight_CreateWithColor $2E4
#cmd LNDirectionalLight_SetEnabled $2D4
#cmd LNDirectionalLight_IsEnabled $2D6
#cmd LNDirectionalLight_SetColor $2D7
#cmd LNDirectionalLight_GetColor $2D9
#cmd LNDirectionalLight_SetIntensity $2DA
#cmd LNDirectionalLight_GetIntensity $2DC
#cmd LNDirectionalLight_SetShadowEffectiveDistance $2DD
#cmd LNDirectionalLight_GetShadowEffectiveDistance $2DF
#cmd LNDirectionalLight_SetShadowEffectiveDepth $2E0
#cmd LNDirectionalLight_GetShadowEffectiveDepth $2E2
#cmd LNDirectionalLight_SetPrototype_OnSerialize $B46
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $B51
#cmd LNDirectionalLight_SetPrototype_OnUpdate $B5C
#cmd LNPointLight_Create $2F6
#cmd LNPointLight_CreateWithColorAndRange $2F7
#cmd LNPointLight_SetEnabled $2E7
#cmd LNPointLight_IsEnabled $2E9
#cmd LNPointLight_SetColor $2EA
#cmd LNPointLight_GetColor $2EC
#cmd LNPointLight_SetIntensity $2ED
#cmd LNPointLight_GetIntensity $2EF
#cmd LNPointLight_SetRange $2F0
#cmd LNPointLight_GetRange $2F2
#cmd LNPointLight_SetAttenuation $2F3
#cmd LNPointLight_GetAttenuation $2F5
#cmd LNPointLight_SetPrototype_OnSerialize $B67
#cmd LNPointLight_SetPrototype_OnPreUpdate $B72
#cmd LNPointLight_SetPrototype_OnUpdate $B7D
#cmd LNSpotLight_Create $310
#cmd LNSpotLight_CreateWithColorAndRange $311
#cmd LNSpotLight_SetEnabled $2FB
#cmd LNSpotLight_IsEnabled $2FD
#cmd LNSpotLight_SetColor $2FE
#cmd LNSpotLight_GetColor $300
#cmd LNSpotLight_SetIntensity $301
#cmd LNSpotLight_GetIntensity $303
#cmd LNSpotLight_SetRange $304
#cmd LNSpotLight_GetRange $306
#cmd LNSpotLight_SetAttenuation $307
#cmd LNSpotLight_GetAttenuation $309
#cmd LNSpotLight_SetAngle $30A
#cmd LNSpotLight_GetAngle $30C
#cmd LNSpotLight_SetPenumbra $30D
#cmd LNSpotLight_GetPenumbra $30F
#cmd LNSpotLight_SetPrototype_OnSerialize $B88
#cmd LNSpotLight_SetPrototype_OnPreUpdate $B93
#cmd LNSpotLight_SetPrototype_OnUpdate $B9E
#cmd LNTestDelegate_Create $817
#cmd LNSprite_Create $327
#cmd LNSprite_CreateWithTexture $328
#cmd LNSprite_CreateWithTextureAndSize $32A
#cmd LNSprite_SetTexture $319
#cmd LNSprite_SetSize $31B
#cmd LNSprite_SetSizeWH $31D
#cmd LNSprite_SetSourceRectXYWH $320
#cmd LNSprite_SetCallerTest $325
#cmd LNSprite_SetPrototype_OnSerialize $BA9
#cmd LNSprite_SetPrototype_OnPreUpdate $BB4
#cmd LNSprite_SetPrototype_OnUpdate $BBF
#cmd LNCameraOrbitControlComponent_Create $32F
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $BCA
#cmd LNRaycaster_FromScreen $331
#cmd LNRaycaster_IntersectPlane $333
#cmd LNRaycaster_SetPrototype_OnSerialize $BD5
#cmd LNRaycastResult_GetPoint $338
#cmd LNRaycastResult_SetPrototype_OnSerialize $BE0
#cmd LNWorldRenderView_SetGuideGridEnabled $33A
#cmd LNWorldRenderView_GetGuideGridEnabled $33C
#cmd LNWorldRenderView_SetPrototype_OnSerialize $BEB
#cmd LNBoxMesh_Create $33E
#cmd LNBoxMesh_SetPrototype_OnSerialize $BF6
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $C01
#cmd LNBoxMesh_SetPrototype_OnUpdate $C0C
#cmd LNPlaneMesh_Create $340
#cmd LNPlaneMesh_SetPrototype_OnSerialize $C17
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $C22
#cmd LNPlaneMesh_SetPrototype_OnUpdate $C2D
#cmd LNStaticMesh_Load $342
#cmd LNStaticMesh_GetModel $344
#cmd LNStaticMesh_MakeCollisionBody $345
#cmd LNStaticMesh_SetPrototype_OnSerialize $C38
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $C43
#cmd LNStaticMesh_SetPrototype_OnUpdate $C4E
#cmd LNStaticMeshComponent_Create $34C
#cmd LNStaticMeshComponent_SetModel $348
#cmd LNStaticMeshComponent_MakeCollisionBody $34A
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $C59
#cmd LNSkinnedMeshComponent_Create $350
#cmd LNSkinnedMeshComponent_SetModel $34E
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $C64
#cmd LNCollision_GetWorldObject $352
#cmd LNCollision_SetPrototype_OnSerialize $C6F
#cmd LNTriggerBodyComponent_Create $356
#cmd LNTriggerBodyComponent_AddCollisionShape $354
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $C7A
#cmd LNParticleEmitter_Create $358
#cmd LNParticleEmitter_SetPrototype_OnSerialize $C85
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $C90
#cmd LNParticleEmitter_SetPrototype_OnUpdate $C9B
#cmd LNScene_SetClearMode $35B
#cmd LNScene_SetSkyColor $35D
#cmd LNScene_SetSkyHorizonColor $35F
#cmd LNScene_SetSkyCloudColor $361
#cmd LNScene_SetSkyOverlayColor $363
#cmd LNScene_GotoLevel $365
#cmd LNScene_CallLevel $368
#cmd LNScene_ReturnLevel $36B
#cmd LNScene_ActiveLevel $36D
#cmd LNScene_IsTransitionEffectRunning $36E
#cmd LNScene_SetTransitionEffectMode $36F
#cmd LNScene_TransitionEffectMode $371
#cmd LNScene_SetTransitionDuration $372
#cmd LNScene_TransitionDuration $374
#cmd LNScene_SetTransitionEffectColor $375
#cmd LNScene_TransitionEffectColor $377
#cmd LNScene_SetTransitionEffectMaskTexture $378
#cmd LNScene_TransitionEffectMaskTexture $37A
#cmd LNScene_SetTransitionEffectVague $37B
#cmd LNScene_TransitionEffectVague $37D
#cmd LNScene_StartFadeOut $37E
#cmd LNScene_StartFadeIn $37F
#cmd LNScene_SetFogStartDistance $380
#cmd LNScene_SetFogColor $382
#cmd LNScene_SetFogDensity $384
#cmd LNScene_SetFogHeightDensity $386
#cmd LNScene_SetFogLowerHeight $388
#cmd LNScene_SetFogUpperHeight $38A
#cmd LNScene_SetHDREnabled $38C
#cmd LNScene_IsHDREnabled $38E
#cmd LNScene_SetScreenBlendColor $38F
#cmd LNScene_ScreenBlendColor $391
#cmd LNScene_SetColorTone $392
#cmd LNScene_ColorTone $394
#cmd LNScene_SetAntialiasEnabled $395
#cmd LNScene_IsAntialiasEnabled $397
#cmd LNScene_SetSSREnabled $398
#cmd LNScene_IsSSREnabled $39A
#cmd LNScene_SetSSAOEnabled $39B
#cmd LNScene_IsSSAOEnabled $39D
#cmd LNScene_SetBloomEnabled $39E
#cmd LNScene_IsBloomEnabled $3A0
#cmd LNScene_SetDOFEnabled $3A1
#cmd LNScene_IsDOFEnabled $3A3
#cmd LNScene_SetTonemapEnabled $3A4
#cmd LNScene_IsTonemapEnabled $3A6
#cmd LNScene_SetVignetteEnabled $3A7
#cmd LNScene_IsVignetteEnabled $3A9
#cmd LNScene_SetGammaEnabled $3AA
#cmd LNScene_IsGammaEnabled $3AC
#cmd LNScene_SetTonemapExposure $3AD
#cmd LNScene_SetTonemapLinearWhite $3AF
#cmd LNScene_SetTonemapShoulderStrength $3B1
#cmd LNScene_SetTonemapLinearStrength $3B3
#cmd LNScene_SetTonemapLinearAngle $3B5
#cmd LNScene_SetTonemapToeStrength $3B7
#cmd LNScene_SetTonemapToeNumerator $3B9
#cmd LNScene_SetTonemapToeDenominator $3BB
#cmd LNLevel_Create $3CC
#cmd LNLevel_AddObject $3BE
#cmd LNLevel_RemoveObject $3C0
#cmd LNLevel_AddSubLevel $3C2
#cmd LNLevel_RemoveSubLevel $3C4
#cmd LNLevel_RemoveAllSubLevels $3C6
#cmd LNLevel_SetPrototype_OnSerialize $CA6
#cmd LNLevel_SetPrototype_OnStart $CB1
#cmd LNLevel_SetPrototype_OnStop $CBC
#cmd LNLevel_SetPrototype_OnPause $CC7
#cmd LNLevel_SetPrototype_OnResume $CD2
#cmd LNLevel_SetPrototype_OnUpdate $CDD
#cmd LNUIEventArgs_Sender $3D7
#cmd LNUIEventArgs_SetPrototype_OnSerialize $CE8
#cmd LNUIGeneralEventHandler_Create $876
#cmd LNUIEventHandler_Create $87B
#cmd LNUILayoutElement_SetPrototype_OnSerialize $CF3
#cmd LNUIElement_SetSize $3E9
#cmd LNUIElement_SetSizeWH $3EB
#cmd LNUIElement_SetWidth $3EE
#cmd LNUIElement_GetWidth $3F0
#cmd LNUIElement_SetHeight $3F1
#cmd LNUIElement_GetHeight $3F3
#cmd LNUIElement_SetMargin $3F4
#cmd LNUIElement_GetMargin $3F6
#cmd LNUIElement_SetPadding $3F7
#cmd LNUIElement_GetPadding $3F9
#cmd LNUIElement_SetHAlignment $3FA
#cmd LNUIElement_GetHAlignment $3FC
#cmd LNUIElement_SetVAlignment $3FD
#cmd LNUIElement_GetVAlignment $3FF
#cmd LNUIElement_SetAlignments $400
#cmd LNUIElement_SetPosition $403
#cmd LNUIElement_SetPositionXYZ $405
#cmd LNUIElement_GetPosition $409
#cmd LNUIElement_SetRotation $40A
#cmd LNUIElement_SetEulerAngles $40C
#cmd LNUIElement_GetRotation $410
#cmd LNUIElement_SetScale $411
#cmd LNUIElement_SetScaleS $413
#cmd LNUIElement_SetScaleXY $415
#cmd LNUIElement_GetScale $418
#cmd LNUIElement_SetCenterPoint $419
#cmd LNUIElement_SetCenterPointXYZ $41B
#cmd LNUIElement_GetCenterPoint $41F
#cmd LNUIElement_SetEnabled $420
#cmd LNUIElement_IsEnabled $422
#cmd LNUIElement_SetData $423
#cmd LNUIElement_GetData $425
#cmd LNUIElement_SetBackgroundColor $426
#cmd LNUIElement_GetBackgroundColor $428
#cmd LNUIElement_SetBorderThickness $429
#cmd LNUIElement_GetBorderThickness $42B
#cmd LNUIElement_SetBorderColor $42C
#cmd LNUIElement_GetBorderColor $42E
#cmd LNUIElement_SetCornerRadius $42F
#cmd LNUIElement_GetCornerRadius $431
#cmd LNUIElement_SetVisibility $432
#cmd LNUIElement_GetVisibility $434
#cmd LNUIElement_SetOpacity $435
#cmd LNUIElement_GetOpacity $437
#cmd LNUIElement_AddChild $438
#cmd LNUIElement_SetPrototype_OnSerialize $CFE
#cmd LNUITextBlock_Create $43E
#cmd LNUITextBlock_CreateWithText $43F
#cmd LNUITextBlock_SetText $43B
#cmd LNUITextBlock_GetText $43D
#cmd LNUITextBlock_SetPrototype_OnSerialize $D09
#cmd LNUISprite_Create $44C
#cmd LNUISprite_CreateWithTexture $44D
#cmd LNUISprite_SetTexture $442
#cmd LNUISprite_SetSourceRect $444
#cmd LNUISprite_SetSourceRectXYWH $446
#cmd LNUISprite_GetSourceRect $44B
#cmd LNUISprite_SetPrototype_OnSerialize $D14
#cmd LNUIIcon_LoadFontIcon $450
#cmd LNUIIcon_LoadFontIconWithNameSize $452
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $455
#cmd LNUIIcon_SetPrototype_OnSerialize $D1F
#cmd LNUIMessageTextArea_Create $45E
#cmd LNUIMessageTextArea_SetText $45A
#cmd LNUIMessageTextArea_SetTypingSpeed $45C
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $D2A
#cmd LNUI_Add $460
#cmd LNUI_Remove $462
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $D35
#cmd LNUIBoxLayout_Create $469
#cmd LNUIBoxLayout_SetOrientation $466
#cmd LNUIBoxLayout_GetOrientation $468
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $D40
#cmd LNUIGridLayout_Create $479
#cmd LNUIGridLayout_SetColumnCount $46B
#cmd LNUIGridLayout_SetRow $46D
#cmd LNUIGridLayout_SetColumn $470
#cmd LNUIGridLayout_SetPlacement $473
#cmd LNUIGridLayout_SetPrototype_OnSerialize $D4B
#cmd LNUIControl_Create $485
#cmd LNUIControl_AddInlineVisual $486
#cmd LNUIControl_SetPrototype_OnSerialize $D56
#cmd LNUIButtonBase_SetText $48A
#cmd LNUIButtonBase_SetPrototype_OnSerialize $D61
#cmd LNUIButton_Create $48D
#cmd LNUIButton_CreateWithText $48E
#cmd LNUIButton_ConnectOnClicked $490
#cmd LNUIButton_SetPrototype_OnSerialize $D6C
#cmd LNUIWindow_Create $493
#cmd LNUIWindow_SetPrototype_OnSerialize $D77
#cmd LNUIListItem_ConnectOnSubmit $498
#cmd LNUIListItem_SetPrototype_OnSerialize $D82
#cmd LNUIListItemsControl_SetItemsLayoutPanel $49B
#cmd LNUIListItemsControl_SetSubmitMode $49D
#cmd LNUIListItemsControl_GetSubmitMode $49F
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $D8D
#cmd LNUIListBoxItem_Create $4A1
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $D98
#cmd LNUIListBox_Create $4A6
#cmd LNUIListBox_AddItem $4A4
#cmd LNUIListBox_SetPrototype_OnSerialize $DA3
#cmd LNInputGesture_SetPrototype_OnSerialize $DAE
#cmd LNKeyGesture_Create $4A9
#cmd LNKeyGesture_SetPrototype_OnSerialize $DB9
#cmd LNInput_IsPressed $4AC
#cmd LNInput_IsTriggered $4AE
#cmd LNInput_IsTriggeredOff $4B0
#cmd LNInput_IsRepeated $4B2
#cmd LNInput_GetAxisValue $4B4
#cmd LNInput_AddBinding $4B6
#cmd LNInput_RemoveBinding $4B9
#cmd LNInput_ClearBindings $4BB
#cmd LNInput_ClearAllBindings $4BD
#cmd LNMouse_Pressed $4BF
#cmd LNMouse_Triggered $4C1
#cmd LNMouse_TriggeredOff $4C3
#cmd LNMouse_Repeated $4C5
#cmd LNMouse_Position $4C7
#cmd LNInterpreterCommand_Code $4C9
#cmd LNInterpreterCommand_ParamsCount $4CA
#cmd LNInterpreterCommand_Param $4CB
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $DC4
#cmd LNInterpreterCommandList_Create $4E2
#cmd LNInterpreterCommandList_AddCommand $4CE
#cmd LNInterpreterCommandList_AddCommand1 $4D0
#cmd LNInterpreterCommandList_AddCommand2 $4D3
#cmd LNInterpreterCommandList_AddCommand3 $4D7
#cmd LNInterpreterCommandList_AddCommand4 $4DC
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $DCF
#cmd LNInterpreterCommandDelegate_Create $934
#cmd LNInterpreter_Create $4F6
#cmd LNInterpreter_Clear $4E7
#cmd LNInterpreter_Run $4E8
#cmd LNInterpreter_IsRunning $4EA
#cmd LNInterpreter_Update $4EB
#cmd LNInterpreter_Terminate $4EC
#cmd LNInterpreter_RegisterCommandHandler $4ED
#cmd LNInterpreter_SetWaitMode $4F0
#cmd LNInterpreter_GetWaitMode $4F2
#cmd LNInterpreter_SetWaitCount $4F3
#cmd LNInterpreter_GetWaitCount $4F5
#cmd LNInterpreter_SetPrototype_OnSerialize $DDA
#cmd LNInterpreter_SetPrototype_OnUpdateWait $DE6
#cmd LNEngineSettings_SetMainWindowSize $4F9
#cmd LNEngineSettings_SetMainWorldViewSize $4FC
#cmd LNEngineSettings_SetMainWindowTitle $4FF
#cmd LNEngineSettings_AddAssetDirectory $501
#cmd LNEngineSettings_AddAssetArchive $503
#cmd LNEngineSettings_SetFrameRate $506
#cmd LNEngineSettings_SetUITheme $508
#cmd LNEngineSettings_SetFontFile $50A
#cmd LNEngineSettings_SetDebugToolEnabled $50C
#cmd LNEngineSettings_SetEngineLogEnabled $50E
#cmd LNEngineSettings_SetEngineLogFilePath $510
#cmd LNEngineSettings_SetDeveloperToolEnabled $512
#cmd LNEngine_Initialize $515
#cmd LNEngine_Finalize $516
#cmd LNEngine_Update $517
#cmd LNEngine_Run $518
#cmd LNEngine_Time $51A
#cmd LNEngine_GetCamera $51B
#cmd LNEngine_GetMainLight $51C
#cmd LNEngine_GetRenderView $51D
#cmd LNApplication_Create $522
#cmd LNApplication_OnInit $51F
#cmd LNApplication_OnUpdate $520
#cmd LNApplication_World $521
#cmd LNApplication_SetPrototype_OnSerialize $DF1
#cmd LNApplication_SetPrototype_OnInit $DFC
#cmd LNApplication_SetPrototype_OnUpdate $E07
#cmd LNDebug_SetGuideGridEnabled $524
#cmd LNDebug_SetPhysicsDebugDrawEnabled $526
#cmd LNDebug_Print $528
#cmd LNDebug_PrintWithTime $52A
#cmd LNDebug_PrintWithTimeAndColor $52D
#cmd LNObjectSerializeHandler_Create $960
#cmd LNEventConnectionSerializeHandler_Create $96B
#cmd LNVariantSerializeHandler_Create $976
#cmd LNZVTestClass1SerializeHandler_Create $981
#cmd LNZVTestEventArgs1SerializeHandler_Create $98C
#cmd LNSerializer2SerializeHandler_Create $997
#cmd LNAssetObjectSerializeHandler_Create $9A2
#cmd LNAssetImportSettingsSerializeHandler_Create $9AD
#cmd LNAssetModelSerializeHandler_Create $9B8
#cmd LNSoundSerializeHandler_Create $9C3
#cmd LNTextureSerializeHandler_Create $9CE
#cmd LNTexture2DSerializeHandler_Create $9D9
#cmd LNShaderSerializeHandler_Create $9E4
#cmd LNRenderViewSerializeHandler_Create $9EF
#cmd LNMaterialSerializeHandler_Create $9FA
#cmd LNMeshNodeSerializeHandler_Create $A05
#cmd LNStaticMeshModelSerializeHandler_Create $A10
#cmd LNSkinnedMeshModelSerializeHandler_Create $A1B
#cmd LNAnimationControllerSerializeHandler_Create $A26
#cmd LNCollisionShapeSerializeHandler_Create $A31
#cmd LNBoxCollisionShapeSerializeHandler_Create $A3C
#cmd LNAnimationClipSerializeHandler_Create $A47
#cmd LNAnimationStateSerializeHandler_Create $A52
#cmd LNEffectResourceSerializeHandler_Create $A5D
#cmd LNParticleEmitterModelSerializeHandler_Create $A68
#cmd LNParticleModelSerializeHandler_Create $A73
#cmd LNComponentSerializeHandler_Create $A7E
#cmd LNVisualComponentSerializeHandler_Create $A89
#cmd LNSpriteComponentSerializeHandler_Create $A94
#cmd LNCharacterControllerSerializeHandler_Create $A9F
#cmd LNWorldSerializeHandler_Create $AAA
#cmd LNComponentListSerializeHandler_Create $AB5
#cmd LNWorldObjectSerializeHandler_Create $AC0
#cmd LNWorldObjectPreUpdateHandler_Create $ACB
#cmd LNWorldObjectUpdateHandler_Create $AD6
#cmd LNVisualObjectSerializeHandler_Create $AE1
#cmd LNVisualObjectPreUpdateHandler_Create $AEC
#cmd LNVisualObjectUpdateHandler_Create $AF7
#cmd LNCameraSerializeHandler_Create $B02
#cmd LNCameraPreUpdateHandler_Create $B0D
#cmd LNCameraUpdateHandler_Create $B18
#cmd LNEnvironmentLightSerializeHandler_Create $B23
#cmd LNEnvironmentLightPreUpdateHandler_Create $B2E
#cmd LNEnvironmentLightUpdateHandler_Create $B39
#cmd LNDirectionalLightSerializeHandler_Create $B44
#cmd LNDirectionalLightPreUpdateHandler_Create $B4F
#cmd LNDirectionalLightUpdateHandler_Create $B5A
#cmd LNPointLightSerializeHandler_Create $B65
#cmd LNPointLightPreUpdateHandler_Create $B70
#cmd LNPointLightUpdateHandler_Create $B7B
#cmd LNSpotLightSerializeHandler_Create $B86
#cmd LNSpotLightPreUpdateHandler_Create $B91
#cmd LNSpotLightUpdateHandler_Create $B9C
#cmd LNSpriteSerializeHandler_Create $BA7
#cmd LNSpritePreUpdateHandler_Create $BB2
#cmd LNSpriteUpdateHandler_Create $BBD
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $BC8
#cmd LNRaycasterSerializeHandler_Create $BD3
#cmd LNRaycastResultSerializeHandler_Create $BDE
#cmd LNWorldRenderViewSerializeHandler_Create $BE9
#cmd LNBoxMeshSerializeHandler_Create $BF4
#cmd LNBoxMeshPreUpdateHandler_Create $BFF
#cmd LNBoxMeshUpdateHandler_Create $C0A
#cmd LNPlaneMeshSerializeHandler_Create $C15
#cmd LNPlaneMeshPreUpdateHandler_Create $C20
#cmd LNPlaneMeshUpdateHandler_Create $C2B
#cmd LNStaticMeshSerializeHandler_Create $C36
#cmd LNStaticMeshPreUpdateHandler_Create $C41
#cmd LNStaticMeshUpdateHandler_Create $C4C
#cmd LNStaticMeshComponentSerializeHandler_Create $C57
#cmd LNSkinnedMeshComponentSerializeHandler_Create $C62
#cmd LNCollisionSerializeHandler_Create $C6D
#cmd LNTriggerBodyComponentSerializeHandler_Create $C78
#cmd LNParticleEmitterSerializeHandler_Create $C83
#cmd LNParticleEmitterPreUpdateHandler_Create $C8E
#cmd LNParticleEmitterUpdateHandler_Create $C99
#cmd LNLevelSerializeHandler_Create $CA4
#cmd LNLevelStartHandler_Create $CAF
#cmd LNLevelStopHandler_Create $CBA
#cmd LNLevelPauseHandler_Create $CC5
#cmd LNLevelResumeHandler_Create $CD0
#cmd LNLevelUpdateHandler_Create $CDB
#cmd LNUIEventArgsSerializeHandler_Create $CE6
#cmd LNUILayoutElementSerializeHandler_Create $CF1
#cmd LNUIElementSerializeHandler_Create $CFC
#cmd LNUITextBlockSerializeHandler_Create $D07
#cmd LNUISpriteSerializeHandler_Create $D12
#cmd LNUIIconSerializeHandler_Create $D1D
#cmd LNUIMessageTextAreaSerializeHandler_Create $D28
#cmd LNUILayoutPanelSerializeHandler_Create $D33
#cmd LNUIBoxLayoutSerializeHandler_Create $D3E
#cmd LNUIGridLayoutSerializeHandler_Create $D49
#cmd LNUIControlSerializeHandler_Create $D54
#cmd LNUIButtonBaseSerializeHandler_Create $D5F
#cmd LNUIButtonSerializeHandler_Create $D6A
#cmd LNUIWindowSerializeHandler_Create $D75
#cmd LNUIListItemSerializeHandler_Create $D80
#cmd LNUIListItemsControlSerializeHandler_Create $D8B
#cmd LNUIListBoxItemSerializeHandler_Create $D96
#cmd LNUIListBoxSerializeHandler_Create $DA1
#cmd LNInputGestureSerializeHandler_Create $DAC
#cmd LNKeyGestureSerializeHandler_Create $DB7
#cmd LNInterpreterCommandSerializeHandler_Create $DC2
#cmd LNInterpreterCommandListSerializeHandler_Create $DCD
#cmd LNInterpreterSerializeHandler_Create $DD8
#cmd LNInterpreterUpdateWaitHandler_Create $DE4
#cmd LNApplicationSerializeHandler_Create $DEF
#cmd LNApplicationInitHandler_Create $DFA
#cmd LNApplicationUpdateHandler_Create $E05



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
