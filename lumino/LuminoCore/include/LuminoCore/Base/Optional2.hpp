// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once

#include "CRCHash.hpp"

namespace ln {

/**
 * 任意の型Tの値と、追加の無効値状態を表現できる型です。
 * 
 * 次の例では、値を表示する前に変数に値が格納されているかどうかをテストします。
 * 
 * ~~~{.cpp}
 * Optional2<int> x = 10;
 * if (x)
 *     std::cout << x.value();
 * else
 *     std::cout << "Undefined";
 * ~~~
 */
template<typename T>
class Optional2
{
public:
	/** Constructs an object that does not contain a value. */
    constexpr Optional2() noexcept;

	/** Constructs an object that does not contain a value. */
    constexpr Optional2(std::nullptr_t) noexcept;

	/** Copy constructor. */
	constexpr Optional2(const Optional2& other);

	/** Move constructor. */
	constexpr Optional2(Optional2&& other) noexcept;

	/** T に変換可能な型 U を持つ Optional2 をコピーして構築します。 */
	template <class U>
	Optional2(const Optional2<U>& rhs)
		: m_value(std::forward<U>(rhs.m_value)), m_hasValue(rhs.m_hasValue)
	{}

	/** T に変換可能な型 U を持つ Optional2 をムーブして構築します。 */
	template <class U>
	Optional2(Optional2<U>&& rhs)
		: m_value(std::forward<U>(rhs.m_value)), m_hasValue(rhs.m_hasValue)
	{}

	/** T に変換可能な型 U の値から構築します。 */
	template<class U, typename std::enable_if<std::is_constructible<T, U>::value && std::is_convertible<U, T>::value, bool>::type = false>
	constexpr Optional2(U&& value)
		: m_value(std::forward<U>(value)), m_hasValue(true)
	{
		// Note: Perfect Initialization
	}

	/** T に変換可能な型 U の値から構築します。 */
	template<class U, typename std::enable_if< std::is_constructible<T, U>::value && !std::is_convertible<U, T>::value, bool>::type = false>
	constexpr Optional2(U&& value)
		: m_value(std::forward<U>(value)), m_hasValue(true)
	{
		// Note: Perfect Initialization
	}

    /** Destructor. */
    ~Optional2() = default;

    Optional2& operator=(std::nullptr_t) noexcept;
    Optional2& operator=(const Optional2& other);
    Optional2& operator=(Optional2&& other) noexcept;
    Optional2& operator=(const T& value);
    Optional2& operator=(T&& value);

    template<class U>
    Optional2& operator=(const Optional2<U>& other)
    {
        if (other.hasValue()) {
            assign(other.value());
        }
        else {
            reset();
        }
        return *this;
    }

    template<class U>
    Optional2& operator=(Optional2<U>&& other)
    {
        m_value = std::move(other.m_value);
        m_hasValue = other.m_hasValue;
        other.m_hasValue = false;
        return *this;
    }

    template<class U, typename std::enable_if<std::is_constructible<T, U>::value&& std::is_convertible<U, T>::value, bool>::type = false>
    Optional2& operator=(U&& other)
    {
        assign(std::forward<U>(other));
        //m_value = std::move(other);
        //m_hasValue = true;
        return *this;
    }


    /** 値を保持していない状態にします。 */
    void reset() noexcept { m_hasValue = false; }

    /** 保持している値へのポインタを返します。 */
    constexpr const T* operator->() const;

    /** 保持している値へのポインタを返します。 */
    constexpr T* operator->();

    /** 間接参照演算子で値を取得します。 */
    constexpr const T& operator*() const &;

    /** 間接参照演算子で値を取得します。 */
    constexpr T& operator*() &;

    /** 間接参照演算子で値を取得します。 */
    constexpr const T&& operator*() const &&;

    /** 間接参照演算子で値を取得します。 */
    constexpr T&& operator*() &&;

    /** 値を保持しているかを確認します。 */
    constexpr explicit operator bool() const noexcept { return m_hasValue; }

    /** 値を保持しているかを確認します。 */
    constexpr bool hasValue() const noexcept { return m_hasValue; }

    /** 値を取得します。 */
    constexpr T& value() &;

    /** 値を取得します。 */
    constexpr const T& value() const &;

    /** 値を取得します。 */
    constexpr T&& value() &&;

    /** 値を取得します。 */
    constexpr const T&& value() const &&;

    /** 値を取得します。値を保持していない場合は指定された既定値を返します。 */
    template<class U>
    constexpr T valueOr(U&& defaultValue) const &;

    /** 値を取得します。値を保持していない場合は指定された既定値を返します。 */
    template<class U>
    constexpr T valueOr(U&& defaultValue) &&;

    bool equals(const Optional2& right) const;

private:
    template<class U>
    void assign(U&& value)
    {
        if (m_hasValue) {
            m_value = std::forward<U>(value);
        }
        else {
            construct(std::forward<U>(value));
        }
    }

    template<class U>
    T& construct(U&& value) {
        ::new (reinterpret_cast<void*>(&m_value)) T(std::forward<U>(value));
        m_hasValue = true;
        return m_value;
    }

