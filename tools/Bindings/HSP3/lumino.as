
#ifndef __lumino__
#define __lumino__

#ifdef LUMINO_DEBUG
    #regcmd "_hsp3cmdinit@4","LuminoHSP3d.dll", 11
#else
    #regcmd "_hsp3cmdinit@4","LuminoHSP3.dll", 11
#endif

#const global LN_TRUE 1
#const global LN_FALSE 0
#const global LN_NULL_HANDLE 0

#const global LN_ENCODING_TYPE_UNKNOWN 0
#const global LN_ENCODING_TYPE_ASCII 1
#const global LN_ENCODING_TYPE_UTF8 2
#const global LN_ENCODING_TYPE_UTF8N 3
#const global LN_ENCODING_TYPE_SJIS 4
#const global LN_KEYS_UNKNOWN 0
#const global LN_KEYS_A 1
#const global LN_KEYS_B 2
#const global LN_KEYS_C 3
#const global LN_KEYS_D 4
#const global LN_KEYS_E 5
#const global LN_KEYS_F 6
#const global LN_KEYS_G 7
#const global LN_KEYS_H 8
#const global LN_KEYS_I 9
#const global LN_KEYS_J 10
#const global LN_KEYS_K 11
#const global LN_KEYS_L 12
#const global LN_KEYS_M 13
#const global LN_KEYS_N 14
#const global LN_KEYS_O 15
#const global LN_KEYS_P 16
#const global LN_KEYS_Q 17
#const global LN_KEYS_R 18
#const global LN_KEYS_S 19
#const global LN_KEYS_T 20
#const global LN_KEYS_U 21
#const global LN_KEYS_V 22
#const global LN_KEYS_W 23
#const global LN_KEYS_X 24
#const global LN_KEYS_Y 25
#const global LN_KEYS_Z 26
#const global LN_KEYS_D0 27
#const global LN_KEYS_D1 28
#const global LN_KEYS_D2 29
#const global LN_KEYS_D3 30
#const global LN_KEYS_D4 31
#const global LN_KEYS_D5 32
#const global LN_KEYS_D6 33
#const global LN_KEYS_D7 34
#const global LN_KEYS_D8 35
#const global LN_KEYS_D9 36
#const global LN_KEYS_F1 37
#const global LN_KEYS_F2 38
#const global LN_KEYS_F3 39
#const global LN_KEYS_F4 40
#const global LN_KEYS_F5 41
#const global LN_KEYS_F6 42
#const global LN_KEYS_F7 43
#const global LN_KEYS_F8 44
#const global LN_KEYS_F9 45
#const global LN_KEYS_F10 46
#const global LN_KEYS_F11 47
#const global LN_KEYS_F12 48
#const global LN_KEYS_SPACE 49
#const global LN_KEYS_ESCAPE 50
#const global LN_KEYS_UP 51
#const global LN_KEYS_DOWN 52
#const global LN_KEYS_LEFT 53
#const global LN_KEYS_RIGHT 54
#const global LN_KEYS_LSHIFT 55
#const global LN_KEYS_RSHIFT 56
#const global LN_KEYS_LCTRL 57
#const global LN_KEYS_RCTRL 58
#const global LN_KEYS_LALT 59
#const global LN_KEYS_RALT 60
#const global LN_KEYS_TAB 61
#const global LN_KEYS_ENTER 62
#const global LN_KEYS_BACK_SPACE 63
#const global LN_KEYS_INSERT 64
#const global LN_KEYS_DELETE 65
#const global LN_KEYS_PAGE_UP 66
#const global LN_KEYS_PAGE_DOWN 67
#const global LN_KEYS_HOME 68
#const global LN_KEYS_END 69
#const global LN_KEYS_COLON 70
#const global LN_KEYS_SEMICOLON 71
#const global LN_KEYS_COMMA 72
#const global LN_KEYS_PERIOD 73
#const global LN_KEYS_SLASH 74
#const global LN_KEYS_MINUS 75
#const global LN_KEYS_BACKSLASH 76
#const global LN_KEYS_YEN 77
#const global LN_KEYS_CARET 78
#const global LN_KEYS_LBRACKET 79
#const global LN_KEYS_RBRACKET 80
#const global LN_KEYS_ANDROID_PLUS 81
#const global LN_KEYS_TERMINATOR 82
#const global LN_MOUSE_BUTTONS_NONE 0
#const global LN_MOUSE_BUTTONS_LEFT 1
#const global LN_MOUSE_BUTTONS_RIGHT 2
#const global LN_MOUSE_BUTTONS_MIDDLE 3
#const global LN_MOUSE_BUTTONS_X1 4
#const global LN_MOUSE_BUTTONS_X2 5
#const global LN_SOUND_FADE_BEHAVIOR_CONTINUE 0
#const global LN_SOUND_FADE_BEHAVIOR_STOP 1
#const global LN_SOUND_FADE_BEHAVIOR_STOP_RESET 2
#const global LN_SOUND_FADE_BEHAVIOR_PAUSE 3
#const global LN_SOUND_FADE_BEHAVIOR_PAUSE_RESET 4
#const global LN_GRAPHICS_API_DEFAULT 0
#const global LN_GRAPHICS_API_OPEN_GL 1
#const global LN_GRAPHICS_API_VULKAN 2
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
#const global LN_SHADING_MODEL_DEFAULT 0
#const global LN_SHADING_MODEL_UNLIT 1
#const global LN_BLEND_MODE_NORMAL 0
#const global LN_BLEND_MODE_ALPHA 1
#const global LN_BLEND_MODE_ADD 2
#const global LN_BLEND_MODE_SUBTRACT 3
#const global LN_BLEND_MODE_MULTIPLY 4
#const global LN_SCENE_CLEAR_MODE_NONE 0
#const global LN_SCENE_CLEAR_MODE_COLOR_AND_DEPTH 1
#const global LN_SCENE_CLEAR_MODE_SKY 2
#const global LN_SCENE_CLEAR_MODE_SKY0 3
#const global LN_SCENE_CLEAR_MODE_SKY_DOME 4
#const global LN_ANIMATION_WRAP_MODE_ONCE 0
#const global LN_ANIMATION_WRAP_MODE_LOOP 1
#const global LN_ANIMATION_WRAP_MODE_ALTERNATE 2
#const global LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT 0
#const global LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT_Y 1
#const global LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION 2
#const global LN_HIERARCHICAL_ANIMATION_MODE_DISABLE_TRANSLATION 3
#const global LN_TANGENT_MODE_LINEAR 0
#const global LN_TANGENT_MODE_TANGENT 1
#const global LN_TANGENT_MODE_AUTO 2
#const global LN_TANGENT_MODE_CONSTANT 3
#const global LN_PARTICLE_EMITTER_SHAPE_TYPE_SPHERE 0
#const global LN_PARTICLE_EMITTER_SHAPE_TYPE_CONE 1
#const global LN_PARTICLE_EMITTER_SHAPE_TYPE_BOX 2
#const global LN_PARTICLE_GEOMETRY_DIRECTION_TO_VIEW 0
#const global LN_PARTICLE_GEOMETRY_DIRECTION_TOP 1
#const global LN_PARTICLE_GEOMETRY_DIRECTION_VERTICAL_BILLBOARD 2
#const global LN_PARTICLE_GEOMETRY_DIRECTION_HORIZONTAL_BILLBOARD 3
#const global LN_LEVEL_TRANSITION_EFFECT_MODE_NONE 0
#const global LN_LEVEL_TRANSITION_EFFECT_MODE_FADE_IN_OUT 1
#const global LN_LEVEL_TRANSITION_EFFECT_MODE_CROSS_FADE 2
#const global LN_UILAYOUT_ORIENTATION_HORIZONTAL 0
#const global LN_UILAYOUT_ORIENTATION_VERTICAL 1
#const global LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL 2
#const global LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL 3
#const global LN_UIVISIBILITY_VISIBLE 0
#const global LN_UIVISIBILITY_HIDDEN 1
#const global LN_UIVISIBILITY_COLLAPSED 2
#const global LN_UIVALIGNMENT_TOP 0
#const global LN_UIVALIGNMENT_CENTER 1
#const global LN_UIVALIGNMENT_BOTTOM 2
#const global LN_UIVALIGNMENT_STRETCH 3
#const global LN_UIHALIGNMENT_LEFT 0
#const global LN_UIHALIGNMENT_CENTER 1
#const global LN_UIHALIGNMENT_RIGHT 2
#const global LN_UIHALIGNMENT_STRETCH 3
#const global LN_UIINLINE_PLACEMENT_TOP_LEFT 0
#const global LN_UIINLINE_PLACEMENT_TOP 1
#const global LN_UIINLINE_PLACEMENT_TOP_RIGHT 2
#const global LN_UIINLINE_PLACEMENT_LEFT 3
#const global LN_UIINLINE_PLACEMENT_CENTER 4
#const global LN_UIINLINE_PLACEMENT_RIGHT 5
#const global LN_UIINLINE_PLACEMENT_BOTTOM_LEFT 6
#const global LN_UIINLINE_PLACEMENT_BOTTOM 7
#const global LN_UIINLINE_PLACEMENT_BOTTOM_RIGHT 8
#const global LN_UILIST_SUBMIT_MODE_SINGLE 0
#const global LN_UILIST_SUBMIT_MODE_DOUBLE 1

