
#if 0
#pragma once
#include <functional>
#include "Variant.h"

LN_NAMESPACE_BEGIN

class EventArgs
	: public CoreObject
{
public:
	EventArgs() {}
	virtual ~EventArgs() {}
};

/**
	@brief		
*/
class RoutedEventArgs	// TOOD: 名前 RoutedEventArgs
	: public CoreObject
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	RoutedEventArgs();
	virtual ~RoutedEventArgs();

public:
	CoreObject*	Sender;
	//UIElement* HandlerOwner;
	bool Handled;

protected:

};



class PropertyChangedEventArgs
	: public EventArgs//CoreObject
	//: public RoutedEventArgs
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	PropertyChangedEventArgs(const Property* prop, const Variant& newValue, const Variant& oldValue)
		: ChangedProperty(prop)
		, NewValue(newValue)
		, OldValue(oldValue)
	{}
	//virtual ~PropertyChangedEventArgs();

public:
	const Property*	ChangedProperty;
	//String	PropertyName;	// TODO: 削除する
	Variant	NewValue;
	Variant	OldValue;
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
	//virtual TypeInfo* GetOwnerClassTypeInfo() const = 0;

	virtual const String& GetName() const = 0;

	//virtual void Raise(CoreObject* target, CoreObject* sender, RoutedEventArgs* e) = 0;
	virtual void CallEvent(CoreObject* target, RoutedEventArgs* e) const = 0;

private:
	friend class TypeInfo;
	bool	m_registerd;
};

/**
	@brief		
*/
//template<class TOwnerClass/*, typename TEventArgs*/>

class StaticTypedRoutedEvent : public RoutedEvent
{
public:
	//typedef void (TClass::*CallEventHandler)(CoreObject* sender, TEventArgs*);
	//typedef void (TClass::*OnEvent)(TEventArgs*);

	// Event を直接参照してはならない。このクラスは Property と同じく、複数の UIElement で共有される。状態を持ってはならない。
	// なので、イベントを Raise する関数ポインタを参照する。

	typedef void(*RaiseEventFunc)(CoreObject* obj, RoutedEventArgs* e);

public:
	//TypedRoutedEvent(const String& name, RaiseEvent raiseEvent)
	//	: m_name(name)
	//	, m_raiseEvent(raiseEvent)
	//{}
	//StaticTypedRoutedEvent(const String& name/*, OnEvent onEvent*/, std::function< void(TOwnerClass*, /*CoreObject*, */TEventArgs*) > callEventHandler/*CallEventHandler callEventHandler*/)
	StaticTypedRoutedEvent(const String& name, RaiseEventFunc raiseEvent)
		: m_name(name)
		, m_raiseEvent(raiseEvent)
		//, m_callEventHandler(callEventHandler)
		//, m_callEventHandler(callEventHandler)
		//, m_onEvent(onEvent)
	{
		//TOwnerClass::GetClassTypeInfo()->RegisterRoutedEvent(this);
	}


	virtual ~StaticTypedRoutedEvent() {}

	//virtual TypeInfo* GetOwnerClassTypeInfo() const { return TOwnerClass::GetClassTypeInfo(); }

	virtual const String& GetName() const { return m_name; }	// TODO: virtual にする必要ないかも

	virtual void CallEvent(CoreObject* target, RoutedEventArgs* e) const
	{
		m_raiseEvent(target, e);
		//TOwnerClass* instance = static_cast<TOwnerClass*>(target);
		//TEventArgs* et = static_cast<TEventArgs*>(e);
		////(instance->*m_raiseEvent)(sender, et);
		////(instance->*m_callEventHandler)(sender, et);
		//m_callEventHandler(instance, et);
	}

	//virtual void Raise(CoreObject* target, CoreObject* sender, RoutedEventArgs* e)
	//{
	//	TClass* instance = static_cast<TClass*>(target);
	//	TEventArgs* et = static_cast<TEventArgs*>(e);
	//	//(instance->*m_raiseEvent)(sender, et);
	//	(instance->*m_onEvent)(et);
	//}

	/// グローバル空間に定義された static 変数を初期化するのが目的
	static RoutedEvent* RegisterRoutedEvent(TypeInfo* ownerClass, RoutedEvent* ev)
	{
		ownerClass->RegisterRoutedEvent(ev);
		return ev;
	}

private:
	String		m_name;
	RaiseEventFunc	m_raiseEvent;
	//CallEventHandler	m_callEventHandler;
	//std::function< void(TOwnerClass*, /*CoreObject*, */TEventArgs*) >	m_callEventHandler;
	//OnEvent	m_onEvent;
};

//template<typename TValue>
//class TypedRoutedEventInitializer
//{
//public:
//	typedef void(*RaiseEventFunc)(CoreObject* obj, RoutedEventArgs* e);
//
//	TypedPropertyInitializer(StaticTypedRoutedEvent* ev, RaiseEventFunc raiseEvent)
//	{
//		prop->m_setter = setter;
//		prop->m_getter = getter;
//		prop->m_propChanged = propChanged;
//	}
//
//
//};
//
//#define LN_ROUTED_EVENT(eventArgs, eventVar) \
//	public:  static const RoutedEvent* eventVar; \
//	private: static void  _raise_##eventVar(CoreObject* obj, RoutedEventArgs* e); \
//	private: static StaticTypedRoutedEvent _init_##eventVar;
//
//#define LN_ROUTED_EVENT_IMPLEMENT(ownerClass, eventArgs, eventVar, name, slot) \
//	StaticTypedRoutedEvent	ownerClass::_init_##eventVar(_T(name), &ownerClass::_raise_##eventVar); \
//	const RoutedEvent*		ownerClass::eventVar = StaticTypedRoutedEvent::RegisterRoutedEvent(ownerClass::GetClassTypeInfo(), &_init_##eventVar); \
//	void					ownerClass::_raise_##eventVar(CoreObject* obj, RoutedEventArgs* e) { static_cast<ownerClass*>(obj)->EmitEventSlot(static_cast<ownerClass*>(obj)->slot, static_cast<eventArgs*>(e)); }
	


//{ \
//	static ::ln::GUI::TypedRoutedEvent<classType, eventArgsType> ev(name, callEventFuncPtr); \
//	RegisterRoutedEvent(&ev); \
//}
//
//
//#define LN_DEFINE_ROUTED_EVENT(classType, eventArgsType, name, onEventFuncPtr, callEventFuncPtr) \
//{ \
//	static ::ln::GUI::TypedRoutedEvent<classType, eventArgsType> ev(name, onEventFuncPtr, callEventFuncPtr); \
//	RegisterRoutedEvent(&ev); \
//}




class RoutedEventHandler
{
protected:
	RoutedEventHandler() : m_registerd(false) {}
	virtual ~RoutedEventHandler() {}

public:
	virtual void Call(CoreObject* target, RoutedEventArgs* e) = 0;

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

	virtual void Call(CoreObject* target, RoutedEventArgs* e)
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
/// On～ をオーバーライドすることと似ているが、On～ は this がイベントの送信元でなければ呼ばれない。
/// つまり、子Visualで発生したルーティングイベントを元に処理したい場合はこのマクロで登録する必要がある。
#define LN_REGISTER_ROUTED_EVENT_HANDLER(ownerClass, eventArgs, routedEvent, handler) \
{ \
	static ::ln::TypedRoutedEventHandler<ownerClass, eventArgs> h(routedEvent, [](ownerClass* t, eventArgs* e) { t->handler(e); }); \
}

LN_NAMESPACE_END

#endif
