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
	void initialize();

    // TODO: internal
    const Size& actualViewboxSize() const { return m_actualViewboxSize; }

protected:
    virtual Size arrangeOverride(const Size& finalSize) override;
    //virtual void render(UIRenderingContext* context);
    virtual void onUpdateFrame(float elapsedSeconds) override;
	virtual void onRender(UIRenderingContext* context) override;
    virtual void onRoutedEvent(UIEventArgs* e) override;

private:
    detail::UIManager* m_manager;
    Ref<detail::ImageEffectRenderer> m_imageEffectRenderer;
    List<Ref<RenderView>> m_renderViews;
    Size m_actualViewboxSize;
};

} // namespace ln

