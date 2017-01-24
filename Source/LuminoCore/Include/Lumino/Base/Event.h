
#pragma once
#include "../Base/List.h"
#include "../Base/Delegate.h"

#ifdef LN_CPP11

LN_NAMESPACE_BEGIN

template<typename>
class Event {};
template<typename TRet, typename... TArgs>
class Event<TRet(TArgs...)>
{
public:
	typedef Delegate<TRet(TArgs...)> DelegateType;

public:
	void AddHandler(const DelegateType& handler)
	{
		m_handlerList.Add(handler);
	}

	void RemoveHandler(const DelegateType& handler)
	{
		m_handlerList.Remove(handler);
	}

	void Clear()
	{
		m_handlerList.Clear();
	}

	void Raise(TArgs... args)
	{
		if (!IsEmpty())
		{
			int count = m_handlerList.GetCount();
			for (int i = 0; i < count - 1; ++i)
			{
				m_handlerList[i].Call(args...);
			}
			return m_handlerList[count - 1].Call(args...);	// 戻り値を戻すのは最後の1つ。(.NET の動作)
		}
	}

	bool IsEmpty() const
	{
		return m_handlerList.IsEmpty();
	}

	void operator += (const DelegateType& handler)
	{
		AddHandler(handler);
	}

	void operator -= (const DelegateType& handler)
	{
		RemoveHandler(handler);
	}

	void operator () (TArgs... args)
	{
		Raise(args...);
	}

private:
	List<DelegateType> m_handlerList;
};

LN_NAMESPACE_END

#else
LN_NAMESPACE_BEGIN

#define LN_EVENT_CLASS_NAME				Event01
#define LN_EVENT_DELEGATE_CLASS_NAME	Delegate01
#define LN_EVENT_TEMPLATE_ARGS			typename A1
#define LN_EVENT_ARGS_DECL				A1 a1
#define LN_EVENT_CALL_ARGS				a1
#define LN_EVENT_TEMPLATE_TYPES			A1
#include "Event.inl"

#define LN_EVENT_CLASS_NAME				Event02
#define LN_EVENT_DELEGATE_CLASS_NAME	Delegate02
#define LN_EVENT_TEMPLATE_ARGS			typename A1, typename A2
#define LN_EVENT_ARGS_DECL				A1 a1, A2 a2
#define LN_EVENT_CALL_ARGS				a1, a2
#define LN_EVENT_TEMPLATE_TYPES			A1, A2
#include "Event.inl"

#define LN_EVENT_CLASS_NAME				Event03
#define LN_EVENT_DELEGATE_CLASS_NAME	Delegate03
#define LN_EVENT_TEMPLATE_ARGS			typename A1, typename A2, typename A3
#define LN_EVENT_ARGS_DECL				A1 a1, A2 a2, A3 a3
#define LN_EVENT_CALL_ARGS				a1, a2, a3
#define LN_EVENT_TEMPLATE_TYPES			A1, A2, A3
#include "Event.inl"

#define LN_EVENT_CLASS_NAME				Event04
#define LN_EVENT_DELEGATE_CLASS_NAME	Delegate04
#define LN_EVENT_TEMPLATE_ARGS			typename A1, typename A2, typename A3, typename A4
#define LN_EVENT_ARGS_DECL				A1 a1, A2 a2, A3 a3, A4 a4
#define LN_EVENT_CALL_ARGS				a1, a2, a3, a4
#define LN_EVENT_TEMPLATE_TYPES			A1, A2, A3, A4
#include "Event.inl"

LN_NAMESPACE_END
#endif