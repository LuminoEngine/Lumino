// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once

#include "CRCHash.hpp"

namespace ln {
namespace detail {
struct NontrivialDummyType {
    constexpr NontrivialDummyType() noexcept {}
};
}

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
 *
 * vs std::optional
 * ----------
 * ln::Optional は参照を扱うことができます。
 * これは主にコンテナの find() の戻り値として使用されています。
 *
 * @see Why Optional References Didn't Make It In C++17
 * https://www.fluentcpp.com/2018/10/05/pros-cons-optional-references/
 */
template<typename T>
class Optional {
public:
    /** Constructs an object that does not contain a value. */
    constexpr Optional() noexcept;

    /** Constructs an object that does not contain a value. */
    constexpr Optional(std::nullopt_t) noexcept;

    /** Copy constructor. */
    constexpr Optional(const Optional& other);

    /** Move constructor. */
    constexpr Optional(Optional&& other) noexcept;

    /** T に変換可能な型 U を持つ Optional をコピーして構築します。 */
    template<class U>
    explicit Optional(const Optional<U>& rhs)
        : m_value(std::forward<U>(rhs.m_value))
        , m_hasValue(rhs.m_hasValue) {}

    /** T に変換可能な型 U を持つ Optional をムーブして構築します。 */
    template<class U>
    explicit Optional(Optional<U>&& rhs)
        : m_value(std::forward<U>(rhs.m_value))
        , m_hasValue(rhs.m_hasValue) {}

    /** T に変換可能な型 U の値から構築します。 */
    template<class U, typename std::enable_if<std::is_constructible<T, U>::value && std::is_convertible<U, T>::value, bool>::type = false>
    constexpr Optional(U&& value)
        : m_value(std::forward<U>(value))
        , m_hasValue(true) {
        // Note: Perfect Initialization
    }

    /** T に変換可能な型 U の値から構築します。 */
    template<class U, typename std::enable_if<std::is_constructible<T, U>::value && !std::is_convertible<U, T>::value, bool>::type = false>
    explicit constexpr Optional(U&& value)
        : m_value(std::forward<U>(value))
        , m_hasValue(true) {
        // Note: Perfect Initialization
    }

    /** Destructor. */
    ~Optional() noexcept;

    Optional& operator=(std::nullopt_t) noexcept;
    Optional& operator=(const Optional& other);
    Optional& operator=(Optional&& other) noexcept;
    Optional& operator=(const T& value);
    Optional& operator=(T&& value);

    template<class U>
    Optional& operator=(const Optional<U>& other) {
        if (other.hasValue()) {
            assign(other.value());
        }
        else {
            reset();
        }
        return *this;
    }

    template<class U>
    Optional& operator=(Optional<U>&& other) {
        m_value = std::move(other.m_value);
        m_hasValue = other.m_hasValue;
        other.m_hasValue = false;
        return *this;
    }

    template<class U, typename std::enable_if<std::is_constructible<T, U>::value && std::is_convertible<U, T>::value, bool>::type = false>
    Optional& operator=(U&& other) {
        assign(std::forward<U>(other));
        // m_value = std::move(other);
        // m_hasValue = true;
        return *this;
    }

    /** 値を保持していない状態にします。 */
    void reset() noexcept;

    /** 保持している値へのポインタを返します。 */
    constexpr const T* operator->() const;

    /** 保持している値へのポインタを返します。 */
    constexpr T* operator->();

    /** 間接参照演算子で値を取得します。 */
    constexpr const T& operator*() const&;

    /** 間接参照演算子で値を取得します。 */
    constexpr T& operator*() &;

    /** 間接参照演算子で値を取得します。 */
    constexpr const T&& operator*() const&&;

    /** 間接参照演算子で値を取得します。 */
    constexpr T&& operator*() &&;

    /** 値を保持しているかを確認します。 */
    constexpr explicit operator bool() const noexcept { return m_hasValue; }

    /** 値を保持しているかを確認します。 */
    constexpr bool hasValue() const noexcept { return m_hasValue; }

    /** 値を取得します。 */
    constexpr T& value() &;

    /** 値を取得します。 */
    constexpr const T& value() const&;

    /** 値を取得します。 */
    constexpr T&& value() &&;

    /** 値を取得します。 */
    constexpr const T&& value() const&&;

    /** 値を取得します。値を保持していない場合は指定された既定値を返します。 */
    template<class U>
    constexpr T valueOr(U&& defaultValue) const&;

    /** 値を取得します。値を保持していない場合は指定された既定値を返します。 */
    template<class U>
    constexpr T valueOr(U&& defaultValue) &&;

