#pragma once

namespace ln {
namespace detail {

// IRenderPass のライフサイクル (createとdispose) はこの中で管理する
class NativeRenderPassCache {
public:
    struct FindKey {
        std::array<RHIResource*, MaxMultiRenderTargets> renderTargets = {};
        RHIResource* depthBuffer = nullptr;
        ClearFlags clearFlags = ClearFlags::All;
        Color clearColor = Color(0, 0, 0, 0);
        float clearDepth = 1.0f;
        uint8_t clearStencil = 0x00;
    };

    NativeRenderPassCache(IGraphicsDevice* device);
    void clear();
    IRenderPass* findOrCreate(const FindKey& key);
    void invalidateRenderTarget(RHIResource* renderTarget);
    void invalidateDepthBuffer(RHIResource* depthBuffer);
    void release2(IRenderPass* value);
    static uint64_t computeHash(const FindKey& key);

private:
    struct Entry {
        int referenceCount = 0;
        Ref<IRenderPass> value;
    };

    IGraphicsDevice* m_device;
    std::unordered_map<uint64_t, Entry> m_hashMap;
};

// IPipeline のライフサイクル (createとdispose) はこの中で管理する。
// IShaderPass または IRenderPass の dispose 時に、invalidate が呼ばれ、関係している IPipeline が削除される。
class NativePipelineCache {
public:
    using FindKey = DevicePipelineStateDesc;

    NativePipelineCache(IGraphicsDevice* device);
    void clear();
    IPipeline* findOrCreate(const FindKey& key);
    void invalidate(IVertexDeclaration* value);
    void invalidate(IRenderPass* value);
    void invalidate(IShaderPass* value);
    static uint64_t computeHash(const FindKey& key);

private:
    IGraphicsDevice* m_device;
    std::unordered_map<uint64_t, Ref<IPipeline>> m_hashMap;
};

class NativeCommandListPool {
public:
    NativeCommandListPool(IGraphicsDevice* device);
    ICommandList* get(); // get or wait

private:
    IGraphicsDevice* m_device;
    std::unordered_map<uint64_t, Ref<IRenderPass>> m_hashMap;

    /*
	UseCases:

	普通に使うとき:
		auto commandList = GraphicsCommandList::get(swapchain);
		commandList->・・・Rendering モジュールで使う
		graphicsQueue->submit(commandList);

	スポット的に使うとき(Compute や RenderTargetあらかじめ作ったり)：
		auto commandList = GraphicsCommandList::get();
		commandList->・・・
		computeQueue->submit(commandList);

	他Engine組み込み (UIFrameWindowあたり):
		IGraphicsDevice* device = ...;
		auto commandListRHI = device->getCommandList(VkCommandList など);
		auto commandList = GraphicsCommandList::get(commandListRHI);
		commandList->・・・Rendering モジュールで使う
	*/
};

} // namespace detail
} // namespace ln
