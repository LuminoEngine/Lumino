
/*
	GLX 参考ソース:
	http://www.cl.cam.ac.uk/~cs448/git/trunk/src/progs/xdemos/overlay.c
*/
#include "../../Internal.h"
#include "X11WindowManager.h"
#include "X11Window.h"

namespace Lumino
{
namespace Platform
{

//=============================================================================
// X11Window
//=============================================================================

void PrintFBConfig(FILE* fp, Display* display, const GLXFBConfig* fbConfig)
{
	int value;
	glXGetFBConfigAttrib(display, *fbConfig, GLX_FBCONFIG_ID, &value);
	fprintf(fp, "id: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_DOUBLEBUFFER, &value);
	fprintf(fp, "  GLX_DOUBLEBUFFER: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_VISUAL_ID, &value);
	fprintf(fp, "  GLX_VISUAL_ID: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_RENDER_TYPE, &value);
	fprintf(fp, "  GLX_RENDER_TYPE.GLX_RGBA_BIT: %d\n", value & GLX_RGBA_BIT);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_DRAWABLE_TYPE, &value);
	fprintf(fp, "  GLX_DRAWABLE_TYPE.GLX_WINDOW_BIT: %d\n", value & GLX_WINDOW_BIT);

	glXGetFBConfigAttrib(display, *fbConfig, GLX_RED_SIZE, &value);
	fprintf(fp, "  GLX_RED_SIZE: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_GREEN_SIZE, &value);
	fprintf(fp, "  GLX_GREEN_SIZE: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_BLUE_SIZE, &value);
	fprintf(fp, "  GLX_BLUE_SIZE: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_ALPHA_SIZE, &value);
	fprintf(fp, "  GLX_ALPHA_SIZE: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_DEPTH_SIZE, &value);
	fprintf(fp, "  GLX_DEPTH_SIZE: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_STENCIL_SIZE, &value);
	fprintf(fp, "  GLX_STENCIL_SIZE: %d\n", value);

	glXGetFBConfigAttrib(display, *fbConfig, GLX_ACCUM_RED_SIZE, &value);
	fprintf(fp, "  GLX_ACCUM_RED_SIZE: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_ACCUM_GREEN_SIZE, &value);
	fprintf(fp, "  GLX_ACCUM_GREEN_SIZE: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_ACCUM_BLUE_SIZE, &value);
	fprintf(fp, "  GLX_ACCUM_BLUE_SIZE: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_ACCUM_ALPHA_SIZE, &value);
	fprintf(fp, "  GLX_ACCUM_ALPHA_SIZE: %d\n", value);

	glXGetFBConfigAttrib(display, *fbConfig, GLX_AUX_BUFFERS, &value);
	fprintf(fp, "  GLX_AUX_BUFFERS: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_STEREO, &value);
	fprintf(fp, "  GLX_STEREO: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_SAMPLES, &value);
	fprintf(fp, "  GLX_SAMPLES: %d\n", value);
	glXGetFBConfigAttrib(display, *fbConfig, GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB, &value);
	fprintf(fp, "  GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB: %d\n", value);
}
	
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
X11Window::X11Window(X11WindowManager* windowManager, const SettingData& settingData)
	: WindowBase(windowManager)
	, m_visualInfo(NULL)
	//, m_x11Window(NULL)
{
	m_titleText = settingData.TitleText;
	m_clientSize = settingData.ClientSize;
	
	Display*	x11Display			= GetWindowManager()->GetX11Display();
	int			x11DefaultScreen	= GetWindowManager()->GetX11DefaultScreen();
	::Window	x11RootWindow		= GetWindowManager()->GetX11RootWindow();
	
	//---------------------------------------------------------
	// XVisualInfo を取得する
	//		ここでは GLX の API を使用し、OpenGL で使いたいサーフェイスフォーマットから
	//		XVisualInfo を取得している。
	//		OpenGL 使わない場合は XMatchVisualInfo() 等、他にも取得する手段はある。
	//
	//		ちなみに、glfw はユーザーの指定や、環境が拡張機能をサポートしているかをチェックし、
	//		文字コード自動判別みたいに各 Visual 候補にスコアを振って、
	//		ベストな Visual を決めようとしていた。
	//		そこまでやるかは今後の要望しだいで。とりあえず一番ベーシックな A8R8G8B、D24S8 で取得する。
	//		※glfx では glXChooseVisual は使っていない。glXGetFBConfigs か、拡張が使えるときは glxChooseFBConfigSGIX。
	// http://earth.uni-muenster.de/~joergs/opengl/glXChooseVisual.html
	// 各設定の初期値:
	// http://www-01.ibm.com/support/knowledgecenter/ssw_aix_53/com.ibm.aix.opengl/doc/openglrf/glXChooseFBConfig.htm%23glxchoosefbconfig
	// http://wlog.flatlib.jp/item/1679
	int32_t attributes[] =
	{
		//GLX_USE_GL,		// リファレンスには "無視される" とある。
		GLX_RENDER_TYPE,	GLX_RGBA_BIT,
		GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
		//GLX_LEVEL, 0,
		GLX_RGBA,			1,		// TrueColor 使用
		GLX_DOUBLEBUFFER,	1,		// ダブルバッファリング有効
		GLX_RED_SIZE,		8,		// R 8bit
		GLX_GREEN_SIZE,		8,		// G 8bit
		GLX_BLUE_SIZE,		8,		// B 8bit
		GLX_ALPHA_SIZE,		0,		// A 8bit
		GLX_DEPTH_SIZE,		24,		// 深度値 24bit
		GLX_STENCIL_SIZE,	8,		// ステンシル 8bit
		//GLX_SAMPLES,        MultiSamples,	// マルチサンプリング (アンチエイリアス)
		None
	};
	
	// FBConfig 取得。とりあえず取得できたリストの先頭のを選択する
	int itemCount = 0;
	GLXFBConfig* configArray = glXChooseFBConfig(x11Display, x11DefaultScreen, attributes, &itemCount);
	LN_THROW(itemCount > 0, InvalidOperationException);
    m_fbConfig = configArray[0];
	XFree(configArray);
	
	// http://manpages.ubuntu.com/manpages/gutsy/ja/man3/glXChooseVisual.3x.html
	// http://www.wakhok.ac.jp/~tatsuo/sen96/4shuu/section1.html
	//m_visualInfo = glXChooseVisual(x11Display, x11DefaultScreen, attributes);
	m_visualInfo = glXGetVisualFromFBConfig(x11Display, m_fbConfig);
	
	//---------------------------------------------------------
	// カラーマップを選択する。
	// 		カラーマップとは、ピクセルフォーマットのようなもので、256色 とか 65535色等様々なパレットが存在している。
	// 		これの実態はサーバ側にあり、XCreateColormap() は指定された visual から最適なカラーマップの ID を返している。そんなイメージ。
	// 		このカラーマップを XCreateWindow() に指定することで、ウィンドウのピクセルフォーマットを決定する。
	Colormap colorMap = XCreateColormap(x11Display, x11RootWindow, m_visualInfo->visual, AllocNone);
	
	//---------------------------------------------------------
	// ウィンドウの属性 (Win32 のウィンドウスタイルみたいなもの)
	//		http://homepage3.nifty.com/rio_i/lab/xlib/015attribute.htm
	XSetWindowAttributes winAttr;
	winAttr.colormap = colorMap;
	winAttr.border_pixel = 0;
	winAttr.override_redirect = False;
	
	// Xサーバからどのイベントを受け取りたいのかを指定する
	winAttr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;
	
	//---------------------------------------------------------
	// ウィンドウを作る
	m_x11Window = XCreateWindow(
		x11Display,
		x11RootWindow,
		0, 0,
		m_clientSize.Width,
		m_clientSize.Height,
		0,                  // Border width
		m_visualInfo->depth,      // Color depth
		InputOutput,
		m_visualInfo->visual,
		CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,	// XSetWindowAttributes のどの部分を考慮するか
		&winAttr );
	
	// X11 のウィンドウとこのクラスのインスタンスを関連付ける (WinAPI の SetProp にあたる)
	int result = XSaveContext(
		x11Display,
		m_x11Window,
		GetWindowManager()->GetX11Context(),
		(XPointer)this);
	LN_THROW(result == 0, InvalidOperationException);
	
	// WM_DELETE_WINDOW が通知されるようにする
	XSetWMProtocols(x11Display, m_x11Window, &GetWindowManager()->m_atom_WM_DELETE_WINDOW, 1);
	
	SetVisible(true);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
X11Window::~X11Window()
{
	if (m_visualInfo != NULL) {
		XFree(m_visualInfo);
		m_visualInfo = NULL;
	}
	if (m_x11Window != NULL)
	{
		Display* x11Display = GetWindowManager()->GetX11Display();
		XDeleteContext(x11Display, m_x11Window, GetWindowManager()->GetX11Context());
        XUnmapWindow(x11Display, m_x11Window);
		XDestroyWindow(x11Display, m_x11Window);
		m_x11Window = (::Window)0;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void X11Window::SetFullScreenEnabled(bool flag)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void X11Window::CaptureMouse()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void X11Window::ReleaseMouseCapture()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void X11Window::SetVisible(bool show)
{
	Display* x11Display = GetWindowManager()->GetX11Display();

	// http://homepage3.nifty.com/rio_i/lab/xlib/001window.htm
	if (show)
	{
		XMapWindow( x11Display, m_x11Window );
		XFlush( x11Display );
	}
	else
	{
		XUnmapWindow( x11Display, m_x11Window );
		XFlush( x11Display );
	}
}

} // namespace Platform
} // namespace Lumino
