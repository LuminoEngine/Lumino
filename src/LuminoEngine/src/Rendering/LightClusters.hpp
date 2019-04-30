#pragma once

namespace ln {
class Texture2D;
class Texture3D;
namespace detail {

/*
	Cluster のテクスチャは 2D テクスチャで表現する。3Dテクスチャのほうがイメージしやすいのだが、一部のモバイル環境では 3D テクスチャが使えない。
	テクスチャの X 方向にワールド上の XY 情報を格納、Y 方向に Z 情報を格納。
	クラスタの数は 16*16*32 なので、256*32 サイズのテクスチャとなる。（もう少し大きくしてもいいかも）
*/
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

	const Ref<Texture2D>& getClustersVolumeTexture() const { return m_clustersTexture; }
	const Ref<Texture2D>& getLightInfoTexture() const { return m_lightInfoTexture; }
	const Ref<Texture2D>& getGlobalLightInfoTexture() const { return m_globalLightInfoTexture; }

	int getGlobalLightCount() const { return m_globalLightInofs.size(); }
	bool hasLight() const { return m_lightInofs.size() > 1 || m_globalLightInofs.size() > 1; }

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

	// Texture2D の 1 行分として書き込むため、float4 の倍数サイズである必要がある
	struct GlobalLightInfo
	{
		Color	color;			// DirectionalColor, AmbientColor, (sky)Hemisphere
		Color	groundColor;
		Vector4	directionAndType;	// w=Type
		Vector4	dummy;
	};

	static const int		ClusterWidth;
	static const int		ClusterHeight;
	static const int		ClusterDepth;
	std::vector<ColorI>	m_clustersData;		// TODO: Texture3D がまだ setData も getMappedData もサポートしていないので。できれば getMappedData にそのまま書き込みたい
	std::vector<int>		m_clustersAddCount;	// あるクラスタに書かれたデータの数
	Ref<Texture2D>		m_clustersTexture;

	static const int		MaxLights;
	List<LightInfo>			m_lightInofs;		// m_lightInfoTexture に書き込む。TODO: Texture2D が float4 書き込みをちゃんとサポートしたら必要ない。
	Ref<Texture2D>			m_lightInfoTexture;
	List<GlobalLightInfo>	m_globalLightInofs;		// m_globalLightInfoTexture に書き込む。TODO: Texture2D が float4 書き込みをちゃんとサポートしたら必要ない。
	Ref<Texture2D>			m_globalLightInfoTexture;

};

} // namespace detail
} // namespace ln

