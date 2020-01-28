#pragma once
#include "UIContainerElement.hpp"

namespace ln {
class RenderView;
class ImageEffect;
class Material;
namespace detail {
class UIManager;
class ImageEffectRenderer;
}

/** Viewport の配置方法 */
enum class UIViewportPlacement
{
	Stretch,		/**< 転送先領域全体に拡大または縮小する */
	//AutoResize,		/**< 転送先領域と同じピクセルサイズになるよう自動的にリサイズする */
	ViewBox,		/**< 転送元領域のアスペクト比が維持されるように余白を挿入する */
};

class UIViewport
	: public UIContainerElement
{
public:
    void addRenderView(RenderView* view);
    void removeRenderView(RenderView* view);

    void addImageEffect(ImageEffect* effect);
    void removeImageEffect(ImageEffect* effect);



	void setViewBoxSize(const Size& value) { m_viewBoxSize = value; }


    UIViewport();
    virtual ~UIViewport();
	void init();
    virtual void onDispose(bool explicitDisposing) override;

    // TODO: internal
    const Size& actualViewboxSize() const { return m_actualViewboxSize; }

protected:
    virtual UIElement* lookupMouseHoverElement(const Point& frameClientPosition) override;
	virtual void onUpdateFrame(float elapsedSeconds) override;
	virtual void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;
	virtual void onUpdateLayout(UILayoutContext* layoutContext) override;
    //virtual void render(UIRenderingContext* context);
	virtual void onRender(UIRenderingContext* context) override;
    virtual void onRoutedEvent(UIEventArgs* e) override;

private:
	bool isViewBoxRenderTargetAutoResize() const { return m_viewBoxSize.width <= 0.0f && m_viewBoxSize.height <= 0.0f; }
	bool isViewBoxAspectScaling() const { return !isViewBoxRenderTargetAutoResize() && m_placement == UIViewportPlacement::ViewBox; }
	void acquirePrimaryTarget(const SizeI& viewPixelSize);
	static void makeViewBoxTransform(const SizeI& dstSize, const SizeI& srcSize, Matrix* mat);

    detail::UIManager* m_manager;
    Ref<detail::ImageEffectRenderer> m_imageEffectRenderer;
    List<Ref<RenderView>> m_renderViews;
	//Ref<RenderPass> m_renderPass;
    Size m_actualViewboxSize;
    Ref<Material> m_blitMaterial;
	Ref<RenderTargetTexture> m_primaryTarget;
	UIViewportPlacement m_placement;
	Size m_viewBoxSize;
};

} // namespace ln

