/*
	ScrollViewer の概要
	https://msdn.microsoft.com/ja-jp/library/ms750665(v=vs.110).aspx

	ScrollViewer の各種プロパティ
	http://smart-pda.net/wp7/tips/scrollviewer_tips/

	ScrollViewer はメソッド呼び出しを (一般的には一度コマンドキューに溜め込んで) 
	ScrollViewer.ScrollInfo に通知する。
	ScrollViewer が子要素のオフセットを計算するのではない点に注意。
	
	ScrollContentPresenter は Child(IScrollInfo) を、TemplatedParent(ScrollViewer).ScrollInfo にセットする。
	(HookupScrollingComponents で検索)

	IScrollInfo は StackPanel や DocumentViewer、TextBoxView 等として実装される。
	実際に子要素のオフセットを計算するのはこれらのクラス。
	これを実現するために、各クラス ScrollData という内部クラスを持っている。

*/
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/ScrollViewer.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// ScrollContentPresenter
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollContentPresenter, ContentPresenter);
LN_UI_ELEMENT_SUBCLASS_IMPL(ScrollContentPresenter);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollContentPresenter::ScrollContentPresenter(GUIManager* manager)
	: ContentPresenter(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollContentPresenter::~ScrollContentPresenter()
{
}

//=============================================================================
// ScrollViewer
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollViewer, ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(ScrollViewer);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollViewer::ScrollViewer(GUIManager* manager)
	: ContentControl(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollViewer::~ScrollViewer()
{
}

} // namespace GUI
} // namespace Lumino

