
#pragma once

#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Size.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Array.h>
#include <Lumino/Threading/Thread.h>

namespace Lumino
{
namespace Platform
{
class WindowManagerBase;
class Window;

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

	WindowCreationSettings();
};

/// Application を初期化するための設定
struct ApplicationSettings
{
	WindowSystemAPI			API;
	WindowCreationSettings	MainWindowSettings;
	bool					UseInternalUIThread;	///<  (default:false)

	ApplicationSettings();
};

class PlatformManager
	: public RefObject
{
	//public:
	//
	//	static Application* Create();

public:
	PlatformManager();
	PlatformManager(const ApplicationSettings& settings);
	virtual ~PlatformManager();

public:

	void Initialize(const ApplicationSettings& settings);
	Window* GetMainWindow();
	// override Application
	//virtual void CreateMainWindow(const WindowCreationSettings& settings, bool useThread);
	bool DoEvents();

public:
	void Dispose();

protected:

private:
	void Thread_MainWindow();

private:
	friend class Window;
	bool						m_useThread;
	WindowCreationSettings		m_windowCreationSettings;
	WindowManagerBase*			m_windowManager;
	//Window*						m_mainWindow;
	Threading::DelegateThread	m_mainWindowThread;
	Threading::EventFlag		m_mainWindowThreadInitFinished;
	Threading::EventFlag		m_mainWindowThreadEndRequested;
};

} // namespace Platform
} // namespace Lumino
