
#include "Internal.hpp"
#include <LuminoGraphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include "RLIBatchState.hpp"
#include "RLIMaterial.hpp"

namespace ln {
namespace detail {

RLIMaterial::RLIMaterial()
{
	reset();
}

RLIMaterial::RLIMaterial(const RLIBatchState& src)
	: material(src.material)
	, blendMode(src.blendMode)
	, cullingMode(src.cullingMode)
	, depthTestEnabled(src.depthTestEnabled)
	, depthWriteEnabled(src.depthWriteEnabled)
	, shadingModel(src.shadingModel)
	, primitiveTopology(src.primitiveTopology)
{
}

void RLIMaterial::reset()
{
	material = nullptr;
	blendMode = BlendMode::Normal;
	cullingMode = CullMode::Front;
	depthTestEnabled = true;
	depthWriteEnabled = true;
	shadingModel = ShadingModel::Default;
	primitiveTopology = PrimitiveTopology::TriangleList;
}

bool RLIMaterial::equals(const RLIMaterial* other) const
{
	return
		material == other->material &&  // TODO: Material が一致することはまずない。ちゃんと中身かhashを見ること。
		blendMode == other->blendMode &&
		cullingMode == other->cullingMode &&
		depthTestEnabled == other->depthTestEnabled &&
		depthWriteEnabled == other->depthWriteEnabled &&
		shadingModel == other->shadingModel &&
		primitiveTopology == other->primitiveTopology;
}

void RLIMaterial::applyRenderStates(GraphicsContext* context) const
{
	// BlendState
	{
		BlendStateDesc state;
		state.independentBlendEnable = false;
		makeBlendMode(blendMode, &state.renderTargets[0]);
		context->setBlendState(state);
	}
	// RasterizerState
	{
		RasterizerStateDesc state;
		state.fillMode = FillMode::Solid;
		state.cullMode = cullingMode;
		context->setRasterizerState(state);
	}
	// DepthStencilState
	{
		DepthStencilStateDesc state;
		state.depthTestFunc = (depthTestEnabled) ? ComparisonFunc::LessEqual : ComparisonFunc::Always;
		state.depthWriteEnabled = depthWriteEnabled;
		context->setDepthStencilState(state);
	}
	// Topology
	{
		context->setPrimitiveTopology(primitiveTopology);
	}
}

void RLIMaterial::makeBlendMode(BlendMode mode, RenderTargetBlendDesc* state)
{
	// もっといろいろ http://d.hatena.ne.jp/Ko-Ta/20070618/p1
	// TODO: アルファも一緒のブレンド方式にしているので、個別指定で改善できそう
	switch (mode)
	{
	case BlendMode::Normal:
		state->blendEnable = false;
		state->sourceBlend = BlendFactor::One;
		state->destinationBlend = BlendFactor::Zero;
		state->blendOp = BlendOp::Add;
		state->sourceBlendAlpha = BlendFactor::One;
		state->destinationBlendAlpha = BlendFactor::Zero;
		state->blendOpAlpha = BlendOp::Add;
		break;
	case BlendMode::Alpha:
		state->blendEnable = true;
		state->sourceBlend = BlendFactor::SourceAlpha;
		state->destinationBlend = BlendFactor::InverseSourceAlpha;
		state->blendOp = BlendOp::Add;
		state->sourceBlendAlpha = BlendFactor::SourceAlpha;
		//state->destinationBlendAlpha = BlendFactor::InverseSourceAlpha;
        state->destinationBlendAlpha = BlendFactor::DestinationAlpha;
		state->blendOpAlpha = BlendOp::Add;
		break;
	case BlendMode::Add:
		state->blendEnable = true;
		state->sourceBlend = BlendFactor::SourceAlpha;
		state->destinationBlend = BlendFactor::One;
		state->blendOp = BlendOp::Add;
		state->sourceBlendAlpha = BlendFactor::SourceAlpha;
		state->destinationBlendAlpha = BlendFactor::One;
		state->blendOpAlpha = BlendOp::Add;
		break;
	case BlendMode::Subtract:
		state->blendEnable = true;
		state->sourceBlend = BlendFactor::SourceAlpha;
		state->destinationBlend = BlendFactor::One;
		state->blendOp = BlendOp::ReverseSubtract;
		state->sourceBlendAlpha = BlendFactor::SourceAlpha;
		state->destinationBlendAlpha = BlendFactor::One;
		state->blendOpAlpha = BlendOp::Add;
		break;
	case BlendMode::Multiply:
		state->blendEnable = true;
		state->sourceBlend = BlendFactor::Zero;	// AlphaDisable (Alpha を別指定できない今の仕様では Alpha を考慮できない)
		state->destinationBlend = BlendFactor::SourceColor;
		state->blendOp = BlendOp::Add;
		state->sourceBlendAlpha = BlendFactor::SourceAlpha;
		state->destinationBlendAlpha = BlendFactor::One;
		state->blendOpAlpha = BlendOp::Add;
		break;
		//case BlendMode_Screen:
		//	m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//	m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//	m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHASAT);
		//	m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		//	m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 255);
		//	break;
		//case BlendMode_Reverse:
		//	m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//	m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//	m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//	m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
		//	m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 1);
		//	break;
	default:
		assert(0);
		break;
	}
}

} // namespace detail
} // namespace ln

