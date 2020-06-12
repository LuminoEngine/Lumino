
#ifndef __lumino__
#define __lumino__

#regcmd "_hsp3cmdinit@4","LuminoHSP.dll", 6

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
#const global LN_TEXTURE_FORMAT_UNKNOWN 0
#const global LN_TEXTURE_FORMAT_RGBA8 1
#const global LN_TEXTURE_FORMAT_RGB8 2
#const global LN_TEXTURE_FORMAT_RGBA16F 3
#const global LN_TEXTURE_FORMAT_RGBA32F 4
#const global LN_TEXTURE_FORMAT_R16F 5
#const global LN_TEXTURE_FORMAT_R32F 6
#const global LN_TEXTURE_FORMAT_R32U 7
#const global LN_DEPTH_BUFFER_FORMAT_D24S8 0
#const global LN_VALIGNMENT_TOP 0
#const global LN_VALIGNMENT_CENTER 1
#const global LN_VALIGNMENT_BOTTOM 2
#const global LN_VALIGNMENT_STRETCH 3
#const global LN_HALIGNMENT_LEFT 0
#const global LN_HALIGNMENT_CENTER 1
#const global LN_HALIGNMENT_RIGHT 2
#const global LN_HALIGNMENT_STRETCH 3

#cmd LnVector3 $2B
#cmd LnVector4 $3D
#cmd LnQuaternion $48
#cmd LnMatrix $56
#cmd LnColor $E8
#cmd LnPoint $F3
#cmd LnSize $FA
#cmd LnRect $101
#cmd LnThickness $110

