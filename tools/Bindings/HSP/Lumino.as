
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
#cmd LNObject_SetPrototype_OnSerialize $585
#cmd LNEventConnection_SetPrototype_OnSerialize $590
#cmd LNPromiseFailureDelegate_Create $351
#cmd LNZVTestDelegate1_Create $356
#cmd LNZVTestDelegate2_Create $35C
#cmd LNZVTestDelegate3_Create $361
#cmd LNZVTestEventHandler1_Create $366
#cmd LNZVTestEventHandler2_Create $36B
#cmd LNZVTestPromise1_ThenWith $36E
#cmd LNZVTestPromise1_CatchWith $370
#cmd LNZVTestPromise2_ThenWith $374
#cmd LNZVTestPromise2_CatchWith $376
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $59B
#cmd LNZVTestEventArgs1_Create $A1
#cmd LNZVTestEventArgs1_CreateWithValue $A2
#cmd LNZVTestEventArgs1_GetValue $A0
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $5A6
#cmd LNSerializer2_SetPrototype_OnSerialize $5B1
#cmd LNAssetModel_Create $AE
#cmd LNAssetModel_Target $AD
#cmd LNAssetModel_SetPrototype_OnSerialize $5BC
#cmd LNAssets_SaveAssetToLocalFile $B1
#cmd LNAssets_LoadAssetFromLocalFile $B4
#cmd LNAssets_LoadAsset $B6
#cmd LNAssets_ReloadAsset $B8
#cmd LNTexture2DDelegate_Create $39A
#cmd LNTexture2DPromise_ThenWith $39D
#cmd LNTexture2DPromise_CatchWith $39F
#cmd LNTexture_SetPrototype_OnSerialize $5C7
#cmd LNTexture2D_Create $10A
#cmd LNTexture2D_CreateWithFormat $10D
#cmd LNTexture2D_Load $106
#cmd LNTexture2D_LoadEmoji $108
#cmd LNTexture2D_SetPrototype_OnSerialize $5D2
#cmd LNRenderView_SetPrototype_OnSerialize $5DD
#cmd LNComponent_SetPrototype_OnSerialize $5E8
#cmd LNVisualComponent_SetVisible $11A
#cmd LNVisualComponent_IsVisible $11C
#cmd LNVisualComponent_SetPrototype_OnSerialize $5F3
#cmd LNSpriteComponent_SetTexture $11E
#cmd LNSpriteComponent_SetPrototype_OnSerialize $5FE
#cmd LNWorld_Add $125
#cmd LNWorld_SetPrototype_OnSerialize $609
#cmd LNComponentList_GetLength $42E
#cmd LNComponentList_GetItem $430
#cmd LNComponentList_SetPrototype_OnSerialize $614
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
#cmd LNWorldObject_SetPrototype_OnSerialize $61F
#cmd LNWorldObject_SetPrototype_OnUpdate $62A
#cmd LNVisualObject_SetVisible $155
#cmd LNVisualObject_IsVisible $157
#cmd LNVisualObject_SetPrototype_OnSerialize $635
#cmd LNVisualObject_SetPrototype_OnUpdate $640
#cmd LNCamera_SetPrototype_OnSerialize $64B
#cmd LNCamera_SetPrototype_OnUpdate $656
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
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $661
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $66C
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
#cmd LNDirectionalLight_SetPrototype_OnSerialize $677
#cmd LNDirectionalLight_SetPrototype_OnUpdate $682
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
#cmd LNPointLight_SetPrototype_OnSerialize $68D
#cmd LNPointLight_SetPrototype_OnUpdate $698
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
#cmd LNSpotLight_SetPrototype_OnSerialize $6A3
#cmd LNSpotLight_SetPrototype_OnUpdate $6AE
#cmd LNTestDelegate_Create $4CE
#cmd LNSprite_Create $1C6
#cmd LNSprite_CreateWithTexture $1C7
#cmd LNSprite_CreateWithTextureAndSize $1C9
#cmd LNSprite_SetTexture $1B8
#cmd LNSprite_SetSize $1BA
#cmd LNSprite_SetSizeWH $1BC
#cmd LNSprite_SetSourceRectXYWH $1BF
#cmd LNSprite_SetCallerTest $1C4
#cmd LNSprite_SetPrototype_OnSerialize $6B9
#cmd LNSprite_SetPrototype_OnUpdate $6C4
#cmd LNCameraOrbitControlComponent_Create $1CE
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $6CF
#cmd LNRaycaster_FromScreen $1D0
#cmd LNRaycaster_IntersectPlane $1D2
#cmd LNRaycaster_SetPrototype_OnSerialize $6DA
#cmd LNRaycastResult_GetPoint $1D7
#cmd LNRaycastResult_SetPrototype_OnSerialize $6E5
#cmd LNWorldRenderView_SetGuideGridEnabled $1D9
#cmd LNWorldRenderView_GetGuideGridEnabled $1DB
#cmd LNWorldRenderView_SetPrototype_OnSerialize $6F0
#cmd LNBoxMesh_Create $1DD
#cmd LNBoxMesh_SetPrototype_OnSerialize $6FB
#cmd LNBoxMesh_SetPrototype_OnUpdate $706
#cmd LNPlaneMesh_Create $1DF
#cmd LNPlaneMesh_SetPrototype_OnSerialize $711
#cmd LNPlaneMesh_SetPrototype_OnUpdate $71C
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
#cmd LNLevel_Create $207
#cmd LNLevel_SetPrototype_OnSerialize $727
#cmd LNLevel_SetPrototype_OnStart $732
#cmd LNLevel_SetPrototype_OnStop $73D
#cmd LNLevel_SetPrototype_OnPause $748
#cmd LNLevel_SetPrototype_OnResume $753
#cmd LNLevel_SetPrototype_OnUpdate $75E
#cmd LNUIEventArgs_Sender $209
#cmd LNUIEventArgs_SetPrototype_OnSerialize $769
#cmd LNUIGeneralEventHandler_Create $4FB
#cmd LNUIEventHandler_Create $500
#cmd LNUILayoutElement_SetPrototype_OnSerialize $774
#cmd LNUIElement_SetMargin $21B
#cmd LNUIElement_GetMargin $21D
#cmd LNUIElement_SetPadding $21E
#cmd LNUIElement_GetPadding $220
#cmd LNUIElement_SetHAlignment $221
#cmd LNUIElement_GetHAlignment $223
#cmd LNUIElement_SetVAlignment $224
#cmd LNUIElement_GetVAlignment $226
#cmd LNUIElement_SetAlignments $227
#cmd LNUIElement_SetPosition $22A
#cmd LNUIElement_SetPositionXYZ $22C
#cmd LNUIElement_GetPosition $230
#cmd LNUIElement_SetRotation $231
#cmd LNUIElement_SetEulerAngles $233
#cmd LNUIElement_GetRotation $237
#cmd LNUIElement_SetScale $238
#cmd LNUIElement_SetScaleS $23A
#cmd LNUIElement_SetScaleXY $23C
#cmd LNUIElement_GetScale $23F
#cmd LNUIElement_SetCenterPoint $240
#cmd LNUIElement_SetCenterPointXYZ $242
#cmd LNUIElement_GetCenterPoint $246
#cmd LNUIElement_AddChild $247
#cmd LNUIElement_SetPrototype_OnSerialize $77F
#cmd LNUITextBlock_Create $24A
#cmd LNUITextBlock_CreateWithText $24B
#cmd LNUITextBlock_SetPrototype_OnSerialize $78A
#cmd LNUISprite_Create $258
#cmd LNUISprite_CreateWithTexture $259
#cmd LNUISprite_SetTexture $24E
#cmd LNUISprite_SetSourceRect $250
#cmd LNUISprite_SetSourceRectXYWH $252
#cmd LNUISprite_GetSourceRect $257
#cmd LNUISprite_SetPrototype_OnSerialize $795
#cmd LNInput_Pressed $25C
#cmd LNInput_Triggered $25E
#cmd LNInput_TriggeredOff $260
#cmd LNInput_Repeated $262
#cmd LNInput_GetAxisValue $264
#cmd LNInput_ClearAllBindings $266
#cmd LNMouse_Pressed $268
#cmd LNMouse_Triggered $26A
#cmd LNMouse_TriggeredOff $26C
#cmd LNMouse_Repeated $26E
#cmd LNMouse_Position $270
#cmd LNInterpreterCommand_Code $272
#cmd LNInterpreterCommand_ParamsCount $273
#cmd LNInterpreterCommand_Param $274
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $7A0
#cmd LNInterpreterCommandList_Create $28B
#cmd LNInterpreterCommandList_AddCommand $277
#cmd LNInterpreterCommandList_AddCommand1 $279
#cmd LNInterpreterCommandList_AddCommand2 $27C
#cmd LNInterpreterCommandList_AddCommand3 $280
#cmd LNInterpreterCommandList_AddCommand4 $285
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $7AB
#cmd LNInterpreterCommandDelegate_Create $559
#cmd LNInterpreter_Create $29F
#cmd LNInterpreter_Clear $290
#cmd LNInterpreter_Run $291
#cmd LNInterpreter_IsRunning $293
#cmd LNInterpreter_Update $294
#cmd LNInterpreter_Terminate $295
#cmd LNInterpreter_RegisterCommandHandler $296
#cmd LNInterpreter_SetWaitMode $299
#cmd LNInterpreter_GetWaitMode $29B
#cmd LNInterpreter_SetWaitCount $29C
#cmd LNInterpreter_GetWaitCount $29E
#cmd LNInterpreter_SetPrototype_OnSerialize $7B6
#cmd LNInterpreter_SetPrototype_OnUpdateWait $7C2
#cmd LNEngineSettings_SetMainWindowSize $2A2
#cmd LNEngineSettings_SetMainWorldViewSize $2A5
#cmd LNEngineSettings_SetMainWindowTitle $2A8
#cmd LNEngineSettings_AddAssetDirectory $2AA
#cmd LNEngineSettings_AddAssetArchive $2AC
#cmd LNEngineSettings_SetFrameRate $2AF
#cmd LNEngineSettings_SetDebugToolEnabled $2B1
#cmd LNEngineSettings_SetEngineLogEnabled $2B3
#cmd LNEngineSettings_SetEngineLogFilePath $2B5
#cmd LNEngine_Initialize $2B8
#cmd LNEngine_Finalize $2B9
#cmd LNEngine_Update $2BA
#cmd LNEngine_Run $2BB
#cmd LNEngine_Time $2BD
#cmd LNEngine_GetCamera $2BE
#cmd LNEngine_GetMainLight $2BF
#cmd LNEngine_GetRenderView $2C0
#cmd LNApplication_Create $2C5
#cmd LNApplication_OnInit $2C2
#cmd LNApplication_OnUpdate $2C3
#cmd LNApplication_World $2C4
#cmd LNApplication_SetPrototype_OnSerialize $7CD
#cmd LNApplication_SetPrototype_OnInit $7D8
#cmd LNApplication_SetPrototype_OnUpdate $7E3
#cmd LNDebug_Print $2C7
#cmd LNDebug_PrintWithTime $2C9
#cmd LNDebug_PrintWithTimeAndColor $2CC
#cmd LNObjectSerializeHandler_Create $583
#cmd LNEventConnectionSerializeHandler_Create $58E
#cmd LNZVTestClass1SerializeHandler_Create $599
#cmd LNZVTestEventArgs1SerializeHandler_Create $5A4
#cmd LNSerializer2SerializeHandler_Create $5AF
#cmd LNAssetModelSerializeHandler_Create $5BA
#cmd LNTextureSerializeHandler_Create $5C5
#cmd LNTexture2DSerializeHandler_Create $5D0
#cmd LNRenderViewSerializeHandler_Create $5DB
#cmd LNComponentSerializeHandler_Create $5E6
#cmd LNVisualComponentSerializeHandler_Create $5F1
#cmd LNSpriteComponentSerializeHandler_Create $5FC
#cmd LNWorldSerializeHandler_Create $607
#cmd LNComponentListSerializeHandler_Create $612
#cmd LNWorldObjectSerializeHandler_Create $61D
#cmd LNWorldObjectUpdateHandler_Create $628
#cmd LNVisualObjectSerializeHandler_Create $633
#cmd LNVisualObjectUpdateHandler_Create $63E
#cmd LNCameraSerializeHandler_Create $649
#cmd LNCameraUpdateHandler_Create $654
#cmd LNEnvironmentLightSerializeHandler_Create $65F
#cmd LNEnvironmentLightUpdateHandler_Create $66A
#cmd LNDirectionalLightSerializeHandler_Create $675
#cmd LNDirectionalLightUpdateHandler_Create $680
#cmd LNPointLightSerializeHandler_Create $68B
#cmd LNPointLightUpdateHandler_Create $696
#cmd LNSpotLightSerializeHandler_Create $6A1
#cmd LNSpotLightUpdateHandler_Create $6AC
#cmd LNSpriteSerializeHandler_Create $6B7
#cmd LNSpriteUpdateHandler_Create $6C2
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $6CD
#cmd LNRaycasterSerializeHandler_Create $6D8
#cmd LNRaycastResultSerializeHandler_Create $6E3
#cmd LNWorldRenderViewSerializeHandler_Create $6EE
#cmd LNBoxMeshSerializeHandler_Create $6F9
#cmd LNBoxMeshUpdateHandler_Create $704
#cmd LNPlaneMeshSerializeHandler_Create $70F
#cmd LNPlaneMeshUpdateHandler_Create $71A
#cmd LNLevelSerializeHandler_Create $725
#cmd LNLevelStartHandler_Create $730
#cmd LNLevelStopHandler_Create $73B
#cmd LNLevelPauseHandler_Create $746
#cmd LNLevelResumeHandler_Create $751
#cmd LNLevelUpdateHandler_Create $75C
#cmd LNUIEventArgsSerializeHandler_Create $767
#cmd LNUILayoutElementSerializeHandler_Create $772
#cmd LNUIElementSerializeHandler_Create $77D
#cmd LNUITextBlockSerializeHandler_Create $788
#cmd LNUISpriteSerializeHandler_Create $793
#cmd LNInterpreterCommandSerializeHandler_Create $79E
#cmd LNInterpreterCommandListSerializeHandler_Create $7A9
#cmd LNInterpreterSerializeHandler_Create $7B4
#cmd LNInterpreterUpdateWaitHandler_Create $7C0
#cmd LNApplicationSerializeHandler_Create $7CB
#cmd LNApplicationInitHandler_Create $7D6
#cmd LNApplicationUpdateHandler_Create $7E1



#cmd ln_args $1

#endif // __lumino__
