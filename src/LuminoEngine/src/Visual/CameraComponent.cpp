
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
	, m_zoom(1.0f)
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

	const Size& viewSize = m_ownerRenderView->actualSize();
	if (LN_REQUIRE(!Math::isNaN(viewSize.width) && !Math::isNaN(viewSize.height))) return;
	if (LN_REQUIRE(viewSize.width > 0 && viewSize.height > 0)) return;

    if (m_ownerRenderView) {

        const Matrix& worldMatrix = worldObject()->worldMatrix();

	    // 注視点
        Vector3 lookAt = Vector3::transformCoord(Vector3(0, 0, 1), worldMatrix);

	    // ビュー行列
#ifdef LN_COORD_RH
		m_viewMatrix = Matrix::makeLookAtRH(worldMatrix.position(), lookAt, m_upDirection);
#else
		m_viewMatrix = Matrix::makeLookAtLH(worldMatrix.position(), lookAt, m_upDirection);
#endif


		//auto f = Matrix::makeInverse(m_viewMatrix).front();

	    //if (m_reflectionPlane.Normal != Vector3::Zero)
	    //{
	    //	m_viewMatrix = Matrix::makeReflection(m_reflectionPlane) * m_viewMatrix;
	    //}

	    if (m_projectionMode == ProjectionMode::Perspective)
	    {
		    float aspect = m_aspect;
		    aspect = (m_aspect > 0.0f) ? m_aspect : viewSize.width / viewSize.height;
#ifdef LN_COORD_RH
			// プロジェクション行列の更新
			// https://sites.google.com/site/mmereference/home/Annotations-and-Semantics-of-the-parameter/2-1-geometry-translation
			m_projMatrix = Matrix::makePerspectiveFovRH(m_fovY, aspect, m_nearClip, m_farClip);
#else
			// プロジェクション行列の更新
			// https://sites.google.com/site/mmereference/home/Annotations-and-Semantics-of-the-parameter/2-1-geometry-translation
			m_projMatrix = Matrix::makePerspectiveFovLH(m_fovY, aspect, m_nearClip, m_farClip);
#endif
	    }
	    else
	    {
			Size orthoSize = actualOrthographicViewSize();
			//float scaleW = orthoSize.width / viewSize.width / m_zoom;
			//float scaleH = orthoSize.height / viewSize.height / m_zoom;
			float scaleW = m_zoom;
			float scaleH = m_zoom;
#ifdef LN_COORD_RH
			m_projMatrix = Matrix::makeOrthoRH(orthoSize.width * scaleW, orthoSize.height * scaleH, m_nearClip, m_farClip);
#else
			m_projMatrix = Matrix::makeOrthoLH(orthoSize.width * scaleW, orthoSize.height * scaleH, m_nearClip, m_farClip);
#endif
	    }

		const auto pt1 = Vector3::transformCoord(Vector3(0, 0, 0.5), m_viewMatrix);
		const auto pt2 = Vector3::transformCoord(pt1, m_projMatrix);

	    m_viewProjMatrix = m_viewMatrix * m_projMatrix;

	    // 正面方向
	    Vector3 d = lookAt - worldMatrix.position();
	    d.mutatingNormalize();
	    m_direction = Vector4(d, 0.0f);

        // ViewFrustum
	    m_viewFrustum = ViewFrustum(m_viewProjMatrix);

        // ShadowMap 作成などでよく使うのでここで計算しておく
	    m_viewProjMatrixI = Matrix::makeInverse(m_viewProjMatrix);
    }
}

Size CameraComponent::actualOrthographicViewSize() const
{
	const Size& viewSize = m_ownerRenderView->actualSize();
	if (LN_REQUIRE(!Math::isNaN(viewSize.width) && !Math::isNaN(viewSize.height))) return Size(1, 1);
	if (LN_REQUIRE(viewSize.width > 0 && viewSize.height > 0)) return Size(1, 1);

	float width = m_orthographicSize.width;
	float height = m_orthographicSize.height;
	if (Math::nearEqual(width, 0.0f)) {
		width = m_orthographicSize.height * (viewSize.width / viewSize.height);
	}
	if (Math::nearEqual(height, 0.0f)) {
		height = m_orthographicSize.width * (viewSize.height / viewSize.width);
	}

	return Size(width, height);
}


} // namespace ln
