#pragma once
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoGraphics/Texture.hpp>
#include <LuminoGraphics/DepthBuffer.hpp>
#include <LuminoGraphics/ShaderInterfaceFramework.hpp>
#include <LuminoEngine/Rendering/Common.hpp>
#include <LuminoCore/Base/LinearAllocator.hpp>

namespace ln {
class Shader;
class GraphicsContext;
class Material;
class RenderFeature;
namespace detail {
class RenderDrawElement;
class RenderStage;
class DrawElementList;
class DrawElementListBuilder;
class RenderFeatureBatchList;
struct ElementInfo;
struct SubsetInfo;


// インスタンスは DrawElementList の LinearAllocator に配置される。
// 描画が終わったら解放される。
class FrameBufferStageParameters
	: public IDrawElementListFrameData
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

    void copyFrom(const FrameBufferStageParameters& other)
    {
        *this = other;
    }
};

// インスタンスは DrawElementList の LinearAllocator に配置される。
// 描画が終わったら解放される。
// GeometryStageParameters は
// - RenderingContext がもステート
// - VisualNode が持つステート
// の2か所で使われる。
// これは VisualNode::onRender() 内で、VisualNode が持つ State をベースとして RenderingContext を使えるようにするための仕組み。
// 例えば VisualNode::onRender() 内で RenderingContext::setOpacity() すると、VisualNode の opacity と乗算された結果が 最終結果となる。
class GeometryStageParameters
	: public IDrawElementListFrameData
{
public:
	// まだ確定状態となるわけではないので Optional が含まれることもある。
	Ref<Material>			m_material;			// setMaterial() で指定されたマテリアル
	Optional<BlendMode>			m_blendMode;
	Optional<CullMode>		m_cullingMode;
	Optional<bool>				m_depthTestEnabled;
	Optional<bool>				m_depthWriteEnabled;
	Optional<ShadingModel>		shadingModel;
	PrimitiveTopology primitiveTopology;
	//int m_objectId = 0;

	GeometryStageParameters()
	{
		reset();
	}

	void reset()
	{
		m_material = nullptr;
		m_blendMode = nullptr;
		m_cullingMode = nullptr;
		m_depthTestEnabled = nullptr;
		m_depthWriteEnabled = nullptr;
		shadingModel = nullptr;
		primitiveTopology = PrimitiveTopology::TriangleList;
		//builtinEffectData.reset();
		//m_objectId = 0;
	}

	bool equals(const GeometryStageParameters* other) const
	{
		return
			m_material == other->m_material &&  // TODO: Material が一致することはまずない。ちゃんと中身かhashを見ること。
			m_blendMode == other->m_blendMode &&
			m_cullingMode == other->m_cullingMode &&
			m_depthTestEnabled == other->m_depthTestEnabled &&
			m_depthWriteEnabled == other->m_depthWriteEnabled &&
			shadingModel == other->shadingModel &&
			primitiveTopology == other->primitiveTopology;//&&
			//m_objectId == other->m_objectId;// &&
			//builtinEffectData.equals(&other->builtinEffectData);
	}

    void copyFrom(const GeometryStageParameters& other)
    {
        *this = other;
    }
};

/*
 * RenderStage
 * 
 * State と Command(DrawElement) を固めて持っておくイメージ。
 * 基本的にどんなタイミングでも、「RenderStage を描画」すれば同じジオメトリが表示される。
 * 
 * ↑× なんやかんややっているうちに、単なる state の塊になってきた。
 * 
 * 
 */
// 各 Parameters クラスは LinearAllocator 上のインスタンスを指す。
// 内容が同じ場合、メモリ節約のため、別の RenderStage と同じインスタンスを共有することもある。
// 各 Parameters クラスは まだ確定状態ではない。
// - SceneRenderer が ZPrepass などの描画でデフォルトの　RT を指定してくることがある。
// - SceneRenderer がシェーダの種類を指定してくることがある。（ShadowPass で書きたい。ユーザーシェーダがそれを持っていればそれを使うし、そうでなければデフォルトのを使う）
//
// ステートをまとめることの留意点として、
// そもそもまとめるのは 不透明・半透明 によって描画パスの振り分けを行うことが主な目的。
// シェーダ変数の変更は別にかまわない。（というかそれまで固定するのはちょっと現実的ではない）
class RenderStage
	//: public RefObject
	: public IDrawElementListFrameData
{
public:
	// RenderingContext のステート。
	FrameBufferStageParameters* frameBufferStageParameters;

	// VisualNode と RenderingContext のステートはマージ済みの状態である。
	GeometryStageParameters* geometryStageParameters;

	int m_objectId = 0;


	// これは Manager が持っているインスタンスを指す。ユーザー定義の場合はそっち。インスタンスは RenderStage モジュール内では管理しない。参照のみ。
	RenderFeature* renderFeature;



	RenderStage();

	bool equals(const RenderStage* other) const;
	void flush(GraphicsContext* context);


	RenderTargetTexture* getRenderTargetFinal(int index) const { return frameBufferStageParameters->m_renderTargets[index]; }
	DepthBuffer* getDepthBufferFinal() const { return frameBufferStageParameters->m_depthBuffer; }
	const RectI& getViewportRectFinal() const { return frameBufferStageParameters->m_viewportRect; }
	const RectI& getScissorRectFinal() const { return frameBufferStageParameters->m_scissorRect; }


	Material* getMaterialFinal(Material* priorityValue, Material* sceneDefaultMaterial) const;
	ShadingModel getShadingModelFinal(const Material* finalMaterial) const;	// getMaterialFinal() で確定した Material を渡すこと
	BlendMode getBlendModeFinal(const Material* finalMaterial = nullptr) const;	// getMaterialFinal() で確定した Material を渡すこと
	CullMode getCullingModeFinal(const Material* finalMaterial = nullptr) const;	// getMaterialFinal() で確定した Material を渡すこと
    ComparisonFunc getDepthTestFuncFinal(const Material* finalMaterial = nullptr) const;	// getMaterialFinal() で確定した Material を渡すこと
	bool isDepthWriteEnabledFinal(const Material* finalMaterial = nullptr) const;	// getMaterialFinal() で確定した Material を渡すこと
	//const Matrix& getTransformFinal() const { return geometryStageParameters->; }
	//Brush* getBrushFinal() const { return renderingContextParameters.getBrush(); }
	//Pen* getPenFinal() const { return nullptr; }	// TODO:
	//Font* getFontFinal() const { return renderingContextParameters.getFont(); }
	//Material* getPriorityMaterialFinal() const { return renderingContextParameters.getPriorityMaterial(); }
	// BuiltinEffectData
	//Shader* getShaderFinal(Material* finalMaterial) const;	// getMaterialFinal() で確定した Material を渡すこと
    float getOpacityFinal(RenderDrawElement* element) const;
    const Color& getColorScaleFinal(RenderDrawElement* element) const;
    const Color& getBlendColorFinal(RenderDrawElement* element) const;
    const ColorTone& getToneFinal(RenderDrawElement* element) const;

	//static void applyFrameBufferStatus(RenderPass* renderPass, const RenderStage* stage, const FrameBuffer& defaultFrameBufferInPass);
	//static void applyGeometryStatus(GraphicsContext* context, const RenderStage* stage, const Material* priorityMaterial);
	//static void makeBlendMode(BlendMode mode, RenderTargetBlendDesc* state);

private:

	//List<RenderDrawElement*> m_drawCommands;	// TODO: ポインタのリンクリストでもいいかな
};

} // namespace detail
} // namespace ln

