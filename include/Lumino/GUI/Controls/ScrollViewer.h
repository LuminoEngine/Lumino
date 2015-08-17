
#pragma once
#include "../UIElement.h"
#include "../ContentPresenter.h"
#include "ScrollBar.h"
#include "IScrollInfo.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class ScrollContentPresenter
	: public ContentPresenter
	, public IScrollInfo
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollContentPresenter);
public:
	static ScrollContentPresenterPtr Create();

	ScrollContentPresenter(GUIManager* manager);
	virtual ~ScrollContentPresenter();

protected:
	// IScrollInfo override
	virtual void SetCanHorizontallyScroll(bool enabled) { m_scrollData.CanHorizontallyScroll = enabled; }
	virtual bool CanHorizontallyScroll() const { return m_scrollData.CanHorizontallyScroll; }
	virtual void SetCanVerticallyScroll(bool enabled) { m_scrollData.CanVerticallyScroll = enabled; }
	virtual bool CanVerticallyScroll() const { return m_scrollData.CanVerticallyScroll; }
	virtual float GetExtentWidth() const { return m_scrollData.Extent.Width; }
	virtual float GetExtentHeight() const { return m_scrollData.Extent.Height; }
	virtual float GetViewportWidth() const { return m_scrollData.Viewport.Width; }
	virtual float GetViewportHeight() const { return m_scrollData.Viewport.Height; }
	virtual void SetHorizontalOffset(float offset) { m_scrollData.Offset.X = offset; /*TODO: InvalidateArrange();*/ }
	virtual float GetHorizontalOffset() const { return m_scrollData.Offset.X; }
	virtual void SetVerticalOffset(float offset) { m_scrollData.Offset.Y = offset; /*TODO: InvalidateArrange();*/ }
	virtual float GetVerticalOffset() const { return m_scrollData.Offset.Y; }
	virtual void SetScrollOwner(ScrollViewer* owner) { m_scrollData.ScrollOwner = owner; }
	virtual void LineUp() { if (IsScrollClient()){ SetHorizontalOffset(GetHorizontalOffset() - m_scrollLineDelta); } }
	virtual void LineDown() { if (IsScrollClient()){ SetHorizontalOffset(GetHorizontalOffset() + m_scrollLineDelta); } }
	virtual void LineLeft()	{ if (IsScrollClient()){ SetVerticalOffset(GetVerticalOffset() - m_scrollLineDelta); } }
	virtual void LineRight() { if (IsScrollClient()){ SetVerticalOffset(GetVerticalOffset() + m_scrollLineDelta); } }

	// UIElement override
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

private:
	bool IsScrollClient() const;
	void HookupScrollingComponents();

private:
	static const float m_scrollLineDelta;
	ScrollData		m_scrollData;
	IScrollInfo*	m_scrollInfo;		///< ビジュアルの子要素に IScrollInfo がいた場合はそれを指す。いなければ this を指す。
};

/**
	@brief

	@par	ScrollViewer コントロールの名前付きパーツ
	PART_ScrollContentPresenter	ScrollContentPresenter	ScrollViewer のコンテンツのプレースホルダーです。
	PART_VerticalScrollBar	ScrollBar	コンテンツを垂直方向にスクロールするために使用する ScrollBar です。
	PART_HorizontalScrollBar	ScrollBar	コンテンツを水平方向にスクロールするために使用する ScrollBar です。
*/
class ScrollViewer
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollViewer);
public:
	static const String PART_ScrollContentPresenterTemplateName;
	static const String PART_VerticalScrollBarTemplateName;
	static const String PART_HorizontalScrollBarTemplateName;

public:
	static ScrollViewerPtr Create();

	ScrollViewer(GUIManager* manager);
	virtual ~ScrollViewer();

	//-------------------------------------------------------------------
	/** @name Properties */
	/** @{ */


	void SetHorizontalOffset(float offset);

	void SetVerticalOffset(float offset);


	/** @} */

	/// ビューに表示している範囲の幅を取得します。(通常、ActualSize - ScrollBarSize となる)
	float GetViewportWidth() const;

	/// ビューに表示している範囲の高さを取得します。(通常、ActualSize - ScrollBarSize となる)
	float GetViewportHeight() const;

	/// スクロールできるコンテンツ要素の幅を取得します。(通常、ExtentSize - ViewportSize となる)
	float GetScrollableWidth() const;

	/// スクロールできるコンテンツ要素の高さを取得します。(通常、ExtentSize - ViewportSize となる)
	float GetScrollableHeight() const;

	/// スクロールした横方向のオフセットを取得します。
	float GetHorizontalOffset() const;

	/// スクロールした縦方向のオフセットを取得します。
	float GetVerticalOffset() const;




	/// スクロール対象となる Panel 全体のサイズ。子要素を追加し、レイアウトを更新するとこの値も更新される。
	const Size& GetExtentSize() const;



	/// コンテンツを 1 行分下にスクロールします。
	void LineDown();

	/// コンテンツを 1 列分左にスクロールします。
	void LineLeft();

	/// コンテンツを 1 列分右にスクロールします。
	void LineRight();

	/// コンテンツを 1 行分上にスクロールします。
	void LineUp();

	/// コンテンツの末尾まで垂直方向にスクロールします。
	void ScrollToBottom();

	/// コンテンツの先頭まで水平方向にスクロールします。
	void ScrollToLeftEnd();

	/// コンテンツの末尾まで水平方向にスクロールします。
	void ScrollToRightEnd();

	/// コンテンツの先頭まで垂直方向にスクロールします。
	void ScrollToTop();

	/// 指定された水平オフセット位置までスクロールします。
	void ScrollToHorizontalOffset(double offset);

	/// 指定された垂直オフセット位置までスクロールします。
	void ScrollToVerticalOffset(double offset);

protected:
	void OnLayoutUpdated();
	virtual void PollingTemplateChildCreated(UIElement* newElement);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	//OnMouseWheel

private:
	friend class ScrollContentPresenter;
	void SetScrollInfo(IScrollInfo* scrollInfo);

	void Handler_ScrollBar_Scroll(ScrollEventArgs* e);


private:
	ScrollContentPresenter*	m_scrollContentPresenter;	///< VisualTree 内の ScrollContentPresenter
	ScrollBar*				m_verticalScrollBar;		///< VisualTree 内の VerticalScrollBar
	ScrollBar*				m_horizontalScrollBar;		///< VisualTree 内の HorizontalScrollBar
	IScrollInfo*			m_scrollInfo;				///< スクロール操作対象
};


} // namespace GUI
} // namespace Lumino
