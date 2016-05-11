
/*

	[2015/4/16] EventArgs
		以前は EventArgs クラスから派生した MouseEventArgs や KeyEventArgs を定義するスタイルを取っていた。
		しかし、メインスレッドと UI スレッドを分ける場合イベントは一度キューに積む必要があり、
		そのためにはメモリの動的確保(new)が必須となる。頻繁に飛んでくるメッセージに対して new はできればやりたくない。
		new しない場合にできなくなるのは継承を使ったオブジェクト指向的なユーザー拡張イベント引数の実装。
		…であるが、別の Lumino.GUI であればともかく、Platform イベントを拡張したい、しかもこのライブラリを使って、
		ということがあるのかというと、まずないと思われる。
		例えあったとしても相当レアケースなので、new のコストと天秤に掛けると却下するべき。ユーザーデータとして void* 持っておくだけで十分。

	■[2015/4/16] クラス設計とかインターフェイスパターンとか。

		Interface パターンにすると、ユーザーが派生クラスを作れなくなる。
		Widget Toolkit としてそれはどうなの？というところだが・・・。

		プラットフォーム依存の部分を隠すために Interface にしている。
		それをさらにユーザーに派生させてカスタマイズさせようという考えが、そもそも欲張りすぎ。
		Win32Window をユーザーに公開すると言うことは、プラットフォーム依存のアレコレも全て公開すると言うこと。
		
		wxWidgets はこれをやっていて、wxWindow は wxWin32Window のマクロである。
		でもそうすると、Windows 環境で Win32Window と GLFWWindow を切り替えたりできなくなる。
		コンパイル時にオプションで変えなければならず、バイナリが完全に別になる。管理が面倒になる。

		本モジュールとは別になるが、昔は SceneGraph モジュール等でユーザーに公開するものをシンプルにするために
		Interface パターンで実装しようとした。
		しかし、SceneNode はユーザーによる拡張が可能なようにするべきで、そういう場合は Interface パターンは使うべきではない。



	■[2015/2/6] メッセージループの別スレッド化について
		・別スレッドで動かす場合
			- ウィンドウドラッグ中等もゲーム処理続行可能。
			  システムの都合でメインスレッドを極力中断しなくて良い。
			- イベントハンドラはメインスレッドで実行する。そのため、戻り値を返すことができない。
			  例えば、Closing をコールバック形式で処理できない。
			  普通のGUIアプリだったら OnClosing で handled = true を返してクローズをキャンセルできたりするけど、
			  これは不可能である。
			  ゲーム用途として、ゲーム内GUIでクローズ確認するならいいんだけど。
		・メインスレッドで動かす場合
			- 普通の GUI アプリとして自然な動作。実装もシンプルになる。
			- ゲーム用途としてはあまりメリット無い。ゲーム内GUIでクローズ確認するならどのみち一度 Closing を Post しなければならない。




	■メッセージ処理手順について

		システムから通知されたメッセージは、一度ライブラリのイベントに変換したうえで "Post" する。
		つまり、メインスレッドと同じスレッドで通知されたイベントだとしても一度ライブラリのイベントキューを通す。
		このイベントキューはスレッドセーフにする
		なぜこうするかというと、
		・Win32 環境で別スレッドでメッセージ処理を行うことで、ウィンドウドラッグ中もメインスレッドで処理を行えるようにする。
		・Android 等、既に出来上がっているフレームワークに組み込む場合、メインループを記述することが困難になる場合がある。
		  その場合はゲームスレッドを起動してそこにメインループを記述するが、イベント発生元と処理が別のスレッドになることになる。

		問題はマルチスレッド関係なので、シングルスレッドで動作させるなら "Send" でもOK。
		  
		
*/

#include "../Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#if defined(LN_OS_WIN32)
	#include "Win32/Win32PlatformWindowManager.h"
	#include <Lumino/Platform/Win32/Win32PlatformWindow.h>
#elif defined(LN_OS_MAC)
    #include "Cocoa/CocoaPlatformWindowManager.h"
#elif defined(LN_X11)
	#include "X11/X11WindowManager.h"
#endif
#include "PlatformManager.h"


LN_NAMESPACE_BEGIN

//==============================================================================
// PlatformManager
//==============================================================================

