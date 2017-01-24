
LN_NAMESPACE_BEGIN

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool Plane::CheckInside(const Vector3& point) const
{
	return (Plane::DotCoord(*this, point) < 0.0f);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool Plane::CheckInsideLower(const Vector3& point) const
{
	return (Plane::DotCoord(*this, point) <= 0.0f);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool Plane::CheckInside(const Vector3& point, float radius) const
{
	return (Plane::DotCoord(*this, point) - radius < 0.0f);
}

LN_NAMESPACE_END
