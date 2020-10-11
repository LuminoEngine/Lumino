
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
#const global LN_SCENE_CLEAR_MODE_NONE 0
#const global LN_SCENE_CLEAR_MODE_COLOR_AND_DEPTH 1
#const global LN_SCENE_CLEAR_MODE_SKY 2
#const global LN_SCENE_CLEAR_MODE_SKY0 3
#const global LN_SCENE_CLEAR_MODE_SKY_DOME 4
#const global LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT 0
#const global LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT_Y 1
#const global LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION 2
#const global LN_HIERARCHICAL_ANIMATION_MODE_DISABLE_TRANSLATION 3
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
#cmd LNVector3_GetX $418
#cmd LNVector3_SetX $41A
#cmd LNVector3_GetY $41B
#cmd LNVector3_SetY $41D
#cmd LNVector3_GetZ $41E
#cmd LNVector3_SetZ $420
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $437
#cmd LNVector4_SetX $439
#cmd LNVector4_GetY $43A
#cmd LNVector4_SetY $43C
#cmd LNVector4_GetZ $43D
#cmd LNVector4_SetZ $43F
#cmd LNVector4_GetW $440
#cmd LNVector4_SetW $442
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $455
#cmd LNQuaternion_SetX $457
#cmd LNQuaternion_GetY $458
#cmd LNQuaternion_SetY $45A
#cmd LNQuaternion_GetZ $45B
#cmd LNQuaternion_SetZ $45D
#cmd LNQuaternion_GetW $45E
#cmd LNQuaternion_SetW $460
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $474
#cmd LNMatrix_SetRow0 $476
#cmd LNMatrix_GetRow1 $477
#cmd LNMatrix_SetRow1 $479
#cmd LNMatrix_GetRow2 $47A
#cmd LNMatrix_SetRow2 $47C
#cmd LNMatrix_GetRow3 $47D
#cmd LNMatrix_SetRow3 $47F
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $F9
#cmd LNColor_GetR $4FD
#cmd LNColor_SetR $4FF
#cmd LNColor_GetG $500
#cmd LNColor_SetG $502
#cmd LNColor_GetB $503
#cmd LNColor_SetB $505
#cmd LNColor_GetA $506
#cmd LNColor_SetA $508
#cmd LNColor_SetZeros $FE
#cmd LNColor_Set $FF
#cmd LNPoint $104
#cmd LNPoint_GetX $51C
#cmd LNPoint_SetX $51E
#cmd LNPoint_GetY $51F
#cmd LNPoint_SetY $521
#cmd LNPoint_SetZeros $107
#cmd LNPoint_Set $108
#cmd LNSize $10B
#cmd LNSize_GetWidth $52C
#cmd LNSize_SetWidth $52E
#cmd LNSize_GetHeight $52F
#cmd LNSize_SetHeight $531
#cmd LNSize_SetZeros $10E
#cmd LNSize_Set $10F
#cmd LNRect $112
#cmd LNRect_GetX $53C
#cmd LNRect_SetX $53E
#cmd LNRect_GetY $53F
#cmd LNRect_SetY $541
#cmd LNRect_GetWidth $542
#cmd LNRect_SetWidth $544
#cmd LNRect_GetHeight $545
#cmd LNRect_SetHeight $547
#cmd LNRect_SetZeros $117
#cmd LNRect_Set $118
#cmd LNRect_GetLeft $11D
#cmd LNRect_SetSize $11E
#cmd LNRect_GetSize $120
#cmd LNThickness $121
#cmd LNThickness_GetLeft $560
#cmd LNThickness_SetLeft $562
#cmd LNThickness_GetTop $563
#cmd LNThickness_SetTop $565
#cmd LNThickness_GetRight $566
#cmd LNThickness_SetRight $568
#cmd LNThickness_GetBottom $569
#cmd LNThickness_SetBottom $56B
#cmd LNThickness_SetZeros $126
#cmd LNThickness_Set $127
#cmd LNCornerRadius $12C
#cmd LNCornerRadius_GetTopleft $57E
#cmd LNCornerRadius_SetTopleft $580
#cmd LNCornerRadius_GetTopright $581
#cmd LNCornerRadius_SetTopright $583
#cmd LNCornerRadius_GetBottomright $584
#cmd LNCornerRadius_SetBottomright $586
#cmd LNCornerRadius_GetBottomleft $587
#cmd LNCornerRadius_SetBottomleft $589
#cmd LNCornerRadius_SetZeros $131
#cmd LNCornerRadius_Set $132

