
#include "Internal.hpp"
#include <LuminoEngine/UI/UIViewport.hpp>
#include "../Rendering/ImageEffectRenderer.hpp"

namespace ln {

//==============================================================================
// UIViewport

UIViewport::UIViewport()
{
}

void UIViewport::initialize()
{
	UIContainerElement::initialize();
    m_imageEffectRenderer = makeRef<detail::ImageEffectRenderer>();
}

void UIViewport::addRenderView(RenderView* view)
{
    m_renderViews.add(view);
}

void UIViewport::removeRenderView(RenderView* view)
{
    m_renderViews.remove(view);
}

void UIViewport::addImageEffect(ImageEffect* effect)
{
    m_imageEffectRenderer->addImageEffect(effect);
}

void UIViewport::removeImageEffect(ImageEffect* effect)
{
    m_imageEffectRenderer->removeImageEffect(effect);
}

void UIViewport::render(UIRenderingContext* context)
{
}

void UIViewport::onRender(UIRenderingContext* context)
{

}

} // namespace ln

