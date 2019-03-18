
#include "Internal.hpp"
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include "../Rendering/RenderStage.hpp"
#include "../Rendering/DrawElementListBuilder.hpp"
#include "../Rendering/FrameRectRenderFeature.hpp"

namespace ln {

//==============================================================================
// UIRenderingContext

UIRenderingContext::UIRenderingContext()
	: m_elementList(makeRef<detail::DrawElementList>(detail::EngineDomain::renderingManager()))
{
	setDrawElementList(m_elementList);
}

void UIRenderingContext::drawBoxBackground(const Rect& rect, const Thickness& borderThickness, const CornerRadius& cornerRadius, BrushImageDrawMode mode, AbstractMaterial* material, const Rect& textureSourceRect)
{
    m_builder->setMaterial(material);

    auto* element = m_builder->addNewDrawElement<detail::DrawFrameRectElement>(
        m_manager->frameRectRenderFeature(),
        m_builder->frameRectRenderFeatureStageParameters());

    element->rect = rect;
    element->transform = Matrix();  // TODO:
    element->imageDrawMode = mode;
    element->borderThickness = borderThickness;
    element->srcRect = textureSourceRect;
    element->wrapMode = BrushWrapMode::Stretch;

    // TODO: bounding box
}

void UIRenderingContext::resetForBeginRendering()
{
	RenderingContext::resetForBeginRendering();
	m_elementList->clear();
}

} // namespace ln

