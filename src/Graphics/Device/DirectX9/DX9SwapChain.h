
#pragma once 
#include "DX9Texture.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class DX9GraphicsDevice;

class DX9SwapChain
	: public ISwapChain
{
public:
	DX9SwapChain();
	virtual ~DX9SwapChain();
	void InitializeDefault(DX9GraphicsDevice* device, PlatformWindow* window, const SizeI& backBufferSize);
	void InitializeSub(DX9GraphicsDevice* device, PlatformWindow* window, const SizeI& backBufferSize);

public:

	/// この関数はデバイスが初期化された直後に呼ばれる
	void PostInitialize();

	/// バックバッファサイズの取得
	const SizeI& GetBackBufferSize() const { return m_backBufferSize; }

public:
	// override IDeviceObject
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	// override ISwapChain
	virtual ITexture* GetBackBuffer() { return m_backBuffer; }
	virtual void Resize(const SizeI& size);
	virtual void Present(ITexture* colorBuffer);

private:
	DX9GraphicsDevice*			m_graphicsDevice;
	IDirect3DSwapChain9*		m_dxSwapChain;
	PlatformWindow*				m_targetWindow;
	HWND						m_targetHWnd;
	SizeI						m_backBufferSize;
	DX9BackBufferTexture*		m_backBuffer;
	bool						m_isDefault;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
