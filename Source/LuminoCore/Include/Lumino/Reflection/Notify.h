
#pragma once
#include "Common.h"
//#include "../Base/List.h"
//#include "../Base/String.h"
#include "../Base/Delegate.h"
#include "../Base/StlHelper.h"

LN_NAMESPACE_BEGIN
namespace tr
{

/**
	@brief		
*/
class ReflectionEventBase
{
protected:
	ReflectionEventBase() = default;
	virtual ~ReflectionEventBase() = default;

private:
	friend class Object;
	virtual void raise(ReflectionEventArgs* e) const = 0;
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
	void addHandler(const Delegate<void(TArgs*)>& handler)
	{
		m_handlerList.push_back(handler);
	}
	
	/**
		@brief	指定したハンドラに一致するハンドラを、このスロットから削除します。
	*/
	void removeHandler(const Delegate<void(TArgs*)>& handler)
	{
		ln::detail::StlHelper::remove(m_handlerList, handler);
	}
	
	/**
		@brief	ルーティングイベントのハンドラを追加します。
	*/
	//void operator += (const std::function<void(TArgs*)>& handler)
	//{
	//	addHandler(handler);
	//}

	void operator += (const Delegate<void(TArgs*)>& handler)
	{
		m_handlerList.push_back(handler);
	}
	
	/**
		@brief	指定したハンドラに一致するハンドラを、このスロットから削除します。
	*/
	void operator -= (const Delegate<void(TArgs*)>& handler)
	{
		ln::detail::StlHelper::remove(m_handlerList, handler);
	}

private:
	std::vector< Delegate<void(TArgs*)> > m_handlerList;

	virtual void raise(ReflectionEventArgs* e) const override
	{
		for (const Delegate<void(TArgs*)>& d : m_handlerList)
		{
			d.call(static_cast<TArgs*>(e));
		}
	}
};





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

	void addHandler(const DelegateType& handler)
	{
		ln::detail::StlHelper::remove(m_handlerList, handler);
	}

	void addHandler(const std::function<void(TArgs...)>& handler)	// void operator += (const DelegateType& handler) だけだと暗黙変換が効かずコンパイルエラーとなっていたため用意
	{
		addHandler(DelegateType(handler));
	}

	void removeHandler(const DelegateType& handler)
	{
		ln::detail::StlHelper::remove(m_handlerList, handler);
	}

	void operator += (const DelegateType& handler)
	{
		addHandler(handler);
	}

	//void operator += (const std::function<void(TArgs...)>& handler)	// void operator += (const DelegateType& handler) だけだと暗黙変換が効かずコンパイルエラーとなっていたため用意
	//{
	//	addHandler(DelegateType(handler));
	//}

	void operator -= (const DelegateType& handler)
	{
		removeHandler(handler);
	}

private:
	friend class Object;

	std::vector<DelegateType> m_handlerList;

	void clear()
	{
		m_handlerList.clear();
	}

	bool isEmpty() const
	{
		return m_handlerList.empty();
	}

	void raise(TArgs... args)
	{
		int count = m_handlerList.size();
		if (count > 0)
		{
			for (int i = 0; i < count - 1; ++i)
			{
				m_handlerList[i].call(args...);
			}
			m_handlerList[count - 1].call(args...);	// 戻り値を戻すのは最後の1つ。(.NET の動作)
		}
	}
};





} // namespace tr
LN_NAMESPACE_END
