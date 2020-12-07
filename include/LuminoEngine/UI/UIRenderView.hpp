#pragma once
#include <LuminoEngine/Rendering/RenderView.hpp>
#include "UIContainerElement.hpp"

namespace ln {
class UIRenderingContext;
class UIElement;
class UIFrameWindow;
class UIAdornerLayer;
class UIDialog;
class UIFocusNavigator;
namespace detail {
class FlatRenderingPipeline;
} // namespace detail


// UIFrameWindow 用の RenderView.
// その Window のすべてのレンダリングのルートとなる。
class UIFrameRenderView
    : public RenderView
{
public:
    void setRootElement(UIDomainProvidor* element);

    UIDomainProvidor* rootElement() const;
    UIAdornerLayer* adornerLayer() const;

    // TODO: internal
    // 描画コマンド構築と実行まですべて行う
    //void renderTree(GraphicsContext* graphicsContext, UIElement* element);
    virtual void render(GraphicsContext* graphicsContext, RenderTargetTexture* renderTarget) override;

    void invalidate(detail::UIElementDirtyFlags flags);

protected:
    UIFrameRenderView();
    void init();

private:
    UIViewport* m_ownerViewport;    // TODO: obsolete. ベースクラスに追加した
    Ref<UIDomainProvidor> m_rootElement;
    Ref<UIRenderingContext> m_renderingContext;
	//Ref<UIRenderingContext> m_debugRenderingContext;
    Ref<detail::FlatRenderingPipeline> m_sceneRenderingPipeline;
    Ref<RenderViewPoint> m_viewPoint;
    Ref<UIAdornerLayer> m_adornerLayer;

    friend class UIFrameWindow;
    friend class UIViewport;
    friend class UIDomainProvidor;
};


// TODO: 0.6.0 時点で、UI の描画最適化 (更新のあったところだけ再描画) を計画している。
// これは UI ツリーのあるコンテナを、RenderTarget を持つ特別なコンテナとするフラグを持たせ、
// 更新がある場合だけその RenderTarget を再描画することで行おうとしている。
// その時の RenderTarget を表現するのがこのクラス。となる予定。
class UIRenderView	// TODO: detail かも
	: public UIFrameRenderView
{
public:
    UIRenderView();
	void init();



    // TODO: internal
    // TODO: 複数表示のため push/pop の方がいいかもしれない
    //void setDialog(UIDialog* dialog);

    //const Ref<UIFocusNavigator>& focusNavigator() const { return m_focusNavigator; }

public:	// TODO: protected
	virtual void onUpdateFrame(float elapsedSeconds) override;
	virtual void onUpdateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
	virtual void onUpdateUILayout(UILayoutContext* layoutContext) override;
    virtual UIElement* onLookupMouseHoverElement(const Point& frameClientPosition) override;


private:
    //Ref<RenderPass> m_clearRenderPass;
    //Ref<UIDialog> m_dialog;
};

/**
 * UIRenderView の直接の子要素となり、ルーティングイベントを UIRenderView 以上へ通知するための中間要素。
 */
class UIDomainProvidor
	: public UIContainerElement
{
public:
    // TODO: internal
    void setupNavigator();
    const Ref<UIFocusNavigator>& focusNavigator() const { return m_focusNavigator; }

protected:
	void onRoutedEvent(UIEventArgs* e) override;
    UIElement* lookupMouseHoverElement(const Point& frameClientPosition) override;

LN_CONSTRUCT_ACCESS:
    UIDomainProvidor();
	bool init();

private:
    UIFrameRenderView* m_parentRenderView;
    Ref<UIFocusNavigator> m_focusNavigator;

	friend class UIFrameRenderView;
    friend class UIRenderView;
};

} // namespace ln

