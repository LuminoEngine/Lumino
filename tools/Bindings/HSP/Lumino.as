
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
#cmd LNVector3_GetX $404
#cmd LNVector3_SetX $406
#cmd LNVector3_GetY $407
#cmd LNVector3_SetY $409
#cmd LNVector3_GetZ $40A
#cmd LNVector3_SetZ $40C
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $423
#cmd LNVector4_SetX $425
#cmd LNVector4_GetY $426
#cmd LNVector4_SetY $428
#cmd LNVector4_GetZ $429
#cmd LNVector4_SetZ $42B
#cmd LNVector4_GetW $42C
#cmd LNVector4_SetW $42E
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $441
#cmd LNQuaternion_SetX $443
#cmd LNQuaternion_GetY $444
#cmd LNQuaternion_SetY $446
#cmd LNQuaternion_GetZ $447
#cmd LNQuaternion_SetZ $449
#cmd LNQuaternion_GetW $44A
#cmd LNQuaternion_SetW $44C
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $460
#cmd LNMatrix_SetRow0 $462
#cmd LNMatrix_GetRow1 $463
#cmd LNMatrix_SetRow1 $465
#cmd LNMatrix_GetRow2 $466
#cmd LNMatrix_SetRow2 $468
#cmd LNMatrix_GetRow3 $469
#cmd LNMatrix_SetRow3 $46B
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $F9
#cmd LNColor_GetR $4E9
#cmd LNColor_SetR $4EB
#cmd LNColor_GetG $4EC
#cmd LNColor_SetG $4EE
#cmd LNColor_GetB $4EF
#cmd LNColor_SetB $4F1
#cmd LNColor_GetA $4F2
#cmd LNColor_SetA $4F4
#cmd LNColor_SetZeros $FE
#cmd LNColor_Set $FF
#cmd LNPoint $104
#cmd LNPoint_GetX $508
#cmd LNPoint_SetX $50A
#cmd LNPoint_GetY $50B
#cmd LNPoint_SetY $50D
#cmd LNPoint_SetZeros $107
#cmd LNPoint_Set $108
#cmd LNSize $10B
#cmd LNSize_GetWidth $518
#cmd LNSize_SetWidth $51A
#cmd LNSize_GetHeight $51B
#cmd LNSize_SetHeight $51D
#cmd LNSize_SetZeros $10E
#cmd LNSize_Set $10F
#cmd LNRect $112
#cmd LNRect_GetX $528
#cmd LNRect_SetX $52A
#cmd LNRect_GetY $52B
#cmd LNRect_SetY $52D
#cmd LNRect_GetWidth $52E
#cmd LNRect_SetWidth $530
#cmd LNRect_GetHeight $531
#cmd LNRect_SetHeight $533
#cmd LNRect_SetZeros $117
#cmd LNRect_Set $118
#cmd LNRect_GetLeft $11D
#cmd LNRect_SetSize $11E
#cmd LNRect_GetSize $120
#cmd LNThickness $121
#cmd LNThickness_GetLeft $54C
#cmd LNThickness_SetLeft $54E
#cmd LNThickness_GetTop $54F
#cmd LNThickness_SetTop $551
#cmd LNThickness_GetRight $552
#cmd LNThickness_SetRight $554
#cmd LNThickness_GetBottom $555
#cmd LNThickness_SetBottom $557
#cmd LNThickness_SetZeros $126
#cmd LNThickness_Set $127
#cmd LNCornerRadius $12C
#cmd LNCornerRadius_GetTopleft $56A
#cmd LNCornerRadius_SetTopleft $56C
#cmd LNCornerRadius_GetTopright $56D
#cmd LNCornerRadius_SetTopright $56F
#cmd LNCornerRadius_GetBottomright $570
#cmd LNCornerRadius_SetBottomright $572
#cmd LNCornerRadius_GetBottomleft $573
#cmd LNCornerRadius_SetBottomleft $575
#cmd LNCornerRadius_SetZeros $131
#cmd LNCornerRadius_Set $132

