
LN_NAMESPACE_BEGIN

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool Plane::checkInside(const Vector3& point) const
{
	return (Plane::dotCoord(*this, point) < 0.0f);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool Plane::checkInsideLower(const Vector3& point) const
{
	return (Plane::dotCoord(*this, point) <= 0.0f);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline bool Plane::checkInside(const Vector3& point, float radius) const
{
	return (Plane::dotCoord(*this, point) - radius < 0.0f);
}

LN_NAMESPACE_END
