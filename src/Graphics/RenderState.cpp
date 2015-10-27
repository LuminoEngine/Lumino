
#include "../Internal.h"
#include "../../include/Lumino/Graphics/RenderState.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderState::RenderState()
	: Blend(BlendMode_Alpha)
	, Culling(CullingMode_Back)
	, Fill(FillMode_Solid)
	, AlphaTest(true)
	//, PointSprite(false)
{}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint32_t RenderState::GetHashCode() const
{
	return
		((uint32_t)Blend) * 1000000 +
		((uint32_t)Culling) * 100000 +
		((uint32_t)Fill) * 10000 +
		((uint32_t)AlphaTest) * 1000/* +
		((uint32_t)DepthTest) * 100 +
		((uint32_t)DepthWrite) * 10*/;
		//((uint32_t)PointSprite);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool RenderState::operator == (const RenderState& obj) const
{
	return
		Blend == obj.Blend &&
		Culling == obj.Culling &&
		Fill == obj.Fill &&
		AlphaTest == obj.AlphaTest/* &&
		DepthTest == obj.DepthTest &&
		DepthWrite == obj.DepthWrite*/;
		//PointSprite == obj.PointSprite;
}


//=============================================================================
// DepthStencilState
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DepthStencilState::DepthStencilState()
	: DepthEnable(true)
	, DepthWriteEnable(true)
	, DepthFunc(CompareFunc::LessEqual)
	, StencilEnable(false)
	, StencilFunc(CompareFunc::Always)
	, StencilReferenceValue(0)
	, StencilFailOp(StencilOp::Keep)
	, StencilDepthFailOp(StencilOp::Keep)
	, StencilPassOp(StencilOp::Keep)
{
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
