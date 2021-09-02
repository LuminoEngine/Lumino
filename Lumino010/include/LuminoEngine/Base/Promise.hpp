
#pragma once
#include "Delegate.hpp"
#include "Task.hpp"

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
	static void enqueueContinueWith(const Ref<PromiseBase>& p, Task* task);
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
		//static ::ln::TypeInfo* _lnref_typeInfo = _lnref_registerTypeInfo();
		return _lnref_typeInfo;
	}
	::ln::TypeInfo* _lnref_getThisTypeInfo() const { return _lnref_getTypeInfo(); }

	static ::ln::TypeInfo* _lnref_registerTypeInfo()
	{
		auto* context = ::ln::EngineContext::current();
		return context->registerType<Promise<TResult>>("__Promise", ::ln::TypeInfo::getTypeInfo<PromiseBase>() , {});
	}
	
	static ::ln::TypeInfo* const _lnref_typeInfo LN_ATTRIBUTE_UNUSED_;




	static Ref<Promise> run(const std::function<void(Promise<TResult>*)>& action)
	{
		auto p = Ref<Promise>(LN_NEW Promise(action), false);
		PromiseBase::enqueueInvoke(p);
		return p;
	}

	// TODO: internal
	static Ref<Promise> continueWith(Ref<GenericTask<TResult>> task)
	{
		auto p = Ref<Promise>(LN_NEW Promise([task](Promise<TResult>* p) {
			p->resolve(task->result());
		}), false);
		PromiseBase::enqueueContinueWith(p, task);
		return p;
	}

	void resolve(TResult value)
	{
		std::lock_guard<std::mutex> locl(m_mutex);
		std::cout << "resolve 1 " << m_rejected << std::endl;
		m_result = value;
		std::cout << "resolve 2 " << m_rejected << std::endl;
	}

	void reject()
	{
		std::lock_guard<std::mutex> locl(m_mutex);
		m_rejected = true;
		std::cout << "reject 2" << std::endl;
	}

	const TResult& result() const { return m_result; }

	void thenWith(Delegate<void(TResult value)>* action)
	{
		std::cout << "Promise(" << this << ") thenWith 1 " << m_rejected << std::endl;
		std::lock_guard<std::mutex> locl(m_mutex);
		std::cout << "xxxx" << std::endl;
		std::cout << RefObjectHelper::getReferenceCount(this) << std::endl;
		m_thenAction = action;
		std::cout << "Promise(" << this << ") thenWith 1 " << m_rejected << std::endl;
	}

	void thenWith(std::function<void(TResult value)> action)
	{
		std::lock_guard<std::mutex> locl(m_mutex);
		std::cout << "Promise(" << this << ") thenWith 2 " << m_rejected << std::endl;
		m_thenAction = makeObject<Delegate<void(TResult)>>(action);
		std::cout << "Promise(" << this << ") thenWith 2 " << m_rejected << std::endl;
	}

	void catchWith(PromiseFailureDelegate* action)
	{
		std::lock_guard<std::mutex> locl(m_mutex);
		m_failAction = action;
	}

protected:
	// called task thread.
	virtual void invoke() override
	{
		std::cout << "invoke 1 " << m_rejected << std::endl;
		m_action(this);
		std::cout << "invoke 2 " << m_rejected << std::endl;
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
		, m_rejected(false)
	{
		std::cout << "Promise(" << this << ") cotr " << m_rejected << std::endl;
	}

	Promise(const std::function<void(Promise<TResult>*)>& action)
		: m_action(action)
		, m_rejected(false)
	{
		std::cout << "Promise(" << this << ") cotr " << m_rejected << std::endl;
	}

	~Promise()
	{
		std::cout << "Promise(" << this << ") destructor"  << std::endl;
	}

public:
	std::function<void(Promise<TResult>*)> m_action;
	TResult m_result;
	std::atomic<bool> m_rejected;
	Ref<Delegate<void(TResult value)>> m_thenAction;
	Ref<PromiseFailureDelegate> m_failAction;
	std::mutex m_mutex;
};

template<class TResult>
::ln::TypeInfo* const Promise<TResult>::_lnref_typeInfo = Promise<TResult>::_lnref_registerTypeInfo();

} // namespace ln
