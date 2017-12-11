
#pragma once
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Rendering/SceneRenderer.h>

LN_NAMESPACE_BEGIN
namespace detail {

struct FogParams
{
	Color	color;
	float	density = 0.0f;
};

class LightClusters
{
public:
	Vector3					m_cameraPos;
	Matrix					m_view;
	Matrix					m_proj;
	float					m_nearClip;
	float					m_farClip;

	LightClusters();
	void init();

	void beginMakeClusters(const Matrix& view, const Matrix& proj, const Vector3& cameraPos, float nearClip, float farClip);
	void endMakeClusters();

	void addPointLight(const Vector3& pos, float range, float attenuation, const Color& color);
	void addSpotLight(const Vector3& pos, float range, float attenuation, const Color& color, const Vector3& direction, float cone, float penumbra);
	void addDirectionalLight(const Vector3& dir, const Color& color);
	void addAmbientLight(const Color& color);
	void addHemisphereLight(const Color& skyColor, const Color& groundColor);

	const Ref<tr::Texture3D>& getClustersVolumeTexture() const { return m_clustersTexture; }
	const Ref<Texture2D>& getLightInfoTexture() const { return m_lightInfoTexture; }
	const Ref<Texture2D>& getGlobalLightInfoTexture() const { return m_globalLightInfoTexture; }

	int getGlobalLightCount() const { return m_globalLightInofs.getCount(); }

private:
	//static float bias(float b, float x) { return pow(x, log(b) / log(0.5)); }
	//static float bias(float b, float x) { return x; }
	void addClusterSpherical(const Vector3& pos, float range);
	void addClusterData(int x, int y, int z, int lightId);

	// Texture2D の 1 行分として書き込むため、float4 の倍数サイズである必要がある
	struct LightInfo
	{
		Vector4 posAndRange;		// xyz=pos, w=range
		Vector4	directionAndAtt;	// xyz=dir, w=attenuation
		Vector4	spotAngle;			// x > 0 is spot light. coneCos, penumbraCos, zw=NotUse
		Color	color;
	};

	struct GlobalLightInfo
	{
		Color	color;			// DirectionalColor, AmbientColor, (sky)Hemisphere
		Color	groundColor;
		Vector4	directionAndType;	// w=Type
		Vector4	dummy;
	};

	static const int		ClusterWidth = 16;
	static const int		ClusterHeight = 16;
	static const int		ClusterDepth = 32;
	std::vector<Color32>	m_clustersData;		// TODO: Texture3D がまだ setData も getMappedData もサポートしていないので。できれば getMappedData にそのまま書き込みたい
	std::vector<int>		m_clustersAddCount;	// あるクラスタに書かれたデータの数
	Ref<tr::Texture3D>		m_clustersTexture;

	static const int		MaxLights = 64;
	List<LightInfo>			m_lightInofs;		// m_lightInfoTexture に書き込む。TODO: Texture2D が float4 書き込みをちゃんとサポートしたら必要ない。
	Ref<Texture2D>			m_lightInfoTexture;
	List<GlobalLightInfo>	m_globalLightInofs;		// m_globalLightInfoTexture に書き込む。TODO: Texture2D が float4 書き込みをちゃんとサポートしたら必要ない。
	Ref<Texture2D>			m_globalLightInfoTexture;

};

class ClusteredShadingGeometryRenderingPass
	: public RenderingPass2
{
public:
	ClusteredShadingGeometryRenderingPass();
	virtual ~ClusteredShadingGeometryRenderingPass();
	void initialize();

	//virtual Shader* getDefaultShader() const override;
	virtual void selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy) override;

	virtual void onBeginPass(DefaultStatus* defaultStatus) override;

private:
	Ref<Shader>					m_defaultShader;
	ShaderTechnique*			m_defaultShaderTechnique;
	Ref<Shader>					m_unLightingShader;
	ShaderTechnique*			m_unLightingShaderTechnique;
};

class DepthPrepass
	: public RenderingPass2
{
public:
	DepthPrepass();
	virtual ~DepthPrepass();
	void initialize();

	virtual void selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy) override;
	virtual void onBeginPass(DefaultStatus* defaultStatus) override;

public:	// TODO:
	Ref<Shader>					m_defaultShader;
	Ref<RenderTargetTexture>	m_depthMap;
};

class ShadowCasterPass
	: public RenderingPass2
{
public:
	CameraInfo	view;

	ShadowCasterPass();
	virtual ~ShadowCasterPass();
	void initialize();

	//virtual Shader* getDefaultShader() const override;

	virtual void selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy) override;

	virtual void onBeginPass(DefaultStatus* defaultStatus) override;

	virtual void overrideCameraInfo(detail::CameraInfo* cameraInfo) override;

protected:
	//virtual ShaderPass* selectShaderPass(Shader* shader) override;

public:	// TODO:
	Ref<Shader>		m_defaultShader;
	Ref<RenderTargetTexture>	m_shadowMap;
};


class ClusteredShadingSceneRenderer
	: public SceneRenderer
{
public:
	ClusteredShadingSceneRenderer();
	virtual ~ClusteredShadingSceneRenderer();
	void initialize(GraphicsManager* manager);
	//void setSceneGlobalRenderSettings(const SceneGlobalRenderSettings& settings) { m_renderSettings = settings; }
	void setFogParams(const FogParams& params) { m_fogParams = params; }
	DepthPrepass* getDepthPrepass() const { return m_depthPrepass; }

protected:
	virtual void collect(RenderingPass2* pass, const detail::CameraInfo& cameraInfo) override;
	virtual void prepare() override;
	virtual void onCollectLight(DynamicLightInfo* light) override;
	virtual void onShaderPassChainging(ShaderPass* pass) override;

private:
	LightClusters				m_lightClusters;
	//SceneGlobalRenderSettings	m_renderSettings;
	FogParams					m_fogParams;
	Ref<DepthPrepass>			m_depthPrepass;
};

} // namespace detail
LN_NAMESPACE_END

