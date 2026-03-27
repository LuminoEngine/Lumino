#pragma once
#include <lumino_base/math/Math.hpp>

namespace ln {

enum class ProjectionMode {
    Perspective,
    Orthographic,
};

/** Camera for 3D rendering. Produces view and projection matrices. */
class Camera {
public:
    Camera();

    void setPerspective(f32 fovY, f32 aspect, f32 nearClip, f32 farClip);
    void setOrthographic(f32 width, f32 height, f32 nearClip, f32 farClip);
    void setLookAt(const Vector3& eye, const Vector3& target, const Vector3& up = Vector3::unitY());

    /** Helper: orbit around target at given distance, yaw and pitch angles (radians). */
    void setOrbit(const Vector3& target, f32 distance, f32 yaw, f32 pitch);

    const Vector3& position() const { return m_position; }
    const Vector3& target() const { return m_target; }

    Matrix4x4 viewMatrix() const;
    Matrix4x4 projectionMatrix() const;
    Matrix4x4 viewProjectionMatrix() const;

private:
    Vector3 m_position = {0, 0, 5};
    Vector3 m_target = Vector3::zero();
    Vector3 m_up = Vector3::unitY();

    ProjectionMode m_mode = ProjectionMode::Perspective;
    f32 m_fovY = 1.0472f; // ~60 deg
    f32 m_aspect = 16.0f / 9.0f;
    f32 m_orthoWidth = 10;
    f32 m_orthoHeight = 10;
    f32 m_nearClip = 0.1f;
    f32 m_farClip = 1000.0f;
};

} // namespace ln
