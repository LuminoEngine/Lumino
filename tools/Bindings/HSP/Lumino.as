
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
#const global LN_MOUSE_BUTTONS_NONE 0
#const global LN_MOUSE_BUTTONS_LEFT 1
#const global LN_MOUSE_BUTTONS_RIGHT 2
#const global LN_MOUSE_BUTTONS_MIDDLE 3
#const global LN_MOUSE_BUTTONS_X1 4
#const global LN_MOUSE_BUTTONS_X2 5
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
#cmd LNVector3_GetX $46B
#cmd LNVector3_SetX $46D
#cmd LNVector3_GetY $46E
#cmd LNVector3_SetY $470
#cmd LNVector3_GetZ $471
#cmd LNVector3_SetZ $473
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $48A
#cmd LNVector4_SetX $48C
#cmd LNVector4_GetY $48D
#cmd LNVector4_SetY $48F
#cmd LNVector4_GetZ $490
#cmd LNVector4_SetZ $492
#cmd LNVector4_GetW $493
#cmd LNVector4_SetW $495
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $4A8
#cmd LNQuaternion_SetX $4AA
#cmd LNQuaternion_GetY $4AB
#cmd LNQuaternion_SetY $4AD
#cmd LNQuaternion_GetZ $4AE
#cmd LNQuaternion_SetZ $4B0
#cmd LNQuaternion_GetW $4B1
#cmd LNQuaternion_SetW $4B3
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $4C7
#cmd LNMatrix_SetRow0 $4C9
#cmd LNMatrix_GetRow1 $4CA
#cmd LNMatrix_SetRow1 $4CC
#cmd LNMatrix_GetRow2 $4CD
#cmd LNMatrix_SetRow2 $4CF
#cmd LNMatrix_GetRow3 $4D0
#cmd LNMatrix_SetRow3 $4D2
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $104
#cmd LNColor_GetR $552
#cmd LNColor_SetR $554
#cmd LNColor_GetG $555
#cmd LNColor_SetG $557
#cmd LNColor_GetB $558
#cmd LNColor_SetB $55A
#cmd LNColor_GetA $55B
#cmd LNColor_SetA $55D
#cmd LNColor_SetZeros $109
#cmd LNColor_Set $10A
#cmd LNPoint $10F
#cmd LNPoint_GetX $571
#cmd LNPoint_SetX $573
#cmd LNPoint_GetY $574
#cmd LNPoint_SetY $576
#cmd LNPoint_SetZeros $112
#cmd LNPoint_Set $113
#cmd LNSize $116
#cmd LNSize_GetWidth $581
#cmd LNSize_SetWidth $583
#cmd LNSize_GetHeight $584
#cmd LNSize_SetHeight $586
#cmd LNSize_SetZeros $119
#cmd LNSize_Set $11A
#cmd LNRect $11D
#cmd LNRect_GetX $591
#cmd LNRect_SetX $593
#cmd LNRect_GetY $594
#cmd LNRect_SetY $596
#cmd LNRect_GetWidth $597
#cmd LNRect_SetWidth $599
#cmd LNRect_GetHeight $59A
#cmd LNRect_SetHeight $59C
#cmd LNRect_SetZeros $122
#cmd LNRect_Set $123
#cmd LNRect_GetLeft $128
#cmd LNRect_SetSize $129
#cmd LNRect_GetSize $12B
#cmd LNThickness $12C
#cmd LNThickness_GetLeft $5B5
#cmd LNThickness_SetLeft $5B7
#cmd LNThickness_GetTop $5B8
#cmd LNThickness_SetTop $5BA
#cmd LNThickness_GetRight $5BB
#cmd LNThickness_SetRight $5BD
#cmd LNThickness_GetBottom $5BE
#cmd LNThickness_SetBottom $5C0
#cmd LNThickness_SetZeros $131
#cmd LNThickness_Set $132
#cmd LNCornerRadius $137
#cmd LNCornerRadius_GetTopleft $5D3
#cmd LNCornerRadius_SetTopleft $5D5
#cmd LNCornerRadius_GetTopright $5D6
#cmd LNCornerRadius_SetTopright $5D8
#cmd LNCornerRadius_GetBottomright $5D9
#cmd LNCornerRadius_SetBottomright $5DB
#cmd LNCornerRadius_GetBottomleft $5DC
#cmd LNCornerRadius_SetBottomleft $5DE
#cmd LNCornerRadius_SetZeros $13C
#cmd LNCornerRadius_Set $13D

