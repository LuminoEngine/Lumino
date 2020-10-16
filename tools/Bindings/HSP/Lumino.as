
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
#cmd LNVector3_GetX $4F4
#cmd LNVector3_SetX $4F6
#cmd LNVector3_GetY $4F7
#cmd LNVector3_SetY $4F9
#cmd LNVector3_GetZ $4FA
#cmd LNVector3_SetZ $4FC
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $513
#cmd LNVector4_SetX $515
#cmd LNVector4_GetY $516
#cmd LNVector4_SetY $518
#cmd LNVector4_GetZ $519
#cmd LNVector4_SetZ $51B
#cmd LNVector4_GetW $51C
#cmd LNVector4_SetW $51E
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $531
#cmd LNQuaternion_SetX $533
#cmd LNQuaternion_GetY $534
#cmd LNQuaternion_SetY $536
#cmd LNQuaternion_GetZ $537
#cmd LNQuaternion_SetZ $539
#cmd LNQuaternion_GetW $53A
#cmd LNQuaternion_SetW $53C
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $550
#cmd LNMatrix_SetRow0 $552
#cmd LNMatrix_GetRow1 $553
#cmd LNMatrix_SetRow1 $555
#cmd LNMatrix_GetRow2 $556
#cmd LNMatrix_SetRow2 $558
#cmd LNMatrix_GetRow3 $559
#cmd LNMatrix_SetRow3 $55B
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $173
#cmd LNColor_GetR $5ED
#cmd LNColor_SetR $5EF
#cmd LNColor_GetG $5F0
#cmd LNColor_SetG $5F2
#cmd LNColor_GetB $5F3
#cmd LNColor_SetB $5F5
#cmd LNColor_GetA $5F6
#cmd LNColor_SetA $5F8
#cmd LNColor_SetZeros $178
#cmd LNColor_Set $179
#cmd LNPoint $17E
#cmd LNPoint_GetX $60C
#cmd LNPoint_SetX $60E
#cmd LNPoint_GetY $60F
#cmd LNPoint_SetY $611
#cmd LNPoint_SetZeros $181
#cmd LNPoint_Set $182
#cmd LNSize $185
#cmd LNSize_GetWidth $61C
#cmd LNSize_SetWidth $61E
#cmd LNSize_GetHeight $61F
#cmd LNSize_SetHeight $621
#cmd LNSize_SetZeros $188
#cmd LNSize_Set $189
#cmd LNRect $18C
#cmd LNRect_GetX $62C
#cmd LNRect_SetX $62E
#cmd LNRect_GetY $62F
#cmd LNRect_SetY $631
#cmd LNRect_GetWidth $632
#cmd LNRect_SetWidth $634
#cmd LNRect_GetHeight $635
#cmd LNRect_SetHeight $637
#cmd LNRect_SetZeros $191
#cmd LNRect_Set $192
#cmd LNRect_GetLeft $197
#cmd LNRect_SetSize $198
#cmd LNRect_GetSize $19A
#cmd LNThickness $19B
#cmd LNThickness_GetLeft $650
#cmd LNThickness_SetLeft $652
#cmd LNThickness_GetTop $653
#cmd LNThickness_SetTop $655
#cmd LNThickness_GetRight $656
#cmd LNThickness_SetRight $658
#cmd LNThickness_GetBottom $659
#cmd LNThickness_SetBottom $65B
#cmd LNThickness_SetZeros $1A0
#cmd LNThickness_Set $1A1
#cmd LNCornerRadius $1A6
#cmd LNCornerRadius_GetTopleft $66E
#cmd LNCornerRadius_SetTopleft $670
#cmd LNCornerRadius_GetTopright $671
#cmd LNCornerRadius_SetTopright $673
#cmd LNCornerRadius_GetBottomright $674
#cmd LNCornerRadius_SetBottomright $676
#cmd LNCornerRadius_GetBottomleft $677
#cmd LNCornerRadius_SetBottomleft $679
#cmd LNCornerRadius_SetZeros $1AB
#cmd LNCornerRadius_Set $1AC

