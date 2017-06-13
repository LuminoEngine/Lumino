/**
	@file	Delegate.h
*/
#pragma once
#include "Common.h"

#ifdef LN_CPP11

#include <functional>

LN_NAMESPACE_BEGIN

template<typename>
class Event;
template<typename TRet, typename... TArgs>
class Event<TRet(TArgs...)>;

template<typename>
class Delegate {};
template<typename TRet, typename... TArgs>
class Delegate<TRet(TArgs...)>
{
private:
	enum class HolderType
	{
		Static = 0,
		Member,
		FuncObj,
	};

	class HolderBase
	{
	public:
		virtual ~HolderBase() = default;
		virtual TRet call(TArgs... args) const = 0;
		virtual bool equals(const HolderBase* p) const = 0;
		virtual HolderBase* copy() const = 0;
	};

	class StaticHolder
		: public HolderBase
	{
	private:
		typedef TRet(*StaticFunction)(TArgs...);
		StaticFunction	m_func;

	public:
		StaticHolder(StaticFunction func)
			: m_func(func)
		{}
		virtual TRet call(TArgs... args) const
		{
			return m_func(args...);
		}
		virtual bool equals(const HolderBase* p) const
		{
			return (m_func == static_cast<const StaticHolder*>(p)->m_func);
		}
		virtual HolderBase* copy() const
		{
			return LN_NEW StaticHolder(m_func);
		}
	};

	template<class T>
	class MemberHolder
		: public HolderBase
	{
	private:
		typedef void (T::*MemberFunction)(TArgs...);
		T* m_obj;
		MemberFunction	m_func;

	public:
		MemberHolder(T* obj, MemberFunction func)
			: m_obj(obj)
			, m_func(func)
		{}
		virtual TRet call(TArgs... args) const
		{
			return (m_obj->*m_func)(args...);
		}
		virtual bool equals(const HolderBase* p) const
		{
			return	m_obj == static_cast<const MemberHolder*>(p)->m_obj &&
				m_func == static_cast<const MemberHolder*>(p)->m_func;
		}
		virtual HolderBase* copy() const
		{
			return LN_NEW MemberHolder(m_obj, m_func);
		}
	};

	class FuncObjHolder
		: public HolderBase
	{
	private:
		typedef std::function<TRet(TArgs...)> FuncObj;
		FuncObj	m_func;

	public:
		FuncObjHolder(const FuncObj& func)
			: m_func(func)
		{}
		virtual TRet call(TArgs... args) const
		{
			return m_func(args...);
		}
		virtual bool equals(const HolderBase* p) const
		{
			return false;	// std::function を比較することは出来ない
		}
		virtual HolderBase* copy() const
		{
			return LN_NEW FuncObjHolder(m_func);
		}
	};

public:

	using EventType = Event<TRet(TArgs...)>;

	/** デフォルトコンストラクタ */
	Delegate()
		: m_holder(nullptr)
		, m_type(HolderType::Static)
	{}

	/** static 関数用のコンストラクタ */
	//explicit Delegate(TRet(*func)(TArgs...))
	//	: m_holder(LN_NEW StaticHolder(func))
	//	, m_type(HolderType::Static)
	//{}


	/** メンバ関数用のコンストラクタ */
	template < typename T >
	Delegate(T* objPtr, TRet(T::*func)(TArgs...))
		: m_holder(LN_NEW MemberHolder<T>(objPtr, func))
		, m_type(HolderType::Member)
	{}

	/** static 関数用・ラムダ式用のコンストラクタ */
	//Delegate(const std::function<TRet(TArgs...)>& func)
	//	: m_holder(LN_NEW FuncObjHolder(func))
	//	, m_type(HolderType::FuncObj)
	//{}

	/** static 関数用・ラムダ式用のコンストラクタ */
	template<typename TFunc>
	Delegate(const TFunc& func)
		: m_holder(LN_NEW FuncObjHolder(func))
		, m_type(HolderType::FuncObj)
	{}

	/** コピーコンストラクタ */
	Delegate(const Delegate& d)
		: Delegate()
	{
		if (d.m_holder != nullptr) {
			m_holder = d.m_holder->copy();
			m_type = d.m_type;
		}
	}

	/** ムーブコンストラクタ */
	Delegate(Delegate&& d)
		: Delegate()
	{
		if (d.m_holder != nullptr) {
			m_holder = d.m_holder;
			d.m_holder = nullptr;
			m_type = d.m_type;
		}
	}

	/** デストラクタ */
	~Delegate()
	{
		delete m_holder;
	}

public:

	/** 空の Delegate であるかを確認します。*/
	bool isEmpty() const
	{
		return m_holder == nullptr;
	}

	/** 関数を呼び出します。operator() と同じ動作です。*/
	TRet call(TArgs... args) const
	{
		return m_holder->call(args...);
	}

