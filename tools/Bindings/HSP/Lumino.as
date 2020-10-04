
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
#cmd LNObject_SetPrototype_OnSerialize $5F2
#cmd LNEventConnection_SetPrototype_OnSerialize $5FD
#cmd LNPromiseFailureDelegate_Create $391
#cmd LNZVTestDelegate1_Create $396
#cmd LNZVTestDelegate2_Create $39C
#cmd LNZVTestDelegate3_Create $3A1
#cmd LNZVTestEventHandler1_Create $3A6
#cmd LNZVTestEventHandler2_Create $3AB
#cmd LNZVTestPromise1_ThenWith $3AE
#cmd LNZVTestPromise1_CatchWith $3B0
#cmd LNZVTestPromise2_ThenWith $3B4
#cmd LNZVTestPromise2_CatchWith $3B6
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $608
#cmd LNZVTestEventArgs1_Create $A1
#cmd LNZVTestEventArgs1_CreateWithValue $A2
#cmd LNZVTestEventArgs1_GetValue $A0
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $613
#cmd LNSerializer2_SetPrototype_OnSerialize $61E
#cmd LNAssetModel_Create $AE
#cmd LNAssetModel_Target $AD
#cmd LNAssetModel_SetPrototype_OnSerialize $629
#cmd LNAssets_SaveAssetToLocalFile $B1
#cmd LNAssets_LoadAssetFromLocalFile $B4
#cmd LNAssets_LoadAsset $B6
#cmd LNAssets_ReloadAsset $B8
#cmd LNTexture2DDelegate_Create $3DA
#cmd LNTexture2DPromise_ThenWith $3DD
#cmd LNTexture2DPromise_CatchWith $3DF
#cmd LNTexture_SetPrototype_OnSerialize $634
#cmd LNTexture2D_Create $10A
#cmd LNTexture2D_CreateWithFormat $10D
#cmd LNTexture2D_Load $106
#cmd LNTexture2D_LoadEmoji $108
#cmd LNTexture2D_SetPrototype_OnSerialize $63F
#cmd LNRenderView_SetPrototype_OnSerialize $64A
#cmd LNSkinnedMeshModel_Load $119
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $655
#cmd LNCollisionShape_SetPrototype_OnSerialize $660
#cmd LNBoxCollisionShape_Create $11D
#cmd LNBoxCollisionShape_CreateWHD $11F
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $66B
#cmd LNComponent_SetPrototype_OnSerialize $676
#cmd LNVisualComponent_SetVisible $125
#cmd LNVisualComponent_IsVisible $127
#cmd LNVisualComponent_SetPrototype_OnSerialize $681
#cmd LNSpriteComponent_SetTexture $129
#cmd LNSpriteComponent_SetPrototype_OnSerialize $68C
#cmd LNCollisionEventHandler_Create $473
#cmd LNCharacterController_Create $13F
#cmd LNCharacterController_SetHeight $133
#cmd LNCharacterController_GetHeight $135
#cmd LNCharacterController_SetCameraRadius $136
#cmd LNCharacterController_GetCameraRadius $138
#cmd LNCharacterController_SetCollisionEnter $139
#cmd LNCharacterController_SetCollisionLeave $13B
#cmd LNCharacterController_SetCollisionStay $13D
#cmd LNCharacterController_SetPrototype_OnSerialize $697
#cmd LNWorld_Add $141
#cmd LNWorld_SetPrototype_OnSerialize $6A2
#cmd LNComponentList_GetLength $485
#cmd LNComponentList_GetItem $487
#cmd LNComponentList_SetPrototype_OnSerialize $6AD
#cmd LNWorldObject_Create $177
#cmd LNWorldObject_SetPosition $145
#cmd LNWorldObject_SetPositionXYZ $147
#cmd LNWorldObject_GetPosition $14B
#cmd LNWorldObject_SetRotationQuaternion $14C
#cmd LNWorldObject_SetRotation $14E
#cmd LNWorldObject_GetRotation $152
#cmd LNWorldObject_SetScale $153
#cmd LNWorldObject_SetScaleS $155
#cmd LNWorldObject_SetScaleXYZ $157
#cmd LNWorldObject_GetScale $15B
#cmd LNWorldObject_SetCenterPoint $15C
#cmd LNWorldObject_SetCenterPointXYZ $15E
#cmd LNWorldObject_GetCenterPoint $162
#cmd LNWorldObject_LookAt $163
#cmd LNWorldObject_LookAtXYZ $165
#cmd LNWorldObject_AddComponent $169
#cmd LNWorldObject_RemoveComponent $16B
#cmd LNWorldObject_AddTag $16D
#cmd LNWorldObject_RemoveTag $16F
#cmd LNWorldObject_HasTag $171
#cmd LNWorldObject_Destroy $173
#cmd LNWorldObject_GetComponents $174
#cmd LNWorldObject_SetPrototype_OnSerialize $6B8
#cmd LNWorldObject_SetPrototype_OnUpdate $6C3
#cmd LNVisualObject_SetVisible $179
#cmd LNVisualObject_IsVisible $17B
#cmd LNVisualObject_SetPrototype_OnSerialize $6CE
#cmd LNVisualObject_SetPrototype_OnUpdate $6D9
#cmd LNCamera_SetPrototype_OnSerialize $6E4
#cmd LNCamera_SetPrototype_OnUpdate $6EF
#cmd LNEnvironmentLight_SetEnabled $17E
#cmd LNEnvironmentLight_IsEnabled $180
#cmd LNEnvironmentLight_SetColor $181
#cmd LNEnvironmentLight_GetColor $183
#cmd LNEnvironmentLight_SetAmbientColor $184
#cmd LNEnvironmentLight_GetAmbientColor $186
#cmd LNEnvironmentLight_GetSkyColor $187
#cmd LNEnvironmentLight_SetSkyColor $188
#cmd LNEnvironmentLight_GetGroundColor $18A
#cmd LNEnvironmentLight_SetGroundColor $18B
#cmd LNEnvironmentLight_SetIntensity $18D
#cmd LNEnvironmentLight_GetIntensity $18F
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $190
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $192
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $193
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $195
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $6FA
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $705
#cmd LNDirectionalLight_Create $1A6
#cmd LNDirectionalLight_CreateWithColor $1A7
#cmd LNDirectionalLight_SetEnabled $197
#cmd LNDirectionalLight_IsEnabled $199
#cmd LNDirectionalLight_SetColor $19A
#cmd LNDirectionalLight_GetColor $19C
#cmd LNDirectionalLight_SetIntensity $19D
#cmd LNDirectionalLight_GetIntensity $19F
#cmd LNDirectionalLight_SetShadowEffectiveDistance $1A0
#cmd LNDirectionalLight_GetShadowEffectiveDistance $1A2
#cmd LNDirectionalLight_SetShadowEffectiveDepth $1A3
#cmd LNDirectionalLight_GetShadowEffectiveDepth $1A5
#cmd LNDirectionalLight_SetPrototype_OnSerialize $710
#cmd LNDirectionalLight_SetPrototype_OnUpdate $71B
#cmd LNPointLight_Create $1B9
#cmd LNPointLight_CreateWithColorAndRange $1BA
#cmd LNPointLight_SetEnabled $1AA
#cmd LNPointLight_IsEnabled $1AC
#cmd LNPointLight_SetColor $1AD
#cmd LNPointLight_GetColor $1AF
#cmd LNPointLight_SetIntensity $1B0
#cmd LNPointLight_GetIntensity $1B2
#cmd LNPointLight_SetRange $1B3
#cmd LNPointLight_GetRange $1B5
#cmd LNPointLight_SetAttenuation $1B6
#cmd LNPointLight_GetAttenuation $1B8
#cmd LNPointLight_SetPrototype_OnSerialize $726
#cmd LNPointLight_SetPrototype_OnUpdate $731
#cmd LNSpotLight_Create $1D3
#cmd LNSpotLight_CreateWithColorAndRange $1D4
#cmd LNSpotLight_SetEnabled $1BE
#cmd LNSpotLight_IsEnabled $1C0
#cmd LNSpotLight_SetColor $1C1
#cmd LNSpotLight_GetColor $1C3
#cmd LNSpotLight_SetIntensity $1C4
#cmd LNSpotLight_GetIntensity $1C6
#cmd LNSpotLight_SetRange $1C7
#cmd LNSpotLight_GetRange $1C9
#cmd LNSpotLight_SetAttenuation $1CA
#cmd LNSpotLight_GetAttenuation $1CC
#cmd LNSpotLight_SetAngle $1CD
#cmd LNSpotLight_GetAngle $1CF
#cmd LNSpotLight_SetPenumbra $1D0
#cmd LNSpotLight_GetPenumbra $1D2
#cmd LNSpotLight_SetPrototype_OnSerialize $73C
#cmd LNSpotLight_SetPrototype_OnUpdate $747
#cmd LNTestDelegate_Create $52B
#cmd LNSprite_Create $1EA
#cmd LNSprite_CreateWithTexture $1EB
#cmd LNSprite_CreateWithTextureAndSize $1ED
#cmd LNSprite_SetTexture $1DC
#cmd LNSprite_SetSize $1DE
#cmd LNSprite_SetSizeWH $1E0
#cmd LNSprite_SetSourceRectXYWH $1E3
#cmd LNSprite_SetCallerTest $1E8
#cmd LNSprite_SetPrototype_OnSerialize $752
#cmd LNSprite_SetPrototype_OnUpdate $75D
#cmd LNCameraOrbitControlComponent_Create $1F2
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $768
#cmd LNRaycaster_FromScreen $1F4
#cmd LNRaycaster_IntersectPlane $1F6
#cmd LNRaycaster_SetPrototype_OnSerialize $773
#cmd LNRaycastResult_GetPoint $1FB
#cmd LNRaycastResult_SetPrototype_OnSerialize $77E
#cmd LNWorldRenderView_SetGuideGridEnabled $1FD
#cmd LNWorldRenderView_GetGuideGridEnabled $1FF
#cmd LNWorldRenderView_SetPrototype_OnSerialize $789
#cmd LNBoxMesh_Create $201
#cmd LNBoxMesh_SetPrototype_OnSerialize $794
#cmd LNBoxMesh_SetPrototype_OnUpdate $79F
#cmd LNPlaneMesh_Create $203
#cmd LNPlaneMesh_SetPrototype_OnSerialize $7AA
#cmd LNPlaneMesh_SetPrototype_OnUpdate $7B5
#cmd LNStaticMesh_Load $205
#cmd LNStaticMesh_MakeCollisionBody $207
#cmd LNStaticMesh_SetPrototype_OnSerialize $7C0
#cmd LNStaticMesh_SetPrototype_OnUpdate $7CB
#cmd LNSkinnedMeshComponent_Create $20C
#cmd LNSkinnedMeshComponent_SetModel $20A
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $7D6
#cmd LNCollision_GetWorldObject $20E
#cmd LNCollision_SetPrototype_OnSerialize $7E1
#cmd LNTriggerBodyComponent_Create $212
#cmd LNTriggerBodyComponent_AddCollisionShape $210
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $7EC
#cmd LNScene_SetClearMode $214
#cmd LNScene_SetSkyColor $216
#cmd LNScene_SetSkyHorizonColor $218
#cmd LNScene_SetSkyCloudColor $21A
#cmd LNScene_SetSkyOverlayColor $21C
#cmd LNScene_GotoLevel $21E
#cmd LNScene_ActiveLevel $221
#cmd LNScene_IsTransitionEffectRunning $222
#cmd LNScene_SetTransitionEffectMode $223
#cmd LNScene_TransitionEffectMode $225
#cmd LNScene_SetTransitionDuration $226
#cmd LNScene_TransitionDuration $228
#cmd LNScene_SetTransitionEffectColor $229
#cmd LNScene_TransitionEffectColor $22B
#cmd LNScene_SetTransitionEffectMaskTexture $22C
#cmd LNScene_TransitionEffectMaskTexture $22E
#cmd LNScene_SetTransitionEffectVague $22F
#cmd LNScene_TransitionEffectVague $231
#cmd LNScene_StartFadeOut $232
#cmd LNScene_StartFadeIn $233
#cmd LNLevel_Create $243
#cmd LNLevel_AddObject $235
#cmd LNLevel_RemoveObject $237
#cmd LNLevel_AddSubLevel $239
#cmd LNLevel_RemoveSubLevel $23B
#cmd LNLevel_RemoveAllSubLevels $23D
#cmd LNLevel_SetPrototype_OnSerialize $7F7
#cmd LNLevel_SetPrototype_OnStart $802
#cmd LNLevel_SetPrototype_OnStop $80D
#cmd LNLevel_SetPrototype_OnPause $818
#cmd LNLevel_SetPrototype_OnResume $823
#cmd LNLevel_SetPrototype_OnUpdate $82E
#cmd LNUIEventArgs_Sender $245
#cmd LNUIEventArgs_SetPrototype_OnSerialize $839
#cmd LNUIGeneralEventHandler_Create $566
#cmd LNUIEventHandler_Create $56B
#cmd LNUILayoutElement_SetPrototype_OnSerialize $844
#cmd LNUIElement_SetMargin $257
#cmd LNUIElement_GetMargin $259
#cmd LNUIElement_SetPadding $25A
#cmd LNUIElement_GetPadding $25C
#cmd LNUIElement_SetHAlignment $25D
#cmd LNUIElement_GetHAlignment $25F
#cmd LNUIElement_SetVAlignment $260
#cmd LNUIElement_GetVAlignment $262
#cmd LNUIElement_SetAlignments $263
#cmd LNUIElement_SetPosition $266
#cmd LNUIElement_SetPositionXYZ $268
#cmd LNUIElement_GetPosition $26C
#cmd LNUIElement_SetRotation $26D
#cmd LNUIElement_SetEulerAngles $26F
#cmd LNUIElement_GetRotation $273
#cmd LNUIElement_SetScale $274
#cmd LNUIElement_SetScaleS $276
#cmd LNUIElement_SetScaleXY $278
#cmd LNUIElement_GetScale $27B
#cmd LNUIElement_SetCenterPoint $27C
#cmd LNUIElement_SetCenterPointXYZ $27E
#cmd LNUIElement_GetCenterPoint $282
#cmd LNUIElement_AddChild $283
#cmd LNUIElement_SetPrototype_OnSerialize $84F
#cmd LNUITextBlock_Create $286
#cmd LNUITextBlock_CreateWithText $287
#cmd LNUITextBlock_SetPrototype_OnSerialize $85A
#cmd LNUISprite_Create $294
#cmd LNUISprite_CreateWithTexture $295
#cmd LNUISprite_SetTexture $28A
#cmd LNUISprite_SetSourceRect $28C
#cmd LNUISprite_SetSourceRectXYWH $28E
#cmd LNUISprite_GetSourceRect $293
#cmd LNUISprite_SetPrototype_OnSerialize $865
#cmd LNInput_Pressed $298
#cmd LNInput_Triggered $29A
#cmd LNInput_TriggeredOff $29C
#cmd LNInput_Repeated $29E
#cmd LNInput_GetAxisValue $2A0
#cmd LNInput_ClearAllBindings $2A2
#cmd LNMouse_Pressed $2A4
#cmd LNMouse_Triggered $2A6
#cmd LNMouse_TriggeredOff $2A8
#cmd LNMouse_Repeated $2AA
#cmd LNMouse_Position $2AC
#cmd LNInterpreterCommand_Code $2AE
#cmd LNInterpreterCommand_ParamsCount $2AF
#cmd LNInterpreterCommand_Param $2B0
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $870
#cmd LNInterpreterCommandList_Create $2C7
#cmd LNInterpreterCommandList_AddCommand $2B3
#cmd LNInterpreterCommandList_AddCommand1 $2B5
#cmd LNInterpreterCommandList_AddCommand2 $2B8
#cmd LNInterpreterCommandList_AddCommand3 $2BC
#cmd LNInterpreterCommandList_AddCommand4 $2C1
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $87B
#cmd LNInterpreterCommandDelegate_Create $5C4
#cmd LNInterpreter_Create $2DB
#cmd LNInterpreter_Clear $2CC
#cmd LNInterpreter_Run $2CD
#cmd LNInterpreter_IsRunning $2CF
#cmd LNInterpreter_Update $2D0
#cmd LNInterpreter_Terminate $2D1
#cmd LNInterpreter_RegisterCommandHandler $2D2
#cmd LNInterpreter_SetWaitMode $2D5
#cmd LNInterpreter_GetWaitMode $2D7
#cmd LNInterpreter_SetWaitCount $2D8
#cmd LNInterpreter_GetWaitCount $2DA
#cmd LNInterpreter_SetPrototype_OnSerialize $886
#cmd LNInterpreter_SetPrototype_OnUpdateWait $892
#cmd LNEngineSettings_SetMainWindowSize $2DE
#cmd LNEngineSettings_SetMainWorldViewSize $2E1
#cmd LNEngineSettings_SetMainWindowTitle $2E4
#cmd LNEngineSettings_AddAssetDirectory $2E6
#cmd LNEngineSettings_AddAssetArchive $2E8
#cmd LNEngineSettings_SetFrameRate $2EB
#cmd LNEngineSettings_SetDebugToolEnabled $2ED
#cmd LNEngineSettings_SetEngineLogEnabled $2EF
#cmd LNEngineSettings_SetEngineLogFilePath $2F1
#cmd LNEngine_Initialize $2F4
#cmd LNEngine_Finalize $2F5
#cmd LNEngine_Update $2F6
#cmd LNEngine_Run $2F7
#cmd LNEngine_Time $2F9
#cmd LNEngine_GetCamera $2FA
#cmd LNEngine_GetMainLight $2FB
#cmd LNEngine_GetRenderView $2FC
#cmd LNApplication_Create $301
#cmd LNApplication_OnInit $2FE
#cmd LNApplication_OnUpdate $2FF
#cmd LNApplication_World $300
#cmd LNApplication_SetPrototype_OnSerialize $89D
#cmd LNApplication_SetPrototype_OnInit $8A8
#cmd LNApplication_SetPrototype_OnUpdate $8B3
#cmd LNDebug_SetGuideGridEnabled $303
#cmd LNDebug_SetPhysicsDebugDrawEnabled $305
#cmd LNDebug_Print $307
#cmd LNDebug_PrintWithTime $309
#cmd LNDebug_PrintWithTimeAndColor $30C
#cmd LNObjectSerializeHandler_Create $5F0
#cmd LNEventConnectionSerializeHandler_Create $5FB
#cmd LNZVTestClass1SerializeHandler_Create $606
#cmd LNZVTestEventArgs1SerializeHandler_Create $611
#cmd LNSerializer2SerializeHandler_Create $61C
#cmd LNAssetModelSerializeHandler_Create $627
#cmd LNTextureSerializeHandler_Create $632
#cmd LNTexture2DSerializeHandler_Create $63D
#cmd LNRenderViewSerializeHandler_Create $648
#cmd LNSkinnedMeshModelSerializeHandler_Create $653
#cmd LNCollisionShapeSerializeHandler_Create $65E
#cmd LNBoxCollisionShapeSerializeHandler_Create $669
#cmd LNComponentSerializeHandler_Create $674
#cmd LNVisualComponentSerializeHandler_Create $67F
#cmd LNSpriteComponentSerializeHandler_Create $68A
#cmd LNCharacterControllerSerializeHandler_Create $695
#cmd LNWorldSerializeHandler_Create $6A0
#cmd LNComponentListSerializeHandler_Create $6AB
#cmd LNWorldObjectSerializeHandler_Create $6B6
#cmd LNWorldObjectUpdateHandler_Create $6C1
#cmd LNVisualObjectSerializeHandler_Create $6CC
#cmd LNVisualObjectUpdateHandler_Create $6D7
#cmd LNCameraSerializeHandler_Create $6E2
#cmd LNCameraUpdateHandler_Create $6ED
#cmd LNEnvironmentLightSerializeHandler_Create $6F8
#cmd LNEnvironmentLightUpdateHandler_Create $703
#cmd LNDirectionalLightSerializeHandler_Create $70E
#cmd LNDirectionalLightUpdateHandler_Create $719
#cmd LNPointLightSerializeHandler_Create $724
#cmd LNPointLightUpdateHandler_Create $72F
#cmd LNSpotLightSerializeHandler_Create $73A
#cmd LNSpotLightUpdateHandler_Create $745
#cmd LNSpriteSerializeHandler_Create $750
#cmd LNSpriteUpdateHandler_Create $75B
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $766
#cmd LNRaycasterSerializeHandler_Create $771
#cmd LNRaycastResultSerializeHandler_Create $77C
#cmd LNWorldRenderViewSerializeHandler_Create $787
#cmd LNBoxMeshSerializeHandler_Create $792
#cmd LNBoxMeshUpdateHandler_Create $79D
#cmd LNPlaneMeshSerializeHandler_Create $7A8
#cmd LNPlaneMeshUpdateHandler_Create $7B3
#cmd LNStaticMeshSerializeHandler_Create $7BE
#cmd LNStaticMeshUpdateHandler_Create $7C9
#cmd LNSkinnedMeshComponentSerializeHandler_Create $7D4
#cmd LNCollisionSerializeHandler_Create $7DF
#cmd LNTriggerBodyComponentSerializeHandler_Create $7EA
#cmd LNLevelSerializeHandler_Create $7F5
#cmd LNLevelStartHandler_Create $800
#cmd LNLevelStopHandler_Create $80B
#cmd LNLevelPauseHandler_Create $816
#cmd LNLevelResumeHandler_Create $821
#cmd LNLevelUpdateHandler_Create $82C
#cmd LNUIEventArgsSerializeHandler_Create $837
#cmd LNUILayoutElementSerializeHandler_Create $842
#cmd LNUIElementSerializeHandler_Create $84D
#cmd LNUITextBlockSerializeHandler_Create $858
#cmd LNUISpriteSerializeHandler_Create $863
#cmd LNInterpreterCommandSerializeHandler_Create $86E
#cmd LNInterpreterCommandListSerializeHandler_Create $879
#cmd LNInterpreterSerializeHandler_Create $884
#cmd LNInterpreterUpdateWaitHandler_Create $890
#cmd LNApplicationSerializeHandler_Create $89B
#cmd LNApplicationInitHandler_Create $8A6
#cmd LNApplicationUpdateHandler_Create $8B1



#cmd ln_args $1

#endif // __lumino__
