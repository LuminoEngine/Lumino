
#include "Internal.hpp"
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoPlatform/PlatformSupport.hpp>
#include "DX12DeviceContext.hpp"
#include "DX12Textures.hpp"
#include "DX12SwapChain.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// DX12SwapChain

DX12SwapChain::DX12SwapChain()
    : m_device(nullptr)
    , m_backbufferCount(0)
    , m_frameIndex(0)
{
}

Result DX12SwapChain::init(DX12Device* deviceContext, PlatformWindow* window, const SizeI& backbufferSize)
{
    LN_DCHECK(deviceContext);
    m_device = deviceContext;
    m_backbufferCount = DX12Device::BackBufferCount;
    m_backbufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

    HWND hWnd = (HWND)PlatformSupport::getWin32WindowHandle(window);
    HINSTANCE hInstance = ::GetModuleHandle(NULL);

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferDesc.Width = backbufferSize.width;
    swapChainDesc.BufferDesc.Height = backbufferSize.height;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = m_backbufferFormat;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = m_backbufferCount;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    IDXGIFactory6* dxgiFactory = m_device->dxgiFactory();
    ComPtr<IDXGISwapChain> swapChain;
    HRESULT hr = dxgiFactory->CreateSwapChain(m_device->dxCommandQueue(), &swapChainDesc, &swapChain);
    if (FAILED(hr)) {
        LN_ERROR("CreateSwapChain failed.");
        return err();
    }

    if (FAILED(swapChain.As(&m_dxgiSwapChain))) {
        LN_ERROR("Cast to IDXGISwapChain3 failed.");
        return err();
    }

    // Create a RTV for each frame.
    if (!createSwapChainResources()) {
        return err();
    }

    // Barrior CommandList
    for (int i = 0; i < m_backbufferCount; i++) {
        Ref<DX12CommandListCore> list = makeRef<DX12CommandListCore>();
        if (!list->init(m_device)) {
            return err();
        }
        m_presentBarriorCommandLists.push_back(list);
    }

    {
        ID3D12Device* dxDevice = m_device->device();

        m_fenceValues.resize(m_backbufferCount);

        if (FAILED(dxDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)))) {
            LN_ERROR("CreateFence failed.");
            return err();
        }

        m_fenceEvent = ::CreateEvent(0, 0, 0, 0);
        if (!m_fenceEvent) {
            LN_ERROR("CreateEvent failed.");
            return err();
        }
    }

	return ok();
}

void DX12SwapChain::dispose()
{
    if (m_fenceEvent) {
        ::CloseHandle(m_fenceEvent);
        m_fenceEvent = nullptr;
    }
    m_fence.Reset();
    for (auto& i : m_presentBarriorCommandLists) {
        i->dispose();
    }
    disposeSwapChainResources();
    m_dxgiSwapChain.Reset();
    ISwapChain::dispose();
}

uint32_t DX12SwapChain::getBackbufferCount()
{
    return m_backbufferCount;
}

void DX12SwapChain::acquireNextImage(int* outIndex)
{
    // GetCurrentBackBufferIndex() は Present() の呼び出しで次へ進む
    m_frameIndex = m_dxgiSwapChain->GetCurrentBackBufferIndex();
    *outIndex = m_frameIndex;


    // 以前の Present が終了するまで待つ。
    // [2021/1/28] 多分不要なのだが、これ or Present の後の Signal が無いと
    // Present用の resourceBarrior で使う CommandAllocator::Reset で
    // ID3D12CommandAllocator Object' is being reset before previous executions associated with the allocator have completed.
    // というデバッグレイヤーのエラーが発生したり、SwapChain の Release で落ちたりしていた。
    waitForCurrentFrameFence();
}

RHIResource* DX12SwapChain::getRenderTarget(int imageIndex) const
{
	return m_renderTargets[imageIndex];
}

