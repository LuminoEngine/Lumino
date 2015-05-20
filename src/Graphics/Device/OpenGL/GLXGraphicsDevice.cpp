/*
 *	glXIntro - X ウィンドウシステムで OpenGL を使う方法の解説
 *	http://manpages.ubuntu.com/manpages/gutsy/ja/man3/glXIntro.3x.html
 *	基本的な GLX コンテキストの使い方は、
 *	・glXChooseVisual() で Visual を選択
 *	・glXCreateContext() で Visual からコンテキストを作る
 *	・glXMakeCurrent() でウィンドウとくっつけてアクティブにする
 *	・glXSwapBuffers() で画面更新
 *	・XFree() で Visual を破棄
 *	・glXDestroyContext() でコンテキストを破棄
 *	※	Visual の破棄は glXCreateContext() の後すぐに行ってもいいかもしれないが、
 *		本ライブラリでは glXChooseVisual() するのは X11Window であり、
 *		GLXGraphicsDevice とは離れたところにいる。いつでも取得しやすいように、
 *		ウィンドウ存在中は破棄せずに持っておく。
 */
#include "../../../Internal.h"
#include "GLXSwapChain.h"
#include "GLXGraphicsDevice.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{

//=============================================================================
// GLXGraphicsDevice
//=============================================================================

