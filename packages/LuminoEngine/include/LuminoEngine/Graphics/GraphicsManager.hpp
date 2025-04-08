#pragma once
#include <LuminoEngine/Engine/EngineInstance.hpp>
#include <LuminoEngine/Engine/Module.hpp>
#include <LuminoEngine/Base/detail/RefObjectCache.hpp>
#include "../Graphics/GPU/Common.hpp"
#include "../Graphics/GPU/detail/RenderingCommandList.hpp"

namespace ln {
class CommandQueue;
class SamplerState;
class PlatformWindow;

namespace detail {
class IGraphicsDevice;
class ICommandList;
class RenderTargetTextureCacheManager;
class DepthBufferCacheManager;
class FrameBufferCache;
class RenderingQueue;
struct AssetRequiredPathSet;
class AssetManager;
class PlatformManager;
class ShaderManager;
class StreamingBufferAllocatorManager;
//class RenderingManager2;
class MeshManager;
} // namespace detail

class GraphicsManager : public RefObject {
public:
    struct Settings {
        LNGraphicsBackend graphicsAPI = LN_GRAPHICS_BACKEND_DEFAULT;
        String priorityGPUName;
        bool debugMode = false;
    };

    static GraphicsManager* instance();

    GraphicsManager();
    MaybeResult init(const Settings& settings);
    void dispose();

    virtual ~GraphicsManager() = default;

    Result_deprecated<Ref<GraphicsContext>> createGraphicsContext(PlatformWindow* window);

    //void enterRendering();
    //void leaveRendering();

    void addGraphicsResource(IGraphicsResource* resource);
    void removeGraphicsResource(IGraphicsResource* resource);

    detail::AssetManager* assetManager() const { return m_assetManager; }
    detail::MeshManager* meshManager() const { return m_meshManager; }

    // deviceContext() は、リソースの CRUD のみを目的として IGraphicsDevice にアクセスしたいときに使うこと。
    // 描画を目的としたステートの変更や、clear、draw 系は GraphicsCommandList::commitState() の戻り値を使うこと。
    // またこれらの予防として、IGraphicsDevice のポインタは持ち出してメンバに保持したりせず、
    // 必要な時に GraphicsManager または GraphicsCommandList から取得すること。
    // TODO: こういう事情がでてきたので、以前のバージョンのように IGraphicsDevice をリソース関係と描画関係で分離するのもアリかもしれない。
    //const Ref<IGraphicsDevice>& deviceContext() const { return m_deviceContext; }
    //const Ref<CommandQueue>& graphicsQueue() const { return m_graphicsQueue; }
    //const Ref<CommandQueue>& computeQueue() const { return m_computeQueue; }

    
    detail::GraphicsObjectRegistry* resourceRegistry() const { return m_resourceRegistry; }
    //GraphicsResourceRegistry* vertexLayoutRegistry() const { return m_vertexLayoutRegistry; }
    //GraphicsResourceRegistry* vertexBufferRegistry() const { return m_vertexBufferRegistry; }
    //GraphicsResourceRegistry* indexBufferRegistry() const { return m_indexBufferRegistry; }
    //GraphicsResourceRegistry* constantBufferRegistry() const { return m_constantBufferRegistry; }
    //GraphicsResourceRegistry* shaderPassRegistry() const { return m_shaderPassRegistry; }
    //GraphicsResourceRegistry* textureRegistry() const { return m_textureRegistry; }
    //GraphicsResourceRegistry* depthBufferRegistry() const { return m_depthBufferRegistry; }

    const std::unique_ptr<detail::GraphicsProfiler>& profiler() const { return m_profiler; }
    const Ref<detail::LinearAllocatorPageManager>& linearAllocatorPageManager() const { return m_linearAllocatorPageManager; }
    const Ref<detail::RenderingQueue>& renderingQueue() const { return m_renderingQueue; }
    //RenderingType renderingType() const { return RenderingType::Immediate; }
    //const Ref<RenderingCommandList>& primaryRenderingCommandList2() const { return m_primaryRenderingCommandList; }
    const Ref<detail::RenderTargetTextureCacheManager>& renderTargetTextureCacheManager() const { return m_renderTargetTextureCacheManager; }
    const Ref<detail::DepthBufferCacheManager>& depthBufferCacheManager() const { return m_depthBufferCacheManager; }
    const Ref<detail::FrameBufferCache>& frameBufferCache() const { return m_frameBufferCache; }
    const URef<detail::RenderPassCache>& renderPassCache() const { return m_renderPassCache; }
    detail::ObjectCache<String, Texture2D>* texture2DCache() { return &m_texture2DCache; }

