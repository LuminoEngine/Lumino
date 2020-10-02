
#ifndef __lumino__
#define __lumino__

#ifdef LUMINO_DEBUG
    #regcmd "_hsp3cmdinit@4","LuminoHSPd.dll", 6
#else
    #regcmd "_hsp3cmdinit@4","LuminoHSP.dll", 6
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
#const global LN_LEVEL_TRANSITION_EFFECT_MODE_NONE 0
#const global LN_LEVEL_TRANSITION_EFFECT_MODE_FADE_IN_OUT 1
#const global LN_LEVEL_TRANSITION_EFFECT_MODE_CROSS_FADE 2
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
#cmd LNObject_SetPrototype_OnSerialize $58D
#cmd LNEventConnection_SetPrototype_OnSerialize $598
#cmd LNPromiseFailureDelegate_Create $356
#cmd LNZVTestDelegate1_Create $35B
#cmd LNZVTestDelegate2_Create $361
#cmd LNZVTestDelegate3_Create $366
#cmd LNZVTestEventHandler1_Create $36B
#cmd LNZVTestEventHandler2_Create $370
#cmd LNZVTestPromise1_ThenWith $373
#cmd LNZVTestPromise1_CatchWith $375
#cmd LNZVTestPromise2_ThenWith $379
#cmd LNZVTestPromise2_CatchWith $37B
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $5A3
#cmd LNZVTestEventArgs1_Create $A1
#cmd LNZVTestEventArgs1_CreateWithValue $A2
#cmd LNZVTestEventArgs1_GetValue $A0
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $5AE
#cmd LNSerializer2_SetPrototype_OnSerialize $5B9
#cmd LNAssetModel_Create $AE
#cmd LNAssetModel_Target $AD
#cmd LNAssetModel_SetPrototype_OnSerialize $5C4
#cmd LNAssets_SaveAssetToLocalFile $B1
#cmd LNAssets_LoadAssetFromLocalFile $B4
#cmd LNAssets_LoadAsset $B6
#cmd LNAssets_ReloadAsset $B8
#cmd LNTexture2DDelegate_Create $39F
#cmd LNTexture2DPromise_ThenWith $3A2
#cmd LNTexture2DPromise_CatchWith $3A4
#cmd LNTexture_SetPrototype_OnSerialize $5CF
#cmd LNTexture2D_Create $10A
#cmd LNTexture2D_CreateWithFormat $10D
#cmd LNTexture2D_Load $106
#cmd LNTexture2D_LoadEmoji $108
#cmd LNTexture2D_SetPrototype_OnSerialize $5DA
#cmd LNRenderView_SetPrototype_OnSerialize $5E5
#cmd LNComponent_SetPrototype_OnSerialize $5F0
#cmd LNVisualComponent_SetVisible $11A
#cmd LNVisualComponent_IsVisible $11C
#cmd LNVisualComponent_SetPrototype_OnSerialize $5FB
#cmd LNSpriteComponent_SetTexture $11E
#cmd LNSpriteComponent_SetPrototype_OnSerialize $606
#cmd LNWorld_Add $125
#cmd LNWorld_SetPrototype_OnSerialize $611
#cmd LNComponentList_GetLength $433
#cmd LNComponentList_GetItem $435
#cmd LNComponentList_SetPrototype_OnSerialize $61C
#cmd LNWorldObject_SetPosition $129
#cmd LNWorldObject_SetPositionXYZ $12B
#cmd LNWorldObject_GetPosition $12F
#cmd LNWorldObject_SetRotationQuaternion $130
#cmd LNWorldObject_SetRotation $132
#cmd LNWorldObject_GetRotation $136
#cmd LNWorldObject_SetScale $137
#cmd LNWorldObject_SetScaleS $139
#cmd LNWorldObject_SetScaleXYZ $13B
#cmd LNWorldObject_GetScale $13F
#cmd LNWorldObject_SetCenterPoint $140
#cmd LNWorldObject_SetCenterPointXYZ $142
#cmd LNWorldObject_GetCenterPoint $146
#cmd LNWorldObject_LookAt $147
#cmd LNWorldObject_LookAtXYZ $149
#cmd LNWorldObject_AddComponent $14D
#cmd LNWorldObject_RemoveComponent $14F
#cmd LNWorldObject_GetComponents $151
#cmd LNWorldObject_SetPrototype_OnSerialize $627
#cmd LNWorldObject_SetPrototype_OnUpdate $632
#cmd LNVisualObject_SetVisible $155
#cmd LNVisualObject_IsVisible $157
#cmd LNVisualObject_SetPrototype_OnSerialize $63D
#cmd LNVisualObject_SetPrototype_OnUpdate $648
#cmd LNCamera_SetPrototype_OnSerialize $653
#cmd LNCamera_SetPrototype_OnUpdate $65E
#cmd LNEnvironmentLight_SetEnabled $15A
#cmd LNEnvironmentLight_IsEnabled $15C
#cmd LNEnvironmentLight_SetColor $15D
#cmd LNEnvironmentLight_GetColor $15F
#cmd LNEnvironmentLight_SetAmbientColor $160
#cmd LNEnvironmentLight_GetAmbientColor $162
#cmd LNEnvironmentLight_GetSkyColor $163
#cmd LNEnvironmentLight_SetSkyColor $164
#cmd LNEnvironmentLight_GetGroundColor $166
#cmd LNEnvironmentLight_SetGroundColor $167
#cmd LNEnvironmentLight_SetIntensity $169
#cmd LNEnvironmentLight_GetIntensity $16B
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $16C
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $16E
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $16F
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $171
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $669
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $674
#cmd LNDirectionalLight_Create $182
#cmd LNDirectionalLight_CreateWithColor $183
#cmd LNDirectionalLight_SetEnabled $173
#cmd LNDirectionalLight_IsEnabled $175
#cmd LNDirectionalLight_SetColor $176
#cmd LNDirectionalLight_GetColor $178
#cmd LNDirectionalLight_SetIntensity $179
#cmd LNDirectionalLight_GetIntensity $17B
#cmd LNDirectionalLight_SetShadowEffectiveDistance $17C
#cmd LNDirectionalLight_GetShadowEffectiveDistance $17E
#cmd LNDirectionalLight_SetShadowEffectiveDepth $17F
#cmd LNDirectionalLight_GetShadowEffectiveDepth $181
#cmd LNDirectionalLight_SetPrototype_OnSerialize $67F
#cmd LNDirectionalLight_SetPrototype_OnUpdate $68A
#cmd LNPointLight_Create $195
#cmd LNPointLight_CreateWithColorAndRange $196
#cmd LNPointLight_SetEnabled $186
#cmd LNPointLight_IsEnabled $188
#cmd LNPointLight_SetColor $189
#cmd LNPointLight_GetColor $18B
#cmd LNPointLight_SetIntensity $18C
#cmd LNPointLight_GetIntensity $18E
#cmd LNPointLight_SetRange $18F
#cmd LNPointLight_GetRange $191
#cmd LNPointLight_SetAttenuation $192
#cmd LNPointLight_GetAttenuation $194
#cmd LNPointLight_SetPrototype_OnSerialize $695
#cmd LNPointLight_SetPrototype_OnUpdate $6A0
#cmd LNSpotLight_Create $1AF
#cmd LNSpotLight_CreateWithColorAndRange $1B0
#cmd LNSpotLight_SetEnabled $19A
#cmd LNSpotLight_IsEnabled $19C
#cmd LNSpotLight_SetColor $19D
#cmd LNSpotLight_GetColor $19F
#cmd LNSpotLight_SetIntensity $1A0
#cmd LNSpotLight_GetIntensity $1A2
#cmd LNSpotLight_SetRange $1A3
#cmd LNSpotLight_GetRange $1A5
#cmd LNSpotLight_SetAttenuation $1A6
#cmd LNSpotLight_GetAttenuation $1A8
#cmd LNSpotLight_SetAngle $1A9
#cmd LNSpotLight_GetAngle $1AB
#cmd LNSpotLight_SetPenumbra $1AC
#cmd LNSpotLight_GetPenumbra $1AE
#cmd LNSpotLight_SetPrototype_OnSerialize $6AB
#cmd LNSpotLight_SetPrototype_OnUpdate $6B6
#cmd LNTestDelegate_Create $4D3
#cmd LNSprite_Create $1C6
#cmd LNSprite_CreateWithTexture $1C7
#cmd LNSprite_CreateWithTextureAndSize $1C9
#cmd LNSprite_SetTexture $1B8
#cmd LNSprite_SetSize $1BA
#cmd LNSprite_SetSizeWH $1BC
#cmd LNSprite_SetSourceRectXYWH $1BF
#cmd LNSprite_SetCallerTest $1C4
#cmd LNSprite_SetPrototype_OnSerialize $6C1
#cmd LNSprite_SetPrototype_OnUpdate $6CC
#cmd LNCameraOrbitControlComponent_Create $1CE
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $6D7
#cmd LNRaycaster_FromScreen $1D0
#cmd LNRaycaster_IntersectPlane $1D2
#cmd LNRaycaster_SetPrototype_OnSerialize $6E2
#cmd LNRaycastResult_GetPoint $1D7
#cmd LNRaycastResult_SetPrototype_OnSerialize $6ED
#cmd LNWorldRenderView_SetGuideGridEnabled $1D9
#cmd LNWorldRenderView_GetGuideGridEnabled $1DB
#cmd LNWorldRenderView_SetPrototype_OnSerialize $6F8
#cmd LNBoxMesh_Create $1DD
#cmd LNBoxMesh_SetPrototype_OnSerialize $703
#cmd LNBoxMesh_SetPrototype_OnUpdate $70E
#cmd LNPlaneMesh_Create $1DF
#cmd LNPlaneMesh_SetPrototype_OnSerialize $719
#cmd LNPlaneMesh_SetPrototype_OnUpdate $724
#cmd LNScene_SetClearMode $1E1
#cmd LNScene_SetSkyColor $1E3
#cmd LNScene_SetSkyHorizonColor $1E5
#cmd LNScene_SetSkyCloudColor $1E7
#cmd LNScene_SetSkyOverlayColor $1E9
#cmd LNScene_GotoLevel $1EB
#cmd LNScene_ActiveLevel $1EE
#cmd LNScene_IsTransitionEffectRunning $1EF
#cmd LNScene_SetTransitionEffectMode $1F0
#cmd LNScene_TransitionEffectMode $1F2
#cmd LNScene_SetTransitionDuration $1F3
#cmd LNScene_TransitionDuration $1F5
#cmd LNScene_SetTransitionEffectColor $1F6
#cmd LNScene_TransitionEffectColor $1F8
#cmd LNScene_SetTransitionEffectMaskTexture $1F9
#cmd LNScene_TransitionEffectMaskTexture $1FB
#cmd LNScene_SetTransitionEffectVague $1FC
#cmd LNScene_TransitionEffectVague $1FE
#cmd LNScene_StartFadeOut $1FF
#cmd LNScene_StartFadeIn $200
#cmd LNLevel_Create $20C
#cmd LNLevel_AddSubLevel $202
#cmd LNLevel_RemoveSubLevel $204
#cmd LNLevel_RemoveAllSubLevels $206
#cmd LNLevel_SetPrototype_OnSerialize $72F
#cmd LNLevel_SetPrototype_OnStart $73A
#cmd LNLevel_SetPrototype_OnStop $745
#cmd LNLevel_SetPrototype_OnPause $750
#cmd LNLevel_SetPrototype_OnResume $75B
#cmd LNLevel_SetPrototype_OnUpdate $766
#cmd LNUIEventArgs_Sender $20E
#cmd LNUIEventArgs_SetPrototype_OnSerialize $771
#cmd LNUIGeneralEventHandler_Create $503
#cmd LNUIEventHandler_Create $508
#cmd LNUILayoutElement_SetPrototype_OnSerialize $77C
#cmd LNUIElement_SetMargin $220
#cmd LNUIElement_GetMargin $222
#cmd LNUIElement_SetPadding $223
#cmd LNUIElement_GetPadding $225
#cmd LNUIElement_SetHAlignment $226
#cmd LNUIElement_GetHAlignment $228
#cmd LNUIElement_SetVAlignment $229
#cmd LNUIElement_GetVAlignment $22B
#cmd LNUIElement_SetAlignments $22C
#cmd LNUIElement_SetPosition $22F
#cmd LNUIElement_SetPositionXYZ $231
#cmd LNUIElement_GetPosition $235
#cmd LNUIElement_SetRotation $236
#cmd LNUIElement_SetEulerAngles $238
#cmd LNUIElement_GetRotation $23C
#cmd LNUIElement_SetScale $23D
#cmd LNUIElement_SetScaleS $23F
#cmd LNUIElement_SetScaleXY $241
#cmd LNUIElement_GetScale $244
#cmd LNUIElement_SetCenterPoint $245
#cmd LNUIElement_SetCenterPointXYZ $247
#cmd LNUIElement_GetCenterPoint $24B
#cmd LNUIElement_AddChild $24C
#cmd LNUIElement_SetPrototype_OnSerialize $787
#cmd LNUITextBlock_Create $24F
#cmd LNUITextBlock_CreateWithText $250
#cmd LNUITextBlock_SetPrototype_OnSerialize $792
#cmd LNUISprite_Create $25D
#cmd LNUISprite_CreateWithTexture $25E
#cmd LNUISprite_SetTexture $253
#cmd LNUISprite_SetSourceRect $255
#cmd LNUISprite_SetSourceRectXYWH $257
#cmd LNUISprite_GetSourceRect $25C
#cmd LNUISprite_SetPrototype_OnSerialize $79D
#cmd LNInput_Pressed $261
#cmd LNInput_Triggered $263
#cmd LNInput_TriggeredOff $265
#cmd LNInput_Repeated $267
#cmd LNInput_GetAxisValue $269
#cmd LNInput_ClearAllBindings $26B
#cmd LNMouse_Pressed $26D
#cmd LNMouse_Triggered $26F
#cmd LNMouse_TriggeredOff $271
#cmd LNMouse_Repeated $273
#cmd LNMouse_Position $275
#cmd LNInterpreterCommand_Code $277
#cmd LNInterpreterCommand_ParamsCount $278
#cmd LNInterpreterCommand_Param $279
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $7A8
#cmd LNInterpreterCommandList_Create $290
#cmd LNInterpreterCommandList_AddCommand $27C
#cmd LNInterpreterCommandList_AddCommand1 $27E
#cmd LNInterpreterCommandList_AddCommand2 $281
#cmd LNInterpreterCommandList_AddCommand3 $285
#cmd LNInterpreterCommandList_AddCommand4 $28A
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $7B3
#cmd LNInterpreterCommandDelegate_Create $561
#cmd LNInterpreter_Create $2A4
#cmd LNInterpreter_Clear $295
#cmd LNInterpreter_Run $296
#cmd LNInterpreter_IsRunning $298
#cmd LNInterpreter_Update $299
#cmd LNInterpreter_Terminate $29A
#cmd LNInterpreter_RegisterCommandHandler $29B
#cmd LNInterpreter_SetWaitMode $29E
#cmd LNInterpreter_GetWaitMode $2A0
#cmd LNInterpreter_SetWaitCount $2A1
#cmd LNInterpreter_GetWaitCount $2A3
#cmd LNInterpreter_SetPrototype_OnSerialize $7BE
#cmd LNInterpreter_SetPrototype_OnUpdateWait $7CA
#cmd LNEngineSettings_SetMainWindowSize $2A7
#cmd LNEngineSettings_SetMainWorldViewSize $2AA
#cmd LNEngineSettings_SetMainWindowTitle $2AD
#cmd LNEngineSettings_AddAssetDirectory $2AF
#cmd LNEngineSettings_AddAssetArchive $2B1
#cmd LNEngineSettings_SetFrameRate $2B4
#cmd LNEngineSettings_SetDebugToolEnabled $2B6
#cmd LNEngineSettings_SetEngineLogEnabled $2B8
#cmd LNEngineSettings_SetEngineLogFilePath $2BA
#cmd LNEngine_Initialize $2BD
#cmd LNEngine_Finalize $2BE
#cmd LNEngine_Update $2BF
#cmd LNEngine_Run $2C0
#cmd LNEngine_Time $2C2
#cmd LNEngine_GetCamera $2C3
#cmd LNEngine_GetMainLight $2C4
#cmd LNEngine_GetRenderView $2C5
#cmd LNApplication_Create $2CA
#cmd LNApplication_OnInit $2C7
#cmd LNApplication_OnUpdate $2C8
#cmd LNApplication_World $2C9
#cmd LNApplication_SetPrototype_OnSerialize $7D5
#cmd LNApplication_SetPrototype_OnInit $7E0
#cmd LNApplication_SetPrototype_OnUpdate $7EB
#cmd LNDebug_Print $2CC
#cmd LNDebug_PrintWithTime $2CE
#cmd LNDebug_PrintWithTimeAndColor $2D1
#cmd LNObjectSerializeHandler_Create $58B
#cmd LNEventConnectionSerializeHandler_Create $596
#cmd LNZVTestClass1SerializeHandler_Create $5A1
#cmd LNZVTestEventArgs1SerializeHandler_Create $5AC
#cmd LNSerializer2SerializeHandler_Create $5B7
#cmd LNAssetModelSerializeHandler_Create $5C2
#cmd LNTextureSerializeHandler_Create $5CD
#cmd LNTexture2DSerializeHandler_Create $5D8
#cmd LNRenderViewSerializeHandler_Create $5E3
#cmd LNComponentSerializeHandler_Create $5EE
#cmd LNVisualComponentSerializeHandler_Create $5F9
#cmd LNSpriteComponentSerializeHandler_Create $604
#cmd LNWorldSerializeHandler_Create $60F
#cmd LNComponentListSerializeHandler_Create $61A
#cmd LNWorldObjectSerializeHandler_Create $625
#cmd LNWorldObjectUpdateHandler_Create $630
#cmd LNVisualObjectSerializeHandler_Create $63B
#cmd LNVisualObjectUpdateHandler_Create $646
#cmd LNCameraSerializeHandler_Create $651
#cmd LNCameraUpdateHandler_Create $65C
#cmd LNEnvironmentLightSerializeHandler_Create $667
#cmd LNEnvironmentLightUpdateHandler_Create $672
#cmd LNDirectionalLightSerializeHandler_Create $67D
#cmd LNDirectionalLightUpdateHandler_Create $688
#cmd LNPointLightSerializeHandler_Create $693
#cmd LNPointLightUpdateHandler_Create $69E
#cmd LNSpotLightSerializeHandler_Create $6A9
#cmd LNSpotLightUpdateHandler_Create $6B4
#cmd LNSpriteSerializeHandler_Create $6BF
#cmd LNSpriteUpdateHandler_Create $6CA
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $6D5
#cmd LNRaycasterSerializeHandler_Create $6E0
#cmd LNRaycastResultSerializeHandler_Create $6EB
#cmd LNWorldRenderViewSerializeHandler_Create $6F6
#cmd LNBoxMeshSerializeHandler_Create $701
#cmd LNBoxMeshUpdateHandler_Create $70C
#cmd LNPlaneMeshSerializeHandler_Create $717
#cmd LNPlaneMeshUpdateHandler_Create $722
#cmd LNLevelSerializeHandler_Create $72D
#cmd LNLevelStartHandler_Create $738
#cmd LNLevelStopHandler_Create $743
#cmd LNLevelPauseHandler_Create $74E
#cmd LNLevelResumeHandler_Create $759
#cmd LNLevelUpdateHandler_Create $764
#cmd LNUIEventArgsSerializeHandler_Create $76F
#cmd LNUILayoutElementSerializeHandler_Create $77A
#cmd LNUIElementSerializeHandler_Create $785
#cmd LNUITextBlockSerializeHandler_Create $790
#cmd LNUISpriteSerializeHandler_Create $79B
#cmd LNInterpreterCommandSerializeHandler_Create $7A6
#cmd LNInterpreterCommandListSerializeHandler_Create $7B1
#cmd LNInterpreterSerializeHandler_Create $7BC
#cmd LNInterpreterUpdateWaitHandler_Create $7C8
#cmd LNApplicationSerializeHandler_Create $7D3
#cmd LNApplicationInitHandler_Create $7DE
#cmd LNApplicationUpdateHandler_Create $7E9



#cmd ln_args $1

#endif // __lumino__