    T m_value;
    bool m_hasValue;
};

template<class T>
constexpr bool operator==(const Optional2<T>& lhs, const T& rhs)
{
    return lhs.hasValue() && lhs.value() == rhs;
}
template<class T>
constexpr bool operator==(const Optional2<T>& lhs, const Optional2<T>& rhs)
{
    return lhs.equals(rhs);
}
template<class T>
constexpr bool operator==(const Optional2<T>& lhs, std::nullptr_t rhs)
{
    return !lhs.hasValue();
}
template<class T>
constexpr bool operator!=(const Optional2<T>& lhs, const T& rhs)
{
    return !operator==(lhs, rhs);
}
template<class T>
constexpr bool operator!=(const Optional2<T>& lhs, const Optional2<T>& rhs)
{
    return !operator==(lhs, rhs);
}
template<class T>
constexpr bool operator!=(const Optional2<T>& lhs, std::nullptr_t rhs)
{
    return !operator==(lhs, rhs);
}

template<class T>
constexpr Optional2<T>::Optional2() noexcept
    : m_value()
    , m_hasValue(false)
{
}

template<class T>
constexpr Optional2<T>::Optional2(std::nullptr_t) noexcept
    : m_value()
    , m_hasValue(false)
{
}

template<class T>
constexpr Optional2<T>::Optional2(const Optional2<T>& other)
	: m_value(other.m_value)
	, m_hasValue(other.m_hasValue)
{}

template<class T>
constexpr Optional2<T>::Optional2(Optional2&& other) noexcept
    : m_value(std::move(other.m_value))
    , m_hasValue(other.m_hasValue)
{
    other.reset();
}

//template<class T>
//Optional2<T>::Optional2(const T& value)
//    : m_value(value)
//    , m_hasValue(true)
//{
//}
//
//template<class T>
//Optional2<T>::Optional2(T&& value)
//    : m_value(std::move(value))
//    , m_hasValue(true)
//{
//}

template<class T>
Optional2<T>& Optional2<T>::operator=(const Optional2& other)
{
    if (this != &other) {
        m_value = other.m_value;
        m_hasValue = other.m_hasValue;
    }
    return *this;
}

template<class T>
Optional2<T>& Optional2<T>::operator=(Optional2&& other) noexcept
{
    m_value = std::move(other.m_value);
    m_hasValue = other.m_hasValue;
    other.m_hasValue = false;
    return *this;
}

template<class T>
Optional2<T>& Optional2<T>::operator=(std::nullptr_t) noexcept
{
    reset();
    return *this;
}

template<class T>
Optional2<T>& Optional2<T>::operator=(const T& value)
{
    m_value = value;
    m_hasValue = true;
    return *this;
}

template<class T>
Optional2<T>& Optional2<T>::operator=(T&& value)
{
    m_value = std::move(value);
    m_hasValue = true;
    return *this;
}

template<class T>
constexpr const T* Optional2<T>::operator->() const
{
    LN_CHECK(m_hasValue);
    return &m_value;
}

template<class T>
constexpr T* Optional2<T>::operator->()
{
    LN_CHECK(m_hasValue);
    return &m_value;
}

template<class T>
constexpr const T& Optional2<T>::operator*() const &
{
    LN_CHECK(m_hasValue);
    return m_value;
}

template<class T>
constexpr T& Optional2<T>::operator*() &
{
    LN_CHECK(m_hasValue);
    return m_value;
}

template<class T>
constexpr const T&& Optional2<T>::operator*() const &&
{
    LN_CHECK(m_hasValue);
    return std::move(m_value);
}

template<class T>
constexpr T&& Optional2<T>::operator*() &&
{
    LN_CHECK(m_hasValue);
    return std::move(m_value);
}

template<class T>
constexpr T& Optional2<T>::value() &
{
    LN_CHECK(m_hasValue);
    return m_value;
}

template<class T>
constexpr const T& Optional2<T>::value() const &
{
    LN_CHECK(m_hasValue);
    return m_value;
}

template<class T>
constexpr T&& Optional2<T>::value() &&
{
    LN_CHECK(m_hasValue);
    return std::move(m_value);
}

template<class T>
constexpr const T&& Optional2<T>::value() const &&
{
    LN_CHECK(m_hasValue);
    return std::move(m_value);
}

template<class T>
template<class U>
constexpr T Optional2<T>::valueOr(U&& defaultValue) const &
{
    return hasValue() ? value() : static_cast<T>(std::forward<U>(defaultValue));
}

template<class T>
template<class U>
constexpr T Optional2<T>::valueOr(U&& defaultValue) &&
{
    return hasValue() ? std::move(value()) : static_cast<T>(std::forward<U>(defaultValue));
}

template<class T>
bool Optional2<T>::equals(const Optional2& right) const
{
    if (m_hasValue != right.m_hasValue)
        return false;
    if (!m_hasValue && !right.m_hasValue)
        return true;
    return m_value == right.m_value;
}









template<typename T>
class Optional2<T&>
{
public:
    Optional2()
        : m_value(nullptr)
    {}

    Optional2(T& value)
        : m_value(&value)
    {}

    bool hasValue() const { return m_value != nullptr; }

    T& value()& { return *m_value; }


private:
    T* m_value;
};





namespace detail {

template<class T>
uint32_t hashCode(ln::Optional2<T>& opt)
{
    return (opt.hasValue()) ? CRCHash::compute(reinterpret_cast<const char*>(&opt.value()), sizeof(T)) : 0;
}

} // namespace detail
} // namespace ln

namespace std {

template<class T>
void swap(ln::Optional2<T>& a, ln::Optional2<T>& b) noexcept
{
    std::swap(a.m_hasValue, b.m_hasValue);
    std::swap(a.m_value, b.m_value);
}

} // namespace std
