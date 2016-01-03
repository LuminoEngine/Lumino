/*
	[2016/1/1] いろいろなイベントハンドリング
		・オーバーライド
			UIElement 等を継承し、OnMouseMove() 等をオーバーライドする方法。

		・動的なハンドラ登録
			button.clicked += handler; のように、コントロールを使う人がイベントを登録する方法。

		・静的なハンドラ登録
			オーバーライドとは別に、派生クラスがベースに対してハンドラ関数を登録する方法。
			静的なハンドラは、動的なハンドラが呼び出される前に呼ばれる。

		WPF では、On～ は内部で RaiseEvent を呼び出すのが目的。
		つまり、子要素からルーティングで通知されてきたイベントを On～ でハンドリングすることはできない。

		Lumino としては On～ をルーティング通知で呼び出されるようにしてしまっていいと思う。
		わかりやすさ重視の方向で。

	[2016/1/1] ドッキングウィンドウめも
		メインウィンドウは通常通り。
		AvalonDock のように、DockManager コントロールを配置する。
		サブウィンドウは Platform モジュールの NativeWindow を継承したウィンドウを作った方が良いかもしれない。

*/
#include "Internal.h"
#include <Lumino/UI/UIContext.h>
#include "EventArgsPool.h"
#include "UIManager.h"

LN_NAMESPACE_BEGIN
namespace detail
{
	
//=============================================================================
// UIManager
//=============================================================================
const float UIManager::MouseButtonClickTimeout = 0.3f;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIManager::UIManager()
	: m_eventArgsPool(nullptr)
	, m_graphicsManager(nullptr)
	, m_defaultUIContext(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIManager::~UIManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIManager::Initialize(const Settings& settings)
{
	m_eventArgsPool = LN_NEW EventArgsPool();
	m_graphicsManager = settings.graphicsManager;
	m_mainWindow = settings.mainWindow;

	m_defaultUIContext = LN_NEW UIContext();
	m_defaultUIContext->Initialize(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIManager::Finalize()
{
	LN_SAFE_RELEASE(m_defaultUIContext);
	LN_SAFE_DELETE(m_eventArgsPool);
}



} // namespace detail
LN_NAMESPACE_END
