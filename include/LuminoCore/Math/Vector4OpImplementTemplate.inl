
inline LN_OP_TYPE& LN_OP_TYPE::operator+=(const LN_OP_TYPE& v) noexcept
{
	LN_E0 += v.LN_E0;
	LN_E1 += v.LN_E1;
	LN_E2 += v.LN_E2;
	LN_E3 += v.LN_E3;
	return (*this);
}

inline LN_OP_TYPE& LN_OP_TYPE::operator+=(float v) noexcept
{
	LN_E0 += v;
	LN_E1 += v;
	LN_E2 += v;
	LN_E3 += v;
	return (*this);
}

inline LN_OP_TYPE& LN_OP_TYPE::operator-=(const LN_OP_TYPE& v) noexcept
{
	LN_E0 -= v.LN_E0;
	LN_E1 -= v.LN_E1;
	LN_E2 -= v.LN_E2;
	LN_E3 -= v.LN_E3;
	return (*this);
}

inline LN_OP_TYPE& LN_OP_TYPE::operator-=(float v) noexcept
{
	LN_E0 -= v;
	LN_E1 -= v;
	LN_E2 -= v;
	LN_E3 -= v;
	return (*this);
}

inline LN_OP_TYPE& LN_OP_TYPE::operator*=(const LN_OP_TYPE& v) noexcept
{
	LN_E0 *= v.LN_E0;
	LN_E1 *= v.LN_E1;
	LN_E2 *= v.LN_E2;
	LN_E3 *= v.LN_E3;
	return (*this);
}

inline LN_OP_TYPE& LN_OP_TYPE::operator*=(float v) noexcept
{
	LN_E0 *= v;
	LN_E1 *= v;
	LN_E2 *= v;
	LN_E3 *= v;
	return (*this);
}

inline LN_OP_TYPE& LN_OP_TYPE::operator/=(const LN_OP_TYPE& v) noexcept
{
	LN_E0 /= v.LN_E0;
	LN_E1 /= v.LN_E1;
	LN_E2 /= v.LN_E2;
	LN_E3 /= v.LN_E3;
	return (*this);
}

inline LN_OP_TYPE& LN_OP_TYPE::operator/=(float v) noexcept
{
	v = 1.0f / v;
	LN_E0 *= v;
	LN_E1 *= v;
	LN_E2 *= v;
	LN_E3 *= v;
	return (*this);
}

inline constexpr LN_OP_TYPE operator+(const LN_OP_TYPE& v1, const LN_OP_TYPE& v2) noexcept
{
	return LN_OP_TYPE(
		v1.LN_E0 + v2.LN_E0,
		v1.LN_E1 + v2.LN_E1,
		v1.LN_E2 + v2.LN_E2,
		v1.LN_E3 + v2.LN_E3);
}

inline constexpr LN_OP_TYPE operator+(const LN_OP_TYPE& v1, float v2) noexcept
{
	return LN_OP_TYPE(
		v1.LN_E0 + v2,
		v1.LN_E1 + v2,
		v1.LN_E2 + v2,
		v1.LN_E3 + v2);
}

inline constexpr LN_OP_TYPE operator+(float v1, const LN_OP_TYPE& v2) noexcept
{
	return LN_OP_TYPE(
		v1 + v2.LN_E0,
		v1 + v2.LN_E1,
		v1 + v2.LN_E2,
		v1 + v2.LN_E3);
}

inline constexpr LN_OP_TYPE operator-(const LN_OP_TYPE& v1, const LN_OP_TYPE& v2) noexcept
{
	return LN_OP_TYPE(
		v1.LN_E0 - v2.LN_E0,
		v1.LN_E1 - v2.LN_E1,
		v1.LN_E2 - v2.LN_E2,
		v1.LN_E3 - v2.LN_E3);
}

inline constexpr LN_OP_TYPE operator-(const LN_OP_TYPE& v1, float v2) noexcept
{
	return LN_OP_TYPE(
		v1.LN_E0 - v2,
		v1.LN_E1 - v2,
		v1.LN_E2 - v2,
		v1.LN_E3 - v2);
}

inline constexpr LN_OP_TYPE operator-(float v1, const LN_OP_TYPE& v2) noexcept
{
	return LN_OP_TYPE(
		v1 - v2.LN_E0,
		v1 - v2.LN_E1,
		v1 - v2.LN_E2,
		v1 - v2.LN_E3);
}

inline constexpr LN_OP_TYPE operator*(const LN_OP_TYPE& v1, const LN_OP_TYPE& v2) noexcept
{
	return LN_OP_TYPE(
		v1.LN_E0 * v2.LN_E0,
		v1.LN_E1 * v2.LN_E1,
		v1.LN_E2 * v2.LN_E2,
		v1.LN_E3 * v2.LN_E3);
}

inline constexpr LN_OP_TYPE operator*(const LN_OP_TYPE& v1, float v2) noexcept
{
	return LN_OP_TYPE(
		v1.LN_E0 * v2,
		v1.LN_E1 * v2,
		v1.LN_E2 * v2,
		v1.LN_E3 * v2);
}

inline constexpr LN_OP_TYPE operator*(float v1, const LN_OP_TYPE& v2) noexcept
{
	return LN_OP_TYPE(
		v1 * v2.LN_E0,
		v1 * v2.LN_E1,
		v1 * v2.LN_E2,
		v1 * v2.LN_E3);
}

inline constexpr LN_OP_TYPE operator/(const LN_OP_TYPE& v1, const LN_OP_TYPE& v2) noexcept
{
	return LN_OP_TYPE(
		v1.LN_E0 / v2.LN_E0,
		v1.LN_E1 / v2.LN_E1,
		v1.LN_E2 / v2.LN_E2,
		v1.LN_E3 / v2.LN_E3);
}

inline constexpr LN_OP_TYPE operator/(const LN_OP_TYPE& v1, float v2) noexcept
{
	v2 = 1.0f / v2;
	return LN_OP_TYPE(
		v1.LN_E0 * v2,
		v1.LN_E1 * v2,
		v1.LN_E2 * v2,
		v1.LN_E3 * v2);
}

inline constexpr LN_OP_TYPE operator/(float v1, const LN_OP_TYPE& v2) noexcept
{
	return LN_OP_TYPE(
		v1 / v2.LN_E0,
		v1 / v2.LN_E1,
		v1 / v2.LN_E2,
		v1 / v2.LN_E3);
}

inline constexpr LN_OP_TYPE operator-(const LN_OP_TYPE& v1) noexcept
{
	return LN_OP_TYPE(
		-v1.LN_E0,
		-v1.LN_E1,
		-v1.LN_E2,
		-v1.LN_E3);
}

inline constexpr bool LN_OP_TYPE::operator==(const LN_OP_TYPE& v) const noexcept
{
	return (LN_E0 == v.LN_E0 && LN_E1 == v.LN_E1 && LN_E2 == v.LN_E2 && LN_E3 == v.LN_E3);
}

inline constexpr bool LN_OP_TYPE::operator!=(const LN_OP_TYPE& v) const noexcept
{
	return (LN_E0 != v.LN_E0 || LN_E1 != v.LN_E1 || LN_E2 != v.LN_E2 || LN_E3 != v.LN_E3);
}

