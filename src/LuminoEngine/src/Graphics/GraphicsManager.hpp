#pragma once

namespace ln {
class GraphicsContext;

namespace detail {
class PlatformWindow;
class IGraphicsDeviceContext;

class GraphicsManager
	: public RefObject
{
public:
	struct Settings
	{
		PlatformWindow* mainWindow = nullptr;
	};

	GraphicsManager();
	virtual ~GraphicsManager() = default;

	void initialize(const Settings& settings);
	void dispose();

	const Ref<IGraphicsDeviceContext>& deviceContext() const { return m_deviceContext; }
	const Ref<GraphicsContext>& graphicsContext() const { return m_graphicsContext; }

private:
	Ref<IGraphicsDeviceContext> m_deviceContext;
	Ref<GraphicsContext> m_graphicsContext;
};

} // namespace detail
} // namespace ln

