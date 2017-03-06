
#include "Internal.h"
#include <Lumino/UI/UIControl.h>
#include "LayoutImpl.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIControl
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIControl, UIElement);

LN_TR_PROPERTY_IMPLEMENT(UIControl, HAlignment, HContentAlignment, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIControl, VAlignment, VContentAlignment, tr::PropertyMetadata());

//------------------------------------------------------------------------------
UIControl::UIControl()
	//: m_visualTreeRoot(nullptr)
{
}

//------------------------------------------------------------------------------
UIControl::~UIControl()
{
	//LN_SAFE_RELEASE(m_visualTreeRoot);
}

//------------------------------------------------------------------------------
void UIControl::Initialize(detail::UIManager* manager)
{
	UIElement::Initialize(manager);
}

////------------------------------------------------------------------------------
//int UIControl::GetVisualChildrenCount() const
//{
//	return (m_visualTreeRoot != nullptr) ? 1 : 0;
//}
//
////------------------------------------------------------------------------------
//ILayoutElement* UIControl::GetVisualChild(int index) const
//{
//	LN_THROW(0 <= index && index < GetVisualChildrenCount(), OutOfRangeException);
//	return m_visualTreeRoot;
//}
//
//------------------------------------------------------------------------------
Size UIControl::MeasureOverride(const Size& constraint)
{
	return detail::LayoutImpl<UIControl>::UILayoutPanel_MeasureOverride(
		this, constraint,
		[](UIControl* panel, const Size& constraint) { return panel->UIElement::MeasureOverride(constraint); });
	//Size desiredSize = UIElement::MeasureOverride(constraint);
	//if (m_visualTreeRoot != nullptr)
	//{
	//    m_visualTreeRoot->MeasureLayout(constraint);
	//    const Size& childDesiredSize = m_visualTreeRoot->GetDesiredSize();

	//    desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
	//    desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
	//}
	//return desiredSize;

}

//------------------------------------------------------------------------------
Size UIControl::ArrangeOverride(const Size& finalSize)
{
	return detail::LayoutImpl<UIControl>::UILayoutPanel_ArrangeOverride(this, finalSize);
	//RectF childFinal(0, 0, finalSize);
	//if (m_visualTreeRoot != nullptr)
	//{
	//    Size childDesiredSize = m_visualTreeRoot->GetDesiredSize();
	//    childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
	//    childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
	//    m_visualTreeRoot->ArrangeLayout(RectF(0, 0, childDesiredSize));
	//}
	//return finalSize;
}


//------------------------------------------------------------------------------
const HAlignment* UIControl::GetPriorityContentHAlignment()
{
	if (HContentAlignment.Get() == VAlignment::Stretch) return nullptr;
	return &HContentAlignment.Get();
}
//------------------------------------------------------------------------------
const VAlignment* UIControl::GetPriorityContentVAlignment()
{
	if (VContentAlignment.Get() == VAlignment::Stretch) return nullptr;
	return &VContentAlignment.Get();
}

//------------------------------------------------------------------------------
//void UIControl::SetVisualTreeRoot(UIElement* element)
//{
//	if (m_visualTreeRoot != nullptr)
//	{
//		m_visualTreeRoot->SetParent(nullptr);
//	}
//
//	LN_REFOBJ_SET(m_visualTreeRoot, element);
//
//	if (m_visualTreeRoot != nullptr)
//	{
//		m_visualTreeRoot->SetParent(this);
//	}
//}

LN_NAMESPACE_END
