// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once
#include <optional>
#include "CRCHash.hpp"

namespace ln {

/**
 * 任意の型Tの値と、追加の無効値状態を表現できる型です。
 * 
 * 次の例では、値を表示する前に変数に値が格納されているかどうかをテストします。
 * 
 * ~~~{.cpp}
 * Optional<int> x = 10;
 * if (x)
 *     std::cout << x.value();
 * else
 *     std::cout << "Undefined";
 * ~~~
 */
template<class T>
class Optional
{
public:
    /** Constructs an object that does not contain a value. */
    constexpr Optional() noexcept;

    /** Constructs an object that does not contain a value. */
    constexpr Optional(std::nullopt_t) noexcept;

    /** Copy constructor. */
	constexpr Optional(const Optional<T>& other);

    /** Move constructor. */
	constexpr Optional(Optional&& other) noexcept;

	/** 受け取った値を有効値として保持して構築します。 */
    Optional(const T& value);

	/** 受け取った値を有効値として保持して構築します。 */
    Optional(T&& value);

	/** T に変換可能な型 U を持つ Optional をコピーして構築します。 */
	template <class U>
    explicit Optional(const Optional<U>& rhs);

	/** T に変換可能な型 U を持つ Optional をムーブして構築します。 */
	template <class U>
    explicit Optional(Optional<U>&& rhs);

	/** T に変換可能な型 U の値から構築します。 */
	template<class U, typename std::enable_if<std::is_constructible<T, U>::value && std::is_convertible<U, T>::value, bool>::type = false>
    constexpr Optional(U&& value);

	/** T に変換可能な型 U の値から構築します。 */
	template<class U, typename std::enable_if< std::is_constructible<T, U>::value && !std::is_convertible<U, T>::value, bool>::type = false>
    explicit constexpr Optional(U&& value);

    /** Destructor. */
    ~Optional() = default;

    /** Assign content. */
    Optional& operator=(const Optional& other);

    /** Assign content. */
    Optional& operator=(Optional&& other) noexcept;

    /** Assign content. */
    Optional& operator=(std::nullptr_t) noexcept;

    /** Assign content. */
    Optional& operator=(const T& value);

    /** Assign content. */
    Optional& operator=(T&& value);

    /** @defgroup Observers */
    /** @{ */

    /** 値を保持しているかを確認します。 */
    constexpr explicit operator bool() const noexcept { return m_hasValue; }

    /** 値を保持しているかを確認します。 */
    bool hasValue() const { return m_hasValue; }

    /** 間接参照演算子で値を取得します。 */
    constexpr T& operator*() &;

    /** 間接参照演算子で値を取得します。 */
    constexpr const T& operator*() const &;

    /** 間接参照演算子で値を取得します。 */
    constexpr T&& operator*() &&;

    /** 間接参照演算子で値を取得します。 */
    constexpr const T&& operator*() const &&;

    /** 保持している値へのポインタを返します。 */
    constexpr T* operator->();

    /** 保持している値へのポインタを返します。 */
    constexpr const T* operator->() const;

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

    /** @} */
    /** @defgroup Modifiers */
    /** @{ */

    /** 値を保持していない状態にします。 */
    void reset() noexcept { m_hasValue = false; }

    void swap(Optional& other) noexcept;

    /** @} */

