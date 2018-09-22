#pragma once
#include "RenderStage.hpp"
#include "SpriteRenderFeature.hpp"

namespace ln {
namespace detail {

// CommandBuffer から使われることを想定。
// draw 系を呼んだときに、ステートが変化していれば新しい RenderStage を作ったり、
// 0.4.0 まで DrawList クラスがやってたことを担当する。
// CommandBuffer や DrawElementList と分離しているのは、
// - 一部のステートセットをユーザーから隠蔽したい
//		- → VisualNode のステートセット
// - メモリ効率をよくしたい
//		- → CommandBuffer や DrawElementList はたくさん作られることがあるが、ステート変化検出用の一時ステートを全部持ち続けるのは無駄。
//			DrawElementListBuilder は EngineDomain 内の唯一インスタンスでもよい。（並列化はできなくなるが・・・まぁ、その場合はインスタンスを複数作ればいいだけ）
class DrawElementListBuilder
	: public RefObject
{
public:
	DrawElementListBuilder();
	DrawElementList* targetList() const { return m_targetList; }
	void setTargetList(DrawElementList* targetList);
	void reset();

	void setRenderTarget(int index, RenderTargetTexture* value);
	void setDepthBuffer(DepthBuffer* value);
	void setViewportRect(const RectI& value);
	void setScissorRect(const RectI& value);

	void setBlendMode(const Optional<BlendMode>& value);
	void setCullingMode(const Optional<CullingMode>& value);
	void setDepthTestEnabled(const Optional<bool>& value);
	void setDepthWriteEnabled(const Optional<bool>& value);

	void setShadingModel(const Optional<ShadingModel>& value);
	void setMaterial(AbstractMaterial* value);

	// BuiltinEffectData
	//void setTransfrom(const Matrix& value);
	void setOpacity(float value);
	void setColorScale(const Color& value);
	void setBlendColor(const Color& value);
	void setTone(const ToneF& value);

	SpriteRenderFeatureStageParameters* spriteRenderFeatureStageParameters() { return &m_spriteRenderFeatureStageParameters; }

	template<class TElement>
	TElement* addNewDrawElement(
		RenderFeature* renderFeature,
		RenderFeatureStageParameters* params)
	{
		RenderStage* stage = prepareRenderStage(renderFeature, params);
		if (LN_ENSURE(stage)) return nullptr;
		TElement* element = m_targetList->newFrameData<TElement>();
		m_targetList->addElement(stage, element);
		return element;
	}

private:
	RenderStage* prepareRenderStage(RenderFeature* renderFeature, RenderFeatureStageParameters* featureParams);

	DrawElementList* m_targetList;
	FrameBufferStageParameters m_primaryFrameBufferStageParameters;
	GeometryStageParameters m_primaryGeometryStageParameters;

	// 以下、各 RenderFeature のステート。
	// これは m_modified 対象外。代わりに prepareRenderStage() のたびに equals() でチェックされる。
	// ユーザー定義する場合は外部でこれらを定義し、draw 時にそのポインタを指定する必要がある。
	// (もしフレームワークに沿った流れでなくても大丈夫ならグローバル変数とかで受け取ったりしても大丈夫)
	SpriteRenderFeatureStageParameters m_spriteRenderFeatureStageParameters;

	bool m_modified;
};

} // namespace detail
} // namespace ln

