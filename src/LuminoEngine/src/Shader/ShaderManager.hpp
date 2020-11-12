
#pragma once
#include <LuminoEngine/Shader/Common.hpp>
#include "../Base/RefObjectCache.hpp"

namespace ln {
namespace detail {
class GraphicsManager;

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

private:
    GraphicsManager* m_graphicsManager;
    std::vector<std::pair<std::string, std::string>> m_builtinShaderList;
    ObjectCache<String, Shader> m_shaderCache;
};

} // namespace detail
} // namespace ln
