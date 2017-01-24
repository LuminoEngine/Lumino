
#pragma once 

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xlocale.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "GLGraphicsDevice.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{
class GLXSwapChain;
class GLXContextObject;

/// GLX 固有の GLGraphicsDevice の実装
class GLXGraphicsDevice
	: public GLGraphicsDevice
{
public:
	GLXGraphicsDevice();
	~GLXGraphicsDevice();

public:
	/// 描画を行うスレッドで初期化する必要がある
	void Initialize(const GraphicsDeviceConfigData& configData);

	/// MakeCurrentContext
	//void MakeCurrentContext(GLXContextObject* context);

public:
	// override IGraphicsDevice
	virtual ISwapChain* GetDefaultSwapChain();
	virtual ISwapChain* CreateSwapChain(Platform::Window* window);
	virtual void AttachRenderingThread();
	virtual void DetachRenderingThread();
	// override GLGraphicsDevice
	virtual GLContext* GetMainContext();
	virtual GLContext* GetMainRenderingContext();
	virtual void MakeCurrentContext(GLContext* context);

public:
	static void* GetGLXProcAddress(const char* procName);
	static PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;

private:
	int CheckPlatformExtensionSupported(const char* extension);
	bool CheckContainsExtensionString(const char* string, const GLubyte* extensions);

private:
	::Display*			m_x11Display;
	GLXContextObject*	m_mainContext;
	GLXContextObject*	m_mainRenderingContext;
	GLXSwapChain*		m_defaultSwapChain;
};

} // namespace Device
} // namespace Graphics
} // namespace Lumino
