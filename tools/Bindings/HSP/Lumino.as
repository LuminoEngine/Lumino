
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
#cmd LNVector3_GetX $4FB
#cmd LNVector3_SetX $4FD
#cmd LNVector3_GetY $4FE
#cmd LNVector3_SetY $500
#cmd LNVector3_GetZ $501
#cmd LNVector3_SetZ $503
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $51A
#cmd LNVector4_SetX $51C
#cmd LNVector4_GetY $51D
#cmd LNVector4_SetY $51F
#cmd LNVector4_GetZ $520
#cmd LNVector4_SetZ $522
#cmd LNVector4_GetW $523
#cmd LNVector4_SetW $525
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $538
#cmd LNQuaternion_SetX $53A
#cmd LNQuaternion_GetY $53B
#cmd LNQuaternion_SetY $53D
#cmd LNQuaternion_GetZ $53E
#cmd LNQuaternion_SetZ $540
#cmd LNQuaternion_GetW $541
#cmd LNQuaternion_SetW $543
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $557
#cmd LNMatrix_SetRow0 $559
#cmd LNMatrix_GetRow1 $55A
#cmd LNMatrix_SetRow1 $55C
#cmd LNMatrix_GetRow2 $55D
#cmd LNMatrix_SetRow2 $55F
#cmd LNMatrix_GetRow3 $560
#cmd LNMatrix_SetRow3 $562
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $173
#cmd LNColor_GetR $5F4
#cmd LNColor_SetR $5F6
#cmd LNColor_GetG $5F7
#cmd LNColor_SetG $5F9
#cmd LNColor_GetB $5FA
#cmd LNColor_SetB $5FC
#cmd LNColor_GetA $5FD
#cmd LNColor_SetA $5FF
#cmd LNColor_SetZeros $178
#cmd LNColor_Set $179
#cmd LNPoint $17E
#cmd LNPoint_GetX $613
#cmd LNPoint_SetX $615
#cmd LNPoint_GetY $616
#cmd LNPoint_SetY $618
#cmd LNPoint_SetZeros $181
#cmd LNPoint_Set $182
#cmd LNSize $185
#cmd LNSize_GetWidth $623
#cmd LNSize_SetWidth $625
#cmd LNSize_GetHeight $626
#cmd LNSize_SetHeight $628
#cmd LNSize_SetZeros $188
#cmd LNSize_Set $189
#cmd LNRect $18C
#cmd LNRect_GetX $633
#cmd LNRect_SetX $635
#cmd LNRect_GetY $636
#cmd LNRect_SetY $638
#cmd LNRect_GetWidth $639
#cmd LNRect_SetWidth $63B
#cmd LNRect_GetHeight $63C
#cmd LNRect_SetHeight $63E
#cmd LNRect_SetZeros $191
#cmd LNRect_Set $192
#cmd LNRect_GetLeft $197
#cmd LNRect_SetSize $198
#cmd LNRect_GetSize $19A
#cmd LNThickness $19B
#cmd LNThickness_GetLeft $657
#cmd LNThickness_SetLeft $659
#cmd LNThickness_GetTop $65A
#cmd LNThickness_SetTop $65C
#cmd LNThickness_GetRight $65D
#cmd LNThickness_SetRight $65F
#cmd LNThickness_GetBottom $660
#cmd LNThickness_SetBottom $662
#cmd LNThickness_SetZeros $1A0
#cmd LNThickness_Set $1A1
#cmd LNCornerRadius $1A6
#cmd LNCornerRadius_GetTopleft $675
#cmd LNCornerRadius_SetTopleft $677
#cmd LNCornerRadius_GetTopright $678
#cmd LNCornerRadius_SetTopright $67A
#cmd LNCornerRadius_GetBottomright $67B
#cmd LNCornerRadius_SetBottomright $67D
#cmd LNCornerRadius_GetBottomleft $67E
#cmd LNCornerRadius_SetBottomleft $680
#cmd LNCornerRadius_SetZeros $1AB
#cmd LNCornerRadius_Set $1AC

