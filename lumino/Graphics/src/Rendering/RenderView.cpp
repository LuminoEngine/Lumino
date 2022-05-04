#include "Internal.hpp"
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoGraphics/Rendering/RenderView.hpp>
#include <LuminoGraphics/Rendering/RenderingPipeline/StandardRenderingPipeline.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>
#include "RenderStage.hpp"
#include "SceneRenderer.hpp"

namespace ln {

//==============================================================================
// RenderViewPoint

void RenderViewPoint::resetUnproject(const Size& viewPixelSize_) {
    worldMatrix = Matrix::Identity;
    viewPixelSize = viewPixelSize_;
    viewPosition = Vector3::Zero;
    viewDirection = Vector3::UnitZ;
    viewMatrix = Matrix::Identity;
    projMatrix = Matrix::Identity;
    viewProjMatrix = Matrix::Identity;
    viewFrustum = ViewFrustum();
    fovY = 1.0f;
    nearClip = 0.0f;
    farClip = 1.0f;
    dpiScale = 1.0f;
}

void RenderViewPoint::resetPerspective(const Vector3& viewPos, const Vector3& viewDir, float fovY, const Size& size, float n, float f) {
    worldMatrix = Matrix::Identity; // TODO: Lookat
    viewPixelSize = size;
    viewPosition = viewPos;
    viewDirection = viewDir;
#ifdef LN_COORD_RH
    viewMatrix = Matrix::makeLookAtRH(viewPos, viewPos + viewDir, Vector3::UnitY);
    // projMatrix = Matrix::makeOrthoRH(size.width, size.height, n, f);//Matrix::makePerspectiveFovLH(fovY, size.width / size.height, n, f);
    projMatrix = Matrix::makePerspectiveFovRH(fovY, size.width / size.height, n, f);
#else
    viewMatrix = Matrix::makeLookAtLH(viewPos, viewPos + viewDir, Vector3::UnitY);
    // projMatrix = Matrix::makeOrthoLH(size.width, size.height, n, f);//Matrix::makePerspectiveFovLH(fovY, size.width / size.height, n, f);
    projMatrix = Matrix::makePerspectiveFovLH(fovY, size.width / size.height, n, f);
#endif
    viewProjMatrix = viewMatrix * projMatrix;
    viewFrustum = ViewFrustum(viewProjMatrix);
    // zSortDistanceBase = ZSortDistanceBase::CameraScreenDistance;
    this->fovY = fovY;
    nearClip = n;
    farClip = f;
    dpiScale = 1.0f;
}

void RenderViewPoint::copyFrom(const RenderViewPoint* other) {
    LN_DCHECK(other);
    worldMatrix = other->worldMatrix;
    viewPixelSize = other->viewPixelSize;
    viewPosition = other->viewPosition;
    viewDirection = other->viewDirection;
    viewMatrix = other->viewMatrix;
    projMatrix = other->projMatrix;
    viewProjMatrix = other->viewProjMatrix;
    viewFrustum = other->viewFrustum;
    fovY = other->fovY;
    nearClip = other->nearClip;
    farClip = other->farClip;
    dpiScale = other->dpiScale;
}

void RenderViewPoint::makeCameraInfo(detail::CameraInfo* cameraInfo) const {
    LN_DCHECK(cameraInfo);
    cameraInfo->viewPixelSize = viewPixelSize;
    cameraInfo->viewPosition = viewPosition;
    cameraInfo->viewDirection = viewDirection;
    cameraInfo->viewMatrix = viewMatrix;
    cameraInfo->projMatrix = projMatrix;
    cameraInfo->viewProjMatrix = viewProjMatrix;
    cameraInfo->viewFrustum = viewFrustum;
    cameraInfo->nearClip = nearClip;
    cameraInfo->farClip = farClip;
    cameraInfo->dpiScale = dpiScale;
}

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

LN_OBJECT_IMPLEMENT(RenderView, Object) {
}

RenderView::RenderView()
    : m_clearMode(SceneClearMode::None)
    , m_backgroundColor(Color::White)
    , m_currentCommandList(nullptr)
    , m_currentRenderTarget(nullptr)
    , m_viewProjections{}
    , m_builtinRenderTextures{} {
}

RenderView::~RenderView() {
}

void RenderView::init(RenderingContext* renderingContext) {
    Object::init();
    m_manager = detail::RenderingManager::instance();

    if (renderingContext) {
        m_renderingContext = renderingContext;
    }
    else {
        m_renderingContext = makeObject<RenderingContext>();
    }

    for (auto i = 0; i < m_viewProjections.size(); i++) {
        m_viewProjections[i] = makeObject<RenderViewPoint>();
    }

    m_viewPoint = makeObject<RenderViewPoint>();
}

void RenderView::makeViewProjections(const RenderViewPoint* base) {
    m_viewProjections[static_cast<int>(RenderPart::Geometry)]->copyFrom(base);
    m_viewProjections[static_cast<int>(RenderPart::Gizmo)]->copyFrom(base);
    m_viewProjections[static_cast<int>(RenderPart::PostEffect)]->resetUnproject(base->viewPixelSize);

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

    RenderViewPoint* info = m_viewProjections[static_cast<int>(RenderPart::Gizmo2D)];
    info->viewPixelSize = base->viewPixelSize;
    info->viewPosition = Vector3::Zero;
    info->viewDirection = Vector3::UnitZ;
#ifdef LN_COORD_RH
    info->viewMatrix = Matrix::makeLookAtRH(Vector3::Zero, /*-*/ Vector3::UnitZ, Vector3::UnitY);
    info->projMatrix = Matrix::makePerspective2DRH(base->viewPixelSize.width / base->dpiScale, base->viewPixelSize.height / base->dpiScale, 0, 1000);
#else
    info->viewMatrix = Matrix::makeLookAtLH(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);
    info->projMatrix = Matrix::makePerspective2DLH(base.viewPixelSize.width / dpiScale, base.viewPixelSize.height / dpiScale, 0, 1000);
#endif
    info->viewProjMatrix = info->viewMatrix * info->projMatrix;
    info->viewFrustum = ViewFrustum(info->viewProjMatrix);
    info->nearClip = 0;
    info->farClip = 1000;

    // Vector3 pos1 = Vector3::transformCoord(Vector3(0, 0, 0), info->viewProjMatrix);
    // Vector3 pos2 = Vector3::transformCoord(Vector3(0, 0, 1), info->viewProjMatrix);;
    // Vector3 pos3 = Vector3::transformCoord(Vector3(0, 0, -1), info->viewProjMatrix);
    // printf("");
}

Vector3 RenderView::transformProjection(const Vector3& pos, RenderPart from, RenderPart to) const {
    const RenderViewPoint* fromView = m_viewProjections[static_cast<int>(from)];
    const auto clipPos = Vector3::transformCoord(pos, fromView->viewProjMatrix);

    const RenderViewPoint* toView = m_viewProjections[static_cast<int>(to)];
    const auto inv = Matrix::makeInverse(toView->viewProjMatrix);
    return Vector3::transformCoord(clipPos, inv);
}

RenderingPipeline* RenderView::renderingPipeline() const {
    return m_renderingPipeline;
}

void RenderView::setRenderingPipeline(RenderingPipeline* value) {
    m_renderingPipeline = value;
}

void RenderView::clearBuiltinRenderTextures() {
    m_builtinRenderTextures = {};
}

RenderingContext* RenderView::getContext() const {
    return m_renderingContext;
}

void RenderView::updateFrame(float elapsedSeconds) {
    onUpdateFrame(elapsedSeconds);
}

void RenderView::renderPipeline(GraphicsCommandList* graphicsContext, /*RenderingContext* renderingContext,*/ RenderTargetTexture* renderTarget) {
    beginRenderPipeline(graphicsContext, renderTarget);
    endRenderPipeline();
}

RenderingContext* RenderView::beginRenderPipeline(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget) {
    if (LN_ASSERT(m_renderingPipeline)) return nullptr;
    if (LN_ASSERT(!m_currentCommandList)) return nullptr;
    m_currentCommandList = graphicsContext;
    m_currentRenderTarget = renderTarget;

    m_viewPoint->viewPixelSize = Size(renderTarget->width(), renderTarget->height());
    onUpdateViewPoint(m_viewPoint, renderTarget);
    makeViewProjections(m_viewPoint);

    clearBuiltinRenderTextures();

    m_renderingPipeline->prepare(this, renderTarget);

    m_renderingContext->resetForBeginRendering(m_viewPoint);
    onRender(graphicsContext, m_renderingContext, renderTarget);

    return m_renderingContext;
}

void RenderView::endRenderPipeline() {
    if (LN_ASSERT(m_currentCommandList)) return;
    m_renderingPipeline->render(m_currentCommandList, m_renderingContext, m_currentRenderTarget, this, nullptr);
    m_currentCommandList = nullptr;
    m_currentRenderTarget = nullptr;
}

//void RenderView::onUpdateViewPoint(RenderViewPoint* viewPoint) {
//}

void RenderView::onUpdateFrame(float elapsedSeconds) {
}

void RenderView::setActualSize(const Size& size) {
    m_actualSize = size;
}
// void RenderView::render(GraphicsCommandList* graphicsContext, const FrameBuffer& frameBuffer, detail::SceneRenderer* sceneRenderer)
//{
//	m_renderingFrameBufferSize = SizeI(frameBuffer.renderTarget[0]->width(), frameBuffer.renderTarget[0]->height());
//
//	sceneRenderer->render(graphicsContext, this, frameBuffer);
//
//	// 誤用防止
//	m_renderingFrameBufferSize = SizeI();
// }

} // namespace ln
