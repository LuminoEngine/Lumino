#pragma once
#include <Lumino/Graphics/ColorStructs.hpp>
#include <Lumino/Graphics/RenderState.hpp>
#include <Lumino/Graphics/Texture.hpp>
#include <Lumino/Graphics/DepthBuffer.hpp>
#include "../Engine/LinearAllocator.hpp"

namespace ln {
class Shader;
class GraphicsContext;
class AbstractMaterial;
class RenderFeature;
namespace detail {
class RenderStage;

struct BuiltinEffectData
{
	Matrix						m_transfrom;
	//Ref<Shader>	shader;
	float opacity;
	Color colorScale;
	Color blendColor;
	ToneF tone;

	BuiltinEffectData()
	{
		reset();
	}

	void reset()
	{
		m_transfrom = Matrix();
		opacity = 1.0;
		colorScale = Color(1.0f, 1.0f, 1.0f, 1.0f);
		blendColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
		tone = ToneF(0.0f, 0.0f, 0.0f, 0.0f);
	}

	bool equals(const BuiltinEffectData* other) const
	{
		// TODO: hash
		return
			m_transfrom == other->m_transfrom &&
			opacity == other->opacity &&
			colorScale == other->colorScale &&
			blendColor == other->blendColor &&
			tone == other->tone;
	}
};

// インスタンスは RenderStageList の LinearAllocator に配置される。
// 描画が終わったら解放される。
class FrameBufferStageParameters
{
public:
	Ref<RenderTargetTexture>	m_renderTargets[MaxMultiRenderTargets];
	Ref<DepthBuffer>			m_depthBuffer;
	RectI						m_viewportRect;		// isEmpty() == true の場合は不使用
	RectI						m_scissorRect;		// isEmpty() == true の場合は不使用

	FrameBufferStageParameters()
	{
		reset();
	}

	void reset()
	{
		for (int i = 0; i < LN_ARRAY_SIZE_OF(m_renderTargets); i++) {
			m_renderTargets[i] = nullptr;
		}
		m_depthBuffer = nullptr;
		m_viewportRect = RectI::Empty;
		m_scissorRect = RectI::Empty;
	}

	bool equals(const FrameBufferStageParameters* other) const
	{
		for (int i = 0; i < LN_ARRAY_SIZE_OF(m_renderTargets); i++) {
			if (m_renderTargets[i] != other->m_renderTargets[i]) {
				return false;
			}
		}
		return
			m_depthBuffer == other->m_depthBuffer &&
			m_viewportRect == other->m_viewportRect &&
			m_scissorRect == other->m_scissorRect;
	}
};

// インスタンスは RenderStageList の LinearAllocator に配置される。
// 描画が終わったら解放される。
// GeometryStageParameters は
// - RenderingContext がもステート
// - VisualNode が持つステート
// の2か所で使われる。
// これは VisualNode::onRender() 内で、VisualNode が持つ State をベースとして RenderingContext を使えるようにするための仕組み。
// 例えば VisualNode::onRender() 内で RenderingContext::setOpacity() すると、VisualNode の opacity と乗算された結果が 最終結果となる。
class GeometryStageParameters
{
public:
	// まだ確定状態となるわけではないので Optional が含まれることもある。
	Optional<BlendMode>			m_blendMode;
	Optional<CullingMode>		m_cullingMode;
	Optional<bool>				m_depthTestEnabled;
	Optional<bool>				m_depthWriteEnabled;
	Ref<AbstractMaterial>			m_material;			// setMaterial() で指定されたマテリアル
	Optional<ShadingModel>		shadingModel;
	BuiltinEffectData builtinEffectData;

	GeometryStageParameters()
	{
		reset();
	}

	void reset()
	{
		m_blendMode = nullptr;
		m_cullingMode = nullptr;
		m_depthTestEnabled = nullptr;
		m_depthWriteEnabled = nullptr;
		m_material = nullptr;
		shadingModel = nullptr;
		builtinEffectData.reset();
	}

