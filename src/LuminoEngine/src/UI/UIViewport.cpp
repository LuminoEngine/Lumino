
#include "Internal.hpp"
//#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/UI/UIViewport.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "../ImageEffect/ImageEffectRenderer.hpp"
#include "UIManager.hpp"

namespace ln {

//==============================================================================
// UIViewport

UIViewport::UIViewport()
    : m_manager(detail::EngineDomain::uiManager())
	, m_placement(UIViewportPlacement::ViewBox)
{
}

UIViewport::~UIViewport()
{
}

void UIViewport::init()
{
	UIContainerElement::init();
	setHorizontalAlignment(HAlignment::Stretch);
	setVerticalAlignment(VAlignment::Stretch);

    //m_imageEffectRenderer = makeRef<detail::ImageEffectRenderer>();
    m_blitMaterial = makeObject<Material>();
	m_blitMaterial->setBlendMode(BlendMode::Normal);

	//auto shader = makeObject<Shader>(u"D:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/ToneImageEffect.fx");
	//m_blitMaterial->setShader(shader);

	//m_renderPass = makeObject<RenderPass>();
}

void UIViewport::onDispose(bool explicitDisposing)
{
	//if (m_primaryTarget) {
	//	RenderTargetTexture::releaseTemporary(m_primaryTarget);
	//	m_primaryTarget = nullptr;
	//}

    m_renderViews.clear();
	UIContainerElement::onDispose(explicitDisposing);
}

void UIViewport::addRenderView(RenderView* view)
{
    m_renderViews.add(view);
}

void UIViewport::removeRenderView(RenderView* view)
{
    m_renderViews.remove(view);
}

//void UIViewport::addImageEffect(ImageEffect* effect)
//{
//    m_imageEffectRenderer->addImageEffect(effect);
//}
//
//void UIViewport::removeImageEffect(ImageEffect* effect)
//{
//    m_imageEffectRenderer->removeImageEffect(effect);
//}

UIElement* UIViewport::lookupMouseHoverElement(const Point& frameClientPosition)
{
    for (auto& view : m_renderViews) {
        UIElement* e = view->onLookupMouseHoverElement(frameClientPosition);
        if (e) return e;
    }
    return UIContainerElement::lookupMouseHoverElement(frameClientPosition);
}

void UIViewport::onUpdateFrame(float elapsedTimer)
{
	for (auto& view : m_renderViews) {
		view->updateFrame(elapsedTimer);
	}

    //m_imageEffectRenderer->updateFrame(elapsedTimer);
}

void UIViewport::onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
	for (auto& view : m_renderViews) {
		view->updateUIStyle(styleContext, finalStyle);
	}
}

Size UIViewport::arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize)
{
	if (isViewBoxRenderTargetAutoResize()) {
		m_actualViewboxSize = finalSize;
	}
	else {
		m_actualViewboxSize = m_viewBoxSize;
	}

    // TODO: tmp
    for (auto& rv : m_renderViews) {
        rv->setActualSize(m_actualViewboxSize);
    }

    return UIContainerElement::arrangeOverride(layoutContext, finalSize);
}


void UIViewport::onUpdateLayout(UILayoutContext* layoutContext)
{
	for (auto& view : m_renderViews) {
		view->updateUILayout(layoutContext);
		// TODO: view box
		//view->setActualScreenOffset(Point(finalGlobalRect.x, finalGlobalRect.y));
	}
}

//void UIViewport::render(UIRenderingContext* context)
//{
//}

