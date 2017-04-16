
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
void DrawingContext::DrawTexture(const RectF& destRect, Texture* texture, const RectF& sourceRect)
{
	DrawSprite(Vector3(destRect.x, destRect.y, 0), destRect.GetSize(), Vector2(0, 0), texture, sourceRect, Color::White, SpriteBaseDirection::Basic2D, nullptr);
}

//------------------------------------------------------------------------------
void DrawingContext::DrawBoxBorder(
	const RectF& rect, const ThicknessF& thickness,
	const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor,
	float ltRad, float rtRad, float lbRad, float rbRad)
{
	auto* ptr = ResolveDrawElement<DrawElement_DrawShapesRendererCommandList>(detail::DrawingSectionId::NanoVG, GetManager()->GetInternalContext()->m_shapesRenderer, nullptr);
	auto* list = ptr->GetGCommandList(this);
	list->AddDrawBoxBorder(
		rect.x, rect.y, rect.width, rect.height, thickness.Left, thickness.Top, thickness.Right, thickness.Bottom,
		leftColor, topColor, rightColor, bottomColor,
		ltRad, rtRad, lbRad, rbRad);
}

//------------------------------------------------------------------------------
void DrawingContext::DrawBoxShadow(const RectF& rect, const Color& color, float blur, float width, bool inset)
{
	auto* ptr = ResolveDrawElement<DrawElement_DrawShapesRendererCommandList>(detail::DrawingSectionId::NanoVG, GetManager()->GetInternalContext()->m_shapesRenderer, nullptr);
	auto* list = ptr->GetGCommandList(this);
	list->AddDrawBoxShadow(rect.x, rect.y, rect.width, rect.height, color, blur, width, inset);
}

LN_NAMESPACE_END