#cmd LNObject_Release $74
#cmd LNObject_Retain $75
#cmd LNObject_GetReferenceCount $76
#cmd LNObject_SetPrototype_OnSerialize $7B7
#cmd LNEventConnection_SetPrototype_OnSerialize $7C2
#cmd LNPromiseFailureDelegate_Create $49A
#cmd LNVariant_Create $7B
#cmd LNVariant_SetInt $7C
#cmd LNVariant_GetInt $7E
#cmd LNVariant_SetPrototype_OnSerialize $7CD
#cmd LNZVTestDelegate1_Create $4A3
#cmd LNZVTestDelegate2_Create $4A9
#cmd LNZVTestDelegate3_Create $4AE
#cmd LNZVTestEventHandler1_Create $4B3
#cmd LNZVTestEventHandler2_Create $4B8
#cmd LNZVTestPromise1_ThenWith $4BB
#cmd LNZVTestPromise1_CatchWith $4BD
#cmd LNZVTestPromise2_ThenWith $4C1
#cmd LNZVTestPromise2_CatchWith $4C3
#cmd LNZVTestClass1_Create $A7
#cmd LNZVTestClass1_SetTestDelegate1 $91
#cmd LNZVTestClass1_SetTestDelegate2 $93
#cmd LNZVTestClass1_SetTestDelegate3 $95
#cmd LNZVTestClass1_CallTestDelegate1 $97
#cmd LNZVTestClass1_CallTestDelegate2 $99
#cmd LNZVTestClass1_CallTestDelegate3 $9C
#cmd LNZVTestClass1_LoadAsync $9D
#cmd LNZVTestClass1_ExecuteAsync $9F
#cmd LNZVTestClass1_GetFilePath $A0
#cmd LNZVTestClass1_ConnectOnEvent1 $A1
#cmd LNZVTestClass1_RaiseEvent1 $A3
#cmd LNZVTestClass1_ConnectOnEvent2 $A4
#cmd LNZVTestClass1_RaiseEvent2 $A6
#cmd LNZVTestClass1_SetPrototype_OnSerialize $7D8
#cmd LNZVTestEventArgs1_Create $AA
#cmd LNZVTestEventArgs1_CreateWithValue $AB
#cmd LNZVTestEventArgs1_GetValue $A9
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $7E3
#cmd LNSerializer2_SetPrototype_OnSerialize $7EE
#cmd LNAssetModel_Create $B7
#cmd LNAssetModel_Target $B6
#cmd LNAssetModel_SetPrototype_OnSerialize $7F9
#cmd LNAssets_SaveAssetToLocalFile $BA
#cmd LNAssets_LoadAssetFromLocalFile $BD
#cmd LNAssets_LoadAsset $BF
#cmd LNAssets_ReloadAsset $C1
#cmd LNAudio_PlayBGM $C5
#cmd LNAudio_StopBGM $CA
#cmd LNAudio_PlayBGS $CC
#cmd LNAudio_StopBGS $D1
#cmd LNAudio_PlayME $D3
#cmd LNAudio_StopME $D7
#cmd LNAudio_PlaySE $D8
#cmd LNAudio_PlaySE3D $DC
#cmd LNAudio_StopSE $E2
#cmd LNTexture2DDelegate_Create $4F5
#cmd LNTexture2DPromise_ThenWith $4F8
#cmd LNTexture2DPromise_CatchWith $4FA
#cmd LNTexture_SetPrototype_OnSerialize $804
#cmd LNTexture2D_Create $13D
#cmd LNTexture2D_CreateWithFormat $140
#cmd LNTexture2D_Load $139
#cmd LNTexture2D_LoadEmoji $13B
#cmd LNTexture2D_SetPrototype_OnSerialize $80F
#cmd LNShader_Load $145
#cmd LNShader_SetFloat $147
#cmd LNShader_SetVector3 $14A
#cmd LNShader_SetVector4 $14D
#cmd LNShader_SetPrototype_OnSerialize $81A
#cmd LNRenderView_SetPrototype_OnSerialize $825
#cmd LNMaterial_SetShader $158
#cmd LNMaterial_GetShader $15A
#cmd LNMaterial_SetPrototype_OnSerialize $830
#cmd LNStaticMeshModel_Load $15C
#cmd LNStaticMeshModel_MaterialCount $15E
#cmd LNStaticMeshModel_Material $15F
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $83B
#cmd LNSkinnedMeshModel_Load $162
#cmd LNSkinnedMeshModel_GetAnimationController $164
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $846
#cmd LNAnimationController_AddClip $166
#cmd LNAnimationController_Play $168
#cmd LNAnimationController_SetPrototype_OnSerialize $851
#cmd LNCollisionShape_SetPrototype_OnSerialize $85C
#cmd LNBoxCollisionShape_Create $16D
#cmd LNBoxCollisionShape_CreateWHD $16F
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $867
#cmd LNAnimationClip_Load $179
#cmd LNAnimationClip_SetHierarchicalAnimationMode $17B
#cmd LNAnimationClip_HierarchicalAnimationMode $17D
#cmd LNAnimationClip_SetPrototype_OnSerialize $872
#cmd LNAnimationState_SetPrototype_OnSerialize $87D
#cmd LNComponent_SetPrototype_OnSerialize $888
#cmd LNVisualComponent_SetVisible $181
#cmd LNVisualComponent_IsVisible $183
#cmd LNVisualComponent_SetPrototype_OnSerialize $893
#cmd LNSpriteComponent_SetTexture $185
#cmd LNSpriteComponent_SetPrototype_OnSerialize $89E
#cmd LNCollisionEventHandler_Create $5C5
#cmd LNCharacterController_Create $1A2
#cmd LNCharacterController_SetVelocity $18F
#cmd LNCharacterController_GetVelocity $191
#cmd LNCharacterController_SetInputControlEnabled $192
#cmd LNCharacterController_SetCameraControlEnabled $194
#cmd LNCharacterController_SetHeight $196
#cmd LNCharacterController_GetHeight $198
#cmd LNCharacterController_SetCameraRadius $199
#cmd LNCharacterController_GetCameraRadius $19B
#cmd LNCharacterController_SetCollisionEnter $19C
#cmd LNCharacterController_SetCollisionLeave $19E
#cmd LNCharacterController_SetCollisionStay $1A0
#cmd LNCharacterController_SetPrototype_OnSerialize $8A9
#cmd LNWorld_Add $1A4
#cmd LNWorld_SetPrototype_OnSerialize $8B4
#cmd LNComponentList_GetLength $5E0
#cmd LNComponentList_GetItem $5E2
#cmd LNComponentList_SetPrototype_OnSerialize $8BF
#cmd LNWorldObject_Create $1DB
#cmd LNWorldObject_SetPosition $1A8
#cmd LNWorldObject_SetPositionXYZ $1AA
#cmd LNWorldObject_GetPosition $1AE
#cmd LNWorldObject_SetRotationQuaternion $1AF
#cmd LNWorldObject_SetRotation $1B1
#cmd LNWorldObject_GetRotation $1B5
#cmd LNWorldObject_SetScale $1B6
#cmd LNWorldObject_SetScaleS $1B8
#cmd LNWorldObject_SetScaleXYZ $1BA
#cmd LNWorldObject_GetScale $1BE
#cmd LNWorldObject_SetCenterPoint $1BF
#cmd LNWorldObject_SetCenterPointXYZ $1C1
#cmd LNWorldObject_GetCenterPoint $1C5
#cmd LNWorldObject_LookAt $1C6
#cmd LNWorldObject_LookAtXYZ $1C8
#cmd LNWorldObject_AddComponent $1CC
#cmd LNWorldObject_RemoveComponent $1CE
#cmd LNWorldObject_AddTag $1D0
#cmd LNWorldObject_RemoveTag $1D2
#cmd LNWorldObject_HasTag $1D4
#cmd LNWorldObject_Destroy $1D6
#cmd LNWorldObject_GetComponents $1D7
#cmd LNWorldObject_SetPrototype_OnSerialize $8CA
#cmd LNWorldObject_SetPrototype_OnPreUpdate $8D5
#cmd LNWorldObject_SetPrototype_OnUpdate $8E0
#cmd LNVisualObject_SetVisible $1DD
#cmd LNVisualObject_IsVisible $1DF
#cmd LNVisualObject_SetPrototype_OnSerialize $8EB
#cmd LNVisualObject_SetPrototype_OnPreUpdate $8F6
#cmd LNVisualObject_SetPrototype_OnUpdate $901
#cmd LNCamera_SetPrototype_OnSerialize $90C
#cmd LNCamera_SetPrototype_OnPreUpdate $917
#cmd LNCamera_SetPrototype_OnUpdate $922
#cmd LNEnvironmentLight_SetEnabled $1E2
#cmd LNEnvironmentLight_IsEnabled $1E4
#cmd LNEnvironmentLight_SetColor $1E5
#cmd LNEnvironmentLight_GetColor $1E7
#cmd LNEnvironmentLight_SetAmbientColor $1E8
#cmd LNEnvironmentLight_GetAmbientColor $1EA
#cmd LNEnvironmentLight_GetSkyColor $1EB
#cmd LNEnvironmentLight_SetSkyColor $1EC
#cmd LNEnvironmentLight_GetGroundColor $1EE
#cmd LNEnvironmentLight_SetGroundColor $1EF
#cmd LNEnvironmentLight_SetIntensity $1F1
#cmd LNEnvironmentLight_GetIntensity $1F3
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $1F4
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $1F6
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $1F7
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $1F9
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $92D
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $938
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $943
#cmd LNDirectionalLight_Create $20A
#cmd LNDirectionalLight_CreateWithColor $20B
#cmd LNDirectionalLight_SetEnabled $1FB
#cmd LNDirectionalLight_IsEnabled $1FD
#cmd LNDirectionalLight_SetColor $1FE
#cmd LNDirectionalLight_GetColor $200
#cmd LNDirectionalLight_SetIntensity $201
#cmd LNDirectionalLight_GetIntensity $203
#cmd LNDirectionalLight_SetShadowEffectiveDistance $204
#cmd LNDirectionalLight_GetShadowEffectiveDistance $206
#cmd LNDirectionalLight_SetShadowEffectiveDepth $207
#cmd LNDirectionalLight_GetShadowEffectiveDepth $209
#cmd LNDirectionalLight_SetPrototype_OnSerialize $94E
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $959
#cmd LNDirectionalLight_SetPrototype_OnUpdate $964
#cmd LNPointLight_Create $21D
#cmd LNPointLight_CreateWithColorAndRange $21E
#cmd LNPointLight_SetEnabled $20E
#cmd LNPointLight_IsEnabled $210
#cmd LNPointLight_SetColor $211
#cmd LNPointLight_GetColor $213
#cmd LNPointLight_SetIntensity $214
#cmd LNPointLight_GetIntensity $216
#cmd LNPointLight_SetRange $217
#cmd LNPointLight_GetRange $219
#cmd LNPointLight_SetAttenuation $21A
#cmd LNPointLight_GetAttenuation $21C
#cmd LNPointLight_SetPrototype_OnSerialize $96F
#cmd LNPointLight_SetPrototype_OnPreUpdate $97A
#cmd LNPointLight_SetPrototype_OnUpdate $985
#cmd LNSpotLight_Create $237
#cmd LNSpotLight_CreateWithColorAndRange $238
#cmd LNSpotLight_SetEnabled $222
#cmd LNSpotLight_IsEnabled $224
#cmd LNSpotLight_SetColor $225
#cmd LNSpotLight_GetColor $227
#cmd LNSpotLight_SetIntensity $228
#cmd LNSpotLight_GetIntensity $22A
#cmd LNSpotLight_SetRange $22B
#cmd LNSpotLight_GetRange $22D
#cmd LNSpotLight_SetAttenuation $22E
#cmd LNSpotLight_GetAttenuation $230
#cmd LNSpotLight_SetAngle $231
#cmd LNSpotLight_GetAngle $233
#cmd LNSpotLight_SetPenumbra $234
#cmd LNSpotLight_GetPenumbra $236
#cmd LNSpotLight_SetPrototype_OnSerialize $990
#cmd LNSpotLight_SetPrototype_OnPreUpdate $99B
#cmd LNSpotLight_SetPrototype_OnUpdate $9A6
#cmd LNTestDelegate_Create $687
#cmd LNSprite_Create $24E
#cmd LNSprite_CreateWithTexture $24F
#cmd LNSprite_CreateWithTextureAndSize $251
#cmd LNSprite_SetTexture $240
#cmd LNSprite_SetSize $242
#cmd LNSprite_SetSizeWH $244
#cmd LNSprite_SetSourceRectXYWH $247
#cmd LNSprite_SetCallerTest $24C
#cmd LNSprite_SetPrototype_OnSerialize $9B1
#cmd LNSprite_SetPrototype_OnPreUpdate $9BC
#cmd LNSprite_SetPrototype_OnUpdate $9C7
#cmd LNCameraOrbitControlComponent_Create $256
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $9D2
#cmd LNRaycaster_FromScreen $258
#cmd LNRaycaster_IntersectPlane $25A
#cmd LNRaycaster_SetPrototype_OnSerialize $9DD
#cmd LNRaycastResult_GetPoint $25F
#cmd LNRaycastResult_SetPrototype_OnSerialize $9E8
#cmd LNWorldRenderView_SetGuideGridEnabled $261
#cmd LNWorldRenderView_GetGuideGridEnabled $263
#cmd LNWorldRenderView_SetPrototype_OnSerialize $9F3
#cmd LNBoxMesh_Create $265
#cmd LNBoxMesh_SetPrototype_OnSerialize $9FE
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $A09
#cmd LNBoxMesh_SetPrototype_OnUpdate $A14
#cmd LNPlaneMesh_Create $267
#cmd LNPlaneMesh_SetPrototype_OnSerialize $A1F
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $A2A
#cmd LNPlaneMesh_SetPrototype_OnUpdate $A35
#cmd LNStaticMesh_Load $269
#cmd LNStaticMesh_GetModel $26B
#cmd LNStaticMesh_MakeCollisionBody $26C
#cmd LNStaticMesh_SetPrototype_OnSerialize $A40
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $A4B
#cmd LNStaticMesh_SetPrototype_OnUpdate $A56
#cmd LNStaticMeshComponent_Create $273
#cmd LNStaticMeshComponent_SetModel $26F
#cmd LNStaticMeshComponent_MakeCollisionBody $271
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $A61
#cmd LNSkinnedMeshComponent_Create $277
#cmd LNSkinnedMeshComponent_SetModel $275
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $A6C
#cmd LNCollision_GetWorldObject $279
#cmd LNCollision_SetPrototype_OnSerialize $A77
#cmd LNTriggerBodyComponent_Create $27D
#cmd LNTriggerBodyComponent_AddCollisionShape $27B
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $A82
#cmd LNScene_SetClearMode $27F
#cmd LNScene_SetSkyColor $281
#cmd LNScene_SetSkyHorizonColor $283
#cmd LNScene_SetSkyCloudColor $285
#cmd LNScene_SetSkyOverlayColor $287
#cmd LNScene_GotoLevel $289
#cmd LNScene_CallLevel $28C
#cmd LNScene_ReturnLevel $28F
#cmd LNScene_ActiveLevel $291
#cmd LNScene_IsTransitionEffectRunning $292
#cmd LNScene_SetTransitionEffectMode $293
#cmd LNScene_TransitionEffectMode $295
#cmd LNScene_SetTransitionDuration $296
#cmd LNScene_TransitionDuration $298
#cmd LNScene_SetTransitionEffectColor $299
#cmd LNScene_TransitionEffectColor $29B
#cmd LNScene_SetTransitionEffectMaskTexture $29C
#cmd LNScene_TransitionEffectMaskTexture $29E
#cmd LNScene_SetTransitionEffectVague $29F
#cmd LNScene_TransitionEffectVague $2A1
#cmd LNScene_StartFadeOut $2A2
#cmd LNScene_StartFadeIn $2A3
#cmd LNScene_SetScreenBlendColor $2A4
#cmd LNScene_ScreenBlendColor $2A6
#cmd LNScene_SetAntialiasEnabled $2A7
#cmd LNScene_IsAntialiasEnabled $2A9
#cmd LNScene_SetSSREnabled $2AA
#cmd LNScene_IsSSREnabled $2AC
#cmd LNScene_SetSSAOEnabled $2AD
#cmd LNScene_IsSSAOEnabled $2AF
#cmd LNScene_SetBloomEnabled $2B0
#cmd LNScene_IsBloomEnabled $2B2
#cmd LNScene_SetDOFEnabled $2B3
#cmd LNScene_IsDOFEnabled $2B5
#cmd LNScene_SetTonemapEnabled $2B6
#cmd LNScene_IsTonemapEnabled $2B8
#cmd LNScene_SetVignetteEnabled $2B9
#cmd LNScene_IsVignetteEnabled $2BB
#cmd LNScene_SetGammaEnabled $2BC
#cmd LNScene_IsGammaEnabled $2BE
#cmd LNLevel_Create $2CE
#cmd LNLevel_AddObject $2C0
#cmd LNLevel_RemoveObject $2C2
#cmd LNLevel_AddSubLevel $2C4
#cmd LNLevel_RemoveSubLevel $2C6
#cmd LNLevel_RemoveAllSubLevels $2C8
#cmd LNLevel_SetPrototype_OnSerialize $A8D
#cmd LNLevel_SetPrototype_OnStart $A98
#cmd LNLevel_SetPrototype_OnStop $AA3
#cmd LNLevel_SetPrototype_OnPause $AAE
#cmd LNLevel_SetPrototype_OnResume $AB9
#cmd LNLevel_SetPrototype_OnUpdate $AC4
#cmd LNUIEventArgs_Sender $2D9
#cmd LNUIEventArgs_SetPrototype_OnSerialize $ACF
#cmd LNUIGeneralEventHandler_Create $6DB
#cmd LNUIEventHandler_Create $6E0
#cmd LNUILayoutElement_SetPrototype_OnSerialize $ADA
#cmd LNUIElement_SetSize $2EB
#cmd LNUIElement_SetSizeWH $2ED
#cmd LNUIElement_SetMargin $2F0
#cmd LNUIElement_GetMargin $2F2
#cmd LNUIElement_SetPadding $2F3
#cmd LNUIElement_GetPadding $2F5
#cmd LNUIElement_SetHAlignment $2F6
#cmd LNUIElement_GetHAlignment $2F8
#cmd LNUIElement_SetVAlignment $2F9
#cmd LNUIElement_GetVAlignment $2FB
#cmd LNUIElement_SetAlignments $2FC
#cmd LNUIElement_SetPosition $2FF
#cmd LNUIElement_SetPositionXYZ $301
#cmd LNUIElement_GetPosition $305
#cmd LNUIElement_SetRotation $306
#cmd LNUIElement_SetEulerAngles $308
#cmd LNUIElement_GetRotation $30C
#cmd LNUIElement_SetScale $30D
#cmd LNUIElement_SetScaleS $30F
#cmd LNUIElement_SetScaleXY $311
#cmd LNUIElement_GetScale $314
#cmd LNUIElement_SetCenterPoint $315
#cmd LNUIElement_SetCenterPointXYZ $317
#cmd LNUIElement_GetCenterPoint $31B
#cmd LNUIElement_SetEnabled $31C
#cmd LNUIElement_IsEnabled $31E
#cmd LNUIElement_SetData $31F
#cmd LNUIElement_GetData $321
#cmd LNUIElement_SetBackgroundColor $322
#cmd LNUIElement_GetBackgroundColor $324
#cmd LNUIElement_SetBorderThickness $325
#cmd LNUIElement_GetBorderThickness $327
#cmd LNUIElement_SetBorderColor $328
#cmd LNUIElement_GetBorderColor $32A
#cmd LNUIElement_SetCornerRadius $32B
#cmd LNUIElement_GetCornerRadius $32D
#cmd LNUIElement_SetVisibility $32E
#cmd LNUIElement_GetVisibility $330
#cmd LNUIElement_SetOpacity $331
#cmd LNUIElement_GetOpacity $333
#cmd LNUIElement_AddChild $334
#cmd LNUIElement_SetPrototype_OnSerialize $AE5
#cmd LNUITextBlock_Create $33A
#cmd LNUITextBlock_CreateWithText $33B
#cmd LNUITextBlock_SetText $337
#cmd LNUITextBlock_GetText $339
#cmd LNUITextBlock_SetPrototype_OnSerialize $AF0
#cmd LNUISprite_Create $348
#cmd LNUISprite_CreateWithTexture $349
#cmd LNUISprite_SetTexture $33E
#cmd LNUISprite_SetSourceRect $340
#cmd LNUISprite_SetSourceRectXYWH $342
#cmd LNUISprite_GetSourceRect $347
#cmd LNUISprite_SetPrototype_OnSerialize $AFB
#cmd LNUIIcon_LoadFontIcon $34C
#cmd LNUIIcon_LoadFontIconWithNameSize $34E
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $351
#cmd LNUIIcon_SetPrototype_OnSerialize $B06
#cmd LNUI_Add $356
#cmd LNUI_Remove $358
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $B11
#cmd LNUIBoxLayout_Create $35F
#cmd LNUIBoxLayout_SetOrientation $35C
#cmd LNUIBoxLayout_GetOrientation $35E
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $B1C
#cmd LNUIGridLayout_Create $36F
#cmd LNUIGridLayout_SetColumnCount $361
#cmd LNUIGridLayout_SetRow $363
#cmd LNUIGridLayout_SetColumn $366
#cmd LNUIGridLayout_SetPlacement $369
#cmd LNUIGridLayout_SetPrototype_OnSerialize $B27
#cmd LNUIControl_Create $37B
#cmd LNUIControl_AddInlineVisual $37C
#cmd LNUIControl_SetPrototype_OnSerialize $B32
#cmd LNUIButtonBase_SetText $380
#cmd LNUIButtonBase_SetPrototype_OnSerialize $B3D
#cmd LNUIButton_Create $383
#cmd LNUIButton_CreateWithText $384
#cmd LNUIButton_ConnectOnClicked $386
#cmd LNUIButton_SetPrototype_OnSerialize $B48
#cmd LNUIWindow_Create $389
#cmd LNUIWindow_SetPrototype_OnSerialize $B53
#cmd LNUIListItem_ConnectOnSubmit $38E
#cmd LNUIListItem_SetPrototype_OnSerialize $B5E
#cmd LNUIListItemsControl_SetItemsLayoutPanel $391
#cmd LNUIListItemsControl_SetSubmitMode $393
#cmd LNUIListItemsControl_GetSubmitMode $395
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $B69
#cmd LNUIListBoxItem_Create $397
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $B74
#cmd LNUIListBox_Create $39C
#cmd LNUIListBox_AddItem $39A
#cmd LNUIListBox_SetPrototype_OnSerialize $B7F
#cmd LNInput_IsPressed $39E
#cmd LNInput_IsTriggered $3A0
#cmd LNInput_IsTriggeredOff $3A2
#cmd LNInput_IsRepeated $3A4
#cmd LNInput_GetAxisValue $3A6
#cmd LNInput_ClearAllBindings $3A8
#cmd LNMouse_Pressed $3AA
#cmd LNMouse_Triggered $3AC
#cmd LNMouse_TriggeredOff $3AE
#cmd LNMouse_Repeated $3B0
#cmd LNMouse_Position $3B2
#cmd LNInterpreterCommand_Code $3B4
#cmd LNInterpreterCommand_ParamsCount $3B5
#cmd LNInterpreterCommand_Param $3B6
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $B8A
#cmd LNInterpreterCommandList_Create $3CD
#cmd LNInterpreterCommandList_AddCommand $3B9
#cmd LNInterpreterCommandList_AddCommand1 $3BB
#cmd LNInterpreterCommandList_AddCommand2 $3BE
#cmd LNInterpreterCommandList_AddCommand3 $3C2
#cmd LNInterpreterCommandList_AddCommand4 $3C7
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $B95
#cmd LNInterpreterCommandDelegate_Create $789
#cmd LNInterpreter_Create $3E1
#cmd LNInterpreter_Clear $3D2
#cmd LNInterpreter_Run $3D3
#cmd LNInterpreter_IsRunning $3D5
#cmd LNInterpreter_Update $3D6
#cmd LNInterpreter_Terminate $3D7
#cmd LNInterpreter_RegisterCommandHandler $3D8
#cmd LNInterpreter_SetWaitMode $3DB
#cmd LNInterpreter_GetWaitMode $3DD
#cmd LNInterpreter_SetWaitCount $3DE
#cmd LNInterpreter_GetWaitCount $3E0
#cmd LNInterpreter_SetPrototype_OnSerialize $BA0
#cmd LNInterpreter_SetPrototype_OnUpdateWait $BAC
#cmd LNEngineSettings_SetMainWindowSize $3E4
#cmd LNEngineSettings_SetMainWorldViewSize $3E7
#cmd LNEngineSettings_SetMainWindowTitle $3EA
#cmd LNEngineSettings_AddAssetDirectory $3EC
#cmd LNEngineSettings_AddAssetArchive $3EE
#cmd LNEngineSettings_SetFrameRate $3F1
#cmd LNEngineSettings_SetDefaultUITheme $3F3
#cmd LNEngineSettings_SetDebugToolEnabled $3F5
#cmd LNEngineSettings_SetEngineLogEnabled $3F7
#cmd LNEngineSettings_SetEngineLogFilePath $3F9
#cmd LNEngine_Initialize $3FC
#cmd LNEngine_Finalize $3FD
#cmd LNEngine_Update $3FE
#cmd LNEngine_Run $3FF
#cmd LNEngine_Time $401
#cmd LNEngine_GetCamera $402
#cmd LNEngine_GetMainLight $403
#cmd LNEngine_GetRenderView $404
#cmd LNApplication_Create $409
#cmd LNApplication_OnInit $406
#cmd LNApplication_OnUpdate $407
#cmd LNApplication_World $408
#cmd LNApplication_SetPrototype_OnSerialize $BB7
#cmd LNApplication_SetPrototype_OnInit $BC2
#cmd LNApplication_SetPrototype_OnUpdate $BCD
#cmd LNDebug_SetGuideGridEnabled $40B
#cmd LNDebug_SetPhysicsDebugDrawEnabled $40D
#cmd LNDebug_Print $40F
#cmd LNDebug_PrintWithTime $411
#cmd LNDebug_PrintWithTimeAndColor $414
#cmd LNObjectSerializeHandler_Create $7B5
#cmd LNEventConnectionSerializeHandler_Create $7C0
#cmd LNVariantSerializeHandler_Create $7CB
#cmd LNZVTestClass1SerializeHandler_Create $7D6
#cmd LNZVTestEventArgs1SerializeHandler_Create $7E1
#cmd LNSerializer2SerializeHandler_Create $7EC
#cmd LNAssetModelSerializeHandler_Create $7F7
#cmd LNTextureSerializeHandler_Create $802
#cmd LNTexture2DSerializeHandler_Create $80D
#cmd LNShaderSerializeHandler_Create $818
#cmd LNRenderViewSerializeHandler_Create $823
#cmd LNMaterialSerializeHandler_Create $82E
#cmd LNStaticMeshModelSerializeHandler_Create $839
#cmd LNSkinnedMeshModelSerializeHandler_Create $844
#cmd LNAnimationControllerSerializeHandler_Create $84F
#cmd LNCollisionShapeSerializeHandler_Create $85A
#cmd LNBoxCollisionShapeSerializeHandler_Create $865
#cmd LNAnimationClipSerializeHandler_Create $870
#cmd LNAnimationStateSerializeHandler_Create $87B
#cmd LNComponentSerializeHandler_Create $886
#cmd LNVisualComponentSerializeHandler_Create $891
#cmd LNSpriteComponentSerializeHandler_Create $89C
#cmd LNCharacterControllerSerializeHandler_Create $8A7
#cmd LNWorldSerializeHandler_Create $8B2
#cmd LNComponentListSerializeHandler_Create $8BD
#cmd LNWorldObjectSerializeHandler_Create $8C8
#cmd LNWorldObjectPreUpdateHandler_Create $8D3
#cmd LNWorldObjectUpdateHandler_Create $8DE
#cmd LNVisualObjectSerializeHandler_Create $8E9
#cmd LNVisualObjectPreUpdateHandler_Create $8F4
#cmd LNVisualObjectUpdateHandler_Create $8FF
#cmd LNCameraSerializeHandler_Create $90A
#cmd LNCameraPreUpdateHandler_Create $915
#cmd LNCameraUpdateHandler_Create $920
#cmd LNEnvironmentLightSerializeHandler_Create $92B
#cmd LNEnvironmentLightPreUpdateHandler_Create $936
#cmd LNEnvironmentLightUpdateHandler_Create $941
#cmd LNDirectionalLightSerializeHandler_Create $94C
#cmd LNDirectionalLightPreUpdateHandler_Create $957
#cmd LNDirectionalLightUpdateHandler_Create $962
#cmd LNPointLightSerializeHandler_Create $96D
#cmd LNPointLightPreUpdateHandler_Create $978
#cmd LNPointLightUpdateHandler_Create $983
#cmd LNSpotLightSerializeHandler_Create $98E
#cmd LNSpotLightPreUpdateHandler_Create $999
#cmd LNSpotLightUpdateHandler_Create $9A4
#cmd LNSpriteSerializeHandler_Create $9AF
#cmd LNSpritePreUpdateHandler_Create $9BA
#cmd LNSpriteUpdateHandler_Create $9C5
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $9D0
#cmd LNRaycasterSerializeHandler_Create $9DB
#cmd LNRaycastResultSerializeHandler_Create $9E6
#cmd LNWorldRenderViewSerializeHandler_Create $9F1
#cmd LNBoxMeshSerializeHandler_Create $9FC
#cmd LNBoxMeshPreUpdateHandler_Create $A07
#cmd LNBoxMeshUpdateHandler_Create $A12
#cmd LNPlaneMeshSerializeHandler_Create $A1D
#cmd LNPlaneMeshPreUpdateHandler_Create $A28
#cmd LNPlaneMeshUpdateHandler_Create $A33
#cmd LNStaticMeshSerializeHandler_Create $A3E
#cmd LNStaticMeshPreUpdateHandler_Create $A49
#cmd LNStaticMeshUpdateHandler_Create $A54
#cmd LNStaticMeshComponentSerializeHandler_Create $A5F
#cmd LNSkinnedMeshComponentSerializeHandler_Create $A6A
#cmd LNCollisionSerializeHandler_Create $A75
#cmd LNTriggerBodyComponentSerializeHandler_Create $A80
#cmd LNLevelSerializeHandler_Create $A8B
#cmd LNLevelStartHandler_Create $A96
#cmd LNLevelStopHandler_Create $AA1
#cmd LNLevelPauseHandler_Create $AAC
#cmd LNLevelResumeHandler_Create $AB7
#cmd LNLevelUpdateHandler_Create $AC2
#cmd LNUIEventArgsSerializeHandler_Create $ACD
#cmd LNUILayoutElementSerializeHandler_Create $AD8
#cmd LNUIElementSerializeHandler_Create $AE3
#cmd LNUITextBlockSerializeHandler_Create $AEE
#cmd LNUISpriteSerializeHandler_Create $AF9
#cmd LNUIIconSerializeHandler_Create $B04
#cmd LNUILayoutPanelSerializeHandler_Create $B0F
#cmd LNUIBoxLayoutSerializeHandler_Create $B1A
#cmd LNUIGridLayoutSerializeHandler_Create $B25
#cmd LNUIControlSerializeHandler_Create $B30
#cmd LNUIButtonBaseSerializeHandler_Create $B3B
#cmd LNUIButtonSerializeHandler_Create $B46
#cmd LNUIWindowSerializeHandler_Create $B51
#cmd LNUIListItemSerializeHandler_Create $B5C
#cmd LNUIListItemsControlSerializeHandler_Create $B67
#cmd LNUIListBoxItemSerializeHandler_Create $B72
#cmd LNUIListBoxSerializeHandler_Create $B7D
#cmd LNInterpreterCommandSerializeHandler_Create $B88
#cmd LNInterpreterCommandListSerializeHandler_Create $B93
#cmd LNInterpreterSerializeHandler_Create $B9E
#cmd LNInterpreterUpdateWaitHandler_Create $BAA
#cmd LNApplicationSerializeHandler_Create $BB5
#cmd LNApplicationInitHandler_Create $BC0
#cmd LNApplicationUpdateHandler_Create $BCB



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
