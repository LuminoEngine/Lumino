
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
#cmd LNVector3_GetX $4E5
#cmd LNVector3_SetX $4E7
#cmd LNVector3_GetY $4E8
#cmd LNVector3_SetY $4EA
#cmd LNVector3_GetZ $4EB
#cmd LNVector3_SetZ $4ED
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $504
#cmd LNVector4_SetX $506
#cmd LNVector4_GetY $507
#cmd LNVector4_SetY $509
#cmd LNVector4_GetZ $50A
#cmd LNVector4_SetZ $50C
#cmd LNVector4_GetW $50D
#cmd LNVector4_SetW $50F
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $522
#cmd LNQuaternion_SetX $524
#cmd LNQuaternion_GetY $525
#cmd LNQuaternion_SetY $527
#cmd LNQuaternion_GetZ $528
#cmd LNQuaternion_SetZ $52A
#cmd LNQuaternion_GetW $52B
#cmd LNQuaternion_SetW $52D
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $541
#cmd LNMatrix_SetRow0 $543
#cmd LNMatrix_GetRow1 $544
#cmd LNMatrix_SetRow1 $546
#cmd LNMatrix_GetRow2 $547
#cmd LNMatrix_SetRow2 $549
#cmd LNMatrix_GetRow3 $54A
#cmd LNMatrix_SetRow3 $54C
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $173
#cmd LNColor_GetR $5DE
#cmd LNColor_SetR $5E0
#cmd LNColor_GetG $5E1
#cmd LNColor_SetG $5E3
#cmd LNColor_GetB $5E4
#cmd LNColor_SetB $5E6
#cmd LNColor_GetA $5E7
#cmd LNColor_SetA $5E9
#cmd LNColor_SetZeros $178
#cmd LNColor_Set $179
#cmd LNPoint $17E
#cmd LNPoint_GetX $5FD
#cmd LNPoint_SetX $5FF
#cmd LNPoint_GetY $600
#cmd LNPoint_SetY $602
#cmd LNPoint_SetZeros $181
#cmd LNPoint_Set $182
#cmd LNSize $185
#cmd LNSize_GetWidth $60D
#cmd LNSize_SetWidth $60F
#cmd LNSize_GetHeight $610
#cmd LNSize_SetHeight $612
#cmd LNSize_SetZeros $188
#cmd LNSize_Set $189
#cmd LNRect $18C
#cmd LNRect_GetX $61D
#cmd LNRect_SetX $61F
#cmd LNRect_GetY $620
#cmd LNRect_SetY $622
#cmd LNRect_GetWidth $623
#cmd LNRect_SetWidth $625
#cmd LNRect_GetHeight $626
#cmd LNRect_SetHeight $628
#cmd LNRect_SetZeros $191
#cmd LNRect_Set $192
#cmd LNRect_GetLeft $197
#cmd LNRect_SetSize $198
#cmd LNRect_GetSize $19A
#cmd LNThickness $19B
#cmd LNThickness_GetLeft $641
#cmd LNThickness_SetLeft $643
#cmd LNThickness_GetTop $644
#cmd LNThickness_SetTop $646
#cmd LNThickness_GetRight $647
#cmd LNThickness_SetRight $649
#cmd LNThickness_GetBottom $64A
#cmd LNThickness_SetBottom $64C
#cmd LNThickness_SetZeros $1A0
#cmd LNThickness_Set $1A1
#cmd LNCornerRadius $1A6
#cmd LNCornerRadius_GetTopleft $65F
#cmd LNCornerRadius_SetTopleft $661
#cmd LNCornerRadius_GetTopright $662
#cmd LNCornerRadius_SetTopright $664
#cmd LNCornerRadius_GetBottomright $665
#cmd LNCornerRadius_SetBottomright $667
#cmd LNCornerRadius_GetBottomleft $668
#cmd LNCornerRadius_SetBottomleft $66A
#cmd LNCornerRadius_SetZeros $1AB
#cmd LNCornerRadius_Set $1AC

