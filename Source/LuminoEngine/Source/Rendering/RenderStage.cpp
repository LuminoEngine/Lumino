
#include "../Internal.h"
#include <Lumino/Graphics/Brush.h>
#include <Lumino/Graphics/Text/Font.h>
#include "RenderStage.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// RenderingContextParameters
//==============================================================================
void RenderingContextParameters::reset()
{
	std::fill(m_renderTargets, m_renderTargets + Graphics::MaxMultiRenderTargets, nullptr);
	m_depthBuffer = nullptr;
	m_viewportRect = RectI::Empty;
	m_scissorRect = RectI::Empty;
	m_blendMode = BlendMode::Normal;	// TODO: Default
	m_cullingMode = CullingMode::Back;	// TODO: Default
	m_depthTestEnabled = true;
	m_depthWriteEnabled = true;

	m_transfrom = Matrix::Identity;

	m_brush = nullptr;
	m_font = nullptr;
	m_material = nullptr;
	//m_priorityMaterial = nullptr;

	m_hashDirty = true;
}

void RenderingContextParameters::copyFrom(const RenderingContextParameters* other)
{
	std::copy(other->m_renderTargets, other->m_renderTargets + Graphics::MaxMultiRenderTargets, m_renderTargets);
	m_depthBuffer = other->m_depthBuffer;
	m_viewportRect = other->m_viewportRect;
	m_scissorRect = other->m_scissorRect;
	m_blendMode = other->m_blendMode;
	m_cullingMode = other->m_cullingMode;
	m_depthTestEnabled = other->m_depthTestEnabled;
	m_depthWriteEnabled = other->m_depthWriteEnabled;

	m_transfrom = other->m_transfrom;

	m_brush = other->m_brush;
	m_font = other->m_font;
	m_material = other->m_material;
	//m_priorityMaterial = other->m_priorityMaterial;

	m_hashDirty = true;
}

void RenderingContextParameters::setRenderTarget(int index, RenderTargetTexture* renderTarget)
{
	if (m_renderTargets[index] != renderTarget)
	{
		m_renderTargets[index] = renderTarget;
		m_hashDirty = true;
	}
}

void RenderingContextParameters::setDepthBuffer(DepthBuffer* depthBuffer)
{
	if (m_depthBuffer != depthBuffer)
	{
		m_depthBuffer = depthBuffer;
		m_hashDirty = true;
	}
}

void RenderingContextParameters::setViewportRect(const RectI& rect)
{
	if (m_viewportRect != rect)
	{
		m_viewportRect = rect;
		m_hashDirty = true;
	}
}

void RenderingContextParameters::setScissorRect(const RectI& scissorRect)
{
	if (m_scissorRect != scissorRect)
	{
		m_scissorRect = scissorRect;
		m_hashDirty = true;
	}
}

void RenderingContextParameters::setBlendMode(const Nullable<BlendMode>& mode)
{
	if (m_blendMode != mode)
	{
		m_blendMode = mode;
		m_hashDirty = true;
	}
}

void RenderingContextParameters::setCullingMode(const Nullable<CullingMode>& mode)
{
	if (m_cullingMode != mode)
	{
		m_cullingMode = mode;
		m_hashDirty = true;
	}
}

void RenderingContextParameters::setDepthTestEnabled(const Nullable<bool>& enabled)
{
	if (m_depthTestEnabled != enabled)
	{
		m_depthTestEnabled = enabled;
		m_hashDirty = true;
	}
}

void RenderingContextParameters::setDepthWriteEnabled(const Nullable<bool>& enabled)
{
	if (m_depthWriteEnabled != enabled)
	{
		m_depthWriteEnabled = enabled;
		m_hashDirty = true;
	}
}

void RenderingContextParameters::setTransform(const Matrix& value)
{
	if (m_transfrom != value)
	{
		m_transfrom = value;
		m_hashDirty = true;
	}
}

void RenderingContextParameters::setMaterial(CommonMaterial* value)
{
	if (m_material != value)
	{
		m_material = value;
		m_hashDirty = true;
	}
}

void RenderingContextParameters::setBrush(Brush* brush)
{
	if (m_brush != brush)
	{
		m_brush = brush;
		m_hashDirty = true;
	}
}

Brush* RenderingContextParameters::getBrush() const
{
	return m_brush;
}

void RenderingContextParameters::setFont(Font* font)
{
	if (m_font != font)
	{
		m_font = font;
		m_hashDirty = true;
	}
}

Font* RenderingContextParameters::getFont() const
{
	return m_font;
}

//void RenderingContextParameters::setPriorityMaterial(CommonMaterial* value)
//{
//	if (m_priorityMaterial != value)
//	{
//		m_priorityMaterial = value;
//		m_hashDirty = true;
//	}
//}

uint32_t RenderingContextParameters::getHashCode() const
{
	if (m_hashDirty)
	{
		m_hashCode = 0;
		m_hashDirty = false;
		m_hashCode = Hash::calcHash(reinterpret_cast<const char*>(this), sizeof(RenderingContextParameters));
	}
	return m_hashCode;
}

