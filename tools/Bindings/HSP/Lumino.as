
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
#cmd LNVector3_GetX $552
#cmd LNVector3_SetX $554
#cmd LNVector3_GetY $555
#cmd LNVector3_SetY $557
#cmd LNVector3_GetZ $558
#cmd LNVector3_SetZ $55A
#cmd LNVector3_SetZeros $2F
#cmd LNVector3_Set $30
#cmd LNVector3_Get $34
#cmd LNVector3_Length $38
#cmd LNVector3_LengthSquared $39
#cmd LNVector3_MutatingNormalize $3A
#cmd LNVector3_NormalizeXYZ $3B
#cmd LNVector3_Normalize $3F
#cmd LNVector4 $41
#cmd LNVector4_GetX $571
#cmd LNVector4_SetX $573
#cmd LNVector4_GetY $574
#cmd LNVector4_SetY $576
#cmd LNVector4_GetZ $577
#cmd LNVector4_SetZ $579
#cmd LNVector4_GetW $57A
#cmd LNVector4_SetW $57C
#cmd LNVector4_SetZeros $46
#cmd LNVector4_Set $47
#cmd LNQuaternion $4C
#cmd LNQuaternion_GetX $58F
#cmd LNQuaternion_SetX $591
#cmd LNQuaternion_GetY $592
#cmd LNQuaternion_SetY $594
#cmd LNQuaternion_GetZ $595
#cmd LNQuaternion_SetZ $597
#cmd LNQuaternion_GetW $598
#cmd LNQuaternion_SetW $59A
#cmd LNQuaternion_SetZeros $51
#cmd LNQuaternion_Set $52
#cmd LNQuaternion_SetFromAxis $57
#cmd LNMatrix $5A
#cmd LNMatrix_GetRow0 $5AE
#cmd LNMatrix_SetRow0 $5B0
#cmd LNMatrix_GetRow1 $5B1
#cmd LNMatrix_SetRow1 $5B3
#cmd LNMatrix_GetRow2 $5B4
#cmd LNMatrix_SetRow2 $5B6
#cmd LNMatrix_GetRow3 $5B7
#cmd LNMatrix_SetRow3 $5B9
#cmd LNMatrix_SetZeros $5F
#cmd LNMatrix_Set $60
#cmd LNColor $179
#cmd LNColor_GetR $64D
#cmd LNColor_SetR $64F
#cmd LNColor_GetG $650
#cmd LNColor_SetG $652
#cmd LNColor_GetB $653
#cmd LNColor_SetB $655
#cmd LNColor_GetA $656
#cmd LNColor_SetA $658
#cmd LNColor_SetZeros $17E
#cmd LNColor_Set $17F
#cmd LNColorTone $184
#cmd LNColorTone_GetR $66C
#cmd LNColorTone_SetR $66E
#cmd LNColorTone_GetG $66F
#cmd LNColorTone_SetG $671
#cmd LNColorTone_GetB $672
#cmd LNColorTone_SetB $674
#cmd LNColorTone_GetS $675
#cmd LNColorTone_SetS $677
#cmd LNColorTone_SetZeros $189
#cmd LNColorTone_Set $18A
#cmd LNPoint $18F
#cmd LNPoint_GetX $68A
#cmd LNPoint_SetX $68C
#cmd LNPoint_GetY $68D
#cmd LNPoint_SetY $68F
#cmd LNPoint_SetZeros $192
#cmd LNPoint_Set $193
#cmd LNSize $196
#cmd LNSize_GetWidth $69A
#cmd LNSize_SetWidth $69C
#cmd LNSize_GetHeight $69D
#cmd LNSize_SetHeight $69F
#cmd LNSize_SetZeros $199
#cmd LNSize_Set $19A
#cmd LNRect $19D
#cmd LNRect_GetX $6AA
#cmd LNRect_SetX $6AC
#cmd LNRect_GetY $6AD
#cmd LNRect_SetY $6AF
#cmd LNRect_GetWidth $6B0
#cmd LNRect_SetWidth $6B2
#cmd LNRect_GetHeight $6B3
#cmd LNRect_SetHeight $6B5
#cmd LNRect_SetZeros $1A2
#cmd LNRect_Set $1A3
#cmd LNRect_GetLeft $1A8
#cmd LNRect_SetSize $1A9
#cmd LNRect_GetSize $1AB
#cmd LNThickness $1AC
#cmd LNThickness_GetLeft $6CE
#cmd LNThickness_SetLeft $6D0
#cmd LNThickness_GetTop $6D1
#cmd LNThickness_SetTop $6D3
#cmd LNThickness_GetRight $6D4
#cmd LNThickness_SetRight $6D6
#cmd LNThickness_GetBottom $6D7
#cmd LNThickness_SetBottom $6D9
#cmd LNThickness_SetZeros $1B1
#cmd LNThickness_Set $1B2
#cmd LNCornerRadius $1B7
#cmd LNCornerRadius_GetTopleft $6EC
#cmd LNCornerRadius_SetTopleft $6EE
#cmd LNCornerRadius_GetTopright $6EF
#cmd LNCornerRadius_SetTopright $6F1
#cmd LNCornerRadius_GetBottomright $6F2
#cmd LNCornerRadius_SetBottomright $6F4
#cmd LNCornerRadius_GetBottomleft $6F5
#cmd LNCornerRadius_SetBottomleft $6F7
#cmd LNCornerRadius_SetZeros $1BC
#cmd LNCornerRadius_Set $1BD

