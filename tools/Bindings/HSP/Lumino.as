
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
#cmd LNVector3_GetX $559
#cmd LNVector3_SetX $55B
#cmd LNVector3_GetY $55C
#cmd LNVector3_SetY $55E
#cmd LNVector3_GetZ $55F
#cmd LNVector3_SetZ $561
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $578
#cmd LNVector4_SetX $57A
#cmd LNVector4_GetY $57B
#cmd LNVector4_SetY $57D
#cmd LNVector4_GetZ $57E
#cmd LNVector4_SetZ $580
#cmd LNVector4_GetW $581
#cmd LNVector4_SetW $583
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $596
#cmd LNQuaternion_SetX $598
#cmd LNQuaternion_GetY $599
#cmd LNQuaternion_SetY $59B
#cmd LNQuaternion_GetZ $59C
#cmd LNQuaternion_SetZ $59E
#cmd LNQuaternion_GetW $59F
#cmd LNQuaternion_SetW $5A1
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $5B5
#cmd LNMatrix_SetRow0 $5B7
#cmd LNMatrix_GetRow1 $5B8
#cmd LNMatrix_SetRow1 $5BA
#cmd LNMatrix_GetRow2 $5BB
#cmd LNMatrix_SetRow2 $5BD
#cmd LNMatrix_GetRow3 $5BE
#cmd LNMatrix_SetRow3 $5C0
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $179
#cmd LNColor_GetR $654
#cmd LNColor_SetR $656
#cmd LNColor_GetG $657
#cmd LNColor_SetG $659
#cmd LNColor_GetB $65A
#cmd LNColor_SetB $65C
#cmd LNColor_GetA $65D
#cmd LNColor_SetA $65F
#cmd LNColor_SetZeros $17E
#cmd LNColor_Set $17F
#cmd LNColorTone $184
#cmd LNColorTone_GetR $673
#cmd LNColorTone_SetR $675
#cmd LNColorTone_GetG $676
#cmd LNColorTone_SetG $678
#cmd LNColorTone_GetB $679
#cmd LNColorTone_SetB $67B
#cmd LNColorTone_GetS $67C
#cmd LNColorTone_SetS $67E
#cmd LNColorTone_SetZeros $189
#cmd LNColorTone_Set $18A
#cmd LNPoint $18F
#cmd LNPoint_GetX $691
#cmd LNPoint_SetX $693
#cmd LNPoint_GetY $694
#cmd LNPoint_SetY $696
#cmd LNPoint_SetZeros $192
#cmd LNPoint_Set $193
#cmd LNSize $196
#cmd LNSize_GetWidth $6A1
#cmd LNSize_SetWidth $6A3
#cmd LNSize_GetHeight $6A4
#cmd LNSize_SetHeight $6A6
#cmd LNSize_SetZeros $199
#cmd LNSize_Set $19A
#cmd LNRect $19D
#cmd LNRect_GetX $6B1
#cmd LNRect_SetX $6B3
#cmd LNRect_GetY $6B4
#cmd LNRect_SetY $6B6
#cmd LNRect_GetWidth $6B7
#cmd LNRect_SetWidth $6B9
#cmd LNRect_GetHeight $6BA
#cmd LNRect_SetHeight $6BC
#cmd LNRect_SetZeros $1A2
#cmd LNRect_Set $1A3
#cmd LNRect_GetLeft $1A8
#cmd LNRect_SetSize $1A9
#cmd LNRect_GetSize $1AB
#cmd LNThickness $1AC
#cmd LNThickness_GetLeft $6D5
#cmd LNThickness_SetLeft $6D7
#cmd LNThickness_GetTop $6D8
#cmd LNThickness_SetTop $6DA
#cmd LNThickness_GetRight $6DB
#cmd LNThickness_SetRight $6DD
#cmd LNThickness_GetBottom $6DE
#cmd LNThickness_SetBottom $6E0
#cmd LNThickness_SetZeros $1B1
#cmd LNThickness_Set $1B2
#cmd LNCornerRadius $1B7
#cmd LNCornerRadius_GetTopleft $6F3
#cmd LNCornerRadius_SetTopleft $6F5
#cmd LNCornerRadius_GetTopright $6F6
#cmd LNCornerRadius_SetTopright $6F8
#cmd LNCornerRadius_GetBottomright $6F9
#cmd LNCornerRadius_SetBottomright $6FB
#cmd LNCornerRadius_GetBottomleft $6FC
#cmd LNCornerRadius_SetBottomleft $6FE
#cmd LNCornerRadius_SetZeros $1BC
#cmd LNCornerRadius_Set $1BD

