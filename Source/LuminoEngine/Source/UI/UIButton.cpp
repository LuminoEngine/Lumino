
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIButton.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIButton
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIButton, UIContentControl);

// Event definition
LN_ROUTED_EVENT_IMPLEMENT(UIButton, UIMouseEventArgs, ClickEvent, "Click", click);

//------------------------------------------------------------------------------
UIButtonPtr UIButton::Create()
{
	auto ptr = UIButtonPtr::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UIButton::UIButton()
{

}

//------------------------------------------------------------------------------
UIButton::~UIButton()
{

}

//------------------------------------------------------------------------------
void UIButton::Initialize(detail::UIManager* manager)
{
	UIContentControl::Initialize(manager);
}

//------------------------------------------------------------------------------
void UIButton::SetText(const StringRef& text)
{
	auto textBlock = UITextBlockPtr::MakeRef();
	textBlock->Initialize(GetManager());
	textBlock->SetText(text);
	SetContent(textBlock);
}

//------------------------------------------------------------------------------
void UIButton::OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e)
{
	if (ev == UIElement::MouseUpEvent)
	{
		OnClick(static_cast<UIMouseEventArgs*>(e));
	}
	else if (ev == UIElement::MouseLeaveEvent)
	{
	}

	UIContentControl::OnRoutedEvent(ev, e);
}

//------------------------------------------------------------------------------
void UIButton::OnClick(UIMouseEventArgs* e)
{
	if (!e->handled)
	{
		RaiseEvent(ClickEvent, this, e);
	}
}

LN_NAMESPACE_END