#cmd LNObject_Release $7A
#cmd LNObject_Retain $7B
#cmd LNObject_GetReferenceCount $7C
#cmd LNObject_SetPrototype_OnSerialize $998
#cmd LNEventConnection_SetPrototype_OnSerialize $9A3
#cmd LNPromiseFailureDelegate_Create $5D4
#cmd LNVariant_Create $81
#cmd LNVariant_SetInt $82
#cmd LNVariant_GetInt $84
#cmd LNVariant_SetPrototype_OnSerialize $9AE
#cmd LNZVTestDelegate1_Create $5DD
#cmd LNZVTestDelegate2_Create $5E3
#cmd LNZVTestDelegate3_Create $5E8
#cmd LNZVTestEventHandler1_Create $5ED
#cmd LNZVTestEventHandler2_Create $5F2
#cmd LNZVTestPromise1_ThenWith $5F5
#cmd LNZVTestPromise1_CatchWith $5F7
#cmd LNZVTestPromise2_ThenWith $5FB
#cmd LNZVTestPromise2_CatchWith $5FD
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
#cmd LNZVTestClass1_SetPrototype_OnSerialize $9B9
#cmd LNZVTestEventArgs1_Create $B0
#cmd LNZVTestEventArgs1_CreateWithValue $B1
#cmd LNZVTestEventArgs1_GetValue $AF
#cmd LNZVTestEventArgs1_SetPrototype_OnSerialize $9C4
#cmd LNSerializer2_SetPrototype_OnSerialize $9CF
#cmd LNAssetObject_SetPrototype_OnSerialize $9DA
#cmd LNAssetImportSettings_SetPrototype_OnSerialize $9E5
#cmd LNAssetModel_Create $113
#cmd LNAssetModel_Target $112
#cmd LNAssetModel_SetPrototype_OnSerialize $9F0
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
#cmd LNSound_SetPrototype_OnSerialize $9FB
#cmd LNAudio_PlayBGM $13F
#cmd LNAudio_StopBGM $144
#cmd LNAudio_PlayBGS $146
#cmd LNAudio_StopBGS $14B
#cmd LNAudio_PlayME $14D
#cmd LNAudio_StopME $151
#cmd LNAudio_PlaySE $152
#cmd LNAudio_PlaySE3D $156
#cmd LNAudio_StopSE $15C
#cmd LNTexture2DDelegate_Create $643
#cmd LNTexture2DPromise_ThenWith $646
#cmd LNTexture2DPromise_CatchWith $648
#cmd LNGraphics_GetActiveGraphicsAPI $178
#cmd LNTexture_SetPrototype_OnSerialize $A06
#cmd LNTexture2D_Create $1C8
#cmd LNTexture2D_CreateWithFormat $1CB
#cmd LNTexture2D_Load $1C4
#cmd LNTexture2D_LoadEmoji $1C6
#cmd LNTexture2D_SetPrototype_OnSerialize $A11
#cmd LNShader_Load $1D0
#cmd LNShader_SetFloat $1D3
#cmd LNShader_SetVector3 $1D6
#cmd LNShader_SetVector4 $1D9
#cmd LNShader_SetPrototype_OnSerialize $A1C
#cmd LNRenderView_SetPrototype_OnSerialize $A27
#cmd LNMaterial_Create $1FE
#cmd LNMaterial_SetMainTexture $1ED
#cmd LNMaterial_GetMainTexture $1EF
#cmd LNMaterial_SetColor $1F0
#cmd LNMaterial_SetRoughness $1F2
#cmd LNMaterial_SetMetallic $1F4
#cmd LNMaterial_SetEmissive $1F6
#cmd LNMaterial_SetShadingModel $1F8
#cmd LNMaterial_GetShadingModel $1FA
#cmd LNMaterial_SetShader $1FB
#cmd LNMaterial_GetShader $1FD
#cmd LNMaterial_SetPrototype_OnSerialize $A32
#cmd LNMeshNode_SetVisible $200
#cmd LNMeshNode_IsVisible $202
#cmd LNMeshNode_SetPrototype_OnSerialize $A3D
#cmd LNStaticMeshModel_Load $204
#cmd LNStaticMeshModel_FindNode $206
#cmd LNStaticMeshModel_FindMaterial $208
#cmd LNStaticMeshModel_MaterialCount $20A
#cmd LNStaticMeshModel_Material $20B
#cmd LNStaticMeshModel_SetPrototype_OnSerialize $A48
#cmd LNSkinnedMeshModel_Load $20E
#cmd LNSkinnedMeshModel_GetAnimationController $210
#cmd LNSkinnedMeshModel_SetPrototype_OnSerialize $A53
#cmd LNAnimationController_AddClip $212
#cmd LNAnimationController_Play $214
#cmd LNAnimationController_SetPrototype_OnSerialize $A5E
#cmd LNCollisionShape_SetPrototype_OnSerialize $A69
#cmd LNBoxCollisionShape_Create $219
#cmd LNBoxCollisionShape_CreateWHD $21B
#cmd LNBoxCollisionShape_SetPrototype_OnSerialize $A74
#cmd LNAnimationCurve_Evaluate $229
#cmd LNAnimationCurve_SetPrototype_OnSerialize $A7F
#cmd LNKeyFrameAnimationCurve_Create $236
#cmd LNKeyFrameAnimationCurve_AddKeyFrame $231
#cmd LNKeyFrameAnimationCurve_SetPrototype_OnSerialize $A8A
#cmd LNAnimationClip_Load $238
#cmd LNAnimationClip_SetWrapMode $23A
#cmd LNAnimationClip_GetWrapMode $23C
#cmd LNAnimationClip_SetHierarchicalAnimationMode $23D
#cmd LNAnimationClip_GetHierarchicalAnimationMode $23F
#cmd LNAnimationClip_SetPrototype_OnSerialize $A95
#cmd LNAnimationState_SetPrototype_OnSerialize $AA0
#cmd LNEffectResource_SetPrototype_OnSerialize $AAB
#cmd LNParticleEmitterModel_Create $264
#cmd LNParticleEmitterModel_SetMaxParticles $24C
#cmd LNParticleEmitterModel_SetSpawnRate $24E
#cmd LNParticleEmitterModel_SetLifeTime $250
#cmd LNParticleEmitterModel_SetupBoxShape $252
#cmd LNParticleEmitterModel_SetSize $254
#cmd LNParticleEmitterModel_SetSizeVelocity $256
#cmd LNParticleEmitterModel_SetSizeAcceleration $258
#cmd LNParticleEmitterModel_SetForwardVelocityMin $25A
#cmd LNParticleEmitterModel_SetForwardVelocityMax $25C
#cmd LNParticleEmitterModel_SetForwardScale $25E
#cmd LNParticleEmitterModel_SetGeometryDirection $260
#cmd LNParticleEmitterModel_SetupSpriteModule $262
#cmd LNParticleEmitterModel_SetPrototype_OnSerialize $AB6
#cmd LNParticleModel_Create $26B
#cmd LNParticleModel_SetLoop $266
#cmd LNParticleModel_IsLoop $268
#cmd LNParticleModel_AddEmitter $269
#cmd LNParticleModel_SetPrototype_OnSerialize $AC1
#cmd LNComponent_SetPrototype_OnSerialize $ACC
#cmd LNVisualComponent_SetVisible $26E
#cmd LNVisualComponent_IsVisible $270
#cmd LNVisualComponent_SetPrototype_OnSerialize $AD7
#cmd LNSpriteComponent_SetTexture $272
#cmd LNSpriteComponent_SetPrototype_OnSerialize $AE2
#cmd LNCollisionEventHandler_Create $779
#cmd LNCharacterController_Create $292
#cmd LNCharacterController_SetWalkVelocity $27C
#cmd LNCharacterController_GetWalkVelocity $27E
#cmd LNCharacterController_SetVelocity $27F
#cmd LNCharacterController_GetVelocity $281
#cmd LNCharacterController_SetInputControlEnabled $282
#cmd LNCharacterController_SetCameraControlEnabled $284
#cmd LNCharacterController_SetHeight $286
#cmd LNCharacterController_GetHeight $288
#cmd LNCharacterController_SetCameraRadius $289
#cmd LNCharacterController_GetCameraRadius $28B
#cmd LNCharacterController_SetCollisionEnter $28C
#cmd LNCharacterController_SetCollisionLeave $28E
#cmd LNCharacterController_SetCollisionStay $290
#cmd LNCharacterController_SetPrototype_OnSerialize $AED
#cmd LNWorld_Add $294
#cmd LNWorld_SetPrototype_OnSerialize $AF8
#cmd LNComponentList_GetLength $799
#cmd LNComponentList_GetItem $79B
#cmd LNComponentList_SetPrototype_OnSerialize $B03
#cmd LNWorldObject_Create $2CB
#cmd LNWorldObject_SetPosition $298
#cmd LNWorldObject_SetPositionXYZ $29A
#cmd LNWorldObject_GetPosition $29E
#cmd LNWorldObject_SetRotationQuaternion $29F
#cmd LNWorldObject_SetRotation $2A1
#cmd LNWorldObject_GetRotation $2A5
#cmd LNWorldObject_SetScale $2A6
#cmd LNWorldObject_SetScaleS $2A8
#cmd LNWorldObject_SetScaleXYZ $2AA
#cmd LNWorldObject_GetScale $2AE
#cmd LNWorldObject_SetCenterPoint $2AF
#cmd LNWorldObject_SetCenterPointXYZ $2B1
#cmd LNWorldObject_GetCenterPoint $2B5
#cmd LNWorldObject_LookAt $2B6
#cmd LNWorldObject_LookAtXYZ $2B8
#cmd LNWorldObject_AddComponent $2BC
#cmd LNWorldObject_RemoveComponent $2BE
#cmd LNWorldObject_AddTag $2C0
#cmd LNWorldObject_RemoveTag $2C2
#cmd LNWorldObject_HasTag $2C4
#cmd LNWorldObject_Destroy $2C6
#cmd LNWorldObject_GetComponents $2C7
#cmd LNWorldObject_SetPrototype_OnSerialize $B0E
#cmd LNWorldObject_SetPrototype_OnPreUpdate $B19
#cmd LNWorldObject_SetPrototype_OnUpdate $B24
#cmd LNVisualObject_SetVisible $2CD
#cmd LNVisualObject_IsVisible $2CF
#cmd LNVisualObject_SetBlendMode2 $2D0
#cmd LNVisualObject_SetOpacity $2D2
#cmd LNVisualObject_GetOpacity $2D4
#cmd LNVisualObject_SetPrototype_OnSerialize $B2F
#cmd LNVisualObject_SetPrototype_OnPreUpdate $B3A
#cmd LNVisualObject_SetPrototype_OnUpdate $B45
#cmd LNCamera_SetPrototype_OnSerialize $B50
#cmd LNCamera_SetPrototype_OnPreUpdate $B5B
#cmd LNCamera_SetPrototype_OnUpdate $B66
#cmd LNEnvironmentLight_SetEnabled $2D7
#cmd LNEnvironmentLight_IsEnabled $2D9
#cmd LNEnvironmentLight_SetColor $2DA
#cmd LNEnvironmentLight_GetColor $2DC
#cmd LNEnvironmentLight_SetAmbientColor $2DD
#cmd LNEnvironmentLight_GetAmbientColor $2DF
#cmd LNEnvironmentLight_GetSkyColor $2E0
#cmd LNEnvironmentLight_SetSkyColor $2E1
#cmd LNEnvironmentLight_GetGroundColor $2E3
#cmd LNEnvironmentLight_SetGroundColor $2E4
#cmd LNEnvironmentLight_SetIntensity $2E6
#cmd LNEnvironmentLight_GetIntensity $2E8
#cmd LNEnvironmentLight_SetShadowEffectiveDistance $2E9
#cmd LNEnvironmentLight_GetShadowEffectiveDistance $2EB
#cmd LNEnvironmentLight_SetShadowEffectiveDepth $2EC
#cmd LNEnvironmentLight_GetShadowEffectiveDepth $2EE
#cmd LNEnvironmentLight_SetPrototype_OnSerialize $B71
#cmd LNEnvironmentLight_SetPrototype_OnPreUpdate $B7C
#cmd LNEnvironmentLight_SetPrototype_OnUpdate $B87
#cmd LNDirectionalLight_Create $2FF
#cmd LNDirectionalLight_CreateWithColor $300
#cmd LNDirectionalLight_SetEnabled $2F0
#cmd LNDirectionalLight_IsEnabled $2F2
#cmd LNDirectionalLight_SetColor $2F3
#cmd LNDirectionalLight_GetColor $2F5
#cmd LNDirectionalLight_SetIntensity $2F6
#cmd LNDirectionalLight_GetIntensity $2F8
#cmd LNDirectionalLight_SetShadowEffectiveDistance $2F9
#cmd LNDirectionalLight_GetShadowEffectiveDistance $2FB
#cmd LNDirectionalLight_SetShadowEffectiveDepth $2FC
#cmd LNDirectionalLight_GetShadowEffectiveDepth $2FE
#cmd LNDirectionalLight_SetPrototype_OnSerialize $B92
#cmd LNDirectionalLight_SetPrototype_OnPreUpdate $B9D
#cmd LNDirectionalLight_SetPrototype_OnUpdate $BA8
#cmd LNPointLight_Create $312
#cmd LNPointLight_CreateWithColorAndRange $313
#cmd LNPointLight_SetEnabled $303
#cmd LNPointLight_IsEnabled $305
#cmd LNPointLight_SetColor $306
#cmd LNPointLight_GetColor $308
#cmd LNPointLight_SetIntensity $309
#cmd LNPointLight_GetIntensity $30B
#cmd LNPointLight_SetRange $30C
#cmd LNPointLight_GetRange $30E
#cmd LNPointLight_SetAttenuation $30F
#cmd LNPointLight_GetAttenuation $311
#cmd LNPointLight_SetPrototype_OnSerialize $BB3
#cmd LNPointLight_SetPrototype_OnPreUpdate $BBE
#cmd LNPointLight_SetPrototype_OnUpdate $BC9
#cmd LNSpotLight_Create $32C
#cmd LNSpotLight_CreateWithColorAndRange $32D
#cmd LNSpotLight_SetEnabled $317
#cmd LNSpotLight_IsEnabled $319
#cmd LNSpotLight_SetColor $31A
#cmd LNSpotLight_GetColor $31C
#cmd LNSpotLight_SetIntensity $31D
#cmd LNSpotLight_GetIntensity $31F
#cmd LNSpotLight_SetRange $320
#cmd LNSpotLight_GetRange $322
#cmd LNSpotLight_SetAttenuation $323
#cmd LNSpotLight_GetAttenuation $325
#cmd LNSpotLight_SetAngle $326
#cmd LNSpotLight_GetAngle $328
#cmd LNSpotLight_SetPenumbra $329
#cmd LNSpotLight_GetPenumbra $32B
#cmd LNSpotLight_SetPrototype_OnSerialize $BD4
#cmd LNSpotLight_SetPrototype_OnPreUpdate $BDF
#cmd LNSpotLight_SetPrototype_OnUpdate $BEA
#cmd LNTestDelegate_Create $847
#cmd LNSprite_Create $343
#cmd LNSprite_CreateWithTexture $344
#cmd LNSprite_CreateWithTextureAndSize $346
#cmd LNSprite_SetTexture $335
#cmd LNSprite_SetSize $337
#cmd LNSprite_SetSizeWH $339
#cmd LNSprite_SetSourceRectXYWH $33C
#cmd LNSprite_SetCallerTest $341
#cmd LNSprite_SetPrototype_OnSerialize $BF5
#cmd LNSprite_SetPrototype_OnPreUpdate $C00
#cmd LNSprite_SetPrototype_OnUpdate $C0B
#cmd LNCameraOrbitControlComponent_Create $34B
#cmd LNCameraOrbitControlComponent_SetPrototype_OnSerialize $C16
#cmd LNRaycaster_FromScreen $34D
#cmd LNRaycaster_IntersectPlane $34F
#cmd LNRaycaster_SetPrototype_OnSerialize $C21
#cmd LNRaycastResult_GetPoint $354
#cmd LNRaycastResult_SetPrototype_OnSerialize $C2C
#cmd LNWorldRenderView_SetGuideGridEnabled $356
#cmd LNWorldRenderView_GetGuideGridEnabled $358
#cmd LNWorldRenderView_SetPrototype_OnSerialize $C37
#cmd LNBoxMesh_Create $35A
#cmd LNBoxMesh_SetPrototype_OnSerialize $C42
#cmd LNBoxMesh_SetPrototype_OnPreUpdate $C4D
#cmd LNBoxMesh_SetPrototype_OnUpdate $C58
#cmd LNPlaneMesh_Create $35C
#cmd LNPlaneMesh_SetPrototype_OnSerialize $C63
#cmd LNPlaneMesh_SetPrototype_OnPreUpdate $C6E
#cmd LNPlaneMesh_SetPrototype_OnUpdate $C79
#cmd LNStaticMesh_Load $35E
#cmd LNStaticMesh_GetModel $360
#cmd LNStaticMesh_MakeCollisionBody $361
#cmd LNStaticMesh_SetPrototype_OnSerialize $C84
#cmd LNStaticMesh_SetPrototype_OnPreUpdate $C8F
#cmd LNStaticMesh_SetPrototype_OnUpdate $C9A
#cmd LNStaticMeshComponent_Create $368
#cmd LNStaticMeshComponent_SetModel $364
#cmd LNStaticMeshComponent_MakeCollisionBody $366
#cmd LNStaticMeshComponent_SetPrototype_OnSerialize $CA5
#cmd LNSkinnedMeshComponent_Create $36C
#cmd LNSkinnedMeshComponent_SetModel $36A
#cmd LNSkinnedMeshComponent_SetPrototype_OnSerialize $CB0
#cmd LNCollision_GetWorldObject $36E
#cmd LNCollision_SetPrototype_OnSerialize $CBB
#cmd LNTriggerBodyComponent_Create $372
#cmd LNTriggerBodyComponent_AddCollisionShape $370
#cmd LNTriggerBodyComponent_SetPrototype_OnSerialize $CC6
#cmd LNParticleEmitter_Create $374
#cmd LNParticleEmitter_SetPrototype_OnSerialize $CD1
#cmd LNParticleEmitter_SetPrototype_OnPreUpdate $CDC
#cmd LNParticleEmitter_SetPrototype_OnUpdate $CE7
#cmd LNScene_SetClearMode $377
#cmd LNScene_SetSkyColor $379
#cmd LNScene_SetSkyHorizonColor $37B
#cmd LNScene_SetSkyCloudColor $37D
#cmd LNScene_SetSkyOverlayColor $37F
#cmd LNScene_GotoLevel $381
#cmd LNScene_CallLevel $384
#cmd LNScene_ReturnLevel $387
#cmd LNScene_ActiveLevel $389
#cmd LNScene_IsTransitionEffectRunning $38A
#cmd LNScene_SetTransitionEffectMode $38B
#cmd LNScene_TransitionEffectMode $38D
#cmd LNScene_SetTransitionDuration $38E
#cmd LNScene_TransitionDuration $390
#cmd LNScene_SetTransitionEffectColor $391
#cmd LNScene_TransitionEffectColor $393
#cmd LNScene_SetTransitionEffectMaskTexture $394
#cmd LNScene_TransitionEffectMaskTexture $396
#cmd LNScene_SetTransitionEffectVague $397
#cmd LNScene_TransitionEffectVague $399
#cmd LNScene_StartFadeOut $39A
#cmd LNScene_StartFadeIn $39B
#cmd LNScene_SetFogStartDistance $39C
#cmd LNScene_SetFogColor $39E
#cmd LNScene_SetFogDensity $3A0
#cmd LNScene_SetFogHeightDensity $3A2
#cmd LNScene_SetFogLowerHeight $3A4
#cmd LNScene_SetFogUpperHeight $3A6
#cmd LNScene_SetHDREnabled $3A8
#cmd LNScene_IsHDREnabled $3AA
#cmd LNScene_SetScreenBlendColor $3AB
#cmd LNScene_ScreenBlendColor $3AD
#cmd LNScene_SetColorTone $3AE
#cmd LNScene_ColorTone $3B0
#cmd LNScene_SetAntialiasEnabled $3B1
#cmd LNScene_IsAntialiasEnabled $3B3
#cmd LNScene_SetSSREnabled $3B4
#cmd LNScene_IsSSREnabled $3B6
#cmd LNScene_SetSSAOEnabled $3B7
#cmd LNScene_IsSSAOEnabled $3B9
#cmd LNScene_SetBloomEnabled $3BA
#cmd LNScene_IsBloomEnabled $3BC
#cmd LNScene_SetDOFEnabled $3BD
#cmd LNScene_IsDOFEnabled $3BF
#cmd LNScene_SetTonemapEnabled $3C0
#cmd LNScene_IsTonemapEnabled $3C2
#cmd LNScene_SetVignetteEnabled $3C3
#cmd LNScene_IsVignetteEnabled $3C5
#cmd LNScene_SetGammaEnabled $3C6
#cmd LNScene_IsGammaEnabled $3C8
#cmd LNScene_SetTonemapExposure $3C9
#cmd LNScene_SetTonemapLinearWhite $3CB
#cmd LNScene_SetTonemapShoulderStrength $3CD
#cmd LNScene_SetTonemapLinearStrength $3CF
#cmd LNScene_SetTonemapLinearAngle $3D1
#cmd LNScene_SetTonemapToeStrength $3D3
#cmd LNScene_SetTonemapToeNumerator $3D5
#cmd LNScene_SetTonemapToeDenominator $3D7
#cmd LNLevel_Create $3E8
#cmd LNLevel_AddObject $3DA
#cmd LNLevel_RemoveObject $3DC
#cmd LNLevel_AddSubLevel $3DE
#cmd LNLevel_RemoveSubLevel $3E0
#cmd LNLevel_RemoveAllSubLevels $3E2
#cmd LNLevel_SetPrototype_OnSerialize $CF2
#cmd LNLevel_SetPrototype_OnStart $CFD
#cmd LNLevel_SetPrototype_OnStop $D08
#cmd LNLevel_SetPrototype_OnPause $D13
#cmd LNLevel_SetPrototype_OnResume $D1E
#cmd LNLevel_SetPrototype_OnUpdate $D29
#cmd LNUIEventArgs_Sender $3F3
#cmd LNUIEventArgs_SetPrototype_OnSerialize $D34
#cmd LNUIGeneralEventHandler_Create $8A6
#cmd LNUIEventHandler_Create $8AB
#cmd LNUILayoutElement_SetPrototype_OnSerialize $D3F
#cmd LNUIElement_SetSize $405
#cmd LNUIElement_SetSizeWH $407
#cmd LNUIElement_SetWidth $40A
#cmd LNUIElement_GetWidth $40C
#cmd LNUIElement_SetHeight $40D
#cmd LNUIElement_GetHeight $40F
#cmd LNUIElement_SetMargin $410
#cmd LNUIElement_GetMargin $412
#cmd LNUIElement_SetPadding $413
#cmd LNUIElement_GetPadding $415
#cmd LNUIElement_SetHAlignment $416
#cmd LNUIElement_GetHAlignment $418
#cmd LNUIElement_SetVAlignment $419
#cmd LNUIElement_GetVAlignment $41B
#cmd LNUIElement_SetAlignments $41C
#cmd LNUIElement_SetPosition $41F
#cmd LNUIElement_SetPositionXYZ $421
#cmd LNUIElement_GetPosition $425
#cmd LNUIElement_SetRotation $426
#cmd LNUIElement_SetEulerAngles $428
#cmd LNUIElement_GetRotation $42C
#cmd LNUIElement_SetScale $42D
#cmd LNUIElement_SetScaleS $42F
#cmd LNUIElement_SetScaleXY $431
#cmd LNUIElement_GetScale $434
#cmd LNUIElement_SetCenterPoint $435
#cmd LNUIElement_SetCenterPointXYZ $437
#cmd LNUIElement_GetCenterPoint $43B
#cmd LNUIElement_SetEnabled $43C
#cmd LNUIElement_IsEnabled $43E
#cmd LNUIElement_SetData $43F
#cmd LNUIElement_GetData $441
#cmd LNUIElement_SetBackgroundColor $442
#cmd LNUIElement_GetBackgroundColor $444
#cmd LNUIElement_SetBorderThickness $445
#cmd LNUIElement_GetBorderThickness $447
#cmd LNUIElement_SetBorderColor $448
#cmd LNUIElement_GetBorderColor $44A
#cmd LNUIElement_SetCornerRadius $44B
#cmd LNUIElement_GetCornerRadius $44D
#cmd LNUIElement_SetVisibility $44E
#cmd LNUIElement_GetVisibility $450
#cmd LNUIElement_SetOpacity $451
#cmd LNUIElement_GetOpacity $453
#cmd LNUIElement_AddChild $454
#cmd LNUIElement_SetPrototype_OnSerialize $D4A
#cmd LNUITextBlock_Create $45A
#cmd LNUITextBlock_CreateWithText $45B
#cmd LNUITextBlock_SetText $457
#cmd LNUITextBlock_GetText $459
#cmd LNUITextBlock_SetPrototype_OnSerialize $D55
#cmd LNUISprite_Create $468
#cmd LNUISprite_CreateWithTexture $469
#cmd LNUISprite_SetTexture $45E
#cmd LNUISprite_SetSourceRect $460
#cmd LNUISprite_SetSourceRectXYWH $462
#cmd LNUISprite_GetSourceRect $467
#cmd LNUISprite_SetPrototype_OnSerialize $D60
#cmd LNUIIcon_LoadFontIcon $46C
#cmd LNUIIcon_LoadFontIconWithNameSize $46E
#cmd LNUIIcon_LoadFontIconWithNameSizeColor $471
#cmd LNUIIcon_SetPrototype_OnSerialize $D6B
#cmd LNUIMessageTextArea_Create $47A
#cmd LNUIMessageTextArea_SetText $476
#cmd LNUIMessageTextArea_SetTypingSpeed $478
#cmd LNUIMessageTextArea_SetPrototype_OnSerialize $D76
#cmd LNUI_Add $47C
#cmd LNUI_Remove $47E
#cmd LNUILayoutPanel_SetPrototype_OnSerialize $D81
#cmd LNUIBoxLayout_Create $485
#cmd LNUIBoxLayout_SetOrientation $482
#cmd LNUIBoxLayout_GetOrientation $484
#cmd LNUIBoxLayout_SetPrototype_OnSerialize $D8C
#cmd LNUIStackLayout_Create $48A
#cmd LNUIStackLayout_SetOrientation $487
#cmd LNUIStackLayout_GetOrientation $489
#cmd LNUIStackLayout_SetPrototype_OnSerialize $D97
#cmd LNUIGridLayout_Create $49A
#cmd LNUIGridLayout_SetColumnCount $48C
#cmd LNUIGridLayout_SetRow $48E
#cmd LNUIGridLayout_SetColumn $491
#cmd LNUIGridLayout_SetPlacement $494
#cmd LNUIGridLayout_SetPrototype_OnSerialize $DA2
#cmd LNUIControl_Create $4A6
#cmd LNUIControl_AddInlineVisual $4A7
#cmd LNUIControl_SetPrototype_OnSerialize $DAD
#cmd LNUIButtonBase_SetText $4AB
#cmd LNUIButtonBase_SetPrototype_OnSerialize $DB8
#cmd LNUIButton_Create $4AE
#cmd LNUIButton_CreateWithText $4AF
#cmd LNUIButton_ConnectOnClicked $4B1
#cmd LNUIButton_SetPrototype_OnSerialize $DC3
#cmd LNUIWindow_Create $4B4
#cmd LNUIWindow_SetPrototype_OnSerialize $DCE
#cmd LNUIListItem_ConnectOnSubmit $4B9
#cmd LNUIListItem_SetPrototype_OnSerialize $DD9
#cmd LNUIListItemsControl_SetItemsLayoutPanel $4BC
#cmd LNUIListItemsControl_SetSubmitMode $4BE
#cmd LNUIListItemsControl_GetSubmitMode $4C0
#cmd LNUIListItemsControl_SetPrototype_OnSerialize $DE4
#cmd LNUIListBoxItem_Create $4C2
#cmd LNUIListBoxItem_SetPrototype_OnSerialize $DEF
#cmd LNUIListBox_Create $4C7
#cmd LNUIListBox_AddItem $4C5
#cmd LNUIListBox_SetPrototype_OnSerialize $DFA
#cmd LNInputGesture_SetPrototype_OnSerialize $E05
#cmd LNKeyGesture_Create $4CA
#cmd LNKeyGesture_SetPrototype_OnSerialize $E10
#cmd LNInput_IsPressed $4CD
#cmd LNInput_IsTriggered $4CF
#cmd LNInput_IsTriggeredOff $4D1
#cmd LNInput_IsRepeated $4D3
#cmd LNInput_GetAxisValue $4D5
#cmd LNInput_AddBinding $4D7
#cmd LNInput_RemoveBinding $4DA
#cmd LNInput_ClearBindings $4DC
#cmd LNInput_ClearAllBindings $4DE
#cmd LNMouse_Pressed $4E0
#cmd LNMouse_Triggered $4E2
#cmd LNMouse_TriggeredOff $4E4
#cmd LNMouse_Repeated $4E6
#cmd LNMouse_Position $4E8
#cmd LNInterpreterCommand_Code $4EA
#cmd LNInterpreterCommand_ParamsCount $4EB
#cmd LNInterpreterCommand_Param $4EC
#cmd LNInterpreterCommand_SetPrototype_OnSerialize $E1B
#cmd LNInterpreterCommandList_Create $503
#cmd LNInterpreterCommandList_AddCommand $4EF
#cmd LNInterpreterCommandList_AddCommand1 $4F1
#cmd LNInterpreterCommandList_AddCommand2 $4F4
#cmd LNInterpreterCommandList_AddCommand3 $4F8
#cmd LNInterpreterCommandList_AddCommand4 $4FD
#cmd LNInterpreterCommandList_SetPrototype_OnSerialize $E26
#cmd LNInterpreterCommandDelegate_Create $96A
#cmd LNInterpreter_Create $517
#cmd LNInterpreter_Clear $508
#cmd LNInterpreter_Run $509
#cmd LNInterpreter_IsRunning $50B
#cmd LNInterpreter_Update $50C
#cmd LNInterpreter_Terminate $50D
#cmd LNInterpreter_RegisterCommandHandler $50E
#cmd LNInterpreter_SetWaitMode $511
#cmd LNInterpreter_GetWaitMode $513
#cmd LNInterpreter_SetWaitCount $514
#cmd LNInterpreter_GetWaitCount $516
#cmd LNInterpreter_SetPrototype_OnSerialize $E31
#cmd LNInterpreter_SetPrototype_OnUpdateWait $E3D
#cmd LNEngineSettings_SetMainWindowSize $51A
#cmd LNEngineSettings_SetMainWorldViewSize $51D
#cmd LNEngineSettings_SetMainWindowTitle $520
#cmd LNEngineSettings_AddAssetDirectory $522
#cmd LNEngineSettings_AddAssetArchive $524
#cmd LNEngineSettings_SetFrameRate $527
#cmd LNEngineSettings_SetUITheme $529
#cmd LNEngineSettings_SetFontFile $52B
#cmd LNEngineSettings_SetDebugToolEnabled $52D
#cmd LNEngineSettings_SetEngineLogEnabled $52F
#cmd LNEngineSettings_SetEngineLogFilePath $531
#cmd LNEngineSettings_SetDeveloperToolEnabled $533
#cmd LNEngine_Initialize $536
#cmd LNEngine_Finalize $537
#cmd LNEngine_Update $538
#cmd LNEngine_Run $539
#cmd LNEngine_Time $53B
#cmd LNEngine_GetCamera $53C
#cmd LNEngine_GetMainLight $53D
#cmd LNEngine_GetRenderView $53E
#cmd LNApplication_Create $543
#cmd LNApplication_OnInit $540
#cmd LNApplication_OnUpdate $541
#cmd LNApplication_World $542
#cmd LNApplication_SetPrototype_OnSerialize $E48
#cmd LNApplication_SetPrototype_OnInit $E53
#cmd LNApplication_SetPrototype_OnUpdate $E5E
#cmd LNDebug_SetGuideGridEnabled $545
#cmd LNDebug_SetPhysicsDebugDrawEnabled $547
#cmd LNDebug_Print $549
#cmd LNDebug_PrintWithTime $54B
#cmd LNDebug_PrintWithTimeAndColor $54E
#cmd LNObjectSerializeHandler_Create $996
#cmd LNEventConnectionSerializeHandler_Create $9A1
#cmd LNVariantSerializeHandler_Create $9AC
#cmd LNZVTestClass1SerializeHandler_Create $9B7
#cmd LNZVTestEventArgs1SerializeHandler_Create $9C2
#cmd LNSerializer2SerializeHandler_Create $9CD
#cmd LNAssetObjectSerializeHandler_Create $9D8
#cmd LNAssetImportSettingsSerializeHandler_Create $9E3
#cmd LNAssetModelSerializeHandler_Create $9EE
#cmd LNSoundSerializeHandler_Create $9F9
#cmd LNTextureSerializeHandler_Create $A04
#cmd LNTexture2DSerializeHandler_Create $A0F
#cmd LNShaderSerializeHandler_Create $A1A
#cmd LNRenderViewSerializeHandler_Create $A25
#cmd LNMaterialSerializeHandler_Create $A30
#cmd LNMeshNodeSerializeHandler_Create $A3B
#cmd LNStaticMeshModelSerializeHandler_Create $A46
#cmd LNSkinnedMeshModelSerializeHandler_Create $A51
#cmd LNAnimationControllerSerializeHandler_Create $A5C
#cmd LNCollisionShapeSerializeHandler_Create $A67
#cmd LNBoxCollisionShapeSerializeHandler_Create $A72
#cmd LNAnimationCurveSerializeHandler_Create $A7D
#cmd LNKeyFrameAnimationCurveSerializeHandler_Create $A88
#cmd LNAnimationClipSerializeHandler_Create $A93
#cmd LNAnimationStateSerializeHandler_Create $A9E
#cmd LNEffectResourceSerializeHandler_Create $AA9
#cmd LNParticleEmitterModelSerializeHandler_Create $AB4
#cmd LNParticleModelSerializeHandler_Create $ABF
#cmd LNComponentSerializeHandler_Create $ACA
#cmd LNVisualComponentSerializeHandler_Create $AD5
#cmd LNSpriteComponentSerializeHandler_Create $AE0
#cmd LNCharacterControllerSerializeHandler_Create $AEB
#cmd LNWorldSerializeHandler_Create $AF6
#cmd LNComponentListSerializeHandler_Create $B01
#cmd LNWorldObjectSerializeHandler_Create $B0C
#cmd LNWorldObjectPreUpdateHandler_Create $B17
#cmd LNWorldObjectUpdateHandler_Create $B22
#cmd LNVisualObjectSerializeHandler_Create $B2D
#cmd LNVisualObjectPreUpdateHandler_Create $B38
#cmd LNVisualObjectUpdateHandler_Create $B43
#cmd LNCameraSerializeHandler_Create $B4E
#cmd LNCameraPreUpdateHandler_Create $B59
#cmd LNCameraUpdateHandler_Create $B64
#cmd LNEnvironmentLightSerializeHandler_Create $B6F
#cmd LNEnvironmentLightPreUpdateHandler_Create $B7A
#cmd LNEnvironmentLightUpdateHandler_Create $B85
#cmd LNDirectionalLightSerializeHandler_Create $B90
#cmd LNDirectionalLightPreUpdateHandler_Create $B9B
#cmd LNDirectionalLightUpdateHandler_Create $BA6
#cmd LNPointLightSerializeHandler_Create $BB1
#cmd LNPointLightPreUpdateHandler_Create $BBC
#cmd LNPointLightUpdateHandler_Create $BC7
#cmd LNSpotLightSerializeHandler_Create $BD2
#cmd LNSpotLightPreUpdateHandler_Create $BDD
#cmd LNSpotLightUpdateHandler_Create $BE8
#cmd LNSpriteSerializeHandler_Create $BF3
#cmd LNSpritePreUpdateHandler_Create $BFE
#cmd LNSpriteUpdateHandler_Create $C09
#cmd LNCameraOrbitControlComponentSerializeHandler_Create $C14
#cmd LNRaycasterSerializeHandler_Create $C1F
#cmd LNRaycastResultSerializeHandler_Create $C2A
#cmd LNWorldRenderViewSerializeHandler_Create $C35
#cmd LNBoxMeshSerializeHandler_Create $C40
#cmd LNBoxMeshPreUpdateHandler_Create $C4B
#cmd LNBoxMeshUpdateHandler_Create $C56
#cmd LNPlaneMeshSerializeHandler_Create $C61
#cmd LNPlaneMeshPreUpdateHandler_Create $C6C
#cmd LNPlaneMeshUpdateHandler_Create $C77
#cmd LNStaticMeshSerializeHandler_Create $C82
#cmd LNStaticMeshPreUpdateHandler_Create $C8D
#cmd LNStaticMeshUpdateHandler_Create $C98
#cmd LNStaticMeshComponentSerializeHandler_Create $CA3
#cmd LNSkinnedMeshComponentSerializeHandler_Create $CAE
#cmd LNCollisionSerializeHandler_Create $CB9
#cmd LNTriggerBodyComponentSerializeHandler_Create $CC4
#cmd LNParticleEmitterSerializeHandler_Create $CCF
#cmd LNParticleEmitterPreUpdateHandler_Create $CDA
#cmd LNParticleEmitterUpdateHandler_Create $CE5
#cmd LNLevelSerializeHandler_Create $CF0
#cmd LNLevelStartHandler_Create $CFB
#cmd LNLevelStopHandler_Create $D06
#cmd LNLevelPauseHandler_Create $D11
#cmd LNLevelResumeHandler_Create $D1C
#cmd LNLevelUpdateHandler_Create $D27
#cmd LNUIEventArgsSerializeHandler_Create $D32
#cmd LNUILayoutElementSerializeHandler_Create $D3D
#cmd LNUIElementSerializeHandler_Create $D48
#cmd LNUITextBlockSerializeHandler_Create $D53
#cmd LNUISpriteSerializeHandler_Create $D5E
#cmd LNUIIconSerializeHandler_Create $D69
#cmd LNUIMessageTextAreaSerializeHandler_Create $D74
#cmd LNUILayoutPanelSerializeHandler_Create $D7F
#cmd LNUIBoxLayoutSerializeHandler_Create $D8A
#cmd LNUIStackLayoutSerializeHandler_Create $D95
#cmd LNUIGridLayoutSerializeHandler_Create $DA0
#cmd LNUIControlSerializeHandler_Create $DAB
#cmd LNUIButtonBaseSerializeHandler_Create $DB6
#cmd LNUIButtonSerializeHandler_Create $DC1
#cmd LNUIWindowSerializeHandler_Create $DCC
#cmd LNUIListItemSerializeHandler_Create $DD7
#cmd LNUIListItemsControlSerializeHandler_Create $DE2
#cmd LNUIListBoxItemSerializeHandler_Create $DED
#cmd LNUIListBoxSerializeHandler_Create $DF8
#cmd LNInputGestureSerializeHandler_Create $E03
#cmd LNKeyGestureSerializeHandler_Create $E0E
#cmd LNInterpreterCommandSerializeHandler_Create $E19
#cmd LNInterpreterCommandListSerializeHandler_Create $E24
#cmd LNInterpreterSerializeHandler_Create $E2F
#cmd LNInterpreterUpdateWaitHandler_Create $E3B
#cmd LNApplicationSerializeHandler_Create $E46
#cmd LNApplicationInitHandler_Create $E51
#cmd LNApplicationUpdateHandler_Create $E5C



#cmd ln_args $1
#cmd ln_set_args $2

#endif // __lumino__