//------------------------------------------------------------------------------
PlatformManager::PlatformManager()
	: m_useThread(false)
	, m_windowManager(NULL)
{
}

//------------------------------------------------------------------------------
PlatformManager::PlatformManager(const Settings& settings)
	: m_useThread(false)
	, m_windowManager(NULL)
{
	Initialize(settings);
}

//------------------------------------------------------------------------------
PlatformManager::~PlatformManager()
{
	Dispose();
}

//------------------------------------------------------------------------------
void PlatformManager::Initialize(const Settings& settings)
{
	m_windowCreationSettings = settings.mainWindowSettings;
	m_useThread = settings.useInternalUIThread;

	WindowSystemAPI api = settings.windowSystemAPI;

#if defined(LN_OS_WIN32)
	// select default
	if (api == WindowSystemAPI::Default)
	{
		api = WindowSystemAPI::Win32API;
	}
	// create window manager
	if (api == WindowSystemAPI::Win32API)
	{
		auto m = RefPtr<Win32WindowManager>::MakeRef(0);
		m_windowManager = m.DetachMove();
    }
#elif defined(LN_OS_MAC)
    // select default
    if (api == WindowSystemAPI::Default)
    {
        api = WindowSystemAPI::Cocoa;
    }
    // create window manager
    if (api == WindowSystemAPI::Cocoa)
    {
        auto m = RefPtr<CocoaPlatformWindowManager>::MakeRef();
        m->Initialize();
        m_windowManager = m.DetachMove();
    }

#elif defined(LN_X11)
	// select default
	if (api == WindowSystemAPI::Default)
	{
		api = WindowSystemAPI::X11;
	}
	// create window manager
	if (api == WindowSystemAPI::X11)
	{
		auto m = RefPtr<X11WindowManager>::MakeRef();
		m_windowManager = m.DetachMove();
	}
#endif
	LN_THROW(m_windowManager != nullptr, ArgumentException);

	if (m_useThread) {
		m_mainWindowThreadInitFinished.SetFalse();
		m_mainWindowThreadEndRequested.SetFalse();
		m_mainWindowThread.Start(CreateDelegate(this, &PlatformManager::Thread_MainWindow));
		m_mainWindowThreadInitFinished.Wait();	// 初期化終了まで待機する
	}
	else {
		m_windowManager->CreateMainWindow(m_windowCreationSettings);
	}

	// MainWindow
	//m_mainWindow = LN_NEW PlatformWindow(m_windowManager->GetMainWindow());
    
    m_windowManager->GetMainWindow()->SetVisible(true);
}

//------------------------------------------------------------------------------
PlatformWindow* PlatformManager::GetMainWindow()
{
	return m_windowManager->GetMainWindow();
}

//------------------------------------------------------------------------------
bool PlatformManager::DoEvents()
{
	// メインスレッドでメッセージ処理する場合は InternalDoEvents
	if (!m_useThread) {
		m_windowManager->DoEvents();
	}

	return !m_windowManager->IsEndRequested();
}

//------------------------------------------------------------------------------
void PlatformManager::Dispose()
{
	if (m_windowManager != NULL)
	{
		// 別スレッドでメッセージ処理していた場合異はスレッド終了待機
		if (m_useThread) {
			m_mainWindowThreadEndRequested.SetTrue();	// 終了要求だして、
			m_mainWindowThread.Wait();					// 待つ
		}
		// メインスレッドでメッセージ処理していた場合はここで Destroy
		else {
			m_windowManager->Finalize();
		}
		//LN_SAFE_RELEASE(m_mainWindow);
		LN_SAFE_RELEASE(m_windowManager);
	}
}

//------------------------------------------------------------------------------
void PlatformManager::Thread_MainWindow()
{
	// 初期化
	m_windowManager->CreateMainWindow(m_windowCreationSettings);
	m_mainWindowThreadInitFinished.SetTrue();	// 初期化完了

	// メッセージループ
	while (!m_mainWindowThreadEndRequested.IsTrue())
	{
		m_windowManager->DoEvents();
		Threading::Thread::Sleep(10);
	}

	// 終了処理
	m_windowManager->Finalize();
}

LN_NAMESPACE_END
