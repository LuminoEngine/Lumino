// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once

namespace ln {

template<class T>
class Optional<T&>
{
public:
    constexpr Optional() noexcept;
    constexpr Optional(std::nullopt_t) noexcept;
	constexpr Optional(const Optional& other);
    constexpr Optional(T& value);
	template <class U>
    explicit Optional(Optional<U&>& rhs);

    ~Optional() = default;

    Optional& operator=(const Optional& other);
    Optional& operator=(std::nullopt_t) noexcept;
    Optional& operator=(T& value);

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
    void swap(Optional& other) noexcept;

    template<class U>
    bool equals(const Optional<U>& right) const;

private:
    T* m_value;
};

template<class T>
constexpr Optional<T&>::Optional() noexcept
    : m_value(nullptr)
{
}

template<class T>
constexpr Optional<T&>::Optional(std::nullopt_t) noexcept
    : m_value(nullptr)
{
}

template<class T>
constexpr Optional<T&>::Optional(const Optional& other)
	: m_value(other.m_value)
{}


template<class T>
constexpr Optional<T&>::Optional(T& value)
    : m_value(&value)
{
}

template<class T>
template <class U>
Optional<T&>::Optional(Optional<U&>& rhs)
    : m_value(&rhs.value())
{}

template<class T>
Optional<T&>& Optional<T&>::operator=(const Optional& other)
{
    if (this != &other) {
        m_value = other.m_value;
    }
    return *this;
}

template<class T>
Optional<T&>& Optional<T&>::operator=(std::nullopt_t) noexcept
{
    reset();
    return *this;
}

template<class T>
Optional<T&>& Optional<T&>::operator=(T& value)
{
    m_value = &value;
    return *this;
}

template<class T>
constexpr T& Optional<T&>::operator*() &
{
    LN_CHECK(m_value);
    return *m_value;
}

template<class T>
constexpr const T& Optional<T&>::operator*() const &
{
    LN_CHECK(m_value);
    return *m_value;
}

template<class T>
constexpr T* Optional<T&>::operator->()
{
    LN_CHECK(m_value);
    return &m_value;
}

template<class T>
constexpr const T* Optional<T&>::operator->() const
{
    LN_CHECK(m_value);
    return &m_value;
}

template<class T>
constexpr T& Optional<T&>::value() &
{
    LN_CHECK(m_value);
    return *m_value;
}

template<class T>
constexpr const T& Optional<T&>::value() const &
{
    LN_CHECK(m_value);
    return *m_value;
}

template<class T>
template<class U>
constexpr T Optional<T&>::valueOr(U&& defaultValue) const &
{
    return hasValue() ? value() : static_cast<T>(std::forward<U>(defaultValue));
}

template<class T>
template<class U>
bool Optional<T&>::equals(const Optional<U>& right) const
{
    if (hasValue() != right.hasValue())
        return false;
    if (!hasValue() && !right.hasValue())
        return true;
    return value() == right.value();
}

template<class T>
void Optional<T&>::swap(Optional& other) noexcept
{
    std::swap(m_value, other.m_value);
}

} // namespace ln