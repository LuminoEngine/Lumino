
#pragma once

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

	void initialize(const Settings& settings);
	void dispose();

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }

	//const List<Path>& shaderIncludePaths() const { return m_shaderIncludePaths; }

    const std::vector<std::pair<std::string, std::string>>& builtinShaderList() const { return m_builtinShaderList; }

private:
	GraphicsManager* m_graphicsManager;
    std::vector<std::pair<std::string, std::string>> m_builtinShaderList;
	//List<Path> m_shaderIncludePaths;
};

} // namespace detail
} // namespace ln
