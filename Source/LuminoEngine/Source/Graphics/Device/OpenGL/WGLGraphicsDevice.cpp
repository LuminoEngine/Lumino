/*
 *	EXT とか ARB とか
 *	https://shikihuiku.wordpress.com/2013/10/08/hardtounderstandoglext/
 *	http://www.asahi-net.or.jp/~YW3T-TRNS/opengl/extensions/index.htm
 *
 *	OpenGL のはじめ方 (2) wgl
 *	http://wlog.flatlib.jp/archive/1/2009-9-6

	wgl マルチスレッド
	https://veelck.wordpress.com/2008/11/28/multithread-texture-loading-in-opengl/

	[OpenGL] FrameBufferとRenderBufferについてメモ
	http://qiita.com/edo_m18/items/95483cabf50494f53bb5
 */
#include "../../Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/Platform/PlatformSupport.h>
#include "WGLGraphicsDevice.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
	
//==============================================================================
// WGLContext
//==============================================================================

//------------------------------------------------------------------------------
WGLContext::WGLContext(WGLGraphicsDevice* device, PlatformWindow* window, WGLContext* parentContext)
	: m_hWnd(NULL)
	, m_hDC(NULL)
	, m_hGLRC(NULL)
{
	m_hWnd = PlatformSupport::getWindowHandle(window);
	m_hDC = ::GetDC(m_hWnd);

	HGLRC share = NULL;
	if (parentContext) {
		share = parentContext->m_hGLRC;
	}

	// とりあえず今はこのフォーマット固定で。
	/*
	Note:
	PFD_DEPTH_DONTCARE を指定すると Create が失敗する。サポート外？
	depth=24, stencil=8 でも、glGetFramebufferAttachmentParameteriv で 24, 0 となった。
	depth や stencil に 0 を指定しても、glGetFramebufferAttachmentParameteriv で 24, 0 となった。
	depth=32, stencil=0 でも、glGetFramebufferAttachmentParameteriv で 24, 0 となった。
	*/
	static PIXELFORMATDESCRIPTOR   pformat = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER/* | PFD_DEPTH_DONTCARE*/,
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
	int pfmt = ::ChoosePixelFormat(m_hDC, &pformat);
	if (LN_ENSURE_WIN32(pfmt, ::GetLastError())) return;

	BOOL r = ::SetPixelFormat(m_hDC, pfmt, &pformat);
	if (LN_ENSURE_WIN32(r, ::GetLastError())) return;

	if (device->getOpenGLMajorVersion() != 0)
	{
		// 使用する OpenGL のバージョンとプロファイルの指定
		//	WGL_CONTEXT_FLAGS_ARB
		//		WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB	
		//			前方互換プロファイル, 古い機能が使えない (基本的に 0 でよいはず)
		//	WGL_CONTEXT_PROFILE_MASK_ARB
		//		3.2 で意味がある。それ以前では無視される。省略した場合はシステム設定。
		//		WGL_CONTEXT_CORE_PROFILE_BIT_ARB
		//			Core Profile (OpenGL 3.1 と上位互換。(OpenGL 3.0 以前とは非互換))
		//		WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
		//			Compatibility Profile (OpenGL 3.1 + GL_ARB_compatibility と上位互換。(OpenGL 3.0 以前と上位互換))
		// http://www.asahi-net.or.jp/~yw3t-trns/opengl/version/
		// http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20120908
		// https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
		int attr[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, device->getOpenGLMajorVersion(),
			WGL_CONTEXT_MINOR_VERSION_ARB, device->getOpenGLMinorVersion(),
			WGL_CONTEXT_FLAGS_ARB, 0,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,	// 古い機能と互換性あり (glBegin 使いたい)
			//WGL_CONTEXT_FLAGS_ARB, 0,//WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,//WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
			//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,//WGL_CONTEXT_CORE_PROFILE_BIT_ARB,//WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
			0, 0,
		};

		m_hGLRC = WGLGraphicsDevice::CreateContextAttribsARB(m_hDC, share, attr);	// wglCreateContextAttribsARB なら wglShareLists は必要ない
		if (LN_ENSURE_WIN32(m_hGLRC, ::GetLastError())) return;
	}
	else
	{
		m_hGLRC = wglCreateContext(m_hDC);
		if (LN_ENSURE_WIN32(m_hGLRC, ::GetLastError())) return;

		if (share != NULL)
		{
			r = wglShareLists(share, m_hGLRC);
			if (LN_ENSURE_WIN32(r, ::GetLastError())) return;
		}
	}
}

