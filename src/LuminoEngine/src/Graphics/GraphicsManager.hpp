#pragma once

namespace ln {
namespace detail {
class GraphicsDeviceContext;

class GraphicsManager
	: public RefObject
{
public:
	GraphicsManager();
	virtual ~GraphicsManager() = default;

	void initialize();
	void dispose();

	const Ref<GraphicsDeviceContext>& deviceContext() const { return m_deviceContext; }

private:
	Ref<GraphicsDeviceContext> m_deviceContext;
};

} // namespace detail
} // namespace ln

