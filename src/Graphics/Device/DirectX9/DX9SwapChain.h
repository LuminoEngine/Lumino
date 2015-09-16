
#pragma once 

#include "../DeviceInterface.h"
#include "DX9Texture.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Device
{
class DX9GraphicsDevice;

class DX9SwapChain
	: public ISwapChain
{
public:
	DX9SwapChain(DX9GraphicsDevice* device, Platform::Window* window, const Size& backBufferSize);
	virtual ~DX9SwapChain();

public:

	/// この関数はデバイスが初期化された直後に呼ばれる
	void PostInitialize();

	/// バックバッファサイズの取得
	const Size& GetBackBufferSize() const { return m_backBufferSize; }

public:
	// override IDeviceObject
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	// override ISwapChain
	virtual ITexture* GetBackBuffer() { return m_backBuffer; }
	virtual void Resize(const Size& size);
	virtual void Present(ITexture* colorBuffer);

private:
	DX9GraphicsDevice*			m_graphicsDevice;
	Platform::Window*			m_targetWindow;
	HWND						m_targetHWnd;
	Size						m_backBufferSize;
	DX9BackBufferTexture*		m_backBuffer;
};

} // namespace Device
LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
