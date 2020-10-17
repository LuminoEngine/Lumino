
#ifndef __lumino__
#define __lumino__

#ifdef LUMINO_DEBUG
    #regcmd "_hsp3cmdinit@4","LuminoHSPd.dll", 10
#else
    #regcmd "_hsp3cmdinit@4","LuminoHSP.dll", 10
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
#cmd LNVector3_GetX $517
#cmd LNVector3_SetX $519
#cmd LNVector3_GetY $51A
#cmd LNVector3_SetY $51C
#cmd LNVector3_GetZ $51D
#cmd LNVector3_SetZ $51F
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $536
#cmd LNVector4_SetX $538
#cmd LNVector4_GetY $539
#cmd LNVector4_SetY $53B
#cmd LNVector4_GetZ $53C
#cmd LNVector4_SetZ $53E
#cmd LNVector4_GetW $53F
#cmd LNVector4_SetW $541
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $554
#cmd LNQuaternion_SetX $556
#cmd LNQuaternion_GetY $557
#cmd LNQuaternion_SetY $559
#cmd LNQuaternion_GetZ $55A
#cmd LNQuaternion_SetZ $55C
#cmd LNQuaternion_GetW $55D
#cmd LNQuaternion_SetW $55F
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $573
#cmd LNMatrix_SetRow0 $575
#cmd LNMatrix_GetRow1 $576
#cmd LNMatrix_SetRow1 $578
#cmd LNMatrix_GetRow2 $579
#cmd LNMatrix_SetRow2 $57B
#cmd LNMatrix_GetRow3 $57C
#cmd LNMatrix_SetRow3 $57E
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $173
#cmd LNColor_GetR $610
#cmd LNColor_SetR $612
#cmd LNColor_GetG $613
#cmd LNColor_SetG $615
#cmd LNColor_GetB $616
#cmd LNColor_SetB $618
#cmd LNColor_GetA $619
#cmd LNColor_SetA $61B
#cmd LNColor_SetZeros $178
#cmd LNColor_Set $179
#cmd LNPoint $17E
#cmd LNPoint_GetX $62F
#cmd LNPoint_SetX $631
#cmd LNPoint_GetY $632
#cmd LNPoint_SetY $634
#cmd LNPoint_SetZeros $181
#cmd LNPoint_Set $182
#cmd LNSize $185
#cmd LNSize_GetWidth $63F
#cmd LNSize_SetWidth $641
#cmd LNSize_GetHeight $642
#cmd LNSize_SetHeight $644
#cmd LNSize_SetZeros $188
#cmd LNSize_Set $189
#cmd LNRect $18C
#cmd LNRect_GetX $64F
#cmd LNRect_SetX $651
#cmd LNRect_GetY $652
#cmd LNRect_SetY $654
#cmd LNRect_GetWidth $655
#cmd LNRect_SetWidth $657
#cmd LNRect_GetHeight $658
#cmd LNRect_SetHeight $65A
#cmd LNRect_SetZeros $191
#cmd LNRect_Set $192
#cmd LNRect_GetLeft $197
#cmd LNRect_SetSize $198
#cmd LNRect_GetSize $19A
#cmd LNThickness $19B
#cmd LNThickness_GetLeft $673
#cmd LNThickness_SetLeft $675
#cmd LNThickness_GetTop $676
#cmd LNThickness_SetTop $678
#cmd LNThickness_GetRight $679
#cmd LNThickness_SetRight $67B
#cmd LNThickness_GetBottom $67C
#cmd LNThickness_SetBottom $67E
#cmd LNThickness_SetZeros $1A0
#cmd LNThickness_Set $1A1
#cmd LNCornerRadius $1A6
#cmd LNCornerRadius_GetTopleft $691
#cmd LNCornerRadius_SetTopleft $693
#cmd LNCornerRadius_GetTopright $694
#cmd LNCornerRadius_SetTopright $696
#cmd LNCornerRadius_GetBottomright $697
#cmd LNCornerRadius_SetBottomright $699
#cmd LNCornerRadius_GetBottomleft $69A
#cmd LNCornerRadius_SetBottomleft $69C
#cmd LNCornerRadius_SetZeros $1AB
#cmd LNCornerRadius_Set $1AC

