
#include "Internal.hpp"
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Scene/TransformControls.hpp>

namespace ln {

const float TransformControls::CenterBoxSize = 0.15f;
const float TransformControls::BarRadius = 0.05f;
const float TransformControls::BarLendth = 1.0f;
const float TransformControls::OnPlaneBoxSize = 0.4f;
const float TransformControls::RotationRingInner = 0.9f;
const float TransformControls::RotationRingOuter = 1.0f;
const float TransformControls::RotationViewZRingInner = 1.0f;
const float TransformControls::RotationViewZRingOuter = 1.1f;
const float TransformControls::BaseOpacity = 0.5f;

//==============================================================================
// Sprite

TransformControls::TransformControls()
    : m_transformType(TransformControlsType::Translation)
    , m_displayScale(1.25f)
{
}

void TransformControls::init()
{
	//WorldObject::init();
    Object::init();

    m_xColorMaterial = makeObject<Material>();
    m_xColorMaterial->setColor(Color::Red);

    m_yColorMaterial = makeObject<Material>();
    m_yColorMaterial->setColor(Color::Green);

    m_zColorMaterial = makeObject<Material>();
    m_zColorMaterial->setColor(Color::Black);

    m_activeColorMaterial = makeObject<Material>();
    m_activeColorMaterial->setColor(Color::Yellow);
}

void TransformControls::setViewInfo(const Vector3& viewPosition, const Matrix& view, const Matrix& proj, const Size& viewPixelSize)
{
    m_viewPosition = viewPosition;
    m_view = view;
    m_proj = proj;
    m_viewPixelSize = viewPixelSize;

    makeScreenFactor();
}

void TransformControls::onRender(RenderingContext* context)
{
    context->pushState();
    context->setDepthTestEnabled(false);
    context->setDepthWriteEnabled(false);

	//context->drawLine(Vector3(0, 0, 0), Color::Red, Vector3(1, 0, 0), (m_operationType == OperationType::X) ? Color::Yellow : Color::Red);

    context->setMaterial((m_operationType == OperationType::X) ? m_activeColorMaterial : m_xColorMaterial);
    context->drawLine(Vector3(0, 0, 0), Color::White, Vector3(1, 0, 0), Color::White);

    context->setMaterial((m_operationType == OperationType::Y) ? m_activeColorMaterial : m_yColorMaterial);
	context->drawLine(Vector3(0, 0, 0), Color::Green, Vector3(0, 1, 0), Color::Green);

    context->setMaterial((m_operationType == OperationType::Z) ? m_activeColorMaterial : m_zColorMaterial);
	context->drawLine(Vector3(0, 0, 0), Color::Blue, Vector3(0, 0, -1), Color::Blue);

    context->popState();
}

void TransformControls::onRoutedEvent(UIEventArgs* e)
{
    if (e->type() == UIEvents::MouseMoveEvent) {
        auto me = static_cast<UIMouseEventArgs*>(e);
        auto pt = me->getPosition();
        m_operationType = getDirectionOperationType(pt.x, pt.y, nullptr);
    }
}

void TransformControls::makeScreenFactor()
{
    Matrix viewproj = m_view * m_proj;
    Vector4 trf = Vector4(m_gizmoTransform.position(), 1.0f);
    trf = Vector4::transform(trf, viewproj);
    m_screenFactor = m_displayScale * 0.15f * trf.w;
}

TransformControls::OperationType TransformControls::getDirectionOperationType(float x, float y, Plane* outLocalPlane) const
{
    bool xz, xy, yz;
    Vector3 ptXZ, ptXY, ptYZ;
    Ray localViewRay;
    intersectsLocalAAPlanes(x, y, &xz, &ptXZ, &xy, &ptXY, &yz, &ptYZ, &localViewRay);

    if (xz && xy && yz &&
        (abs(ptXZ.x) <= CenterBoxSize && (ptXZ.z) <= CenterBoxSize) &&
        (abs(ptXY.x) <= CenterBoxSize && (ptXY.y) <= CenterBoxSize) &&
        (abs(ptYZ.y) <= CenterBoxSize && (ptYZ.z) <= CenterBoxSize))
    {
        if (outLocalPlane) *outLocalPlane = Plane(-localViewRay.direction);
        return OperationType::XYZ;
    }

    switch (m_transformType)
    {
        case TransformControlsType::Translation:
        case TransformControlsType::Scaling:
        {
            if (xz)
            {
                if (outLocalPlane) *outLocalPlane = Plane(Vector3::UnitY);
                // on axis line?
                if ((ptXZ.x >= 0) && (ptXZ.x <= 1) && (fabs(ptXZ.z) < 0.1f)) return OperationType::X;
                if ((ptXZ.z >= 0) && (ptXZ.z <= 1) && (fabs(ptXZ.x) < 0.1f)) return OperationType::Z;
                // on XY plane near the center ?
                if ((ptXZ.x < OnPlaneBoxSize) && (ptXZ.z < OnPlaneBoxSize) && (ptXZ.x > 0) && (ptXZ.z > 0))  return OperationType::XZ;
            }
            if (xy)
            {
                if (outLocalPlane) *outLocalPlane = Plane(Vector3::UnitZ);
                // on axis line?
                if ((ptXY.x >= 0) && (ptXY.x <= 1) && (fabs(ptXY.y) < 0.1f)) return OperationType::X;
                if ((ptXY.y >= 0) && (ptXY.y <= 1) && (fabs(ptXY.x) < 0.1f)) return OperationType::Y;
                // on XY plane near the center ?
                if ((ptXY.x < OnPlaneBoxSize) && (ptXY.y < OnPlaneBoxSize) && (ptXY.x > 0) && (ptXY.y > 0)) return OperationType::XY;
            }
            if (yz)
            {
                if (outLocalPlane) *outLocalPlane = Plane(Vector3::UnitX);
                // on axis line?
                if ((ptYZ.y >= 0) && (ptYZ.y <= 1) && (fabs(ptYZ.z) < 0.1f)) return OperationType::Y;
                if ((ptYZ.z >= 0) && (ptYZ.z <= 1) && (fabs(ptYZ.y) < 0.1f)) return OperationType::Z;
                // on YZ plane near the center ?
                if ((ptYZ.y < OnPlaneBoxSize) && (ptYZ.z < OnPlaneBoxSize) && (ptYZ.y > 0) && (ptYZ.z > 0)) return OperationType::YZ;
            }
            break;
        }
        case TransformControlsType::Rotation:
        {
            if (xz)
            {
                if (outLocalPlane) *outLocalPlane = Plane(Vector3::UnitY);
                float d = ptXZ.length();
                if (RotationRingInner <= d && d <= RotationRingOuter) return OperationType::Y;
            }
            if (xy)
            {
                if (outLocalPlane) *outLocalPlane = Plane(Vector3::UnitZ);
                float d = ptXY.length();
                if (RotationRingInner <= d && d <= RotationRingOuter) return OperationType::Z;
            }
            if (yz)
            {
                if (outLocalPlane) *outLocalPlane = Plane(Vector3::UnitX);
                float d = ptYZ.length();
                if (RotationRingInner <= d && d <= RotationRingOuter) return OperationType::X;
            }

            if (xz && xy && yz)
            {
                Plane localPlane(-localViewRay.direction);
                if (outLocalPlane) *outLocalPlane = localPlane;

                Vector3 pt;
                localPlane.intersects(localViewRay, &pt);
                float d = pt.length();
                if (RotationViewZRingInner <= d && d <= RotationViewZRingOuter) return OperationType::ViewZ;
            }
            break;
        }
    }

    return OperationType::None;
}

void TransformControls::intersectsLocalAAPlanes(float x, float y, bool* xz, Vector3* ptXZ, bool* xy, Vector3* ptXY, bool* yz, Vector3* ptYZ, Ray* localViewRay) const
{
    Ray ray = makeLocalRay(x, y);

    // 各軸平面と衝突判定
    *xz = Plane(Vector3::UnitY).intersects(ray, ptXZ);
    *xy = Plane(Vector3::UnitZ).intersects(ray, ptXY);
    *yz = Plane(Vector3::UnitX).intersects(ray, ptYZ);

    *localViewRay = ray;
}

Ray TransformControls::makeLocalRay(float x, float y) const
{
    Matrix gizmoMat;
    gizmoMat.scale(m_screenFactor);
    gizmoMat *= m_gizmoInitialTransform;

    Matrix viewproj = m_view * m_proj;

    Ray ray;
    ray.origin = m_viewPosition;
    ray.direction = Vector3::unproject(Vector3(x, y, 0), viewproj, 0, 0, m_viewPixelSize.width, m_viewPixelSize.height);
    ray.direction = ray.direction - ray.origin;

    // ray を gizmo のローカル空間へ変換する
    Matrix gizmoInv = Matrix::makeInverse(gizmoMat);
    {
        //ray.transformCoord(gizmoInv);
        ray.direction += ray.origin;
        ray.origin.transformCoord(gizmoInv);
        ray.direction.transformCoord(gizmoInv);
        ray.direction -= ray.origin;
        ray.direction.normalize();
    }

    return ray;
}

} // namespace ln

