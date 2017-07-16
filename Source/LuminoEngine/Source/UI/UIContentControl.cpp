
#include "Internal.h"
#include <Lumino/UI/UIContext.h>		// TODO: いらないかも
#include <Lumino/UI/UILayoutView.h>		// TODO: いらないかも
#include <Lumino/UI/UILayoutPanel.h>
#include <Lumino/UI/UIContentControl.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIContentControl
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIContentControl, UIElement);

//------------------------------------------------------------------------------
UIContentControl::UIContentControl()
	: m_contentElement(nullptr)
	, m_contentElementModified(false)
{
}

//------------------------------------------------------------------------------
UIContentControl::~UIContentControl()
{
	LN_SAFE_RELEASE(m_contentElement);
}

//------------------------------------------------------------------------------
void UIContentControl::initialize()
{
	UIControl::initialize();

	setVContentAlignment(VAlignment::Stretch);
	setHContentAlignment(HAlignment::Stretch);
}

//------------------------------------------------------------------------------
void UIContentControl::setContent(UIElement* element)
{
	// 既に content を持っていれば取り除いておく
	if (m_contentElement != nullptr && m_contentElement != element)
	{
		removeVisualChild(m_contentElement);
		LN_SAFE_RELEASE(m_contentElement);
	}
	m_contentElement = nullptr;

	// 新しい m_contentElement を保持する
	//    SetVisualTreeRoot(element);
	//LN_NOTIMPLEMENTED();
	if (element != nullptr)
	{
		addVisualChild(element);
		LN_REFOBJ_SET(m_contentElement, element);
		m_contentElement = element;
	}
	//if (m_contentElement != nullptr)
	//{
	//	m_contentElementModified = true;
	//}
}

//------------------------------------------------------------------------------
//void UIContentControl::onUpdatingLayout()
//{
//	// VisualTree が無ければ単なる Panel を作って m_contentHost として使う。
//	// この時点で VisualTree が無いということは、Style を検索できなかったか、
//	// Style の持つ Template に Host となれる Panel が含まれていなかったということ。
//	if (GetVisualTreeRoot() == nullptr)
//	{
//		auto panel = Ref<UIPanel>::MakeRef();
//		panel->initialize(getManager());
//
//		// ContentAlignment 設定
//		panel->SetVerticalAlignment(GetVerticalContentAlignment());
//		panel->SetHorizontalAlignment(GetHorizontalContentAlignment());
//
//		m_contentHost = panel.DetachMove();
//		SetVisualTreeRoot(m_contentHost);
//	}
//
//	// m_contentHost に m_contentElement を追加する
//	if (m_contentElementModified)
//	{
//		m_contentHost->addChild(m_contentElement);
//
//		// 要素ツリーに入ったのでスタイルを更新する
//		//m_contentHost->applyTemplateHierarchy(GetOwnerLayoutView()->getOwnerContext()->getRootStyleTable(), GetLocalStyle());
//		m_contentElementModified = false;
//	}
//
//	UIElement::onUpdatingLayout();
//}

LN_NAMESPACE_END
