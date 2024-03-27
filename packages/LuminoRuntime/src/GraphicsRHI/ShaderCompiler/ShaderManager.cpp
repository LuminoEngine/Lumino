
#include "Internal.hpp"
#include <LuminoEngine/GraphicsRHI/ShaderCompiler/detail/ShaderManager.hpp>
#include "ShaderTranspiler.hpp"
#include <LuminoEngine/Asset/detail/AssetManager.hpp>

#ifdef _WIN32
#include <LuminoEngine/GraphicsRHI/ShaderCompiler/detail/D3DCompilerAPI.hpp>
#endif

namespace ln {
namespace detail {

//==============================================================================
// ShaderManager

ShaderManager* ShaderManager::initialize(const Settings& settings)
{
    if (instance()) return instance();

    auto m = Ref<ShaderManager>(LN_NEW detail::ShaderManager(), false);
    if (!m->init(settings)) return nullptr;

    EngineContext2::instance()->registerModule(m);
    EngineContext2::instance()->shaderManager = m;
    return m;
}

void ShaderManager::terminate()
{
    if (instance()) {
        instance()->dispose();
        EngineContext2::instance()->unregisterModule(instance());
        EngineContext2::instance()->shaderManager = nullptr;
    }
}

ShaderManager::ShaderManager()
    : m_builtinShaderList()
#ifdef _WIN32
    , m_hD3DCompilerDLL(nullptr)
#endif
{
}

ShaderManager::~ShaderManager()
{
}

bool ShaderManager::init(const Settings& settings)
{
    LN_LOG_DEBUG("ShaderManager Initialization started.");

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
    kokage::ShaderCodeTranspiler::initializeGlobals();
#endif

#ifdef _WIN32
    m_hD3DCompilerDLL = ::LoadLibrary(D3DCOMPILER_DLL_W);
    if (m_hD3DCompilerDLL) {
        D3DCompile2 = reinterpret_cast<PFN_D3DCompile2>(::GetProcAddress(m_hD3DCompilerDLL, "D3DCompile2"));
    }
    else {
        LN_LOG_ERROR("{} not found.", D3DCOMPILER_DLL_A);
    }
    D3DCompilerAPI::Initialize();
#endif

    LN_LOG_DEBUG("ShaderManager Initialization ended.");
    return true;
}

void ShaderManager::dispose()
{
#ifdef _WIN32
    if (m_hD3DCompilerDLL) {
        ::FreeLibrary(m_hD3DCompilerDLL);
        m_hD3DCompilerDLL = nullptr;
    }
    D3DCompile2 = nullptr;
#endif

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
    kokage::ShaderCodeTranspiler::finalizeGlobals();
#endif

}

} // namespace detail
} // namespace ln
