
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderView.hpp>
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

RenderView::RenderView()
	: m_clearMode(RenderViewClearMode::None)
	, m_backgroundColor(Color::White)
{
}

RenderView::~RenderView()
{
}

void RenderView::init()
{
	Object::init();
	m_manager = detail::EngineDomain::renderingManager();
}

void RenderView::clearDrawElementListManagers()
{
	m_elementListManagers.clear();
}

void RenderView::addDrawElementListManager(detail::DrawElementListCollector* elementListManager)
{
	m_elementListManagers.add(elementListManager);
}

EventConnection RenderView::connectOnUIEvent(UIEventHandler handler)
{
    return m_onUIEvent.connect(handler);
}

void RenderView::updateFrame(float elapsedSeconds)
{
	onUpdateFrame(elapsedSeconds);
}

void RenderView::updateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* parentFinalStyle)
{
	onUpdateUIStyle(styleContext, parentFinalStyle);
}

void RenderView::updateUILayout(UILayoutContext* layoutContext, const Rect& parentFinalGlobalRect)
{
	// TODO: こんなのでいいのか要チェック
	onUpdateUILayout(layoutContext, Rect(parentFinalGlobalRect.x, parentFinalGlobalRect.y, m_actualSize.width, m_actualSize.height));
}

void RenderView::onUpdateFrame(float elapsedSeconds)
{
}

void RenderView::onUpdateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
}

void RenderView::onUpdateUILayout(UILayoutContext* layoutContext, const Rect& finalGlobalRect)
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

//void RenderView::render(GraphicsContext* graphicsContext, const FrameBuffer& frameBuffer, detail::SceneRenderer* sceneRenderer)
//{
//	m_renderingFrameBufferSize = SizeI(frameBuffer.renderTarget[0]->width(), frameBuffer.renderTarget[0]->height());
//
//	sceneRenderer->render(graphicsContext, this, frameBuffer);
//
//	// 誤用防止
//	m_renderingFrameBufferSize = SizeI();
//}

} // namespace ln

