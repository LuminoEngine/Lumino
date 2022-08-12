
#include "Internal.hpp"
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/Scene/Camera.hpp>

namespace ln {
    
//==============================================================================
// Camera

LN_OBJECT_IMPLEMENT(Camera, WorldObject) {}

Ref<Camera> Camera::create()
{
    return makeObject_deprecated<Camera>();
}

Camera::Camera()
	: WorldObject()
	, m_component(nullptr)
    //, m_ownerRenderView(nullptr)
{
}

Camera::~Camera()
{
}

void Camera::init()
{
	WorldObject::init();
    m_component = makeObject_deprecated<CameraComponent>();
    addComponent(m_component);

	// Note: 以前のデフォルトだった -10.0f とかの方が数字的なキリはいいのだが、
	// チュートリアルや開発の初期段階で利用する XXXXMesh や Sprite のデフォルトサイズは 1.0 であり、-10.0f だとかなり小さく見えてしまう。
    setPosition(0, 0, 5.0f);
	lookAt(0, 0, 0);
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

Vector3 Camera::worldToScreenPoint(const Vector3& position) const
{
    WorldRenderView* view = renderView();
	const Size& size = view->actualSize();
	return Vector3::project(position, m_component->getViewProjectionMatrix(), 0.0f, 0.0f, size.width, size.height, m_component->getNearClip(), m_component->getFarClip());
}

Vector3 Camera::screenToWorldPoint(const Vector3& position) const
{
    WorldRenderView* view = renderView();
	const Size& size = view->actualSize();
    float nearClip = m_component->getNearClip();
    float farClip = m_component->getFarClip();
	Vector3 v;
	v.x = (((position.x - 0) / size.width) * 2.0f) - 1.0f;
	v.y = -((((position.y - 0) / size.height) * 2.0f) - 1.0f);
	v.z = (position.z - nearClip) / (farClip - nearClip);
	return Vector3::transformCoord(v, m_component->getViewProjectionMatrixInverse());
}

Ray Camera::screenToWorldRay(const Vector2& position) const
{
	float farClip = m_component->getFarClip();
	Vector3 pt = screenToWorldPoint(Vector3(position, farClip));
	Vector3 origin = m_component->worldObject()->worldMatrix().position();
	return Ray(
		origin,
		Vector3::normalize(pt - origin),
		Vector3::distance(pt, origin));
}

SceneClearMode Camera::clearMode() const
{
    WorldRenderView* view = renderView();
	if (view) {
		return view->clearMode();
	}
	return SceneClearMode::None;
}

void Camera::setClearMode(SceneClearMode value)
{
    WorldRenderView* view = renderView();
	if (view) {
        view->setClearMode(value);
	}
}

const Color& Camera::backgroundColor() const
{
    WorldRenderView* view = renderView();
	if (view) {
		return view->backgroundColor();
	}
	return Color::Transparency;
}

void Camera::setBackgroundColor(const Color& value)
{
    WorldRenderView* view = renderView();
	if (view) {
        view->setBackgroundColor(value);
	}
}

CameraComponent* Camera::cameraComponent() const
{
	return m_component;
}

WorldRenderView* Camera::renderView() const
{
    return (m_component) ? m_component->ownerRenderView() : nullptr;
}

void Camera::onUpdate(float elapsedSeconds)
{
    m_component->updateMatrices();
}

//void Camera::setCameraComponent(CameraComponent* component)
//{
//	m_component = component;
//	addComponent(m_component);
//}

} // namespace ln

