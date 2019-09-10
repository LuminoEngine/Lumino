#pragma once

#include <LuminoCore.hpp>

#include "LuminoEngine/Base/Regex.hpp"
#include "LuminoEngine/Base/Variant.hpp"

#include "LuminoEngine/Engine/EngineSettings.hpp"
#include "LuminoEngine/Engine/Engine.hpp"
#include "LuminoEngine/Engine/Application.hpp"
#include "LuminoEngine/Engine/Diagnostics.hpp"

#include "LuminoEngine/Animation/Common.hpp"
#include "LuminoEngine/Animation/AnimationCurve.hpp"
#include "LuminoEngine/Animation/AnimationTrack.hpp"
#include "LuminoEngine/Animation/AnimationClip.hpp"
#include "LuminoEngine/Animation/AnimationClock.hpp"
#include "LuminoEngine/Animation/EasingFunctions.hpp"

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

#include "LuminoEngine/Shader/Shader.hpp"

#include "LuminoEngine/Graphics/RenderState.hpp"
#include "LuminoEngine/Graphics/SamplerState.hpp"
#include "LuminoEngine/Graphics/VertexLayout.hpp"
#include "LuminoEngine/Graphics/VertexBuffer.hpp"
#include "LuminoEngine/Graphics/IndexBuffer.hpp"
#include "LuminoEngine/Graphics/Texture.hpp"
#include "LuminoEngine/Graphics/SwapChain.hpp"
#include "LuminoEngine/Graphics/GraphicsContext.hpp"
#include "LuminoEngine/Graphics/Bitmap.hpp"
#include "LuminoEngine/Graphics/Graphics.hpp"

#include "LuminoEngine/Font/Font.hpp"

#include "LuminoEngine/Mesh/Mesh.hpp"

#include "LuminoEngine/Rendering/Common.hpp"
#include "LuminoEngine/Rendering/Material.hpp"
#include "LuminoEngine/Rendering/RenderView.hpp"
#include "LuminoEngine/Rendering/RenderFeature.hpp"
#include "LuminoEngine/Rendering/RenderingContext.hpp"

#include "LuminoEngine/ImageEffect/ImageEffect.hpp"
#include "LuminoEngine/ImageEffect/ToneImageEffect.hpp"
#include "LuminoEngine/ImageEffect/ScreenBlurImageEffect.hpp"

#include "LuminoEngine/Physics/CollisionShape.hpp"
#include "LuminoEngine/Physics/PhysicsWorld.hpp"
#include "LuminoEngine/Physics/PhysicsObject.hpp"
#include "LuminoEngine/Physics/RigidBody.hpp"
#include "LuminoEngine/Physics/SoftBody.hpp"

#include "LuminoEngine/Visual/LightComponent.hpp"
#include "LuminoEngine/Visual/SpriteComponent.hpp"
#include "LuminoEngine/Visual/StaticMeshComponent.hpp"

#include "LuminoEngine/Scene/WorldRenderView.hpp"
#include "LuminoEngine/Scene/World.hpp"
#include "LuminoEngine/Scene/WorldObject.hpp"
#include "LuminoEngine/Scene/Camera.hpp"
#include "LuminoEngine/Scene/CameraOrbitControlComponent.hpp"
#include "LuminoEngine/Scene/Light.hpp"
#include "LuminoEngine/Scene/Sprite.hpp"
#include "LuminoEngine/Scene/StaticMesh.hpp"
#include "LuminoEngine/Scene/RigidBodyComponent.hpp"

#include "LuminoEngine/Scene/Scene.hpp"
#include "LuminoEngine/Scene/SceneConductor.hpp"

#include "LuminoEngine/UI/UIRenderView.hpp"
#include "LuminoEngine/UI/UIFrameWindow.hpp"
#include "LuminoEngine/UI/UIViewport.hpp"
#include "LuminoEngine/UI/UISprite.hpp"
#include "LuminoEngine/UI/UITextBlock.hpp"
#include "LuminoEngine/UI/UILayoutPanel.hpp"
#include "LuminoEngine/UI/UIWindow.hpp"

#include "LuminoEngine/Asset/Assets.hpp"

#include "LuminoEngine/Tilemap/Tileset.hpp"
#include "LuminoEngine/Tilemap/TilemapLayer.hpp"
#include "LuminoEngine/Tilemap/TilemapModel.hpp"
#include "LuminoEngine/Tilemap/TilemapComponent.hpp"
#include "LuminoEngine/Tilemap/Tilemap.hpp"

#include "LuminoEngine/Effect/EffectContext.hpp"



#ifdef _DEBUG
#define LN_MSVC_AUTO_LINK_POSTFIX    "d"
#else
#define LN_MSVC_AUTO_LINK_POSTFIX
#endif

#ifdef LN_MSVC_AUTO_LINK_LIBRARIES
#pragma comment(lib, "glfw3" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
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
#pragma comment(lib, "libpng16" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "OGLCompiler" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "OSDependent" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "SPIRV" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "SPVRemapper" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "zlib" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "LinearMath" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "BulletCollision" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "BulletDynamics" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "BulletSoftBody" LN_MSVC_AUTO_LINK_POSTFIX ".lib")
#pragma comment(lib, "Box2D" LN_MSVC_AUTO_LINK_POSTFIX ".lib")

#ifdef _DEBUG
#pragma comment(lib, "tmxlite-s-d.lib")
#else
#pragma comment(lib, "tmxlite-s.lib")
#endif
#endif