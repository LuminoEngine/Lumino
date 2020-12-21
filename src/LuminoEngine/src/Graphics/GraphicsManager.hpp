﻿#pragma once
#include <LuminoEngine/Graphics/Common.hpp>
#include <LuminoEngine/Engine/RenderingCommandList.hpp>
#include "../Base/RefObjectCache.hpp"

namespace ln {
class GraphicsContext; 
class CommandQueue;
class SamplerState;

namespace detail {
class PlatformWindow;
class IGraphicsDevice;
class ICommandList;
class RenderTargetTextureCacheManager;
class DepthBufferCacheManager;
class FrameBufferCache;
class RenderingQueue;
class AssetRequiredPathSet;
class SingleFrameUniformBufferAllocatorPageManager;

class GraphicsManager
	: public RefObject
{
public:
	struct Settings
	{
        AssetManager* assetManager = nullptr;
		PlatformManager* platformManager = nullptr;
		PlatformWindow* mainWindow = nullptr;
		GraphicsAPI graphicsAPI;
		bool debugMode = false;
	};

	GraphicsManager();
	virtual ~GraphicsManager() = default;

	void init(const Settings& settings);
	void dispose();

    //void enterRendering();
    //void leaveRendering();

	void addGraphicsResource(IGraphicsResource* resource);
	void removeGraphicsResource(IGraphicsResource* resource);

    AssetManager* assetManager() const { return m_assetManager; }
	// deviceContext() は、リソースの CRUD のみを目的として IGraphicsDevice にアクセスしたいときに使うこと。
	// 描画を目的としたステートの変更や、clear、draw 系は GraphicsContext::commitState() の戻り値を使うこと。
	// またこれらの予防として、IGraphicsDevice のポインタは持ち出してメンバに保持したりせず、
	// 必要な時に GraphicsManager または GraphicsContext から取得すること。
	// TODO: こういう事情がでてきたので、以前のバージョンのように IGraphicsDevice をリソース関係と描画関係で分離するのもアリかもしれない。
	const Ref<IGraphicsDevice>& deviceContext() const { return m_deviceContext; }
    //const Ref<GraphicsContext>& mainWindowGraphicsContext() const { return m_graphicsContext; }
	const Ref<CommandQueue>& graphicsQueue() const { return m_graphicsQueue; }
	const Ref<CommandQueue>& computeQueue() const { return m_computeQueue; }
	const Ref<LinearAllocatorPageManager>& linearAllocatorPageManager() const { return m_linearAllocatorPageManager; }
	const Ref<RenderingQueue>& renderingQueue() const { return m_renderingQueue; }
	RenderingType renderingType() const { return RenderingType::Immediate; }
	//const Ref<RenderingCommandList>& primaryRenderingCommandList2() const { return m_primaryRenderingCommandList; }
	const Ref<RenderTargetTextureCacheManager>& renderTargetTextureCacheManager() const { return m_renderTargetTextureCacheManager; }
	const Ref<DepthBufferCacheManager>& depthBufferCacheManager() const { return m_depthBufferCacheManager; }
	const Ref<FrameBufferCache>& frameBufferCache() const { return m_frameBufferCache; }
	ObjectCache<String, Texture2D>* texture2DCache() { return &m_texture2DCache; }
	const Ref<SingleFrameUniformBufferAllocatorPageManager>& singleFrameUniformBufferAllocatorPageManager() const { return m_singleFrameUniformBufferAllocatorPageManager; }

	const List<IGraphicsResource*>& graphicsResources() const { return m_graphicsResources; }

	int registerExtension(INativeGraphicsExtension* extension);
	void unregisterExtension(INativeGraphicsExtension* extension);
	Ref<Texture> requestTexture(const AssetPath& assetPath);

	Ref<Texture2D> loadTexture2D(const StringRef& filePath);
	Ref<Texture2D> loadTexture2DFromOnMemoryData(const detail::AssetPath* baseDir, const StringRef& filePath, std::function<Ref<Texture2D>(const AssetRequiredPathSet*)> factory);

    const Ref<Texture2D>& blackTexture() const { return m_blackTexture; }
    const Ref<Texture2D>& whiteTexture() const { return m_whiteTexture; }
	const Ref<Texture2D>& defaultNormalMap() const { return m_defaultNormalMap; }
	const Ref<SamplerState>& defaultSamplerState() const { return m_defaultSamplerState; }
	const Ref<SamplerState>& linearSamplerState() const { return m_linearSamplerState; }
	const Ref<SamplerState>& pointClampSamplerState() const { return m_pointClampSamplerState; }
	const Ref<SamplerState>& linearClampSamplerState() const { return m_linearClampSamplerState; }

	static bool checkVulkanSupported();

private:
	//struct ExtensionInfo
	//{

	//	INativeGraphicsExtension* extension;
	//};

	void createOpenGLContext(const Settings& settings);
	void createVulkanContext(const Settings& settings);

    AssetManager* m_assetManager;
	PlatformManager* m_platformManager;
	Ref<IGraphicsDevice> m_deviceContext;
	//Ref<GraphicsContext> m_graphicsContext;
	Ref<CommandQueue> m_graphicsQueue;
	Ref<CommandQueue> m_computeQueue;

	Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	Ref<RenderingQueue> m_renderingQueue;
	Ref<RenderTargetTextureCacheManager> m_renderTargetTextureCacheManager;
	Ref<DepthBufferCacheManager> m_depthBufferCacheManager;
	Ref<FrameBufferCache> m_frameBufferCache;
	ObjectCache<String, Texture2D> m_texture2DCache;
	Ref<SingleFrameUniformBufferAllocatorPageManager> m_singleFrameUniformBufferAllocatorPageManager;
	List<IGraphicsResource*> m_graphicsResources;
	List<INativeGraphicsExtension*> m_extensions;

    Ref<Texture2D> m_blackTexture;
    Ref<Texture2D> m_whiteTexture;
	Ref<Texture2D> m_defaultNormalMap;
	Ref<SamplerState> m_defaultSamplerState;
	Ref<SamplerState> m_linearSamplerState;
	Ref<SamplerState> m_pointClampSamplerState;
	Ref<SamplerState> m_linearClampSamplerState;
	
};

} // namespace detail
} // namespace ln

