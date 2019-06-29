#pragma once
#include <LuminoEngine/Rendering/RenderView.hpp>

namespace ln {
class UIRenderingContext;
class UIElement;
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
    virtual void render(GraphicsContext* graphicsContext) override;

protected:
	virtual void onUpdateFrame(float elapsedSeconds) override;
	virtual void onUpdateUIStyle(const detail::UIStyleInstance* finalStyle) override;
	virtual void onUpdateUILayout(const Rect& finalGlobalRect) override;

private:
	Ref<UIRenderingContext> m_renderingContext;
	Ref<detail::FlatRenderingPipeline> m_sceneRenderingPipeline;
	Ref<detail::DrawElementListCollector> m_drawElementListCollector;
    Ref<RenderViewPoint> m_viewPoint;
    Ref<UIElement> m_rootElement;
};

} // namespace ln

