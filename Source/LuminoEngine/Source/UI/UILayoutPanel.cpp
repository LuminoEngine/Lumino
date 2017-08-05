
#include "Internal.h"
#include <Lumino/UI/UILayoutPanel.h>
#include "UIManager.h"
#include "LayoutImpl.h"
#include "LayoutHelper.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UILayoutPanel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UILayoutPanel, UIElement)

//------------------------------------------------------------------------------
UILayoutPanel::UILayoutPanel()
{
}

//------------------------------------------------------------------------------
UILayoutPanel::~UILayoutPanel()
{
}

//------------------------------------------------------------------------------
void UILayoutPanel::initialize()
{
	UIElement::initialize();
	setHitTestVisible(false);
	m_children = Ref<UIElementCollection>::makeRef(this);

	// Panel 系のデフォルトは Stretch
	//setHAlignment(HAlignment::Stretch);
	//setVAlignment(VAlignment::Stretch);
}

//------------------------------------------------------------------------------
void UILayoutPanel::addChild(UIElement* element)
{
	m_children->add(element);
	element->setLogicalParent(this);
}

//------------------------------------------------------------------------------
void UILayoutPanel::removeChild(UIElement* element)
{
	m_children->remove(element);
	element->setLogicalParent(nullptr);
}

//------------------------------------------------------------------------------
int UILayoutPanel::getVisualChildrenCount() const
{
	return m_children->getCount();
}

//------------------------------------------------------------------------------
UIElement* UILayoutPanel::getVisualChild(int index) const
{
	return m_children->getAt(index);
}

//------------------------------------------------------------------------------
Size UILayoutPanel::measureOverride(const Size& constraint)
{
	return detail::LayoutImpl<UILayoutPanel>::UILayoutPanel_measureOverride(
		this, constraint,
		[](UILayoutPanel* panel, const Size& constraint){ return panel->UIElement::measureOverride(constraint); });
}

//------------------------------------------------------------------------------
Size UILayoutPanel::arrangeOverride(const Size& finalSize)
{
	Vector2 pixelOffset = -m_scrollOffset;

	return detail::LayoutImpl<UILayoutPanel>::UILayoutPanel_arrangeOverride(this, pixelOffset, finalSize);
}

//------------------------------------------------------------------------------
void UILayoutPanel::onChildCollectionChanged(const tr::ChildCollectionChangedArgs& e)
{
	/*
		ListBox や Button からコンテンツを追加する場合もこの関数が呼ばれる。
		このときは VisualChild として追加する。LogicalChild ではない点に注意。
	*/

	// 新しく追加されたものたち
	for (UIElement* element : e.newItems)
	{
		//element->setParent(this);
		addVisualChild(element);
	}

	// 削除されたものたち
	for (UIElement* element : e.oldItems)
	{
		//element->setParent(nullptr);
		removeVisualChild(element);
	}
}

//------------------------------------------------------------------------------
int UILayoutPanel::getLayoutChildrenCount() { return m_children->getCount(); }
ILayoutElement* UILayoutPanel::getLayoutChild(int index) { return m_children->getAt(index); }
int UILayoutPanel::getLayoutGridRowDefinitionCount() { return 0; }
detail::GridDefinitionData* UILayoutPanel::getLayoutGridRowDefinition(int index) { return nullptr; }
int UILayoutPanel::getLayoutGridColumnDefinitionCount() { return 0; }
detail::GridDefinitionData* UILayoutPanel::getLayoutGridColumnDefinition(int index) { return nullptr; }

//------------------------------------------------------------------------------
float UILayoutPanel::getExtentWidth() const { return getDesiredSize().width; }
float UILayoutPanel::getExtentHeight() const { return getDesiredSize().height; }
float UILayoutPanel::getViewportWidth() const { return getActualSize().width; }
float UILayoutPanel::getViewportHeight() const { return getActualSize().height; }
void UILayoutPanel::setHorizontalOffset(float offset) { m_scrollOffset.x = offset; }
float UILayoutPanel::getHorizontalOffset() const { return m_scrollOffset.x; }
void UILayoutPanel::setVerticalOffset(float offset) { m_scrollOffset.y = offset; }
float UILayoutPanel::getVerticalOffset() const { return m_scrollOffset.y; }

//==============================================================================
// UIStackPanel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIStackPanel, UILayoutPanel)

