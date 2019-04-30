
#pragma once
#include "Component.hpp"

namespace ln {
class UIEventArgs;

struct Spherical
{
    float radius = 1.0;
    float phi = 0;
    float theta = 0;

    void set(float radius_, float phi_, float theta_)
    {
        radius = radius_;
        phi = phi_;
        theta = theta_;
    }
};

class CameraOrbitControlComponent
	: public Component
{
public:

protected:
    virtual void onAttached(WorldObject* owner) override;

LN_CONSTRUCT_ACCESS:
	CameraOrbitControlComponent();
	virtual ~CameraOrbitControlComponent() = default;
	void init();

private:
    enum class State
    {
        None,
        Rotate,
        Dolly,
        Pan,
    };


    void handleUIEvent(UIEventArgs* e);
    void startRotate(const Vector2& mousePos);
    void startDolly(const Vector2& mousePos);
    void startPan(const Vector2& mousePos);
    void handleMouseMoveRotate(const Vector2& mousePos);
    void handleMouseMoveDolly(const Vector2& mousePos);
    void handleMouseMovePan(const Vector2& mousePos);
    void handleMouseWheel(int delta);
    void rotateLeft(float angle);
    void rotateUp(float angle);
    void dollyIn(float dollyScale);
    void dollyOut(float dollyScale);
    void pan(float deltaX, float deltaY);
    void panLeft(float distance, const Matrix& matrix);
    void panUp(float distance, const Matrix& matrix);
    bool update();
    float autoRotationAngle() const;
    float zoomScale() const;


    Camera* m_camera;
    State m_state;

    float m_rotateSpeed = 1.0f;
    float m_zoomSpeed = 1.0f;
    float m_panSpeed = 1.0f;
    bool m_screenSpacePanning = true;

    float m_minDistance = 0;
    float m_maxDistance = Math::Inf;
    float m_minPolarAngle = 0; // radians
    float m_maxPolarAngle = Math::PI; // radians
    float m_minAzimuthAngle = -Math::Inf; // radians
    float m_maxAzimuthAngle = Math::Inf; // radians

    Vector3 m_target;
    Spherical m_spherical;  // TODO: local?
    Spherical m_sphericalDelta;
    float m_scale = 1.0f;
    Vector3 m_panOffset;
    bool m_zoomChanged = false;

    Vector3 m_lastPosition;
    Quaternion m_lastQuaternion;


    Vector2 m_rotateStart;
    Vector2 m_rotateEnd;
    Vector2 m_dollyStart;
    Vector2 m_dollyEnd;
    Vector2 m_dollyDelta;   // TODO: local?
    Vector2 m_panStart;
    Vector2 m_panEnd;
    Vector2 m_panDelta;   // TODO: local?

    bool m_autoRotate = false;
    float m_autoRotateSpeed = 2.0;

    bool m_enableDamping = false;
    float m_dampingFactor = 0.25;

};

} // namespace ln
