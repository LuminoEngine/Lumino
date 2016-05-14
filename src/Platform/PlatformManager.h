
#pragma once
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Size.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Array.h>
#include <Lumino/Threading/Thread.h>
#include <Lumino/Platform/Common.h>

LN_NAMESPACE_BEGIN

/// ウィンドウを作成するための設定
struct WindowCreationSettings
{
	String	title;							// ウィンドウタイトル
	Size	clientSize = Size(640, 480);	// クライアント領域のピクセルサイズ
	bool	fullscreen = false;				// フルスクリーンモードで作成するかどうか
	bool	resizable = true;				// 可変ウィンドウとして作成するかどうか
	void*	userWindow = nullptr;
};


class PlatformManager
	: public RefObject
{
public:
	/// Application を初期化するための設定
	struct Settings
	{
		WindowSystemAPI			windowSystemAPI = WindowSystemAPI::Default;
		WindowCreationSettings	mainWindowSettings;
		bool					useInternalUIThread = false;

		//void*					ExternalMainWindow;		/**< ユーザー定義のウィンドウハンドル (windows の場合は HWND、X11 は Window*。ただし、X11 は未対応) */
	};

	static PlatformManager* GetInstance(PlatformManager* priority = nullptr);

public:
	PlatformManager();
	PlatformManager(const Settings& settings);
	virtual ~PlatformManager();

public:

	void Initialize(const Settings& settings);
	PlatformWindow* GetMainWindow();
	bool DoEvents();

	WindowManagerBase* GetWindowManager() const LN_NOEXCEPT { return m_windowManager; }

public:
	void Dispose();

protected:

private:
	void Thread_MainWindow();

private:
	bool						m_useThread;
	WindowCreationSettings		m_windowCreationSettings;
	WindowManagerBase*			m_windowManager;
	Threading::DelegateThread	m_mainWindowThread;
	Threading::EventFlag		m_mainWindowThreadInitFinished;
	Threading::EventFlag		m_mainWindowThreadEndRequested;
};

LN_NAMESPACE_END
