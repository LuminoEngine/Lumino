
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
#cmd LNVector3_GetX $432
#cmd LNVector3_SetX $434
#cmd LNVector3_GetY $435
#cmd LNVector3_SetY $437
#cmd LNVector3_GetZ $438
#cmd LNVector3_SetZ $43A
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $451
#cmd LNVector4_SetX $453
#cmd LNVector4_GetY $454
#cmd LNVector4_SetY $456
#cmd LNVector4_GetZ $457
#cmd LNVector4_SetZ $459
#cmd LNVector4_GetW $45A
#cmd LNVector4_SetW $45C
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $46F
#cmd LNQuaternion_SetX $471
#cmd LNQuaternion_GetY $472
#cmd LNQuaternion_SetY $474
#cmd LNQuaternion_GetZ $475
#cmd LNQuaternion_SetZ $477
#cmd LNQuaternion_GetW $478
#cmd LNQuaternion_SetW $47A
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $48E
#cmd LNMatrix_SetRow0 $490
#cmd LNMatrix_GetRow1 $491
#cmd LNMatrix_SetRow1 $493
#cmd LNMatrix_GetRow2 $494
#cmd LNMatrix_SetRow2 $496
#cmd LNMatrix_GetRow3 $497
#cmd LNMatrix_SetRow3 $499
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $102
#cmd LNColor_GetR $519
#cmd LNColor_SetR $51B
#cmd LNColor_GetG $51C
#cmd LNColor_SetG $51E
#cmd LNColor_GetB $51F
#cmd LNColor_SetB $521
#cmd LNColor_GetA $522
#cmd LNColor_SetA $524
#cmd LNColor_SetZeros $107
#cmd LNColor_Set $108
#cmd LNPoint $10D
#cmd LNPoint_GetX $538
#cmd LNPoint_SetX $53A
#cmd LNPoint_GetY $53B
#cmd LNPoint_SetY $53D
#cmd LNPoint_SetZeros $110
#cmd LNPoint_Set $111
#cmd LNSize $114
#cmd LNSize_GetWidth $548
#cmd LNSize_SetWidth $54A
#cmd LNSize_GetHeight $54B
#cmd LNSize_SetHeight $54D
#cmd LNSize_SetZeros $117
#cmd LNSize_Set $118
#cmd LNRect $11B
#cmd LNRect_GetX $558
#cmd LNRect_SetX $55A
#cmd LNRect_GetY $55B
#cmd LNRect_SetY $55D
#cmd LNRect_GetWidth $55E
#cmd LNRect_SetWidth $560
#cmd LNRect_GetHeight $561
#cmd LNRect_SetHeight $563
#cmd LNRect_SetZeros $120
#cmd LNRect_Set $121
#cmd LNRect_GetLeft $126
#cmd LNRect_SetSize $127
#cmd LNRect_GetSize $129
#cmd LNThickness $12A
#cmd LNThickness_GetLeft $57C
#cmd LNThickness_SetLeft $57E
#cmd LNThickness_GetTop $57F
#cmd LNThickness_SetTop $581
#cmd LNThickness_GetRight $582
#cmd LNThickness_SetRight $584
#cmd LNThickness_GetBottom $585
#cmd LNThickness_SetBottom $587
#cmd LNThickness_SetZeros $12F
#cmd LNThickness_Set $130
#cmd LNCornerRadius $135
#cmd LNCornerRadius_GetTopleft $59A
#cmd LNCornerRadius_SetTopleft $59C
#cmd LNCornerRadius_GetTopright $59D
#cmd LNCornerRadius_SetTopright $59F
#cmd LNCornerRadius_GetBottomright $5A0
#cmd LNCornerRadius_SetBottomright $5A2
#cmd LNCornerRadius_GetBottomleft $5A3
#cmd LNCornerRadius_SetBottomleft $5A5
#cmd LNCornerRadius_SetZeros $13A
#cmd LNCornerRadius_Set $13B

