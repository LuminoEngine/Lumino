
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

bool RenderTargetBlendDesc::equals(const RenderTargetBlendDesc& lhs, const RenderTargetBlendDesc& rhs)
{
	return
		lhs.blendEnable == rhs.blendEnable &&
		lhs.sourceBlend == rhs.sourceBlend &&
		lhs.destinationBlend == rhs.destinationBlend &&
		lhs.blendOp == rhs.blendOp &&
		lhs.sourceBlendAlpha == rhs.sourceBlendAlpha &&
		lhs.destinationBlendAlpha == rhs.destinationBlendAlpha &&
		lhs.blendOpAlpha == rhs.blendOpAlpha;
}

//==============================================================================
// BlendStateDesc

BlendStateDesc::BlendStateDesc()
    : independentBlendEnable(false)
    , renderTargets{}
{
}

bool BlendStateDesc::equals(const BlendStateDesc& lhs, const BlendStateDesc& rhs)
{
	for (int i = 0; i < LN_ARRAY_SIZE_OF(renderTargets); i++) {
		if (!RenderTargetBlendDesc::equals(lhs.renderTargets[i], rhs.renderTargets[i])) {
			return false;
		}
	}
	return lhs.independentBlendEnable == rhs.independentBlendEnable;
}

//==============================================================================
// RasterizerStateDesc

RasterizerStateDesc::RasterizerStateDesc()
    : fillMode(FillMode::Solid)
    , cullMode(CullMode::Back)
{
}

bool RasterizerStateDesc::equals(const RasterizerStateDesc& lhs, const RasterizerStateDesc& rhs)
{
	return
		lhs.fillMode == rhs.fillMode &&
		lhs.cullMode == rhs.cullMode;
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

bool StencilOpDesc::equals(const StencilOpDesc& lhs, const StencilOpDesc& rhs)
{
	return
		lhs.stencilFailOp == rhs.stencilFailOp &&
		lhs.stencilDepthFailOp == rhs.stencilDepthFailOp &&
		lhs.stencilPassOp == rhs.stencilPassOp &&
		lhs.stencilFunc == rhs.stencilFunc;
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

bool DepthStencilStateDesc::equals(const DepthStencilStateDesc& lhs, const DepthStencilStateDesc& rhs)
{
	return
		lhs.depthTestFunc == rhs.depthTestFunc &&
		lhs.depthWriteEnabled == rhs.depthWriteEnabled &&
		lhs.stencilEnabled == rhs.stencilEnabled &&
		lhs.stencilReferenceValue == rhs.stencilReferenceValue &&
		StencilOpDesc::equals(lhs.frontFace, rhs.frontFace) &&
		StencilOpDesc::equals(lhs.backFace, rhs.backFace);
}

} // namespace ln
