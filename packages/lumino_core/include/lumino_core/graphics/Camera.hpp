#pragma once
#include <lumino_base/Math.hpp>

namespace lumino {

enum class ProjectionMode {
    Perspective,
    Orthographic,
};

/// Camera for 3D rendering. Produces view and projection matrices.
class Camera {
public:
    Camera();

    void setPerspective(f32 fovY, f32 aspect, f32 nearClip, f32 farClip);
    void setOrthographic(f32 width, f32 height, f32 nearClip, f32 farClip);
    void setLookAt(const Vector3& eye, const Vector3& target, const Vector3& up = Vector3::unitY());

    /// Helper: orbit around target at given distance, yaw and pitch angles (radians).
    void setOrbit(const Vector3& target, f32 distance, f32 yaw, f32 pitch);

    const Vector3& position() const { return position_; }
    const Vector3& target() const { return target_; }

    Matrix4x4 viewMatrix() const;
    Matrix4x4 projectionMatrix() const;
    Matrix4x4 viewProjectionMatrix() const;

private:
    Vector3 position_ = {0, 0, 5};
    Vector3 target_ = Vector3::zero();
    Vector3 up_ = Vector3::unitY();

    ProjectionMode mode_ = ProjectionMode::Perspective;
    f32 fovY_ = 1.0472f; // ~60 deg
    f32 aspect_ = 16.0f / 9.0f;
    f32 orthoWidth_ = 10;
    f32 orthoHeight_ = 10;
    f32 nearClip_ = 0.1f;
    f32 farClip_ = 1000.0f;
};

} // namespace lumino