	bool equals(const GeometryStageParameters* other) const
	{
		return
			m_blendMode == other->m_blendMode &&
			m_cullingMode == other->m_cullingMode &&
			m_depthTestEnabled == other->m_depthTestEnabled &&
			m_depthWriteEnabled == other->m_depthWriteEnabled &&
			m_material == other->m_material &&
			shadingModel == other->shadingModel &&
			builtinEffectData.equals(&other->builtinEffectData);
	}
};

// インスタンスは RenderStageList の LinearAllocator に配置される。
// 描画が終わったら解放される。
class RenderFeatureStageParameters
{
public:
	virtual bool equals(const RenderFeatureStageParameters* other) = 0;
	virtual void copyTo(RenderFeatureStageParameters* params) = 0;

	//Ref<Brush>					m_brush;
	//Ref<Font>					m_font;
};

// インスタンスは RenderStageList の LinearAllocator に配置される。
// clear や draw 系のメソッド呼び出しをおこなう。
// ステートは変更するべきではない。
class RenderDrawElement
{
public:
	RenderDrawElement();
	virtual ~RenderDrawElement();

	// 描画実行。
	// 純粋に描画のみを行うこと。
	// ステートは RenderFeature::onStateChanged に通知済み。
	// もしどうしてもステートを変更したい場合、描画した後は必ず元に戻さなければならない。
	// この中で使えるのは GraphicsContext のみ。RenderingContext や Device 側の機能を呼び出してはならない。
	virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeature) = 0;

private:
	RenderStage* m_stage;
	RenderDrawElement* m_next;

	friend class RenderStage;
};

/*
 * RenderStage
 * 
 * State と Command(DrawElement) を固めて持っておくイメージ。
 * 基本的にどんなタイミングでも、「RenderStage を描画」すれば同じジオメトリが表示される。
 * 
 * 
 * 
 * 
 */
// 各 Parameters クラスは LinearAllocator 上のインスタンスを指す。
// 内容が同じ場合、メモリ節約のため、別の RenderStage と同じインスタンスを共有することもある。
// 各 Parameters クラスは まだ確定状態ではない。
// - SceneRenderer が ZPrepass などの描画でデフォルトの　RT を指定してくることがある。
// - SceneRenderer がシェーダの種類を指定してくることがある。（ShadowPass で書きたい。ユーザーシェーダがそれを持っていればそれを使うし、そうでなければデフォルトのを使う）
class RenderStage
	: public RefObject
{
public:
	// RenderingContext のステート。
	FrameBufferStageParameters* frameBufferStageParameters;

	// VisualNode と RenderingContext のステートはマージ済みの状態である。
	GeometryStageParameters* geometryStageParameters;

	RenderFeatureStageParameters* renderFeatureStageParameters;


	// これは Manager が持っているインスタンスを指す。ユーザー定義の場合はそっち。インスタンスは RenderStage モジュール内では管理しない。山荘のみ。
	RenderFeature* renderFeature;

	RenderStage();

	void addElement(RenderDrawElement* element);

private:
	RenderDrawElement* m_headElement;	// head of link list.
	RenderDrawElement* m_tailElement;	// tail of link list.

	//List<RenderDrawElement*> m_drawCommands;	// TODO: ポインタのリンクリストでもいいかな
};

class RenderStageList
	: public RefObject
{
public:
	RenderStageList(RenderingManager* manager);

	bool isEmpty() const { return m_renderStageList.isEmpty(); }
	RenderStage* last() const { return m_renderStageList.back(); }

	template<class T>
	T* newData()
	{
		void* buffer = m_dataAllocator->allocate(sizeof(T));
		return new (buffer)T();
	}

	RenderStage* addNewRenderStage();

private:
	Ref<LinearAllocator> m_dataAllocator;
	List<RenderStage*> m_renderStageList;	// TODO: ポインタのリンクリストでもいいかな
};

} // namespace detail
} // namespace ln

