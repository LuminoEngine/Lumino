
#include "../Internal.h"
#include <Lumino/UI/UIEvent.h>
#include <Lumino/UI/UITypeInfo.h>

LN_NAMESPACE_BEGIN

////==============================================================================
//// UIEvent
////==============================================================================
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//UIEvent::UIEvent(UITypeInfo* ownerClass, const String& name, RaiseEventFunc raiseEvent)
//	: m_name(name)
//	, m_raiseEvent(raiseEvent)
//	, m_registerd(false)
//{
//	ownerClass->RegisterRoutedEvent(this);
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//UIEvent::~UIEvent()
//{
//
//}

//==============================================================================
// UIEventManager
//==============================================================================
//static List<UIEventInfo> g_eventInfoList;
static int g_eventInfoList = 0;
// TODO: いまは取り急ぎイベントの種類を識別するものがほしい。あとでちゃんと詳細情報詰めておく

//------------------------------------------------------------------------------
UIEventType UIEventManager::RegisterEvent()
{
	g_eventInfoList++;
	return g_eventInfoList;
}

//==============================================================================
// UIEvents
//==============================================================================
UIEventType	UIEvents::GotFocusEvent = 0;
UIEventType	UIEvents::LostFocusEvent = 0;
UIEventType	UIEvents::MouseMoveEvent = 0;
UIEventType	UIEvents::MouseDownEvent = 0;
UIEventType	UIEvents::MouseUpEvent = 0;
UIEventType	UIEvents::MouseWheelEvent = 0;
UIEventType	UIEvents::MouseEnterEvent = 0;
UIEventType	UIEvents::MouseLeaveEvent = 0;
UIEventType	UIEvents::KeyDownEvent = 0;
UIEventType	UIEvents::KeyUpEvent = 0;
UIEventType	UIEvents::TextInputEvent = 0;
UIEventType	UIEvents::CheckedEvent = 0;
UIEventType	UIEvents::UncheckedEvent = 0;
UIEventType	UIEvents::DragStartedEvent = 0;
UIEventType	UIEvents::DragDeltaEvent = 0;
UIEventType	UIEvents::DragCompletedEvent = 0;
UIEventType	UIEvents::DragCanceledEvent = 0;
UIEventType	UIEvents::ScrollEvent = 0;

LN_NAMESPACE_END
