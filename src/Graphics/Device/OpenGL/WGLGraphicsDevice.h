
#pragma once 
#include "GLGraphicsDevice.h"
#include "GLSwapChain.h"
#include "GLContext.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class WGLGraphicsDevice;

// WGL 固有の GLContext の実装
class WGLContext
	: public GLContext
{
public:
	WGLContext(WGLGraphicsDevice* device, PlatformWindow* window, WGLContext* parentContext);
	virtual ~WGLContext();
	virtual void SwapBuffers() override;

	HDC GetDC() const { return m_hDC; }
	HGLRC GetGLRC() const { return m_hGLRC; }

private:
	HWND	m_hWnd;
	HDC		m_hDC;
	HGLRC	m_hGLRC;
};

// WGL 固有の GLGraphicsDevice の実装
class WGLGraphicsDevice
	: public GLGraphicsDevice
{
public:
	WGLGraphicsDevice();
	virtual ~WGLGraphicsDevice();

	// GLGraphicsDevice interface
	virtual RefPtr<GLContext> InitializeMainContext(const ConfigData& configData) override;
	virtual RefPtr<GLContext> CreateContext(PlatformWindow* window) override;
	virtual void MakeCurrentContext(GLContext* context) override;

	static PFNWGLGETEXTENSIONSSTRINGEXTPROC		GetExtensionsStringEXT;
	static PFNWGLGETEXTENSIONSSTRINGARBPROC		GetExtensionsStringARB;
	static PFNWGLCREATECONTEXTATTRIBSARBPROC	CreateContextAttribsARB;

private:
	int CheckPlatformExtensionSupported(const char* extension);
	bool CheckContainsExtensionString(const char* string, const GLubyte* extensions);

	bool			m_glInited;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
