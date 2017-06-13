
#include "../../Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/Platform/PlatformSupport.h>
#include "DX9GraphicsDevice.h"
#include "DX9SwapChain.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//==============================================================================
// DX9SwapChain
//==============================================================================

//------------------------------------------------------------------------------
DX9SwapChain::DX9SwapChain()
	: m_graphicsDevice(nullptr)
	, m_dxSwapChain(nullptr)
	, m_targetWindow(nullptr)
	, m_targetHWnd(nullptr)
	, m_backBufferSize()
	, m_backBuffer(nullptr)
	, m_isDefault(true)
	, m_dirtySize(false)
{
}

//------------------------------------------------------------------------------
DX9SwapChain::~DX9SwapChain()
{
	onLostDevice();
	LN_SAFE_RELEASE(m_backBuffer);
	LN_SAFE_RELEASE(m_targetWindow);
	LN_SAFE_RELEASE(m_graphicsDevice);
}

//------------------------------------------------------------------------------
void DX9SwapChain::initializeDefault(DX9GraphicsDevice* device, PlatformWindow* window, const SizeI& backBufferSize)
{
	LN_REFOBJ_SET(m_graphicsDevice, device);
	LN_REFOBJ_SET(m_targetWindow, window);
	m_targetHWnd = PlatformSupport::GetWindowHandle(m_targetWindow);
	m_backBufferSize = backBufferSize;
	m_isDefault = true;

	// デバイスロスト発生時も getBackbuffer() 等で一応正常なポインタを
	// 返すようにするため、実態は常に持っておく。ラップするテクスチャは復帰時に再取得。
	m_backBuffer = LN_NEW DX9BackBufferTexture(m_graphicsDevice);
}

//------------------------------------------------------------------------------
void DX9SwapChain::initializeSub(DX9GraphicsDevice* device, PlatformWindow* window, const SizeI& backBufferSize)
{
	initializeDefault(device, window, backBufferSize);
	m_isDefault = false;
	postInitialize();
}

//------------------------------------------------------------------------------
void DX9SwapChain::postInitialize()
{
	onResetDevice();
}

//------------------------------------------------------------------------------
void DX9SwapChain::onLostDevice()
{
	// バックバッファ IDirect3DSurface9 の参照を外す
	if (m_backBuffer) {
		m_backBuffer->reset(NULL);
	}

	LN_COM_SAFE_RELEASE(m_dxSwapChain);
}

//------------------------------------------------------------------------------
void DX9SwapChain::onResetDevice()
{
	if (LN_CHECK_STATE(m_dxSwapChain == nullptr)) return;

	IDirect3DDevice9* dxDevice = m_graphicsDevice->getIDirect3DDevice9();
	if (m_isDefault)
	{
		LN_COMCALL(dxDevice->GetSwapChain(0, &m_dxSwapChain));
	}
	else
	{
		D3DPRESENT_PARAMETERS pp = m_graphicsDevice->getPresentParameters();
		pp.BackBufferWidth = m_backBufferSize.width;
		pp.BackBufferHeight = m_backBufferSize.height;
		LN_COMCALL(dxDevice->CreateAdditionalSwapChain(&pp, &m_dxSwapChain));
	}

	// バックバッファサーフェイスを保持
	IDirect3DSurface9* surface;
	//LN_COMCALL(m_graphicsDevice->getIDirect3DDevice9()->getRenderTarget(0, &surface));
	m_dxSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &surface);
	m_backBuffer->reset(surface);
	LN_COM_SAFE_RELEASE(surface);
}

//------------------------------------------------------------------------------
void DX9SwapChain::resize(const SizeI& size)
{
	m_backBufferSize = size;

	if (m_isDefault)
	{
		// ソフト的にデバイスロスト状態にして、
		// 次回のロストチェック時にバックバッファを作り直す。
		// (描画スレッドを分けている場合の対応)
		m_graphicsDevice->setDeviceLostFlag();
	}
	else
	{
		m_dirtySize = true;
	}
}

//------------------------------------------------------------------------------
void DX9SwapChain::present(ITexture* /*colorBuffer*/)
{
	if (m_graphicsDevice->getDeviceState() != DeviceState_Enabled) {
		return;
	}

	// ここでバックバッファに戻しておく
	DX9Renderer* r = static_cast<DX9Renderer*>(m_graphicsDevice->getRenderer());
	r->setRenderTarget(0, m_backBuffer);
	r->setDepthBuffer(NULL);
	

	//IDirect3DDevice9* dxDevice = m_graphicsDevice->getIDirect3DDevice9();
	//D3DCOLOR dxc = D3DCOLOR_ARGB(255, 255, 0, 0);
	//LN_COMCALL(dxDevice->Clear(0, NULL, D3DCLEAR_TARGET, dxc, 1.0f, 0));

	// 転送
	//
	//HRESULT hr = dxDevice->present(NULL, NULL, m_targetHWnd, NULL);
	HRESULT hr = m_dxSwapChain->Present(NULL, NULL, m_targetHWnd, NULL, 0);

	// デバイスロスト確認
	if (hr == D3DERR_DEVICELOST)
	{
		IDirect3DDevice9* dxDevice = m_graphicsDevice->getIDirect3DDevice9();
		hr = dxDevice->TestCooperativeLevel();
		switch (hr)
		{
			// デバイスロスト
		case D3DERR_DEVICELOST:
			// TODO: ダメなロスト
			//::SleepEx( 1000, true );      
			break;
			// デバイスロスト：リセット可能状態
		case D3DERR_DEVICENOTRESET:
			m_graphicsDevice->setDeviceLostFlag();
			break;
		default:
			LN_THROW(0, COMException, hr);
			break;
		}
	}
	else
	{
		if (m_dirtySize)
		{
			onLostDevice();
			onResetDevice();
		}
	}

	m_graphicsDevice->gcDeviceResource();
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