#cmd LNVector3 $2B
#cmd LNVector3_GetX $560
#cmd LNVector3_SetX $562
#cmd LNVector3_GetY $563
#cmd LNVector3_SetY $565
#cmd LNVector3_GetZ $566
#cmd LNVector3_SetZ $568
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $57F
#cmd LNVector4_SetX $581
#cmd LNVector4_GetY $582
#cmd LNVector4_SetY $584
#cmd LNVector4_GetZ $585
#cmd LNVector4_SetZ $587
#cmd LNVector4_GetW $588
#cmd LNVector4_SetW $58A
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $59D
#cmd LNQuaternion_SetX $59F
#cmd LNQuaternion_GetY $5A0
#cmd LNQuaternion_SetY $5A2
#cmd LNQuaternion_GetZ $5A3
#cmd LNQuaternion_SetZ $5A5
#cmd LNQuaternion_GetW $5A6
#cmd LNQuaternion_SetW $5A8
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $5BC
#cmd LNMatrix_SetRow0 $5BE
#cmd LNMatrix_GetRow1 $5BF
#cmd LNMatrix_SetRow1 $5C1
#cmd LNMatrix_GetRow2 $5C2
#cmd LNMatrix_SetRow2 $5C4
#cmd LNMatrix_GetRow3 $5C5
#cmd LNMatrix_SetRow3 $5C7
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $179
#cmd LNColor_GetR $65B
#cmd LNColor_SetR $65D
#cmd LNColor_GetG $65E
#cmd LNColor_SetG $660
#cmd LNColor_GetB $661
#cmd LNColor_SetB $663
#cmd LNColor_GetA $664
#cmd LNColor_SetA $666
#cmd LNColor_SetZeros $17E
#cmd LNColor_Set $17F
#cmd LNColorTone $184
#cmd LNColorTone_GetR $67A
#cmd LNColorTone_SetR $67C
#cmd LNColorTone_GetG $67D
#cmd LNColorTone_SetG $67F
#cmd LNColorTone_GetB $680
#cmd LNColorTone_SetB $682
#cmd LNColorTone_GetS $683
#cmd LNColorTone_SetS $685
#cmd LNColorTone_SetZeros $189
#cmd LNColorTone_Set $18A
#cmd LNPoint $18F
#cmd LNPoint_GetX $698
#cmd LNPoint_SetX $69A
#cmd LNPoint_GetY $69B
#cmd LNPoint_SetY $69D
#cmd LNPoint_SetZeros $192
#cmd LNPoint_Set $193
#cmd LNSize $196
#cmd LNSize_GetWidth $6A8
#cmd LNSize_SetWidth $6AA
#cmd LNSize_GetHeight $6AB
#cmd LNSize_SetHeight $6AD
#cmd LNSize_SetZeros $199
#cmd LNSize_Set $19A
#cmd LNRect $19D
#cmd LNRect_GetX $6B8
#cmd LNRect_SetX $6BA
#cmd LNRect_GetY $6BB
#cmd LNRect_SetY $6BD
#cmd LNRect_GetWidth $6BE
#cmd LNRect_SetWidth $6C0
#cmd LNRect_GetHeight $6C1
#cmd LNRect_SetHeight $6C3
#cmd LNRect_SetZeros $1A2
#cmd LNRect_Set $1A3
#cmd LNRect_GetLeft $1A8
#cmd LNRect_SetSize $1A9
#cmd LNRect_GetSize $1AB
#cmd LNThickness $1AC
#cmd LNThickness_GetLeft $6DC
#cmd LNThickness_SetLeft $6DE
#cmd LNThickness_GetTop $6DF
#cmd LNThickness_SetTop $6E1
#cmd LNThickness_GetRight $6E2
#cmd LNThickness_SetRight $6E4
#cmd LNThickness_GetBottom $6E5
#cmd LNThickness_SetBottom $6E7
#cmd LNThickness_SetZeros $1B1
#cmd LNThickness_Set $1B2
#cmd LNCornerRadius $1B7
#cmd LNCornerRadius_GetTopleft $6FA
#cmd LNCornerRadius_SetTopleft $6FC
#cmd LNCornerRadius_GetTopright $6FD
#cmd LNCornerRadius_SetTopright $6FF
#cmd LNCornerRadius_GetBottomright $700
#cmd LNCornerRadius_SetBottomright $702
#cmd LNCornerRadius_GetBottomleft $703
#cmd LNCornerRadius_SetBottomleft $705
#cmd LNCornerRadius_SetZeros $1BC
#cmd LNCornerRadius_Set $1BD

