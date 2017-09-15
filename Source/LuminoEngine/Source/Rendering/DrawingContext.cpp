
#include "../Internal.h"
#include "ShapesRenderFeature.h"
#include "../Graphics/GraphicsManager.h"
#include <Lumino/Rendering/DrawingContext.h>

LN_NAMESPACE_BEGIN

class DrawElement_DrawShapesRendererCommandList : public detail::DrawElement
{
public:
	detail::ShapesRendererCommandList*	m_commandList = nullptr;

	detail::ShapesRendererCommandList* GetGCommandList(DrawList* owner)
	{
		if (m_commandList == nullptr)
		{
			// このコマンドリストは ShapesRenderFeature 側で解放される
			// TODO: それだと描画フレームスキップ時に対応できない
			m_commandList = owner->getManager()->getShapesRendererCommandListCache()->queryCommandList();
		}
		return m_commandList;
	}

	virtual void drawSubset(const DrawArgs& e) override
	{
		auto* r = e.context->beginShapesRenderer();
		r->executeCommand(m_commandList);
		m_commandList = nullptr;
	}
	virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_T("DrawShapesRendererCommandList")); }
};

//==============================================================================
// DrawingContext
//==============================================================================

//------------------------------------------------------------------------------
DrawingContext::DrawingContext()
{
}

//------------------------------------------------------------------------------
DrawingContext::~DrawingContext()
{
}

//------------------------------------------------------------------------------
void DrawingContext::initialize()
{
	DrawList::initialize(detail::EngineDomain::getGraphicsManager());
}

//------------------------------------------------------------------------------
void DrawingContext::drawTexture(const Rect& destRect, Texture* texture, const Rect& sourceRect)
{
	drawSprite(Vector3(destRect.x, destRect.y, 0), destRect.getSize(), Vector2(0, 0), texture, sourceRect, Color::White, SpriteBaseDirection::Basic2D, BillboardType::None, nullptr);
}

//------------------------------------------------------------------------------
void DrawingContext::drawBoxBackground(const Rect& rect, const CornerRadius& cornerRadius)
{
	Brush* brush = getBrush();
	if (brush != nullptr &&
		(brush->getImageDrawMode() == BrushImageDrawMode::BoxFrame || brush->getImageDrawMode() == BrushImageDrawMode::BorderFrame))
	{
		drawFrameRectangle(rect);
	}
	else
	{
		auto* ptr = resolveDrawElement<DrawElement_DrawShapesRendererCommandList>(getManager()->getInternalContext()->m_shapesRenderer, nullptr, true);
		auto* list = ptr->GetGCommandList(this);
		list->addDrawBoxBackground(rect, cornerRadius);
	}
}

//------------------------------------------------------------------------------
void DrawingContext::drawBoxBorder(
	const Rect& rect, const Thickness& thickness, const CornerRadius& cornerRadius,
	const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor,
	BorderDirection borderDirection,
	const Color& shadowColor, float shadowBlur, float shadowWidth, ShadowDirection shadowDirection)
{
	if (Math::nearEqual(shadowWidth, 0))
	{
		drawBoxBorderLine(rect, thickness, cornerRadius, leftColor, topColor, rightColor, bottomColor, borderDirection);
	}
	else
	{
		auto* ptr = resolveDrawElement<DrawElement_DrawShapesRendererCommandList>(getManager()->getInternalContext()->m_shapesRenderer, nullptr, true);
		auto* list = ptr->GetGCommandList(this);
		list->addDrawBoxBorder(
			rect, thickness, cornerRadius,
			leftColor, topColor, rightColor, bottomColor,
			shadowColor, shadowBlur, shadowWidth, (shadowDirection == ShadowDirection::Inside), (borderDirection == BorderDirection::Inside));
	}
}

//------------------------------------------------------------------------------
void DrawingContext::drawBoxBorderLine(
	const Rect& rect, const Thickness& thickness, const CornerRadius& cornerRadius,
	const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor,
	BorderDirection borderDirection)
{
	auto* ptr = resolveDrawElement<DrawElement_DrawShapesRendererCommandList>(getManager()->getInternalContext()->m_shapesRenderer, nullptr, true);
	auto* list = ptr->GetGCommandList(this);
	list->drawBoxBorderLine(
		rect, thickness,
		leftColor, topColor, rightColor, bottomColor,
		cornerRadius, (borderDirection == BorderDirection::Inside));
}


//------------------------------------------------------------------------------
void DrawingContext::drawBoxShadow(const Rect& rect, const CornerRadius& cornerRadius, const Color& color, float blur, float width, ShadowDirection shadowDirection)
{
	auto* ptr = resolveDrawElement<DrawElement_DrawShapesRendererCommandList>(getManager()->getInternalContext()->m_shapesRenderer, nullptr, true);
	auto* list = ptr->GetGCommandList(this);
	list->addDrawBoxShadow(rect, cornerRadius, color, blur, width, (shadowDirection == ShadowDirection::Inside));
}

LN_NAMESPACE_END