void UIViewport::onRender(UIRenderingContext* context)
{
	//return;

    GraphicsContext* graphicsContext = context->m_frameWindowRenderingGraphicsContext;
    //auto* renderTarget = graphicsContext->renderTarget(0);
	//Ref<RenderPass> oldRenderPass = graphicsContext->renderPass();

    // TODO: dp -> px 変換
    Size viewSize = actualSize();
	acquirePrimaryTarget(SizeI::fromFloatSize(m_actualViewboxSize));

	// このスコープ終端で RenderTargetTexture::releaseTemporary() するわけにはいかない。
	// この RenderTarget は context->drawImage() に乗ってこのスコープの外側でも使われるため、次回の描画までは再利用されないようにしたい。
	//if (m_primaryTarget) {
	//	RenderTargetTexture::releaseTemporary(m_primaryTarget);
	//}

	//m_primaryTarget = RenderTargetTexture::getTemporary(viewSize.width, viewSize.height, TextureFormat::RGBA8, false);

    //graphicsContext->setRenderTarget(0, primaryTarget);
	//m_renderPass->setRenderTarget(0, primaryTarget);
	//graphicsContext->beginRenderPass(m_renderPass);

	
	//context->clear(ClearFlags::All, Color::Blue);	// TODO: renderPass の clear でカバー

	//context->pushState();
    for (auto& view : m_renderViews) {
        view->render(graphicsContext, m_primaryTarget);
    }
    //m_imageEffectRenderer->render(context, m_primaryTarget);
	//context->popState();

    //context->blit(primaryTarget, renderTarget);

    //detail::TextureInternal::readData(m_primaryTarget, graphicsContext)->save(u"test.png");


	//auto tmp = Material::create(Texture2D::whiteTexture());
	//context->blit(tmp, m_primaryTarget);


	


	//context->setViewportRect(RectI(0, 0, SizeI::fromFloatSize(viewSize)));
	//context->setScissorRect(RectI(0, 0, SizeI::fromFloatSize(viewSize)));
	//

#if 1
	Matrix t;
	t.scale((viewSize.width * 0.5), -(viewSize.height * 0.5), 0);
	t.translate((viewSize.width * 0.5), (viewSize.height * 0.5), 0);
	context->setBaseTransfrom(t);
	context->setCullingMode(CullMode::None);
	context->setBlendMode(BlendMode::Normal);
	m_blitMaterial->setMainTexture(m_primaryTarget);
	context->blit(m_blitMaterial, nullptr, RenderPhaseClass::Geometry);
#else
	// TODO: ポストプロセスの結果を転送したいので、Sprite 描画では描画できない。
	// 現状、RenderPhaseClass::ImageEffect を使っている blit を利用する必要がある。
	// なお、現時点では blit は スクリーン全体への転送を想定しているため、View Proj Matrix を受け取らないようになっている。
	// その調整のため、ここでいろいろ変換行列を計算している。
	// 後々、UIVew 描画の RT キャッシュとかも考えているので、blitImage(Rect, Matrial) とかにまとめたい。
	auto* vp = context->viewPoint();
	Matrix t;
	t.scale(1.0f, -1.0f, 0);
	t.translate(1, 1, 0);
	t.scale((viewSize.width * 0.5),  (viewSize.height * 0.5), 0);
	t *= context->baseTransform();
	t *= vp->viewProjMatrix;

	if (isViewBoxAspectScaling()) {
		Matrix viewBoxMatrix;
		makeViewBoxTransform(SizeI::fromFloatSize(viewSize), SizeI(m_primaryTarget->width(), m_primaryTarget->height()), &viewBoxMatrix);
		t *= viewBoxMatrix;
	}

	context->setBaseTransfrom(t);
	context->setCullingMode(CullMode::None);
	context->setBlendMode(BlendMode::Normal);
    m_blitMaterial->setMainTexture(m_primaryTarget);
	context->blit(m_blitMaterial, nullptr, RenderPhaseClass::Geometry);
#endif

	//context->drawSolidRectangle(Rect(0, 0, 100, 100), Color::Blue);

    //RenderTargetTexture::releaseTemporary(primaryTarget);

    //graphicsContext->setRenderTarget(0, renderTarget);
	//graphicsContext->endRenderPass();

    // TODO: ViewBoxTransform

    //graphicsContext->resetState();
}

void UIViewport::onRoutedEvent(UIEventArgs* e)
{
    for (auto& view : m_renderViews)
    {
        view->onRoutedEvent(e);
		if (e->handled) {
			invalidateVisual();	// TODO: Editor Mode で、Scene 上を D&Dしたらカメラ動かしたいのでとりあえずここで処理してるが、無駄が多いかも
			return;
		}
    }

    UIContainerElement::onRoutedEvent(e);
}

void UIViewport::acquirePrimaryTarget(const SizeI& viewPixelSize)
{
	if (!m_primaryTarget || m_primaryTarget->width() != viewPixelSize.width || m_primaryTarget->height() != viewPixelSize.height) {
		m_primaryTarget = makeObject<RenderTargetTexture>(viewPixelSize.width, viewPixelSize.height, TextureFormat::RGBA8, false);
	}
}

void UIViewport::makeViewBoxTransform(const SizeI& dstSize, const SizeI& srcSize, Matrix* mat)
{
	float sw = static_cast<float>(srcSize.width);   // 転送元
	float sh = static_cast<float>(srcSize.height);
	float dw = static_cast<float>(dstSize.width);	// 転送先
	float dh = static_cast<float>(dstSize.height);

	float new_x, new_y;
	float new_w, new_h;

	float ratio_w;
	float ratio_h;

	// バックバッファサイズとスクリーンサイズが同じ場合
	if (sw == dw && sh == dh)
	{
		// そのまま設定
		new_x = 0;
		new_y = 0;
		new_w = sw;
		new_h = sh;
		ratio_w = 1.0f;
		ratio_h = 1.0f;
	}
	else
	{
		// 現在のスクリーンサイズ(デフォルトビューポートのサイズ)と画面サイズの比率計算
		ratio_w = dw / sw;
		ratio_h = dh / sh;

		// 縦方向に合わせる ( 左右が余る )
		if (ratio_w > ratio_h)
		{
			new_w = static_cast<float>(sw * ratio_h);
			new_h = static_cast<float>(dh);
			new_x = static_cast<float>((dw / 2) - (new_w / 2));
			new_y = 0;
		}
		//横方向にあわせる
		else
		{
			new_w = static_cast<float>(dw);
			new_h = static_cast<float>(sh * ratio_w);
			new_x = 0;
			new_y = static_cast<float>((dh / 2) - (new_h / 2));
		}
	}

#if 0	// pxel based
	* mat = Matrix::Identity;
	mat->scale(new_w / sw, new_h / sh, 1.0f);
	mat->translate(new_x, new_y, 0.0f);
#else	// screen coord based
	*mat = Matrix::Identity;
	mat->scale(new_w / dw, new_h / dh, 1.0f);
#endif
}

} // namespace ln