#cmd LNObject_Release $7A
#cmd LNObject_Retain $7B
#cmd LNObject_GetReferenceCount $7C
#cmd LNObject_SetPrototype_OnSerialize $9B1
#cmd LNEventConnection_SetPrototype_OnSerialize $9BC
#cmd LNPromiseFailureDelegate_Create $5E2
#cmd LNVariant_Create $81
#cmd LNVariant_SetInt $82
#cmd LNVariant_GetInt $84
#cmd LNVariant_SetPrototype_OnSerialize $9C7
#cmd LNZVTestDelegate1_Create $5EB
#cmd LNZVTestDelegate2_Create $5F1
#cmd LNZVTestDelegate3_Create $5F6
#cmd LNZVTestEventHandler1_Create $5FB
#cmd LNZVTestEventHandler2_Create $600
#cmd LNZVTestPromise1_ThenWith $603
#cmd LNZVTestPromise1_CatchWith $605
#cmd LNZVTestPromise2_ThenWith $609
#cmd LNZVTestPromise2_CatchWith $60B
#cmd LNZVTestClass1_Create $AD
#cmd LNZVTestClass1_SetTestDelegate1 $97
#cmd LNZVTestClass1_SetTestDelegate2 $99
#cmd LNZVTestClass1_SetTestDelegate3 $9B
#cmd LNZVTestClass1_CallTestDelegate1 $9D
#cmd LNZVTestClass1_CallTestDelegate2 $9F
#cmd LNZVTestClass1_CallTestDelegate3 $A2
#cmd LNZVTestClass1_LoadAsync $A3
#cmd LNZVTestClass1_ExecuteAsync $A5
#cmd LNZVTestClass1_GetFilePath $A6
#cmd LNZVTestClass1_ConnectOnEvent1 $A7
#cmd LNZVTestClass1_RaiseEvent1 $A9
#cmd LNZVTestClass1_ConnectOnEvent2 $AA
#cmd LNZVTestClass1_RaiseEvent2 $AC
#cmd LNZVTestClass1_SetPrototype_OnSerialize $9D2
#cmd LNZVTestEventArgs1_Create $B0
#cmd LNZVTestEventArgs1_CreateWithValue $B1
#cmd LNZVTestEventArgs1_GetValue $AF
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $9DD
#cmd LNSerializer2_SetPrototype_OnSerialize $9E8
#cmd LNAssetObject_SetPrototype_OnSerialize $9F3
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $9FE
#cmd LNAssetModel_Create $113
#cmd LNAssetModel_Target $112
#cmd LNAssetModel_SetPrototype_OnSerialize $A09
#cmd LNAssets_SaveAssetToLocalFile $116
#cmd LNAssets_LoadAssetFromLocalFile $119
#cmd LNAssets_LoadAsset $11B
#cmd LNAssets_ReloadAsset $11D
#cmd LNAssets_ReadAllText $120
#cmd LNSound_SetVolume $12A
#cmd LNSound_GetVolume $12C
#cmd LNSound_SetPitch $12D
#cmd LNSound_GetPitch $12F
#cmd LNSound_SetLoopEnabled $130
#cmd LNSound_IsLoopEnabled $132
#cmd LNSound_SetLoopRange $133
#cmd LNSound_Play $136
#cmd LNSound_Stop $137
#cmd LNSound_Pause $138
#cmd LNSound_Resume $139
#cmd LNSound_FadeVolume $13A
#cmd LNSound_SetPrototype_OnSerialize $A14
#cmd LNAudio_PlayBGM $13F
#cmd LNAudio_StopBGM $144
#cmd LNAudio_PlayBGS $146
#cmd LNAudio_StopBGS $14B
#cmd LNAudio_PlayME $14D
#cmd LNAudio_StopME $151
#cmd LNAudio_PlaySE $152
#cmd LNAudio_PlaySE3D $156
#cmd LNAudio_StopSE $15C
#cmd LNTexture2DDelegate_Create $651
#cmd LNTexture2DPromise_ThenWith $654
#cmd LNTexture2DPromise_CatchWith $656
#cmd LNGraphics_GetActiveGraphicsAPI $178
#cmd LNTexture_SetPrototype_OnSerialize $A1F
#cmd LNTexture2D_Create $1C8
#cmd LNTexture2D_CreateWithFormat $1CB
#cmd LNTexture2D_Load $1C4
#cmd LNTexture2D_LoadEmoji $1C6
#cmd LNTexture2D_SetPrototype_OnSerialize $A2A
#cmd LNShader_Load $1D0
#cmd LNShader_SetFloat $1D3
#cmd LNShader_SetVector3 $1D6
#cmd LNShader_SetVector4 $1D9
#cmd LNShader_SetTexture $1DC
#cmd LNShader_SetPrototype_OnSerialize $A35
#cmd LNRenderView_SetPrototype_OnSerialize $A40
#cmd LNMaterial_Create $201
#cmd LNMaterial_SetMainTexture $1F0
#cmd LNMaterial_GetMainTexture $1F2
#cmd LNMaterial_SetColor $1F3
#cmd LNMaterial_SetRoughness $1F5
#cmd LNMaterial_SetMetallic $1F7
#cmd LNMaterial_SetEmissive $1F9
#cmd LNMaterial_SetShadingModel $1FB
#cmd LNMaterial_GetShadingModel $1FD
#cmd LNMaterial_SetShader $1FE
#cmd LNMaterial_GetShader $200
#cmd LNMaterial_SetPrototype_OnSerialize $A4B
#cmd LNMeshNode_SetVisible $203
#cmd LNMeshNode_IsVisible $205
#cmd LNMeshNode_SetPrototype_OnSerialize $A56
#cmd LNAnimationController_AddClip $207
#cmd LNAnimationController_Play $209
#cmd LNAnimationController_SetPrototype_OnSerialize $A61
#cmd LNMeshModel_Load $20D
#cmd LNMeshModel_FindNode $210
#cmd LNMeshModel_FindMaterial $212
#cmd LNMeshModel_MaterialCount $214
#cmd LNMeshModel_Material $215
#cmd LNMeshModel_GetAnimationController $217
#cmd LNMeshModel_SetPrototype_OnSerialize $A6C
#cmd LNMeshImportSettings_Create $219
#cmd LNMeshImportSettings_SetPrototype_OnSerialize $A77
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $A82
#cmd LNCollisionShape_SetPrototype_OnSerialize $A8D
#cmd LNBoxCollisionShape_Create $21D
#cmd LNBoxCollisionShape_CreateWHD $21F
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $A98
#cmd LNAnimationCurve_Evaluate $22D
#cmd LNAnimationCurve_SetPrototype_OnSerialize $AA3
#cmd LNKeyFrameAnimationCurve_Create $23A
#cmd LNKeyFrameAnimationCurve_AddKeyFrame $235
#cmd LNKeyFrameAnimationCurve_SetPrototype_OnSerialize $AAE
#cmd LNAnimationClip_Load $23C
#cmd LNAnimationClip_SetWrapMode $23E
#cmd LNAnimationClip_GetWrapMode $240
#cmd LNAnimationClip_SetHierarchicalAnimationMode $241
#cmd LNAnimationClip_GetHierarchicalAnimationMode $243
#cmd LNAnimationClip_SetPrototype_OnSerialize $AB9
#cmd LNAnimationState_SetPrototype_OnSerialize $AC4
#cmd LNEffectResource_SetPrototype_OnSerialize $ACF
#cmd LNParticleEmitterModel_Create $268
#cmd LNParticleEmitterModel_SetMaxParticles $250
#cmd LNParticleEmitterModel_SetSpawnRate $252
#cmd LNParticleEmitterModel_SetLifeTime $254
#cmd LNParticleEmitterModel_SetupBoxShape $256
#cmd LNParticleEmitterModel_SetSize $258
#cmd LNParticleEmitterModel_SetSizeVelocity $25A
#cmd LNParticleEmitterModel_SetSizeAcceleration $25C
#cmd LNParticleEmitterModel_SetForwardVelocityMin $25E
#cmd LNParticleEmitterModel_SetForwardVelocityMax $260
#cmd LNParticleEmitterModel_SetForwardScale $262
#cmd LNParticleEmitterModel_SetGeometryDirection $264
#cmd LNParticleEmitterModel_SetupSpriteModule $266
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $ADA
#cmd LNParticleModel_Create $26F
#cmd LNParticleModel_SetLoop $26A
#cmd LNParticleModel_IsLoop $26C
#cmd LNParticleModel_AddEmitter $26D
#cmd LNParticleModel_SetPrototype_OnSerialize $AE5
#cmd LNComponent_SetPrototype_OnSerialize $AF0
#cmd LNVisualComponent_SetVisible $272
#cmd LNVisualComponent_IsVisible $274
#cmd LNVisualComponent_SetPrototype_OnSerialize $AFB
#cmd LNSpriteComponent_SetTexture $276
#cmd LNSpriteComponent_SetPrototype_OnSerialize $B06
#cmd LNCollisionEventHandler_Create $788
#cmd LNCharacterController_Create $296
#cmd LNCharacterController_SetWalkVelocity $280
#cmd LNCharacterController_GetWalkVelocity $282
#cmd LNCharacterController_SetVelocity $283
#cmd LNCharacterController_GetVelocity $285
#cmd LNCharacterController_SetInputControlEnabled $286
#cmd LNCharacterController_SetCameraControlEnabled $288
#cmd LNCharacterController_SetHeight $28A
#cmd LNCharacterController_GetHeight $28C
#cmd LNCharacterController_SetCameraRadius $28D
#cmd LNCharacterController_GetCameraRadius $28F
#cmd LNCharacterController_SetCollisionEnter $290
#cmd LNCharacterController_SetCollisionLeave $292
#cmd LNCharacterController_SetCollisionStay $294
#cmd LNCharacterController_SetPrototype_OnSerialize $B11
#cmd LNWorld_Add $298
#cmd LNWorld_SetPrototype_OnSerialize $B1C
#cmd LNComponentList_GetLength $7A8
#cmd LNComponentList_GetItem $7AA
#cmd LNComponentList_SetPrototype_OnSerialize $B27
#cmd LNWorldObject_Create $2CF
#cmd LNWorldObject_SetPosition $29C
#cmd LNWorldObject_SetPositionXYZ $29E
#cmd LNWorldObject_GetPosition $2A2
#cmd LNWorldObject_SetRotationQuaternion $2A3
#cmd LNWorldObject_SetRotation $2A5
#cmd LNWorldObject_GetRotation $2A9
#cmd LNWorldObject_SetScale $2AA
#cmd LNWorldObject_SetScaleS $2AC
#cmd LNWorldObject_SetScaleXYZ $2AE
#cmd LNWorldObject_GetScale $2B2
#cmd LNWorldObject_SetCenterPoint $2B3
#cmd LNWorldObject_SetCenterPointXYZ $2B5
#cmd LNWorldObject_GetCenterPoint $2B9
#cmd LNWorldObject_LookAt $2BA
#cmd LNWorldObject_LookAtXYZ $2BC
#cmd LNWorldObject_AddComponent $2C0
#cmd LNWorldObject_RemoveComponent $2C2
#cmd LNWorldObject_AddTag $2C4
#cmd LNWorldObject_RemoveTag $2C6
#cmd LNWorldObject_HasTag $2C8
#cmd LNWorldObject_Destroy $2CA
#cmd LNWorldObject_GetComponents $2CB
#cmd LNWorldObject_SetPrototype_OnSerialize $B32
#cmd LNWorldObject_SetPrototype_OnPreUpdate $B3D
#cmd LNWorldObject_SetPrototype_OnUpdate $B48
#cmd LNVisualObject_SetVisible $2D1
#cmd LNVisualObject_IsVisible $2D3
#cmd LNVisualObject_SetBlendMode2 $2D4
#cmd LNVisualObject_SetOpacity $2D6
#cmd LNVisualObject_GetOpacity $2D8
#cmd LNVisualObject_SetPrototype_OnSerialize $B53
#cmd LNVisualObject_SetPrototype_OnPreUpdate $B5E
#cmd LNVisualObject_SetPrototype_OnUpdate $B69
#cmd LNCamera_SetPrototype_OnSerialize $B74
#cmd LNCamera_SetPrototype_OnPreUpdate $B7F
#cmd LNCamera_SetPrototype_OnUpdate $B8A
#cmd LNEnvironmentLight_SetEnabled $2DB
#cmd LNEnvironmentLight_IsEnabled $2DD
#cmd LNEnvironmentLight_SetColor $2DE
#cmd LNEnvironmentLight_GetColor $2E0
#cmd LNEnvironmentLight_SetAmbientColor $2E1
#cmd LNEnvironmentLight_GetAmbientColor $2E3
#cmd LNEnvironmentLight_GetSkyColor $2E4
#cmd LNEnvironmentLight_SetSkyColor $2E5
#cmd LNEnvironmentLight_GetGroundColor $2E7
#cmd LNEnvironmentLight_SetGroundColor $2E8
#cmd LNEnvironmentLight_SetIntensity $2EA
#cmd LNEnvironmentLight_GetIntensity $2EC
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $2ED
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $2EF
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $2F0
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $2F2
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $B95
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $BA0
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $BAB
#cmd LNDirectionalLight_Create $303
#cmd LNDirectionalLight_CreateWithColor $304
#cmd LNDirectionalLight_SetEnabled $2F4
#cmd LNDirectionalLight_IsEnabled $2F6
#cmd LNDirectionalLight_SetColor $2F7
#cmd LNDirectionalLight_GetColor $2F9
#cmd LNDirectionalLight_SetIntensity $2FA
#cmd LNDirectionalLight_GetIntensity $2FC
#cmd LNDirectionalLight_SetShadowEffectiveDistance $2FD
#cmd LNDirectionalLight_GetShadowEffectiveDistance $2FF
#cmd LNDirectionalLight_SetShadowEffectiveDepth $300
#cmd LNDirectionalLight_GetShadowEffectiveDepth $302
#cmd LNDirectionalLight_SetPrototype_OnSerialize $BB6
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $BC1
#cmd LNDirectionalLight_SetPrototype_OnUpdate $BCC
#cmd LNPointLight_Create $316
#cmd LNPointLight_CreateWithColorAndRange $317
#cmd LNPointLight_SetEnabled $307
#cmd LNPointLight_IsEnabled $309
#cmd LNPointLight_SetColor $30A
#cmd LNPointLight_GetColor $30C
#cmd LNPointLight_SetIntensity $30D
#cmd LNPointLight_GetIntensity $30F
#cmd LNPointLight_SetRange $310
#cmd LNPointLight_GetRange $312
#cmd LNPointLight_SetAttenuation $313
#cmd LNPointLight_GetAttenuation $315
#cmd LNPointLight_SetPrototype_OnSerialize $BD7
#cmd LNPointLight_SetPrototype_OnPreUpdate $BE2
#cmd LNPointLight_SetPrototype_OnUpdate $BED
#cmd LNSpotLight_Create $330
#cmd LNSpotLight_CreateWithColorAndRange $331
#cmd LNSpotLight_SetEnabled $31B
#cmd LNSpotLight_IsEnabled $31D
#cmd LNSpotLight_SetColor $31E
#cmd LNSpotLight_GetColor $320
#cmd LNSpotLight_SetIntensity $321
#cmd LNSpotLight_GetIntensity $323
#cmd LNSpotLight_SetRange $324
#cmd LNSpotLight_GetRange $326
#cmd LNSpotLight_SetAttenuation $327
#cmd LNSpotLight_GetAttenuation $329
#cmd LNSpotLight_SetAngle $32A
#cmd LNSpotLight_GetAngle $32C
#cmd LNSpotLight_SetPenumbra $32D
#cmd LNSpotLight_GetPenumbra $32F
#cmd LNSpotLight_SetPrototype_OnSerialize $BF8
#cmd LNSpotLight_SetPrototype_OnPreUpdate $C03
#cmd LNSpotLight_SetPrototype_OnUpdate $C0E
#cmd LNTestDelegate_Create $856
#cmd LNSprite_Create $347
#cmd LNSprite_CreateWithTexture $348
#cmd LNSprite_CreateWithTextureAndSize $34A
#cmd LNSprite_SetTexture $339
#cmd LNSprite_SetSize $33B
#cmd LNSprite_SetSizeWH $33D
#cmd LNSprite_SetSourceRectXYWH $340
#cmd LNSprite_SetCallerTest $345
#cmd LNSprite_SetPrototype_OnSerialize $C19
#cmd LNSprite_SetPrototype_OnPreUpdate $C24
#cmd LNSprite_SetPrototype_OnUpdate $C2F
#cmd LNCameraOrbitControlComponent_Create $34F
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $C3A
#cmd LNRaycaster_FromScreen $351
#cmd LNRaycaster_IntersectPlane $353
#cmd LNRaycaster_SetPrototype_OnSerialize $C45
#cmd LNRaycastResult_GetPoint $358
#cmd LNRaycastResult_SetPrototype_OnSerialize $C50
#cmd LNWorldRenderView_SetGuideGridEnabled $35A
#cmd LNWorldRenderView_GetGuideGridEnabled $35C
#cmd LNWorldRenderView_SetPrototype_OnSerialize $C5B
#cmd LNBoxMesh_Create $35E
#cmd LNBoxMesh_CreateXYZ $35F
#cmd LNBoxMesh_SetPrototype_OnSerialize $C66
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $C71
#cmd LNBoxMesh_SetPrototype_OnUpdate $C7C
#cmd LNPlaneMesh_Create $364
#cmd LNPlaneMesh_SetPrototype_OnSerialize $C87
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $C92
#cmd LNPlaneMesh_SetPrototype_OnUpdate $C9D
#cmd LNStaticMesh_Load $366
#cmd LNStaticMesh_GetModel $368
#cmd LNStaticMesh_MakeCollisionBody $369
#cmd LNStaticMesh_SetPrototype_OnSerialize $CA8
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $CB3
#cmd LNStaticMesh_SetPrototype_OnUpdate $CBE
#cmd LNStaticMeshComponent_Create $370
#cmd LNStaticMeshComponent_SetModel $36C
#cmd LNStaticMeshComponent_MakeCollisionBody $36E
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $CC9
#cmd LNSkinnedMeshComponent_Create $372
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $CD4
#cmd LNCollision_GetWorldObject $374
#cmd LNCollision_SetPrototype_OnSerialize $CDF
#cmd LNTriggerBodyComponent_Create $378
#cmd LNTriggerBodyComponent_AddCollisionShape $376
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $CEA
#cmd LNParticleEmitter_Create $37A
#cmd LNParticleEmitter_SetPrototype_OnSerialize $CF5
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $D00
#cmd LNParticleEmitter_SetPrototype_OnUpdate $D0B
#cmd LNScene_SetClearMode $37D
#cmd LNScene_SetSkyColor $37F
#cmd LNScene_SetSkyHorizonColor $381
#cmd LNScene_SetSkyCloudColor $383
#cmd LNScene_SetSkyOverlayColor $385
#cmd LNScene_GotoLevel $387
#cmd LNScene_CallLevel $38A
#cmd LNScene_ReturnLevel $38D
#cmd LNScene_ActiveLevel $38F
#cmd LNScene_IsTransitionEffectRunning $390
#cmd LNScene_SetTransitionEffectMode $391
#cmd LNScene_TransitionEffectMode $393
#cmd LNScene_SetTransitionDuration $394
#cmd LNScene_TransitionDuration $396
#cmd LNScene_SetTransitionEffectColor $397
#cmd LNScene_TransitionEffectColor $399
#cmd LNScene_SetTransitionEffectMaskTexture $39A
#cmd LNScene_TransitionEffectMaskTexture $39C
#cmd LNScene_SetTransitionEffectVague $39D
#cmd LNScene_TransitionEffectVague $39F
#cmd LNScene_StartFadeOut $3A0
#cmd LNScene_StartFadeIn $3A1
#cmd LNScene_SetFogStartDistance $3A2
#cmd LNScene_SetFogColor $3A4
#cmd LNScene_SetFogDensity $3A6
#cmd LNScene_SetFogHeightDensity $3A8
#cmd LNScene_SetFogLowerHeight $3AA
#cmd LNScene_SetFogUpperHeight $3AC
#cmd LNScene_SetHDREnabled $3AE
#cmd LNScene_IsHDREnabled $3B0
#cmd LNScene_SetScreenBlendColor $3B1
#cmd LNScene_ScreenBlendColor $3B3
#cmd LNScene_SetColorTone $3B4
#cmd LNScene_ColorTone $3B6
#cmd LNScene_SetAntialiasEnabled $3B7
#cmd LNScene_IsAntialiasEnabled $3B9
#cmd LNScene_SetSSREnabled $3BA
#cmd LNScene_IsSSREnabled $3BC
#cmd LNScene_SetSSAOEnabled $3BD
#cmd LNScene_IsSSAOEnabled $3BF
#cmd LNScene_SetBloomEnabled $3C0
#cmd LNScene_IsBloomEnabled $3C2
#cmd LNScene_SetDOFEnabled $3C3
#cmd LNScene_IsDOFEnabled $3C5
#cmd LNScene_SetTonemapEnabled $3C6
#cmd LNScene_IsTonemapEnabled $3C8
#cmd LNScene_SetVignetteEnabled $3C9
#cmd LNScene_IsVignetteEnabled $3CB
#cmd LNScene_SetGammaEnabled $3CC
#cmd LNScene_IsGammaEnabled $3CE
#cmd LNScene_SetTonemapExposure $3CF
#cmd LNScene_SetTonemapLinearWhite $3D1
#cmd LNScene_SetTonemapShoulderStrength $3D3
#cmd LNScene_SetTonemapLinearStrength $3D5
#cmd LNScene_SetTonemapLinearAngle $3D7
#cmd LNScene_SetTonemapToeStrength $3D9
#cmd LNScene_SetTonemapToeNumerator $3DB
#cmd LNScene_SetTonemapToeDenominator $3DD
#cmd LNLevel_Create $3EE
#cmd LNLevel_AddObject $3E0
#cmd LNLevel_RemoveObject $3E2
#cmd LNLevel_AddSubLevel $3E4
#cmd LNLevel_RemoveSubLevel $3E6
#cmd LNLevel_RemoveAllSubLevels $3E8
#cmd LNLevel_SetPrototype_OnSerialize $D16
#cmd LNLevel_SetPrototype_OnStart $D21
#cmd LNLevel_SetPrototype_OnStop $D2C
#cmd LNLevel_SetPrototype_OnPause $D37
#cmd LNLevel_SetPrototype_OnResume $D42
#cmd LNLevel_SetPrototype_OnUpdate $D4D
#cmd LNUIEventArgs_Sender $3F9
#cmd LNUIEventArgs_SetPrototype_OnSerialize $D58
#cmd LNUIGeneralEventHandler_Create $8B5
#cmd LNUIEventHandler_Create $8BA
#cmd LNUILayoutElement_SetPrototype_OnSerialize $D63
#cmd LNUIElement_SetSize $40B
#cmd LNUIElement_SetSizeWH $40D
#cmd LNUIElement_SetWidth $410
#cmd LNUIElement_GetWidth $412
#cmd LNUIElement_SetHeight $413
#cmd LNUIElement_GetHeight $415
#cmd LNUIElement_SetMargin $416
#cmd LNUIElement_GetMargin $418
#cmd LNUIElement_SetPadding $419
#cmd LNUIElement_GetPadding $41B
#cmd LNUIElement_SetHAlignment $41C
#cmd LNUIElement_GetHAlignment $41E
#cmd LNUIElement_SetVAlignment $41F
#cmd LNUIElement_GetVAlignment $421
#cmd LNUIElement_SetAlignments $422
#cmd LNUIElement_SetPosition $425
#cmd LNUIElement_SetPositionXYZ $427
#cmd LNUIElement_GetPosition $42B
#cmd LNUIElement_SetRotation $42C
#cmd LNUIElement_SetEulerAngles $42E
#cmd LNUIElement_GetRotation $432
#cmd LNUIElement_SetScale $433
#cmd LNUIElement_SetScaleS $435
#cmd LNUIElement_SetScaleXY $437
#cmd LNUIElement_GetScale $43A
#cmd LNUIElement_SetCenterPoint $43B
#cmd LNUIElement_SetCenterPointXYZ $43D
#cmd LNUIElement_GetCenterPoint $441
#cmd LNUIElement_SetEnabled $442
#cmd LNUIElement_IsEnabled $444
#cmd LNUIElement_SetData $445
#cmd LNUIElement_GetData $447
#cmd LNUIElement_SetBackgroundColor $448
#cmd LNUIElement_GetBackgroundColor $44A
#cmd LNUIElement_SetBorderThickness $44B
#cmd LNUIElement_GetBorderThickness $44D
#cmd LNUIElement_SetBorderColor $44E
#cmd LNUIElement_GetBorderColor $450
#cmd LNUIElement_SetCornerRadius $451
#cmd LNUIElement_GetCornerRadius $453
#cmd LNUIElement_SetVisibility $454
#cmd LNUIElement_GetVisibility $456
#cmd LNUIElement_SetOpacity $457
#cmd LNUIElement_GetOpacity $459
#cmd LNUIElement_AddChild $45A
#cmd LNUIElement_SetFocusable $45C
#cmd LNUIElement_GetFocusable $45E
#cmd LNUIElement_SetPrototype_OnSerialize $D6E
#cmd LNUITextBlock_Create $463
#cmd LNUITextBlock_CreateWithText $464
#cmd LNUITextBlock_SetText $460
#cmd LNUITextBlock_GetText $462
#cmd LNUITextBlock_SetPrototype_OnSerialize $D79
#cmd LNUISprite_Create $473
#cmd LNUISprite_CreateWithTexture $474
#cmd LNUISprite_SetTexture $467
#cmd LNUISprite_SetSourceRect $469
#cmd LNUISprite_SetSourceRectXYWH $46B
#cmd LNUISprite_GetSourceRect $470
#cmd LNUISprite_SetShader $471
#cmd LNUISprite_SetPrototype_OnSerialize $D84
#cmd LNUIIcon_LoadFontIcon $477
#cmd LNUIIcon_LoadFontIconWithNameSize $479
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $47C
#cmd LNUIIcon_SetPrototype_OnSerialize $D8F
#cmd LNUIMessageTextArea_Create $485
#cmd LNUIMessageTextArea_SetText $481
#cmd LNUIMessageTextArea_SetTypingSpeed $483
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $D9A
#cmd LNUI_Add $487
#cmd LNUI_Remove $489
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $DA5
#cmd LNUIBoxLayout_Create $490
#cmd LNUIBoxLayout_SetOrientation $48D
#cmd LNUIBoxLayout_GetOrientation $48F
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $DB0
#cmd LNUIStackLayout_Create $495
#cmd LNUIStackLayout_SetOrientation $492
#cmd LNUIStackLayout_GetOrientation $494
#cmd LNUIStackLayout_SetPrototype_OnSerialize $DBB
#cmd LNUIGridLayout_Create $4A5
#cmd LNUIGridLayout_SetColumnCount $497
#cmd LNUIGridLayout_SetRow $499
#cmd LNUIGridLayout_SetColumn $49C
#cmd LNUIGridLayout_SetPlacement $49F
#cmd LNUIGridLayout_SetPrototype_OnSerialize $DC6
#cmd LNUIControl_Create $4B1
#cmd LNUIControl_AddInlineVisual $4B2
#cmd LNUIControl_SetPrototype_OnSerialize $DD1
#cmd LNUIButtonBase_SetText $4B6
#cmd LNUIButtonBase_SetPrototype_OnSerialize $DDC
#cmd LNUIButton_Create $4B9
#cmd LNUIButton_CreateWithText $4BA
#cmd LNUIButton_ConnectOnClicked $4BC
#cmd LNUIButton_SetPrototype_OnSerialize $DE7
#cmd LNUIWindow_Create $4BF
#cmd LNUIWindow_SetPrototype_OnSerialize $DF2
#cmd LNUIListItem_ConnectOnSubmit $4C4
#cmd LNUIListItem_SetPrototype_OnSerialize $DFD
#cmd LNUIListItemsControl_SetItemsLayoutPanel $4C7
#cmd LNUIListItemsControl_SetSubmitMode $4C9
#cmd LNUIListItemsControl_GetSubmitMode $4CB
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $E08
#cmd LNUIListBoxItem_Create $4CD
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $E13
#cmd LNUIListBox_Create $4D2
#cmd LNUIListBox_AddItem $4D0
#cmd LNUIListBox_SetPrototype_OnSerialize $E1E
#cmd LNInputGesture_SetPrototype_OnSerialize $E29
#cmd LNKeyGesture_Create $4D5
#cmd LNKeyGesture_SetPrototype_OnSerialize $E34
#cmd LNInput_IsPressed $4D8
#cmd LNInput_IsTriggered $4DA
#cmd LNInput_IsTriggeredOff $4DC
#cmd LNInput_IsRepeated $4DE
#cmd LNInput_GetAxisValue $4E0
#cmd LNInput_AddBinding $4E2
#cmd LNInput_RemoveBinding $4E5
#cmd LNInput_ClearBindings $4E7
#cmd LNInput_ClearAllBindings $4E9
#cmd LNMouse_Pressed $4EB
#cmd LNMouse_Triggered $4ED
#cmd LNMouse_TriggeredOff $4EF
#cmd LNMouse_Repeated $4F1
#cmd LNMouse_Position $4F3
#cmd LNInterpreterCommand_Code $4F5
#cmd LNInterpreterCommand_ParamsCount $4F6
#cmd LNInterpreterCommand_Param $4F7
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $E3F
#cmd LNInterpreterCommandList_Create $50E
#cmd LNInterpreterCommandList_AddCommand $4FA
#cmd LNInterpreterCommandList_AddCommand1 $4FC
#cmd LNInterpreterCommandList_AddCommand2 $4FF
#cmd LNInterpreterCommandList_AddCommand3 $503
#cmd LNInterpreterCommandList_AddCommand4 $508
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $E4A
#cmd LNInterpreterCommandDelegate_Create $980
#cmd LNInterpreter_Create $522
#cmd LNInterpreter_Clear $513
#cmd LNInterpreter_Run $514
#cmd LNInterpreter_IsRunning $516
#cmd LNInterpreter_Update $517
#cmd LNInterpreter_Terminate $518
#cmd LNInterpreter_RegisterCommandHandler $519
#cmd LNInterpreter_SetWaitMode $51C
#cmd LNInterpreter_GetWaitMode $51E
#cmd LNInterpreter_SetWaitCount $51F
#cmd LNInterpreter_GetWaitCount $521
#cmd LNInterpreter_SetPrototype_OnSerialize $E55
#cmd LNInterpreter_SetPrototype_OnUpdateWait $E61
#cmd LNEngineSettings_SetMainWindowSize $525
#cmd LNEngineSettings_SetMainWorldViewSize $528
#cmd LNEngineSettings_SetMainWindowTitle $52B
#cmd LNEngineSettings_SetMainWindowResizable $52D
#cmd LNEngineSettings_AddAssetDirectory $52F
#cmd LNEngineSettings_AddAssetArchive $531
#cmd LNEngineSettings_SetFrameRate $534
#cmd LNEngineSettings_SetUITheme $536
#cmd LNEngineSettings_SetFontFile $538
#cmd LNEngineSettings_SetDebugToolEnabled $53A
#cmd LNEngineSettings_SetEngineLogEnabled $53C
#cmd LNEngineSettings_SetEngineLogFilePath $53E
#cmd LNEngineSettings_SetDeveloperToolEnabled $540
#cmd LNEngine_Initialize $543
#cmd LNEngine_Finalize $544
#cmd LNEngine_Update $545
#cmd LNEngine_Run $546
#cmd LNEngine_GetTime $548
#cmd LNEngine_GetWorld $549
#cmd LNEngine_GetCamera $54A
#cmd LNEngine_GetMainLight $54B
#cmd LNEngine_GetRenderView $54C
#cmd LNApplication_Create $551
#cmd LNApplication_OnInit $54E
#cmd LNApplication_OnUpdate $54F
#cmd LNApplication_World $550
#cmd LNApplication_SetPrototype_OnSerialize $E6C
#cmd LNApplication_SetPrototype_OnInit $E77
#cmd LNApplication_SetPrototype_OnUpdate $E82
#cmd LNDebug_SetGuideGridEnabled $553
#cmd LNDebug_SetPhysicsDebugDrawEnabled $555
#cmd LNDebug_Print $557
#cmd LNDebug_PrintWithTime $559
#cmd LNDebug_PrintWithTimeAndColor $55C
#cmd LNObjectSerializeHandler_Create $9AF
#cmd LNEventConnectionSerializeHandler_Create $9BA
#cmd LNVariantSerializeHandler_Create $9C5
#cmd LNZVTestClass1SerializeHandler_Create $9D0
#cmd LNZVTestEventArgs1SerializeHandler_Create $9DB
#cmd LNSerializer2SerializeHandler_Create $9E6
#cmd LNAssetObjectSerializeHandler_Create $9F1
#cmd LNAssetImportSettingsSerializeHandler_Create $9FC
#cmd LNAssetModelSerializeHandler_Create $A07
#cmd LNSoundSerializeHandler_Create $A12
#cmd LNTextureSerializeHandler_Create $A1D
#cmd LNTexture2DSerializeHandler_Create $A28
#cmd LNShaderSerializeHandler_Create $A33
#cmd LNRenderViewSerializeHandler_Create $A3E
#cmd LNMaterialSerializeHandler_Create $A49
#cmd LNMeshNodeSerializeHandler_Create $A54
#cmd LNAnimationControllerSerializeHandler_Create $A5F
#cmd LNMeshModelSerializeHandler_Create $A6A
#cmd LNMeshImportSettingsSerializeHandler_Create $A75
#cmd LNSkinnedMeshModelSerializeHandler_Create $A80
#cmd LNCollisionShapeSerializeHandler_Create $A8B
#cmd LNBoxCollisionShapeSerializeHandler_Create $A96
#cmd LNAnimationCurveSerializeHandler_Create $AA1
#cmd LNKeyFrameAnimationCurveSerializeHandler_Create $AAC
#cmd LNAnimationClipSerializeHandler_Create $AB7
#cmd LNAnimationStateSerializeHandler_Create $AC2
#cmd LNEffectResourceSerializeHandler_Create $ACD
#cmd LNParticleEmitterModelSerializeHandler_Create $AD8
#cmd LNParticleModelSerializeHandler_Create $AE3
#cmd LNComponentSerializeHandler_Create $AEE
#cmd LNVisualComponentSerializeHandler_Create $AF9
#cmd LNSpriteComponentSerializeHandler_Create $B04
#cmd LNCharacterControllerSerializeHandler_Create $B0F
#cmd LNWorldSerializeHandler_Create $B1A
#cmd LNComponentListSerializeHandler_Create $B25
#cmd LNWorldObjectSerializeHandler_Create $B30
#cmd LNWorldObjectPreUpdateHandler_Create $B3B
#cmd LNWorldObjectUpdateHandler_Create $B46
#cmd LNVisualObjectSerializeHandler_Create $B51
#cmd LNVisualObjectPreUpdateHandler_Create $B5C
#cmd LNVisualObjectUpdateHandler_Create $B67
#cmd LNCameraSerializeHandler_Create $B72
#cmd LNCameraPreUpdateHandler_Create $B7D
#cmd LNCameraUpdateHandler_Create $B88
#cmd LNEnvironmentLightSerializeHandler_Create $B93
#cmd LNEnvironmentLightPreUpdateHandler_Create $B9E
#cmd LNEnvironmentLightUpdateHandler_Create $BA9
#cmd LNDirectionalLightSerializeHandler_Create $BB4
#cmd LNDirectionalLightPreUpdateHandler_Create $BBF
#cmd LNDirectionalLightUpdateHandler_Create $BCA
#cmd LNPointLightSerializeHandler_Create $BD5
#cmd LNPointLightPreUpdateHandler_Create $BE0
#cmd LNPointLightUpdateHandler_Create $BEB
#cmd LNSpotLightSerializeHandler_Create $BF6
#cmd LNSpotLightPreUpdateHandler_Create $C01
#cmd LNSpotLightUpdateHandler_Create $C0C
#cmd LNSpriteSerializeHandler_Create $C17
#cmd LNSpritePreUpdateHandler_Create $C22
#cmd LNSpriteUpdateHandler_Create $C2D
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $C38
#cmd LNRaycasterSerializeHandler_Create $C43
#cmd LNRaycastResultSerializeHandler_Create $C4E
#cmd LNWorldRenderViewSerializeHandler_Create $C59
#cmd LNBoxMeshSerializeHandler_Create $C64
#cmd LNBoxMeshPreUpdateHandler_Create $C6F
#cmd LNBoxMeshUpdateHandler_Create $C7A
#cmd LNPlaneMeshSerializeHandler_Create $C85
#cmd LNPlaneMeshPreUpdateHandler_Create $C90
#cmd LNPlaneMeshUpdateHandler_Create $C9B
#cmd LNStaticMeshSerializeHandler_Create $CA6
#cmd LNStaticMeshPreUpdateHandler_Create $CB1
#cmd LNStaticMeshUpdateHandler_Create $CBC
#cmd LNStaticMeshComponentSerializeHandler_Create $CC7
#cmd LNSkinnedMeshComponentSerializeHandler_Create $CD2
#cmd LNCollisionSerializeHandler_Create $CDD
#cmd LNTriggerBodyComponentSerializeHandler_Create $CE8
#cmd LNParticleEmitterSerializeHandler_Create $CF3
#cmd LNParticleEmitterPreUpdateHandler_Create $CFE
#cmd LNParticleEmitterUpdateHandler_Create $D09
#cmd LNLevelSerializeHandler_Create $D14
#cmd LNLevelStartHandler_Create $D1F
#cmd LNLevelStopHandler_Create $D2A
#cmd LNLevelPauseHandler_Create $D35
#cmd LNLevelResumeHandler_Create $D40
#cmd LNLevelUpdateHandler_Create $D4B
#cmd LNUIEventArgsSerializeHandler_Create $D56
#cmd LNUILayoutElementSerializeHandler_Create $D61
#cmd LNUIElementSerializeHandler_Create $D6C
#cmd LNUITextBlockSerializeHandler_Create $D77
#cmd LNUISpriteSerializeHandler_Create $D82
#cmd LNUIIconSerializeHandler_Create $D8D
#cmd LNUIMessageTextAreaSerializeHandler_Create $D98
#cmd LNUILayoutPanelSerializeHandler_Create $DA3
#cmd LNUIBoxLayoutSerializeHandler_Create $DAE
#cmd LNUIStackLayoutSerializeHandler_Create $DB9
#cmd LNUIGridLayoutSerializeHandler_Create $DC4
#cmd LNUIControlSerializeHandler_Create $DCF
#cmd LNUIButtonBaseSerializeHandler_Create $DDA
#cmd LNUIButtonSerializeHandler_Create $DE5
#cmd LNUIWindowSerializeHandler_Create $DF0
#cmd LNUIListItemSerializeHandler_Create $DFB
#cmd LNUIListItemsControlSerializeHandler_Create $E06
#cmd LNUIListBoxItemSerializeHandler_Create $E11
#cmd LNUIListBoxSerializeHandler_Create $E1C
#cmd LNInputGestureSerializeHandler_Create $E27
#cmd LNKeyGestureSerializeHandler_Create $E32
#cmd LNInterpreterCommandSerializeHandler_Create $E3D
#cmd LNInterpreterCommandListSerializeHandler_Create $E48
#cmd LNInterpreterSerializeHandler_Create $E53
#cmd LNInterpreterUpdateWaitHandler_Create $E5F
#cmd LNApplicationSerializeHandler_Create $E6A
#cmd LNApplicationInitHandler_Create $E75
#cmd LNApplicationUpdateHandler_Create $E80



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