#cmd LNObject_Release $7A
#cmd LNObject_Retain $7B
#cmd LNObject_GetReferenceCount $7C
#cmd LNObject_SetPrototype_OnSerialize $929
#cmd LNEventConnection_SetPrototype_OnSerialize $934
#cmd LNPromiseFailureDelegate_Create $599
#cmd LNVariant_Create $81
#cmd LNVariant_SetInt $82
#cmd LNVariant_GetInt $84
#cmd LNVariant_SetPrototype_OnSerialize $93F
#cmd LNZVTestDelegate1_Create $5A2
#cmd LNZVTestDelegate2_Create $5A8
#cmd LNZVTestDelegate3_Create $5AD
#cmd LNZVTestEventHandler1_Create $5B2
#cmd LNZVTestEventHandler2_Create $5B7
#cmd LNZVTestPromise1_ThenWith $5BA
#cmd LNZVTestPromise1_CatchWith $5BC
#cmd LNZVTestPromise2_ThenWith $5C0
#cmd LNZVTestPromise2_CatchWith $5C2
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $94A
#cmd LNZVTestEventArgs1_Create $B0
#cmd LNZVTestEventArgs1_CreateWithValue $B1
#cmd LNZVTestEventArgs1_GetValue $AF
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $955
#cmd LNSerializer2_SetPrototype_OnSerialize $960
#cmd LNAssetObject_SetPrototype_OnSerialize $96B
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $976
#cmd LNAssetModel_Create $113
#cmd LNAssetModel_Target $112
#cmd LNAssetModel_SetPrototype_OnSerialize $981
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
#cmd LNSound_SetPrototype_OnSerialize $98C
#cmd LNAudio_PlayBGM $13F
#cmd LNAudio_StopBGM $144
#cmd LNAudio_PlayBGS $146
#cmd LNAudio_StopBGS $14B
#cmd LNAudio_PlayME $14D
#cmd LNAudio_StopME $151
#cmd LNAudio_PlaySE $152
#cmd LNAudio_PlaySE3D $156
#cmd LNAudio_StopSE $15C
#cmd LNTexture2DDelegate_Create $608
#cmd LNTexture2DPromise_ThenWith $60B
#cmd LNTexture2DPromise_CatchWith $60D
#cmd LNTexture_SetPrototype_OnSerialize $997
#cmd LNTexture2D_Create $1B7
#cmd LNTexture2D_CreateWithFormat $1BA
#cmd LNTexture2D_Load $1B3
#cmd LNTexture2D_LoadEmoji $1B5
#cmd LNTexture2D_SetPrototype_OnSerialize $9A2
#cmd LNShader_Load $1BF
#cmd LNShader_SetFloat $1C2
#cmd LNShader_SetVector3 $1C5
#cmd LNShader_SetVector4 $1C8
#cmd LNShader_SetPrototype_OnSerialize $9AD
#cmd LNRenderView_SetPrototype_OnSerialize $9B8
#cmd LNMaterial_Create $1ED
#cmd LNMaterial_SetMainTexture $1DC
#cmd LNMaterial_GetMainTexture $1DE
#cmd LNMaterial_SetColor $1DF
#cmd LNMaterial_SetRoughness $1E1
#cmd LNMaterial_SetMetallic $1E3
#cmd LNMaterial_SetEmissive $1E5
#cmd LNMaterial_SetShadingModel $1E7
#cmd LNMaterial_GetShadingModel $1E9
#cmd LNMaterial_SetShader $1EA
#cmd LNMaterial_GetShader $1EC
#cmd LNMaterial_SetPrototype_OnSerialize $9C3
#cmd LNMeshNode_SetVisible $1EF
#cmd LNMeshNode_IsVisible $1F1
#cmd LNMeshNode_SetPrototype_OnSerialize $9CE
#cmd LNStaticMeshModel_Load $1F3
#cmd LNStaticMeshModel_FindNode $1F5
#cmd LNStaticMeshModel_FindMaterial $1F7
#cmd LNStaticMeshModel_MaterialCount $1F9
#cmd LNStaticMeshModel_Material $1FA
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $9D9
#cmd LNSkinnedMeshModel_Load $1FD
#cmd LNSkinnedMeshModel_GetAnimationController $1FF
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $9E4
#cmd LNAnimationController_AddClip $201
#cmd LNAnimationController_Play $203
#cmd LNAnimationController_SetPrototype_OnSerialize $9EF
#cmd LNCollisionShape_SetPrototype_OnSerialize $9FA
#cmd LNBoxCollisionShape_Create $208
#cmd LNBoxCollisionShape_CreateWHD $20A
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $A05
#cmd LNAnimationClip_Load $214
#cmd LNAnimationClip_SetHierarchicalAnimationMode $216
#cmd LNAnimationClip_HierarchicalAnimationMode $218
#cmd LNAnimationClip_SetPrototype_OnSerialize $A10
#cmd LNAnimationState_SetPrototype_OnSerialize $A1B
#cmd LNEffectResource_SetPrototype_OnSerialize $A26
#cmd LNParticleEmitterModel_Create $23D
#cmd LNParticleEmitterModel_SetMaxParticles $225
#cmd LNParticleEmitterModel_SetSpawnRate $227
#cmd LNParticleEmitterModel_SetLifeTime $229
#cmd LNParticleEmitterModel_SetupBoxShape $22B
#cmd LNParticleEmitterModel_SetSize $22D
#cmd LNParticleEmitterModel_SetSizeVelocity $22F
#cmd LNParticleEmitterModel_SetSizeAcceleration $231
#cmd LNParticleEmitterModel_SetForwardVelocityMin $233
#cmd LNParticleEmitterModel_SetForwardVelocityMax $235
#cmd LNParticleEmitterModel_SetForwardScale $237
#cmd LNParticleEmitterModel_SetGeometryDirection $239
#cmd LNParticleEmitterModel_SetupSpriteModule $23B
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $A31
#cmd LNParticleModel_Create $244
#cmd LNParticleModel_SetLoop $23F
#cmd LNParticleModel_IsLoop $241
#cmd LNParticleModel_AddEmitter $242
#cmd LNParticleModel_SetPrototype_OnSerialize $A3C
#cmd LNComponent_SetPrototype_OnSerialize $A47
#cmd LNVisualComponent_SetVisible $247
#cmd LNVisualComponent_IsVisible $249
#cmd LNVisualComponent_SetPrototype_OnSerialize $A52
#cmd LNSpriteComponent_SetTexture $24B
#cmd LNSpriteComponent_SetPrototype_OnSerialize $A5D
#cmd LNCollisionEventHandler_Create $711
#cmd LNCharacterController_Create $26B
#cmd LNCharacterController_SetWalkVelocity $255
#cmd LNCharacterController_GetWalkVelocity $257
#cmd LNCharacterController_SetVelocity $258
#cmd LNCharacterController_GetVelocity $25A
#cmd LNCharacterController_SetInputControlEnabled $25B
#cmd LNCharacterController_SetCameraControlEnabled $25D
#cmd LNCharacterController_SetHeight $25F
#cmd LNCharacterController_GetHeight $261
#cmd LNCharacterController_SetCameraRadius $262
#cmd LNCharacterController_GetCameraRadius $264
#cmd LNCharacterController_SetCollisionEnter $265
#cmd LNCharacterController_SetCollisionLeave $267
#cmd LNCharacterController_SetCollisionStay $269
#cmd LNCharacterController_SetPrototype_OnSerialize $A68
#cmd LNWorld_Add $26D
#cmd LNWorld_SetPrototype_OnSerialize $A73
#cmd LNComponentList_GetLength $731
#cmd LNComponentList_GetItem $733
#cmd LNComponentList_SetPrototype_OnSerialize $A7E
#cmd LNWorldObject_Create $2A4
#cmd LNWorldObject_SetPosition $271
#cmd LNWorldObject_SetPositionXYZ $273
#cmd LNWorldObject_GetPosition $277
#cmd LNWorldObject_SetRotationQuaternion $278
#cmd LNWorldObject_SetRotation $27A
#cmd LNWorldObject_GetRotation $27E
#cmd LNWorldObject_SetScale $27F
#cmd LNWorldObject_SetScaleS $281
#cmd LNWorldObject_SetScaleXYZ $283
#cmd LNWorldObject_GetScale $287
#cmd LNWorldObject_SetCenterPoint $288
#cmd LNWorldObject_SetCenterPointXYZ $28A
#cmd LNWorldObject_GetCenterPoint $28E
#cmd LNWorldObject_LookAt $28F
#cmd LNWorldObject_LookAtXYZ $291
#cmd LNWorldObject_AddComponent $295
#cmd LNWorldObject_RemoveComponent $297
#cmd LNWorldObject_AddTag $299
#cmd LNWorldObject_RemoveTag $29B
#cmd LNWorldObject_HasTag $29D
#cmd LNWorldObject_Destroy $29F
#cmd LNWorldObject_GetComponents $2A0
#cmd LNWorldObject_SetPrototype_OnSerialize $A89
#cmd LNWorldObject_SetPrototype_OnPreUpdate $A94
#cmd LNWorldObject_SetPrototype_OnUpdate $A9F
#cmd LNVisualObject_SetVisible $2A6
#cmd LNVisualObject_IsVisible $2A8
#cmd LNVisualObject_SetBlendMode2 $2A9
#cmd LNVisualObject_SetOpacity $2AB
#cmd LNVisualObject_GetOpacity $2AD
#cmd LNVisualObject_SetPrototype_OnSerialize $AAA
#cmd LNVisualObject_SetPrototype_OnPreUpdate $AB5
#cmd LNVisualObject_SetPrototype_OnUpdate $AC0
#cmd LNCamera_SetPrototype_OnSerialize $ACB
#cmd LNCamera_SetPrototype_OnPreUpdate $AD6
#cmd LNCamera_SetPrototype_OnUpdate $AE1
#cmd LNEnvironmentLight_SetEnabled $2B0
#cmd LNEnvironmentLight_IsEnabled $2B2
#cmd LNEnvironmentLight_SetColor $2B3
#cmd LNEnvironmentLight_GetColor $2B5
#cmd LNEnvironmentLight_SetAmbientColor $2B6
#cmd LNEnvironmentLight_GetAmbientColor $2B8
#cmd LNEnvironmentLight_GetSkyColor $2B9
#cmd LNEnvironmentLight_SetSkyColor $2BA
#cmd LNEnvironmentLight_GetGroundColor $2BC
#cmd LNEnvironmentLight_SetGroundColor $2BD
#cmd LNEnvironmentLight_SetIntensity $2BF
#cmd LNEnvironmentLight_GetIntensity $2C1
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $2C2
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $2C4
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $2C5
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $2C7
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $AEC
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $AF7
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $B02
#cmd LNDirectionalLight_Create $2D8
#cmd LNDirectionalLight_CreateWithColor $2D9
#cmd LNDirectionalLight_SetEnabled $2C9
#cmd LNDirectionalLight_IsEnabled $2CB
#cmd LNDirectionalLight_SetColor $2CC
#cmd LNDirectionalLight_GetColor $2CE
#cmd LNDirectionalLight_SetIntensity $2CF
#cmd LNDirectionalLight_GetIntensity $2D1
#cmd LNDirectionalLight_SetShadowEffectiveDistance $2D2
#cmd LNDirectionalLight_GetShadowEffectiveDistance $2D4
#cmd LNDirectionalLight_SetShadowEffectiveDepth $2D5
#cmd LNDirectionalLight_GetShadowEffectiveDepth $2D7
#cmd LNDirectionalLight_SetPrototype_OnSerialize $B0D
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $B18
#cmd LNDirectionalLight_SetPrototype_OnUpdate $B23
#cmd LNPointLight_Create $2EB
#cmd LNPointLight_CreateWithColorAndRange $2EC
#cmd LNPointLight_SetEnabled $2DC
#cmd LNPointLight_IsEnabled $2DE
#cmd LNPointLight_SetColor $2DF
#cmd LNPointLight_GetColor $2E1
#cmd LNPointLight_SetIntensity $2E2
#cmd LNPointLight_GetIntensity $2E4
#cmd LNPointLight_SetRange $2E5
#cmd LNPointLight_GetRange $2E7
#cmd LNPointLight_SetAttenuation $2E8
#cmd LNPointLight_GetAttenuation $2EA
#cmd LNPointLight_SetPrototype_OnSerialize $B2E
#cmd LNPointLight_SetPrototype_OnPreUpdate $B39
#cmd LNPointLight_SetPrototype_OnUpdate $B44
#cmd LNSpotLight_Create $305
#cmd LNSpotLight_CreateWithColorAndRange $306
#cmd LNSpotLight_SetEnabled $2F0
#cmd LNSpotLight_IsEnabled $2F2
#cmd LNSpotLight_SetColor $2F3
#cmd LNSpotLight_GetColor $2F5
#cmd LNSpotLight_SetIntensity $2F6
#cmd LNSpotLight_GetIntensity $2F8
#cmd LNSpotLight_SetRange $2F9
#cmd LNSpotLight_GetRange $2FB
#cmd LNSpotLight_SetAttenuation $2FC
#cmd LNSpotLight_GetAttenuation $2FE
#cmd LNSpotLight_SetAngle $2FF
#cmd LNSpotLight_GetAngle $301
#cmd LNSpotLight_SetPenumbra $302
#cmd LNSpotLight_GetPenumbra $304
#cmd LNSpotLight_SetPrototype_OnSerialize $B4F
#cmd LNSpotLight_SetPrototype_OnPreUpdate $B5A
#cmd LNSpotLight_SetPrototype_OnUpdate $B65
#cmd LNTestDelegate_Create $7DF
#cmd LNSprite_Create $31C
#cmd LNSprite_CreateWithTexture $31D
#cmd LNSprite_CreateWithTextureAndSize $31F
#cmd LNSprite_SetTexture $30E
#cmd LNSprite_SetSize $310
#cmd LNSprite_SetSizeWH $312
#cmd LNSprite_SetSourceRectXYWH $315
#cmd LNSprite_SetCallerTest $31A
#cmd LNSprite_SetPrototype_OnSerialize $B70
#cmd LNSprite_SetPrototype_OnPreUpdate $B7B
#cmd LNSprite_SetPrototype_OnUpdate $B86
#cmd LNCameraOrbitControlComponent_Create $324
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $B91
#cmd LNRaycaster_FromScreen $326
#cmd LNRaycaster_IntersectPlane $328
#cmd LNRaycaster_SetPrototype_OnSerialize $B9C
#cmd LNRaycastResult_GetPoint $32D
#cmd LNRaycastResult_SetPrototype_OnSerialize $BA7
#cmd LNWorldRenderView_SetGuideGridEnabled $32F
#cmd LNWorldRenderView_GetGuideGridEnabled $331
#cmd LNWorldRenderView_SetPrototype_OnSerialize $BB2
#cmd LNBoxMesh_Create $333
#cmd LNBoxMesh_SetPrototype_OnSerialize $BBD
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $BC8
#cmd LNBoxMesh_SetPrototype_OnUpdate $BD3
#cmd LNPlaneMesh_Create $335
#cmd LNPlaneMesh_SetPrototype_OnSerialize $BDE
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $BE9
#cmd LNPlaneMesh_SetPrototype_OnUpdate $BF4
#cmd LNStaticMesh_Load $337
#cmd LNStaticMesh_GetModel $339
#cmd LNStaticMesh_MakeCollisionBody $33A
#cmd LNStaticMesh_SetPrototype_OnSerialize $BFF
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $C0A
#cmd LNStaticMesh_SetPrototype_OnUpdate $C15
#cmd LNStaticMeshComponent_Create $341
#cmd LNStaticMeshComponent_SetModel $33D
#cmd LNStaticMeshComponent_MakeCollisionBody $33F
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $C20
#cmd LNSkinnedMeshComponent_Create $345
#cmd LNSkinnedMeshComponent_SetModel $343
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $C2B
#cmd LNCollision_GetWorldObject $347
#cmd LNCollision_SetPrototype_OnSerialize $C36
#cmd LNTriggerBodyComponent_Create $34B
#cmd LNTriggerBodyComponent_AddCollisionShape $349
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $C41
#cmd LNParticleEmitter_Create $34D
#cmd LNParticleEmitter_SetPrototype_OnSerialize $C4C
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $C57
#cmd LNParticleEmitter_SetPrototype_OnUpdate $C62
#cmd LNScene_SetClearMode $350
#cmd LNScene_SetSkyColor $352
#cmd LNScene_SetSkyHorizonColor $354
#cmd LNScene_SetSkyCloudColor $356
#cmd LNScene_SetSkyOverlayColor $358
#cmd LNScene_GotoLevel $35A
#cmd LNScene_CallLevel $35D
#cmd LNScene_ReturnLevel $360
#cmd LNScene_ActiveLevel $362
#cmd LNScene_IsTransitionEffectRunning $363
#cmd LNScene_SetTransitionEffectMode $364
#cmd LNScene_TransitionEffectMode $366
#cmd LNScene_SetTransitionDuration $367
#cmd LNScene_TransitionDuration $369
#cmd LNScene_SetTransitionEffectColor $36A
#cmd LNScene_TransitionEffectColor $36C
#cmd LNScene_SetTransitionEffectMaskTexture $36D
#cmd LNScene_TransitionEffectMaskTexture $36F
#cmd LNScene_SetTransitionEffectVague $370
#cmd LNScene_TransitionEffectVague $372
#cmd LNScene_StartFadeOut $373
#cmd LNScene_StartFadeIn $374
#cmd LNScene_SetHDREnabled $375
#cmd LNScene_IsHDREnabled $377
#cmd LNScene_SetScreenBlendColor $378
#cmd LNScene_ScreenBlendColor $37A
#cmd LNScene_SetAntialiasEnabled $37B
#cmd LNScene_IsAntialiasEnabled $37D
#cmd LNScene_SetSSREnabled $37E
#cmd LNScene_IsSSREnabled $380
#cmd LNScene_SetSSAOEnabled $381
#cmd LNScene_IsSSAOEnabled $383
#cmd LNScene_SetBloomEnabled $384
#cmd LNScene_IsBloomEnabled $386
#cmd LNScene_SetDOFEnabled $387
#cmd LNScene_IsDOFEnabled $389
#cmd LNScene_SetTonemapEnabled $38A
#cmd LNScene_IsTonemapEnabled $38C
#cmd LNScene_SetVignetteEnabled $38D
#cmd LNScene_IsVignetteEnabled $38F
#cmd LNScene_SetGammaEnabled $390
#cmd LNScene_IsGammaEnabled $392
#cmd LNScene_SetTonemapExposure $393
#cmd LNScene_SetTonemapLinearWhite $395
#cmd LNScene_SetTonemapShoulderStrength $397
#cmd LNScene_SetTonemapLinearStrength $399
#cmd LNScene_SetTonemapLinearAngle $39B
#cmd LNScene_SetTonemapToeStrength $39D
#cmd LNScene_SetTonemapToeNumerator $39F
#cmd LNScene_SetTonemapToeDenominator $3A1
#cmd LNLevel_Create $3B2
#cmd LNLevel_AddObject $3A4
#cmd LNLevel_RemoveObject $3A6
#cmd LNLevel_AddSubLevel $3A8
#cmd LNLevel_RemoveSubLevel $3AA
#cmd LNLevel_RemoveAllSubLevels $3AC
#cmd LNLevel_SetPrototype_OnSerialize $C6D
#cmd LNLevel_SetPrototype_OnStart $C78
#cmd LNLevel_SetPrototype_OnStop $C83
#cmd LNLevel_SetPrototype_OnPause $C8E
#cmd LNLevel_SetPrototype_OnResume $C99
#cmd LNLevel_SetPrototype_OnUpdate $CA4
#cmd LNUIEventArgs_Sender $3BD
#cmd LNUIEventArgs_SetPrototype_OnSerialize $CAF
#cmd LNUIGeneralEventHandler_Create $83D
#cmd LNUIEventHandler_Create $842
#cmd LNUILayoutElement_SetPrototype_OnSerialize $CBA
#cmd LNUIElement_SetSize $3CF
#cmd LNUIElement_SetSizeWH $3D1
#cmd LNUIElement_SetWidth $3D4
#cmd LNUIElement_GetWidth $3D6
#cmd LNUIElement_SetHeight $3D7
#cmd LNUIElement_GetHeight $3D9
#cmd LNUIElement_SetMargin $3DA
#cmd LNUIElement_GetMargin $3DC
#cmd LNUIElement_SetPadding $3DD
#cmd LNUIElement_GetPadding $3DF
#cmd LNUIElement_SetHAlignment $3E0
#cmd LNUIElement_GetHAlignment $3E2
#cmd LNUIElement_SetVAlignment $3E3
#cmd LNUIElement_GetVAlignment $3E5
#cmd LNUIElement_SetAlignments $3E6
#cmd LNUIElement_SetPosition $3E9
#cmd LNUIElement_SetPositionXYZ $3EB
#cmd LNUIElement_GetPosition $3EF
#cmd LNUIElement_SetRotation $3F0
#cmd LNUIElement_SetEulerAngles $3F2
#cmd LNUIElement_GetRotation $3F6
#cmd LNUIElement_SetScale $3F7
#cmd LNUIElement_SetScaleS $3F9
#cmd LNUIElement_SetScaleXY $3FB
#cmd LNUIElement_GetScale $3FE
#cmd LNUIElement_SetCenterPoint $3FF
#cmd LNUIElement_SetCenterPointXYZ $401
#cmd LNUIElement_GetCenterPoint $405
#cmd LNUIElement_SetEnabled $406
#cmd LNUIElement_IsEnabled $408
#cmd LNUIElement_SetData $409
#cmd LNUIElement_GetData $40B
#cmd LNUIElement_SetBackgroundColor $40C
#cmd LNUIElement_GetBackgroundColor $40E
#cmd LNUIElement_SetBorderThickness $40F
#cmd LNUIElement_GetBorderThickness $411
#cmd LNUIElement_SetBorderColor $412
#cmd LNUIElement_GetBorderColor $414
#cmd LNUIElement_SetCornerRadius $415
#cmd LNUIElement_GetCornerRadius $417
#cmd LNUIElement_SetVisibility $418
#cmd LNUIElement_GetVisibility $41A
#cmd LNUIElement_SetOpacity $41B
#cmd LNUIElement_GetOpacity $41D
#cmd LNUIElement_AddChild $41E
#cmd LNUIElement_SetPrototype_OnSerialize $CC5
#cmd LNUITextBlock_Create $424
#cmd LNUITextBlock_CreateWithText $425
#cmd LNUITextBlock_SetText $421
#cmd LNUITextBlock_GetText $423
#cmd LNUITextBlock_SetPrototype_OnSerialize $CD0
#cmd LNUISprite_Create $432
#cmd LNUISprite_CreateWithTexture $433
#cmd LNUISprite_SetTexture $428
#cmd LNUISprite_SetSourceRect $42A
#cmd LNUISprite_SetSourceRectXYWH $42C
#cmd LNUISprite_GetSourceRect $431
#cmd LNUISprite_SetPrototype_OnSerialize $CDB
#cmd LNUIIcon_LoadFontIcon $436
#cmd LNUIIcon_LoadFontIconWithNameSize $438
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $43B
#cmd LNUIIcon_SetPrototype_OnSerialize $CE6
#cmd LNUIMessageTextArea_Create $444
#cmd LNUIMessageTextArea_SetText $440
#cmd LNUIMessageTextArea_SetTypingSpeed $442
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $CF1
#cmd LNUI_Add $446
#cmd LNUI_Remove $448
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $CFC
#cmd LNUIBoxLayout_Create $44F
#cmd LNUIBoxLayout_SetOrientation $44C
#cmd LNUIBoxLayout_GetOrientation $44E
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $D07
#cmd LNUIGridLayout_Create $45F
#cmd LNUIGridLayout_SetColumnCount $451
#cmd LNUIGridLayout_SetRow $453
#cmd LNUIGridLayout_SetColumn $456
#cmd LNUIGridLayout_SetPlacement $459
#cmd LNUIGridLayout_SetPrototype_OnSerialize $D12
#cmd LNUIControl_Create $46B
#cmd LNUIControl_AddInlineVisual $46C
#cmd LNUIControl_SetPrototype_OnSerialize $D1D
#cmd LNUIButtonBase_SetText $470
#cmd LNUIButtonBase_SetPrototype_OnSerialize $D28
#cmd LNUIButton_Create $473
#cmd LNUIButton_CreateWithText $474
#cmd LNUIButton_ConnectOnClicked $476
#cmd LNUIButton_SetPrototype_OnSerialize $D33
#cmd LNUIWindow_Create $479
#cmd LNUIWindow_SetPrototype_OnSerialize $D3E
#cmd LNUIListItem_ConnectOnSubmit $47E
#cmd LNUIListItem_SetPrototype_OnSerialize $D49
#cmd LNUIListItemsControl_SetItemsLayoutPanel $481
#cmd LNUIListItemsControl_SetSubmitMode $483
#cmd LNUIListItemsControl_GetSubmitMode $485
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $D54
#cmd LNUIListBoxItem_Create $487
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $D5F
#cmd LNUIListBox_Create $48C
#cmd LNUIListBox_AddItem $48A
#cmd LNUIListBox_SetPrototype_OnSerialize $D6A
#cmd LNInputGesture_SetPrototype_OnSerialize $D75
#cmd LNKeyGesture_Create $48F
#cmd LNKeyGesture_SetPrototype_OnSerialize $D80
#cmd LNInput_IsPressed $492
#cmd LNInput_IsTriggered $494
#cmd LNInput_IsTriggeredOff $496
#cmd LNInput_IsRepeated $498
#cmd LNInput_GetAxisValue $49A
#cmd LNInput_AddBinding $49C
#cmd LNInput_RemoveBinding $49F
#cmd LNInput_ClearBindings $4A1
#cmd LNInput_ClearAllBindings $4A3
#cmd LNMouse_Pressed $4A5
#cmd LNMouse_Triggered $4A7
#cmd LNMouse_TriggeredOff $4A9
#cmd LNMouse_Repeated $4AB
#cmd LNMouse_Position $4AD
#cmd LNInterpreterCommand_Code $4AF
#cmd LNInterpreterCommand_ParamsCount $4B0
#cmd LNInterpreterCommand_Param $4B1
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $D8B
#cmd LNInterpreterCommandList_Create $4C8
#cmd LNInterpreterCommandList_AddCommand $4B4
#cmd LNInterpreterCommandList_AddCommand1 $4B6
#cmd LNInterpreterCommandList_AddCommand2 $4B9
#cmd LNInterpreterCommandList_AddCommand3 $4BD
#cmd LNInterpreterCommandList_AddCommand4 $4C2
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $D96
#cmd LNInterpreterCommandDelegate_Create $8FB
#cmd LNInterpreter_Create $4DC
#cmd LNInterpreter_Clear $4CD
#cmd LNInterpreter_Run $4CE
#cmd LNInterpreter_IsRunning $4D0
#cmd LNInterpreter_Update $4D1
#cmd LNInterpreter_Terminate $4D2
#cmd LNInterpreter_RegisterCommandHandler $4D3
#cmd LNInterpreter_SetWaitMode $4D6
#cmd LNInterpreter_GetWaitMode $4D8
#cmd LNInterpreter_SetWaitCount $4D9
#cmd LNInterpreter_GetWaitCount $4DB
#cmd LNInterpreter_SetPrototype_OnSerialize $DA1
#cmd LNInterpreter_SetPrototype_OnUpdateWait $DAD
#cmd LNEngineSettings_SetMainWindowSize $4DF
#cmd LNEngineSettings_SetMainWorldViewSize $4E2
#cmd LNEngineSettings_SetMainWindowTitle $4E5
#cmd LNEngineSettings_AddAssetDirectory $4E7
#cmd LNEngineSettings_AddAssetArchive $4E9
#cmd LNEngineSettings_SetFrameRate $4EC
#cmd LNEngineSettings_SetUITheme $4EE
#cmd LNEngineSettings_SetFontFile $4F0
#cmd LNEngineSettings_SetDebugToolEnabled $4F2
#cmd LNEngineSettings_SetEngineLogEnabled $4F4
#cmd LNEngineSettings_SetEngineLogFilePath $4F6
#cmd LNEngineSettings_SetDeveloperToolEnabled $4F8
#cmd LNEngine_Initialize $4FB
#cmd LNEngine_Finalize $4FC
#cmd LNEngine_Update $4FD
#cmd LNEngine_Run $4FE
#cmd LNEngine_Time $500
#cmd LNEngine_GetCamera $501
#cmd LNEngine_GetMainLight $502
#cmd LNEngine_GetRenderView $503
#cmd LNApplication_Create $508
#cmd LNApplication_OnInit $505
#cmd LNApplication_OnUpdate $506
#cmd LNApplication_World $507
#cmd LNApplication_SetPrototype_OnSerialize $DB8
#cmd LNApplication_SetPrototype_OnInit $DC3
#cmd LNApplication_SetPrototype_OnUpdate $DCE
#cmd LNDebug_SetGuideGridEnabled $50A
#cmd LNDebug_SetPhysicsDebugDrawEnabled $50C
#cmd LNDebug_Print $50E
#cmd LNDebug_PrintWithTime $510
#cmd LNDebug_PrintWithTimeAndColor $513
#cmd LNObjectSerializeHandler_Create $927
#cmd LNEventConnectionSerializeHandler_Create $932
#cmd LNVariantSerializeHandler_Create $93D
#cmd LNZVTestClass1SerializeHandler_Create $948
#cmd LNZVTestEventArgs1SerializeHandler_Create $953
#cmd LNSerializer2SerializeHandler_Create $95E
#cmd LNAssetObjectSerializeHandler_Create $969
#cmd LNAssetImportSettingsSerializeHandler_Create $974
#cmd LNAssetModelSerializeHandler_Create $97F
#cmd LNSoundSerializeHandler_Create $98A
#cmd LNTextureSerializeHandler_Create $995
#cmd LNTexture2DSerializeHandler_Create $9A0
#cmd LNShaderSerializeHandler_Create $9AB
#cmd LNRenderViewSerializeHandler_Create $9B6
#cmd LNMaterialSerializeHandler_Create $9C1
#cmd LNMeshNodeSerializeHandler_Create $9CC
#cmd LNStaticMeshModelSerializeHandler_Create $9D7
#cmd LNSkinnedMeshModelSerializeHandler_Create $9E2
#cmd LNAnimationControllerSerializeHandler_Create $9ED
#cmd LNCollisionShapeSerializeHandler_Create $9F8
#cmd LNBoxCollisionShapeSerializeHandler_Create $A03
#cmd LNAnimationClipSerializeHandler_Create $A0E
#cmd LNAnimationStateSerializeHandler_Create $A19
#cmd LNEffectResourceSerializeHandler_Create $A24
#cmd LNParticleEmitterModelSerializeHandler_Create $A2F
#cmd LNParticleModelSerializeHandler_Create $A3A
#cmd LNComponentSerializeHandler_Create $A45
#cmd LNVisualComponentSerializeHandler_Create $A50
#cmd LNSpriteComponentSerializeHandler_Create $A5B
#cmd LNCharacterControllerSerializeHandler_Create $A66
#cmd LNWorldSerializeHandler_Create $A71
#cmd LNComponentListSerializeHandler_Create $A7C
#cmd LNWorldObjectSerializeHandler_Create $A87
#cmd LNWorldObjectPreUpdateHandler_Create $A92
#cmd LNWorldObjectUpdateHandler_Create $A9D
#cmd LNVisualObjectSerializeHandler_Create $AA8
#cmd LNVisualObjectPreUpdateHandler_Create $AB3
#cmd LNVisualObjectUpdateHandler_Create $ABE
#cmd LNCameraSerializeHandler_Create $AC9
#cmd LNCameraPreUpdateHandler_Create $AD4
#cmd LNCameraUpdateHandler_Create $ADF
#cmd LNEnvironmentLightSerializeHandler_Create $AEA
#cmd LNEnvironmentLightPreUpdateHandler_Create $AF5
#cmd LNEnvironmentLightUpdateHandler_Create $B00
#cmd LNDirectionalLightSerializeHandler_Create $B0B
#cmd LNDirectionalLightPreUpdateHandler_Create $B16
#cmd LNDirectionalLightUpdateHandler_Create $B21
#cmd LNPointLightSerializeHandler_Create $B2C
#cmd LNPointLightPreUpdateHandler_Create $B37
#cmd LNPointLightUpdateHandler_Create $B42
#cmd LNSpotLightSerializeHandler_Create $B4D
#cmd LNSpotLightPreUpdateHandler_Create $B58
#cmd LNSpotLightUpdateHandler_Create $B63
#cmd LNSpriteSerializeHandler_Create $B6E
#cmd LNSpritePreUpdateHandler_Create $B79
#cmd LNSpriteUpdateHandler_Create $B84
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $B8F
#cmd LNRaycasterSerializeHandler_Create $B9A
#cmd LNRaycastResultSerializeHandler_Create $BA5
#cmd LNWorldRenderViewSerializeHandler_Create $BB0
#cmd LNBoxMeshSerializeHandler_Create $BBB
#cmd LNBoxMeshPreUpdateHandler_Create $BC6
#cmd LNBoxMeshUpdateHandler_Create $BD1
#cmd LNPlaneMeshSerializeHandler_Create $BDC
#cmd LNPlaneMeshPreUpdateHandler_Create $BE7
#cmd LNPlaneMeshUpdateHandler_Create $BF2
#cmd LNStaticMeshSerializeHandler_Create $BFD
#cmd LNStaticMeshPreUpdateHandler_Create $C08
#cmd LNStaticMeshUpdateHandler_Create $C13
#cmd LNStaticMeshComponentSerializeHandler_Create $C1E
#cmd LNSkinnedMeshComponentSerializeHandler_Create $C29
#cmd LNCollisionSerializeHandler_Create $C34
#cmd LNTriggerBodyComponentSerializeHandler_Create $C3F
#cmd LNParticleEmitterSerializeHandler_Create $C4A
#cmd LNParticleEmitterPreUpdateHandler_Create $C55
#cmd LNParticleEmitterUpdateHandler_Create $C60
#cmd LNLevelSerializeHandler_Create $C6B
#cmd LNLevelStartHandler_Create $C76
#cmd LNLevelStopHandler_Create $C81
#cmd LNLevelPauseHandler_Create $C8C
#cmd LNLevelResumeHandler_Create $C97
#cmd LNLevelUpdateHandler_Create $CA2
#cmd LNUIEventArgsSerializeHandler_Create $CAD
#cmd LNUILayoutElementSerializeHandler_Create $CB8
#cmd LNUIElementSerializeHandler_Create $CC3
#cmd LNUITextBlockSerializeHandler_Create $CCE
#cmd LNUISpriteSerializeHandler_Create $CD9
#cmd LNUIIconSerializeHandler_Create $CE4
#cmd LNUIMessageTextAreaSerializeHandler_Create $CEF
#cmd LNUILayoutPanelSerializeHandler_Create $CFA
#cmd LNUIBoxLayoutSerializeHandler_Create $D05
#cmd LNUIGridLayoutSerializeHandler_Create $D10
#cmd LNUIControlSerializeHandler_Create $D1B
#cmd LNUIButtonBaseSerializeHandler_Create $D26
#cmd LNUIButtonSerializeHandler_Create $D31
#cmd LNUIWindowSerializeHandler_Create $D3C
#cmd LNUIListItemSerializeHandler_Create $D47
#cmd LNUIListItemsControlSerializeHandler_Create $D52
#cmd LNUIListBoxItemSerializeHandler_Create $D5D
#cmd LNUIListBoxSerializeHandler_Create $D68
#cmd LNInputGestureSerializeHandler_Create $D73
#cmd LNKeyGestureSerializeHandler_Create $D7E
#cmd LNInterpreterCommandSerializeHandler_Create $D89
#cmd LNInterpreterCommandListSerializeHandler_Create $D94
#cmd LNInterpreterSerializeHandler_Create $D9F
#cmd LNInterpreterUpdateWaitHandler_Create $DAB
#cmd LNApplicationSerializeHandler_Create $DB6
#cmd LNApplicationInitHandler_Create $DC1
#cmd LNApplicationUpdateHandler_Create $DCC



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
