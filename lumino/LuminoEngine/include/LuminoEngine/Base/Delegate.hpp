
#pragma once
#include <functional>
#include "../Engine/Object.hpp"

namespace ln {

// base for specialization
template<typename>
class Delegate;

/**
 */
template<class TReturn, class... TArgs>
class Delegate<TReturn(TArgs...)>
	: public Object
{
public:
	Delegate(const std::function<TReturn(TArgs...)>& function)
		: m_function(function)
		, m_alternative()
	{}

	Delegate(const std::function<void()>& function)
		: m_function()
		, m_alternative(function)
	{}

	bool isEmpty() const
	{
		return !m_function || !m_alternative;
	}

	TReturn call(TArgs... args)
	{
		if (m_function)
			return m_function(std::forward<TArgs>(args)...);
		else if (m_alternative)
			m_alternative();
	}

LN_CONSTRUCT_ACCESS:
	Delegate()
		: m_function()
	{}

	bool init()
	{
		return Object::init();
	}

	void init(const std::function<TReturn(TArgs...)>& function)
	{
		init();
		m_function = function;
	}

private:
	std::function<TReturn(TArgs...)> m_function;
	std::function<void()> m_alternative;
};

/**
 */
template<>
class Delegate<void()>
	: public Object
{
public:
	Delegate(const std::function<void()>& function)
		: m_function(function)
	{}

	bool isEmpty() const
	{
		return !m_function;
	}

	void call()
	{
		m_function();
	}

LN_CONSTRUCT_ACCESS:
	Delegate()
		: m_function()
	{}

	bool init()
	{
		return Object::init();
	}

	void init(const std::function<void()>& function)
	{
		init();
		m_function = function;
	}

private:
	std::function<void()> m_function;
};

template<class TReturn, class... TArgs>
class Ref<Delegate<TReturn(TArgs...)>>
{
public:
	constexpr Ref() noexcept;

	constexpr Ref(std::nullptr_t) noexcept;

	Ref(Delegate<TReturn(TArgs...)>* ptr);

	Ref(Delegate<TReturn(TArgs...)>* ptr, bool retain);

	Ref(const Ref& ref) noexcept;

	template<class Y>
	Ref(const Ref<Y>& ref) noexcept;

	Ref(Ref&& ref) noexcept;

	template<class Y>
	Ref(Ref<Y>&& ref) noexcept;


	template<class TF>
	Ref(TF function)
		: m_ptr(LN_NEW Delegate<TReturn(TArgs...)>(function))
	{}

	//Ref(std::function<TReturn(TArgs...)> function)
	//	: m_ptr(LN_NEW Delegate<TReturn(TArgs...)>(function))
	//{}


	~Ref();

	void reset(Delegate<TReturn(TArgs...)>* ptr, bool retain = true);

	void reset();

	void swap(Ref<Delegate<TReturn(TArgs...)>>& other);

	Delegate<TReturn(TArgs...)>* detach();

	Delegate<TReturn(TArgs...)>* get() const;

	Ref& operator=(const Ref& ref) noexcept;

	template<class Y>
	Ref& operator=(const Ref<Y>& ref) noexcept;

	Ref& operator=(Ref&& ref) noexcept;

	template<class Y>
	Ref& operator=(Ref<Y>&& ref) noexcept;

	Delegate<TReturn(TArgs...)>& operator*() const noexcept;

	Delegate<TReturn(TArgs...)>* operator->() const noexcept;

	explicit operator bool() const noexcept { return (m_ptr != nullptr); }

	operator Delegate<TReturn(TArgs...)>*() const { return static_cast<Delegate<TReturn(TArgs...)>*>(m_ptr); } // ここでコンパイルエラーとなる場合、Delegate<TReturn(TArgs...)> の定義があるヘッダファイルを include しているか確認すること。

	RefObject* basePointer() const { return m_ptr; }

protected:
	void safeAddRef()
	{
		LN_SAFE_RETAIN(m_ptr);
	}

	void safeRelease()
	{
		LN_SAFE_RELEASE(m_ptr);
	}

	Delegate<TReturn(TArgs...)>* m_ptr;

