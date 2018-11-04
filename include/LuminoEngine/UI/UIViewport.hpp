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

protected:
    //virtual void render(UIRenderingContext* context);
	virtual void onRender(UIRenderingContext* context);

private:
    detail::UIManager* m_manager;
    Ref<detail::ImageEffectRenderer> m_imageEffectRenderer;
    List<Ref<RenderView>> m_renderViews;
};

} // namespace ln

