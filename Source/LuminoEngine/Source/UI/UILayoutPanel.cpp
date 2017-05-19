
#include "Internal.h"
#include <Lumino/UI/UILayoutPanel.h>
#include "UIManager.h"
#include "LayoutImpl.h"

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
void UILayoutPanel::Initialize()
{
	UIElement::Initialize();
	SetHitTestVisible(false);
	m_children = RefPtr<UIElementCollection>::MakeRef(this);

	// Panel 系のデフォルトは Stretch
	//SetHAlignment(HAlignment::Stretch);
	//SetVAlignment(VAlignment::Stretch);
}

//------------------------------------------------------------------------------
void UILayoutPanel::AddChild(UIElement* element)
{
	m_children->Add(element);
	element->SetLogicalParent(this);
}

//------------------------------------------------------------------------------
void UILayoutPanel::RemoveChild(UIElement* element)
{
	m_children->Remove(element);
	element->SetLogicalParent(nullptr);
}

//------------------------------------------------------------------------------
int UILayoutPanel::GetVisualChildrenCount() const
{
	return m_children->GetCount();
}

//------------------------------------------------------------------------------
UIElement* UILayoutPanel::GetVisualChild(int index) const
{
	return m_children->GetAt(index);
}

//------------------------------------------------------------------------------
Size UILayoutPanel::MeasureOverride(const Size& constraint)
{
	return detail::LayoutImpl<UILayoutPanel>::UILayoutPanel_MeasureOverride(
		this, constraint,
		[](UILayoutPanel* panel, const Size& constraint){ return panel->UIElement::MeasureOverride(constraint); });
}

//------------------------------------------------------------------------------
Size UILayoutPanel::ArrangeOverride(const Size& finalSize)
{
	Vector2 pixelOffset = -m_scrollOffset;

	return detail::LayoutImpl<UILayoutPanel>::UILayoutPanel_ArrangeOverride(this, pixelOffset, finalSize);
}

//------------------------------------------------------------------------------
void UILayoutPanel::OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e)
{
	/*
		ListBox や Button からコンテンツを追加する場合もこの関数が呼ばれる。
		このときは VisualChild として追加する。LogicalChild ではない点に注意。
	*/

	// 新しく追加されたものたち
	for (UIElement* element : e.newItems)
	{
		//element->SetParent(this);
		AddVisualChild(element);
	}

	// 削除されたものたち
	for (UIElement* element : e.oldItems)
	{
		//element->SetParent(nullptr);
		RemoveVisualChild(element);
	}
}

//------------------------------------------------------------------------------
int UILayoutPanel::GetLayoutChildrenCount() { return m_children->GetCount(); }
ILayoutElement* UILayoutPanel::GetLayoutChild(int index) { return m_children->GetAt(index); }
int UILayoutPanel::GetLayoutGridColumnDefinitionCount() { return 0; }
detail::GridDefinitionData* UILayoutPanel::GetLayoutGridColumnDefinition(int index) { return nullptr; }
int UILayoutPanel::GetLayoutGridRowDefinitionCount() { return 0; }
detail::GridDefinitionData* UILayoutPanel::GetLayoutGridRowDefinition(int index) { return nullptr; }

//------------------------------------------------------------------------------
float UILayoutPanel::GetExtentWidth() const { return GetDesiredSize().width; }
float UILayoutPanel::GetExtentHeight() const { return GetDesiredSize().height; }
float UILayoutPanel::GetViewportWidth() const { return GetLayoutFinalLocalRect().width; }
float UILayoutPanel::GetViewportHeight() const { return GetLayoutFinalLocalRect().height; }
void UILayoutPanel::SetHorizontalOffset(float offset) { m_scrollOffset.x = offset; }
float UILayoutPanel::GetHorizontalOffset() const { return m_scrollOffset.x; }
void UILayoutPanel::SetVerticalOffset(float offset) { m_scrollOffset.y = offset; }
float UILayoutPanel::GetVerticalOffset() const { return m_scrollOffset.y; }

//==============================================================================
// UIStackPanel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIStackPanel, UILayoutPanel)

