
#include "Internal.hpp"
#include <Lumino/Rendering/Material.hpp>
#include "RenderStageListBuilder.hpp"

namespace ln {
namespace detail {

//==============================================================================
// RenderStageListBuilder

RenderStageListBuilder::RenderStageListBuilder()
{

}

void RenderStageListBuilder::setTargetList(DrawElementList * targetList)
{
	m_targetList = targetList;
}

void RenderStageListBuilder::reset()
{
	m_primaryFrameBufferStageParameters.reset();
	m_primaryGeometryStageParameters.reset();
	m_modified = true;
}

void RenderStageListBuilder::setRenderTarget(int index, RenderTargetTexture * value)
{
	if (m_primaryFrameBufferStageParameters.m_renderTargets[index] != value) {
		m_primaryFrameBufferStageParameters.m_renderTargets[index] = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setDepthBuffer(DepthBuffer * value)
{
	if (m_primaryFrameBufferStageParameters.m_depthBuffer != value) {
		m_primaryFrameBufferStageParameters.m_depthBuffer = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setViewportRect(const RectI & value)
{
	if (m_primaryFrameBufferStageParameters.m_viewportRect != value) {
		m_primaryFrameBufferStageParameters.m_viewportRect = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setScissorRect(const RectI & value)
{
	if (m_primaryFrameBufferStageParameters.m_scissorRect != value) {
		m_primaryFrameBufferStageParameters.m_scissorRect = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setBlendMode(const Optional<BlendMode>& value)
{
	if (m_primaryGeometryStageParameters.m_blendMode != value) {
		m_primaryGeometryStageParameters.m_blendMode = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setCullingMode(const Optional<CullingMode>& value)
{
	if (m_primaryGeometryStageParameters.m_cullingMode != value) {
		m_primaryGeometryStageParameters.m_cullingMode = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setDepthTestEnabled(const Optional<bool>& value)
{
	if (m_primaryGeometryStageParameters.m_depthTestEnabled != value) {
		m_primaryGeometryStageParameters.m_depthTestEnabled = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setDepthWriteEnabled(const Optional<bool>& value)
{
	if (m_primaryGeometryStageParameters.m_depthWriteEnabled != value) {
		m_primaryGeometryStageParameters.m_depthWriteEnabled = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setShadingModel(const Optional<ShadingModel>& value)
{
	if (m_primaryGeometryStageParameters.shadingModel != value) {
		m_primaryGeometryStageParameters.shadingModel = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setMaterial(AbstractMaterial * value)
{
	if (m_primaryGeometryStageParameters.m_material != value) {
		m_primaryGeometryStageParameters.m_material = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setTransfrom(const Matrix & value)
{
	if (m_primaryGeometryStageParameters.builtinEffectData.m_transfrom != value) {
		m_primaryGeometryStageParameters.builtinEffectData.m_transfrom = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setOpacity(float value)
{
	if (m_primaryGeometryStageParameters.builtinEffectData.opacity != value) {
		m_primaryGeometryStageParameters.builtinEffectData.opacity = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setColorScale(const Color & value)
{
	if (m_primaryGeometryStageParameters.builtinEffectData.colorScale != value) {
		m_primaryGeometryStageParameters.builtinEffectData.colorScale = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setBlendColor(const Color & value)
{
	if (m_primaryGeometryStageParameters.builtinEffectData.blendColor != value) {
		m_primaryGeometryStageParameters.builtinEffectData.blendColor = value;
		m_modified = true;
	}
}

void RenderStageListBuilder::setTone(const ToneF & value)
{
	if (m_primaryGeometryStageParameters.builtinEffectData.tone != value) {
		m_primaryGeometryStageParameters.builtinEffectData.tone = value;
		m_modified = true;
	}
}

RenderStage * RenderStageListBuilder::prepareRenderStage(RenderFeature* renderFeature, RenderFeatureStageParameters * featureParams)
{
	RenderStage* newStage = nullptr;
	if (m_targetList->isEmpty()) {
		newStage = m_targetList->addNewRenderStage();
	}
	else {
		RenderStage* lastStage = m_targetList->last();

		if (!m_modified)
		{
			if (lastStage->renderFeature != renderFeature) {
				m_modified = true;
			}
		}

		if (!m_modified)
		{
			// RenderFeatureStageParameters の変化確認
			if (lastStage->renderFeatureStageParameters == nullptr && featureParams != nullptr ||
				lastStage->renderFeatureStageParameters != nullptr && featureParams == nullptr) {
				// 新しく持ったり、あるいは外したりするときは変化あり
				m_modified = true;
			}
			else if (lastStage->renderFeatureStageParameters && featureParams &&
				!lastStage->renderFeatureStageParameters->equals(featureParams)) {
				m_modified = true;
			}
		}

		if (m_modified) {
			newStage = m_targetList->addNewRenderStage();
		}
	}

	m_modified = false;

	if (newStage) {
		// ステートが変化したので新しい Stage が作られた。setup して返す
		newStage->renderFeature = renderFeature;
		newStage->renderFeatureStageParameters = featureParams;
		return newStage;
	}
	else {
		// ステートに変化がないので、最後の Stage に追記できる
		return m_targetList->last();
	}
}

} // namespace detail
} // namespace ln