#cmd LNObject_Release $7A
#cmd LNObject_Retain $7B
#cmd LNObject_GetReferenceCount $7C
#cmd LNObject_SetPrototype_OnSerialize $7E7
#cmd LNEventConnection_SetPrototype_OnSerialize $7F2
#cmd LNPromiseFailureDelegate_Create $4B4
#cmd LNVariant_Create $81
#cmd LNVariant_SetInt $82
#cmd LNVariant_GetInt $84
#cmd LNVariant_SetPrototype_OnSerialize $7FD
#cmd LNZVTestDelegate1_Create $4BD
#cmd LNZVTestDelegate2_Create $4C3
#cmd LNZVTestDelegate3_Create $4C8
#cmd LNZVTestEventHandler1_Create $4CD
#cmd LNZVTestEventHandler2_Create $4D2
#cmd LNZVTestPromise1_ThenWith $4D5
#cmd LNZVTestPromise1_CatchWith $4D7
#cmd LNZVTestPromise2_ThenWith $4DB
#cmd LNZVTestPromise2_CatchWith $4DD
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $808
#cmd LNZVTestEventArgs1_Create $B0
#cmd LNZVTestEventArgs1_CreateWithValue $B1
#cmd LNZVTestEventArgs1_GetValue $AF
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $813
#cmd LNSerializer2_SetPrototype_OnSerialize $81E
#cmd LNAssetModel_Create $BD
#cmd LNAssetModel_Target $BC
#cmd LNAssetModel_SetPrototype_OnSerialize $829
#cmd LNAssets_SaveAssetToLocalFile $C0
#cmd LNAssets_LoadAssetFromLocalFile $C3
#cmd LNAssets_LoadAsset $C5
#cmd LNAssets_ReloadAsset $C7
#cmd LNAssets_ReadAllText $CA
#cmd LNAudio_PlayBGM $CE
#cmd LNAudio_StopBGM $D3
#cmd LNAudio_PlayBGS $D5
#cmd LNAudio_StopBGS $DA
#cmd LNAudio_PlayME $DC
#cmd LNAudio_StopME $E0
#cmd LNAudio_PlaySE $E1
#cmd LNAudio_PlaySE3D $E5
#cmd LNAudio_StopSE $EB
#cmd LNTexture2DDelegate_Create $511
#cmd LNTexture2DPromise_ThenWith $514
#cmd LNTexture2DPromise_CatchWith $516
#cmd LNTexture_SetPrototype_OnSerialize $834
#cmd LNTexture2D_Create $146
#cmd LNTexture2D_CreateWithFormat $149
#cmd LNTexture2D_Load $142
#cmd LNTexture2D_LoadEmoji $144
#cmd LNTexture2D_SetPrototype_OnSerialize $83F
#cmd LNShader_Load $14E
#cmd LNShader_SetFloat $150
#cmd LNShader_SetVector3 $153
#cmd LNShader_SetVector4 $156
#cmd LNShader_SetPrototype_OnSerialize $84A
#cmd LNRenderView_SetPrototype_OnSerialize $855
#cmd LNMaterial_SetShader $161
#cmd LNMaterial_GetShader $163
#cmd LNMaterial_SetPrototype_OnSerialize $860
#cmd LNStaticMeshModel_Load $165
#cmd LNStaticMeshModel_MaterialCount $167
#cmd LNStaticMeshModel_Material $168
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $86B
#cmd LNSkinnedMeshModel_Load $16B
#cmd LNSkinnedMeshModel_GetAnimationController $16D
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $876
#cmd LNAnimationController_AddClip $16F
#cmd LNAnimationController_Play $171
#cmd LNAnimationController_SetPrototype_OnSerialize $881
#cmd LNCollisionShape_SetPrototype_OnSerialize $88C
#cmd LNBoxCollisionShape_Create $176
#cmd LNBoxCollisionShape_CreateWHD $178
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $897
#cmd LNAnimationClip_Load $182
#cmd LNAnimationClip_SetHierarchicalAnimationMode $184
#cmd LNAnimationClip_HierarchicalAnimationMode $186
#cmd LNAnimationClip_SetPrototype_OnSerialize $8A2
#cmd LNAnimationState_SetPrototype_OnSerialize $8AD
#cmd LNComponent_SetPrototype_OnSerialize $8B8
#cmd LNVisualComponent_SetVisible $18A
#cmd LNVisualComponent_IsVisible $18C
#cmd LNVisualComponent_SetPrototype_OnSerialize $8C3
#cmd LNSpriteComponent_SetTexture $18E
#cmd LNSpriteComponent_SetPrototype_OnSerialize $8CE
#cmd LNCollisionEventHandler_Create $5E1
#cmd LNCharacterController_Create $1AB
#cmd LNCharacterController_SetVelocity $198
#cmd LNCharacterController_GetVelocity $19A
#cmd LNCharacterController_SetInputControlEnabled $19B
#cmd LNCharacterController_SetCameraControlEnabled $19D
#cmd LNCharacterController_SetHeight $19F
#cmd LNCharacterController_GetHeight $1A1
#cmd LNCharacterController_SetCameraRadius $1A2
#cmd LNCharacterController_GetCameraRadius $1A4
#cmd LNCharacterController_SetCollisionEnter $1A5
#cmd LNCharacterController_SetCollisionLeave $1A7
#cmd LNCharacterController_SetCollisionStay $1A9
#cmd LNCharacterController_SetPrototype_OnSerialize $8D9
#cmd LNWorld_Add $1AD
#cmd LNWorld_SetPrototype_OnSerialize $8E4
#cmd LNComponentList_GetLength $5FC
#cmd LNComponentList_GetItem $5FE
#cmd LNComponentList_SetPrototype_OnSerialize $8EF
#cmd LNWorldObject_Create $1E4
#cmd LNWorldObject_SetPosition $1B1
#cmd LNWorldObject_SetPositionXYZ $1B3
#cmd LNWorldObject_GetPosition $1B7
#cmd LNWorldObject_SetRotationQuaternion $1B8
#cmd LNWorldObject_SetRotation $1BA
#cmd LNWorldObject_GetRotation $1BE
#cmd LNWorldObject_SetScale $1BF
#cmd LNWorldObject_SetScaleS $1C1
#cmd LNWorldObject_SetScaleXYZ $1C3
#cmd LNWorldObject_GetScale $1C7
#cmd LNWorldObject_SetCenterPoint $1C8
#cmd LNWorldObject_SetCenterPointXYZ $1CA
#cmd LNWorldObject_GetCenterPoint $1CE
#cmd LNWorldObject_LookAt $1CF
#cmd LNWorldObject_LookAtXYZ $1D1
#cmd LNWorldObject_AddComponent $1D5
#cmd LNWorldObject_RemoveComponent $1D7
#cmd LNWorldObject_AddTag $1D9
#cmd LNWorldObject_RemoveTag $1DB
#cmd LNWorldObject_HasTag $1DD
#cmd LNWorldObject_Destroy $1DF
#cmd LNWorldObject_GetComponents $1E0
#cmd LNWorldObject_SetPrototype_OnSerialize $8FA
#cmd LNWorldObject_SetPrototype_OnPreUpdate $905
#cmd LNWorldObject_SetPrototype_OnUpdate $910
#cmd LNVisualObject_SetVisible $1E6
#cmd LNVisualObject_IsVisible $1E8
#cmd LNVisualObject_SetOpacity $1E9
#cmd LNVisualObject_GetOpacity $1EB
#cmd LNVisualObject_SetPrototype_OnSerialize $91B
#cmd LNVisualObject_SetPrototype_OnPreUpdate $926
#cmd LNVisualObject_SetPrototype_OnUpdate $931
#cmd LNCamera_SetPrototype_OnSerialize $93C
#cmd LNCamera_SetPrototype_OnPreUpdate $947
#cmd LNCamera_SetPrototype_OnUpdate $952
#cmd LNEnvironmentLight_SetEnabled $1EE
#cmd LNEnvironmentLight_IsEnabled $1F0
#cmd LNEnvironmentLight_SetColor $1F1
#cmd LNEnvironmentLight_GetColor $1F3
#cmd LNEnvironmentLight_SetAmbientColor $1F4
#cmd LNEnvironmentLight_GetAmbientColor $1F6
#cmd LNEnvironmentLight_GetSkyColor $1F7
#cmd LNEnvironmentLight_SetSkyColor $1F8
#cmd LNEnvironmentLight_GetGroundColor $1FA
#cmd LNEnvironmentLight_SetGroundColor $1FB
#cmd LNEnvironmentLight_SetIntensity $1FD
#cmd LNEnvironmentLight_GetIntensity $1FF
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $200
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $202
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $203
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $205
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $95D
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $968
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $973
#cmd LNDirectionalLight_Create $216
#cmd LNDirectionalLight_CreateWithColor $217
#cmd LNDirectionalLight_SetEnabled $207
#cmd LNDirectionalLight_IsEnabled $209
#cmd LNDirectionalLight_SetColor $20A
#cmd LNDirectionalLight_GetColor $20C
#cmd LNDirectionalLight_SetIntensity $20D
#cmd LNDirectionalLight_GetIntensity $20F
#cmd LNDirectionalLight_SetShadowEffectiveDistance $210
#cmd LNDirectionalLight_GetShadowEffectiveDistance $212
#cmd LNDirectionalLight_SetShadowEffectiveDepth $213
#cmd LNDirectionalLight_GetShadowEffectiveDepth $215
#cmd LNDirectionalLight_SetPrototype_OnSerialize $97E
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $989
#cmd LNDirectionalLight_SetPrototype_OnUpdate $994
#cmd LNPointLight_Create $229
#cmd LNPointLight_CreateWithColorAndRange $22A
#cmd LNPointLight_SetEnabled $21A
#cmd LNPointLight_IsEnabled $21C
#cmd LNPointLight_SetColor $21D
#cmd LNPointLight_GetColor $21F
#cmd LNPointLight_SetIntensity $220
#cmd LNPointLight_GetIntensity $222
#cmd LNPointLight_SetRange $223
#cmd LNPointLight_GetRange $225
#cmd LNPointLight_SetAttenuation $226
#cmd LNPointLight_GetAttenuation $228
#cmd LNPointLight_SetPrototype_OnSerialize $99F
#cmd LNPointLight_SetPrototype_OnPreUpdate $9AA
#cmd LNPointLight_SetPrototype_OnUpdate $9B5
#cmd LNSpotLight_Create $243
#cmd LNSpotLight_CreateWithColorAndRange $244
#cmd LNSpotLight_SetEnabled $22E
#cmd LNSpotLight_IsEnabled $230
#cmd LNSpotLight_SetColor $231
#cmd LNSpotLight_GetColor $233
#cmd LNSpotLight_SetIntensity $234
#cmd LNSpotLight_GetIntensity $236
#cmd LNSpotLight_SetRange $237
#cmd LNSpotLight_GetRange $239
#cmd LNSpotLight_SetAttenuation $23A
#cmd LNSpotLight_GetAttenuation $23C
#cmd LNSpotLight_SetAngle $23D
#cmd LNSpotLight_GetAngle $23F
#cmd LNSpotLight_SetPenumbra $240
#cmd LNSpotLight_GetPenumbra $242
#cmd LNSpotLight_SetPrototype_OnSerialize $9C0
#cmd LNSpotLight_SetPrototype_OnPreUpdate $9CB
#cmd LNSpotLight_SetPrototype_OnUpdate $9D6
#cmd LNTestDelegate_Create $6A8
#cmd LNSprite_Create $25A
#cmd LNSprite_CreateWithTexture $25B
#cmd LNSprite_CreateWithTextureAndSize $25D
#cmd LNSprite_SetTexture $24C
#cmd LNSprite_SetSize $24E
#cmd LNSprite_SetSizeWH $250
#cmd LNSprite_SetSourceRectXYWH $253
#cmd LNSprite_SetCallerTest $258
#cmd LNSprite_SetPrototype_OnSerialize $9E1
#cmd LNSprite_SetPrototype_OnPreUpdate $9EC
#cmd LNSprite_SetPrototype_OnUpdate $9F7
#cmd LNCameraOrbitControlComponent_Create $262
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $A02
#cmd LNRaycaster_FromScreen $264
#cmd LNRaycaster_IntersectPlane $266
#cmd LNRaycaster_SetPrototype_OnSerialize $A0D
#cmd LNRaycastResult_GetPoint $26B
#cmd LNRaycastResult_SetPrototype_OnSerialize $A18
#cmd LNWorldRenderView_SetGuideGridEnabled $26D
#cmd LNWorldRenderView_GetGuideGridEnabled $26F
#cmd LNWorldRenderView_SetPrototype_OnSerialize $A23
#cmd LNBoxMesh_Create $271
#cmd LNBoxMesh_SetPrototype_OnSerialize $A2E
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $A39
#cmd LNBoxMesh_SetPrototype_OnUpdate $A44
#cmd LNPlaneMesh_Create $273
#cmd LNPlaneMesh_SetPrototype_OnSerialize $A4F
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $A5A
#cmd LNPlaneMesh_SetPrototype_OnUpdate $A65
#cmd LNStaticMesh_Load $275
#cmd LNStaticMesh_GetModel $277
#cmd LNStaticMesh_MakeCollisionBody $278
#cmd LNStaticMesh_SetPrototype_OnSerialize $A70
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $A7B
#cmd LNStaticMesh_SetPrototype_OnUpdate $A86
#cmd LNStaticMeshComponent_Create $27F
#cmd LNStaticMeshComponent_SetModel $27B
#cmd LNStaticMeshComponent_MakeCollisionBody $27D
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $A91
#cmd LNSkinnedMeshComponent_Create $283
#cmd LNSkinnedMeshComponent_SetModel $281
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $A9C
#cmd LNCollision_GetWorldObject $285
#cmd LNCollision_SetPrototype_OnSerialize $AA7
#cmd LNTriggerBodyComponent_Create $289
#cmd LNTriggerBodyComponent_AddCollisionShape $287
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $AB2
#cmd LNScene_SetClearMode $28B
#cmd LNScene_SetSkyColor $28D
#cmd LNScene_SetSkyHorizonColor $28F
#cmd LNScene_SetSkyCloudColor $291
#cmd LNScene_SetSkyOverlayColor $293
#cmd LNScene_GotoLevel $295
#cmd LNScene_CallLevel $298
#cmd LNScene_ReturnLevel $29B
#cmd LNScene_ActiveLevel $29D
#cmd LNScene_IsTransitionEffectRunning $29E
#cmd LNScene_SetTransitionEffectMode $29F
#cmd LNScene_TransitionEffectMode $2A1
#cmd LNScene_SetTransitionDuration $2A2
#cmd LNScene_TransitionDuration $2A4
#cmd LNScene_SetTransitionEffectColor $2A5
#cmd LNScene_TransitionEffectColor $2A7
#cmd LNScene_SetTransitionEffectMaskTexture $2A8
#cmd LNScene_TransitionEffectMaskTexture $2AA
#cmd LNScene_SetTransitionEffectVague $2AB
#cmd LNScene_TransitionEffectVague $2AD
#cmd LNScene_StartFadeOut $2AE
#cmd LNScene_StartFadeIn $2AF
#cmd LNScene_SetScreenBlendColor $2B0
#cmd LNScene_ScreenBlendColor $2B2
#cmd LNScene_SetAntialiasEnabled $2B3
#cmd LNScene_IsAntialiasEnabled $2B5
#cmd LNScene_SetSSREnabled $2B6
#cmd LNScene_IsSSREnabled $2B8
#cmd LNScene_SetSSAOEnabled $2B9
#cmd LNScene_IsSSAOEnabled $2BB
#cmd LNScene_SetBloomEnabled $2BC
#cmd LNScene_IsBloomEnabled $2BE
#cmd LNScene_SetDOFEnabled $2BF
#cmd LNScene_IsDOFEnabled $2C1
#cmd LNScene_SetTonemapEnabled $2C2
#cmd LNScene_IsTonemapEnabled $2C4
#cmd LNScene_SetVignetteEnabled $2C5
#cmd LNScene_IsVignetteEnabled $2C7
#cmd LNScene_SetGammaEnabled $2C8
#cmd LNScene_IsGammaEnabled $2CA
#cmd LNLevel_Create $2DA
#cmd LNLevel_AddObject $2CC
#cmd LNLevel_RemoveObject $2CE
#cmd LNLevel_AddSubLevel $2D0
#cmd LNLevel_RemoveSubLevel $2D2
#cmd LNLevel_RemoveAllSubLevels $2D4
#cmd LNLevel_SetPrototype_OnSerialize $ABD
#cmd LNLevel_SetPrototype_OnStart $AC8
#cmd LNLevel_SetPrototype_OnStop $AD3
#cmd LNLevel_SetPrototype_OnPause $ADE
#cmd LNLevel_SetPrototype_OnResume $AE9
#cmd LNLevel_SetPrototype_OnUpdate $AF4
#cmd LNUIEventArgs_Sender $2E5
#cmd LNUIEventArgs_SetPrototype_OnSerialize $AFF
#cmd LNUIGeneralEventHandler_Create $6FC
#cmd LNUIEventHandler_Create $701
#cmd LNUILayoutElement_SetPrototype_OnSerialize $B0A
#cmd LNUIElement_SetSize $2F7
#cmd LNUIElement_SetSizeWH $2F9
#cmd LNUIElement_SetWidth $2FC
#cmd LNUIElement_GetWidth $2FE
#cmd LNUIElement_SetHeight $2FF
#cmd LNUIElement_GetHeight $301
#cmd LNUIElement_SetMargin $302
#cmd LNUIElement_GetMargin $304
#cmd LNUIElement_SetPadding $305
#cmd LNUIElement_GetPadding $307
#cmd LNUIElement_SetHAlignment $308
#cmd LNUIElement_GetHAlignment $30A
#cmd LNUIElement_SetVAlignment $30B
#cmd LNUIElement_GetVAlignment $30D
#cmd LNUIElement_SetAlignments $30E
#cmd LNUIElement_SetPosition $311
#cmd LNUIElement_SetPositionXYZ $313
#cmd LNUIElement_GetPosition $317
#cmd LNUIElement_SetRotation $318
#cmd LNUIElement_SetEulerAngles $31A
#cmd LNUIElement_GetRotation $31E
#cmd LNUIElement_SetScale $31F
#cmd LNUIElement_SetScaleS $321
#cmd LNUIElement_SetScaleXY $323
#cmd LNUIElement_GetScale $326
#cmd LNUIElement_SetCenterPoint $327
#cmd LNUIElement_SetCenterPointXYZ $329
#cmd LNUIElement_GetCenterPoint $32D
#cmd LNUIElement_SetEnabled $32E
#cmd LNUIElement_IsEnabled $330
#cmd LNUIElement_SetData $331
#cmd LNUIElement_GetData $333
#cmd LNUIElement_SetBackgroundColor $334
#cmd LNUIElement_GetBackgroundColor $336
#cmd LNUIElement_SetBorderThickness $337
#cmd LNUIElement_GetBorderThickness $339
#cmd LNUIElement_SetBorderColor $33A
#cmd LNUIElement_GetBorderColor $33C
#cmd LNUIElement_SetCornerRadius $33D
#cmd LNUIElement_GetCornerRadius $33F
#cmd LNUIElement_SetVisibility $340
#cmd LNUIElement_GetVisibility $342
#cmd LNUIElement_SetOpacity $343
#cmd LNUIElement_GetOpacity $345
#cmd LNUIElement_AddChild $346
#cmd LNUIElement_SetPrototype_OnSerialize $B15
#cmd LNUITextBlock_Create $34C
#cmd LNUITextBlock_CreateWithText $34D
#cmd LNUITextBlock_SetText $349
#cmd LNUITextBlock_GetText $34B
#cmd LNUITextBlock_SetPrototype_OnSerialize $B20
#cmd LNUISprite_Create $35A
#cmd LNUISprite_CreateWithTexture $35B
#cmd LNUISprite_SetTexture $350
#cmd LNUISprite_SetSourceRect $352
#cmd LNUISprite_SetSourceRectXYWH $354
#cmd LNUISprite_GetSourceRect $359
#cmd LNUISprite_SetPrototype_OnSerialize $B2B
#cmd LNUIIcon_LoadFontIcon $35E
#cmd LNUIIcon_LoadFontIconWithNameSize $360
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $363
#cmd LNUIIcon_SetPrototype_OnSerialize $B36
#cmd LNUIMessageTextArea_Create $36C
#cmd LNUIMessageTextArea_SetText $368
#cmd LNUIMessageTextArea_SetTypingSpeed $36A
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $B41
#cmd LNUI_Add $36E
#cmd LNUI_Remove $370
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $B4C
#cmd LNUIBoxLayout_Create $377
#cmd LNUIBoxLayout_SetOrientation $374
#cmd LNUIBoxLayout_GetOrientation $376
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $B57
#cmd LNUIGridLayout_Create $387
#cmd LNUIGridLayout_SetColumnCount $379
#cmd LNUIGridLayout_SetRow $37B
#cmd LNUIGridLayout_SetColumn $37E
#cmd LNUIGridLayout_SetPlacement $381
#cmd LNUIGridLayout_SetPrototype_OnSerialize $B62
#cmd LNUIControl_Create $393
#cmd LNUIControl_AddInlineVisual $394
#cmd LNUIControl_SetPrototype_OnSerialize $B6D
#cmd LNUIButtonBase_SetText $398
#cmd LNUIButtonBase_SetPrototype_OnSerialize $B78
#cmd LNUIButton_Create $39B
#cmd LNUIButton_CreateWithText $39C
#cmd LNUIButton_ConnectOnClicked $39E
#cmd LNUIButton_SetPrototype_OnSerialize $B83
#cmd LNUIWindow_Create $3A1
#cmd LNUIWindow_SetPrototype_OnSerialize $B8E
#cmd LNUIListItem_ConnectOnSubmit $3A6
#cmd LNUIListItem_SetPrototype_OnSerialize $B99
#cmd LNUIListItemsControl_SetItemsLayoutPanel $3A9
#cmd LNUIListItemsControl_SetSubmitMode $3AB
#cmd LNUIListItemsControl_GetSubmitMode $3AD
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $BA4
#cmd LNUIListBoxItem_Create $3AF
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $BAF
#cmd LNUIListBox_Create $3B4
#cmd LNUIListBox_AddItem $3B2
#cmd LNUIListBox_SetPrototype_OnSerialize $BBA
#cmd LNInput_IsPressed $3B6
#cmd LNInput_IsTriggered $3B8
#cmd LNInput_IsTriggeredOff $3BA
#cmd LNInput_IsRepeated $3BC
#cmd LNInput_GetAxisValue $3BE
#cmd LNInput_ClearAllBindings $3C0
#cmd LNMouse_Pressed $3C2
#cmd LNMouse_Triggered $3C4
#cmd LNMouse_TriggeredOff $3C6
#cmd LNMouse_Repeated $3C8
#cmd LNMouse_Position $3CA
#cmd LNInterpreterCommand_Code $3CC
#cmd LNInterpreterCommand_ParamsCount $3CD
#cmd LNInterpreterCommand_Param $3CE
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $BC5
#cmd LNInterpreterCommandList_Create $3E5
#cmd LNInterpreterCommandList_AddCommand $3D1
#cmd LNInterpreterCommandList_AddCommand1 $3D3
#cmd LNInterpreterCommandList_AddCommand2 $3D6
#cmd LNInterpreterCommandList_AddCommand3 $3DA
#cmd LNInterpreterCommandList_AddCommand4 $3DF
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $BD0
#cmd LNInterpreterCommandDelegate_Create $7B9
#cmd LNInterpreter_Create $3F9
#cmd LNInterpreter_Clear $3EA
#cmd LNInterpreter_Run $3EB
#cmd LNInterpreter_IsRunning $3ED
#cmd LNInterpreter_Update $3EE
#cmd LNInterpreter_Terminate $3EF
#cmd LNInterpreter_RegisterCommandHandler $3F0
#cmd LNInterpreter_SetWaitMode $3F3
#cmd LNInterpreter_GetWaitMode $3F5
#cmd LNInterpreter_SetWaitCount $3F6
#cmd LNInterpreter_GetWaitCount $3F8
#cmd LNInterpreter_SetPrototype_OnSerialize $BDB
#cmd LNInterpreter_SetPrototype_OnUpdateWait $BE7
#cmd LNEngineSettings_SetMainWindowSize $3FC
#cmd LNEngineSettings_SetMainWorldViewSize $3FF
#cmd LNEngineSettings_SetMainWindowTitle $402
#cmd LNEngineSettings_AddAssetDirectory $404
#cmd LNEngineSettings_AddAssetArchive $406
#cmd LNEngineSettings_SetFrameRate $409
#cmd LNEngineSettings_SetUITheme $40B
#cmd LNEngineSettings_SetFontFile $40D
#cmd LNEngineSettings_SetDebugToolEnabled $40F
#cmd LNEngineSettings_SetEngineLogEnabled $411
#cmd LNEngineSettings_SetEngineLogFilePath $413
#cmd LNEngine_Initialize $416
#cmd LNEngine_Finalize $417
#cmd LNEngine_Update $418
#cmd LNEngine_Run $419
#cmd LNEngine_Time $41B
#cmd LNEngine_GetCamera $41C
#cmd LNEngine_GetMainLight $41D
#cmd LNEngine_GetRenderView $41E
#cmd LNApplication_Create $423
#cmd LNApplication_OnInit $420
#cmd LNApplication_OnUpdate $421
#cmd LNApplication_World $422
#cmd LNApplication_SetPrototype_OnSerialize $BF2
#cmd LNApplication_SetPrototype_OnInit $BFD
#cmd LNApplication_SetPrototype_OnUpdate $C08
#cmd LNDebug_SetGuideGridEnabled $425
#cmd LNDebug_SetPhysicsDebugDrawEnabled $427
#cmd LNDebug_Print $429
#cmd LNDebug_PrintWithTime $42B
#cmd LNDebug_PrintWithTimeAndColor $42E
#cmd LNObjectSerializeHandler_Create $7E5
#cmd LNEventConnectionSerializeHandler_Create $7F0
#cmd LNVariantSerializeHandler_Create $7FB
#cmd LNZVTestClass1SerializeHandler_Create $806
#cmd LNZVTestEventArgs1SerializeHandler_Create $811
#cmd LNSerializer2SerializeHandler_Create $81C
#cmd LNAssetModelSerializeHandler_Create $827
#cmd LNTextureSerializeHandler_Create $832
#cmd LNTexture2DSerializeHandler_Create $83D
#cmd LNShaderSerializeHandler_Create $848
#cmd LNRenderViewSerializeHandler_Create $853
#cmd LNMaterialSerializeHandler_Create $85E
#cmd LNStaticMeshModelSerializeHandler_Create $869
#cmd LNSkinnedMeshModelSerializeHandler_Create $874
#cmd LNAnimationControllerSerializeHandler_Create $87F
#cmd LNCollisionShapeSerializeHandler_Create $88A
#cmd LNBoxCollisionShapeSerializeHandler_Create $895
#cmd LNAnimationClipSerializeHandler_Create $8A0
#cmd LNAnimationStateSerializeHandler_Create $8AB
#cmd LNComponentSerializeHandler_Create $8B6
#cmd LNVisualComponentSerializeHandler_Create $8C1
#cmd LNSpriteComponentSerializeHandler_Create $8CC
#cmd LNCharacterControllerSerializeHandler_Create $8D7
#cmd LNWorldSerializeHandler_Create $8E2
#cmd LNComponentListSerializeHandler_Create $8ED
#cmd LNWorldObjectSerializeHandler_Create $8F8
#cmd LNWorldObjectPreUpdateHandler_Create $903
#cmd LNWorldObjectUpdateHandler_Create $90E
#cmd LNVisualObjectSerializeHandler_Create $919
#cmd LNVisualObjectPreUpdateHandler_Create $924
#cmd LNVisualObjectUpdateHandler_Create $92F
#cmd LNCameraSerializeHandler_Create $93A
#cmd LNCameraPreUpdateHandler_Create $945
#cmd LNCameraUpdateHandler_Create $950
#cmd LNEnvironmentLightSerializeHandler_Create $95B
#cmd LNEnvironmentLightPreUpdateHandler_Create $966
#cmd LNEnvironmentLightUpdateHandler_Create $971
#cmd LNDirectionalLightSerializeHandler_Create $97C
#cmd LNDirectionalLightPreUpdateHandler_Create $987
#cmd LNDirectionalLightUpdateHandler_Create $992
#cmd LNPointLightSerializeHandler_Create $99D
#cmd LNPointLightPreUpdateHandler_Create $9A8
#cmd LNPointLightUpdateHandler_Create $9B3
#cmd LNSpotLightSerializeHandler_Create $9BE
#cmd LNSpotLightPreUpdateHandler_Create $9C9
#cmd LNSpotLightUpdateHandler_Create $9D4
#cmd LNSpriteSerializeHandler_Create $9DF
#cmd LNSpritePreUpdateHandler_Create $9EA
#cmd LNSpriteUpdateHandler_Create $9F5
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $A00
#cmd LNRaycasterSerializeHandler_Create $A0B
#cmd LNRaycastResultSerializeHandler_Create $A16
#cmd LNWorldRenderViewSerializeHandler_Create $A21
#cmd LNBoxMeshSerializeHandler_Create $A2C
#cmd LNBoxMeshPreUpdateHandler_Create $A37
#cmd LNBoxMeshUpdateHandler_Create $A42
#cmd LNPlaneMeshSerializeHandler_Create $A4D
#cmd LNPlaneMeshPreUpdateHandler_Create $A58
#cmd LNPlaneMeshUpdateHandler_Create $A63
#cmd LNStaticMeshSerializeHandler_Create $A6E
#cmd LNStaticMeshPreUpdateHandler_Create $A79
#cmd LNStaticMeshUpdateHandler_Create $A84
#cmd LNStaticMeshComponentSerializeHandler_Create $A8F
#cmd LNSkinnedMeshComponentSerializeHandler_Create $A9A
#cmd LNCollisionSerializeHandler_Create $AA5
#cmd LNTriggerBodyComponentSerializeHandler_Create $AB0
#cmd LNLevelSerializeHandler_Create $ABB
#cmd LNLevelStartHandler_Create $AC6
#cmd LNLevelStopHandler_Create $AD1
#cmd LNLevelPauseHandler_Create $ADC
#cmd LNLevelResumeHandler_Create $AE7
#cmd LNLevelUpdateHandler_Create $AF2
#cmd LNUIEventArgsSerializeHandler_Create $AFD
#cmd LNUILayoutElementSerializeHandler_Create $B08
#cmd LNUIElementSerializeHandler_Create $B13
#cmd LNUITextBlockSerializeHandler_Create $B1E
#cmd LNUISpriteSerializeHandler_Create $B29
#cmd LNUIIconSerializeHandler_Create $B34
#cmd LNUIMessageTextAreaSerializeHandler_Create $B3F
#cmd LNUILayoutPanelSerializeHandler_Create $B4A
#cmd LNUIBoxLayoutSerializeHandler_Create $B55
#cmd LNUIGridLayoutSerializeHandler_Create $B60
#cmd LNUIControlSerializeHandler_Create $B6B
#cmd LNUIButtonBaseSerializeHandler_Create $B76
#cmd LNUIButtonSerializeHandler_Create $B81
#cmd LNUIWindowSerializeHandler_Create $B8C
#cmd LNUIListItemSerializeHandler_Create $B97
#cmd LNUIListItemsControlSerializeHandler_Create $BA2
#cmd LNUIListBoxItemSerializeHandler_Create $BAD
#cmd LNUIListBoxSerializeHandler_Create $BB8
#cmd LNInterpreterCommandSerializeHandler_Create $BC3
#cmd LNInterpreterCommandListSerializeHandler_Create $BCE
#cmd LNInterpreterSerializeHandler_Create $BD9
#cmd LNInterpreterUpdateWaitHandler_Create $BE5
#cmd LNApplicationSerializeHandler_Create $BF0
#cmd LNApplicationInitHandler_Create $BFB
#cmd LNApplicationUpdateHandler_Create $C06



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
