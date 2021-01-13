
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include "RLIMaterial.hpp"

namespace ln {
namespace detail {

RLIMaterial::RLIMaterial()
{
	reset();
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

void RLIMaterial::mergeFrom(const GeometryStageParameters* geometoryParams, Material* finalMaterial)
{
	assert(geometoryParams);

	material = finalMaterial;

	// BlendState
	{
		if (finalMaterial && finalMaterial->getBlendMode().hasValue())
			blendMode = finalMaterial->getBlendMode().value();
		// specified context->setXXXX() or meshObj->setXXXX()
		else if (geometoryParams->m_blendMode.hasValue())
			blendMode = geometoryParams->m_blendMode.value();
		// default
		else
			blendMode = BlendMode::Normal;
	}
	// RasterizerState
	{
		if (finalMaterial && finalMaterial->getCullingMode().hasValue())
			cullingMode = finalMaterial->getCullingMode().value();
		// specified context->setXXXX() or meshObj->setXXXX()
		else if (geometoryParams->m_cullingMode.hasValue())
			cullingMode = geometoryParams->m_cullingMode.value();
		// default
		else
			cullingMode = CullMode::Back;
	}
	// DepthStencilState
	{
		if (finalMaterial && finalMaterial->isDepthTestEnabled().hasValue())
			depthTestEnabled = (finalMaterial->isDepthTestEnabled().value());// ? ComparisonFunc::LessEqual : ComparisonFunc::Always;
		// specified context->setXXXX() or meshObj->setXXXX()
		else if (geometoryParams->m_depthTestEnabled.hasValue())
			depthTestEnabled = (geometoryParams->m_depthTestEnabled.value());// ? ComparisonFunc::LessEqual : ComparisonFunc::Always;
		// default
		else
			depthTestEnabled = true;// ComparisonFunc::LessEqual;


	}
	{
		if (finalMaterial && finalMaterial->isDepthWriteEnabled().hasValue())
			depthWriteEnabled = finalMaterial->isDepthWriteEnabled().value();
		// specified context->setXXXX() or meshObj->setXXXX()
		else if (geometoryParams->m_depthWriteEnabled.hasValue())
			depthWriteEnabled = geometoryParams->m_depthWriteEnabled.value();
		// default
		else
			depthWriteEnabled = true;
	}
	{
		// specified meshObj->setXXXX()
		if (geometoryParams && geometoryParams->shadingModel.hasValue())
			shadingModel = geometoryParams->shadingModel.value();
		// specified meshObj->getMaterial(0)->setXXXX() etc...
		if (finalMaterial)
			shadingModel = finalMaterial->shadingModel();
		else
			shadingModel = ShadingModel::Default;
	}
	// Topology
	{
		primitiveTopology = geometoryParams->primitiveTopology;
	}
}

} // namespace detail
} // namespace ln

