#include <lumino_core/graphics/Camera.hpp>
#include <cmath>

namespace lumino {

Camera::Camera() = default;

void Camera::setPerspective(f32 fovY, f32 aspect, f32 nearClip, f32 farClip) {
    mode_ = ProjectionMode::Perspective;
    fovY_ = fovY;
    aspect_ = aspect;
    nearClip_ = nearClip;
    farClip_ = farClip;
}

void Camera::setOrthographic(f32 width, f32 height, f32 nearClip, f32 farClip) {
    mode_ = ProjectionMode::Orthographic;
    orthoWidth_ = width;
    orthoHeight_ = height;
    nearClip_ = nearClip;
    farClip_ = farClip;
}

void Camera::setLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
    position_ = eye;
    target_ = target;
    up_ = up;
}

void Camera::setOrbit(const Vector3& target, f32 distance, f32 yaw, f32 pitch) {
    target_ = target;
    f32 cp = std::cos(pitch);
    position_ = {
        target.x + distance * cp * std::sin(yaw),
        target.y + distance * std::sin(pitch),
        target.z + distance * cp * std::cos(yaw),
    };
    up_ = Vector3::unitY();
}

Matrix4x4 Camera::viewMatrix() const {
    return Matrix4x4::lookAt(position_, target_, up_);
}

Matrix4x4 Camera::projectionMatrix() const {
    if (mode_ == ProjectionMode::Perspective) {
        return Matrix4x4::perspective(fovY_, aspect_, nearClip_, farClip_);
    } else {
        f32 hw = orthoWidth_ * 0.5f;
        f32 hh = orthoHeight_ * 0.5f;
        return Matrix4x4::ortho(-hw, hw, -hh, hh, nearClip_, farClip_);
    }
}

Matrix4x4 Camera::viewProjectionMatrix() const {
    return projectionMatrix() * viewMatrix();
}

} // namespace lumino