	template<class U> friend class Ref;
};

template<class TReturn, class... TArgs>
constexpr Ref<Delegate<TReturn(TArgs...)>>::Ref() noexcept
	: m_ptr(nullptr)
{
}

template<class TReturn, class... TArgs>
constexpr Ref<Delegate<TReturn(TArgs...)>>::Ref(std::nullptr_t) noexcept
	: m_ptr(nullptr)
{
}

template<class TReturn, class... TArgs>
Ref<Delegate<TReturn(TArgs...)>>::Ref(Delegate<TReturn(TArgs...)>* ptr)
	: Ref(ptr, true)
{
}

template<class TReturn, class... TArgs>
Ref<Delegate<TReturn(TArgs...)>>::Ref(Delegate<TReturn(TArgs...)>* ptr, bool retain)
	: m_ptr(ptr)
{
	if (retain) {
		LN_SAFE_RETAIN(m_ptr);
	}
}

template<class TReturn, class... TArgs>
Ref<Delegate<TReturn(TArgs...)>>::Ref(const Ref& ref) noexcept
	: m_ptr(ref.m_ptr)
{
	LN_SAFE_RETAIN(m_ptr);
}

template<class TReturn, class... TArgs>
template<class Y>
Ref<Delegate<TReturn(TArgs...)>>::Ref(const Ref<Y>& ref) noexcept
	: m_ptr(static_cast<Delegate<TReturn(TArgs...)>*>(ref.get()))
{
	LN_SAFE_RETAIN(m_ptr);
}

template<class TReturn, class... TArgs>
Ref<Delegate<TReturn(TArgs...)>>::Ref(Ref&& ref) noexcept
{
	m_ptr = ref.m_ptr;
	ref.m_ptr = nullptr;
}

template<class TReturn, class... TArgs>
template<class Y>
Ref<Delegate<TReturn(TArgs...)>>::Ref(Ref<Y>&& ref) noexcept
{
	m_ptr = static_cast<Delegate<TReturn(TArgs...)>*>(ref.get());
	ref.m_ptr = nullptr;
}

template<class TReturn, class... TArgs>
Ref<Delegate<TReturn(TArgs...)>>::~Ref()
{
	LN_SAFE_RELEASE(m_ptr);
}

template<class TReturn, class... TArgs>
void Ref<Delegate<TReturn(TArgs...)>>::reset(Delegate<TReturn(TArgs...)>* ptr, bool retain)
{
	if (ptr != m_ptr) {
		safeRelease();
		m_ptr = ptr;
		if (retain) {
			safeAddRef();
		}
	}
}

template<class TReturn, class... TArgs>
void Ref<Delegate<TReturn(TArgs...)>>::reset()
{
	safeRelease();
}

template<class TReturn, class... TArgs>
void Ref<Delegate<TReturn(TArgs...)>>::swap(Ref<Delegate<TReturn(TArgs...)>>& other)
{
	if (&other != this) {
		Delegate<TReturn(TArgs...)>* t = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = t;
	}
}

template<class TReturn, class... TArgs>
Delegate<TReturn(TArgs...)>* Ref<Delegate<TReturn(TArgs...)>>::detach()
{
	RefObject* ptr = m_ptr;
	m_ptr = nullptr;
	return static_cast<Delegate<TReturn(TArgs...)>*>(ptr);
}

template<class TReturn, class... TArgs>
Delegate<TReturn(TArgs...)>* Ref<Delegate<TReturn(TArgs...)>>::get() const
{
	return static_cast<Delegate<TReturn(TArgs...)>*>(m_ptr);
}

template<class TReturn, class... TArgs>
Ref<Delegate<TReturn(TArgs...)>>& Ref<Delegate<TReturn(TArgs...)>>::operator=(const Ref<Delegate<TReturn(TArgs...)>>& ref) noexcept
{
	LN_REFOBJ_SET(m_ptr, ref.m_ptr);
	return *this;
}

template<class TReturn, class... TArgs>
template<class Y>
Ref<Delegate<TReturn(TArgs...)>>& Ref<Delegate<TReturn(TArgs...)>>::operator=(const Ref<Y>& ref) noexcept
{
	LN_REFOBJ_SET(m_ptr, static_cast<Delegate<TReturn(TArgs...)>*>(ref.get()));
	return *this;
}

template<class TReturn, class... TArgs>
Ref<Delegate<TReturn(TArgs...)>>& Ref<Delegate<TReturn(TArgs...)>>::operator=(Ref&& ref) noexcept
{
	if (&ref != this) {
		LN_SAFE_RELEASE(m_ptr);
		m_ptr = ref.m_ptr;
		ref.m_ptr = nullptr;
	}
	return *this;
}

template<class TReturn, class... TArgs>
template<class Y>
Ref<Delegate<TReturn(TArgs...)>>& Ref<Delegate<TReturn(TArgs...)>>::operator=(Ref<Y>&& ref) noexcept
{
	LN_SAFE_RELEASE(m_ptr);
	m_ptr = static_cast<Delegate<TReturn(TArgs...)>*>(ref.get());
	ref.m_ptr = nullptr;
	return *this;
}

template<class TReturn, class... TArgs>
Delegate<TReturn(TArgs...)>& Ref<Delegate<TReturn(TArgs...)>>::operator*() const noexcept
{
	assert(m_ptr != nullptr);
	return *static_cast<Delegate<TReturn(TArgs...)>*>(m_ptr);
}

template<class TReturn, class... TArgs>
Delegate<TReturn(TArgs...)>* Ref<Delegate<TReturn(TArgs...)>>::operator->() const noexcept
{
	assert(m_ptr != nullptr);
	return static_cast<Delegate<TReturn(TArgs...)>*>(m_ptr);
}

//==============================================================================

template<class TClass, typename TReturn, typename... TArgs>
Ref<Delegate<TReturn(TArgs...)>> makeDelegate(TClass* c, TReturn(TClass::*m)(TArgs...))
{
	auto* ptr = LN_NEW Delegate<TReturn(TArgs...)>([=](TArgs&&... args) { return (c->*m)(std::forward<decltype(args)>(args)...); });
	return Ref<Delegate<TReturn(TArgs...)>>(ptr, false);
}

template<class TClass, typename TReturn, typename... TArgs>
Ref<Delegate<TReturn(TArgs...)>> makeDelegate(const TClass* c, TReturn(TClass::*m)(TArgs...) const)
{
	auto* ptr = LN_NEW Delegate<TReturn(TArgs...)>([=](TArgs&&... args) { return (c->*m)(std::forward<decltype(args)>(args)...); });
	return Ref<Delegate<TReturn(TArgs...)>>(ptr, false);
}

} // namespace ln
