#pragma once
#include <Lumino/Graphics/Common.hpp>
#include "../Engine/RenderingCommandList.hpp"

namespace ln {
class GraphicsContext; 
class GraphicsResource;
class SamplerState;

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

	void addGraphicsResource(GraphicsResource* resource);
	void removeGraphicsResource(GraphicsResource* resource);

	const Ref<IGraphicsDeviceContext>& deviceContext() const { return m_deviceContext; }
	const Ref<GraphicsContext>& graphicsContext() const { return m_graphicsContext; }
	const Ref<LinearAllocatorPageManager>& linearAllocatorPageManager() const { return m_linearAllocatorPageManager; }
	RenderingType renderingType() const { return RenderingType::Immediate; }
	const Ref<RenderingCommandList>& primaryRenderingCommandList() const { return m_primaryRenderingCommandList; }

	const Ref<SamplerState>& defaultSamplerState() const { return m_defaultSamplerState; }

private:
	Ref<IGraphicsDeviceContext> m_deviceContext;
	Ref<GraphicsContext> m_graphicsContext;
	Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	Ref<RenderingCommandList> m_primaryRenderingCommandList;
	List<GraphicsResource*> m_graphicsResources;

	Ref<SamplerState> m_defaultSamplerState;
};

} // namespace detail
} // namespace ln