//------------------------------------------------------------------------------
UIStackPanelPtr UIStackPanel::create()
{
	auto ptr = Ref<UIStackPanel>::makeRef();
	ptr->initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UIStackPanel::UIStackPanel()
	: m_orientation(Orientation::Vertical)
{
}

//------------------------------------------------------------------------------
UIStackPanel::~UIStackPanel()
{
}

//------------------------------------------------------------------------------
void UIStackPanel::initialize()
{
	UILayoutPanel::initialize();
}

//------------------------------------------------------------------------------
Size UIStackPanel::measureOverride(const Size& constraint)
{
	return Size::max(
		detail::LayoutImpl<UIStackPanel>::UIStackPanel_measureOverride(this, constraint, m_orientation),
		UIElement::measureOverride(constraint));
}

//------------------------------------------------------------------------------
Size UIStackPanel::arrangeOverride(const Size& finalSize)
{
	return detail::LayoutImpl<UIStackPanel>::UIStackPanel_arrangeOverride(this, finalSize, m_orientation);
}


//==============================================================================
// UIAbsoluteLayout
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIAbsoluteLayout, UILayoutPanel)

//------------------------------------------------------------------------------
UIAbsoluteLayoutPtr UIAbsoluteLayout::create()
{
	auto ptr = UIAbsoluteLayoutPtr::makeRef();
	ptr->initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UIAbsoluteLayout::UIAbsoluteLayout()
{
}

//------------------------------------------------------------------------------
UIAbsoluteLayout::~UIAbsoluteLayout()
{
}

//------------------------------------------------------------------------------
void UIAbsoluteLayout::initialize()
{
	UILayoutPanel::initialize();
}

//------------------------------------------------------------------------------
Size UIAbsoluteLayout::measureOverride(const Size& constraint)
{
	// LayoutPanel ではなく、UIElement の measureOverride を実施 (this のサイズを測る)
	return detail::LayoutHelper2::measureOverride_AbsoluteLayout<UIAbsoluteLayout, UIElement, UIElementCollection>(
		this, constraint, *getChildren());
}

//------------------------------------------------------------------------------
Size UIAbsoluteLayout::arrangeOverride(const Size& finalSize)
{
	return detail::LayoutHelper2::arrangeOverride_AbsoluteLayout<UIAbsoluteLayout, UIElement, UIElementCollection>(
		this, finalSize, *getChildren());
}


//==============================================================================
// DefinitionBase
//==============================================================================
class UIGridLayout::DefinitionBase
	: public Object
{
public:

	DefinitionBase()
	{
	}

	virtual ~DefinitionBase()
	{
	}

	GridLengthType getType() const { return m_data.type; }

	//float getAvailableDesiredSize() const
	//{
	//	if (m_data.type == GridLengthType::Auto) {
	//		return m_data.desiredSize;
	//	}
	//	else if (m_data.type == GridLengthType::Pixel) {
	//		return Math::Clamp(m_data.size, m_data.minSize, m_data.maxSize);
	//	}
	//	else {
	//		return 0;
	//	}
	//}

	//float GetRatioWeight() const
	//{
	//	return (m_data.size == 0.0f) ? 1.0f : m_data.size;
	//}

	//void adjustActualSize()
	//{
	//	m_data.actualSize = Math::Clamp(m_data.actualSize, m_data.minSize, m_data.maxSize);
	//}

LN_INTERNAL_ACCESS:
	detail::GridDefinitionData	m_data;
//	GridLengthType	m_type;
//	float			m_size;
//	float			m_minSize;
//	float			m_maxSize;
//
//public:
//	float			m_desiredSize;
//	float			m_actualOffset;	// 最終オフセット
//	float			m_actualSize;	// 最終サイズ
};

//==============================================================================
// RowDefinition
//==============================================================================
class UIGridLayout::RowDefinition
	: public DefinitionBase
{
public:

	RowDefinition()
	{
	}

	virtual ~RowDefinition()
	{
	}

	void setHeight(float value, GridLengthType type = GridLengthType::Pixel) { m_data.size = value; m_data.type = type; }
	float getHeight() const { return m_data.size; }

	void setMinHeight(float value) { m_data.minSize = value; }
	float GetMinHeight() const { return m_data.minSize; }

	void setMaxHeight(float value) { m_data.maxSize = value; }
	float GetMaxHeight() const { return m_data.maxSize; }
};

//==============================================================================
// ColumnDefinition
//==============================================================================
class UIGridLayout::ColumnDefinition
	: public DefinitionBase
{
public:

	ColumnDefinition()
	{
	}

	virtual ~ColumnDefinition()
	{
	}

	void setWidth(float value, GridLengthType type = GridLengthType::Pixel) { m_data.size = value; m_data.type = type; }
	float getWidth() const { return m_data.size; }

	void setMinWidth(float value) { m_data.minSize = value; }
	float GetMinWidth() const { return m_data.minSize; }

	void setMaxWidth(float value) { m_data.maxSize = value; }
	float GetMaxWidth() const { return m_data.maxSize; }
};

//==============================================================================
// UIGridLayout
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIGridLayout, UILayoutPanel)

