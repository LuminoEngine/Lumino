#pragma once

#include <LuminoCore.hpp>

#include "LuminoEngine/Engine/EngineSettings.hpp"
#include "LuminoEngine/Engine/Engine.hpp"
#include "LuminoEngine/Engine/Application.hpp"
#include "LuminoEngine/Engine/Diagnostics.hpp"

#include "LuminoEngine/Animation/Common.hpp"
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
#include "LuminoEngine/Graphics/VertexDeclaration.hpp"
#include "LuminoEngine/Graphics/VertexBuffer.hpp"
#include "LuminoEngine/Graphics/IndexBuffer.hpp"
#include "LuminoEngine/Graphics/Texture.hpp"
#include "LuminoEngine/Graphics/SwapChain.hpp"
#include "LuminoEngine/Graphics/GraphicsContext.hpp"
#include "LuminoEngine/Graphics/Bitmap.hpp"

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

#include "LuminoEngine/Visual/LightComponent.hpp"
#include "LuminoEngine/Visual/SpriteComponent.hpp"

#include "LuminoEngine/Scene/World.hpp"
#include "LuminoEngine/Scene/WorldObject.hpp"
#include "LuminoEngine/Scene/Camera.hpp"
#include "LuminoEngine/Scene/Light.hpp"
#include "LuminoEngine/Scene/Sprite.hpp"

#include "LuminoEngine/UI/UIFrameWindow.hpp"
#include "LuminoEngine/UI/UIViewport.hpp"
#include "LuminoEngine/UI/UISprite.hpp"

#include "LuminoEngine/Asset/Assets.hpp"

#include "LuminoEngine/Tilemap/Tileset.hpp"
#include "LuminoEngine/Tilemap/TilemapLayer.hpp"
#include "LuminoEngine/Tilemap/TilemapModel.hpp"
#include "LuminoEngine/Tilemap/Tilemap.hpp"

#ifdef LN_MSVC_AUTO_LINK_LIBRARIES
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "ogg.lib")
#pragma comment(lib, "spirv-cross-core.lib")
#pragma comment(lib, "spirv-cross-cpp.lib")
#pragma comment(lib, "spirv-cross-glsl.lib")
#pragma comment(lib, "spirv-cross-msl.lib")
#pragma comment(lib, "spirv-cross-reflect.lib")
#pragma comment(lib, "spirv-cross-util.lib")
#pragma comment(lib, "vorbis.lib")

#ifdef _DEBUG
#pragma comment(lib, "LuminoEngined.lib")
#pragma comment(lib, "freetyped.lib")
#pragma comment(lib, "gladd.lib")
#pragma comment(lib, "glslangd.lib")
#pragma comment(lib, "HLSLd.lib")
#pragma comment(lib, "libpng16d.lib")
#pragma comment(lib, "OGLCompilerd.lib")
#pragma comment(lib, "OSDependentd.lib")
#pragma comment(lib, "SPIRVd.lib")
#pragma comment(lib, "SPVRemapperd.lib")
#pragma comment(lib, "zlibd.lib")
#else
#pragma comment(lib, "LuminoEngine.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "glad.lib")
#pragma comment(lib, "glslang.lib")
#pragma comment(lib, "HLSL.lib")
#pragma comment(lib, "libpng16.lib")
#pragma comment(lib, "OGLCompiler.lib")
#pragma comment(lib, "OSDependent.lib")
#pragma comment(lib, "SPIRV.lib")
#pragma comment(lib, "SPVRemapper.lib")
#pragma comment(lib, "zlib.lib")
#endif
#endif
