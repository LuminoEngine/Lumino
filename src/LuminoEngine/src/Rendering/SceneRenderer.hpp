#pragma once
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>


namespace ln {
class AbstractMaterial;
namespace detail {
class RenderStage;
class RenderDrawElement;
class RenderingPipeline;

enum class ZSortDistanceBase
{
	NodeZ,					/**< ノードの Z 値を距離として使用する */
	CameraDistance,			/**< カメラとノードの距離を使用する */
	CameraScreenDistance,	/**< カメラが映すスクリーン平面とノードの距離を使用する */
};


class SceneRendererPass
	: public RefObject
{
public:
	SceneRendererPass();
	virtual ~SceneRendererPass();
	void init();

	virtual void onBeginRender(SceneRenderer* sceneRenderer);
	virtual void onEndRender(SceneRenderer* sceneRenderer);

	// このパスのデフォルトの RenderPass を構築する。
	// renderTarget と depthBuffer は、シーンレンダリングの最終出力先。
	// もし G-Buffer を作るときなど、内部的な RenderTarget に書きたい場合はこれらは使用せずに派生側で各種バッファを用意する。
	virtual void onBeginPass(GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) = 0;

	// このパスを実行するときのデフォルトの RenderPass を取得する。
	// インスタンスは、このパスの実行側の onBeginPass() で作っておく。
	virtual RenderPass* renderPass() const = 0;

	// Element の情報と派生 Pass から、最終的に使いたい ShaderTechnique を求める
	virtual ShaderTechnique* selectShaderTechnique(
		ShaderTechniqueClass_MeshProcess requestedMeshProcess,
		Shader* requestedShader,
		ShadingModel requestedShadingModel) = 0;

	RenderingManager* manager() const { return m_manager; }

	ShaderTechniqueClass_ShadingModel tlanslateShadingModel(ShadingModel value) { return (ShaderTechniqueClass_ShadingModel)value; }

private:
	RenderingManager* m_manager;
};

class SceneRenderer
	: public RefObject
{
public:
	void render(
		GraphicsContext* graphicsContext,
        RenderingPipeline* renderingPipeline,
		RenderTargetTexture* renderTarget,
        const ClearInfo& clearInfo,
        const detail::CameraInfo& mainCameraInfo,
        RendringPhase targetPhase,
		const detail::SceneGlobalRenderParams* sceneGlobalParams);

    RenderingPipeline* renderingPipeline() const { return m_renderingPipeline; }
	const detail::SceneGlobalRenderParams* sceneGlobalParams() const { return m_sceneGlobalRenderParams; }
    const detail::CameraInfo& mainCameraInfo() const { return m_mainCameraInfo; }


protected:
	SceneRenderer();
	void init();

	//void setDefaultMaterial(AbstractMaterial* material);

	void addPass(SceneRendererPass* pass);

	void renderPass(GraphicsContext* graphicsContext, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer, SceneRendererPass* pass);

	// レンダリング準備として、描画に関係する各種オブジェクト (DrawElement や Light) を収集するフェーズ
	virtual void collect(const CameraInfo& cameraInfo);

	// レンダリング準備として、効率的な描画を行うために収集した各種オブジェクトのソートなどを行う
	void prepare();

	//virtual void onBeginRender() = 0;
	//virtual void onEndRender() = 0;

	virtual void onCollectLight(const DynamicLightInfo& light);
	virtual void onSetAdditionalShaderPassVariables(Shader* shader);


private:
	RenderPass* getOrCreateRenderPass(RenderPass* currentRenderPass, RenderStage* stage, RenderTargetTexture* defaultRenderTarget, DepthBuffer* defaultDepthBuffer/*, const ClearInfo& clearInfo*/);
	static bool equalsFramebuffer(RenderPass* currentRenderPass, const FrameBuffer& fb);

	detail::RenderingManager* m_manager;
	List<Ref<SceneRendererPass>> m_renderingPassList;
	RenderFeatureBatchList m_renderFeatureBatchList;

    RenderingPipeline* m_renderingPipeline;
	const detail::SceneGlobalRenderParams* m_sceneGlobalRenderParams;

	//const FrameBuffer* m_defaultFrameBuffer;
	ZSortDistanceBase m_zSortDistanceBase;
	//Ref<AbstractMaterial> m_defaultMaterial;
	//Ref<RenderPass> m_renderPass;
	List<Ref<RenderPass>> m_renderPassPool;
	int m_renderPassPoolUsed;

    ClearInfo m_firstClearInfo;

    // 1つのパイプラインの別フェーズで SceneRenderer を使うとき、
    // viewproj 行列を分けたいことがある (Default と ImageEffect など) ため、SceneRenderer 側に実態で持つ 
    CameraInfo m_mainCameraInfo;

    RendringPhase m_targetPhase;

	// build by collect().
	List<RenderDrawElement*> m_renderingElementList;

	List<SceneRendererPass*>	m_renderingActualPassList;

	// TODO: ちゃんとした置き場は SkinnedMesh 実装時に考える
	Ref<Texture2D>    m_skinningMatricesTexture;            // GPU スキニング用のテクスチャ
	Ref<Texture2D>    m_skinningLocalQuaternionsTexture;    // GPU スキニング用のテクスチャ
};

} // namespace detail
} // namespace ln

