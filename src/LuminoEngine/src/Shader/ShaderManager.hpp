
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
		GraphicsManager* graphicsManager;
	};

	ShaderManager();
	virtual ~ShaderManager();

	void initialize(const Settings& settings);
	void dispose();

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }

private:
	GraphicsManager * m_graphicsManager;
};

} // namespace detail
} // namespace ln
