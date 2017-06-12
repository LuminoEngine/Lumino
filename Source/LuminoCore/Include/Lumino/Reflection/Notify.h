
#pragma once
#include "Common.h"
#include "../Base/List.h"
#include "../Base/String.h"
#include "../Base/Delegate.h"

LN_NAMESPACE_BEGIN
namespace tr
{
	
/**
	@brief		
*/
class ReflectionEventInfo
{
public:
	// Event を直接参照してはならない。このクラスは Property と同じく、複数の UIElement で共有される。状態を持ってはならない。
	// なので、イベントを Raise する関数ポインタを参照する。

	typedef void(*RaiseEventFunc)(ReflectionObject* target, ReflectionEventArgs* e);

public:
	ReflectionEventInfo(TypeInfo* ownerClass, const TCHAR* name, RaiseEventFunc raiseEvent);
	~ReflectionEventInfo();

	const String& GetName() const { return m_name; }
	void CallEvent(ReflectionObject* target, ReflectionEventArgs* e) const { m_raiseEvent(target, e); }

private:
	friend class TypeInfo;
	String			m_name;
	RaiseEventFunc	m_raiseEvent;
	bool			m_registerd;
};


/**
	@brief		
*/
class ReflectionEventBase
{
protected:
	ReflectionEventBase() = default;
	virtual ~ReflectionEventBase() = default;

private:
	friend class ReflectionObject;
	virtual void Raise(ReflectionEventArgs* e) const = 0;
};

/**
	@brief		
	@details	
*/
template<class TArgs>
class ReflectionEvent
	: public ReflectionEventBase
{
public:
	ReflectionEvent() = default;
	virtual ~ReflectionEvent() = default;

public:

	/**
		@brief	ルーティングイベントのハンドラを追加します。
	*/
	void AddHandler(const Delegate<void(TArgs*)>& handler)
	{
		m_handlerList.Add(handler);
	}
	
	/**
		@brief	指定したハンドラに一致するハンドラを、このスロットから削除します。
	*/
	void RemoveHandler(const Delegate<void(TArgs*)>& handler)
	{
		m_handlerList.Remove(handler);
	}
	
	/**
		@brief	ルーティングイベントのハンドラを追加します。
	*/
	//void operator += (const std::function<void(TArgs*)>& handler)
	//{
	//	AddHandler(handler);
	//}

	void operator += (const Delegate<void(TArgs*)>& handler)
	{
		m_handlerList.Add(handler);
	}
	
	/**
		@brief	指定したハンドラに一致するハンドラを、このスロットから削除します。
	*/
	void operator -= (const Delegate<void(TArgs*)>& handler)
	{
		m_handlerList.Remove(handler);
	}

private:
	List< Delegate<void(TArgs*)> > m_handlerList;

	virtual void Raise(ReflectionEventArgs* e) const override
	{
		for (const Delegate<void(TArgs*)>& d : m_handlerList)
		{
			d.Call(static_cast<TArgs*>(e));
		}
	}
};


#define LN_REFLECTION_EVENT_COMMON(typeInfo, eventArgs, eventInfoVar) \
	public:  static const typeInfo* eventInfoVar; \
	private: static void		_raise_##eventInfoVar(tr::ReflectionObject* obj, tr::ReflectionEventArgs* e); \
	private: static typeInfo	_init_##eventInfoVar;

#define LN_REFLECTION_EVENT_IMPLEMENT_COMMON(typeInfo, ownerClass, eventArgs, eventInfoVar, name, ev) \
	typeInfo					ownerClass::_init_##eventInfoVar(tr::TypeInfo::GetTypeInfo<ownerClass>(), _T(name), &ownerClass::_raise_##eventInfoVar); \
	const typeInfo*				ownerClass::eventInfoVar = &_init_##eventInfoVar; \
	void						ownerClass::_raise_##eventInfoVar(tr::ReflectionObject* obj, tr::ReflectionEventArgs* e) { static_cast<ownerClass*>(obj)->RaiseReflectionEvent(static_cast<ownerClass*>(obj)->ev, static_cast<eventArgs*>(e)); }

#define LN_REFLECTION_EVENT(eventArgs, eventInfoVar) \
	LN_REFLECTION_EVENT_COMMON(tr::ReflectionEventInfo, eventArgs, eventInfoVar);

#define LN_REFLECTION_EVENT_IMPLEMENT(ownerClass, eventArgs, eventInfoVar, name, ev) \
	LN_REFLECTION_EVENT_IMPLEMENT_COMMON(tr::ReflectionEventInfo, ownerClass, eventArgs, eventInfoVar, name, ev);






/**
	@brief		ルーティングフレームワークを伴わないイベント。
*/
//template<typename>
//class DelegateEvent {};
template<typename... TArgs>
class DelegateEvent/*<void(TArgs...)>*/		// 戻り値は void 固定。ハンドラが1つも登録されていないときは何を返せばいいのか分からないため。
{
public:
	typedef Delegate<void(TArgs...)> DelegateType;

	void AddHandler(const DelegateType& handler)
	{
		m_handlerList.Add(handler);
	}

	void AddHandler(const std::function<void(TArgs...)>& handler)	// void operator += (const DelegateType& handler) だけだと暗黙変換が効かずコンパイルエラーとなっていたため用意
	{
		AddHandler(DelegateType(handler));
	}

	void RemoveHandler(const DelegateType& handler)
	{
		m_handlerList.Remove(handler);
	}

	void operator += (const DelegateType& handler)
	{
		AddHandler(handler);
	}

	//void operator += (const std::function<void(TArgs...)>& handler)	// void operator += (const DelegateType& handler) だけだと暗黙変換が効かずコンパイルエラーとなっていたため用意
	//{
	//	AddHandler(DelegateType(handler));
	//}

	void operator -= (const DelegateType& handler)
	{
		RemoveHandler(handler);
	}

private:
	friend class ReflectionObject;

	List<DelegateType> m_handlerList;

	void clear()
	{
		m_handlerList.clear();
	}

	bool IsEmpty() const
	{
		return m_handlerList.IsEmpty();
	}

	void Raise(TArgs... args)
	{
		int count = m_handlerList.GetCount();
		if (count > 0)
		{
			for (int i = 0; i < count - 1; ++i)
			{
				m_handlerList[i].Call(args...);
			}
			m_handlerList[count - 1].Call(args...);	// 戻り値を戻すのは最後の1つ。(.NET の動作)
		}
	}
};





} // namespace tr
LN_NAMESPACE_END
