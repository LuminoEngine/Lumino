
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
#cmd LNVector3_GetX $3F9
#cmd LNVector3_SetX $3FB
#cmd LNVector3_GetY $3FC
#cmd LNVector3_SetY $3FE
#cmd LNVector3_GetZ $3FF
#cmd LNVector3_SetZ $401
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $418
#cmd LNVector4_SetX $41A
#cmd LNVector4_GetY $41B
#cmd LNVector4_SetY $41D
#cmd LNVector4_GetZ $41E
#cmd LNVector4_SetZ $420
#cmd LNVector4_GetW $421
#cmd LNVector4_SetW $423
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $436
#cmd LNQuaternion_SetX $438
#cmd LNQuaternion_GetY $439
#cmd LNQuaternion_SetY $43B
#cmd LNQuaternion_GetZ $43C
#cmd LNQuaternion_SetZ $43E
#cmd LNQuaternion_GetW $43F
#cmd LNQuaternion_SetW $441
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $455
#cmd LNMatrix_SetRow0 $457
#cmd LNMatrix_GetRow1 $458
#cmd LNMatrix_SetRow1 $45A
#cmd LNMatrix_GetRow2 $45B
#cmd LNMatrix_SetRow2 $45D
#cmd LNMatrix_GetRow3 $45E
#cmd LNMatrix_SetRow3 $460
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $F9
#cmd LNColor_GetR $4DE
#cmd LNColor_SetR $4E0
#cmd LNColor_GetG $4E1
#cmd LNColor_SetG $4E3
#cmd LNColor_GetB $4E4
#cmd LNColor_SetB $4E6
#cmd LNColor_GetA $4E7
#cmd LNColor_SetA $4E9
#cmd LNColor_SetZeros $FE
#cmd LNColor_Set $FF
#cmd LNPoint $104
#cmd LNPoint_GetX $4FD
#cmd LNPoint_SetX $4FF
#cmd LNPoint_GetY $500
#cmd LNPoint_SetY $502
#cmd LNPoint_SetZeros $107
#cmd LNPoint_Set $108
#cmd LNSize $10B
#cmd LNSize_GetWidth $50D
#cmd LNSize_SetWidth $50F
#cmd LNSize_GetHeight $510
#cmd LNSize_SetHeight $512
#cmd LNSize_SetZeros $10E
#cmd LNSize_Set $10F
#cmd LNRect $112
#cmd LNRect_GetX $51D
#cmd LNRect_SetX $51F
#cmd LNRect_GetY $520
#cmd LNRect_SetY $522
#cmd LNRect_GetWidth $523
#cmd LNRect_SetWidth $525
#cmd LNRect_GetHeight $526
#cmd LNRect_SetHeight $528
#cmd LNRect_SetZeros $117
#cmd LNRect_Set $118
#cmd LNRect_GetLeft $11D
#cmd LNRect_SetSize $11E
#cmd LNRect_GetSize $120
#cmd LNThickness $121
#cmd LNThickness_GetLeft $541
#cmd LNThickness_SetLeft $543
#cmd LNThickness_GetTop $544
#cmd LNThickness_SetTop $546
#cmd LNThickness_GetRight $547
#cmd LNThickness_SetRight $549
#cmd LNThickness_GetBottom $54A
#cmd LNThickness_SetBottom $54C
#cmd LNThickness_SetZeros $126
#cmd LNThickness_Set $127
#cmd LNCornerRadius $12C
#cmd LNCornerRadius_GetTopleft $55F
#cmd LNCornerRadius_SetTopleft $561
#cmd LNCornerRadius_GetTopright $562
#cmd LNCornerRadius_SetTopright $564
#cmd LNCornerRadius_GetBottomright $565
#cmd LNCornerRadius_SetBottomright $567
#cmd LNCornerRadius_GetBottomleft $568
#cmd LNCornerRadius_SetBottomleft $56A
#cmd LNCornerRadius_SetZeros $131
#cmd LNCornerRadius_Set $132

