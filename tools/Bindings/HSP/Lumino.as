
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

#cmd LnVector3 $2B
#cmd LnVector4 $3D
#cmd LnQuaternion $48
#cmd LnMatrix $56
#cmd LnColor $CE
#cmd LnPoint $D9
#cmd LnSize $E0
#cmd LnRect $E7
#cmd LnThickness $F6

#cmd LnObject_SetPrototype_SetPrototype_OnSerialize $4A7
#cmd LnEventConnection_SetPrototype_SetPrototype_OnSerialize $4B2
#cmd LnPromiseFailureDelegate_Create $2CB
#cmd LnZVTestDelegate1_Create $2D0
#cmd LnZVTestDelegate2_Create $2D6
#cmd LnZVTestDelegate3_Create $2DB
#cmd LnZVTestEventHandler1_Create $2E0
#cmd LnZVTestEventHandler2_Create $2E5
#cmd LnZVTestPromise1_ThenWith $2E8
#cmd LnZVTestPromise1_CatchWith $2EA
#cmd LnZVTestPromise2_ThenWith $2EE
#cmd LnZVTestPromise2_CatchWith $2F0
#cmd LnZVTestClass1_Create $9B
#cmd LnZVTestClass1_SetTestDelegate1 $85
#cmd LnZVTestClass1_SetTestDelegate2 $87
#cmd LnZVTestClass1_SetTestDelegate3 $89
#cmd LnZVTestClass1_CallTestDelegate1 $8B
#cmd LnZVTestClass1_CallTestDelegate2 $8D
#cmd LnZVTestClass1_CallTestDelegate3 $90
#cmd LnZVTestClass1_LoadAsyncA $91
#cmd LnZVTestClass1_ExecuteAsync $93
#cmd LnZVTestClass1_GetFilePathA $94
#cmd LnZVTestClass1_ConnectOnEvent1 $95
#cmd LnZVTestClass1_RaiseEvent1 $97
#cmd LnZVTestClass1_ConnectOnEvent2 $98
#cmd LnZVTestClass1_RaiseEvent2 $9A
#cmd LnZVTestClass1_SetPrototype_SetPrototype_OnSerialize $4BD
#cmd LnZVTestEventArgs1_Create $9E
#cmd LnZVTestEventArgs1_CreateWithValue $9F
#cmd LnZVTestEventArgs1_GetValue $9D
#cmd LnZVTestEventArgs1_SetPrototype_SetPrototype_OnSerialize $4C8
#cmd LnSerializer2_SetPrototype_SetPrototype_OnSerialize $4D3
#cmd LnAssetModel_Create $AB
#cmd LnAssetModel_Target $AA
#cmd LnAssetModel_SetPrototype_SetPrototype_OnSerialize $4DE
#cmd LnAssets_SaveAssetToLocalFileA $AE
#cmd LnAssets_LoadAssetFromLocalFileA $B1
#cmd LnAssets_LoadAssetA $B3
#cmd LnAssets_ReloadAssetA $B5
#cmd LnTexture2DDelegate_Create $314
#cmd LnTexture2DPromise_ThenWith $317
#cmd LnTexture2DPromise_CatchWith $319
#cmd LnTexture_SetPrototype_SetPrototype_OnSerialize $4E9
#cmd LnTexture2D_Create $107
#cmd LnTexture2D_CreateWithFormat $10A
#cmd LnTexture2D_LoadA $103
#cmd LnTexture2D_LoadEmojiA $105
#cmd LnTexture2D_SetPrototype_SetPrototype_OnSerialize $4F4
#cmd LnRenderView_SetPrototype_SetPrototype_OnSerialize $4FF
#cmd LnComponent_SetPrototype_SetPrototype_OnSerialize $50A
#cmd LnVisualComponent_SetVisible $111
#cmd LnVisualComponent_IsVisible $113
#cmd LnVisualComponent_SetPrototype_SetPrototype_OnSerialize $515
#cmd LnSpriteComponent_SetTexture $115
#cmd LnSpriteComponent_SetPrototype_SetPrototype_OnSerialize $520
#cmd LnWorld_Add $118
#cmd LnWorld_SetPrototype_SetPrototype_OnSerialize $52B
#cmd LnComponentList_GetLength $3A8
#cmd LnComponentList_GetItem $3AA
#cmd LnComponentList_SetPrototype_SetPrototype_OnSerialize $536
#cmd LnWorldObject_SetPosition $11C
#cmd LnWorldObject_SetPositionXYZ $11E
#cmd LnWorldObject_GetPosition $122
#cmd LnWorldObject_SetRotationQuaternion $123
#cmd LnWorldObject_SetRotation $125
#cmd LnWorldObject_GetRotation $129
#cmd LnWorldObject_SetScale $12A
#cmd LnWorldObject_SetScaleS $12C
#cmd LnWorldObject_SetScaleXYZ $12E
#cmd LnWorldObject_GetScale $132
#cmd LnWorldObject_SetCenterPoint $133
#cmd LnWorldObject_SetCenterPointXYZ $135
#cmd LnWorldObject_GetCenterPoint $139
#cmd LnWorldObject_LookAt $13A
#cmd LnWorldObject_LookAtXYZ $13C
#cmd LnWorldObject_AddComponent $140
#cmd LnWorldObject_GetComponents $142
#cmd LnWorldObject_SetPrototype_SetPrototype_OnSerialize $541
#cmd LnWorldObject_SetPrototype_SetPrototype_OnUpdate $54C
#cmd LnVisualObject_SetVisible $146
#cmd LnVisualObject_IsVisible $148
#cmd LnVisualObject_SetPrototype_SetPrototype_OnSerialize $557
#cmd LnVisualObject_SetPrototype_SetPrototype_OnUpdate $562
#cmd LnCamera_SetPrototype_SetPrototype_OnSerialize $56D
#cmd LnCamera_SetPrototype_SetPrototype_OnUpdate $578
#cmd LnDirectionalLight_Create $15A
#cmd LnDirectionalLight_CreateWithColor $15B
#cmd LnDirectionalLight_SetEnabled $14B
#cmd LnDirectionalLight_IsEnabled $14D
#cmd LnDirectionalLight_SetColor $14E
#cmd LnDirectionalLight_GetColor $150
#cmd LnDirectionalLight_SetIntensity $151
#cmd LnDirectionalLight_GetIntensity $153
#cmd LnDirectionalLight_SetShadowEffectiveDistance $154
#cmd LnDirectionalLight_GetShadowEffectiveDistance $156
#cmd LnDirectionalLight_SetShadowEffectiveDepth $157
#cmd LnDirectionalLight_GetShadowEffectiveDepth $159
#cmd LnDirectionalLight_SetPrototype_SetPrototype_OnSerialize $583
#cmd LnDirectionalLight_SetPrototype_SetPrototype_OnUpdate $58E
#cmd LnPointLight_Create $16D
#cmd LnPointLight_CreateWithColorAndRange $16E
#cmd LnPointLight_SetEnabled $15E
#cmd LnPointLight_IsEnabled $160
#cmd LnPointLight_SetColor $161
#cmd LnPointLight_GetColor $163
#cmd LnPointLight_SetIntensity $164
#cmd LnPointLight_GetIntensity $166
#cmd LnPointLight_SetRange $167
#cmd LnPointLight_GetRange $169
#cmd LnPointLight_SetAttenuation $16A
#cmd LnPointLight_GetAttenuation $16C
#cmd LnPointLight_SetPrototype_SetPrototype_OnSerialize $599
#cmd LnPointLight_SetPrototype_SetPrototype_OnUpdate $5A4
#cmd LnSpotLight_Create $187
#cmd LnSpotLight_CreateWithColorAndRange $188
#cmd LnSpotLight_SetEnabled $172
#cmd LnSpotLight_IsEnabled $174
#cmd LnSpotLight_SetColor $175
#cmd LnSpotLight_GetColor $177
#cmd LnSpotLight_SetIntensity $178
#cmd LnSpotLight_GetIntensity $17A
#cmd LnSpotLight_SetRange $17B
#cmd LnSpotLight_GetRange $17D
#cmd LnSpotLight_SetAttenuation $17E
#cmd LnSpotLight_GetAttenuation $180
#cmd LnSpotLight_SetAngle $181
#cmd LnSpotLight_GetAngle $183
#cmd LnSpotLight_SetPenumbra $184
#cmd LnSpotLight_GetPenumbra $186
#cmd LnSpotLight_SetPrototype_SetPrototype_OnSerialize $5AF
#cmd LnSpotLight_SetPrototype_SetPrototype_OnUpdate $5BA
#cmd LnTestDelegate_Create $425
#cmd LnSprite_Create $19E
#cmd LnSprite_CreateWithTexture $19F
#cmd LnSprite_CreateWithTextureAndSize $1A1
#cmd LnSprite_SetTexture $190
#cmd LnSprite_SetSize $192
#cmd LnSprite_SetSizeWH $194
#cmd LnSprite_SetSourceRectXYWH $197
#cmd LnSprite_SetCallerTest $19C
#cmd LnSprite_SetPrototype_SetPrototype_OnSerialize $5C5
#cmd LnSprite_SetPrototype_SetPrototype_OnUpdate $5D0
#cmd LnCameraOrbitControlComponent_Create $1A6
#cmd LnCameraOrbitControlComponent_SetPrototype_SetPrototype_OnSerialize $5DB
#cmd LnRaycaster_FromScreen $1A8
#cmd LnRaycaster_IntersectPlane $1AA
#cmd LnRaycaster_SetPrototype_SetPrototype_OnSerialize $5E6
#cmd LnRaycastResult_GetPoint $1AF
#cmd LnRaycastResult_SetPrototype_SetPrototype_OnSerialize $5F1
#cmd LnWorldRenderView_SetGuideGridEnabled $1B1
#cmd LnWorldRenderView_GetGuideGridEnabled $1B3
#cmd LnWorldRenderView_SetPrototype_SetPrototype_OnSerialize $5FC
#cmd LnBoxMesh_Create $1B5
#cmd LnBoxMesh_SetPrototype_SetPrototype_OnSerialize $607
#cmd LnBoxMesh_SetPrototype_SetPrototype_OnUpdate $612
#cmd LnPlaneMesh_Create $1B7
#cmd LnPlaneMesh_SetPrototype_SetPrototype_OnSerialize $61D
#cmd LnPlaneMesh_SetPrototype_SetPrototype_OnUpdate $628
#cmd LnUIEventArgs_Sender $1B9
#cmd LnUIEventArgs_SetPrototype_SetPrototype_OnSerialize $633
#cmd LnUIGeneralEventHandler_Create $444
#cmd LnUIEventHandler_Create $449
#cmd LnUILayoutElement_SetPrototype_SetPrototype_OnSerialize $63E
#cmd LnUIElement_SetMargin $1CB
#cmd LnUIElement_GetMargin $1CD
#cmd LnUIElement_SetPadding $1CE
#cmd LnUIElement_GetPadding $1D0
#cmd LnUIElement_SetHAlignment $1D1
#cmd LnUIElement_GetHAlignment $1D3
#cmd LnUIElement_SetVAlignment $1D4
#cmd LnUIElement_GetVAlignment $1D6
#cmd LnUIElement_SetAlignments $1D7
#cmd LnUIElement_SetPosition $1DA
#cmd LnUIElement_SetPositionXYZ $1DC
#cmd LnUIElement_GetPosition $1E0
#cmd LnUIElement_SetRotation $1E1
#cmd LnUIElement_SetEulerAngles $1E3
#cmd LnUIElement_GetRotation $1E7
#cmd LnUIElement_SetScale $1E8
#cmd LnUIElement_SetScaleS $1EA
#cmd LnUIElement_SetScaleXY $1EC
#cmd LnUIElement_GetScale $1EF
#cmd LnUIElement_SetCenterPoint $1F0
#cmd LnUIElement_SetCenterPointXYZ $1F2
#cmd LnUIElement_GetCenterPoint $1F6
#cmd LnUIElement_AddChild $1F7
#cmd LnUIElement_SetPrototype_SetPrototype_OnSerialize $649
#cmd LnUITextBlock_Create $1FA
#cmd LnUITextBlock_CreateWithTextA $1FB
#cmd LnUITextBlock_SetPrototype_SetPrototype_OnSerialize $654
#cmd LnUISprite_Create $208
#cmd LnUISprite_CreateWithTexture $209
#cmd LnUISprite_SetTexture $1FE
#cmd LnUISprite_SetSourceRect $200
#cmd LnUISprite_SetSourceRectXYWH $202
#cmd LnUISprite_GetSourceRect $207
#cmd LnUISprite_SetPrototype_SetPrototype_OnSerialize $65F
#cmd LnInput_PressedA $20C
#cmd LnInput_TriggeredA $20E
#cmd LnInput_TriggeredOffA $210
#cmd LnInput_RepeatedA $212
#cmd LnInput_GetAxisValueA $214
#cmd LnInput_ClearAllBindings $216
#cmd LnMouse_Pressed $218
#cmd LnMouse_Triggered $21A
#cmd LnMouse_TriggeredOff $21C
#cmd LnMouse_Repeated $21E
#cmd LnMouse_Position $220
#cmd LnEngineSettings_SetMainWindowSize $222
#cmd LnEngineSettings_SetMainWorldViewSize $225
#cmd LnEngineSettings_SetMainWindowTitleA $228
#cmd LnEngineSettings_AddAssetDirectoryA $22A
#cmd LnEngineSettings_AddAssetArchiveA $22C
#cmd LnEngineSettings_SetFrameRate $22F
#cmd LnEngineSettings_SetDebugToolEnabled $231
#cmd LnEngineSettings_SetEngineLogEnabled $233
#cmd LnEngineSettings_SetEngineLogFilePathA $235
#cmd LnEngine_Initialize $238
#cmd LnEngine_Finalize $239
#cmd LnEngine_Update $23A
#cmd LnEngine_Time $23B
#cmd LnEngine_GetCamera $23C
#cmd LnEngine_GetLight $23D
#cmd LnEngine_GetRenderView $23E
#cmd LnApplication_Create $243
#cmd LnApplication_OnInit $240
#cmd LnApplication_OnUpdate $241
#cmd LnApplication_World $242
#cmd LnApplication_SetPrototype_SetPrototype_OnSerialize $66A
#cmd LnApplication_SetPrototype_SetPrototype_OnInit $675
#cmd LnApplication_SetPrototype_SetPrototype_OnUpdate $680
#cmd LnDebug_PrintA $245
#cmd LnDebug_PrintWithTimeA $247
#cmd LnDebug_PrintWithTimeAndColorA $24A
#cmd LnObjectSerializeHandler_Create $4A5
#cmd LnEventConnectionSerializeHandler_Create $4B0
#cmd LnZVTestClass1SerializeHandler_Create $4BB
#cmd LnZVTestEventArgs1SerializeHandler_Create $4C6
#cmd LnSerializer2SerializeHandler_Create $4D1
#cmd LnAssetModelSerializeHandler_Create $4DC
#cmd LnTextureSerializeHandler_Create $4E7
#cmd LnTexture2DSerializeHandler_Create $4F2
#cmd LnRenderViewSerializeHandler_Create $4FD
#cmd LnComponentSerializeHandler_Create $508
#cmd LnVisualComponentSerializeHandler_Create $513
#cmd LnSpriteComponentSerializeHandler_Create $51E
#cmd LnWorldSerializeHandler_Create $529
#cmd LnComponentListSerializeHandler_Create $534
#cmd LnWorldObjectSerializeHandler_Create $53F
#cmd LnWorldObjectUpdateHandler_Create $54A
#cmd LnVisualObjectSerializeHandler_Create $555
#cmd LnVisualObjectUpdateHandler_Create $560
#cmd LnCameraSerializeHandler_Create $56B
#cmd LnCameraUpdateHandler_Create $576
#cmd LnDirectionalLightSerializeHandler_Create $581
#cmd LnDirectionalLightUpdateHandler_Create $58C
#cmd LnPointLightSerializeHandler_Create $597
#cmd LnPointLightUpdateHandler_Create $5A2
#cmd LnSpotLightSerializeHandler_Create $5AD
#cmd LnSpotLightUpdateHandler_Create $5B8
#cmd LnSpriteSerializeHandler_Create $5C3
#cmd LnSpriteUpdateHandler_Create $5CE
#cmd LnCameraOrbitControlComponentSerializeHandler_Create $5D9
#cmd LnRaycasterSerializeHandler_Create $5E4
#cmd LnRaycastResultSerializeHandler_Create $5EF
#cmd LnWorldRenderViewSerializeHandler_Create $5FA
#cmd LnBoxMeshSerializeHandler_Create $605
#cmd LnBoxMeshUpdateHandler_Create $610
#cmd LnPlaneMeshSerializeHandler_Create $61B
#cmd LnPlaneMeshUpdateHandler_Create $626
#cmd LnUIEventArgsSerializeHandler_Create $631
#cmd LnUILayoutElementSerializeHandler_Create $63C
#cmd LnUIElementSerializeHandler_Create $647
#cmd LnUITextBlockSerializeHandler_Create $652
#cmd LnUISpriteSerializeHandler_Create $65D
#cmd LnApplicationSerializeHandler_Create $668
#cmd LnApplicationInitHandler_Create $673
#cmd LnApplicationUpdateHandler_Create $67E



#cmd ln_args $1

#endif // __lumino__
