
#include "Internal.hpp"
#include "ShaderManager.hpp"
#include "ShaderTranspiler.hpp"

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

void ShaderManager::initialize(const Settings& settings)
{
	m_graphicsManager = settings.graphicsManager;

    // Lumino.fxh.h
    {
        static const unsigned char data[] =
        {
#include "Resource/Lumino.fxh.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({ "Lumino.fxh", std::string((const char*)data, size) });
    }

    // LuminoForward.fxh.h
    {
        static const unsigned char data[] =
        {
#include "Resource/LuminoForward.fxh.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({ "LuminoForward.fxh", std::string((const char*)data, size) });
    }

    // LuminoPBR.fxh.h
    {
        static const unsigned char data[] =
        {
#include "Resource/LuminoPBR.fxh.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({ "LuminoPBR.fxh", std::string((const char*)data, size) });
    }

    // LuminoShadow.fxh.h
    {
        static const unsigned char data[] =
        {
#include "Resource/LuminoShadow.fxh.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({ "LuminoShadow.fxh", std::string((const char*)data, size) });
    }

    // LuminoSkinning.fxh.h
    {
        static const unsigned char data[] =
        {
#include "Resource/LuminoSkinning.fxh.inl"
        };
        static const size_t size = LN_ARRAY_SIZE_OF(data);
        m_builtinShaderList.push_back({ "LuminoSkinning.fxh", std::string((const char*)data, size) });
    }

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
	ShaderCodeTranspiler::initializeGlobals();
#endif
}

void ShaderManager::dispose()
{
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
    ShaderCodeTranspiler::finalizeGlobals();
#endif
}

} // namespace detail
} // namespace ln

