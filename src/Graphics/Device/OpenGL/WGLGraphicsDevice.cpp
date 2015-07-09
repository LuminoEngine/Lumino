/*
 *	EXT とか ARB とか
 *	https://shikihuiku.wordpress.com/2013/10/08/hardtounderstandoglext/
 *	http://www.asahi-net.or.jp/~YW3T-TRNS/opengl/extensions/index.htm
 *
 *	OpenGL のはじめ方 (2) wgl
 *	http://wlog.flatlib.jp/archive/1/2009-9-6

	wgl マルチスレッド
	https://veelck.wordpress.com/2008/11/28/multithread-texture-loading-in-opengl/

 */
#include "../../../Internal.h"
#include "WGLSwapChain.h"
#include "WGLGraphicsDevice.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{

//=============================================================================
// WGLGraphicsDevice
//=============================================================================

PFNWGLGETEXTENSIONSSTRINGEXTPROC	WGLGraphicsDevice::GetExtensionsStringEXT = NULL;
PFNWGLGETEXTENSIONSSTRINGARBPROC	WGLGraphicsDevice::GetExtensionsStringARB = NULL;
PFNWGLCREATECONTEXTATTRIBSARBPROC	WGLGraphicsDevice::CreateContextAttribsARB = NULL;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
WGLGraphicsDevice::WGLGraphicsDevice()
	: m_defaultSwapChain(NULL)
	, m_mainContext(NULL)
	, m_mainRenderingContext(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
WGLGraphicsDevice::~WGLGraphicsDevice()
{
	LN_SAFE_RELEASE(m_defaultSwapChain);
	LN_SAFE_RELEASE(m_mainContext);
	LN_SAFE_RELEASE(m_mainRenderingContext);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void WGLGraphicsDevice::Initialize(const ConfigData& configData)
{
	GLGraphicsDevice::Initialize(configData);

	HWND hWnd = Platform::PlatformSupport::GetWindowHandle(m_mainWindow);
	HDC hDC = ::GetDC(hWnd);

	// まずは wglCreateContext で適当にコンテキストを作る。
	// コンテキストを作らないと wglGetProcAddress() が機能しないため、ダミーとして作る必要がある。
	static PIXELFORMATDESCRIPTOR   pformat = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,     // color
		0, 0,   // R
		0, 0,   // G
		0, 0,   // B
		0, 0,   // A
		0, 0, 0, 0, 0,      // AC R G B A
		24,     // depth
		8,      // stencil
		0,      // aux
		0,      // layertype	… このオプションは使用されなくなった https://www.opengl.org/discussion_boards/showthread.php/127004-What-does-the-PFD_MAIN_PLANE-mean
		0,		// reserved
		0,		// layermask
		0,		// visiblemask
		0,		// damagemask
	};
	int pfmt = ChoosePixelFormat(hDC, &pformat);
	SetPixelFormat(hDC, pfmt, &pformat);
	HGLRC hGLRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hGLRC);


	GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
	GetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	CreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	Logger::WriteLine("Default OpenGL info:");
	Logger::WriteLine("    GL_VERSION                  : %s", glGetString(GL_VERSION));
	Logger::WriteLine("    GL_SHADING_LANGUAGE_VERSION : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	Logger::WriteLine("    Extensions strings          : %s", GetExtensionsStringEXT());

	SelectGLVersion(configData.OpenGLMajorVersion, configData.OpenGLMinorVersion);

	// コンテキスト作成後、wglMakeCurrent() で設定しないとエラーが返る
	if (glewInit() != GLEW_OK) {
		LN_THROW(0, InvalidOperationException);
	}

	m_mainContext = LN_NEW WGLContext(this, configData.MainWindow, NULL);

	wglDeleteContext(hGLRC);
	::ReleaseDC(hWnd, hDC);


	m_mainRenderingContext = LN_NEW WGLContext(this, configData.MainWindow, m_mainContext);

	// m_defaultSwapChain->Create() でシェーダとか作るので先にアクティブにしておく
	MakeCurrentContext(m_mainContext);



	m_defaultSwapChain = LN_NEW WGLSwapChain();
	m_defaultSwapChain->Create(this, m_mainWindow, m_mainContext);
	//m_defaultSwapChain->MakeCurrentContext();


	//MakeCurrentContext(m_mainRenderingContext);

	// Renderer の初期化でもオブジェクトを生成したりするのでメインのコンテキストがアクティブになっているときに初期化する必要がある
	m_renderer = LN_NEW GLRenderer();
	m_renderer->Initialize();

	MakeCurrentContext(NULL);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void WGLGraphicsDevice::MakeCurrentContext(GLContext* context)
{
	if (context) {
		BOOL r = wglMakeCurrent(static_cast<WGLContext*>(context)->GetDC(), static_cast<WGLContext*>(context)->GetGLRC());
		LN_THROW(r, Win32Exception, ::GetLastError());
	}
	else {
		BOOL r = wglMakeCurrent(NULL, NULL);
		LN_THROW(r, Win32Exception, ::GetLastError());
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLContext* WGLGraphicsDevice::GetMainContext()
{
	return m_mainContext;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLContext* WGLGraphicsDevice::GetMainRenderingContext()
{
	return m_mainRenderingContext;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ISwapChain* WGLGraphicsDevice::GetDefaultSwapChain()
{
	return m_defaultSwapChain;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ISwapChain* WGLGraphicsDevice::CreateSwapChain(Platform::Window* window)
{
	ScopedContext lock(this);
	RefPtr<WGLSwapChain> obj(LN_NEW WGLSwapChain());
	obj->Create(this, window, m_mainContext);
	AddDeviceResource(obj);
	obj.SafeAddRef();
	return obj;
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void WGLGraphicsDevice::AttachRenderingThread()
{
	MakeCurrentContext(m_mainRenderingContext);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void WGLGraphicsDevice::DetachRenderingThread()
{
	MakeCurrentContext(NULL);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int WGLGraphicsDevice::CheckPlatformExtensionSupported(const char* extension)
{
	//const GLubyte* extensions;

	//if (wgl.GetExtensionsStringEXT != NULL)
	//{
	//	extensions = (GLubyte*)wgl.GetExtensionsStringEXT();
	//	if (extensions != NULL)
	//	{
	//		//printf("%s\n", extensions);
	//		//if (_glfwStringInExtensionString(extension, extensions))
	//		//	return GL_TRUE;
	//	}
	//}

	//if (wgl.GetExtensionsStringARB != NULL)
	//{
	//	extensions = (GLubyte*)wgl.GetExtensionsStringARB(wgl.m_hDC);
	//	if (extensions != NULL)
	//	{
	//		//if (_glfwStringInExtensionString(extension, extensions))
	//		//	return GL_TRUE;
	//	}
	//}

	return GL_FALSE;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool WGLGraphicsDevice::CheckContainsExtensionString(const char* string, const GLubyte* extensions)
{
	if (!extensions) {
		return false;
	}

	const GLubyte* start;
	GLubyte* where;
	GLubyte* terminator;

	start = extensions;
	for (;;)
	{
		where = (GLubyte*)strstr((const char*)start, string);
		if (!where)
			return GL_FALSE;

		terminator = where + strlen(string);
		if (where == start || *(where - 1) == ' ')
		{
			if (*terminator == ' ' || *terminator == '\0')
				break;
		}

		start = terminator;
	}

	return GL_TRUE;
}

} // namespace Device
} // namespace Graphics
} // namespace Lumino
