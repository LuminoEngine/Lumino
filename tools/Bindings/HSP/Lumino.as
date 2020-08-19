
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

#cmd LnObject_SetPrototype_OnSerialize $4E21
#cmd LnObject_SetPrototype_OnSerialize2 $5022
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
#cmd LnZVTestEventArgs1_Create $8020
#cmd LnZVTestEventArgs1_CreateWithValue $8120
#cmd LnZVTestEventArgs1_GetValue $7F20
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
#cmd LnAssetModel_Create $AD20
#cmd LnAssetModel_Target $AC20
#cmd LnAssets_SaveAssetToLocalFileA $B020
#cmd LnAssets_LoadAssetFromLocalFileA $B320
#cmd LnAssets_LoadAssetA $B520
#cmd LnAssets_ReloadAssetA $B720
#cmd LnTexture2DDelegate_Create $32820
#cmd LnTexture2DPromise_ThenWith $32B20
#cmd LnTexture2DPromise_CatchWith $32D20
#cmd LnTexture2D_Create $10920
#cmd LnTexture2D_CreateWithFormat $10C20
#cmd LnTexture2D_LoadA $10520
#cmd LnTexture2D_LoadEmojiA $10720
#cmd LnVisualComponent_SetVisible $11320
#cmd LnVisualComponent_IsVisible $11520
#cmd LnSpriteComponent_SetTexture $11720
#cmd LnWorld_Add $11A20
#cmd LnComponentList_GetLength $3BC20
#cmd LnComponentList_GetItem $3BE20
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
#cmd LnWorldObject_SetPrototype_OnUpdate $14521
#cmd LnVisualObject_SetVisible $14820
#cmd LnVisualObject_IsVisible $14A20
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
#cmd LnTestDelegate_Create $43920
#cmd LnSprite_Create $1A020
#cmd LnSprite_CreateWithTexture $1A120
#cmd LnSprite_CreateWithTextureAndSize $1A320
#cmd LnSprite_SetTexture $19220
#cmd LnSprite_SetSize $19420
#cmd LnSprite_SetSizeWH $19620
#cmd LnSprite_SetSourceRectXYWH $19920
#cmd LnSprite_SetCallerTest $19E20
#cmd LnCameraOrbitControlComponent_Create $1A820
#cmd LnRaycaster_FromScreen $1AA20
#cmd LnRaycaster_IntersectPlane $1AC20
#cmd LnRaycastResult_GetPoint $1B120
#cmd LnWorldRenderView_SetGuideGridEnabled $1B320
#cmd LnWorldRenderView_GetGuideGridEnabled $1B520
#cmd LnBoxMesh_Create $1B720
#cmd LnPlaneMesh_Create $1B920
#cmd LnUIEventArgs_Sender $1BB20
#cmd LnUIGeneralEventHandler_Create $45820
#cmd LnUIEventHandler_Create $45D20
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
#cmd LnUITextBlock_Create $1FC20
#cmd LnUITextBlock_CreateWithTextA $1FD20
#cmd LnUISprite_Create $20A20
#cmd LnUISprite_CreateWithTexture $20B20
#cmd LnUISprite_SetTexture $20020
#cmd LnUISprite_SetSourceRect $20220
#cmd LnUISprite_SetSourceRectXYWH $20420
#cmd LnUISprite_GetSourceRect $20920
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
#cmd LnApplication_SetPrototype_OnInit $24221
#cmd LnApplication_SetPrototype_OnUpdate $24322
#cmd LnDebug_PrintA $24720
#cmd LnDebug_PrintWithTimeA $24920
#cmd LnDebug_PrintWithTimeAndColorA $24C20



#cmd ln_args $1

#endif // __lumino__
