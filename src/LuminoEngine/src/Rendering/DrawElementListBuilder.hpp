#pragma once
#include "RenderStage.hpp"
#include "BlitRenderFeature.hpp"
#include "SpriteRenderFeature.hpp"
#include "MeshRenderFeature.hpp"

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
    void resetForBeginRendering();  // 描画開始時のリセット。スタックもクリアする
	void reset2();   // スタックに積んである分はリセットしないしスタックも消さない

	void setRenderTarget(int index, RenderTargetTexture* value);
    RenderTargetTexture* renderTarget(int index) const;
	void setDepthBuffer(DepthBuffer* value);
	void setViewportRect(const RectI& value);
	void setScissorRect(const RectI& value);

	void setBlendMode(const Optional<BlendMode>& value);
	void setCullingMode(const Optional<CullMode>& value);
	void setDepthTestEnabled(const Optional<bool>& value);
	void setDepthWriteEnabled(const Optional<bool>& value);

	void setShadingModel(const Optional<ShadingModel>& value);
	void setMaterial(AbstractMaterial* value);  // 一度 set したマテリアルは描画完了まで変更してはならない。TODO: Freezed みたいな状態にしたい

	// BuiltinEffectData
	//void setTransfrom(const Matrix& value);
	void setOpacity(float value);
	void setColorScale(const Color& value);
	void setBlendColor(const Color& value);
	void setTone(const ToneF& value);

    void pushState(bool reset);
    void popState();

    BlitRenderFeatureStageParameters* blitRenderFeatureStageParameters() { return &m_blitRenderFeatureStageParameters; }
	SpriteRenderFeatureStageParameters* spriteRenderFeatureStageParameters() { return &m_spriteRenderFeatureStageParameters; }
	MeshRenderFeatureStageParameters* meshRenderFeatureStageParameters() { return &m_meshRenderFeatureStageParameters; }

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
    class State : public RefObject
    {
    public:
        FrameBufferStageParameters frameBufferStageParameters;
        GeometryStageParameters geometryStageParameters;
    };

	RenderStage* prepareRenderStage(RenderFeature* renderFeature, RenderFeatureStageParameters* featureParams);
    FrameBufferStageParameters& primaryFrameBufferStageParameters() { return m_aliveStateStack.front()->frameBufferStageParameters; }
    const FrameBufferStageParameters& primaryFrameBufferStageParameters() const { return m_aliveStateStack.front()->frameBufferStageParameters; }
    GeometryStageParameters& primaryGeometryStageParameters() { return m_aliveStateStack.front()->geometryStageParameters; }
    const GeometryStageParameters& primaryGeometryStageParameters() const { return m_aliveStateStack.front()->geometryStageParameters; }

	DrawElementList* m_targetList;
	//FrameBufferStageParameters m_primaryFrameBufferStageParameters;
	//GeometryStageParameters m_primaryGeometryStageParameters;
    List<Ref<State>> m_freeStateStack;
    List<Ref<State>> m_aliveStateStack;	// size >= 1


	// 以下、各 RenderFeature のステート。
	// これは m_modified 対象外。代わりに prepareRenderStage() のたびに equals() でチェックされる。
	// ユーザー定義する場合は外部でこれらを定義し、draw 時にそのポインタを指定する必要がある。
	// (もしフレームワークに沿った流れでなくても大丈夫ならグローバル変数とかで受け取ったりしても大丈夫)
    BlitRenderFeatureStageParameters m_blitRenderFeatureStageParameters;
	SpriteRenderFeatureStageParameters m_spriteRenderFeatureStageParameters;
	MeshRenderFeatureStageParameters m_meshRenderFeatureStageParameters;

	bool m_modified;
};

} // namespace detail
} // namespace ln

