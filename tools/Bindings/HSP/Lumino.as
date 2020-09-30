
#ifndef __lumino__
#define __lumino__

#ifdef LUMINO_DEBUG
    #regcmd "_hsp3cmdinit@4","LuminoHSPd.dll", 6
#else
    #regcmd "_hsp3cmdinit@4","LuminoHSP.dll", 6
#endif

#const global LN_TRUE 1
#const global LN_FALSE 0

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
#const global LN_VALIGNMENT_TOP 0
#const global LN_VALIGNMENT_CENTER 1
#const global LN_VALIGNMENT_BOTTOM 2
#const global LN_VALIGNMENT_STRETCH 3
#const global LN_HALIGNMENT_LEFT 0
#const global LN_HALIGNMENT_CENTER 1
#const global LN_HALIGNMENT_RIGHT 2
#const global LN_HALIGNMENT_STRETCH 3

#cmd LNVector3 $2B
#cmd LNVector4 $3D
#cmd LNQuaternion $48
#cmd LNMatrix $56
#cmd LNColor $CE
#cmd LNPoint $D9
#cmd LNSize $E0
#cmd LNRect $E7
#cmd LNThickness $F6

#cmd LNObject_SetPrototype_OnSerialize $4F4
#cmd LNEventConnection_SetPrototype_OnSerialize $4FF
#cmd LNPromiseFailureDelegate_Create $2EF
#cmd LNZVTestDelegate1_Create $2F4
#cmd LNZVTestDelegate2_Create $2FA
#cmd LNZVTestDelegate3_Create $2FF
#cmd LNZVTestEventHandler1_Create $304
#cmd LNZVTestEventHandler2_Create $309
#cmd LNZVTestPromise1_ThenWith $30C
#cmd LNZVTestPromise1_CatchWith $30E
#cmd LNZVTestPromise2_ThenWith $312
#cmd LNZVTestPromise2_CatchWith $314
#cmd LNZVTestClass1_Create $9B
#cmd LNZVTestClass1_SetTestDelegate1 $85
#cmd LNZVTestClass1_SetTestDelegate2 $87
#cmd LNZVTestClass1_SetTestDelegate3 $89
#cmd LNZVTestClass1_CallTestDelegate1 $8B
#cmd LNZVTestClass1_CallTestDelegate2 $8D
#cmd LNZVTestClass1_CallTestDelegate3 $90
#cmd LNZVTestClass1_LoadAsync $91
#cmd LNZVTestClass1_ExecuteAsync $93
#cmd LNZVTestClass1_GetFilePath $94
#cmd LNZVTestClass1_ConnectOnEvent1 $95
#cmd LNZVTestClass1_RaiseEvent1 $97
#cmd LNZVTestClass1_ConnectOnEvent2 $98
#cmd LNZVTestClass1_RaiseEvent2 $9A
#cmd LNZVTestClass1_SetPrototype_OnSerialize $50A
#cmd LNZVTestEventArgs1_Create $9E
#cmd LNZVTestEventArgs1_CreateWithValue $9F
#cmd LNZVTestEventArgs1_GetValue $9D
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $515
#cmd LNSerializer2_SetPrototype_OnSerialize $520
#cmd LNAssetModel_Create $AB
#cmd LNAssetModel_Target $AA
#cmd LNAssetModel_SetPrototype_OnSerialize $52B
#cmd LNAssets_SaveAssetToLocalFile $AE
#cmd LNAssets_LoadAssetFromLocalFile $B1
#cmd LNAssets_LoadAsset $B3
#cmd LNAssets_ReloadAsset $B5
#cmd LNTexture2DDelegate_Create $338
#cmd LNTexture2DPromise_ThenWith $33B
#cmd LNTexture2DPromise_CatchWith $33D
#cmd LNTexture_SetPrototype_OnSerialize $536
#cmd LNTexture2D_Create $107
#cmd LNTexture2D_CreateWithFormat $10A
#cmd LNTexture2D_Load $103
#cmd LNTexture2D_LoadEmoji $105
#cmd LNTexture2D_SetPrototype_OnSerialize $541
#cmd LNRenderView_SetPrototype_OnSerialize $54C
#cmd LNComponent_SetPrototype_OnSerialize $557
#cmd LNVisualComponent_SetVisible $111
#cmd LNVisualComponent_IsVisible $113
#cmd LNVisualComponent_SetPrototype_OnSerialize $562
#cmd LNSpriteComponent_SetTexture $115
#cmd LNSpriteComponent_SetPrototype_OnSerialize $56D
#cmd LNWorld_Add $118
#cmd LNWorld_SetPrototype_OnSerialize $578
#cmd LNComponentList_GetLength $3CC
#cmd LNComponentList_GetItem $3CE
#cmd LNComponentList_SetPrototype_OnSerialize $583
#cmd LNWorldObject_SetPosition $11C
#cmd LNWorldObject_SetPositionXYZ $11E
#cmd LNWorldObject_GetPosition $122
#cmd LNWorldObject_SetRotationQuaternion $123
#cmd LNWorldObject_SetRotation $125
#cmd LNWorldObject_GetRotation $129
#cmd LNWorldObject_SetScale $12A
#cmd LNWorldObject_SetScaleS $12C
#cmd LNWorldObject_SetScaleXYZ $12E
#cmd LNWorldObject_GetScale $132
#cmd LNWorldObject_SetCenterPoint $133
#cmd LNWorldObject_SetCenterPointXYZ $135
#cmd LNWorldObject_GetCenterPoint $139
#cmd LNWorldObject_LookAt $13A
#cmd LNWorldObject_LookAtXYZ $13C
#cmd LNWorldObject_AddComponent $140
#cmd LNWorldObject_RemoveComponent $142
#cmd LNWorldObject_GetComponents $144
#cmd LNWorldObject_SetPrototype_OnSerialize $58E
#cmd LNWorldObject_SetPrototype_OnUpdate $599
#cmd LNVisualObject_SetVisible $148
#cmd LNVisualObject_IsVisible $14A
#cmd LNVisualObject_SetPrototype_OnSerialize $5A4
#cmd LNVisualObject_SetPrototype_OnUpdate $5AF
#cmd LNCamera_SetPrototype_OnSerialize $5BA
#cmd LNCamera_SetPrototype_OnUpdate $5C5
#cmd LNEnvironmentLight_SetEnabled $14D
#cmd LNEnvironmentLight_IsEnabled $14F
#cmd LNEnvironmentLight_SetColor $150
#cmd LNEnvironmentLight_GetColor $152
#cmd LNEnvironmentLight_SetAmbientColor $153
#cmd LNEnvironmentLight_GetAmbientColor $155
#cmd LNEnvironmentLight_GetSkyColor $156
#cmd LNEnvironmentLight_SetSkyColor $157
#cmd LNEnvironmentLight_GetGroundColor $159
#cmd LNEnvironmentLight_SetGroundColor $15A
#cmd LNEnvironmentLight_SetIntensity $15C
#cmd LNEnvironmentLight_GetIntensity $15E
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $15F
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $161
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $162
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $164
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $5D0
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $5DB
#cmd LNDirectionalLight_Create $175
#cmd LNDirectionalLight_CreateWithColor $176
#cmd LNDirectionalLight_SetEnabled $166
#cmd LNDirectionalLight_IsEnabled $168
#cmd LNDirectionalLight_SetColor $169
#cmd LNDirectionalLight_GetColor $16B
#cmd LNDirectionalLight_SetIntensity $16C
#cmd LNDirectionalLight_GetIntensity $16E
#cmd LNDirectionalLight_SetShadowEffectiveDistance $16F
#cmd LNDirectionalLight_GetShadowEffectiveDistance $171
#cmd LNDirectionalLight_SetShadowEffectiveDepth $172
#cmd LNDirectionalLight_GetShadowEffectiveDepth $174
#cmd LNDirectionalLight_SetPrototype_OnSerialize $5E6
#cmd LNDirectionalLight_SetPrototype_OnUpdate $5F1
#cmd LNPointLight_Create $188
#cmd LNPointLight_CreateWithColorAndRange $189
#cmd LNPointLight_SetEnabled $179
#cmd LNPointLight_IsEnabled $17B
#cmd LNPointLight_SetColor $17C
#cmd LNPointLight_GetColor $17E
#cmd LNPointLight_SetIntensity $17F
#cmd LNPointLight_GetIntensity $181
#cmd LNPointLight_SetRange $182
#cmd LNPointLight_GetRange $184
#cmd LNPointLight_SetAttenuation $185
#cmd LNPointLight_GetAttenuation $187
#cmd LNPointLight_SetPrototype_OnSerialize $5FC
#cmd LNPointLight_SetPrototype_OnUpdate $607
#cmd LNSpotLight_Create $1A2
#cmd LNSpotLight_CreateWithColorAndRange $1A3
#cmd LNSpotLight_SetEnabled $18D
#cmd LNSpotLight_IsEnabled $18F
#cmd LNSpotLight_SetColor $190
#cmd LNSpotLight_GetColor $192
#cmd LNSpotLight_SetIntensity $193
#cmd LNSpotLight_GetIntensity $195
#cmd LNSpotLight_SetRange $196
#cmd LNSpotLight_GetRange $198
#cmd LNSpotLight_SetAttenuation $199
#cmd LNSpotLight_GetAttenuation $19B
#cmd LNSpotLight_SetAngle $19C
#cmd LNSpotLight_GetAngle $19E
#cmd LNSpotLight_SetPenumbra $19F
#cmd LNSpotLight_GetPenumbra $1A1
#cmd LNSpotLight_SetPrototype_OnSerialize $612
#cmd LNSpotLight_SetPrototype_OnUpdate $61D
#cmd LNTestDelegate_Create $46C
#cmd LNSprite_Create $1B9
#cmd LNSprite_CreateWithTexture $1BA
#cmd LNSprite_CreateWithTextureAndSize $1BC
#cmd LNSprite_SetTexture $1AB
#cmd LNSprite_SetSize $1AD
#cmd LNSprite_SetSizeWH $1AF
#cmd LNSprite_SetSourceRectXYWH $1B2
#cmd LNSprite_SetCallerTest $1B7
#cmd LNSprite_SetPrototype_OnSerialize $628
#cmd LNSprite_SetPrototype_OnUpdate $633
#cmd LNCameraOrbitControlComponent_Create $1C1
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $63E
#cmd LNRaycaster_FromScreen $1C3
#cmd LNRaycaster_IntersectPlane $1C5
#cmd LNRaycaster_SetPrototype_OnSerialize $649
#cmd LNRaycastResult_GetPoint $1CA
#cmd LNRaycastResult_SetPrototype_OnSerialize $654
#cmd LNWorldRenderView_SetGuideGridEnabled $1CC
#cmd LNWorldRenderView_GetGuideGridEnabled $1CE
#cmd LNWorldRenderView_SetPrototype_OnSerialize $65F
#cmd LNBoxMesh_Create $1D0
#cmd LNBoxMesh_SetPrototype_OnSerialize $66A
#cmd LNBoxMesh_SetPrototype_OnUpdate $675
#cmd LNPlaneMesh_Create $1D2
#cmd LNPlaneMesh_SetPrototype_OnSerialize $680
#cmd LNPlaneMesh_SetPrototype_OnUpdate $68B
#cmd LNLevel_Create $1D9
#cmd LNLevel_SetPrototype_OnSerialize $696
#cmd LNLevel_SetPrototype_OnStart $6A1
#cmd LNLevel_SetPrototype_OnStop $6AC
#cmd LNLevel_SetPrototype_OnPause $6B7
#cmd LNLevel_SetPrototype_OnResume $6C2
#cmd LNLevel_SetPrototype_OnUpdate $6CD
#cmd LNUIEventArgs_Sender $1DB
#cmd LNUIEventArgs_SetPrototype_OnSerialize $6D8
#cmd LNUIGeneralEventHandler_Create $491
#cmd LNUIEventHandler_Create $496
#cmd LNUILayoutElement_SetPrototype_OnSerialize $6E3
#cmd LNUIElement_SetMargin $1ED
#cmd LNUIElement_GetMargin $1EF
#cmd LNUIElement_SetPadding $1F0
#cmd LNUIElement_GetPadding $1F2
#cmd LNUIElement_SetHAlignment $1F3
#cmd LNUIElement_GetHAlignment $1F5
#cmd LNUIElement_SetVAlignment $1F6
#cmd LNUIElement_GetVAlignment $1F8
#cmd LNUIElement_SetAlignments $1F9
#cmd LNUIElement_SetPosition $1FC
#cmd LNUIElement_SetPositionXYZ $1FE
#cmd LNUIElement_GetPosition $202
#cmd LNUIElement_SetRotation $203
#cmd LNUIElement_SetEulerAngles $205
#cmd LNUIElement_GetRotation $209
#cmd LNUIElement_SetScale $20A
#cmd LNUIElement_SetScaleS $20C
#cmd LNUIElement_SetScaleXY $20E
#cmd LNUIElement_GetScale $211
#cmd LNUIElement_SetCenterPoint $212
#cmd LNUIElement_SetCenterPointXYZ $214
#cmd LNUIElement_GetCenterPoint $218
#cmd LNUIElement_AddChild $219
#cmd LNUIElement_SetPrototype_OnSerialize $6EE
#cmd LNUITextBlock_Create $21C
#cmd LNUITextBlock_CreateWithText $21D
#cmd LNUITextBlock_SetPrototype_OnSerialize $6F9
#cmd LNUISprite_Create $22A
#cmd LNUISprite_CreateWithTexture $22B
#cmd LNUISprite_SetTexture $220
#cmd LNUISprite_SetSourceRect $222
#cmd LNUISprite_SetSourceRectXYWH $224
#cmd LNUISprite_GetSourceRect $229
#cmd LNUISprite_SetPrototype_OnSerialize $704
#cmd LNInput_Pressed $22E
#cmd LNInput_Triggered $230
#cmd LNInput_TriggeredOff $232
#cmd LNInput_Repeated $234
#cmd LNInput_GetAxisValue $236
#cmd LNInput_ClearAllBindings $238
#cmd LNMouse_Pressed $23A
#cmd LNMouse_Triggered $23C
#cmd LNMouse_TriggeredOff $23E
#cmd LNMouse_Repeated $240
#cmd LNMouse_Position $242
#cmd LNEngineSettings_SetMainWindowSize $244
#cmd LNEngineSettings_SetMainWorldViewSize $247
#cmd LNEngineSettings_SetMainWindowTitle $24A
#cmd LNEngineSettings_AddAssetDirectory $24C
#cmd LNEngineSettings_AddAssetArchive $24E
#cmd LNEngineSettings_SetFrameRate $251
#cmd LNEngineSettings_SetDebugToolEnabled $253
#cmd LNEngineSettings_SetEngineLogEnabled $255
#cmd LNEngineSettings_SetEngineLogFilePath $257
#cmd LNEngine_Initialize $25A
#cmd LNEngine_Finalize $25B
#cmd LNEngine_Update $25C
#cmd LNEngine_Run $25D
#cmd LNEngine_Time $25F
#cmd LNEngine_GetCamera $260
#cmd LNEngine_GetMainLight $261
#cmd LNEngine_GetRenderView $262
#cmd LNApplication_Create $267
#cmd LNApplication_OnInit $264
#cmd LNApplication_OnUpdate $265
#cmd LNApplication_World $266
#cmd LNApplication_SetPrototype_OnSerialize $70F
#cmd LNApplication_SetPrototype_OnInit $71A
#cmd LNApplication_SetPrototype_OnUpdate $725
#cmd LNDebug_Print $269
#cmd LNDebug_PrintWithTime $26B
#cmd LNDebug_PrintWithTimeAndColor $26E
#cmd LNObjectSerializeHandler_Create $4F2
#cmd LNEventConnectionSerializeHandler_Create $4FD
#cmd LNZVTestClass1SerializeHandler_Create $508
#cmd LNZVTestEventArgs1SerializeHandler_Create $513
#cmd LNSerializer2SerializeHandler_Create $51E
#cmd LNAssetModelSerializeHandler_Create $529
#cmd LNTextureSerializeHandler_Create $534
#cmd LNTexture2DSerializeHandler_Create $53F
#cmd LNRenderViewSerializeHandler_Create $54A
#cmd LNComponentSerializeHandler_Create $555
#cmd LNVisualComponentSerializeHandler_Create $560
#cmd LNSpriteComponentSerializeHandler_Create $56B
#cmd LNWorldSerializeHandler_Create $576
#cmd LNComponentListSerializeHandler_Create $581
#cmd LNWorldObjectSerializeHandler_Create $58C
#cmd LNWorldObjectUpdateHandler_Create $597
#cmd LNVisualObjectSerializeHandler_Create $5A2
#cmd LNVisualObjectUpdateHandler_Create $5AD
#cmd LNCameraSerializeHandler_Create $5B8
#cmd LNCameraUpdateHandler_Create $5C3
#cmd LNEnvironmentLightSerializeHandler_Create $5CE
#cmd LNEnvironmentLightUpdateHandler_Create $5D9
#cmd LNDirectionalLightSerializeHandler_Create $5E4
#cmd LNDirectionalLightUpdateHandler_Create $5EF
#cmd LNPointLightSerializeHandler_Create $5FA
#cmd LNPointLightUpdateHandler_Create $605
#cmd LNSpotLightSerializeHandler_Create $610
#cmd LNSpotLightUpdateHandler_Create $61B
#cmd LNSpriteSerializeHandler_Create $626
#cmd LNSpriteUpdateHandler_Create $631
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $63C
#cmd LNRaycasterSerializeHandler_Create $647
#cmd LNRaycastResultSerializeHandler_Create $652
#cmd LNWorldRenderViewSerializeHandler_Create $65D
#cmd LNBoxMeshSerializeHandler_Create $668
#cmd LNBoxMeshUpdateHandler_Create $673
#cmd LNPlaneMeshSerializeHandler_Create $67E
#cmd LNPlaneMeshUpdateHandler_Create $689
#cmd LNLevelSerializeHandler_Create $694
#cmd LNLevelStartHandler_Create $69F
#cmd LNLevelStopHandler_Create $6AA
#cmd LNLevelPauseHandler_Create $6B5
#cmd LNLevelResumeHandler_Create $6C0
#cmd LNLevelUpdateHandler_Create $6CB
#cmd LNUIEventArgsSerializeHandler_Create $6D6
#cmd LNUILayoutElementSerializeHandler_Create $6E1
#cmd LNUIElementSerializeHandler_Create $6EC
#cmd LNUITextBlockSerializeHandler_Create $6F7
#cmd LNUISpriteSerializeHandler_Create $702
#cmd LNApplicationSerializeHandler_Create $70D
#cmd LNApplicationInitHandler_Create $718
#cmd LNApplicationUpdateHandler_Create $723



#cmd ln_args $1

#endif // __lumino__
