#pragma once
#include <LuminoEngine/Rendering/RenderView.hpp>

namespace ln {
class UIRenderingContext;
class UIElement;
class UIAdornerLayer;
class UIDialog;
namespace detail {
class FlatRenderingPipeline;
} // namespace detail

// TODO: 0.6.0 時点で、UI の描画最適化 (更新のあったところだけ再描画) を計画している。
// これは UI ツリーのあるコンテナを、RenderTarget を持つ特別なコンテナとするフラグを持たせ、
// 更新がある場合だけその RenderTarget を再描画することで行おうとしている。
// その時の RenderTarget を表現するのがこのクラス。となる予定。
class UIRenderView	// TODO: detail かも
	: public RenderView
{
public:
    UIRenderView();
	void init();

    void setRootElement(UIElement* element);

	// TODO: internal
	// 描画コマンド構築と実行まですべて行う
	//void renderTree(GraphicsContext* graphicsContext, UIElement* element);
    virtual void render(GraphicsContext* graphicsContext, RenderTargetTexture* renderTarget) override;

    UIAdornerLayer* adornerLayer() const;

    // TODO: internal
    // TODO: 複数表示のため push/pop の方がいいかもしれない
    void setDialog(UIDialog* dialog);

protected:
	virtual void onUpdateFrame(float elapsedSeconds) override;
	virtual void onUpdateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
	virtual void onUpdateUILayout(const Rect& finalGlobalRect) override;
    virtual UIElement* onLookupMouseHoverElement(const Point& frameClientPosition) override;

private:
	Ref<UIRenderingContext> m_renderingContext;
	Ref<detail::FlatRenderingPipeline> m_sceneRenderingPipeline;
	Ref<detail::DrawElementListCollector> m_drawElementListCollector;
    Ref<RenderViewPoint> m_viewPoint;
    //Ref<RenderPass> m_clearRenderPass;
    Ref<UIElement> m_rootElement;
    Ref<UIAdornerLayer> m_adornerLayer;
    Ref<UIDialog> m_dialog;

};

} // namespace ln