    bool equals(const Optional& right) const;

private:
    T m_value;
    bool m_hasValue;
};

template<class T>
constexpr bool operator==(const Optional<T>& lhs, const T& rhs)
{
    return lhs.hasValue() && lhs.value() == rhs;
}
template<class T>
constexpr bool operator==(const Optional<T>& lhs, const Optional<T>& rhs)
{
    return lhs.equals(rhs);
}
template<class T>
constexpr bool operator==(const Optional<T>& lhs, std::nullopt_t rhs)
{
    return !lhs.hasValue();
}
template<class T>
constexpr bool operator!=(const Optional<T>& lhs, const T& rhs)
{
    return !operator==(lhs, rhs);
}
template<class T>
constexpr bool operator!=(const Optional<T>& lhs, const Optional<T>& rhs)
{
    return !operator==(lhs, rhs);
}
template<class T>
constexpr bool operator!=(const Optional<T>& lhs, std::nullopt_t rhs)
{
    return !operator==(lhs, rhs);
}

template<class T>
constexpr Optional<T>::Optional() noexcept
    : m_value()
    , m_hasValue(false)
{
}

template<class T>
constexpr Optional<T>::Optional(std::nullopt_t) noexcept
    : m_value()
    , m_hasValue(false)
{
}

template<class T>
constexpr Optional<T>::Optional(const Optional<T>& other)
	: m_value(other.m_value)
	, m_hasValue(other.m_hasValue)
{}

template<class T>
constexpr Optional<T>::Optional(Optional&& other) noexcept
    : m_value(std::move(other.m_value))
    , m_hasValue(other.m_hasValue)
{
    other.reset();
}

template<class T>
Optional<T>::Optional(const T& value)
    : m_value(value)
    , m_hasValue(true)
{
}

template<class T>
Optional<T>::Optional(T&& value)
    : m_value(std::move(value))
    , m_hasValue(true)
{
}

template<class T>
template <class U>
Optional<T>::Optional(const Optional<U>& rhs)
    : m_value(std::forward<U>(rhs.m_value)), m_hasValue(rhs.m_hasValue)
{}

template<class T>
template <class U>
Optional<T>::Optional(Optional<U>&& rhs)
    : m_value(std::forward<U>(rhs.m_value)), m_hasValue(rhs.m_hasValue)
{}

template<class T>
template<class U, typename std::enable_if<std::is_constructible<T, U>::value&& std::is_convertible<U, T>::value, bool>::type>
constexpr Optional<T>::Optional(U&& value)
    : m_value(std::forward<U>(value)), m_hasValue(true)
{
    // Note: Perfect Initialization
}

template<class T>
template<class U, typename std::enable_if< std::is_constructible<T, U>::value && !std::is_convertible<U, T>::value, bool>::type>
constexpr Optional<T>::Optional(U&& value)
    : m_value(std::forward<U>(value)), m_hasValue(true)
{
    // Note: Perfect Initialization
}

template<class T>
Optional<T>& Optional<T>::operator=(const Optional& other)
{
    if (this != &other) {
        m_value = other.m_value;
        m_hasValue = other.m_hasValue;
    }
    return *this;
}

template<class T>
Optional<T>& Optional<T>::operator=(Optional&& other) noexcept
{
    m_value = std::move(other.m_value);
    m_hasValue = other.m_hasValue;
    other.m_hasValue = false;
    return *this;
}

template<class T>
Optional<T>& Optional<T>::operator=(std::nullptr_t) noexcept
{
    reset();
    return *this;
}

template<class T>
Optional<T>& Optional<T>::operator=(const T& value)
{
    m_value = value;
    m_hasValue = true;
    return *this;
}

template<class T>
Optional<T>& Optional<T>::operator=(T&& value)
{
    m_value = std::move(value);
    m_hasValue = true;
    return *this;
}

template<class T>
constexpr T& Optional<T>::operator*() &
{
    LN_CHECK(m_hasValue);
    return m_value;
}

template<class T>
constexpr const T& Optional<T>::operator*() const &
{
    LN_CHECK(m_hasValue);
    return m_value;
}

template<class T>
constexpr T&& Optional<T>::operator*() &&
{
    LN_CHECK(m_hasValue);
    return std::move(m_value);
}

template<class T>
constexpr const T&& Optional<T>::operator*() const &&
{
    LN_CHECK(m_hasValue);
    return std::move(m_value);
}

template<class T>
constexpr T* Optional<T>::operator->()
{
    LN_CHECK(m_hasValue);
    return &m_value;
}

template<class T>
constexpr const T* Optional<T>::operator->() const
{
    LN_CHECK(m_hasValue);
    return &m_value;
}

template<class T>
constexpr T& Optional<T>::value() &
{
    LN_CHECK(m_hasValue);
    return m_value;
}

template<class T>
constexpr const T& Optional<T>::value() const &
{
    LN_CHECK(m_hasValue);
    return m_value;
}

template<class T>
constexpr T&& Optional<T>::value() &&
{
    LN_CHECK(m_hasValue);
    return std::move(m_value);
}

template<class T>
constexpr const T&& Optional<T>::value() const &&
{
    LN_CHECK(m_hasValue);
    return std::move(m_value);
}

template<class T>
template<class U>
constexpr T Optional<T>::valueOr(U&& defaultValue) const &
{
    return hasValue() ? value() : static_cast<T>(std::forward<U>(defaultValue));
}

template<class T>
template<class U>
constexpr T Optional<T>::valueOr(U&& defaultValue) &&
{
    return hasValue() ? std::move(value()) : static_cast<T>(std::forward<U>(defaultValue));
}

template<class T>
void Optional<T>::swap(Optional& other) noexcept
{
    std::swap(m_value, other.m_value);
}

template<class T>
bool Optional<T>::equals(const Optional& right) const
{
    if (m_hasValue != right.m_hasValue)
        return false;
    if (!m_hasValue && !right.m_hasValue)
        return true;
    return m_value == right.m_value;
}

namespace detail {

template<class T>
uint32_t hashCode(ln::Optional<T>& opt)
{
    return (opt.hasValue()) ? CRCHash::compute(reinterpret_cast<const char*>(&opt.value()), sizeof(T)) : 0;
}

} // namespace detail
} // namespace ln

namespace std {

template<class T>
void swap(ln::Optional<T>& a, ln::Optional<T>& b) noexcept
{
    a.swap(b.m_hasValue);
}

} // namespace std

#include "OptionalRef.ipp"
