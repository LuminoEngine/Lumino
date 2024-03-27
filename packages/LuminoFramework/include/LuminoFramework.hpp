#pragma once

#include <LuminoCore.hpp>
#include <LuminoEngine/Base/Regex.hpp>
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Base/Task.hpp>

#include "LuminoEngine/Common.hpp"
#include "LuminoFramework/EngineContext.hpp"

#include "LuminoFramework/Engine/EngineSettings.hpp"
#include "LuminoFramework/Engine/Engine.hpp"
#include "LuminoFramework/Engine/Time.hpp"
#include "LuminoFramework/Engine/Application.hpp"
#include "LuminoEngine/Engine/Diagnostics.hpp"
#include "LuminoFramework/Engine/Debug.hpp"

#include "LuminoEngine/Animation/Common.hpp"
#include "LuminoEngine/Animation/AnimationCurve.hpp"
#include "LuminoEngine/Animation/AnimationTrack.hpp"
#include "LuminoEngine/Animation/AnimationClip.hpp"
#include "LuminoEngine/Animation/AnimationClock.hpp"
#include "LuminoEngine/Animation/EasingFunctions.hpp"

#include "LuminoFramework/Input/Common.hpp"
#include "LuminoFramework/Input/Input.hpp"
#include "LuminoFramework/Input/InputBinding.hpp"
#include "LuminoFramework/Input/Mouse.hpp"

#include "LuminoFramework/Audio/AudioContext.hpp"
#include "LuminoFramework/Audio/AudioBus.hpp"
#include "LuminoFramework/Audio/AudioNode.hpp"
#include "LuminoFramework/Audio/AudioGainNode.hpp"
#include "LuminoFramework/Audio/AudioProcessorNode.hpp"
#include "LuminoFramework/Audio/Sound.hpp"
#include "LuminoFramework/Audio/GameAudio.hpp"
#include "LuminoFramework/Audio/Audio.hpp"

#include "LuminoEngine/Graphics/RenderState.hpp"
#include <LuminoEngine/LuminoGraphics.hpp>

#include "LuminoEngine/Mesh/MeshPrimitive.hpp"
#include "LuminoEngine/Mesh/MeshModel.hpp"
#include "LuminoEngine/Mesh/AnimationController.hpp"
#include "LuminoEngine/Mesh/SkinnedMeshModel.hpp"

#include "LuminoEngine/Rendering/Common.hpp"
#include "LuminoEngine/Rendering/Material.hpp"
#include "LuminoEngine/Rendering/CommandList.hpp"
#include "LuminoEngine/Rendering/RenderView.hpp"
#include "LuminoEngine/Rendering/RenderFeature.hpp"
#include "LuminoEngine/Rendering/RenderingContext.hpp"
#include "LuminoEngine/Rendering/Shaders.hpp"

#include "LuminoFramework/PostEffect/PostEffect.hpp"
#include "LuminoFramework/PostEffect/TonePostEffect.hpp"
#include "LuminoFramework/PostEffect/ScreenBlurPostEffect.hpp"
#include "LuminoFramework/PostEffect/BloomPostEffect.hpp"
#include "LuminoFramework/PostEffect/FilmicPostEffect.hpp"

#include "LuminoFramework/Physics/CollisionShape.hpp"
#include "LuminoFramework/Physics/PhysicsWorld.hpp"
#include "LuminoFramework/Physics/PhysicsObject.hpp"
#include "LuminoFramework/Physics/RigidBody.hpp"
#include "LuminoFramework/Physics/SoftBody.hpp"

#include "LuminoFramework/Visual/LightComponent.hpp"
#include "LuminoFramework/Visual/SpriteComponent.hpp"

