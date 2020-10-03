
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
#cmd LNObject_SetPrototype_OnSerialize $5B4
#cmd LNEventConnection_SetPrototype_OnSerialize $5BF
#cmd LNPromiseFailureDelegate_Create $36B
#cmd LNZVTestDelegate1_Create $370
#cmd LNZVTestDelegate2_Create $376
#cmd LNZVTestDelegate3_Create $37B
#cmd LNZVTestEventHandler1_Create $380
#cmd LNZVTestEventHandler2_Create $385
#cmd LNZVTestPromise1_ThenWith $388
#cmd LNZVTestPromise1_CatchWith $38A
#cmd LNZVTestPromise2_ThenWith $38E
#cmd LNZVTestPromise2_CatchWith $390
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $5CA
#cmd LNZVTestEventArgs1_Create $A1
#cmd LNZVTestEventArgs1_CreateWithValue $A2
#cmd LNZVTestEventArgs1_GetValue $A0
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $5D5
#cmd LNSerializer2_SetPrototype_OnSerialize $5E0
#cmd LNAssetModel_Create $AE
#cmd LNAssetModel_Target $AD
#cmd LNAssetModel_SetPrototype_OnSerialize $5EB
#cmd LNAssets_SaveAssetToLocalFile $B1
#cmd LNAssets_LoadAssetFromLocalFile $B4
#cmd LNAssets_LoadAsset $B6
#cmd LNAssets_ReloadAsset $B8
#cmd LNTexture2DDelegate_Create $3B4
#cmd LNTexture2DPromise_ThenWith $3B7
#cmd LNTexture2DPromise_CatchWith $3B9
#cmd LNTexture_SetPrototype_OnSerialize $5F6
#cmd LNTexture2D_Create $10A
#cmd LNTexture2D_CreateWithFormat $10D
#cmd LNTexture2D_Load $106
#cmd LNTexture2D_LoadEmoji $108
#cmd LNTexture2D_SetPrototype_OnSerialize $601
#cmd LNRenderView_SetPrototype_OnSerialize $60C
#cmd LNSkinnedMeshModel_Load $119
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $617
#cmd LNComponent_SetPrototype_OnSerialize $622
#cmd LNVisualComponent_SetVisible $11D
#cmd LNVisualComponent_IsVisible $11F
#cmd LNVisualComponent_SetPrototype_OnSerialize $62D
#cmd LNSpriteComponent_SetTexture $121
#cmd LNSpriteComponent_SetPrototype_OnSerialize $638
#cmd LNCharacterController_SetHeight $128
#cmd LNCharacterController_GetHeight $12A
#cmd LNCharacterController_SetCameraRadius $12B
#cmd LNCharacterController_GetCameraRadius $12D
#cmd LNCharacterController_SetPrototype_OnSerialize $643
#cmd LNWorld_Add $12F
#cmd LNWorld_SetPrototype_OnSerialize $64E
#cmd LNComponentList_GetLength $453
#cmd LNComponentList_GetItem $455
#cmd LNComponentList_SetPrototype_OnSerialize $659
#cmd LNWorldObject_Create $164
#cmd LNWorldObject_SetPosition $133
#cmd LNWorldObject_SetPositionXYZ $135
#cmd LNWorldObject_GetPosition $139
#cmd LNWorldObject_SetRotationQuaternion $13A
#cmd LNWorldObject_SetRotation $13C
#cmd LNWorldObject_GetRotation $140
#cmd LNWorldObject_SetScale $141
#cmd LNWorldObject_SetScaleS $143
#cmd LNWorldObject_SetScaleXYZ $145
#cmd LNWorldObject_GetScale $149
#cmd LNWorldObject_SetCenterPoint $14A
#cmd LNWorldObject_SetCenterPointXYZ $14C
#cmd LNWorldObject_GetCenterPoint $150
#cmd LNWorldObject_LookAt $151
#cmd LNWorldObject_LookAtXYZ $153
#cmd LNWorldObject_AddComponent $157
#cmd LNWorldObject_RemoveComponent $159
#cmd LNWorldObject_AddTag $15B
#cmd LNWorldObject_RemoveTag $15D
#cmd LNWorldObject_HasTag $15F
#cmd LNWorldObject_GetComponents $161
#cmd LNWorldObject_SetPrototype_OnSerialize $664
#cmd LNWorldObject_SetPrototype_OnUpdate $66F
#cmd LNVisualObject_SetVisible $166
#cmd LNVisualObject_IsVisible $168
#cmd LNVisualObject_SetPrototype_OnSerialize $67A
#cmd LNVisualObject_SetPrototype_OnUpdate $685
#cmd LNCamera_SetPrototype_OnSerialize $690
#cmd LNCamera_SetPrototype_OnUpdate $69B
#cmd LNEnvironmentLight_SetEnabled $16B
#cmd LNEnvironmentLight_IsEnabled $16D
#cmd LNEnvironmentLight_SetColor $16E
#cmd LNEnvironmentLight_GetColor $170
#cmd LNEnvironmentLight_SetAmbientColor $171
#cmd LNEnvironmentLight_GetAmbientColor $173
#cmd LNEnvironmentLight_GetSkyColor $174
#cmd LNEnvironmentLight_SetSkyColor $175
#cmd LNEnvironmentLight_GetGroundColor $177
#cmd LNEnvironmentLight_SetGroundColor $178
#cmd LNEnvironmentLight_SetIntensity $17A
#cmd LNEnvironmentLight_GetIntensity $17C
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $17D
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $17F
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $180
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $182
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $6A6
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $6B1
#cmd LNDirectionalLight_Create $193
#cmd LNDirectionalLight_CreateWithColor $194
#cmd LNDirectionalLight_SetEnabled $184
#cmd LNDirectionalLight_IsEnabled $186
#cmd LNDirectionalLight_SetColor $187
#cmd LNDirectionalLight_GetColor $189
#cmd LNDirectionalLight_SetIntensity $18A
#cmd LNDirectionalLight_GetIntensity $18C
#cmd LNDirectionalLight_SetShadowEffectiveDistance $18D
#cmd LNDirectionalLight_GetShadowEffectiveDistance $18F
#cmd LNDirectionalLight_SetShadowEffectiveDepth $190
#cmd LNDirectionalLight_GetShadowEffectiveDepth $192
#cmd LNDirectionalLight_SetPrototype_OnSerialize $6BC
#cmd LNDirectionalLight_SetPrototype_OnUpdate $6C7
#cmd LNPointLight_Create $1A6
#cmd LNPointLight_CreateWithColorAndRange $1A7
#cmd LNPointLight_SetEnabled $197
#cmd LNPointLight_IsEnabled $199
#cmd LNPointLight_SetColor $19A
#cmd LNPointLight_GetColor $19C
#cmd LNPointLight_SetIntensity $19D
#cmd LNPointLight_GetIntensity $19F
#cmd LNPointLight_SetRange $1A0
#cmd LNPointLight_GetRange $1A2
#cmd LNPointLight_SetAttenuation $1A3
#cmd LNPointLight_GetAttenuation $1A5
#cmd LNPointLight_SetPrototype_OnSerialize $6D2
#cmd LNPointLight_SetPrototype_OnUpdate $6DD
#cmd LNSpotLight_Create $1C0
#cmd LNSpotLight_CreateWithColorAndRange $1C1
#cmd LNSpotLight_SetEnabled $1AB
#cmd LNSpotLight_IsEnabled $1AD
#cmd LNSpotLight_SetColor $1AE
#cmd LNSpotLight_GetColor $1B0
#cmd LNSpotLight_SetIntensity $1B1
#cmd LNSpotLight_GetIntensity $1B3
#cmd LNSpotLight_SetRange $1B4
#cmd LNSpotLight_GetRange $1B6
#cmd LNSpotLight_SetAttenuation $1B7
#cmd LNSpotLight_GetAttenuation $1B9
#cmd LNSpotLight_SetAngle $1BA
#cmd LNSpotLight_GetAngle $1BC
#cmd LNSpotLight_SetPenumbra $1BD
#cmd LNSpotLight_GetPenumbra $1BF
#cmd LNSpotLight_SetPrototype_OnSerialize $6E8
#cmd LNSpotLight_SetPrototype_OnUpdate $6F3
#cmd LNTestDelegate_Create $4F8
#cmd LNSprite_Create $1D7
#cmd LNSprite_CreateWithTexture $1D8
#cmd LNSprite_CreateWithTextureAndSize $1DA
#cmd LNSprite_SetTexture $1C9
#cmd LNSprite_SetSize $1CB
#cmd LNSprite_SetSizeWH $1CD
#cmd LNSprite_SetSourceRectXYWH $1D0
#cmd LNSprite_SetCallerTest $1D5
#cmd LNSprite_SetPrototype_OnSerialize $6FE
#cmd LNSprite_SetPrototype_OnUpdate $709
#cmd LNCameraOrbitControlComponent_Create $1DF
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $714
#cmd LNRaycaster_FromScreen $1E1
#cmd LNRaycaster_IntersectPlane $1E3
#cmd LNRaycaster_SetPrototype_OnSerialize $71F
#cmd LNRaycastResult_GetPoint $1E8
#cmd LNRaycastResult_SetPrototype_OnSerialize $72A
#cmd LNWorldRenderView_SetGuideGridEnabled $1EA
#cmd LNWorldRenderView_GetGuideGridEnabled $1EC
#cmd LNWorldRenderView_SetPrototype_OnSerialize $735
#cmd LNBoxMesh_Create $1EE
#cmd LNBoxMesh_SetPrototype_OnSerialize $740
#cmd LNBoxMesh_SetPrototype_OnUpdate $74B
#cmd LNPlaneMesh_Create $1F0
#cmd LNPlaneMesh_SetPrototype_OnSerialize $756
#cmd LNPlaneMesh_SetPrototype_OnUpdate $761
#cmd LNSkinnedMeshComponent_Create $1F4
#cmd LNSkinnedMeshComponent_SetModel $1F2
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $76C
#cmd LNScene_SetClearMode $1F6
#cmd LNScene_SetSkyColor $1F8
#cmd LNScene_SetSkyHorizonColor $1FA
#cmd LNScene_SetSkyCloudColor $1FC
#cmd LNScene_SetSkyOverlayColor $1FE
#cmd LNScene_GotoLevel $200
#cmd LNScene_ActiveLevel $203
#cmd LNScene_IsTransitionEffectRunning $204
#cmd LNScene_SetTransitionEffectMode $205
#cmd LNScene_TransitionEffectMode $207
#cmd LNScene_SetTransitionDuration $208
#cmd LNScene_TransitionDuration $20A
#cmd LNScene_SetTransitionEffectColor $20B
#cmd LNScene_TransitionEffectColor $20D
#cmd LNScene_SetTransitionEffectMaskTexture $20E
#cmd LNScene_TransitionEffectMaskTexture $210
#cmd LNScene_SetTransitionEffectVague $211
#cmd LNScene_TransitionEffectVague $213
#cmd LNScene_StartFadeOut $214
#cmd LNScene_StartFadeIn $215
#cmd LNLevel_Create $221
#cmd LNLevel_AddSubLevel $217
#cmd LNLevel_RemoveSubLevel $219
#cmd LNLevel_RemoveAllSubLevels $21B
#cmd LNLevel_SetPrototype_OnSerialize $777
#cmd LNLevel_SetPrototype_OnStart $782
#cmd LNLevel_SetPrototype_OnStop $78D
#cmd LNLevel_SetPrototype_OnPause $798
#cmd LNLevel_SetPrototype_OnResume $7A3
#cmd LNLevel_SetPrototype_OnUpdate $7AE
#cmd LNUIEventArgs_Sender $223
#cmd LNUIEventArgs_SetPrototype_OnSerialize $7B9
#cmd LNUIGeneralEventHandler_Create $52A
#cmd LNUIEventHandler_Create $52F
#cmd LNUILayoutElement_SetPrototype_OnSerialize $7C4
#cmd LNUIElement_SetMargin $235
#cmd LNUIElement_GetMargin $237
#cmd LNUIElement_SetPadding $238
#cmd LNUIElement_GetPadding $23A
#cmd LNUIElement_SetHAlignment $23B
#cmd LNUIElement_GetHAlignment $23D
#cmd LNUIElement_SetVAlignment $23E
#cmd LNUIElement_GetVAlignment $240
#cmd LNUIElement_SetAlignments $241
#cmd LNUIElement_SetPosition $244
#cmd LNUIElement_SetPositionXYZ $246
#cmd LNUIElement_GetPosition $24A
#cmd LNUIElement_SetRotation $24B
#cmd LNUIElement_SetEulerAngles $24D
#cmd LNUIElement_GetRotation $251
#cmd LNUIElement_SetScale $252
#cmd LNUIElement_SetScaleS $254
#cmd LNUIElement_SetScaleXY $256
#cmd LNUIElement_GetScale $259
#cmd LNUIElement_SetCenterPoint $25A
#cmd LNUIElement_SetCenterPointXYZ $25C
#cmd LNUIElement_GetCenterPoint $260
#cmd LNUIElement_AddChild $261
#cmd LNUIElement_SetPrototype_OnSerialize $7CF
#cmd LNUITextBlock_Create $264
#cmd LNUITextBlock_CreateWithText $265
#cmd LNUITextBlock_SetPrototype_OnSerialize $7DA
#cmd LNUISprite_Create $272
#cmd LNUISprite_CreateWithTexture $273
#cmd LNUISprite_SetTexture $268
#cmd LNUISprite_SetSourceRect $26A
#cmd LNUISprite_SetSourceRectXYWH $26C
#cmd LNUISprite_GetSourceRect $271
#cmd LNUISprite_SetPrototype_OnSerialize $7E5
#cmd LNInput_Pressed $276
#cmd LNInput_Triggered $278
#cmd LNInput_TriggeredOff $27A
#cmd LNInput_Repeated $27C
#cmd LNInput_GetAxisValue $27E
#cmd LNInput_ClearAllBindings $280
#cmd LNMouse_Pressed $282
#cmd LNMouse_Triggered $284
#cmd LNMouse_TriggeredOff $286
#cmd LNMouse_Repeated $288
#cmd LNMouse_Position $28A
#cmd LNInterpreterCommand_Code $28C
#cmd LNInterpreterCommand_ParamsCount $28D
#cmd LNInterpreterCommand_Param $28E
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $7F0
#cmd LNInterpreterCommandList_Create $2A5
#cmd LNInterpreterCommandList_AddCommand $291
#cmd LNInterpreterCommandList_AddCommand1 $293
#cmd LNInterpreterCommandList_AddCommand2 $296
#cmd LNInterpreterCommandList_AddCommand3 $29A
#cmd LNInterpreterCommandList_AddCommand4 $29F
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $7FB
#cmd LNInterpreterCommandDelegate_Create $588
#cmd LNInterpreter_Create $2B9
#cmd LNInterpreter_Clear $2AA
#cmd LNInterpreter_Run $2AB
#cmd LNInterpreter_IsRunning $2AD
#cmd LNInterpreter_Update $2AE
#cmd LNInterpreter_Terminate $2AF
#cmd LNInterpreter_RegisterCommandHandler $2B0
#cmd LNInterpreter_SetWaitMode $2B3
#cmd LNInterpreter_GetWaitMode $2B5
#cmd LNInterpreter_SetWaitCount $2B6
#cmd LNInterpreter_GetWaitCount $2B8
#cmd LNInterpreter_SetPrototype_OnSerialize $806
#cmd LNInterpreter_SetPrototype_OnUpdateWait $812
#cmd LNEngineSettings_SetMainWindowSize $2BC
#cmd LNEngineSettings_SetMainWorldViewSize $2BF
#cmd LNEngineSettings_SetMainWindowTitle $2C2
#cmd LNEngineSettings_AddAssetDirectory $2C4
#cmd LNEngineSettings_AddAssetArchive $2C6
#cmd LNEngineSettings_SetFrameRate $2C9
#cmd LNEngineSettings_SetDebugToolEnabled $2CB
#cmd LNEngineSettings_SetEngineLogEnabled $2CD
#cmd LNEngineSettings_SetEngineLogFilePath $2CF
#cmd LNEngine_Initialize $2D2
#cmd LNEngine_Finalize $2D3
#cmd LNEngine_Update $2D4
#cmd LNEngine_Run $2D5
#cmd LNEngine_Time $2D7
#cmd LNEngine_GetCamera $2D8
#cmd LNEngine_GetMainLight $2D9
#cmd LNEngine_GetRenderView $2DA
#cmd LNApplication_Create $2DF
#cmd LNApplication_OnInit $2DC
#cmd LNApplication_OnUpdate $2DD
#cmd LNApplication_World $2DE
#cmd LNApplication_SetPrototype_OnSerialize $81D
#cmd LNApplication_SetPrototype_OnInit $828
#cmd LNApplication_SetPrototype_OnUpdate $833
#cmd LNDebug_Print $2E1
#cmd LNDebug_PrintWithTime $2E3
#cmd LNDebug_PrintWithTimeAndColor $2E6
#cmd LNObjectSerializeHandler_Create $5B2
#cmd LNEventConnectionSerializeHandler_Create $5BD
#cmd LNZVTestClass1SerializeHandler_Create $5C8
#cmd LNZVTestEventArgs1SerializeHandler_Create $5D3
#cmd LNSerializer2SerializeHandler_Create $5DE
#cmd LNAssetModelSerializeHandler_Create $5E9
#cmd LNTextureSerializeHandler_Create $5F4
#cmd LNTexture2DSerializeHandler_Create $5FF
#cmd LNRenderViewSerializeHandler_Create $60A
#cmd LNSkinnedMeshModelSerializeHandler_Create $615
#cmd LNComponentSerializeHandler_Create $620
#cmd LNVisualComponentSerializeHandler_Create $62B
#cmd LNSpriteComponentSerializeHandler_Create $636
#cmd LNCharacterControllerSerializeHandler_Create $641
#cmd LNWorldSerializeHandler_Create $64C
#cmd LNComponentListSerializeHandler_Create $657
#cmd LNWorldObjectSerializeHandler_Create $662
#cmd LNWorldObjectUpdateHandler_Create $66D
#cmd LNVisualObjectSerializeHandler_Create $678
#cmd LNVisualObjectUpdateHandler_Create $683
#cmd LNCameraSerializeHandler_Create $68E
#cmd LNCameraUpdateHandler_Create $699
#cmd LNEnvironmentLightSerializeHandler_Create $6A4
#cmd LNEnvironmentLightUpdateHandler_Create $6AF
#cmd LNDirectionalLightSerializeHandler_Create $6BA
#cmd LNDirectionalLightUpdateHandler_Create $6C5
#cmd LNPointLightSerializeHandler_Create $6D0
#cmd LNPointLightUpdateHandler_Create $6DB
#cmd LNSpotLightSerializeHandler_Create $6E6
#cmd LNSpotLightUpdateHandler_Create $6F1
#cmd LNSpriteSerializeHandler_Create $6FC
#cmd LNSpriteUpdateHandler_Create $707
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $712
#cmd LNRaycasterSerializeHandler_Create $71D
#cmd LNRaycastResultSerializeHandler_Create $728
#cmd LNWorldRenderViewSerializeHandler_Create $733
#cmd LNBoxMeshSerializeHandler_Create $73E
#cmd LNBoxMeshUpdateHandler_Create $749
#cmd LNPlaneMeshSerializeHandler_Create $754
#cmd LNPlaneMeshUpdateHandler_Create $75F
#cmd LNSkinnedMeshComponentSerializeHandler_Create $76A
#cmd LNLevelSerializeHandler_Create $775
#cmd LNLevelStartHandler_Create $780
#cmd LNLevelStopHandler_Create $78B
#cmd LNLevelPauseHandler_Create $796
#cmd LNLevelResumeHandler_Create $7A1
#cmd LNLevelUpdateHandler_Create $7AC
#cmd LNUIEventArgsSerializeHandler_Create $7B7
#cmd LNUILayoutElementSerializeHandler_Create $7C2
#cmd LNUIElementSerializeHandler_Create $7CD
#cmd LNUITextBlockSerializeHandler_Create $7D8
#cmd LNUISpriteSerializeHandler_Create $7E3
#cmd LNInterpreterCommandSerializeHandler_Create $7EE
#cmd LNInterpreterCommandListSerializeHandler_Create $7F9
#cmd LNInterpreterSerializeHandler_Create $804
#cmd LNInterpreterUpdateWaitHandler_Create $810
#cmd LNApplicationSerializeHandler_Create $81B
#cmd LNApplicationInitHandler_Create $826
#cmd LNApplicationUpdateHandler_Create $831



#cmd ln_args $1

#endif // __lumino__