#cmd LNObject_Release $7A
#cmd LNObject_Retain $7B
#cmd LNObject_GetReferenceCount $7C
#cmd LNObject_SetPrototype_OnSerialize $8EF
#cmd LNEventConnection_SetPrototype_OnSerialize $8FA
#cmd LNPromiseFailureDelegate_Create $576
#cmd LNVariant_Create $81
#cmd LNVariant_SetInt $82
#cmd LNVariant_GetInt $84
#cmd LNVariant_SetPrototype_OnSerialize $905
#cmd LNZVTestDelegate1_Create $57F
#cmd LNZVTestDelegate2_Create $585
#cmd LNZVTestDelegate3_Create $58A
#cmd LNZVTestEventHandler1_Create $58F
#cmd LNZVTestEventHandler2_Create $594
#cmd LNZVTestPromise1_ThenWith $597
#cmd LNZVTestPromise1_CatchWith $599
#cmd LNZVTestPromise2_ThenWith $59D
#cmd LNZVTestPromise2_CatchWith $59F
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $910
#cmd LNZVTestEventArgs1_Create $B0
#cmd LNZVTestEventArgs1_CreateWithValue $B1
#cmd LNZVTestEventArgs1_GetValue $AF
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $91B
#cmd LNSerializer2_SetPrototype_OnSerialize $926
#cmd LNAssetObject_SetPrototype_OnSerialize $931
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $93C
#cmd LNAssetModel_Create $113
#cmd LNAssetModel_Target $112
#cmd LNAssetModel_SetPrototype_OnSerialize $947
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
#cmd LNSound_SetPrototype_OnSerialize $952
#cmd LNAudio_PlayBGM $13F
#cmd LNAudio_StopBGM $144
#cmd LNAudio_PlayBGS $146
#cmd LNAudio_StopBGS $14B
#cmd LNAudio_PlayME $14D
#cmd LNAudio_StopME $151
#cmd LNAudio_PlaySE $152
#cmd LNAudio_PlaySE3D $156
#cmd LNAudio_StopSE $15C
#cmd LNTexture2DDelegate_Create $5E5
#cmd LNTexture2DPromise_ThenWith $5E8
#cmd LNTexture2DPromise_CatchWith $5EA
#cmd LNTexture_SetPrototype_OnSerialize $95D
#cmd LNTexture2D_Create $1B7
#cmd LNTexture2D_CreateWithFormat $1BA
#cmd LNTexture2D_Load $1B3
#cmd LNTexture2D_LoadEmoji $1B5
#cmd LNTexture2D_SetPrototype_OnSerialize $968
#cmd LNShader_Load $1BF
#cmd LNShader_SetFloat $1C2
#cmd LNShader_SetVector3 $1C5
#cmd LNShader_SetVector4 $1C8
#cmd LNShader_SetPrototype_OnSerialize $973
#cmd LNRenderView_SetPrototype_OnSerialize $97E
#cmd LNMaterial_Create $1E7
#cmd LNMaterial_SetMainTexture $1D6
#cmd LNMaterial_GetMainTexture $1D8
#cmd LNMaterial_SetColor $1D9
#cmd LNMaterial_SetRoughness $1DB
#cmd LNMaterial_SetMetallic $1DD
#cmd LNMaterial_SetEmissive $1DF
#cmd LNMaterial_SetShadingModel $1E1
#cmd LNMaterial_GetShadingModel $1E3
#cmd LNMaterial_SetShader $1E4
#cmd LNMaterial_GetShader $1E6
#cmd LNMaterial_SetPrototype_OnSerialize $989
#cmd LNMeshNode_SetVisible $1E9
#cmd LNMeshNode_IsVisible $1EB
#cmd LNMeshNode_SetPrototype_OnSerialize $994
#cmd LNStaticMeshModel_Load $1ED
#cmd LNStaticMeshModel_MaterialCount $1EF
#cmd LNStaticMeshModel_Material $1F0
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $99F
#cmd LNSkinnedMeshModel_Load $1F3
#cmd LNSkinnedMeshModel_GetAnimationController $1F5
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $9AA
#cmd LNAnimationController_AddClip $1F7
#cmd LNAnimationController_Play $1F9
#cmd LNAnimationController_SetPrototype_OnSerialize $9B5
#cmd LNCollisionShape_SetPrototype_OnSerialize $9C0
#cmd LNBoxCollisionShape_Create $1FE
#cmd LNBoxCollisionShape_CreateWHD $200
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $9CB
#cmd LNAnimationClip_Load $20A
#cmd LNAnimationClip_SetHierarchicalAnimationMode $20C
#cmd LNAnimationClip_HierarchicalAnimationMode $20E
#cmd LNAnimationClip_SetPrototype_OnSerialize $9D6
#cmd LNAnimationState_SetPrototype_OnSerialize $9E1
#cmd LNEffectResource_SetPrototype_OnSerialize $9EC
#cmd LNParticleEmitterModel_Create $22F
#cmd LNParticleEmitterModel_SetMaxParticles $21B
#cmd LNParticleEmitterModel_SetSpawnRate $21D
#cmd LNParticleEmitterModel_SetLifeTime $21F
#cmd LNParticleEmitterModel_SetupBoxShape $221
#cmd LNParticleEmitterModel_SetSize $223
#cmd LNParticleEmitterModel_SetForwardVelocityMin $225
#cmd LNParticleEmitterModel_SetForwardVelocityMax $227
#cmd LNParticleEmitterModel_SetForwardScale $229
#cmd LNParticleEmitterModel_SetGeometryDirection $22B
#cmd LNParticleEmitterModel_SetupSpriteModule $22D
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $9F7
#cmd LNParticleModel_Create $236
#cmd LNParticleModel_SetLoop $231
#cmd LNParticleModel_IsLoop $233
#cmd LNParticleModel_AddEmitter $234
#cmd LNParticleModel_SetPrototype_OnSerialize $A02
#cmd LNComponent_SetPrototype_OnSerialize $A0D
#cmd LNVisualComponent_SetVisible $239
#cmd LNVisualComponent_IsVisible $23B
#cmd LNVisualComponent_SetPrototype_OnSerialize $A18
#cmd LNSpriteComponent_SetTexture $23D
#cmd LNSpriteComponent_SetPrototype_OnSerialize $A23
#cmd LNCollisionEventHandler_Create $6E6
#cmd LNCharacterController_Create $25A
#cmd LNCharacterController_SetVelocity $247
#cmd LNCharacterController_GetVelocity $249
#cmd LNCharacterController_SetInputControlEnabled $24A
#cmd LNCharacterController_SetCameraControlEnabled $24C
#cmd LNCharacterController_SetHeight $24E
#cmd LNCharacterController_GetHeight $250
#cmd LNCharacterController_SetCameraRadius $251
#cmd LNCharacterController_GetCameraRadius $253
#cmd LNCharacterController_SetCollisionEnter $254
#cmd LNCharacterController_SetCollisionLeave $256
#cmd LNCharacterController_SetCollisionStay $258
#cmd LNCharacterController_SetPrototype_OnSerialize $A2E
#cmd LNWorld_Add $25C
#cmd LNWorld_SetPrototype_OnSerialize $A39
#cmd LNComponentList_GetLength $701
#cmd LNComponentList_GetItem $703
#cmd LNComponentList_SetPrototype_OnSerialize $A44
#cmd LNWorldObject_Create $293
#cmd LNWorldObject_SetPosition $260
#cmd LNWorldObject_SetPositionXYZ $262
#cmd LNWorldObject_GetPosition $266
#cmd LNWorldObject_SetRotationQuaternion $267
#cmd LNWorldObject_SetRotation $269
#cmd LNWorldObject_GetRotation $26D
#cmd LNWorldObject_SetScale $26E
#cmd LNWorldObject_SetScaleS $270
#cmd LNWorldObject_SetScaleXYZ $272
#cmd LNWorldObject_GetScale $276
#cmd LNWorldObject_SetCenterPoint $277
#cmd LNWorldObject_SetCenterPointXYZ $279
#cmd LNWorldObject_GetCenterPoint $27D
#cmd LNWorldObject_LookAt $27E
#cmd LNWorldObject_LookAtXYZ $280
#cmd LNWorldObject_AddComponent $284
#cmd LNWorldObject_RemoveComponent $286
#cmd LNWorldObject_AddTag $288
#cmd LNWorldObject_RemoveTag $28A
#cmd LNWorldObject_HasTag $28C
#cmd LNWorldObject_Destroy $28E
#cmd LNWorldObject_GetComponents $28F
#cmd LNWorldObject_SetPrototype_OnSerialize $A4F
#cmd LNWorldObject_SetPrototype_OnPreUpdate $A5A
#cmd LNWorldObject_SetPrototype_OnUpdate $A65
#cmd LNVisualObject_SetVisible $295
#cmd LNVisualObject_IsVisible $297
#cmd LNVisualObject_SetOpacity $298
#cmd LNVisualObject_GetOpacity $29A
#cmd LNVisualObject_SetPrototype_OnSerialize $A70
#cmd LNVisualObject_SetPrototype_OnPreUpdate $A7B
#cmd LNVisualObject_SetPrototype_OnUpdate $A86
#cmd LNCamera_SetPrototype_OnSerialize $A91
#cmd LNCamera_SetPrototype_OnPreUpdate $A9C
#cmd LNCamera_SetPrototype_OnUpdate $AA7
#cmd LNEnvironmentLight_SetEnabled $29D
#cmd LNEnvironmentLight_IsEnabled $29F
#cmd LNEnvironmentLight_SetColor $2A0
#cmd LNEnvironmentLight_GetColor $2A2
#cmd LNEnvironmentLight_SetAmbientColor $2A3
#cmd LNEnvironmentLight_GetAmbientColor $2A5
#cmd LNEnvironmentLight_GetSkyColor $2A6
#cmd LNEnvironmentLight_SetSkyColor $2A7
#cmd LNEnvironmentLight_GetGroundColor $2A9
#cmd LNEnvironmentLight_SetGroundColor $2AA
#cmd LNEnvironmentLight_SetIntensity $2AC
#cmd LNEnvironmentLight_GetIntensity $2AE
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $2AF
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $2B1
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $2B2
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $2B4
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $AB2
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $ABD
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $AC8
#cmd LNDirectionalLight_Create $2C5
#cmd LNDirectionalLight_CreateWithColor $2C6
#cmd LNDirectionalLight_SetEnabled $2B6
#cmd LNDirectionalLight_IsEnabled $2B8
#cmd LNDirectionalLight_SetColor $2B9
#cmd LNDirectionalLight_GetColor $2BB
#cmd LNDirectionalLight_SetIntensity $2BC
#cmd LNDirectionalLight_GetIntensity $2BE
#cmd LNDirectionalLight_SetShadowEffectiveDistance $2BF
#cmd LNDirectionalLight_GetShadowEffectiveDistance $2C1
#cmd LNDirectionalLight_SetShadowEffectiveDepth $2C2
#cmd LNDirectionalLight_GetShadowEffectiveDepth $2C4
#cmd LNDirectionalLight_SetPrototype_OnSerialize $AD3
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $ADE
#cmd LNDirectionalLight_SetPrototype_OnUpdate $AE9
#cmd LNPointLight_Create $2D8
#cmd LNPointLight_CreateWithColorAndRange $2D9
#cmd LNPointLight_SetEnabled $2C9
#cmd LNPointLight_IsEnabled $2CB
#cmd LNPointLight_SetColor $2CC
#cmd LNPointLight_GetColor $2CE
#cmd LNPointLight_SetIntensity $2CF
#cmd LNPointLight_GetIntensity $2D1
#cmd LNPointLight_SetRange $2D2
#cmd LNPointLight_GetRange $2D4
#cmd LNPointLight_SetAttenuation $2D5
#cmd LNPointLight_GetAttenuation $2D7
#cmd LNPointLight_SetPrototype_OnSerialize $AF4
#cmd LNPointLight_SetPrototype_OnPreUpdate $AFF
#cmd LNPointLight_SetPrototype_OnUpdate $B0A
#cmd LNSpotLight_Create $2F2
#cmd LNSpotLight_CreateWithColorAndRange $2F3
#cmd LNSpotLight_SetEnabled $2DD
#cmd LNSpotLight_IsEnabled $2DF
#cmd LNSpotLight_SetColor $2E0
#cmd LNSpotLight_GetColor $2E2
#cmd LNSpotLight_SetIntensity $2E3
#cmd LNSpotLight_GetIntensity $2E5
#cmd LNSpotLight_SetRange $2E6
#cmd LNSpotLight_GetRange $2E8
#cmd LNSpotLight_SetAttenuation $2E9
#cmd LNSpotLight_GetAttenuation $2EB
#cmd LNSpotLight_SetAngle $2EC
#cmd LNSpotLight_GetAngle $2EE
#cmd LNSpotLight_SetPenumbra $2EF
#cmd LNSpotLight_GetPenumbra $2F1
#cmd LNSpotLight_SetPrototype_OnSerialize $B15
#cmd LNSpotLight_SetPrototype_OnPreUpdate $B20
#cmd LNSpotLight_SetPrototype_OnUpdate $B2B
#cmd LNTestDelegate_Create $7AD
#cmd LNSprite_Create $309
#cmd LNSprite_CreateWithTexture $30A
#cmd LNSprite_CreateWithTextureAndSize $30C
#cmd LNSprite_SetTexture $2FB
#cmd LNSprite_SetSize $2FD
#cmd LNSprite_SetSizeWH $2FF
#cmd LNSprite_SetSourceRectXYWH $302
#cmd LNSprite_SetCallerTest $307
#cmd LNSprite_SetPrototype_OnSerialize $B36
#cmd LNSprite_SetPrototype_OnPreUpdate $B41
#cmd LNSprite_SetPrototype_OnUpdate $B4C
#cmd LNCameraOrbitControlComponent_Create $311
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $B57
#cmd LNRaycaster_FromScreen $313
#cmd LNRaycaster_IntersectPlane $315
#cmd LNRaycaster_SetPrototype_OnSerialize $B62
#cmd LNRaycastResult_GetPoint $31A
#cmd LNRaycastResult_SetPrototype_OnSerialize $B6D
#cmd LNWorldRenderView_SetGuideGridEnabled $31C
#cmd LNWorldRenderView_GetGuideGridEnabled $31E
#cmd LNWorldRenderView_SetPrototype_OnSerialize $B78
#cmd LNBoxMesh_Create $320
#cmd LNBoxMesh_SetPrototype_OnSerialize $B83
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $B8E
#cmd LNBoxMesh_SetPrototype_OnUpdate $B99
#cmd LNPlaneMesh_Create $322
#cmd LNPlaneMesh_SetPrototype_OnSerialize $BA4
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $BAF
#cmd LNPlaneMesh_SetPrototype_OnUpdate $BBA
#cmd LNStaticMesh_Load $324
#cmd LNStaticMesh_GetModel $326
#cmd LNStaticMesh_MakeCollisionBody $327
#cmd LNStaticMesh_SetPrototype_OnSerialize $BC5
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $BD0
#cmd LNStaticMesh_SetPrototype_OnUpdate $BDB
#cmd LNStaticMeshComponent_Create $32E
#cmd LNStaticMeshComponent_SetModel $32A
#cmd LNStaticMeshComponent_MakeCollisionBody $32C
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $BE6
#cmd LNSkinnedMeshComponent_Create $332
#cmd LNSkinnedMeshComponent_SetModel $330
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $BF1
#cmd LNCollision_GetWorldObject $334
#cmd LNCollision_SetPrototype_OnSerialize $BFC
#cmd LNTriggerBodyComponent_Create $338
#cmd LNTriggerBodyComponent_AddCollisionShape $336
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $C07
#cmd LNParticleEmitter_Create $33A
#cmd LNParticleEmitter_SetPrototype_OnSerialize $C12
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $C1D
#cmd LNParticleEmitter_SetPrototype_OnUpdate $C28
#cmd LNScene_SetClearMode $33D
#cmd LNScene_SetSkyColor $33F
#cmd LNScene_SetSkyHorizonColor $341
#cmd LNScene_SetSkyCloudColor $343
#cmd LNScene_SetSkyOverlayColor $345
#cmd LNScene_GotoLevel $347
#cmd LNScene_CallLevel $34A
#cmd LNScene_ReturnLevel $34D
#cmd LNScene_ActiveLevel $34F
#cmd LNScene_IsTransitionEffectRunning $350
#cmd LNScene_SetTransitionEffectMode $351
#cmd LNScene_TransitionEffectMode $353
#cmd LNScene_SetTransitionDuration $354
#cmd LNScene_TransitionDuration $356
#cmd LNScene_SetTransitionEffectColor $357
#cmd LNScene_TransitionEffectColor $359
#cmd LNScene_SetTransitionEffectMaskTexture $35A
#cmd LNScene_TransitionEffectMaskTexture $35C
#cmd LNScene_SetTransitionEffectVague $35D
#cmd LNScene_TransitionEffectVague $35F
#cmd LNScene_StartFadeOut $360
#cmd LNScene_StartFadeIn $361
#cmd LNScene_SetHDREnabled $362
#cmd LNScene_IsHDREnabled $364
#cmd LNScene_SetScreenBlendColor $365
#cmd LNScene_ScreenBlendColor $367
#cmd LNScene_SetAntialiasEnabled $368
#cmd LNScene_IsAntialiasEnabled $36A
#cmd LNScene_SetSSREnabled $36B
#cmd LNScene_IsSSREnabled $36D
#cmd LNScene_SetSSAOEnabled $36E
#cmd LNScene_IsSSAOEnabled $370
#cmd LNScene_SetBloomEnabled $371
#cmd LNScene_IsBloomEnabled $373
#cmd LNScene_SetDOFEnabled $374
#cmd LNScene_IsDOFEnabled $376
#cmd LNScene_SetTonemapEnabled $377
#cmd LNScene_IsTonemapEnabled $379
#cmd LNScene_SetVignetteEnabled $37A
#cmd LNScene_IsVignetteEnabled $37C
#cmd LNScene_SetGammaEnabled $37D
#cmd LNScene_IsGammaEnabled $37F
#cmd LNLevel_Create $38F
#cmd LNLevel_AddObject $381
#cmd LNLevel_RemoveObject $383
#cmd LNLevel_AddSubLevel $385
#cmd LNLevel_RemoveSubLevel $387
#cmd LNLevel_RemoveAllSubLevels $389
#cmd LNLevel_SetPrototype_OnSerialize $C33
#cmd LNLevel_SetPrototype_OnStart $C3E
#cmd LNLevel_SetPrototype_OnStop $C49
#cmd LNLevel_SetPrototype_OnPause $C54
#cmd LNLevel_SetPrototype_OnResume $C5F
#cmd LNLevel_SetPrototype_OnUpdate $C6A
#cmd LNUIEventArgs_Sender $39A
#cmd LNUIEventArgs_SetPrototype_OnSerialize $C75
#cmd LNUIGeneralEventHandler_Create $803
#cmd LNUIEventHandler_Create $808
#cmd LNUILayoutElement_SetPrototype_OnSerialize $C80
#cmd LNUIElement_SetSize $3AC
#cmd LNUIElement_SetSizeWH $3AE
#cmd LNUIElement_SetWidth $3B1
#cmd LNUIElement_GetWidth $3B3
#cmd LNUIElement_SetHeight $3B4
#cmd LNUIElement_GetHeight $3B6
#cmd LNUIElement_SetMargin $3B7
#cmd LNUIElement_GetMargin $3B9
#cmd LNUIElement_SetPadding $3BA
#cmd LNUIElement_GetPadding $3BC
#cmd LNUIElement_SetHAlignment $3BD
#cmd LNUIElement_GetHAlignment $3BF
#cmd LNUIElement_SetVAlignment $3C0
#cmd LNUIElement_GetVAlignment $3C2
#cmd LNUIElement_SetAlignments $3C3
#cmd LNUIElement_SetPosition $3C6
#cmd LNUIElement_SetPositionXYZ $3C8
#cmd LNUIElement_GetPosition $3CC
#cmd LNUIElement_SetRotation $3CD
#cmd LNUIElement_SetEulerAngles $3CF
#cmd LNUIElement_GetRotation $3D3
#cmd LNUIElement_SetScale $3D4
#cmd LNUIElement_SetScaleS $3D6
#cmd LNUIElement_SetScaleXY $3D8
#cmd LNUIElement_GetScale $3DB
#cmd LNUIElement_SetCenterPoint $3DC
#cmd LNUIElement_SetCenterPointXYZ $3DE
#cmd LNUIElement_GetCenterPoint $3E2
#cmd LNUIElement_SetEnabled $3E3
#cmd LNUIElement_IsEnabled $3E5
#cmd LNUIElement_SetData $3E6
#cmd LNUIElement_GetData $3E8
#cmd LNUIElement_SetBackgroundColor $3E9
#cmd LNUIElement_GetBackgroundColor $3EB
#cmd LNUIElement_SetBorderThickness $3EC
#cmd LNUIElement_GetBorderThickness $3EE
#cmd LNUIElement_SetBorderColor $3EF
#cmd LNUIElement_GetBorderColor $3F1
#cmd LNUIElement_SetCornerRadius $3F2
#cmd LNUIElement_GetCornerRadius $3F4
#cmd LNUIElement_SetVisibility $3F5
#cmd LNUIElement_GetVisibility $3F7
#cmd LNUIElement_SetOpacity $3F8
#cmd LNUIElement_GetOpacity $3FA
#cmd LNUIElement_AddChild $3FB
#cmd LNUIElement_SetPrototype_OnSerialize $C8B
#cmd LNUITextBlock_Create $401
#cmd LNUITextBlock_CreateWithText $402
#cmd LNUITextBlock_SetText $3FE
#cmd LNUITextBlock_GetText $400
#cmd LNUITextBlock_SetPrototype_OnSerialize $C96
#cmd LNUISprite_Create $40F
#cmd LNUISprite_CreateWithTexture $410
#cmd LNUISprite_SetTexture $405
#cmd LNUISprite_SetSourceRect $407
#cmd LNUISprite_SetSourceRectXYWH $409
#cmd LNUISprite_GetSourceRect $40E
#cmd LNUISprite_SetPrototype_OnSerialize $CA1
#cmd LNUIIcon_LoadFontIcon $413
#cmd LNUIIcon_LoadFontIconWithNameSize $415
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $418
#cmd LNUIIcon_SetPrototype_OnSerialize $CAC
#cmd LNUIMessageTextArea_Create $421
#cmd LNUIMessageTextArea_SetText $41D
#cmd LNUIMessageTextArea_SetTypingSpeed $41F
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $CB7
#cmd LNUI_Add $423
#cmd LNUI_Remove $425
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $CC2
#cmd LNUIBoxLayout_Create $42C
#cmd LNUIBoxLayout_SetOrientation $429
#cmd LNUIBoxLayout_GetOrientation $42B
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $CCD
#cmd LNUIGridLayout_Create $43C
#cmd LNUIGridLayout_SetColumnCount $42E
#cmd LNUIGridLayout_SetRow $430
#cmd LNUIGridLayout_SetColumn $433
#cmd LNUIGridLayout_SetPlacement $436
#cmd LNUIGridLayout_SetPrototype_OnSerialize $CD8
#cmd LNUIControl_Create $448
#cmd LNUIControl_AddInlineVisual $449
#cmd LNUIControl_SetPrototype_OnSerialize $CE3
#cmd LNUIButtonBase_SetText $44D
#cmd LNUIButtonBase_SetPrototype_OnSerialize $CEE
#cmd LNUIButton_Create $450
#cmd LNUIButton_CreateWithText $451
#cmd LNUIButton_ConnectOnClicked $453
#cmd LNUIButton_SetPrototype_OnSerialize $CF9
#cmd LNUIWindow_Create $456
#cmd LNUIWindow_SetPrototype_OnSerialize $D04
#cmd LNUIListItem_ConnectOnSubmit $45B
#cmd LNUIListItem_SetPrototype_OnSerialize $D0F
#cmd LNUIListItemsControl_SetItemsLayoutPanel $45E
#cmd LNUIListItemsControl_SetSubmitMode $460
#cmd LNUIListItemsControl_GetSubmitMode $462
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $D1A
#cmd LNUIListBoxItem_Create $464
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $D25
#cmd LNUIListBox_Create $469
#cmd LNUIListBox_AddItem $467
#cmd LNUIListBox_SetPrototype_OnSerialize $D30
#cmd LNInputGesture_SetPrototype_OnSerialize $D3B
#cmd LNKeyGesture_Create $46C
#cmd LNKeyGesture_SetPrototype_OnSerialize $D46
#cmd LNInput_IsPressed $46F
#cmd LNInput_IsTriggered $471
#cmd LNInput_IsTriggeredOff $473
#cmd LNInput_IsRepeated $475
#cmd LNInput_GetAxisValue $477
#cmd LNInput_AddBinding $479
#cmd LNInput_RemoveBinding $47C
#cmd LNInput_ClearBindings $47E
#cmd LNInput_ClearAllBindings $480
#cmd LNMouse_Pressed $482
#cmd LNMouse_Triggered $484
#cmd LNMouse_TriggeredOff $486
#cmd LNMouse_Repeated $488
#cmd LNMouse_Position $48A
#cmd LNInterpreterCommand_Code $48C
#cmd LNInterpreterCommand_ParamsCount $48D
#cmd LNInterpreterCommand_Param $48E
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $D51
#cmd LNInterpreterCommandList_Create $4A5
#cmd LNInterpreterCommandList_AddCommand $491
#cmd LNInterpreterCommandList_AddCommand1 $493
#cmd LNInterpreterCommandList_AddCommand2 $496
#cmd LNInterpreterCommandList_AddCommand3 $49A
#cmd LNInterpreterCommandList_AddCommand4 $49F
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $D5C
#cmd LNInterpreterCommandDelegate_Create $8C1
#cmd LNInterpreter_Create $4B9
#cmd LNInterpreter_Clear $4AA
#cmd LNInterpreter_Run $4AB
#cmd LNInterpreter_IsRunning $4AD
#cmd LNInterpreter_Update $4AE
#cmd LNInterpreter_Terminate $4AF
#cmd LNInterpreter_RegisterCommandHandler $4B0
#cmd LNInterpreter_SetWaitMode $4B3
#cmd LNInterpreter_GetWaitMode $4B5
#cmd LNInterpreter_SetWaitCount $4B6
#cmd LNInterpreter_GetWaitCount $4B8
#cmd LNInterpreter_SetPrototype_OnSerialize $D67
#cmd LNInterpreter_SetPrototype_OnUpdateWait $D73
#cmd LNEngineSettings_SetMainWindowSize $4BC
#cmd LNEngineSettings_SetMainWorldViewSize $4BF
#cmd LNEngineSettings_SetMainWindowTitle $4C2
#cmd LNEngineSettings_AddAssetDirectory $4C4
#cmd LNEngineSettings_AddAssetArchive $4C6
#cmd LNEngineSettings_SetFrameRate $4C9
#cmd LNEngineSettings_SetUITheme $4CB
#cmd LNEngineSettings_SetFontFile $4CD
#cmd LNEngineSettings_SetDebugToolEnabled $4CF
#cmd LNEngineSettings_SetEngineLogEnabled $4D1
#cmd LNEngineSettings_SetEngineLogFilePath $4D3
#cmd LNEngineSettings_SetDeveloperToolEnabled $4D5
#cmd LNEngine_Initialize $4D8
#cmd LNEngine_Finalize $4D9
#cmd LNEngine_Update $4DA
#cmd LNEngine_Run $4DB
#cmd LNEngine_Time $4DD
#cmd LNEngine_GetCamera $4DE
#cmd LNEngine_GetMainLight $4DF
#cmd LNEngine_GetRenderView $4E0
#cmd LNApplication_Create $4E5
#cmd LNApplication_OnInit $4E2
#cmd LNApplication_OnUpdate $4E3
#cmd LNApplication_World $4E4
#cmd LNApplication_SetPrototype_OnSerialize $D7E
#cmd LNApplication_SetPrototype_OnInit $D89
#cmd LNApplication_SetPrototype_OnUpdate $D94
#cmd LNDebug_SetGuideGridEnabled $4E7
#cmd LNDebug_SetPhysicsDebugDrawEnabled $4E9
#cmd LNDebug_Print $4EB
#cmd LNDebug_PrintWithTime $4ED
#cmd LNDebug_PrintWithTimeAndColor $4F0
#cmd LNObjectSerializeHandler_Create $8ED
#cmd LNEventConnectionSerializeHandler_Create $8F8
#cmd LNVariantSerializeHandler_Create $903
#cmd LNZVTestClass1SerializeHandler_Create $90E
#cmd LNZVTestEventArgs1SerializeHandler_Create $919
#cmd LNSerializer2SerializeHandler_Create $924
#cmd LNAssetObjectSerializeHandler_Create $92F
#cmd LNAssetImportSettingsSerializeHandler_Create $93A
#cmd LNAssetModelSerializeHandler_Create $945
#cmd LNSoundSerializeHandler_Create $950
#cmd LNTextureSerializeHandler_Create $95B
#cmd LNTexture2DSerializeHandler_Create $966
#cmd LNShaderSerializeHandler_Create $971
#cmd LNRenderViewSerializeHandler_Create $97C
#cmd LNMaterialSerializeHandler_Create $987
#cmd LNMeshNodeSerializeHandler_Create $992
#cmd LNStaticMeshModelSerializeHandler_Create $99D
#cmd LNSkinnedMeshModelSerializeHandler_Create $9A8
#cmd LNAnimationControllerSerializeHandler_Create $9B3
#cmd LNCollisionShapeSerializeHandler_Create $9BE
#cmd LNBoxCollisionShapeSerializeHandler_Create $9C9
#cmd LNAnimationClipSerializeHandler_Create $9D4
#cmd LNAnimationStateSerializeHandler_Create $9DF
#cmd LNEffectResourceSerializeHandler_Create $9EA
#cmd LNParticleEmitterModelSerializeHandler_Create $9F5
#cmd LNParticleModelSerializeHandler_Create $A00
#cmd LNComponentSerializeHandler_Create $A0B
#cmd LNVisualComponentSerializeHandler_Create $A16
#cmd LNSpriteComponentSerializeHandler_Create $A21
#cmd LNCharacterControllerSerializeHandler_Create $A2C
#cmd LNWorldSerializeHandler_Create $A37
#cmd LNComponentListSerializeHandler_Create $A42
#cmd LNWorldObjectSerializeHandler_Create $A4D
#cmd LNWorldObjectPreUpdateHandler_Create $A58
#cmd LNWorldObjectUpdateHandler_Create $A63
#cmd LNVisualObjectSerializeHandler_Create $A6E
#cmd LNVisualObjectPreUpdateHandler_Create $A79
#cmd LNVisualObjectUpdateHandler_Create $A84
#cmd LNCameraSerializeHandler_Create $A8F
#cmd LNCameraPreUpdateHandler_Create $A9A
#cmd LNCameraUpdateHandler_Create $AA5
#cmd LNEnvironmentLightSerializeHandler_Create $AB0
#cmd LNEnvironmentLightPreUpdateHandler_Create $ABB
#cmd LNEnvironmentLightUpdateHandler_Create $AC6
#cmd LNDirectionalLightSerializeHandler_Create $AD1
#cmd LNDirectionalLightPreUpdateHandler_Create $ADC
#cmd LNDirectionalLightUpdateHandler_Create $AE7
#cmd LNPointLightSerializeHandler_Create $AF2
#cmd LNPointLightPreUpdateHandler_Create $AFD
#cmd LNPointLightUpdateHandler_Create $B08
#cmd LNSpotLightSerializeHandler_Create $B13
#cmd LNSpotLightPreUpdateHandler_Create $B1E
#cmd LNSpotLightUpdateHandler_Create $B29
#cmd LNSpriteSerializeHandler_Create $B34
#cmd LNSpritePreUpdateHandler_Create $B3F
#cmd LNSpriteUpdateHandler_Create $B4A
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $B55
#cmd LNRaycasterSerializeHandler_Create $B60
#cmd LNRaycastResultSerializeHandler_Create $B6B
#cmd LNWorldRenderViewSerializeHandler_Create $B76
#cmd LNBoxMeshSerializeHandler_Create $B81
#cmd LNBoxMeshPreUpdateHandler_Create $B8C
#cmd LNBoxMeshUpdateHandler_Create $B97
#cmd LNPlaneMeshSerializeHandler_Create $BA2
#cmd LNPlaneMeshPreUpdateHandler_Create $BAD
#cmd LNPlaneMeshUpdateHandler_Create $BB8
#cmd LNStaticMeshSerializeHandler_Create $BC3
#cmd LNStaticMeshPreUpdateHandler_Create $BCE
#cmd LNStaticMeshUpdateHandler_Create $BD9
#cmd LNStaticMeshComponentSerializeHandler_Create $BE4
#cmd LNSkinnedMeshComponentSerializeHandler_Create $BEF
#cmd LNCollisionSerializeHandler_Create $BFA
#cmd LNTriggerBodyComponentSerializeHandler_Create $C05
#cmd LNParticleEmitterSerializeHandler_Create $C10
#cmd LNParticleEmitterPreUpdateHandler_Create $C1B
#cmd LNParticleEmitterUpdateHandler_Create $C26
#cmd LNLevelSerializeHandler_Create $C31
#cmd LNLevelStartHandler_Create $C3C
#cmd LNLevelStopHandler_Create $C47
#cmd LNLevelPauseHandler_Create $C52
#cmd LNLevelResumeHandler_Create $C5D
#cmd LNLevelUpdateHandler_Create $C68
#cmd LNUIEventArgsSerializeHandler_Create $C73
#cmd LNUILayoutElementSerializeHandler_Create $C7E
#cmd LNUIElementSerializeHandler_Create $C89
#cmd LNUITextBlockSerializeHandler_Create $C94
#cmd LNUISpriteSerializeHandler_Create $C9F
#cmd LNUIIconSerializeHandler_Create $CAA
#cmd LNUIMessageTextAreaSerializeHandler_Create $CB5
#cmd LNUILayoutPanelSerializeHandler_Create $CC0
#cmd LNUIBoxLayoutSerializeHandler_Create $CCB
#cmd LNUIGridLayoutSerializeHandler_Create $CD6
#cmd LNUIControlSerializeHandler_Create $CE1
#cmd LNUIButtonBaseSerializeHandler_Create $CEC
#cmd LNUIButtonSerializeHandler_Create $CF7
#cmd LNUIWindowSerializeHandler_Create $D02
#cmd LNUIListItemSerializeHandler_Create $D0D
#cmd LNUIListItemsControlSerializeHandler_Create $D18
#cmd LNUIListBoxItemSerializeHandler_Create $D23
#cmd LNUIListBoxSerializeHandler_Create $D2E
#cmd LNInputGestureSerializeHandler_Create $D39
#cmd LNKeyGestureSerializeHandler_Create $D44
#cmd LNInterpreterCommandSerializeHandler_Create $D4F
#cmd LNInterpreterCommandListSerializeHandler_Create $D5A
#cmd LNInterpreterSerializeHandler_Create $D65
#cmd LNInterpreterUpdateWaitHandler_Create $D71
#cmd LNApplicationSerializeHandler_Create $D7C
#cmd LNApplicationInitHandler_Create $D87
#cmd LNApplicationUpdateHandler_Create $D92



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
