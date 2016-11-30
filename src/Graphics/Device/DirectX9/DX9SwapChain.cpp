
#include "../../../Internal.h"
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
{
}

//------------------------------------------------------------------------------
DX9SwapChain::~DX9SwapChain()
{
	OnLostDevice();
	LN_SAFE_RELEASE(m_backBuffer);
	LN_SAFE_RELEASE(m_targetWindow);
	LN_SAFE_RELEASE(m_graphicsDevice);
}

//------------------------------------------------------------------------------
void DX9SwapChain::InitializeDefault(DX9GraphicsDevice* device, PlatformWindow* window, const SizeI& backBufferSize)
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
void DX9SwapChain::InitializeSub(DX9GraphicsDevice* device, PlatformWindow* window, const SizeI& backBufferSize)
{
	InitializeDefault(device, window, backBufferSize);
	m_isDefault = false;
	PostInitialize();
}

//------------------------------------------------------------------------------
void DX9SwapChain::PostInitialize()
{
	OnResetDevice();
}

//------------------------------------------------------------------------------
void DX9SwapChain::OnLostDevice()
{
	// バックバッファ IDirect3DSurface9 の参照を外す
	if (m_backBuffer) {
		m_backBuffer->Reset(NULL);
	}

	LN_SAFE_RELEASE(m_dxSwapChain);
}

//------------------------------------------------------------------------------
void DX9SwapChain::OnResetDevice()
{
	LN_CHECK_STATE(m_dxSwapChain == nullptr);

	IDirect3DDevice9* dxDevice = m_graphicsDevice->GetIDirect3DDevice9();
	if (m_isDefault)
	{
		LN_COMCALL(dxDevice->GetSwapChain(0, &m_dxSwapChain));
	}
	else
	{
		D3DPRESENT_PARAMETERS pp = m_graphicsDevice->GetPresentParameters();
		pp.BackBufferWidth = m_backBufferSize.width;
		pp.BackBufferHeight = m_backBufferSize.height;
		LN_COMCALL(dxDevice->CreateAdditionalSwapChain(&pp, &m_dxSwapChain));
	}

	// バックバッファサーフェイスを保持
	IDirect3DSurface9* surface;
	//LN_COMCALL(m_graphicsDevice->GetIDirect3DDevice9()->GetRenderTarget(0, &surface));
	m_dxSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &surface);
	m_backBuffer->Reset(surface);
	LN_SAFE_RELEASE(surface);
}

//------------------------------------------------------------------------------
void DX9SwapChain::Resize(const SizeI& size)
{
	m_backBufferSize = size;

	// ソフト的にデバイスロスト状態にして、
	// 次回のロストチェック時にバックバッファを作り直す。
	// (描画スレッドを分けている場合の対応)
	m_graphicsDevice->SetDeviceLostFlag();
}