//------------------------------------------------------------------------------
UIStackPanelPtr UIStackPanel::Create()
{
	auto ptr = RefPtr<UIStackPanel>::MakeRef();
	ptr->Initialize();
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
void UIStackPanel::Initialize()
{
	UILayoutPanel::Initialize();
}

//------------------------------------------------------------------------------
Size UIStackPanel::MeasureOverride(const Size& constraint)
{
	return Size::Max(
		detail::LayoutImpl<UIStackPanel>::UIStackPanel_MeasureOverride(this, constraint, m_orientation),
		UIElement::MeasureOverride(constraint));
}

//------------------------------------------------------------------------------
Size UIStackPanel::ArrangeOverride(const Size& finalSize)
{
	return detail::LayoutImpl<UIStackPanel>::UIStackPanel_ArrangeOverride(this, finalSize, m_orientation);
}


//==============================================================================
// UIAbsoluteLayout
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIAbsoluteLayout, UILayoutPanel)

//------------------------------------------------------------------------------
UIAbsoluteLayoutPtr UIAbsoluteLayout::Create()
{
	auto ptr = UIAbsoluteLayoutPtr::MakeRef();
	ptr->Initialize();
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
void UIAbsoluteLayout::Initialize()
{
	UILayoutPanel::Initialize();
}

//------------------------------------------------------------------------------
Size UIAbsoluteLayout::MeasureOverride(const Size& constraint)
{
	//return UILayoutPanel::MeasureOverride(constraint);
	
	// LayoutPanel ではなく、UIElement の MeasureOverride を実施 (this のサイズを測る)
	Size size = UIElement::MeasureOverride(constraint);

	Size childMaxSize(0, 0);
	for (UIElement* child : *GetChildren())
	{
		child->MeasureLayout(constraint);
		const Size& desiredSize = child->GetDesiredSize();
		const PointF& pos = child->GetPositionInternal();

		childMaxSize.width  = std::max(childMaxSize.width,  pos.x + desiredSize.width);
		childMaxSize.height = std::max(childMaxSize.height, pos.y + desiredSize.height);
	}

	return Size::Min(constraint, Size::Max(size, childMaxSize));

	//if (Math::IsNaN(size.width)) size.width = 

	//Size desiredSize = baseCallback(static_cast<TPanel*>(panel), constraint);
	//int childCount = panel->GetLayoutChildrenCount();
	//for (int i = 0; i < childCount; i++)
	//{
	//	ILayoutElement* child = panel->GetLayoutChild(i);
	//	PointF pos = child->GetPositionInternal();

	//	child->MeasureLayout(constraint);
	//	const Size& childDesiredSize = child->GetLayoutDesiredSize();

	//	desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
	//	desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
	//}
	//return desiredSize;


	//Size size = UILayoutPanel::MeasureOverride(constraint);
	//
	//for (UIElement* child : *GetChildren())
	//{

	//}

	//return size;
}

//------------------------------------------------------------------------------
Size UIAbsoluteLayout::ArrangeOverride(const Size& finalSize)
{
	//ThicknessF canvas;
	
	for (UIElement* child : *GetChildren())
	{
		//if (child->GetInvalidateFlags().TestFlag(detail::InvalidateFlags::ParentChangedUpdating))
		//{
		//}
		
		const Size& desiredSize = child->GetDesiredSize();
		//Size layoutSize = child->GetSizeInternal();
		Size size = desiredSize;
		//size.width = Math::IsNaN(layoutSize.width) ? desiredSize.width : layoutSize.width;
		//size.height = Math::IsNaN(layoutSize.height) ? desiredSize.height : layoutSize.height;

		Rect childRect(child->GetPositionInternal(), size/*child->GetSizeInternal()*/);
		AlignmentAnchor anchor = child->GetAnchorInternal();
		
		if (anchor != AlignmentAnchor::None)
		{
			const ThicknessF& margin = GetMargineInternal();
			//float l = childRect.GetLeft(), t = childRect.GetTop(), r = childRect.GetRight(), b = childRect.GetBottom();

			//if (anchor.TestFlag(AlignmentAnchor::LeftOffsets))
			//	l = margin.Left;
			//else if (anchor.TestFlag(AlignmentAnchor::LeftRatios))
			//	l = finalSize.width * margin.Left;

			//if (anchor.TestFlag(AlignmentAnchor::TopOffsets))
			//	t = margin.Top;
			//else if (anchor.TestFlag(AlignmentAnchor::TopRatios))
			//	t = finalSize.height * margin.Top;

			//if (anchor.TestFlag(AlignmentAnchor::RightOffsets))
			//	r = finalSize.width - margin.Right;
			//else if (anchor.TestFlag(AlignmentAnchor::RightRatios))
			//	r = finalSize.width - (finalSize.width * margin.Right);

			//if (anchor.TestFlag(AlignmentAnchor::BottomOffsets))
			//	b = finalSize.height - margin.Bottom;
			//else if (anchor.TestFlag(AlignmentAnchor::BottomRatios))
			//	b = finalSize.height - (finalSize.height * margin.Bottom);

			//if (anchor.TestFlag(AlignmentAnchor::HCenter))
			//	childRect.x = (finalSize.width - childRect.width) / 2;

			//if (anchor.TestFlag(AlignmentAnchor::VCenter))
			//	childRect.y = (finalSize.height - childRect.height) / 2;

			//childRect.Set(l, t, r - l, b - t);

			//child->ArrangeLayout(childRect);
#if 1
			float l = NAN, t = NAN, r = NAN, b = NAN;
			if (anchor.TestFlag(AlignmentAnchor::LeftOffsets))
				l = margin.Left;
			else if (anchor.TestFlag(AlignmentAnchor::LeftRatios))
				l = finalSize.width * margin.Left;
			
			if (anchor.TestFlag(AlignmentAnchor::TopOffsets))
				t = margin.Top;
			else if (anchor.TestFlag(AlignmentAnchor::TopRatios))
				t = finalSize.height * margin.Top;
			
			if (anchor.TestFlag(AlignmentAnchor::RightOffsets))
				r = finalSize.width - margin.Right;
			else if (anchor.TestFlag(AlignmentAnchor::RightRatios))
				r = finalSize.width - (finalSize.width * margin.Right);
			
			if (anchor.TestFlag(AlignmentAnchor::BottomOffsets))
				b = finalSize.height - margin.Bottom;
			else if (anchor.TestFlag(AlignmentAnchor::BottomRatios))
				b = finalSize.height - (finalSize.height * margin.Bottom);

			if (anchor.TestFlag(AlignmentAnchor::HCenter))
				childRect.x = (finalSize.width - childRect.width) / 2;

			if (anchor.TestFlag(AlignmentAnchor::VCenter))
				childRect.y = (finalSize.height - childRect.height) / 2;

			if (!Math::IsNaN(l) || !Math::IsNaN(r))
			{
				if (!Math::IsNaN(l) && Math::IsNaN(r))
				{
					childRect.x = l;
				}
				else if (Math::IsNaN(l) && !Math::IsNaN(r))
				{
					childRect.x = r - childRect.width;
				}
				else
				{
					childRect.x = l;
					childRect.width = r - l;
				}
			}

			if (!Math::IsNaN(t) || !Math::IsNaN(b))
			{
				if (!Math::IsNaN(t) && Math::IsNaN(b))
				{
					childRect.y = t;
				}
				else if (Math::IsNaN(t) && !Math::IsNaN(b))
				{
					childRect.y = b - childRect.height;
				}
				else
				{
					childRect.y = t;
					childRect.height = b - t;
				}
			}

			child->ArrangeLayout(childRect);
#endif
		}
		else
		{
			//if (Math::IsNaN(layoutSize.width))
			//	childRect.width = finalSize.width;
			//if (Math::IsNaN(layoutSize.height))
			//	childRect.height = finalSize.height;
			child->ArrangeLayout(childRect);
		}
	}

	return finalSize;
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

	GridLengthType GetType() const { return m_data.type; }

	//float GetAvailableDesiredSize() const
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

	//void AdjustActualSize()
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

	void SetWidth(float value, GridLengthType type = GridLengthType::Pixel) { m_data.size = value; m_data.type = type; }
	float GetWidth() const { return m_data.size; }

	void SetMinWidth(float value) { m_data.minSize = value; }
	float GetMinWidth() const { return m_data.minSize; }

	void SetMaxWidth(float value) { m_data.maxSize = value; }
	float GetMaxWidth() const { return m_data.maxSize; }
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

	void SetHeight(float value, GridLengthType type = GridLengthType::Pixel) { m_data.size = value; m_data.type = type; }
	float GetHeight() const { return m_data.size; }

	void SetMinHeight(float value) { m_data.minSize = value; }
	float GetMinHeight() const { return m_data.minSize; }

	void SetMaxHeight(float value) { m_data.maxSize = value; }
	float GetMaxHeight() const { return m_data.maxSize; }
};

//==============================================================================
// UIGridLayout
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIGridLayout, UILayoutPanel)

