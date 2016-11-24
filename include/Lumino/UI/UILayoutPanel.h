
#pragma once
#include "UIElement.h"
#include "UIElementCollection.h"

LN_NAMESPACE_BEGIN
class UICanvas;
class UIGridLayout;
using UICanvasPtr = RefPtr<UICanvas>;
using UIGridLayoutPtr = RefPtr<UIGridLayout>;

/** コントロールのレイアウト方向を示します。*/
LN_ENUM(Orientation)
{
	Vertical = 0,		/**< 垂直方向に配置します。*/
	Horizontal,			/**< 水平方向に配置します。*/
};
LN_ENUM_DECLARE(Orientation);

/**
	@brief		1つ以上の子 UIElement を持つ UI 要素のベースクラスです。

	@details	HAlignment 及び VAlignment の規定値は Stretch です。
*/
class UILayoutPanel
	: public UIElement
	, public tr::IUIElementCollectionOwner
	, public detail::ILayoutPanel
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
	virtual UIElement* GetVisualChildOrderd(int index) const override;
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

	// IUIElementCollectionOwner interface
	virtual void OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e) override;

	// ILayoutPanel interface
	virtual int GetLayoutChildrenCount() const;
	virtual ILayoutElement* GetLayoutChild(int index) const;
	virtual int GetLayoutGridColumnDefinitionCount() const;
	virtual detail::GridDefinitionData* GetLayoutGridColumnDefinition(int index) const;
	virtual int GetLayoutGridRowDefinitionCount() const;
	virtual detail::GridDefinitionData* GetLayoutGridRowDefinition(int index) const;

private:
	RefPtr<UIElementCollection>	m_children;
};

/**
	@brief		
*/
class UIStackPanel
	: public UILayoutPanel
{
	LN_UI_TYPEINFO_DECLARE();
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
	virtual int GetLayoutGridColumnDefinitionCount() const;
	virtual detail::GridDefinitionData* GetLayoutGridColumnDefinition(int index) const;
	virtual int GetLayoutGridRowDefinitionCount() const;
	virtual detail::GridDefinitionData* GetLayoutGridRowDefinition(int index) const;

private:
	class DefinitionBase;
	class ColumnDefinition;
	class RowDefinition;
	List<RefPtr<ColumnDefinition>>	m_columnDefinitions;
	List<RefPtr<RowDefinition>>	m_rowDefinitions;
};

LN_NAMESPACE_END
