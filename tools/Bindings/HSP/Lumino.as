
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
#cmd LNColor $D6
#cmd LNPoint $E1
#cmd LNSize $E8
#cmd LNRect $EF
#cmd LNThickness $FE

#cmd LNObject_Release $70
#cmd LNObject_Retain $71
#cmd LNObject_GetReferenceCount $72
#cmd LNObject_SetPrototype_OnSerialize $68F
#cmd LNEventConnection_SetPrototype_OnSerialize $69A
#cmd LNPromiseFailureDelegate_Create $3F8
#cmd LNVariant_Create $77
#cmd LNVariant_SetInt $78
#cmd LNVariant_GetInt $7A
#cmd LNVariant_SetPrototype_OnSerialize $6A5
#cmd LNZVTestDelegate1_Create $401
#cmd LNZVTestDelegate2_Create $407
#cmd LNZVTestDelegate3_Create $40C
#cmd LNZVTestEventHandler1_Create $411
#cmd LNZVTestEventHandler2_Create $416
#cmd LNZVTestPromise1_ThenWith $419
#cmd LNZVTestPromise1_CatchWith $41B
#cmd LNZVTestPromise2_ThenWith $41F
#cmd LNZVTestPromise2_CatchWith $421
#cmd LNZVTestClass1_Create $A3
#cmd LNZVTestClass1_SetTestDelegate1 $8D
#cmd LNZVTestClass1_SetTestDelegate2 $8F
#cmd LNZVTestClass1_SetTestDelegate3 $91
#cmd LNZVTestClass1_CallTestDelegate1 $93
#cmd LNZVTestClass1_CallTestDelegate2 $95
#cmd LNZVTestClass1_CallTestDelegate3 $98
#cmd LNZVTestClass1_LoadAsync $99
#cmd LNZVTestClass1_ExecuteAsync $9B
#cmd LNZVTestClass1_GetFilePath $9C
#cmd LNZVTestClass1_ConnectOnEvent1 $9D
#cmd LNZVTestClass1_RaiseEvent1 $9F
#cmd LNZVTestClass1_ConnectOnEvent2 $A0
#cmd LNZVTestClass1_RaiseEvent2 $A2
#cmd LNZVTestClass1_SetPrototype_OnSerialize $6B0
#cmd LNZVTestEventArgs1_Create $A6
#cmd LNZVTestEventArgs1_CreateWithValue $A7
#cmd LNZVTestEventArgs1_GetValue $A5
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $6BB
#cmd LNSerializer2_SetPrototype_OnSerialize $6C6
#cmd LNAssetModel_Create $B3
#cmd LNAssetModel_Target $B2
#cmd LNAssetModel_SetPrototype_OnSerialize $6D1
#cmd LNAssets_SaveAssetToLocalFile $B6
#cmd LNAssets_LoadAssetFromLocalFile $B9
#cmd LNAssets_LoadAsset $BB
#cmd LNAssets_ReloadAsset $BD
#cmd LNTexture2DDelegate_Create $445
#cmd LNTexture2DPromise_ThenWith $448
#cmd LNTexture2DPromise_CatchWith $44A
#cmd LNTexture_SetPrototype_OnSerialize $6DC
#cmd LNTexture2D_Create $10F
#cmd LNTexture2D_CreateWithFormat $112
#cmd LNTexture2D_Load $10B
#cmd LNTexture2D_LoadEmoji $10D
#cmd LNTexture2D_SetPrototype_OnSerialize $6E7
#cmd LNRenderView_SetPrototype_OnSerialize $6F2
#cmd LNSkinnedMeshModel_Load $11E
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $6FD
#cmd LNCollisionShape_SetPrototype_OnSerialize $708
#cmd LNBoxCollisionShape_Create $122
#cmd LNBoxCollisionShape_CreateWHD $124
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $713
#cmd LNComponent_SetPrototype_OnSerialize $71E
#cmd LNVisualComponent_SetVisible $12A
#cmd LNVisualComponent_IsVisible $12C
#cmd LNVisualComponent_SetPrototype_OnSerialize $729
#cmd LNSpriteComponent_SetTexture $12E
#cmd LNSpriteComponent_SetPrototype_OnSerialize $734
#cmd LNCollisionEventHandler_Create $4DE
#cmd LNCharacterController_Create $148
#cmd LNCharacterController_SetInputControlEnabled $138
#cmd LNCharacterController_SetCameraControlEnabled $13A
#cmd LNCharacterController_SetHeight $13C
#cmd LNCharacterController_GetHeight $13E
#cmd LNCharacterController_SetCameraRadius $13F
#cmd LNCharacterController_GetCameraRadius $141
#cmd LNCharacterController_SetCollisionEnter $142
#cmd LNCharacterController_SetCollisionLeave $144
#cmd LNCharacterController_SetCollisionStay $146
#cmd LNCharacterController_SetPrototype_OnSerialize $73F
#cmd LNWorld_Add $14A
#cmd LNWorld_SetPrototype_OnSerialize $74A
#cmd LNComponentList_GetLength $4F4
#cmd LNComponentList_GetItem $4F6
#cmd LNComponentList_SetPrototype_OnSerialize $755
#cmd LNWorldObject_Create $180
#cmd LNWorldObject_SetPosition $14E
#cmd LNWorldObject_SetPositionXYZ $150
#cmd LNWorldObject_GetPosition $154
#cmd LNWorldObject_SetRotationQuaternion $155
#cmd LNWorldObject_SetRotation $157
#cmd LNWorldObject_GetRotation $15B
#cmd LNWorldObject_SetScale $15C
#cmd LNWorldObject_SetScaleS $15E
#cmd LNWorldObject_SetScaleXYZ $160
#cmd LNWorldObject_GetScale $164
#cmd LNWorldObject_SetCenterPoint $165
#cmd LNWorldObject_SetCenterPointXYZ $167
#cmd LNWorldObject_GetCenterPoint $16B
#cmd LNWorldObject_LookAt $16C
#cmd LNWorldObject_LookAtXYZ $16E
#cmd LNWorldObject_AddComponent $172
#cmd LNWorldObject_RemoveComponent $174
#cmd LNWorldObject_AddTag $176
#cmd LNWorldObject_RemoveTag $178
#cmd LNWorldObject_HasTag $17A
#cmd LNWorldObject_Destroy $17C
#cmd LNWorldObject_GetComponents $17D
#cmd LNWorldObject_SetPrototype_OnSerialize $760
#cmd LNWorldObject_SetPrototype_OnUpdate $76B
#cmd LNVisualObject_SetVisible $182
#cmd LNVisualObject_IsVisible $184
#cmd LNVisualObject_SetPrototype_OnSerialize $776
#cmd LNVisualObject_SetPrototype_OnUpdate $781
#cmd LNCamera_SetPrototype_OnSerialize $78C
#cmd LNCamera_SetPrototype_OnUpdate $797
#cmd LNEnvironmentLight_SetEnabled $187
#cmd LNEnvironmentLight_IsEnabled $189
#cmd LNEnvironmentLight_SetColor $18A
#cmd LNEnvironmentLight_GetColor $18C
#cmd LNEnvironmentLight_SetAmbientColor $18D
#cmd LNEnvironmentLight_GetAmbientColor $18F
#cmd LNEnvironmentLight_GetSkyColor $190
#cmd LNEnvironmentLight_SetSkyColor $191
#cmd LNEnvironmentLight_GetGroundColor $193
#cmd LNEnvironmentLight_SetGroundColor $194
#cmd LNEnvironmentLight_SetIntensity $196
#cmd LNEnvironmentLight_GetIntensity $198
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $199
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $19B
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $19C
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $19E
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $7A2
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $7AD
#cmd LNDirectionalLight_Create $1AF
#cmd LNDirectionalLight_CreateWithColor $1B0
#cmd LNDirectionalLight_SetEnabled $1A0
#cmd LNDirectionalLight_IsEnabled $1A2
#cmd LNDirectionalLight_SetColor $1A3
#cmd LNDirectionalLight_GetColor $1A5
#cmd LNDirectionalLight_SetIntensity $1A6
#cmd LNDirectionalLight_GetIntensity $1A8
#cmd LNDirectionalLight_SetShadowEffectiveDistance $1A9
#cmd LNDirectionalLight_GetShadowEffectiveDistance $1AB
#cmd LNDirectionalLight_SetShadowEffectiveDepth $1AC
#cmd LNDirectionalLight_GetShadowEffectiveDepth $1AE
#cmd LNDirectionalLight_SetPrototype_OnSerialize $7B8
#cmd LNDirectionalLight_SetPrototype_OnUpdate $7C3
#cmd LNPointLight_Create $1C2
#cmd LNPointLight_CreateWithColorAndRange $1C3
#cmd LNPointLight_SetEnabled $1B3
#cmd LNPointLight_IsEnabled $1B5
#cmd LNPointLight_SetColor $1B6
#cmd LNPointLight_GetColor $1B8
#cmd LNPointLight_SetIntensity $1B9
#cmd LNPointLight_GetIntensity $1BB
#cmd LNPointLight_SetRange $1BC
#cmd LNPointLight_GetRange $1BE
#cmd LNPointLight_SetAttenuation $1BF
#cmd LNPointLight_GetAttenuation $1C1
#cmd LNPointLight_SetPrototype_OnSerialize $7CE
#cmd LNPointLight_SetPrototype_OnUpdate $7D9
#cmd LNSpotLight_Create $1DC
#cmd LNSpotLight_CreateWithColorAndRange $1DD
#cmd LNSpotLight_SetEnabled $1C7
#cmd LNSpotLight_IsEnabled $1C9
#cmd LNSpotLight_SetColor $1CA
#cmd LNSpotLight_GetColor $1CC
#cmd LNSpotLight_SetIntensity $1CD
#cmd LNSpotLight_GetIntensity $1CF
#cmd LNSpotLight_SetRange $1D0
#cmd LNSpotLight_GetRange $1D2
#cmd LNSpotLight_SetAttenuation $1D3
#cmd LNSpotLight_GetAttenuation $1D5
#cmd LNSpotLight_SetAngle $1D6
#cmd LNSpotLight_GetAngle $1D8
#cmd LNSpotLight_SetPenumbra $1D9
#cmd LNSpotLight_GetPenumbra $1DB
#cmd LNSpotLight_SetPrototype_OnSerialize $7E4
#cmd LNSpotLight_SetPrototype_OnUpdate $7EF
#cmd LNTestDelegate_Create $59A
#cmd LNSprite_Create $1F3
#cmd LNSprite_CreateWithTexture $1F4
#cmd LNSprite_CreateWithTextureAndSize $1F6
#cmd LNSprite_SetTexture $1E5
#cmd LNSprite_SetSize $1E7
#cmd LNSprite_SetSizeWH $1E9
#cmd LNSprite_SetSourceRectXYWH $1EC
#cmd LNSprite_SetCallerTest $1F1
#cmd LNSprite_SetPrototype_OnSerialize $7FA
#cmd LNSprite_SetPrototype_OnUpdate $805
#cmd LNCameraOrbitControlComponent_Create $1FB
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $810
#cmd LNRaycaster_FromScreen $1FD
#cmd LNRaycaster_IntersectPlane $1FF
#cmd LNRaycaster_SetPrototype_OnSerialize $81B
#cmd LNRaycastResult_GetPoint $204
#cmd LNRaycastResult_SetPrototype_OnSerialize $826
#cmd LNWorldRenderView_SetGuideGridEnabled $206
#cmd LNWorldRenderView_GetGuideGridEnabled $208
#cmd LNWorldRenderView_SetPrototype_OnSerialize $831
#cmd LNBoxMesh_Create $20A
#cmd LNBoxMesh_SetPrototype_OnSerialize $83C
#cmd LNBoxMesh_SetPrototype_OnUpdate $847
#cmd LNPlaneMesh_Create $20C
#cmd LNPlaneMesh_SetPrototype_OnSerialize $852
#cmd LNPlaneMesh_SetPrototype_OnUpdate $85D
#cmd LNStaticMesh_Load $20E
#cmd LNStaticMesh_MakeCollisionBody $210
#cmd LNStaticMesh_SetPrototype_OnSerialize $868
#cmd LNStaticMesh_SetPrototype_OnUpdate $873
#cmd LNSkinnedMeshComponent_Create $215
#cmd LNSkinnedMeshComponent_SetModel $213
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $87E
#cmd LNCollision_GetWorldObject $217
#cmd LNCollision_SetPrototype_OnSerialize $889
#cmd LNTriggerBodyComponent_Create $21B
#cmd LNTriggerBodyComponent_AddCollisionShape $219
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $894
#cmd LNScene_SetClearMode $21D
#cmd LNScene_SetSkyColor $21F
#cmd LNScene_SetSkyHorizonColor $221
#cmd LNScene_SetSkyCloudColor $223
#cmd LNScene_SetSkyOverlayColor $225
#cmd LNScene_GotoLevel $227
#cmd LNScene_CallLevel $22A
#cmd LNScene_ReturnLevel $22D
#cmd LNScene_ActiveLevel $22F
#cmd LNScene_IsTransitionEffectRunning $230
#cmd LNScene_SetTransitionEffectMode $231
#cmd LNScene_TransitionEffectMode $233
#cmd LNScene_SetTransitionDuration $234
#cmd LNScene_TransitionDuration $236
#cmd LNScene_SetTransitionEffectColor $237
#cmd LNScene_TransitionEffectColor $239
#cmd LNScene_SetTransitionEffectMaskTexture $23A
#cmd LNScene_TransitionEffectMaskTexture $23C
#cmd LNScene_SetTransitionEffectVague $23D
#cmd LNScene_TransitionEffectVague $23F
#cmd LNScene_StartFadeOut $240
#cmd LNScene_StartFadeIn $241
#cmd LNScene_SetScreenBlendColor $242
#cmd LNScene_ScreenBlendColor $244
#cmd LNLevel_Create $254
#cmd LNLevel_AddObject $246
#cmd LNLevel_RemoveObject $248
#cmd LNLevel_AddSubLevel $24A
#cmd LNLevel_RemoveSubLevel $24C
#cmd LNLevel_RemoveAllSubLevels $24E
#cmd LNLevel_SetPrototype_OnSerialize $89F
#cmd LNLevel_SetPrototype_OnStart $8AA
#cmd LNLevel_SetPrototype_OnStop $8B5
#cmd LNLevel_SetPrototype_OnPause $8C0
#cmd LNLevel_SetPrototype_OnResume $8CB
#cmd LNLevel_SetPrototype_OnUpdate $8D6
#cmd LNUIEventArgs_Sender $25B
#cmd LNUIEventArgs_SetPrototype_OnSerialize $8E1
#cmd LNUIGeneralEventHandler_Create $5D8
#cmd LNUIEventHandler_Create $5DD
#cmd LNUILayoutElement_SetPrototype_OnSerialize $8EC
#cmd LNUIElement_SetSize $26D
#cmd LNUIElement_SetSizeWH $26F
#cmd LNUIElement_SetMargin $272
#cmd LNUIElement_GetMargin $274
#cmd LNUIElement_SetPadding $275
#cmd LNUIElement_GetPadding $277
#cmd LNUIElement_SetHAlignment $278
#cmd LNUIElement_GetHAlignment $27A
#cmd LNUIElement_SetVAlignment $27B
#cmd LNUIElement_GetVAlignment $27D
#cmd LNUIElement_SetAlignments $27E
#cmd LNUIElement_SetPosition $281
#cmd LNUIElement_SetPositionXYZ $283
#cmd LNUIElement_GetPosition $287
#cmd LNUIElement_SetRotation $288
#cmd LNUIElement_SetEulerAngles $28A
#cmd LNUIElement_GetRotation $28E
#cmd LNUIElement_SetScale $28F
#cmd LNUIElement_SetScaleS $291
#cmd LNUIElement_SetScaleXY $293
#cmd LNUIElement_GetScale $296
#cmd LNUIElement_SetCenterPoint $297
#cmd LNUIElement_SetCenterPointXYZ $299
#cmd LNUIElement_GetCenterPoint $29D
#cmd LNUIElement_SetEnabled $29E
#cmd LNUIElement_IsEnabled $2A0
#cmd LNUIElement_SetData $2A1
#cmd LNUIElement_GetData $2A3
#cmd LNUIElement_AddChild $2A4
#cmd LNUIElement_SetPrototype_OnSerialize $8F7
#cmd LNUITextBlock_Create $2A7
#cmd LNUITextBlock_CreateWithText $2A8
#cmd LNUITextBlock_SetPrototype_OnSerialize $902
#cmd LNUISprite_Create $2B5
#cmd LNUISprite_CreateWithTexture $2B6
#cmd LNUISprite_SetTexture $2AB
#cmd LNUISprite_SetSourceRect $2AD
#cmd LNUISprite_SetSourceRectXYWH $2AF
#cmd LNUISprite_GetSourceRect $2B4
#cmd LNUISprite_SetPrototype_OnSerialize $90D
#cmd LNUIIcon_LoadFontIcon $2B9
#cmd LNUIIcon_LoadFontIconWithNameSize $2BB
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $2BE
#cmd LNUIIcon_SetPrototype_OnSerialize $918
#cmd LNUI_Add $2C3
#cmd LNUI_Remove $2C5
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $923
#cmd LNUIBoxLayout_Create $2CC
#cmd LNUIBoxLayout_SetOrientation $2C9
#cmd LNUIBoxLayout_GetOrientation $2CB
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $92E
#cmd LNUIGridLayout_Create $2DC
#cmd LNUIGridLayout_SetColumnCount $2CE
#cmd LNUIGridLayout_SetRow $2D0
#cmd LNUIGridLayout_SetColumn $2D3
#cmd LNUIGridLayout_SetPlacement $2D6
#cmd LNUIGridLayout_SetPrototype_OnSerialize $939
#cmd LNUIControl_SetPrototype_OnSerialize $944
#cmd LNUIButtonBase_SetText $2DF
#cmd LNUIButtonBase_SetPrototype_OnSerialize $94F
#cmd LNUIButton_Create $2E2
#cmd LNUIButton_CreateWithText $2E3
#cmd LNUIButton_ConnectOnClicked $2E5
#cmd LNUIButton_SetPrototype_OnSerialize $95A
#cmd LNUIWindow_Create $2E8
#cmd LNUIWindow_SetPrototype_OnSerialize $965
#cmd LNUIListItem_ConnectOnSubmit $2ED
#cmd LNUIListItem_SetPrototype_OnSerialize $970
#cmd LNUIListItemsControl_SetItemsLayoutPanel $2F0
#cmd LNUIListItemsControl_SetSubmitMode $2F2
#cmd LNUIListItemsControl_GetSubmitMode $2F4
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $97B
#cmd LNUIListBoxItem_Create $2F6
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $986
#cmd LNUIListBox_Create $2FB
#cmd LNUIListBox_AddItem $2F9
#cmd LNUIListBox_SetPrototype_OnSerialize $991
#cmd LNInput_IsPressed $2FD
#cmd LNInput_IsTriggered $2FF
#cmd LNInput_IsTriggeredOff $301
#cmd LNInput_IsRepeated $303
#cmd LNInput_GetAxisValue $305
#cmd LNInput_ClearAllBindings $307
#cmd LNMouse_Pressed $309
#cmd LNMouse_Triggered $30B
#cmd LNMouse_TriggeredOff $30D
#cmd LNMouse_Repeated $30F
#cmd LNMouse_Position $311
#cmd LNInterpreterCommand_Code $313
#cmd LNInterpreterCommand_ParamsCount $314
#cmd LNInterpreterCommand_Param $315
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $99C
#cmd LNInterpreterCommandList_Create $32C
#cmd LNInterpreterCommandList_AddCommand $318
#cmd LNInterpreterCommandList_AddCommand1 $31A
#cmd LNInterpreterCommandList_AddCommand2 $31D
#cmd LNInterpreterCommandList_AddCommand3 $321
#cmd LNInterpreterCommandList_AddCommand4 $326
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $9A7
#cmd LNInterpreterCommandDelegate_Create $661
#cmd LNInterpreter_Create $340
#cmd LNInterpreter_Clear $331
#cmd LNInterpreter_Run $332
#cmd LNInterpreter_IsRunning $334
#cmd LNInterpreter_Update $335
#cmd LNInterpreter_Terminate $336
#cmd LNInterpreter_RegisterCommandHandler $337
#cmd LNInterpreter_SetWaitMode $33A
#cmd LNInterpreter_GetWaitMode $33C
#cmd LNInterpreter_SetWaitCount $33D
#cmd LNInterpreter_GetWaitCount $33F
#cmd LNInterpreter_SetPrototype_OnSerialize $9B2
#cmd LNInterpreter_SetPrototype_OnUpdateWait $9BE
#cmd LNEngineSettings_SetMainWindowSize $343
#cmd LNEngineSettings_SetMainWorldViewSize $346
#cmd LNEngineSettings_SetMainWindowTitle $349
#cmd LNEngineSettings_AddAssetDirectory $34B
#cmd LNEngineSettings_AddAssetArchive $34D
#cmd LNEngineSettings_SetFrameRate $350
#cmd LNEngineSettings_SetDefaultUITheme $352
#cmd LNEngineSettings_SetDebugToolEnabled $354
#cmd LNEngineSettings_SetEngineLogEnabled $356
#cmd LNEngineSettings_SetEngineLogFilePath $358
#cmd LNEngine_Initialize $35B
#cmd LNEngine_Finalize $35C
#cmd LNEngine_Update $35D
#cmd LNEngine_Run $35E
#cmd LNEngine_Time $360
#cmd LNEngine_GetCamera $361
#cmd LNEngine_GetMainLight $362
#cmd LNEngine_GetRenderView $363
#cmd LNApplication_Create $368
#cmd LNApplication_OnInit $365
#cmd LNApplication_OnUpdate $366
#cmd LNApplication_World $367
#cmd LNApplication_SetPrototype_OnSerialize $9C9
#cmd LNApplication_SetPrototype_OnInit $9D4
#cmd LNApplication_SetPrototype_OnUpdate $9DF
#cmd LNDebug_SetGuideGridEnabled $36A
#cmd LNDebug_SetPhysicsDebugDrawEnabled $36C
#cmd LNDebug_Print $36E
#cmd LNDebug_PrintWithTime $370
#cmd LNDebug_PrintWithTimeAndColor $373
#cmd LNObjectSerializeHandler_Create $68D
#cmd LNEventConnectionSerializeHandler_Create $698
#cmd LNVariantSerializeHandler_Create $6A3
#cmd LNZVTestClass1SerializeHandler_Create $6AE
#cmd LNZVTestEventArgs1SerializeHandler_Create $6B9
#cmd LNSerializer2SerializeHandler_Create $6C4
#cmd LNAssetModelSerializeHandler_Create $6CF
#cmd LNTextureSerializeHandler_Create $6DA
#cmd LNTexture2DSerializeHandler_Create $6E5
#cmd LNRenderViewSerializeHandler_Create $6F0
#cmd LNSkinnedMeshModelSerializeHandler_Create $6FB
#cmd LNCollisionShapeSerializeHandler_Create $706
#cmd LNBoxCollisionShapeSerializeHandler_Create $711
#cmd LNComponentSerializeHandler_Create $71C
#cmd LNVisualComponentSerializeHandler_Create $727
#cmd LNSpriteComponentSerializeHandler_Create $732
#cmd LNCharacterControllerSerializeHandler_Create $73D
#cmd LNWorldSerializeHandler_Create $748
#cmd LNComponentListSerializeHandler_Create $753
#cmd LNWorldObjectSerializeHandler_Create $75E
#cmd LNWorldObjectUpdateHandler_Create $769
#cmd LNVisualObjectSerializeHandler_Create $774
#cmd LNVisualObjectUpdateHandler_Create $77F
#cmd LNCameraSerializeHandler_Create $78A
#cmd LNCameraUpdateHandler_Create $795
#cmd LNEnvironmentLightSerializeHandler_Create $7A0
#cmd LNEnvironmentLightUpdateHandler_Create $7AB
#cmd LNDirectionalLightSerializeHandler_Create $7B6
#cmd LNDirectionalLightUpdateHandler_Create $7C1
#cmd LNPointLightSerializeHandler_Create $7CC
#cmd LNPointLightUpdateHandler_Create $7D7
#cmd LNSpotLightSerializeHandler_Create $7E2
#cmd LNSpotLightUpdateHandler_Create $7ED
#cmd LNSpriteSerializeHandler_Create $7F8
#cmd LNSpriteUpdateHandler_Create $803
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $80E
#cmd LNRaycasterSerializeHandler_Create $819
#cmd LNRaycastResultSerializeHandler_Create $824
#cmd LNWorldRenderViewSerializeHandler_Create $82F
#cmd LNBoxMeshSerializeHandler_Create $83A
#cmd LNBoxMeshUpdateHandler_Create $845
#cmd LNPlaneMeshSerializeHandler_Create $850
#cmd LNPlaneMeshUpdateHandler_Create $85B
#cmd LNStaticMeshSerializeHandler_Create $866
#cmd LNStaticMeshUpdateHandler_Create $871
#cmd LNSkinnedMeshComponentSerializeHandler_Create $87C
#cmd LNCollisionSerializeHandler_Create $887
#cmd LNTriggerBodyComponentSerializeHandler_Create $892
#cmd LNLevelSerializeHandler_Create $89D
#cmd LNLevelStartHandler_Create $8A8
#cmd LNLevelStopHandler_Create $8B3
#cmd LNLevelPauseHandler_Create $8BE
#cmd LNLevelResumeHandler_Create $8C9
#cmd LNLevelUpdateHandler_Create $8D4
#cmd LNUIEventArgsSerializeHandler_Create $8DF
#cmd LNUILayoutElementSerializeHandler_Create $8EA
#cmd LNUIElementSerializeHandler_Create $8F5
#cmd LNUITextBlockSerializeHandler_Create $900
#cmd LNUISpriteSerializeHandler_Create $90B
#cmd LNUIIconSerializeHandler_Create $916
#cmd LNUILayoutPanelSerializeHandler_Create $921
#cmd LNUIBoxLayoutSerializeHandler_Create $92C
#cmd LNUIGridLayoutSerializeHandler_Create $937
#cmd LNUIControlSerializeHandler_Create $942
#cmd LNUIButtonBaseSerializeHandler_Create $94D
#cmd LNUIButtonSerializeHandler_Create $958
#cmd LNUIWindowSerializeHandler_Create $963
#cmd LNUIListItemSerializeHandler_Create $96E
#cmd LNUIListItemsControlSerializeHandler_Create $979
#cmd LNUIListBoxItemSerializeHandler_Create $984
#cmd LNUIListBoxSerializeHandler_Create $98F
#cmd LNInterpreterCommandSerializeHandler_Create $99A
#cmd LNInterpreterCommandListSerializeHandler_Create $9A5
#cmd LNInterpreterSerializeHandler_Create $9B0
#cmd LNInterpreterUpdateWaitHandler_Create $9BC
#cmd LNApplicationSerializeHandler_Create $9C7
#cmd LNApplicationInitHandler_Create $9D2
#cmd LNApplicationUpdateHandler_Create $9DD



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
