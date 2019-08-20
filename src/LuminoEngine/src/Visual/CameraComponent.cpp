
#include "Internal.hpp"
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>   // TODO: WorldObjectTransform の定義を Component.hpp にもってくればいらなくなる
#include <LuminoEngine/Visual/CameraComponent.hpp>

namespace ln {

 
//==============================================================================
// CameraComponent
/*
 * RenderView の参照を持っておく必要はある？
 * ----------
 * ProjectionMatrix を求めるためには ViewSize (アスペクト比) が必須となる。
 * そのため親 RenderView (m_ownerRenderView) は CameraComponent が持っておく。
 *
 * また、できれば RenderView と Camera は 1:1 にしておきたい。
 * そうしておかないと、WorldObject の onUpdate とかで camera->projectionMatrix() を呼び出したとき、
 * どの RenderView をベースに計算されたものか、ぱっと見で非常にわかりづらい。
 *
 * 今後内部的にデバッグなどの用途で n:1 にしたくなったら、RenderViewPoint を共有しよう。
 */

CameraComponent::CameraComponent()
    : VisualComponent()
    , m_projectionMode(ProjectionMode::Perspective)
    , m_upDirection(Vector3::UnitY)
    , m_fovY(Math::PI / 3.0f)	// Unity based.
    , m_aspect(0.0f)
    , m_nearClip(0.3f)			// Unity based.
    , m_farClip(1000.0f)
    , m_orthographicSize(0.0f, 10.0f)	// Unity based. ※ Unity は縦幅の "半分" だが、ちょっとイメージしづらいので Lumino では全体
    , m_ownerRenderView(nullptr)
//, m_zSortDistanceBase(ZSortDistanceBase::CameraDistance)
//, m_cameraBehavior(nullptr)
{
	// ※ 2D では m_nearClip を0より大きくしたり、Z位置をマイナスにすると何も見えなくなるので注意。 
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::init()
{
    VisualComponent::init();
}

//void CameraComponent::setCameraBehavior(CameraBehavior* behavior)
//{
//	if (m_cameraBehavior != nullptr) {
//		m_cameraBehavior->setTargetCamera(nullptr);
//	}
//	LN_REFOBJ_SET(m_cameraBehavior, behavior);
//	m_cameraBehavior->setTargetCamera(this);
//}


//void CameraComponent::onUpdate(float elapsedSeconds)
//{
//
//}

void CameraComponent::updateMatrices()
{
    // TODO: 1フレームに何回か呼ばれる。更新不要ならしないようにしておく。
    // ・Cameraオブジェクトとしての onUpdate
    // ・１度も update されない状態での描画に備える render

    if (m_ownerRenderView) {
        const Size& viewSize = m_ownerRenderView->actualSize();
        if (LN_REQUIRE(!Math::isNaN(viewSize.width) && !Math::isNaN(viewSize.height))) return;
        if (LN_REQUIRE(viewSize.width > 0 && viewSize.height > 0)) return;

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
            float width = m_orthographicSize.width;
            float height = m_orthographicSize.height;
            if (Math::nearEqual(width, 0.0f)) {
                width = m_orthographicSize.height * (viewSize.width / viewSize.height);
            }
            if (Math::nearEqual(height, 0.0f)) {
                height = m_orthographicSize.width * (viewSize.height / viewSize.width);
            }

            m_projMatrix = Matrix::makeOrthoLH(width, height, m_nearClip, m_farClip);
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
