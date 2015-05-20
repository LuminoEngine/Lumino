
#pragma once 

#define GLX_GLXEXT_LEGACY
#include <GL/glx.h>
#include <GL/glxext.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include "../DeviceInterface.h"
#include "GLGraphicsDevice.h"
#include "GLSwapChain.h"
#include "GLTexture.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{
class GLXGraphicsDevice;
class GLXContextObject;

/// GLX 固有の ISwapChain の実装
class GLXSwapChain
	: public GLSwapChain
{
public:
	GLXSwapChain(GLXGraphicsDevice* device, Platform::Window* window, GLXContextObject* parentContext);
	virtual ~GLXSwapChain();

public:
	// override ISwapChain
	virtual void Resize(const Size& size);
	virtual void Present(ITexture* colorBuffer);

private:
	GLXGraphicsDevice*	m_device;
	Platform::Window*	m_window;
	GLXContextObject*	m_context;
};

class GLXContextObject	// GLXContext は glx で使われているので 'Object' をつけておく
	: public GLContext
{
public:
	GLXContextObject(GLXGraphicsDevice* device, Platform::Window* window, GLXContextObject* parentContext);
	virtual ~GLXContextObject();
	
	::Display*	GetX11Display() { return m_x11Display; }
	::Window	GetX11Window() { return m_x11Window; }
	GLXContext	GetGLXContext() { return m_glxContext; }

private:
	::Display* 	m_x11Display;
	::Window	m_x11Window;
	GLXContext	m_glxContext;
};

} // namespace Device
} // namespace Graphics
} // namespace Lumino
