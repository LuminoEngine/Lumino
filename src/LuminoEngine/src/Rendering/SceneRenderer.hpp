#pragma once
#include <Lumino/Graphics/RenderState.hpp>
#include <Lumino/Shader/Shader.hpp>
#include <Lumino/Rendering/RenderView.hpp>

namespace ln {
class AbstractMaterial;
namespace detail {
class RenderStage;
class RenderDrawElement;

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
	// Element の情報と派生 Pass から、最終的に使いたい ShaderTechnique を求める
	virtual ShaderTechnique* selectShaderTechnique(
		ShaderTechniqueClass_MeshProcess requestedMeshProcess,
		Shader* requestedShader,
		ShadingModel requestedShadingModel) = 0;

private:
};

class SceneRenderer
	: public RefObject
{
public:
	void render(
		GraphicsContext* graphicsContext,
		RenderView* renderView,
		const FrameBuffer& defaultFrameBuffer);

protected:
	SceneRenderer();

	void addPass(SceneRendererPass* pass);

	void renderPass(GraphicsContext* graphicsContext, SceneRendererPass* pass);

	// レンダリング準備として、描画に関係する各種オブジェクト (DrawElement や Light) を収集するフェーズ
	void collect(/*SceneRendererPass* pass, */const detail::CameraInfo& cameraInfo);

	// レンダリング準備として、効率的な描画を行うために収集した各種オブジェクトのソートなどを行う
	void prepare();

private:
	void applyFrameBufferStatus(GraphicsContext* context, RenderStage* stage);
	void applyGeometryStatus(GraphicsContext* context, RenderStage* stage, AbstractMaterial* priorityMaterial);
	static void makeBlendMode(BlendMode mode, RenderTargetBlendDesc* state);

	List<Ref<SceneRendererPass>> m_renderingPassList;

	RenderView* m_renderingRenderView;
	const FrameBuffer* m_defaultFrameBuffer;
	ZSortDistanceBase m_zSortDistanceBase;

	// build by collect().
	List<RenderDrawElement*> m_renderingElementList;

	List<SceneRendererPass*>	m_renderingActualPassList;
};

} // namespace detail
} // namespace ln

