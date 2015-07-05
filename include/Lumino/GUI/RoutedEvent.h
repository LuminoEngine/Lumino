
#pragma once
#include <functional>

namespace Lumino
{
namespace GUI
{

/**
	@brief		
*/
class RoutedEvent
{
public:
	RoutedEvent() {}
	virtual ~RoutedEvent() {}

public:
	virtual const String& GetName() const = 0;

	//virtual void Raise(CoreObject* target, CoreObject* sender, EventArgs* e) = 0;
	virtual void CallEvent(CoreObject* target/*, CoreObject* sender*/, EventArgs* e) = 0;
};

/**
	@brief		
*/
template<class TClass, typename TEventArgs>
class TypedRoutedEvent : public RoutedEvent
{
public:
	//typedef void (TClass::*CallEventHandler)(CoreObject* sender, TEventArgs*);
	//typedef void (TClass::*OnEvent)(TEventArgs*);

	// Event を直接参照してはならない。このクラスは Property と同じく、複数の UIElement で共有される。状態を持ってはならない。
	// なので、イベントを Raise する関数ポインタを参照する。

public:
	//TypedRoutedEvent(const String& name, RaiseEvent raiseEvent)
	//	: m_name(name)
	//	, m_raiseEvent(raiseEvent)
	//{}
	TypedRoutedEvent(const String& name/*, OnEvent onEvent*/, std::function< void(TClass*, /*CoreObject*, */TEventArgs*) > callEventHandler/*CallEventHandler callEventHandler*/)
		: m_name(name)
		//, m_callEventHandler(callEventHandler)
		, m_callEventHandler(callEventHandler)
		//, m_onEvent(onEvent)
	{}


	virtual ~TypedRoutedEvent() {}

	virtual const String& GetName() const { return m_name; }

	virtual void CallEvent(CoreObject* target, EventArgs* e)
	{
		TClass* instance = static_cast<TClass*>(target);
		TEventArgs* et = static_cast<TEventArgs*>(e);
		//(instance->*m_raiseEvent)(sender, et);
		//(instance->*m_callEventHandler)(sender, et);
		m_callEventHandler(instance, et);
	}

	//virtual void Raise(CoreObject* target, CoreObject* sender, EventArgs* e)
	//{
	//	TClass* instance = static_cast<TClass*>(target);
	//	TEventArgs* et = static_cast<TEventArgs*>(e);
	//	//(instance->*m_raiseEvent)(sender, et);
	//	(instance->*m_onEvent)(et);
	//}

private:
	String		m_name;
	//CallEventHandler	m_callEventHandler;
	std::function< void(TClass*, /*CoreObject*, */TEventArgs*) >	m_callEventHandler;
	//OnEvent	m_onEvent;
};

#define LN_DEFINE_ROUTED_EVENT(classType, eventArgsType, name, callEventFuncPtr) \
{ \
	static ::Lumino::GUI::TypedRoutedEvent<classType, eventArgsType> ev(name, callEventFuncPtr); \
	RegisterRoutedEvent(&ev); \
}
//
//
//#define LN_DEFINE_ROUTED_EVENT(classType, eventArgsType, name, onEventFuncPtr, callEventFuncPtr) \
//{ \
//	static ::Lumino::GUI::TypedRoutedEvent<classType, eventArgsType> ev(name, onEventFuncPtr, callEventFuncPtr); \
//	RegisterRoutedEvent(&ev); \
//}




class RoutedEventHandler
{
protected:
	RoutedEventHandler() {}
	virtual ~RoutedEventHandler() {}

public:
	virtual void Call(CoreObject* target, EventArgs* e) = 0;
};


template<class TOwnerClass, typename TEventArgs>
class TypedRoutedEventHandler : public RoutedEventHandler
{
public:
	typedef std::function< void(TOwnerClass*, TEventArgs*) > Handler;

	TypedRoutedEventHandler(Handler handler)
		: m_handler(handler)
	{}

	virtual void Call(CoreObject* target, EventArgs* e)
	{
		TOwnerClass* instance = static_cast<TOwnerClass*>(target);
		TEventArgs* et = static_cast<TEventArgs*>(e);
		m_handler(instance, et);
	}

private:
	Handler	m_handler;
};

/// https://msdn.microsoft.com/ja-jp/library/ms597875%28v=vs.110%29.aspx
#define LN_REGISTER_ROUTED_EVENT_HANDLER(ownerClassType, eventArgsType, eventId, handler) \
{ \
	static ::Lumino::GUI::TypedRoutedEventHandler<ownerClassType, eventArgsType> h([](ownerClassType* t, eventArgsType* e) { t->handler(e); }); \
	RegisterRoutedEventHandler(eventId, &h); \
}

} // namespace GUI
} // namespace Lumino
