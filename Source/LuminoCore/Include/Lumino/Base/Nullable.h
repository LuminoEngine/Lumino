
#pragma once 

LN_NAMESPACE_BEGIN

template<typename T> 
class Nullable
{
public:
	Nullable() : m_value(), m_isSet(false) {}
	Nullable(std::nullptr_t) : m_value(), m_isSet(false) {}
    Nullable(const T& value) : m_value(value), m_isSet(true) {} 
    Nullable(const Nullable& other) : m_value(other.m_value), m_isSet(other.m_isSet) {}

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

	T& operator=(const T& value) { set(value); return m_value; }
	bool operator==(const T& value) const { return m_isSet && value == m_value; }
	bool operator==(const Nullable& right) const { return Equals(right); }
	bool operator!=(const Nullable& right) const { return !Equals(right); }
    operator T() const { return Get(); }

    T& Get() 
	{
		if (LN_CHECK_STATE(m_isSet)) return m_value;
        return m_value;
	}
	const T& Get() const
	{
		if (LN_CHECK_STATE(m_isSet)) return m_value;
		return m_value;
	}

	bool IsSet() const { return m_isSet; }
	bool IsNull() const { return !m_isSet; }

    void reset() { m_isSet = false; m_value = T(); }


	uint32_t GetHashCode() const
	{
		return (m_isSet) ? Hash::CalcHash(reinterpret_cast<const char*>(&m_value), sizeof(m_value)) : 0;
	}

private:
    void set(T value) { m_value = value; m_isSet = true; }

	bool Equals(const Nullable& right) const
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