#include "LuminoFramework/Scene/WorldRenderView.hpp"
#include "LuminoFramework/Scene/World.hpp"
#include "LuminoFramework/Scene/WorldObject.hpp"
#include "LuminoFramework/Scene/Camera.hpp"
#include "LuminoFramework/Scene/CameraOrbitControlComponent.hpp"
#include "LuminoFramework/Scene/Raycaster.hpp"
#include "LuminoFramework/Scene/Light.hpp"
#include "LuminoFramework/Scene/Sprite.hpp"
#include "LuminoFramework/Scene/CharacterController.hpp"
#include "LuminoFramework/Scene/Physics/Collision.hpp"
#include "LuminoFramework/Scene/Physics/RigidBodyComponent.hpp"
#include "LuminoFramework/Scene/Physics/RigidBody2DComponent.hpp"
#include "LuminoFramework/Scene/Text/Text.hpp"
#include "LuminoFramework/Scene/Mesh/MeshComponent.hpp"
#include "LuminoFramework/Scene/Mesh/StaticMesh.hpp"
#include "LuminoFramework/Scene/Mesh/SkinnedMesh.hpp"
#include "LuminoFramework/Scene/Shapes/MeshPrimitives.hpp"
#include "LuminoFramework/Scene/Shapes/MeshPrimitiveComponent.hpp"
#include "LuminoFramework/Scene/Shapes/ConeMesh.hpp"
#include "LuminoFramework/Scene/Shapes/CylinderMesh.hpp"
#include "LuminoFramework/Scene/Shapes/TeapotMesh.hpp"
#include "LuminoFramework/Scene/Effect/ParticleEmitterComponent.hpp"
#include "LuminoFramework/Scene/Effect/ParticleEmitter.hpp"

#include "LuminoFramework/Scene/Level.hpp"
#include "LuminoFramework/Scene/Scene.hpp"
#include "LuminoFramework/Scene/SceneConductor.hpp"

#include "LuminoFramework/UI/UIEvents.hpp"
#include "LuminoFramework/UI/UIColors.hpp"
#include "LuminoFramework/UI/UIStyle.hpp"
#include "LuminoFramework/UI/UIRenderView.hpp"
#include "LuminoFramework/UI/UIFrameWindow.hpp"
#include "LuminoFramework/UI/UIViewport.hpp"
#include "LuminoFramework/UI/UISprite.hpp"
#include "LuminoFramework/UI/UIText.hpp"
#include "LuminoFramework/UI/UIIcon.hpp"
#include "LuminoFramework/UI/Layout/UILayoutPanel.hpp"
#include "LuminoFramework/UI/Layout/UIGridLayout.hpp"
#include "LuminoFramework/UI/Controls/UIWindow.hpp"
#include "LuminoFramework/UI/Controls/UIButton.hpp"
#include "LuminoFramework/UI/Controls/UIListBox.hpp"
#include "LuminoFramework/UI/Controls/UIListBoxItem.hpp"
#include "LuminoFramework/UI/Controls/UIListView.hpp"
#include "LuminoFramework/UI/UI.hpp"

#include "LuminoFramework/UI/UIMessageTextArea.hpp"

#include "LuminoEngine/Asset/Assets.hpp"

#include "LuminoFramework/Tilemap/Tileset.hpp"
#include "LuminoFramework/Tilemap/TilemapLayer.hpp"
#include "LuminoFramework/Tilemap/TilemapModel.hpp"
#include "LuminoFramework/Tilemap/TilemapComponent.hpp"
#include "LuminoFramework/Tilemap/Tilemap.hpp"

#include "LuminoFramework/Effect/EffectContext.hpp"


#ifdef _DEBUG
#define LN_MSVC_AUTO_LINK_POSTFIX    "d"
#else
#define LN_MSVC_AUTO_LINK_POSTFIX
#endif

#ifdef LN_MSVC_AUTO_LINK_LIBRARIES
#pragma comment(lib, "glfw3" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "nanovg" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "ogg" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "spirv-cross-core" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "spirv-cross-cpp" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "spirv-cross-glsl" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "spirv-cross-msl" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "spirv-cross-reflect" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "spirv-cross-util" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "vorbis" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "vorbisfile" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "LuminoEngine" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "freetype" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "glad" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "glslang" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "HLSL" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "OGLCompiler" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "OSDependent" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "SPIRV" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "SPVRemapper" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "GenericCodeGen" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "MachineIndependent" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "libpng16" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "zlib" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "LinearMath" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "BulletCollision" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "BulletDynamics" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "BulletSoftBody" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "Box2D" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "yaml-cpp" LN_MSVC_AUTO_LINK_POSTFIX ".lib")

#ifdef _DEBUG
#pragma comment(lib, "tmxlite-s-d.lib")
#else
#pragma comment(lib, "tmxlite-s.lib")
#endif
#endif