    bool equals(const Optional& right) const;

private:
    template<class U>
    void assign(U&& value) {
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
	
    union {
        detail::NontrivialDummyType m_dummy;    // Avoid calling the constructor of T when it has no value.
        std::remove_const_t<T> m_value;
    };
    bool m_hasValue;
};

template<class T>
constexpr bool operator==(const Optional<T>& lhs, const T& rhs) {
    return lhs.hasValue() && lhs.value() == rhs;
}
template<class T>
constexpr bool operator==(const Optional<T>& lhs, const Optional<T>& rhs) {
    return lhs.equals(rhs);
}
template<class T>
constexpr bool operator==(const Optional<T>& lhs, std::nullptr_t rhs) {
    return !lhs.hasValue();
}
template<class T>
constexpr bool operator!=(const Optional<T>& lhs, const T& rhs) {
    return !operator==(lhs, rhs);
}
template<class T>
constexpr bool operator!=(const Optional<T>& lhs, const Optional<T>& rhs) {
    return !operator==(lhs, rhs);
}
template<class T>
constexpr bool operator!=(const Optional<T>& lhs, std::nullptr_t rhs) {
    return !operator==(lhs, rhs);
}

template<class T>
constexpr Optional<T>::Optional() noexcept
    : m_dummy{}
    , m_hasValue(false) {
}

template<class T>
constexpr Optional<T>::Optional(std::nullopt_t) noexcept
    : m_dummy{}
    , m_hasValue(false) {
}

template<class T>
constexpr Optional<T>::Optional(const Optional<T>& other)
    : m_value(other.m_value)
    , m_hasValue(other.m_hasValue) {
}

template<class T>
constexpr Optional<T>::Optional(Optional&& other) noexcept
    : m_value(std::move(other.m_value))
    , m_hasValue(other.m_hasValue) {
    other.reset();
}

// template<class T>
// Optional<T>::Optional(const T& value)
//     : m_value(value)
//     , m_hasValue(true)
//{
// }
//
// template<class T>
// Optional<T>::Optional(T&& value)
//     : m_value(std::move(value))
//     , m_hasValue(true)
//{
// }

template<class T>
Optional<T>::~Optional() noexcept {
    reset();
}

template<class T>
Optional<T>& Optional<T>::operator=(const Optional& other) {
    if (other.hasValue()) {
        assign(other.value());
    }
    else {
        reset();
    }
    return *this;
}

template<class T>
Optional<T>& Optional<T>::operator=(Optional&& other) noexcept {
    if (other.hasValue()) {
        assign(std::move(other.value()));
    }
    else {
        reset();
    }
    other.m_hasValue = false;
    return *this;
}

template<class T>
Optional<T>& Optional<T>::operator=(std::nullopt_t) noexcept {
    reset();
    return *this;
}

template<class T>
Optional<T>& Optional<T>::operator=(const T& value) {
    assign(value);
    return *this;
}

template<class T>
Optional<T>& Optional<T>::operator=(T&& value) {
    assign(std::forward<T>(value));
    return *this;
}

template<class T>
void Optional<T>::reset() noexcept {
    if (m_hasValue) {
        m_value.~T();
        m_hasValue = false;
    }
}

template<class T>
constexpr const T* Optional<T>::operator->() const {
    LN_CHECK(m_hasValue);
    return &m_value;
}

template<class T>
constexpr T* Optional<T>::operator->() {
    LN_CHECK(m_hasValue);
    return &m_value;
}

template<class T>
constexpr const T& Optional<T>::operator*() const& {
    LN_CHECK(m_hasValue);
    return m_value;
}

template<class T>
constexpr T& Optional<T>::operator*() & {
    LN_CHECK(m_hasValue);
    return m_value;
}

template<class T>
constexpr const T&& Optional<T>::operator*() const&& {
    LN_CHECK(m_hasValue);
    return std::move(m_value);
}

template<class T>
constexpr T&& Optional<T>::operator*() && {
    LN_CHECK(m_hasValue);
    return std::move(m_value);
}

template<class T>
constexpr T& Optional<T>::value() & {
    LN_CHECK(m_hasValue);
    return m_value;
}

template<class T>
constexpr const T& Optional<T>::value() const& {
    LN_CHECK(m_hasValue);
    return m_value;
}

template<class T>
constexpr T&& Optional<T>::value() && {
    LN_CHECK(m_hasValue);
    return std::move(m_value);
}

template<class T>
constexpr const T&& Optional<T>::value() const&& {
    LN_CHECK(m_hasValue);
    return std::move(m_value);
}

template<class T>
template<class U>
constexpr T Optional<T>::valueOr(U&& defaultValue) const& {
    return hasValue() ? value() : static_cast<T>(std::forward<U>(defaultValue));
}

template<class T>
template<class U>
constexpr T Optional<T>::valueOr(U&& defaultValue) && {
    return hasValue() ? std::move(value()) : static_cast<T>(std::forward<U>(defaultValue));
}

template<class T>
bool Optional<T>::equals(const Optional& right) const {
    if (m_hasValue != right.m_hasValue)
        return false;
    if (!m_hasValue && !right.m_hasValue)
        return true;
    return m_value == right.m_value;
}

template<typename T>
class Optional<T&> {
public:
    Optional()
        : m_value(nullptr) {}

    Optional(T& value)
        : m_value(&value) {}

    bool hasValue() const { return m_value != nullptr; }

    T& value() & { return *m_value; }

    constexpr const T* operator->() const {
        LN_CHECK(m_value);
        return m_value;
    }

    constexpr T* operator->() {
        LN_CHECK(m_value);
        return m_value;
    }

    constexpr const T& operator*() const& {
        LN_CHECK(m_value);
        return *m_value;
    }

    constexpr T& operator*() & {
        LN_CHECK(m_value);
        return *m_value;
    }

    constexpr const T&& operator*() const&& {
        LN_CHECK(m_value);
        return *m_value;
    }

    constexpr T&& operator*() && {
        LN_CHECK(m_value);
        return *m_value;
    }

    constexpr explicit operator bool() const noexcept { return m_value != nullptr; }

private:
    T* m_value;
};

namespace detail {

template<class T>
uint32_t hashCode(ln::Optional<T>& opt) {
    return (opt.hasValue()) ? CRCHash::compute(reinterpret_cast<const char*>(&opt.value()), sizeof(T)) : 0;
}

} // namespace detail
} // namespace ln

namespace std {

template<class T>
void swap(ln::Optional<T>& a, ln::Optional<T>& b) noexcept {
    std::swap(a.m_hasValue, b.m_hasValue);
    std::swap(a.m_value, b.m_value);
}

} // namespace std
