
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIViewport.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "../ImageEffect/ImageEffectRenderer.hpp"
#include "UIManager.hpp"

namespace ln {

//==============================================================================
// UIViewport

UIViewport::UIViewport()
    : m_manager(detail::EngineDomain::uiManager())
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

Size UIViewport::arrangeOverride(const Size& finalSize)
{
    // TODO: tmp
    for (auto& rv : m_renderViews) {
        rv->setActualPixelSize(finalSize);
    }
    return UIContainerElement::arrangeOverride(finalSize);
}

//void UIViewport::render(UIRenderingContext* context)
//{
//}

void UIViewport::onUpdateFrame(float elapsedTimer)
{
    m_imageEffectRenderer->updateFrame(elapsedTimer);
}

void UIViewport::onRender(UIRenderingContext* context)
{
    // TODO: ViewBoxTransform

    GraphicsContext* graphicsContext = m_manager->graphicsManager()->graphicsContext();


    for (auto& view : m_renderViews)
    {
        view->render(graphicsContext);
    }

    m_imageEffectRenderer->render(context, graphicsContext->colorBuffer(0));
}

} // namespace ln

