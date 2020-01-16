﻿
#include "Internal.hpp"
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
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
    : m_target(nullptr)
    , m_transformType(TransformControlsType::Translation)
    , m_displayScale(1.25f)
    , m_dragging(false)
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
    m_zColorMaterial->setColor(Color::Blue);

    m_activeColorMaterial = makeObject<Material>();
    m_activeColorMaterial->setColor(Color::Yellow);
}

void TransformControls::setTarget(WorldObject* value)
{
    m_target = value;
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
    if (!enabled()) return;

    context->pushState();
    context->setBaseTransfrom(m_gizmoTransform);
    context->setDepthTestEnabled(false);
    context->setDepthWriteEnabled(false);

    context->setMaterial((m_operationType == OperationType::X) ? m_activeColorMaterial : m_xColorMaterial);
    context->drawLine(Vector3(0, 0, 0), Color::White, Vector3(1, 0, 0), Color::White);

    context->setMaterial((m_operationType == OperationType::Y) ? m_activeColorMaterial : m_yColorMaterial);
	context->drawLine(Vector3(0, 0, 0), Color::White, Vector3(0, 1, 0), Color::White);

    context->setMaterial((m_operationType == OperationType::Z) ? m_activeColorMaterial : m_zColorMaterial);
	context->drawLine(Vector3(0, 0, 0), Color::White, Vector3(0, 0, 1), Color::White);



	//const float i1 = RotationRingInner;
	//const float o1 = RotationRingOuter;
	//Color c;
	//// Z
	//c = (m_operationType == OperationType::Z) ? Color::White : Color(0, 0, 1, BaseOpacity);
	//context->drawArc(0, Math::PI2, i1, o1, 32, c, Matrix::makeRotationX(Math::PIDiv2), m_tmat);

    context->popState();
}

void TransformControls::onRoutedEvent(UIEventArgs* e)
{
    if (!enabled()) return;

    if (e->type() == UIEvents::MouseDownEvent) {
        e->handled = handleMouseDown(static_cast<UIMouseEventArgs*>(e)->getPosition());
    }
    else if (e->type() == UIEvents::MouseUpEvent) {
        e->handled = handleMouseUp(static_cast<UIMouseEventArgs*>(e)->getPosition());
    }
    else if (e->type() == UIEvents::MouseMoveEvent) {
        e->handled = handleMouseMove(static_cast<UIMouseEventArgs*>(e)->getPosition());
        //auto me = static_cast<UIMouseEventArgs*>(e);
        //auto pt = me->getPosition();
        //m_operationType = getDirectionOperationType(pt.x, pt.y, nullptr);
    }
}

bool TransformControls::handleMouseDown(const Point& point)
{
    if (!m_dragging)
    {
        m_operationType = getDirectionOperationType(point.x, point.y, &m_draggingLocalPlane);
        if (m_operationType != OperationType::None)
        {
            m_draggingLocalPlane.intersects(makeLocalRay(point.x, point.y), &m_draggingStartLocalPosition);
            m_draggingStartGizmoTransform = m_gizmoTransform;
            m_draggingStartViewPixelPoint = point;
            m_dragging = true;
            startEditing();
            return true;
        }
    }
    return false;
}

bool TransformControls::handleMouseUp(const Point& point)
{
    if (m_dragging)
    {
        submitEditing();

        m_operationType = getDirectionOperationType(point.x, point.y, nullptr);

        return true;
    }
    return false;
}

