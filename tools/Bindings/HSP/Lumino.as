
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
#cmd LnColor $D020
#cmd LnPoint $DB20
#cmd LnSize $E220
#cmd LnRect $E920
#cmd LnThickness $F820

#cmd LnObject_SetPrototype_SetPrototype_OnSerialize $4BB21
#cmd LnObject_SetPrototype_SetPrototype_OnSerialize2 $4C622
#cmd LnEventConnection_SetPrototype_SetPrototype_OnSerialize $4D121
#cmd LnEventConnection_SetPrototype_SetPrototype_OnSerialize2 $4DC22
#cmd LnPromiseFailureDelegate_Create $2CE20
#cmd LnZVTestDelegate1_Create $2D320
#cmd LnZVTestDelegate2_Create $2D920
#cmd LnZVTestDelegate3_Create $2DE20
#cmd LnZVTestEventHandler1_Create $2E320
#cmd LnZVTestEventHandler2_Create $2E820
#cmd LnZVTestPromise1_ThenWith $2EB20
#cmd LnZVTestPromise1_CatchWith $2ED20
#cmd LnZVTestPromise2_ThenWith $2F120
#cmd LnZVTestPromise2_CatchWith $2F320
#cmd LnZVTestClass1_Create $7D20
#cmd LnZVTestClass1_SetTestDelegate1 $6720
#cmd LnZVTestClass1_SetTestDelegate2 $6920
#cmd LnZVTestClass1_SetTestDelegate3 $6B20
#cmd LnZVTestClass1_CallTestDelegate1 $6D20
#cmd LnZVTestClass1_CallTestDelegate2 $6F20
#cmd LnZVTestClass1_CallTestDelegate3 $7220
#cmd LnZVTestClass1_LoadAsyncA $7320
#cmd LnZVTestClass1_ExecuteAsync $7520
#cmd LnZVTestClass1_GetFilePathA $7620
#cmd LnZVTestClass1_ConnectOnEvent1 $7720
#cmd LnZVTestClass1_RaiseEvent1 $7920
#cmd LnZVTestClass1_ConnectOnEvent2 $7A20
#cmd LnZVTestClass1_RaiseEvent2 $7C20
#cmd LnZVTestClass1_SetPrototype_SetPrototype_OnSerialize $4E721
#cmd LnZVTestClass1_SetPrototype_SetPrototype_OnSerialize2 $4F222
#cmd LnZVTestEventArgs1_Create $8020
#cmd LnZVTestEventArgs1_CreateWithValue $8120
#cmd LnZVTestEventArgs1_GetValue $7F20
#cmd LnZVTestEventArgs1_SetPrototype_SetPrototype_OnSerialize $4FD21
#cmd LnZVTestEventArgs1_SetPrototype_SetPrototype_OnSerialize2 $50822
#cmd LnSerializer_WriteBoolA $8420
#cmd LnSerializer_WriteIntA $8720
#cmd LnSerializer_WriteFloatA $8A20
#cmd LnSerializer_WriteStringA $8D20
#cmd LnSerializer_WriteObjectA $9020
#cmd LnSerializer_ReadBoolA $9320
#cmd LnSerializer_ReadIntA $9520
#cmd LnSerializer_ReadFloatA $9720
#cmd LnSerializer_ReadStringA $9920
#cmd LnSerializer_ReadObjectA $9B20
#cmd LnSerializer_SerializeA $9D20
#cmd LnSerializer_DeserializeA $A020
#cmd LnSerializer_SetPrototype_SetPrototype_OnSerialize $51321
#cmd LnSerializer_SetPrototype_SetPrototype_OnSerialize2 $51E22
#cmd LnSerializer2_SetPrototype_SetPrototype_OnSerialize $52921
#cmd LnSerializer2_SetPrototype_SetPrototype_OnSerialize2 $53422
#cmd LnAssetModel_Create $AD20
#cmd LnAssetModel_Target $AC20
#cmd LnAssetModel_SetPrototype_SetPrototype_OnSerialize $53F21
#cmd LnAssetModel_SetPrototype_SetPrototype_OnSerialize2 $54A22
#cmd LnAssets_SaveAssetToLocalFileA $B020
#cmd LnAssets_LoadAssetFromLocalFileA $B320
#cmd LnAssets_LoadAssetA $B520
#cmd LnAssets_ReloadAssetA $B720
#cmd LnTexture2DDelegate_Create $32820
#cmd LnTexture2DPromise_ThenWith $32B20
#cmd LnTexture2DPromise_CatchWith $32D20
#cmd LnTexture_SetPrototype_SetPrototype_OnSerialize $55521
#cmd LnTexture_SetPrototype_SetPrototype_OnSerialize2 $56022
#cmd LnTexture2D_Create $10920
#cmd LnTexture2D_CreateWithFormat $10C20
#cmd LnTexture2D_LoadA $10520
#cmd LnTexture2D_LoadEmojiA $10720
#cmd LnTexture2D_SetPrototype_SetPrototype_OnSerialize $56B21
#cmd LnTexture2D_SetPrototype_SetPrototype_OnSerialize2 $57622
#cmd LnRenderView_SetPrototype_SetPrototype_OnSerialize $58121
#cmd LnRenderView_SetPrototype_SetPrototype_OnSerialize2 $58C22
#cmd LnComponent_SetPrototype_SetPrototype_OnSerialize $59721
#cmd LnComponent_SetPrototype_SetPrototype_OnSerialize2 $5A222
#cmd LnVisualComponent_SetVisible $11320
#cmd LnVisualComponent_IsVisible $11520
#cmd LnVisualComponent_SetPrototype_SetPrototype_OnSerialize $5AD21
#cmd LnVisualComponent_SetPrototype_SetPrototype_OnSerialize2 $5B822
#cmd LnSpriteComponent_SetTexture $11720
#cmd LnSpriteComponent_SetPrototype_SetPrototype_OnSerialize $5C321
#cmd LnSpriteComponent_SetPrototype_SetPrototype_OnSerialize2 $5CE22
#cmd LnWorld_Add $11A20
#cmd LnWorld_SetPrototype_SetPrototype_OnSerialize $5D921
#cmd LnWorld_SetPrototype_SetPrototype_OnSerialize2 $5E422
#cmd LnComponentList_GetLength $3BC20
#cmd LnComponentList_GetItem $3BE20
#cmd LnComponentList_SetPrototype_SetPrototype_OnSerialize $5EF21
#cmd LnComponentList_SetPrototype_SetPrototype_OnSerialize2 $5FA22
#cmd LnWorldObject_SetPosition $11E20
#cmd LnWorldObject_SetPositionXYZ $12020
#cmd LnWorldObject_GetPosition $12420
#cmd LnWorldObject_SetRotationQuaternion $12520
#cmd LnWorldObject_SetRotation $12720
#cmd LnWorldObject_GetRotation $12B20
#cmd LnWorldObject_SetScale $12C20
#cmd LnWorldObject_SetScaleS $12E20
#cmd LnWorldObject_SetScaleXYZ $13020
#cmd LnWorldObject_GetScale $13420
#cmd LnWorldObject_SetCenterPoint $13520
#cmd LnWorldObject_SetCenterPointXYZ $13720
#cmd LnWorldObject_GetCenterPoint $13B20
#cmd LnWorldObject_LookAt $13C20
#cmd LnWorldObject_LookAtXYZ $13E20
#cmd LnWorldObject_AddComponent $14220
#cmd LnWorldObject_GetComponents $14420
#cmd LnWorldObject_SetPrototype_SetPrototype_OnSerialize $60521
#cmd LnWorldObject_SetPrototype_SetPrototype_OnSerialize2 $61022
#cmd LnWorldObject_SetPrototype_SetPrototype_OnUpdate $61B23
#cmd LnVisualObject_SetVisible $14820
#cmd LnVisualObject_IsVisible $14A20
#cmd LnVisualObject_SetPrototype_SetPrototype_OnSerialize $62621
#cmd LnVisualObject_SetPrototype_SetPrototype_OnSerialize2 $63122
#cmd LnVisualObject_SetPrototype_SetPrototype_OnUpdate $63C23
#cmd LnCamera_SetPrototype_SetPrototype_OnSerialize $64721
#cmd LnCamera_SetPrototype_SetPrototype_OnSerialize2 $65222
#cmd LnCamera_SetPrototype_SetPrototype_OnUpdate $65D23
#cmd LnDirectionalLight_Create $15C20
#cmd LnDirectionalLight_CreateWithColor $15D20
#cmd LnDirectionalLight_SetEnabled $14D20
#cmd LnDirectionalLight_IsEnabled $14F20
#cmd LnDirectionalLight_SetColor $15020
#cmd LnDirectionalLight_GetColor $15220
#cmd LnDirectionalLight_SetIntensity $15320
#cmd LnDirectionalLight_GetIntensity $15520
#cmd LnDirectionalLight_SetShadowEffectiveDistance $15620
#cmd LnDirectionalLight_GetShadowEffectiveDistance $15820
#cmd LnDirectionalLight_SetShadowEffectiveDepth $15920
#cmd LnDirectionalLight_GetShadowEffectiveDepth $15B20
#cmd LnDirectionalLight_SetPrototype_SetPrototype_OnSerialize $66821
#cmd LnDirectionalLight_SetPrototype_SetPrototype_OnSerialize2 $67322
#cmd LnDirectionalLight_SetPrototype_SetPrototype_OnUpdate $67E23
#cmd LnPointLight_Create $16F20
#cmd LnPointLight_CreateWithColorAndRange $17020
#cmd LnPointLight_SetEnabled $16020
#cmd LnPointLight_IsEnabled $16220
#cmd LnPointLight_SetColor $16320
#cmd LnPointLight_GetColor $16520
#cmd LnPointLight_SetIntensity $16620
#cmd LnPointLight_GetIntensity $16820
#cmd LnPointLight_SetRange $16920
#cmd LnPointLight_GetRange $16B20
#cmd LnPointLight_SetAttenuation $16C20
#cmd LnPointLight_GetAttenuation $16E20
#cmd LnPointLight_SetPrototype_SetPrototype_OnSerialize $68921
#cmd LnPointLight_SetPrototype_SetPrototype_OnSerialize2 $69422
#cmd LnPointLight_SetPrototype_SetPrototype_OnUpdate $69F23
#cmd LnSpotLight_Create $18920
#cmd LnSpotLight_CreateWithColorAndRange $18A20
#cmd LnSpotLight_SetEnabled $17420
#cmd LnSpotLight_IsEnabled $17620
#cmd LnSpotLight_SetColor $17720
#cmd LnSpotLight_GetColor $17920
#cmd LnSpotLight_SetIntensity $17A20
#cmd LnSpotLight_GetIntensity $17C20
#cmd LnSpotLight_SetRange $17D20
#cmd LnSpotLight_GetRange $17F20
#cmd LnSpotLight_SetAttenuation $18020
#cmd LnSpotLight_GetAttenuation $18220
#cmd LnSpotLight_SetAngle $18320
#cmd LnSpotLight_GetAngle $18520
#cmd LnSpotLight_SetPenumbra $18620
#cmd LnSpotLight_GetPenumbra $18820
#cmd LnSpotLight_SetPrototype_SetPrototype_OnSerialize $6AA21
#cmd LnSpotLight_SetPrototype_SetPrototype_OnSerialize2 $6B522
#cmd LnSpotLight_SetPrototype_SetPrototype_OnUpdate $6C023
#cmd LnTestDelegate_Create $43920
#cmd LnSprite_Create $1A020
#cmd LnSprite_CreateWithTexture $1A120
#cmd LnSprite_CreateWithTextureAndSize $1A320
#cmd LnSprite_SetTexture $19220
#cmd LnSprite_SetSize $19420
#cmd LnSprite_SetSizeWH $19620
#cmd LnSprite_SetSourceRectXYWH $19920
#cmd LnSprite_SetCallerTest $19E20
#cmd LnSprite_SetPrototype_SetPrototype_OnSerialize $6CB21
#cmd LnSprite_SetPrototype_SetPrototype_OnSerialize2 $6D622
#cmd LnSprite_SetPrototype_SetPrototype_OnUpdate $6E123
#cmd LnCameraOrbitControlComponent_Create $1A820
#cmd LnCameraOrbitControlComponent_SetPrototype_SetPrototype_OnSerialize $6EC21
#cmd LnCameraOrbitControlComponent_SetPrototype_SetPrototype_OnSerialize2 $6F722
#cmd LnRaycaster_FromScreen $1AA20
#cmd LnRaycaster_IntersectPlane $1AC20
#cmd LnRaycaster_SetPrototype_SetPrototype_OnSerialize $70221
#cmd LnRaycaster_SetPrototype_SetPrototype_OnSerialize2 $70D22
#cmd LnRaycastResult_GetPoint $1B120
#cmd LnRaycastResult_SetPrototype_SetPrototype_OnSerialize $71821
#cmd LnRaycastResult_SetPrototype_SetPrototype_OnSerialize2 $72322
#cmd LnWorldRenderView_SetGuideGridEnabled $1B320
#cmd LnWorldRenderView_GetGuideGridEnabled $1B520
#cmd LnWorldRenderView_SetPrototype_SetPrototype_OnSerialize $72E21
#cmd LnWorldRenderView_SetPrototype_SetPrototype_OnSerialize2 $73922
#cmd LnBoxMesh_Create $1B720
#cmd LnBoxMesh_SetPrototype_SetPrototype_OnSerialize $74421
#cmd LnBoxMesh_SetPrototype_SetPrototype_OnSerialize2 $74F22
#cmd LnBoxMesh_SetPrototype_SetPrototype_OnUpdate $75A23
#cmd LnPlaneMesh_Create $1B920
#cmd LnPlaneMesh_SetPrototype_SetPrototype_OnSerialize $76521
#cmd LnPlaneMesh_SetPrototype_SetPrototype_OnSerialize2 $77022
#cmd LnPlaneMesh_SetPrototype_SetPrototype_OnUpdate $77B23
#cmd LnUIEventArgs_Sender $1BB20
#cmd LnUIEventArgs_SetPrototype_SetPrototype_OnSerialize $78621
#cmd LnUIEventArgs_SetPrototype_SetPrototype_OnSerialize2 $79122
#cmd LnUIGeneralEventHandler_Create $45820
#cmd LnUIEventHandler_Create $45D20
#cmd LnUILayoutElement_SetPrototype_SetPrototype_OnSerialize $79C21
#cmd LnUILayoutElement_SetPrototype_SetPrototype_OnSerialize2 $7A722
#cmd LnUIElement_SetMargin $1CD20
#cmd LnUIElement_GetMargin $1CF20
#cmd LnUIElement_SetPadding $1D020
#cmd LnUIElement_GetPadding $1D220
#cmd LnUIElement_SetHAlignment $1D320
#cmd LnUIElement_GetHAlignment $1D520
#cmd LnUIElement_SetVAlignment $1D620
#cmd LnUIElement_GetVAlignment $1D820
#cmd LnUIElement_SetAlignments $1D920
#cmd LnUIElement_SetPosition $1DC20
#cmd LnUIElement_SetPositionXYZ $1DE20
#cmd LnUIElement_GetPosition $1E220
#cmd LnUIElement_SetRotation $1E320
#cmd LnUIElement_SetEulerAngles $1E520
#cmd LnUIElement_GetRotation $1E920
#cmd LnUIElement_SetScale $1EA20
#cmd LnUIElement_SetScaleS $1EC20
#cmd LnUIElement_SetScaleXY $1EE20
#cmd LnUIElement_GetScale $1F120
#cmd LnUIElement_SetCenterPoint $1F220
#cmd LnUIElement_SetCenterPointXYZ $1F420
#cmd LnUIElement_GetCenterPoint $1F820
#cmd LnUIElement_AddChild $1F920
#cmd LnUIElement_SetPrototype_SetPrototype_OnSerialize $7B221
#cmd LnUIElement_SetPrototype_SetPrototype_OnSerialize2 $7BD22
#cmd LnUITextBlock_Create $1FC20
#cmd LnUITextBlock_CreateWithTextA $1FD20
#cmd LnUITextBlock_SetPrototype_SetPrototype_OnSerialize $7C821
#cmd LnUITextBlock_SetPrototype_SetPrototype_OnSerialize2 $7D322
#cmd LnUISprite_Create $20A20
#cmd LnUISprite_CreateWithTexture $20B20
#cmd LnUISprite_SetTexture $20020
#cmd LnUISprite_SetSourceRect $20220
#cmd LnUISprite_SetSourceRectXYWH $20420
#cmd LnUISprite_GetSourceRect $20920
#cmd LnUISprite_SetPrototype_SetPrototype_OnSerialize $7DE21
#cmd LnUISprite_SetPrototype_SetPrototype_OnSerialize2 $7E922
#cmd LnInput_PressedA $20E20
#cmd LnInput_TriggeredA $21020
#cmd LnInput_TriggeredOffA $21220
#cmd LnInput_RepeatedA $21420
#cmd LnInput_GetAxisValueA $21620
#cmd LnInput_ClearAllBindings $21820
#cmd LnMouse_Pressed $21A20
#cmd LnMouse_Triggered $21C20
#cmd LnMouse_TriggeredOff $21E20
#cmd LnMouse_Repeated $22020
#cmd LnMouse_Position $22220
#cmd LnEngineSettings_SetMainWindowSize $22420
#cmd LnEngineSettings_SetMainWorldViewSize $22720
#cmd LnEngineSettings_SetMainWindowTitleA $22A20
#cmd LnEngineSettings_AddAssetDirectoryA $22C20
#cmd LnEngineSettings_AddAssetArchiveA $22E20
#cmd LnEngineSettings_SetFrameRate $23120
#cmd LnEngineSettings_SetDebugToolEnabled $23320
#cmd LnEngineSettings_SetEngineLogEnabled $23520
#cmd LnEngineSettings_SetEngineLogFilePathA $23720
#cmd LnEngine_Initialize $23A20
#cmd LnEngine_Finalize $23B20
#cmd LnEngine_Update $23C20
#cmd LnEngine_Time $23D20
#cmd LnEngine_GetCamera $23E20
#cmd LnEngine_GetLight $23F20
#cmd LnEngine_GetRenderView $24020
#cmd LnApplication_Create $24520
#cmd LnApplication_OnInit $24220
#cmd LnApplication_OnUpdate $24320
#cmd LnApplication_World $24420
#cmd LnApplication_SetPrototype_SetPrototype_OnSerialize $7F421
#cmd LnApplication_SetPrototype_SetPrototype_OnSerialize2 $7FF22
#cmd LnApplication_SetPrototype_SetPrototype_OnInit $80A23
#cmd LnApplication_SetPrototype_SetPrototype_OnUpdate $81524
#cmd LnDebug_PrintA $24720
#cmd LnDebug_PrintWithTimeA $24920
#cmd LnDebug_PrintWithTimeAndColorA $24C20
#cmd LnObjectSerializeHandler_Create $4B920
#cmd LnObjectSerialize2Handler_Create $4C420
#cmd LnEventConnectionSerializeHandler_Create $4CF20
#cmd LnEventConnectionSerialize2Handler_Create $4DA20
#cmd LnZVTestClass1SerializeHandler_Create $4E520
#cmd LnZVTestClass1Serialize2Handler_Create $4F020
#cmd LnZVTestEventArgs1SerializeHandler_Create $4FB20
#cmd LnZVTestEventArgs1Serialize2Handler_Create $50620
#cmd LnSerializerSerializeHandler_Create $51120
#cmd LnSerializerSerialize2Handler_Create $51C20
#cmd LnSerializer2SerializeHandler_Create $52720
#cmd LnSerializer2Serialize2Handler_Create $53220
#cmd LnAssetModelSerializeHandler_Create $53D20
#cmd LnAssetModelSerialize2Handler_Create $54820
#cmd LnTextureSerializeHandler_Create $55320
#cmd LnTextureSerialize2Handler_Create $55E20
#cmd LnTexture2DSerializeHandler_Create $56920
#cmd LnTexture2DSerialize2Handler_Create $57420
#cmd LnRenderViewSerializeHandler_Create $57F20
#cmd LnRenderViewSerialize2Handler_Create $58A20
#cmd LnComponentSerializeHandler_Create $59520
#cmd LnComponentSerialize2Handler_Create $5A020
#cmd LnVisualComponentSerializeHandler_Create $5AB20
#cmd LnVisualComponentSerialize2Handler_Create $5B620
#cmd LnSpriteComponentSerializeHandler_Create $5C120
#cmd LnSpriteComponentSerialize2Handler_Create $5CC20
#cmd LnWorldSerializeHandler_Create $5D720
#cmd LnWorldSerialize2Handler_Create $5E220
#cmd LnComponentListSerializeHandler_Create $5ED20
#cmd LnComponentListSerialize2Handler_Create $5F820
#cmd LnWorldObjectSerializeHandler_Create $60320
#cmd LnWorldObjectSerialize2Handler_Create $60E20
#cmd LnWorldObjectUpdateHandler_Create $61920
#cmd LnVisualObjectSerializeHandler_Create $62420
#cmd LnVisualObjectSerialize2Handler_Create $62F20
#cmd LnVisualObjectUpdateHandler_Create $63A20
#cmd LnCameraSerializeHandler_Create $64520
#cmd LnCameraSerialize2Handler_Create $65020
#cmd LnCameraUpdateHandler_Create $65B20
#cmd LnDirectionalLightSerializeHandler_Create $66620
#cmd LnDirectionalLightSerialize2Handler_Create $67120
#cmd LnDirectionalLightUpdateHandler_Create $67C20
#cmd LnPointLightSerializeHandler_Create $68720
#cmd LnPointLightSerialize2Handler_Create $69220
#cmd LnPointLightUpdateHandler_Create $69D20
#cmd LnSpotLightSerializeHandler_Create $6A820
#cmd LnSpotLightSerialize2Handler_Create $6B320
#cmd LnSpotLightUpdateHandler_Create $6BE20
#cmd LnSpriteSerializeHandler_Create $6C920
#cmd LnSpriteSerialize2Handler_Create $6D420
#cmd LnSpriteUpdateHandler_Create $6DF20
#cmd LnCameraOrbitControlComponentSerializeHandler_Create $6EA20
#cmd LnCameraOrbitControlComponentSerialize2Handler_Create $6F520
#cmd LnRaycasterSerializeHandler_Create $70020
#cmd LnRaycasterSerialize2Handler_Create $70B20
#cmd LnRaycastResultSerializeHandler_Create $71620
#cmd LnRaycastResultSerialize2Handler_Create $72120
#cmd LnWorldRenderViewSerializeHandler_Create $72C20
#cmd LnWorldRenderViewSerialize2Handler_Create $73720
#cmd LnBoxMeshSerializeHandler_Create $74220
#cmd LnBoxMeshSerialize2Handler_Create $74D20
#cmd LnBoxMeshUpdateHandler_Create $75820
#cmd LnPlaneMeshSerializeHandler_Create $76320
#cmd LnPlaneMeshSerialize2Handler_Create $76E20
#cmd LnPlaneMeshUpdateHandler_Create $77920
#cmd LnUIEventArgsSerializeHandler_Create $78420
#cmd LnUIEventArgsSerialize2Handler_Create $78F20
#cmd LnUILayoutElementSerializeHandler_Create $79A20
#cmd LnUILayoutElementSerialize2Handler_Create $7A520
#cmd LnUIElementSerializeHandler_Create $7B020
#cmd LnUIElementSerialize2Handler_Create $7BB20
#cmd LnUITextBlockSerializeHandler_Create $7C620
#cmd LnUITextBlockSerialize2Handler_Create $7D120
#cmd LnUISpriteSerializeHandler_Create $7DC20
#cmd LnUISpriteSerialize2Handler_Create $7E720
#cmd LnApplicationSerializeHandler_Create $7F220
#cmd LnApplicationSerialize2Handler_Create $7FD20
#cmd LnApplicationInitHandler_Create $80820
#cmd LnApplicationUpdateHandler_Create $81320



#cmd ln_args $1

#endif // __lumino__
