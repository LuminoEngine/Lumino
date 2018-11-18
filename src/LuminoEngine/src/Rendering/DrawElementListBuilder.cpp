
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include "DrawElementListBuilder.hpp"

namespace ln {
namespace detail {

//==============================================================================
// DrawElementListBuilder

DrawElementListBuilder::DrawElementListBuilder()
{
    m_defaultMaterial = newObject<Material>();
}

void DrawElementListBuilder::setTargetList(DrawElementList * targetList)
{
	m_targetList = targetList;

    // とりあえず 10 個くらい
    for (int i = 0; i < 10; i++) {
        m_freeStateStack.add(makeRef<State>());
    }
}

void DrawElementListBuilder::resetForBeginRendering()
{
    for (auto s : m_aliveStateStack)
    {
        m_freeStateStack.add(s);
    }
    m_aliveStateStack.clear();
    pushState(true);	// 1つスタックに積んでおく。コレがルートのステート
}

void DrawElementListBuilder::reset2()
{
	primaryFrameBufferStageParameters().reset();
	primaryGeometryStageParameters().reset();
    m_dirtyFlags = DirtyFlags::All;
	m_modified = true;
}

void DrawElementListBuilder::setRenderTarget(int index, RenderTargetTexture * value)
{
	if (primaryFrameBufferStageParameters().m_renderTargets[index] != value) {
		primaryFrameBufferStageParameters().m_renderTargets[index] = value;
		m_modified = true;
	}
}

RenderTargetTexture* DrawElementListBuilder::renderTarget(int index) const
{
    return primaryFrameBufferStageParameters().m_renderTargets[index];
}

void DrawElementListBuilder::setDepthBuffer(DepthBuffer * value)
{
	if (primaryFrameBufferStageParameters().m_depthBuffer != value) {
		primaryFrameBufferStageParameters().m_depthBuffer = value;
		m_modified = true;
	}
}

void DrawElementListBuilder::setViewportRect(const RectI & value)
{
	if (primaryFrameBufferStageParameters().m_viewportRect != value) {
		primaryFrameBufferStageParameters().m_viewportRect = value;
		m_modified = true;
	}
}

void DrawElementListBuilder::setScissorRect(const RectI & value)
{
	if (primaryFrameBufferStageParameters().m_scissorRect != value) {
		primaryFrameBufferStageParameters().m_scissorRect = value;
		m_modified = true;
	}
}

void DrawElementListBuilder::setBlendMode(const Optional<BlendMode>& value)
{
	if (primaryGeometryStageParameters().m_blendMode != value) {
		primaryGeometryStageParameters().m_blendMode = value;
		m_modified = true;
	}
}

void DrawElementListBuilder::setCullingMode(const Optional<CullMode>& value)
{
	if (primaryGeometryStageParameters().m_cullingMode != value) {
		primaryGeometryStageParameters().m_cullingMode = value;
		m_modified = true;
	}
}

void DrawElementListBuilder::setDepthTestEnabled(const Optional<bool>& value)
{
	if (primaryGeometryStageParameters().m_depthTestEnabled != value) {
		primaryGeometryStageParameters().m_depthTestEnabled = value;
		m_modified = true;
	}
}

void DrawElementListBuilder::setDepthWriteEnabled(const Optional<bool>& value)
{
	if (primaryGeometryStageParameters().m_depthWriteEnabled != value) {
		primaryGeometryStageParameters().m_depthWriteEnabled = value;
		m_modified = true;
	}
}

void DrawElementListBuilder::setShadingModel(const Optional<ShadingModel>& value)
{
	if (primaryGeometryStageParameters().shadingModel != value) {
		primaryGeometryStageParameters().shadingModel = value;
		m_modified = true;
	}
}

void DrawElementListBuilder::setMaterial(AbstractMaterial* value)
{
    if (!value) {
        value = m_defaultMaterial;
    }

	if (primaryGeometryStageParameters().m_material != value) {
		primaryGeometryStageParameters().m_material = value;
		m_modified = true;
	}
}

//void DrawElementListBuilder::setTransfrom(const Matrix & value)
//{
//	if (primaryGeometryStageParameters().builtinEffectData.m_transfrom != value) {
//		primaryGeometryStageParameters().builtinEffectData.m_transfrom = value;
//		m_modified = true;
//	}
//}

void DrawElementListBuilder::setOpacity(float value)
{
	if (primaryState()->builtinEffectData.opacity != value) {
        primaryState()->builtinEffectData.opacity = value;
		m_dirtyFlags.set(DirtyFlags::BuiltinEffect);
	}
}

void DrawElementListBuilder::setColorScale(const Color & value)
{
	if (primaryState()->builtinEffectData.colorScale != value) {
        primaryState()->builtinEffectData.colorScale = value;
        m_dirtyFlags.set(DirtyFlags::BuiltinEffect);
	}
}

void DrawElementListBuilder::setBlendColor(const Color & value)
{
	if (primaryState()->builtinEffectData.blendColor != value) {
        primaryState()->builtinEffectData.blendColor = value;
        m_dirtyFlags.set(DirtyFlags::BuiltinEffect);
	}
}

void DrawElementListBuilder::setTone(const ToneF & value)
{
	if (primaryState()->builtinEffectData.tone != value) {
        primaryState()->builtinEffectData.tone = value;
        m_dirtyFlags.set(DirtyFlags::BuiltinEffect);
	}
}

void DrawElementListBuilder::pushState(bool reset)
{
    Ref<State> state;
    if (m_freeStateStack.isEmpty()) {
        state = makeRef<State>();
    }
    else {
        state = m_freeStateStack.back();
        m_freeStateStack.removeLast();
    }

    if (m_aliveStateStack.isEmpty()) {
        // 描画開始時に1つ積んでおくため
    }
    else {
        // 現在のステートを保持
        state->frameBufferStageParameters.copyFrom(primaryFrameBufferStageParameters());
        state->geometryStageParameters.copyFrom(primaryGeometryStageParameters());
    }

    m_aliveStateStack.add(state);

    if (reset) {
        this->reset2();
    }
}

void DrawElementListBuilder::popState()
{
    auto state = m_aliveStateStack.back();
    m_aliveStateStack.removeLast();
    m_freeStateStack.add(state);
}

RenderStage* DrawElementListBuilder::prepareRenderStage(RenderFeature* renderFeature, RenderFeatureStageParameters * featureParams)
{
	RenderStage* lastStage = nullptr;
	RenderStage* newStage = nullptr;
	if (m_targetList->isEmpty()) {
		newStage = m_targetList->addNewRenderStage();
	}
	else {
		lastStage = m_targetList->last();

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

        // FrameBufferStageParameters と GeometryStageParameters は、内容が本当に変更されていなければ、
        // メモリ節約のためにひとつ前の Stage が持っているものとインスタンスを共有する。
		bool sharedFrameBufferStageParameters = false;
		bool sharedGeometryStageParameters = false;
		if (lastStage) {
			sharedFrameBufferStageParameters = primaryFrameBufferStageParameters().equals(lastStage->frameBufferStageParameters);
			sharedGeometryStageParameters = primaryGeometryStageParameters().equals(lastStage->geometryStageParameters);
		}
		if (!sharedFrameBufferStageParameters) {
			newStage->frameBufferStageParameters = m_targetList->newFrameData<FrameBufferStageParameters>();
			*newStage->frameBufferStageParameters = primaryFrameBufferStageParameters();
		}
		if (!sharedGeometryStageParameters) {
			newStage->geometryStageParameters = m_targetList->newFrameData<GeometryStageParameters>();
			*newStage->geometryStageParameters = primaryGeometryStageParameters();
		}

		return newStage;
	}
	else {
		// ステートに変化がないので、最後の Stage に追記できる
		return m_targetList->last();
	}
}

void DrawElementListBuilder::prepareRenderDrawElement(RenderDrawElement* newElement, RenderDrawElement* lastElement)
{
    // newElement が持つべき BuiltinEffectData を決定する
    BuiltinEffectData* data = nullptr;
    if (m_dirtyFlags.hasFlag(DirtyFlags::BuiltinEffect)) {
        data = m_targetList->newFrameData<BuiltinEffectData>(primaryState()->builtinEffectData);
    }
    else {
        if (lastElement) {
            data = lastElement->builtinEffectData;
        }
        else {
            data = m_targetList->newFrameData<BuiltinEffectData>(primaryState()->builtinEffectData);
        }
    }

    newElement->builtinEffectData = data;
    m_dirtyFlags.unset(DirtyFlags::BuiltinEffect);
}

} // namespace detail
} // namespace ln

