
#ifndef __lumino__
#define __lumino__

#ifdef LUMINO_DEBUG
    #regcmd "_hsp3cmdinit@4","LuminoHSPd.dll", 11
#else
    #regcmd "_hsp3cmdinit@4","LuminoHSP.dll", 11
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
#cmd LNVector3_GetX $53D
#cmd LNVector3_SetX $53F
#cmd LNVector3_GetY $540
#cmd LNVector3_SetY $542
#cmd LNVector3_GetZ $543
#cmd LNVector3_SetZ $545
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $55C
#cmd LNVector4_SetX $55E
#cmd LNVector4_GetY $55F
#cmd LNVector4_SetY $561
#cmd LNVector4_GetZ $562
#cmd LNVector4_SetZ $564
#cmd LNVector4_GetW $565
#cmd LNVector4_SetW $567
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $57A
#cmd LNQuaternion_SetX $57C
#cmd LNQuaternion_GetY $57D
#cmd LNQuaternion_SetY $57F
#cmd LNQuaternion_GetZ $580
#cmd LNQuaternion_SetZ $582
#cmd LNQuaternion_GetW $583
#cmd LNQuaternion_SetW $585
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $599
#cmd LNMatrix_SetRow0 $59B
#cmd LNMatrix_GetRow1 $59C
#cmd LNMatrix_SetRow1 $59E
#cmd LNMatrix_GetRow2 $59F
#cmd LNMatrix_SetRow2 $5A1
#cmd LNMatrix_GetRow3 $5A2
#cmd LNMatrix_SetRow3 $5A4
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $173
#cmd LNColor_GetR $636
#cmd LNColor_SetR $638
#cmd LNColor_GetG $639
#cmd LNColor_SetG $63B
#cmd LNColor_GetB $63C
#cmd LNColor_SetB $63E
#cmd LNColor_GetA $63F
#cmd LNColor_SetA $641
#cmd LNColor_SetZeros $178
#cmd LNColor_Set $179
#cmd LNColorTone $17E
#cmd LNColorTone_GetR $655
#cmd LNColorTone_SetR $657
#cmd LNColorTone_GetG $658
#cmd LNColorTone_SetG $65A
#cmd LNColorTone_GetB $65B
#cmd LNColorTone_SetB $65D
#cmd LNColorTone_GetS $65E
#cmd LNColorTone_SetS $660
#cmd LNColorTone_SetZeros $183
#cmd LNColorTone_Set $184
#cmd LNPoint $189
#cmd LNPoint_GetX $673
#cmd LNPoint_SetX $675
#cmd LNPoint_GetY $676
#cmd LNPoint_SetY $678
#cmd LNPoint_SetZeros $18C
#cmd LNPoint_Set $18D
#cmd LNSize $190
#cmd LNSize_GetWidth $683
#cmd LNSize_SetWidth $685
#cmd LNSize_GetHeight $686
#cmd LNSize_SetHeight $688
#cmd LNSize_SetZeros $193
#cmd LNSize_Set $194
#cmd LNRect $197
#cmd LNRect_GetX $693
#cmd LNRect_SetX $695
#cmd LNRect_GetY $696
#cmd LNRect_SetY $698
#cmd LNRect_GetWidth $699
#cmd LNRect_SetWidth $69B
#cmd LNRect_GetHeight $69C
#cmd LNRect_SetHeight $69E
#cmd LNRect_SetZeros $19C
#cmd LNRect_Set $19D
#cmd LNRect_GetLeft $1A2
#cmd LNRect_SetSize $1A3
#cmd LNRect_GetSize $1A5
#cmd LNThickness $1A6
#cmd LNThickness_GetLeft $6B7
#cmd LNThickness_SetLeft $6B9
#cmd LNThickness_GetTop $6BA
#cmd LNThickness_SetTop $6BC
#cmd LNThickness_GetRight $6BD
#cmd LNThickness_SetRight $6BF
#cmd LNThickness_GetBottom $6C0
#cmd LNThickness_SetBottom $6C2
#cmd LNThickness_SetZeros $1AB
#cmd LNThickness_Set $1AC
#cmd LNCornerRadius $1B1
#cmd LNCornerRadius_GetTopleft $6D5
#cmd LNCornerRadius_SetTopleft $6D7
#cmd LNCornerRadius_GetTopright $6D8
#cmd LNCornerRadius_SetTopright $6DA
#cmd LNCornerRadius_GetBottomright $6DB
#cmd LNCornerRadius_SetBottomright $6DD
#cmd LNCornerRadius_GetBottomleft $6DE
#cmd LNCornerRadius_SetBottomleft $6E0
#cmd LNCornerRadius_SetZeros $1B6
#cmd LNCornerRadius_Set $1B7

