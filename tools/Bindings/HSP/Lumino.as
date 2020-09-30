
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
#cmd LNObject_GetReferenceCount $72
#cmd LNObject_SetPrototype_OnSerialize $552
#cmd LNEventConnection_SetPrototype_OnSerialize $55D
#cmd LNPromiseFailureDelegate_Create $326
#cmd LNZVTestDelegate1_Create $32B
#cmd LNZVTestDelegate2_Create $331
#cmd LNZVTestDelegate3_Create $336
#cmd LNZVTestEventHandler1_Create $33B
#cmd LNZVTestEventHandler2_Create $340
#cmd LNZVTestPromise1_ThenWith $343
#cmd LNZVTestPromise1_CatchWith $345
#cmd LNZVTestPromise2_ThenWith $349
#cmd LNZVTestPromise2_CatchWith $34B
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $568
#cmd LNZVTestEventArgs1_Create $A1
#cmd LNZVTestEventArgs1_CreateWithValue $A2
#cmd LNZVTestEventArgs1_GetValue $A0
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $573
#cmd LNSerializer2_SetPrototype_OnSerialize $57E
#cmd LNAssetModel_Create $AE
#cmd LNAssetModel_Target $AD
#cmd LNAssetModel_SetPrototype_OnSerialize $589
#cmd LNAssets_SaveAssetToLocalFile $B1
#cmd LNAssets_LoadAssetFromLocalFile $B4
#cmd LNAssets_LoadAsset $B6
#cmd LNAssets_ReloadAsset $B8
#cmd LNTexture2DDelegate_Create $36F
#cmd LNTexture2DPromise_ThenWith $372
#cmd LNTexture2DPromise_CatchWith $374
#cmd LNTexture_SetPrototype_OnSerialize $594
#cmd LNTexture2D_Create $10A
#cmd LNTexture2D_CreateWithFormat $10D
#cmd LNTexture2D_Load $106
#cmd LNTexture2D_LoadEmoji $108
#cmd LNTexture2D_SetPrototype_OnSerialize $59F
#cmd LNRenderView_SetPrototype_OnSerialize $5AA
#cmd LNComponent_SetPrototype_OnSerialize $5B5
#cmd LNVisualComponent_SetVisible $114
#cmd LNVisualComponent_IsVisible $116
#cmd LNVisualComponent_SetPrototype_OnSerialize $5C0
#cmd LNSpriteComponent_SetTexture $118
#cmd LNSpriteComponent_SetPrototype_OnSerialize $5CB
#cmd LNWorld_Add $11B
#cmd LNWorld_SetPrototype_OnSerialize $5D6
#cmd LNComponentList_GetLength $403
#cmd LNComponentList_GetItem $405
#cmd LNComponentList_SetPrototype_OnSerialize $5E1
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
#cmd LNWorldObject_SetPrototype_OnSerialize $5EC
#cmd LNWorldObject_SetPrototype_OnUpdate $5F7
#cmd LNVisualObject_SetVisible $14B
#cmd LNVisualObject_IsVisible $14D
#cmd LNVisualObject_SetPrototype_OnSerialize $602
#cmd LNVisualObject_SetPrototype_OnUpdate $60D
#cmd LNCamera_SetPrototype_OnSerialize $618
#cmd LNCamera_SetPrototype_OnUpdate $623
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
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $62E
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $639
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
#cmd LNDirectionalLight_SetPrototype_OnSerialize $644
#cmd LNDirectionalLight_SetPrototype_OnUpdate $64F
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
#cmd LNPointLight_SetPrototype_OnSerialize $65A
#cmd LNPointLight_SetPrototype_OnUpdate $665
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
#cmd LNSpotLight_SetPrototype_OnSerialize $670
#cmd LNSpotLight_SetPrototype_OnUpdate $67B
#cmd LNTestDelegate_Create $4A3
#cmd LNSprite_Create $1BC
#cmd LNSprite_CreateWithTexture $1BD
#cmd LNSprite_CreateWithTextureAndSize $1BF
#cmd LNSprite_SetTexture $1AE
#cmd LNSprite_SetSize $1B0
#cmd LNSprite_SetSizeWH $1B2
#cmd LNSprite_SetSourceRectXYWH $1B5
#cmd LNSprite_SetCallerTest $1BA
#cmd LNSprite_SetPrototype_OnSerialize $686
#cmd LNSprite_SetPrototype_OnUpdate $691
#cmd LNCameraOrbitControlComponent_Create $1C4
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $69C
#cmd LNRaycaster_FromScreen $1C6
#cmd LNRaycaster_IntersectPlane $1C8
#cmd LNRaycaster_SetPrototype_OnSerialize $6A7
#cmd LNRaycastResult_GetPoint $1CD
#cmd LNRaycastResult_SetPrototype_OnSerialize $6B2
#cmd LNWorldRenderView_SetGuideGridEnabled $1CF
#cmd LNWorldRenderView_GetGuideGridEnabled $1D1
#cmd LNWorldRenderView_SetPrototype_OnSerialize $6BD
#cmd LNBoxMesh_Create $1D3
#cmd LNBoxMesh_SetPrototype_OnSerialize $6C8
#cmd LNBoxMesh_SetPrototype_OnUpdate $6D3
#cmd LNPlaneMesh_Create $1D5
#cmd LNPlaneMesh_SetPrototype_OnSerialize $6DE
#cmd LNPlaneMesh_SetPrototype_OnUpdate $6E9
#cmd LNLevel_Create $1DC
#cmd LNLevel_SetPrototype_OnSerialize $6F4
#cmd LNLevel_SetPrototype_OnStart $6FF
#cmd LNLevel_SetPrototype_OnStop $70A
#cmd LNLevel_SetPrototype_OnPause $715
#cmd LNLevel_SetPrototype_OnResume $720
#cmd LNLevel_SetPrototype_OnUpdate $72B
#cmd LNUIEventArgs_Sender $1DE
#cmd LNUIEventArgs_SetPrototype_OnSerialize $736
#cmd LNUIGeneralEventHandler_Create $4C8
#cmd LNUIEventHandler_Create $4CD
#cmd LNUILayoutElement_SetPrototype_OnSerialize $741
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
#cmd LNUIElement_SetPrototype_OnSerialize $74C
#cmd LNUITextBlock_Create $21F
#cmd LNUITextBlock_CreateWithText $220
#cmd LNUITextBlock_SetPrototype_OnSerialize $757
#cmd LNUISprite_Create $22D
#cmd LNUISprite_CreateWithTexture $22E
#cmd LNUISprite_SetTexture $223
#cmd LNUISprite_SetSourceRect $225
#cmd LNUISprite_SetSourceRectXYWH $227
#cmd LNUISprite_GetSourceRect $22C
#cmd LNUISprite_SetPrototype_OnSerialize $762
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
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $76D
#cmd LNInterpreterCommandList_Create $260
#cmd LNInterpreterCommandList_AddCommand $24C
#cmd LNInterpreterCommandList_AddCommand1 $24E
#cmd LNInterpreterCommandList_AddCommand2 $251
#cmd LNInterpreterCommandList_AddCommand3 $255
#cmd LNInterpreterCommandList_AddCommand4 $25A
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $778
#cmd LNInterpreterCommandDelegate_Create $526
#cmd LNInterpreter_Create $274
#cmd LNInterpreter_Clear $265
#cmd LNInterpreter_Run $266
#cmd LNInterpreter_IsRunning $268
#cmd LNInterpreter_Update $269
#cmd LNInterpreter_Terminate $26A
#cmd LNInterpreter_RegisterCommandHandler $26B
#cmd LNInterpreter_SetWaitMode $26E
#cmd LNInterpreter_GetWaitMode $270
#cmd LNInterpreter_SetWaitCount $271
#cmd LNInterpreter_GetWaitCount $273
#cmd LNInterpreter_SetPrototype_OnSerialize $783
#cmd LNInterpreter_SetPrototype_OnUpdateWait $78F
#cmd LNEngineSettings_SetMainWindowSize $277
#cmd LNEngineSettings_SetMainWorldViewSize $27A
#cmd LNEngineSettings_SetMainWindowTitle $27D
#cmd LNEngineSettings_AddAssetDirectory $27F
#cmd LNEngineSettings_AddAssetArchive $281
#cmd LNEngineSettings_SetFrameRate $284
#cmd LNEngineSettings_SetDebugToolEnabled $286
#cmd LNEngineSettings_SetEngineLogEnabled $288
#cmd LNEngineSettings_SetEngineLogFilePath $28A
#cmd LNEngine_Initialize $28D
#cmd LNEngine_Finalize $28E
#cmd LNEngine_Update $28F
#cmd LNEngine_Run $290
#cmd LNEngine_Time $292
#cmd LNEngine_GetCamera $293
#cmd LNEngine_GetMainLight $294
#cmd LNEngine_GetRenderView $295
#cmd LNApplication_Create $29A
#cmd LNApplication_OnInit $297
#cmd LNApplication_OnUpdate $298
#cmd LNApplication_World $299
#cmd LNApplication_SetPrototype_OnSerialize $79A
#cmd LNApplication_SetPrototype_OnInit $7A5
#cmd LNApplication_SetPrototype_OnUpdate $7B0
#cmd LNDebug_Print $29C
#cmd LNDebug_PrintWithTime $29E
#cmd LNDebug_PrintWithTimeAndColor $2A1
#cmd LNObjectSerializeHandler_Create $550
#cmd LNEventConnectionSerializeHandler_Create $55B
#cmd LNZVTestClass1SerializeHandler_Create $566
#cmd LNZVTestEventArgs1SerializeHandler_Create $571
#cmd LNSerializer2SerializeHandler_Create $57C
#cmd LNAssetModelSerializeHandler_Create $587
#cmd LNTextureSerializeHandler_Create $592
#cmd LNTexture2DSerializeHandler_Create $59D
#cmd LNRenderViewSerializeHandler_Create $5A8
#cmd LNComponentSerializeHandler_Create $5B3
#cmd LNVisualComponentSerializeHandler_Create $5BE
#cmd LNSpriteComponentSerializeHandler_Create $5C9
#cmd LNWorldSerializeHandler_Create $5D4
#cmd LNComponentListSerializeHandler_Create $5DF
#cmd LNWorldObjectSerializeHandler_Create $5EA
#cmd LNWorldObjectUpdateHandler_Create $5F5
#cmd LNVisualObjectSerializeHandler_Create $600
#cmd LNVisualObjectUpdateHandler_Create $60B
#cmd LNCameraSerializeHandler_Create $616
#cmd LNCameraUpdateHandler_Create $621
#cmd LNEnvironmentLightSerializeHandler_Create $62C
#cmd LNEnvironmentLightUpdateHandler_Create $637
#cmd LNDirectionalLightSerializeHandler_Create $642
#cmd LNDirectionalLightUpdateHandler_Create $64D
#cmd LNPointLightSerializeHandler_Create $658
#cmd LNPointLightUpdateHandler_Create $663
#cmd LNSpotLightSerializeHandler_Create $66E
#cmd LNSpotLightUpdateHandler_Create $679
#cmd LNSpriteSerializeHandler_Create $684
#cmd LNSpriteUpdateHandler_Create $68F
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $69A
#cmd LNRaycasterSerializeHandler_Create $6A5
#cmd LNRaycastResultSerializeHandler_Create $6B0
#cmd LNWorldRenderViewSerializeHandler_Create $6BB
#cmd LNBoxMeshSerializeHandler_Create $6C6
#cmd LNBoxMeshUpdateHandler_Create $6D1
#cmd LNPlaneMeshSerializeHandler_Create $6DC
#cmd LNPlaneMeshUpdateHandler_Create $6E7
#cmd LNLevelSerializeHandler_Create $6F2
#cmd LNLevelStartHandler_Create $6FD
#cmd LNLevelStopHandler_Create $708
#cmd LNLevelPauseHandler_Create $713
#cmd LNLevelResumeHandler_Create $71E
#cmd LNLevelUpdateHandler_Create $729
#cmd LNUIEventArgsSerializeHandler_Create $734
#cmd LNUILayoutElementSerializeHandler_Create $73F
#cmd LNUIElementSerializeHandler_Create $74A
#cmd LNUITextBlockSerializeHandler_Create $755
#cmd LNUISpriteSerializeHandler_Create $760
#cmd LNInterpreterCommandSerializeHandler_Create $76B
#cmd LNInterpreterCommandListSerializeHandler_Create $776
#cmd LNInterpreterSerializeHandler_Create $781
#cmd LNInterpreterUpdateWaitHandler_Create $78D
#cmd LNApplicationSerializeHandler_Create $798
#cmd LNApplicationInitHandler_Create $7A3
#cmd LNApplicationUpdateHandler_Create $7AE



#cmd ln_args $1

#endif // __lumino__
