
#pragma once
#include <functional>
#include "Variant.h"

namespace Lumino
{

/**
	@brief		
*/
class EventArgs	// TOOD: 名前 RoutedEventArgs
	: public CoreObject
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	EventArgs();
	virtual ~EventArgs();

public:
	CoreObject*	Sender;
	//UIElement* HandlerOwner;
	bool Handled;

protected:

};

/**
	@brief		
*/
class RoutedEvent
{
public:
	RoutedEvent() : m_registerd(false) {}
	virtual ~RoutedEvent() {}

public:
	virtual TypeInfo* GetOwnerClassTypeInfo() const = 0;

	virtual const String& GetName() const = 0;

	//virtual void Raise(CoreObject* target, CoreObject* sender, EventArgs* e) = 0;
	virtual void CallEvent(CoreObject* target, EventArgs* e) const = 0;

private:
	friend class TypeInfo;
	bool	m_registerd;
};

/**
	@brief		
*/
template<class TOwnerClass, typename TEventArgs>
class StaticTypedRoutedEvent : public RoutedEvent
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
	StaticTypedRoutedEvent(const String& name/*, OnEvent onEvent*/, std::function< void(TOwnerClass*, /*CoreObject*, */TEventArgs*) > callEventHandler/*CallEventHandler callEventHandler*/)
		: m_name(name)
		//, m_callEventHandler(callEventHandler)
		, m_callEventHandler(callEventHandler)
		//, m_onEvent(onEvent)
	{
		TOwnerClass::GetClassTypeInfo()->RegisterRoutedEvent(this);
	}


	virtual ~StaticTypedRoutedEvent() {}

	virtual TypeInfo* GetOwnerClassTypeInfo() const { return TOwnerClass::GetClassTypeInfo(); }

	virtual const String& GetName() const { return m_name; }

	virtual void CallEvent(CoreObject* target, EventArgs* e) const
	{
		TOwnerClass* instance = static_cast<TOwnerClass*>(target);
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
	std::function< void(TOwnerClass*, /*CoreObject*, */TEventArgs*) >	m_callEventHandler;
	//OnEvent	m_onEvent;
};


#define LN_DEFINE_ROUTED_EVENT(ownerClass, eventArgs, var, name, ev) \
	static StaticTypedRoutedEvent<ownerClass, eventArgs> _##var(_T(name), [](ownerClass* t, eventArgs* e) { t->ev(e); }); \
	const RoutedEvent* ownerClass::var = &_##var;


//{ \
//	static ::Lumino::GUI::TypedRoutedEvent<classType, eventArgsType> ev(name, callEventFuncPtr); \
//	RegisterRoutedEvent(&ev); \
//}
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
	RoutedEventHandler() : m_registerd(false) {}
	virtual ~RoutedEventHandler() {}

public:
	virtual void Call(CoreObject* target, EventArgs* e) = 0;

private:
	friend class TypeInfo;
	bool	m_registerd;
};


template<class TOwnerClass, typename TEventArgs>
class TypedRoutedEventHandler : public RoutedEventHandler
{
public:
	typedef std::function< void(TOwnerClass*, TEventArgs*) > Handler;

	TypedRoutedEventHandler(const RoutedEvent* ev, Handler handler)
		: m_handler(handler)
	{
		TOwnerClass::GetClassTypeInfo()->RegisterRoutedEventHandler(ev, this);
	}

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
/// LN_DEFINE_ROUTED_EVENT との違いは、イベントを受け取るものであること。
#define LN_REGISTER_ROUTED_EVENT_HANDLER(ownerClass, eventArgs, routedEvent, handler) \
{ \
	static ::Lumino::TypedRoutedEventHandler<ownerClass, eventArgs> h(routedEvent, [](ownerClass* t, eventArgs* e) { t->handler(e); }); \
}

} // namespace Lumino
