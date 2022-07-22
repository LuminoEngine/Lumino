#pragma once

#include <LuminoCore.hpp>
#include <LuminoEngine/Base/Regex.hpp>
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Base/Task.hpp>
#include <LuminoBitmap/Bitmap.hpp>
#include <LuminoFont/Font.hpp>

#include "LuminoEngine/Common.hpp"
#include "LuminoEngine/EngineContext.hpp"

#include "LuminoEngine/Engine/EngineSettings.hpp"
#include "LuminoEngine/Engine/Engine.hpp"
#include "LuminoEngine/Engine/Time.hpp"
#include "LuminoEngine/Engine/Application.hpp"
#include "LuminoEngine/Engine/Diagnostics.hpp"
#include "LuminoEngine/Engine/Debug.hpp"

#include "LuminoGraphics/Animation/Common.hpp"
#include "LuminoGraphics/Animation/AnimationCurve.hpp"
#include "LuminoGraphics/Animation/AnimationTrack.hpp"
#include "LuminoGraphics/Animation/AnimationClip.hpp"
#include "LuminoGraphics/Animation/AnimationClock.hpp"
#include "LuminoGraphics/Animation/EasingFunctions.hpp"

#include "LuminoEngine/Input/Common.hpp"
#include "LuminoEngine/Input/Input.hpp"
#include "LuminoEngine/Input/InputBinding.hpp"
#include "LuminoEngine/Input/Mouse.hpp"

#include "LuminoEngine/Audio/AudioContext.hpp"
#include "LuminoEngine/Audio/AudioBus.hpp"
#include "LuminoEngine/Audio/AudioNode.hpp"
#include "LuminoEngine/Audio/AudioGainNode.hpp"
#include "LuminoEngine/Audio/AudioProcessorNode.hpp"
#include "LuminoEngine/Audio/Sound.hpp"
#include "LuminoEngine/Audio/GameAudio.hpp"
#include "LuminoEngine/Audio/Audio.hpp"


#include "LuminoEngine/Graphics/RenderState.hpp"
#include "LuminoGraphics/RHI/Shader.hpp"
#include "LuminoGraphics/RHI/SamplerState.hpp"
#include "LuminoGraphics/RHI/VertexLayout.hpp"
#include "LuminoGraphics/RHI/VertexBuffer.hpp"
#include "LuminoGraphics/RHI/IndexBuffer.hpp"
#include "LuminoGraphics/RHI/Texture.hpp"
#include "LuminoGraphics/RHI/RenderPass.hpp"
#include "LuminoGraphics/RHI/SwapChain.hpp"
#include "LuminoGraphics/RHI/GraphicsCommandBuffer.hpp"
#include "LuminoGraphics/RHI/Graphics.hpp"


#include "LuminoGraphics/Mesh/MeshPrimitive.hpp"
#include "LuminoGraphics/Mesh/MeshModel.hpp"
#include "LuminoGraphics/Mesh/AnimationController.hpp"
#include "LuminoGraphics/Mesh/SkinnedMeshModel.hpp"

#include "LuminoGraphics/Rendering/Common.hpp"
#include "LuminoGraphics/Rendering/Material.hpp"
#include "LuminoGraphics/Rendering/CommandList.hpp"
#include "LuminoGraphics/Rendering/RenderView.hpp"
#include "LuminoGraphics/Rendering/RenderFeature.hpp"
#include "LuminoGraphics/Rendering/RenderingContext.hpp"
#include "LuminoGraphics/Rendering/Shaders.hpp"

#include "LuminoEngine/PostEffect/PostEffect.hpp"
#include "LuminoEngine/PostEffect/TonePostEffect.hpp"
#include "LuminoEngine/PostEffect/ScreenBlurPostEffect.hpp"
#include "LuminoEngine/PostEffect/BloomPostEffect.hpp"
#include "LuminoEngine/PostEffect/FilmicPostEffect.hpp"

