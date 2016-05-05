
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIButton.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// UIButton
//=============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIButton, UIContentControl);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIButtonPtr UIButton::Create()
{
	auto ptr = UIButtonPtr::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
	return ptr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIButton::UIButton()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIButton::~UIButton()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIButton::Initialize(detail::UIManager* manager)
{
	UIContentControl::Initialize(manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIButton::SetText(const StringRef& text)
{
	auto textBlock = UITextBlockPtr::MakeRef();
	textBlock->Initialize(GetManager());
	textBlock->SetText(text);
	SetContent(textBlock);
}

LN_NAMESPACE_END
