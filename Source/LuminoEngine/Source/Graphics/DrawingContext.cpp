
#include "../Internal.h"
#include <Lumino/Graphics/DrawingContext.h>

LN_NAMESPACE_BEGIN

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

LN_NAMESPACE_END

