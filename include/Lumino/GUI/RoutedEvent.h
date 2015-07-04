
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
	~RoutedEvent() {}

public:
	virtual const String& GetName() const = 0;

	virtual void Raise(CoreObject* target, CoreObject* sender, EventArgs* e) = 0;
	virtual void CallEvent(CoreObject* target, CoreObject* sender, EventArgs* e) = 0;
};

/**
	@brief		
*/
template<class TClass, typename TEventArgs>
class TypedRoutedEvent : public RoutedEvent
{
public:
	//typedef void (TClass::*CallEventHandler)(CoreObject* sender, TEventArgs*);
	typedef void (TClass::*OnEvent)(TEventArgs*);

	// Event を直接参照してはならない。このクラスは Property と同じく、複数の UIElement で共有される。状態を持ってはならない。
	// なので、イベントを Raise する関数ポインタを参照する。

public:
	//TypedRoutedEvent(const String& name, RaiseEvent raiseEvent)
	//	: m_name(name)
	//	, m_raiseEvent(raiseEvent)
	//{}
	TypedRoutedEvent(const String& name, OnEvent onEvent, std::function< void(TClass*, CoreObject*, TEventArgs*) > callEventHandler/*CallEventHandler callEventHandler*/)
		: m_name(name)
		//, m_callEventHandler(callEventHandler)
		, m_callEventHandler(callEventHandler)
		, m_onEvent(onEvent)
	{}


	virtual ~TypedRoutedEvent() {}

	virtual const String& GetName() const { return m_name; }

	virtual void CallEvent(CoreObject* target, CoreObject* sender, EventArgs* e)
	{
		TClass* instance = static_cast<TClass*>(target);
		TEventArgs* et = static_cast<TEventArgs*>(e);
		//(instance->*m_raiseEvent)(sender, et);
		//(instance->*m_callEventHandler)(sender, et);
		m_callEventHandler(instance, sender, et);
	}

	virtual void Raise(CoreObject* target, CoreObject* sender, EventArgs* e)
	{
		TClass* instance = static_cast<TClass*>(target);
		TEventArgs* et = static_cast<TEventArgs*>(e);
		//(instance->*m_raiseEvent)(sender, et);
		(instance->*m_onEvent)(et);
	}

private:
	String		m_name;
	//CallEventHandler	m_callEventHandler;
	std::function< void(TClass*, CoreObject*, TEventArgs*) >	m_callEventHandler;
	OnEvent	m_onEvent;
};

#define LN_DEFINE_ROUTED_EVENT(classType, eventArgsType, name, onEventFuncPtr, callEventFuncPtr) \
{ \
	static ::Lumino::GUI::TypedRoutedEvent<classType, eventArgsType> ev(name, onEventFuncPtr, callEventFuncPtr); \
	RegisterRoutedEvent(&ev); \
}

} // namespace GUI
} // namespace Lumino
