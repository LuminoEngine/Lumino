
#include "../../include/Lumino/Math/Matrix.h"
#include "../../include/Lumino/Math/ViewFrustum.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// ViewFrustum
//==============================================================================

//------------------------------------------------------------------------------
ViewFrustum::ViewFrustum()
{
}

//------------------------------------------------------------------------------
ViewFrustum::ViewFrustum(const Matrix& viewProjMatrix)
{
	SetViewProjMatrix(viewProjMatrix);
}

//------------------------------------------------------------------------------
void ViewFrustum::SetViewProjMatrix(const Matrix& mat)
{
	m_planes[static_cast<int>(FrustumPlane::Near)] = Plane(-mat.m13, -mat.m23, -mat.m33, -mat.m43);
	m_planes[static_cast<int>(FrustumPlane::Far)] = Plane(-mat.m14 + mat.m13, -mat.m24 + mat.m23, -mat.m34 + mat.m33, -mat.m44 + mat.m43);
	m_planes[static_cast<int>(FrustumPlane::Left)] = Plane(-mat.m14 - mat.m11, -mat.m24 - mat.m21, -mat.m34 - mat.m31, -mat.m44 - mat.m41);
	m_planes[static_cast<int>(FrustumPlane::Right)] = Plane(-mat.m14 + mat.m11, -mat.m24 + mat.m21, -mat.m34 + mat.m31, -mat.m44 + mat.m41);
	m_planes[static_cast<int>(FrustumPlane::Top)] = Plane(-mat.m14 + mat.m12, -mat.m24 + mat.m22, -mat.m34 + mat.m32, -mat.m44 + mat.m42);
	m_planes[static_cast<int>(FrustumPlane::Bottom)] = Plane(-mat.m14 - mat.m12, -mat.m24 - mat.m22, -mat.m34 - mat.m32, -mat.m44 - mat.m42);

	for (int i = 0; i < FrustumPlane_Max; ++i)
	{
		m_planes[i].Normalize();
	}

	m_viewProjMatrix = mat;
}

//------------------------------------------------------------------------------
bool ViewFrustum::Intersects(const Vector3& point) const
{
	for (int i = 0; i < FrustumPlane_Max; ++i)
	{
		if (!m_planes[i].CheckInsideLower(point)) {
			return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------------
bool ViewFrustum::Intersects(const Vector3& center, float radius) const
{
	for (int i = 0; i < FrustumPlane_Max; ++i)
	{
		if (!m_planes[i].CheckInside(center, radius)) {
			return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------------
void ViewFrustum::GetCornerPoints(Vector3* points) const
{
	if (points == nullptr) return;

	// Near
	points[0].set(-1, 1, 0);
	points[1].set(-1, -1, 0);
	points[2].set(1, -1, 0);
	points[3].set(1, 1, 0);

	// Far
	points[4].set(-1, 1, 1);
	points[5].set(-1, -1, 1);
	points[6].set(1, -1, 1);
	points[7].set(1, 1, 1);

	// Transfrom
	Matrix inv = Matrix::MakeInverse(m_viewProjMatrix);
	for (int i = 0; i < 8; ++i)
	{
		points[i].TransformCoord(inv);
	}
}

LN_NAMESPACE_END