#cmd LNObject_Release $7A
#cmd LNObject_Retain $7B
#cmd LNObject_GetReferenceCount $7C
#cmd LNObject_SetPrototype_OnSerialize $9A2
#cmd LNEventConnection_SetPrototype_OnSerialize $9AD
#cmd LNPromiseFailureDelegate_Create $5DB
#cmd LNVariant_Create $81
#cmd LNVariant_SetInt $82
#cmd LNVariant_GetInt $84
#cmd LNVariant_SetPrototype_OnSerialize $9B8
#cmd LNZVTestDelegate1_Create $5E4
#cmd LNZVTestDelegate2_Create $5EA
#cmd LNZVTestDelegate3_Create $5EF
#cmd LNZVTestEventHandler1_Create $5F4
#cmd LNZVTestEventHandler2_Create $5F9
#cmd LNZVTestPromise1_ThenWith $5FC
#cmd LNZVTestPromise1_CatchWith $5FE
#cmd LNZVTestPromise2_ThenWith $602
#cmd LNZVTestPromise2_CatchWith $604
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $9C3
#cmd LNZVTestEventArgs1_Create $B0
#cmd LNZVTestEventArgs1_CreateWithValue $B1
#cmd LNZVTestEventArgs1_GetValue $AF
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $9CE
#cmd LNSerializer2_SetPrototype_OnSerialize $9D9
#cmd LNAssetObject_SetPrototype_OnSerialize $9E4
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $9EF
#cmd LNAssetModel_Create $113
#cmd LNAssetModel_Target $112
#cmd LNAssetModel_SetPrototype_OnSerialize $9FA
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
#cmd LNSound_SetPrototype_OnSerialize $A05
#cmd LNAudio_PlayBGM $13F
#cmd LNAudio_StopBGM $144
#cmd LNAudio_PlayBGS $146
#cmd LNAudio_StopBGS $14B
#cmd LNAudio_PlayME $14D
#cmd LNAudio_StopME $151
#cmd LNAudio_PlaySE $152
#cmd LNAudio_PlaySE3D $156
#cmd LNAudio_StopSE $15C
#cmd LNTexture2DDelegate_Create $64A
#cmd LNTexture2DPromise_ThenWith $64D
#cmd LNTexture2DPromise_CatchWith $64F
#cmd LNGraphics_GetActiveGraphicsAPI $178
#cmd LNTexture_SetPrototype_OnSerialize $A10
#cmd LNTexture2D_Create $1C8
#cmd LNTexture2D_CreateWithFormat $1CB
#cmd LNTexture2D_Load $1C4
#cmd LNTexture2D_LoadEmoji $1C6
#cmd LNTexture2D_SetPrototype_OnSerialize $A1B
#cmd LNShader_Load $1D0
#cmd LNShader_SetFloat $1D3
#cmd LNShader_SetVector3 $1D6
#cmd LNShader_SetVector4 $1D9
#cmd LNShader_SetTexture $1DC
#cmd LNShader_SetPrototype_OnSerialize $A26
#cmd LNRenderView_SetPrototype_OnSerialize $A31
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
#cmd LNMaterial_SetPrototype_OnSerialize $A3C
#cmd LNMeshNode_SetVisible $203
#cmd LNMeshNode_IsVisible $205
#cmd LNMeshNode_SetPrototype_OnSerialize $A47
#cmd LNStaticMeshModel_Load $207
#cmd LNStaticMeshModel_FindNode $209
#cmd LNStaticMeshModel_FindMaterial $20B
#cmd LNStaticMeshModel_MaterialCount $20D
#cmd LNStaticMeshModel_Material $20E
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $A52
#cmd LNSkinnedMeshModel_Load $211
#cmd LNSkinnedMeshModel_GetAnimationController $213
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $A5D
#cmd LNAnimationController_AddClip $215
#cmd LNAnimationController_Play $217
#cmd LNAnimationController_SetPrototype_OnSerialize $A68
#cmd LNCollisionShape_SetPrototype_OnSerialize $A73
#cmd LNBoxCollisionShape_Create $21C
#cmd LNBoxCollisionShape_CreateWHD $21E
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $A7E
#cmd LNAnimationCurve_Evaluate $22C
#cmd LNAnimationCurve_SetPrototype_OnSerialize $A89
#cmd LNKeyFrameAnimationCurve_Create $239
#cmd LNKeyFrameAnimationCurve_AddKeyFrame $234
#cmd LNKeyFrameAnimationCurve_SetPrototype_OnSerialize $A94
#cmd LNAnimationClip_Load $23B
#cmd LNAnimationClip_SetWrapMode $23D
#cmd LNAnimationClip_GetWrapMode $23F
#cmd LNAnimationClip_SetHierarchicalAnimationMode $240
#cmd LNAnimationClip_GetHierarchicalAnimationMode $242
#cmd LNAnimationClip_SetPrototype_OnSerialize $A9F
#cmd LNAnimationState_SetPrototype_OnSerialize $AAA
#cmd LNEffectResource_SetPrototype_OnSerialize $AB5
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
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $AC0
#cmd LNParticleModel_Create $26E
#cmd LNParticleModel_SetLoop $269
#cmd LNParticleModel_IsLoop $26B
#cmd LNParticleModel_AddEmitter $26C
#cmd LNParticleModel_SetPrototype_OnSerialize $ACB
#cmd LNComponent_SetPrototype_OnSerialize $AD6
#cmd LNVisualComponent_SetVisible $271
#cmd LNVisualComponent_IsVisible $273
#cmd LNVisualComponent_SetPrototype_OnSerialize $AE1
#cmd LNSpriteComponent_SetTexture $275
#cmd LNSpriteComponent_SetPrototype_OnSerialize $AEC
#cmd LNCollisionEventHandler_Create $781
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
#cmd LNCharacterController_SetPrototype_OnSerialize $AF7
#cmd LNWorld_Add $297
#cmd LNWorld_SetPrototype_OnSerialize $B02
#cmd LNComponentList_GetLength $7A1
#cmd LNComponentList_GetItem $7A3
#cmd LNComponentList_SetPrototype_OnSerialize $B0D
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
#cmd LNWorldObject_SetPrototype_OnSerialize $B18
#cmd LNWorldObject_SetPrototype_OnPreUpdate $B23
#cmd LNWorldObject_SetPrototype_OnUpdate $B2E
#cmd LNVisualObject_SetVisible $2D0
#cmd LNVisualObject_IsVisible $2D2
#cmd LNVisualObject_SetBlendMode2 $2D3
#cmd LNVisualObject_SetOpacity $2D5
#cmd LNVisualObject_GetOpacity $2D7
#cmd LNVisualObject_SetPrototype_OnSerialize $B39
#cmd LNVisualObject_SetPrototype_OnPreUpdate $B44
#cmd LNVisualObject_SetPrototype_OnUpdate $B4F
#cmd LNCamera_SetPrototype_OnSerialize $B5A
#cmd LNCamera_SetPrototype_OnPreUpdate $B65
#cmd LNCamera_SetPrototype_OnUpdate $B70
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
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $B7B
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $B86
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $B91
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
#cmd LNDirectionalLight_SetPrototype_OnSerialize $B9C
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $BA7
#cmd LNDirectionalLight_SetPrototype_OnUpdate $BB2
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
#cmd LNPointLight_SetPrototype_OnSerialize $BBD
#cmd LNPointLight_SetPrototype_OnPreUpdate $BC8
#cmd LNPointLight_SetPrototype_OnUpdate $BD3
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
#cmd LNSpotLight_SetPrototype_OnSerialize $BDE
#cmd LNSpotLight_SetPrototype_OnPreUpdate $BE9
#cmd LNSpotLight_SetPrototype_OnUpdate $BF4
#cmd LNTestDelegate_Create $84F
#cmd LNSprite_Create $346
#cmd LNSprite_CreateWithTexture $347
#cmd LNSprite_CreateWithTextureAndSize $349
#cmd LNSprite_SetTexture $338
#cmd LNSprite_SetSize $33A
#cmd LNSprite_SetSizeWH $33C
#cmd LNSprite_SetSourceRectXYWH $33F
#cmd LNSprite_SetCallerTest $344
#cmd LNSprite_SetPrototype_OnSerialize $BFF
#cmd LNSprite_SetPrototype_OnPreUpdate $C0A
#cmd LNSprite_SetPrototype_OnUpdate $C15
#cmd LNCameraOrbitControlComponent_Create $34E
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $C20
#cmd LNRaycaster_FromScreen $350
#cmd LNRaycaster_IntersectPlane $352
#cmd LNRaycaster_SetPrototype_OnSerialize $C2B
#cmd LNRaycastResult_GetPoint $357
#cmd LNRaycastResult_SetPrototype_OnSerialize $C36
#cmd LNWorldRenderView_SetGuideGridEnabled $359
#cmd LNWorldRenderView_GetGuideGridEnabled $35B
#cmd LNWorldRenderView_SetPrototype_OnSerialize $C41
#cmd LNBoxMesh_Create $35D
#cmd LNBoxMesh_SetPrototype_OnSerialize $C4C
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $C57
#cmd LNBoxMesh_SetPrototype_OnUpdate $C62
#cmd LNPlaneMesh_Create $35F
#cmd LNPlaneMesh_SetPrototype_OnSerialize $C6D
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $C78
#cmd LNPlaneMesh_SetPrototype_OnUpdate $C83
#cmd LNStaticMesh_Load $361
#cmd LNStaticMesh_GetModel $363
#cmd LNStaticMesh_MakeCollisionBody $364
#cmd LNStaticMesh_SetPrototype_OnSerialize $C8E
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $C99
#cmd LNStaticMesh_SetPrototype_OnUpdate $CA4
#cmd LNStaticMeshComponent_Create $36B
#cmd LNStaticMeshComponent_SetModel $367
#cmd LNStaticMeshComponent_MakeCollisionBody $369
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $CAF
#cmd LNSkinnedMeshComponent_Create $36F
#cmd LNSkinnedMeshComponent_SetModel $36D
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $CBA
#cmd LNCollision_GetWorldObject $371
#cmd LNCollision_SetPrototype_OnSerialize $CC5
#cmd LNTriggerBodyComponent_Create $375
#cmd LNTriggerBodyComponent_AddCollisionShape $373
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $CD0
#cmd LNParticleEmitter_Create $377
#cmd LNParticleEmitter_SetPrototype_OnSerialize $CDB
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $CE6
#cmd LNParticleEmitter_SetPrototype_OnUpdate $CF1
#cmd LNScene_SetClearMode $37A
#cmd LNScene_SetSkyColor $37C
#cmd LNScene_SetSkyHorizonColor $37E
#cmd LNScene_SetSkyCloudColor $380
#cmd LNScene_SetSkyOverlayColor $382
#cmd LNScene_GotoLevel $384
#cmd LNScene_CallLevel $387
#cmd LNScene_ReturnLevel $38A
#cmd LNScene_ActiveLevel $38C
#cmd LNScene_IsTransitionEffectRunning $38D
#cmd LNScene_SetTransitionEffectMode $38E
#cmd LNScene_TransitionEffectMode $390
#cmd LNScene_SetTransitionDuration $391
#cmd LNScene_TransitionDuration $393
#cmd LNScene_SetTransitionEffectColor $394
#cmd LNScene_TransitionEffectColor $396
#cmd LNScene_SetTransitionEffectMaskTexture $397
#cmd LNScene_TransitionEffectMaskTexture $399
#cmd LNScene_SetTransitionEffectVague $39A
#cmd LNScene_TransitionEffectVague $39C
#cmd LNScene_StartFadeOut $39D
#cmd LNScene_StartFadeIn $39E
#cmd LNScene_SetFogStartDistance $39F
#cmd LNScene_SetFogColor $3A1
#cmd LNScene_SetFogDensity $3A3
#cmd LNScene_SetFogHeightDensity $3A5
#cmd LNScene_SetFogLowerHeight $3A7
#cmd LNScene_SetFogUpperHeight $3A9
#cmd LNScene_SetHDREnabled $3AB
#cmd LNScene_IsHDREnabled $3AD
#cmd LNScene_SetScreenBlendColor $3AE
#cmd LNScene_ScreenBlendColor $3B0
#cmd LNScene_SetColorTone $3B1
#cmd LNScene_ColorTone $3B3
#cmd LNScene_SetAntialiasEnabled $3B4
#cmd LNScene_IsAntialiasEnabled $3B6
#cmd LNScene_SetSSREnabled $3B7
#cmd LNScene_IsSSREnabled $3B9
#cmd LNScene_SetSSAOEnabled $3BA
#cmd LNScene_IsSSAOEnabled $3BC
#cmd LNScene_SetBloomEnabled $3BD
#cmd LNScene_IsBloomEnabled $3BF
#cmd LNScene_SetDOFEnabled $3C0
#cmd LNScene_IsDOFEnabled $3C2
#cmd LNScene_SetTonemapEnabled $3C3
#cmd LNScene_IsTonemapEnabled $3C5
#cmd LNScene_SetVignetteEnabled $3C6
#cmd LNScene_IsVignetteEnabled $3C8
#cmd LNScene_SetGammaEnabled $3C9
#cmd LNScene_IsGammaEnabled $3CB
#cmd LNScene_SetTonemapExposure $3CC
#cmd LNScene_SetTonemapLinearWhite $3CE
#cmd LNScene_SetTonemapShoulderStrength $3D0
#cmd LNScene_SetTonemapLinearStrength $3D2
#cmd LNScene_SetTonemapLinearAngle $3D4
#cmd LNScene_SetTonemapToeStrength $3D6
#cmd LNScene_SetTonemapToeNumerator $3D8
#cmd LNScene_SetTonemapToeDenominator $3DA
#cmd LNLevel_Create $3EB
#cmd LNLevel_AddObject $3DD
#cmd LNLevel_RemoveObject $3DF
#cmd LNLevel_AddSubLevel $3E1
#cmd LNLevel_RemoveSubLevel $3E3
#cmd LNLevel_RemoveAllSubLevels $3E5
#cmd LNLevel_SetPrototype_OnSerialize $CFC
#cmd LNLevel_SetPrototype_OnStart $D07
#cmd LNLevel_SetPrototype_OnStop $D12
#cmd LNLevel_SetPrototype_OnPause $D1D
#cmd LNLevel_SetPrototype_OnResume $D28
#cmd LNLevel_SetPrototype_OnUpdate $D33
#cmd LNUIEventArgs_Sender $3F6
#cmd LNUIEventArgs_SetPrototype_OnSerialize $D3E
#cmd LNUIGeneralEventHandler_Create $8AE
#cmd LNUIEventHandler_Create $8B3
#cmd LNUILayoutElement_SetPrototype_OnSerialize $D49
#cmd LNUIElement_SetSize $408
#cmd LNUIElement_SetSizeWH $40A
#cmd LNUIElement_SetWidth $40D
#cmd LNUIElement_GetWidth $40F
#cmd LNUIElement_SetHeight $410
#cmd LNUIElement_GetHeight $412
#cmd LNUIElement_SetMargin $413
#cmd LNUIElement_GetMargin $415
#cmd LNUIElement_SetPadding $416
#cmd LNUIElement_GetPadding $418
#cmd LNUIElement_SetHAlignment $419
#cmd LNUIElement_GetHAlignment $41B
#cmd LNUIElement_SetVAlignment $41C
#cmd LNUIElement_GetVAlignment $41E
#cmd LNUIElement_SetAlignments $41F
#cmd LNUIElement_SetPosition $422
#cmd LNUIElement_SetPositionXYZ $424
#cmd LNUIElement_GetPosition $428
#cmd LNUIElement_SetRotation $429
#cmd LNUIElement_SetEulerAngles $42B
#cmd LNUIElement_GetRotation $42F
#cmd LNUIElement_SetScale $430
#cmd LNUIElement_SetScaleS $432
#cmd LNUIElement_SetScaleXY $434
#cmd LNUIElement_GetScale $437
#cmd LNUIElement_SetCenterPoint $438
#cmd LNUIElement_SetCenterPointXYZ $43A
#cmd LNUIElement_GetCenterPoint $43E
#cmd LNUIElement_SetEnabled $43F
#cmd LNUIElement_IsEnabled $441
#cmd LNUIElement_SetData $442
#cmd LNUIElement_GetData $444
#cmd LNUIElement_SetBackgroundColor $445
#cmd LNUIElement_GetBackgroundColor $447
#cmd LNUIElement_SetBorderThickness $448
#cmd LNUIElement_GetBorderThickness $44A
#cmd LNUIElement_SetBorderColor $44B
#cmd LNUIElement_GetBorderColor $44D
#cmd LNUIElement_SetCornerRadius $44E
#cmd LNUIElement_GetCornerRadius $450
#cmd LNUIElement_SetVisibility $451
#cmd LNUIElement_GetVisibility $453
#cmd LNUIElement_SetOpacity $454
#cmd LNUIElement_GetOpacity $456
#cmd LNUIElement_AddChild $457
#cmd LNUIElement_SetPrototype_OnSerialize $D54
#cmd LNUITextBlock_Create $45D
#cmd LNUITextBlock_CreateWithText $45E
#cmd LNUITextBlock_SetText $45A
#cmd LNUITextBlock_GetText $45C
#cmd LNUITextBlock_SetPrototype_OnSerialize $D5F
#cmd LNUISprite_Create $46D
#cmd LNUISprite_CreateWithTexture $46E
#cmd LNUISprite_SetTexture $461
#cmd LNUISprite_SetSourceRect $463
#cmd LNUISprite_SetSourceRectXYWH $465
#cmd LNUISprite_GetSourceRect $46A
#cmd LNUISprite_SetShader $46B
#cmd LNUISprite_SetPrototype_OnSerialize $D6A
#cmd LNUIIcon_LoadFontIcon $471
#cmd LNUIIcon_LoadFontIconWithNameSize $473
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $476
#cmd LNUIIcon_SetPrototype_OnSerialize $D75
#cmd LNUIMessageTextArea_Create $47F
#cmd LNUIMessageTextArea_SetText $47B
#cmd LNUIMessageTextArea_SetTypingSpeed $47D
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $D80
#cmd LNUI_Add $481
#cmd LNUI_Remove $483
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $D8B
#cmd LNUIBoxLayout_Create $48A
#cmd LNUIBoxLayout_SetOrientation $487
#cmd LNUIBoxLayout_GetOrientation $489
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $D96
#cmd LNUIStackLayout_Create $48F
#cmd LNUIStackLayout_SetOrientation $48C
#cmd LNUIStackLayout_GetOrientation $48E
#cmd LNUIStackLayout_SetPrototype_OnSerialize $DA1
#cmd LNUIGridLayout_Create $49F
#cmd LNUIGridLayout_SetColumnCount $491
#cmd LNUIGridLayout_SetRow $493
#cmd LNUIGridLayout_SetColumn $496
#cmd LNUIGridLayout_SetPlacement $499
#cmd LNUIGridLayout_SetPrototype_OnSerialize $DAC
#cmd LNUIControl_Create $4AB
#cmd LNUIControl_AddInlineVisual $4AC
#cmd LNUIControl_SetPrototype_OnSerialize $DB7
#cmd LNUIButtonBase_SetText $4B0
#cmd LNUIButtonBase_SetPrototype_OnSerialize $DC2
#cmd LNUIButton_Create $4B3
#cmd LNUIButton_CreateWithText $4B4
#cmd LNUIButton_ConnectOnClicked $4B6
#cmd LNUIButton_SetPrototype_OnSerialize $DCD
#cmd LNUIWindow_Create $4B9
#cmd LNUIWindow_SetPrototype_OnSerialize $DD8
#cmd LNUIListItem_ConnectOnSubmit $4BE
#cmd LNUIListItem_SetPrototype_OnSerialize $DE3
#cmd LNUIListItemsControl_SetItemsLayoutPanel $4C1
#cmd LNUIListItemsControl_SetSubmitMode $4C3
#cmd LNUIListItemsControl_GetSubmitMode $4C5
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $DEE
#cmd LNUIListBoxItem_Create $4C7
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $DF9
#cmd LNUIListBox_Create $4CC
#cmd LNUIListBox_AddItem $4CA
#cmd LNUIListBox_SetPrototype_OnSerialize $E04
#cmd LNInputGesture_SetPrototype_OnSerialize $E0F
#cmd LNKeyGesture_Create $4CF
#cmd LNKeyGesture_SetPrototype_OnSerialize $E1A
#cmd LNInput_IsPressed $4D2
#cmd LNInput_IsTriggered $4D4
#cmd LNInput_IsTriggeredOff $4D6
#cmd LNInput_IsRepeated $4D8
#cmd LNInput_GetAxisValue $4DA
#cmd LNInput_AddBinding $4DC
#cmd LNInput_RemoveBinding $4DF
#cmd LNInput_ClearBindings $4E1
#cmd LNInput_ClearAllBindings $4E3
#cmd LNMouse_Pressed $4E5
#cmd LNMouse_Triggered $4E7
#cmd LNMouse_TriggeredOff $4E9
#cmd LNMouse_Repeated $4EB
#cmd LNMouse_Position $4ED
#cmd LNInterpreterCommand_Code $4EF
#cmd LNInterpreterCommand_ParamsCount $4F0
#cmd LNInterpreterCommand_Param $4F1
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $E25
#cmd LNInterpreterCommandList_Create $508
#cmd LNInterpreterCommandList_AddCommand $4F4
#cmd LNInterpreterCommandList_AddCommand1 $4F6
#cmd LNInterpreterCommandList_AddCommand2 $4F9
#cmd LNInterpreterCommandList_AddCommand3 $4FD
#cmd LNInterpreterCommandList_AddCommand4 $502
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $E30
#cmd LNInterpreterCommandDelegate_Create $974
#cmd LNInterpreter_Create $51C
#cmd LNInterpreter_Clear $50D
#cmd LNInterpreter_Run $50E
#cmd LNInterpreter_IsRunning $510
#cmd LNInterpreter_Update $511
#cmd LNInterpreter_Terminate $512
#cmd LNInterpreter_RegisterCommandHandler $513
#cmd LNInterpreter_SetWaitMode $516
#cmd LNInterpreter_GetWaitMode $518
#cmd LNInterpreter_SetWaitCount $519
#cmd LNInterpreter_GetWaitCount $51B
#cmd LNInterpreter_SetPrototype_OnSerialize $E3B
#cmd LNInterpreter_SetPrototype_OnUpdateWait $E47
#cmd LNEngineSettings_SetMainWindowSize $51F
#cmd LNEngineSettings_SetMainWorldViewSize $522
#cmd LNEngineSettings_SetMainWindowTitle $525
#cmd LNEngineSettings_SetMainWindowResizable $527
#cmd LNEngineSettings_AddAssetDirectory $529
#cmd LNEngineSettings_AddAssetArchive $52B
#cmd LNEngineSettings_SetFrameRate $52E
#cmd LNEngineSettings_SetUITheme $530
#cmd LNEngineSettings_SetFontFile $532
#cmd LNEngineSettings_SetDebugToolEnabled $534
#cmd LNEngineSettings_SetEngineLogEnabled $536
#cmd LNEngineSettings_SetEngineLogFilePath $538
#cmd LNEngineSettings_SetDeveloperToolEnabled $53A
#cmd LNEngine_Initialize $53D
#cmd LNEngine_Finalize $53E
#cmd LNEngine_Update $53F
#cmd LNEngine_Run $540
#cmd LNEngine_Time $542
#cmd LNEngine_GetCamera $543
#cmd LNEngine_GetMainLight $544
#cmd LNEngine_GetRenderView $545
#cmd LNApplication_Create $54A
#cmd LNApplication_OnInit $547
#cmd LNApplication_OnUpdate $548
#cmd LNApplication_World $549
#cmd LNApplication_SetPrototype_OnSerialize $E52
#cmd LNApplication_SetPrototype_OnInit $E5D
#cmd LNApplication_SetPrototype_OnUpdate $E68
#cmd LNDebug_SetGuideGridEnabled $54C
#cmd LNDebug_SetPhysicsDebugDrawEnabled $54E
#cmd LNDebug_Print $550
#cmd LNDebug_PrintWithTime $552
#cmd LNDebug_PrintWithTimeAndColor $555
#cmd LNObjectSerializeHandler_Create $9A0
#cmd LNEventConnectionSerializeHandler_Create $9AB
#cmd LNVariantSerializeHandler_Create $9B6
#cmd LNZVTestClass1SerializeHandler_Create $9C1
#cmd LNZVTestEventArgs1SerializeHandler_Create $9CC
#cmd LNSerializer2SerializeHandler_Create $9D7
#cmd LNAssetObjectSerializeHandler_Create $9E2
#cmd LNAssetImportSettingsSerializeHandler_Create $9ED
#cmd LNAssetModelSerializeHandler_Create $9F8
#cmd LNSoundSerializeHandler_Create $A03
#cmd LNTextureSerializeHandler_Create $A0E
#cmd LNTexture2DSerializeHandler_Create $A19
#cmd LNShaderSerializeHandler_Create $A24
#cmd LNRenderViewSerializeHandler_Create $A2F
#cmd LNMaterialSerializeHandler_Create $A3A
#cmd LNMeshNodeSerializeHandler_Create $A45
#cmd LNStaticMeshModelSerializeHandler_Create $A50
#cmd LNSkinnedMeshModelSerializeHandler_Create $A5B
#cmd LNAnimationControllerSerializeHandler_Create $A66
#cmd LNCollisionShapeSerializeHandler_Create $A71
#cmd LNBoxCollisionShapeSerializeHandler_Create $A7C
#cmd LNAnimationCurveSerializeHandler_Create $A87
#cmd LNKeyFrameAnimationCurveSerializeHandler_Create $A92
#cmd LNAnimationClipSerializeHandler_Create $A9D
#cmd LNAnimationStateSerializeHandler_Create $AA8
#cmd LNEffectResourceSerializeHandler_Create $AB3
#cmd LNParticleEmitterModelSerializeHandler_Create $ABE
#cmd LNParticleModelSerializeHandler_Create $AC9
#cmd LNComponentSerializeHandler_Create $AD4
#cmd LNVisualComponentSerializeHandler_Create $ADF
#cmd LNSpriteComponentSerializeHandler_Create $AEA
#cmd LNCharacterControllerSerializeHandler_Create $AF5
#cmd LNWorldSerializeHandler_Create $B00
#cmd LNComponentListSerializeHandler_Create $B0B
#cmd LNWorldObjectSerializeHandler_Create $B16
#cmd LNWorldObjectPreUpdateHandler_Create $B21
#cmd LNWorldObjectUpdateHandler_Create $B2C
#cmd LNVisualObjectSerializeHandler_Create $B37
#cmd LNVisualObjectPreUpdateHandler_Create $B42
#cmd LNVisualObjectUpdateHandler_Create $B4D
#cmd LNCameraSerializeHandler_Create $B58
#cmd LNCameraPreUpdateHandler_Create $B63
#cmd LNCameraUpdateHandler_Create $B6E
#cmd LNEnvironmentLightSerializeHandler_Create $B79
#cmd LNEnvironmentLightPreUpdateHandler_Create $B84
#cmd LNEnvironmentLightUpdateHandler_Create $B8F
#cmd LNDirectionalLightSerializeHandler_Create $B9A
#cmd LNDirectionalLightPreUpdateHandler_Create $BA5
#cmd LNDirectionalLightUpdateHandler_Create $BB0
#cmd LNPointLightSerializeHandler_Create $BBB
#cmd LNPointLightPreUpdateHandler_Create $BC6
#cmd LNPointLightUpdateHandler_Create $BD1
#cmd LNSpotLightSerializeHandler_Create $BDC
#cmd LNSpotLightPreUpdateHandler_Create $BE7
#cmd LNSpotLightUpdateHandler_Create $BF2
#cmd LNSpriteSerializeHandler_Create $BFD
#cmd LNSpritePreUpdateHandler_Create $C08
#cmd LNSpriteUpdateHandler_Create $C13
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $C1E
#cmd LNRaycasterSerializeHandler_Create $C29
#cmd LNRaycastResultSerializeHandler_Create $C34
#cmd LNWorldRenderViewSerializeHandler_Create $C3F
#cmd LNBoxMeshSerializeHandler_Create $C4A
#cmd LNBoxMeshPreUpdateHandler_Create $C55
#cmd LNBoxMeshUpdateHandler_Create $C60
#cmd LNPlaneMeshSerializeHandler_Create $C6B
#cmd LNPlaneMeshPreUpdateHandler_Create $C76
#cmd LNPlaneMeshUpdateHandler_Create $C81
#cmd LNStaticMeshSerializeHandler_Create $C8C
#cmd LNStaticMeshPreUpdateHandler_Create $C97
#cmd LNStaticMeshUpdateHandler_Create $CA2
#cmd LNStaticMeshComponentSerializeHandler_Create $CAD
#cmd LNSkinnedMeshComponentSerializeHandler_Create $CB8
#cmd LNCollisionSerializeHandler_Create $CC3
#cmd LNTriggerBodyComponentSerializeHandler_Create $CCE
#cmd LNParticleEmitterSerializeHandler_Create $CD9
#cmd LNParticleEmitterPreUpdateHandler_Create $CE4
#cmd LNParticleEmitterUpdateHandler_Create $CEF
#cmd LNLevelSerializeHandler_Create $CFA
#cmd LNLevelStartHandler_Create $D05
#cmd LNLevelStopHandler_Create $D10
#cmd LNLevelPauseHandler_Create $D1B
#cmd LNLevelResumeHandler_Create $D26
#cmd LNLevelUpdateHandler_Create $D31
#cmd LNUIEventArgsSerializeHandler_Create $D3C
#cmd LNUILayoutElementSerializeHandler_Create $D47
#cmd LNUIElementSerializeHandler_Create $D52
#cmd LNUITextBlockSerializeHandler_Create $D5D
#cmd LNUISpriteSerializeHandler_Create $D68
#cmd LNUIIconSerializeHandler_Create $D73
#cmd LNUIMessageTextAreaSerializeHandler_Create $D7E
#cmd LNUILayoutPanelSerializeHandler_Create $D89
#cmd LNUIBoxLayoutSerializeHandler_Create $D94
#cmd LNUIStackLayoutSerializeHandler_Create $D9F
#cmd LNUIGridLayoutSerializeHandler_Create $DAA
#cmd LNUIControlSerializeHandler_Create $DB5
#cmd LNUIButtonBaseSerializeHandler_Create $DC0
#cmd LNUIButtonSerializeHandler_Create $DCB
#cmd LNUIWindowSerializeHandler_Create $DD6
#cmd LNUIListItemSerializeHandler_Create $DE1
#cmd LNUIListItemsControlSerializeHandler_Create $DEC
#cmd LNUIListBoxItemSerializeHandler_Create $DF7
#cmd LNUIListBoxSerializeHandler_Create $E02
#cmd LNInputGestureSerializeHandler_Create $E0D
#cmd LNKeyGestureSerializeHandler_Create $E18
#cmd LNInterpreterCommandSerializeHandler_Create $E23
#cmd LNInterpreterCommandListSerializeHandler_Create $E2E
#cmd LNInterpreterSerializeHandler_Create $E39
#cmd LNInterpreterUpdateWaitHandler_Create $E45
#cmd LNApplicationSerializeHandler_Create $E50
#cmd LNApplicationInitHandler_Create $E5B
#cmd LNApplicationUpdateHandler_Create $E66



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
