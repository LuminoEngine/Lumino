
#include "Internal.h"
#include <Lumino/UI/UIEventArgs.h>
#include <Lumino/UI/UIElement.h>
#include "EventArgsPool.h"
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIEventArgs, tr::ReflectionEventArgs)

//------------------------------------------------------------------------------
UIEventArgsPtr UIEventArgs::Create(Object* sender, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::GetInstance()->GetEventArgsPool();
		return UIEventArgsPtr(pool->Create<UIEventArgs>(), false);
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

//------------------------------------------------------------------------------
UIEventArgs::UIEventArgs()
	: sender(nullptr)
	//, handled(false)
{
}

//------------------------------------------------------------------------------
UIEventArgs::~UIEventArgs()
{
}

//------------------------------------------------------------------------------
void UIEventArgs::Initialize()
{
}

//==============================================================================
// UIMouseEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIMouseEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
RefPtr<UIMouseEventArgs> UIMouseEventArgs::Create(MouseButtons button, float x, float y, int clickCount, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::GetInstance()->GetEventArgsPool();
		RefPtr<UIMouseEventArgs> ptr(pool->Create<UIMouseEventArgs>(), false);
		ptr->Initialize(button, x, y, clickCount);
		return ptr;
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

//------------------------------------------------------------------------------
UIMouseEventArgs::UIMouseEventArgs()
	: m_button(MouseButtons::None)
	, m_position(0, 0)
	, m_clickCount(0)
{
}

//------------------------------------------------------------------------------
UIMouseEventArgs::~UIMouseEventArgs()
{
}

//------------------------------------------------------------------------------
void UIMouseEventArgs::Initialize(MouseButtons button, float x, float y, int clickCount)
{
	UIEventArgs::Initialize();
	m_button = button;
	m_position.x = x;
	m_position.y = y;
	m_clickCount = clickCount;
}

//------------------------------------------------------------------------------
PointF UIMouseEventArgs::GetPosition(UIElement* relativeTo) const
{
	const RectF& rc = relativeTo->GetFinalGlobalRect();
	return PointF(m_position.x - rc.x, m_position.y - rc.y);
}

//==============================================================================
// UIKeyEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIKeyEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
RefPtr<UIKeyEventArgs> UIKeyEventArgs::Create(Keys keyCode, ModifierKeys modifierKeys, TCHAR charCode, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::GetInstance()->GetEventArgsPool();
		RefPtr<UIKeyEventArgs> ptr(pool->Create<UIKeyEventArgs>(), false);
		ptr->Initialize(keyCode, modifierKeys, charCode);
		return ptr;
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

//------------------------------------------------------------------------------
UIKeyEventArgs::UIKeyEventArgs()
	: m_keyCode(Keys::Unknown)
	, m_modifierKeys(ModifierKeys::None)
	, m_charCode(0)
{
}

//------------------------------------------------------------------------------
UIKeyEventArgs::~UIKeyEventArgs()
{
}

//------------------------------------------------------------------------------
void UIKeyEventArgs::Initialize(Keys keyCode, ModifierKeys modifierKeys, TCHAR charCode)
{
	UIEventArgs::Initialize();
	m_keyCode = keyCode;
	m_modifierKeys = modifierKeys;
	m_charCode = charCode;
}

//==============================================================================
// UIMouseWheelEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIMouseWheelEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
RefPtr<UIMouseWheelEventArgs> UIMouseWheelEventArgs::Create(int delta, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::GetInstance()->GetEventArgsPool();
		RefPtr<UIMouseWheelEventArgs> ptr(pool->Create<UIMouseWheelEventArgs>(), false);
		ptr->Initialize(delta);
		return ptr;
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

//------------------------------------------------------------------------------
UIMouseWheelEventArgs::UIMouseWheelEventArgs()
	: m_delta(0)
{
}

//------------------------------------------------------------------------------
UIMouseWheelEventArgs::~UIMouseWheelEventArgs()
{
}

//------------------------------------------------------------------------------
void UIMouseWheelEventArgs::Initialize(int delta)
{
	UIEventArgs::Initialize();
	m_delta = delta;
}

LN_NAMESPACE_END
