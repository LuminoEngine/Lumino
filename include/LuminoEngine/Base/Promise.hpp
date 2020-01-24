
#pragma once
#include "Delegate.hpp"

namespace ln {
	
/**
 * Promise failure.
 */
LN_DELEGATE()
using PromiseFailureDelegate = Delegate<void(void)>;

class PromiseBase
	: public Object
{
public:
	//template<class TInput>
	//Ref<Promise> then(const std::function<void(Promise* p, TInput value)>& action)
	//{
	//	auto p = makeObject<PromiseTask>(action);

	//	return p;
	//}

public:
	virtual void invoke() = 0;
	virtual void callNext() = 0;
	static void enqueueInvoke(const Ref<PromiseBase>& p);
	static void enqueueThen(const Ref<PromiseBase>& p);
};

/**
 * (メソッドチェーンを実装するためのものではない。Binding でそれを作るのは非常に骨が折れるが、多くの場合単一メソッドで足りる)
 */
template<class TResult>
class Promise
	: public PromiseBase
{
public:
	//LN_OBJECT;
	friend class ::ln::TypeInfo;
	friend class ::ln::detail::EngineDomain;
	friend class ::ln::EngineContext;
	static ::ln::TypeInfo* _lnref_getTypeInfo()
	{
		static ::ln::TypeInfo typeInfo("__Promise", ::ln::TypeInfo::getTypeInfo<PromiseBase>());
		return &typeInfo;
	}
	::ln::TypeInfo* _lnref_getThisTypeInfo() const { return _lnref_getTypeInfo(); }

	static int _lnref_registerTypeInfo(::ln::EngineContext* context)
	{
		if (context) {
			context->registerType<Promise<TResult>>({});
		}
		return 10;
	}
	
	static int const test_info_ /*GTEST_ATTRIBUTE_UNUSED_*/;




	static Ref<Promise> run(const std::function<void(Promise<TResult>*)>& action)
	{
		auto p = Ref<Promise>(LN_NEW Promise(action), false);
		PromiseBase::enqueueInvoke(p);
		return p;
	}

	void resolve(TResult value)
	{
		m_result = value;
	}

	void reject()
	{
		m_rejected = true;
	}

	const TResult& result() const { return m_result; }

	void thenWith(Delegate<void(TResult value)>* action)
	{
		m_thenAction = action;
	}

	void thenWith(std::function<void(TResult value)> action)
	{
		m_thenAction = makeObject<Delegate<void(Ref<Object>)>>(action);
	}

	void catchWith(PromiseFailureDelegate* action)
	{
		m_failAction = action;
	}

protected:
	// called task thread.
	virtual void invoke() override
	{
		m_action(this);
	}

	virtual void callNext() override
	{
		if (m_rejected) {
			if (m_failAction) {
				m_failAction->call();
			}
			else {
				LN_ERROR(u"Promise rejected. (unhandling error)");
			}
		}
		else if (m_thenAction) {
			m_thenAction->call(m_result);
		}
	}

LN_CONSTRUCT_ACCESS:
	Promise()
		: m_action()
	{}

	Promise(const std::function<void(Promise<TResult>*)>& action)
		: m_action(action)
	{
		test_info_;
	}

private:

	std::function<void(Promise<TResult>*)> m_action;
	TResult m_result;
	bool m_rejected = false;
	Ref<Delegate<void(TResult value)>> m_thenAction;
	Ref<PromiseFailureDelegate> m_failAction;
};

template<class TResult>
int const Promise<TResult>::test_info_ = Promise<TResult>::_lnref_registerTypeInfo(0);

} // namespace ln
