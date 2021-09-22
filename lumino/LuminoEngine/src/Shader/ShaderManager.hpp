
#pragma once
#ifdef _WIN32
#include <d3dcompiler.h>
#endif
#include <LuminoEngine/Shader/Common.hpp>
#include "../../../RuntimeCore/src/Base/RefObjectCache.hpp"

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
    : public RefObject
{
public:
    struct Settings
    {
        GraphicsManager* graphicsManager = nullptr;
    };

    ShaderManager();
    virtual ~ShaderManager();
    void init(const Settings& settings);
    void dispose();
    Ref<Shader> loadShader(const StringRef& filePath);

    GraphicsManager* graphicsManager() const { return m_graphicsManager; }
    const std::vector<std::pair<std::string, std::string>>& builtinShaderList() const { return m_builtinShaderList; }

#ifdef _WIN32
    PFN_D3DCompile2 D3DCompile2 = nullptr;
#endif

private:
    GraphicsManager* m_graphicsManager;
    std::vector<std::pair<std::string, std::string>> m_builtinShaderList;
    ObjectCache<String, Shader> m_shaderCache;

#ifdef _WIN32
    HMODULE m_hD3DCompilerDLL;
#endif
};

} // namespace detail
} // namespace ln