//------------------------------------------------------------------------------
UIGridLayoutPtr UIGridLayout::Create()
{
	auto ptr = UIGridLayoutPtr::MakeRef();
	ptr->Initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UIGridLayoutPtr UIGridLayout::Create(int columnCount, int rowCount)
{
	auto ptr = UIGridLayoutPtr::MakeRef();
	ptr->Initialize();
	ptr->SetGridSize(columnCount, rowCount);
	return ptr;
}

//------------------------------------------------------------------------------
UIGridLayout::UIGridLayout()
	: m_columnDefinitions()
	, m_rowDefinitions()
{
}

//------------------------------------------------------------------------------
UIGridLayout::~UIGridLayout()
{
}

//------------------------------------------------------------------------------
void UIGridLayout::Initialize()
{
	UILayoutPanel::Initialize();
}

//------------------------------------------------------------------------------
void UIGridLayout::SetGridSize(int columnCount, int rowCount)
{
	m_columnDefinitions.Clear();
	m_rowDefinitions.Clear();

	for (int i = 0; i < columnCount; ++i)
		m_columnDefinitions.Add(RefPtr<ColumnDefinition>::MakeRef());
	for (int i = 0; i < rowCount; ++i)
		m_rowDefinitions.Add(RefPtr<RowDefinition>::MakeRef());
}

//------------------------------------------------------------------------------
void UIGridLayout::AddColumnDefinition(GridLengthType type, float width, float minWidth, float maxWidth)
{
	auto ptr = RefPtr<ColumnDefinition>::MakeRef();
	ptr->SetWidth(width, type);
	ptr->SetMinWidth(minWidth);
	ptr->SetMaxWidth(maxWidth);
	m_columnDefinitions.Add(ptr);
}

//------------------------------------------------------------------------------
void UIGridLayout::AddRowDefinition(GridLengthType type, float height, float minHeight, float maxHeight)
{
	auto ptr = RefPtr<RowDefinition>::MakeRef();
	ptr->SetHeight(height, type);
	ptr->SetMinHeight(minHeight);
	ptr->SetMaxHeight(maxHeight);
	m_rowDefinitions.Add(ptr);
}

//------------------------------------------------------------------------------
Size UIGridLayout::MeasureOverride(const Size& constraint)
{
	return detail::LayoutImpl<UIGridLayout>::UIGridLayout_MeasureOverride(
		this, constraint,
		[](UIGridLayout* panel, const Size& constraint){ return panel->UILayoutPanel::MeasureOverride(constraint); });
}

//------------------------------------------------------------------------------
Size UIGridLayout::ArrangeOverride(const Size& finalSize)
{
	return detail::LayoutImpl<UIGridLayout>::UIGridLayout_ArrangeOverride(this, finalSize);
}

//------------------------------------------------------------------------------
int UIGridLayout::GetLayoutGridColumnDefinitionCount() { return m_columnDefinitions.GetCount(); }
detail::GridDefinitionData* UIGridLayout::GetLayoutGridColumnDefinition(int index) { return &m_columnDefinitions[index]->m_data; }
int UIGridLayout::GetLayoutGridRowDefinitionCount() { return m_rowDefinitions.GetCount(); }
detail::GridDefinitionData* UIGridLayout::GetLayoutGridRowDefinition(int index) { return &m_rowDefinitions[index]->m_data; }

LN_NAMESPACE_END
