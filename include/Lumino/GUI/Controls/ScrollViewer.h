
#pragma once
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{
class ScrollViewer;

/**
	@brief	ScrollViewer コントロール内のメインのスクロール可能領域を表します。
*/
class IScrollInfo
{
public:
	//virtual bool CanVerticallyScroll() const = 0;
	//virtual bool CanHorizontallyScroll() const = 0;

	/// エクステント (コンテンツ全体) の横幅を取得します。
	virtual float GetExtentWidth() const = 0;

	/// エクステント (コンテンツ全体) の縦幅を取得します。
	virtual float GetExtentHeight() const = 0;

	/// コンテンツに対する実際の表示領域の横幅を取得します。
	virtual float GetViewportWidth() const = 0;

	/// コンテンツに対する実際の表示領域の縦幅を取得します。
	virtual float GetViewportHeight() const = 0;

	/// スクロールしたコンテンツの水平オフセットを取得します。
	virtual float GetHorizontalOffset() const = 0;

	/// スクロールしたコンテンツの水平オフセットを設定します。
	virtual void SetHorizontalOffset(float offset) const = 0;

	/// スクロールしたコンテンツの垂直オフセットを取得します。
	virtual float GetVerticalOffset() const = 0;

	/// スクロールしたコンテンツの水平オフセットを設定します。
	virtual void SetVerticalOffset(float offset) const = 0;

	/// スクロール動作を制御する ScrollViewer 要素を設定します。
	virtual void SetScrollOwner(ScrollViewer* owner) const = 0;

	/// コンテンツ内を 1 論理単位ずつ上にスクロールします。
	virtual void LineUp() = 0;

	/// コンテンツ内を 1 論理単位ずつ下にスクロールします。
	virtual void LineDown() = 0;

	/// コンテンツ内を 1 論理単位ずつ左にスクロールします。
	virtual void LineLeft() = 0;

	/// コンテンツ内を 1 論理単位ずつ右にスクロールします。
	virtual void LineRight() = 0;
	
#if 0
	/// コンテンツ内を 1 ページずつ上にスクロールします。
	virtual void PageUp() = 0;

	/// コンテンツ内を 1 ページずつ下にスクロールします。
	virtual void PageDown() = 0;

	/// コンテンツ内を 1 ページずつ左にスクロールします。
	virtual void PageLeft() = 0;

	/// コンテンツ内を 1 ページずつ右にスクロールします。
	virtual void PageRight() = 0;

	/// コンテンツ内をマウスホイール操作 1 回分、上にスクロールします。
	virtual void MouseWheelUp() = 0;

	/// コンテンツ内をマウスホイール操作 1 回分、下にスクロールします。
	virtual void MouseWheelDown() = 0;

	/// コンテンツ内をマウスホイール操作 1 回分、左にスクロールします。
	virtual void MouseWheelLeft() = 0;

	/// コンテンツ内をマウスホイール操作 1 回分、右にスクロールします。
	virtual void MouseWheelRight() = 0;
#endif

	//Rect MakeVisible(UIElement visual, Rect rectangle);
};
	
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
	virtual Size MeasureOverride(Size availableSize);
	virtual Size ArrangeOverride(Size finalSize);
	//OnMouseWheel
};


} // namespace GUI
} // namespace Lumino
