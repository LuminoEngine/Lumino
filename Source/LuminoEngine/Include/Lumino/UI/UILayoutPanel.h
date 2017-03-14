
#pragma once
#include "UIElement.h"
#include "UIElementCollection.h"

LN_NAMESPACE_BEGIN
class UIStackPanel;
class UICanvas;
class UIGridLayout;
using UIStackPanelPtr = RefPtr<UIStackPanel>;
using UICanvasPtr = RefPtr<UICanvas>;
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
	LN_UI_TYPEINFO_DECLARE();
public:
	void AddChild(UIElement* element);
	void RemoveChild(UIElement* element);

LN_PROTECTED_INTERNAL_ACCESS:
	UILayoutPanel();
	virtual ~UILayoutPanel();
	void Initialize(detail::UIManager* manager);
	UIElementCollection* GetChildren() const { return m_children; }

	// UIElement interface
	virtual int GetVisualChildrenCount() const override;
	virtual UIElement* GetVisualChild(int index) const override;
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

	// IUIElementCollectionOwner interface
	virtual void OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e) override;

	// ILayoutPanel interface
	virtual int GetLayoutChildrenCount() override;
	virtual ILayoutElement* GetLayoutChild(int index) override;
	virtual int GetLayoutGridColumnDefinitionCount() override;
	virtual detail::GridDefinitionData* GetLayoutGridColumnDefinition(int index) override;
	virtual int GetLayoutGridRowDefinitionCount() override;
	virtual detail::GridDefinitionData* GetLayoutGridRowDefinition(int index) override;

	// IScrollInfo interface
	virtual float GetExtentWidth() const;
	virtual float GetExtentHeight() const;
	virtual float GetViewportWidth() const;
	virtual float GetViewportHeight() const;
	virtual void SetHorizontalOffset(float offset);
	virtual float GetHorizontalOffset() const;
	virtual void SetVerticalOffset(float offset);
	virtual float GetVerticalOffset() const;

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
	LN_UI_TYPEINFO_DECLARE();
public:
	static UIStackPanelPtr Create();

public:
	void SetOrientation(Orientation orientation) { m_orientation = orientation; }
	Orientation GetOrientation() const { return m_orientation; }

LN_PROTECTED_INTERNAL_ACCESS:
	UIStackPanel();
	virtual ~UIStackPanel();
	void Initialize(detail::UIManager* manager);

	// UIElement interface
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

private:
	Orientation	m_orientation;
};

/**
	@brief		
*/
class UICanvas
	: public UILayoutPanel
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static UICanvasPtr Create();

LN_PROTECTED_INTERNAL_ACCESS:
	UICanvas();
	virtual ~UICanvas();
	void Initialize(detail::UIManager* manager);

	// UIElement interface
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

private:
};


/**
	@brief		
*/
class UIGridLayout
	: public UILayoutPanel
{
	LN_UI_TYPEINFO_DECLARE();
public:

	/**
		@brief		UIGridLayout オブジェクトを作成します。
	*/
	static UIGridLayoutPtr Create();

	/**
		@brief		指定した数の行列をもつ UIGridLayout オブジェクトを作成します。
		@details	各セルの幅と高さは GridLengthType::Ratio となります。
	*/
	static UIGridLayoutPtr Create(int columnCount, int rowCount);

public:

	void SetGridSize(int columnCount, int rowCount);
	void AddColumnDefinition(GridLengthType type = GridLengthType::Ratio, float width = 1.0f, float minWidth = 0.0f, float maxWidth = FLT_MAX);
	void AddRowDefinition(GridLengthType type = GridLengthType::Ratio, float height = 1.0f, float minHeight = 0.0f, float maxHeight = FLT_MAX);

LN_PROTECTED_INTERNAL_ACCESS:
	UIGridLayout();
	virtual ~UIGridLayout();
	void Initialize(detail::UIManager* manager);

	// UIElement interface
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

	// ILayoutPanel interface
	virtual int GetLayoutGridColumnDefinitionCount() override;
	virtual detail::GridDefinitionData* GetLayoutGridColumnDefinition(int index) override;
	virtual int GetLayoutGridRowDefinitionCount() override;
	virtual detail::GridDefinitionData* GetLayoutGridRowDefinition(int index) override;

private:
	class DefinitionBase;
	class ColumnDefinition;
	class RowDefinition;
	List<RefPtr<ColumnDefinition>>	m_columnDefinitions;
	List<RefPtr<RowDefinition>>	m_rowDefinitions;
};

LN_NAMESPACE_END