	/** 関数を呼び出します。*/
	void operator ()(TArgs... args) const
	{
		m_holder->call(args...);
	}

	/** コピー */
	Delegate& operator = (const Delegate& d)
	{
		detach();
		if (d.m_holder != nullptr) {
			m_holder = d.m_holder->copy();
			m_type = d.m_type;
		}
		return *this;
	}

	/** static 関数を割り当てます。*/
	//Delegate& operator = (TRet(*func)(TArgs...))
	//{
	//	Detach();
	//	m_holder = LN_NEW StaticHolder(func);
	//	m_type = HolderType::Static;
	//	return *this;
	//}

	/** ラムダ式・関数オブジェクトを割り当てます。*/
	//Delegate& operator = (const std::function<TRet(TArgs...)>& func)
	//{
	//	Detach();
	//	m_holder = LN_NEW FuncObjHolder(func);
	//	m_type = HolderType::FuncObj;
	//	return *this;
	//}

	/** 比較 */
	bool operator==(std::nullptr_t left) const
	{
		return m_holder == left;
	}

	/** 比較 */
	bool operator==(const Delegate& left) const
	{
		return equals(left);
	}

	/** 比較 */
	bool operator!=(std::nullptr_t left) const
	{
		return m_holder != left;
	}

	/** 比較 */
	bool operator!=(const Delegate& left) const
	{
		return !equals(left);
	}

private:
	void detach()
	{
		delete m_holder;
		m_holder = nullptr;
	}
	bool equals(const Delegate& left) const
	{
		if (m_type != left.m_type) { return false; }
		if (m_holder == NULL &&  left.m_holder == NULL) { return true; }
		if (m_holder != NULL && left.m_holder != NULL) {
			return m_holder->equals(left.m_holder);
		}
		else {
			return false;	// this か obj 一方が NULL で、もう一方が 非NULL であればここに来る。
		}
	}

	HolderBase*	m_holder;
	HolderType	m_type;
};

/**	
	@brief		Delegate を作成します。(static 関数用)
	@details	Delegate のコンストラクタと同じですが、冗長な記述を避けるために使用します。
*/
template<class TRet, class... TArgs>
Delegate<TRet(TArgs...)> createDelegate(TRet(*func)(TArgs...))
{
	return Delegate<TRet(TArgs...)>(func);
}

/**	
	@brief		Delegate を作成します。(メンバ関数用)
	@details	Delegate のコンストラクタと同じですが、冗長な記述を避けるために使用します。
*/
template<class T, class TRet, class... TArgs>
Delegate<TRet(TArgs...)> createDelegate(T* objPtr, TRet(T::*func)(TArgs...))
{
	return Delegate<TRet(TArgs...)>(objPtr, func);
}

//template<class TRet, class... TArgs>
//Delegate<TRet(TArgs...)> CreateDelegate(const std::function<TRet(TArgs...)>& func)
//{
//	return Delegate<TRet(TArgs...)>(func);
//}

LN_NAMESPACE_END

#else


#include "RefObject.h"

LN_NAMESPACE_BEGIN

/**
	@brief		関数ポインタのラッパークラス
	@details
				デリゲートはクラスインスタンスへのポインタとメンバ関数のポインタを保持し、
				関数ポインタ経由の呼び出しを簡略化します。
				メンバ関数だけでなく、static 関数もデリゲートでラップすることができます。
				設定できる関数は、戻り値が void で引数が 0～8 個の関数です。
				例)
				・Delegate00						… void Func();
				・Delegate01<int>					… void Func(int p1);
				・Delegate02<const char*, Test*>	… void Func(const char* p1, Test* p2);
	@code
				class Test
				{
					void run(Delegate01<int> func) {
						func.Call(10);
					}
					void Callback(int value) {
						printf("%d", value);
					}
					void Main() {
						run(LN_CreateDelegate(this, &Test::Callback));
					}
				}
	@endcode

	@par		Delegate の比較について

				Delegate は比較をサポートします。
				比較はインスタンスのポインタと関数ポインタをもとに行い、双方が一致している場合、同一の Delegate であるとみなします。
	@code
				Delegate01<int>	d1 = LN_CreateDelegate(&class1, &Class1::Callback1);
				Delegate01<int>	d2 = LN_CreateDelegate(&class1, &Class1::Callback1);
				Delegate01<int>	d3 = LN_CreateDelegate(&class2, &Class1::Callback1);
				Delegate01<int>	d4 = LN_CreateDelegate(&class1, &Class1::Callback2);
				d1 == d2;	// true
				d1 == d3;	// false
				d1 == d4;	// false
	@endcode
*/
class LUMINO_EXPORT Delegate00
{
private:

	/// 関数ポインタを保持する内部クラスの基底
	class DelegateHolderBase
		: public RefObject
	{
	public:
		virtual void call() const = 0;
	};