bool TransformControls::handleMouseMove(const Point& point)
{
    if (m_dragging)
    {
        Vector3 localOffaet;
        m_draggingLocalPlane.intersects(makeLocalRay(point.x, point.y), &localOffaet);
        localOffaet -= m_draggingStartLocalPosition;

        localOffaet *= m_screenFactor;

        switch (m_transformType)
        {
            case TransformControlsType::Translation:
            {
                switch (m_operationType)
                {
                case OperationType::X:
                    localOffaet = Vector3(localOffaet.x, 0, 0);
                    break;
                case OperationType::Y:
                    localOffaet = Vector3(0, localOffaet.y, 0);
                    break;
                case OperationType::Z:
                    localOffaet = Vector3(0, 0, localOffaet.z);
                    break;
                case OperationType::XY:
                    localOffaet = Vector3(localOffaet.x, localOffaet.y, 0);
                    break;
                case OperationType::XZ:
                    localOffaet = Vector3(localOffaet.x, 0, localOffaet.z);
                    break;
                case OperationType::YZ:
                    localOffaet = Vector3(0, localOffaet.y, localOffaet.z);
                    break;
                case OperationType::XYZ:
                    break;
                }

                m_gizmoTransform = m_draggingStartGizmoTransform * Matrix::makeTranslation(localOffaet);
                m_targetTransform.translation = m_targetInitialTransform.translation + localOffaet;
                break;
            }
            case TransformControlsType::Rotation:
            {
                Matrix rot;
                switch (m_operationType)
                {
                case OperationType::X:
                    rot.rotateAxis(m_draggingLocalPlane.normal, -atan2(localOffaet.y, localOffaet.z));
                    break;
                case OperationType::Y:
                    rot.rotateAxis(m_draggingLocalPlane.normal, -atan2(localOffaet.z, localOffaet.x));
                    break;
                case OperationType::Z:
                    rot.rotateAxis(m_draggingLocalPlane.normal, atan2(localOffaet.y, localOffaet.x));
                    break;
                case OperationType::XYZ:
                {
                    Ray ray1 = makeLocalRay(point.x + 1, point.y);
                    Ray ray2 = makeLocalRay(point.x, point.y + 1);
                    Vector3 lp1, lp2;
                    m_draggingLocalPlane.intersects(ray1, &lp1);
                    m_draggingLocalPlane.intersects(ray2, &lp2);
                    lp1 -= m_draggingStartLocalPosition;
                    lp2 -= m_draggingStartLocalPosition;
                    Vector3 raxis = Vector3::normalize(lp1 - localOffaet);
                    Vector3 taxis = Vector3::normalize(lp2 - localOffaet);
                    rot.rotateAxis(raxis, (point.y - m_draggingStartViewPixelPoint.y) * -0.05);
                    rot.rotateAxis(taxis, (point.x - m_draggingStartViewPixelPoint.x) * 0.05);
                    break;
                }
                case OperationType::ViewZ:
                    rot.rotateAxis(m_draggingLocalPlane.normal, atan2(localOffaet.y, localOffaet.x));
                    break;
                }

                m_gizmoTransform = rot * m_draggingStartGizmoTransform;
                m_targetTransform.rotation = m_targetInitialTransform.rotation * Quaternion::makeFromRotationMatrix(rot);
                break;
            }
            case TransformControlsType::Scaling:
            {
                float s = Vector3::dot(localOffaet, Vector3::Ones);
                switch (m_operationType)
                {
                case OperationType::X:
                    localOffaet = Vector3(s, 0, 0);
                    break;
                case OperationType::Y:
                    localOffaet = Vector3(0, s, 0);
                    break;
                case OperationType::Z:
                    localOffaet = Vector3(0, 0, s);
                    break;
                case OperationType::XY:
                    localOffaet = Vector3(s, s, 0);
                    break;
                case OperationType::XZ:
                    localOffaet = Vector3(s, 0, s);
                    break;
                case OperationType::YZ:
                    localOffaet = Vector3(0, s, s);
                    break;
                case OperationType::XYZ:
                    localOffaet = Vector3(s, s, s);
                    break;
                }

                //localOffaet += Vector3::Ones;
                //m_gizmoTransform = Matrix::makeScaling(localOffaet) * m_draggingStartGizmoTransform;
                m_targetTransform.scale = m_targetInitialTransform.scale + localOffaet;
                break;
            }
        }

        onTargetTransformChanged();
        return true;
    }
    else
    {
        m_operationType = getDirectionOperationType(point.x, point.y, nullptr);
    }
    return false;
}

void TransformControls::startEditing()
{
    m_targetTransform.translation = m_target->transform()->position();
    m_targetTransform.rotation = m_target->transform()->rotation();
    m_targetTransform.scale = m_target->transform()->scale();
    m_targetInitialTransform = m_targetTransform;
}

void TransformControls::onTargetTransformChanged()
{
    m_target->setPosition(m_targetTransform.translation);
    m_target->setRotation(m_targetTransform.rotation);
    m_target->setScale(m_targetTransform.scale);
    //m_onTargetTransformChanged.raise(this);
}

void TransformControls::submitEditing()
{
    m_targetInitialTransform = m_targetTransform;

    // 拡大・回転をリセット
    m_gizmoTransform = Matrix::makeTranslation(m_gizmoTransform.position());
    m_gizmoInitialTransform = m_gizmoTransform;

    m_dragging = false;

    //m_onSubmitEditing.raise(this);
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
        ray.direction.mutatingNormalize();
    }

    return ray;
}

} // namespace ln

