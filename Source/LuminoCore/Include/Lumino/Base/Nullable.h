
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
	bool operator==(const T& value) const { return m_isSet && value == m_value; }
	bool operator==(const Nullable& right) const { return equals(right); }
	bool operator!=(const Nullable& right) const { return !equals(right); }
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

private:
    void set(T value) { m_value = value; m_isSet = true; }

	bool equals(const Nullable& right) const
	{
		if (m_isSet != right.m_isSet) return false;
		if (!m_isSet && !right.m_isSet) return true;
		return m_value == right.m_value;
	}

private:
    T m_value;
    bool m_isSet;
};

LN_NAMESPACE_END