#cmd LnPromiseFailureDelegate_Create $2EE
#cmd LnZVTestDelegate1_Create $2F3
#cmd LnZVTestDelegate2_Create $2F9
#cmd LnZVTestDelegate3_Create $2FE
#cmd LnZVTestEventHandler1_Create $303
#cmd LnZVTestEventHandler2_Create $308
#cmd LnZVTestPromise1_ThenWith $30B
#cmd LnZVTestPromise1_CatchWith $30D
#cmd LnZVTestPromise2_ThenWith $311
#cmd LnZVTestPromise2_CatchWith $313
#cmd LnZVTestClass1_SetTestDelegate1 $87
#cmd LnZVTestClass1_SetTestDelegate2 $89
#cmd LnZVTestClass1_SetTestDelegate3 $8B
#cmd LnZVTestClass1_CallTestDelegate1 $8D
#cmd LnZVTestClass1_CallTestDelegate2 $8F
#cmd LnZVTestClass1_CallTestDelegate3 $92
#cmd LnZVTestClass1_LoadAsyncA $93
#cmd LnZVTestClass1_ExecuteAsync $95
#cmd LnZVTestClass1_GetFilePathA $96
#cmd LnZVTestClass1_ConnectOnEvent1 $97
#cmd LnZVTestClass1_RaiseEvent1 $99
#cmd LnZVTestClass1_ConnectOnEvent2 $9A
#cmd LnZVTestClass1_RaiseEvent2 $9C
#cmd LnZVTestClass1_Create $9D
#cmd LnZVTestEventArgs1_GetValue $9F
#cmd LnZVTestEventArgs1_Create $A0
#cmd LnZVTestEventArgs1_CreateWithValue $A1
#cmd LnSerializer_WriteBoolA $A4
#cmd LnSerializer_WriteIntA $A7
#cmd LnSerializer_WriteFloatA $AA
#cmd LnSerializer_WriteStringA $AD
#cmd LnSerializer_WriteObjectA $B0
#cmd LnSerializer_ReadBoolA $B3
#cmd LnSerializer_ReadIntA $B5
#cmd LnSerializer_ReadFloatA $B7
#cmd LnSerializer_ReadStringA $B9
#cmd LnSerializer_ReadObjectA $BB
#cmd LnSerializer_SerializeA $BD
#cmd LnSerializer_DeserializeA $C0
#cmd LnAssetModel_Target $CC
#cmd LnAssetModel_Create $CD
#cmd LnAssets_SaveAssetToLocalFileA $D0
#cmd LnAssets_LoadAssetFromLocalFileA $D3
#cmd LnAssets_LoadAssetA $D5
#cmd LnTexture2D_LoadA $11E
#cmd LnTexture2D_LoadEmojiA $120
#cmd LnTexture2D_Create $122
#cmd LnTexture2D_CreateWithFormat $125
#cmd LnTexture2D_CreateFromFileA $129
#cmd LnVisualComponent_SetVisible $12F
#cmd LnVisualComponent_IsVisible $131
#cmd LnSpriteComponent_SetTexture $133
#cmd LnWorld_Add $136
#cmd LnComponentList_GetLength $3D3
#cmd LnComponentList_GetItem $3D5
#cmd LnWorldObject_SetPosition $13A
#cmd LnWorldObject_SetPositionXYZ $13C
#cmd LnWorldObject_GetPosition $140
#cmd LnWorldObject_SetRotationQuaternion $141
#cmd LnWorldObject_SetRotation $143
#cmd LnWorldObject_GetRotation $147
#cmd LnWorldObject_SetScale $148
#cmd LnWorldObject_SetScaleS $14A
#cmd LnWorldObject_SetScaleXYZ $14C
#cmd LnWorldObject_GetScale $150
#cmd LnWorldObject_SetCenterPoint $151
#cmd LnWorldObject_SetCenterPointXYZ $153
#cmd LnWorldObject_GetCenterPoint $157
#cmd LnWorldObject_LookAt $158
#cmd LnWorldObject_LookAtXYZ $15A
#cmd LnWorldObject_AddComponent $15E
#cmd LnWorldObject_GetComponents $160
#cmd LnVisualObject_SetVisible $164
#cmd LnVisualObject_IsVisible $166
#cmd LnDirectionalLight_SetEnabled $169
#cmd LnDirectionalLight_IsEnabled $16B
#cmd LnDirectionalLight_SetColor $16C
#cmd LnDirectionalLight_GetColor $16E
#cmd LnDirectionalLight_SetIntensity $16F
#cmd LnDirectionalLight_GetIntensity $171
#cmd LnDirectionalLight_Create $172
#cmd LnDirectionalLight_CreateWithColor $173
#cmd LnPointLight_SetEnabled $176
#cmd LnPointLight_IsEnabled $178
#cmd LnPointLight_SetColor $179
#cmd LnPointLight_GetColor $17B
#cmd LnPointLight_SetIntensity $17C
#cmd LnPointLight_GetIntensity $17E
#cmd LnPointLight_SetRange $17F
#cmd LnPointLight_GetRange $181
#cmd LnPointLight_SetAttenuation $182
#cmd LnPointLight_GetAttenuation $184
#cmd LnPointLight_Create $185
#cmd LnPointLight_CreateWithColorAndRange $186
#cmd LnSpotLight_SetEnabled $18A
#cmd LnSpotLight_IsEnabled $18C
#cmd LnSpotLight_SetColor $18D
#cmd LnSpotLight_GetColor $18F
#cmd LnSpotLight_SetIntensity $190
#cmd LnSpotLight_GetIntensity $192
#cmd LnSpotLight_SetRange $193
#cmd LnSpotLight_GetRange $195
#cmd LnSpotLight_SetAttenuation $196
#cmd LnSpotLight_GetAttenuation $198
#cmd LnSpotLight_SetAngle $199
#cmd LnSpotLight_GetAngle $19B
#cmd LnSpotLight_SetPenumbra $19C
#cmd LnSpotLight_GetPenumbra $19E
#cmd LnSpotLight_Create $19F
#cmd LnSpotLight_CreateWithColorAndRange $1A0
#cmd LnTestDelegate_Create $446
#cmd LnSprite_SetTexture $1A8
#cmd LnSprite_SetSize $1AA
#cmd LnSprite_SetSizeWH $1AC
#cmd LnSprite_SetSourceRectXYWH $1AF
#cmd LnSprite_SetCallerTest $1B4
#cmd LnSprite_Create $1B6
#cmd LnSprite_CreateWithTexture $1B7
#cmd LnSprite_CreateWithTextureAndSize $1B9
#cmd LnCameraOrbitControlComponent_Create $1BE
#cmd LnRaycaster_FromScreen $1C0
#cmd LnRaycaster_IntersectPlane $1C2
#cmd LnRaycastResult_GetPoint $1C7
#cmd LnWorldRenderView_SetGuideGridEnabled $1C9
#cmd LnWorldRenderView_GetGuideGridEnabled $1CB
#cmd LnBoxMesh_Create $1CD
#cmd LnPlaneMesh_Create $1CF
#cmd LnUIEventArgs_Sender $1D1
#cmd LnUIGeneralEventHandler_Create $465
#cmd LnUIEventHandler_Create $46A
#cmd LnUIElement_SetMargin $1E3
#cmd LnUIElement_GetMargin $1E5
#cmd LnUIElement_SetPadding $1E6
#cmd LnUIElement_GetPadding $1E8
#cmd LnUIElement_SetHAlignment $1E9
#cmd LnUIElement_GetHAlignment $1EB
#cmd LnUIElement_SetVAlignment $1EC
#cmd LnUIElement_GetVAlignment $1EE
#cmd LnUIElement_SetAlignments $1EF
#cmd LnUIElement_SetPosition $1F2
#cmd LnUIElement_SetPositionXYZ $1F4
#cmd LnUIElement_GetPosition $1F8
#cmd LnUIElement_SetRotation $1F9
#cmd LnUIElement_SetEulerAngles $1FB
#cmd LnUIElement_GetRotation $1FF
#cmd LnUIElement_SetScale $200
#cmd LnUIElement_SetScaleS $202
#cmd LnUIElement_SetScaleXY $204
#cmd LnUIElement_GetScale $207
#cmd LnUIElement_SetCenterPoint $208
#cmd LnUIElement_SetCenterPointXYZ $20A
#cmd LnUIElement_GetCenterPoint $20E
#cmd LnUIElement_AddChild $20F
#cmd LnUIButtonBase_SetTextA $213
#cmd LnUIButton_Create $216
#cmd LnUIButton_CreateWithTextA $217
#cmd LnUIButton_ConnectOnClicked $219
#cmd LnUITextBlock_Create $21C
#cmd LnUITextBlock_CreateWithTextA $21D
#cmd LnUISprite_SetTexture $220
#cmd LnUISprite_SetSourceRect $222
#cmd LnUISprite_SetSourceRectXYWH $224
#cmd LnUISprite_GetSourceRect $229
#cmd LnUISprite_Create $22A
#cmd LnUISprite_CreateWithTexture $22B
#cmd LnInput_PressedA $22E
#cmd LnInput_TriggeredA $230
#cmd LnInput_TriggeredOffA $232
#cmd LnInput_RepeatedA $234
#cmd LnInput_GetAxisValueA $236
#cmd LnInput_ClearAllBindings $238
#cmd LnMouse_Pressed $23A
#cmd LnMouse_Triggered $23C
#cmd LnMouse_TriggeredOff $23E
#cmd LnMouse_Repeated $240
#cmd LnMouse_Position $242
#cmd LnEngineSettings_SetMainWindowSize $244
#cmd LnEngineSettings_SetMainWorldViewSize $247
#cmd LnEngineSettings_SetMainWindowTitleA $24A
#cmd LnEngineSettings_AddAssetDirectoryA $24C
#cmd LnEngineSettings_AddAssetArchiveA $24E
#cmd LnEngineSettings_SetFrameRate $251
#cmd LnEngineSettings_SetDebugToolEnabled $253
#cmd LnEngineSettings_SetEngineLogEnabled $255
#cmd LnEngineSettings_SetEngineLogFilePathA $257
#cmd LnEngine_Initialize $25A
#cmd LnEngine_Finalize $25B
#cmd LnEngine_Update $25C
#cmd LnEngine_Time $25D
#cmd LnEngine_GetCamera $25E
#cmd LnEngine_GetLight $25F
#cmd LnEngine_GetRenderView $260
#cmd LnApplication_OnInit $262
#cmd LnApplication_OnUpdate $263
#cmd LnApplication_World $264
#cmd LnApplication_Create $265
#cmd LnDebug_PrintA $267
#cmd LnDebug_PrintWithTimeA $269
#cmd LnDebug_PrintWithTimeAndColorA $26C



#cmd ln_args $1

#endif // __lumino__
