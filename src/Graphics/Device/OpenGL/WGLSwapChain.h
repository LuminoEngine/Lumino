
#pragma once 

#include "../DeviceInterface.h"
#include "GLSwapChain.h"
#include "GLGraphicsDevice.h"
#include "GLTexture.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{
class WGLGraphicsDevice;
class WGLContext;

/// WGL 固有の ISwapChain の実装
class WGLSwapChain
	: public GLSwapChain
{
public:
	WGLSwapChain();
	virtual ~WGLSwapChain();

public:
	/// オブジェクト生成
	void Create(WGLGraphicsDevice* device, Platform::Window* window, WGLContext* parentContext);


public:
	// override ISwapChain
	virtual void Resize(const Size& size);
	//virtual ITexture* GetBackBuffer();
	//virtual void Begin();
	//virtual void End();
	virtual void Present(ITexture* colorBuffer);

	// override GLSwapChain
	//virtual void MakeCurrentContext();	// これは削除予定

private:
	WGLGraphicsDevice*	m_device;
	Platform::Window*	m_window;
	WGLContext*			m_context;
	//HWND	m_hWnd;
	//HDC		m_hDC;
	//HGLRC	m_hGLRC;
	//GLBackbufferRenderTarget*	m_backBuffer;
};

class WGLContext
	: public GLContext
{
public:
	WGLContext(WGLGraphicsDevice* device, Platform::Window* window, WGLContext* parentContext);
	virtual ~WGLContext();

	//virtual void MakeCurrentContext(bool attach);

	HDC GetDC() const { return m_hDC; }
	HGLRC GetGLRC() const { return m_hGLRC; }

private:
	HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hGLRC;
};

} // namespace Device
} // namespace Graphics
} // namespace Lumino
