
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
#const global LN_UILIST_SUBMIT_MODE_SINGLE 0
#const global LN_UILIST_SUBMIT_MODE_DOUBLE 1

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
#cmd LNObject_SetPrototype_OnSerialize $634
#cmd LNEventConnection_SetPrototype_OnSerialize $63F
#cmd LNPromiseFailureDelegate_Create $3BC
#cmd LNZVTestDelegate1_Create $3C1
#cmd LNZVTestDelegate2_Create $3C7
#cmd LNZVTestDelegate3_Create $3CC
#cmd LNZVTestEventHandler1_Create $3D1
#cmd LNZVTestEventHandler2_Create $3D6
#cmd LNZVTestPromise1_ThenWith $3D9
#cmd LNZVTestPromise1_CatchWith $3DB
#cmd LNZVTestPromise2_ThenWith $3DF
#cmd LNZVTestPromise2_CatchWith $3E1
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $64A
#cmd LNZVTestEventArgs1_Create $A1
#cmd LNZVTestEventArgs1_CreateWithValue $A2
#cmd LNZVTestEventArgs1_GetValue $A0
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $655
#cmd LNSerializer2_SetPrototype_OnSerialize $660
#cmd LNAssetModel_Create $AE
#cmd LNAssetModel_Target $AD
#cmd LNAssetModel_SetPrototype_OnSerialize $66B
#cmd LNAssets_SaveAssetToLocalFile $B1
#cmd LNAssets_LoadAssetFromLocalFile $B4
#cmd LNAssets_LoadAsset $B6
#cmd LNAssets_ReloadAsset $B8
#cmd LNTexture2DDelegate_Create $405
#cmd LNTexture2DPromise_ThenWith $408
#cmd LNTexture2DPromise_CatchWith $40A
#cmd LNTexture_SetPrototype_OnSerialize $676
#cmd LNTexture2D_Create $10A
#cmd LNTexture2D_CreateWithFormat $10D
#cmd LNTexture2D_Load $106
#cmd LNTexture2D_LoadEmoji $108
#cmd LNTexture2D_SetPrototype_OnSerialize $681
#cmd LNRenderView_SetPrototype_OnSerialize $68C
#cmd LNSkinnedMeshModel_Load $119
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $697
#cmd LNCollisionShape_SetPrototype_OnSerialize $6A2
#cmd LNBoxCollisionShape_Create $11D
#cmd LNBoxCollisionShape_CreateWHD $11F
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $6AD
#cmd LNComponent_SetPrototype_OnSerialize $6B8
#cmd LNVisualComponent_SetVisible $125
#cmd LNVisualComponent_IsVisible $127
#cmd LNVisualComponent_SetPrototype_OnSerialize $6C3
#cmd LNSpriteComponent_SetTexture $129
#cmd LNSpriteComponent_SetPrototype_OnSerialize $6CE
#cmd LNCollisionEventHandler_Create $49E
#cmd LNCharacterController_Create $13F
#cmd LNCharacterController_SetHeight $133
#cmd LNCharacterController_GetHeight $135
#cmd LNCharacterController_SetCameraRadius $136
#cmd LNCharacterController_GetCameraRadius $138
#cmd LNCharacterController_SetCollisionEnter $139
#cmd LNCharacterController_SetCollisionLeave $13B
#cmd LNCharacterController_SetCollisionStay $13D
#cmd LNCharacterController_SetPrototype_OnSerialize $6D9
#cmd LNWorld_Add $141
#cmd LNWorld_SetPrototype_OnSerialize $6E4
#cmd LNComponentList_GetLength $4B0
#cmd LNComponentList_GetItem $4B2
#cmd LNComponentList_SetPrototype_OnSerialize $6EF
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
#cmd LNWorldObject_SetPrototype_OnSerialize $6FA
#cmd LNWorldObject_SetPrototype_OnUpdate $705
#cmd LNVisualObject_SetVisible $179
#cmd LNVisualObject_IsVisible $17B
#cmd LNVisualObject_SetPrototype_OnSerialize $710
#cmd LNVisualObject_SetPrototype_OnUpdate $71B
#cmd LNCamera_SetPrototype_OnSerialize $726
#cmd LNCamera_SetPrototype_OnUpdate $731
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
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $73C
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $747
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
#cmd LNDirectionalLight_SetPrototype_OnSerialize $752
#cmd LNDirectionalLight_SetPrototype_OnUpdate $75D
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
#cmd LNPointLight_SetPrototype_OnSerialize $768
#cmd LNPointLight_SetPrototype_OnUpdate $773
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
#cmd LNSpotLight_SetPrototype_OnSerialize $77E
#cmd LNSpotLight_SetPrototype_OnUpdate $789
#cmd LNTestDelegate_Create $556
#cmd LNSprite_Create $1EA
#cmd LNSprite_CreateWithTexture $1EB
#cmd LNSprite_CreateWithTextureAndSize $1ED
#cmd LNSprite_SetTexture $1DC
#cmd LNSprite_SetSize $1DE
#cmd LNSprite_SetSizeWH $1E0
#cmd LNSprite_SetSourceRectXYWH $1E3
#cmd LNSprite_SetCallerTest $1E8
#cmd LNSprite_SetPrototype_OnSerialize $794
#cmd LNSprite_SetPrototype_OnUpdate $79F
#cmd LNCameraOrbitControlComponent_Create $1F2
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $7AA
#cmd LNRaycaster_FromScreen $1F4
#cmd LNRaycaster_IntersectPlane $1F6
#cmd LNRaycaster_SetPrototype_OnSerialize $7B5
#cmd LNRaycastResult_GetPoint $1FB
#cmd LNRaycastResult_SetPrototype_OnSerialize $7C0
#cmd LNWorldRenderView_SetGuideGridEnabled $1FD
#cmd LNWorldRenderView_GetGuideGridEnabled $1FF
#cmd LNWorldRenderView_SetPrototype_OnSerialize $7CB
#cmd LNBoxMesh_Create $201
#cmd LNBoxMesh_SetPrototype_OnSerialize $7D6
#cmd LNBoxMesh_SetPrototype_OnUpdate $7E1
#cmd LNPlaneMesh_Create $203
#cmd LNPlaneMesh_SetPrototype_OnSerialize $7EC
#cmd LNPlaneMesh_SetPrototype_OnUpdate $7F7
#cmd LNStaticMesh_Load $205
#cmd LNStaticMesh_MakeCollisionBody $207
#cmd LNStaticMesh_SetPrototype_OnSerialize $802
#cmd LNStaticMesh_SetPrototype_OnUpdate $80D
#cmd LNSkinnedMeshComponent_Create $20C
#cmd LNSkinnedMeshComponent_SetModel $20A
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $818
#cmd LNCollision_GetWorldObject $20E
#cmd LNCollision_SetPrototype_OnSerialize $823
#cmd LNTriggerBodyComponent_Create $212
#cmd LNTriggerBodyComponent_AddCollisionShape $210
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $82E
#cmd LNScene_SetClearMode $214
#cmd LNScene_SetSkyColor $216
#cmd LNScene_SetSkyHorizonColor $218
#cmd LNScene_SetSkyCloudColor $21A
#cmd LNScene_SetSkyOverlayColor $21C
#cmd LNScene_GotoLevel $21E
#cmd LNScene_CallLevel $221
#cmd LNScene_ReturnLevel $224
#cmd LNScene_ActiveLevel $226
#cmd LNScene_IsTransitionEffectRunning $227
#cmd LNScene_SetTransitionEffectMode $228
#cmd LNScene_TransitionEffectMode $22A
#cmd LNScene_SetTransitionDuration $22B
#cmd LNScene_TransitionDuration $22D
#cmd LNScene_SetTransitionEffectColor $22E
#cmd LNScene_TransitionEffectColor $230
#cmd LNScene_SetTransitionEffectMaskTexture $231
#cmd LNScene_TransitionEffectMaskTexture $233
#cmd LNScene_SetTransitionEffectVague $234
#cmd LNScene_TransitionEffectVague $236
#cmd LNScene_StartFadeOut $237
#cmd LNScene_StartFadeIn $238
#cmd LNLevel_Create $248
#cmd LNLevel_AddObject $23A
#cmd LNLevel_RemoveObject $23C
#cmd LNLevel_AddSubLevel $23E
#cmd LNLevel_RemoveSubLevel $240
#cmd LNLevel_RemoveAllSubLevels $242
#cmd LNLevel_SetPrototype_OnSerialize $839
#cmd LNLevel_SetPrototype_OnStart $844
#cmd LNLevel_SetPrototype_OnStop $84F
#cmd LNLevel_SetPrototype_OnPause $85A
#cmd LNLevel_SetPrototype_OnResume $865
#cmd LNLevel_SetPrototype_OnUpdate $870
#cmd LNUIEventArgs_Sender $24A
#cmd LNUIEventArgs_SetPrototype_OnSerialize $87B
#cmd LNUIGeneralEventHandler_Create $593
#cmd LNUIEventHandler_Create $598
#cmd LNUILayoutElement_SetPrototype_OnSerialize $886
#cmd LNUIElement_SetSize $25C
#cmd LNUIElement_SetSizeWH $25E
#cmd LNUIElement_SetMargin $261
#cmd LNUIElement_GetMargin $263
#cmd LNUIElement_SetPadding $264
#cmd LNUIElement_GetPadding $266
#cmd LNUIElement_SetHAlignment $267
#cmd LNUIElement_GetHAlignment $269
#cmd LNUIElement_SetVAlignment $26A
#cmd LNUIElement_GetVAlignment $26C
#cmd LNUIElement_SetAlignments $26D
#cmd LNUIElement_SetPosition $270
#cmd LNUIElement_SetPositionXYZ $272
#cmd LNUIElement_GetPosition $276
#cmd LNUIElement_SetRotation $277
#cmd LNUIElement_SetEulerAngles $279
#cmd LNUIElement_GetRotation $27D
#cmd LNUIElement_SetScale $27E
#cmd LNUIElement_SetScaleS $280
#cmd LNUIElement_SetScaleXY $282
#cmd LNUIElement_GetScale $285
#cmd LNUIElement_SetCenterPoint $286
#cmd LNUIElement_SetCenterPointXYZ $288
#cmd LNUIElement_GetCenterPoint $28C
#cmd LNUIElement_SetEnabled $28D
#cmd LNUIElement_GetEnabled $28F
#cmd LNUIElement_AddChild $290
#cmd LNUIElement_SetPrototype_OnSerialize $891
#cmd LNUITextBlock_Create $293
#cmd LNUITextBlock_CreateWithText $294
#cmd LNUITextBlock_SetPrototype_OnSerialize $89C
#cmd LNUISprite_Create $2A1
#cmd LNUISprite_CreateWithTexture $2A2
#cmd LNUISprite_SetTexture $297
#cmd LNUISprite_SetSourceRect $299
#cmd LNUISprite_SetSourceRectXYWH $29B
#cmd LNUISprite_GetSourceRect $2A0
#cmd LNUISprite_SetPrototype_OnSerialize $8A7
#cmd LNUI_Add $2A5
#cmd LNUIControl_SetPrototype_OnSerialize $8B2
#cmd LNUIButtonBase_SetText $2A9
#cmd LNUIButtonBase_SetPrototype_OnSerialize $8BD
#cmd LNUIButton_Create $2AC
#cmd LNUIButton_CreateWithText $2AD
#cmd LNUIButton_ConnectOnClicked $2AF
#cmd LNUIButton_SetPrototype_OnSerialize $8C8
#cmd LNUIWindow_Create $2B2
#cmd LNUIWindow_SetPrototype_OnSerialize $8D3
#cmd LNUIListItem_SetPrototype_OnSerialize $8DE
#cmd LNUIListItemsControl_SetSubmitMode $2B8
#cmd LNUIListItemsControl_GetSubmitMode $2BA
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $8E9
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $8F4
#cmd LNUIListBox_Create $2BF
#cmd LNUIListBox_AddItem $2BD
#cmd LNUIListBox_SetPrototype_OnSerialize $8FF
#cmd LNInput_IsPressed $2C1
#cmd LNInput_IsTriggered $2C3
#cmd LNInput_IsTriggeredOff $2C5
#cmd LNInput_IsRepeated $2C7
#cmd LNInput_GetAxisValue $2C9
#cmd LNInput_ClearAllBindings $2CB
#cmd LNMouse_Pressed $2CD
#cmd LNMouse_Triggered $2CF
#cmd LNMouse_TriggeredOff $2D1
#cmd LNMouse_Repeated $2D3
#cmd LNMouse_Position $2D5
#cmd LNInterpreterCommand_Code $2D7
#cmd LNInterpreterCommand_ParamsCount $2D8
#cmd LNInterpreterCommand_Param $2D9
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $90A
#cmd LNInterpreterCommandList_Create $2F0
#cmd LNInterpreterCommandList_AddCommand $2DC
#cmd LNInterpreterCommandList_AddCommand1 $2DE
#cmd LNInterpreterCommandList_AddCommand2 $2E1
#cmd LNInterpreterCommandList_AddCommand3 $2E5
#cmd LNInterpreterCommandList_AddCommand4 $2EA
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $915
#cmd LNInterpreterCommandDelegate_Create $606
#cmd LNInterpreter_Create $304
#cmd LNInterpreter_Clear $2F5
#cmd LNInterpreter_Run $2F6
#cmd LNInterpreter_IsRunning $2F8
#cmd LNInterpreter_Update $2F9
#cmd LNInterpreter_Terminate $2FA
#cmd LNInterpreter_RegisterCommandHandler $2FB
#cmd LNInterpreter_SetWaitMode $2FE
#cmd LNInterpreter_GetWaitMode $300
#cmd LNInterpreter_SetWaitCount $301
#cmd LNInterpreter_GetWaitCount $303
#cmd LNInterpreter_SetPrototype_OnSerialize $920
#cmd LNInterpreter_SetPrototype_OnUpdateWait $92C
#cmd LNEngineSettings_SetMainWindowSize $307
#cmd LNEngineSettings_SetMainWorldViewSize $30A
#cmd LNEngineSettings_SetMainWindowTitle $30D
#cmd LNEngineSettings_AddAssetDirectory $30F
#cmd LNEngineSettings_AddAssetArchive $311
#cmd LNEngineSettings_SetFrameRate $314
#cmd LNEngineSettings_SetDefaultUITheme $316
#cmd LNEngineSettings_SetDebugToolEnabled $318
#cmd LNEngineSettings_SetEngineLogEnabled $31A
#cmd LNEngineSettings_SetEngineLogFilePath $31C
#cmd LNEngine_Initialize $31F
#cmd LNEngine_Finalize $320
#cmd LNEngine_Update $321
#cmd LNEngine_Run $322
#cmd LNEngine_Time $324
#cmd LNEngine_GetCamera $325
#cmd LNEngine_GetMainLight $326
#cmd LNEngine_GetRenderView $327
#cmd LNApplication_Create $32C
#cmd LNApplication_OnInit $329
#cmd LNApplication_OnUpdate $32A
#cmd LNApplication_World $32B
#cmd LNApplication_SetPrototype_OnSerialize $937
#cmd LNApplication_SetPrototype_OnInit $942
#cmd LNApplication_SetPrototype_OnUpdate $94D
#cmd LNDebug_SetGuideGridEnabled $32E
#cmd LNDebug_SetPhysicsDebugDrawEnabled $330
#cmd LNDebug_Print $332
#cmd LNDebug_PrintWithTime $334
#cmd LNDebug_PrintWithTimeAndColor $337
#cmd LNObjectSerializeHandler_Create $632
#cmd LNEventConnectionSerializeHandler_Create $63D
#cmd LNZVTestClass1SerializeHandler_Create $648
#cmd LNZVTestEventArgs1SerializeHandler_Create $653
#cmd LNSerializer2SerializeHandler_Create $65E
#cmd LNAssetModelSerializeHandler_Create $669
#cmd LNTextureSerializeHandler_Create $674
#cmd LNTexture2DSerializeHandler_Create $67F
#cmd LNRenderViewSerializeHandler_Create $68A
#cmd LNSkinnedMeshModelSerializeHandler_Create $695
#cmd LNCollisionShapeSerializeHandler_Create $6A0
#cmd LNBoxCollisionShapeSerializeHandler_Create $6AB
#cmd LNComponentSerializeHandler_Create $6B6
#cmd LNVisualComponentSerializeHandler_Create $6C1
#cmd LNSpriteComponentSerializeHandler_Create $6CC
#cmd LNCharacterControllerSerializeHandler_Create $6D7
#cmd LNWorldSerializeHandler_Create $6E2
#cmd LNComponentListSerializeHandler_Create $6ED
#cmd LNWorldObjectSerializeHandler_Create $6F8
#cmd LNWorldObjectUpdateHandler_Create $703
#cmd LNVisualObjectSerializeHandler_Create $70E
#cmd LNVisualObjectUpdateHandler_Create $719
#cmd LNCameraSerializeHandler_Create $724
#cmd LNCameraUpdateHandler_Create $72F
#cmd LNEnvironmentLightSerializeHandler_Create $73A
#cmd LNEnvironmentLightUpdateHandler_Create $745
#cmd LNDirectionalLightSerializeHandler_Create $750
#cmd LNDirectionalLightUpdateHandler_Create $75B
#cmd LNPointLightSerializeHandler_Create $766
#cmd LNPointLightUpdateHandler_Create $771
#cmd LNSpotLightSerializeHandler_Create $77C
#cmd LNSpotLightUpdateHandler_Create $787
#cmd LNSpriteSerializeHandler_Create $792
#cmd LNSpriteUpdateHandler_Create $79D
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $7A8
#cmd LNRaycasterSerializeHandler_Create $7B3
#cmd LNRaycastResultSerializeHandler_Create $7BE
#cmd LNWorldRenderViewSerializeHandler_Create $7C9
#cmd LNBoxMeshSerializeHandler_Create $7D4
#cmd LNBoxMeshUpdateHandler_Create $7DF
#cmd LNPlaneMeshSerializeHandler_Create $7EA
#cmd LNPlaneMeshUpdateHandler_Create $7F5
#cmd LNStaticMeshSerializeHandler_Create $800
#cmd LNStaticMeshUpdateHandler_Create $80B
#cmd LNSkinnedMeshComponentSerializeHandler_Create $816
#cmd LNCollisionSerializeHandler_Create $821
#cmd LNTriggerBodyComponentSerializeHandler_Create $82C
#cmd LNLevelSerializeHandler_Create $837
#cmd LNLevelStartHandler_Create $842
#cmd LNLevelStopHandler_Create $84D
#cmd LNLevelPauseHandler_Create $858
#cmd LNLevelResumeHandler_Create $863
#cmd LNLevelUpdateHandler_Create $86E
#cmd LNUIEventArgsSerializeHandler_Create $879
#cmd LNUILayoutElementSerializeHandler_Create $884
#cmd LNUIElementSerializeHandler_Create $88F
#cmd LNUITextBlockSerializeHandler_Create $89A
#cmd LNUISpriteSerializeHandler_Create $8A5
#cmd LNUIControlSerializeHandler_Create $8B0
#cmd LNUIButtonBaseSerializeHandler_Create $8BB
#cmd LNUIButtonSerializeHandler_Create $8C6
#cmd LNUIWindowSerializeHandler_Create $8D1
#cmd LNUIListItemSerializeHandler_Create $8DC
#cmd LNUIListItemsControlSerializeHandler_Create $8E7
#cmd LNUIListBoxItemSerializeHandler_Create $8F2
#cmd LNUIListBoxSerializeHandler_Create $8FD
#cmd LNInterpreterCommandSerializeHandler_Create $908
#cmd LNInterpreterCommandListSerializeHandler_Create $913
#cmd LNInterpreterSerializeHandler_Create $91E
#cmd LNInterpreterUpdateWaitHandler_Create $92A
#cmd LNApplicationSerializeHandler_Create $935
#cmd LNApplicationInitHandler_Create $940
#cmd LNApplicationUpdateHandler_Create $94B



#cmd ln_args $1

#endif // __lumino__