//------------------------------------------------------------------------------
void DX9SwapChain::Present(ITexture* /*colorBuffer*/)
{
	if (m_graphicsDevice->GetDeviceState() != DeviceState_Enabled) {
		return;
	}

	// ここでバックバッファに戻しておく
	DX9Renderer* r = static_cast<DX9Renderer*>(m_graphicsDevice->GetRenderer());
	r->SetRenderTarget(0, m_backBuffer);
	r->SetDepthBuffer(NULL);
	

	//D3DCOLOR dxc = D3DCOLOR_ARGB(255, 255, 0, 0);
	//LN_COMCALL(dxDevice->Clear(0, NULL, D3DCLEAR_TARGET, dxc, 1.0f, 0));

	// 転送
	//IDirect3DDevice9* dxDevice = m_graphicsDevice->GetIDirect3DDevice9();
	//HRESULT hr = dxDevice->Present(NULL, NULL, m_targetHWnd, NULL);
	HRESULT hr = m_dxSwapChain->Present(NULL, NULL, m_targetHWnd, NULL, 0);

	// デバイスロスト確認
	if (hr == D3DERR_DEVICELOST)
	{
		IDirect3DDevice9* dxDevice = m_graphicsDevice->GetIDirect3DDevice9();
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
			LN_THROW(0, COMException, hr);
			break;
		}
	}

	m_graphicsDevice->GCDeviceResource();







	Bitmap* bmp = m_backBuffer->Lock();



	int uWidth = 640, uHeight = 480;               // pngファイルの縦横サイズを得るための変数
	int nWidth = 640, nHeight = 480;               // pngファイルの縦横サイズを得るための変数

	// ==========================
	// ビットマップハンドルの作成
	BITMAPV5HEADER bi;
	void *lpBits;

	ZeroMemory(&bi, sizeof(BITMAPV5HEADER));
	bi.bV5Size = sizeof(BITMAPV5HEADER);
	bi.bV5Width = nWidth;
	bi.bV5Height = nHeight;
	bi.bV5Planes = 1;
	bi.bV5BitCount = 32;
	bi.bV5Compression = BI_BITFIELDS;
	bi.bV5RedMask = 0x00FF0000;
	bi.bV5GreenMask = 0x0000FF00;
	bi.bV5BlueMask = 0x000000FF;
	bi.bV5AlphaMask = 0xFF000000;

	HDC hdc = GetDC(m_targetHWnd);
	HBITMAP hBitmap = CreateDIBSection(hdc, (BITMAPINFO *)&bi, DIB_RGB_COLORS,
		(void **)&lpBits, NULL, (DWORD)0);
	ReleaseDC(m_targetHWnd, hdc);
	DWORD *lpdwPixel = (DWORD *)lpBits;
	DWORD x, y;
	//png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	// 画像データを順次読み出し
	for (x = 0; x<nWidth; ++x) {
		for (y = 0; y<nHeight; ++y) { // RGBA→ARGB 変換
			Color32 c = bmp->GetPixel(x, y);
			DWORD r = c.r;//row_pointers[y][4 * x];
			DWORD g = c.g;
			DWORD b = c.g;
			DWORD alpha = c.a;

			// [↓あえて冗長に作ってあります]
			if (alpha == 255) {
				lpdwPixel[(nHeight - y - 1)*nWidth + x] = (r << 16) | (g << 8) | (b << 0) | (alpha << 24);
			}
			else if (alpha == 0) {
				lpdwPixel[(nHeight - y - 1)*nWidth + x] = 0;
			}
			else {
				r = r * alpha / 255;
				g = g * alpha / 255;
				b = b * alpha / 255;
				lpdwPixel[(nHeight - y - 1)*nWidth + x] = (r << 16) | (g << 8) | (b << 0) | (alpha << 24);
			}
		}
	}




	// ==========================
	// レイヤードウィンドウの設定
	// ==========================

	// 各種デバイスコンテキストの取得
	HDC hmemdc, hsdc;
	hsdc = GetDC(0);                      // デスクトップのデバイスコンテキスト（色情報指定用）
	hdc = GetDC(m_targetHWnd);                   // このウィンドウのデバイスコンテキスト
	hmemdc = CreateCompatibleDC(hdc);       // hdcの互換デバイスコンテキスト

	POINT wndPos;
	SIZE  wndSize;
	RECT  rc;

	// レイヤードウィンドウの画面位置とサイズ
	GetWindowRect(m_targetHWnd, &rc);
	wndPos.x = rc.left;
	wndPos.y = rc.top;
	wndSize.cx = uWidth;
	wndSize.cy = uHeight;

	// デバイスコンテキストにおけるレイヤの位置
	POINT po;
	po.x = po.y = 0;

	// レイヤードウィンドウの指定
	BLENDFUNCTION blend;
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255; // 不透明度（レイヤードウィンドウ全体のアルファ値）
	blend.AlphaFormat = AC_SRC_ALPHA;

	// 画像を描画をする
	HGDIOBJ hOldObj = SelectObject(hmemdc, hBitmap);
	BitBlt(hdc, 0, 0, uWidth, uHeight, hmemdc, 0, 0, SRCCOPY | CAPTUREBLT); // レイヤードウィンドウではCAPTUREBLTが必要

																			// レイヤードウィンドウの位置、サイズ、形、内容、透明度を更新
	if (0 == UpdateLayeredWindow(m_targetHWnd, hsdc, &wndPos, &wndSize, hmemdc, &po, 0, &blend, ULW_ALPHA)) {
		TCHAR strErrMes[80];
		DWORD err = GetLastError();

		wsprintf(strErrMes, _T("UpdateLayeredWindow失敗：エラーコード＝%d"), err);
		MessageBox(m_targetHWnd, strErrMes, _T("エラー"), MB_OK | MB_ICONSTOP);
		//DestroyWindow(hWnd);
	}

	SelectObject(hmemdc, hOldObj);
	DeleteDC(hmemdc);
	ReleaseDC(m_targetHWnd, hdc);
	ReleaseDC(0, hsdc);

	DeleteObject(hBitmap);

	m_backBuffer->Unlock();
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
