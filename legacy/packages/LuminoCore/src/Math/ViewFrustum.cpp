
#include <LuminoCore/Math/Matrix.hpp>
#include <LuminoCore/Math/Geometries.hpp>
#include <LuminoCore/Math/ViewFrustum.hpp>

namespace ln {

//==============================================================================
// ViewFrustum

ViewFrustum::ViewFrustum()
{
}

ViewFrustum::ViewFrustum(const Matrix& viewProjMatrix)
{
    setViewProjMatrix(viewProjMatrix);
}

void ViewFrustum::setViewProjMatrix(const Matrix& mat)
{
    m_planes[static_cast<int>(FrustumPlane::Near)] = Plane(-mat.m13, -mat.m23, -mat.m33, -mat.m43);
    m_planes[static_cast<int>(FrustumPlane::Far)] = Plane(-mat.m14 + mat.m13, -mat.m24 + mat.m23, -mat.m34 + mat.m33, -mat.m44 + mat.m43);
    m_planes[static_cast<int>(FrustumPlane::Left)] = Plane(-mat.m14 - mat.m11, -mat.m24 - mat.m21, -mat.m34 - mat.m31, -mat.m44 - mat.m41);
    m_planes[static_cast<int>(FrustumPlane::Right)] = Plane(-mat.m14 + mat.m11, -mat.m24 + mat.m21, -mat.m34 + mat.m31, -mat.m44 + mat.m41);
    m_planes[static_cast<int>(FrustumPlane::Top)] = Plane(-mat.m14 + mat.m12, -mat.m24 + mat.m22, -mat.m34 + mat.m32, -mat.m44 + mat.m42);
    m_planes[static_cast<int>(FrustumPlane::Bottom)] = Plane(-mat.m14 - mat.m12, -mat.m24 - mat.m22, -mat.m34 - mat.m32, -mat.m44 - mat.m42);

    for (int i = 0; i < FrustumPlane_Max; ++i) {
        m_planes[i].normalize();
    }

    m_viewProjMatrix = mat;
}

bool ViewFrustum::intersects(const Vector3& point) const
{
    for (int i = 0; i < FrustumPlane_Max; ++i) {
        if (!m_planes[i].checkInsideLower(point)) {
            return false;
        }
    }
    return true;
}

bool ViewFrustum::intersects(const Vector3& center, float radius) const
{
    for (int i = 0; i < FrustumPlane_Max; ++i) {
        if (!m_planes[i].checkInside(center, radius)) {
            return false;
        }
    }
    return true;
}

IntersectResult ViewFrustum::intersects(const Box& box) const
{
    IntersectResult result = IntersectResult::Inside;
    auto mins = box.minPoint();
    auto maxs = box.maxPoint();
    Vector3 vmin, vmax;

    for (int i = 0; i < FrustumPlane_Max; i++) {
        Vector3 normal = m_planes[i].normal;

        // X axis
        if (m_planes[i].normal.x > 0) {
            vmin.x = mins.x;
            vmax.x = maxs.x;
        } else {
            vmin.x = maxs.x;
            vmax.x = mins.x;
        }
        // Y axis
        if (m_planes[i].normal.y > 0) {
            vmin.y = mins.y;
            vmax.y = maxs.y;
        } else {
            vmin.y = maxs.y;
            vmax.y = mins.y;
        }
        // Z axis
        if (m_planes[i].normal.z > 0) {
            vmin.z = mins.z;
            vmax.z = maxs.z;
        } else {
            vmin.z = maxs.z;
            vmax.z = mins.z;
        }

        if (m_planes[i].getDistanceToPoint(vmin) > 0) {
            return IntersectResult::Outside;
        }
        if (m_planes[i].getDistanceToPoint(vmax) >= 0) {
            result = IntersectResult::Intersect;
        }
    }

    return result;
}

void ViewFrustum::getCornerPoints(Vector3* points) const
{
    if (points == nullptr)
        return;

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
    Matrix inv = Matrix::makeInverse(m_viewProjMatrix);
    for (int i = 0; i < 8; ++i) {
        points[i].transformCoord(inv);
    }
}

} // namespace ln
