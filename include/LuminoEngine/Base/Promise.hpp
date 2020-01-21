
#pragma once
#include "Delegate.hpp"

namespace ln {


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
	static Ref<Promise> run(const std::function<void(Promise<TResult>*)>& action)
	{
		auto p = Ref<Promise>(LN_NEW Promise(action));
		PromiseBase::enqueueInvoke(p);
		return p;
	}

	void resolve(TResult value)
	{
		m_result = value;
	}

	void reject()
	{

	}

	const TResult& result() const { return m_result; }

	void then(Delegate<void(TResult value)>* action)
	{
		m_thenAction = action;
	}

	void then(std::function<void(TResult value)> action)
	{
		m_thenAction = makeObject<Delegate<void(Ref<Object>)>>(action);
	}


protected:
	// called task thread.
	virtual void invoke() override
	{
		m_action(this);
	}

	virtual void callNext() override
	{
		m_thenAction->call(m_result);
	}

private:
	Promise(const std::function<void(Promise<TResult>*)>& action)
		: m_action(action)
	{}


	std::function<void(Promise<TResult>*)> m_action;
	TResult m_result;
	Ref<Delegate<void(TResult value)>> m_thenAction;
};

} // namespace ln
