
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

#cmd LnVector3 $B20
#cmd LnVector4 $1D20
#cmd LnQuaternion $2820
#cmd LnMatrix $3620
#cmd LnColor $AE20
#cmd LnPoint $B920
#cmd LnSize $C020
#cmd LnRect $C720
#cmd LnThickness $D620

#cmd LnObject_SetPrototype_SetPrototype_OnSerialize $48721
#cmd LnEventConnection_SetPrototype_SetPrototype_OnSerialize $49221
#cmd LnPromiseFailureDelegate_Create $2AB20
#cmd LnZVTestDelegate1_Create $2B020
#cmd LnZVTestDelegate2_Create $2B620
#cmd LnZVTestDelegate3_Create $2BB20
#cmd LnZVTestEventHandler1_Create $2C020
#cmd LnZVTestEventHandler2_Create $2C520
#cmd LnZVTestPromise1_ThenWith $2C820
#cmd LnZVTestPromise1_CatchWith $2CA20
#cmd LnZVTestPromise2_ThenWith $2CE20
#cmd LnZVTestPromise2_CatchWith $2D020
#cmd LnZVTestClass1_Create $7B20
#cmd LnZVTestClass1_SetTestDelegate1 $6520
#cmd LnZVTestClass1_SetTestDelegate2 $6720
#cmd LnZVTestClass1_SetTestDelegate3 $6920
#cmd LnZVTestClass1_CallTestDelegate1 $6B20
#cmd LnZVTestClass1_CallTestDelegate2 $6D20
#cmd LnZVTestClass1_CallTestDelegate3 $7020
#cmd LnZVTestClass1_LoadAsyncA $7120
#cmd LnZVTestClass1_ExecuteAsync $7320
#cmd LnZVTestClass1_GetFilePathA $7420
#cmd LnZVTestClass1_ConnectOnEvent1 $7520
#cmd LnZVTestClass1_RaiseEvent1 $7720
#cmd LnZVTestClass1_ConnectOnEvent2 $7820
#cmd LnZVTestClass1_RaiseEvent2 $7A20
#cmd LnZVTestClass1_SetPrototype_SetPrototype_OnSerialize $49D21
#cmd LnZVTestEventArgs1_Create $7E20
#cmd LnZVTestEventArgs1_CreateWithValue $7F20
#cmd LnZVTestEventArgs1_GetValue $7D20
#cmd LnZVTestEventArgs1_SetPrototype_SetPrototype_OnSerialize $4A821
#cmd LnSerializer2_SetPrototype_SetPrototype_OnSerialize $4B321
#cmd LnAssetModel_Create $8B20
#cmd LnAssetModel_Target $8A20
#cmd LnAssetModel_SetPrototype_SetPrototype_OnSerialize $4BE21
#cmd LnAssets_SaveAssetToLocalFileA $8E20
#cmd LnAssets_LoadAssetFromLocalFileA $9120
#cmd LnAssets_LoadAssetA $9320
#cmd LnAssets_ReloadAssetA $9520
#cmd LnTexture2DDelegate_Create $2F420
#cmd LnTexture2DPromise_ThenWith $2F720
#cmd LnTexture2DPromise_CatchWith $2F920
#cmd LnTexture_SetPrototype_SetPrototype_OnSerialize $4C921
#cmd LnTexture2D_Create $E720
#cmd LnTexture2D_CreateWithFormat $EA20
#cmd LnTexture2D_LoadA $E320
#cmd LnTexture2D_LoadEmojiA $E520
#cmd LnTexture2D_SetPrototype_SetPrototype_OnSerialize $4D421
#cmd LnRenderView_SetPrototype_SetPrototype_OnSerialize $4DF21
#cmd LnComponent_SetPrototype_SetPrototype_OnSerialize $4EA21
#cmd LnVisualComponent_SetVisible $F120
#cmd LnVisualComponent_IsVisible $F320
#cmd LnVisualComponent_SetPrototype_SetPrototype_OnSerialize $4F521
#cmd LnSpriteComponent_SetTexture $F520
#cmd LnSpriteComponent_SetPrototype_SetPrototype_OnSerialize $50021
#cmd LnWorld_Add $F820
#cmd LnWorld_SetPrototype_SetPrototype_OnSerialize $50B21
#cmd LnComponentList_GetLength $38820
#cmd LnComponentList_GetItem $38A20
#cmd LnComponentList_SetPrototype_SetPrototype_OnSerialize $51621
#cmd LnWorldObject_SetPosition $FC20
#cmd LnWorldObject_SetPositionXYZ $FE20
#cmd LnWorldObject_GetPosition $10220
#cmd LnWorldObject_SetRotationQuaternion $10320
#cmd LnWorldObject_SetRotation $10520
#cmd LnWorldObject_GetRotation $10920
#cmd LnWorldObject_SetScale $10A20
#cmd LnWorldObject_SetScaleS $10C20
#cmd LnWorldObject_SetScaleXYZ $10E20
#cmd LnWorldObject_GetScale $11220
#cmd LnWorldObject_SetCenterPoint $11320
#cmd LnWorldObject_SetCenterPointXYZ $11520
#cmd LnWorldObject_GetCenterPoint $11920
#cmd LnWorldObject_LookAt $11A20
#cmd LnWorldObject_LookAtXYZ $11C20
#cmd LnWorldObject_AddComponent $12020
#cmd LnWorldObject_GetComponents $12220
#cmd LnWorldObject_SetPrototype_SetPrototype_OnSerialize $52121
#cmd LnWorldObject_SetPrototype_SetPrototype_OnUpdate $52C22
#cmd LnVisualObject_SetVisible $12620
#cmd LnVisualObject_IsVisible $12820
#cmd LnVisualObject_SetPrototype_SetPrototype_OnSerialize $53721
#cmd LnVisualObject_SetPrototype_SetPrototype_OnUpdate $54222
#cmd LnCamera_SetPrototype_SetPrototype_OnSerialize $54D21
#cmd LnCamera_SetPrototype_SetPrototype_OnUpdate $55822
#cmd LnDirectionalLight_Create $13A20
#cmd LnDirectionalLight_CreateWithColor $13B20
#cmd LnDirectionalLight_SetEnabled $12B20
#cmd LnDirectionalLight_IsEnabled $12D20
#cmd LnDirectionalLight_SetColor $12E20
#cmd LnDirectionalLight_GetColor $13020
#cmd LnDirectionalLight_SetIntensity $13120
#cmd LnDirectionalLight_GetIntensity $13320
#cmd LnDirectionalLight_SetShadowEffectiveDistance $13420
#cmd LnDirectionalLight_GetShadowEffectiveDistance $13620
#cmd LnDirectionalLight_SetShadowEffectiveDepth $13720
#cmd LnDirectionalLight_GetShadowEffectiveDepth $13920
#cmd LnDirectionalLight_SetPrototype_SetPrototype_OnSerialize $56321
#cmd LnDirectionalLight_SetPrototype_SetPrototype_OnUpdate $56E22
#cmd LnPointLight_Create $14D20
#cmd LnPointLight_CreateWithColorAndRange $14E20
#cmd LnPointLight_SetEnabled $13E20
#cmd LnPointLight_IsEnabled $14020
#cmd LnPointLight_SetColor $14120
#cmd LnPointLight_GetColor $14320
#cmd LnPointLight_SetIntensity $14420
#cmd LnPointLight_GetIntensity $14620
#cmd LnPointLight_SetRange $14720
#cmd LnPointLight_GetRange $14920
#cmd LnPointLight_SetAttenuation $14A20
#cmd LnPointLight_GetAttenuation $14C20
#cmd LnPointLight_SetPrototype_SetPrototype_OnSerialize $57921
#cmd LnPointLight_SetPrototype_SetPrototype_OnUpdate $58422
#cmd LnSpotLight_Create $16720
#cmd LnSpotLight_CreateWithColorAndRange $16820
#cmd LnSpotLight_SetEnabled $15220
#cmd LnSpotLight_IsEnabled $15420
#cmd LnSpotLight_SetColor $15520
#cmd LnSpotLight_GetColor $15720
#cmd LnSpotLight_SetIntensity $15820
#cmd LnSpotLight_GetIntensity $15A20
#cmd LnSpotLight_SetRange $15B20
#cmd LnSpotLight_GetRange $15D20
#cmd LnSpotLight_SetAttenuation $15E20
#cmd LnSpotLight_GetAttenuation $16020
#cmd LnSpotLight_SetAngle $16120
#cmd LnSpotLight_GetAngle $16320
#cmd LnSpotLight_SetPenumbra $16420
#cmd LnSpotLight_GetPenumbra $16620
#cmd LnSpotLight_SetPrototype_SetPrototype_OnSerialize $58F21
#cmd LnSpotLight_SetPrototype_SetPrototype_OnUpdate $59A22
#cmd LnTestDelegate_Create $40520
#cmd LnSprite_Create $17E20
#cmd LnSprite_CreateWithTexture $17F20
#cmd LnSprite_CreateWithTextureAndSize $18120
#cmd LnSprite_SetTexture $17020
#cmd LnSprite_SetSize $17220
#cmd LnSprite_SetSizeWH $17420
#cmd LnSprite_SetSourceRectXYWH $17720
#cmd LnSprite_SetCallerTest $17C20
#cmd LnSprite_SetPrototype_SetPrototype_OnSerialize $5A521
#cmd LnSprite_SetPrototype_SetPrototype_OnUpdate $5B022
#cmd LnCameraOrbitControlComponent_Create $18620
#cmd LnCameraOrbitControlComponent_SetPrototype_SetPrototype_OnSerialize $5BB21
#cmd LnRaycaster_FromScreen $18820
#cmd LnRaycaster_IntersectPlane $18A20
#cmd LnRaycaster_SetPrototype_SetPrototype_OnSerialize $5C621
#cmd LnRaycastResult_GetPoint $18F20
#cmd LnRaycastResult_SetPrototype_SetPrototype_OnSerialize $5D121
#cmd LnWorldRenderView_SetGuideGridEnabled $19120
#cmd LnWorldRenderView_GetGuideGridEnabled $19320
#cmd LnWorldRenderView_SetPrototype_SetPrototype_OnSerialize $5DC21
#cmd LnBoxMesh_Create $19520
#cmd LnBoxMesh_SetPrototype_SetPrototype_OnSerialize $5E721
#cmd LnBoxMesh_SetPrototype_SetPrototype_OnUpdate $5F222
#cmd LnPlaneMesh_Create $19720
#cmd LnPlaneMesh_SetPrototype_SetPrototype_OnSerialize $5FD21
#cmd LnPlaneMesh_SetPrototype_SetPrototype_OnUpdate $60822
#cmd LnUIEventArgs_Sender $19920
#cmd LnUIEventArgs_SetPrototype_SetPrototype_OnSerialize $61321
#cmd LnUIGeneralEventHandler_Create $42420
#cmd LnUIEventHandler_Create $42920
#cmd LnUILayoutElement_SetPrototype_SetPrototype_OnSerialize $61E21
#cmd LnUIElement_SetMargin $1AB20
#cmd LnUIElement_GetMargin $1AD20
#cmd LnUIElement_SetPadding $1AE20
#cmd LnUIElement_GetPadding $1B020
#cmd LnUIElement_SetHAlignment $1B120
#cmd LnUIElement_GetHAlignment $1B320
#cmd LnUIElement_SetVAlignment $1B420
#cmd LnUIElement_GetVAlignment $1B620
#cmd LnUIElement_SetAlignments $1B720
#cmd LnUIElement_SetPosition $1BA20
#cmd LnUIElement_SetPositionXYZ $1BC20
#cmd LnUIElement_GetPosition $1C020
#cmd LnUIElement_SetRotation $1C120
#cmd LnUIElement_SetEulerAngles $1C320
#cmd LnUIElement_GetRotation $1C720
#cmd LnUIElement_SetScale $1C820
#cmd LnUIElement_SetScaleS $1CA20
#cmd LnUIElement_SetScaleXY $1CC20
#cmd LnUIElement_GetScale $1CF20
#cmd LnUIElement_SetCenterPoint $1D020
#cmd LnUIElement_SetCenterPointXYZ $1D220
#cmd LnUIElement_GetCenterPoint $1D620
#cmd LnUIElement_AddChild $1D720
#cmd LnUIElement_SetPrototype_SetPrototype_OnSerialize $62921
#cmd LnUITextBlock_Create $1DA20
#cmd LnUITextBlock_CreateWithTextA $1DB20
#cmd LnUITextBlock_SetPrototype_SetPrototype_OnSerialize $63421
#cmd LnUISprite_Create $1E820
#cmd LnUISprite_CreateWithTexture $1E920
#cmd LnUISprite_SetTexture $1DE20
#cmd LnUISprite_SetSourceRect $1E020
#cmd LnUISprite_SetSourceRectXYWH $1E220
#cmd LnUISprite_GetSourceRect $1E720
#cmd LnUISprite_SetPrototype_SetPrototype_OnSerialize $63F21
#cmd LnInput_PressedA $1EC20
#cmd LnInput_TriggeredA $1EE20
#cmd LnInput_TriggeredOffA $1F020
#cmd LnInput_RepeatedA $1F220
#cmd LnInput_GetAxisValueA $1F420
#cmd LnInput_ClearAllBindings $1F620
#cmd LnMouse_Pressed $1F820
#cmd LnMouse_Triggered $1FA20
#cmd LnMouse_TriggeredOff $1FC20
#cmd LnMouse_Repeated $1FE20
#cmd LnMouse_Position $20020
#cmd LnEngineSettings_SetMainWindowSize $20220
#cmd LnEngineSettings_SetMainWorldViewSize $20520
#cmd LnEngineSettings_SetMainWindowTitleA $20820
#cmd LnEngineSettings_AddAssetDirectoryA $20A20
#cmd LnEngineSettings_AddAssetArchiveA $20C20
#cmd LnEngineSettings_SetFrameRate $20F20
#cmd LnEngineSettings_SetDebugToolEnabled $21120
#cmd LnEngineSettings_SetEngineLogEnabled $21320
#cmd LnEngineSettings_SetEngineLogFilePathA $21520
#cmd LnEngine_Initialize $21820
#cmd LnEngine_Finalize $21920
#cmd LnEngine_Update $21A20
#cmd LnEngine_Time $21B20
#cmd LnEngine_GetCamera $21C20
#cmd LnEngine_GetLight $21D20
#cmd LnEngine_GetRenderView $21E20
#cmd LnApplication_Create $22320
#cmd LnApplication_OnInit $22020
#cmd LnApplication_OnUpdate $22120
#cmd LnApplication_World $22220
#cmd LnApplication_SetPrototype_SetPrototype_OnSerialize $64A21
#cmd LnApplication_SetPrototype_SetPrototype_OnInit $65522
#cmd LnApplication_SetPrototype_SetPrototype_OnUpdate $66023
#cmd LnDebug_PrintA $22520
#cmd LnDebug_PrintWithTimeA $22720
#cmd LnDebug_PrintWithTimeAndColorA $22A20
#cmd LnObjectSerializeHandler_Create $48520
#cmd LnEventConnectionSerializeHandler_Create $49020
#cmd LnZVTestClass1SerializeHandler_Create $49B20
#cmd LnZVTestEventArgs1SerializeHandler_Create $4A620
#cmd LnSerializer2SerializeHandler_Create $4B120
#cmd LnAssetModelSerializeHandler_Create $4BC20
#cmd LnTextureSerializeHandler_Create $4C720
#cmd LnTexture2DSerializeHandler_Create $4D220
#cmd LnRenderViewSerializeHandler_Create $4DD20
#cmd LnComponentSerializeHandler_Create $4E820
#cmd LnVisualComponentSerializeHandler_Create $4F320
#cmd LnSpriteComponentSerializeHandler_Create $4FE20
#cmd LnWorldSerializeHandler_Create $50920
#cmd LnComponentListSerializeHandler_Create $51420
#cmd LnWorldObjectSerializeHandler_Create $51F20
#cmd LnWorldObjectUpdateHandler_Create $52A20
#cmd LnVisualObjectSerializeHandler_Create $53520
#cmd LnVisualObjectUpdateHandler_Create $54020
#cmd LnCameraSerializeHandler_Create $54B20
#cmd LnCameraUpdateHandler_Create $55620
#cmd LnDirectionalLightSerializeHandler_Create $56120
#cmd LnDirectionalLightUpdateHandler_Create $56C20
#cmd LnPointLightSerializeHandler_Create $57720
#cmd LnPointLightUpdateHandler_Create $58220
#cmd LnSpotLightSerializeHandler_Create $58D20
#cmd LnSpotLightUpdateHandler_Create $59820
#cmd LnSpriteSerializeHandler_Create $5A320
#cmd LnSpriteUpdateHandler_Create $5AE20
#cmd LnCameraOrbitControlComponentSerializeHandler_Create $5B920
#cmd LnRaycasterSerializeHandler_Create $5C420
#cmd LnRaycastResultSerializeHandler_Create $5CF20
#cmd LnWorldRenderViewSerializeHandler_Create $5DA20
#cmd LnBoxMeshSerializeHandler_Create $5E520
#cmd LnBoxMeshUpdateHandler_Create $5F020
#cmd LnPlaneMeshSerializeHandler_Create $5FB20
#cmd LnPlaneMeshUpdateHandler_Create $60620
#cmd LnUIEventArgsSerializeHandler_Create $61120
#cmd LnUILayoutElementSerializeHandler_Create $61C20
#cmd LnUIElementSerializeHandler_Create $62720
#cmd LnUITextBlockSerializeHandler_Create $63220
#cmd LnUISpriteSerializeHandler_Create $63D20
#cmd LnApplicationSerializeHandler_Create $64820
#cmd LnApplicationInitHandler_Create $65320
#cmd LnApplicationUpdateHandler_Create $65E20



#cmd ln_args $1

#endif // __lumino__
