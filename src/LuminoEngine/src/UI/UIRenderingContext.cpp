
#include "Internal.hpp"
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include "../Rendering/RenderStage.hpp"
#include "../Rendering/DrawElementListBuilder.hpp"
#include "../Rendering/FrameRectRenderFeature.hpp"
#include "../Rendering/ShapesRenderFeature.hpp"

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


	if (0)
	{
		auto* element = m_builder->addNewDrawElement<detail::DrawFrameRectElement>(
			m_manager->frameRectRenderFeature(),
			m_builder->frameRectRenderFeatureStageParameters());

		element->rect = rect;
		element->transform = Matrix();  // TODO:
		element->imageDrawMode = mode;
		element->borderThickness = borderThickness;
		element->srcRect = textureSourceRect;
		element->wrapMode = BrushWrapMode::Stretch;
	}
	else
	{
		auto* element = m_builder->addNewDrawElement<detail::DrawShapesElement>(
			m_manager->shapesRenderFeature(),
			m_builder->shapesRenderFeatureStageParameters());

        
		element->commandList.addDrawBoxBackground(m_manager->graphicsManager()->primaryRenderingCommandList()->linearAllocator(), rect, cornerRadius);
	}

    // TODO: bounding box
}

void UIRenderingContext::resetForBeginRendering()
{
	RenderingContext::resetForBeginRendering();
	m_elementList->clear();
}

} // namespace ln

