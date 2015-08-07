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


	ListBox の Temlate は2パターン。
	ItemsPresenter を使う方法と、使わない方法。

	使わない場合は Panel の IsItemsHost を true にする。この場合、VisualTree に ItemsPresenter は現れない。自動生成されたりしない。
	
	使う場合、ScrollContentPresenter の子は ItemsPresenter になる。
	その ItemsPresenter の子は、Panel になる。
	このとき、その Panel が IScrollInfo を実装していれば、スクロール操作の対象になる。
	ちなみに、ItemsPresenter は IScrollInfo ではない。

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

const float ScrollContentPresenter::m_scrollLineDelta = 16.0f;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollContentPresenterPtr ScrollContentPresenter::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollContentPresenter::ScrollContentPresenter(GUIManager* manager)
	: ContentPresenter(manager)
	, m_scrollData()
	, m_scrollInfo(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollContentPresenter::~ScrollContentPresenter()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollContentPresenter::OnApplyTemplate(CombinedLocalResource* localResource)
{
	ContentPresenter::OnApplyTemplate(localResource);
	HookupScrollingComponents();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ScrollContentPresenter::MeasureOverride(const SizeF& constraint)
{
	SizeF desiredSize;
	if (IsScrollClient())
	{
		SizeF childConstraint = constraint;

		// 縦横それぞれ、スクロールできるのであればサイズ制限を設けない
		if (m_scrollData.CanHorizontallyScroll) { childConstraint.Width = std::numeric_limits<float>::infinity(); }
		if (m_scrollData.CanVerticallyScroll) { childConstraint.Height = std::numeric_limits<float>::infinity(); }

		desiredSize = ContentPresenter::MeasureOverride(childConstraint);	// 子要素の Measure はこの中で。

		//m_scrollData.Viewport = constraint;
		m_scrollData.Extent = desiredSize;
	}
	else
	{
		// 子要素に対してスクロール操作する場合は、Measure も子要素に任せる
		desiredSize = ContentPresenter::MeasureOverride(constraint);		// 子要素の Measure はこの中で。
	}

	desiredSize.Width = std::min(constraint.Width, desiredSize.Width);
	desiredSize.Height = std::min(constraint.Height, desiredSize.Height);
	return desiredSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ScrollContentPresenter::ArrangeOverride(const SizeF& finalSize)
{
	if (IsScrollClient())
	{
		m_scrollData.Viewport = finalSize;
	}

	if (!m_visualChildren.IsEmpty())
	{
		UIElement* child = m_visualChildren[0];
		if (child != NULL)
		{
			RectF childRect(PointF::Zero, child->GetDesiredSize());

			if (IsScrollClient())
			{
				childRect.X = -GetHorizontalOffset();
				childRect.Y = -GetVerticalOffset();
			}
			childRect.Width = std::max(childRect.Width, finalSize.Width);
			childRect.Height = std::max(childRect.Height, finalSize.Height);

			//printf("%f\n", childRect.X);

			child->ArrangeLayout(childRect);
		}
	}

	return finalSize;
}

//-----------------------------------------------------------------------------
// この ScrollContentPresenter が、子要素のオフセットを直接操作するかどうか。
// つまり、子要素は IScrollInfo ではないか。
//-----------------------------------------------------------------------------
bool ScrollContentPresenter::IsScrollClient() const
{
	return m_scrollInfo == this;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollContentPresenter::HookupScrollingComponents()
{
	ScrollViewer* owner = dynamic_cast<ScrollViewer*>(m_templateParent);
	LN_VERIFY_RETURN(owner != NULL);	// ありえないんだけれども。

	// TODO: ... いろいろ子要素を探って IScrollInfo を探す
	// TODO: バグ。なんか ItemsPresenter がたくさん追加されていた・・・
	IScrollInfo* scrollInfo = NULL;

	// 子、孫に IScrollInfo が見つからなかった場合は、this をスクロール操作の対象とする
	if (scrollInfo == NULL)
	{
		scrollInfo = this;
	}

	// 見つかった IScrollInfo と、オーナーの ScrollViewer を結びつける
	scrollInfo->SetScrollOwner(owner);
	owner->SetScrollInfo(scrollInfo);

	m_scrollInfo = scrollInfo;
}

//=============================================================================
// ScrollViewer
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollViewer, ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(ScrollViewer);

const String ScrollViewer::PART_ScrollContentPresenterTemplateName(_T("PART_ScrollContentPresenter"));
const String ScrollViewer::PART_VerticalScrollBarTemplateName(_T("PART_VerticalScrollBar"));
const String ScrollViewer::PART_HorizontalScrollBarTemplateName(_T("PART_HorizontalScrollBar"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollViewerPtr ScrollViewer::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollViewer::ScrollViewer(GUIManager* manager)
	: ContentControl(manager)
{
	LN_REGISTER_ROUTED_EVENT_HANDLER(ScrollViewer, ScrollEventArgs, ScrollBar::ScrollEvent, Handler_ScrollBar_Scroll);

	//LayoutUpdated += LN_CreateDelegate(this, &ScrollViewer::ScrollViewer_LayoutUpdated);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollViewer::~ScrollViewer()
{
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollViewer::SetHorizontalOffset(float offset)
{
	if (m_scrollInfo != NULL) {
		m_scrollInfo->SetHorizontalOffset(offset);
		m_horizontalScrollBar->SetValue(offset);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollViewer::SetVerticalOffset(float offset)
{
	if (m_scrollInfo != NULL) {
		m_scrollInfo->SetVerticalOffset(offset);
		m_verticalScrollBar->SetValue(offset);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollViewer::OnLayoutUpdated()
{
	if (m_scrollInfo != NULL)
	{
		m_verticalScrollBar->SetTrackExtent(m_scrollInfo->GetExtentHeight() - m_scrollInfo->GetViewportHeight());
		m_verticalScrollBar->SetTrackViewportSize(m_scrollInfo->GetViewportHeight());

		m_horizontalScrollBar->SetTrackExtent(m_scrollInfo->GetExtentWidth() - m_scrollInfo->GetViewportWidth());
		m_horizontalScrollBar->SetTrackViewportSize(m_scrollInfo->GetViewportWidth());
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollViewer::PollingTemplateChildCreated(UIElement* newElement)
{
	// PART のキーに一致する名前を持つ要素が見つかったらメンバに保持しておく
	if (newElement->GetKeyName() == PART_ScrollContentPresenterTemplateName) {
		m_scrollContentPresenter = dynamic_cast<ScrollContentPresenter*>(newElement);
	}
	else if (newElement->GetKeyName() == PART_VerticalScrollBarTemplateName) {
		m_verticalScrollBar = dynamic_cast<ScrollBar*>(newElement);
		m_verticalScrollBar->m_isStandalone = false;	// ScrollViewer で面倒を見るフラグ
	}
	else if (newElement->GetKeyName() == PART_HorizontalScrollBarTemplateName) {
		m_horizontalScrollBar = dynamic_cast<ScrollBar*>(newElement);
		m_horizontalScrollBar->m_isStandalone = false;	// ScrollViewer で面倒を見るフラグ
	}

	ContentControl::PollingTemplateChildCreated(newElement);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ScrollViewer::MeasureOverride(const SizeF& constraint)
{
	// スクロールバー自動表示の処理はここでする

	return ContentControl::MeasureOverride(constraint);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ScrollViewer::ArrangeOverride(const SizeF& finalSize)
{
	return ContentControl::ArrangeOverride(finalSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollViewer::SetScrollInfo(IScrollInfo* scrollInfo)
{
	m_scrollInfo = scrollInfo;
	// TODO:
	//m_scrollInfo->SetCanVerticallyScroll();
	//m_scrollInfo->SetCanHorizontallyScroll();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollViewer::Handler_ScrollBar_Scroll(ScrollEventArgs* e)
{
	if (m_scrollInfo != NULL)
	{
		switch (e->Type)
		{
		case ScrollEventType::ThumbTrack:
			// 本来は一度コマンドリスト化して、遅延に備えるべき
			if (e->Sender == m_verticalScrollBar) {
				SetVerticalOffset(e->NewValue);
				//m_scrollInfo->SetVerticalOffset(e->NewValue);
				//printf("%f\n", e->NewValue);
			}
			else if (e->Sender == m_horizontalScrollBar) {
				SetHorizontalOffset(e->NewValue);
				//m_scrollInfo->SetHorizontalOffset(e->NewValue);
			}
			break;
		default:
			break;
		}
	}
}

} // namespace GUI
} // namespace Lumino