#cmd LNObject_Release $7A
#cmd LNObject_Retain $7B
#cmd LNObject_GetReferenceCount $7C
#cmd LNObject_SetPrototype_OnSerialize $846
#cmd LNEventConnection_SetPrototype_OnSerialize $851
#cmd LNPromiseFailureDelegate_Create $4ED
#cmd LNVariant_Create $81
#cmd LNVariant_SetInt $82
#cmd LNVariant_GetInt $84
#cmd LNVariant_SetPrototype_OnSerialize $85C
#cmd LNZVTestDelegate1_Create $4F6
#cmd LNZVTestDelegate2_Create $4FC
#cmd LNZVTestDelegate3_Create $501
#cmd LNZVTestEventHandler1_Create $506
#cmd LNZVTestEventHandler2_Create $50B
#cmd LNZVTestPromise1_ThenWith $50E
#cmd LNZVTestPromise1_CatchWith $510
#cmd LNZVTestPromise2_ThenWith $514
#cmd LNZVTestPromise2_CatchWith $516
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $867
#cmd LNZVTestEventArgs1_Create $B0
#cmd LNZVTestEventArgs1_CreateWithValue $B1
#cmd LNZVTestEventArgs1_GetValue $AF
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $872
#cmd LNSerializer2_SetPrototype_OnSerialize $87D
#cmd LNAssetObject_SetPrototype_OnSerialize $888
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $893
#cmd LNAssetModel_Create $BF
#cmd LNAssetModel_Target $BE
#cmd LNAssetModel_SetPrototype_OnSerialize $89E
#cmd LNAssets_SaveAssetToLocalFile $C2
#cmd LNAssets_LoadAssetFromLocalFile $C5
#cmd LNAssets_LoadAsset $C7
#cmd LNAssets_ReloadAsset $C9
#cmd LNAssets_ReadAllText $CC
#cmd LNAudio_PlayBGM $D0
#cmd LNAudio_StopBGM $D5
#cmd LNAudio_PlayBGS $D7
#cmd LNAudio_StopBGS $DC
#cmd LNAudio_PlayME $DE
#cmd LNAudio_StopME $E2
#cmd LNAudio_PlaySE $E3
#cmd LNAudio_PlaySE3D $E7
#cmd LNAudio_StopSE $ED
#cmd LNTexture2DDelegate_Create $54A
#cmd LNTexture2DPromise_ThenWith $54D
#cmd LNTexture2DPromise_CatchWith $54F
#cmd LNTexture_SetPrototype_OnSerialize $8A9
#cmd LNTexture2D_Create $148
#cmd LNTexture2D_CreateWithFormat $14B
#cmd LNTexture2D_Load $144
#cmd LNTexture2D_LoadEmoji $146
#cmd LNTexture2D_SetPrototype_OnSerialize $8B4
#cmd LNShader_Load $150
#cmd LNShader_SetFloat $153
#cmd LNShader_SetVector3 $156
#cmd LNShader_SetVector4 $159
#cmd LNShader_SetPrototype_OnSerialize $8BF
#cmd LNRenderView_SetPrototype_OnSerialize $8CA
#cmd LNMaterial_Create $170
#cmd LNMaterial_SetMainTexture $167
#cmd LNMaterial_GetMainTexture $169
#cmd LNMaterial_SetShadingModel $16A
#cmd LNMaterial_GetShadingModel $16C
#cmd LNMaterial_SetShader $16D
#cmd LNMaterial_GetShader $16F
#cmd LNMaterial_SetPrototype_OnSerialize $8D5
#cmd LNStaticMeshModel_Load $172
#cmd LNStaticMeshModel_MaterialCount $174
#cmd LNStaticMeshModel_Material $175
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $8E0
#cmd LNSkinnedMeshModel_Load $178
#cmd LNSkinnedMeshModel_GetAnimationController $17A
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $8EB
#cmd LNAnimationController_AddClip $17C
#cmd LNAnimationController_Play $17E
#cmd LNAnimationController_SetPrototype_OnSerialize $8F6
#cmd LNCollisionShape_SetPrototype_OnSerialize $901
#cmd LNBoxCollisionShape_Create $183
#cmd LNBoxCollisionShape_CreateWHD $185
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $90C
#cmd LNAnimationClip_Load $18F
#cmd LNAnimationClip_SetHierarchicalAnimationMode $191
#cmd LNAnimationClip_HierarchicalAnimationMode $193
#cmd LNAnimationClip_SetPrototype_OnSerialize $917
#cmd LNAnimationState_SetPrototype_OnSerialize $922
#cmd LNEffectResource_SetPrototype_OnSerialize $92D
#cmd LNParticleEmitterModel_Create $1B4
#cmd LNParticleEmitterModel_SetMaxParticles $1A0
#cmd LNParticleEmitterModel_SetSpawnRate $1A2
#cmd LNParticleEmitterModel_SetLifeTime $1A4
#cmd LNParticleEmitterModel_SetupBoxShape $1A6
#cmd LNParticleEmitterModel_SetSize $1A8
#cmd LNParticleEmitterModel_SetForwardVelocityMin $1AA
#cmd LNParticleEmitterModel_SetForwardVelocityMax $1AC
#cmd LNParticleEmitterModel_SetForwardScale $1AE
#cmd LNParticleEmitterModel_SetGeometryDirection $1B0
#cmd LNParticleEmitterModel_SetupSpriteModule $1B2
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $938
#cmd LNParticleModel_Create $1BB
#cmd LNParticleModel_SetLoop $1B6
#cmd LNParticleModel_IsLoop $1B8
#cmd LNParticleModel_AddEmitter $1B9
#cmd LNParticleModel_SetPrototype_OnSerialize $943
#cmd LNComponent_SetPrototype_OnSerialize $94E
#cmd LNVisualComponent_SetVisible $1BE
#cmd LNVisualComponent_IsVisible $1C0
#cmd LNVisualComponent_SetPrototype_OnSerialize $959
#cmd LNSpriteComponent_SetTexture $1C2
#cmd LNSpriteComponent_SetPrototype_OnSerialize $964
#cmd LNCollisionEventHandler_Create $63F
#cmd LNCharacterController_Create $1DF
#cmd LNCharacterController_SetVelocity $1CC
#cmd LNCharacterController_GetVelocity $1CE
#cmd LNCharacterController_SetInputControlEnabled $1CF
#cmd LNCharacterController_SetCameraControlEnabled $1D1
#cmd LNCharacterController_SetHeight $1D3
#cmd LNCharacterController_GetHeight $1D5
#cmd LNCharacterController_SetCameraRadius $1D6
#cmd LNCharacterController_GetCameraRadius $1D8
#cmd LNCharacterController_SetCollisionEnter $1D9
#cmd LNCharacterController_SetCollisionLeave $1DB
#cmd LNCharacterController_SetCollisionStay $1DD
#cmd LNCharacterController_SetPrototype_OnSerialize $96F
#cmd LNWorld_Add $1E1
#cmd LNWorld_SetPrototype_OnSerialize $97A
#cmd LNComponentList_GetLength $65A
#cmd LNComponentList_GetItem $65C
#cmd LNComponentList_SetPrototype_OnSerialize $985
#cmd LNWorldObject_Create $218
#cmd LNWorldObject_SetPosition $1E5
#cmd LNWorldObject_SetPositionXYZ $1E7
#cmd LNWorldObject_GetPosition $1EB
#cmd LNWorldObject_SetRotationQuaternion $1EC
#cmd LNWorldObject_SetRotation $1EE
#cmd LNWorldObject_GetRotation $1F2
#cmd LNWorldObject_SetScale $1F3
#cmd LNWorldObject_SetScaleS $1F5
#cmd LNWorldObject_SetScaleXYZ $1F7
#cmd LNWorldObject_GetScale $1FB
#cmd LNWorldObject_SetCenterPoint $1FC
#cmd LNWorldObject_SetCenterPointXYZ $1FE
#cmd LNWorldObject_GetCenterPoint $202
#cmd LNWorldObject_LookAt $203
#cmd LNWorldObject_LookAtXYZ $205
#cmd LNWorldObject_AddComponent $209
#cmd LNWorldObject_RemoveComponent $20B
#cmd LNWorldObject_AddTag $20D
#cmd LNWorldObject_RemoveTag $20F
#cmd LNWorldObject_HasTag $211
#cmd LNWorldObject_Destroy $213
#cmd LNWorldObject_GetComponents $214
#cmd LNWorldObject_SetPrototype_OnSerialize $990
#cmd LNWorldObject_SetPrototype_OnPreUpdate $99B
#cmd LNWorldObject_SetPrototype_OnUpdate $9A6
#cmd LNVisualObject_SetVisible $21A
#cmd LNVisualObject_IsVisible $21C
#cmd LNVisualObject_SetOpacity $21D
#cmd LNVisualObject_GetOpacity $21F
#cmd LNVisualObject_SetPrototype_OnSerialize $9B1
#cmd LNVisualObject_SetPrototype_OnPreUpdate $9BC
#cmd LNVisualObject_SetPrototype_OnUpdate $9C7
#cmd LNCamera_SetPrototype_OnSerialize $9D2
#cmd LNCamera_SetPrototype_OnPreUpdate $9DD
#cmd LNCamera_SetPrototype_OnUpdate $9E8
#cmd LNEnvironmentLight_SetEnabled $222
#cmd LNEnvironmentLight_IsEnabled $224
#cmd LNEnvironmentLight_SetColor $225
#cmd LNEnvironmentLight_GetColor $227
#cmd LNEnvironmentLight_SetAmbientColor $228
#cmd LNEnvironmentLight_GetAmbientColor $22A
#cmd LNEnvironmentLight_GetSkyColor $22B
#cmd LNEnvironmentLight_SetSkyColor $22C
#cmd LNEnvironmentLight_GetGroundColor $22E
#cmd LNEnvironmentLight_SetGroundColor $22F
#cmd LNEnvironmentLight_SetIntensity $231
#cmd LNEnvironmentLight_GetIntensity $233
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $234
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $236
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $237
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $239
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $9F3
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $9FE
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $A09
#cmd LNDirectionalLight_Create $24A
#cmd LNDirectionalLight_CreateWithColor $24B
#cmd LNDirectionalLight_SetEnabled $23B
#cmd LNDirectionalLight_IsEnabled $23D
#cmd LNDirectionalLight_SetColor $23E
#cmd LNDirectionalLight_GetColor $240
#cmd LNDirectionalLight_SetIntensity $241
#cmd LNDirectionalLight_GetIntensity $243
#cmd LNDirectionalLight_SetShadowEffectiveDistance $244
#cmd LNDirectionalLight_GetShadowEffectiveDistance $246
#cmd LNDirectionalLight_SetShadowEffectiveDepth $247
#cmd LNDirectionalLight_GetShadowEffectiveDepth $249
#cmd LNDirectionalLight_SetPrototype_OnSerialize $A14
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $A1F
#cmd LNDirectionalLight_SetPrototype_OnUpdate $A2A
#cmd LNPointLight_Create $25D
#cmd LNPointLight_CreateWithColorAndRange $25E
#cmd LNPointLight_SetEnabled $24E
#cmd LNPointLight_IsEnabled $250
#cmd LNPointLight_SetColor $251
#cmd LNPointLight_GetColor $253
#cmd LNPointLight_SetIntensity $254
#cmd LNPointLight_GetIntensity $256
#cmd LNPointLight_SetRange $257
#cmd LNPointLight_GetRange $259
#cmd LNPointLight_SetAttenuation $25A
#cmd LNPointLight_GetAttenuation $25C
#cmd LNPointLight_SetPrototype_OnSerialize $A35
#cmd LNPointLight_SetPrototype_OnPreUpdate $A40
#cmd LNPointLight_SetPrototype_OnUpdate $A4B
#cmd LNSpotLight_Create $277
#cmd LNSpotLight_CreateWithColorAndRange $278
#cmd LNSpotLight_SetEnabled $262
#cmd LNSpotLight_IsEnabled $264
#cmd LNSpotLight_SetColor $265
#cmd LNSpotLight_GetColor $267
#cmd LNSpotLight_SetIntensity $268
#cmd LNSpotLight_GetIntensity $26A
#cmd LNSpotLight_SetRange $26B
#cmd LNSpotLight_GetRange $26D
#cmd LNSpotLight_SetAttenuation $26E
#cmd LNSpotLight_GetAttenuation $270
#cmd LNSpotLight_SetAngle $271
#cmd LNSpotLight_GetAngle $273
#cmd LNSpotLight_SetPenumbra $274
#cmd LNSpotLight_GetPenumbra $276
#cmd LNSpotLight_SetPrototype_OnSerialize $A56
#cmd LNSpotLight_SetPrototype_OnPreUpdate $A61
#cmd LNSpotLight_SetPrototype_OnUpdate $A6C
#cmd LNTestDelegate_Create $706
#cmd LNSprite_Create $28E
#cmd LNSprite_CreateWithTexture $28F
#cmd LNSprite_CreateWithTextureAndSize $291
#cmd LNSprite_SetTexture $280
#cmd LNSprite_SetSize $282
#cmd LNSprite_SetSizeWH $284
#cmd LNSprite_SetSourceRectXYWH $287
#cmd LNSprite_SetCallerTest $28C
#cmd LNSprite_SetPrototype_OnSerialize $A77
#cmd LNSprite_SetPrototype_OnPreUpdate $A82
#cmd LNSprite_SetPrototype_OnUpdate $A8D
#cmd LNCameraOrbitControlComponent_Create $296
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $A98
#cmd LNRaycaster_FromScreen $298
#cmd LNRaycaster_IntersectPlane $29A
#cmd LNRaycaster_SetPrototype_OnSerialize $AA3
#cmd LNRaycastResult_GetPoint $29F
#cmd LNRaycastResult_SetPrototype_OnSerialize $AAE
#cmd LNWorldRenderView_SetGuideGridEnabled $2A1
#cmd LNWorldRenderView_GetGuideGridEnabled $2A3
#cmd LNWorldRenderView_SetPrototype_OnSerialize $AB9
#cmd LNBoxMesh_Create $2A5
#cmd LNBoxMesh_SetPrototype_OnSerialize $AC4
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $ACF
#cmd LNBoxMesh_SetPrototype_OnUpdate $ADA
#cmd LNPlaneMesh_Create $2A7
#cmd LNPlaneMesh_SetPrototype_OnSerialize $AE5
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $AF0
#cmd LNPlaneMesh_SetPrototype_OnUpdate $AFB
#cmd LNStaticMesh_Load $2A9
#cmd LNStaticMesh_GetModel $2AB
#cmd LNStaticMesh_MakeCollisionBody $2AC
#cmd LNStaticMesh_SetPrototype_OnSerialize $B06
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $B11
#cmd LNStaticMesh_SetPrototype_OnUpdate $B1C
#cmd LNStaticMeshComponent_Create $2B3
#cmd LNStaticMeshComponent_SetModel $2AF
#cmd LNStaticMeshComponent_MakeCollisionBody $2B1
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $B27
#cmd LNSkinnedMeshComponent_Create $2B7
#cmd LNSkinnedMeshComponent_SetModel $2B5
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $B32
#cmd LNCollision_GetWorldObject $2B9
#cmd LNCollision_SetPrototype_OnSerialize $B3D
#cmd LNTriggerBodyComponent_Create $2BD
#cmd LNTriggerBodyComponent_AddCollisionShape $2BB
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $B48
#cmd LNParticleEmitter_Create $2BF
#cmd LNParticleEmitter_SetPrototype_OnSerialize $B53
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $B5E
#cmd LNParticleEmitter_SetPrototype_OnUpdate $B69
#cmd LNScene_SetClearMode $2C2
#cmd LNScene_SetSkyColor $2C4
#cmd LNScene_SetSkyHorizonColor $2C6
#cmd LNScene_SetSkyCloudColor $2C8
#cmd LNScene_SetSkyOverlayColor $2CA
#cmd LNScene_GotoLevel $2CC
#cmd LNScene_CallLevel $2CF
#cmd LNScene_ReturnLevel $2D2
#cmd LNScene_ActiveLevel $2D4
#cmd LNScene_IsTransitionEffectRunning $2D5
#cmd LNScene_SetTransitionEffectMode $2D6
#cmd LNScene_TransitionEffectMode $2D8
#cmd LNScene_SetTransitionDuration $2D9
#cmd LNScene_TransitionDuration $2DB
#cmd LNScene_SetTransitionEffectColor $2DC
#cmd LNScene_TransitionEffectColor $2DE
#cmd LNScene_SetTransitionEffectMaskTexture $2DF
#cmd LNScene_TransitionEffectMaskTexture $2E1
#cmd LNScene_SetTransitionEffectVague $2E2
#cmd LNScene_TransitionEffectVague $2E4
#cmd LNScene_StartFadeOut $2E5
#cmd LNScene_StartFadeIn $2E6
#cmd LNScene_SetScreenBlendColor $2E7
#cmd LNScene_ScreenBlendColor $2E9
#cmd LNScene_SetAntialiasEnabled $2EA
#cmd LNScene_IsAntialiasEnabled $2EC
#cmd LNScene_SetSSREnabled $2ED
#cmd LNScene_IsSSREnabled $2EF
#cmd LNScene_SetSSAOEnabled $2F0
#cmd LNScene_IsSSAOEnabled $2F2
#cmd LNScene_SetBloomEnabled $2F3
#cmd LNScene_IsBloomEnabled $2F5
#cmd LNScene_SetDOFEnabled $2F6
#cmd LNScene_IsDOFEnabled $2F8
#cmd LNScene_SetTonemapEnabled $2F9
#cmd LNScene_IsTonemapEnabled $2FB
#cmd LNScene_SetVignetteEnabled $2FC
#cmd LNScene_IsVignetteEnabled $2FE
#cmd LNScene_SetGammaEnabled $2FF
#cmd LNScene_IsGammaEnabled $301
#cmd LNLevel_Create $311
#cmd LNLevel_AddObject $303
#cmd LNLevel_RemoveObject $305
#cmd LNLevel_AddSubLevel $307
#cmd LNLevel_RemoveSubLevel $309
#cmd LNLevel_RemoveAllSubLevels $30B
#cmd LNLevel_SetPrototype_OnSerialize $B74
#cmd LNLevel_SetPrototype_OnStart $B7F
#cmd LNLevel_SetPrototype_OnStop $B8A
#cmd LNLevel_SetPrototype_OnPause $B95
#cmd LNLevel_SetPrototype_OnResume $BA0
#cmd LNLevel_SetPrototype_OnUpdate $BAB
#cmd LNUIEventArgs_Sender $31C
#cmd LNUIEventArgs_SetPrototype_OnSerialize $BB6
#cmd LNUIGeneralEventHandler_Create $75B
#cmd LNUIEventHandler_Create $760
#cmd LNUILayoutElement_SetPrototype_OnSerialize $BC1
#cmd LNUIElement_SetSize $32E
#cmd LNUIElement_SetSizeWH $330
#cmd LNUIElement_SetWidth $333
#cmd LNUIElement_GetWidth $335
#cmd LNUIElement_SetHeight $336
#cmd LNUIElement_GetHeight $338
#cmd LNUIElement_SetMargin $339
#cmd LNUIElement_GetMargin $33B
#cmd LNUIElement_SetPadding $33C
#cmd LNUIElement_GetPadding $33E
#cmd LNUIElement_SetHAlignment $33F
#cmd LNUIElement_GetHAlignment $341
#cmd LNUIElement_SetVAlignment $342
#cmd LNUIElement_GetVAlignment $344
#cmd LNUIElement_SetAlignments $345
#cmd LNUIElement_SetPosition $348
#cmd LNUIElement_SetPositionXYZ $34A
#cmd LNUIElement_GetPosition $34E
#cmd LNUIElement_SetRotation $34F
#cmd LNUIElement_SetEulerAngles $351
#cmd LNUIElement_GetRotation $355
#cmd LNUIElement_SetScale $356
#cmd LNUIElement_SetScaleS $358
#cmd LNUIElement_SetScaleXY $35A
#cmd LNUIElement_GetScale $35D
#cmd LNUIElement_SetCenterPoint $35E
#cmd LNUIElement_SetCenterPointXYZ $360
#cmd LNUIElement_GetCenterPoint $364
#cmd LNUIElement_SetEnabled $365
#cmd LNUIElement_IsEnabled $367
#cmd LNUIElement_SetData $368
#cmd LNUIElement_GetData $36A
#cmd LNUIElement_SetBackgroundColor $36B
#cmd LNUIElement_GetBackgroundColor $36D
#cmd LNUIElement_SetBorderThickness $36E
#cmd LNUIElement_GetBorderThickness $370
#cmd LNUIElement_SetBorderColor $371
#cmd LNUIElement_GetBorderColor $373
#cmd LNUIElement_SetCornerRadius $374
#cmd LNUIElement_GetCornerRadius $376
#cmd LNUIElement_SetVisibility $377
#cmd LNUIElement_GetVisibility $379
#cmd LNUIElement_SetOpacity $37A
#cmd LNUIElement_GetOpacity $37C
#cmd LNUIElement_AddChild $37D
#cmd LNUIElement_SetPrototype_OnSerialize $BCC
#cmd LNUITextBlock_Create $383
#cmd LNUITextBlock_CreateWithText $384
#cmd LNUITextBlock_SetText $380
#cmd LNUITextBlock_GetText $382
#cmd LNUITextBlock_SetPrototype_OnSerialize $BD7
#cmd LNUISprite_Create $391
#cmd LNUISprite_CreateWithTexture $392
#cmd LNUISprite_SetTexture $387
#cmd LNUISprite_SetSourceRect $389
#cmd LNUISprite_SetSourceRectXYWH $38B
#cmd LNUISprite_GetSourceRect $390
#cmd LNUISprite_SetPrototype_OnSerialize $BE2
#cmd LNUIIcon_LoadFontIcon $395
#cmd LNUIIcon_LoadFontIconWithNameSize $397
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $39A
#cmd LNUIIcon_SetPrototype_OnSerialize $BED
#cmd LNUIMessageTextArea_Create $3A3
#cmd LNUIMessageTextArea_SetText $39F
#cmd LNUIMessageTextArea_SetTypingSpeed $3A1
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $BF8
#cmd LNUI_Add $3A5
#cmd LNUI_Remove $3A7
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $C03
#cmd LNUIBoxLayout_Create $3AE
#cmd LNUIBoxLayout_SetOrientation $3AB
#cmd LNUIBoxLayout_GetOrientation $3AD
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $C0E
#cmd LNUIGridLayout_Create $3BE
#cmd LNUIGridLayout_SetColumnCount $3B0
#cmd LNUIGridLayout_SetRow $3B2
#cmd LNUIGridLayout_SetColumn $3B5
#cmd LNUIGridLayout_SetPlacement $3B8
#cmd LNUIGridLayout_SetPrototype_OnSerialize $C19
#cmd LNUIControl_Create $3CA
#cmd LNUIControl_AddInlineVisual $3CB
#cmd LNUIControl_SetPrototype_OnSerialize $C24
#cmd LNUIButtonBase_SetText $3CF
#cmd LNUIButtonBase_SetPrototype_OnSerialize $C2F
#cmd LNUIButton_Create $3D2
#cmd LNUIButton_CreateWithText $3D3
#cmd LNUIButton_ConnectOnClicked $3D5
#cmd LNUIButton_SetPrototype_OnSerialize $C3A
#cmd LNUIWindow_Create $3D8
#cmd LNUIWindow_SetPrototype_OnSerialize $C45
#cmd LNUIListItem_ConnectOnSubmit $3DD
#cmd LNUIListItem_SetPrototype_OnSerialize $C50
#cmd LNUIListItemsControl_SetItemsLayoutPanel $3E0
#cmd LNUIListItemsControl_SetSubmitMode $3E2
#cmd LNUIListItemsControl_GetSubmitMode $3E4
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $C5B
#cmd LNUIListBoxItem_Create $3E6
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $C66
#cmd LNUIListBox_Create $3EB
#cmd LNUIListBox_AddItem $3E9
#cmd LNUIListBox_SetPrototype_OnSerialize $C71
#cmd LNInput_IsPressed $3ED
#cmd LNInput_IsTriggered $3EF
#cmd LNInput_IsTriggeredOff $3F1
#cmd LNInput_IsRepeated $3F3
#cmd LNInput_GetAxisValue $3F5
#cmd LNInput_ClearAllBindings $3F7
#cmd LNMouse_Pressed $3F9
#cmd LNMouse_Triggered $3FB
#cmd LNMouse_TriggeredOff $3FD
#cmd LNMouse_Repeated $3FF
#cmd LNMouse_Position $401
#cmd LNInterpreterCommand_Code $403
#cmd LNInterpreterCommand_ParamsCount $404
#cmd LNInterpreterCommand_Param $405
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $C7C
#cmd LNInterpreterCommandList_Create $41C
#cmd LNInterpreterCommandList_AddCommand $408
#cmd LNInterpreterCommandList_AddCommand1 $40A
#cmd LNInterpreterCommandList_AddCommand2 $40D
#cmd LNInterpreterCommandList_AddCommand3 $411
#cmd LNInterpreterCommandList_AddCommand4 $416
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $C87
#cmd LNInterpreterCommandDelegate_Create $818
#cmd LNInterpreter_Create $430
#cmd LNInterpreter_Clear $421
#cmd LNInterpreter_Run $422
#cmd LNInterpreter_IsRunning $424
#cmd LNInterpreter_Update $425
#cmd LNInterpreter_Terminate $426
#cmd LNInterpreter_RegisterCommandHandler $427
#cmd LNInterpreter_SetWaitMode $42A
#cmd LNInterpreter_GetWaitMode $42C
#cmd LNInterpreter_SetWaitCount $42D
#cmd LNInterpreter_GetWaitCount $42F
#cmd LNInterpreter_SetPrototype_OnSerialize $C92
#cmd LNInterpreter_SetPrototype_OnUpdateWait $C9E
#cmd LNEngineSettings_SetMainWindowSize $433
#cmd LNEngineSettings_SetMainWorldViewSize $436
#cmd LNEngineSettings_SetMainWindowTitle $439
#cmd LNEngineSettings_AddAssetDirectory $43B
#cmd LNEngineSettings_AddAssetArchive $43D
#cmd LNEngineSettings_SetFrameRate $440
#cmd LNEngineSettings_SetUITheme $442
#cmd LNEngineSettings_SetFontFile $444
#cmd LNEngineSettings_SetDebugToolEnabled $446
#cmd LNEngineSettings_SetEngineLogEnabled $448
#cmd LNEngineSettings_SetEngineLogFilePath $44A
#cmd LNEngineSettings_SetDeveloperToolEnabled $44C
#cmd LNEngine_Initialize $44F
#cmd LNEngine_Finalize $450
#cmd LNEngine_Update $451
#cmd LNEngine_Run $452
#cmd LNEngine_Time $454
#cmd LNEngine_GetCamera $455
#cmd LNEngine_GetMainLight $456
#cmd LNEngine_GetRenderView $457
#cmd LNApplication_Create $45C
#cmd LNApplication_OnInit $459
#cmd LNApplication_OnUpdate $45A
#cmd LNApplication_World $45B
#cmd LNApplication_SetPrototype_OnSerialize $CA9
#cmd LNApplication_SetPrototype_OnInit $CB4
#cmd LNApplication_SetPrototype_OnUpdate $CBF
#cmd LNDebug_SetGuideGridEnabled $45E
#cmd LNDebug_SetPhysicsDebugDrawEnabled $460
#cmd LNDebug_Print $462
#cmd LNDebug_PrintWithTime $464
#cmd LNDebug_PrintWithTimeAndColor $467
#cmd LNObjectSerializeHandler_Create $844
#cmd LNEventConnectionSerializeHandler_Create $84F
#cmd LNVariantSerializeHandler_Create $85A
#cmd LNZVTestClass1SerializeHandler_Create $865
#cmd LNZVTestEventArgs1SerializeHandler_Create $870
#cmd LNSerializer2SerializeHandler_Create $87B
#cmd LNAssetObjectSerializeHandler_Create $886
#cmd LNAssetImportSettingsSerializeHandler_Create $891
#cmd LNAssetModelSerializeHandler_Create $89C
#cmd LNTextureSerializeHandler_Create $8A7
#cmd LNTexture2DSerializeHandler_Create $8B2
#cmd LNShaderSerializeHandler_Create $8BD
#cmd LNRenderViewSerializeHandler_Create $8C8
#cmd LNMaterialSerializeHandler_Create $8D3
#cmd LNStaticMeshModelSerializeHandler_Create $8DE
#cmd LNSkinnedMeshModelSerializeHandler_Create $8E9
#cmd LNAnimationControllerSerializeHandler_Create $8F4
#cmd LNCollisionShapeSerializeHandler_Create $8FF
#cmd LNBoxCollisionShapeSerializeHandler_Create $90A
#cmd LNAnimationClipSerializeHandler_Create $915
#cmd LNAnimationStateSerializeHandler_Create $920
#cmd LNEffectResourceSerializeHandler_Create $92B
#cmd LNParticleEmitterModelSerializeHandler_Create $936
#cmd LNParticleModelSerializeHandler_Create $941
#cmd LNComponentSerializeHandler_Create $94C
#cmd LNVisualComponentSerializeHandler_Create $957
#cmd LNSpriteComponentSerializeHandler_Create $962
#cmd LNCharacterControllerSerializeHandler_Create $96D
#cmd LNWorldSerializeHandler_Create $978
#cmd LNComponentListSerializeHandler_Create $983
#cmd LNWorldObjectSerializeHandler_Create $98E
#cmd LNWorldObjectPreUpdateHandler_Create $999
#cmd LNWorldObjectUpdateHandler_Create $9A4
#cmd LNVisualObjectSerializeHandler_Create $9AF
#cmd LNVisualObjectPreUpdateHandler_Create $9BA
#cmd LNVisualObjectUpdateHandler_Create $9C5
#cmd LNCameraSerializeHandler_Create $9D0
#cmd LNCameraPreUpdateHandler_Create $9DB
#cmd LNCameraUpdateHandler_Create $9E6
#cmd LNEnvironmentLightSerializeHandler_Create $9F1
#cmd LNEnvironmentLightPreUpdateHandler_Create $9FC
#cmd LNEnvironmentLightUpdateHandler_Create $A07
#cmd LNDirectionalLightSerializeHandler_Create $A12
#cmd LNDirectionalLightPreUpdateHandler_Create $A1D
#cmd LNDirectionalLightUpdateHandler_Create $A28
#cmd LNPointLightSerializeHandler_Create $A33
#cmd LNPointLightPreUpdateHandler_Create $A3E
#cmd LNPointLightUpdateHandler_Create $A49
#cmd LNSpotLightSerializeHandler_Create $A54
#cmd LNSpotLightPreUpdateHandler_Create $A5F
#cmd LNSpotLightUpdateHandler_Create $A6A
#cmd LNSpriteSerializeHandler_Create $A75
#cmd LNSpritePreUpdateHandler_Create $A80
#cmd LNSpriteUpdateHandler_Create $A8B
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $A96
#cmd LNRaycasterSerializeHandler_Create $AA1
#cmd LNRaycastResultSerializeHandler_Create $AAC
#cmd LNWorldRenderViewSerializeHandler_Create $AB7
#cmd LNBoxMeshSerializeHandler_Create $AC2
#cmd LNBoxMeshPreUpdateHandler_Create $ACD
#cmd LNBoxMeshUpdateHandler_Create $AD8
#cmd LNPlaneMeshSerializeHandler_Create $AE3
#cmd LNPlaneMeshPreUpdateHandler_Create $AEE
#cmd LNPlaneMeshUpdateHandler_Create $AF9
#cmd LNStaticMeshSerializeHandler_Create $B04
#cmd LNStaticMeshPreUpdateHandler_Create $B0F
#cmd LNStaticMeshUpdateHandler_Create $B1A
#cmd LNStaticMeshComponentSerializeHandler_Create $B25
#cmd LNSkinnedMeshComponentSerializeHandler_Create $B30
#cmd LNCollisionSerializeHandler_Create $B3B
#cmd LNTriggerBodyComponentSerializeHandler_Create $B46
#cmd LNParticleEmitterSerializeHandler_Create $B51
#cmd LNParticleEmitterPreUpdateHandler_Create $B5C
#cmd LNParticleEmitterUpdateHandler_Create $B67
#cmd LNLevelSerializeHandler_Create $B72
#cmd LNLevelStartHandler_Create $B7D
#cmd LNLevelStopHandler_Create $B88
#cmd LNLevelPauseHandler_Create $B93
#cmd LNLevelResumeHandler_Create $B9E
#cmd LNLevelUpdateHandler_Create $BA9
#cmd LNUIEventArgsSerializeHandler_Create $BB4
#cmd LNUILayoutElementSerializeHandler_Create $BBF
#cmd LNUIElementSerializeHandler_Create $BCA
#cmd LNUITextBlockSerializeHandler_Create $BD5
#cmd LNUISpriteSerializeHandler_Create $BE0
#cmd LNUIIconSerializeHandler_Create $BEB
#cmd LNUIMessageTextAreaSerializeHandler_Create $BF6
#cmd LNUILayoutPanelSerializeHandler_Create $C01
#cmd LNUIBoxLayoutSerializeHandler_Create $C0C
#cmd LNUIGridLayoutSerializeHandler_Create $C17
#cmd LNUIControlSerializeHandler_Create $C22
#cmd LNUIButtonBaseSerializeHandler_Create $C2D
#cmd LNUIButtonSerializeHandler_Create $C38
#cmd LNUIWindowSerializeHandler_Create $C43
#cmd LNUIListItemSerializeHandler_Create $C4E
#cmd LNUIListItemsControlSerializeHandler_Create $C59
#cmd LNUIListBoxItemSerializeHandler_Create $C64
#cmd LNUIListBoxSerializeHandler_Create $C6F
#cmd LNInterpreterCommandSerializeHandler_Create $C7A
#cmd LNInterpreterCommandListSerializeHandler_Create $C85
#cmd LNInterpreterSerializeHandler_Create $C90
#cmd LNInterpreterUpdateWaitHandler_Create $C9C
#cmd LNApplicationSerializeHandler_Create $CA7
#cmd LNApplicationInitHandler_Create $CB2
#cmd LNApplicationUpdateHandler_Create $CBD



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
