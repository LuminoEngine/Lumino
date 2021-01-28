﻿
#pragma once
#include "DX12Helper.hpp"

namespace ln {
namespace detail {
class DX12RenderTarget;

class DX12SwapChain
	: public ISwapChain
{
public:
	DX12SwapChain();
	Result init(DX12Device* deviceContext, PlatformWindow* window, const SizeI& backbufferSize);
    void dispose() override;
    uint32_t getBackbufferCount() override;
    void acquireNextImage(int* outImageIndex) override;
	ITexture* getRenderTarget(int imageIndex) const override;
	Result resizeBackbuffer(uint32_t width, uint32_t height) override;
	void present() override;

private:
    DX12Device* m_device;
    ComPtr<IDXGISwapChain3> m_dxgiSwapChain;
    uint32_t m_backbufferCount;
    uint32_t m_frameIndex;
    std::vector<Ref<DX12RenderTarget>> m_renderTargets;
    std::vector<Ref<DX12CommandListCore>> m_presentBarriorCommandLists;
    std::vector<UINT64> m_fenceValues;
    ComPtr<ID3D12Fence> m_fence;
    HANDLE m_fenceEvent;
};

} // namespace detail
} // namespace ln
