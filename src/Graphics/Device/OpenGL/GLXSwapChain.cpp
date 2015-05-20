
/*
[OpenGL] FrameBufferとRenderBufferについてメモ
http://qiita.com/edo_m18/items/95483cabf50494f53bb5
*/

#include "../../../Internal.h"
#include <Lumino/Platform/PlatformSupport.h>
#include "GLXGraphicsDevice.h"
#include "GLXSwapChain.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{

//=============================================================================
// GLXSwapChain
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLXSwapChain::GLXSwapChain(GLXGraphicsDevice* device, Platform::Window* window, GLXContextObject* parentContext)
	: m_device(device)
	, m_window(window)
	, m_context(NULL)
{
	m_context = LN_NEW GLXContextObject(m_device, m_window, parentContext);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLXSwapChain::~GLXSwapChain()
{
	LN_SAFE_RELEASE(m_context);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLXSwapChain::Resize(const Size& size)
{
	LN_THROW(0, NotImplementedException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLXSwapChain::Present(ITexture* colorBuffer)
{
	m_device->MakeCurrentContext(m_context);

	try
	{
		// colorBuffer をバックバッファに描画する
		InternalPresent(colorBuffer, static_cast<GLRenderer*>(m_device->GetRenderer())->GetVertexArrayObject());
		// バックバッファをフロントバッファに転送する
		glXSwapBuffers(m_context->GetX11Display(), m_context->GetX11Window());
	}
	catch (...)
	{
		m_device->MakeCurrentContext(m_device->GetMainRenderingContext());
		throw;
	}
	m_device->MakeCurrentContext(m_device->GetMainRenderingContext());
}


//=============================================================================
// GLXContextObject
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLXContextObject::GLXContextObject(GLXGraphicsDevice* device, Platform::Window* window, GLXContextObject* parentContext)
	: m_x11Display(0)
	, m_x11Window(0)
	, m_glxContext(0)
{
	m_x11Display = Platform::PlatformSupport::GetX11Display(window);
	m_x11Window = Platform::PlatformSupport::GetX11WindowID(window);
	const GLXFBConfig fbConfig = *(Platform::PlatformSupport::GetGLXFBConfig(window));
	
	GLXContext share = NULL;
	if (parentContext != NULL) {
		share = parentContext->m_glxContext;
	}
	
	if (GLXGraphicsDevice::glXCreateContextAttribsARB &&
		device->GetOpenGLMajorVersio() > 0)
	{
		int flags = 0;
		if (1/*config->Forward*/) {
			flags |= GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
		}
		if (0/*config->Debug*/) {
			flags |= GLX_CONTEXT_DEBUG_BIT_ARB;
		}
		
		int mask = 0;
		if (0/*config->Profile == GLFW_OPENGL_CORE_PROFILE*/) {
			mask |= GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
		}
		else if (1/*config->Profile == GLFW_OPENGL_COMPAT_PROFILE*/) {
			mask |= GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
		}
		
		// コンテキスト作成
		int attr[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, device->GetOpenGLMajorVersio(),
			GLX_CONTEXT_MINOR_VERSION_ARB, device->GetOpenGLMinorVersio(),
			GLX_CONTEXT_PROFILE_MASK_ARB, mask,
			GLX_CONTEXT_FLAGS_ARB, flags,
			None,
		};
		m_glxContext = GLXGraphicsDevice::glXCreateContextAttribsARB(m_x11Display, fbConfig, 0, True, attr);
		LN_THROW(m_glxContext == NULL, InvalidOperationException);
	}
	else
	{
		m_glxContext = glXCreateNewContext(m_x11Display, fbConfig, GLX_RGBA_TYPE, share, True);
#if 0
		glXMakeCurrent(
			m_x11Display,
			m_x11Window,
			m_glxContext);
		printf("Vendor : %s\n", glGetString(GL_VENDOR));
		printf("GPU : %s\n", glGetString(GL_RENDERER));
		printf("OpenGL : %s\n", glGetString(GL_VERSION));
		glXMakeCurrent(m_x11Display, None, NULL);
#endif
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLXContextObject::~GLXContextObject()
{
	if (m_glxContext)
	{
		glXDestroyContext(m_x11Display, m_glxContext);
		m_glxContext = NULL;
	}
}

} // namespace Device
} // namespace Graphics
} // namespace Lumino