    const List<IGraphicsResource*>& graphicsResources() const { return m_graphicsResources; }

    //int registerExtension(INativeGraphicsExtension* extension);
    //void unregisterExtension(INativeGraphicsExtension* extension);
    Ref<Texture> requestTexture(const detail::AssetPath& assetPath);

    Ref<Texture2D> loadTexture2D(const StringView& filePath);
    Ref<Texture2D> loadTexture2DFromOnMemoryData(const detail::AssetPath* baseDir, const StringView& filePath, std::function<Ref<Texture2D>(const detail::AssetRequiredPathSet*)> factory);
    Ref<Shader> loadShader(const StringView& filePath);

    const Ref<Texture2D>& blackTexture() const { return m_blackTexture; }
    const Ref<Texture2D>& whiteTexture() const { return m_whiteTexture; }
    const Ref<Texture2D>& defaultNormalMap() const { return m_defaultNormalMap; }
    const Ref<SamplerState>& defaultSamplerState() const { return m_defaultSamplerState; }
    const Ref<SamplerState>& linearSamplerState() const { return m_linearSamplerState; }
    const Ref<SamplerState>& pointClampSamplerState() const { return m_pointClampSamplerState; }
    const Ref<SamplerState>& linearClampSamplerState() const { return m_linearClampSamplerState; }

    //GraphicsCommandList* getOpenGLIntegrationCommandList();

    detail::StreamingBufferAllocatorManager* obtainVertexBufferStreamingAllocatorManager(size_t elementSize);
    detail::StreamingBufferAllocatorManager* obtainIndexBufferStreamingAllocatorManager(IndexBufferFormat format);

    static bool checkVulkanSupported();
    static void selectDefaultSystem(LNGraphicsBackend* api, WindowSystem* ws);

private:

    //void createOpenGLContext(const Settings& settings);
    //void createVulkanContext(const Settings& settings);
    //void createDirectX12Context(const Settings& settings);

    detail::AssetManager* m_assetManager;
    //PlatformManager* m_platformManager;
    detail::ShaderManager* m_shaderManager;
    URef<detail::MeshManager> m_meshManager;
    //Ref<IGraphicsDevice> m_deviceContext;
    Ref<CommandQueue> m_graphicsQueue;
    Ref<CommandQueue> m_computeQueue;
    Settings m_settings;

    URef<detail::GraphicsObjectRegistry> m_resourceRegistry;
    //URef<GraphicsResourceRegistry> m_vertexLayoutRegistry;
    //URef<GraphicsResourceRegistry> m_vertexBufferRegistry;
    //URef<GraphicsResourceRegistry> m_indexBufferRegistry;
    //URef<GraphicsResourceRegistry> m_constantBufferRegistry;
    //URef<GraphicsResourceRegistry> m_shaderPassRegistry;
    //URef<GraphicsResourceRegistry> m_textureRegistry;
    //URef<GraphicsResourceRegistry> m_depthBufferRegistry;

    std::unique_ptr<detail::GraphicsProfiler> m_profiler;
    Ref<detail::LinearAllocatorPageManager> m_linearAllocatorPageManager;
    Ref<detail::RenderingQueue> m_renderingQueue;
    Ref<detail::RenderTargetTextureCacheManager> m_renderTargetTextureCacheManager;
    Ref<detail::DepthBufferCacheManager> m_depthBufferCacheManager;
    Ref<detail::FrameBufferCache> m_frameBufferCache; // TODO: いらないかも
    URef<detail::RenderPassCache> m_renderPassCache;
    detail::ObjectCache<String, Texture2D> m_texture2DCache;
    detail::ObjectCache<String, Shader> m_shaderCache;
    List<IGraphicsResource*> m_graphicsResources;
    List<INativeGraphicsExtension*> m_extensions;

    Ref<Texture2D> m_blackTexture;
    Ref<Texture2D> m_whiteTexture;
    Ref<Texture2D> m_defaultNormalMap;
    Ref<SamplerState> m_defaultSamplerState;
    Ref<SamplerState> m_linearSamplerState;
    Ref<SamplerState> m_pointClampSamplerState;
    Ref<SamplerState> m_linearClampSamplerState;

    Ref<GraphicsCommandList> m_openglIntegrationCommandList;

    Array<URef<detail::StreamingBufferAllocatorManager>> m_vertexBufferStreamingAllocatorManager;
    Array<URef<detail::StreamingBufferAllocatorManager>> m_indexBufferStreamingAllocatorManager;
};

} // namespace ln
