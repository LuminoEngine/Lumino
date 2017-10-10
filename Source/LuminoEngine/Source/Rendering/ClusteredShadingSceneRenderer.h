
#pragma once
#include <Lumino/Rendering/SceneRenderer.h>

LN_NAMESPACE_BEGIN
namespace detail {

class LightClusters
{
public:
	LightClusters();
	void init();
	void beginMakeClusters(const Matrix& view, const Matrix& proj, const Vector3& cameraPos, float nearClip, float farClip);
	void endMakeClusters();

private:

	// Texture2D の 1 行分として書き込むため、float4 の倍数サイズである必要がある
	struct LightInfo
	{
		Vector4 posAndRange;	// xyz=pos, w=range
		Vector4	spotDirection;	// xyz=dir, w=NotUse
		Vector4	spotAngle;		// x > 0 is spot light. x=cos(OuterCone), y=1.0/cos(ConeDifference), zw=NotUse
		Color	color;
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
};

class ClusteredShadingGeometryRenderingPass
	: public RenderingPass2
{
public:
	ClusteredShadingGeometryRenderingPass();
	virtual ~ClusteredShadingGeometryRenderingPass();
	void initialize();

	virtual Shader* getDefaultShader() const override;

protected:
	virtual ShaderPass* selectShaderPass(Shader* shader) override;

private:
	Ref<Shader>		m_defaultShader;
};

class ClusteredShadingSceneRenderer
	: public SceneRenderer
{
public:
	ClusteredShadingSceneRenderer();
	virtual ~ClusteredShadingSceneRenderer();
	void initialize(GraphicsManager* manager);

protected:
	virtual void collect() override;
	virtual void prepare() override;
	virtual void onCollectLight(DynamicLightInfo* light) override;
	virtual void onShaderPassChainging(ShaderPass* pass) override;
};

} // namespace detail
LN_NAMESPACE_END

