
#pragma once
#include "Common.h"
#include "../Graphics/Color.h"
#include "../Graphics/RenderState.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include "../Graphics/ContextInterface.h"
#include "../Graphics/Material.h"

LN_NAMESPACE_BEGIN
class RenderView;
class RenderDiag;

//enum class AmbientLightingMode
//{
//
//};

namespace detail {
class RenderingPass2;
class DrawElement;
class DrawElementList;
struct DefaultStatus;


/**
	@brief	シーンの描画方法を定義し、描画コマンドを実行します。
*/
class SceneRenderer
	: public RefObject
{
public:
	SceneRenderer();
	virtual ~SceneRenderer();
	void initialize(GraphicsManager* manager);

	void render(
		RenderView* drawElementListSet,
		//DrawElementList* elementList,
		//const detail::CameraInfo& cameraInfo,
		RenderTargetTexture* defaultRenderTarget,
		DepthBuffer* defaultDepthBuffer,
		RenderDiag* diag,
		bool clearColorBuffer,
		const Color& clearColor);

protected:

	// render
	RenderView* getRenderView() const { return m_renderingRenderView; }

	// レンダリング準備として、描画に関係する各種オブジェクト (DrawElement や Light) を収集するフェーズ
	virtual void collect(RenderingPass2* pass, const detail::CameraInfo& cameraInfo);

	// レンダリング準備として、効率的な描画を行うために収集した各種オブジェクトのソートなどを行う
	virtual void prepare();

	virtual void onPreRender(DrawElementList* elementList);
	//virtual ShaderTechnique* selectShaderTechnique(Shader* shader);

	// レンダリング準備時、影響するライトを通知する
	virtual void onCollectLight(DynamicLightInfo* light);

	virtual void onShaderPassChainging(ShaderPass* pass);

	void addPass(RenderingPass2* pass);

private:
	GraphicsManager*				m_manager;
	List<Ref<RenderingPass2>>		m_renderingPassList;
	List<detail::DrawElement*>		m_renderingElementList;

	RenderView*				m_renderingRenderView;
	RenderTargetTexture*	m_renderingDefaultRenderTarget;
	DepthBuffer*			m_renderingDefaultDepthBuffer;
	List<RenderingPass2*>	m_renderingActualPassList;

	List<detail::ShadowCasterPass*>	m_renderingShadowCasterPassList;
	//friend class RenderingPass2;
	Ref<CommonMaterial>				m_defaultMaterial;
};


struct ElementRenderingPolicy
{
	Shader*		shader;		// null もありえる。Clear など。
	ShaderPass*	shaderPass;
	bool		visible;
};

class RenderingPass2
	: public Object
{
public:
	struct RenderStageFinalData
	{
		RenderStage* stage;
		CommonMaterial* material;
		Shader* shader;
		ShadingModel shadingModel;
	};

	RenderingPass2();
	virtual ~RenderingPass2();
	//void initialize(GraphicsManager* manager);

	//virtual Shader* getDefaultShader() const = 0;

	virtual void selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy) = 0;

	//virtual void RenderElement(DrawList* renderer, DrawElement* element);
	//virtual void RenderElementSubset(DrawList* renderer, DrawElement* element, int subsetIndex);


	virtual void onBeginPass(DefaultStatus* defaultStatus);

	virtual void overrideCameraInfo(detail::CameraInfo* cameraInfo);

protected:
	//virtual ShaderTechnique* selectShaderTechnique(Shader* shader);
	//virtual ShaderPass* selectShaderPass(Shader* shader);

	// TODO: name は hash でもいいかな
	ShaderPass* selectShaderPassHelper(Shader* materialShader, const String& techniqueName, const String& passName, ShaderPass* defaultPass);

	// Obsolete 古い記述用。
	ShaderPass* selectShaderPassHelperSimple(Shader* materialShader, Shader* defaultShader);

private:
};

class NonShadingRenderer
	: public SceneRenderer
{
public:
	NonShadingRenderer();
	virtual ~NonShadingRenderer();
	void initialize(GraphicsManager* manager);

private:
};

class NonShadingRenderingPass
	: public RenderingPass2
{
public:
	NonShadingRenderingPass();
	virtual ~NonShadingRenderingPass();
	void initialize(GraphicsManager* manager);
	virtual void selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy) override;
	//virtual Shader* getDefaultShader() const override;

private:
	Ref<Shader>	m_defaultShader;
};



class ForwardShadingRenderer
	: public SceneRenderer
{
public:
	ForwardShadingRenderer();
	virtual ~ForwardShadingRenderer();
	void initialize(GraphicsManager* manager);

protected:
	virtual void onPreRender(DrawElementList* elementList);

private:
	void updateAffectLights(DrawElement* element, DrawElementList* elementList);

	List<DynamicLightInfo*>	m_selectingLights;	// updateAffectLights() の作業用変数
};


class ForwardShadingRenderingPass
	: public RenderingPass2
{
public:
	ForwardShadingRenderingPass();
	virtual ~ForwardShadingRenderingPass();
	void initialize(GraphicsManager* manager);
	virtual void selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy) override;
	//virtual Shader* getDefaultShader() const override;

private:
	Ref<Shader>	m_defaultShader;
};


class InfomationRenderingPass
	: public NonShadingRenderingPass
{
public:
	InfomationRenderingPass();
	virtual ~InfomationRenderingPass();
	void initialize(GraphicsManager* manager);
};


} // namespace detail
LN_NAMESPACE_END

