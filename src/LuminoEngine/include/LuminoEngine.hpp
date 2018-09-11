#pragma once

#include <LuminoCore.hpp>

#include "Lumino/Engine/EngineSettings.hpp"
#include "Lumino/Engine/Engine.hpp"
#include "Lumino/Engine/Application.hpp"

#include "Lumino/Graphics/RenderState.hpp"
#include "Lumino/Graphics/GraphicsContext.hpp"
#include "Lumino/Graphics/VertexDeclaration.hpp"
#include "Lumino/Graphics/VertexBuffer.hpp"
#include "Lumino/Graphics/IndexBuffer.hpp"
#include "Lumino/Graphics/Texture.hpp"
#include "Lumino/Graphics/SamplerState.hpp"
#include "Lumino/Graphics/Bitmap.hpp"

#include "Lumino/Shader/Shader.hpp"

#include "Lumino/UI/UIFrameWindow.hpp"


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
#pragma comment(lib, "Shlwapi.lib")
