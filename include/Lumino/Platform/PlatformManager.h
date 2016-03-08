
#pragma once
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Size.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Array.h>
#include <Lumino/Threading/Thread.h>
#include "Common.h"

LN_NAMESPACE_BEGIN

/// ウィンドウシステムに使用するAPI・ライブラリ
enum WindowSystemAPI
{
	WindowSystemAPI_Win32API = 0,		///< WindowsAPI
	WindowSystemAPI_GLFW,
};

/// ウィンドウを作成するための設定
struct WindowCreationSettings
{
	String	Title;			///< ウィンドウタイトル (default:"")
	Size	ClientSize;		///< クライアント領域のピクセルサイズ (default:640,480)
	bool	Fullscreen;		///< フルスクリーンモードで作成するかどうか (default:false)
	bool	Resizable;		///< 可変ウィンドウとして作成するかどうか (default:true)
	void*	UserWindow;

	WindowCreationSettings();
};


class PlatformManager
	: public RefObject
{
public:
	/// Application を初期化するための設定
	struct Settings
	{
		WindowSystemAPI			API;
		WindowCreationSettings	MainWindowSettings;		/**< メインウィンドウの作成に使用する設定 */
		bool					UseInternalUIThread;	/**<  (default:false) */

		//void*					ExternalMainWindow;		/**< ユーザー定義のウィンドウハンドル (windows の場合は HWND、X11 は Window*。ただし、X11 は未対応) */

		Settings();
	};

public:
	PlatformManager();
	PlatformManager(const Settings& settings);
	virtual ~PlatformManager();

public:

	void Initialize(const Settings& settings);
	PlatformWindow* GetMainWindow();
	bool DoEvents();

public:
	void Dispose();

protected:

private:
	void Thread_MainWindow();

private:
	friend class PlatformWindow;
	bool						m_useThread;
	WindowCreationSettings		m_windowCreationSettings;
	WindowManagerBase*			m_windowManager;
	//PlatformWindow*						m_mainWindow;
	Threading::DelegateThread	m_mainWindowThread;
	Threading::EventFlag		m_mainWindowThreadInitFinished;
	Threading::EventFlag		m_mainWindowThreadEndRequested;
};

LN_NAMESPACE_END