#cmd LNObject_Release $7A
#cmd LNObject_Retain $7B
#cmd LNObject_GetReferenceCount $7C
#cmd LNObject_SetPrototype_OnSerialize $8D3
#cmd LNEventConnection_SetPrototype_OnSerialize $8DE
#cmd LNPromiseFailureDelegate_Create $567
#cmd LNVariant_Create $81
#cmd LNVariant_SetInt $82
#cmd LNVariant_GetInt $84
#cmd LNVariant_SetPrototype_OnSerialize $8E9
#cmd LNZVTestDelegate1_Create $570
#cmd LNZVTestDelegate2_Create $576
#cmd LNZVTestDelegate3_Create $57B
#cmd LNZVTestEventHandler1_Create $580
#cmd LNZVTestEventHandler2_Create $585
#cmd LNZVTestPromise1_ThenWith $588
#cmd LNZVTestPromise1_CatchWith $58A
#cmd LNZVTestPromise2_ThenWith $58E
#cmd LNZVTestPromise2_CatchWith $590
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $8F4
#cmd LNZVTestEventArgs1_Create $B0
#cmd LNZVTestEventArgs1_CreateWithValue $B1
#cmd LNZVTestEventArgs1_GetValue $AF
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $8FF
#cmd LNSerializer2_SetPrototype_OnSerialize $90A
#cmd LNAssetObject_SetPrototype_OnSerialize $915
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $920
#cmd LNAssetModel_Create $113
#cmd LNAssetModel_Target $112
#cmd LNAssetModel_SetPrototype_OnSerialize $92B
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
#cmd LNSound_SetPrototype_OnSerialize $936
#cmd LNAudio_PlayBGM $13F
#cmd LNAudio_StopBGM $144
#cmd LNAudio_PlayBGS $146
#cmd LNAudio_StopBGS $14B
#cmd LNAudio_PlayME $14D
#cmd LNAudio_StopME $151
#cmd LNAudio_PlaySE $152
#cmd LNAudio_PlaySE3D $156
#cmd LNAudio_StopSE $15C
#cmd LNTexture2DDelegate_Create $5D6
#cmd LNTexture2DPromise_ThenWith $5D9
#cmd LNTexture2DPromise_CatchWith $5DB
#cmd LNTexture_SetPrototype_OnSerialize $941
#cmd LNTexture2D_Create $1B7
#cmd LNTexture2D_CreateWithFormat $1BA
#cmd LNTexture2D_Load $1B3
#cmd LNTexture2D_LoadEmoji $1B5
#cmd LNTexture2D_SetPrototype_OnSerialize $94C
#cmd LNShader_Load $1BF
#cmd LNShader_SetFloat $1C2
#cmd LNShader_SetVector3 $1C5
#cmd LNShader_SetVector4 $1C8
#cmd LNShader_SetPrototype_OnSerialize $957
#cmd LNRenderView_SetPrototype_OnSerialize $962
#cmd LNMaterial_Create $1DF
#cmd LNMaterial_SetMainTexture $1D6
#cmd LNMaterial_GetMainTexture $1D8
#cmd LNMaterial_SetShadingModel $1D9
#cmd LNMaterial_GetShadingModel $1DB
#cmd LNMaterial_SetShader $1DC
#cmd LNMaterial_GetShader $1DE
#cmd LNMaterial_SetPrototype_OnSerialize $96D
#cmd LNStaticMeshModel_Load $1E1
#cmd LNStaticMeshModel_MaterialCount $1E3
#cmd LNStaticMeshModel_Material $1E4
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $978
#cmd LNSkinnedMeshModel_Load $1E7
#cmd LNSkinnedMeshModel_GetAnimationController $1E9
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $983
#cmd LNAnimationController_AddClip $1EB
#cmd LNAnimationController_Play $1ED
#cmd LNAnimationController_SetPrototype_OnSerialize $98E
#cmd LNCollisionShape_SetPrototype_OnSerialize $999
#cmd LNBoxCollisionShape_Create $1F2
#cmd LNBoxCollisionShape_CreateWHD $1F4
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $9A4
#cmd LNAnimationClip_Load $1FE
#cmd LNAnimationClip_SetHierarchicalAnimationMode $200
#cmd LNAnimationClip_HierarchicalAnimationMode $202
#cmd LNAnimationClip_SetPrototype_OnSerialize $9AF
#cmd LNAnimationState_SetPrototype_OnSerialize $9BA
#cmd LNEffectResource_SetPrototype_OnSerialize $9C5
#cmd LNParticleEmitterModel_Create $223
#cmd LNParticleEmitterModel_SetMaxParticles $20F
#cmd LNParticleEmitterModel_SetSpawnRate $211
#cmd LNParticleEmitterModel_SetLifeTime $213
#cmd LNParticleEmitterModel_SetupBoxShape $215
#cmd LNParticleEmitterModel_SetSize $217
#cmd LNParticleEmitterModel_SetForwardVelocityMin $219
#cmd LNParticleEmitterModel_SetForwardVelocityMax $21B
#cmd LNParticleEmitterModel_SetForwardScale $21D
#cmd LNParticleEmitterModel_SetGeometryDirection $21F
#cmd LNParticleEmitterModel_SetupSpriteModule $221
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $9D0
#cmd LNParticleModel_Create $22A
#cmd LNParticleModel_SetLoop $225
#cmd LNParticleModel_IsLoop $227
#cmd LNParticleModel_AddEmitter $228
#cmd LNParticleModel_SetPrototype_OnSerialize $9DB
#cmd LNComponent_SetPrototype_OnSerialize $9E6
#cmd LNVisualComponent_SetVisible $22D
#cmd LNVisualComponent_IsVisible $22F
#cmd LNVisualComponent_SetPrototype_OnSerialize $9F1
#cmd LNSpriteComponent_SetTexture $231
#cmd LNSpriteComponent_SetPrototype_OnSerialize $9FC
#cmd LNCollisionEventHandler_Create $6CB
#cmd LNCharacterController_Create $24E
#cmd LNCharacterController_SetVelocity $23B
#cmd LNCharacterController_GetVelocity $23D
#cmd LNCharacterController_SetInputControlEnabled $23E
#cmd LNCharacterController_SetCameraControlEnabled $240
#cmd LNCharacterController_SetHeight $242
#cmd LNCharacterController_GetHeight $244
#cmd LNCharacterController_SetCameraRadius $245
#cmd LNCharacterController_GetCameraRadius $247
#cmd LNCharacterController_SetCollisionEnter $248
#cmd LNCharacterController_SetCollisionLeave $24A
#cmd LNCharacterController_SetCollisionStay $24C
#cmd LNCharacterController_SetPrototype_OnSerialize $A07
#cmd LNWorld_Add $250
#cmd LNWorld_SetPrototype_OnSerialize $A12
#cmd LNComponentList_GetLength $6E6
#cmd LNComponentList_GetItem $6E8
#cmd LNComponentList_SetPrototype_OnSerialize $A1D
#cmd LNWorldObject_Create $287
#cmd LNWorldObject_SetPosition $254
#cmd LNWorldObject_SetPositionXYZ $256
#cmd LNWorldObject_GetPosition $25A
#cmd LNWorldObject_SetRotationQuaternion $25B
#cmd LNWorldObject_SetRotation $25D
#cmd LNWorldObject_GetRotation $261
#cmd LNWorldObject_SetScale $262
#cmd LNWorldObject_SetScaleS $264
#cmd LNWorldObject_SetScaleXYZ $266
#cmd LNWorldObject_GetScale $26A
#cmd LNWorldObject_SetCenterPoint $26B
#cmd LNWorldObject_SetCenterPointXYZ $26D
#cmd LNWorldObject_GetCenterPoint $271
#cmd LNWorldObject_LookAt $272
#cmd LNWorldObject_LookAtXYZ $274
#cmd LNWorldObject_AddComponent $278
#cmd LNWorldObject_RemoveComponent $27A
#cmd LNWorldObject_AddTag $27C
#cmd LNWorldObject_RemoveTag $27E
#cmd LNWorldObject_HasTag $280
#cmd LNWorldObject_Destroy $282
#cmd LNWorldObject_GetComponents $283
#cmd LNWorldObject_SetPrototype_OnSerialize $A28
#cmd LNWorldObject_SetPrototype_OnPreUpdate $A33
#cmd LNWorldObject_SetPrototype_OnUpdate $A3E
#cmd LNVisualObject_SetVisible $289
#cmd LNVisualObject_IsVisible $28B
#cmd LNVisualObject_SetOpacity $28C
#cmd LNVisualObject_GetOpacity $28E
#cmd LNVisualObject_SetPrototype_OnSerialize $A49
#cmd LNVisualObject_SetPrototype_OnPreUpdate $A54
#cmd LNVisualObject_SetPrototype_OnUpdate $A5F
#cmd LNCamera_SetPrototype_OnSerialize $A6A
#cmd LNCamera_SetPrototype_OnPreUpdate $A75
#cmd LNCamera_SetPrototype_OnUpdate $A80
#cmd LNEnvironmentLight_SetEnabled $291
#cmd LNEnvironmentLight_IsEnabled $293
#cmd LNEnvironmentLight_SetColor $294
#cmd LNEnvironmentLight_GetColor $296
#cmd LNEnvironmentLight_SetAmbientColor $297
#cmd LNEnvironmentLight_GetAmbientColor $299
#cmd LNEnvironmentLight_GetSkyColor $29A
#cmd LNEnvironmentLight_SetSkyColor $29B
#cmd LNEnvironmentLight_GetGroundColor $29D
#cmd LNEnvironmentLight_SetGroundColor $29E
#cmd LNEnvironmentLight_SetIntensity $2A0
#cmd LNEnvironmentLight_GetIntensity $2A2
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $2A3
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $2A5
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $2A6
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $2A8
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $A8B
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $A96
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $AA1
#cmd LNDirectionalLight_Create $2B9
#cmd LNDirectionalLight_CreateWithColor $2BA
#cmd LNDirectionalLight_SetEnabled $2AA
#cmd LNDirectionalLight_IsEnabled $2AC
#cmd LNDirectionalLight_SetColor $2AD
#cmd LNDirectionalLight_GetColor $2AF
#cmd LNDirectionalLight_SetIntensity $2B0
#cmd LNDirectionalLight_GetIntensity $2B2
#cmd LNDirectionalLight_SetShadowEffectiveDistance $2B3
#cmd LNDirectionalLight_GetShadowEffectiveDistance $2B5
#cmd LNDirectionalLight_SetShadowEffectiveDepth $2B6
#cmd LNDirectionalLight_GetShadowEffectiveDepth $2B8
#cmd LNDirectionalLight_SetPrototype_OnSerialize $AAC
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $AB7
#cmd LNDirectionalLight_SetPrototype_OnUpdate $AC2
#cmd LNPointLight_Create $2CC
#cmd LNPointLight_CreateWithColorAndRange $2CD
#cmd LNPointLight_SetEnabled $2BD
#cmd LNPointLight_IsEnabled $2BF
#cmd LNPointLight_SetColor $2C0
#cmd LNPointLight_GetColor $2C2
#cmd LNPointLight_SetIntensity $2C3
#cmd LNPointLight_GetIntensity $2C5
#cmd LNPointLight_SetRange $2C6
#cmd LNPointLight_GetRange $2C8
#cmd LNPointLight_SetAttenuation $2C9
#cmd LNPointLight_GetAttenuation $2CB
#cmd LNPointLight_SetPrototype_OnSerialize $ACD
#cmd LNPointLight_SetPrototype_OnPreUpdate $AD8
#cmd LNPointLight_SetPrototype_OnUpdate $AE3
#cmd LNSpotLight_Create $2E6
#cmd LNSpotLight_CreateWithColorAndRange $2E7
#cmd LNSpotLight_SetEnabled $2D1
#cmd LNSpotLight_IsEnabled $2D3
#cmd LNSpotLight_SetColor $2D4
#cmd LNSpotLight_GetColor $2D6
#cmd LNSpotLight_SetIntensity $2D7
#cmd LNSpotLight_GetIntensity $2D9
#cmd LNSpotLight_SetRange $2DA
#cmd LNSpotLight_GetRange $2DC
#cmd LNSpotLight_SetAttenuation $2DD
#cmd LNSpotLight_GetAttenuation $2DF
#cmd LNSpotLight_SetAngle $2E0
#cmd LNSpotLight_GetAngle $2E2
#cmd LNSpotLight_SetPenumbra $2E3
#cmd LNSpotLight_GetPenumbra $2E5
#cmd LNSpotLight_SetPrototype_OnSerialize $AEE
#cmd LNSpotLight_SetPrototype_OnPreUpdate $AF9
#cmd LNSpotLight_SetPrototype_OnUpdate $B04
#cmd LNTestDelegate_Create $792
#cmd LNSprite_Create $2FD
#cmd LNSprite_CreateWithTexture $2FE
#cmd LNSprite_CreateWithTextureAndSize $300
#cmd LNSprite_SetTexture $2EF
#cmd LNSprite_SetSize $2F1
#cmd LNSprite_SetSizeWH $2F3
#cmd LNSprite_SetSourceRectXYWH $2F6
#cmd LNSprite_SetCallerTest $2FB
#cmd LNSprite_SetPrototype_OnSerialize $B0F
#cmd LNSprite_SetPrototype_OnPreUpdate $B1A
#cmd LNSprite_SetPrototype_OnUpdate $B25
#cmd LNCameraOrbitControlComponent_Create $305
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $B30
#cmd LNRaycaster_FromScreen $307
#cmd LNRaycaster_IntersectPlane $309
#cmd LNRaycaster_SetPrototype_OnSerialize $B3B
#cmd LNRaycastResult_GetPoint $30E
#cmd LNRaycastResult_SetPrototype_OnSerialize $B46
#cmd LNWorldRenderView_SetGuideGridEnabled $310
#cmd LNWorldRenderView_GetGuideGridEnabled $312
#cmd LNWorldRenderView_SetPrototype_OnSerialize $B51
#cmd LNBoxMesh_Create $314
#cmd LNBoxMesh_SetPrototype_OnSerialize $B5C
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $B67
#cmd LNBoxMesh_SetPrototype_OnUpdate $B72
#cmd LNPlaneMesh_Create $316
#cmd LNPlaneMesh_SetPrototype_OnSerialize $B7D
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $B88
#cmd LNPlaneMesh_SetPrototype_OnUpdate $B93
#cmd LNStaticMesh_Load $318
#cmd LNStaticMesh_GetModel $31A
#cmd LNStaticMesh_MakeCollisionBody $31B
#cmd LNStaticMesh_SetPrototype_OnSerialize $B9E
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $BA9
#cmd LNStaticMesh_SetPrototype_OnUpdate $BB4
#cmd LNStaticMeshComponent_Create $322
#cmd LNStaticMeshComponent_SetModel $31E
#cmd LNStaticMeshComponent_MakeCollisionBody $320
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $BBF
#cmd LNSkinnedMeshComponent_Create $326
#cmd LNSkinnedMeshComponent_SetModel $324
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $BCA
#cmd LNCollision_GetWorldObject $328
#cmd LNCollision_SetPrototype_OnSerialize $BD5
#cmd LNTriggerBodyComponent_Create $32C
#cmd LNTriggerBodyComponent_AddCollisionShape $32A
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $BE0
#cmd LNParticleEmitter_Create $32E
#cmd LNParticleEmitter_SetPrototype_OnSerialize $BEB
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $BF6
#cmd LNParticleEmitter_SetPrototype_OnUpdate $C01
#cmd LNScene_SetClearMode $331
#cmd LNScene_SetSkyColor $333
#cmd LNScene_SetSkyHorizonColor $335
#cmd LNScene_SetSkyCloudColor $337
#cmd LNScene_SetSkyOverlayColor $339
#cmd LNScene_GotoLevel $33B
#cmd LNScene_CallLevel $33E
#cmd LNScene_ReturnLevel $341
#cmd LNScene_ActiveLevel $343
#cmd LNScene_IsTransitionEffectRunning $344
#cmd LNScene_SetTransitionEffectMode $345
#cmd LNScene_TransitionEffectMode $347
#cmd LNScene_SetTransitionDuration $348
#cmd LNScene_TransitionDuration $34A
#cmd LNScene_SetTransitionEffectColor $34B
#cmd LNScene_TransitionEffectColor $34D
#cmd LNScene_SetTransitionEffectMaskTexture $34E
#cmd LNScene_TransitionEffectMaskTexture $350
#cmd LNScene_SetTransitionEffectVague $351
#cmd LNScene_TransitionEffectVague $353
#cmd LNScene_StartFadeOut $354
#cmd LNScene_StartFadeIn $355
#cmd LNScene_SetScreenBlendColor $356
#cmd LNScene_ScreenBlendColor $358
#cmd LNScene_SetAntialiasEnabled $359
#cmd LNScene_IsAntialiasEnabled $35B
#cmd LNScene_SetSSREnabled $35C
#cmd LNScene_IsSSREnabled $35E
#cmd LNScene_SetSSAOEnabled $35F
#cmd LNScene_IsSSAOEnabled $361
#cmd LNScene_SetBloomEnabled $362
#cmd LNScene_IsBloomEnabled $364
#cmd LNScene_SetDOFEnabled $365
#cmd LNScene_IsDOFEnabled $367
#cmd LNScene_SetTonemapEnabled $368
#cmd LNScene_IsTonemapEnabled $36A
#cmd LNScene_SetVignetteEnabled $36B
#cmd LNScene_IsVignetteEnabled $36D
#cmd LNScene_SetGammaEnabled $36E
#cmd LNScene_IsGammaEnabled $370
#cmd LNLevel_Create $380
#cmd LNLevel_AddObject $372
#cmd LNLevel_RemoveObject $374
#cmd LNLevel_AddSubLevel $376
#cmd LNLevel_RemoveSubLevel $378
#cmd LNLevel_RemoveAllSubLevels $37A
#cmd LNLevel_SetPrototype_OnSerialize $C0C
#cmd LNLevel_SetPrototype_OnStart $C17
#cmd LNLevel_SetPrototype_OnStop $C22
#cmd LNLevel_SetPrototype_OnPause $C2D
#cmd LNLevel_SetPrototype_OnResume $C38
#cmd LNLevel_SetPrototype_OnUpdate $C43
#cmd LNUIEventArgs_Sender $38B
#cmd LNUIEventArgs_SetPrototype_OnSerialize $C4E
#cmd LNUIGeneralEventHandler_Create $7E7
#cmd LNUIEventHandler_Create $7EC
#cmd LNUILayoutElement_SetPrototype_OnSerialize $C59
#cmd LNUIElement_SetSize $39D
#cmd LNUIElement_SetSizeWH $39F
#cmd LNUIElement_SetWidth $3A2
#cmd LNUIElement_GetWidth $3A4
#cmd LNUIElement_SetHeight $3A5
#cmd LNUIElement_GetHeight $3A7
#cmd LNUIElement_SetMargin $3A8
#cmd LNUIElement_GetMargin $3AA
#cmd LNUIElement_SetPadding $3AB
#cmd LNUIElement_GetPadding $3AD
#cmd LNUIElement_SetHAlignment $3AE
#cmd LNUIElement_GetHAlignment $3B0
#cmd LNUIElement_SetVAlignment $3B1
#cmd LNUIElement_GetVAlignment $3B3
#cmd LNUIElement_SetAlignments $3B4
#cmd LNUIElement_SetPosition $3B7
#cmd LNUIElement_SetPositionXYZ $3B9
#cmd LNUIElement_GetPosition $3BD
#cmd LNUIElement_SetRotation $3BE
#cmd LNUIElement_SetEulerAngles $3C0
#cmd LNUIElement_GetRotation $3C4
#cmd LNUIElement_SetScale $3C5
#cmd LNUIElement_SetScaleS $3C7
#cmd LNUIElement_SetScaleXY $3C9
#cmd LNUIElement_GetScale $3CC
#cmd LNUIElement_SetCenterPoint $3CD
#cmd LNUIElement_SetCenterPointXYZ $3CF
#cmd LNUIElement_GetCenterPoint $3D3
#cmd LNUIElement_SetEnabled $3D4
#cmd LNUIElement_IsEnabled $3D6
#cmd LNUIElement_SetData $3D7
#cmd LNUIElement_GetData $3D9
#cmd LNUIElement_SetBackgroundColor $3DA
#cmd LNUIElement_GetBackgroundColor $3DC
#cmd LNUIElement_SetBorderThickness $3DD
#cmd LNUIElement_GetBorderThickness $3DF
#cmd LNUIElement_SetBorderColor $3E0
#cmd LNUIElement_GetBorderColor $3E2
#cmd LNUIElement_SetCornerRadius $3E3
#cmd LNUIElement_GetCornerRadius $3E5
#cmd LNUIElement_SetVisibility $3E6
#cmd LNUIElement_GetVisibility $3E8
#cmd LNUIElement_SetOpacity $3E9
#cmd LNUIElement_GetOpacity $3EB
#cmd LNUIElement_AddChild $3EC
#cmd LNUIElement_SetPrototype_OnSerialize $C64
#cmd LNUITextBlock_Create $3F2
#cmd LNUITextBlock_CreateWithText $3F3
#cmd LNUITextBlock_SetText $3EF
#cmd LNUITextBlock_GetText $3F1
#cmd LNUITextBlock_SetPrototype_OnSerialize $C6F
#cmd LNUISprite_Create $400
#cmd LNUISprite_CreateWithTexture $401
#cmd LNUISprite_SetTexture $3F6
#cmd LNUISprite_SetSourceRect $3F8
#cmd LNUISprite_SetSourceRectXYWH $3FA
#cmd LNUISprite_GetSourceRect $3FF
#cmd LNUISprite_SetPrototype_OnSerialize $C7A
#cmd LNUIIcon_LoadFontIcon $404
#cmd LNUIIcon_LoadFontIconWithNameSize $406
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $409
#cmd LNUIIcon_SetPrototype_OnSerialize $C85
#cmd LNUIMessageTextArea_Create $412
#cmd LNUIMessageTextArea_SetText $40E
#cmd LNUIMessageTextArea_SetTypingSpeed $410
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $C90
#cmd LNUI_Add $414
#cmd LNUI_Remove $416
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $C9B
#cmd LNUIBoxLayout_Create $41D
#cmd LNUIBoxLayout_SetOrientation $41A
#cmd LNUIBoxLayout_GetOrientation $41C
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $CA6
#cmd LNUIGridLayout_Create $42D
#cmd LNUIGridLayout_SetColumnCount $41F
#cmd LNUIGridLayout_SetRow $421
#cmd LNUIGridLayout_SetColumn $424
#cmd LNUIGridLayout_SetPlacement $427
#cmd LNUIGridLayout_SetPrototype_OnSerialize $CB1
#cmd LNUIControl_Create $439
#cmd LNUIControl_AddInlineVisual $43A
#cmd LNUIControl_SetPrototype_OnSerialize $CBC
#cmd LNUIButtonBase_SetText $43E
#cmd LNUIButtonBase_SetPrototype_OnSerialize $CC7
#cmd LNUIButton_Create $441
#cmd LNUIButton_CreateWithText $442
#cmd LNUIButton_ConnectOnClicked $444
#cmd LNUIButton_SetPrototype_OnSerialize $CD2
#cmd LNUIWindow_Create $447
#cmd LNUIWindow_SetPrototype_OnSerialize $CDD
#cmd LNUIListItem_ConnectOnSubmit $44C
#cmd LNUIListItem_SetPrototype_OnSerialize $CE8
#cmd LNUIListItemsControl_SetItemsLayoutPanel $44F
#cmd LNUIListItemsControl_SetSubmitMode $451
#cmd LNUIListItemsControl_GetSubmitMode $453
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $CF3
#cmd LNUIListBoxItem_Create $455
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $CFE
#cmd LNUIListBox_Create $45A
#cmd LNUIListBox_AddItem $458
#cmd LNUIListBox_SetPrototype_OnSerialize $D09
#cmd LNInputGesture_SetPrototype_OnSerialize $D14
#cmd LNKeyGesture_Create $45D
#cmd LNKeyGesture_SetPrototype_OnSerialize $D1F
#cmd LNInput_IsPressed $460
#cmd LNInput_IsTriggered $462
#cmd LNInput_IsTriggeredOff $464
#cmd LNInput_IsRepeated $466
#cmd LNInput_GetAxisValue $468
#cmd LNInput_AddBinding $46A
#cmd LNInput_RemoveBinding $46D
#cmd LNInput_ClearBindings $46F
#cmd LNInput_ClearAllBindings $471
#cmd LNMouse_Pressed $473
#cmd LNMouse_Triggered $475
#cmd LNMouse_TriggeredOff $477
#cmd LNMouse_Repeated $479
#cmd LNMouse_Position $47B
#cmd LNInterpreterCommand_Code $47D
#cmd LNInterpreterCommand_ParamsCount $47E
#cmd LNInterpreterCommand_Param $47F
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $D2A
#cmd LNInterpreterCommandList_Create $496
#cmd LNInterpreterCommandList_AddCommand $482
#cmd LNInterpreterCommandList_AddCommand1 $484
#cmd LNInterpreterCommandList_AddCommand2 $487
#cmd LNInterpreterCommandList_AddCommand3 $48B
#cmd LNInterpreterCommandList_AddCommand4 $490
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $D35
#cmd LNInterpreterCommandDelegate_Create $8A5
#cmd LNInterpreter_Create $4AA
#cmd LNInterpreter_Clear $49B
#cmd LNInterpreter_Run $49C
#cmd LNInterpreter_IsRunning $49E
#cmd LNInterpreter_Update $49F
#cmd LNInterpreter_Terminate $4A0
#cmd LNInterpreter_RegisterCommandHandler $4A1
#cmd LNInterpreter_SetWaitMode $4A4
#cmd LNInterpreter_GetWaitMode $4A6
#cmd LNInterpreter_SetWaitCount $4A7
#cmd LNInterpreter_GetWaitCount $4A9
#cmd LNInterpreter_SetPrototype_OnSerialize $D40
#cmd LNInterpreter_SetPrototype_OnUpdateWait $D4C
#cmd LNEngineSettings_SetMainWindowSize $4AD
#cmd LNEngineSettings_SetMainWorldViewSize $4B0
#cmd LNEngineSettings_SetMainWindowTitle $4B3
#cmd LNEngineSettings_AddAssetDirectory $4B5
#cmd LNEngineSettings_AddAssetArchive $4B7
#cmd LNEngineSettings_SetFrameRate $4BA
#cmd LNEngineSettings_SetUITheme $4BC
#cmd LNEngineSettings_SetFontFile $4BE
#cmd LNEngineSettings_SetDebugToolEnabled $4C0
#cmd LNEngineSettings_SetEngineLogEnabled $4C2
#cmd LNEngineSettings_SetEngineLogFilePath $4C4
#cmd LNEngineSettings_SetDeveloperToolEnabled $4C6
#cmd LNEngine_Initialize $4C9
#cmd LNEngine_Finalize $4CA
#cmd LNEngine_Update $4CB
#cmd LNEngine_Run $4CC
#cmd LNEngine_Time $4CE
#cmd LNEngine_GetCamera $4CF
#cmd LNEngine_GetMainLight $4D0
#cmd LNEngine_GetRenderView $4D1
#cmd LNApplication_Create $4D6
#cmd LNApplication_OnInit $4D3
#cmd LNApplication_OnUpdate $4D4
#cmd LNApplication_World $4D5
#cmd LNApplication_SetPrototype_OnSerialize $D57
#cmd LNApplication_SetPrototype_OnInit $D62
#cmd LNApplication_SetPrototype_OnUpdate $D6D
#cmd LNDebug_SetGuideGridEnabled $4D8
#cmd LNDebug_SetPhysicsDebugDrawEnabled $4DA
#cmd LNDebug_Print $4DC
#cmd LNDebug_PrintWithTime $4DE
#cmd LNDebug_PrintWithTimeAndColor $4E1
#cmd LNObjectSerializeHandler_Create $8D1
#cmd LNEventConnectionSerializeHandler_Create $8DC
#cmd LNVariantSerializeHandler_Create $8E7
#cmd LNZVTestClass1SerializeHandler_Create $8F2
#cmd LNZVTestEventArgs1SerializeHandler_Create $8FD
#cmd LNSerializer2SerializeHandler_Create $908
#cmd LNAssetObjectSerializeHandler_Create $913
#cmd LNAssetImportSettingsSerializeHandler_Create $91E
#cmd LNAssetModelSerializeHandler_Create $929
#cmd LNSoundSerializeHandler_Create $934
#cmd LNTextureSerializeHandler_Create $93F
#cmd LNTexture2DSerializeHandler_Create $94A
#cmd LNShaderSerializeHandler_Create $955
#cmd LNRenderViewSerializeHandler_Create $960
#cmd LNMaterialSerializeHandler_Create $96B
#cmd LNStaticMeshModelSerializeHandler_Create $976
#cmd LNSkinnedMeshModelSerializeHandler_Create $981
#cmd LNAnimationControllerSerializeHandler_Create $98C
#cmd LNCollisionShapeSerializeHandler_Create $997
#cmd LNBoxCollisionShapeSerializeHandler_Create $9A2
#cmd LNAnimationClipSerializeHandler_Create $9AD
#cmd LNAnimationStateSerializeHandler_Create $9B8
#cmd LNEffectResourceSerializeHandler_Create $9C3
#cmd LNParticleEmitterModelSerializeHandler_Create $9CE
#cmd LNParticleModelSerializeHandler_Create $9D9
#cmd LNComponentSerializeHandler_Create $9E4
#cmd LNVisualComponentSerializeHandler_Create $9EF
#cmd LNSpriteComponentSerializeHandler_Create $9FA
#cmd LNCharacterControllerSerializeHandler_Create $A05
#cmd LNWorldSerializeHandler_Create $A10
#cmd LNComponentListSerializeHandler_Create $A1B
#cmd LNWorldObjectSerializeHandler_Create $A26
#cmd LNWorldObjectPreUpdateHandler_Create $A31
#cmd LNWorldObjectUpdateHandler_Create $A3C
#cmd LNVisualObjectSerializeHandler_Create $A47
#cmd LNVisualObjectPreUpdateHandler_Create $A52
#cmd LNVisualObjectUpdateHandler_Create $A5D
#cmd LNCameraSerializeHandler_Create $A68
#cmd LNCameraPreUpdateHandler_Create $A73
#cmd LNCameraUpdateHandler_Create $A7E
#cmd LNEnvironmentLightSerializeHandler_Create $A89
#cmd LNEnvironmentLightPreUpdateHandler_Create $A94
#cmd LNEnvironmentLightUpdateHandler_Create $A9F
#cmd LNDirectionalLightSerializeHandler_Create $AAA
#cmd LNDirectionalLightPreUpdateHandler_Create $AB5
#cmd LNDirectionalLightUpdateHandler_Create $AC0
#cmd LNPointLightSerializeHandler_Create $ACB
#cmd LNPointLightPreUpdateHandler_Create $AD6
#cmd LNPointLightUpdateHandler_Create $AE1
#cmd LNSpotLightSerializeHandler_Create $AEC
#cmd LNSpotLightPreUpdateHandler_Create $AF7
#cmd LNSpotLightUpdateHandler_Create $B02
#cmd LNSpriteSerializeHandler_Create $B0D
#cmd LNSpritePreUpdateHandler_Create $B18
#cmd LNSpriteUpdateHandler_Create $B23
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $B2E
#cmd LNRaycasterSerializeHandler_Create $B39
#cmd LNRaycastResultSerializeHandler_Create $B44
#cmd LNWorldRenderViewSerializeHandler_Create $B4F
#cmd LNBoxMeshSerializeHandler_Create $B5A
#cmd LNBoxMeshPreUpdateHandler_Create $B65
#cmd LNBoxMeshUpdateHandler_Create $B70
#cmd LNPlaneMeshSerializeHandler_Create $B7B
#cmd LNPlaneMeshPreUpdateHandler_Create $B86
#cmd LNPlaneMeshUpdateHandler_Create $B91
#cmd LNStaticMeshSerializeHandler_Create $B9C
#cmd LNStaticMeshPreUpdateHandler_Create $BA7
#cmd LNStaticMeshUpdateHandler_Create $BB2
#cmd LNStaticMeshComponentSerializeHandler_Create $BBD
#cmd LNSkinnedMeshComponentSerializeHandler_Create $BC8
#cmd LNCollisionSerializeHandler_Create $BD3
#cmd LNTriggerBodyComponentSerializeHandler_Create $BDE
#cmd LNParticleEmitterSerializeHandler_Create $BE9
#cmd LNParticleEmitterPreUpdateHandler_Create $BF4
#cmd LNParticleEmitterUpdateHandler_Create $BFF
#cmd LNLevelSerializeHandler_Create $C0A
#cmd LNLevelStartHandler_Create $C15
#cmd LNLevelStopHandler_Create $C20
#cmd LNLevelPauseHandler_Create $C2B
#cmd LNLevelResumeHandler_Create $C36
#cmd LNLevelUpdateHandler_Create $C41
#cmd LNUIEventArgsSerializeHandler_Create $C4C
#cmd LNUILayoutElementSerializeHandler_Create $C57
#cmd LNUIElementSerializeHandler_Create $C62
#cmd LNUITextBlockSerializeHandler_Create $C6D
#cmd LNUISpriteSerializeHandler_Create $C78
#cmd LNUIIconSerializeHandler_Create $C83
#cmd LNUIMessageTextAreaSerializeHandler_Create $C8E
#cmd LNUILayoutPanelSerializeHandler_Create $C99
#cmd LNUIBoxLayoutSerializeHandler_Create $CA4
#cmd LNUIGridLayoutSerializeHandler_Create $CAF
#cmd LNUIControlSerializeHandler_Create $CBA
#cmd LNUIButtonBaseSerializeHandler_Create $CC5
#cmd LNUIButtonSerializeHandler_Create $CD0
#cmd LNUIWindowSerializeHandler_Create $CDB
#cmd LNUIListItemSerializeHandler_Create $CE6
#cmd LNUIListItemsControlSerializeHandler_Create $CF1
#cmd LNUIListBoxItemSerializeHandler_Create $CFC
#cmd LNUIListBoxSerializeHandler_Create $D07
#cmd LNInputGestureSerializeHandler_Create $D12
#cmd LNKeyGestureSerializeHandler_Create $D1D
#cmd LNInterpreterCommandSerializeHandler_Create $D28
#cmd LNInterpreterCommandListSerializeHandler_Create $D33
#cmd LNInterpreterSerializeHandler_Create $D3E
#cmd LNInterpreterUpdateWaitHandler_Create $D4A
#cmd LNApplicationSerializeHandler_Create $D55
#cmd LNApplicationInitHandler_Create $D60
#cmd LNApplicationUpdateHandler_Create $D6B



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