PFNGLXCREATECONTEXTATTRIBSARBPROC GLXGraphicsDevice::glXCreateContextAttribsARB = NULL;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLXGraphicsDevice::GLXGraphicsDevice()
	: m_mainContext(NULL)
	, m_mainRenderingContext(NULL)
	, m_defaultSwapChain(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLXGraphicsDevice::~GLXGraphicsDevice()
{
	LN_SAFE_RELEASE(m_defaultSwapChain);
	LN_SAFE_RELEASE(m_mainContext);
	LN_SAFE_RELEASE(m_mainRenderingContext);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLXGraphicsDevice::Initialize(const GraphicsDeviceConfigData& configData)
{
	GLGraphicsDevice::Initialize(configData);
	m_x11Display = Platform::PlatformSupport::GetX11Display(configData.MainWindow);
	
	//---------------------------------------------------------
	// 最初に OpenGL のバージョンを調べたいので、ダミーの GLXContext を作って調べる
	::Display*			x11Display = Platform::PlatformSupport::GetX11Display(configData.MainWindow);
	int					x11Screen = DefaultScreen(x11Display);
	Window				x11Window = Platform::PlatformSupport::GetX11WindowID(configData.MainWindow);
	const GLXFBConfig	fbConfig = *(Platform::PlatformSupport::GetGLXFBConfig(configData.MainWindow));
	GLXContext			context = glXCreateNewContext(x11Display, fbConfig, GLX_RGBA_TYPE, NULL, True);
    glXMakeCurrent(x11Display, x11Window, context);
	
	glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC)GetGLXProcAddress("glXCreateContextAttribsARB");
	
	Logger::WriteLine("GLXGraphicsDevice ... Default OpenGL info:");
	Logger::WriteLine("    GL_VERSION                  : %s", glGetString(GL_VERSION));
	Logger::WriteLine("    GL_SHADING_LANGUAGE_VERSION : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	Logger::WriteLine("    Extensions strings          : %s", glXQueryExtensionsString(x11Display, x11Screen));

	SelectGLVersion(configData.OpenGLMajorVersion, configData.OpenGLMinorVersion);
	
	if (glewInit() != GLEW_OK) {
		LN_THROW(0, InvalidOperationException);
	}
	
    glXMakeCurrent(x11Display, None, NULL);
	glXDestroyContext(x11Display, context);
	
	//---------------------------------------------------------
	
	
	m_mainContext = LN_NEW GLXContextObject(this, configData.MainWindow, NULL);
	m_mainRenderingContext = LN_NEW GLXContextObject(this, configData.MainWindow, m_mainContext);

	MakeCurrentContext(m_mainContext);

	m_defaultSwapChain = LN_NEW GLXSwapChain(this, configData.MainWindow, m_mainContext);

	m_renderer = LN_NEW GLRenderer();
	m_renderer->Initialize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLContext* GLXGraphicsDevice::GetMainContext()
{
	return m_mainContext;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLContext* GLXGraphicsDevice::GetMainRenderingContext()
{
	return m_mainRenderingContext;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLXGraphicsDevice::MakeCurrentContext(GLContext* context)
{
	if (context) {
		glXMakeCurrent(
			m_x11Display,
			static_cast<GLXContextObject*>(context)->GetX11Window(),
			static_cast<GLXContextObject*>(context)->GetGLXContext());
	}
	else {
		glXMakeCurrent(m_x11Display, None, NULL);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ISwapChain* GLXGraphicsDevice::GetDefaultSwapChain()
{
	return m_defaultSwapChain;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ISwapChain* GLXGraphicsDevice::CreateSwapChain(Platform::Window* window)
{
	RefPtr<GLXSwapChain> obj(LN_NEW GLXSwapChain(this, window, m_mainContext));
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLXGraphicsDevice::AttachRenderingThread()
{
	MakeCurrentContext(m_mainRenderingContext);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLXGraphicsDevice::DetachRenderingThread()
{
	MakeCurrentContext(NULL);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void* GLXGraphicsDevice::GetGLXProcAddress(const char* procName)
{
	return (void*)glXGetProcAddressARB((const GLubyte*)procName);
	/* glfw はプリプロシンボルで以下のを切り替えられるようになっている。
	glXGetProcAddressARB
	glXGetProcAddress
	glXGetProcAddressEXT
	dlsym
	*/
}

#if 0

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IRenderingContext* GLXGraphicsDevice::CreateRenderingContext(Platform::Window* window)
{
	Display* display = Platform::PlatformSupport::GetX11Display(window);
	XVisualInfo* visual = Platform::PlatformSupport::GetX11VisualInfo(window);
	GLXContext share = NULL;
	
	/* glXCreateContext	https://www.opengl.org/sdk/docs/man2/xhtml/glXCreateContext.xml
	 * glXCreateNewContext https://www.opengl.org/sdk/docs/man2/xhtml/glXCreateNewContext.xml
	 * 違いは、ウィンドウと Pixmap の他に、PixelBuffer にもレンダリングできるかどうか。
	 */
	//GLXContext context = glXCreateNewContext(display, visual, GLX_RGBA_TYPE, share, True);
	GLXContext context = glXCreateContext(display, visual, share, True);
	
	// Make、Swap 時に必要な情報をまとめる
	GLXContextInfo* info = LN_NEW GLXContextInfo();
	info->Display	= display;
	info->WindowID	= Platform::PlatformSupport::GetX11WindowID(window);
	info->Context	= context;

	// GLRenderingContext を生成して返す
	RefPtr<GLRenderingContext> obj(LN_NEW GLRenderingContext());
	obj->Initialize(info);
	obj.SafeAddRef();
	return obj;
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLXGraphicsDevice::BeginScene(IRenderingContext* context)
{
	if (context != NULL)
    {
		GLRenderingContext* glrc = static_cast<GLRenderingContext*>(context);
		GLXContextInfo* info = static_cast<GLXContextInfo*>(glrc->GetContextInfo());
		Bool r = glXMakeCurrent(info.Display, info.WindowID, info.Context);
		LN_THROW(r == True, InvalidOperationException);
    }
	else {
		Bool r = glXMakeCurrent(Platform::PlatformSupport::GetX11Display(m_mainWindow), None, NULL);
		LN_THROW(r == True, InvalidOperationException);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLXGraphicsDevice::EndScene(IRenderingContext* context)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLXGraphicsDevice::Present(IRenderingContext* context)
{
	GLRenderingContext* glrc = static_cast<GLRenderingContext*>(context);
	GLXContextInfo* info = static_cast<GLXContextInfo*>(glrc->GetContextInfo());
	glXSwapBuffers(info->Display, info->WindowID);
}
#endif
	
} // namespace Device
} // namespace Graphics
} // namespace Lumino
