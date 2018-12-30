
#include "Internal.hpp"
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include "UIManager.hpp"
#include "UIEventArgsPool.hpp"

namespace ln {

//==============================================================================
// UIEvents

UIEventType	UIEvents::GotFocusEvent = 1;
UIEventType	UIEvents::LostFocusEvent = 2;
UIEventType	UIEvents::MouseMoveEvent = 3;
UIEventType	UIEvents::MouseDownEvent = 4;
UIEventType	UIEvents::MouseUpEvent = 5;
UIEventType	UIEvents::MouseWheelEvent = 6;
UIEventType	UIEvents::MouseEnterEvent = 7;
UIEventType	UIEvents::MouseLeaveEvent = 8;
UIEventType	UIEvents::KeyDownEvent = 9;
UIEventType	UIEvents::KeyUpEvent = 10;
UIEventType	UIEvents::TextInputEvent = 11;
UIEventType	UIEvents::CheckedEvent = 12;
UIEventType	UIEvents::UncheckedEvent = 13;
UIEventType	UIEvents::ScrollDragStartedEvent = 14;
UIEventType	UIEvents::ScrollDragDeltaEvent = 15;
UIEventType	UIEvents::ScrollDragCompletedEvent = 16;
UIEventType	UIEvents::ScrollDragCanceledEvent = 17;
UIEventType	UIEvents::ScrollEvent = 18;
UIEventType	UIEvents::DragEnterEvent = 19;
UIEventType	UIEvents::DragDropEvent = 20;

//==============================================================================
// UIEventArgs

LN_OBJECT_IMPLEMENT(UIEventArgs, Object);

Ref<UIEventArgs> UIEventArgs::create(UIElement* sender, UIEventType type, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        return Ref<UIEventArgs>(pool->create<UIEventArgs>(sender, type), false);
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UIEventArgs::UIEventArgs()
    : m_sender(nullptr)
    , m_type(0)
{
}

UIEventArgs::~UIEventArgs()
{
}

void UIEventArgs::initialize(UIElement* sender, UIEventType type)
{
    Object::initialize();
    m_sender = sender;
    m_type = type;
}

//==============================================================================
// UIMouseEventArgs

LN_OBJECT_IMPLEMENT(UIMouseEventArgs, UIEventArgs);

//------------------------------------------------------------------------------
Ref<UIMouseEventArgs> UIMouseEventArgs::create(UIElement* sender, UIEventType type, MouseButtons button, float x, float y, int clickCount, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIMouseEventArgs> ptr(pool->create<UIMouseEventArgs>(sender, type, button, x, y, clickCount), false);
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
void UIMouseEventArgs::initialize(UIElement* sender, UIEventType type, MouseButtons button, float x, float y, int clickCount)
{
    UIEventArgs::initialize(sender, type);
    m_button = button;
    m_position.x = x;
    m_position.y = y;
    m_clickCount = clickCount;
}

//------------------------------------------------------------------------------
Point UIMouseEventArgs::getPosition(UIElement* relativeTo) const
{
    const Rect& rc = relativeTo->finalGlobalRect();
    return Point(m_position.x - rc.x, m_position.y - rc.y);
}

//==============================================================================
// UIKeyEventArgs

LN_OBJECT_IMPLEMENT(UIKeyEventArgs, UIEventArgs);

//------------------------------------------------------------------------------
Ref<UIKeyEventArgs> UIKeyEventArgs::create(UIElement* sender, UIEventType type, Keys keyCode, ModifierKeys modifierKeys, Char charCode, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIKeyEventArgs> ptr(pool->create<UIKeyEventArgs>(sender, type, keyCode, modifierKeys, charCode), false);
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
void UIKeyEventArgs::initialize(UIElement* sender, UIEventType type, Keys keyCode, ModifierKeys modifierKeys, Char charCode)
{
    UIEventArgs::initialize(sender, type);
    m_keyCode = keyCode;
    m_modifierKeys = modifierKeys;
    m_charCode = charCode;
}

//==============================================================================
// UIMouseWheelEventArgs

LN_OBJECT_IMPLEMENT(UIMouseWheelEventArgs, UIEventArgs);

//------------------------------------------------------------------------------
Ref<UIMouseWheelEventArgs> UIMouseWheelEventArgs::create(UIElement* sender, UIEventType type, int delta, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIMouseWheelEventArgs> ptr(pool->create<UIMouseWheelEventArgs>(sender, type, delta), false);
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
void UIMouseWheelEventArgs::initialize(UIElement* sender, UIEventType type, int delta)
{
    UIEventArgs::initialize(sender, type);
    m_delta = delta;
}

//==============================================================================
// UIDragDropEventArgs

LN_OBJECT_IMPLEMENT(UIDragDropEventArgs, UIEventArgs);

Ref<UIDragDropEventArgs> UIDragDropEventArgs::create(UIElement* sender, UIEventType type, DataObject* data, DragDropEffects effect, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIDragDropEventArgs> ptr(pool->create<UIDragDropEventArgs>(sender, type, data, effect), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UIDragDropEventArgs::UIDragDropEventArgs()
    : m_data(nullptr)
    , m_effect(DragDropEffects::None)
{
}

UIDragDropEventArgs::~UIDragDropEventArgs()
{
}

void UIDragDropEventArgs::initialize(UIElement* sender, UIEventType type, DataObject* data, DragDropEffects effect)
{
    UIEventArgs::initialize(sender, type);
    if (LN_REQUIRE(data)) return;
    m_data = data;
    m_effect = effect;
}

} // namespace ln

