
#include "Internal.hpp"
#include <LuminoEngine/Graphics/RenderState.hpp>

namespace ln {

//==============================================================================
// RenderTargetBlendDesc

RenderTargetBlendDesc::RenderTargetBlendDesc()
	: blendEnable(false)
	, sourceBlend(BlendFactor::One)
	, destinationBlend(BlendFactor::Zero)
	, blendOp(BlendOp::Add)
	, sourceBlendAlpha(BlendFactor::One)
	, destinationBlendAlpha(BlendFactor::Zero)
	, blendOpAlpha(BlendOp::Add)
{
}

//==============================================================================
// BlendStateDesc

BlendStateDesc::BlendStateDesc()
	: independentBlendEnable(false)
	, renderTargets{}
{
}

//==============================================================================
// RasterizerStateDesc

RasterizerStateDesc::RasterizerStateDesc()
	: fillMode(FillMode::Solid)
	, cullMode(CullingMode::Back)
{
}

//==============================================================================
// StencilOpDesc

StencilOpDesc::StencilOpDesc()
	: stencilFailOp(StencilOp::Keep)
	, stencilDepthFailOp(StencilOp::Keep)
	, stencilPassOp(StencilOp::Keep)
	, stencilFunc(ComparisonFunc::Always)
{
}

//==============================================================================
// DepthStencilStateDesc

DepthStencilStateDesc::DepthStencilStateDesc()
	: depthTestFunc(ComparisonFunc::LessEqual)  //　Note: DX=LessEqual, GL=Less
	, depthWriteEnabled(true)
	, stencilEnabled(false)
	, stencilReferenceValue(0xFF)
	, frontFace()
	, backFace()
{
} 

////==============================================================================
//// RenderStateDesc
//
//RenderStateDesc::RenderStateDesc()
//	: alphaBlendEnabled(false)
//	, blendOp(BlendOp::Add)
//	, sourceBlend(BlendFactor::One)
//	, destinationBlend(BlendFactor::Zero)
//	, cullingMode(CullingMode::Back)
//	, fillMode(FillMode::Solid)
//{
//}

#if 0
//==============================================================================
// RenderState
//==============================================================================
const RenderState RenderState::Default;

//------------------------------------------------------------------------------
RenderState::RenderState()
	: alphaBlendEnabled(false)
	, blendOp(BlendOp::Add)
	, sourceBlend(BlendFactor::One)
	, destinationBlend(BlendFactor::Zero)
	, Blend(BlendMode::Normal)
	, Culling(CullingMode::Back)
	, Fill(FillMode_Solid)
	, AlphaTest(true)
{}

//------------------------------------------------------------------------------
uint32_t RenderState::getHashCode() const
{
	return
		((uint32_t)Blend) * 1000000 +
		((uint32_t)Culling) * 100000 +
		((uint32_t)Fill) * 10000 +
		((uint32_t)AlphaTest) * 1000 +
		((uint32_t)blendOp) * 100 +
		((uint32_t)sourceBlend) * 10 +
		((uint32_t)destinationBlend);
}

//------------------------------------------------------------------------------
bool RenderState::equals(const RenderState& state) const
{
	return
		Blend == state.Blend &&
		Culling == state.Culling &&
		Fill == state.Fill &&
		AlphaTest == state.AlphaTest &&
		blendOp == state.blendOp &&
		sourceBlend == state.sourceBlend &&
		destinationBlend == state.destinationBlend;
}

//------------------------------------------------------------------------------
bool RenderState::operator == (const RenderState& obj) const
{
	return
		Blend == obj.Blend &&
		Culling == obj.Culling &&
		Fill == obj.Fill &&
		AlphaTest == obj.AlphaTest &&
		blendOp == obj.blendOp &&
		sourceBlend == obj.sourceBlend &&
		destinationBlend == obj.destinationBlend;
}


//==============================================================================
// DepthStencilState
//==============================================================================
const DepthStencilState DepthStencilState::Default;

//------------------------------------------------------------------------------
DepthStencilState::DepthStencilState()
	: DepthTestEnabled(true)
	, DepthWriteEnabled(true)
	, DepthTestFunc(CompareFunc::LessEqual)
	, StencilEnabled(false)
	, StencilFunc(CompareFunc::Always)
	, StencilReferenceValue(0)
	, StencilFailOp(StencilOp::Keep)
	, StencilDepthFailOp(StencilOp::Keep)
	, StencilPassOp(StencilOp::Keep)
{
}

//------------------------------------------------------------------------------
bool DepthStencilState::equals(const DepthStencilState& state) const
{
	return
		DepthTestEnabled == state.DepthTestEnabled &&
		DepthWriteEnabled == state.DepthWriteEnabled &&
		DepthTestFunc == state.DepthTestFunc &&
		StencilEnabled == state.StencilEnabled &&
		StencilFunc == state.StencilFunc &&
		StencilReferenceValue == state.StencilReferenceValue &&
		StencilFailOp == state.StencilFailOp &&
		StencilDepthFailOp == state.StencilDepthFailOp &&
		StencilPassOp == state.StencilPassOp;
}
#endif

} // namespace ln
