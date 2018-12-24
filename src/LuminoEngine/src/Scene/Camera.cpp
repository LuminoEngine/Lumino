﻿
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

Vector3 Camera::worldToViewportPoint(const Vector3& position) const
{
	const Size& size = m_ownerRenderView->actualPixelSize();
	return Vector3::project(position, m_component->getViewProjectionMatrix(), 0.0f, 0.0f, size.width, size.height, m_component->getNearClip(), m_component->getFarClip());
}

Vector3 Camera::viewportToWorldPoint(const Vector3& position) const
{
	const Size& size = m_ownerRenderView->actualPixelSize();
    float nearClip = m_component->getNearClip();
    float farClip = m_component->getFarClip();
	Vector3 v;
	v.x = (((position.x - 0) / size.width) * 2.0f) - 1.0f;
	v.y = -((((position.y - 0) / size.height) * 2.0f) - 1.0f);
	v.z = (position.z - nearClip) / (farClip - nearClip);
	return Vector3::transformCoord(v, m_component->getViewProjectionMatrixInverse());
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

