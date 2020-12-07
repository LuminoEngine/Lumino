
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include "RenderingManager.hpp"
#include "DrawElementListBuilder.hpp"

namespace ln {
namespace detail {

//==============================================================================
// DrawElementListBuilder

DrawElementListBuilder::DrawElementListBuilder()
    : m_manager(EngineDomain::renderingManager())
    , m_currentCommandFence(0)
{
}

DrawElementListBuilder::~DrawElementListBuilder()
{
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
    m_currentCommandFence = 0;
}

void DrawElementListBuilder::reset2()
{
    primaryState()->reset();
    m_dirtyFlags = DirtyFlags::All;
	m_modified = true;
}

void DrawElementListBuilder::advanceFence()
{
    m_currentCommandFence++;
}

void DrawElementListBuilder::setRenderTarget(int index, RenderTargetTexture * value)
{
	if (primaryFrameBufferStageParameters().m_renderTargets[index] != value) {
		primaryFrameBufferStageParameters().m_renderTargets[index] = value;
		m_modified = true;
        advanceFence();
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
        advanceFence();
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

void DrawElementListBuilder::setObjectId(int value)
{
    //if (primaryGeometryStageParameters().m_objectId != value) {
    //    primaryGeometryStageParameters().m_objectId = value;
    //    m_modified = true;
    //}
    m_objectId = value;
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

void DrawElementListBuilder::setPrimitiveTopology(PrimitiveTopology value)
{
	if (primaryGeometryStageParameters().primitiveTopology != value) {
		primaryGeometryStageParameters().primitiveTopology = value;
		m_modified = true;	// TODO: 最後に確定したもの (m_targetList->lastState() ) との差で付けたい
	}
}

void DrawElementListBuilder::setShadingModel(const Optional<ShadingModel>& value)
{
	if (primaryGeometryStageParameters().shadingModel != value) {
		primaryGeometryStageParameters().shadingModel = value;
		m_modified = true;
	}
}

void DrawElementListBuilder::setMaterial(Material* value)
{
    if (!value) {
        value = m_manager->defaultMaterial();
    }

	if (primaryGeometryStageParameters().m_material != value) {
		primaryGeometryStageParameters().m_material = value;
		m_modified = true;
	}
}

Material* DrawElementListBuilder::material() const
{
    return primaryGeometryStageParameters().m_material;
}

void DrawElementListBuilder::setTransfrom(const Matrix & value)
{
    primaryState()->transform = value;
}

void DrawElementListBuilder::setBaseTransfrom(const Optional<Matrix>& value)
{
    primaryState()->baseTransform = value;
}

const Matrix& DrawElementListBuilder::baseTransform() const
{
	auto v = primaryStateConst()->baseTransform;
	if (v)
		return primaryStateConst()->baseTransform.value();
	else
		return Matrix::Identity;
}

void DrawElementListBuilder::setRenderPriority(int value)
{
    primaryState()->renderPriority = value;
}

void DrawElementListBuilder::setRenderPhase(RenderPart value)
{
    primaryState()->rendringPhase = value;
}

RenderPart DrawElementListBuilder::renderPhase()
{
    return primaryState()->rendringPhase;
}

void DrawElementListBuilder::setAdditionalElementFlags(RenderDrawElementTypeFlags value)
{
	primaryState()->additionalElementFlags = value;
}

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

void DrawElementListBuilder::setTone(const ColorTone& value)
{
	if (primaryState()->builtinEffectData.tone != value) {
        primaryState()->builtinEffectData.tone = value;
        m_dirtyFlags.set(DirtyFlags::BuiltinEffect);
	}
}

void DrawElementListBuilder::setBaseBuiltinEffectData(const Optional<BuiltinEffectData>& value)
{
    bool modify = false;
    if (primaryState()->baseBuiltinEffectData.hasValue() != value.hasValue()) {
        modify = true;
    }
    else if (!primaryState()->baseBuiltinEffectData.hasValue()) {
        // 両方値を持っていない
    }
    else if (!primaryState()->baseBuiltinEffectData.value().equals(&value.value())) {
        modify = true;
    }

    if (modify) {
        primaryState()->baseBuiltinEffectData = value;
        m_dirtyFlags.set(DirtyFlags::BuiltinEffect);
    }
}

void DrawElementListBuilder::setFont(Font* value)
{
	primaryState()->font = value;
}

void DrawElementListBuilder::setTextColor(const Color& value)
{
	primaryState()->textColor = value;
}

void DrawElementListBuilder::setViewPoint(RenderViewPoint* value)
{
    primaryState()->viewPoint = value;
}

RenderViewPoint* DrawElementListBuilder::viewPoint() const
{
    return primaryStateConst()->viewPoint;
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
        state->copyFrom(primaryState());
    }

    m_aliveStateStack.add(state);

    if (reset) {
        this->reset2();

		// TODO: もうちょいスマートな方法あると思うけど…
		//if (m_stateStackMode == StateStackMode::ScissorPushPop && m_aliveStateStack.size() >= 2) {
		//	auto& prevState = m_aliveStateStack[m_aliveStateStack.size() - 2];
		//	setScissorRect(state->frameBufferStageParameters.m_scissorRect);
		//}
		if (m_stateStackMode == StateStackMode::ScissorPushPop) {
			setScissorRect(state->frameBufferStageParameters.m_scissorRect);
		}
    }
}

void DrawElementListBuilder::popState()
{
    auto state = m_aliveStateStack.back();
    m_aliveStateStack.removeLast();
    m_freeStateStack.add(state);

	if (m_stateStackMode == StateStackMode::ScissorPushPop && !m_aliveStateStack.isEmpty()) {
		setScissorRect(m_aliveStateStack.back()->frameBufferStageParameters.m_scissorRect);
	}
}

RenderStage* DrawElementListBuilder::prepareRenderStage(RenderFeature* renderFeature)
{
	RenderStage* lastStage = nullptr;
	RenderStage* newStage = nullptr;
	if (m_targetList->isEmpty()) {
		newStage = m_targetList->addNewRenderStage();
	}
	else {
		lastStage = m_targetList->last();

		if (m_modified) {
			newStage = m_targetList->addNewRenderStage();
		}
	}

	m_modified = false;

	if (newStage) {
		// ステートが変化したので新しい Stage が作られた。setup して返す
		newStage->renderFeature = renderFeature;

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

        newStage->m_objectId = m_objectId;

		return newStage;
	}
	else {
		// ステートに変化がないので、最後の Stage に追記できる
		return m_targetList->last();
	}
}

void DrawElementListBuilder::prepareRenderDrawElement(RenderDrawElement* newElement, RenderDrawElement* lastElement, RenderStage* stage)
{
    if (LN_REQUIRE(stage)) return;

    newElement->m_stage = stage;

    // newElement が持つべき BuiltinEffectData を決定する
    BuiltinEffectData* data = nullptr;
    if (m_dirtyFlags.hasFlag(DirtyFlags::BuiltinEffect)) {
        // make new
    }
    else {
        if (lastElement) {
            data = lastElement->builtinEffectData;
        }
        else {
            // make new
        }
    }
    if (!data) {
        data = m_targetList->newFrameData<BuiltinEffectData>(primaryState()->builtinEffectData);
        if (primaryState()->baseBuiltinEffectData.hasValue()) {
            data->inherit(primaryState()->baseBuiltinEffectData.value());
        }
    }

    newElement->builtinEffectData = data;
    m_dirtyFlags.unset(DirtyFlags::BuiltinEffect);

    // transform
    if (primaryState()->baseTransform.hasValue()) {
        newElement->m_combinedWorldMatrix = primaryState()->transform * primaryState()->baseTransform.value();
    }
    else {
        newElement->m_combinedWorldMatrix = primaryState()->transform;
    }

    newElement->priority = primaryState()->renderPriority;
    newElement->targetPhase = primaryState()->rendringPhase;
	newElement->fixFlags(primaryState()->additionalElementFlags);
    newElement->commandFence = m_currentCommandFence;
}


void DrawElementListBuilder::State::reset()
{
    frameBufferStageParameters.reset();
    geometryStageParameters.reset();
    builtinEffectData = BuiltinEffectData::DefaultValue;
    baseBuiltinEffectData = nullptr;
    transform = Matrix::Identity;
    renderPriority = 0;
    baseTransform = nullptr;
    rendringPhase = RenderPart::Geometry;
	additionalElementFlags = RenderDrawElementTypeFlags::None;
	font = nullptr;
	textColor = Color::Black;
}

void DrawElementListBuilder::State::copyFrom(const State* other)
{
    frameBufferStageParameters.copyFrom(other->frameBufferStageParameters);
    geometryStageParameters.copyFrom(other->geometryStageParameters);
    builtinEffectData = other->builtinEffectData;
    baseBuiltinEffectData = other->baseBuiltinEffectData;
    transform = other->transform;
    renderPriority = other->renderPriority;
    baseTransform = other->baseTransform;
    rendringPhase = other->rendringPhase;
	additionalElementFlags = other->additionalElementFlags;
}

} // namespace detail
} // namespace ln

