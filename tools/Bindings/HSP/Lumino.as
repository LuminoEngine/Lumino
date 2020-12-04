
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
#cmd LNVector3_GetX $55E
#cmd LNVector3_SetX $560
#cmd LNVector3_GetY $561
#cmd LNVector3_SetY $563
#cmd LNVector3_GetZ $564
#cmd LNVector3_SetZ $566
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $57D
#cmd LNVector4_SetX $57F
#cmd LNVector4_GetY $580
#cmd LNVector4_SetY $582
#cmd LNVector4_GetZ $583
#cmd LNVector4_SetZ $585
#cmd LNVector4_GetW $586
#cmd LNVector4_SetW $588
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $59B
#cmd LNQuaternion_SetX $59D
#cmd LNQuaternion_GetY $59E
#cmd LNQuaternion_SetY $5A0
#cmd LNQuaternion_GetZ $5A1
#cmd LNQuaternion_SetZ $5A3
#cmd LNQuaternion_GetW $5A4
#cmd LNQuaternion_SetW $5A6
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $5BA
#cmd LNMatrix_SetRow0 $5BC
#cmd LNMatrix_GetRow1 $5BD
#cmd LNMatrix_SetRow1 $5BF
#cmd LNMatrix_GetRow2 $5C0
#cmd LNMatrix_SetRow2 $5C2
#cmd LNMatrix_GetRow3 $5C3
#cmd LNMatrix_SetRow3 $5C5
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $179
#cmd LNColor_GetR $659
#cmd LNColor_SetR $65B
#cmd LNColor_GetG $65C
#cmd LNColor_SetG $65E
#cmd LNColor_GetB $65F
#cmd LNColor_SetB $661
#cmd LNColor_GetA $662
#cmd LNColor_SetA $664
#cmd LNColor_SetZeros $17E
#cmd LNColor_Set $17F
#cmd LNColorTone $184
#cmd LNColorTone_GetR $678
#cmd LNColorTone_SetR $67A
#cmd LNColorTone_GetG $67B
#cmd LNColorTone_SetG $67D
#cmd LNColorTone_GetB $67E
#cmd LNColorTone_SetB $680
#cmd LNColorTone_GetS $681
#cmd LNColorTone_SetS $683
#cmd LNColorTone_SetZeros $189
#cmd LNColorTone_Set $18A
#cmd LNPoint $18F
#cmd LNPoint_GetX $696
#cmd LNPoint_SetX $698
#cmd LNPoint_GetY $699
#cmd LNPoint_SetY $69B
#cmd LNPoint_SetZeros $192
#cmd LNPoint_Set $193
#cmd LNSize $196
#cmd LNSize_GetWidth $6A6
#cmd LNSize_SetWidth $6A8
#cmd LNSize_GetHeight $6A9
#cmd LNSize_SetHeight $6AB
#cmd LNSize_SetZeros $199
#cmd LNSize_Set $19A
#cmd LNRect $19D
#cmd LNRect_GetX $6B6
#cmd LNRect_SetX $6B8
#cmd LNRect_GetY $6B9
#cmd LNRect_SetY $6BB
#cmd LNRect_GetWidth $6BC
#cmd LNRect_SetWidth $6BE
#cmd LNRect_GetHeight $6BF
#cmd LNRect_SetHeight $6C1
#cmd LNRect_SetZeros $1A2
#cmd LNRect_Set $1A3
#cmd LNRect_GetLeft $1A8
#cmd LNRect_SetSize $1A9
#cmd LNRect_GetSize $1AB
#cmd LNThickness $1AC
#cmd LNThickness_GetLeft $6DA
#cmd LNThickness_SetLeft $6DC
#cmd LNThickness_GetTop $6DD
#cmd LNThickness_SetTop $6DF
#cmd LNThickness_GetRight $6E0
#cmd LNThickness_SetRight $6E2
#cmd LNThickness_GetBottom $6E3
#cmd LNThickness_SetBottom $6E5
#cmd LNThickness_SetZeros $1B1
#cmd LNThickness_Set $1B2
#cmd LNCornerRadius $1B7
#cmd LNCornerRadius_GetTopleft $6F8
#cmd LNCornerRadius_SetTopleft $6FA
#cmd LNCornerRadius_GetTopright $6FB
#cmd LNCornerRadius_SetTopright $6FD
#cmd LNCornerRadius_GetBottomright $6FE
#cmd LNCornerRadius_SetBottomright $700
#cmd LNCornerRadius_GetBottomleft $701
#cmd LNCornerRadius_SetBottomleft $703
#cmd LNCornerRadius_SetZeros $1BC
#cmd LNCornerRadius_Set $1BD

