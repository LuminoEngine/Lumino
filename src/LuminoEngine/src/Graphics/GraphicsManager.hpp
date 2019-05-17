#pragma once
#include <LuminoEngine/Graphics/Common.hpp>
#include "../Engine/RenderingCommandList.hpp"

namespace ln {
class GraphicsContext; 
class GraphicsResource;
class Texture2D;
class SamplerState;

namespace detail {
class PlatformWindow;
class IGraphicsDevice;
class RenderTargetTextureCacheManager;
class DepthBufferCacheManager;
class FrameBufferCache;
class RenderingQueue;

class GraphicsManager
	: public RefObject
{
public:
	struct Settings
	{
		PlatformWindow* mainWindow = nullptr;
		GraphicsAPI graphicsAPI;
	};

	GraphicsManager();
	virtual ~GraphicsManager() = default;

	void init(const Settings& settings);
	void dispose();

    //void enterRendering();
    //void leaveRendering();

	void addGraphicsResource(GraphicsResource* resource);
	void removeGraphicsResource(GraphicsResource* resource);

	// deviceContext() は、リソースの CRUD のみを目的として IGraphicsDevice にアクセスしたいときに使うこと。
	// 描画を目的としたステートの変更や、clear、draw 系は GraphicsContext::commitState() の戻り値を使うこと。
	// またこれらの予防として、IGraphicsDevice のポインタは持ち出してメンバに保持したりせず、
	// 必要な時に GraphicsManager または GraphicsContext から取得すること。
	// TODO: こういう事情がでてきたので、以前のバージョンのように IGraphicsDevice をリソース関係と描画関係で分離するのもアリかもしれない。
	const Ref<IGraphicsDevice>& deviceContext() const { return m_deviceContext; }
	const Ref<GraphicsContext>& graphicsContext2() const { return m_graphicsContext; }
    const Ref<GraphicsContext>& mainWindowGraphicsContext() const { return m_graphicsContext; }
	const Ref<LinearAllocatorPageManager>& linearAllocatorPageManager() const { return m_linearAllocatorPageManager; }
	const Ref<RenderingQueue>& renderingQueue() const { return m_renderingQueue; }
	RenderingType renderingType() const { return RenderingType::Immediate; }
	//const Ref<RenderingCommandList>& primaryRenderingCommandList2() const { return m_primaryRenderingCommandList; }
	const Ref<RenderTargetTextureCacheManager>& renderTargetTextureCacheManager() const { return m_renderTargetTextureCacheManager; }
	const Ref<DepthBufferCacheManager>& depthBufferCacheManager() const { return m_depthBufferCacheManager; }
	const Ref<FrameBufferCache>& frameBufferCache() const { return m_frameBufferCache; }

    const Ref<Texture2D>& blackTexture() const { return m_blackTexture; }
    const Ref<Texture2D>& whiteTexture() const { return m_whiteTexture; }
	const Ref<SamplerState>& defaultSamplerState() const { return m_defaultSamplerState; }


private:
	void createOpenGLContext(const Settings& settings);
	void createVulkanContext(const Settings& settings);

	Ref<IGraphicsDevice> m_deviceContext;
	Ref<GraphicsContext> m_graphicsContext;
	Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	Ref<RenderingQueue> m_renderingQueue;
	Ref<RenderTargetTextureCacheManager> m_renderTargetTextureCacheManager;
	Ref<DepthBufferCacheManager> m_depthBufferCacheManager;
	Ref<FrameBufferCache> m_frameBufferCache;
	List<GraphicsResource*> m_graphicsResources;

    Ref<Texture2D> m_blackTexture;
    Ref<Texture2D> m_whiteTexture;
	Ref<SamplerState> m_defaultSamplerState;
};

} // namespace detail
} // namespace ln