#cmd LNObject_Release $7A
#cmd LNObject_Retain $7B
#cmd LNObject_GetReferenceCount $7C
#cmd LNObject_SetPrototype_OnSerialize $8FF
#cmd LNEventConnection_SetPrototype_OnSerialize $90A
#cmd LNPromiseFailureDelegate_Create $57D
#cmd LNVariant_Create $81
#cmd LNVariant_SetInt $82
#cmd LNVariant_GetInt $84
#cmd LNVariant_SetPrototype_OnSerialize $915
#cmd LNZVTestDelegate1_Create $586
#cmd LNZVTestDelegate2_Create $58C
#cmd LNZVTestDelegate3_Create $591
#cmd LNZVTestEventHandler1_Create $596
#cmd LNZVTestEventHandler2_Create $59B
#cmd LNZVTestPromise1_ThenWith $59E
#cmd LNZVTestPromise1_CatchWith $5A0
#cmd LNZVTestPromise2_ThenWith $5A4
#cmd LNZVTestPromise2_CatchWith $5A6
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $920
#cmd LNZVTestEventArgs1_Create $B0
#cmd LNZVTestEventArgs1_CreateWithValue $B1
#cmd LNZVTestEventArgs1_GetValue $AF
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $92B
#cmd LNSerializer2_SetPrototype_OnSerialize $936
#cmd LNAssetObject_SetPrototype_OnSerialize $941
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $94C
#cmd LNAssetModel_Create $113
#cmd LNAssetModel_Target $112
#cmd LNAssetModel_SetPrototype_OnSerialize $957
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
#cmd LNSound_SetPrototype_OnSerialize $962
#cmd LNAudio_PlayBGM $13F
#cmd LNAudio_StopBGM $144
#cmd LNAudio_PlayBGS $146
#cmd LNAudio_StopBGS $14B
#cmd LNAudio_PlayME $14D
#cmd LNAudio_StopME $151
#cmd LNAudio_PlaySE $152
#cmd LNAudio_PlaySE3D $156
#cmd LNAudio_StopSE $15C
#cmd LNTexture2DDelegate_Create $5EC
#cmd LNTexture2DPromise_ThenWith $5EF
#cmd LNTexture2DPromise_CatchWith $5F1
#cmd LNTexture_SetPrototype_OnSerialize $96D
#cmd LNTexture2D_Create $1B7
#cmd LNTexture2D_CreateWithFormat $1BA
#cmd LNTexture2D_Load $1B3
#cmd LNTexture2D_LoadEmoji $1B5
#cmd LNTexture2D_SetPrototype_OnSerialize $978
#cmd LNShader_Load $1BF
#cmd LNShader_SetFloat $1C2
#cmd LNShader_SetVector3 $1C5
#cmd LNShader_SetVector4 $1C8
#cmd LNShader_SetPrototype_OnSerialize $983
#cmd LNRenderView_SetPrototype_OnSerialize $98E
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
#cmd LNMaterial_SetPrototype_OnSerialize $999
#cmd LNMeshNode_SetVisible $1E9
#cmd LNMeshNode_IsVisible $1EB
#cmd LNMeshNode_SetPrototype_OnSerialize $9A4
#cmd LNStaticMeshModel_Load $1ED
#cmd LNStaticMeshModel_FindNode $1EF
#cmd LNStaticMeshModel_FindMaterial $1F1
#cmd LNStaticMeshModel_MaterialCount $1F3
#cmd LNStaticMeshModel_Material $1F4
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $9AF
#cmd LNSkinnedMeshModel_Load $1F7
#cmd LNSkinnedMeshModel_GetAnimationController $1F9
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $9BA
#cmd LNAnimationController_AddClip $1FB
#cmd LNAnimationController_Play $1FD
#cmd LNAnimationController_SetPrototype_OnSerialize $9C5
#cmd LNCollisionShape_SetPrototype_OnSerialize $9D0
#cmd LNBoxCollisionShape_Create $202
#cmd LNBoxCollisionShape_CreateWHD $204
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $9DB
#cmd LNAnimationClip_Load $20E
#cmd LNAnimationClip_SetHierarchicalAnimationMode $210
#cmd LNAnimationClip_HierarchicalAnimationMode $212
#cmd LNAnimationClip_SetPrototype_OnSerialize $9E6
#cmd LNAnimationState_SetPrototype_OnSerialize $9F1
#cmd LNEffectResource_SetPrototype_OnSerialize $9FC
#cmd LNParticleEmitterModel_Create $233
#cmd LNParticleEmitterModel_SetMaxParticles $21F
#cmd LNParticleEmitterModel_SetSpawnRate $221
#cmd LNParticleEmitterModel_SetLifeTime $223
#cmd LNParticleEmitterModel_SetupBoxShape $225
#cmd LNParticleEmitterModel_SetSize $227
#cmd LNParticleEmitterModel_SetForwardVelocityMin $229
#cmd LNParticleEmitterModel_SetForwardVelocityMax $22B
#cmd LNParticleEmitterModel_SetForwardScale $22D
#cmd LNParticleEmitterModel_SetGeometryDirection $22F
#cmd LNParticleEmitterModel_SetupSpriteModule $231
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $A07
#cmd LNParticleModel_Create $23A
#cmd LNParticleModel_SetLoop $235
#cmd LNParticleModel_IsLoop $237
#cmd LNParticleModel_AddEmitter $238
#cmd LNParticleModel_SetPrototype_OnSerialize $A12
#cmd LNComponent_SetPrototype_OnSerialize $A1D
#cmd LNVisualComponent_SetVisible $23D
#cmd LNVisualComponent_IsVisible $23F
#cmd LNVisualComponent_SetPrototype_OnSerialize $A28
#cmd LNSpriteComponent_SetTexture $241
#cmd LNSpriteComponent_SetPrototype_OnSerialize $A33
#cmd LNCollisionEventHandler_Create $6F1
#cmd LNCharacterController_Create $261
#cmd LNCharacterController_SetWalkVelocity $24B
#cmd LNCharacterController_GetWalkVelocity $24D
#cmd LNCharacterController_SetVelocity $24E
#cmd LNCharacterController_GetVelocity $250
#cmd LNCharacterController_SetInputControlEnabled $251
#cmd LNCharacterController_SetCameraControlEnabled $253
#cmd LNCharacterController_SetHeight $255
#cmd LNCharacterController_GetHeight $257
#cmd LNCharacterController_SetCameraRadius $258
#cmd LNCharacterController_GetCameraRadius $25A
#cmd LNCharacterController_SetCollisionEnter $25B
#cmd LNCharacterController_SetCollisionLeave $25D
#cmd LNCharacterController_SetCollisionStay $25F
#cmd LNCharacterController_SetPrototype_OnSerialize $A3E
#cmd LNWorld_Add $263
#cmd LNWorld_SetPrototype_OnSerialize $A49
#cmd LNComponentList_GetLength $711
#cmd LNComponentList_GetItem $713
#cmd LNComponentList_SetPrototype_OnSerialize $A54
#cmd LNWorldObject_Create $29A
#cmd LNWorldObject_SetPosition $267
#cmd LNWorldObject_SetPositionXYZ $269
#cmd LNWorldObject_GetPosition $26D
#cmd LNWorldObject_SetRotationQuaternion $26E
#cmd LNWorldObject_SetRotation $270
#cmd LNWorldObject_GetRotation $274
#cmd LNWorldObject_SetScale $275
#cmd LNWorldObject_SetScaleS $277
#cmd LNWorldObject_SetScaleXYZ $279
#cmd LNWorldObject_GetScale $27D
#cmd LNWorldObject_SetCenterPoint $27E
#cmd LNWorldObject_SetCenterPointXYZ $280
#cmd LNWorldObject_GetCenterPoint $284
#cmd LNWorldObject_LookAt $285
#cmd LNWorldObject_LookAtXYZ $287
#cmd LNWorldObject_AddComponent $28B
#cmd LNWorldObject_RemoveComponent $28D
#cmd LNWorldObject_AddTag $28F
#cmd LNWorldObject_RemoveTag $291
#cmd LNWorldObject_HasTag $293
#cmd LNWorldObject_Destroy $295
#cmd LNWorldObject_GetComponents $296
#cmd LNWorldObject_SetPrototype_OnSerialize $A5F
#cmd LNWorldObject_SetPrototype_OnPreUpdate $A6A
#cmd LNWorldObject_SetPrototype_OnUpdate $A75
#cmd LNVisualObject_SetVisible $29C
#cmd LNVisualObject_IsVisible $29E
#cmd LNVisualObject_SetOpacity $29F
#cmd LNVisualObject_GetOpacity $2A1
#cmd LNVisualObject_SetPrototype_OnSerialize $A80
#cmd LNVisualObject_SetPrototype_OnPreUpdate $A8B
#cmd LNVisualObject_SetPrototype_OnUpdate $A96
#cmd LNCamera_SetPrototype_OnSerialize $AA1
#cmd LNCamera_SetPrototype_OnPreUpdate $AAC
#cmd LNCamera_SetPrototype_OnUpdate $AB7
#cmd LNEnvironmentLight_SetEnabled $2A4
#cmd LNEnvironmentLight_IsEnabled $2A6
#cmd LNEnvironmentLight_SetColor $2A7
#cmd LNEnvironmentLight_GetColor $2A9
#cmd LNEnvironmentLight_SetAmbientColor $2AA
#cmd LNEnvironmentLight_GetAmbientColor $2AC
#cmd LNEnvironmentLight_GetSkyColor $2AD
#cmd LNEnvironmentLight_SetSkyColor $2AE
#cmd LNEnvironmentLight_GetGroundColor $2B0
#cmd LNEnvironmentLight_SetGroundColor $2B1
#cmd LNEnvironmentLight_SetIntensity $2B3
#cmd LNEnvironmentLight_GetIntensity $2B5
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $2B6
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $2B8
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $2B9
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $2BB
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $AC2
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $ACD
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $AD8
#cmd LNDirectionalLight_Create $2CC
#cmd LNDirectionalLight_CreateWithColor $2CD
#cmd LNDirectionalLight_SetEnabled $2BD
#cmd LNDirectionalLight_IsEnabled $2BF
#cmd LNDirectionalLight_SetColor $2C0
#cmd LNDirectionalLight_GetColor $2C2
#cmd LNDirectionalLight_SetIntensity $2C3
#cmd LNDirectionalLight_GetIntensity $2C5
#cmd LNDirectionalLight_SetShadowEffectiveDistance $2C6
#cmd LNDirectionalLight_GetShadowEffectiveDistance $2C8
#cmd LNDirectionalLight_SetShadowEffectiveDepth $2C9
#cmd LNDirectionalLight_GetShadowEffectiveDepth $2CB
#cmd LNDirectionalLight_SetPrototype_OnSerialize $AE3
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $AEE
#cmd LNDirectionalLight_SetPrototype_OnUpdate $AF9
#cmd LNPointLight_Create $2DF
#cmd LNPointLight_CreateWithColorAndRange $2E0
#cmd LNPointLight_SetEnabled $2D0
#cmd LNPointLight_IsEnabled $2D2
#cmd LNPointLight_SetColor $2D3
#cmd LNPointLight_GetColor $2D5
#cmd LNPointLight_SetIntensity $2D6
#cmd LNPointLight_GetIntensity $2D8
#cmd LNPointLight_SetRange $2D9
#cmd LNPointLight_GetRange $2DB
#cmd LNPointLight_SetAttenuation $2DC
#cmd LNPointLight_GetAttenuation $2DE
#cmd LNPointLight_SetPrototype_OnSerialize $B04
#cmd LNPointLight_SetPrototype_OnPreUpdate $B0F
#cmd LNPointLight_SetPrototype_OnUpdate $B1A
#cmd LNSpotLight_Create $2F9
#cmd LNSpotLight_CreateWithColorAndRange $2FA
#cmd LNSpotLight_SetEnabled $2E4
#cmd LNSpotLight_IsEnabled $2E6
#cmd LNSpotLight_SetColor $2E7
#cmd LNSpotLight_GetColor $2E9
#cmd LNSpotLight_SetIntensity $2EA
#cmd LNSpotLight_GetIntensity $2EC
#cmd LNSpotLight_SetRange $2ED
#cmd LNSpotLight_GetRange $2EF
#cmd LNSpotLight_SetAttenuation $2F0
#cmd LNSpotLight_GetAttenuation $2F2
#cmd LNSpotLight_SetAngle $2F3
#cmd LNSpotLight_GetAngle $2F5
#cmd LNSpotLight_SetPenumbra $2F6
#cmd LNSpotLight_GetPenumbra $2F8
#cmd LNSpotLight_SetPrototype_OnSerialize $B25
#cmd LNSpotLight_SetPrototype_OnPreUpdate $B30
#cmd LNSpotLight_SetPrototype_OnUpdate $B3B
#cmd LNTestDelegate_Create $7BD
#cmd LNSprite_Create $310
#cmd LNSprite_CreateWithTexture $311
#cmd LNSprite_CreateWithTextureAndSize $313
#cmd LNSprite_SetTexture $302
#cmd LNSprite_SetSize $304
#cmd LNSprite_SetSizeWH $306
#cmd LNSprite_SetSourceRectXYWH $309
#cmd LNSprite_SetCallerTest $30E
#cmd LNSprite_SetPrototype_OnSerialize $B46
#cmd LNSprite_SetPrototype_OnPreUpdate $B51
#cmd LNSprite_SetPrototype_OnUpdate $B5C
#cmd LNCameraOrbitControlComponent_Create $318
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $B67
#cmd LNRaycaster_FromScreen $31A
#cmd LNRaycaster_IntersectPlane $31C
#cmd LNRaycaster_SetPrototype_OnSerialize $B72
#cmd LNRaycastResult_GetPoint $321
#cmd LNRaycastResult_SetPrototype_OnSerialize $B7D
#cmd LNWorldRenderView_SetGuideGridEnabled $323
#cmd LNWorldRenderView_GetGuideGridEnabled $325
#cmd LNWorldRenderView_SetPrototype_OnSerialize $B88
#cmd LNBoxMesh_Create $327
#cmd LNBoxMesh_SetPrototype_OnSerialize $B93
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $B9E
#cmd LNBoxMesh_SetPrototype_OnUpdate $BA9
#cmd LNPlaneMesh_Create $329
#cmd LNPlaneMesh_SetPrototype_OnSerialize $BB4
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $BBF
#cmd LNPlaneMesh_SetPrototype_OnUpdate $BCA
#cmd LNStaticMesh_Load $32B
#cmd LNStaticMesh_GetModel $32D
#cmd LNStaticMesh_MakeCollisionBody $32E
#cmd LNStaticMesh_SetPrototype_OnSerialize $BD5
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $BE0
#cmd LNStaticMesh_SetPrototype_OnUpdate $BEB
#cmd LNStaticMeshComponent_Create $335
#cmd LNStaticMeshComponent_SetModel $331
#cmd LNStaticMeshComponent_MakeCollisionBody $333
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $BF6
#cmd LNSkinnedMeshComponent_Create $339
#cmd LNSkinnedMeshComponent_SetModel $337
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $C01
#cmd LNCollision_GetWorldObject $33B
#cmd LNCollision_SetPrototype_OnSerialize $C0C
#cmd LNTriggerBodyComponent_Create $33F
#cmd LNTriggerBodyComponent_AddCollisionShape $33D
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $C17
#cmd LNParticleEmitter_Create $341
#cmd LNParticleEmitter_SetPrototype_OnSerialize $C22
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $C2D
#cmd LNParticleEmitter_SetPrototype_OnUpdate $C38
#cmd LNScene_SetClearMode $344
#cmd LNScene_SetSkyColor $346
#cmd LNScene_SetSkyHorizonColor $348
#cmd LNScene_SetSkyCloudColor $34A
#cmd LNScene_SetSkyOverlayColor $34C
#cmd LNScene_GotoLevel $34E
#cmd LNScene_CallLevel $351
#cmd LNScene_ReturnLevel $354
#cmd LNScene_ActiveLevel $356
#cmd LNScene_IsTransitionEffectRunning $357
#cmd LNScene_SetTransitionEffectMode $358
#cmd LNScene_TransitionEffectMode $35A
#cmd LNScene_SetTransitionDuration $35B
#cmd LNScene_TransitionDuration $35D
#cmd LNScene_SetTransitionEffectColor $35E
#cmd LNScene_TransitionEffectColor $360
#cmd LNScene_SetTransitionEffectMaskTexture $361
#cmd LNScene_TransitionEffectMaskTexture $363
#cmd LNScene_SetTransitionEffectVague $364
#cmd LNScene_TransitionEffectVague $366
#cmd LNScene_StartFadeOut $367
#cmd LNScene_StartFadeIn $368
#cmd LNScene_SetHDREnabled $369
#cmd LNScene_IsHDREnabled $36B
#cmd LNScene_SetScreenBlendColor $36C
#cmd LNScene_ScreenBlendColor $36E
#cmd LNScene_SetAntialiasEnabled $36F
#cmd LNScene_IsAntialiasEnabled $371
#cmd LNScene_SetSSREnabled $372
#cmd LNScene_IsSSREnabled $374
#cmd LNScene_SetSSAOEnabled $375
#cmd LNScene_IsSSAOEnabled $377
#cmd LNScene_SetBloomEnabled $378
#cmd LNScene_IsBloomEnabled $37A
#cmd LNScene_SetDOFEnabled $37B
#cmd LNScene_IsDOFEnabled $37D
#cmd LNScene_SetTonemapEnabled $37E
#cmd LNScene_IsTonemapEnabled $380
#cmd LNScene_SetVignetteEnabled $381
#cmd LNScene_IsVignetteEnabled $383
#cmd LNScene_SetGammaEnabled $384
#cmd LNScene_IsGammaEnabled $386
#cmd LNLevel_Create $396
#cmd LNLevel_AddObject $388
#cmd LNLevel_RemoveObject $38A
#cmd LNLevel_AddSubLevel $38C
#cmd LNLevel_RemoveSubLevel $38E
#cmd LNLevel_RemoveAllSubLevels $390
#cmd LNLevel_SetPrototype_OnSerialize $C43
#cmd LNLevel_SetPrototype_OnStart $C4E
#cmd LNLevel_SetPrototype_OnStop $C59
#cmd LNLevel_SetPrototype_OnPause $C64
#cmd LNLevel_SetPrototype_OnResume $C6F
#cmd LNLevel_SetPrototype_OnUpdate $C7A
#cmd LNUIEventArgs_Sender $3A1
#cmd LNUIEventArgs_SetPrototype_OnSerialize $C85
#cmd LNUIGeneralEventHandler_Create $813
#cmd LNUIEventHandler_Create $818
#cmd LNUILayoutElement_SetPrototype_OnSerialize $C90
#cmd LNUIElement_SetSize $3B3
#cmd LNUIElement_SetSizeWH $3B5
#cmd LNUIElement_SetWidth $3B8
#cmd LNUIElement_GetWidth $3BA
#cmd LNUIElement_SetHeight $3BB
#cmd LNUIElement_GetHeight $3BD
#cmd LNUIElement_SetMargin $3BE
#cmd LNUIElement_GetMargin $3C0
#cmd LNUIElement_SetPadding $3C1
#cmd LNUIElement_GetPadding $3C3
#cmd LNUIElement_SetHAlignment $3C4
#cmd LNUIElement_GetHAlignment $3C6
#cmd LNUIElement_SetVAlignment $3C7
#cmd LNUIElement_GetVAlignment $3C9
#cmd LNUIElement_SetAlignments $3CA
#cmd LNUIElement_SetPosition $3CD
#cmd LNUIElement_SetPositionXYZ $3CF
#cmd LNUIElement_GetPosition $3D3
#cmd LNUIElement_SetRotation $3D4
#cmd LNUIElement_SetEulerAngles $3D6
#cmd LNUIElement_GetRotation $3DA
#cmd LNUIElement_SetScale $3DB
#cmd LNUIElement_SetScaleS $3DD
#cmd LNUIElement_SetScaleXY $3DF
#cmd LNUIElement_GetScale $3E2
#cmd LNUIElement_SetCenterPoint $3E3
#cmd LNUIElement_SetCenterPointXYZ $3E5
#cmd LNUIElement_GetCenterPoint $3E9
#cmd LNUIElement_SetEnabled $3EA
#cmd LNUIElement_IsEnabled $3EC
#cmd LNUIElement_SetData $3ED
#cmd LNUIElement_GetData $3EF
#cmd LNUIElement_SetBackgroundColor $3F0
#cmd LNUIElement_GetBackgroundColor $3F2
#cmd LNUIElement_SetBorderThickness $3F3
#cmd LNUIElement_GetBorderThickness $3F5
#cmd LNUIElement_SetBorderColor $3F6
#cmd LNUIElement_GetBorderColor $3F8
#cmd LNUIElement_SetCornerRadius $3F9
#cmd LNUIElement_GetCornerRadius $3FB
#cmd LNUIElement_SetVisibility $3FC
#cmd LNUIElement_GetVisibility $3FE
#cmd LNUIElement_SetOpacity $3FF
#cmd LNUIElement_GetOpacity $401
#cmd LNUIElement_AddChild $402
#cmd LNUIElement_SetPrototype_OnSerialize $C9B
#cmd LNUITextBlock_Create $408
#cmd LNUITextBlock_CreateWithText $409
#cmd LNUITextBlock_SetText $405
#cmd LNUITextBlock_GetText $407
#cmd LNUITextBlock_SetPrototype_OnSerialize $CA6
#cmd LNUISprite_Create $416
#cmd LNUISprite_CreateWithTexture $417
#cmd LNUISprite_SetTexture $40C
#cmd LNUISprite_SetSourceRect $40E
#cmd LNUISprite_SetSourceRectXYWH $410
#cmd LNUISprite_GetSourceRect $415
#cmd LNUISprite_SetPrototype_OnSerialize $CB1
#cmd LNUIIcon_LoadFontIcon $41A
#cmd LNUIIcon_LoadFontIconWithNameSize $41C
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $41F
#cmd LNUIIcon_SetPrototype_OnSerialize $CBC
#cmd LNUIMessageTextArea_Create $428
#cmd LNUIMessageTextArea_SetText $424
#cmd LNUIMessageTextArea_SetTypingSpeed $426
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $CC7
#cmd LNUI_Add $42A
#cmd LNUI_Remove $42C
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $CD2
#cmd LNUIBoxLayout_Create $433
#cmd LNUIBoxLayout_SetOrientation $430
#cmd LNUIBoxLayout_GetOrientation $432
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $CDD
#cmd LNUIGridLayout_Create $443
#cmd LNUIGridLayout_SetColumnCount $435
#cmd LNUIGridLayout_SetRow $437
#cmd LNUIGridLayout_SetColumn $43A
#cmd LNUIGridLayout_SetPlacement $43D
#cmd LNUIGridLayout_SetPrototype_OnSerialize $CE8
#cmd LNUIControl_Create $44F
#cmd LNUIControl_AddInlineVisual $450
#cmd LNUIControl_SetPrototype_OnSerialize $CF3
#cmd LNUIButtonBase_SetText $454
#cmd LNUIButtonBase_SetPrototype_OnSerialize $CFE
#cmd LNUIButton_Create $457
#cmd LNUIButton_CreateWithText $458
#cmd LNUIButton_ConnectOnClicked $45A
#cmd LNUIButton_SetPrototype_OnSerialize $D09
#cmd LNUIWindow_Create $45D
#cmd LNUIWindow_SetPrototype_OnSerialize $D14
#cmd LNUIListItem_ConnectOnSubmit $462
#cmd LNUIListItem_SetPrototype_OnSerialize $D1F
#cmd LNUIListItemsControl_SetItemsLayoutPanel $465
#cmd LNUIListItemsControl_SetSubmitMode $467
#cmd LNUIListItemsControl_GetSubmitMode $469
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $D2A
#cmd LNUIListBoxItem_Create $46B
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $D35
#cmd LNUIListBox_Create $470
#cmd LNUIListBox_AddItem $46E
#cmd LNUIListBox_SetPrototype_OnSerialize $D40
#cmd LNInputGesture_SetPrototype_OnSerialize $D4B
#cmd LNKeyGesture_Create $473
#cmd LNKeyGesture_SetPrototype_OnSerialize $D56
#cmd LNInput_IsPressed $476
#cmd LNInput_IsTriggered $478
#cmd LNInput_IsTriggeredOff $47A
#cmd LNInput_IsRepeated $47C
#cmd LNInput_GetAxisValue $47E
#cmd LNInput_AddBinding $480
#cmd LNInput_RemoveBinding $483
#cmd LNInput_ClearBindings $485
#cmd LNInput_ClearAllBindings $487
#cmd LNMouse_Pressed $489
#cmd LNMouse_Triggered $48B
#cmd LNMouse_TriggeredOff $48D
#cmd LNMouse_Repeated $48F
#cmd LNMouse_Position $491
#cmd LNInterpreterCommand_Code $493
#cmd LNInterpreterCommand_ParamsCount $494
#cmd LNInterpreterCommand_Param $495
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $D61
#cmd LNInterpreterCommandList_Create $4AC
#cmd LNInterpreterCommandList_AddCommand $498
#cmd LNInterpreterCommandList_AddCommand1 $49A
#cmd LNInterpreterCommandList_AddCommand2 $49D
#cmd LNInterpreterCommandList_AddCommand3 $4A1
#cmd LNInterpreterCommandList_AddCommand4 $4A6
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $D6C
#cmd LNInterpreterCommandDelegate_Create $8D1
#cmd LNInterpreter_Create $4C0
#cmd LNInterpreter_Clear $4B1
#cmd LNInterpreter_Run $4B2
#cmd LNInterpreter_IsRunning $4B4
#cmd LNInterpreter_Update $4B5
#cmd LNInterpreter_Terminate $4B6
#cmd LNInterpreter_RegisterCommandHandler $4B7
#cmd LNInterpreter_SetWaitMode $4BA
#cmd LNInterpreter_GetWaitMode $4BC
#cmd LNInterpreter_SetWaitCount $4BD
#cmd LNInterpreter_GetWaitCount $4BF
#cmd LNInterpreter_SetPrototype_OnSerialize $D77
#cmd LNInterpreter_SetPrototype_OnUpdateWait $D83
#cmd LNEngineSettings_SetMainWindowSize $4C3
#cmd LNEngineSettings_SetMainWorldViewSize $4C6
#cmd LNEngineSettings_SetMainWindowTitle $4C9
#cmd LNEngineSettings_AddAssetDirectory $4CB
#cmd LNEngineSettings_AddAssetArchive $4CD
#cmd LNEngineSettings_SetFrameRate $4D0
#cmd LNEngineSettings_SetUITheme $4D2
#cmd LNEngineSettings_SetFontFile $4D4
#cmd LNEngineSettings_SetDebugToolEnabled $4D6
#cmd LNEngineSettings_SetEngineLogEnabled $4D8
#cmd LNEngineSettings_SetEngineLogFilePath $4DA
#cmd LNEngineSettings_SetDeveloperToolEnabled $4DC
#cmd LNEngine_Initialize $4DF
#cmd LNEngine_Finalize $4E0
#cmd LNEngine_Update $4E1
#cmd LNEngine_Run $4E2
#cmd LNEngine_Time $4E4
#cmd LNEngine_GetCamera $4E5
#cmd LNEngine_GetMainLight $4E6
#cmd LNEngine_GetRenderView $4E7
#cmd LNApplication_Create $4EC
#cmd LNApplication_OnInit $4E9
#cmd LNApplication_OnUpdate $4EA
#cmd LNApplication_World $4EB
#cmd LNApplication_SetPrototype_OnSerialize $D8E
#cmd LNApplication_SetPrototype_OnInit $D99
#cmd LNApplication_SetPrototype_OnUpdate $DA4
#cmd LNDebug_SetGuideGridEnabled $4EE
#cmd LNDebug_SetPhysicsDebugDrawEnabled $4F0
#cmd LNDebug_Print $4F2
#cmd LNDebug_PrintWithTime $4F4
#cmd LNDebug_PrintWithTimeAndColor $4F7
#cmd LNObjectSerializeHandler_Create $8FD
#cmd LNEventConnectionSerializeHandler_Create $908
#cmd LNVariantSerializeHandler_Create $913
#cmd LNZVTestClass1SerializeHandler_Create $91E
#cmd LNZVTestEventArgs1SerializeHandler_Create $929
#cmd LNSerializer2SerializeHandler_Create $934
#cmd LNAssetObjectSerializeHandler_Create $93F
#cmd LNAssetImportSettingsSerializeHandler_Create $94A
#cmd LNAssetModelSerializeHandler_Create $955
#cmd LNSoundSerializeHandler_Create $960
#cmd LNTextureSerializeHandler_Create $96B
#cmd LNTexture2DSerializeHandler_Create $976
#cmd LNShaderSerializeHandler_Create $981
#cmd LNRenderViewSerializeHandler_Create $98C
#cmd LNMaterialSerializeHandler_Create $997
#cmd LNMeshNodeSerializeHandler_Create $9A2
#cmd LNStaticMeshModelSerializeHandler_Create $9AD
#cmd LNSkinnedMeshModelSerializeHandler_Create $9B8
#cmd LNAnimationControllerSerializeHandler_Create $9C3
#cmd LNCollisionShapeSerializeHandler_Create $9CE
#cmd LNBoxCollisionShapeSerializeHandler_Create $9D9
#cmd LNAnimationClipSerializeHandler_Create $9E4
#cmd LNAnimationStateSerializeHandler_Create $9EF
#cmd LNEffectResourceSerializeHandler_Create $9FA
#cmd LNParticleEmitterModelSerializeHandler_Create $A05
#cmd LNParticleModelSerializeHandler_Create $A10
#cmd LNComponentSerializeHandler_Create $A1B
#cmd LNVisualComponentSerializeHandler_Create $A26
#cmd LNSpriteComponentSerializeHandler_Create $A31
#cmd LNCharacterControllerSerializeHandler_Create $A3C
#cmd LNWorldSerializeHandler_Create $A47
#cmd LNComponentListSerializeHandler_Create $A52
#cmd LNWorldObjectSerializeHandler_Create $A5D
#cmd LNWorldObjectPreUpdateHandler_Create $A68
#cmd LNWorldObjectUpdateHandler_Create $A73
#cmd LNVisualObjectSerializeHandler_Create $A7E
#cmd LNVisualObjectPreUpdateHandler_Create $A89
#cmd LNVisualObjectUpdateHandler_Create $A94
#cmd LNCameraSerializeHandler_Create $A9F
#cmd LNCameraPreUpdateHandler_Create $AAA
#cmd LNCameraUpdateHandler_Create $AB5
#cmd LNEnvironmentLightSerializeHandler_Create $AC0
#cmd LNEnvironmentLightPreUpdateHandler_Create $ACB
#cmd LNEnvironmentLightUpdateHandler_Create $AD6
#cmd LNDirectionalLightSerializeHandler_Create $AE1
#cmd LNDirectionalLightPreUpdateHandler_Create $AEC
#cmd LNDirectionalLightUpdateHandler_Create $AF7
#cmd LNPointLightSerializeHandler_Create $B02
#cmd LNPointLightPreUpdateHandler_Create $B0D
#cmd LNPointLightUpdateHandler_Create $B18
#cmd LNSpotLightSerializeHandler_Create $B23
#cmd LNSpotLightPreUpdateHandler_Create $B2E
#cmd LNSpotLightUpdateHandler_Create $B39
#cmd LNSpriteSerializeHandler_Create $B44
#cmd LNSpritePreUpdateHandler_Create $B4F
#cmd LNSpriteUpdateHandler_Create $B5A
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $B65
#cmd LNRaycasterSerializeHandler_Create $B70
#cmd LNRaycastResultSerializeHandler_Create $B7B
#cmd LNWorldRenderViewSerializeHandler_Create $B86
#cmd LNBoxMeshSerializeHandler_Create $B91
#cmd LNBoxMeshPreUpdateHandler_Create $B9C
#cmd LNBoxMeshUpdateHandler_Create $BA7
#cmd LNPlaneMeshSerializeHandler_Create $BB2
#cmd LNPlaneMeshPreUpdateHandler_Create $BBD
#cmd LNPlaneMeshUpdateHandler_Create $BC8
#cmd LNStaticMeshSerializeHandler_Create $BD3
#cmd LNStaticMeshPreUpdateHandler_Create $BDE
#cmd LNStaticMeshUpdateHandler_Create $BE9
#cmd LNStaticMeshComponentSerializeHandler_Create $BF4
#cmd LNSkinnedMeshComponentSerializeHandler_Create $BFF
#cmd LNCollisionSerializeHandler_Create $C0A
#cmd LNTriggerBodyComponentSerializeHandler_Create $C15
#cmd LNParticleEmitterSerializeHandler_Create $C20
#cmd LNParticleEmitterPreUpdateHandler_Create $C2B
#cmd LNParticleEmitterUpdateHandler_Create $C36
#cmd LNLevelSerializeHandler_Create $C41
#cmd LNLevelStartHandler_Create $C4C
#cmd LNLevelStopHandler_Create $C57
#cmd LNLevelPauseHandler_Create $C62
#cmd LNLevelResumeHandler_Create $C6D
#cmd LNLevelUpdateHandler_Create $C78
#cmd LNUIEventArgsSerializeHandler_Create $C83
#cmd LNUILayoutElementSerializeHandler_Create $C8E
#cmd LNUIElementSerializeHandler_Create $C99
#cmd LNUITextBlockSerializeHandler_Create $CA4
#cmd LNUISpriteSerializeHandler_Create $CAF
#cmd LNUIIconSerializeHandler_Create $CBA
#cmd LNUIMessageTextAreaSerializeHandler_Create $CC5
#cmd LNUILayoutPanelSerializeHandler_Create $CD0
#cmd LNUIBoxLayoutSerializeHandler_Create $CDB
#cmd LNUIGridLayoutSerializeHandler_Create $CE6
#cmd LNUIControlSerializeHandler_Create $CF1
#cmd LNUIButtonBaseSerializeHandler_Create $CFC
#cmd LNUIButtonSerializeHandler_Create $D07
#cmd LNUIWindowSerializeHandler_Create $D12
#cmd LNUIListItemSerializeHandler_Create $D1D
#cmd LNUIListItemsControlSerializeHandler_Create $D28
#cmd LNUIListBoxItemSerializeHandler_Create $D33
#cmd LNUIListBoxSerializeHandler_Create $D3E
#cmd LNInputGestureSerializeHandler_Create $D49
#cmd LNKeyGestureSerializeHandler_Create $D54
#cmd LNInterpreterCommandSerializeHandler_Create $D5F
#cmd LNInterpreterCommandListSerializeHandler_Create $D6A
#cmd LNInterpreterSerializeHandler_Create $D75
#cmd LNInterpreterUpdateWaitHandler_Create $D81
#cmd LNApplicationSerializeHandler_Create $D8C
#cmd LNApplicationInitHandler_Create $D97
#cmd LNApplicationUpdateHandler_Create $DA2



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
