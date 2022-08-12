
#include "Internal.hpp"
#include <LuminoEngine/Visual/CameraComponent.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
#include <LuminoEngine/Scene/CameraOrbitControlComponent.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/UI/UIEvents.hpp>

namespace ln {

//==============================================================================
// CameraOrbitControlComponent
// https://github.com/mrdoob/three.js/blob/master/examples/js/controls/OrbitControls.js

LN_OBJECT_IMPLEMENT(CameraOrbitControlComponent, Component) {}

Ref<CameraOrbitControlComponent> CameraOrbitControlComponent::create()
{
	return makeObject_deprecated<CameraOrbitControlComponent>();
}

CameraOrbitControlComponent::CameraOrbitControlComponent()
    : m_camera(nullptr)
    , m_capturdElement(nullptr)
    , m_state(State::None)
	, m_orthoMinZoom(0.0001f)
	, m_orthoMaxZoom(std::numeric_limits<float>::infinity())
{
}

CameraOrbitControlComponent::~CameraOrbitControlComponent()
{
}

void CameraOrbitControlComponent::init()
{
    Component::init();
}

void CameraOrbitControlComponent::onAttached(WorldObject* owner)
{
    m_camera = dynamic_cast<Camera*>(owner);
    if (LN_REQUIRE(m_camera)) return;

    m_camera->renderView()->connectOnUIEvent(ln::bind(this, &CameraOrbitControlComponent::handleUIEvent));
}

void CameraOrbitControlComponent::handleUIEvent(UIEventArgs* e)
{
    if (e->handled) return;

    if (e->type() == UIEvents::MouseDownEvent)
    {
        auto me = static_cast<UIMouseEventArgs*>(e);
        switch (me->getMouseButtons())
        {
            case MouseButtons::Left:
				//startDolly(me->getPosition());
				//m_capturdElement = e->sender();
				//m_capturdElement->retainCapture();
				//e->handled = true;
                break;
            case MouseButtons::Right:
				startRotate(me->getPosition());
				m_capturdElement = e->sender();
				m_capturdElement->retainCapture();
				e->handled = true;
                break;
            case MouseButtons::Middle:
                startPan(me->getPosition());
                m_capturdElement = e->sender();
                m_capturdElement->retainCapture();
                e->handled = true;
                break;
        }
    }
    else if (e->type() == UIEvents::MouseMoveEvent)
    {
        auto me = static_cast<UIMouseEventArgs*>(e);
        e->handled = handleMouseMove(me->getPosition());
    }
    else if (e->type() == UIEvents::MouseUpEvent)
    {
        handleMouseUp();
    }
    else if (e->type() == UIEvents::MouseWheelEvent)
    {
        auto me = static_cast<UIMouseWheelEventArgs*>(e);
        handleMouseWheel(me->getDelta());
        e->handled = true;
    }
}

void CameraOrbitControlComponent::startRotate(const Vector2& mousePos)
{
    m_rotateStart = mousePos;
    m_state = State::Rotate;
}

void CameraOrbitControlComponent::startDolly(const Vector2& mousePos)
{
    m_dollyStart = mousePos;
    m_state = State::Dolly;
}

void CameraOrbitControlComponent::startPan(const Vector2& mousePos)
{
    m_panStart = mousePos;
    m_state = State::Pan;
}

bool CameraOrbitControlComponent::handleMouseMove(const Vector2& mousePos)
{
    switch (m_state)
    {
    case State::Rotate:
        handleMouseMoveRotate(mousePos);
        return true;
    case State::Dolly:
        handleMouseMoveDolly(mousePos);
        return true;
    case State::Pan:
        handleMouseMovePan(mousePos);
        return true;
    default:
        return false;
    }
}

void CameraOrbitControlComponent::handleMouseMoveRotate(const Vector2& mousePos)
{
    float clientHeight = m_camera->renderView()->actualSize().height;

    m_rotateEnd = mousePos;

    Vector2 delta = (m_rotateEnd - m_rotateStart) * m_rotateSpeed;

#ifdef LN_COORD_RH
    rotateLeft(-2 * Math::PI * delta.x / clientHeight);
#else
    rotateLeft(2 * Math::PI * delta.x / clientHeight);
#endif

    rotateUp(2 * Math::PI * delta.y / clientHeight);

    m_rotateStart = m_rotateEnd;

    update();
}

void CameraOrbitControlComponent::handleMouseMoveDolly(const Vector2& mousePos)
{
    m_dollyEnd = mousePos;

    m_dollyDelta = m_dollyEnd - m_dollyStart;

    if (m_dollyDelta.y > 0.0f) {
        dollyIn(zoomScale());
    }
    else if (m_dollyDelta.y < 0.0f) {
        dollyOut(zoomScale());
    }

    m_dollyStart = m_dollyEnd;

    update();
}

void CameraOrbitControlComponent::handleMouseMovePan(const Vector2& mousePos)
{
    m_panEnd = mousePos;

    m_panDelta = (m_panEnd - m_panStart) * m_panSpeed;

#ifdef LN_COORD_RH
    pan(-m_panDelta.x, m_panDelta.y);
#else
    pan(m_panDelta.x, m_panDelta.y);
#endif

    m_panStart = m_panEnd;

    update();
}

void CameraOrbitControlComponent::handleMouseUp()
{
    if (m_capturdElement) {
        m_capturdElement->releaseCapture();
        m_capturdElement = nullptr;
    }
    m_state = State::None;
}

void CameraOrbitControlComponent::handleMouseWheel(int delta)
{
    if (delta < 0) {
        dollyIn(zoomScale());
    }
    else if (delta > 0) {
        dollyOut(zoomScale());
    }

    update();
}

void CameraOrbitControlComponent::rotateLeft(float angle)
{
    m_sphericalDelta.theta += angle;
}

void CameraOrbitControlComponent::rotateUp(float angle)
{
    m_sphericalDelta.phi -= angle;
}

void CameraOrbitControlComponent::dollyIn(float dollyScale)
{
    ProjectionMode mode = m_camera->cameraComponent()->projectionMode();
    if (mode == ProjectionMode::Perspective) {
        m_scale *= dollyScale;
    }
    else if (mode == ProjectionMode::Orthographic) {
		m_camera->cameraComponent()->m_zoom = std::max(m_orthoMinZoom, std::min(m_orthoMaxZoom, m_camera->cameraComponent()->m_zoom * dollyScale));
		m_zoomChanged = true;
    }
    else {
        LN_UNREACHABLE();
    }
}

void CameraOrbitControlComponent::dollyOut(float dollyScale)
{
    ProjectionMode mode = m_camera->cameraComponent()->projectionMode();
    if (mode == ProjectionMode::Perspective) {
        m_scale /= dollyScale;
    }
    else if (mode == ProjectionMode::Orthographic) {
		m_camera->cameraComponent()->m_zoom = std::max(m_orthoMinZoom, std::min(m_orthoMaxZoom, m_camera->cameraComponent()->m_zoom / dollyScale));
		m_zoomChanged = true;
    }
    else {
        LN_UNREACHABLE();
    }
}

void CameraOrbitControlComponent::pan(float deltaX, float deltaY)
{
    Size clientSize = m_camera->renderView()->actualSize();
	if (clientSize.isSquashed()) return;

    Matrix matrix = m_camera->worldMatrix();

    ProjectionMode mode = m_camera->cameraComponent()->projectionMode();
    if (mode == ProjectionMode::Perspective) {

        // perspective
        Vector3 offset = m_camera->position() - m_target;
        float targetDistance = offset.length();

        // half of the fov is center to top of screen
        targetDistance *= std::tan((m_camera->fov() / 2.0f) * Math::PI / 180.0f);

        // we use only clientHeight here so aspect ratio does not distort speed
        panLeft(100.0f * deltaX * targetDistance / clientSize.height, matrix);
        panUp(100.0f * deltaY * targetDistance / clientSize.height, matrix);

    }
    else if (mode == ProjectionMode::Orthographic) {
		auto viewSize = m_camera->cameraComponent()->ownerRenderView()->actualSize();

		Size orthoSize = m_camera->cameraComponent()->actualOrthographicViewSize();
		float zoom = m_camera->cameraComponent()->m_zoom;

		panLeft(deltaX * (orthoSize.width / clientSize.width) * zoom, matrix);
		panUp(deltaY * (orthoSize.height / clientSize.height) * zoom, matrix);
        //panLeft(deltaX * orthoSize.width / zoom / clientSize.width, matrix);
        //panUp(deltaY * orthoSize.height / zoom / clientSize.height, matrix);

    }
    else {
        LN_UNREACHABLE();
    }
}

void CameraOrbitControlComponent::panLeft(float distance, const Matrix& matrix)
{
    m_panOffset += (matrix.right() * (-distance));
}

void CameraOrbitControlComponent::panUp(float distance, const Matrix& matrix)
{
    Vector3 v;

    if (m_screenSpacePanning) {
        v = matrix.up();
    }
    else {
        v = matrix.right();
        v = Vector3::cross(m_camera->transform()->up(), v);
    }

    m_panOffset += v * distance;
}

Quaternion Quaternion_setFromUnitVectors(const Vector3& vFrom, const Vector3& vTo)
{
    const float EPS = 0.000001;

    Vector3 v1;
    float r = Vector3::dot(vFrom, vTo) + 1;
    if (r < EPS) {
        r = 0.0f;
        if (std::abs(vFrom.x) > std::abs(vFrom.z)) {
            v1.set(-vFrom.y, vFrom.x, 0);
        }
        else {
            v1.set(0, -vFrom.z, vFrom.y);
        }
    }
    else {
        v1 = Vector3::cross(vFrom, vTo);
    }

    Quaternion q;
    q.x = v1.x;
    q.y = v1.y;
    q.z = v1.z;
    q.w = r;
    return  Quaternion::normalize(q);
};

Spherical Spherical_setFromCartesianCoords(float x, float y, float z)
{
    Spherical spherical;
    spherical.radius = std::sqrt(x * x + y * y + z * z);

    if (spherical.radius == 0.0f) {
        spherical.theta = 0.0f;
        spherical.phi = 0.0f;
    }
    else {
        spherical.theta = std::atan2(x, z);
        spherical.phi = std::acos(Math::clamp(y / spherical.radius, -1, 1));
    }

    return spherical;
}

Spherical Spherical_makeSafe(const Spherical& value)
{
    const float EPS = 0.000001;
    Spherical spherical = value;
    spherical.phi = std::max(EPS, std::min(Math::PI - EPS, spherical.phi));
    return spherical;
}

Vector3 Vector3_setFromSphericalCoords(float radius, float phi, float theta)
{
    float sinPhiRadius = std::sin(phi) * radius;
    return Vector3(
        sinPhiRadius * std::sin(theta),
        std::cos(phi) * radius,
        sinPhiRadius * std::cos(theta));

}

/*
    applyQuaternion: function ( q ) {

        var x = this.x, y = this.y, z = this.z;
        var qx = q.x, qy = q.y, qz = q.z, qw = q.w;

        // calculate quat * vector

        var ix = qw * x + qy * z - qz * y;
        var iy = qw * y + qz * x - qx * z;
        var iz = qw * z + qx * y - qy * x;
        var iw = - qx * x - qy * y - qz * z;

        // calculate result * inverse quat

        this.x = ix * qw + iw * - qx + iy * - qz - iz * - qy;
        this.y = iy * qw + iw * - qy + iz * - qx - ix * - qz;
        this.z = iz * qw + iw * - qz + ix * - qy - iy * - qx;

        return this;

    },
*/
bool CameraOrbitControlComponent::update()
{
    Quaternion quat = Quaternion_setFromUnitVectors(m_camera->transform()->up(), Vector3::UnitY);
    Quaternion quatInverse = Quaternion::makeInverse(quat);

    Vector3 position = m_camera->position();

    Vector3 offset = position - m_target;

    // rotate offset to "y-axis-is-up" space
    offset = Vector3::transform(offset, quat);

    // angle from z-axis around y-axis
    m_spherical = Spherical_setFromCartesianCoords(offset.x, offset.y, offset.z);

    if (m_autoRotate && m_state == State::None) {
        rotateLeft(autoRotationAngle());
    }

    m_spherical.theta += m_sphericalDelta.theta;
    m_spherical.phi += m_sphericalDelta.phi;

    // restrict theta to be between desired limits
    m_spherical.theta = std::max(m_minAzimuthAngle, std::min(m_maxAzimuthAngle, m_spherical.theta));

    // restrict phi to be between desired limits
    m_spherical.phi = std::max(m_minPolarAngle, std::min(m_maxPolarAngle, m_spherical.phi));

    m_spherical = Spherical_makeSafe(m_spherical);


    m_spherical.radius *= m_scale;

    // restrict radius to be between desired limits
    m_spherical.radius = std::max(m_minDistance, std::min(m_maxDistance, m_spherical.radius));

    // move target to panned location
    m_target += m_panOffset;

    offset = Vector3_setFromSphericalCoords(m_spherical.radius, m_spherical.phi, m_spherical.theta);

    // rotate offset back to "camera-up-vector-is-up" space
    //offset.applyQuaternion(quatInverse);
    offset = Vector3::transform(offset, quatInverse);

    //position = m_target + offset;
    m_camera->setPosition(m_target + offset);
    m_camera->lookAt(m_target);

    if (m_enableDamping) {
        m_sphericalDelta.theta *= (1.0f - m_dampingFactor);
        m_sphericalDelta.phi *= (1.0f - m_dampingFactor);
        m_panOffset *= (1.0f - m_dampingFactor);
    }
    else {
        m_sphericalDelta.set(0, 0, 0);
        m_panOffset.set(0, 0, 0);
    }

    m_scale = 1;

    // update condition is:
    // min(camera displacement, camera rotation in radians)^2 > EPS
    // using small-angle approximation cos(x/2) = 1 - x^2 / 8

    //if (m_zoomChanged ||
    //    lastPosition.distanceToSquared(scope.object.position) > EPS ||
    //    8 * (1 - lastQuaternion.dot(scope.object.quaternion)) > EPS) {

    //    scope.dispatchEvent(changeEvent);

    //    m_lastPosition = m_camera->position;
    //    m_lastQuaternion = m_camera->rotation();
    //    zoomChanged = false;

    //    return true;

    //}

    return false;
}

float CameraOrbitControlComponent::autoRotationAngle() const
{
    return 2.0f * Math::PI / 60.0f / 60.0f * m_autoRotateSpeed;
}

float CameraOrbitControlComponent::zoomScale() const
{
    return std::pow(0.95, m_zoomSpeed);
}

} // namespace ln