#cmd LNObject_Release $74
#cmd LNObject_Retain $75
#cmd LNObject_GetReferenceCount $76
#cmd LNObject_SetPrototype_OnSerialize $77D
#cmd LNEventConnection_SetPrototype_OnSerialize $788
#cmd LNPromiseFailureDelegate_Create $47B
#cmd LNVariant_Create $7B
#cmd LNVariant_SetInt $7C
#cmd LNVariant_GetInt $7E
#cmd LNVariant_SetPrototype_OnSerialize $793
#cmd LNZVTestDelegate1_Create $484
#cmd LNZVTestDelegate2_Create $48A
#cmd LNZVTestDelegate3_Create $48F
#cmd LNZVTestEventHandler1_Create $494
#cmd LNZVTestEventHandler2_Create $499
#cmd LNZVTestPromise1_ThenWith $49C
#cmd LNZVTestPromise1_CatchWith $49E
#cmd LNZVTestPromise2_ThenWith $4A2
#cmd LNZVTestPromise2_CatchWith $4A4
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $79E
#cmd LNZVTestEventArgs1_Create $AA
#cmd LNZVTestEventArgs1_CreateWithValue $AB
#cmd LNZVTestEventArgs1_GetValue $A9
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $7A9
#cmd LNSerializer2_SetPrototype_OnSerialize $7B4
#cmd LNAssetModel_Create $B7
#cmd LNAssetModel_Target $B6
#cmd LNAssetModel_SetPrototype_OnSerialize $7BF
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
#cmd LNTexture2DDelegate_Create $4D6
#cmd LNTexture2DPromise_ThenWith $4D9
#cmd LNTexture2DPromise_CatchWith $4DB
#cmd LNTexture_SetPrototype_OnSerialize $7CA
#cmd LNTexture2D_Create $13D
#cmd LNTexture2D_CreateWithFormat $140
#cmd LNTexture2D_Load $139
#cmd LNTexture2D_LoadEmoji $13B
#cmd LNTexture2D_SetPrototype_OnSerialize $7D5
#cmd LNRenderView_SetPrototype_OnSerialize $7E0
#cmd LNStaticMeshModel_Load $14C
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $7EB
#cmd LNSkinnedMeshModel_Load $14F
#cmd LNSkinnedMeshModel_GetAnimationController $151
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $7F6
#cmd LNAnimationController_AddClip $153
#cmd LNAnimationController_Play $155
#cmd LNAnimationController_SetPrototype_OnSerialize $801
#cmd LNCollisionShape_SetPrototype_OnSerialize $80C
#cmd LNBoxCollisionShape_Create $15A
#cmd LNBoxCollisionShape_CreateWHD $15C
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $817
#cmd LNAnimationClip_Load $166
#cmd LNAnimationClip_SetHierarchicalAnimationMode $168
#cmd LNAnimationClip_HierarchicalAnimationMode $16A
#cmd LNAnimationClip_SetPrototype_OnSerialize $822
#cmd LNAnimationState_SetPrototype_OnSerialize $82D
#cmd LNComponent_SetPrototype_OnSerialize $838
#cmd LNVisualComponent_SetVisible $16E
#cmd LNVisualComponent_IsVisible $170
#cmd LNVisualComponent_SetPrototype_OnSerialize $843
#cmd LNSpriteComponent_SetTexture $172
#cmd LNSpriteComponent_SetPrototype_OnSerialize $84E
#cmd LNCollisionEventHandler_Create $599
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
#cmd LNCharacterController_SetPrototype_OnSerialize $859
#cmd LNWorld_Add $191
#cmd LNWorld_SetPrototype_OnSerialize $864
#cmd LNComponentList_GetLength $5B4
#cmd LNComponentList_GetItem $5B6
#cmd LNComponentList_SetPrototype_OnSerialize $86F
#cmd LNWorldObject_Create $1C7
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
#cmd LNWorldObject_SetPrototype_OnSerialize $87A
#cmd LNWorldObject_SetPrototype_OnUpdate $885
#cmd LNVisualObject_SetVisible $1C9
#cmd LNVisualObject_IsVisible $1CB
#cmd LNVisualObject_SetPrototype_OnSerialize $890
#cmd LNVisualObject_SetPrototype_OnUpdate $89B
#cmd LNCamera_SetPrototype_OnSerialize $8A6
#cmd LNCamera_SetPrototype_OnUpdate $8B1
#cmd LNEnvironmentLight_SetEnabled $1CE
#cmd LNEnvironmentLight_IsEnabled $1D0
#cmd LNEnvironmentLight_SetColor $1D1
#cmd LNEnvironmentLight_GetColor $1D3
#cmd LNEnvironmentLight_SetAmbientColor $1D4
#cmd LNEnvironmentLight_GetAmbientColor $1D6
#cmd LNEnvironmentLight_GetSkyColor $1D7
#cmd LNEnvironmentLight_SetSkyColor $1D8
#cmd LNEnvironmentLight_GetGroundColor $1DA
#cmd LNEnvironmentLight_SetGroundColor $1DB
#cmd LNEnvironmentLight_SetIntensity $1DD
#cmd LNEnvironmentLight_GetIntensity $1DF
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $1E0
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $1E2
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $1E3
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $1E5
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $8BC
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $8C7
#cmd LNDirectionalLight_Create $1F6
#cmd LNDirectionalLight_CreateWithColor $1F7
#cmd LNDirectionalLight_SetEnabled $1E7
#cmd LNDirectionalLight_IsEnabled $1E9
#cmd LNDirectionalLight_SetColor $1EA
#cmd LNDirectionalLight_GetColor $1EC
#cmd LNDirectionalLight_SetIntensity $1ED
#cmd LNDirectionalLight_GetIntensity $1EF
#cmd LNDirectionalLight_SetShadowEffectiveDistance $1F0
#cmd LNDirectionalLight_GetShadowEffectiveDistance $1F2
#cmd LNDirectionalLight_SetShadowEffectiveDepth $1F3
#cmd LNDirectionalLight_GetShadowEffectiveDepth $1F5
#cmd LNDirectionalLight_SetPrototype_OnSerialize $8D2
#cmd LNDirectionalLight_SetPrototype_OnUpdate $8DD
#cmd LNPointLight_Create $209
#cmd LNPointLight_CreateWithColorAndRange $20A
#cmd LNPointLight_SetEnabled $1FA
#cmd LNPointLight_IsEnabled $1FC
#cmd LNPointLight_SetColor $1FD
#cmd LNPointLight_GetColor $1FF
#cmd LNPointLight_SetIntensity $200
#cmd LNPointLight_GetIntensity $202
#cmd LNPointLight_SetRange $203
#cmd LNPointLight_GetRange $205
#cmd LNPointLight_SetAttenuation $206
#cmd LNPointLight_GetAttenuation $208
#cmd LNPointLight_SetPrototype_OnSerialize $8E8
#cmd LNPointLight_SetPrototype_OnUpdate $8F3
#cmd LNSpotLight_Create $223
#cmd LNSpotLight_CreateWithColorAndRange $224
#cmd LNSpotLight_SetEnabled $20E
#cmd LNSpotLight_IsEnabled $210
#cmd LNSpotLight_SetColor $211
#cmd LNSpotLight_GetColor $213
#cmd LNSpotLight_SetIntensity $214
#cmd LNSpotLight_GetIntensity $216
#cmd LNSpotLight_SetRange $217
#cmd LNSpotLight_GetRange $219
#cmd LNSpotLight_SetAttenuation $21A
#cmd LNSpotLight_GetAttenuation $21C
#cmd LNSpotLight_SetAngle $21D
#cmd LNSpotLight_GetAngle $21F
#cmd LNSpotLight_SetPenumbra $220
#cmd LNSpotLight_GetPenumbra $222
#cmd LNSpotLight_SetPrototype_OnSerialize $8FE
#cmd LNSpotLight_SetPrototype_OnUpdate $909
#cmd LNTestDelegate_Create $65A
#cmd LNSprite_Create $23A
#cmd LNSprite_CreateWithTexture $23B
#cmd LNSprite_CreateWithTextureAndSize $23D
#cmd LNSprite_SetTexture $22C
#cmd LNSprite_SetSize $22E
#cmd LNSprite_SetSizeWH $230
#cmd LNSprite_SetSourceRectXYWH $233
#cmd LNSprite_SetCallerTest $238
#cmd LNSprite_SetPrototype_OnSerialize $914
#cmd LNSprite_SetPrototype_OnUpdate $91F
#cmd LNCameraOrbitControlComponent_Create $242
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $92A
#cmd LNRaycaster_FromScreen $244
#cmd LNRaycaster_IntersectPlane $246
#cmd LNRaycaster_SetPrototype_OnSerialize $935
#cmd LNRaycastResult_GetPoint $24B
#cmd LNRaycastResult_SetPrototype_OnSerialize $940
#cmd LNWorldRenderView_SetGuideGridEnabled $24D
#cmd LNWorldRenderView_GetGuideGridEnabled $24F
#cmd LNWorldRenderView_SetPrototype_OnSerialize $94B
#cmd LNBoxMesh_Create $251
#cmd LNBoxMesh_SetPrototype_OnSerialize $956
#cmd LNBoxMesh_SetPrototype_OnUpdate $961
#cmd LNPlaneMesh_Create $253
#cmd LNPlaneMesh_SetPrototype_OnSerialize $96C
#cmd LNPlaneMesh_SetPrototype_OnUpdate $977
#cmd LNStaticMesh_Load $255
#cmd LNStaticMesh_MakeCollisionBody $257
#cmd LNStaticMesh_SetPrototype_OnSerialize $982
#cmd LNStaticMesh_SetPrototype_OnUpdate $98D
#cmd LNStaticMeshComponent_Create $25E
#cmd LNStaticMeshComponent_SetModel $25A
#cmd LNStaticMeshComponent_MakeCollisionBody $25C
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $998
#cmd LNSkinnedMeshComponent_Create $262
#cmd LNSkinnedMeshComponent_SetModel $260
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $9A3
#cmd LNCollision_GetWorldObject $264
#cmd LNCollision_SetPrototype_OnSerialize $9AE
#cmd LNTriggerBodyComponent_Create $268
#cmd LNTriggerBodyComponent_AddCollisionShape $266
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $9B9
#cmd LNScene_SetClearMode $26A
#cmd LNScene_SetSkyColor $26C
#cmd LNScene_SetSkyHorizonColor $26E
#cmd LNScene_SetSkyCloudColor $270
#cmd LNScene_SetSkyOverlayColor $272
#cmd LNScene_GotoLevel $274
#cmd LNScene_CallLevel $277
#cmd LNScene_ReturnLevel $27A
#cmd LNScene_ActiveLevel $27C
#cmd LNScene_IsTransitionEffectRunning $27D
#cmd LNScene_SetTransitionEffectMode $27E
#cmd LNScene_TransitionEffectMode $280
#cmd LNScene_SetTransitionDuration $281
#cmd LNScene_TransitionDuration $283
#cmd LNScene_SetTransitionEffectColor $284
#cmd LNScene_TransitionEffectColor $286
#cmd LNScene_SetTransitionEffectMaskTexture $287
#cmd LNScene_TransitionEffectMaskTexture $289
#cmd LNScene_SetTransitionEffectVague $28A
#cmd LNScene_TransitionEffectVague $28C
#cmd LNScene_StartFadeOut $28D
#cmd LNScene_StartFadeIn $28E
#cmd LNScene_SetScreenBlendColor $28F
#cmd LNScene_ScreenBlendColor $291
#cmd LNScene_SetAntialiasEnabled $292
#cmd LNScene_IsAntialiasEnabled $294
#cmd LNScene_SetSSREnabled $295
#cmd LNScene_IsSSREnabled $297
#cmd LNScene_SetSSAOEnabled $298
#cmd LNScene_IsSSAOEnabled $29A
#cmd LNScene_SetBloomEnabled $29B
#cmd LNScene_IsBloomEnabled $29D
#cmd LNScene_SetDOFEnabled $29E
#cmd LNScene_IsDOFEnabled $2A0
#cmd LNScene_SetTonemapEnabled $2A1
#cmd LNScene_IsTonemapEnabled $2A3
#cmd LNScene_SetVignetteEnabled $2A4
#cmd LNScene_IsVignetteEnabled $2A6
#cmd LNScene_SetGammaEnabled $2A7
#cmd LNScene_IsGammaEnabled $2A9
#cmd LNLevel_Create $2B9
#cmd LNLevel_AddObject $2AB
#cmd LNLevel_RemoveObject $2AD
#cmd LNLevel_AddSubLevel $2AF
#cmd LNLevel_RemoveSubLevel $2B1
#cmd LNLevel_RemoveAllSubLevels $2B3
#cmd LNLevel_SetPrototype_OnSerialize $9C4
#cmd LNLevel_SetPrototype_OnStart $9CF
#cmd LNLevel_SetPrototype_OnStop $9DA
#cmd LNLevel_SetPrototype_OnPause $9E5
#cmd LNLevel_SetPrototype_OnResume $9F0
#cmd LNLevel_SetPrototype_OnUpdate $9FB
#cmd LNUIEventArgs_Sender $2C0
#cmd LNUIEventArgs_SetPrototype_OnSerialize $A06
#cmd LNUIGeneralEventHandler_Create $6AB
#cmd LNUIEventHandler_Create $6B0
#cmd LNUILayoutElement_SetPrototype_OnSerialize $A11
#cmd LNUIElement_SetSize $2D2
#cmd LNUIElement_SetSizeWH $2D4
#cmd LNUIElement_SetMargin $2D7
#cmd LNUIElement_GetMargin $2D9
#cmd LNUIElement_SetPadding $2DA
#cmd LNUIElement_GetPadding $2DC
#cmd LNUIElement_SetHAlignment $2DD
#cmd LNUIElement_GetHAlignment $2DF
#cmd LNUIElement_SetVAlignment $2E0
#cmd LNUIElement_GetVAlignment $2E2
#cmd LNUIElement_SetAlignments $2E3
#cmd LNUIElement_SetPosition $2E6
#cmd LNUIElement_SetPositionXYZ $2E8
#cmd LNUIElement_GetPosition $2EC
#cmd LNUIElement_SetRotation $2ED
#cmd LNUIElement_SetEulerAngles $2EF
#cmd LNUIElement_GetRotation $2F3
#cmd LNUIElement_SetScale $2F4
#cmd LNUIElement_SetScaleS $2F6
#cmd LNUIElement_SetScaleXY $2F8
#cmd LNUIElement_GetScale $2FB
#cmd LNUIElement_SetCenterPoint $2FC
#cmd LNUIElement_SetCenterPointXYZ $2FE
#cmd LNUIElement_GetCenterPoint $302
#cmd LNUIElement_SetEnabled $303
#cmd LNUIElement_IsEnabled $305
#cmd LNUIElement_SetData $306
#cmd LNUIElement_GetData $308
#cmd LNUIElement_SetBackgroundColor $309
#cmd LNUIElement_GetBackgroundColor $30B
#cmd LNUIElement_SetBorderThickness $30C
#cmd LNUIElement_GetBorderThickness $30E
#cmd LNUIElement_SetBorderColor $30F
#cmd LNUIElement_GetBorderColor $311
#cmd LNUIElement_SetCornerRadius $312
#cmd LNUIElement_GetCornerRadius $314
#cmd LNUIElement_SetOpacity $315
#cmd LNUIElement_GetOpacity $317
#cmd LNUIElement_AddChild $318
#cmd LNUIElement_SetPrototype_OnSerialize $A1C
#cmd LNUITextBlock_Create $31B
#cmd LNUITextBlock_CreateWithText $31C
#cmd LNUITextBlock_SetPrototype_OnSerialize $A27
#cmd LNUISprite_Create $329
#cmd LNUISprite_CreateWithTexture $32A
#cmd LNUISprite_SetTexture $31F
#cmd LNUISprite_SetSourceRect $321
#cmd LNUISprite_SetSourceRectXYWH $323
#cmd LNUISprite_GetSourceRect $328
#cmd LNUISprite_SetPrototype_OnSerialize $A32
#cmd LNUIIcon_LoadFontIcon $32D
#cmd LNUIIcon_LoadFontIconWithNameSize $32F
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $332
#cmd LNUIIcon_SetPrototype_OnSerialize $A3D
#cmd LNUI_Add $337
#cmd LNUI_Remove $339
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $A48
#cmd LNUIBoxLayout_Create $340
#cmd LNUIBoxLayout_SetOrientation $33D
#cmd LNUIBoxLayout_GetOrientation $33F
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $A53
#cmd LNUIGridLayout_Create $350
#cmd LNUIGridLayout_SetColumnCount $342
#cmd LNUIGridLayout_SetRow $344
#cmd LNUIGridLayout_SetColumn $347
#cmd LNUIGridLayout_SetPlacement $34A
#cmd LNUIGridLayout_SetPrototype_OnSerialize $A5E
#cmd LNUIControl_Create $35C
#cmd LNUIControl_AddInlineVisual $35D
#cmd LNUIControl_SetPrototype_OnSerialize $A69
#cmd LNUIButtonBase_SetText $361
#cmd LNUIButtonBase_SetPrototype_OnSerialize $A74
#cmd LNUIButton_Create $364
#cmd LNUIButton_CreateWithText $365
#cmd LNUIButton_ConnectOnClicked $367
#cmd LNUIButton_SetPrototype_OnSerialize $A7F
#cmd LNUIWindow_Create $36A
#cmd LNUIWindow_SetPrototype_OnSerialize $A8A
#cmd LNUIListItem_ConnectOnSubmit $36F
#cmd LNUIListItem_SetPrototype_OnSerialize $A95
#cmd LNUIListItemsControl_SetItemsLayoutPanel $372
#cmd LNUIListItemsControl_SetSubmitMode $374
#cmd LNUIListItemsControl_GetSubmitMode $376
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $AA0
#cmd LNUIListBoxItem_Create $378
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $AAB
#cmd LNUIListBox_Create $37D
#cmd LNUIListBox_AddItem $37B
#cmd LNUIListBox_SetPrototype_OnSerialize $AB6
#cmd LNInput_IsPressed $37F
#cmd LNInput_IsTriggered $381
#cmd LNInput_IsTriggeredOff $383
#cmd LNInput_IsRepeated $385
#cmd LNInput_GetAxisValue $387
#cmd LNInput_ClearAllBindings $389
#cmd LNMouse_Pressed $38B
#cmd LNMouse_Triggered $38D
#cmd LNMouse_TriggeredOff $38F
#cmd LNMouse_Repeated $391
#cmd LNMouse_Position $393
#cmd LNInterpreterCommand_Code $395
#cmd LNInterpreterCommand_ParamsCount $396
#cmd LNInterpreterCommand_Param $397
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $AC1
#cmd LNInterpreterCommandList_Create $3AE
#cmd LNInterpreterCommandList_AddCommand $39A
#cmd LNInterpreterCommandList_AddCommand1 $39C
#cmd LNInterpreterCommandList_AddCommand2 $39F
#cmd LNInterpreterCommandList_AddCommand3 $3A3
#cmd LNInterpreterCommandList_AddCommand4 $3A8
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $ACC
#cmd LNInterpreterCommandDelegate_Create $74F
#cmd LNInterpreter_Create $3C2
#cmd LNInterpreter_Clear $3B3
#cmd LNInterpreter_Run $3B4
#cmd LNInterpreter_IsRunning $3B6
#cmd LNInterpreter_Update $3B7
#cmd LNInterpreter_Terminate $3B8
#cmd LNInterpreter_RegisterCommandHandler $3B9
#cmd LNInterpreter_SetWaitMode $3BC
#cmd LNInterpreter_GetWaitMode $3BE
#cmd LNInterpreter_SetWaitCount $3BF
#cmd LNInterpreter_GetWaitCount $3C1
#cmd LNInterpreter_SetPrototype_OnSerialize $AD7
#cmd LNInterpreter_SetPrototype_OnUpdateWait $AE3
#cmd LNEngineSettings_SetMainWindowSize $3C5
#cmd LNEngineSettings_SetMainWorldViewSize $3C8
#cmd LNEngineSettings_SetMainWindowTitle $3CB
#cmd LNEngineSettings_AddAssetDirectory $3CD
#cmd LNEngineSettings_AddAssetArchive $3CF
#cmd LNEngineSettings_SetFrameRate $3D2
#cmd LNEngineSettings_SetDefaultUITheme $3D4
#cmd LNEngineSettings_SetDebugToolEnabled $3D6
#cmd LNEngineSettings_SetEngineLogEnabled $3D8
#cmd LNEngineSettings_SetEngineLogFilePath $3DA
#cmd LNEngine_Initialize $3DD
#cmd LNEngine_Finalize $3DE
#cmd LNEngine_Update $3DF
#cmd LNEngine_Run $3E0
#cmd LNEngine_Time $3E2
#cmd LNEngine_GetCamera $3E3
#cmd LNEngine_GetMainLight $3E4
#cmd LNEngine_GetRenderView $3E5
#cmd LNApplication_Create $3EA
#cmd LNApplication_OnInit $3E7
#cmd LNApplication_OnUpdate $3E8
#cmd LNApplication_World $3E9
#cmd LNApplication_SetPrototype_OnSerialize $AEE
#cmd LNApplication_SetPrototype_OnInit $AF9
#cmd LNApplication_SetPrototype_OnUpdate $B04
#cmd LNDebug_SetGuideGridEnabled $3EC
#cmd LNDebug_SetPhysicsDebugDrawEnabled $3EE
#cmd LNDebug_Print $3F0
#cmd LNDebug_PrintWithTime $3F2
#cmd LNDebug_PrintWithTimeAndColor $3F5
#cmd LNObjectSerializeHandler_Create $77B
#cmd LNEventConnectionSerializeHandler_Create $786
#cmd LNVariantSerializeHandler_Create $791
#cmd LNZVTestClass1SerializeHandler_Create $79C
#cmd LNZVTestEventArgs1SerializeHandler_Create $7A7
#cmd LNSerializer2SerializeHandler_Create $7B2
#cmd LNAssetModelSerializeHandler_Create $7BD
#cmd LNTextureSerializeHandler_Create $7C8
#cmd LNTexture2DSerializeHandler_Create $7D3
#cmd LNRenderViewSerializeHandler_Create $7DE
#cmd LNStaticMeshModelSerializeHandler_Create $7E9
#cmd LNSkinnedMeshModelSerializeHandler_Create $7F4
#cmd LNAnimationControllerSerializeHandler_Create $7FF
#cmd LNCollisionShapeSerializeHandler_Create $80A
#cmd LNBoxCollisionShapeSerializeHandler_Create $815
#cmd LNAnimationClipSerializeHandler_Create $820
#cmd LNAnimationStateSerializeHandler_Create $82B
#cmd LNComponentSerializeHandler_Create $836
#cmd LNVisualComponentSerializeHandler_Create $841
#cmd LNSpriteComponentSerializeHandler_Create $84C
#cmd LNCharacterControllerSerializeHandler_Create $857
#cmd LNWorldSerializeHandler_Create $862
#cmd LNComponentListSerializeHandler_Create $86D
#cmd LNWorldObjectSerializeHandler_Create $878
#cmd LNWorldObjectUpdateHandler_Create $883
#cmd LNVisualObjectSerializeHandler_Create $88E
#cmd LNVisualObjectUpdateHandler_Create $899
#cmd LNCameraSerializeHandler_Create $8A4
#cmd LNCameraUpdateHandler_Create $8AF
#cmd LNEnvironmentLightSerializeHandler_Create $8BA
#cmd LNEnvironmentLightUpdateHandler_Create $8C5
#cmd LNDirectionalLightSerializeHandler_Create $8D0
#cmd LNDirectionalLightUpdateHandler_Create $8DB
#cmd LNPointLightSerializeHandler_Create $8E6
#cmd LNPointLightUpdateHandler_Create $8F1
#cmd LNSpotLightSerializeHandler_Create $8FC
#cmd LNSpotLightUpdateHandler_Create $907
#cmd LNSpriteSerializeHandler_Create $912
#cmd LNSpriteUpdateHandler_Create $91D
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $928
#cmd LNRaycasterSerializeHandler_Create $933
#cmd LNRaycastResultSerializeHandler_Create $93E
#cmd LNWorldRenderViewSerializeHandler_Create $949
#cmd LNBoxMeshSerializeHandler_Create $954
#cmd LNBoxMeshUpdateHandler_Create $95F
#cmd LNPlaneMeshSerializeHandler_Create $96A
#cmd LNPlaneMeshUpdateHandler_Create $975
#cmd LNStaticMeshSerializeHandler_Create $980
#cmd LNStaticMeshUpdateHandler_Create $98B
#cmd LNStaticMeshComponentSerializeHandler_Create $996
#cmd LNSkinnedMeshComponentSerializeHandler_Create $9A1
#cmd LNCollisionSerializeHandler_Create $9AC
#cmd LNTriggerBodyComponentSerializeHandler_Create $9B7
#cmd LNLevelSerializeHandler_Create $9C2
#cmd LNLevelStartHandler_Create $9CD
#cmd LNLevelStopHandler_Create $9D8
#cmd LNLevelPauseHandler_Create $9E3
#cmd LNLevelResumeHandler_Create $9EE
#cmd LNLevelUpdateHandler_Create $9F9
#cmd LNUIEventArgsSerializeHandler_Create $A04
#cmd LNUILayoutElementSerializeHandler_Create $A0F
#cmd LNUIElementSerializeHandler_Create $A1A
#cmd LNUITextBlockSerializeHandler_Create $A25
#cmd LNUISpriteSerializeHandler_Create $A30
#cmd LNUIIconSerializeHandler_Create $A3B
#cmd LNUILayoutPanelSerializeHandler_Create $A46
#cmd LNUIBoxLayoutSerializeHandler_Create $A51
#cmd LNUIGridLayoutSerializeHandler_Create $A5C
#cmd LNUIControlSerializeHandler_Create $A67
#cmd LNUIButtonBaseSerializeHandler_Create $A72
#cmd LNUIButtonSerializeHandler_Create $A7D
#cmd LNUIWindowSerializeHandler_Create $A88
#cmd LNUIListItemSerializeHandler_Create $A93
#cmd LNUIListItemsControlSerializeHandler_Create $A9E
#cmd LNUIListBoxItemSerializeHandler_Create $AA9
#cmd LNUIListBoxSerializeHandler_Create $AB4
#cmd LNInterpreterCommandSerializeHandler_Create $ABF
#cmd LNInterpreterCommandListSerializeHandler_Create $ACA
#cmd LNInterpreterSerializeHandler_Create $AD5
#cmd LNInterpreterUpdateWaitHandler_Create $AE1
#cmd LNApplicationSerializeHandler_Create $AEC
#cmd LNApplicationInitHandler_Create $AF7
#cmd LNApplicationUpdateHandler_Create $B02



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
