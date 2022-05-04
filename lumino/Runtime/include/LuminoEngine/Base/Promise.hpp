
#pragma once
#include "../Engine/EngineContext2.hpp"
#include "Delegate.hpp"
#include "Task.hpp"

namespace ln {

/**
 * Promise failure.
 */
LN_DELEGATE()
using PromiseFailureDelegate = Delegate<void(void)>;

class PromiseBase
    : public Object {
public:
    // template<class TInput>
    // Ref<Promise> then(const std::function<void(Promise* p, TInput value)>& action)
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
 *
 * @note
 * JavaScript の Promise と異なり、メソッドチェーンは表現できない。
 * これは Promise を FFI に公開する際、実装が極めて煩雑になってしまうため。
 */
template<class TResult>
class Promise
    : public PromiseBase {
public:
    class Context final {
    public:
        void resolve(TResult value) { m_owner->resolve(value); }
        void reject() { m_owner->reject(); }

    private:
        Context(Promise* owner) : m_owner(owner) {}
        Promise* m_owner;
        friend class Promise;
    };

    // LN_OBJECT;
    friend class ::ln::TypeInfo;
    friend class ::ln::detail::EngineDomain;
    friend class ::ln::EngineContext2;
    static ::ln::TypeInfo* _lnref_getTypeInfo() {
        static ::ln::TypeInfo* _lnref_typeInfo = _lnref_registerTypeInfo();
        return _lnref_typeInfo;
    }
    ::ln::TypeInfo* _lnref_getThisTypeInfo() const { return _lnref_getTypeInfo(); }

    static ::ln::TypeInfo* _lnref_registerTypeInfo() {
        auto* context = ::ln::EngineContext2::instance();
        return context->registerType<Promise<TResult>>("__Promise", ::ln::TypeInfo::getTypeInfo<PromiseBase>(), {});
    }

    // static ::ln::TypeInfo* const _lnref_typeInfo LN_ATTRIBUTE_UNUSED_;

    static Ref<Promise> run(const std::function<void(Promise<TResult>::Context*)>& action) {
        auto p = Ref<Promise>(LN_NEW Promise(action), false);
        PromiseBase::enqueueInvoke(p);
        return p;
    }

    // TODO: internal
    static Ref<Promise> continueWith(Ref<GenericTask<TResult>> task) {
        auto p = Ref<Promise>(LN_NEW Promise([task](Promise<TResult>::Context* p) {
                                  p->resolve(task->result());
                              }),
                              false);
        PromiseBase::enqueueContinueWith(p, task);
        return p;
    }

    const TResult& result() const { return m_result; }

    Promise* then(Delegate<void(TResult value)>* action) {
        std::lock_guard<std::mutex> locl(m_mutex);
        m_thenAction = action;
        return this;
    }

    Promise* then(std::function<void(TResult value)> action) {
        std::lock_guard<std::mutex> locl(m_mutex);
        m_thenAction = makeObject<Delegate<void(TResult)>>(action);
        return this;
    }

    Promise* catchWith(PromiseFailureDelegate* action) {
        std::lock_guard<std::mutex> locl(m_mutex);
        m_failAction = action;
        return this;
    }

    Promise* catchWith(std::function<void()> action) {
        std::lock_guard<std::mutex> locl(m_mutex);
        m_failAction = makeObject<Delegate<void()>>(action);
        return this;
    }

    ~Promise() {
    }

protected:
    // called task thread.
    void invoke() override {
        Context context(this);
        try {
            m_action(&context);
        }
        catch (...) {
            LN_ERROR("An unhandled exception occurred during Promise processing.");
            reject();
        }
    }

    void callNext() override {
        if (m_rejected) {
            if (m_failAction) {
                m_failAction->call();
            }
            else {
                LN_ERROR(_TT("Promise rejected. (unhandling error)"));
            }
        }
        else if (m_thenAction) {
            m_thenAction->call(m_result);
        }
    }

LN_CONSTRUCT_ACCESS:
    Promise()
        : m_action()
        , m_rejected(false) {
    }

    Promise(const std::function<void(Promise<TResult>::Context*)>& action)
        : m_action(action)
        , m_rejected(false) {
    }

private:

    void resolve(TResult value) {
        std::lock_guard<std::mutex> locl(m_mutex);
        m_result = value;
    }

    void reject() {
        std::lock_guard<std::mutex> locl(m_mutex);
        m_rejected = true;
    }

    std::function<void(Promise<TResult>::Context*)> m_action;
    TResult m_result;
    std::atomic<bool> m_rejected;
    Ref<Delegate<void(TResult value)>> m_thenAction;
    Ref<PromiseFailureDelegate> m_failAction;
    std::mutex m_mutex;
};

// template<class TResult>
//::ln::TypeInfo* const Promise<TResult>::_lnref_typeInfo = Promise<TResult>::_lnref_registerTypeInfo();

} // namespace ln