Result DX12SwapChain::resizeBackbuffer(uint32_t width, uint32_t height)
{
    // 現在のフレーム、つまり、最後の Present が終了するまで待つ。
    // (このため、バックバッファのリサイズは present の後、acquireNextImage() の前に行う必要がある)
    if (!waitForCurrentFrameFence()) {
        return err();
    }

    disposeSwapChainResources();

    HRESULT hr = m_dxgiSwapChain->ResizeBuffers(m_backbufferCount, width, height, m_backbufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
    if (FAILED(hr)) {
        LN_ERROR("ResizeBuffers failed.");
        return err();
    }

    if (!createSwapChainResources()) {
        return err();
    }

	return ok();
}

void DX12SwapChain::present()
{
    ID3D12CommandQueue* dxCommandQueue = m_device->dxCommandQueue();

    // バックバッファ RenderTarget の Resource State を Present へ変更する。
    // Barrior を張るのに CommandList が必要だが、管理の複雑さを回避するため、
    // 描画にだけ使いたい DX12CommandList とは独立させて、いる。
    // 同じ GraphicsQueue で ExecuteCommandLists() することで、実行順序は保証される。
    {
        DX12CommandListCore* commandList = m_presentBarriorCommandLists[m_frameIndex];
        if (!commandList->reset()) {
            return;
        }

        m_renderTargets[m_frameIndex]->image()->resourceBarrior(commandList->dxCommandList(), D3D12_RESOURCE_STATE_PRESENT);

        if (!commandList->close()) {
            return;
        }

        ID3D12CommandList* commandLists[] = { commandList->dxCommandList() };
        dxCommandQueue->ExecuteCommandLists(1, commandLists);
    }

    const bool waitVSync = false;   // TODO:
    auto hr = m_dxgiSwapChain->Present(waitVSync ? 1 : 0, 0);
    if (FAILED(hr)) {
        LN_ERROR("Present failed.");
        return;
    }

    // Schedule a Signal command in the queue.
    m_fenceValues[m_frameIndex] = m_device->m_fenceValue;
    const UINT64 currentFenceValue = m_fenceValues[m_frameIndex];
    if (FAILED(dxCommandQueue->Signal(m_fence.Get(), currentFenceValue))) { // GPU 側でこのコマンドが実行されたとき、m_fence の値を currentFenceValue に設定する
        LN_ERROR("Signal failed.");
        return;
    }
    m_device->m_fenceValue++;
}

bool DX12SwapChain::waitForCurrentFrameFence()
{
    const UINT64 fenceValue = m_fenceValues[m_frameIndex];
    if (fenceValue > 0) {
        // If the next frame is not ready to be rendered yet, wait until it is ready.
        if (m_fence->GetCompletedValue() < fenceValue)
        {
            if (FAILED(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent))) {
                LN_ERROR("SetEventOnCompletion failed.");
                return false;
            }
            WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
        }
    }
    return true;
}

bool DX12SwapChain::createSwapChainResources()
{
    for (int i = 0; i < m_backbufferCount; i++) {
        ComPtr<ID3D12Resource> dxRenderTarget;
        if (FAILED(m_dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&dxRenderTarget)))) {
            LN_ERROR("GetBuffer failed.");
            return false;
        }

        Ref<DX12RenderTarget> wrapper = makeRef<DX12RenderTarget>();
        if (!wrapper->init(m_device, dxRenderTarget)) {
            return false;
        }
        wrapper->RHIDeviceObject::m_device = m_device;
        wrapper->m_objectId = m_device->m_objectNextId++;

        m_renderTargets.push_back(wrapper);
    }

    m_frameIndex = m_dxgiSwapChain->GetCurrentBackBufferIndex();

    return true;
}

void DX12SwapChain::disposeSwapChainResources()
{
    for (auto& i : m_renderTargets) {
        i->dispose();
    }
    m_renderTargets.clear();
}

} // namespace detail
} // namespace ln
