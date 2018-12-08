
#include "Internal.hpp"
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/Scene/Camera.hpp>

namespace ln {
    
//==============================================================================
// Camera

Ref<Camera> Camera::create()
{
    return newObject<Camera>();
}

Camera::Camera()
	: WorldObject()
	, m_component(nullptr)
    , m_ownerRenderView(nullptr)
{
}

Camera::~Camera()
{
}

void Camera::initialize()
{
	WorldObject::initialize();
    m_component = newObject<CameraComponent>();
    addComponent(m_component);
    setPosition(0, 0, -10.0f);
}

const Matrix& Camera::viewMatrix() const
{
    return m_component->getViewMatrix();
}

const Matrix& Camera::projectionMatrix() const
{
    return m_component->getProjectionMatrix();
}

const Matrix& Camera::viewProjectionMatrix() const
{
    return m_component->getViewProjectionMatrix();
}

CameraComponent* Camera::cameraComponent() const
{
	return m_component;
}

void Camera::onUpdate(float elapsedSeconds)
{
    if (m_ownerRenderView) {
        m_component->updateMatrices(m_ownerRenderView->actualPixelSize());
    }
}

//void Camera::setCameraComponent(CameraComponent* component)
//{
//	m_component = component;
//	addComponent(m_component);
//}

} // namespace ln