#cmd LNObject_Release $7A
#cmd LNObject_Retain $7B
#cmd LNObject_GetReferenceCount $7C
#cmd LNObject_SetPrototype_OnSerialize $97A
#cmd LNEventConnection_SetPrototype_OnSerialize $985
#cmd LNPromiseFailureDelegate_Create $5BF
#cmd LNVariant_Create $81
#cmd LNVariant_SetInt $82
#cmd LNVariant_GetInt $84
#cmd LNVariant_SetPrototype_OnSerialize $990
#cmd LNZVTestDelegate1_Create $5C8
#cmd LNZVTestDelegate2_Create $5CE
#cmd LNZVTestDelegate3_Create $5D3
#cmd LNZVTestEventHandler1_Create $5D8
#cmd LNZVTestEventHandler2_Create $5DD
#cmd LNZVTestPromise1_ThenWith $5E0
#cmd LNZVTestPromise1_CatchWith $5E2
#cmd LNZVTestPromise2_ThenWith $5E6
#cmd LNZVTestPromise2_CatchWith $5E8
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $99B
#cmd LNZVTestEventArgs1_Create $B0
#cmd LNZVTestEventArgs1_CreateWithValue $B1
#cmd LNZVTestEventArgs1_GetValue $AF
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $9A6
#cmd LNSerializer2_SetPrototype_OnSerialize $9B1
#cmd LNAssetObject_SetPrototype_OnSerialize $9BC
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $9C7
#cmd LNAssetModel_Create $113
#cmd LNAssetModel_Target $112
#cmd LNAssetModel_SetPrototype_OnSerialize $9D2
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
#cmd LNSound_SetPrototype_OnSerialize $9DD
#cmd LNAudio_PlayBGM $13F
#cmd LNAudio_StopBGM $144
#cmd LNAudio_PlayBGS $146
#cmd LNAudio_StopBGS $14B
#cmd LNAudio_PlayME $14D
#cmd LNAudio_StopME $151
#cmd LNAudio_PlaySE $152
#cmd LNAudio_PlaySE3D $156
#cmd LNAudio_StopSE $15C
#cmd LNTexture2DDelegate_Create $62E
#cmd LNTexture2DPromise_ThenWith $631
#cmd LNTexture2DPromise_CatchWith $633
#cmd LNTexture_SetPrototype_OnSerialize $9E8
#cmd LNTexture2D_Create $1C2
#cmd LNTexture2D_CreateWithFormat $1C5
#cmd LNTexture2D_Load $1BE
#cmd LNTexture2D_LoadEmoji $1C0
#cmd LNTexture2D_SetPrototype_OnSerialize $9F3
#cmd LNShader_Load $1CA
#cmd LNShader_SetFloat $1CD
#cmd LNShader_SetVector3 $1D0
#cmd LNShader_SetVector4 $1D3
#cmd LNShader_SetPrototype_OnSerialize $9FE
#cmd LNRenderView_SetPrototype_OnSerialize $A09
#cmd LNMaterial_Create $1F8
#cmd LNMaterial_SetMainTexture $1E7
#cmd LNMaterial_GetMainTexture $1E9
#cmd LNMaterial_SetColor $1EA
#cmd LNMaterial_SetRoughness $1EC
#cmd LNMaterial_SetMetallic $1EE
#cmd LNMaterial_SetEmissive $1F0
#cmd LNMaterial_SetShadingModel $1F2
#cmd LNMaterial_GetShadingModel $1F4
#cmd LNMaterial_SetShader $1F5
#cmd LNMaterial_GetShader $1F7
#cmd LNMaterial_SetPrototype_OnSerialize $A14
#cmd LNMeshNode_SetVisible $1FA
#cmd LNMeshNode_IsVisible $1FC
#cmd LNMeshNode_SetPrototype_OnSerialize $A1F
#cmd LNStaticMeshModel_Load $1FE
#cmd LNStaticMeshModel_FindNode $200
#cmd LNStaticMeshModel_FindMaterial $202
#cmd LNStaticMeshModel_MaterialCount $204
#cmd LNStaticMeshModel_Material $205
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $A2A
#cmd LNSkinnedMeshModel_Load $208
#cmd LNSkinnedMeshModel_GetAnimationController $20A
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $A35
#cmd LNAnimationController_AddClip $20C
#cmd LNAnimationController_Play $20E
#cmd LNAnimationController_SetPrototype_OnSerialize $A40
#cmd LNCollisionShape_SetPrototype_OnSerialize $A4B
#cmd LNBoxCollisionShape_Create $213
#cmd LNBoxCollisionShape_CreateWHD $215
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $A56
#cmd LNAnimationClip_Load $223
#cmd LNAnimationClip_SetWrapMode $225
#cmd LNAnimationClip_GetWrapMode $227
#cmd LNAnimationClip_SetHierarchicalAnimationMode $228
#cmd LNAnimationClip_GetHierarchicalAnimationMode $22A
#cmd LNAnimationClip_SetPrototype_OnSerialize $A61
#cmd LNAnimationState_SetPrototype_OnSerialize $A6C
#cmd LNEffectResource_SetPrototype_OnSerialize $A77
#cmd LNParticleEmitterModel_Create $24F
#cmd LNParticleEmitterModel_SetMaxParticles $237
#cmd LNParticleEmitterModel_SetSpawnRate $239
#cmd LNParticleEmitterModel_SetLifeTime $23B
#cmd LNParticleEmitterModel_SetupBoxShape $23D
#cmd LNParticleEmitterModel_SetSize $23F
#cmd LNParticleEmitterModel_SetSizeVelocity $241
#cmd LNParticleEmitterModel_SetSizeAcceleration $243
#cmd LNParticleEmitterModel_SetForwardVelocityMin $245
#cmd LNParticleEmitterModel_SetForwardVelocityMax $247
#cmd LNParticleEmitterModel_SetForwardScale $249
#cmd LNParticleEmitterModel_SetGeometryDirection $24B
#cmd LNParticleEmitterModel_SetupSpriteModule $24D
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $A82
#cmd LNParticleModel_Create $256
#cmd LNParticleModel_SetLoop $251
#cmd LNParticleModel_IsLoop $253
#cmd LNParticleModel_AddEmitter $254
#cmd LNParticleModel_SetPrototype_OnSerialize $A8D
#cmd LNComponent_SetPrototype_OnSerialize $A98
#cmd LNVisualComponent_SetVisible $259
#cmd LNVisualComponent_IsVisible $25B
#cmd LNVisualComponent_SetPrototype_OnSerialize $AA3
#cmd LNSpriteComponent_SetTexture $25D
#cmd LNSpriteComponent_SetPrototype_OnSerialize $AAE
#cmd LNCollisionEventHandler_Create $75C
#cmd LNCharacterController_Create $27D
#cmd LNCharacterController_SetWalkVelocity $267
#cmd LNCharacterController_GetWalkVelocity $269
#cmd LNCharacterController_SetVelocity $26A
#cmd LNCharacterController_GetVelocity $26C
#cmd LNCharacterController_SetInputControlEnabled $26D
#cmd LNCharacterController_SetCameraControlEnabled $26F
#cmd LNCharacterController_SetHeight $271
#cmd LNCharacterController_GetHeight $273
#cmd LNCharacterController_SetCameraRadius $274
#cmd LNCharacterController_GetCameraRadius $276
#cmd LNCharacterController_SetCollisionEnter $277
#cmd LNCharacterController_SetCollisionLeave $279
#cmd LNCharacterController_SetCollisionStay $27B
#cmd LNCharacterController_SetPrototype_OnSerialize $AB9
#cmd LNWorld_Add $27F
#cmd LNWorld_SetPrototype_OnSerialize $AC4
#cmd LNComponentList_GetLength $77C
#cmd LNComponentList_GetItem $77E
#cmd LNComponentList_SetPrototype_OnSerialize $ACF
#cmd LNWorldObject_Create $2B6
#cmd LNWorldObject_SetPosition $283
#cmd LNWorldObject_SetPositionXYZ $285
#cmd LNWorldObject_GetPosition $289
#cmd LNWorldObject_SetRotationQuaternion $28A
#cmd LNWorldObject_SetRotation $28C
#cmd LNWorldObject_GetRotation $290
#cmd LNWorldObject_SetScale $291
#cmd LNWorldObject_SetScaleS $293
#cmd LNWorldObject_SetScaleXYZ $295
#cmd LNWorldObject_GetScale $299
#cmd LNWorldObject_SetCenterPoint $29A
#cmd LNWorldObject_SetCenterPointXYZ $29C
#cmd LNWorldObject_GetCenterPoint $2A0
#cmd LNWorldObject_LookAt $2A1
#cmd LNWorldObject_LookAtXYZ $2A3
#cmd LNWorldObject_AddComponent $2A7
#cmd LNWorldObject_RemoveComponent $2A9
#cmd LNWorldObject_AddTag $2AB
#cmd LNWorldObject_RemoveTag $2AD
#cmd LNWorldObject_HasTag $2AF
#cmd LNWorldObject_Destroy $2B1
#cmd LNWorldObject_GetComponents $2B2
#cmd LNWorldObject_SetPrototype_OnSerialize $ADA
#cmd LNWorldObject_SetPrototype_OnPreUpdate $AE5
#cmd LNWorldObject_SetPrototype_OnUpdate $AF0
#cmd LNVisualObject_SetVisible $2B8
#cmd LNVisualObject_IsVisible $2BA
#cmd LNVisualObject_SetBlendMode2 $2BB
#cmd LNVisualObject_SetOpacity $2BD
#cmd LNVisualObject_GetOpacity $2BF
#cmd LNVisualObject_SetPrototype_OnSerialize $AFB
#cmd LNVisualObject_SetPrototype_OnPreUpdate $B06
#cmd LNVisualObject_SetPrototype_OnUpdate $B11
#cmd LNCamera_SetPrototype_OnSerialize $B1C
#cmd LNCamera_SetPrototype_OnPreUpdate $B27
#cmd LNCamera_SetPrototype_OnUpdate $B32
#cmd LNEnvironmentLight_SetEnabled $2C2
#cmd LNEnvironmentLight_IsEnabled $2C4
#cmd LNEnvironmentLight_SetColor $2C5
#cmd LNEnvironmentLight_GetColor $2C7
#cmd LNEnvironmentLight_SetAmbientColor $2C8
#cmd LNEnvironmentLight_GetAmbientColor $2CA
#cmd LNEnvironmentLight_GetSkyColor $2CB
#cmd LNEnvironmentLight_SetSkyColor $2CC
#cmd LNEnvironmentLight_GetGroundColor $2CE
#cmd LNEnvironmentLight_SetGroundColor $2CF
#cmd LNEnvironmentLight_SetIntensity $2D1
#cmd LNEnvironmentLight_GetIntensity $2D3
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $2D4
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $2D6
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $2D7
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $2D9
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $B3D
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $B48
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $B53
#cmd LNDirectionalLight_Create $2EA
#cmd LNDirectionalLight_CreateWithColor $2EB
#cmd LNDirectionalLight_SetEnabled $2DB
#cmd LNDirectionalLight_IsEnabled $2DD
#cmd LNDirectionalLight_SetColor $2DE
#cmd LNDirectionalLight_GetColor $2E0
#cmd LNDirectionalLight_SetIntensity $2E1
#cmd LNDirectionalLight_GetIntensity $2E3
#cmd LNDirectionalLight_SetShadowEffectiveDistance $2E4
#cmd LNDirectionalLight_GetShadowEffectiveDistance $2E6
#cmd LNDirectionalLight_SetShadowEffectiveDepth $2E7
#cmd LNDirectionalLight_GetShadowEffectiveDepth $2E9
#cmd LNDirectionalLight_SetPrototype_OnSerialize $B5E
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $B69
#cmd LNDirectionalLight_SetPrototype_OnUpdate $B74
#cmd LNPointLight_Create $2FD
#cmd LNPointLight_CreateWithColorAndRange $2FE
#cmd LNPointLight_SetEnabled $2EE
#cmd LNPointLight_IsEnabled $2F0
#cmd LNPointLight_SetColor $2F1
#cmd LNPointLight_GetColor $2F3
#cmd LNPointLight_SetIntensity $2F4
#cmd LNPointLight_GetIntensity $2F6
#cmd LNPointLight_SetRange $2F7
#cmd LNPointLight_GetRange $2F9
#cmd LNPointLight_SetAttenuation $2FA
#cmd LNPointLight_GetAttenuation $2FC
#cmd LNPointLight_SetPrototype_OnSerialize $B7F
#cmd LNPointLight_SetPrototype_OnPreUpdate $B8A
#cmd LNPointLight_SetPrototype_OnUpdate $B95
#cmd LNSpotLight_Create $317
#cmd LNSpotLight_CreateWithColorAndRange $318
#cmd LNSpotLight_SetEnabled $302
#cmd LNSpotLight_IsEnabled $304
#cmd LNSpotLight_SetColor $305
#cmd LNSpotLight_GetColor $307
#cmd LNSpotLight_SetIntensity $308
#cmd LNSpotLight_GetIntensity $30A
#cmd LNSpotLight_SetRange $30B
#cmd LNSpotLight_GetRange $30D
#cmd LNSpotLight_SetAttenuation $30E
#cmd LNSpotLight_GetAttenuation $310
#cmd LNSpotLight_SetAngle $311
#cmd LNSpotLight_GetAngle $313
#cmd LNSpotLight_SetPenumbra $314
#cmd LNSpotLight_GetPenumbra $316
#cmd LNSpotLight_SetPrototype_OnSerialize $BA0
#cmd LNSpotLight_SetPrototype_OnPreUpdate $BAB
#cmd LNSpotLight_SetPrototype_OnUpdate $BB6
#cmd LNTestDelegate_Create $82A
#cmd LNSprite_Create $32E
#cmd LNSprite_CreateWithTexture $32F
#cmd LNSprite_CreateWithTextureAndSize $331
#cmd LNSprite_SetTexture $320
#cmd LNSprite_SetSize $322
#cmd LNSprite_SetSizeWH $324
#cmd LNSprite_SetSourceRectXYWH $327
#cmd LNSprite_SetCallerTest $32C
#cmd LNSprite_SetPrototype_OnSerialize $BC1
#cmd LNSprite_SetPrototype_OnPreUpdate $BCC
#cmd LNSprite_SetPrototype_OnUpdate $BD7
#cmd LNCameraOrbitControlComponent_Create $336
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $BE2
#cmd LNRaycaster_FromScreen $338
#cmd LNRaycaster_IntersectPlane $33A
#cmd LNRaycaster_SetPrototype_OnSerialize $BED
#cmd LNRaycastResult_GetPoint $33F
#cmd LNRaycastResult_SetPrototype_OnSerialize $BF8
#cmd LNWorldRenderView_SetGuideGridEnabled $341
#cmd LNWorldRenderView_GetGuideGridEnabled $343
#cmd LNWorldRenderView_SetPrototype_OnSerialize $C03
#cmd LNBoxMesh_Create $345
#cmd LNBoxMesh_SetPrototype_OnSerialize $C0E
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $C19
#cmd LNBoxMesh_SetPrototype_OnUpdate $C24
#cmd LNPlaneMesh_Create $347
#cmd LNPlaneMesh_SetPrototype_OnSerialize $C2F
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $C3A
#cmd LNPlaneMesh_SetPrototype_OnUpdate $C45
#cmd LNStaticMesh_Load $349
#cmd LNStaticMesh_GetModel $34B
#cmd LNStaticMesh_MakeCollisionBody $34C
#cmd LNStaticMesh_SetPrototype_OnSerialize $C50
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $C5B
#cmd LNStaticMesh_SetPrototype_OnUpdate $C66
#cmd LNStaticMeshComponent_Create $353
#cmd LNStaticMeshComponent_SetModel $34F
#cmd LNStaticMeshComponent_MakeCollisionBody $351
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $C71
#cmd LNSkinnedMeshComponent_Create $357
#cmd LNSkinnedMeshComponent_SetModel $355
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $C7C
#cmd LNCollision_GetWorldObject $359
#cmd LNCollision_SetPrototype_OnSerialize $C87
#cmd LNTriggerBodyComponent_Create $35D
#cmd LNTriggerBodyComponent_AddCollisionShape $35B
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $C92
#cmd LNParticleEmitter_Create $35F
#cmd LNParticleEmitter_SetPrototype_OnSerialize $C9D
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $CA8
#cmd LNParticleEmitter_SetPrototype_OnUpdate $CB3
#cmd LNScene_SetClearMode $362
#cmd LNScene_SetSkyColor $364
#cmd LNScene_SetSkyHorizonColor $366
#cmd LNScene_SetSkyCloudColor $368
#cmd LNScene_SetSkyOverlayColor $36A
#cmd LNScene_GotoLevel $36C
#cmd LNScene_CallLevel $36F
#cmd LNScene_ReturnLevel $372
#cmd LNScene_ActiveLevel $374
#cmd LNScene_IsTransitionEffectRunning $375
#cmd LNScene_SetTransitionEffectMode $376
#cmd LNScene_TransitionEffectMode $378
#cmd LNScene_SetTransitionDuration $379
#cmd LNScene_TransitionDuration $37B
#cmd LNScene_SetTransitionEffectColor $37C
#cmd LNScene_TransitionEffectColor $37E
#cmd LNScene_SetTransitionEffectMaskTexture $37F
#cmd LNScene_TransitionEffectMaskTexture $381
#cmd LNScene_SetTransitionEffectVague $382
#cmd LNScene_TransitionEffectVague $384
#cmd LNScene_StartFadeOut $385
#cmd LNScene_StartFadeIn $386
#cmd LNScene_SetFogStartDistance $387
#cmd LNScene_SetFogColor $389
#cmd LNScene_SetFogDensity $38B
#cmd LNScene_SetFogHeightDensity $38D
#cmd LNScene_SetFogLowerHeight $38F
#cmd LNScene_SetFogUpperHeight $391
#cmd LNScene_SetHDREnabled $393
#cmd LNScene_IsHDREnabled $395
#cmd LNScene_SetScreenBlendColor $396
#cmd LNScene_ScreenBlendColor $398
#cmd LNScene_SetColorTone $399
#cmd LNScene_ColorTone $39B
#cmd LNScene_SetAntialiasEnabled $39C
#cmd LNScene_IsAntialiasEnabled $39E
#cmd LNScene_SetSSREnabled $39F
#cmd LNScene_IsSSREnabled $3A1
#cmd LNScene_SetSSAOEnabled $3A2
#cmd LNScene_IsSSAOEnabled $3A4
#cmd LNScene_SetBloomEnabled $3A5
#cmd LNScene_IsBloomEnabled $3A7
#cmd LNScene_SetDOFEnabled $3A8
#cmd LNScene_IsDOFEnabled $3AA
#cmd LNScene_SetTonemapEnabled $3AB
#cmd LNScene_IsTonemapEnabled $3AD
#cmd LNScene_SetVignetteEnabled $3AE
#cmd LNScene_IsVignetteEnabled $3B0
#cmd LNScene_SetGammaEnabled $3B1
#cmd LNScene_IsGammaEnabled $3B3
#cmd LNScene_SetTonemapExposure $3B4
#cmd LNScene_SetTonemapLinearWhite $3B6
#cmd LNScene_SetTonemapShoulderStrength $3B8
#cmd LNScene_SetTonemapLinearStrength $3BA
#cmd LNScene_SetTonemapLinearAngle $3BC
#cmd LNScene_SetTonemapToeStrength $3BE
#cmd LNScene_SetTonemapToeNumerator $3C0
#cmd LNScene_SetTonemapToeDenominator $3C2
#cmd LNLevel_Create $3D3
#cmd LNLevel_AddObject $3C5
#cmd LNLevel_RemoveObject $3C7
#cmd LNLevel_AddSubLevel $3C9
#cmd LNLevel_RemoveSubLevel $3CB
#cmd LNLevel_RemoveAllSubLevels $3CD
#cmd LNLevel_SetPrototype_OnSerialize $CBE
#cmd LNLevel_SetPrototype_OnStart $CC9
#cmd LNLevel_SetPrototype_OnStop $CD4
#cmd LNLevel_SetPrototype_OnPause $CDF
#cmd LNLevel_SetPrototype_OnResume $CEA
#cmd LNLevel_SetPrototype_OnUpdate $CF5
#cmd LNUIEventArgs_Sender $3DE
#cmd LNUIEventArgs_SetPrototype_OnSerialize $D00
#cmd LNUIGeneralEventHandler_Create $889
#cmd LNUIEventHandler_Create $88E
#cmd LNUILayoutElement_SetPrototype_OnSerialize $D0B
#cmd LNUIElement_SetSize $3F0
#cmd LNUIElement_SetSizeWH $3F2
#cmd LNUIElement_SetWidth $3F5
#cmd LNUIElement_GetWidth $3F7
#cmd LNUIElement_SetHeight $3F8
#cmd LNUIElement_GetHeight $3FA
#cmd LNUIElement_SetMargin $3FB
#cmd LNUIElement_GetMargin $3FD
#cmd LNUIElement_SetPadding $3FE
#cmd LNUIElement_GetPadding $400
#cmd LNUIElement_SetHAlignment $401
#cmd LNUIElement_GetHAlignment $403
#cmd LNUIElement_SetVAlignment $404
#cmd LNUIElement_GetVAlignment $406
#cmd LNUIElement_SetAlignments $407
#cmd LNUIElement_SetPosition $40A
#cmd LNUIElement_SetPositionXYZ $40C
#cmd LNUIElement_GetPosition $410
#cmd LNUIElement_SetRotation $411
#cmd LNUIElement_SetEulerAngles $413
#cmd LNUIElement_GetRotation $417
#cmd LNUIElement_SetScale $418
#cmd LNUIElement_SetScaleS $41A
#cmd LNUIElement_SetScaleXY $41C
#cmd LNUIElement_GetScale $41F
#cmd LNUIElement_SetCenterPoint $420
#cmd LNUIElement_SetCenterPointXYZ $422
#cmd LNUIElement_GetCenterPoint $426
#cmd LNUIElement_SetEnabled $427
#cmd LNUIElement_IsEnabled $429
#cmd LNUIElement_SetData $42A
#cmd LNUIElement_GetData $42C
#cmd LNUIElement_SetBackgroundColor $42D
#cmd LNUIElement_GetBackgroundColor $42F
#cmd LNUIElement_SetBorderThickness $430
#cmd LNUIElement_GetBorderThickness $432
#cmd LNUIElement_SetBorderColor $433
#cmd LNUIElement_GetBorderColor $435
#cmd LNUIElement_SetCornerRadius $436
#cmd LNUIElement_GetCornerRadius $438
#cmd LNUIElement_SetVisibility $439
#cmd LNUIElement_GetVisibility $43B
#cmd LNUIElement_SetOpacity $43C
#cmd LNUIElement_GetOpacity $43E
#cmd LNUIElement_AddChild $43F
#cmd LNUIElement_SetPrototype_OnSerialize $D16
#cmd LNUITextBlock_Create $445
#cmd LNUITextBlock_CreateWithText $446
#cmd LNUITextBlock_SetText $442
#cmd LNUITextBlock_GetText $444
#cmd LNUITextBlock_SetPrototype_OnSerialize $D21
#cmd LNUISprite_Create $453
#cmd LNUISprite_CreateWithTexture $454
#cmd LNUISprite_SetTexture $449
#cmd LNUISprite_SetSourceRect $44B
#cmd LNUISprite_SetSourceRectXYWH $44D
#cmd LNUISprite_GetSourceRect $452
#cmd LNUISprite_SetPrototype_OnSerialize $D2C
#cmd LNUIIcon_LoadFontIcon $457
#cmd LNUIIcon_LoadFontIconWithNameSize $459
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $45C
#cmd LNUIIcon_SetPrototype_OnSerialize $D37
#cmd LNUIMessageTextArea_Create $465
#cmd LNUIMessageTextArea_SetText $461
#cmd LNUIMessageTextArea_SetTypingSpeed $463
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $D42
#cmd LNUI_Add $467
#cmd LNUI_Remove $469
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $D4D
#cmd LNUIBoxLayout_Create $470
#cmd LNUIBoxLayout_SetOrientation $46D
#cmd LNUIBoxLayout_GetOrientation $46F
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $D58
#cmd LNUIStackLayout_Create $475
#cmd LNUIStackLayout_SetOrientation $472
#cmd LNUIStackLayout_GetOrientation $474
#cmd LNUIStackLayout_SetPrototype_OnSerialize $D63
#cmd LNUIGridLayout_Create $485
#cmd LNUIGridLayout_SetColumnCount $477
#cmd LNUIGridLayout_SetRow $479
#cmd LNUIGridLayout_SetColumn $47C
#cmd LNUIGridLayout_SetPlacement $47F
#cmd LNUIGridLayout_SetPrototype_OnSerialize $D6E
#cmd LNUIControl_Create $491
#cmd LNUIControl_AddInlineVisual $492
#cmd LNUIControl_SetPrototype_OnSerialize $D79
#cmd LNUIButtonBase_SetText $496
#cmd LNUIButtonBase_SetPrototype_OnSerialize $D84
#cmd LNUIButton_Create $499
#cmd LNUIButton_CreateWithText $49A
#cmd LNUIButton_ConnectOnClicked $49C
#cmd LNUIButton_SetPrototype_OnSerialize $D8F
#cmd LNUIWindow_Create $49F
#cmd LNUIWindow_SetPrototype_OnSerialize $D9A
#cmd LNUIListItem_ConnectOnSubmit $4A4
#cmd LNUIListItem_SetPrototype_OnSerialize $DA5
#cmd LNUIListItemsControl_SetItemsLayoutPanel $4A7
#cmd LNUIListItemsControl_SetSubmitMode $4A9
#cmd LNUIListItemsControl_GetSubmitMode $4AB
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $DB0
#cmd LNUIListBoxItem_Create $4AD
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $DBB
#cmd LNUIListBox_Create $4B2
#cmd LNUIListBox_AddItem $4B0
#cmd LNUIListBox_SetPrototype_OnSerialize $DC6
#cmd LNInputGesture_SetPrototype_OnSerialize $DD1
#cmd LNKeyGesture_Create $4B5
#cmd LNKeyGesture_SetPrototype_OnSerialize $DDC
#cmd LNInput_IsPressed $4B8
#cmd LNInput_IsTriggered $4BA
#cmd LNInput_IsTriggeredOff $4BC
#cmd LNInput_IsRepeated $4BE
#cmd LNInput_GetAxisValue $4C0
#cmd LNInput_AddBinding $4C2
#cmd LNInput_RemoveBinding $4C5
#cmd LNInput_ClearBindings $4C7
#cmd LNInput_ClearAllBindings $4C9
#cmd LNMouse_Pressed $4CB
#cmd LNMouse_Triggered $4CD
#cmd LNMouse_TriggeredOff $4CF
#cmd LNMouse_Repeated $4D1
#cmd LNMouse_Position $4D3
#cmd LNInterpreterCommand_Code $4D5
#cmd LNInterpreterCommand_ParamsCount $4D6
#cmd LNInterpreterCommand_Param $4D7
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $DE7
#cmd LNInterpreterCommandList_Create $4EE
#cmd LNInterpreterCommandList_AddCommand $4DA
#cmd LNInterpreterCommandList_AddCommand1 $4DC
#cmd LNInterpreterCommandList_AddCommand2 $4DF
#cmd LNInterpreterCommandList_AddCommand3 $4E3
#cmd LNInterpreterCommandList_AddCommand4 $4E8
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $DF2
#cmd LNInterpreterCommandDelegate_Create $94C
#cmd LNInterpreter_Create $502
#cmd LNInterpreter_Clear $4F3
#cmd LNInterpreter_Run $4F4
#cmd LNInterpreter_IsRunning $4F6
#cmd LNInterpreter_Update $4F7
#cmd LNInterpreter_Terminate $4F8
#cmd LNInterpreter_RegisterCommandHandler $4F9
#cmd LNInterpreter_SetWaitMode $4FC
#cmd LNInterpreter_GetWaitMode $4FE
#cmd LNInterpreter_SetWaitCount $4FF
#cmd LNInterpreter_GetWaitCount $501
#cmd LNInterpreter_SetPrototype_OnSerialize $DFD
#cmd LNInterpreter_SetPrototype_OnUpdateWait $E09
#cmd LNEngineSettings_SetMainWindowSize $505
#cmd LNEngineSettings_SetMainWorldViewSize $508
#cmd LNEngineSettings_SetMainWindowTitle $50B
#cmd LNEngineSettings_AddAssetDirectory $50D
#cmd LNEngineSettings_AddAssetArchive $50F
#cmd LNEngineSettings_SetFrameRate $512
#cmd LNEngineSettings_SetUITheme $514
#cmd LNEngineSettings_SetFontFile $516
#cmd LNEngineSettings_SetDebugToolEnabled $518
#cmd LNEngineSettings_SetEngineLogEnabled $51A
#cmd LNEngineSettings_SetEngineLogFilePath $51C
#cmd LNEngineSettings_SetDeveloperToolEnabled $51E
#cmd LNEngine_Initialize $521
#cmd LNEngine_Finalize $522
#cmd LNEngine_Update $523
#cmd LNEngine_Run $524
#cmd LNEngine_Time $526
#cmd LNEngine_GetCamera $527
#cmd LNEngine_GetMainLight $528
#cmd LNEngine_GetRenderView $529
#cmd LNApplication_Create $52E
#cmd LNApplication_OnInit $52B
#cmd LNApplication_OnUpdate $52C
#cmd LNApplication_World $52D
#cmd LNApplication_SetPrototype_OnSerialize $E14
#cmd LNApplication_SetPrototype_OnInit $E1F
#cmd LNApplication_SetPrototype_OnUpdate $E2A
#cmd LNDebug_SetGuideGridEnabled $530
#cmd LNDebug_SetPhysicsDebugDrawEnabled $532
#cmd LNDebug_Print $534
#cmd LNDebug_PrintWithTime $536
#cmd LNDebug_PrintWithTimeAndColor $539
#cmd LNObjectSerializeHandler_Create $978
#cmd LNEventConnectionSerializeHandler_Create $983
#cmd LNVariantSerializeHandler_Create $98E
#cmd LNZVTestClass1SerializeHandler_Create $999
#cmd LNZVTestEventArgs1SerializeHandler_Create $9A4
#cmd LNSerializer2SerializeHandler_Create $9AF
#cmd LNAssetObjectSerializeHandler_Create $9BA
#cmd LNAssetImportSettingsSerializeHandler_Create $9C5
#cmd LNAssetModelSerializeHandler_Create $9D0
#cmd LNSoundSerializeHandler_Create $9DB
#cmd LNTextureSerializeHandler_Create $9E6
#cmd LNTexture2DSerializeHandler_Create $9F1
#cmd LNShaderSerializeHandler_Create $9FC
#cmd LNRenderViewSerializeHandler_Create $A07
#cmd LNMaterialSerializeHandler_Create $A12
#cmd LNMeshNodeSerializeHandler_Create $A1D
#cmd LNStaticMeshModelSerializeHandler_Create $A28
#cmd LNSkinnedMeshModelSerializeHandler_Create $A33
#cmd LNAnimationControllerSerializeHandler_Create $A3E
#cmd LNCollisionShapeSerializeHandler_Create $A49
#cmd LNBoxCollisionShapeSerializeHandler_Create $A54
#cmd LNAnimationClipSerializeHandler_Create $A5F
#cmd LNAnimationStateSerializeHandler_Create $A6A
#cmd LNEffectResourceSerializeHandler_Create $A75
#cmd LNParticleEmitterModelSerializeHandler_Create $A80
#cmd LNParticleModelSerializeHandler_Create $A8B
#cmd LNComponentSerializeHandler_Create $A96
#cmd LNVisualComponentSerializeHandler_Create $AA1
#cmd LNSpriteComponentSerializeHandler_Create $AAC
#cmd LNCharacterControllerSerializeHandler_Create $AB7
#cmd LNWorldSerializeHandler_Create $AC2
#cmd LNComponentListSerializeHandler_Create $ACD
#cmd LNWorldObjectSerializeHandler_Create $AD8
#cmd LNWorldObjectPreUpdateHandler_Create $AE3
#cmd LNWorldObjectUpdateHandler_Create $AEE
#cmd LNVisualObjectSerializeHandler_Create $AF9
#cmd LNVisualObjectPreUpdateHandler_Create $B04
#cmd LNVisualObjectUpdateHandler_Create $B0F
#cmd LNCameraSerializeHandler_Create $B1A
#cmd LNCameraPreUpdateHandler_Create $B25
#cmd LNCameraUpdateHandler_Create $B30
#cmd LNEnvironmentLightSerializeHandler_Create $B3B
#cmd LNEnvironmentLightPreUpdateHandler_Create $B46
#cmd LNEnvironmentLightUpdateHandler_Create $B51
#cmd LNDirectionalLightSerializeHandler_Create $B5C
#cmd LNDirectionalLightPreUpdateHandler_Create $B67
#cmd LNDirectionalLightUpdateHandler_Create $B72
#cmd LNPointLightSerializeHandler_Create $B7D
#cmd LNPointLightPreUpdateHandler_Create $B88
#cmd LNPointLightUpdateHandler_Create $B93
#cmd LNSpotLightSerializeHandler_Create $B9E
#cmd LNSpotLightPreUpdateHandler_Create $BA9
#cmd LNSpotLightUpdateHandler_Create $BB4
#cmd LNSpriteSerializeHandler_Create $BBF
#cmd LNSpritePreUpdateHandler_Create $BCA
#cmd LNSpriteUpdateHandler_Create $BD5
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $BE0
#cmd LNRaycasterSerializeHandler_Create $BEB
#cmd LNRaycastResultSerializeHandler_Create $BF6
#cmd LNWorldRenderViewSerializeHandler_Create $C01
#cmd LNBoxMeshSerializeHandler_Create $C0C
#cmd LNBoxMeshPreUpdateHandler_Create $C17
#cmd LNBoxMeshUpdateHandler_Create $C22
#cmd LNPlaneMeshSerializeHandler_Create $C2D
#cmd LNPlaneMeshPreUpdateHandler_Create $C38
#cmd LNPlaneMeshUpdateHandler_Create $C43
#cmd LNStaticMeshSerializeHandler_Create $C4E
#cmd LNStaticMeshPreUpdateHandler_Create $C59
#cmd LNStaticMeshUpdateHandler_Create $C64
#cmd LNStaticMeshComponentSerializeHandler_Create $C6F
#cmd LNSkinnedMeshComponentSerializeHandler_Create $C7A
#cmd LNCollisionSerializeHandler_Create $C85
#cmd LNTriggerBodyComponentSerializeHandler_Create $C90
#cmd LNParticleEmitterSerializeHandler_Create $C9B
#cmd LNParticleEmitterPreUpdateHandler_Create $CA6
#cmd LNParticleEmitterUpdateHandler_Create $CB1
#cmd LNLevelSerializeHandler_Create $CBC
#cmd LNLevelStartHandler_Create $CC7
#cmd LNLevelStopHandler_Create $CD2
#cmd LNLevelPauseHandler_Create $CDD
#cmd LNLevelResumeHandler_Create $CE8
#cmd LNLevelUpdateHandler_Create $CF3
#cmd LNUIEventArgsSerializeHandler_Create $CFE
#cmd LNUILayoutElementSerializeHandler_Create $D09
#cmd LNUIElementSerializeHandler_Create $D14
#cmd LNUITextBlockSerializeHandler_Create $D1F
#cmd LNUISpriteSerializeHandler_Create $D2A
#cmd LNUIIconSerializeHandler_Create $D35
#cmd LNUIMessageTextAreaSerializeHandler_Create $D40
#cmd LNUILayoutPanelSerializeHandler_Create $D4B
#cmd LNUIBoxLayoutSerializeHandler_Create $D56
#cmd LNUIStackLayoutSerializeHandler_Create $D61
#cmd LNUIGridLayoutSerializeHandler_Create $D6C
#cmd LNUIControlSerializeHandler_Create $D77
#cmd LNUIButtonBaseSerializeHandler_Create $D82
#cmd LNUIButtonSerializeHandler_Create $D8D
#cmd LNUIWindowSerializeHandler_Create $D98
#cmd LNUIListItemSerializeHandler_Create $DA3
#cmd LNUIListItemsControlSerializeHandler_Create $DAE
#cmd LNUIListBoxItemSerializeHandler_Create $DB9
#cmd LNUIListBoxSerializeHandler_Create $DC4
#cmd LNInputGestureSerializeHandler_Create $DCF
#cmd LNKeyGestureSerializeHandler_Create $DDA
#cmd LNInterpreterCommandSerializeHandler_Create $DE5
#cmd LNInterpreterCommandListSerializeHandler_Create $DF0
#cmd LNInterpreterSerializeHandler_Create $DFB
#cmd LNInterpreterUpdateWaitHandler_Create $E07
#cmd LNApplicationSerializeHandler_Create $E12
#cmd LNApplicationInitHandler_Create $E1D
#cmd LNApplicationUpdateHandler_Create $E28



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
