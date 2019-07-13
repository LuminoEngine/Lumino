
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
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

void UIRenderingContext::drawBoxBackground(const Rect& rect, const Thickness& borderThickness, const CornerRadius& cornerRadius, BrushImageDrawMode mode/*, AbstractMaterial* material*/, const Rect& textureSourceRect, const Color& color)
{
    //m_builder->setMaterial(material);

    if (!m_builder->material()->mainTexture()) {
        mode = BrushImageDrawMode::Image;
    }

	if (mode == BrushImageDrawMode::Image)
	{
        auto* element = m_builder->addNewDrawElement<detail::DrawShapesElement>(
            m_manager->shapesRenderFeature(),
            m_builder->shapesRenderFeatureStageParameters());
		
        if (borderThickness.isZero()) {
            element->commandList.addDrawBoxBackground(m_builder->targetList()->dataAllocator(), element->combinedWorldMatrix(), rect, cornerRadius, color);
        }
        else {
            element->commandList.addDrawBoxBorder(m_builder->targetList()->dataAllocator(), element->combinedWorldMatrix(), rect, borderThickness, cornerRadius, Color::Gray, Color::Gray, Color::Gray, Color::Gray, Color::Gray, 0, 0, false, false);
        }
	}
	else
	{
		auto* element = m_builder->addNewDrawElement<detail::DrawFrameRectElement>(
			m_manager->frameRectRenderFeature(),
			m_builder->frameRectRenderFeatureStageParameters());

		element->rect = rect;
		element->transform = element->combinedWorldMatrix();
		element->imageDrawMode = mode;
		element->borderThickness = borderThickness;
		element->srcRect = textureSourceRect;
		element->wrapMode = BrushWrapMode::Stretch;
	}

    // TODO: bounding box
}

void UIRenderingContext::resetForBeginRendering()
{
	RenderingContext::resetForBeginRendering();
	m_elementList->clear();
}

} // namespace ln

