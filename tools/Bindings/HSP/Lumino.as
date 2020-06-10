
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

#cmd LnVector3 $B
#cmd LnVector4 $1D
#cmd LnQuaternion $28
#cmd LnMatrix $36
#cmd LnColor $C8
#cmd LnPoint $D3
#cmd LnSize $DA
#cmd LnRect $E1
#cmd LnThickness $F0

#cmd LnPromiseFailureDelegate_Create $2CE
#cmd LnZVTestDelegate1_Create $2D3
#cmd LnZVTestDelegate2_Create $2D9
#cmd LnZVTestDelegate3_Create $2DE
#cmd LnZVTestEventHandler1_Create $2E3
#cmd LnZVTestEventHandler2_Create $2E8
#cmd LnZVTestPromise1_ThenWith $2EB
#cmd LnZVTestPromise1_CatchWith $2ED
#cmd LnZVTestPromise2_ThenWith $2F1
#cmd LnZVTestPromise2_CatchWith $2F3
#cmd LnZVTestClass1_SetTestDelegate1 $67
#cmd LnZVTestClass1_SetTestDelegate2 $69
#cmd LnZVTestClass1_SetTestDelegate3 $6B
#cmd LnZVTestClass1_CallTestDelegate1 $6D
#cmd LnZVTestClass1_CallTestDelegate2 $6F
#cmd LnZVTestClass1_CallTestDelegate3 $72
#cmd LnZVTestClass1_LoadAsyncA $73
#cmd LnZVTestClass1_ExecuteAsync $75
#cmd LnZVTestClass1_GetFilePathA $76
#cmd LnZVTestClass1_ConnectOnEvent1 $77
#cmd LnZVTestClass1_RaiseEvent1 $79
#cmd LnZVTestClass1_ConnectOnEvent2 $7A
#cmd LnZVTestClass1_RaiseEvent2 $7C
#cmd LnZVTestClass1_Create $7D
#cmd LnZVTestEventArgs1_GetValue $7F
#cmd LnZVTestEventArgs1_Create $80
#cmd LnZVTestEventArgs1_CreateWithValue $81
#cmd LnSerializer_WriteBoolA $84
#cmd LnSerializer_WriteIntA $87
#cmd LnSerializer_WriteFloatA $8A
#cmd LnSerializer_WriteStringA $8D
#cmd LnSerializer_WriteObjectA $90
#cmd LnSerializer_ReadBoolA $93
#cmd LnSerializer_ReadIntA $95
#cmd LnSerializer_ReadFloatA $97
#cmd LnSerializer_ReadStringA $99
#cmd LnSerializer_ReadObjectA $9B
#cmd LnSerializer_SerializeA $9D
#cmd LnSerializer_DeserializeA $A0
#cmd LnAssetModel_Target $AC
#cmd LnAssetModel_Create $AD
#cmd LnAssets_SaveAssetToLocalFileA $B0
#cmd LnAssets_LoadAssetFromLocalFileA $B3
#cmd LnAssets_LoadAssetA $B5
#cmd LnTexture2D_LoadA $FE
#cmd LnTexture2D_LoadEmojiA $100
#cmd LnTexture2D_Create $102
#cmd LnTexture2D_CreateWithFormat $105
#cmd LnTexture2D_CreateFromFileA $109
#cmd LnVisualComponent_SetVisible $10F
#cmd LnVisualComponent_IsVisible $111
#cmd LnSpriteComponent_SetTexture $113
#cmd LnWorld_Add $116
#cmd LnComponentList_GetLength $3B3
#cmd LnComponentList_GetItem $3B5
#cmd LnWorldObject_SetPosition $11A
#cmd LnWorldObject_SetPositionXYZ $11C
#cmd LnWorldObject_GetPosition $120
#cmd LnWorldObject_SetRotationQuaternion $121
#cmd LnWorldObject_SetRotation $123
#cmd LnWorldObject_GetRotation $127
#cmd LnWorldObject_SetScale $128
#cmd LnWorldObject_SetScaleS $12A
#cmd LnWorldObject_SetScaleXYZ $12C
#cmd LnWorldObject_GetScale $130
#cmd LnWorldObject_SetCenterPoint $131
#cmd LnWorldObject_SetCenterPointXYZ $133
#cmd LnWorldObject_GetCenterPoint $137
#cmd LnWorldObject_LookAt $138
#cmd LnWorldObject_LookAtXYZ $13A
#cmd LnWorldObject_AddComponent $13E
#cmd LnWorldObject_GetComponents $140
#cmd LnVisualObject_SetVisible $144
#cmd LnVisualObject_IsVisible $146
#cmd LnDirectionalLight_SetEnabled $149
#cmd LnDirectionalLight_IsEnabled $14B
#cmd LnDirectionalLight_SetColor $14C
#cmd LnDirectionalLight_GetColor $14E
#cmd LnDirectionalLight_SetIntensity $14F
#cmd LnDirectionalLight_GetIntensity $151
#cmd LnDirectionalLight_Create $152
#cmd LnDirectionalLight_CreateWithColor $153
#cmd LnPointLight_SetEnabled $156
#cmd LnPointLight_IsEnabled $158
#cmd LnPointLight_SetColor $159
#cmd LnPointLight_GetColor $15B
#cmd LnPointLight_SetIntensity $15C
#cmd LnPointLight_GetIntensity $15E
#cmd LnPointLight_SetRange $15F
#cmd LnPointLight_GetRange $161
#cmd LnPointLight_SetAttenuation $162
#cmd LnPointLight_GetAttenuation $164
#cmd LnPointLight_Create $165
#cmd LnPointLight_CreateWithColorAndRange $166
#cmd LnSpotLight_SetEnabled $16A
#cmd LnSpotLight_IsEnabled $16C
#cmd LnSpotLight_SetColor $16D
#cmd LnSpotLight_GetColor $16F
#cmd LnSpotLight_SetIntensity $170
#cmd LnSpotLight_GetIntensity $172
#cmd LnSpotLight_SetRange $173
#cmd LnSpotLight_GetRange $175
#cmd LnSpotLight_SetAttenuation $176
#cmd LnSpotLight_GetAttenuation $178
#cmd LnSpotLight_SetAngle $179
#cmd LnSpotLight_GetAngle $17B
#cmd LnSpotLight_SetPenumbra $17C
#cmd LnSpotLight_GetPenumbra $17E
#cmd LnSpotLight_Create $17F
#cmd LnSpotLight_CreateWithColorAndRange $180
#cmd LnTestDelegate_Create $426
#cmd LnSprite_SetTexture $188
#cmd LnSprite_SetSize $18A
#cmd LnSprite_SetSizeWH $18C
#cmd LnSprite_SetSourceRectXYWH $18F
#cmd LnSprite_SetCallerTest $194
#cmd LnSprite_Create $196
#cmd LnSprite_CreateWithTexture $197
#cmd LnSprite_CreateWithTextureAndSize $199
#cmd LnCameraOrbitControlComponent_Create $19E
#cmd LnRaycaster_FromScreen $1A0
#cmd LnRaycaster_IntersectPlane $1A2
#cmd LnRaycastResult_GetPoint $1A7
#cmd LnWorldRenderView_SetGuideGridEnabled $1A9
#cmd LnWorldRenderView_GetGuideGridEnabled $1AB
#cmd LnBoxMesh_Create $1AD
#cmd LnPlaneMesh_Create $1AF
#cmd LnUIEventArgs_Sender $1B1
#cmd LnUIGeneralEventHandler_Create $445
#cmd LnUIEventHandler_Create $44A
#cmd LnUIElement_SetMargin $1C3
#cmd LnUIElement_GetMargin $1C5
#cmd LnUIElement_SetPadding $1C6
#cmd LnUIElement_GetPadding $1C8
#cmd LnUIElement_SetHAlignment $1C9
#cmd LnUIElement_GetHAlignment $1CB
#cmd LnUIElement_SetVAlignment $1CC
#cmd LnUIElement_GetVAlignment $1CE
#cmd LnUIElement_SetAlignments $1CF
#cmd LnUIElement_SetPosition $1D2
#cmd LnUIElement_SetPositionXYZ $1D4
#cmd LnUIElement_GetPosition $1D8
#cmd LnUIElement_SetRotation $1D9
#cmd LnUIElement_SetEulerAngles $1DB
#cmd LnUIElement_GetRotation $1DF
#cmd LnUIElement_SetScale $1E0
#cmd LnUIElement_SetScaleS $1E2
#cmd LnUIElement_SetScaleXY $1E4
#cmd LnUIElement_GetScale $1E7
#cmd LnUIElement_SetCenterPoint $1E8
#cmd LnUIElement_SetCenterPointXYZ $1EA
#cmd LnUIElement_GetCenterPoint $1EE
#cmd LnUIElement_AddChild $1EF
#cmd LnUIButtonBase_SetTextA $1F3
#cmd LnUIButton_Create $1F6
#cmd LnUIButton_CreateWithTextA $1F7
#cmd LnUIButton_ConnectOnClicked $1F9
#cmd LnUITextBlock_Create $1FC
#cmd LnUITextBlock_CreateWithTextA $1FD
#cmd LnUISprite_SetTexture $200
#cmd LnUISprite_SetSourceRect $202
#cmd LnUISprite_SetSourceRectXYWH $204
#cmd LnUISprite_GetSourceRect $209
#cmd LnUISprite_Create $20A
#cmd LnUISprite_CreateWithTexture $20B
#cmd LnInput_PressedA $20E
#cmd LnInput_TriggeredA $210
#cmd LnInput_TriggeredOffA $212
#cmd LnInput_RepeatedA $214
#cmd LnInput_GetAxisValueA $216
#cmd LnInput_ClearAllBindings $218
#cmd LnMouse_Pressed $21A
#cmd LnMouse_Triggered $21C
#cmd LnMouse_TriggeredOff $21E
#cmd LnMouse_Repeated $220
#cmd LnMouse_Position $222
#cmd LnEngineSettings_SetMainWindowSize $224
#cmd LnEngineSettings_SetMainWorldViewSize $227
#cmd LnEngineSettings_SetMainWindowTitleA $22A
#cmd LnEngineSettings_AddAssetDirectoryA $22C
#cmd LnEngineSettings_AddAssetArchiveA $22E
#cmd LnEngineSettings_SetFrameRate $231
#cmd LnEngineSettings_SetDebugToolEnabled $233
#cmd LnEngineSettings_SetEngineLogEnabled $235
#cmd LnEngineSettings_SetEngineLogFilePathA $237
#cmd LnEngine_Initialize $23A
#cmd LnEngine_Finalize $23B
#cmd LnEngine_Update $23C
#cmd LnEngine_Time $23D
#cmd LnEngine_GetCamera $23E
#cmd LnEngine_GetLight $23F
#cmd LnEngine_GetRenderView $240
#cmd LnApplication_OnInit $242
#cmd LnApplication_OnUpdate $243
#cmd LnApplication_World $244
#cmd LnApplication_Create $245
#cmd LnDebug_PrintA $247
#cmd LnDebug_PrintWithTimeA $249
#cmd LnDebug_PrintWithTimeAndColorA $24C



#endif // __lumino__
