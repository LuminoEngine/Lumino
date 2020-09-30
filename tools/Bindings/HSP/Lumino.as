
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
#cmd LNColor $D1
#cmd LNPoint $DC
#cmd LNSize $E3
#cmd LNRect $EA
#cmd LNThickness $F9

#cmd LNObject_Release $70
#cmd LNObject_Retain $71
#cmd LNObject_ReferenceCount $72
#cmd LNObject_SetPrototype_OnSerialize $542
#cmd LNEventConnection_SetPrototype_OnSerialize $54D
#cmd LNPromiseFailureDelegate_Create $321
#cmd LNZVTestDelegate1_Create $326
#cmd LNZVTestDelegate2_Create $32C
#cmd LNZVTestDelegate3_Create $331
#cmd LNZVTestEventHandler1_Create $336
#cmd LNZVTestEventHandler2_Create $33B
#cmd LNZVTestPromise1_ThenWith $33E
#cmd LNZVTestPromise1_CatchWith $340
#cmd LNZVTestPromise2_ThenWith $344
#cmd LNZVTestPromise2_CatchWith $346
#cmd LNZVTestClass1_Create $9E
#cmd LNZVTestClass1_SetTestDelegate1 $88
#cmd LNZVTestClass1_SetTestDelegate2 $8A
#cmd LNZVTestClass1_SetTestDelegate3 $8C
#cmd LNZVTestClass1_CallTestDelegate1 $8E
#cmd LNZVTestClass1_CallTestDelegate2 $90
#cmd LNZVTestClass1_CallTestDelegate3 $93
#cmd LNZVTestClass1_LoadAsync $94
#cmd LNZVTestClass1_ExecuteAsync $96
#cmd LNZVTestClass1_GetFilePath $97
#cmd LNZVTestClass1_ConnectOnEvent1 $98
#cmd LNZVTestClass1_RaiseEvent1 $9A
#cmd LNZVTestClass1_ConnectOnEvent2 $9B
#cmd LNZVTestClass1_RaiseEvent2 $9D
#cmd LNZVTestClass1_SetPrototype_OnSerialize $558
#cmd LNZVTestEventArgs1_Create $A1
#cmd LNZVTestEventArgs1_CreateWithValue $A2
#cmd LNZVTestEventArgs1_GetValue $A0
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $563
#cmd LNSerializer2_SetPrototype_OnSerialize $56E
#cmd LNAssetModel_Create $AE
#cmd LNAssetModel_Target $AD
#cmd LNAssetModel_SetPrototype_OnSerialize $579
#cmd LNAssets_SaveAssetToLocalFile $B1
#cmd LNAssets_LoadAssetFromLocalFile $B4
#cmd LNAssets_LoadAsset $B6
#cmd LNAssets_ReloadAsset $B8
#cmd LNTexture2DDelegate_Create $36A
#cmd LNTexture2DPromise_ThenWith $36D
#cmd LNTexture2DPromise_CatchWith $36F
#cmd LNTexture_SetPrototype_OnSerialize $584
#cmd LNTexture2D_Create $10A
#cmd LNTexture2D_CreateWithFormat $10D
#cmd LNTexture2D_Load $106
#cmd LNTexture2D_LoadEmoji $108
#cmd LNTexture2D_SetPrototype_OnSerialize $58F
#cmd LNRenderView_SetPrototype_OnSerialize $59A
#cmd LNComponent_SetPrototype_OnSerialize $5A5
#cmd LNVisualComponent_SetVisible $114
#cmd LNVisualComponent_IsVisible $116
#cmd LNVisualComponent_SetPrototype_OnSerialize $5B0
#cmd LNSpriteComponent_SetTexture $118
#cmd LNSpriteComponent_SetPrototype_OnSerialize $5BB
#cmd LNWorld_Add $11B
#cmd LNWorld_SetPrototype_OnSerialize $5C6
#cmd LNComponentList_GetLength $3FE
#cmd LNComponentList_GetItem $400
#cmd LNComponentList_SetPrototype_OnSerialize $5D1
#cmd LNWorldObject_SetPosition $11F
#cmd LNWorldObject_SetPositionXYZ $121
#cmd LNWorldObject_GetPosition $125
#cmd LNWorldObject_SetRotationQuaternion $126
#cmd LNWorldObject_SetRotation $128
#cmd LNWorldObject_GetRotation $12C
#cmd LNWorldObject_SetScale $12D
#cmd LNWorldObject_SetScaleS $12F
#cmd LNWorldObject_SetScaleXYZ $131
#cmd LNWorldObject_GetScale $135
#cmd LNWorldObject_SetCenterPoint $136
#cmd LNWorldObject_SetCenterPointXYZ $138
#cmd LNWorldObject_GetCenterPoint $13C
#cmd LNWorldObject_LookAt $13D
#cmd LNWorldObject_LookAtXYZ $13F
#cmd LNWorldObject_AddComponent $143
#cmd LNWorldObject_RemoveComponent $145
#cmd LNWorldObject_GetComponents $147
#cmd LNWorldObject_SetPrototype_OnSerialize $5DC
#cmd LNWorldObject_SetPrototype_OnUpdate $5E7
#cmd LNVisualObject_SetVisible $14B
#cmd LNVisualObject_IsVisible $14D
#cmd LNVisualObject_SetPrototype_OnSerialize $5F2
#cmd LNVisualObject_SetPrototype_OnUpdate $5FD
#cmd LNCamera_SetPrototype_OnSerialize $608
#cmd LNCamera_SetPrototype_OnUpdate $613
#cmd LNEnvironmentLight_SetEnabled $150
#cmd LNEnvironmentLight_IsEnabled $152
#cmd LNEnvironmentLight_SetColor $153
#cmd LNEnvironmentLight_GetColor $155
#cmd LNEnvironmentLight_SetAmbientColor $156
#cmd LNEnvironmentLight_GetAmbientColor $158
#cmd LNEnvironmentLight_GetSkyColor $159
#cmd LNEnvironmentLight_SetSkyColor $15A
#cmd LNEnvironmentLight_GetGroundColor $15C
#cmd LNEnvironmentLight_SetGroundColor $15D
#cmd LNEnvironmentLight_SetIntensity $15F
#cmd LNEnvironmentLight_GetIntensity $161
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $162
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $164
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $165
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $167
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $61E
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $629
#cmd LNDirectionalLight_Create $178
#cmd LNDirectionalLight_CreateWithColor $179
#cmd LNDirectionalLight_SetEnabled $169
#cmd LNDirectionalLight_IsEnabled $16B
#cmd LNDirectionalLight_SetColor $16C
#cmd LNDirectionalLight_GetColor $16E
#cmd LNDirectionalLight_SetIntensity $16F
#cmd LNDirectionalLight_GetIntensity $171
#cmd LNDirectionalLight_SetShadowEffectiveDistance $172
#cmd LNDirectionalLight_GetShadowEffectiveDistance $174
#cmd LNDirectionalLight_SetShadowEffectiveDepth $175
#cmd LNDirectionalLight_GetShadowEffectiveDepth $177
#cmd LNDirectionalLight_SetPrototype_OnSerialize $634
#cmd LNDirectionalLight_SetPrototype_OnUpdate $63F
#cmd LNPointLight_Create $18B
#cmd LNPointLight_CreateWithColorAndRange $18C
#cmd LNPointLight_SetEnabled $17C
#cmd LNPointLight_IsEnabled $17E
#cmd LNPointLight_SetColor $17F
#cmd LNPointLight_GetColor $181
#cmd LNPointLight_SetIntensity $182
#cmd LNPointLight_GetIntensity $184
#cmd LNPointLight_SetRange $185
#cmd LNPointLight_GetRange $187
#cmd LNPointLight_SetAttenuation $188
#cmd LNPointLight_GetAttenuation $18A
#cmd LNPointLight_SetPrototype_OnSerialize $64A
#cmd LNPointLight_SetPrototype_OnUpdate $655
#cmd LNSpotLight_Create $1A5
#cmd LNSpotLight_CreateWithColorAndRange $1A6
#cmd LNSpotLight_SetEnabled $190
#cmd LNSpotLight_IsEnabled $192
#cmd LNSpotLight_SetColor $193
#cmd LNSpotLight_GetColor $195
#cmd LNSpotLight_SetIntensity $196
#cmd LNSpotLight_GetIntensity $198
#cmd LNSpotLight_SetRange $199
#cmd LNSpotLight_GetRange $19B
#cmd LNSpotLight_SetAttenuation $19C
#cmd LNSpotLight_GetAttenuation $19E
#cmd LNSpotLight_SetAngle $19F
#cmd LNSpotLight_GetAngle $1A1
#cmd LNSpotLight_SetPenumbra $1A2
#cmd LNSpotLight_GetPenumbra $1A4
#cmd LNSpotLight_SetPrototype_OnSerialize $660
#cmd LNSpotLight_SetPrototype_OnUpdate $66B
#cmd LNTestDelegate_Create $49E
#cmd LNSprite_Create $1BC
#cmd LNSprite_CreateWithTexture $1BD
#cmd LNSprite_CreateWithTextureAndSize $1BF
#cmd LNSprite_SetTexture $1AE
#cmd LNSprite_SetSize $1B0
#cmd LNSprite_SetSizeWH $1B2
#cmd LNSprite_SetSourceRectXYWH $1B5
#cmd LNSprite_SetCallerTest $1BA
#cmd LNSprite_SetPrototype_OnSerialize $676
#cmd LNSprite_SetPrototype_OnUpdate $681
#cmd LNCameraOrbitControlComponent_Create $1C4
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $68C
#cmd LNRaycaster_FromScreen $1C6
#cmd LNRaycaster_IntersectPlane $1C8
#cmd LNRaycaster_SetPrototype_OnSerialize $697
#cmd LNRaycastResult_GetPoint $1CD
#cmd LNRaycastResult_SetPrototype_OnSerialize $6A2
#cmd LNWorldRenderView_SetGuideGridEnabled $1CF
#cmd LNWorldRenderView_GetGuideGridEnabled $1D1
#cmd LNWorldRenderView_SetPrototype_OnSerialize $6AD
#cmd LNBoxMesh_Create $1D3
#cmd LNBoxMesh_SetPrototype_OnSerialize $6B8
#cmd LNBoxMesh_SetPrototype_OnUpdate $6C3
#cmd LNPlaneMesh_Create $1D5
#cmd LNPlaneMesh_SetPrototype_OnSerialize $6CE
#cmd LNPlaneMesh_SetPrototype_OnUpdate $6D9
#cmd LNLevel_Create $1DC
#cmd LNLevel_SetPrototype_OnSerialize $6E4
#cmd LNLevel_SetPrototype_OnStart $6EF
#cmd LNLevel_SetPrototype_OnStop $6FA
#cmd LNLevel_SetPrototype_OnPause $705
#cmd LNLevel_SetPrototype_OnResume $710
#cmd LNLevel_SetPrototype_OnUpdate $71B
#cmd LNUIEventArgs_Sender $1DE
#cmd LNUIEventArgs_SetPrototype_OnSerialize $726
#cmd LNUIGeneralEventHandler_Create $4C3
#cmd LNUIEventHandler_Create $4C8
#cmd LNUILayoutElement_SetPrototype_OnSerialize $731
#cmd LNUIElement_SetMargin $1F0
#cmd LNUIElement_GetMargin $1F2
#cmd LNUIElement_SetPadding $1F3
#cmd LNUIElement_GetPadding $1F5
#cmd LNUIElement_SetHAlignment $1F6
#cmd LNUIElement_GetHAlignment $1F8
#cmd LNUIElement_SetVAlignment $1F9
#cmd LNUIElement_GetVAlignment $1FB
#cmd LNUIElement_SetAlignments $1FC
#cmd LNUIElement_SetPosition $1FF
#cmd LNUIElement_SetPositionXYZ $201
#cmd LNUIElement_GetPosition $205
#cmd LNUIElement_SetRotation $206
#cmd LNUIElement_SetEulerAngles $208
#cmd LNUIElement_GetRotation $20C
#cmd LNUIElement_SetScale $20D
#cmd LNUIElement_SetScaleS $20F
#cmd LNUIElement_SetScaleXY $211
#cmd LNUIElement_GetScale $214
#cmd LNUIElement_SetCenterPoint $215
#cmd LNUIElement_SetCenterPointXYZ $217
#cmd LNUIElement_GetCenterPoint $21B
#cmd LNUIElement_AddChild $21C
#cmd LNUIElement_SetPrototype_OnSerialize $73C
#cmd LNUITextBlock_Create $21F
#cmd LNUITextBlock_CreateWithText $220
#cmd LNUITextBlock_SetPrototype_OnSerialize $747
#cmd LNUISprite_Create $22D
#cmd LNUISprite_CreateWithTexture $22E
#cmd LNUISprite_SetTexture $223
#cmd LNUISprite_SetSourceRect $225
#cmd LNUISprite_SetSourceRectXYWH $227
#cmd LNUISprite_GetSourceRect $22C
#cmd LNUISprite_SetPrototype_OnSerialize $752
#cmd LNInput_Pressed $231
#cmd LNInput_Triggered $233
#cmd LNInput_TriggeredOff $235
#cmd LNInput_Repeated $237
#cmd LNInput_GetAxisValue $239
#cmd LNInput_ClearAllBindings $23B
#cmd LNMouse_Pressed $23D
#cmd LNMouse_Triggered $23F
#cmd LNMouse_TriggeredOff $241
#cmd LNMouse_Repeated $243
#cmd LNMouse_Position $245
#cmd LNInterpreterCommand_Code $247
#cmd LNInterpreterCommand_ParamsCount $248
#cmd LNInterpreterCommand_Param $249
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $75D
#cmd LNInterpreterCommandList_Create $260
#cmd LNInterpreterCommandList_AddCommand $24C
#cmd LNInterpreterCommandList_AddCommand1 $24E
#cmd LNInterpreterCommandList_AddCommand2 $251
#cmd LNInterpreterCommandList_AddCommand3 $255
#cmd LNInterpreterCommandList_AddCommand4 $25A
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $768
#cmd LNInterpreterCommandDelegate_Create $521
#cmd LNInterpreter_Create $26E
#cmd LNInterpreter_Clear $265
#cmd LNInterpreter_Run $266
#cmd LNInterpreter_IsRunning $268
#cmd LNInterpreter_Update $269
#cmd LNInterpreter_Terminate $26A
#cmd LNInterpreter_RegisterCommandHandler $26B
#cmd LNInterpreter_SetPrototype_OnSerialize $773
#cmd LNInterpreter_SetPrototype_OnUpdateWait $77F
#cmd LNEngineSettings_SetMainWindowSize $272
#cmd LNEngineSettings_SetMainWorldViewSize $275
#cmd LNEngineSettings_SetMainWindowTitle $278
#cmd LNEngineSettings_AddAssetDirectory $27A
#cmd LNEngineSettings_AddAssetArchive $27C
#cmd LNEngineSettings_SetFrameRate $27F
#cmd LNEngineSettings_SetDebugToolEnabled $281
#cmd LNEngineSettings_SetEngineLogEnabled $283
#cmd LNEngineSettings_SetEngineLogFilePath $285
#cmd LNEngine_Initialize $288
#cmd LNEngine_Finalize $289
#cmd LNEngine_Update $28A
#cmd LNEngine_Run $28B
#cmd LNEngine_Time $28D
#cmd LNEngine_GetCamera $28E
#cmd LNEngine_GetMainLight $28F
#cmd LNEngine_GetRenderView $290
#cmd LNApplication_Create $295
#cmd LNApplication_OnInit $292
#cmd LNApplication_OnUpdate $293
#cmd LNApplication_World $294
#cmd LNApplication_SetPrototype_OnSerialize $78A
#cmd LNApplication_SetPrototype_OnInit $795
#cmd LNApplication_SetPrototype_OnUpdate $7A0
#cmd LNDebug_Print $297
#cmd LNDebug_PrintWithTime $299
#cmd LNDebug_PrintWithTimeAndColor $29C
#cmd LNObjectSerializeHandler_Create $540
#cmd LNEventConnectionSerializeHandler_Create $54B
#cmd LNZVTestClass1SerializeHandler_Create $556
#cmd LNZVTestEventArgs1SerializeHandler_Create $561
#cmd LNSerializer2SerializeHandler_Create $56C
#cmd LNAssetModelSerializeHandler_Create $577
#cmd LNTextureSerializeHandler_Create $582
#cmd LNTexture2DSerializeHandler_Create $58D
#cmd LNRenderViewSerializeHandler_Create $598
#cmd LNComponentSerializeHandler_Create $5A3
#cmd LNVisualComponentSerializeHandler_Create $5AE
#cmd LNSpriteComponentSerializeHandler_Create $5B9
#cmd LNWorldSerializeHandler_Create $5C4
#cmd LNComponentListSerializeHandler_Create $5CF
#cmd LNWorldObjectSerializeHandler_Create $5DA
#cmd LNWorldObjectUpdateHandler_Create $5E5
#cmd LNVisualObjectSerializeHandler_Create $5F0
#cmd LNVisualObjectUpdateHandler_Create $5FB
#cmd LNCameraSerializeHandler_Create $606
#cmd LNCameraUpdateHandler_Create $611
#cmd LNEnvironmentLightSerializeHandler_Create $61C
#cmd LNEnvironmentLightUpdateHandler_Create $627
#cmd LNDirectionalLightSerializeHandler_Create $632
#cmd LNDirectionalLightUpdateHandler_Create $63D
#cmd LNPointLightSerializeHandler_Create $648
#cmd LNPointLightUpdateHandler_Create $653
#cmd LNSpotLightSerializeHandler_Create $65E
#cmd LNSpotLightUpdateHandler_Create $669
#cmd LNSpriteSerializeHandler_Create $674
#cmd LNSpriteUpdateHandler_Create $67F
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $68A
#cmd LNRaycasterSerializeHandler_Create $695
#cmd LNRaycastResultSerializeHandler_Create $6A0
#cmd LNWorldRenderViewSerializeHandler_Create $6AB
#cmd LNBoxMeshSerializeHandler_Create $6B6
#cmd LNBoxMeshUpdateHandler_Create $6C1
#cmd LNPlaneMeshSerializeHandler_Create $6CC
#cmd LNPlaneMeshUpdateHandler_Create $6D7
#cmd LNLevelSerializeHandler_Create $6E2
#cmd LNLevelStartHandler_Create $6ED
#cmd LNLevelStopHandler_Create $6F8
#cmd LNLevelPauseHandler_Create $703
#cmd LNLevelResumeHandler_Create $70E
#cmd LNLevelUpdateHandler_Create $719
#cmd LNUIEventArgsSerializeHandler_Create $724
#cmd LNUILayoutElementSerializeHandler_Create $72F
#cmd LNUIElementSerializeHandler_Create $73A
#cmd LNUITextBlockSerializeHandler_Create $745
#cmd LNUISpriteSerializeHandler_Create $750
#cmd LNInterpreterCommandSerializeHandler_Create $75B
#cmd LNInterpreterCommandListSerializeHandler_Create $766
#cmd LNInterpreterSerializeHandler_Create $771
#cmd LNInterpreterUpdateWaitHandler_Create $77D
#cmd LNApplicationSerializeHandler_Create $788
#cmd LNApplicationInitHandler_Create $793
#cmd LNApplicationUpdateHandler_Create $79E



#cmd ln_args $1

#endif // __lumino__
