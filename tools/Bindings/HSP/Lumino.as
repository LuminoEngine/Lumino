
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
#cmd LnColor $EB
#cmd LnPoint $F6
#cmd LnSize $FD
#cmd LnRect $104
#cmd LnThickness $113

#cmd LnPromiseFailureDelegate_Create $2F0
#cmd LnZVTestDelegate1_Create $2F5
#cmd LnZVTestDelegate2_Create $2FB
#cmd LnZVTestDelegate3_Create $300
#cmd LnZVTestEventHandler1_Create $305
#cmd LnZVTestEventHandler2_Create $30A
#cmd LnZVTestPromise1_ThenWith $30D
#cmd LnZVTestPromise1_CatchWith $30F
#cmd LnZVTestPromise2_ThenWith $313
#cmd LnZVTestPromise2_CatchWith $315
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
#cmd LnAssets_ReloadAssetA $D7
#cmd LnTexture2D_LoadA $120
#cmd LnTexture2D_LoadEmojiA $122
#cmd LnTexture2D_Create $124
#cmd LnTexture2D_CreateWithFormat $127
#cmd LnTexture2D_CreateFromFileA $12B
#cmd LnVisualComponent_SetVisible $131
#cmd LnVisualComponent_IsVisible $133
#cmd LnSpriteComponent_SetTexture $135
#cmd LnWorld_Add $138
#cmd LnComponentList_GetLength $3D5
#cmd LnComponentList_GetItem $3D7
#cmd LnWorldObject_SetPosition $13C
#cmd LnWorldObject_SetPositionXYZ $13E
#cmd LnWorldObject_GetPosition $142
#cmd LnWorldObject_SetRotationQuaternion $143
#cmd LnWorldObject_SetRotation $145
#cmd LnWorldObject_GetRotation $149
#cmd LnWorldObject_SetScale $14A
#cmd LnWorldObject_SetScaleS $14C
#cmd LnWorldObject_SetScaleXYZ $14E
#cmd LnWorldObject_GetScale $152
#cmd LnWorldObject_SetCenterPoint $153
#cmd LnWorldObject_SetCenterPointXYZ $155
#cmd LnWorldObject_GetCenterPoint $159
#cmd LnWorldObject_LookAt $15A
#cmd LnWorldObject_LookAtXYZ $15C
#cmd LnWorldObject_AddComponent $160
#cmd LnWorldObject_GetComponents $162
#cmd LnVisualObject_SetVisible $166
#cmd LnVisualObject_IsVisible $168
#cmd LnDirectionalLight_SetEnabled $16B
#cmd LnDirectionalLight_IsEnabled $16D
#cmd LnDirectionalLight_SetColor $16E
#cmd LnDirectionalLight_GetColor $170
#cmd LnDirectionalLight_SetIntensity $171
#cmd LnDirectionalLight_GetIntensity $173
#cmd LnDirectionalLight_Create $174
#cmd LnDirectionalLight_CreateWithColor $175
#cmd LnPointLight_SetEnabled $178
#cmd LnPointLight_IsEnabled $17A
#cmd LnPointLight_SetColor $17B
#cmd LnPointLight_GetColor $17D
#cmd LnPointLight_SetIntensity $17E
#cmd LnPointLight_GetIntensity $180
#cmd LnPointLight_SetRange $181
#cmd LnPointLight_GetRange $183
#cmd LnPointLight_SetAttenuation $184
#cmd LnPointLight_GetAttenuation $186
#cmd LnPointLight_Create $187
#cmd LnPointLight_CreateWithColorAndRange $188
#cmd LnSpotLight_SetEnabled $18C
#cmd LnSpotLight_IsEnabled $18E
#cmd LnSpotLight_SetColor $18F
#cmd LnSpotLight_GetColor $191
#cmd LnSpotLight_SetIntensity $192
#cmd LnSpotLight_GetIntensity $194
#cmd LnSpotLight_SetRange $195
#cmd LnSpotLight_GetRange $197
#cmd LnSpotLight_SetAttenuation $198
#cmd LnSpotLight_GetAttenuation $19A
#cmd LnSpotLight_SetAngle $19B
#cmd LnSpotLight_GetAngle $19D
#cmd LnSpotLight_SetPenumbra $19E
#cmd LnSpotLight_GetPenumbra $1A0
#cmd LnSpotLight_Create $1A1
#cmd LnSpotLight_CreateWithColorAndRange $1A2
#cmd LnTestDelegate_Create $448
#cmd LnSprite_SetTexture $1AA
#cmd LnSprite_SetSize $1AC
#cmd LnSprite_SetSizeWH $1AE
#cmd LnSprite_SetSourceRectXYWH $1B1
#cmd LnSprite_SetCallerTest $1B6
#cmd LnSprite_Create $1B8
#cmd LnSprite_CreateWithTexture $1B9
#cmd LnSprite_CreateWithTextureAndSize $1BB
#cmd LnCameraOrbitControlComponent_Create $1C0
#cmd LnRaycaster_FromScreen $1C2
#cmd LnRaycaster_IntersectPlane $1C4
#cmd LnRaycastResult_GetPoint $1C9
#cmd LnWorldRenderView_SetGuideGridEnabled $1CB
#cmd LnWorldRenderView_GetGuideGridEnabled $1CD
#cmd LnBoxMesh_Create $1CF
#cmd LnPlaneMesh_Create $1D1
#cmd LnUIEventArgs_Sender $1D3
#cmd LnUIGeneralEventHandler_Create $467
#cmd LnUIEventHandler_Create $46C
#cmd LnUIElement_SetMargin $1E5
#cmd LnUIElement_GetMargin $1E7
#cmd LnUIElement_SetPadding $1E8
#cmd LnUIElement_GetPadding $1EA
#cmd LnUIElement_SetHAlignment $1EB
#cmd LnUIElement_GetHAlignment $1ED
#cmd LnUIElement_SetVAlignment $1EE
#cmd LnUIElement_GetVAlignment $1F0
#cmd LnUIElement_SetAlignments $1F1
#cmd LnUIElement_SetPosition $1F4
#cmd LnUIElement_SetPositionXYZ $1F6
#cmd LnUIElement_GetPosition $1FA
#cmd LnUIElement_SetRotation $1FB
#cmd LnUIElement_SetEulerAngles $1FD
#cmd LnUIElement_GetRotation $201
#cmd LnUIElement_SetScale $202
#cmd LnUIElement_SetScaleS $204
#cmd LnUIElement_SetScaleXY $206
#cmd LnUIElement_GetScale $209
#cmd LnUIElement_SetCenterPoint $20A
#cmd LnUIElement_SetCenterPointXYZ $20C
#cmd LnUIElement_GetCenterPoint $210
#cmd LnUIElement_AddChild $211
#cmd LnUIButtonBase_SetTextA $215
#cmd LnUIButton_Create $218
#cmd LnUIButton_CreateWithTextA $219
#cmd LnUIButton_ConnectOnClicked $21B
#cmd LnUITextBlock_Create $21E
#cmd LnUITextBlock_CreateWithTextA $21F
#cmd LnUISprite_SetTexture $222
#cmd LnUISprite_SetSourceRect $224
#cmd LnUISprite_SetSourceRectXYWH $226
#cmd LnUISprite_GetSourceRect $22B
#cmd LnUISprite_Create $22C
#cmd LnUISprite_CreateWithTexture $22D
#cmd LnInput_PressedA $230
#cmd LnInput_TriggeredA $232
#cmd LnInput_TriggeredOffA $234
#cmd LnInput_RepeatedA $236
#cmd LnInput_GetAxisValueA $238
#cmd LnInput_ClearAllBindings $23A
#cmd LnMouse_Pressed $23C
#cmd LnMouse_Triggered $23E
#cmd LnMouse_TriggeredOff $240
#cmd LnMouse_Repeated $242
#cmd LnMouse_Position $244
#cmd LnEngineSettings_SetMainWindowSize $246
#cmd LnEngineSettings_SetMainWorldViewSize $249
#cmd LnEngineSettings_SetMainWindowTitleA $24C
#cmd LnEngineSettings_AddAssetDirectoryA $24E
#cmd LnEngineSettings_AddAssetArchiveA $250
#cmd LnEngineSettings_SetFrameRate $253
#cmd LnEngineSettings_SetDebugToolEnabled $255
#cmd LnEngineSettings_SetEngineLogEnabled $257
#cmd LnEngineSettings_SetEngineLogFilePathA $259
#cmd LnEngine_Initialize $25C
#cmd LnEngine_Finalize $25D
#cmd LnEngine_Update $25E
#cmd LnEngine_Time $25F
#cmd LnEngine_GetCamera $260
#cmd LnEngine_GetLight $261
#cmd LnEngine_GetRenderView $262
#cmd LnApplication_OnInit $264
#cmd LnApplication_OnUpdate $265
#cmd LnApplication_World $266
#cmd LnApplication_Create $267
#cmd LnDebug_PrintA $269
#cmd LnDebug_PrintWithTimeA $26B
#cmd LnDebug_PrintWithTimeAndColorA $26E



#cmd ln_args $1

#endif // __lumino__
