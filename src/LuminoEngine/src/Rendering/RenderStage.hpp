#pragma once
#include <Lumino/Graphics/ColorStructs.hpp>
#include <Lumino/Graphics/RenderState.hpp>
#include <Lumino/Graphics/Texture.hpp>
#include <Lumino/Graphics/DepthBuffer.hpp>

namespace ln {
class Shader;
class AbstractMaterial;
namespace detail {
class LinearAllocator;

struct BuiltinEffectData
{
	Ref<Shader>	shader;
	float opacity;
	Color colorScale;
	Color blendColor;
	ToneF tone;

	void reset();
	bool equals(const BuiltinEffectData* other) const;
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

	void reset();
	bool equals(const FrameBufferStageParameters* other) const;
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
	Optional<BlendMode>			m_blendMode;
	Optional<CullingMode>		m_cullingMode;
	Optional<bool>				m_depthTestEnabled;
	Optional<bool>				m_depthWriteEnabled;
	Matrix						m_transfrom;
	Ref<AbstractMaterial>			m_material;			// setMaterial() で指定されたマテリアル
	Optional<ShadingModel>		shadingModel;
	BuiltinEffectData builtinEffectData;

	void reset();
	bool equals(const GeometryStageParameters* other) const;
};

// インスタンスは RenderStageList の LinearAllocator に配置される。
// 描画が終わったら解放される。
class RenderFeatureStageParameters
{
public:
	virtual bool equals(const RenderFeatureStageParameters* other) = 0;

	//Ref<Brush>					m_brush;
	//Ref<Font>					m_font;
};

// インスタンスは RenderStageList の LinearAllocator に配置される。
// clear や draw 系のメソッド呼び出しをおこなう。
// ステートは変更するべきではない。
class RenderStageDrawCommand
{
public:
	RenderStageDrawCommand();
	virtual ~RenderStageDrawCommand();
	virtual void onExecute() = 0;

private:
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

	RenderStage();

	template<class T>
	T* addNewCommand()
	{

	}

private:
	List<RenderStageDrawCommand*> m_drawCommands;	// TODO: ポインタのリンクリストでもいいかな
};

class RenderStageList
	: public RefObject
{
public:
	RenderStageList();

	RenderStage* addNewRenderStage();

private:
	Ref<LinearAllocator> m_dataAllocator;
};

} // namespace detail
} // namespace ln

