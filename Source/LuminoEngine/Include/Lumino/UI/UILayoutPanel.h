
#pragma once
#include "UIElement.h"
#include "UIElementCollection.h"

LN_NAMESPACE_BEGIN
class UIStackPanel;
class UIAbsoluteLayout;
class UIGridLayout;
using UIStackPanelPtr = RefPtr<UIStackPanel>;
using UIAbsoluteLayoutPtr = RefPtr<UIAbsoluteLayout>;
using UIGridLayoutPtr = RefPtr<UIGridLayout>;


/**
	@brief        ScrollViewer コントロール内のメインのスクロール可能領域を表します。
*/
class IScrollInfo
{
public:

	///// 水平軸上にスクロールできるかどうかを示す値を設定します。
	///// @note    スクロールバーを表示しない設定になっていたり、そもそも存在しない場合に false がセットされる。
	/////            IScrollInfo の実装クラスで不要な計算を行わないなどのために参照する。
	//virtual void SetCanHorizontallyScroll(bool enabled) = 0;

	///// 水平軸上にスクロールできるかどうかを示す値を取得します。
	//virtual bool CanHorizontallyScroll() const = 0;

	///// 垂直軸上にスクロールできるかどうかを示す値を設定します。
	//virtual void SetCanVerticallyScroll(bool enabled) = 0;

	///// 垂直軸上にスクロールできるかどうかを示す値を取得します。
	//virtual bool CanVerticallyScroll() const = 0;

	/// エクステント (コンテンツ全体) の横幅を取得します。
	virtual float getExtentWidth() const = 0;

	/// エクステント (コンテンツ全体) の縦幅を取得します。
	virtual float getExtentHeight() const = 0;

	/// コンテンツに対する実際の表示領域の横幅を取得します。
	virtual float getViewportWidth() const = 0;

	/// コンテンツに対する実際の表示領域の縦幅を取得します。
	virtual float getViewportHeight() const = 0;

	/// スクロールしたコンテンツの水平オフセットを設定します。
	virtual void setHorizontalOffset(float offset) = 0;

	/// スクロールしたコンテンツの水平オフセットを取得します。
	virtual float getHorizontalOffset() const = 0;

	/// スクロールしたコンテンツの水平オフセットを設定します。
	virtual void setVerticalOffset(float offset) = 0;

	/// スクロールしたコンテンツの垂直オフセットを取得します。
	virtual float getVerticalOffset() const = 0;

	/// スクロール動作を制御する ScrollViewer 要素を設定します。
	//virtual void SetScrollOwner(ScrollViewer* owner) = 0;

#if 0
	/// コンテンツ内を 1 論理単位ずつ上にスクロールします。
	virtual void LineUp() = 0;

	/// コンテンツ内を 1 論理単位ずつ下にスクロールします。
	virtual void LineDown() = 0;

	/// コンテンツ内を 1 論理単位ずつ左にスクロールします。
	virtual void LineLeft() = 0;

	/// コンテンツ内を 1 論理単位ずつ右にスクロールします。
	virtual void LineRight() = 0;

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
	@brief		1つ以上の子 UIElement を持つ UI 要素のベースクラスです。

	@details	HAlignment 及び VAlignment の規定値は Stretch です。
*/
class UILayoutPanel
	: public UIElement
	, public tr::IUIElementCollectionOwner
	, public detail::ILayoutPanel
	, public IScrollInfo
{
	LN_OBJECT();
public:
	void addChild(UIElement* element);
	void removeChild(UIElement* element);

LN_PROTECTED_INTERNAL_ACCESS:
	UILayoutPanel();
	virtual ~UILayoutPanel();
	void initialize();
	UIElementCollection* getChildren() const { return m_children; }

	// UIElement interface
	virtual int getVisualChildrenCount() const override;
	virtual UIElement* getVisualChild(int index) const override;
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

	// IUIElementCollectionOwner interface
	virtual void onChildCollectionChanged(const tr::ChildCollectionChangedArgs& e) override;

	// ILayoutPanel interface
	virtual int getLayoutChildrenCount() override;
	virtual ILayoutElement* getLayoutChild(int index) override;
	virtual int getLayoutGridColumnDefinitionCount() override;
	virtual detail::GridDefinitionData* getLayoutGridColumnDefinition(int index) override;
	virtual int getLayoutGridRowDefinitionCount() override;
	virtual detail::GridDefinitionData* getLayoutGridRowDefinition(int index) override;

	// IScrollInfo interface
	virtual float getExtentWidth() const;
	virtual float getExtentHeight() const;
	virtual float getViewportWidth() const;
	virtual float getViewportHeight() const;
	virtual void setHorizontalOffset(float offset);
	virtual float getHorizontalOffset() const;
	virtual void setVerticalOffset(float offset);
	virtual float getVerticalOffset() const;

private:
	RefPtr<UIElementCollection>	m_children;
	Vector2						m_scrollOffset;
};

/**
	@brief		
*/
class UIStackPanel
	: public UILayoutPanel
{
	LN_OBJECT();
public:
	static UIStackPanelPtr create();

public:
	void setOrientation(Orientation orientation) { m_orientation = orientation; }
	Orientation getOrientation() const { return m_orientation; }

LN_PROTECTED_INTERNAL_ACCESS:
	UIStackPanel();
	virtual ~UIStackPanel();
	void initialize();

	// UIElement interface
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

private:
	Orientation	m_orientation;
};

/**
	@brief		
*/
class UIAbsoluteLayout
	: public UILayoutPanel
{
	LN_OBJECT();
public:
	static UIAbsoluteLayoutPtr create();

LN_PROTECTED_INTERNAL_ACCESS:
	UIAbsoluteLayout();
	virtual ~UIAbsoluteLayout();
	void initialize();

	// UIElement interface
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

private:
};


/**
	@brief		
*/
class UIGridLayout
	: public UILayoutPanel
{
	LN_OBJECT();
public:

	/**
		@brief		UIGridLayout オブジェクトを作成します。
	*/
	static UIGridLayoutPtr create();

	/**
		@brief		指定した数の行列をもつ UIGridLayout オブジェクトを作成します。
		@details	各セルの幅と高さは GridLengthType::Ratio となります。
	*/
	static UIGridLayoutPtr create(int columnCount, int rowCount);

public:

	void setGridSize(int columnCount, int rowCount);
	void addColumnDefinition(GridLengthType type = GridLengthType::Ratio, float width = 1.0f, float minWidth = 0.0f, float maxWidth = FLT_MAX);
	void addRowDefinition(GridLengthType type = GridLengthType::Ratio, float height = 1.0f, float minHeight = 0.0f, float maxHeight = FLT_MAX);

LN_PROTECTED_INTERNAL_ACCESS:
	UIGridLayout();
	virtual ~UIGridLayout();
	void initialize();

	// UIElement interface
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

	// ILayoutPanel interface
	virtual int getLayoutGridColumnDefinitionCount() override;
	virtual detail::GridDefinitionData* getLayoutGridColumnDefinition(int index) override;
	virtual int getLayoutGridRowDefinitionCount() override;
	virtual detail::GridDefinitionData* getLayoutGridRowDefinition(int index) override;

private:
	class DefinitionBase;
	class ColumnDefinition;
	class RowDefinition;
	List<RefPtr<ColumnDefinition>>	m_columnDefinitions;
	List<RefPtr<RowDefinition>>	m_rowDefinitions;
};

LN_NAMESPACE_END
