
#include "../../Internal.h"
#include <X11/Xresource.h>		// for XUniqueContext
#include "X11WindowManager.h"

// http://homepage3.nifty.com/rio_i/lab/xlib/013wm.htm

namespace Lumino
{
namespace Platform
{

//=============================================================================
// X11WindowManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
X11WindowManager::X11WindowManager()
	: m_x11Display(NULL)
	//, m_x11DefaultScreen(NULL)
	//, m_x11RootWindow(NULL)
{
	// X11 の API をスレッド対応にする。他の API 呼び出し前に呼んでおく必要がある。
	XInitThreads();

	m_x11Display = XOpenDisplay(NULL);
	LN_THROW(m_x11Display != NULL, InvalidOperationException);
	
	m_x11DefaultScreen = DefaultScreen(m_x11Display);
	m_x11RootWindow = RootWindow(m_x11Display, m_x11DefaultScreen);
	//m_x11Context = XUniqueContext();
	
	/* ユニークな XContext を作成する。
	 * X11 における Context(コンテクストマネージャ) とは、
	 * リソースID(ウィンドウハンドル等) とユーザーデータの対応表 (Map) と考えてよい。
	 * Win32Window では SetProp() を使用してインスタンスとウィンドウハンドルを関連付けているが、
	 * X11 でもこれと同じことをする必要がある。その前準備としてユニークな XContext を作成している。
	 * http://xjman.dsl.gr.jp/X11R6/X11/CH16.html
	 */
	m_x11Context = XUniqueContext();
	
#if 0
	const char* vendor = glXGetClientString(x11Display, GLX_VENDOR);

	int nativeConfigsCount;
	GLXFBConfig* nativeConfigs = glXGetFBConfigs(x11Display, x11DefaultScreen, &nativeConfigsCount);
	FILE* fp = fopen("fbconigs.txt", "w");
	fprintf(fp, "%s\n", vendor);

	printf("fp %p\n", fp);
	fprintf(fp, "nativeConfigsCount: %d\n", nativeConfigsCount);
	for (int i = 0; i < nativeConfigsCount; ++i)
		PrintFBConfig(fp, x11Display, &nativeConfigs[i]);
	fclose(fp);
#endif
	
	
	// Window manager atoms.
	// ※X11 における「アトム」とは、タイトル等のウィンドウプロパティを表すユニークID。
	// WM_DELETE_WINDOW : ウィンドウを閉じて破棄するべき時に通知される
	m_atom_WM_DELETE_WINDOW = XInternAtom(m_x11Display, "WM_DELETE_WINDOW", False);
	
	
	// http://homepage3.nifty.com/rio_i/lab/xlib/015attribute.htm#15_1_7
	
	
	/*
		GLX extension のバージョンを取得する
		・・・というのは建前。
		VirtualBox 上の Ubuntu12.04 で、GLX クライアントの実装 Vender を Mesa から Chromium に変更するのが目的。
		この環境では、glXGetFBConfigAttrib や glXChooseFBConfig が以下のようなメッセージを標準出力し、エラーを返す問題があった。
			OpenGL Warning: XGetVisualInfo returned 0 visuals for 09fbcc38
			OpenGL Warning: Retry with 0x8003 returned 0 visuals
			OpenGL Warning: glXGetFBConfigAttrib for 09fbcc38, failed to get XVisualInfo
		これは
			glXGetClientString(display, GLX_VENDOR);
		で取得できる文字列が "Mesa Project and SGI" (デフォルト) の時に発生した。
		同環境では GLFW が正常に動作するので GLFW のソースを追ってみたところ、
		GLFW では GLX_VENDOR が "Chromium" となっていた。
		そして、この GLX_VENDOR は何故か glXQueryVersion() の呼び出しで変化する。API リファレンスにもそんなこと乗ってないのに…。
		おそらく、現在の環境で一番いい感じに使える GLX クライアントを自動選択してくれているのだと思われる。
	*/
	int major, minor;
	Bool r = glXQueryVersion(m_x11Display, &major, &minor);
	LN_THROW(r, InvalidOperationException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
X11WindowManager::~X11WindowManager()
{
	if (m_x11Display) {
		XCloseDisplay(m_x11Display);
		m_x11Display = NULL;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void X11WindowManager::OnClosedWindow(Window* closedWindow)
//{
//	// 循環参照防止のため、Close されたら参照を外す
//	if (closedWindo/*w == mMainWindow) {
//		mMainWindow.*/SafeRelease();
//	}
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void X11WindowManager::Exit()
//{
//	::PostQuitMessage(0);
//	PlatformManager::Exit();
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void X11WindowManager::CreateMainWindow(const WindowCreationSettings& settings)
{
	X11Window::SettingData data;
	data.TitleText = settings.Title;
	data.ClientSize = settings.ClientSize;
	data.Fullscreen = settings.Fullscreen;
	data.Resizable = settings.Resizable;
	m_mainWindow.Attach(LN_NEW X11Window(this, data));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Window* X11WindowManager::CreateSubWindow(const WindowCreationSettings& settings)
{
	X11Window::SettingData data;
	data.TitleText = settings.Title;
	data.ClientSize = settings.ClientSize;
	data.Fullscreen = settings.Fullscreen;
	data.Resizable = settings.Resizable;
	return LN_NEW X11Window(this, data);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void X11WindowManager::DoEvents()
{
	/* XPending() は、トップレベルウィンドウが破棄された後に呼ばれると次のようなエラーを表示し強制終了する。
	 *	> XIO:  fatal IO error 11 (リソースが一時的に利用できません) on X server ":0"
	 *	> after 26 requests (26 known processed) with 0 events remaining.
	 */
	int count = XPending(m_x11Display);
	while (count--)
	{
		XEvent xev;
		XNextEvent(m_x11Display, &xev);

		switch (xev.type)
		{
			case ClientMessage:
			{
				// クローズボタンが押された場合
				if ((Atom)xev.xclient.data.l[0] == m_atom_WM_DELETE_WINDOW)
				{
					// TODO
					Exit();
				}
				/*
				if ( mX11MainWindow->checkCloseButtonOfThis(
				xev.xclient.message_type,
				xev.xclient.data.l[ 0 ] ) )
				{
				LNEvent lnev;
				lnev.Type = LNEV_CLOSE;
				this->postMessage( lnev );
				}
				continue;
				*/
				break;
			}
		}
/*
		// ライブラリに必要なメッセージの場合
		if ( X11::checkNecessaryMessage( xev ) )
		{
		LNEvent lnev;
		bool handled;
		X11::translateMessageToLN( &lnev, xev, &handled );
		this->postMessage( lnev );
		}
		*/
	}
	
	/*
	while( 1 ) {
   63			XNextEvent( dpy, &evt );
   64	
   65			switch ( evt.type ) {
   66				case KeyPress:
   67					/ リソースの解放 /
   68					XDestroyWindow( dpy, win );
   69					XCloseDisplay( dpy );
   70					return 0;
   71			}
   72		}
	*/
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void X11WindowManager::Finalize()
{
	m_mainWindow.SafeRelease();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
X11Window* X11WindowManager::FindX11Window(::Window x11Window)
{
	X11Window* window;
	if (XFindContext(
			m_x11Display,
			x11Window,
			m_x11Context,
			(XPointer*)&window) != 0)
	{
		LN_THROW(0, InvalidOperationException);	// 現在の用途としてはあり得ない
	}
    return window;
}

} // namespace Platform
} // namespace Lumino
