
#pragma once
#include "CRCHash.hpp"

namespace ln {

template<typename T> 
class Optional
{
public:
	LN_CONSTEXPR Optional() LN_NOEXCEPT
		: m_value(), m_hasValue(false)
	{}

	LN_CONSTEXPR Optional(std::nullptr_t) LN_NOEXCEPT
		: m_value(), m_hasValue(false)
	{}

	Optional(const Optional& other)
		: m_value(other.m_value), m_hasValue(other.m_hasValue)
	{}

	Optional(Optional&& other) LN_NOEXCEPT
		: m_value(std::move(other.m_value)), m_hasValue(other.m_hasValue)
	{
		other.reset();
	}

	Optional(const T& value)
		: m_value(value), m_hasValue(true)
	{}

	Optional(T&& value)
		: m_value(std::move(value)), m_hasValue(true)
	{}


    Optional& operator=(const Optional& other) 
    {
		if (this != &other)
		{
			m_value = other.m_value;
			m_hasValue = other.m_hasValue;
		}
        return *this;
    }

	Optional& operator=(Optional&& other) LN_NOEXCEPT
	{
		m_value = std::move(other.m_value);
		m_hasValue = other.m_hasValue;
		other.m_hasValue = false;
		return *this;
	}

	Optional& operator=(std::nullptr_t) LN_NOEXCEPT
	{
		reset();
		return *this;
	}

	Optional& operator=(const T& value)
	{
		m_hasValue = true;
		return *this;
	}

	Optional& operator=(T&& value)
	{
		m_value = std::move(value);
		m_hasValue = true;
		return *this;
	}


	/** 値を保持していない状態にします。 */
	void reset() LN_NOEXCEPT { m_hasValue = false; }

	/** 値を保持しているかを確認します。 */
	LN_CONSTEXPR explicit operator bool() const LN_NOEXCEPT
	{
		return m_hasValue;
	}

	/** 値を保持しているかを確認します。 */
	bool hasValue() const { return m_hasValue; }

	/** 間接参照演算子で値を取得します。 */
	LN_CONSTEXPR T& operator*() &
	{
		LN_FATAL(m_hasValue);
		return m_value;
	}

	/** 間接参照演算子で値を取得します。 */
	LN_CONSTEXPR const T& operator*() const&
	{
		LN_FATAL(m_hasValue);
		return m_value;
	}

	/** 間接参照演算子で値を取得します。 */
	LN_CONSTEXPR T&& operator*() &&
	{
		LN_FATAL(m_hasValue);
		return std::move(m_value);
	}

	/** 間接参照演算子で値を取得します。 */
	LN_CONSTEXPR const T&& operator*() const&&
	{
		LN_FATAL(m_hasValue);
		return std::move(m_value);
	}

	/** 保持している値へのポインタを返します。 */
	LN_CONSTEXPR T* operator->()
	{
		LN_FATAL(m_hasValue);
		return &m_value;
	}

	/** 保持している値へのポインタを返します。 */
	LN_CONSTEXPR const T* operator->() const
	{
		LN_FATAL(m_hasValue);
		return &m_value;
	}

	/** 値を取得します。 */
	LN_CONSTEXPR T& value() &
	{
		LN_FATAL(m_hasValue);
		return m_value;
	}

	/** 値を取得します。 */
	LN_CONSTEXPR const T& value() const&
	{
		LN_FATAL(m_hasValue);
		return m_value;
	}

	/** 値を取得します。 */
	LN_CONSTEXPR T&& value() &&
	{
		LN_FATAL(m_hasValue);
		return std::move(m_value);
	}

	/** 値を取得します。 */
	LN_CONSTEXPR const T&& value() const&&
	{
		LN_FATAL(m_hasValue);
		return std::move(m_value);
	}

	/** 値を取得します。値を保持していない場合は指定された既定値を返します。 */
	template <class U>
	LN_CONSTEXPR T valueOr(U&& defaultValue) const&
	{
		return hasValue() ? value() : static_cast<T>(std::forward<U>(defaultValue));
	}

	/** 値を取得します。値を保持していない場合は指定された既定値を返します。 */
	template <class U>
	LN_CONSTEXPR T valueOr(U&& defaultValue) &&
	{
		return hasValue() ? std::move(value()) : static_cast<T>(std::forward<U>(defaultValue));
	}

	uint32_t hashCode() const
	{
		return (m_hasValue) ? Hash::calcHash(reinterpret_cast<const char*>(&m_value), sizeof(m_value)) : 0;
	}

	bool equals(const Optional& right) const
	{
		if (m_hasValue != right.m_hasValue) return false;
		if (!m_hasValue && !right.m_hasValue) return true;
		return m_value == right.m_value;
	}

private:
    T m_value;
    bool m_hasValue;
};

template <class T>
LN_CONSTEXPR bool operator==(const Optional<T>& lhs, const T& rhs) { return lhs.hasValue() && lhs.value() == rhs; }
template <class T>
LN_CONSTEXPR bool operator==(const Optional<T>& lhs, const Optional<T>& rhs) { return lhs.equals(rhs); }
template <class T>
LN_CONSTEXPR bool operator==(const Optional<T>& lhs, nullptr_t rhs) { return !lhs.hasValue(); }
template <class T>
LN_CONSTEXPR bool operator!=(const Optional<T>& lhs, const T& rhs) { return !operator==(lhs, rhs); }
template <class T>
LN_CONSTEXPR bool operator!=(const Optional<T>& lhs, const Optional<T>& rhs) { return !operator==(lhs, rhs); }
template <class T>
LN_CONSTEXPR bool operator!=(const Optional<T>& lhs, nullptr_t rhs) { return !operator==(lhs, rhs); }

} // namespace ln

namespace std {

template <class T>
void swap(ln::Optional<T>& a, ln::Optional<T>& b) LN_NOEXCEPT
{
	std::swap(a.m_hasValue, b.m_hasValue);
	std::swap(a.m_value, b.m_value);
}

} // namespace std
