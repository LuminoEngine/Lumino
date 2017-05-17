
#include "../Internal.h"
#include "Rendering/ShapesRenderer.h"
#include "GraphicsManager.h"
#include <Lumino/Graphics/DrawingContext.h>

LN_NAMESPACE_BEGIN

class DrawElement_DrawShapesRendererCommandList : public detail::DrawElement
{
public:
	detail::ShapesRendererCommandList*	m_commandList = nullptr;

	detail::ShapesRendererCommandList* GetGCommandList(DrawList* owner)
	{
		if (m_commandList == nullptr)
		{
			// このコマンドリストは ShapesRenderer 側で解放される
			// TODO: それだと描画フレームスキップ時に対応できない
			m_commandList = owner->GetManager()->GetShapesRendererCommandListCache()->QueryCommandList();
		}
		return m_commandList;
	}

	virtual void DrawSubset(detail::DrawElementList* oenerList, detail::InternalContext* context) override
	{
		auto* r = context->BeginShapesRenderer();
		r->ExecuteCommand(m_commandList);
		m_commandList = nullptr;
	}
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
void DrawingContext::Initialize()
{
	DrawList::Initialize(detail::EngineDomain::GetGraphicsManager());
}

//------------------------------------------------------------------------------
void DrawingContext::DrawTexture(const Rect& destRect, Texture* texture, const Rect& sourceRect)
{
	DrawSprite(Vector3(destRect.x, destRect.y, 0), destRect.GetSize(), Vector2(0, 0), texture, sourceRect, Color::White, SpriteBaseDirection::Basic2D, nullptr);
}

//------------------------------------------------------------------------------
void DrawingContext::DrawBoxBackground(const Rect& rect, const CornerRadius& cornerRadius)
{
	Brush* brush = GetBrush();
	if (brush != nullptr &&
		(brush->GetImageDrawMode() == BrushImageDrawMode::BoxFrame || brush->GetImageDrawMode() == BrushImageDrawMode::BorderFrame))
	{
		DrawFrameRectangle(rect);
	}
	else
	{
		auto* ptr = ResolveDrawElement<DrawElement_DrawShapesRendererCommandList>(detail::DrawingSectionId::NanoVG, GetManager()->GetInternalContext()->m_shapesRenderer, nullptr);
		auto* list = ptr->GetGCommandList(this);
		list->AddDrawBoxBackground(rect, cornerRadius);
	}
}

//------------------------------------------------------------------------------
void DrawingContext::DrawBoxBorder(
	const Rect& rect, const ThicknessF& thickness, const CornerRadius& cornerRadius,
	const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor,
	BorderDirection borderDirection)
{
	auto* ptr = ResolveDrawElement<DrawElement_DrawShapesRendererCommandList>(detail::DrawingSectionId::NanoVG, GetManager()->GetInternalContext()->m_shapesRenderer, nullptr);
	auto* list = ptr->GetGCommandList(this);
	list->AddDrawBoxBorder2(
		rect, thickness,
		leftColor, topColor, rightColor, bottomColor,
		cornerRadius, (borderDirection == BorderDirection::Inside));
}

//------------------------------------------------------------------------------
void DrawingContext::DrawBoxBorder(
	const Rect& rect, const ThicknessF& thickness,
	const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor,
	float ltRad, float rtRad, float lbRad, float rbRad, BorderDirection borderDirection,
	const Color& shadowColor, float shadowBlur, float shadowWidth, ShadowDirection shadowDirection)
{
	auto* ptr = ResolveDrawElement<DrawElement_DrawShapesRendererCommandList>(detail::DrawingSectionId::NanoVG, GetManager()->GetInternalContext()->m_shapesRenderer, nullptr);
	auto* list = ptr->GetGCommandList(this);
	list->AddDrawBoxBorder(
		rect.x, rect.y, rect.width, rect.height, thickness.Left, thickness.Top, thickness.Right, thickness.Bottom,
		leftColor, topColor, rightColor, bottomColor,
		ltRad, rtRad, lbRad, rbRad,
		shadowColor, shadowBlur, shadowWidth, (shadowDirection == ShadowDirection::Inside), (borderDirection == BorderDirection::Inside));
}

//------------------------------------------------------------------------------
void DrawingContext::DrawBoxShadow(const Rect& rect, const CornerRadius& cornerRadius, const Color& color, float blur, float width, ShadowDirection shadowDirection)
{
	auto* ptr = ResolveDrawElement<DrawElement_DrawShapesRendererCommandList>(detail::DrawingSectionId::NanoVG, GetManager()->GetInternalContext()->m_shapesRenderer, nullptr);
	auto* list = ptr->GetGCommandList(this);
	list->AddDrawBoxShadow(rect, cornerRadius, color, blur, width, (shadowDirection == ShadowDirection::Inside));
}

LN_NAMESPACE_END

