#pragma once

#include <LuminoCore.hpp>

#include "LuminoEngine/Engine/EngineSettings.hpp"
#include "LuminoEngine/Engine/Engine.hpp"
#include "LuminoEngine/Engine/Application.hpp"
#include "LuminoEngine/Engine/Diagnostics.hpp"

#include "LuminoEngine/Input/Common.hpp"
#include "LuminoEngine/Input/Input.hpp"
#include "LuminoEngine/Input/InputBinding.hpp"
#include "LuminoEngine/Input/Mouse.hpp"

#include "LuminoEngine/Graphics/RenderState.hpp"
#include "LuminoEngine/Graphics/SamplerState.hpp"
#include "LuminoEngine/Graphics/VertexDeclaration.hpp"
#include "LuminoEngine/Graphics/VertexBuffer.hpp"
#include "LuminoEngine/Graphics/IndexBuffer.hpp"
#include "LuminoEngine/Graphics/Texture.hpp"
#include "LuminoEngine/Graphics/SwapChain.hpp"
#include "LuminoEngine/Graphics/GraphicsContext.hpp"
#include "LuminoEngine/Graphics/Bitmap.hpp"

#include "LuminoEngine/Shader/Shader.hpp"

#include "LuminoEngine/Mesh/Mesh.hpp"

#include "LuminoEngine/Rendering/Common.hpp"
#include "LuminoEngine/Rendering/RenderView.hpp"
#include "LuminoEngine/Rendering/RenderFeature.hpp"
#include "LuminoEngine/Rendering/RenderingContext.hpp"

#include "LuminoEngine/UI/UIFrameWindow.hpp"


#ifdef LN_MSVC_AUTO_LINK_LIBRARIES
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("LuminoEngine"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("glad"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("glfw3"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("glslang"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("HLSL"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("libpng16"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("OGLCompiler"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("OSDependent"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("SPIRV"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("spirv-cross-core"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("spirv-cross-cpp"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("spirv-cross-glsl"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("spirv-cross-msl"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("spirv-cross-reflect"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("spirv-cross-util"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("SPVRemapper"))
#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("zlib"))
#endif