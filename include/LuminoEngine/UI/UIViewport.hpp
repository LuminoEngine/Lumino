#pragma once
#include "UIContainerElement.hpp"

namespace ln {
class RenderView;
class ImageEffect;
namespace detail {
class UIManager;
class ImageEffectRenderer;
}

class UIViewport
	: public UIContainerElement
{
public:
    void addRenderView(RenderView* view);
    void removeRenderView(RenderView* view);

    void addImageEffect(ImageEffect* effect);
    void removeImageEffect(ImageEffect* effect);

    UIViewport();
    virtual ~UIViewport();
	void init();
    virtual void onDispose(bool explicitDisposing) override;

    // TODO: internal
    const Size& actualViewboxSize() const { return m_actualViewboxSize; }

protected:
    virtual UIElement* lookupMouseHoverElement(const Point& globalPt) override;
	virtual void onUpdateFrame(float elapsedSeconds) override;
	virtual void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
	virtual void onUpdateLayout(const Rect& finalGlobalRect) override;
    virtual Size arrangeOverride(const Size& finalSize) override;
    //virtual void render(UIRenderingContext* context);
	virtual void onRender(UIRenderingContext* context) override;
    virtual void onRoutedEvent(UIEventArgs* e) override;

private:
    detail::UIManager* m_manager;
    Ref<detail::ImageEffectRenderer> m_imageEffectRenderer;
    List<Ref<RenderView>> m_renderViews;
    Size m_actualViewboxSize;
};

} // namespace ln

