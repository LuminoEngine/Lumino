
#include "Internal.hpp"
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
{
}

Camera::~Camera()
{
}

void Camera::initialize()
{
	WorldObject::initialize();
    m_component = newObject<CameraComponent>();
}
//
//CameraComponent* Camera::getCameraComponent() const
//{
//	return m_component;
//}

//void Camera::setCameraComponent(CameraComponent* component)
//{
//	m_component = component;
//	addComponent(m_component);
//}

} // namespace ln

