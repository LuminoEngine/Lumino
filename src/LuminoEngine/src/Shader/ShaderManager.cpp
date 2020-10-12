
#include "Internal.hpp"
#include <LuminoEngine/Shader/Shader.hpp>
#include "ShaderManager.hpp"
#include "ShaderTranspiler.hpp"
#include "../Asset/AssetManager.hpp"

namespace ln {
namespace detail {

//=============================================================================
// ShaderManager

ShaderManager::ShaderManager()
    : m_graphicsManager(nullptr)
    , m_builtinShaderList()
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::init(const Settings& settings)
{
    LN_LOG_DEBUG << "ShaderManager Initialization started.";

    m_graphicsManager = settings.graphicsManager;

    m_shaderCache.init(64);

    // Lumino.fxh.h
    {
        static const unsigned char data[] =
            {
#include "Resource/Lumino.fxh.inl"
            };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({"Lumino.fxh", std::string((const char*)data, size)});
    }

    // LuminoForward.fxh.h
    {
        static const unsigned char data[] =
            {
#include "Resource/LuminoForward.fxh.inl"
            };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({"LuminoForward.fxh", std::string((const char*)data, size)});
    }

    // LuminoPBR.fxh.h
    {
        static const unsigned char data[] =
            {
#include "Resource/LuminoPBR.fxh.inl"
            };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({"LuminoPBR.fxh", std::string((const char*)data, size)});
    }

    // LuminoShadow.fxh.h
    {
        static const unsigned char data[] =
            {
#include "Resource/LuminoShadow.fxh.inl"
            };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({"LuminoShadow.fxh", std::string((const char*)data, size)});
    }

    // LuminoSkinning.fxh.h
    {
        static const unsigned char data[] =
            {
#include "Resource/LuminoSkinning.fxh.inl"
            };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({"LuminoSkinning.fxh", std::string((const char*)data, size)});
    }

    // LuminoNormalMap.fxh.h
    {
        static const unsigned char data[] =
        {
#include "Resource/LuminoNormalMap.fxh.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({ "LuminoNormalMap.fxh", std::string((const char*)data, size) });
    }

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
    ShaderCodeTranspiler::initializeGlobals();
#endif

    LN_LOG_DEBUG << "ShaderManager Initialization ended.";
}

void ShaderManager::dispose()
{
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
    ShaderCodeTranspiler::finalizeGlobals();
#endif

    m_shaderCache.dispose();
}

Ref<Shader> ShaderManager::loadShader(const StringRef& filePath)
{
    m_shaderCache.collectUnreferenceObjects(false);

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
    static const std::vector<const Char*> exts = { u".fx", u".lcfx" };
#else
    static const std::vector<const Char*> exts = { u".lcfx" };
#endif
    return AssetManager::loadObjectWithCacheHelper<Shader>(&m_shaderCache, nullptr, exts, filePath, nullptr);
}

} // namespace detail
} // namespace ln