#cmd LNObject_Release $74
#cmd LNObject_Retain $75
#cmd LNObject_GetReferenceCount $76
#cmd LNObject_SetPrototype_OnSerialize $793
#cmd LNEventConnection_SetPrototype_OnSerialize $79E
#cmd LNPromiseFailureDelegate_Create $486
#cmd LNVariant_Create $7B
#cmd LNVariant_SetInt $7C
#cmd LNVariant_GetInt $7E
#cmd LNVariant_SetPrototype_OnSerialize $7A9
#cmd LNZVTestDelegate1_Create $48F
#cmd LNZVTestDelegate2_Create $495
#cmd LNZVTestDelegate3_Create $49A
#cmd LNZVTestEventHandler1_Create $49F
#cmd LNZVTestEventHandler2_Create $4A4
#cmd LNZVTestPromise1_ThenWith $4A7
#cmd LNZVTestPromise1_CatchWith $4A9
#cmd LNZVTestPromise2_ThenWith $4AD
#cmd LNZVTestPromise2_CatchWith $4AF
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $7B4
#cmd LNZVTestEventArgs1_Create $AA
#cmd LNZVTestEventArgs1_CreateWithValue $AB
#cmd LNZVTestEventArgs1_GetValue $A9
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $7BF
#cmd LNSerializer2_SetPrototype_OnSerialize $7CA
#cmd LNAssetModel_Create $B7
#cmd LNAssetModel_Target $B6
#cmd LNAssetModel_SetPrototype_OnSerialize $7D5
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
#cmd LNTexture2DDelegate_Create $4E1
#cmd LNTexture2DPromise_ThenWith $4E4
#cmd LNTexture2DPromise_CatchWith $4E6
#cmd LNTexture_SetPrototype_OnSerialize $7E0
#cmd LNTexture2D_Create $13D
#cmd LNTexture2D_CreateWithFormat $140
#cmd LNTexture2D_Load $139
#cmd LNTexture2D_LoadEmoji $13B
#cmd LNTexture2D_SetPrototype_OnSerialize $7EB
#cmd LNRenderView_SetPrototype_OnSerialize $7F6
#cmd LNStaticMeshModel_Load $14C
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $801
#cmd LNSkinnedMeshModel_Load $14F
#cmd LNSkinnedMeshModel_GetAnimationController $151
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $80C
#cmd LNAnimationController_AddClip $153
#cmd LNAnimationController_Play $155
#cmd LNAnimationController_SetPrototype_OnSerialize $817
#cmd LNCollisionShape_SetPrototype_OnSerialize $822
#cmd LNBoxCollisionShape_Create $15A
#cmd LNBoxCollisionShape_CreateWHD $15C
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $82D
#cmd LNAnimationClip_Load $166
#cmd LNAnimationClip_SetHierarchicalAnimationMode $168
#cmd LNAnimationClip_HierarchicalAnimationMode $16A
#cmd LNAnimationClip_SetPrototype_OnSerialize $838
#cmd LNAnimationState_SetPrototype_OnSerialize $843
#cmd LNComponent_SetPrototype_OnSerialize $84E
#cmd LNVisualComponent_SetVisible $16E
#cmd LNVisualComponent_IsVisible $170
#cmd LNVisualComponent_SetPrototype_OnSerialize $859
#cmd LNSpriteComponent_SetTexture $172
#cmd LNSpriteComponent_SetPrototype_OnSerialize $864
#cmd LNCollisionEventHandler_Create $5A4
#cmd LNCharacterController_Create $18F
#cmd LNCharacterController_SetVelocity $17C
#cmd LNCharacterController_GetVelocity $17E
#cmd LNCharacterController_SetInputControlEnabled $17F
#cmd LNCharacterController_SetCameraControlEnabled $181
#cmd LNCharacterController_SetHeight $183
#cmd LNCharacterController_GetHeight $185
#cmd LNCharacterController_SetCameraRadius $186
#cmd LNCharacterController_GetCameraRadius $188
#cmd LNCharacterController_SetCollisionEnter $189
#cmd LNCharacterController_SetCollisionLeave $18B
#cmd LNCharacterController_SetCollisionStay $18D
#cmd LNCharacterController_SetPrototype_OnSerialize $86F
#cmd LNWorld_Add $191
#cmd LNWorld_SetPrototype_OnSerialize $87A
#cmd LNComponentList_GetLength $5BF
#cmd LNComponentList_GetItem $5C1
#cmd LNComponentList_SetPrototype_OnSerialize $885
#cmd LNWorldObject_Create $1C8
#cmd LNWorldObject_SetPosition $195
#cmd LNWorldObject_SetPositionXYZ $197
#cmd LNWorldObject_GetPosition $19B
#cmd LNWorldObject_SetRotationQuaternion $19C
#cmd LNWorldObject_SetRotation $19E
#cmd LNWorldObject_GetRotation $1A2
#cmd LNWorldObject_SetScale $1A3
#cmd LNWorldObject_SetScaleS $1A5
#cmd LNWorldObject_SetScaleXYZ $1A7
#cmd LNWorldObject_GetScale $1AB
#cmd LNWorldObject_SetCenterPoint $1AC
#cmd LNWorldObject_SetCenterPointXYZ $1AE
#cmd LNWorldObject_GetCenterPoint $1B2
#cmd LNWorldObject_LookAt $1B3
#cmd LNWorldObject_LookAtXYZ $1B5
#cmd LNWorldObject_AddComponent $1B9
#cmd LNWorldObject_RemoveComponent $1BB
#cmd LNWorldObject_AddTag $1BD
#cmd LNWorldObject_RemoveTag $1BF
#cmd LNWorldObject_HasTag $1C1
#cmd LNWorldObject_Destroy $1C3
#cmd LNWorldObject_GetComponents $1C4
#cmd LNWorldObject_SetPrototype_OnSerialize $890
#cmd LNWorldObject_SetPrototype_OnPreUpdate $89B
#cmd LNWorldObject_SetPrototype_OnUpdate $8A6
#cmd LNVisualObject_SetVisible $1CA
#cmd LNVisualObject_IsVisible $1CC
#cmd LNVisualObject_SetPrototype_OnSerialize $8B1
#cmd LNVisualObject_SetPrototype_OnPreUpdate $8BC
#cmd LNVisualObject_SetPrototype_OnUpdate $8C7
#cmd LNCamera_SetPrototype_OnSerialize $8D2
#cmd LNCamera_SetPrototype_OnPreUpdate $8DD
#cmd LNCamera_SetPrototype_OnUpdate $8E8
#cmd LNEnvironmentLight_SetEnabled $1CF
#cmd LNEnvironmentLight_IsEnabled $1D1
#cmd LNEnvironmentLight_SetColor $1D2
#cmd LNEnvironmentLight_GetColor $1D4
#cmd LNEnvironmentLight_SetAmbientColor $1D5
#cmd LNEnvironmentLight_GetAmbientColor $1D7
#cmd LNEnvironmentLight_GetSkyColor $1D8
#cmd LNEnvironmentLight_SetSkyColor $1D9
#cmd LNEnvironmentLight_GetGroundColor $1DB
#cmd LNEnvironmentLight_SetGroundColor $1DC
#cmd LNEnvironmentLight_SetIntensity $1DE
#cmd LNEnvironmentLight_GetIntensity $1E0
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $1E1
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $1E3
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $1E4
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $1E6
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $8F3
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $8FE
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $909
#cmd LNDirectionalLight_Create $1F7
#cmd LNDirectionalLight_CreateWithColor $1F8
#cmd LNDirectionalLight_SetEnabled $1E8
#cmd LNDirectionalLight_IsEnabled $1EA
#cmd LNDirectionalLight_SetColor $1EB
#cmd LNDirectionalLight_GetColor $1ED
#cmd LNDirectionalLight_SetIntensity $1EE
#cmd LNDirectionalLight_GetIntensity $1F0
#cmd LNDirectionalLight_SetShadowEffectiveDistance $1F1
#cmd LNDirectionalLight_GetShadowEffectiveDistance $1F3
#cmd LNDirectionalLight_SetShadowEffectiveDepth $1F4
#cmd LNDirectionalLight_GetShadowEffectiveDepth $1F6
#cmd LNDirectionalLight_SetPrototype_OnSerialize $914
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $91F
#cmd LNDirectionalLight_SetPrototype_OnUpdate $92A
#cmd LNPointLight_Create $20A
#cmd LNPointLight_CreateWithColorAndRange $20B
#cmd LNPointLight_SetEnabled $1FB
#cmd LNPointLight_IsEnabled $1FD
#cmd LNPointLight_SetColor $1FE
#cmd LNPointLight_GetColor $200
#cmd LNPointLight_SetIntensity $201
#cmd LNPointLight_GetIntensity $203
#cmd LNPointLight_SetRange $204
#cmd LNPointLight_GetRange $206
#cmd LNPointLight_SetAttenuation $207
#cmd LNPointLight_GetAttenuation $209
#cmd LNPointLight_SetPrototype_OnSerialize $935
#cmd LNPointLight_SetPrototype_OnPreUpdate $940
#cmd LNPointLight_SetPrototype_OnUpdate $94B
#cmd LNSpotLight_Create $224
#cmd LNSpotLight_CreateWithColorAndRange $225
#cmd LNSpotLight_SetEnabled $20F
#cmd LNSpotLight_IsEnabled $211
#cmd LNSpotLight_SetColor $212
#cmd LNSpotLight_GetColor $214
#cmd LNSpotLight_SetIntensity $215
#cmd LNSpotLight_GetIntensity $217
#cmd LNSpotLight_SetRange $218
#cmd LNSpotLight_GetRange $21A
#cmd LNSpotLight_SetAttenuation $21B
#cmd LNSpotLight_GetAttenuation $21D
#cmd LNSpotLight_SetAngle $21E
#cmd LNSpotLight_GetAngle $220
#cmd LNSpotLight_SetPenumbra $221
#cmd LNSpotLight_GetPenumbra $223
#cmd LNSpotLight_SetPrototype_OnSerialize $956
#cmd LNSpotLight_SetPrototype_OnPreUpdate $961
#cmd LNSpotLight_SetPrototype_OnUpdate $96C
#cmd LNTestDelegate_Create $666
#cmd LNSprite_Create $23B
#cmd LNSprite_CreateWithTexture $23C
#cmd LNSprite_CreateWithTextureAndSize $23E
#cmd LNSprite_SetTexture $22D
#cmd LNSprite_SetSize $22F
#cmd LNSprite_SetSizeWH $231
#cmd LNSprite_SetSourceRectXYWH $234
#cmd LNSprite_SetCallerTest $239
#cmd LNSprite_SetPrototype_OnSerialize $977
#cmd LNSprite_SetPrototype_OnPreUpdate $982
#cmd LNSprite_SetPrototype_OnUpdate $98D
#cmd LNCameraOrbitControlComponent_Create $243
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $998
#cmd LNRaycaster_FromScreen $245
#cmd LNRaycaster_IntersectPlane $247
#cmd LNRaycaster_SetPrototype_OnSerialize $9A3
#cmd LNRaycastResult_GetPoint $24C
#cmd LNRaycastResult_SetPrototype_OnSerialize $9AE
#cmd LNWorldRenderView_SetGuideGridEnabled $24E
#cmd LNWorldRenderView_GetGuideGridEnabled $250
#cmd LNWorldRenderView_SetPrototype_OnSerialize $9B9
#cmd LNBoxMesh_Create $252
#cmd LNBoxMesh_SetPrototype_OnSerialize $9C4
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $9CF
#cmd LNBoxMesh_SetPrototype_OnUpdate $9DA
#cmd LNPlaneMesh_Create $254
#cmd LNPlaneMesh_SetPrototype_OnSerialize $9E5
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $9F0
#cmd LNPlaneMesh_SetPrototype_OnUpdate $9FB
#cmd LNStaticMesh_Load $256
#cmd LNStaticMesh_MakeCollisionBody $258
#cmd LNStaticMesh_SetPrototype_OnSerialize $A06
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $A11
#cmd LNStaticMesh_SetPrototype_OnUpdate $A1C
#cmd LNStaticMeshComponent_Create $25F
#cmd LNStaticMeshComponent_SetModel $25B
#cmd LNStaticMeshComponent_MakeCollisionBody $25D
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $A27
#cmd LNSkinnedMeshComponent_Create $263
#cmd LNSkinnedMeshComponent_SetModel $261
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $A32
#cmd LNCollision_GetWorldObject $265
#cmd LNCollision_SetPrototype_OnSerialize $A3D
#cmd LNTriggerBodyComponent_Create $269
#cmd LNTriggerBodyComponent_AddCollisionShape $267
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $A48
#cmd LNScene_SetClearMode $26B
#cmd LNScene_SetSkyColor $26D
#cmd LNScene_SetSkyHorizonColor $26F
#cmd LNScene_SetSkyCloudColor $271
#cmd LNScene_SetSkyOverlayColor $273
#cmd LNScene_GotoLevel $275
#cmd LNScene_CallLevel $278
#cmd LNScene_ReturnLevel $27B
#cmd LNScene_ActiveLevel $27D
#cmd LNScene_IsTransitionEffectRunning $27E
#cmd LNScene_SetTransitionEffectMode $27F
#cmd LNScene_TransitionEffectMode $281
#cmd LNScene_SetTransitionDuration $282
#cmd LNScene_TransitionDuration $284
#cmd LNScene_SetTransitionEffectColor $285
#cmd LNScene_TransitionEffectColor $287
#cmd LNScene_SetTransitionEffectMaskTexture $288
#cmd LNScene_TransitionEffectMaskTexture $28A
#cmd LNScene_SetTransitionEffectVague $28B
#cmd LNScene_TransitionEffectVague $28D
#cmd LNScene_StartFadeOut $28E
#cmd LNScene_StartFadeIn $28F
#cmd LNScene_SetScreenBlendColor $290
#cmd LNScene_ScreenBlendColor $292
#cmd LNScene_SetAntialiasEnabled $293
#cmd LNScene_IsAntialiasEnabled $295
#cmd LNScene_SetSSREnabled $296
#cmd LNScene_IsSSREnabled $298
#cmd LNScene_SetSSAOEnabled $299
#cmd LNScene_IsSSAOEnabled $29B
#cmd LNScene_SetBloomEnabled $29C
#cmd LNScene_IsBloomEnabled $29E
#cmd LNScene_SetDOFEnabled $29F
#cmd LNScene_IsDOFEnabled $2A1
#cmd LNScene_SetTonemapEnabled $2A2
#cmd LNScene_IsTonemapEnabled $2A4
#cmd LNScene_SetVignetteEnabled $2A5
#cmd LNScene_IsVignetteEnabled $2A7
#cmd LNScene_SetGammaEnabled $2A8
#cmd LNScene_IsGammaEnabled $2AA
#cmd LNLevel_Create $2BA
#cmd LNLevel_AddObject $2AC
#cmd LNLevel_RemoveObject $2AE
#cmd LNLevel_AddSubLevel $2B0
#cmd LNLevel_RemoveSubLevel $2B2
#cmd LNLevel_RemoveAllSubLevels $2B4
#cmd LNLevel_SetPrototype_OnSerialize $A53
#cmd LNLevel_SetPrototype_OnStart $A5E
#cmd LNLevel_SetPrototype_OnStop $A69
#cmd LNLevel_SetPrototype_OnPause $A74
#cmd LNLevel_SetPrototype_OnResume $A7F
#cmd LNLevel_SetPrototype_OnUpdate $A8A
#cmd LNUIEventArgs_Sender $2C5
#cmd LNUIEventArgs_SetPrototype_OnSerialize $A95
#cmd LNUIGeneralEventHandler_Create $6B7
#cmd LNUIEventHandler_Create $6BC
#cmd LNUILayoutElement_SetPrototype_OnSerialize $AA0
#cmd LNUIElement_SetSize $2D7
#cmd LNUIElement_SetSizeWH $2D9
#cmd LNUIElement_SetMargin $2DC
#cmd LNUIElement_GetMargin $2DE
#cmd LNUIElement_SetPadding $2DF
#cmd LNUIElement_GetPadding $2E1
#cmd LNUIElement_SetHAlignment $2E2
#cmd LNUIElement_GetHAlignment $2E4
#cmd LNUIElement_SetVAlignment $2E5
#cmd LNUIElement_GetVAlignment $2E7
#cmd LNUIElement_SetAlignments $2E8
#cmd LNUIElement_SetPosition $2EB
#cmd LNUIElement_SetPositionXYZ $2ED
#cmd LNUIElement_GetPosition $2F1
#cmd LNUIElement_SetRotation $2F2
#cmd LNUIElement_SetEulerAngles $2F4
#cmd LNUIElement_GetRotation $2F8
#cmd LNUIElement_SetScale $2F9
#cmd LNUIElement_SetScaleS $2FB
#cmd LNUIElement_SetScaleXY $2FD
#cmd LNUIElement_GetScale $300
#cmd LNUIElement_SetCenterPoint $301
#cmd LNUIElement_SetCenterPointXYZ $303
#cmd LNUIElement_GetCenterPoint $307
#cmd LNUIElement_SetEnabled $308
#cmd LNUIElement_IsEnabled $30A
#cmd LNUIElement_SetData $30B
#cmd LNUIElement_GetData $30D
#cmd LNUIElement_SetBackgroundColor $30E
#cmd LNUIElement_GetBackgroundColor $310
#cmd LNUIElement_SetBorderThickness $311
#cmd LNUIElement_GetBorderThickness $313
#cmd LNUIElement_SetBorderColor $314
#cmd LNUIElement_GetBorderColor $316
#cmd LNUIElement_SetCornerRadius $317
#cmd LNUIElement_GetCornerRadius $319
#cmd LNUIElement_SetVisibility $31A
#cmd LNUIElement_GetVisibility $31C
#cmd LNUIElement_SetOpacity $31D
#cmd LNUIElement_GetOpacity $31F
#cmd LNUIElement_AddChild $320
#cmd LNUIElement_SetPrototype_OnSerialize $AAB
#cmd LNUITextBlock_Create $326
#cmd LNUITextBlock_CreateWithText $327
#cmd LNUITextBlock_SetText $323
#cmd LNUITextBlock_GetText $325
#cmd LNUITextBlock_SetPrototype_OnSerialize $AB6
#cmd LNUISprite_Create $334
#cmd LNUISprite_CreateWithTexture $335
#cmd LNUISprite_SetTexture $32A
#cmd LNUISprite_SetSourceRect $32C
#cmd LNUISprite_SetSourceRectXYWH $32E
#cmd LNUISprite_GetSourceRect $333
#cmd LNUISprite_SetPrototype_OnSerialize $AC1
#cmd LNUIIcon_LoadFontIcon $338
#cmd LNUIIcon_LoadFontIconWithNameSize $33A
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $33D
#cmd LNUIIcon_SetPrototype_OnSerialize $ACC
#cmd LNUI_Add $342
#cmd LNUI_Remove $344
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $AD7
#cmd LNUIBoxLayout_Create $34B
#cmd LNUIBoxLayout_SetOrientation $348
#cmd LNUIBoxLayout_GetOrientation $34A
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $AE2
#cmd LNUIGridLayout_Create $35B
#cmd LNUIGridLayout_SetColumnCount $34D
#cmd LNUIGridLayout_SetRow $34F
#cmd LNUIGridLayout_SetColumn $352
#cmd LNUIGridLayout_SetPlacement $355
#cmd LNUIGridLayout_SetPrototype_OnSerialize $AED
#cmd LNUIControl_Create $367
#cmd LNUIControl_AddInlineVisual $368
#cmd LNUIControl_SetPrototype_OnSerialize $AF8
#cmd LNUIButtonBase_SetText $36C
#cmd LNUIButtonBase_SetPrototype_OnSerialize $B03
#cmd LNUIButton_Create $36F
#cmd LNUIButton_CreateWithText $370
#cmd LNUIButton_ConnectOnClicked $372
#cmd LNUIButton_SetPrototype_OnSerialize $B0E
#cmd LNUIWindow_Create $375
#cmd LNUIWindow_SetPrototype_OnSerialize $B19
#cmd LNUIListItem_ConnectOnSubmit $37A
#cmd LNUIListItem_SetPrototype_OnSerialize $B24
#cmd LNUIListItemsControl_SetItemsLayoutPanel $37D
#cmd LNUIListItemsControl_SetSubmitMode $37F
#cmd LNUIListItemsControl_GetSubmitMode $381
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $B2F
#cmd LNUIListBoxItem_Create $383
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $B3A
#cmd LNUIListBox_Create $388
#cmd LNUIListBox_AddItem $386
#cmd LNUIListBox_SetPrototype_OnSerialize $B45
#cmd LNInput_IsPressed $38A
#cmd LNInput_IsTriggered $38C
#cmd LNInput_IsTriggeredOff $38E
#cmd LNInput_IsRepeated $390
#cmd LNInput_GetAxisValue $392
#cmd LNInput_ClearAllBindings $394
#cmd LNMouse_Pressed $396
#cmd LNMouse_Triggered $398
#cmd LNMouse_TriggeredOff $39A
#cmd LNMouse_Repeated $39C
#cmd LNMouse_Position $39E
#cmd LNInterpreterCommand_Code $3A0
#cmd LNInterpreterCommand_ParamsCount $3A1
#cmd LNInterpreterCommand_Param $3A2
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $B50
#cmd LNInterpreterCommandList_Create $3B9
#cmd LNInterpreterCommandList_AddCommand $3A5
#cmd LNInterpreterCommandList_AddCommand1 $3A7
#cmd LNInterpreterCommandList_AddCommand2 $3AA
#cmd LNInterpreterCommandList_AddCommand3 $3AE
#cmd LNInterpreterCommandList_AddCommand4 $3B3
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $B5B
#cmd LNInterpreterCommandDelegate_Create $765
#cmd LNInterpreter_Create $3CD
#cmd LNInterpreter_Clear $3BE
#cmd LNInterpreter_Run $3BF
#cmd LNInterpreter_IsRunning $3C1
#cmd LNInterpreter_Update $3C2
#cmd LNInterpreter_Terminate $3C3
#cmd LNInterpreter_RegisterCommandHandler $3C4
#cmd LNInterpreter_SetWaitMode $3C7
#cmd LNInterpreter_GetWaitMode $3C9
#cmd LNInterpreter_SetWaitCount $3CA
#cmd LNInterpreter_GetWaitCount $3CC
#cmd LNInterpreter_SetPrototype_OnSerialize $B66
#cmd LNInterpreter_SetPrototype_OnUpdateWait $B72
#cmd LNEngineSettings_SetMainWindowSize $3D0
#cmd LNEngineSettings_SetMainWorldViewSize $3D3
#cmd LNEngineSettings_SetMainWindowTitle $3D6
#cmd LNEngineSettings_AddAssetDirectory $3D8
#cmd LNEngineSettings_AddAssetArchive $3DA
#cmd LNEngineSettings_SetFrameRate $3DD
#cmd LNEngineSettings_SetDefaultUITheme $3DF
#cmd LNEngineSettings_SetDebugToolEnabled $3E1
#cmd LNEngineSettings_SetEngineLogEnabled $3E3
#cmd LNEngineSettings_SetEngineLogFilePath $3E5
#cmd LNEngine_Initialize $3E8
#cmd LNEngine_Finalize $3E9
#cmd LNEngine_Update $3EA
#cmd LNEngine_Run $3EB
#cmd LNEngine_Time $3ED
#cmd LNEngine_GetCamera $3EE
#cmd LNEngine_GetMainLight $3EF
#cmd LNEngine_GetRenderView $3F0
#cmd LNApplication_Create $3F5
#cmd LNApplication_OnInit $3F2
#cmd LNApplication_OnUpdate $3F3
#cmd LNApplication_World $3F4
#cmd LNApplication_SetPrototype_OnSerialize $B7D
#cmd LNApplication_SetPrototype_OnInit $B88
#cmd LNApplication_SetPrototype_OnUpdate $B93
#cmd LNDebug_SetGuideGridEnabled $3F7
#cmd LNDebug_SetPhysicsDebugDrawEnabled $3F9
#cmd LNDebug_Print $3FB
#cmd LNDebug_PrintWithTime $3FD
#cmd LNDebug_PrintWithTimeAndColor $400
#cmd LNObjectSerializeHandler_Create $791
#cmd LNEventConnectionSerializeHandler_Create $79C
#cmd LNVariantSerializeHandler_Create $7A7
#cmd LNZVTestClass1SerializeHandler_Create $7B2
#cmd LNZVTestEventArgs1SerializeHandler_Create $7BD
#cmd LNSerializer2SerializeHandler_Create $7C8
#cmd LNAssetModelSerializeHandler_Create $7D3
#cmd LNTextureSerializeHandler_Create $7DE
#cmd LNTexture2DSerializeHandler_Create $7E9
#cmd LNRenderViewSerializeHandler_Create $7F4
#cmd LNStaticMeshModelSerializeHandler_Create $7FF
#cmd LNSkinnedMeshModelSerializeHandler_Create $80A
#cmd LNAnimationControllerSerializeHandler_Create $815
#cmd LNCollisionShapeSerializeHandler_Create $820
#cmd LNBoxCollisionShapeSerializeHandler_Create $82B
#cmd LNAnimationClipSerializeHandler_Create $836
#cmd LNAnimationStateSerializeHandler_Create $841
#cmd LNComponentSerializeHandler_Create $84C
#cmd LNVisualComponentSerializeHandler_Create $857
#cmd LNSpriteComponentSerializeHandler_Create $862
#cmd LNCharacterControllerSerializeHandler_Create $86D
#cmd LNWorldSerializeHandler_Create $878
#cmd LNComponentListSerializeHandler_Create $883
#cmd LNWorldObjectSerializeHandler_Create $88E
#cmd LNWorldObjectPreUpdateHandler_Create $899
#cmd LNWorldObjectUpdateHandler_Create $8A4
#cmd LNVisualObjectSerializeHandler_Create $8AF
#cmd LNVisualObjectPreUpdateHandler_Create $8BA
#cmd LNVisualObjectUpdateHandler_Create $8C5
#cmd LNCameraSerializeHandler_Create $8D0
#cmd LNCameraPreUpdateHandler_Create $8DB
#cmd LNCameraUpdateHandler_Create $8E6
#cmd LNEnvironmentLightSerializeHandler_Create $8F1
#cmd LNEnvironmentLightPreUpdateHandler_Create $8FC
#cmd LNEnvironmentLightUpdateHandler_Create $907
#cmd LNDirectionalLightSerializeHandler_Create $912
#cmd LNDirectionalLightPreUpdateHandler_Create $91D
#cmd LNDirectionalLightUpdateHandler_Create $928
#cmd LNPointLightSerializeHandler_Create $933
#cmd LNPointLightPreUpdateHandler_Create $93E
#cmd LNPointLightUpdateHandler_Create $949
#cmd LNSpotLightSerializeHandler_Create $954
#cmd LNSpotLightPreUpdateHandler_Create $95F
#cmd LNSpotLightUpdateHandler_Create $96A
#cmd LNSpriteSerializeHandler_Create $975
#cmd LNSpritePreUpdateHandler_Create $980
#cmd LNSpriteUpdateHandler_Create $98B
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $996
#cmd LNRaycasterSerializeHandler_Create $9A1
#cmd LNRaycastResultSerializeHandler_Create $9AC
#cmd LNWorldRenderViewSerializeHandler_Create $9B7
#cmd LNBoxMeshSerializeHandler_Create $9C2
#cmd LNBoxMeshPreUpdateHandler_Create $9CD
#cmd LNBoxMeshUpdateHandler_Create $9D8
#cmd LNPlaneMeshSerializeHandler_Create $9E3
#cmd LNPlaneMeshPreUpdateHandler_Create $9EE
#cmd LNPlaneMeshUpdateHandler_Create $9F9
#cmd LNStaticMeshSerializeHandler_Create $A04
#cmd LNStaticMeshPreUpdateHandler_Create $A0F
#cmd LNStaticMeshUpdateHandler_Create $A1A
#cmd LNStaticMeshComponentSerializeHandler_Create $A25
#cmd LNSkinnedMeshComponentSerializeHandler_Create $A30
#cmd LNCollisionSerializeHandler_Create $A3B
#cmd LNTriggerBodyComponentSerializeHandler_Create $A46
#cmd LNLevelSerializeHandler_Create $A51
#cmd LNLevelStartHandler_Create $A5C
#cmd LNLevelStopHandler_Create $A67
#cmd LNLevelPauseHandler_Create $A72
#cmd LNLevelResumeHandler_Create $A7D
#cmd LNLevelUpdateHandler_Create $A88
#cmd LNUIEventArgsSerializeHandler_Create $A93
#cmd LNUILayoutElementSerializeHandler_Create $A9E
#cmd LNUIElementSerializeHandler_Create $AA9
#cmd LNUITextBlockSerializeHandler_Create $AB4
#cmd LNUISpriteSerializeHandler_Create $ABF
#cmd LNUIIconSerializeHandler_Create $ACA
#cmd LNUILayoutPanelSerializeHandler_Create $AD5
#cmd LNUIBoxLayoutSerializeHandler_Create $AE0
#cmd LNUIGridLayoutSerializeHandler_Create $AEB
#cmd LNUIControlSerializeHandler_Create $AF6
#cmd LNUIButtonBaseSerializeHandler_Create $B01
#cmd LNUIButtonSerializeHandler_Create $B0C
#cmd LNUIWindowSerializeHandler_Create $B17
#cmd LNUIListItemSerializeHandler_Create $B22
#cmd LNUIListItemsControlSerializeHandler_Create $B2D
#cmd LNUIListBoxItemSerializeHandler_Create $B38
#cmd LNUIListBoxSerializeHandler_Create $B43
#cmd LNInterpreterCommandSerializeHandler_Create $B4E
#cmd LNInterpreterCommandListSerializeHandler_Create $B59
#cmd LNInterpreterSerializeHandler_Create $B64
#cmd LNInterpreterUpdateWaitHandler_Create $B70
#cmd LNApplicationSerializeHandler_Create $B7B
#cmd LNApplicationInitHandler_Create $B86
#cmd LNApplicationUpdateHandler_Create $B91



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
