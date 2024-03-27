// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once

namespace ln {

template<class T>
class Optional_deprecated<T&>
{
public:
    constexpr Optional_deprecated() noexcept;
    constexpr Optional_deprecated(std::nullopt_t) noexcept;
	constexpr Optional_deprecated(const Optional_deprecated& other);
    constexpr Optional_deprecated(T& value);
	template <class U>
    explicit Optional_deprecated(Optional_deprecated<U&>& rhs);

    ~Optional_deprecated() = default;

    Optional_deprecated& operator=(const Optional_deprecated& other);
    Optional_deprecated& operator=(std::nullopt_t) noexcept;
    Optional_deprecated& operator=(T& value);

    constexpr explicit operator bool() const noexcept { return m_value != nullptr; }

    bool hasValue() const { return m_value != nullptr; }
    constexpr T& operator*() &;
    constexpr const T& operator*() const &;
    constexpr T* operator->();
    constexpr const T* operator->() const;
    constexpr T& value() &;
    constexpr const T& value() const &;
    template<class U>
    constexpr T valueOr(U&& defaultValue) const &;

    void reset() noexcept { m_value = nullptr; }
    void swap(Optional_deprecated& other) noexcept;

    template<class U>
    bool equals(const Optional_deprecated<U>& right) const;

private:
    T* m_value;
};

template<class T>
constexpr Optional_deprecated<T&>::Optional_deprecated() noexcept
    : m_value(nullptr)
{
}

template<class T>
constexpr Optional_deprecated<T&>::Optional_deprecated(std::nullopt_t) noexcept
    : m_value(nullptr)
{
}

template<class T>
constexpr Optional_deprecated<T&>::Optional_deprecated(const Optional_deprecated& other)
	: m_value(other.m_value)
{}


template<class T>
constexpr Optional_deprecated<T&>::Optional_deprecated(T& value)
    : m_value(&value)
{
}

template<class T>
template <class U>
Optional_deprecated<T&>::Optional_deprecated(Optional_deprecated<U&>& rhs)
    : m_value(&rhs.value())
{}

template<class T>
Optional_deprecated<T&>& Optional_deprecated<T&>::operator=(const Optional_deprecated& other)
{
    if (this != &other) {
        m_value = other.m_value;
    }
    return *this;
}

template<class T>
Optional_deprecated<T&>& Optional_deprecated<T&>::operator=(std::nullopt_t) noexcept
{
    reset();
    return *this;
}

template<class T>
Optional_deprecated<T&>& Optional_deprecated<T&>::operator=(T& value)
{
    m_value = &value;
    return *this;
}

template<class T>
constexpr T& Optional_deprecated<T&>::operator*() &
{
    LN_CHECK(m_value);
    return *m_value;
}

template<class T>
constexpr const T& Optional_deprecated<T&>::operator*() const &
{
    LN_CHECK(m_value);
    return *m_value;
}

template<class T>
constexpr T* Optional_deprecated<T&>::operator->()
{
    LN_CHECK(m_value);
    return &m_value;
}

template<class T>
constexpr const T* Optional_deprecated<T&>::operator->() const
{
    LN_CHECK(m_value);
    return &m_value;
}

template<class T>
constexpr T& Optional_deprecated<T&>::value() &
{
    LN_CHECK(m_value);
    return *m_value;
}

template<class T>
constexpr const T& Optional_deprecated<T&>::value() const &
{
    LN_CHECK(m_value);
    return *m_value;
}

template<class T>
template<class U>
constexpr T Optional_deprecated<T&>::valueOr(U&& defaultValue) const &
{
    return hasValue() ? value() : static_cast<T>(std::forward<U>(defaultValue));
}

template<class T>
template<class U>
bool Optional_deprecated<T&>::equals(const Optional_deprecated<U>& right) const
{
    if (hasValue() != right.hasValue())
        return false;
    if (!hasValue() && !right.hasValue())
        return true;
    return value() == right.value();
}

template<class T>
void Optional_deprecated<T&>::swap(Optional_deprecated& other) noexcept
{
    std::swap(m_value, other.m_value);
}

} // namespace ln