﻿
#include "Internal.hpp"
#include <LuminoGraphics/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include "../Rendering/CommandListServer.hpp"
#include "../Rendering/RenderStage.hpp"
#include "../Rendering/DrawElementListBuilder.hpp"
#include "../Rendering/RenderFeature/FrameRectRenderFeature.hpp"
#include "../Rendering/RenderFeature/ShapesRenderFeature.hpp"

namespace ln {

//==============================================================================
// UIRenderingContext

UIRenderingContext::UIRenderingContext()
{
    m_commandList = m_listServer->acquirePrimaryList(RenderPart::Geometry);
    builder()->m_stateStackMode = detail::StateStackMode::ScissorPushPop;
}

void UIRenderingContext::drawSolidRectangle(const Rect& rect, const Color& color)
{
	// TODO: drawBoxElement で代用しているが、ただ四角形描くのにこれはちょっと重い気がする
	BoxElementShapeBaseStyle base;
	base.baseRect = rect;
	BoxElementShapeBackgroundStyle background;
	background.color = color;
	drawBoxElement(base, &background, nullptr, nullptr);

    //auto* element = builder()->addNewDrawElement<detail::DrawShapesElement>(
    //    m_manager->shapesRenderFeature(),
    //    builder()->shapesRenderFeatureStageParameters());

    //element->commandList.addDrawBoxBackground(builder()->targetList()->dataAllocator(), element->combinedWorldMatrix(), rect, CornerRadius(), color);
}

void UIRenderingContext::drawImageBox(const Rect& rect, Sprite9DrawMode mode, const Rect& textureSourceRect, const Thickness& borderThickness, const Color& color)
{
    if (!builder()->material()) return;

    Texture* texture = builder()->material()->mainTexture();
    if (!texture) return;

    Rect actualSourceRect = textureSourceRect;
    if (actualSourceRect.isZero()) actualSourceRect = Rect(0, 0, texture->width(), texture->height());

    //if (mode == BrushImageDrawMode::Image) {
    //    drawSolidRectangle(rect, color);
    //}
    //else {
        auto* element = builder()->addNewDrawElement<detail::DrawFrameRectElement>(m_manager->frameRectRenderFeature());

        element->rect = rect;
        element->transform = element->combinedWorldMatrix();
        element->imageDrawMode = mode;
        element->borderThickness = borderThickness;
        element->srcRect = actualSourceRect;
        //element->wrapMode = Sprite9DrawMode::StretchedBoxFrame;
        //element->wrapMode = mode;
    //}
}

//void UIRenderingContext::drawBoxBackground(const Rect& rect, const CornerRadius& cornerRadius, BrushImageDrawMode mode/*, Material* material*/, const Rect& textureSourceRect, const Color& color)
//{
//    //builder()->setMaterial(material);
//
//    if (builder()->material() && !builder()->material()->mainTexture()) {
//        mode = BrushImageDrawMode::Image;
//    }
//
//	if (mode == BrushImageDrawMode::Image)
//	{
//        auto* element = builder()->addNewDrawElement<detail::DrawShapesElement>(
//            m_manager->shapesRenderFeature(),
//            builder()->shapesRenderFeatureStageParameters());
//		
//        element->commandList.addDrawBoxBackground(builder()->targetList()->dataAllocator(), element->combinedWorldMatrix(), rect, cornerRadius, color);
//	}
//	else
//	{
//		auto* element = builder()->addNewDrawElement<detail::DrawFrameRectElement>(
//			m_manager->frameRectRenderFeature(),
//			builder()->frameRectRenderFeatureStageParameters());
//
//		element->rect = rect;
//		element->transform = element->combinedWorldMatrix();
//		element->imageDrawMode = mode;
//		element->borderThickness = Thickness();	// TODO: //borderThickness;
//		element->srcRect = textureSourceRect;
//		element->wrapMode = BrushWrapMode::Stretch;
//	}
//
//    // TODO: bounding box
//}

void UIRenderingContext::drawBoxBorderLine(const Rect& rect, const Thickness& thickness, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, const CornerRadius& cornerRadius, bool borderInset)
{
    auto* element = builder()->addNewDrawElement<detail::DrawShapesElement>(m_manager->shapesRenderFeature());

    element->commandList.drawBoxBorderLine(builder()->targetList()->dataAllocator(), element->combinedWorldMatrix(), rect, thickness, leftColor, topColor, rightColor, bottomColor, cornerRadius, borderInset);


    // TODO: bounding box
}

void UIRenderingContext::drawBoxBorderLine(const Rect& rect, float thickness, const Color& color, bool borderInset)
{
	drawBoxBorderLine(rect, Thickness(thickness), color, color, color, color, CornerRadius(), borderInset);
}

void UIRenderingContext::drawBoxShadow(const Rect& rect, const CornerRadius& cornerRadius, const Vector2& offset, const Color& color, float blur, float width, bool inset)
{
	auto* element = builder()->addNewDrawElement<detail::DrawShapesElement>(m_manager->shapesRenderFeature());

	element->commandList.addDrawBoxShadow(builder()->targetList()->dataAllocator(), element->combinedWorldMatrix(), rect, cornerRadius, offset, color, blur, width, inset);


	// TODO: bounding box

}

void UIRenderingContext::drawBoxElement(const BoxElementShapeBaseStyle& baseStyle, const BoxElementShapeBackgroundStyle* backbroundStyle, const BoxElementShapeBorderStyle* borderStyle, const BoxElementShapeShadowStyle* shadowStyle)
{
    auto* element = builder()->addNewDrawElement<detail::DrawBoxElementShape>(m_manager->shapesRenderFeature());

    auto& allocator = builder()->targetList()->dataAllocator();
    element->commandList.addResetCommand(allocator);
    element->commandList.addBaseCommand(allocator, baseStyle);
    if (backbroundStyle) element->commandList.addBackgroundCommand(allocator, *backbroundStyle);
    if (borderStyle) element->commandList.addBorderCommand(allocator, *borderStyle);
    if (shadowStyle) element->commandList.addShadowCommand(allocator, *shadowStyle);
    element->commandList.addSubmitCommand(allocator);
}

void UIRenderingContext::drawImage(const Rect& destinationRect, Material* material)
{
    auto texture = material->mainTexture();
    drawSprite(
        Matrix::makeTranslation(destinationRect.x, destinationRect.y, 0),
        destinationRect.getSize(),
        Vector2::Zero,
        Rect(0, 0, 1, 1),
        Color::White,
        SpriteBaseDirection::Basic2D,
        BillboardType::None,
        detail::SpriteFlipFlags::None,
        material);
}

void UIRenderingContext::drawVisual(UIElement* element, const Matrix& transform)
{
	//pushState();
	element->renderClient(this, builder()->baseTransform() * element->m_localTransform * transform);
	//popState();
}

UITheme* UIRenderingContext::theme() const
{
	assert(m_theme);
	return m_theme;
}

//void UIRenderingContext::pushScissorRect(const RectI& rect)
//{
//    if (m_scissorStack.empty()) {
//        m_scissorStack.push(rect);
//    }
//    else {
//        RectI r = rect;
//        r.clip(m_scissorStack.top());
//        m_scissorStack.push(r);
//    }
//
//    setScissorRect(m_scissorStack.top());
//}
//
//void UIRenderingContext::popScissorRect()
//{
//    m_scissorStack.pop();
//    if (m_scissorStack.empty()) {
//        setScissorRect(RectI::Empty);
//    }
//    else {
//        setScissorRect(m_scissorStack.top());
//    }
//}

void UIRenderingContext::resetForBeginRendering()
{
	RenderingContext::resetForBeginRendering();
}

} // namespace ln

