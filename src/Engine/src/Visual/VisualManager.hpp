
#pragma once

namespace ln {
namespace detail {
class GraphicsManager;

class VisualManager
	: public RefObject
{
public:
	struct Settings
	{
		GraphicsManager* graphicsManager;
	};

	VisualManager();
	virtual ~VisualManager();

	void init(const Settings& settings);
	void dispose();

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }

private:
	GraphicsManager * m_graphicsManager;
};

} // namespace detail
} // namespace ln
