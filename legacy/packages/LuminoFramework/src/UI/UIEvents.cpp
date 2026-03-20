#include "Internal.hpp"
#include <LuminoFramework/Input/InputBinding.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoFramework/UI/UIEvents.hpp>
#include <LuminoFramework/UI/UIElement.hpp>
#include "UIManager.hpp"
#include "UIEventArgsPool.hpp"

namespace ln {

//==============================================================================
// UIEvents
    
const UIEventType	UIEvents::Default = 0;
const UIEventType	UIEvents::GotFocusEvent = 1;
const UIEventType	UIEvents::LostFocusEvent = 2;
const UIEventType	UIEvents::MouseMoveEvent = 3;
const UIEventType	UIEvents::MouseDownEvent = 4;
const UIEventType	UIEvents::MouseUpEvent = 5;
const UIEventType	UIEvents::MouseWheelEvent = 6;
const UIEventType	UIEvents::MouseEnterEvent = 7;
const UIEventType	UIEvents::MouseLeaveEvent = 8;
const UIEventType	UIEvents::KeyDownEvent = 9;
const UIEventType	UIEvents::KeyUpEvent = 10;
const UIEventType	UIEvents::TextInputEvent = 11;
const UIEventType	UIEvents::CheckedEvent = 12;
const UIEventType	UIEvents::UncheckedEvent = 13;
const UIEventType	UIEvents::ScrollDragStartedEvent = 14;
const UIEventType	UIEvents::ScrollDragDeltaEvent = 15;
const UIEventType	UIEvents::ScrollDragCompletedEvent = 16;
const UIEventType	UIEvents::ScrollDragCanceledEvent = 17;
const UIEventType	UIEvents::ScrollEvent = 18;
const UIEventType	UIEvents::DragEnterEvent = 19;
const UIEventType	UIEvents::DragDropEvent = 20;

const UIEventType	UIEvents::Selected = 21;
const UIEventType	UIEvents::Unselected = 22;

const UIEventType	UIEvents::Click = 23;
const UIEventType	UIEvents::Submitted = 24;

const UIEventType   UIEvents::SelectionChanged = 25;

const UIEventType   UIEvents::Timer = 26;

const UIEventType	UIEvents::Activated = 27;
const UIEventType	UIEvents::Deactivated = 28;

const UIEventType	UIEvents::ExecuteCommandEvent = 201;
const UIEventType	UIEvents::CanExecuteCommandEvent = 202;
const UIEventType	UIEvents::CanExecuteChangedEvent = 203;

const UIEventType   UIEvents::NotifyPropertyChanged = 301;

const UIEventType	UIEvents::RequestVisualUpdateEvent = 101;
const UIEventType	UIEvents::RequestVisualRedrawEvent = 102;

const UIEventType	UIEvents::RequestNavigate = 103;

//const UIEventType	UIEvents::ComboBoxItemClicked = 1001;


//==============================================================================
// UIEventArgs

LN_OBJECT_IMPLEMENT(UIEventArgs, Object) {}

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

Result<> UIEventArgs::init() {
	return Object::init();
}

bool UIEventArgs::init(UIElement* sender, UIEventType type)
{
	if (!init()) return false;
    m_sender = sender;
    m_type = type;
	return true;
}

bool UIEventArgs::testInputGesture(UIEventArgs* e, InputGesture* gesture)
{
    if (e->type() == UIEvents::KeyDownEvent) {
        auto* ke = static_cast<UIKeyEventArgs*>(e);
        if (gesture->getType() == detail::InputBindingType::Keyboard) {
            auto* g = static_cast<KeyGesture*>(gesture);
            if (ke->getKey() == g->getKey() && ke->getModifierKeys() == g->getModifierKeys()) {
                return true;
            }
        }
    }
    else if (e->type() == UIEvents::MouseDownEvent) {
        if (gesture->getType() == detail::InputBindingType::Mouse) {
            auto* me = static_cast<UIMouseEventArgs*>(e);
            auto* g = static_cast<MouseGesture*>(gesture);
            if (me->modifierKeys() == g->getModifierKeys()) {
                switch (g->getMouseAction())
                {
                case MouseAction::LeftClick:
                    if (me->getMouseButtons() == MouseButtons::Left && me->getClickCount() == 1) {
                        return true;
                    }
                    break;
                case MouseAction::LeftDoubleClick:
                    if (me->getMouseButtons() == MouseButtons::Left && me->getClickCount() == 2) {
                        return true;
                    }
                    break;
                case MouseAction::RightClick:
                    if (me->getMouseButtons() == MouseButtons::Right && me->getClickCount() == 1) {
                        return true;
                    }
                    break;
                case MouseAction::RightDoubleClick:
                    if (me->getMouseButtons() == MouseButtons::Right && me->getClickCount() == 2) {
                        return true;
                    }
                    break;
                case MouseAction::MiddleClick:
                    if (me->getMouseButtons() == MouseButtons::Middle && me->getClickCount() == 1) {
                        return true;
                    }
                    break;
                case MouseAction::MiddleDoubleClick:
                    if (me->getMouseButtons() == MouseButtons::Middle && me->getClickCount() == 2) {
                        return true;
                    }
                    break;
                case MouseAction::X1Click:
                    if (me->getMouseButtons() == MouseButtons::X1 && me->getClickCount() == 1) {
                        return true;
                    }
                    break;
                case MouseAction::X1DoubleClick:
                    if (me->getMouseButtons() == MouseButtons::X1 && me->getClickCount() == 2) {
                        return true;
                    }
                    break;
                case MouseAction::X2Click:
                    if (me->getMouseButtons() == MouseButtons::X2 && me->getClickCount() == 1) {
                        return true;
                    }
                    break;
                case MouseAction::X2DoubleClick:
                    if (me->getMouseButtons() == MouseButtons::X2 && me->getClickCount() == 2) {
                        return true;
                    }
                    break;
                default:
                    LN_UNREACHABLE();
                    break;
                }
            }
        }
    }
    else if (e->type() == UIEvents::MouseWheelEvent) {
        if (gesture->getType() == detail::InputBindingType::Mouse) {
            auto* me = static_cast<UIMouseWheelEventArgs*>(e);
            auto* g = static_cast<MouseGesture*>(gesture);
            switch (g->getMouseAction())
            {
            case MouseAction::WheelPlus:
                if (me->getDelta() > 0) {
                    return true;
                }
                break;
            case MouseAction::WheelMinus:
                if (me->getDelta() < 0) {
                    return true;
                }
                break;
            default:
                LN_UNREACHABLE();
                break;
            }

        }
    }

    return false;
}

//==============================================================================
// UIMouseEventArgs

LN_OBJECT_IMPLEMENT(UIMouseEventArgs, UIEventArgs) {}

Ref<UIMouseEventArgs> UIMouseEventArgs::create(UIElement* sender, UIEventType type, MouseButtons button, float x, float y, int clickCount, ModifierKeys modifierKeys, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIMouseEventArgs> ptr(pool->create<UIMouseEventArgs>(sender, type, button, x, y, clickCount, modifierKeys), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UIMouseEventArgs::UIMouseEventArgs()
    : m_button(MouseButtons::None)
    , m_position(0, 0)
    , m_clickCount(0)
    , m_modifierKeys(ModifierKeys::None)
{
}

UIMouseEventArgs::~UIMouseEventArgs()
{
}

Result<> UIMouseEventArgs::init() {
	return UIEventArgs::init();
}

bool UIMouseEventArgs::init(UIElement* sender, UIEventType type, MouseButtons button, float x, float y, int clickCount, ModifierKeys modifierKeys)
{
	if (!UIEventArgs::init(sender, type)) return false;
    m_button = button;
    m_position.x = x;
    m_position.y = y;
    m_clickCount = clickCount;
    m_modifierKeys = modifierKeys;
	return true;
}

Point UIMouseEventArgs::getPosition(UIElement* relativeTo) const
{
    auto inv = Matrix::makeInverse(relativeTo->m_combinedFinalRenderTransform);
    auto pos = Vector3::transformCoord(Vector3(m_position.x, m_position.y, .0f), inv);
    return Point(pos.x, pos.y);
    //const Rect& rc = relativeTo->finalGlobalRect();
    //return Point(m_position.x - rc.x, m_position.y - rc.y);
}

//Point UIMouseEventArgs::getPosition(RenderView* relativeTo) const
//{
//	auto pt = relativeTo->actualScreenOffset();
//	return Point(m_position.x - pt.x, m_position.y - pt.y);
//}

//==============================================================================
// UIKeyEventArgs

LN_OBJECT_IMPLEMENT(UIKeyEventArgs, UIEventArgs) {}

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

Result<> UIKeyEventArgs::init() {
	return UIEventArgs::init();
}

//------------------------------------------------------------------------------
bool UIKeyEventArgs::init(UIElement* sender, UIEventType type, Keys keyCode, ModifierKeys modifierKeys, Char charCode)
{
	if (!UIEventArgs::init(sender, type)) return false;
    m_keyCode = keyCode;
    m_modifierKeys = modifierKeys;
    m_charCode = charCode;
	return true;
}

//==============================================================================
// UIMouseWheelEventArgs

LN_OBJECT_IMPLEMENT(UIMouseWheelEventArgs, UIEventArgs) {}

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

Result<> UIMouseWheelEventArgs::init() {
	return UIEventArgs::init();
}

//------------------------------------------------------------------------------
bool UIMouseWheelEventArgs::init(UIElement* sender, UIEventType type, int delta)
{
	if (!UIEventArgs::init(sender, type)) return false;
    m_delta = delta;
	return true;
}

//==============================================================================
// UIDragDeltaEventArgs

LN_OBJECT_IMPLEMENT(UIDragDeltaEventArgs, UIEventArgs) {}

Ref<UIDragDeltaEventArgs> UIDragDeltaEventArgs::create(UIElement* sender, UIEventType type, float offsetX, float offsetY, bool caching)
{
	if (caching)
	{
		auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
		Ref<UIDragDeltaEventArgs> ptr(pool->create<UIDragDeltaEventArgs>(sender, type, offsetX, offsetY), false);
		return ptr;
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

UIDragDeltaEventArgs::UIDragDeltaEventArgs()
{
}

UIDragDeltaEventArgs::~UIDragDeltaEventArgs()
{
}

Result<> UIDragDeltaEventArgs::init() {
	return UIEventArgs::init();
}

bool UIDragDeltaEventArgs::init(UIElement* sender, UIEventType type, float offsetX, float offsetY)
{
	if (!UIEventArgs::init(sender, type)) return false;
	m_offsetX = offsetX;
	m_offsetY = offsetY;
	return true;
}


//==============================================================================
// UIScrollEventArgs

LN_OBJECT_IMPLEMENT(UIScrollEventArgs, UIEventArgs) {}

Ref<UIScrollEventArgs> UIScrollEventArgs::create(UIElement* sender, UIEventType type, float newValue, ScrollEventType scrollType, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIScrollEventArgs> ptr(pool->create<UIScrollEventArgs>(sender, type, newValue, scrollType), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UIScrollEventArgs::UIScrollEventArgs()
{
}

UIScrollEventArgs::~UIScrollEventArgs()
{
}

Result<> UIScrollEventArgs::init() {
	return UIEventArgs::init();
}

bool UIScrollEventArgs::init(UIElement* sender, UIEventType type, float newValue, ScrollEventType scrollType)
{
	if (!UIEventArgs::init(sender, type)) return false;
    m_newValue = newValue;
    m_scrollType = scrollType;
	return true;
}

//==============================================================================
// UIDragDropEventArgs

LN_OBJECT_IMPLEMENT(UIDragDropEventArgs, UIEventArgs) {}

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

Result<> UIDragDropEventArgs::init() {
	return UIEventArgs::init();
}

bool UIDragDropEventArgs::init(UIElement* sender, UIEventType type, DataObject* data, DragDropEffects effect)
{
	if (!UIEventArgs::init(sender, type)) return false;
    if (LN_REQUIRE(data)) return false;
    m_data = data;
    m_effect = effect;
	return true;
}

//==============================================================================
// UIClickEventArgs

LN_OBJECT_IMPLEMENT(UIClickEventArgs, UIEventArgs) {}

Ref<UIClickEventArgs> UIClickEventArgs::create(UIElement* sender, UIEventType type, int clickCount, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIClickEventArgs> ptr(pool->create<UIClickEventArgs>(sender, type, clickCount), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UIClickEventArgs::UIClickEventArgs()
    : m_clickCount(0)
{
}

UIClickEventArgs::~UIClickEventArgs()
{
}

Result<> UIClickEventArgs::init() {
	return UIEventArgs::init();
}

bool UIClickEventArgs::init(UIElement* sender, UIEventType type, int clickCount)
{
	if (!UIEventArgs::init(sender, type)) return false;
    m_clickCount = clickCount;
	return true;
}


//==============================================================================
// UISelectionChangedEventArgs

LN_OBJECT_IMPLEMENT(UISelectionChangedEventArgs, UIEventArgs) {}

Ref<UISelectionChangedEventArgs> UISelectionChangedEventArgs::create(UIElement* sender, UIEventType type, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UISelectionChangedEventArgs> ptr(pool->create<UISelectionChangedEventArgs>(sender, type), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UISelectionChangedEventArgs::UISelectionChangedEventArgs()
{
}

Result<> UISelectionChangedEventArgs::init() {
	return UIEventArgs::init();
}

bool UISelectionChangedEventArgs::init(UIElement* sender, UIEventType type)
{
	return UIEventArgs::init(sender, type);
}

//==============================================================================
// UICommandEventArgs

LN_OBJECT_IMPLEMENT(UICommandEventArgs, UIEventArgs) {}

Ref<UICommandEventArgs> UICommandEventArgs::create(UIElement* sender, UIEventType type, UICommand* command, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UICommandEventArgs> ptr(pool->create<UICommandEventArgs>(sender, type, command), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UICommandEventArgs::UICommandEventArgs()
    : m_command(nullptr)
    , m_canExecute(true)
{
}

Result<> UICommandEventArgs::init() {
	return UIEventArgs::init();
}

bool UICommandEventArgs::init(UIElement* sender, UIEventType type, UICommand* command)
{
	if (!UIEventArgs::init(sender, type)) return false;
    m_command = command;
    m_canExecute = true;
	return true;
}

void UICommandEventArgs::raiseExecute(UIElement* target, UICommand* command)
{
    if (LN_REQUIRE(target)) return;
    if (LN_REQUIRE(command)) return;
    auto args = UICommandEventArgs::create(target, UIEvents::ExecuteCommandEvent, command);
    target->raiseEvent(args);
}

//==============================================================================
// UINotifyPropertyChangedEventArgs

LN_OBJECT_IMPLEMENT(UINotifyPropertyChangedEventArgs, UIEventArgs) {}

Ref<UINotifyPropertyChangedEventArgs> UINotifyPropertyChangedEventArgs::create(UIElement* sender, UIEventType type, const StringView& propertyName, bool caching)
{
    if (caching) {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UINotifyPropertyChangedEventArgs> ptr(pool->create<UINotifyPropertyChangedEventArgs>(sender, type, propertyName), false);
        return ptr;
    }
    else {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

Ref<UINotifyPropertyChangedEventArgs> UINotifyPropertyChangedEventArgs::create(UIElement* sender, UIEventType type, UICollectionChangedAction action, int startIndex, int count, bool caching)
{
    if (caching) {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UINotifyPropertyChangedEventArgs> ptr(pool->create<UINotifyPropertyChangedEventArgs>(sender, type, action, startIndex, count), false);
        return ptr;
    }
    else {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UINotifyPropertyChangedEventArgs::UINotifyPropertyChangedEventArgs()
    : m_action(UICollectionChangedAction::Reset)
{
}

Result<> UINotifyPropertyChangedEventArgs::init() {
	return UIEventArgs::init();
}

bool UINotifyPropertyChangedEventArgs::init(UIElement* sender, UIEventType type, const StringView& propertyName)
{
	if (!UIEventArgs::init(sender, type)) return false;
    m_action = UICollectionChangedAction::Reset;
    m_name = propertyName;
    m_startIndex = 0;
    m_count = 0;
	return true;
}

bool UINotifyPropertyChangedEventArgs::init(UIElement* sender, UIEventType type, UICollectionChangedAction action, int startIndex, int count)
{
	if (!UIEventArgs::init(sender, type)) return false;
    m_action = action;
    m_name = String::Empty;
    m_startIndex = startIndex;
    m_count = count;
	return true;
}

//==============================================================================
// UITimerEventArgs

LN_OBJECT_IMPLEMENT(UITimerEventArgs, UIEventArgs) {}

Ref<UITimerEventArgs> UITimerEventArgs::create(UIElement* sender, UIEventType type, float elapsedSeconds, bool caching)
{
	if (caching)
	{
		auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
		Ref<UITimerEventArgs> ptr(pool->create<UITimerEventArgs>(sender, type, elapsedSeconds), false);
		return ptr;
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

UITimerEventArgs::UITimerEventArgs()
	: m_elapsedSeconds(0.0f)
{
}

Result<> UITimerEventArgs::init() {
	return UIEventArgs::init();
}

bool UITimerEventArgs::init(UIElement* sender, UIEventType type, float elapsedSeconds)
{
	if (!UIEventArgs::init(sender, type)) return false;
	m_elapsedSeconds = elapsedSeconds;
	return true;
}

//==============================================================================
// UIRequestNavigateEventArgs

LN_OBJECT_IMPLEMENT(UIRequestNavigateEventArgs, UIEventArgs) {}

Ref<UIRequestNavigateEventArgs> UIRequestNavigateEventArgs::create(UIElement* sender, UIEventType type, const String& url, bool caching)
{
    if (caching)
    {
        auto& pool = detail::EngineDomain::uiManager()->eventArgsPool();
        Ref<UIRequestNavigateEventArgs> ptr(pool->create<UIRequestNavigateEventArgs>(sender, type, url), false);
        return ptr;
    }
    else
    {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }
}

UIRequestNavigateEventArgs::UIRequestNavigateEventArgs()
    : m_url()
{
}

Result<> UIRequestNavigateEventArgs::init() {
    return UIEventArgs::init();
}

bool UIRequestNavigateEventArgs::init(UIElement* sender, UIEventType type, const String& url)
{
    if (!UIEventArgs::init(sender, type)) return false;
    m_url = url;
    return true;
}

} // namespace ln

