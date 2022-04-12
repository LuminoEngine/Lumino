
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderView.hpp>
#include "RenderingPipeline.hpp"
#include "RenderingManager.hpp"
#include "RenderStage.hpp"
#include "SceneRenderer.hpp"

namespace ln {

//==============================================================================
// RenderView

/*
 * 必要性について
 * ----------
 * Camera = RenderView じゃないの？
 *
 * そうなっているのは Unity くらいかも。 Urho3D や Xenko では分かれている。(Camera,View や CameraComponent,RenderView)
 * UE4 は SceneCapture2D がこれに相当するが、WorldPointToScreenPoint などはある程度自分で書く必要があるようだ
 * https://usagi.hatenablog.jp/entry/2018/03/01/131939
 * http://miyahuji111.hatenablog.com/entry/2016/10/17/193434
 *
 * デバッグ用のビューを実装するときは Camera と RenderView は分かれていると便利。
 * だけど、実際にゲームをつくというときに、ユーザーが RenderView の機能をフルに使うかは微妙。
 *
 * 後はユーザーに公開するべきかってところだけど、ほかのエンジンたちは
 * - Unity: Screen.Width
 * - UE4: GEngine->GameViewport->GetViewportSize(Result);
 * みたいに、Camera ではないインターフェイスからビューサイズをとったりする。
 * ひとまずはこの一点だけの利用になりそうだけど、分ける方向で考えてみよう。
*/

LN_OBJECT_IMPLEMENT(RenderView, Object) {}

RenderView::RenderView()
	: m_clearMode(SceneClearMode::None)
	, m_backgroundColor(Color::White)
{
}

RenderView::~RenderView()
{
}

void RenderView::init()
{
	Object::init();
    m_manager = detail::RenderingManager::instance();
}

void RenderView::makeViewProjections(const detail::CameraInfo& base, float dpiScale)
{
	auto* info = &m_viewProjections[static_cast<int>(RenderPart::Geometry)];
	*info = base;

	info = &m_viewProjections[static_cast<int>(RenderPart::Gizmo)];
	*info = base;

	info = &m_viewProjections[static_cast<int>(RenderPart::PostEffect)];
	info->makeUnproject(base.viewPixelSize);

//	info = &m_viewProjections[static_cast<int>(detail::ProjectionKind::Physical2D)];
//	info->viewPixelSize = base.viewPixelSize;
//	info->viewPosition = Vector3::Zero;
//	info->viewDirection = Vector3::UnitZ;
//#ifdef LN_COORD_RH
//	info->viewMatrix = Matrix::makeLookAtRH(Vector3::Zero, -Vector3::UnitZ, Vector3::UnitY);
//	info->projMatrix = Matrix::makePerspective2DRH(base.viewPixelSize.width, base.viewPixelSize.height, 0, 1000);
//#else
//	info->viewMatrix = Matrix::makeLookAtLH(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);
//	info->projMatrix = Matrix::makePerspective2DLH(base.viewPixelSize.width, base.viewPixelSize.height, 0, 1000);
//#endif
//	info->viewProjMatrix = info->viewMatrix * info->projMatrix;
//	info->viewFrustum = ViewFrustum(info->viewProjMatrix);
//	info->nearClip = 0;
//	info->farClip = 1000;

	info = &m_viewProjections[static_cast<int>(RenderPart::Gizmo2D)];
	info->viewPixelSize = base.viewPixelSize;
	info->viewPosition = Vector3::Zero;
	info->viewDirection = Vector3::UnitZ;
#ifdef LN_COORD_RH
	info->viewMatrix = Matrix::makeLookAtRH(Vector3::Zero, /*-*/Vector3::UnitZ, Vector3::UnitY);
	info->projMatrix = Matrix::makePerspective2DRH(base.viewPixelSize.width / dpiScale, base.viewPixelSize.height / dpiScale, 0, 1000);
#else
	info->viewMatrix = Matrix::makeLookAtLH(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);
	info->projMatrix = Matrix::makePerspective2DLH(base.viewPixelSize.width / dpiScale, base.viewPixelSize.height / dpiScale, 0, 1000);
#endif
	info->viewProjMatrix = info->viewMatrix * info->projMatrix;
	info->viewFrustum = ViewFrustum(info->viewProjMatrix);
	info->nearClip = 0;
	info->farClip = 1000;

	//Vector3 pos1 = Vector3::transformCoord(Vector3(0, 0, 0), info->viewProjMatrix);
	//Vector3 pos2 = Vector3::transformCoord(Vector3(0, 0, 1), info->viewProjMatrix);;
	//Vector3 pos3 = Vector3::transformCoord(Vector3(0, 0, -1), info->viewProjMatrix);
	//printf("");
}

Vector3 RenderView::transformProjection(const Vector3& pos, RenderPart from, RenderPart to) const
{
	const auto fromView = m_viewProjections[static_cast<int>(from)];
	const auto clipPos = Vector3::transformCoord(pos, fromView.viewProjMatrix);

	const auto toView = m_viewProjections[static_cast<int>(to)];
	const auto inv = Matrix::makeInverse(toView.viewProjMatrix);
	return Vector3::transformCoord(clipPos, inv);
}

Ref<EventConnection> RenderView::connectOnUIEvent(Ref<UIGeneralEventHandler> handler)
{
    return m_onUIEvent.connect(handler);
}

RenderTargetTexture* RenderView::gbuffer(GBuffer kind) const
{
	if (!m_sceneRenderingPipeline) return nullptr;

	switch (kind)
	{
	case ln::GBuffer::ViewNormalMap:
		return m_sceneRenderingPipeline->viweNormalAndDepthBuffer();
	case ln::GBuffer::ViewDepthMap:
		return m_sceneRenderingPipeline->viweDepthBuffer();
	case ln::GBuffer::ViewMaterialMap:
		return m_sceneRenderingPipeline->materialBuffer();
	default:
		LN_UNREACHABLE();
		return nullptr;
	}
}

void RenderView::updateFrame(float elapsedSeconds)
{
	onUpdateFrame(elapsedSeconds);
}

void RenderView::updateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* parentFinalStyle)
{
	onUpdateUIStyle(styleContext, parentFinalStyle);
}

void RenderView::updateUILayout(UILayoutContext* layoutContext)
{
	// TODO: こんなのでいいのか要チェック
	onUpdateUILayout(layoutContext);
}

void RenderView::onUpdateFrame(float elapsedSeconds)
{
}

void RenderView::onUpdateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
}

void RenderView::onUpdateUILayout(UILayoutContext* layoutContext)
{
}

UIElement* RenderView::onLookupMouseHoverElement(const Point& framewindowClientPoint)
{
    return nullptr;
}

void RenderView::onRoutedEvent(UIEventArgs* e)
{
    m_onUIEvent.raise(e);
}

void RenderView::setActualSize(const Size& size) 
{
	m_actualSize = size;
}
//void RenderView::render(GraphicsCommandList* graphicsContext, const FrameBuffer& frameBuffer, detail::SceneRenderer* sceneRenderer)
//{
//	m_renderingFrameBufferSize = SizeI(frameBuffer.renderTarget[0]->width(), frameBuffer.renderTarget[0]->height());
//
//	sceneRenderer->render(graphicsContext, this, frameBuffer);
//
//	// 誤用防止
//	m_renderingFrameBufferSize = SizeI();
//}

} // namespace ln