//------------------------------------------------------------------------------
WGLContext::~WGLContext()
{
	if (m_hGLRC != NULL) {
		wglDeleteContext(m_hGLRC);
	}
	if (m_hDC != NULL) {
		::ReleaseDC(m_hWnd, m_hDC);
	}
}

//------------------------------------------------------------------------------
void WGLContext::swapBuffers()
{
	BOOL r = ::SwapBuffers(m_hDC);
	if (LN_ENSURE_WIN32(r, ::GetLastError())) return;
}

//==============================================================================
// WGLGraphicsDevice
//==============================================================================

PFNWGLGETEXTENSIONSSTRINGEXTPROC	WGLGraphicsDevice::GetExtensionsStringEXT = nullptr;
PFNWGLGETEXTENSIONSSTRINGARBPROC	WGLGraphicsDevice::GetExtensionsStringARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC	WGLGraphicsDevice::CreateContextAttribsARB = nullptr;

//------------------------------------------------------------------------------
WGLGraphicsDevice::WGLGraphicsDevice()
	: m_glInited(false)
{
}

//------------------------------------------------------------------------------
WGLGraphicsDevice::~WGLGraphicsDevice()
{
}

//------------------------------------------------------------------------------
Ref<GLContext> WGLGraphicsDevice::initializeMainContext(const ConfigData& configData)
{
	HWND hWnd = PlatformSupport::getWindowHandle(configData.mainWindow);
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

	Logger::writeLine("Default OpenGL info:");
	Logger::writeLine("    GL_VERSION                  : %s", glGetString(GL_VERSION));
	Logger::writeLine("    GL_SHADING_LANGUAGE_VERSION : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	Logger::writeLine("    Extensions strings          : %s", GetExtensionsStringEXT());

	selectGLVersion(configData.openGLMajorVersion, configData.openGLMinorVersion);

	
	GLenum result = glewInit();
	if (LN_ENSURE(result == GLEW_OK)) return nullptr;	// コンテキスト作成後、wglMakeCurrent() しないとエラーが返る

	auto mainContext = Ref<WGLContext>::makeRef(this, configData.mainWindow, nullptr);

	wglDeleteContext(hGLRC);
	::ReleaseDC(hWnd, hDC);


	return Ref<GLContext>::staticCast(mainContext);
}

//------------------------------------------------------------------------------
Ref<GLContext> WGLGraphicsDevice::createContext(PlatformWindow* window)
{
	auto ptr = Ref<WGLContext>::makeRef(this, window, static_cast<WGLContext*>(getMainContext()));
	return Ref<GLContext>::staticCast(ptr);
}

//------------------------------------------------------------------------------
void WGLGraphicsDevice::makeCurrentContext(GLContext* context)
{
	if (context != nullptr)
	{
		// 環境によっては、wglMakeCurrent() が失敗するにもかかわらず GetLastError() が 0 を返すことがある。
		// そういうときは、まず wglMakeCurrent(NULL, NULL) してみるとちゃんとエラーが取れる。
		// http://stackoverflow.com/questions/21613842/wglmakecurrent-in-second-thread-fails
		//wglMakeCurrent(NULL, NULL);

		WGLContext* wglContext = static_cast<WGLContext*>(context);
		BOOL r = wglMakeCurrent(wglContext->GetDC(), wglContext->GetGLRC());
		if (LN_ENSURE_WIN32(r, ::GetLastError())) return;
	}
	else
	{
		BOOL r = wglMakeCurrent(NULL, NULL);
		if (LN_ENSURE_WIN32(r, ::GetLastError())) return;
	}
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
