#pragma once
#include <Lumino/Graphics/Common.hpp>

namespace ln {
class GraphicsContext; 
class GraphicsResource;

namespace detail {
class PlatformWindow;
class IGraphicsDeviceContext;
class RenderingCommandList;
class LinearAllocatorPageManager;

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

	void addGraphicsResource(GraphicsResource* resource);
	void removeGraphicsResource(GraphicsResource* resource);

	const Ref<IGraphicsDeviceContext>& deviceContext() const { return m_deviceContext; }
	const Ref<GraphicsContext>& graphicsContext() const { return m_graphicsContext; }
	const Ref<LinearAllocatorPageManager>& linearAllocatorPageManager() const { return m_linearAllocatorPageManager; }
	GraphicsRenderingType renderingType() const { return GraphicsRenderingType::Immediate; }
	const Ref<RenderingCommandList>& primaryRenderingCommandList() const { return m_primaryRenderingCommandList; }

private:
	Ref<IGraphicsDeviceContext> m_deviceContext;
	Ref<GraphicsContext> m_graphicsContext;
	Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	Ref<RenderingCommandList> m_primaryRenderingCommandList;
	List<GraphicsResource*> m_graphicsResources;
};

} // namespace detail
} // namespace ln

