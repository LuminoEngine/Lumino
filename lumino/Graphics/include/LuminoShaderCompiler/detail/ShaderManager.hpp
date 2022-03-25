
#pragma once
#ifdef _WIN32
#define NOMINMAX
#include <d3dcompiler.h>
#endif
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Engine/Module.hpp>
#include <LuminoShaderCompiler/Common.hpp>
#include <LuminoEngine/Base/detail/RefObjectCache.hpp>

namespace ln {
namespace detail {
class GraphicsManager;

#ifdef _WIN32
typedef HRESULT(WINAPI* PFN_D3DCompile2)(
    LPCVOID pSrcData,
    SIZE_T SrcDataSize,
    LPCSTR pSourceName,
    const D3D_SHADER_MACRO* pDefines,
    ID3DInclude* pInclude,
    LPCSTR pEntrypoint,
    LPCSTR pTarget,
    UINT Flags1,
    UINT Flags2,
    UINT SecondaryDataFlags,
    LPCVOID pSecondaryData,
    SIZE_T SecondaryDataSize,
    ID3DBlob** ppCode,
    ID3DBlob** ppErrorMsgs
    );
#endif

class ShaderManager
    : public Module
{
public:
    struct Settings
    {
        GraphicsManager* graphicsManager = nullptr;
    };
    static ShaderManager* initialize(const Settings& settings);
    static void terminate();
    static inline ShaderManager* instance() { return static_cast<ShaderManager*>(EngineContext2::instance()->shaderManager); }

    GraphicsManager* graphicsManager2() const { return m_graphicsManager; }
    const std::vector<std::pair<std::string, std::string>>& builtinShaderList() const { return m_builtinShaderList; }

#ifdef _WIN32
    PFN_D3DCompile2 D3DCompile2 = nullptr;
#endif

    virtual ~ShaderManager();

private:
    ShaderManager();
    bool init(const Settings& settings);
    void dispose();

    GraphicsManager* m_graphicsManager;
    std::vector<std::pair<std::string, std::string>> m_builtinShaderList;

#ifdef _WIN32
    HMODULE m_hD3DCompilerDLL;
#endif
};

} // namespace detail
} // namespace ln
