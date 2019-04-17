
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
	, cullMode(CullMode::Back)
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
	: depthTestFunc(ComparisonFunc::LessEqual)
	, depthWriteEnabled(true)
	, stencilEnabled(false)
	, stencilReferenceValue(0xFF)
	, frontFace()
	, backFace()
{
} 

} // namespace ln
