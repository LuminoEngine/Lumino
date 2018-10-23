
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

	const List<Path>& shaderIncludePaths() const { return m_shaderIncludePaths; }

private:
	GraphicsManager * m_graphicsManager;
	List<Path> m_shaderIncludePaths;
};

} // namespace detail
} // namespace ln
