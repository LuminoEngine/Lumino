#include <LuminoCore/Graphics/Camera.hpp>
#include <cmath>

namespace ln {

Camera::Camera() {
    setLookAt({0, 0, 5}, Vector3::zero());
    setPerspective(1.0472f, 16.0f / 9.0f, 0.1f, 1000.0f);
}

void Camera::setLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
    m_viewMatrix = Matrix4x4::lookAtRH(eye, target, up);
    m_is2D = false;
}

void Camera::setOrbit(const Vector3& target, float distance, float yaw, float pitch) {
    float cp = std::cos(pitch);
    Vector3 eye = {
        target.x + distance * cp * std::sin(yaw),
        target.y + distance * std::sin(pitch),
        target.z + distance * cp * std::cos(yaw),
    };
    m_viewMatrix = Matrix4x4::lookAtRH(eye, target, Vector3::unitY());
    m_is2D = false;
}

void Camera::setPerspective(float fovY, float aspect, float nearClip, float farClip) {
    m_projMatrix = Matrix4x4::perspectiveRH(fovY, aspect, nearClip, farClip);
    m_is2D = false;
}

void Camera::setOrthographic(float width, float height, float nearClip, float farClip) {
    float hw = width * 0.5f;
    float hh = height * 0.5f;
    m_projMatrix = Matrix4x4::ortho(-hw, hw, -hh, hh, nearClip, farClip);
    m_is2D = false;
}

void Camera::setOrthographic2D(float width, float height, float nearClip, float farClip) {
    m_projMatrix = Matrix4x4::ortho(0.0f, width, height, 0.0f, nearClip, farClip);
    m_viewMatrix = Matrix4x4::identity();
    m_is2D = true;
}

Matrix4x4 Camera::viewProjectionMatrix() const {
    return m_projMatrix * m_viewMatrix;
}

} // namespace ln
