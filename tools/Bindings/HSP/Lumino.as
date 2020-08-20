
#ifndef __lumino__
#define __lumino__

#regcmd "_hsp3cmdinit@4","LuminoHSP.dll", 6

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

#cmd LNObject_SetPrototype_OnSerialize $4A9
#cmd LNEventConnection_SetPrototype_OnSerialize $4B4
#cmd LNPromiseFailureDelegate_Create $2CD
#cmd LNZVTestDelegate1_Create $2D2
#cmd LNZVTestDelegate2_Create $2D8
#cmd LNZVTestDelegate3_Create $2DD
#cmd LNZVTestEventHandler1_Create $2E2
#cmd LNZVTestEventHandler2_Create $2E7
#cmd LNZVTestPromise1_ThenWith $2EA
#cmd LNZVTestPromise1_CatchWith $2EC
#cmd LNZVTestPromise2_ThenWith $2F0
#cmd LNZVTestPromise2_CatchWith $2F2
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $4BF
#cmd LNZVTestEventArgs1_Create $9E
#cmd LNZVTestEventArgs1_CreateWithValue $9F
#cmd LNZVTestEventArgs1_GetValue $9D
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $4CA
#cmd LNSerializer2_SetPrototype_OnSerialize $4D5
#cmd LNAssetModel_Create $AB
#cmd LNAssetModel_Target $AA
#cmd LNAssetModel_SetPrototype_OnSerialize $4E0
#cmd LNAssets_SaveAssetToLocalFile $AE
#cmd LNAssets_LoadAssetFromLocalFile $B1
#cmd LNAssets_LoadAsset $B3
#cmd LNAssets_ReloadAsset $B5
#cmd LNTexture2DDelegate_Create $316
#cmd LNTexture2DPromise_ThenWith $319
#cmd LNTexture2DPromise_CatchWith $31B
#cmd LNTexture_SetPrototype_OnSerialize $4EB
#cmd LNTexture2D_Create $107
#cmd LNTexture2D_CreateWithFormat $10A
#cmd LNTexture2D_Load $103
#cmd LNTexture2D_LoadEmoji $105
#cmd LNTexture2D_SetPrototype_OnSerialize $4F6
#cmd LNRenderView_SetPrototype_OnSerialize $501
#cmd LNComponent_SetPrototype_OnSerialize $50C
#cmd LNVisualComponent_SetVisible $111
#cmd LNVisualComponent_IsVisible $113
#cmd LNVisualComponent_SetPrototype_OnSerialize $517
#cmd LNSpriteComponent_SetTexture $115
#cmd LNSpriteComponent_SetPrototype_OnSerialize $522
#cmd LNWorld_Add $118
#cmd LNWorld_SetPrototype_OnSerialize $52D
#cmd LNComponentList_GetLength $3AA
#cmd LNComponentList_GetItem $3AC
#cmd LNComponentList_SetPrototype_OnSerialize $538
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
#cmd LNWorldObject_GetComponents $142
#cmd LNWorldObject_SetPrototype_OnSerialize $543
#cmd LNWorldObject_SetPrototype_OnUpdate $54E
#cmd LNVisualObject_SetVisible $146
#cmd LNVisualObject_IsVisible $148
#cmd LNVisualObject_SetPrototype_OnSerialize $559
#cmd LNVisualObject_SetPrototype_OnUpdate $564
#cmd LNCamera_SetPrototype_OnSerialize $56F
#cmd LNCamera_SetPrototype_OnUpdate $57A
#cmd LNDirectionalLight_Create $15A
#cmd LNDirectionalLight_CreateWithColor $15B
#cmd LNDirectionalLight_SetEnabled $14B
#cmd LNDirectionalLight_IsEnabled $14D
#cmd LNDirectionalLight_SetColor $14E
#cmd LNDirectionalLight_GetColor $150
#cmd LNDirectionalLight_SetIntensity $151
#cmd LNDirectionalLight_GetIntensity $153
#cmd LNDirectionalLight_SetShadowEffectiveDistance $154
#cmd LNDirectionalLight_GetShadowEffectiveDistance $156
#cmd LNDirectionalLight_SetShadowEffectiveDepth $157
#cmd LNDirectionalLight_GetShadowEffectiveDepth $159
#cmd LNDirectionalLight_SetPrototype_OnSerialize $585
#cmd LNDirectionalLight_SetPrototype_OnUpdate $590
#cmd LNPointLight_Create $16D
#cmd LNPointLight_CreateWithColorAndRange $16E
#cmd LNPointLight_SetEnabled $15E
#cmd LNPointLight_IsEnabled $160
#cmd LNPointLight_SetColor $161
#cmd LNPointLight_GetColor $163
#cmd LNPointLight_SetIntensity $164
#cmd LNPointLight_GetIntensity $166
#cmd LNPointLight_SetRange $167
#cmd LNPointLight_GetRange $169
#cmd LNPointLight_SetAttenuation $16A
#cmd LNPointLight_GetAttenuation $16C
#cmd LNPointLight_SetPrototype_OnSerialize $59B
#cmd LNPointLight_SetPrototype_OnUpdate $5A6
#cmd LNSpotLight_Create $187
#cmd LNSpotLight_CreateWithColorAndRange $188
#cmd LNSpotLight_SetEnabled $172
#cmd LNSpotLight_IsEnabled $174
#cmd LNSpotLight_SetColor $175
#cmd LNSpotLight_GetColor $177
#cmd LNSpotLight_SetIntensity $178
#cmd LNSpotLight_GetIntensity $17A
#cmd LNSpotLight_SetRange $17B
#cmd LNSpotLight_GetRange $17D
#cmd LNSpotLight_SetAttenuation $17E
#cmd LNSpotLight_GetAttenuation $180
#cmd LNSpotLight_SetAngle $181
#cmd LNSpotLight_GetAngle $183
#cmd LNSpotLight_SetPenumbra $184
#cmd LNSpotLight_GetPenumbra $186
#cmd LNSpotLight_SetPrototype_OnSerialize $5B1
#cmd LNSpotLight_SetPrototype_OnUpdate $5BC
#cmd LNTestDelegate_Create $427
#cmd LNSprite_Create $19E
#cmd LNSprite_CreateWithTexture $19F
#cmd LNSprite_CreateWithTextureAndSize $1A1
#cmd LNSprite_SetTexture $190
#cmd LNSprite_SetSize $192
#cmd LNSprite_SetSizeWH $194
#cmd LNSprite_SetSourceRectXYWH $197
#cmd LNSprite_SetCallerTest $19C
#cmd LNSprite_SetPrototype_OnSerialize $5C7
#cmd LNSprite_SetPrototype_OnUpdate $5D2
#cmd LNCameraOrbitControlComponent_Create $1A6
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $5DD
#cmd LNRaycaster_FromScreen $1A8
#cmd LNRaycaster_IntersectPlane $1AA
#cmd LNRaycaster_SetPrototype_OnSerialize $5E8
#cmd LNRaycastResult_GetPoint $1AF
#cmd LNRaycastResult_SetPrototype_OnSerialize $5F3
#cmd LNWorldRenderView_SetGuideGridEnabled $1B1
#cmd LNWorldRenderView_GetGuideGridEnabled $1B3
#cmd LNWorldRenderView_SetPrototype_OnSerialize $5FE
#cmd LNBoxMesh_Create $1B5
#cmd LNBoxMesh_SetPrototype_OnSerialize $609
#cmd LNBoxMesh_SetPrototype_OnUpdate $614
#cmd LNPlaneMesh_Create $1B7
#cmd LNPlaneMesh_SetPrototype_OnSerialize $61F
#cmd LNPlaneMesh_SetPrototype_OnUpdate $62A
#cmd LNUIEventArgs_Sender $1B9
#cmd LNUIEventArgs_SetPrototype_OnSerialize $635
#cmd LNUIGeneralEventHandler_Create $446
#cmd LNUIEventHandler_Create $44B
#cmd LNUILayoutElement_SetPrototype_OnSerialize $640
#cmd LNUIElement_SetMargin $1CB
#cmd LNUIElement_GetMargin $1CD
#cmd LNUIElement_SetPadding $1CE
#cmd LNUIElement_GetPadding $1D0
#cmd LNUIElement_SetHAlignment $1D1
#cmd LNUIElement_GetHAlignment $1D3
#cmd LNUIElement_SetVAlignment $1D4
#cmd LNUIElement_GetVAlignment $1D6
#cmd LNUIElement_SetAlignments $1D7
#cmd LNUIElement_SetPosition $1DA
#cmd LNUIElement_SetPositionXYZ $1DC
#cmd LNUIElement_GetPosition $1E0
#cmd LNUIElement_SetRotation $1E1
#cmd LNUIElement_SetEulerAngles $1E3
#cmd LNUIElement_GetRotation $1E7
#cmd LNUIElement_SetScale $1E8
#cmd LNUIElement_SetScaleS $1EA
#cmd LNUIElement_SetScaleXY $1EC
#cmd LNUIElement_GetScale $1EF
#cmd LNUIElement_SetCenterPoint $1F0
#cmd LNUIElement_SetCenterPointXYZ $1F2
#cmd LNUIElement_GetCenterPoint $1F6
#cmd LNUIElement_AddChild $1F7
#cmd LNUIElement_SetPrototype_OnSerialize $64B
#cmd LNUITextBlock_Create $1FA
#cmd LNUITextBlock_CreateWithText $1FB
#cmd LNUITextBlock_SetPrototype_OnSerialize $656
#cmd LNUISprite_Create $208
#cmd LNUISprite_CreateWithTexture $209
#cmd LNUISprite_SetTexture $1FE
#cmd LNUISprite_SetSourceRect $200
#cmd LNUISprite_SetSourceRectXYWH $202
#cmd LNUISprite_GetSourceRect $207
#cmd LNUISprite_SetPrototype_OnSerialize $661
#cmd LNInput_Pressed $20C
#cmd LNInput_Triggered $20E
#cmd LNInput_TriggeredOff $210
#cmd LNInput_Repeated $212
#cmd LNInput_GetAxisValue $214
#cmd LNInput_ClearAllBindings $216
#cmd LNMouse_Pressed $218
#cmd LNMouse_Triggered $21A
#cmd LNMouse_TriggeredOff $21C
#cmd LNMouse_Repeated $21E
#cmd LNMouse_Position $220
#cmd LNEngineSettings_SetMainWindowSize $222
#cmd LNEngineSettings_SetMainWorldViewSize $225
#cmd LNEngineSettings_SetMainWindowTitle $228
#cmd LNEngineSettings_AddAssetDirectory $22A
#cmd LNEngineSettings_AddAssetArchive $22C
#cmd LNEngineSettings_SetFrameRate $22F
#cmd LNEngineSettings_SetDebugToolEnabled $231
#cmd LNEngineSettings_SetEngineLogEnabled $233
#cmd LNEngineSettings_SetEngineLogFilePath $235
#cmd LNEngine_Initialize $238
#cmd LNEngine_Finalize $239
#cmd LNEngine_Update $23A
#cmd LNEngine_Run $23B
#cmd LNEngine_Time $23D
#cmd LNEngine_GetCamera $23E
#cmd LNEngine_GetLight $23F
#cmd LNEngine_GetRenderView $240
#cmd LNApplication_Create $245
#cmd LNApplication_OnInit $242
#cmd LNApplication_OnUpdate $243
#cmd LNApplication_World $244
#cmd LNApplication_SetPrototype_OnSerialize $66C
#cmd LNApplication_SetPrototype_OnInit $677
#cmd LNApplication_SetPrototype_OnUpdate $682
#cmd LNDebug_Print $247
#cmd LNDebug_PrintWithTime $249
#cmd LNDebug_PrintWithTimeAndColor $24C
#cmd LNObjectSerializeHandler_Create $4A7
#cmd LNEventConnectionSerializeHandler_Create $4B2
#cmd LNZVTestClass1SerializeHandler_Create $4BD
#cmd LNZVTestEventArgs1SerializeHandler_Create $4C8
#cmd LNSerializer2SerializeHandler_Create $4D3
#cmd LNAssetModelSerializeHandler_Create $4DE
#cmd LNTextureSerializeHandler_Create $4E9
#cmd LNTexture2DSerializeHandler_Create $4F4
#cmd LNRenderViewSerializeHandler_Create $4FF
#cmd LNComponentSerializeHandler_Create $50A
#cmd LNVisualComponentSerializeHandler_Create $515
#cmd LNSpriteComponentSerializeHandler_Create $520
#cmd LNWorldSerializeHandler_Create $52B
#cmd LNComponentListSerializeHandler_Create $536
#cmd LNWorldObjectSerializeHandler_Create $541
#cmd LNWorldObjectUpdateHandler_Create $54C
#cmd LNVisualObjectSerializeHandler_Create $557
#cmd LNVisualObjectUpdateHandler_Create $562
#cmd LNCameraSerializeHandler_Create $56D
#cmd LNCameraUpdateHandler_Create $578
#cmd LNDirectionalLightSerializeHandler_Create $583
#cmd LNDirectionalLightUpdateHandler_Create $58E
#cmd LNPointLightSerializeHandler_Create $599
#cmd LNPointLightUpdateHandler_Create $5A4
#cmd LNSpotLightSerializeHandler_Create $5AF
#cmd LNSpotLightUpdateHandler_Create $5BA
#cmd LNSpriteSerializeHandler_Create $5C5
#cmd LNSpriteUpdateHandler_Create $5D0
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $5DB
#cmd LNRaycasterSerializeHandler_Create $5E6
#cmd LNRaycastResultSerializeHandler_Create $5F1
#cmd LNWorldRenderViewSerializeHandler_Create $5FC
#cmd LNBoxMeshSerializeHandler_Create $607
#cmd LNBoxMeshUpdateHandler_Create $612
#cmd LNPlaneMeshSerializeHandler_Create $61D
#cmd LNPlaneMeshUpdateHandler_Create $628
#cmd LNUIEventArgsSerializeHandler_Create $633
#cmd LNUILayoutElementSerializeHandler_Create $63E
#cmd LNUIElementSerializeHandler_Create $649
#cmd LNUITextBlockSerializeHandler_Create $654
#cmd LNUISpriteSerializeHandler_Create $65F
#cmd LNApplicationSerializeHandler_Create $66A
#cmd LNApplicationInitHandler_Create $675
#cmd LNApplicationUpdateHandler_Create $680



#cmd ln_args $1

#endif // __lumino__