#include "LuminoEngine/Physics/CollisionShape.hpp"
#include "LuminoEngine/Physics/PhysicsWorld.hpp"
#include "LuminoEngine/Physics/PhysicsObject.hpp"
#include "LuminoEngine/Physics/RigidBody.hpp"
#include "LuminoEngine/Physics/SoftBody.hpp"

#include "LuminoEngine/Visual/LightComponent.hpp"
#include "LuminoEngine/Visual/SpriteComponent.hpp"

#include "LuminoEngine/Scene/WorldRenderView.hpp"
#include "LuminoEngine/Scene/World.hpp"
#include "LuminoEngine/Scene/WorldObject.hpp"
#include "LuminoEngine/Scene/Camera.hpp"
#include "LuminoEngine/Scene/CameraOrbitControlComponent.hpp"
#include "LuminoEngine/Scene/Raycaster.hpp"
#include "LuminoEngine/Scene/Light.hpp"
#include "LuminoEngine/Scene/Sprite.hpp"
#include "LuminoEngine/Scene/CharacterController.hpp"
#include "LuminoEngine/Scene/Physics/Collision.hpp"
#include "LuminoEngine/Scene/Physics/RigidBodyComponent.hpp"
#include "LuminoEngine/Scene/Physics/RigidBody2DComponent.hpp"
#include "LuminoEngine/Scene/Text/Text.hpp"
#include "LuminoEngine/Scene/Mesh/MeshComponent.hpp"
#include "LuminoEngine/Scene/Mesh/StaticMesh.hpp"
#include "LuminoEngine/Scene/Mesh/SkinnedMesh.hpp"
#include "LuminoEngine/Scene/Shapes/MeshPrimitives.hpp"
#include "LuminoEngine/Scene/Shapes/MeshPrimitiveComponent.hpp"
#include "LuminoEngine/Scene/Shapes/ConeMesh.hpp"
#include "LuminoEngine/Scene/Shapes/CylinderMesh.hpp"
#include "LuminoEngine/Scene/Shapes/TeapotMesh.hpp"
#include "LuminoEngine/Scene/Effect/ParticleEmitterComponent.hpp"
#include "LuminoEngine/Scene/Effect/ParticleEmitter.hpp"

#include "LuminoEngine/Scene/Level.hpp"
#include "LuminoEngine/Scene/Scene.hpp"
#include "LuminoEngine/Scene/SceneConductor.hpp"

#include "LuminoEngine/UI/UIEvents.hpp"
#include "LuminoEngine/UI/UIColors.hpp"
#include "LuminoEngine/UI/UIStyle.hpp"
#include "LuminoEngine/UI/UIRenderView.hpp"
#include "LuminoEngine/UI/UIFrameWindow.hpp"
#include "LuminoEngine/UI/UIViewport.hpp"
#include "LuminoEngine/UI/UISprite.hpp"
#include "LuminoEngine/UI/UIText.hpp"
#include "LuminoEngine/UI/UIIcon.hpp"
#include "LuminoEngine/UI/Layout/UILayoutPanel.hpp"
#include "LuminoEngine/UI/Layout/UIGridLayout.hpp"
#include "LuminoEngine/UI/Controls/UIWindow.hpp"
#include "LuminoEngine/UI/Controls/UIButton.hpp"
#include "LuminoEngine/UI/Controls/UIListBox.hpp"
#include "LuminoEngine/UI/Controls/UIListBoxItem.hpp"
#include "LuminoEngine/UI/Controls/UIListView.hpp"
#include "LuminoEngine/UI/UI.hpp"

#include "LuminoEngine/UI/UIMessageTextArea.hpp"

#include "LuminoEngine/Asset/Assets.hpp"

#include "LuminoEngine/Tilemap/Tileset.hpp"
#include "LuminoEngine/Tilemap/TilemapLayer.hpp"
#include "LuminoEngine/Tilemap/TilemapModel.hpp"
#include "LuminoEngine/Tilemap/TilemapComponent.hpp"
#include "LuminoEngine/Tilemap/Tilemap.hpp"

#include "LuminoEngine/Effect/EffectContext.hpp"

#include "LuminoEngine/Scripting/Interpreter.hpp"



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