	/// DelegateHolderStatic (static 関数呼び出し時の実体)
	class DelegateHolderStatic
		: public DelegateHolderBase
	{
	private:
		typedef void (LN_STDCALL *StaticFunction)();

	private:
		StaticFunction	mFunction;

	public:
		DelegateHolderStatic( StaticFunction function )
			: mFunction	( function )
        {}

		virtual void call() const { mFunction(); }
	};

	/// DelegateHolderDynamics (メンバ関数呼び出し時の実体)
	template < typename T >
	class DelegateHolderDynamics
		: public DelegateHolderBase
	{
	private:
		typedef void (T::*ClassMethod)();

	private:
		T*			mClass;
		ClassMethod	mMethod;

	public:
		DelegateHolderDynamics( T* objPtr, ClassMethod method )
			: mClass		( objPtr )
			, mMethod		( method )
		{}
		virtual void call() const { (mClass->*mMethod)(); }
	};

private:
	DelegateHolderBase* mDelegate;

public:

	Delegate00()
		: mDelegate ( NULL )
	{}

	Delegate00( const Delegate00& d )
		: mDelegate ( NULL )
	{
		*this = d;
	}

    Delegate00( void (LN_STDCALL* method)() )
		: mDelegate ( LN_NEW DelegateHolderStatic( method ) )
	{}

	template < typename T >
	Delegate00( T* objPtr, void (T::*method)() )
		: mDelegate ( LN_NEW DelegateHolderDynamics<T>( objPtr, method ) )
	{}

	~Delegate00() { ReleaseMethod(); }

public:

	void ReleaseMethod() { LN_SAFE_RELEASE( mDelegate ); }

	bool IsEmpty() const { return mDelegate == NULL; }

	void Call() const { if ( mDelegate != NULL ) mDelegate->call(); }

	void operator ()() const { Call(); }

	Delegate00& operator = (const Delegate00& d)
	{
        LN_SAFE_ADDREF( d.mDelegate );
		ReleaseMethod();
		mDelegate = d.mDelegate;
		return *this;
	}
};

/// CreateDelegate
template < typename T >
Delegate00 LN_CreateDelegate( T* objPtr, void (T::*method)() )
{
	return Delegate00( objPtr, method );
}

//==============================================================================
// Delegate xx
//==============================================================================
#define LN_DELEGATE_CLASS_NAME		Delegate01
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1
#define LN_DELEGATE_ARGS_DECL 		A1 a1
#define LN_DELEGATE_CALL_ARGS 		a1
#define LN_DELEGATE_TEMPLATE_TYPES  A1
#include "Delegate.inl"

#define LN_DELEGATE_CLASS_NAME		Delegate02
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2
#define LN_DELEGATE_CALL_ARGS 		a1, a2
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2
#include "Delegate.inl"

#define LN_DELEGATE_CLASS_NAME		Delegate03
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2, typename A3
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2, A3 a3
#define LN_DELEGATE_CALL_ARGS 		a1, a2, a3
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2, A3
#include "Delegate.inl"

#define LN_DELEGATE_CLASS_NAME		Delegate04
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2, typename A3, typename A4
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2, A3 a3, A4 a4
#define LN_DELEGATE_CALL_ARGS 		a1, a2, a3, a4
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2, A3, A4
#include "Delegate.inl"

#define LN_DELEGATE_CLASS_NAME		Delegate05
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2, typename A3, typename A4, typename A5
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2, A3 a3, A4 a4, A5 a5
#define LN_DELEGATE_CALL_ARGS 		a1, a2, a3, a4, a5
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2, A3, A4, A5
#include "Delegate.inl"

#define LN_DELEGATE_CLASS_NAME		Delegate06
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2, typename A3, typename A4, typename A5, typename A6
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6
#define LN_DELEGATE_CALL_ARGS 		a1, a2, a3, a4, a5, a6
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2, A3, A4, A5, A6
#include "Delegate.inl"

#define LN_DELEGATE_CLASS_NAME		Delegate07
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7
#define LN_DELEGATE_CALL_ARGS 		a1, a2, a3, a4, a5, a6, a7
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2, A3, A4, A5, A6, A7
#include "Delegate.inl"

#define LN_DELEGATE_CLASS_NAME		Delegate08
#define LN_DELEGATE_TEMPLATE_ARGS 	typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8
#define LN_DELEGATE_ARGS_DECL 		A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8
#define LN_DELEGATE_CALL_ARGS 		a1, a2, a3, a4, a5, a6, a7, a8
#define LN_DELEGATE_TEMPLATE_TYPES  A1, A2, A3, A4, A5, A6, A7, A8
#include "Delegate.inl"

LN_NAMESPACE_END

#endif // #ifdef LN_CPP11
