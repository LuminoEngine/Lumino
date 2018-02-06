
#pragma once 
#include "DX9Texture.h"

LN_NAMESPACE_BEGIN
namespace Driver
{
class DX9GraphicsDevice;

class DX9SwapChain
	: public ISwapChain
{
public:
	DX9SwapChain();
	virtual ~DX9SwapChain();
	void initializeDefault(DX9GraphicsDevice* device, PlatformWindow* window, const SizeI& backBufferSize);
	void initializeSub(DX9GraphicsDevice* device, PlatformWindow* window, const SizeI& backBufferSize);

public:

	/// この関数はデバイスが初期化された直後に呼ばれる
	void postInitialize();

	/// バックバッファサイズの取得
	const SizeI& getBackBufferSize() const { return m_backBufferSize; }

public:
	// override IDeviceObject
	virtual void onLostDevice();
	virtual void onResetDevice();

	// override ISwapChain
	virtual ITexture* getBackBuffer() { return m_backBuffer; }
	virtual void resize(const SizeI& size);
	virtual void present(ITexture* colorBuffer);

private:
	DX9GraphicsDevice*			m_graphicsDevice;
	IDirect3DSwapChain9*		m_dxSwapChain;
	PlatformWindow*				m_targetWindow;
	HWND						m_targetHWnd;
	SizeI						m_backBufferSize;
	DX9BackBufferTexture*		m_backBuffer;
	bool						m_isDefault;
	bool						m_dirtySize;
};

} // namespace Driver
LN_NAMESPACE_END
