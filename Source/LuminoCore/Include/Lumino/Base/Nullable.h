
#pragma once
#include "Hash.h"

LN_NAMESPACE_BEGIN

template<typename T> 
class Nullable
{
public:
	LN_CONSTEXPR Nullable() LN_NOEXCEPT
		: m_value(), m_isSet(false)
	{}

	LN_CONSTEXPR Nullable(std::nullptr_t) LN_NOEXCEPT
		: m_value(), m_isSet(false)
	{}

	Nullable(const Nullable& other)
		: m_value(other.m_value), m_isSet(other.m_isSet)
	{}

	Nullable(Nullable&& other) LN_NOEXCEPT
		: m_value(std::move(other.m_value)), m_isSet(other.m_isSet)
	{
		other.reset();
	}

	Nullable(const T& value)
		: m_value(value), m_isSet(true)
	{}

    friend void swap(Nullable& a, Nullable& b)
    {
        using std::swap;
        swap(a.m_isSet, b.m_isSet);
        swap(a.m_value, b.m_value);
    }

    Nullable& operator=(Nullable other) 
    {
        swap(*this, other);
        return *this;
    }
	Nullable& operator=(std::nullptr_t)
	{
		reset();
		return *this;
	}

	T& operator=(const T& value) { set(value); return m_value; }
    operator T() const { return get(); }

    T& get() 
	{
		if (LN_REQUIRE(m_isSet)) return m_value;
        return m_value;
	}
	const T& get() const
	{
		if (LN_REQUIRE(m_isSet)) return m_value;
		return m_value;
	}

	bool isSet() const { return m_isSet; }	// TODO: hasValue
	bool isNull() const { return !m_isSet; }

    void reset() LN_NOEXCEPT { m_isSet = false; m_value = T(); }


	uint32_t getHashCode() const
	{
		return (m_isSet) ? Hash::calcHash(reinterpret_cast<const char*>(&m_value), sizeof(m_value)) : 0;
	}

	bool equals(const Nullable& right) const
	{
		if (m_isSet != right.m_isSet) return false;
		if (!m_isSet && !right.m_isSet) return true;
		return m_value == right.m_value;
	}

private:
    void set(T value) { m_value = value; m_isSet = true; }

private:
    T m_value;
    bool m_isSet;
};

template <class T>
LN_CONSTEXPR bool operator==(const Nullable<T>& lhs, const T& rhs) { return lhs.isSet() && lhs.get() == rhs; }
template <class T>
LN_CONSTEXPR bool operator==(const Nullable<T>& lhs, const Nullable<T>& rhs) { return lhs.equals(rhs); }
template <class T>
LN_CONSTEXPR bool operator==(const Nullable<T>& lhs, nullptr_t rhs) { return lhs.isNull(); }
template <class T>
LN_CONSTEXPR bool operator!=(const Nullable<T>& lhs, const T& rhs) { return !operator==(lhs, rhs); }
template <class T>
LN_CONSTEXPR bool operator!=(const Nullable<T>& lhs, const Nullable<T>& rhs) { return !operator==(lhs, rhs); }
template <class T>
LN_CONSTEXPR bool operator!=(const Nullable<T>& lhs, nullptr_t rhs) { return !operator==(lhs, rhs); }

LN_NAMESPACE_END
