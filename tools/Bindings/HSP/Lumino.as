
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
#const global LN_UILAYOUT_ORIENTATION_HORIZONTAL 0
#const global LN_UILAYOUT_ORIENTATION_VERTICAL 1
#const global LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL 2
#const global LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL 3
#const global LN_UIVALIGNMENT_TOP 0
#const global LN_UIVALIGNMENT_CENTER 1
#const global LN_UIVALIGNMENT_BOTTOM 2
#const global LN_UIVALIGNMENT_STRETCH 3
#const global LN_UIHALIGNMENT_LEFT 0
#const global LN_UIHALIGNMENT_CENTER 1
#const global LN_UIHALIGNMENT_RIGHT 2
#const global LN_UIHALIGNMENT_STRETCH 3
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
#cmd LNObject_SetPrototype_OnSerialize $66D
#cmd LNEventConnection_SetPrototype_OnSerialize $678
#cmd LNPromiseFailureDelegate_Create $3E5
#cmd LNZVTestDelegate1_Create $3EA
#cmd LNZVTestDelegate2_Create $3F0
#cmd LNZVTestDelegate3_Create $3F5
#cmd LNZVTestEventHandler1_Create $3FA
#cmd LNZVTestEventHandler2_Create $3FF
#cmd LNZVTestPromise1_ThenWith $402
#cmd LNZVTestPromise1_CatchWith $404
#cmd LNZVTestPromise2_ThenWith $408
#cmd LNZVTestPromise2_CatchWith $40A
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $683
#cmd LNZVTestEventArgs1_Create $A1
#cmd LNZVTestEventArgs1_CreateWithValue $A2
#cmd LNZVTestEventArgs1_GetValue $A0
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $68E
#cmd LNSerializer2_SetPrototype_OnSerialize $699
#cmd LNAssetModel_Create $AE
#cmd LNAssetModel_Target $AD
#cmd LNAssetModel_SetPrototype_OnSerialize $6A4
#cmd LNAssets_SaveAssetToLocalFile $B1
#cmd LNAssets_LoadAssetFromLocalFile $B4
#cmd LNAssets_LoadAsset $B6
#cmd LNAssets_ReloadAsset $B8
#cmd LNTexture2DDelegate_Create $42E
#cmd LNTexture2DPromise_ThenWith $431
#cmd LNTexture2DPromise_CatchWith $433
#cmd LNTexture_SetPrototype_OnSerialize $6AF
#cmd LNTexture2D_Create $10A
#cmd LNTexture2D_CreateWithFormat $10D
#cmd LNTexture2D_Load $106
#cmd LNTexture2D_LoadEmoji $108
#cmd LNTexture2D_SetPrototype_OnSerialize $6BA
#cmd LNRenderView_SetPrototype_OnSerialize $6C5
#cmd LNSkinnedMeshModel_Load $119
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $6D0
#cmd LNCollisionShape_SetPrototype_OnSerialize $6DB
#cmd LNBoxCollisionShape_Create $11D
#cmd LNBoxCollisionShape_CreateWHD $11F
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $6E6
#cmd LNComponent_SetPrototype_OnSerialize $6F1
#cmd LNVisualComponent_SetVisible $125
#cmd LNVisualComponent_IsVisible $127
#cmd LNVisualComponent_SetPrototype_OnSerialize $6FC
#cmd LNSpriteComponent_SetTexture $129
#cmd LNSpriteComponent_SetPrototype_OnSerialize $707
#cmd LNCollisionEventHandler_Create $4C7
#cmd LNCharacterController_Create $13F
#cmd LNCharacterController_SetHeight $133
#cmd LNCharacterController_GetHeight $135
#cmd LNCharacterController_SetCameraRadius $136
#cmd LNCharacterController_GetCameraRadius $138
#cmd LNCharacterController_SetCollisionEnter $139
#cmd LNCharacterController_SetCollisionLeave $13B
#cmd LNCharacterController_SetCollisionStay $13D
#cmd LNCharacterController_SetPrototype_OnSerialize $712
#cmd LNWorld_Add $141
#cmd LNWorld_SetPrototype_OnSerialize $71D
#cmd LNComponentList_GetLength $4D9
#cmd LNComponentList_GetItem $4DB
#cmd LNComponentList_SetPrototype_OnSerialize $728
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
#cmd LNWorldObject_SetPrototype_OnSerialize $733
#cmd LNWorldObject_SetPrototype_OnUpdate $73E
#cmd LNVisualObject_SetVisible $179
#cmd LNVisualObject_IsVisible $17B
#cmd LNVisualObject_SetPrototype_OnSerialize $749
#cmd LNVisualObject_SetPrototype_OnUpdate $754
#cmd LNCamera_SetPrototype_OnSerialize $75F
#cmd LNCamera_SetPrototype_OnUpdate $76A
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
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $775
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $780
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
#cmd LNDirectionalLight_SetPrototype_OnSerialize $78B
#cmd LNDirectionalLight_SetPrototype_OnUpdate $796
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
#cmd LNPointLight_SetPrototype_OnSerialize $7A1
#cmd LNPointLight_SetPrototype_OnUpdate $7AC
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
#cmd LNSpotLight_SetPrototype_OnSerialize $7B7
#cmd LNSpotLight_SetPrototype_OnUpdate $7C2
#cmd LNTestDelegate_Create $57F
#cmd LNSprite_Create $1EA
#cmd LNSprite_CreateWithTexture $1EB
#cmd LNSprite_CreateWithTextureAndSize $1ED
#cmd LNSprite_SetTexture $1DC
#cmd LNSprite_SetSize $1DE
#cmd LNSprite_SetSizeWH $1E0
#cmd LNSprite_SetSourceRectXYWH $1E3
#cmd LNSprite_SetCallerTest $1E8
#cmd LNSprite_SetPrototype_OnSerialize $7CD
#cmd LNSprite_SetPrototype_OnUpdate $7D8
#cmd LNCameraOrbitControlComponent_Create $1F2
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $7E3
#cmd LNRaycaster_FromScreen $1F4
#cmd LNRaycaster_IntersectPlane $1F6
#cmd LNRaycaster_SetPrototype_OnSerialize $7EE
#cmd LNRaycastResult_GetPoint $1FB
#cmd LNRaycastResult_SetPrototype_OnSerialize $7F9
#cmd LNWorldRenderView_SetGuideGridEnabled $1FD
#cmd LNWorldRenderView_GetGuideGridEnabled $1FF
#cmd LNWorldRenderView_SetPrototype_OnSerialize $804
#cmd LNBoxMesh_Create $201
#cmd LNBoxMesh_SetPrototype_OnSerialize $80F
#cmd LNBoxMesh_SetPrototype_OnUpdate $81A
#cmd LNPlaneMesh_Create $203
#cmd LNPlaneMesh_SetPrototype_OnSerialize $825
#cmd LNPlaneMesh_SetPrototype_OnUpdate $830
#cmd LNStaticMesh_Load $205
#cmd LNStaticMesh_MakeCollisionBody $207
#cmd LNStaticMesh_SetPrototype_OnSerialize $83B
#cmd LNStaticMesh_SetPrototype_OnUpdate $846
#cmd LNSkinnedMeshComponent_Create $20C
#cmd LNSkinnedMeshComponent_SetModel $20A
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $851
#cmd LNCollision_GetWorldObject $20E
#cmd LNCollision_SetPrototype_OnSerialize $85C
#cmd LNTriggerBodyComponent_Create $212
#cmd LNTriggerBodyComponent_AddCollisionShape $210
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $867
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
#cmd LNLevel_SetPrototype_OnSerialize $872
#cmd LNLevel_SetPrototype_OnStart $87D
#cmd LNLevel_SetPrototype_OnStop $888
#cmd LNLevel_SetPrototype_OnPause $893
#cmd LNLevel_SetPrototype_OnResume $89E
#cmd LNLevel_SetPrototype_OnUpdate $8A9
#cmd LNUIEventArgs_Sender $24F
#cmd LNUIEventArgs_SetPrototype_OnSerialize $8B4
#cmd LNUIGeneralEventHandler_Create $5BC
#cmd LNUIEventHandler_Create $5C1
#cmd LNUILayoutElement_SetPrototype_OnSerialize $8BF
#cmd LNUIElement_SetSize $261
#cmd LNUIElement_SetSizeWH $263
#cmd LNUIElement_SetMargin $266
#cmd LNUIElement_GetMargin $268
#cmd LNUIElement_SetPadding $269
#cmd LNUIElement_GetPadding $26B
#cmd LNUIElement_SetHAlignment $26C
#cmd LNUIElement_GetHAlignment $26E
#cmd LNUIElement_SetVAlignment $26F
#cmd LNUIElement_GetVAlignment $271
#cmd LNUIElement_SetAlignments $272
#cmd LNUIElement_SetPosition $275
#cmd LNUIElement_SetPositionXYZ $277
#cmd LNUIElement_GetPosition $27B
#cmd LNUIElement_SetRotation $27C
#cmd LNUIElement_SetEulerAngles $27E
#cmd LNUIElement_GetRotation $282
#cmd LNUIElement_SetScale $283
#cmd LNUIElement_SetScaleS $285
#cmd LNUIElement_SetScaleXY $287
#cmd LNUIElement_GetScale $28A
#cmd LNUIElement_SetCenterPoint $28B
#cmd LNUIElement_SetCenterPointXYZ $28D
#cmd LNUIElement_GetCenterPoint $291
#cmd LNUIElement_SetEnabled $292
#cmd LNUIElement_GetEnabled $294
#cmd LNUIElement_AddChild $295
#cmd LNUIElement_SetPrototype_OnSerialize $8CA
#cmd LNUITextBlock_Create $298
#cmd LNUITextBlock_CreateWithText $299
#cmd LNUITextBlock_SetPrototype_OnSerialize $8D5
#cmd LNUISprite_Create $2A6
#cmd LNUISprite_CreateWithTexture $2A7
#cmd LNUISprite_SetTexture $29C
#cmd LNUISprite_SetSourceRect $29E
#cmd LNUISprite_SetSourceRectXYWH $2A0
#cmd LNUISprite_GetSourceRect $2A5
#cmd LNUISprite_SetPrototype_OnSerialize $8E0
#cmd LNUIIcon_LoadFontIcon $2AA
#cmd LNUIIcon_LoadFontIconWithNameSize $2AC
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $2AF
#cmd LNUIIcon_SetPrototype_OnSerialize $8EB
#cmd LNUI_Add $2B4
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $8F6
#cmd LNUIBoxLayout_Create $2BB
#cmd LNUIBoxLayout_SetOrientation $2B8
#cmd LNUIBoxLayout_GetOrientation $2BA
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $901
#cmd LNUIGridLayout_Create $2CB
#cmd LNUIGridLayout_SetColumnCount $2BD
#cmd LNUIGridLayout_SetRow $2BF
#cmd LNUIGridLayout_SetColumn $2C2
#cmd LNUIGridLayout_SetPlacement $2C5
#cmd LNUIGridLayout_SetPrototype_OnSerialize $90C
#cmd LNUIControl_SetPrototype_OnSerialize $917
#cmd LNUIButtonBase_SetText $2CE
#cmd LNUIButtonBase_SetPrototype_OnSerialize $922
#cmd LNUIButton_Create $2D1
#cmd LNUIButton_CreateWithText $2D2
#cmd LNUIButton_ConnectOnClicked $2D4
#cmd LNUIButton_SetPrototype_OnSerialize $92D
#cmd LNUIWindow_Create $2D7
#cmd LNUIWindow_SetPrototype_OnSerialize $938
#cmd LNUIListItem_SetPrototype_OnSerialize $943
#cmd LNUIListItemsControl_SetItemsLayoutPanel $2DD
#cmd LNUIListItemsControl_SetSubmitMode $2DF
#cmd LNUIListItemsControl_GetSubmitMode $2E1
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $94E
#cmd LNUIListBoxItem_Create $2E3
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $959
#cmd LNUIListBox_Create $2E8
#cmd LNUIListBox_AddItem $2E6
#cmd LNUIListBox_SetPrototype_OnSerialize $964
#cmd LNInput_IsPressed $2EA
#cmd LNInput_IsTriggered $2EC
#cmd LNInput_IsTriggeredOff $2EE
#cmd LNInput_IsRepeated $2F0
#cmd LNInput_GetAxisValue $2F2
#cmd LNInput_ClearAllBindings $2F4
#cmd LNMouse_Pressed $2F6
#cmd LNMouse_Triggered $2F8
#cmd LNMouse_TriggeredOff $2FA
#cmd LNMouse_Repeated $2FC
#cmd LNMouse_Position $2FE
#cmd LNInterpreterCommand_Code $300
#cmd LNInterpreterCommand_ParamsCount $301
#cmd LNInterpreterCommand_Param $302
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $96F
#cmd LNInterpreterCommandList_Create $319
#cmd LNInterpreterCommandList_AddCommand $305
#cmd LNInterpreterCommandList_AddCommand1 $307
#cmd LNInterpreterCommandList_AddCommand2 $30A
#cmd LNInterpreterCommandList_AddCommand3 $30E
#cmd LNInterpreterCommandList_AddCommand4 $313
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $97A
#cmd LNInterpreterCommandDelegate_Create $63F
#cmd LNInterpreter_Create $32D
#cmd LNInterpreter_Clear $31E
#cmd LNInterpreter_Run $31F
#cmd LNInterpreter_IsRunning $321
#cmd LNInterpreter_Update $322
#cmd LNInterpreter_Terminate $323
#cmd LNInterpreter_RegisterCommandHandler $324
#cmd LNInterpreter_SetWaitMode $327
#cmd LNInterpreter_GetWaitMode $329
#cmd LNInterpreter_SetWaitCount $32A
#cmd LNInterpreter_GetWaitCount $32C
#cmd LNInterpreter_SetPrototype_OnSerialize $985
#cmd LNInterpreter_SetPrototype_OnUpdateWait $991
#cmd LNEngineSettings_SetMainWindowSize $330
#cmd LNEngineSettings_SetMainWorldViewSize $333
#cmd LNEngineSettings_SetMainWindowTitle $336
#cmd LNEngineSettings_AddAssetDirectory $338
#cmd LNEngineSettings_AddAssetArchive $33A
#cmd LNEngineSettings_SetFrameRate $33D
#cmd LNEngineSettings_SetDefaultUITheme $33F
#cmd LNEngineSettings_SetDebugToolEnabled $341
#cmd LNEngineSettings_SetEngineLogEnabled $343
#cmd LNEngineSettings_SetEngineLogFilePath $345
#cmd LNEngine_Initialize $348
#cmd LNEngine_Finalize $349
#cmd LNEngine_Update $34A
#cmd LNEngine_Run $34B
#cmd LNEngine_Time $34D
#cmd LNEngine_GetCamera $34E
#cmd LNEngine_GetMainLight $34F
#cmd LNEngine_GetRenderView $350
#cmd LNApplication_Create $355
#cmd LNApplication_OnInit $352
#cmd LNApplication_OnUpdate $353
#cmd LNApplication_World $354
#cmd LNApplication_SetPrototype_OnSerialize $99C
#cmd LNApplication_SetPrototype_OnInit $9A7
#cmd LNApplication_SetPrototype_OnUpdate $9B2
#cmd LNDebug_SetGuideGridEnabled $357
#cmd LNDebug_SetPhysicsDebugDrawEnabled $359
#cmd LNDebug_Print $35B
#cmd LNDebug_PrintWithTime $35D
#cmd LNDebug_PrintWithTimeAndColor $360
#cmd LNObjectSerializeHandler_Create $66B
#cmd LNEventConnectionSerializeHandler_Create $676
#cmd LNZVTestClass1SerializeHandler_Create $681
#cmd LNZVTestEventArgs1SerializeHandler_Create $68C
#cmd LNSerializer2SerializeHandler_Create $697
#cmd LNAssetModelSerializeHandler_Create $6A2
#cmd LNTextureSerializeHandler_Create $6AD
#cmd LNTexture2DSerializeHandler_Create $6B8
#cmd LNRenderViewSerializeHandler_Create $6C3
#cmd LNSkinnedMeshModelSerializeHandler_Create $6CE
#cmd LNCollisionShapeSerializeHandler_Create $6D9
#cmd LNBoxCollisionShapeSerializeHandler_Create $6E4
#cmd LNComponentSerializeHandler_Create $6EF
#cmd LNVisualComponentSerializeHandler_Create $6FA
#cmd LNSpriteComponentSerializeHandler_Create $705
#cmd LNCharacterControllerSerializeHandler_Create $710
#cmd LNWorldSerializeHandler_Create $71B
#cmd LNComponentListSerializeHandler_Create $726
#cmd LNWorldObjectSerializeHandler_Create $731
#cmd LNWorldObjectUpdateHandler_Create $73C
#cmd LNVisualObjectSerializeHandler_Create $747
#cmd LNVisualObjectUpdateHandler_Create $752
#cmd LNCameraSerializeHandler_Create $75D
#cmd LNCameraUpdateHandler_Create $768
#cmd LNEnvironmentLightSerializeHandler_Create $773
#cmd LNEnvironmentLightUpdateHandler_Create $77E
#cmd LNDirectionalLightSerializeHandler_Create $789
#cmd LNDirectionalLightUpdateHandler_Create $794
#cmd LNPointLightSerializeHandler_Create $79F
#cmd LNPointLightUpdateHandler_Create $7AA
#cmd LNSpotLightSerializeHandler_Create $7B5
#cmd LNSpotLightUpdateHandler_Create $7C0
#cmd LNSpriteSerializeHandler_Create $7CB
#cmd LNSpriteUpdateHandler_Create $7D6
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $7E1
#cmd LNRaycasterSerializeHandler_Create $7EC
#cmd LNRaycastResultSerializeHandler_Create $7F7
#cmd LNWorldRenderViewSerializeHandler_Create $802
#cmd LNBoxMeshSerializeHandler_Create $80D
#cmd LNBoxMeshUpdateHandler_Create $818
#cmd LNPlaneMeshSerializeHandler_Create $823
#cmd LNPlaneMeshUpdateHandler_Create $82E
#cmd LNStaticMeshSerializeHandler_Create $839
#cmd LNStaticMeshUpdateHandler_Create $844
#cmd LNSkinnedMeshComponentSerializeHandler_Create $84F
#cmd LNCollisionSerializeHandler_Create $85A
#cmd LNTriggerBodyComponentSerializeHandler_Create $865
#cmd LNLevelSerializeHandler_Create $870
#cmd LNLevelStartHandler_Create $87B
#cmd LNLevelStopHandler_Create $886
#cmd LNLevelPauseHandler_Create $891
#cmd LNLevelResumeHandler_Create $89C
#cmd LNLevelUpdateHandler_Create $8A7
#cmd LNUIEventArgsSerializeHandler_Create $8B2
#cmd LNUILayoutElementSerializeHandler_Create $8BD
#cmd LNUIElementSerializeHandler_Create $8C8
#cmd LNUITextBlockSerializeHandler_Create $8D3
#cmd LNUISpriteSerializeHandler_Create $8DE
#cmd LNUIIconSerializeHandler_Create $8E9
#cmd LNUILayoutPanelSerializeHandler_Create $8F4
#cmd LNUIBoxLayoutSerializeHandler_Create $8FF
#cmd LNUIGridLayoutSerializeHandler_Create $90A
#cmd LNUIControlSerializeHandler_Create $915
#cmd LNUIButtonBaseSerializeHandler_Create $920
#cmd LNUIButtonSerializeHandler_Create $92B
#cmd LNUIWindowSerializeHandler_Create $936
#cmd LNUIListItemSerializeHandler_Create $941
#cmd LNUIListItemsControlSerializeHandler_Create $94C
#cmd LNUIListBoxItemSerializeHandler_Create $957
#cmd LNUIListBoxSerializeHandler_Create $962
#cmd LNInterpreterCommandSerializeHandler_Create $96D
#cmd LNInterpreterCommandListSerializeHandler_Create $978
#cmd LNInterpreterSerializeHandler_Create $983
#cmd LNInterpreterUpdateWaitHandler_Create $98F
#cmd LNApplicationSerializeHandler_Create $99A
#cmd LNApplicationInitHandler_Create $9A5
#cmd LNApplicationUpdateHandler_Create $9B0



#cmd ln_args $1

#endif // __lumino__
