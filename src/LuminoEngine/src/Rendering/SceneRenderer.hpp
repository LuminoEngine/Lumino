﻿#pragma once
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Rendering/Common.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>


namespace ln {
class Material;
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

struct ShaderTechniqueRequestClasses
{
	ShaderTechniqueClass_MeshProcess meshProcess;
	ShaderTechniqueClass_DrawMode drawMode;
	ShaderTechniqueClass_Normal normal;
	ShaderTechniqueClass_Roughness roughness;
};


class SceneRendererPass
	: public RefObject
{
public:
	SceneRendererPass();
	virtual ~SceneRendererPass();
	void init();

	void setClearInfo(const ClearInfo& value) { m_clearInfo = value; }
	const ClearInfo& clearInfo() const { return m_clearInfo; }

	virtual void onBeginRender(SceneRenderer* sceneRenderer, GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);
	virtual void onEndRender(SceneRenderer* sceneRenderer);

	// このパスのデフォルトの RenderPass を構築する。
	// renderTarget と depthBuffer は、シーンレンダリングの最終出力先。
	// もし G-Buffer を作るときなど、内部的な RenderTarget に書きたい場合はこれらは使用せずに派生側で各種バッファを用意する。
	//virtual void onBeginPass(SceneRenderer* sceneRenderer) = 0;

	// このパスを実行するときのデフォルトの RenderPass を取得する。
	// インスタンスは、このパスの実行側の onBeginPass() で作っておく。
	virtual RenderPass* renderPass() const = 0;

	virtual bool filterElement(RenderDrawElement* element) const;

	// Element の情報と派生 Pass から、最終的に使いたい ShaderTechnique を求める
	virtual ShaderTechnique* selectShaderTechnique(
		const ShaderTechniqueRequestClasses& requester,
		Shader* requestedShader,
		ShadingModel requestedShadingModel) = 0;

	RenderingManager* manager() const { return m_manager; }

	static ShaderTechniqueClass_ShadingModel tlanslateShadingModel(ShadingModel value) { return (ShaderTechniqueClass_ShadingModel)value; }

protected:
	//static ShaderTechnique* selectShaderTechniqueHelper(
	//	const ShaderTechniqueRequestClasses& requester,
	//	Shader* requestedShader,
	//	ShadingModel requestedShadingModel,
	//	Shader* defaultShader,
	//	ShaderTechnique* defaultTechnique,
	//	ShaderTechniqueClass_Phase phase);


private:
	RenderingManager* m_manager;
	ClearInfo m_clearInfo;
};

class SceneRenderer
	: public RefObject
{
public:
	// render の前準備として、効率的な描画を行うためのZソートなどを実施した Element リストを作成する。
	void prepare(
		RenderingPipeline* renderingPipeline,
		RenderingContext* renderingContext,
		//detail::CommandListServer* commandListServer,
		const detail::RenderViewInfo& mainRenderViewInfo,
		RenderPart targetPhase,
		ProjectionKind targetProjection,
		const detail::SceneGlobalRenderParams* sceneGlobalParams);

	void renderPass(
		GraphicsContext* graphicsContext,
		RenderTargetTexture* renderTarget,
		DepthBuffer* depthBuffer,
		SceneRendererPass* pass);

	virtual SceneRendererPass* mainRenderPass() const { return nullptr; }

    RenderingPipeline* renderingPipeline() const { return m_renderingPipeline; }
	const detail::SceneGlobalRenderParams* sceneGlobalParams() const { return m_sceneGlobalRenderParams; }
    const detail::RenderViewInfo& mainRenderViewInfo() const { return m_mainRenderViewInfo; }
	const detail::DynamicLightInfo* mainLightInfo() const { return m_mainLightInfo; }

	SceneRenderer();
	void init();

protected:

	//void setDefaultMaterial(Material* material);

	//void addPass(SceneRendererPass* pass);


	// レンダリング準備として、描画に関係する各種オブジェクト (DrawElement や Light) を収集するフェーズ
	virtual void collect(RenderingPipeline* renderingPipeline, const CameraInfo& cameraInfo, RenderPart targetPhase);

	// レンダリング準備として、効率的な描画を行うために収集した各種オブジェクトのソートなどを行う
	void prepare();

	//virtual void onBeginRender() = 0;
	//virtual void onEndRender() = 0;

	virtual void onCollectLight(const DynamicLightInfo& light);
	virtual void onSetAdditionalShaderPassVariables(ShaderDescriptor* descriptor, ShaderTechnique* technique);


public:	// TODO
	RenderPass* getOrCreateRenderPass(RenderPass* currentRenderPass, RenderStage* stage, RenderPass* defaultRenderPass /*RenderTargetTexture* defaultRenderTarget, DepthBuffer* defaultDepthBuffer*//*, const ClearInfo& clearInfo*/);
	static bool equalsFramebuffer(RenderPass* currentRenderPass, const FrameBuffer& fb);

private:
	detail::RenderingManager* m_manager;
	//List<Ref<SceneRendererPass>> m_renderingPassList;
	RenderFeatureBatchList m_renderFeatureBatchList;

    RenderingPipeline* m_renderingPipeline;
	RenderingContext* m_renderingContext;
	const detail::SceneGlobalRenderParams* m_sceneGlobalRenderParams;
	RenderPart m_currentPart;
	ProjectionKind m_currentProjection;

	//const FrameBuffer* m_defaultFrameBuffer;
	ZSortDistanceBase m_zSortDistanceBase;
	//Ref<Material> m_defaultMaterial;
	//Ref<RenderPass> m_renderPass;
	List<Ref<RenderPass>> m_renderPassPool;
	int m_renderPassPoolUsed;

    // 1つのパイプラインの別フェーズで SceneRenderer を使うとき、
    // viewproj 行列を分けたいことがある (Default と PostEffect など) ため、SceneRenderer 側に実態で持つ 
	RenderViewInfo m_mainRenderViewInfo;
	SceneInfo m_mainSceneInfo;
	const DynamicLightInfo* m_mainLightInfo = nullptr;

    //RenderPart m_targetPhase;

	// build by collect().
	List<RenderDrawElement*> m_renderingElementList;

	//List<SceneRendererPass*>	m_renderingActualPassList;

	// TODO: ちゃんとした置き場は SkinnedMesh 実装時に考える
	Ref<Texture2D>    m_skinningMatricesTexture;            // GPU スキニング用のテクスチャ
	Ref<Texture2D>    m_skinningLocalQuaternionsTexture;    // GPU スキニング用のテクスチャ
};

} // namespace detail
} // namespace ln

