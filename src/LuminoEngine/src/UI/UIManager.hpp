#pragma once

namespace ln {
namespace detail {
class PlatformManager;
class GraphicsManager;

class UIManager
	: public RefObject
{
public:
	struct Settings
	{
		GraphicsManager* graphicsManager;
	};

	UIManager();
	virtual ~UIManager() = default;

	void initialize(const Settings& settings);
	void dispose();

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }

private:
	GraphicsManager* m_graphicsManager;
	//PlatformManager* platformManager;
};

} // namespace detail
} // namespace ln

