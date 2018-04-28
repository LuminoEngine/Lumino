#pragma once

namespace ln {
class GraphicsContext;

namespace detail {
class IGraphicsDeviceContext;

class GraphicsManager
	: public RefObject
{
public:
	struct Settings
	{
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

