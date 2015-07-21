
#pragma once
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{
class ScrollViewer;

	
/**
	@brief
*/
class ScrollContentPresenter
	: public ContentPresenter
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollContentPresenter);
public:
	ScrollContentPresenter(GUIManager* manager);
	virtual ~ScrollContentPresenter();

private:
	void HookupScrollingComponents();
};

/**
	@brief

	@par	ScrollViewer コントロールの名前付きパーツ
	PART_ScrollContentPresenter	ScrollContentPresenter	ScrollViewer のコンテンツのプレースホルダーです。
	PART_HorizontalScrollBar	ScrollBar	コンテンツを水平方向にスクロールするために使用する ScrollBar です。
	PART_VerticalScrollBar	ScrollBar	コンテンツを垂直方向にスクロールするために使用する ScrollBar です。
*/
class ScrollViewer
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollViewer);
public:
	static ScrollViewer* Create(GUIManager* manager);

	ScrollViewer(GUIManager* manager);
	virtual ~ScrollViewer();

	//-------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

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
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	//OnMouseWheel
};


} // namespace GUI
} // namespace Lumino