//==============================================================================
// RenderStage
//==============================================================================
RenderStage::RenderStage()
	: m_id(0)
{
}

RenderStage::~RenderStage()
{
}

void RenderStage::initialize(int id)
{
	Object::initialize();
	m_id = id;
	reset();
}

void RenderStage::reset()
{
	visualNodeParameters = nullptr;
	renderingContextParameters.reset();
}

void RenderStage::copyFrom(const RenderStage* other)
{
	visualNodeParameters = other->visualNodeParameters;
	renderingContextParameters.copyFrom(&other->renderingContextParameters);
}

bool RenderStage::equals(const RenderStage* other) const
{
	if (visualNodeParameters != other->visualNodeParameters) return false;

	return true;
}

void RenderStage::combineParameters()
{
	if (visualNodeParameters)
	{
		m_combinedWorldMatrix = visualNodeParameters->transfrom;
		m_combinedWorldMatrix *= renderingContextParameters.getTransform();
	}
	else
	{
		m_combinedWorldMatrix = renderingContextParameters.getTransform();
	}
}

BlendMode RenderStage::getBlendModeFinal() const
{
	// specified context->setXXXX()
	if (renderingContextParameters.getBlendMode().isSet())
		return renderingContextParameters.getBlendMode();

	// specified meshObj->setXXXX()
	if (visualNodeParameters && visualNodeParameters->blendMode.isSet())
		return visualNodeParameters->blendMode;

	// default
	return BlendMode::Normal;
}

CullingMode RenderStage::getCullingModeFinal() const
{
	// specified context->setXXXX()
	if (renderingContextParameters.getCullingMode().isSet())
		return renderingContextParameters.getCullingMode();

	// specified meshObj->setXXXX()
	if (visualNodeParameters && visualNodeParameters->cullingMode.isSet())
		return visualNodeParameters->cullingMode;

	// default
	return CullingMode::Back;
}

bool RenderStage::isDepthTestEnabledFinal() const
{
	// specified context->setXXXX()
	if (renderingContextParameters.isDepthTestEnabled().isSet())
		return renderingContextParameters.isDepthTestEnabled();

	// specified meshObj->setXXXX()
	if (visualNodeParameters && visualNodeParameters->depthTestEnabled.isSet())
		return visualNodeParameters->depthTestEnabled;

	// default
	return true;
}

bool RenderStage::isDepthWriteEnabledFinal() const
{
	// specified context->setXXXX()
	if (renderingContextParameters.isDepthWriteEnabled().isSet())
		return renderingContextParameters.isDepthWriteEnabled();

	// specified meshObj->setXXXX()
	if (visualNodeParameters && visualNodeParameters->depthWriteEnabled.isSet())
		return visualNodeParameters->depthWriteEnabled;

	// default
	return true;
}

CommonMaterial* RenderStage::getMaterialFinal(CommonMaterial* defaultValue, CommonMaterial* priorityValue) const
{
	// specified drawXXXX()
	if (priorityValue) return priorityValue;

	// specified context->setMaterial()
	if (renderingContextParameters.getMaterial()) return renderingContextParameters.getMaterial();

	// specified meshObj->setMaterial()
	if (visualNodeParameters && visualNodeParameters->material) return visualNodeParameters->material;

	// default は SceneRenderer に決めてもらう
	return defaultValue;
}

Shader* RenderStage::getShaderFinal(CommonMaterial* finalMaterial) const
{
	Shader* shader = nullptr;

	// specified meshObj->setShader()
	shader = (visualNodeParameters) ? visualNodeParameters->builtinEffectData.getShader() : nullptr;
	if (shader) return shader;

	// specified meshObj->getMaterial(0)->setShader() etc...
	shader = finalMaterial->getShader();
	if (shader) return shader;

	return nullptr;
}

ShadingModel RenderStage::getShadingModelFinal(CommonMaterial* finalMaterial) const
{
	LN_ASSERT(finalMaterial);

	// specified meshObj->setXXXX()
	if (visualNodeParameters && visualNodeParameters->shadingModel.isSet())
		return visualNodeParameters->shadingModel;

	//  specified meshObj->getMaterial(0)->setXXXX() etc...
	return finalMaterial->shadingModel;
}

//==============================================================================
// RenderStageCache
//==============================================================================
RenderStageCache::RenderStageCache()
	: m_instanceList()
	, m_used(0)
{
}

void RenderStageCache::initialize(int reserved)
{
	for (int i = 0; i < reserved; i++)
	{
		m_instanceList.add(newObject<RenderStage>(m_instanceList.getCount()));
	}
	m_used = 0;
}

void RenderStageCache::clear()
{
	m_used = 0;
}

RenderStage* RenderStageCache::request()
{
	if (m_used >= m_instanceList.getCount())
	{
		for (int i = 0; i < m_used; i++)
		{
			m_instanceList.add(newObject<RenderStage>(m_instanceList.getCount()));
		}
	}
	m_used++;
	return m_instanceList[m_used - 1];
}

RenderStage* RenderStageCache::get(int id) const
{
	return m_instanceList[id];
}

} // namespace detail
LN_NAMESPACE_END