#cmd LNObject_Release $7A
#cmd LNObject_Retain $7B
#cmd LNObject_GetReferenceCount $7C
#cmd LNObject_SetPrototype_OnSerialize $9AC
#cmd LNEventConnection_SetPrototype_OnSerialize $9B7
#cmd LNPromiseFailureDelegate_Create $5E0
#cmd LNVariant_Create $81
#cmd LNVariant_SetInt $82
#cmd LNVariant_GetInt $84
#cmd LNVariant_SetPrototype_OnSerialize $9C2
#cmd LNZVTestDelegate1_Create $5E9
#cmd LNZVTestDelegate2_Create $5EF
#cmd LNZVTestDelegate3_Create $5F4
#cmd LNZVTestEventHandler1_Create $5F9
#cmd LNZVTestEventHandler2_Create $5FE
#cmd LNZVTestPromise1_ThenWith $601
#cmd LNZVTestPromise1_CatchWith $603
#cmd LNZVTestPromise2_ThenWith $607
#cmd LNZVTestPromise2_CatchWith $609
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $9CD
#cmd LNZVTestEventArgs1_Create $B0
#cmd LNZVTestEventArgs1_CreateWithValue $B1
#cmd LNZVTestEventArgs1_GetValue $AF
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $9D8
#cmd LNSerializer2_SetPrototype_OnSerialize $9E3
#cmd LNAssetObject_SetPrototype_OnSerialize $9EE
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $9F9
#cmd LNAssetModel_Create $113
#cmd LNAssetModel_Target $112
#cmd LNAssetModel_SetPrototype_OnSerialize $A04
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
#cmd LNSound_SetPrototype_OnSerialize $A0F
#cmd LNAudio_PlayBGM $13F
#cmd LNAudio_StopBGM $144
#cmd LNAudio_PlayBGS $146
#cmd LNAudio_StopBGS $14B
#cmd LNAudio_PlayME $14D
#cmd LNAudio_StopME $151
#cmd LNAudio_PlaySE $152
#cmd LNAudio_PlaySE3D $156
#cmd LNAudio_StopSE $15C
#cmd LNTexture2DDelegate_Create $64F
#cmd LNTexture2DPromise_ThenWith $652
#cmd LNTexture2DPromise_CatchWith $654
#cmd LNGraphics_GetActiveGraphicsAPI $178
#cmd LNTexture_SetPrototype_OnSerialize $A1A
#cmd LNTexture2D_Create $1C8
#cmd LNTexture2D_CreateWithFormat $1CB
#cmd LNTexture2D_Load $1C4
#cmd LNTexture2D_LoadEmoji $1C6
#cmd LNTexture2D_SetPrototype_OnSerialize $A25
#cmd LNShader_Load $1D0
#cmd LNShader_SetFloat $1D3
#cmd LNShader_SetVector3 $1D6
#cmd LNShader_SetVector4 $1D9
#cmd LNShader_SetTexture $1DC
#cmd LNShader_SetPrototype_OnSerialize $A30
#cmd LNRenderView_SetPrototype_OnSerialize $A3B
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
#cmd LNMaterial_SetPrototype_OnSerialize $A46
#cmd LNMeshNode_SetVisible $203
#cmd LNMeshNode_IsVisible $205
#cmd LNMeshNode_SetPrototype_OnSerialize $A51
#cmd LNAnimationController_AddClip $207
#cmd LNAnimationController_Play $209
#cmd LNAnimationController_SetPrototype_OnSerialize $A5C
#cmd LNMeshModel_Load $20D
#cmd LNMeshModel_FindNode $20F
#cmd LNMeshModel_FindMaterial $211
#cmd LNMeshModel_MaterialCount $213
#cmd LNMeshModel_Material $214
#cmd LNMeshModel_GetAnimationController $216
#cmd LNMeshModel_SetPrototype_OnSerialize $A67
#cmd LNSkinnedMeshModel_Load $218
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $A72
#cmd LNCollisionShape_SetPrototype_OnSerialize $A7D
#cmd LNBoxCollisionShape_Create $21C
#cmd LNBoxCollisionShape_CreateWHD $21E
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $A88
#cmd LNAnimationCurve_Evaluate $22C
#cmd LNAnimationCurve_SetPrototype_OnSerialize $A93
#cmd LNKeyFrameAnimationCurve_Create $239
#cmd LNKeyFrameAnimationCurve_AddKeyFrame $234
#cmd LNKeyFrameAnimationCurve_SetPrototype_OnSerialize $A9E
#cmd LNAnimationClip_Load $23B
#cmd LNAnimationClip_SetWrapMode $23D
#cmd LNAnimationClip_GetWrapMode $23F
#cmd LNAnimationClip_SetHierarchicalAnimationMode $240
#cmd LNAnimationClip_GetHierarchicalAnimationMode $242
#cmd LNAnimationClip_SetPrototype_OnSerialize $AA9
#cmd LNAnimationState_SetPrototype_OnSerialize $AB4
#cmd LNEffectResource_SetPrototype_OnSerialize $ABF
#cmd LNParticleEmitterModel_Create $267
#cmd LNParticleEmitterModel_SetMaxParticles $24F
#cmd LNParticleEmitterModel_SetSpawnRate $251
#cmd LNParticleEmitterModel_SetLifeTime $253
#cmd LNParticleEmitterModel_SetupBoxShape $255
#cmd LNParticleEmitterModel_SetSize $257
#cmd LNParticleEmitterModel_SetSizeVelocity $259
#cmd LNParticleEmitterModel_SetSizeAcceleration $25B
#cmd LNParticleEmitterModel_SetForwardVelocityMin $25D
#cmd LNParticleEmitterModel_SetForwardVelocityMax $25F
#cmd LNParticleEmitterModel_SetForwardScale $261
#cmd LNParticleEmitterModel_SetGeometryDirection $263
#cmd LNParticleEmitterModel_SetupSpriteModule $265
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $ACA
#cmd LNParticleModel_Create $26E
#cmd LNParticleModel_SetLoop $269
#cmd LNParticleModel_IsLoop $26B
#cmd LNParticleModel_AddEmitter $26C
#cmd LNParticleModel_SetPrototype_OnSerialize $AD5
#cmd LNComponent_SetPrototype_OnSerialize $AE0
#cmd LNVisualComponent_SetVisible $271
#cmd LNVisualComponent_IsVisible $273
#cmd LNVisualComponent_SetPrototype_OnSerialize $AEB
#cmd LNSpriteComponent_SetTexture $275
#cmd LNSpriteComponent_SetPrototype_OnSerialize $AF6
#cmd LNCollisionEventHandler_Create $786
#cmd LNCharacterController_Create $295
#cmd LNCharacterController_SetWalkVelocity $27F
#cmd LNCharacterController_GetWalkVelocity $281
#cmd LNCharacterController_SetVelocity $282
#cmd LNCharacterController_GetVelocity $284
#cmd LNCharacterController_SetInputControlEnabled $285
#cmd LNCharacterController_SetCameraControlEnabled $287
#cmd LNCharacterController_SetHeight $289
#cmd LNCharacterController_GetHeight $28B
#cmd LNCharacterController_SetCameraRadius $28C
#cmd LNCharacterController_GetCameraRadius $28E
#cmd LNCharacterController_SetCollisionEnter $28F
#cmd LNCharacterController_SetCollisionLeave $291
#cmd LNCharacterController_SetCollisionStay $293
#cmd LNCharacterController_SetPrototype_OnSerialize $B01
#cmd LNWorld_Add $297
#cmd LNWorld_SetPrototype_OnSerialize $B0C
#cmd LNComponentList_GetLength $7A6
#cmd LNComponentList_GetItem $7A8
#cmd LNComponentList_SetPrototype_OnSerialize $B17
#cmd LNWorldObject_Create $2CE
#cmd LNWorldObject_SetPosition $29B
#cmd LNWorldObject_SetPositionXYZ $29D
#cmd LNWorldObject_GetPosition $2A1
#cmd LNWorldObject_SetRotationQuaternion $2A2
#cmd LNWorldObject_SetRotation $2A4
#cmd LNWorldObject_GetRotation $2A8
#cmd LNWorldObject_SetScale $2A9
#cmd LNWorldObject_SetScaleS $2AB
#cmd LNWorldObject_SetScaleXYZ $2AD
#cmd LNWorldObject_GetScale $2B1
#cmd LNWorldObject_SetCenterPoint $2B2
#cmd LNWorldObject_SetCenterPointXYZ $2B4
#cmd LNWorldObject_GetCenterPoint $2B8
#cmd LNWorldObject_LookAt $2B9
#cmd LNWorldObject_LookAtXYZ $2BB
#cmd LNWorldObject_AddComponent $2BF
#cmd LNWorldObject_RemoveComponent $2C1
#cmd LNWorldObject_AddTag $2C3
#cmd LNWorldObject_RemoveTag $2C5
#cmd LNWorldObject_HasTag $2C7
#cmd LNWorldObject_Destroy $2C9
#cmd LNWorldObject_GetComponents $2CA
#cmd LNWorldObject_SetPrototype_OnSerialize $B22
#cmd LNWorldObject_SetPrototype_OnPreUpdate $B2D
#cmd LNWorldObject_SetPrototype_OnUpdate $B38
#cmd LNVisualObject_SetVisible $2D0
#cmd LNVisualObject_IsVisible $2D2
#cmd LNVisualObject_SetBlendMode2 $2D3
#cmd LNVisualObject_SetOpacity $2D5
#cmd LNVisualObject_GetOpacity $2D7
#cmd LNVisualObject_SetPrototype_OnSerialize $B43
#cmd LNVisualObject_SetPrototype_OnPreUpdate $B4E
#cmd LNVisualObject_SetPrototype_OnUpdate $B59
#cmd LNCamera_SetPrototype_OnSerialize $B64
#cmd LNCamera_SetPrototype_OnPreUpdate $B6F
#cmd LNCamera_SetPrototype_OnUpdate $B7A
#cmd LNEnvironmentLight_SetEnabled $2DA
#cmd LNEnvironmentLight_IsEnabled $2DC
#cmd LNEnvironmentLight_SetColor $2DD
#cmd LNEnvironmentLight_GetColor $2DF
#cmd LNEnvironmentLight_SetAmbientColor $2E0
#cmd LNEnvironmentLight_GetAmbientColor $2E2
#cmd LNEnvironmentLight_GetSkyColor $2E3
#cmd LNEnvironmentLight_SetSkyColor $2E4
#cmd LNEnvironmentLight_GetGroundColor $2E6
#cmd LNEnvironmentLight_SetGroundColor $2E7
#cmd LNEnvironmentLight_SetIntensity $2E9
#cmd LNEnvironmentLight_GetIntensity $2EB
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $2EC
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $2EE
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $2EF
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $2F1
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $B85
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $B90
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $B9B
#cmd LNDirectionalLight_Create $302
#cmd LNDirectionalLight_CreateWithColor $303
#cmd LNDirectionalLight_SetEnabled $2F3
#cmd LNDirectionalLight_IsEnabled $2F5
#cmd LNDirectionalLight_SetColor $2F6
#cmd LNDirectionalLight_GetColor $2F8
#cmd LNDirectionalLight_SetIntensity $2F9
#cmd LNDirectionalLight_GetIntensity $2FB
#cmd LNDirectionalLight_SetShadowEffectiveDistance $2FC
#cmd LNDirectionalLight_GetShadowEffectiveDistance $2FE
#cmd LNDirectionalLight_SetShadowEffectiveDepth $2FF
#cmd LNDirectionalLight_GetShadowEffectiveDepth $301
#cmd LNDirectionalLight_SetPrototype_OnSerialize $BA6
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $BB1
#cmd LNDirectionalLight_SetPrototype_OnUpdate $BBC
#cmd LNPointLight_Create $315
#cmd LNPointLight_CreateWithColorAndRange $316
#cmd LNPointLight_SetEnabled $306
#cmd LNPointLight_IsEnabled $308
#cmd LNPointLight_SetColor $309
#cmd LNPointLight_GetColor $30B
#cmd LNPointLight_SetIntensity $30C
#cmd LNPointLight_GetIntensity $30E
#cmd LNPointLight_SetRange $30F
#cmd LNPointLight_GetRange $311
#cmd LNPointLight_SetAttenuation $312
#cmd LNPointLight_GetAttenuation $314
#cmd LNPointLight_SetPrototype_OnSerialize $BC7
#cmd LNPointLight_SetPrototype_OnPreUpdate $BD2
#cmd LNPointLight_SetPrototype_OnUpdate $BDD
#cmd LNSpotLight_Create $32F
#cmd LNSpotLight_CreateWithColorAndRange $330
#cmd LNSpotLight_SetEnabled $31A
#cmd LNSpotLight_IsEnabled $31C
#cmd LNSpotLight_SetColor $31D
#cmd LNSpotLight_GetColor $31F
#cmd LNSpotLight_SetIntensity $320
#cmd LNSpotLight_GetIntensity $322
#cmd LNSpotLight_SetRange $323
#cmd LNSpotLight_GetRange $325
#cmd LNSpotLight_SetAttenuation $326
#cmd LNSpotLight_GetAttenuation $328
#cmd LNSpotLight_SetAngle $329
#cmd LNSpotLight_GetAngle $32B
#cmd LNSpotLight_SetPenumbra $32C
#cmd LNSpotLight_GetPenumbra $32E
#cmd LNSpotLight_SetPrototype_OnSerialize $BE8
#cmd LNSpotLight_SetPrototype_OnPreUpdate $BF3
#cmd LNSpotLight_SetPrototype_OnUpdate $BFE
#cmd LNTestDelegate_Create $854
#cmd LNSprite_Create $346
#cmd LNSprite_CreateWithTexture $347
#cmd LNSprite_CreateWithTextureAndSize $349
#cmd LNSprite_SetTexture $338
#cmd LNSprite_SetSize $33A
#cmd LNSprite_SetSizeWH $33C
#cmd LNSprite_SetSourceRectXYWH $33F
#cmd LNSprite_SetCallerTest $344
#cmd LNSprite_SetPrototype_OnSerialize $C09
#cmd LNSprite_SetPrototype_OnPreUpdate $C14
#cmd LNSprite_SetPrototype_OnUpdate $C1F
#cmd LNCameraOrbitControlComponent_Create $34E
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $C2A
#cmd LNRaycaster_FromScreen $350
#cmd LNRaycaster_IntersectPlane $352
#cmd LNRaycaster_SetPrototype_OnSerialize $C35
#cmd LNRaycastResult_GetPoint $357
#cmd LNRaycastResult_SetPrototype_OnSerialize $C40
#cmd LNWorldRenderView_SetGuideGridEnabled $359
#cmd LNWorldRenderView_GetGuideGridEnabled $35B
#cmd LNWorldRenderView_SetPrototype_OnSerialize $C4B
#cmd LNBoxMesh_Create $35D
#cmd LNBoxMesh_CreateXYZ $35E
#cmd LNBoxMesh_SetPrototype_OnSerialize $C56
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $C61
#cmd LNBoxMesh_SetPrototype_OnUpdate $C6C
#cmd LNPlaneMesh_Create $363
#cmd LNPlaneMesh_SetPrototype_OnSerialize $C77
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $C82
#cmd LNPlaneMesh_SetPrototype_OnUpdate $C8D
#cmd LNStaticMesh_Load $365
#cmd LNStaticMesh_GetModel $367
#cmd LNStaticMesh_MakeCollisionBody $368
#cmd LNStaticMesh_SetPrototype_OnSerialize $C98
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $CA3
#cmd LNStaticMesh_SetPrototype_OnUpdate $CAE
#cmd LNStaticMeshComponent_Create $36F
#cmd LNStaticMeshComponent_SetModel $36B
#cmd LNStaticMeshComponent_MakeCollisionBody $36D
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $CB9
#cmd LNSkinnedMeshComponent_Create $371
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $CC4
#cmd LNCollision_GetWorldObject $373
#cmd LNCollision_SetPrototype_OnSerialize $CCF
#cmd LNTriggerBodyComponent_Create $377
#cmd LNTriggerBodyComponent_AddCollisionShape $375
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $CDA
#cmd LNParticleEmitter_Create $379
#cmd LNParticleEmitter_SetPrototype_OnSerialize $CE5
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $CF0
#cmd LNParticleEmitter_SetPrototype_OnUpdate $CFB
#cmd LNScene_SetClearMode $37C
#cmd LNScene_SetSkyColor $37E
#cmd LNScene_SetSkyHorizonColor $380
#cmd LNScene_SetSkyCloudColor $382
#cmd LNScene_SetSkyOverlayColor $384
#cmd LNScene_GotoLevel $386
#cmd LNScene_CallLevel $389
#cmd LNScene_ReturnLevel $38C
#cmd LNScene_ActiveLevel $38E
#cmd LNScene_IsTransitionEffectRunning $38F
#cmd LNScene_SetTransitionEffectMode $390
#cmd LNScene_TransitionEffectMode $392
#cmd LNScene_SetTransitionDuration $393
#cmd LNScene_TransitionDuration $395
#cmd LNScene_SetTransitionEffectColor $396
#cmd LNScene_TransitionEffectColor $398
#cmd LNScene_SetTransitionEffectMaskTexture $399
#cmd LNScene_TransitionEffectMaskTexture $39B
#cmd LNScene_SetTransitionEffectVague $39C
#cmd LNScene_TransitionEffectVague $39E
#cmd LNScene_StartFadeOut $39F
#cmd LNScene_StartFadeIn $3A0
#cmd LNScene_SetFogStartDistance $3A1
#cmd LNScene_SetFogColor $3A3
#cmd LNScene_SetFogDensity $3A5
#cmd LNScene_SetFogHeightDensity $3A7
#cmd LNScene_SetFogLowerHeight $3A9
#cmd LNScene_SetFogUpperHeight $3AB
#cmd LNScene_SetHDREnabled $3AD
#cmd LNScene_IsHDREnabled $3AF
#cmd LNScene_SetScreenBlendColor $3B0
#cmd LNScene_ScreenBlendColor $3B2
#cmd LNScene_SetColorTone $3B3
#cmd LNScene_ColorTone $3B5
#cmd LNScene_SetAntialiasEnabled $3B6
#cmd LNScene_IsAntialiasEnabled $3B8
#cmd LNScene_SetSSREnabled $3B9
#cmd LNScene_IsSSREnabled $3BB
#cmd LNScene_SetSSAOEnabled $3BC
#cmd LNScene_IsSSAOEnabled $3BE
#cmd LNScene_SetBloomEnabled $3BF
#cmd LNScene_IsBloomEnabled $3C1
#cmd LNScene_SetDOFEnabled $3C2
#cmd LNScene_IsDOFEnabled $3C4
#cmd LNScene_SetTonemapEnabled $3C5
#cmd LNScene_IsTonemapEnabled $3C7
#cmd LNScene_SetVignetteEnabled $3C8
#cmd LNScene_IsVignetteEnabled $3CA
#cmd LNScene_SetGammaEnabled $3CB
#cmd LNScene_IsGammaEnabled $3CD
#cmd LNScene_SetTonemapExposure $3CE
#cmd LNScene_SetTonemapLinearWhite $3D0
#cmd LNScene_SetTonemapShoulderStrength $3D2
#cmd LNScene_SetTonemapLinearStrength $3D4
#cmd LNScene_SetTonemapLinearAngle $3D6
#cmd LNScene_SetTonemapToeStrength $3D8
#cmd LNScene_SetTonemapToeNumerator $3DA
#cmd LNScene_SetTonemapToeDenominator $3DC
#cmd LNLevel_Create $3ED
#cmd LNLevel_AddObject $3DF
#cmd LNLevel_RemoveObject $3E1
#cmd LNLevel_AddSubLevel $3E3
#cmd LNLevel_RemoveSubLevel $3E5
#cmd LNLevel_RemoveAllSubLevels $3E7
#cmd LNLevel_SetPrototype_OnSerialize $D06
#cmd LNLevel_SetPrototype_OnStart $D11
#cmd LNLevel_SetPrototype_OnStop $D1C
#cmd LNLevel_SetPrototype_OnPause $D27
#cmd LNLevel_SetPrototype_OnResume $D32
#cmd LNLevel_SetPrototype_OnUpdate $D3D
#cmd LNUIEventArgs_Sender $3F8
#cmd LNUIEventArgs_SetPrototype_OnSerialize $D48
#cmd LNUIGeneralEventHandler_Create $8B3
#cmd LNUIEventHandler_Create $8B8
#cmd LNUILayoutElement_SetPrototype_OnSerialize $D53
#cmd LNUIElement_SetSize $40A
#cmd LNUIElement_SetSizeWH $40C
#cmd LNUIElement_SetWidth $40F
#cmd LNUIElement_GetWidth $411
#cmd LNUIElement_SetHeight $412
#cmd LNUIElement_GetHeight $414
#cmd LNUIElement_SetMargin $415
#cmd LNUIElement_GetMargin $417
#cmd LNUIElement_SetPadding $418
#cmd LNUIElement_GetPadding $41A
#cmd LNUIElement_SetHAlignment $41B
#cmd LNUIElement_GetHAlignment $41D
#cmd LNUIElement_SetVAlignment $41E
#cmd LNUIElement_GetVAlignment $420
#cmd LNUIElement_SetAlignments $421
#cmd LNUIElement_SetPosition $424
#cmd LNUIElement_SetPositionXYZ $426
#cmd LNUIElement_GetPosition $42A
#cmd LNUIElement_SetRotation $42B
#cmd LNUIElement_SetEulerAngles $42D
#cmd LNUIElement_GetRotation $431
#cmd LNUIElement_SetScale $432
#cmd LNUIElement_SetScaleS $434
#cmd LNUIElement_SetScaleXY $436
#cmd LNUIElement_GetScale $439
#cmd LNUIElement_SetCenterPoint $43A
#cmd LNUIElement_SetCenterPointXYZ $43C
#cmd LNUIElement_GetCenterPoint $440
#cmd LNUIElement_SetEnabled $441
#cmd LNUIElement_IsEnabled $443
#cmd LNUIElement_SetData $444
#cmd LNUIElement_GetData $446
#cmd LNUIElement_SetBackgroundColor $447
#cmd LNUIElement_GetBackgroundColor $449
#cmd LNUIElement_SetBorderThickness $44A
#cmd LNUIElement_GetBorderThickness $44C
#cmd LNUIElement_SetBorderColor $44D
#cmd LNUIElement_GetBorderColor $44F
#cmd LNUIElement_SetCornerRadius $450
#cmd LNUIElement_GetCornerRadius $452
#cmd LNUIElement_SetVisibility $453
#cmd LNUIElement_GetVisibility $455
#cmd LNUIElement_SetOpacity $456
#cmd LNUIElement_GetOpacity $458
#cmd LNUIElement_AddChild $459
#cmd LNUIElement_SetFocusable $45B
#cmd LNUIElement_GetFocusable $45D
#cmd LNUIElement_SetPrototype_OnSerialize $D5E
#cmd LNUITextBlock_Create $462
#cmd LNUITextBlock_CreateWithText $463
#cmd LNUITextBlock_SetText $45F
#cmd LNUITextBlock_GetText $461
#cmd LNUITextBlock_SetPrototype_OnSerialize $D69
#cmd LNUISprite_Create $472
#cmd LNUISprite_CreateWithTexture $473
#cmd LNUISprite_SetTexture $466
#cmd LNUISprite_SetSourceRect $468
#cmd LNUISprite_SetSourceRectXYWH $46A
#cmd LNUISprite_GetSourceRect $46F
#cmd LNUISprite_SetShader $470
#cmd LNUISprite_SetPrototype_OnSerialize $D74
#cmd LNUIIcon_LoadFontIcon $476
#cmd LNUIIcon_LoadFontIconWithNameSize $478
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $47B
#cmd LNUIIcon_SetPrototype_OnSerialize $D7F
#cmd LNUIMessageTextArea_Create $484
#cmd LNUIMessageTextArea_SetText $480
#cmd LNUIMessageTextArea_SetTypingSpeed $482
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $D8A
#cmd LNUI_Add $486
#cmd LNUI_Remove $488
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $D95
#cmd LNUIBoxLayout_Create $48F
#cmd LNUIBoxLayout_SetOrientation $48C
#cmd LNUIBoxLayout_GetOrientation $48E
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $DA0
#cmd LNUIStackLayout_Create $494
#cmd LNUIStackLayout_SetOrientation $491
#cmd LNUIStackLayout_GetOrientation $493
#cmd LNUIStackLayout_SetPrototype_OnSerialize $DAB
#cmd LNUIGridLayout_Create $4A4
#cmd LNUIGridLayout_SetColumnCount $496
#cmd LNUIGridLayout_SetRow $498
#cmd LNUIGridLayout_SetColumn $49B
#cmd LNUIGridLayout_SetPlacement $49E
#cmd LNUIGridLayout_SetPrototype_OnSerialize $DB6
#cmd LNUIControl_Create $4B0
#cmd LNUIControl_AddInlineVisual $4B1
#cmd LNUIControl_SetPrototype_OnSerialize $DC1
#cmd LNUIButtonBase_SetText $4B5
#cmd LNUIButtonBase_SetPrototype_OnSerialize $DCC
#cmd LNUIButton_Create $4B8
#cmd LNUIButton_CreateWithText $4B9
#cmd LNUIButton_ConnectOnClicked $4BB
#cmd LNUIButton_SetPrototype_OnSerialize $DD7
#cmd LNUIWindow_Create $4BE
#cmd LNUIWindow_SetPrototype_OnSerialize $DE2
#cmd LNUIListItem_ConnectOnSubmit $4C3
#cmd LNUIListItem_SetPrototype_OnSerialize $DED
#cmd LNUIListItemsControl_SetItemsLayoutPanel $4C6
#cmd LNUIListItemsControl_SetSubmitMode $4C8
#cmd LNUIListItemsControl_GetSubmitMode $4CA
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $DF8
#cmd LNUIListBoxItem_Create $4CC
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $E03
#cmd LNUIListBox_Create $4D1
#cmd LNUIListBox_AddItem $4CF
#cmd LNUIListBox_SetPrototype_OnSerialize $E0E
#cmd LNInputGesture_SetPrototype_OnSerialize $E19
#cmd LNKeyGesture_Create $4D4
#cmd LNKeyGesture_SetPrototype_OnSerialize $E24
#cmd LNInput_IsPressed $4D7
#cmd LNInput_IsTriggered $4D9
#cmd LNInput_IsTriggeredOff $4DB
#cmd LNInput_IsRepeated $4DD
#cmd LNInput_GetAxisValue $4DF
#cmd LNInput_AddBinding $4E1
#cmd LNInput_RemoveBinding $4E4
#cmd LNInput_ClearBindings $4E6
#cmd LNInput_ClearAllBindings $4E8
#cmd LNMouse_Pressed $4EA
#cmd LNMouse_Triggered $4EC
#cmd LNMouse_TriggeredOff $4EE
#cmd LNMouse_Repeated $4F0
#cmd LNMouse_Position $4F2
#cmd LNInterpreterCommand_Code $4F4
#cmd LNInterpreterCommand_ParamsCount $4F5
#cmd LNInterpreterCommand_Param $4F6
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $E2F
#cmd LNInterpreterCommandList_Create $50D
#cmd LNInterpreterCommandList_AddCommand $4F9
#cmd LNInterpreterCommandList_AddCommand1 $4FB
#cmd LNInterpreterCommandList_AddCommand2 $4FE
#cmd LNInterpreterCommandList_AddCommand3 $502
#cmd LNInterpreterCommandList_AddCommand4 $507
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $E3A
#cmd LNInterpreterCommandDelegate_Create $97E
#cmd LNInterpreter_Create $521
#cmd LNInterpreter_Clear $512
#cmd LNInterpreter_Run $513
#cmd LNInterpreter_IsRunning $515
#cmd LNInterpreter_Update $516
#cmd LNInterpreter_Terminate $517
#cmd LNInterpreter_RegisterCommandHandler $518
#cmd LNInterpreter_SetWaitMode $51B
#cmd LNInterpreter_GetWaitMode $51D
#cmd LNInterpreter_SetWaitCount $51E
#cmd LNInterpreter_GetWaitCount $520
#cmd LNInterpreter_SetPrototype_OnSerialize $E45
#cmd LNInterpreter_SetPrototype_OnUpdateWait $E51
#cmd LNEngineSettings_SetMainWindowSize $524
#cmd LNEngineSettings_SetMainWorldViewSize $527
#cmd LNEngineSettings_SetMainWindowTitle $52A
#cmd LNEngineSettings_SetMainWindowResizable $52C
#cmd LNEngineSettings_AddAssetDirectory $52E
#cmd LNEngineSettings_AddAssetArchive $530
#cmd LNEngineSettings_SetFrameRate $533
#cmd LNEngineSettings_SetUITheme $535
#cmd LNEngineSettings_SetFontFile $537
#cmd LNEngineSettings_SetDebugToolEnabled $539
#cmd LNEngineSettings_SetEngineLogEnabled $53B
#cmd LNEngineSettings_SetEngineLogFilePath $53D
#cmd LNEngineSettings_SetDeveloperToolEnabled $53F
#cmd LNEngine_Initialize $542
#cmd LNEngine_Finalize $543
#cmd LNEngine_Update $544
#cmd LNEngine_Run $545
#cmd LNEngine_Time $547
#cmd LNEngine_GetCamera $548
#cmd LNEngine_GetMainLight $549
#cmd LNEngine_GetRenderView $54A
#cmd LNApplication_Create $54F
#cmd LNApplication_OnInit $54C
#cmd LNApplication_OnUpdate $54D
#cmd LNApplication_World $54E
#cmd LNApplication_SetPrototype_OnSerialize $E5C
#cmd LNApplication_SetPrototype_OnInit $E67
#cmd LNApplication_SetPrototype_OnUpdate $E72
#cmd LNDebug_SetGuideGridEnabled $551
#cmd LNDebug_SetPhysicsDebugDrawEnabled $553
#cmd LNDebug_Print $555
#cmd LNDebug_PrintWithTime $557
#cmd LNDebug_PrintWithTimeAndColor $55A
#cmd LNObjectSerializeHandler_Create $9AA
#cmd LNEventConnectionSerializeHandler_Create $9B5
#cmd LNVariantSerializeHandler_Create $9C0
#cmd LNZVTestClass1SerializeHandler_Create $9CB
#cmd LNZVTestEventArgs1SerializeHandler_Create $9D6
#cmd LNSerializer2SerializeHandler_Create $9E1
#cmd LNAssetObjectSerializeHandler_Create $9EC
#cmd LNAssetImportSettingsSerializeHandler_Create $9F7
#cmd LNAssetModelSerializeHandler_Create $A02
#cmd LNSoundSerializeHandler_Create $A0D
#cmd LNTextureSerializeHandler_Create $A18
#cmd LNTexture2DSerializeHandler_Create $A23
#cmd LNShaderSerializeHandler_Create $A2E
#cmd LNRenderViewSerializeHandler_Create $A39
#cmd LNMaterialSerializeHandler_Create $A44
#cmd LNMeshNodeSerializeHandler_Create $A4F
#cmd LNAnimationControllerSerializeHandler_Create $A5A
#cmd LNMeshModelSerializeHandler_Create $A65
#cmd LNSkinnedMeshModelSerializeHandler_Create $A70
#cmd LNCollisionShapeSerializeHandler_Create $A7B
#cmd LNBoxCollisionShapeSerializeHandler_Create $A86
#cmd LNAnimationCurveSerializeHandler_Create $A91
#cmd LNKeyFrameAnimationCurveSerializeHandler_Create $A9C
#cmd LNAnimationClipSerializeHandler_Create $AA7
#cmd LNAnimationStateSerializeHandler_Create $AB2
#cmd LNEffectResourceSerializeHandler_Create $ABD
#cmd LNParticleEmitterModelSerializeHandler_Create $AC8
#cmd LNParticleModelSerializeHandler_Create $AD3
#cmd LNComponentSerializeHandler_Create $ADE
#cmd LNVisualComponentSerializeHandler_Create $AE9
#cmd LNSpriteComponentSerializeHandler_Create $AF4
#cmd LNCharacterControllerSerializeHandler_Create $AFF
#cmd LNWorldSerializeHandler_Create $B0A
#cmd LNComponentListSerializeHandler_Create $B15
#cmd LNWorldObjectSerializeHandler_Create $B20
#cmd LNWorldObjectPreUpdateHandler_Create $B2B
#cmd LNWorldObjectUpdateHandler_Create $B36
#cmd LNVisualObjectSerializeHandler_Create $B41
#cmd LNVisualObjectPreUpdateHandler_Create $B4C
#cmd LNVisualObjectUpdateHandler_Create $B57
#cmd LNCameraSerializeHandler_Create $B62
#cmd LNCameraPreUpdateHandler_Create $B6D
#cmd LNCameraUpdateHandler_Create $B78
#cmd LNEnvironmentLightSerializeHandler_Create $B83
#cmd LNEnvironmentLightPreUpdateHandler_Create $B8E
#cmd LNEnvironmentLightUpdateHandler_Create $B99
#cmd LNDirectionalLightSerializeHandler_Create $BA4
#cmd LNDirectionalLightPreUpdateHandler_Create $BAF
#cmd LNDirectionalLightUpdateHandler_Create $BBA
#cmd LNPointLightSerializeHandler_Create $BC5
#cmd LNPointLightPreUpdateHandler_Create $BD0
#cmd LNPointLightUpdateHandler_Create $BDB
#cmd LNSpotLightSerializeHandler_Create $BE6
#cmd LNSpotLightPreUpdateHandler_Create $BF1
#cmd LNSpotLightUpdateHandler_Create $BFC
#cmd LNSpriteSerializeHandler_Create $C07
#cmd LNSpritePreUpdateHandler_Create $C12
#cmd LNSpriteUpdateHandler_Create $C1D
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $C28
#cmd LNRaycasterSerializeHandler_Create $C33
#cmd LNRaycastResultSerializeHandler_Create $C3E
#cmd LNWorldRenderViewSerializeHandler_Create $C49
#cmd LNBoxMeshSerializeHandler_Create $C54
#cmd LNBoxMeshPreUpdateHandler_Create $C5F
#cmd LNBoxMeshUpdateHandler_Create $C6A
#cmd LNPlaneMeshSerializeHandler_Create $C75
#cmd LNPlaneMeshPreUpdateHandler_Create $C80
#cmd LNPlaneMeshUpdateHandler_Create $C8B
#cmd LNStaticMeshSerializeHandler_Create $C96
#cmd LNStaticMeshPreUpdateHandler_Create $CA1
#cmd LNStaticMeshUpdateHandler_Create $CAC
#cmd LNStaticMeshComponentSerializeHandler_Create $CB7
#cmd LNSkinnedMeshComponentSerializeHandler_Create $CC2
#cmd LNCollisionSerializeHandler_Create $CCD
#cmd LNTriggerBodyComponentSerializeHandler_Create $CD8
#cmd LNParticleEmitterSerializeHandler_Create $CE3
#cmd LNParticleEmitterPreUpdateHandler_Create $CEE
#cmd LNParticleEmitterUpdateHandler_Create $CF9
#cmd LNLevelSerializeHandler_Create $D04
#cmd LNLevelStartHandler_Create $D0F
#cmd LNLevelStopHandler_Create $D1A
#cmd LNLevelPauseHandler_Create $D25
#cmd LNLevelResumeHandler_Create $D30
#cmd LNLevelUpdateHandler_Create $D3B
#cmd LNUIEventArgsSerializeHandler_Create $D46
#cmd LNUILayoutElementSerializeHandler_Create $D51
#cmd LNUIElementSerializeHandler_Create $D5C
#cmd LNUITextBlockSerializeHandler_Create $D67
#cmd LNUISpriteSerializeHandler_Create $D72
#cmd LNUIIconSerializeHandler_Create $D7D
#cmd LNUIMessageTextAreaSerializeHandler_Create $D88
#cmd LNUILayoutPanelSerializeHandler_Create $D93
#cmd LNUIBoxLayoutSerializeHandler_Create $D9E
#cmd LNUIStackLayoutSerializeHandler_Create $DA9
#cmd LNUIGridLayoutSerializeHandler_Create $DB4
#cmd LNUIControlSerializeHandler_Create $DBF
#cmd LNUIButtonBaseSerializeHandler_Create $DCA
#cmd LNUIButtonSerializeHandler_Create $DD5
#cmd LNUIWindowSerializeHandler_Create $DE0
#cmd LNUIListItemSerializeHandler_Create $DEB
#cmd LNUIListItemsControlSerializeHandler_Create $DF6
#cmd LNUIListBoxItemSerializeHandler_Create $E01
#cmd LNUIListBoxSerializeHandler_Create $E0C
#cmd LNInputGestureSerializeHandler_Create $E17
#cmd LNKeyGestureSerializeHandler_Create $E22
#cmd LNInterpreterCommandSerializeHandler_Create $E2D
#cmd LNInterpreterCommandListSerializeHandler_Create $E38
#cmd LNInterpreterSerializeHandler_Create $E43
#cmd LNInterpreterUpdateWaitHandler_Create $E4F
#cmd LNApplicationSerializeHandler_Create $E5A
#cmd LNApplicationInitHandler_Create $E65
#cmd LNApplicationUpdateHandler_Create $E70



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
