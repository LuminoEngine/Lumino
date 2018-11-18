
#include "Internal.hpp"
#include <LuminoEngine/Scene/WorldObject.hpp>   // TODO: WorldObjectTransform の定義を Component.hpp にもってくればいらなくなる
#include <LuminoEngine/Visual/CameraComponent.hpp>

namespace ln {

    
//==============================================================================
// CameraComponent

CameraComponent::CameraComponent()
	: VisualComponent()
	, m_projectionMode(ProjectionMode::Perspective)
	, m_upDirection(Vector3::UnitY)
	, m_fovY(Math::PI / 3.0f)	// Unity based.
	, m_aspect(0.0f)
	, m_nearClip(0.3f)			// Unity based.
	, m_farClip(1000.0f)
	, m_orthographicSize(5.0f)	// Unity based.
	//, m_zSortDistanceBase(ZSortDistanceBase::CameraDistance)
	//, m_cameraBehavior(nullptr)
{
	// ※ 2D では m_nearClip を0より大きくしたり、Z位置をマイナスにすると何も見えなくなるので注意。 
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::initialize()
{
    VisualComponent::initialize();
}

//void CameraComponent::setCameraBehavior(CameraBehavior* behavior)
//{
//	if (m_cameraBehavior != nullptr) {
//		m_cameraBehavior->setTargetCamera(nullptr);
//	}
//	LN_REFOBJ_SET(m_cameraBehavior, behavior);
//	m_cameraBehavior->setTargetCamera(this);
//}

//Vector3 CameraComponent::worldToViewportPoint(const Vector3& position) const
//{
//	const Size& size = m_ownerLayer->getViewSize();
//	return Vector3::project(position, m_viewProjMatrix, 0.0f, 0.0f, size.width, size.height, m_nearClip, m_farClip);
//}
//
//Vector3 CameraComponent::viewportToWorldPoint(const Vector3& position) const
//{
//	const Size& size = m_ownerLayer->getViewSize();
//	Vector3 v;
//	v.x = (((position.x - 0) / size.width) * 2.0f) - 1.0f;
//	v.y = -((((position.y - 0) / size.height) * 2.0f) - 1.0f);
//	v.z = (position.z - m_nearClip) / (m_farClip - m_nearClip);
//	return Vector3::transformCoord(v, m_viewProjMatrixI);
//}

//void CameraComponent::onUpdate(float elapsedSeconds)
//{
//
//}

void CameraComponent::updateMatrices(const Size& viewSize)
{
    const Matrix& worldMatrix = worldObject()->worldMatrix();

	// 注視点
    Vector3 lookAt = Vector3::transformCoord(Vector3(0, 0, 1), worldMatrix);

	// ビュー行列
	m_viewMatrix = Matrix::makeLookAtLH(worldMatrix.position(), lookAt, m_upDirection);

	//if (m_reflectionPlane.Normal != Vector3::Zero)
	//{
	//	m_viewMatrix = Matrix::makeReflection(m_reflectionPlane) * m_viewMatrix;
	//}

	if (m_projectionMode == ProjectionMode::Perspective)
	{
		float aspect = m_aspect;
		aspect = (m_aspect > 0.0f) ? m_aspect : viewSize.width / viewSize.height;

		// プロジェクション行列の更新
		// https://sites.google.com/site/mmereference/home/Annotations-and-Semantics-of-the-parameter/2-1-geometry-translation
		m_projMatrix = Matrix::makePerspectiveFovLH(m_fovY, aspect, m_nearClip, m_farClip);
	}
	else
	{
		float aspect = viewSize.width / viewSize.height;
		float width = m_orthographicSize * aspect;
		m_projMatrix = Matrix::makeOrthoLH(width, m_orthographicSize, m_nearClip, m_farClip);
	}

	m_viewProjMatrix = m_viewMatrix * m_projMatrix;

	// 正面方向
	Vector3 d = lookAt - worldMatrix.position();
	d.normalize();
	m_direction = Vector4(d, 0.0f);

    // ViewFrustum
	m_viewFrustum = ViewFrustum(m_viewProjMatrix);

    // ShadowMap 作成などでよく使うのでここで計算しておく
	m_viewProjMatrixI = Matrix::makeInverse(m_viewProjMatrix);
}

//void CameraComponent::onUIEvent(UIEventArgs* e)
//{
//	if (e->getType() == UIEvents::MouseDownEvent)
//	{
//		if (getCameraBehavior() != nullptr)
//		{
//			auto* me = static_cast<UIMouseEventArgs*>(e);
//			auto pos = PointI::fromFloatPoint(me->getPosition(me->sender));
//			getCameraBehavior()->injectMouseButtonDown(me->getMouseButtons(), pos.x, pos.y);
//		}
//	}
//	else if (e->getType() == UIEvents::MouseUpEvent)
//	{
//		if (getCameraBehavior() != nullptr)
//		{
//			auto* me = static_cast<UIMouseEventArgs*>(e);
//			auto pos = PointI::fromFloatPoint(me->getPosition(me->sender));
//			getCameraBehavior()->injectMouseButtonUp(me->getMouseButtons(), pos.x, pos.y);
//		}
//	}
//	else if (e->getType() == UIEvents::MouseMoveEvent)
//	{
//		if (getCameraBehavior() != nullptr)
//		{
//			auto* me = static_cast<UIMouseEventArgs*>(e);
//			auto pos = PointI::fromFloatPoint(me->getPosition(me->sender));
//			getCameraBehavior()->injectMouseMove(pos.x, pos.y);
//		}
//	}
//	else if (e->getType() == UIEvents::MouseWheelEvent)
//	{
//		if (getCameraBehavior() != nullptr)
//		{
//			if (getCameraBehavior() != nullptr) {
//				auto* me = static_cast<UIMouseWheelEventArgs*>(e);
//				getCameraBehavior()->injectMouseWheel(me->getDelta());
//			}
//		}
//	}
//	else
//	{
//		Component::onUIEvent(e);
//	}
//}


} // namespace ln
