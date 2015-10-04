
#include "../../../Internal.h"
#include <Lumino/Platform/PlatformSupport.h>
#include "DX9GraphicsDevice.h"
#include "DX9SwapChain.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//=============================================================================
// DX9SwapChain
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9SwapChain::DX9SwapChain(DX9GraphicsDevice* device, Platform::Window* window, const Size& backBufferSize)
	: m_graphicsDevice(NULL)
	, m_targetWindow(NULL)
	, m_targetHWnd(NULL)
	, m_backBufferSize(backBufferSize)
	, m_backBuffer(NULL)
{
	LN_REFOBJ_SET(m_graphicsDevice, device);
	LN_REFOBJ_SET(m_targetWindow, window);
	m_targetHWnd = Platform::PlatformSupport::GetWindowHandle(m_targetWindow);

	// デバイスロスト発生時も getBackbuffer() 等で一応正常なポインタを
	// 返すようにするため、実態は常に持っておく。ラップするテクスチャは復帰時に再取得。
	m_backBuffer = LN_NEW DX9BackBufferTexture(m_graphicsDevice);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9SwapChain::~DX9SwapChain()
{
	OnLostDevice();
	LN_SAFE_RELEASE(m_backBuffer);
	LN_SAFE_RELEASE(m_targetWindow);
	LN_SAFE_RELEASE(m_graphicsDevice);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9SwapChain::PostInitialize()
{
	OnResetDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9SwapChain::OnLostDevice()
{
	// バックバッファ IDirect3DSurface9 の参照を外す
	if (m_backBuffer) {
		m_backBuffer->Reset(NULL);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9SwapChain::OnResetDevice()
{
	// バックバッファサーフェイスを保持
	IDirect3DSurface9* surface;
	LN_COMCALL(m_graphicsDevice->GetIDirect3DDevice9()->GetRenderTarget(0, &surface));
	m_backBuffer->Reset(surface);
	LN_SAFE_RELEASE(surface);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9SwapChain::Resize(const Size& size)
{
	m_backBufferSize = size;

	// ソフト的にデバイスロスト状態にして、
	// 次回のロストチェック時にバックバッファを作り直す。
	// (描画スレッドを分けている場合の対応)
	m_graphicsDevice->SetDeviceLostFlag();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9SwapChain::Present(ITexture* colorBuffer)
{
	if (m_graphicsDevice->GetDeviceState() != DeviceState_Enabled) {
		return;
	}

	// ここでバックバッファに戻しておく
	DX9Renderer* r = static_cast<DX9Renderer*>(m_graphicsDevice->GetRenderer());
	r->SetRenderTarget(0, m_backBuffer);
	r->SetDepthBuffer(NULL);

	// バックエンドにコピー

		//GeometryRenderer* gr = this->mGraphicsDevice->getManager()->getGeometryRenderer();
		//const LVector2& size = this->mBackendBuffer->getSize();
		//const LVector2& backbufferSize = this->mBackbuffer->getSize();

		//// 0.5 オフセット
		//LVector2 viewOffset(0.5 / size.X, 0.5 / size.Y);

		//// 単純コピー用のステートにする
		//r->setRenderState(LRenderState::BUFFER_COPY_STATE, true);

		//// コピー
		//r->beginScene();
		//gr->setMatrix(mBackbufferOffset);
		//gr->setViewSize(size);
		//gr->setTexture(this->mBackbuffer);
		//gr->begin(GeometryRenderer::PASS_BASIC);
		//gr->drawSquare(
		//	0.0f, 0.0f, 0xffffffff, 0.0f + viewOffset.X, 0.0f + viewOffset.Y,
		//	backbufferSize.X, 0.0f, 0xffffffff, 1.0f + viewOffset.X, 0.0f + viewOffset.Y,
		//	0.0f, backbufferSize.Y, 0xffffffff, 0.0f + viewOffset.X, 1.0f + viewOffset.Y,
		//	backbufferSize.X, backbufferSize.Y, 0xffffffff, 1.0f + viewOffset.X, 1.0f + viewOffset.Y);
		//gr->end();
		//r->endScene();
	//}

	// シーン終了通知
//	r->TryEndScene();

	// 転送
	IDirect3DDevice9* dxDevice = m_graphicsDevice->GetIDirect3DDevice9();
	HRESULT hr = dxDevice->Present(NULL, NULL, m_targetHWnd, NULL);

	// デバイスロスト確認
	if (hr == D3DERR_DEVICELOST)
	{
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
			m_graphicsDevice->SetDeviceLostFlag();
			break;
		default:
			break;
		}
	}

	m_graphicsDevice->GCDeviceResource();








	//IDirect3DDevice9* dxDevice = this->mGraphicsDevice->getIDirect3DDevice9();
	////こっからスクリーンショット
	//IDirect3DSurface9 *pSurface;

	//int deskTopX = GetSystemMetrics(SM_CXSCREEN);
	//int deskTopY = GetSystemMetrics(SM_CYSCREEN);
	//HRESULT hr = dxDevice->CreateOffscreenPlainSurface(deskTopX, deskTopY, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pSurface, NULL);
	//hr = dxDevice->GetFrontBufferData(0, pSurface);
	//RECT rect;
	//GetClientRect(mTargetWindowHandle, &rect);

	//POINT pt = { 0, 0 };
	//ClientToScreen(mTargetWindowHandle, &pt);
	//rect.left += pt.x;
	//rect.right += pt.x;
	//rect.top += pt.y;
	//rect.bottom += pt.y;
	////GetWindowRect(mTargetWindowHandle, &rect);
	////if (windowed == TRUE)
	//{
	//	//rect.bottom -= GetSystemMetrics(SM_CYDLGFRAME);
	//	//rect.top += GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION);
	//	//rect.left += GetSystemMetrics(SM_CXDLGFRAME);
	//	//rect.right -= GetSystemMetrics(SM_CXDLGFRAME);
	//}
	//hr = D3DXSaveSurfaceToFile(filePath, D3DXIFF_PNG, pSurface, NULL, &rect);
	//SAFE_RELEASE(pSurface);




	//if (mBackbufferResizeMode == BackbufferResizeMode_Resize ||
	//	mBackbufferResizeMode == BackbufferResizeMode_ResizeWithLetterBox)


		//{
		//	// バックバッファの再構築が必要な時はフラグON。
		//	// 複数スレッドで動作させている時は同期フェーズ等、
		//	// 然るべき時にデバイスリセットを行う。
		//	if (mTargetWindow->getSize() != mWindowSize)
		//	{
		//		// フルスクリーンへ切り替えるときもここを通る
		//		mGraphicsDevice->setDeviceLostFlag();
		//	}
		//}
	//else
	//{
	//	if (mTargetWindow->getSize() != mWindowSize)
	//	{
	//		mWindowSize = mTargetWindow->getSize();
	//		updateViewBoxMatrix();
	//	}
	//}
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