//------------------------------------------------------------------------------
UIGridLayoutPtr UIGridLayout::create()
{
	auto ptr = UIGridLayoutPtr::makeRef();
	ptr->initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UIGridLayoutPtr UIGridLayout::create(int rowCount, int columnCount)
{
	auto ptr = UIGridLayoutPtr::makeRef();
	ptr->initialize();
	ptr->setGridSize(rowCount, columnCount);
	return ptr;
}

//------------------------------------------------------------------------------
UIGridLayout::UIGridLayout()
	: m_rowDefinitions()
	, m_columnDefinitions()
{
}

//------------------------------------------------------------------------------
UIGridLayout::~UIGridLayout()
{
}

//------------------------------------------------------------------------------
void UIGridLayout::initialize()
{
	UILayoutPanel::initialize();
}

//------------------------------------------------------------------------------
void UIGridLayout::setGridSize(int rowCount, int columnCount)
{
	m_rowDefinitions.clear();
	m_columnDefinitions.clear();

	for (int i = 0; i < rowCount; ++i)
	{
		m_rowDefinitions.add(Ref<RowDefinition>::makeRef());
	}
	for (int i = 0; i < columnCount; ++i)
	{
		m_columnDefinitions.add(Ref<ColumnDefinition>::makeRef());
	}
}

//------------------------------------------------------------------------------
void UIGridLayout::addRowDefinition(GridLengthType type, float height, float minHeight, float maxHeight)
{
	auto ptr = Ref<RowDefinition>::makeRef();
	ptr->setHeight(height, type);
	ptr->setMinHeight(minHeight);
	ptr->setMaxHeight(maxHeight);
	m_rowDefinitions.add(ptr);
}


//------------------------------------------------------------------------------
void UIGridLayout::addColumnDefinition(GridLengthType type, float width, float minWidth, float maxWidth)
{
	auto ptr = Ref<ColumnDefinition>::makeRef();
	ptr->setWidth(width, type);
	ptr->setMinWidth(minWidth);
	ptr->setMaxWidth(maxWidth);
	m_columnDefinitions.add(ptr);
}

void UIGridLayout::addChild(UIElement* child)
{
	UILayoutPanel::addChild(child);
}

//------------------------------------------------------------------------------
void UIGridLayout::addChild(UIElement* child, int row, int column)
{
	child->setLayoutColumn(column);
	child->setLayoutRow(row);
	addChild(child);
}
//------------------------------------------------------------------------------
Size UIGridLayout::measureOverride(const Size& constraint)
{
	return detail::LayoutImpl<UIGridLayout>::UIGridLayout_measureOverride(
		this, constraint,
		[](UIGridLayout* panel, const Size& constraint){ return panel->UILayoutPanel::measureOverride(constraint); });
}

//------------------------------------------------------------------------------
Size UIGridLayout::arrangeOverride(const Size& finalSize)
{
	return detail::LayoutImpl<UIGridLayout>::UIGridLayout_arrangeOverride(this, finalSize);
}

//------------------------------------------------------------------------------
int UIGridLayout::getLayoutGridRowDefinitionCount() { return m_rowDefinitions.getCount(); }
detail::GridDefinitionData* UIGridLayout::getLayoutGridRowDefinition(int index) { return &m_rowDefinitions[index]->m_data; }
int UIGridLayout::getLayoutGridColumnDefinitionCount() { return m_columnDefinitions.getCount(); }
detail::GridDefinitionData* UIGridLayout::getLayoutGridColumnDefinition(int index) { return &m_columnDefinitions[index]->m_data; }


//==============================================================================
// UIFlowLayout
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIFlowLayout, UIGridLayout)

Ref<UIFlowLayout> UIFlowLayout::create()
{
	return newObject<UIFlowLayout>();
}

UIFlowLayout::UIFlowLayout()
	: m_nextRow(0)
	, m_nextColumn(0)
{
}

UIFlowLayout::~UIFlowLayout()
{
}

void UIFlowLayout::initialize()
{
	UIGridLayout::initialize();
}

void UIFlowLayout::add(UIElement* child)
{
	addChild(child, m_nextRow, m_nextColumn);
	m_nextRow++;
}

void UIFlowLayout::newLine()
{
	m_nextRow = 0;
	m_nextColumn++;
}

LN_NAMESPACE_END
