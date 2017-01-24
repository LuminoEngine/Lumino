
#pragma once

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN
class ScrollViewer;

/**
	@brief		ScrollViewer コントロール内のメインのスクロール可能領域を表します。
	@details	Extent、Viewport、Offset の単位はスクロール対象の設定により異なります。
				添付プロパティ ScrollViewer.CanContentScroll が true の場合はアイテム単位、
				false の場合はピクセル単位です。
*/
class IScrollInfo
{
public:
	/// 水平軸上にスクロールできるかどうかを示す値を設定します。
	/// @note	スクロールバーを表示しない設定になっていたり、そもそも存在しない場合に false がセットされる。
	///			IScrollInfo の実装クラスで不要な計算を行わないなどのために参照する。
	virtual void SetCanHorizontallyScroll(bool enabled) = 0;

	/// 水平軸上にスクロールできるかどうかを示す値を取得します。
	virtual bool CanHorizontallyScroll() const = 0;

	/// 垂直軸上にスクロールできるかどうかを示す値を設定します。
	virtual void SetCanVerticallyScroll(bool enabled) = 0;

	/// 垂直軸上にスクロールできるかどうかを示す値を取得します。
	virtual bool CanVerticallyScroll() const = 0;

	/// エクステント (コンテンツ全体) の横幅を取得します。
	virtual float GetExtentWidth() const = 0;

	/// エクステント (コンテンツ全体) の縦幅を取得します。
	virtual float GetExtentHeight() const = 0;

	/// コンテンツに対する実際の表示領域の横幅を取得します。
	virtual float GetViewportWidth() const = 0;

	/// コンテンツに対する実際の表示領域の縦幅を取得します。
	virtual float GetViewportHeight() const = 0;

	/// スクロールしたコンテンツの水平オフセットを設定します。
	virtual void SetHorizontalOffset(float offset) = 0;

	/// スクロールしたコンテンツの水平オフセットを取得します。
	virtual float GetHorizontalOffset() const = 0;

	/// スクロールしたコンテンツの水平オフセットを設定します。
	virtual void SetVerticalOffset(float offset) = 0;

	/// スクロールしたコンテンツの垂直オフセットを取得します。
	virtual float GetVerticalOffset() const = 0;

	/// スクロール動作を制御する ScrollViewer 要素を設定します。
	virtual void SetScrollOwner(ScrollViewer* owner) = 0;

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

struct ScrollData
{
	ScrollViewer*	ScrollOwner;
	bool			CanHorizontallyScroll;
	bool			CanVerticallyScroll;
	PointF			Offset;
	SizeF			Extent;
	SizeF			Viewport;

	ScrollData()
		: ScrollOwner(NULL)
		, CanHorizontallyScroll(false)
		, CanVerticallyScroll(false)
		, Offset(PointF::Zero)
		, Extent(SizeF::Zero)
		, Viewport(SizeF::Zero)
	{}
};

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
