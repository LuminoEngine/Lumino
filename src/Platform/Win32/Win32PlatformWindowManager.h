
#pragma once
#include "../PlatformWindowManagerBase.h"

LN_NAMESPACE_BEGIN

class Win32WindowManager
	: public WindowManagerBase
{
public:
	/// CreateNativeWindow の引数
	struct NativeWindowCreationData
	{
		String	        TitleText;		///< ウィンドウタイトルの文字列
		int				Width;			///< クライアント領域の幅
		int				Height;			///< クライアント領域の高さ
		bool			Fullscreen;		///< フルスクリーンモードで初期化する場合 true
		bool			Resizable;
		HWND			UserWindow;

		NativeWindowCreationData()
			: TitleText()
			, Width(0)
			, Height(0)
			, Fullscreen(false)
			, Resizable(false)
			, UserWindow(NULL)
		{}
	};

	static const TCHAR*		PROP_WINPROC;		///< SetProp() で使用するキー文字列
	static const DWORD		FULLSCREEN_STYLE;	///< フルスクリーンモードのときのウィンドウスタイル

public:
	Win32WindowManager(int IconResourceID);
	virtual ~Win32WindowManager();

public:
	HINSTANCE GetInstanceHandle() const { return m_hInst; }
	const String& GetWindowClassName() const { return m_windowClassName; }
	HICON GetHIcon() const { return m_hIcon; }

	Win32PlatformWindow* CreateNativeWindow(const NativeWindowCreationData& data);

	static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// クライアント領域のサイズが指定した大きさになるようにウィンドウサイズを調整する
	static void SetWindowClientSize(HWND hWnd, const Size& clientSize);

	/// ウィンドウをスクリーンの中央に移動する
	static void AbjustLocationCentering(HWND hWnd);

public:
	// override Application
	virtual void CreateMainWindow(const WindowCreationSettings& settings);
	virtual PlatformWindow* GetMainWindow();
	virtual PlatformWindow* CreateSubWindow(const WindowCreationSettings& settings);
	virtual void DoEvents();
	virtual void Finalize();

private:
	RefPtr<Win32PlatformWindow>	m_mainWindow;
	HINSTANCE					m_hInst;
	HICON						m_hIcon;
	String						m_windowClassName;
	//bool						m_comInited;
};

LN_NAMESPACE_END
