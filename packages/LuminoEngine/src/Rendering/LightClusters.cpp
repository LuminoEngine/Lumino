#include "Internal.hpp"
#include <float.h>
#include <LuminoEngine/Bitmap/Bitmap.hpp>
#include <LuminoEngine/GPU/Texture.hpp>
#include "LightClusters.hpp"

namespace ln {
namespace detail {

const int		LightClusters::ClusterWidth = 16;
const int		LightClusters::ClusterHeight = 16;
const int		LightClusters::ClusterDepth = 32;
const int		LightClusters::MaxLights = 64;
	
//==============================================================================
// LightClusters

LightClusters::LightClusters()
	: m_clustersData()
	, m_clustersAddCount()
	, m_clustersTexture(nullptr)
	, m_lightInofs()
	, m_lightInfoTexture(nullptr)
	, m_view()
	, m_proj()
	, m_nearClip(0)
	, m_farClip(0)
{
}

void LightClusters::init()
{
	m_lightInofs.reserve(MaxLights);
	m_globalLightInofs.reserve(MaxLights);

	m_clustersTexture = makeObject_deprecated<Texture2D>(ClusterWidth * ClusterHeight, ClusterDepth, TextureFormat::RGBA8);
	m_clustersTexture->setResourceUsage(GraphicsResourceUsage::Dynamic);
	m_lightInfoTexture = makeObject_deprecated<Texture2D>(sizeof(LightInfo) / sizeof(Vector4), MaxLights, TextureFormat::RGBA32F);
	m_lightInfoTexture->setResourceUsage(GraphicsResourceUsage::Dynamic);
	m_globalLightInfoTexture = makeObject_deprecated<Texture2D>(sizeof(GlobalLightInfo) / sizeof(Vector4), MaxLights, TextureFormat::RGBA32F);
	m_globalLightInfoTexture->setResourceUsage(GraphicsResourceUsage::Dynamic);
}

void LightClusters::beginMakeClusters(const Matrix& view, const Matrix& proj, const Vector3& cameraPos, float nearClip, float farClip)
{
	m_view = view;
	m_proj = proj;
	m_cameraPos = cameraPos;
	m_nearClip = nearClip;
	m_farClip = farClip;

	// TODO: 全部 0 クリアする Texture3D::clear() があっても良いかも
	//for (int y = 0; y < ClusterHeight; y++)
	//{
	//	for (int x = 0; x < ClusterWidth; x++)
	//	{
	//		for (int z = 0; z < ClusterDepth; z++)
	//		{
	//			m_clustersTexture->setPixel32(x, y, z, ColorI(0, 0, 0, 0));
	//		}
	//	}
	//}

	m_clustersAddCount.clear();
	m_clustersData.clear();
	m_clustersAddCount.resize(ClusterWidth * ClusterHeight * ClusterDepth);
	m_clustersData.resize(ClusterWidth * ClusterHeight * ClusterDepth);

	m_lightInofs.clear();
	m_lightInofs.add(LightInfo{});	// dummy

	m_globalLightInofs.clear();
}

void LightClusters::addPointLight(const Vector3& pos, float range, float attenuation, const Color& color)
{
	addClusterSpherical(pos, range);

	LightInfo info;
	//info.posAndRange = Vector4(pos, range);
	info.posAndRange = Vector4(Vector3::transform(pos, m_view).xyz(), range);
	info.directionAndAtt = Vector4(0, 0, 0, attenuation);
	info.spotAngle = Vector4::Zero;
	info.color = color;
	m_lightInofs.add(info);
}

// TODO: Vector3 クラスへ
static Vector3 transformDirection(const Vector3& vec, const Matrix& mat)
{
	return Vector3(
		(((vec.x * mat.m11) + (vec.y * mat.m21)) + (vec.z * mat.m31)),
		(((vec.x * mat.m12) + (vec.y * mat.m22)) + (vec.z * mat.m32)),
		(((vec.x * mat.m13) + (vec.y * mat.m23)) + (vec.z * mat.m33)));

}

void LightClusters::addSpotLight(const Vector3& pos, float range, float attenuation, const Color& color, const Vector3& direction, float cone, float penumbra)
{
	// TODO: とりあえず球と同じ方法でクラスタ化。最悪パターンの想定なので不足は無いが、かなり無駄が多い。
	addClusterSpherical(pos, range);

	LightInfo info;
	//info.posAndRange = Vector4(pos, range);
	//info.directionAndAtt = Vector4(-direction, range);
	info.posAndRange = Vector4(Vector3::transform(pos, m_view).xyz(), range);
	info.directionAndAtt = Vector4(transformDirection(direction, m_view), attenuation);
	info.spotAngle = Vector4(cos(cone), cos(cone * (1.0 - penumbra)), 0, 0);
	info.color = color;
	m_lightInofs.add(info);
}

void LightClusters::addDirectionalLight(const Vector3& direction, const Color& color)
{
	GlobalLightInfo info;
	info.color = color;
	info.directionAndType = Vector4(transformDirection(direction, m_view), 1.0f + FLT_EPSILON);
	m_globalLightInofs.add(info);
}

void LightClusters::addAmbientLight(const Color& color)
{
	GlobalLightInfo info;
	info.color = color;
	info.directionAndType = Vector4(0.0f, 0.0f, 0.0f, 2.0f + FLT_EPSILON);
	m_globalLightInofs.add(info);
}
void LightClusters::addHemisphereLight(const Color& skyColor, const Color& groundColor)
{
	GlobalLightInfo info;
	info.color = skyColor;
	info.groundColor = groundColor;
	info.directionAndType = Vector4(0.0f, 0.0f, 0.0f, 3.0f + FLT_EPSILON);
	m_globalLightInofs.add(info);
}

void LightClusters::endMakeClusters()
{
	// Global lights
	{
		// 最大未満であれば番兵を入れることで末尾を検出できるようにする
		if (m_globalLightInofs.size() < MaxLights)
		{
			m_globalLightInofs.add(GlobalLightInfo{});
		}

		detail::TextureInternal::setMappedData(m_globalLightInfoTexture, &m_globalLightInofs[0]);
	}

	// Local lights
	{
		Bitmap2D* surface = m_clustersTexture->map(MapMode::Write);
		for (int y = 0; y < ClusterHeight; y++)
		{
			for (int x = 0; x < ClusterWidth; x++)
			{
				for (int z = 0; z < ClusterDepth; z++)
				{
					int tx = (y * ClusterWidth) + x;
					int ty = z;
					surface->setPixel32(tx, ty, m_clustersData[((ClusterWidth * ClusterHeight * z) + (ClusterWidth * y) + x)]);
				}
			}
		}

		detail::TextureInternal::setMappedData(m_lightInfoTexture, &m_lightInofs[0]);
	}
}

static float b3(float v1, float v2, float v3, float t)
{
	return (1.0f - t)*(1.0f - t)*v1 + 2.0f * (1.0f - t)*t*v2 + t * t*v3;
}

static float b3_2(float v1, float v2, float v3, float t)
{
	return t * t*v3;
}
static float b3_r(float v1, float v2, float v3, float t)
{
	//return t*t*v3;
	//return v3 / ( t * t);
	return sqrt(t);
}

static float bias(float b, float x) { return pow(x, log(b) / log(0.5)); }
//static float bias(float b, float x) { return x; }

static float bias_x1(float x) { return b3(0.0, 0.0, 1.0, x); }
static float bias_x1_r(float x) { return b3_r(0.0, 0.0, 1.0, x); }
static float bias_x2(float x) { return b3(0.0, 1.0, 1.0, x); }


void LightClusters::addClusterSpherical(const Vector3& pos, float range)
{
	float lightRadius = range;
	float lightId = m_lightInofs.size() + 1;
	float flipRange = m_farClip - m_nearClip;

	// カメラから見た位置。奥が Z+。一番手前は 0.0
	Vector4 cp = Vector4::transform(Vector4(pos, 1.0f), m_view);

	// ビュー空間での、AABB の前面と後面 (clip 内に clamp しないと、特に near より手前の時に次の viewPoints を求めるときの符号が反転してしまう)
	float zn = Math::clamp(cp.z - lightRadius, m_nearClip, m_farClip);
	float zf = Math::clamp(cp.z + lightRadius, m_nearClip, m_farClip);

	Vector3 viewPoints[4] =
	{
		Vector3::transformCoord(Vector3(cp.x - lightRadius, cp.y - lightRadius, zf), m_proj),	// 左上 奥
		Vector3::transformCoord(Vector3(cp.x + lightRadius, cp.y + lightRadius, zf), m_proj),	// 右下 奥
		Vector3::transformCoord(Vector3(cp.x - lightRadius, cp.y - lightRadius, zn), m_proj),	// 左上 手前
		Vector3::transformCoord(Vector3(cp.x + lightRadius, cp.y + lightRadius, zn), m_proj),	// 右下 手前
	};

	Vector3 vpMin, vpMax;
	vpMin.x = std::min(viewPoints[0].x, viewPoints[2].x);
	vpMin.y = std::min(viewPoints[0].y, viewPoints[2].y);
	vpMax.x = std::max(viewPoints[1].x, viewPoints[3].x);
	vpMax.y = std::max(viewPoints[1].y, viewPoints[3].y);

	// Viewport 空間での、AABB の前面と後面 (0.0..1.0)
	float vpZn = (zn - m_nearClip) / flipRange;
	float vpZf = (zf - m_nearClip) / flipRange;

	// 効率悪そうだけど、クラスタ情報の書き込むみ先を探すには全クラスタ数のインデックスベースで for しなければならない。
	// bios で指数関数的なクラスタ分割を行うので、座標からクラスタの範囲を求めてループするのはちょっと複雑。

	// index -> viewport
	float xs = 2.0f / ClusterWidth;
	float ys = 2.0f / ClusterHeight;
	float zs = 1.0f / ClusterDepth;

	float biasBase = 0.1;

	for (int y = 0; y < ClusterHeight; y++)
	{
		float cb = (ys * y) - 1.0f;
		float ct = (ys * (y + 1)) - 1.0f;
		if ((vpMax.y > cb && vpMin.y < ct))
		{
			for (int x = 0; x < ClusterWidth; x++)
			{
				float cr = (xs * x) - 1.0f;
				float cl = (xs * (x + 1)) - 1.0f;
				if ((vpMax.x > cl && vpMin.x < cr))
				{
					for (int z = 0; z < ClusterDepth; z++)
					{
						// 0.0..1.0
						float cn = zs * z;
						float cf = zs * (z + 1);

						float czn = bias_x1(cn);
						float czf = bias_x1(cf);
						if (vpZf > czn && vpZn < czf)
						{
							addClusterData(x, y, z, lightId);
						}
					}
				}
			}
		}
	}

	//float aa1 = bias_x1(0.8);
	//float aa2 = bias_x1(0.2);
	//float aa3 = bias_x2(0.8);
	//float aa4 = bias_x2(0.2);
	//float bb1 = aa2 + aa3;
	//float bb2 = aa1 + aa4;
	//float cc1 = bias_x1_r(aa1);
	////float cc2 = b3_2(0,0,1, aa1);

	//for (float z = 0; z < 1.0f; z += 0.01)
	//{
	//	float cz = bias_x1_r(z); //z;// bias(biasBase, z);//1.0 - bias(1.0 - biasBase, 1.0 - z);
	//	int i_cz = /*ClusterDepth - */(int)/*trunc*/(cz * ClusterDepth);
	//	printf("%f (%f): %d\n", z, cz, i_cz);
	//}

	//for (int z = 0; z < ClusterDepth; z++)
	//{
	//	// index -> viewport.z
	//	float cn = zs * z;
	//	float cf = zs * (z + 1);

	//	// viewport.z > cluster range
	//	float czn = bias_x1(cn);
	//	float czf = bias_x1(cf);
	//	//printf("%d: %f:%f r:%f\n", z, cn, czn, bias_x2(czn));
	//	printf("%d: %f .. %f\n", z, czn, czf);
	//	//printf("%d: %f:%f .. %f:%f\n", z, cn, czn, cf, czf);
	//}
}

void LightClusters::addClusterData(int x, int y, int z, int lightId)
{
	int clustersAddCountIndex = ((ClusterWidth * ClusterHeight * z) + (ClusterWidth * y) + x);
	int clustersAddCount = m_clustersAddCount[clustersAddCountIndex];
	if (clustersAddCount < 4)
	{
		ColorI& color = m_clustersData[clustersAddCountIndex];
		switch (clustersAddCount)
		{
		case 0:
			color.r = lightId;
			break;
		case 1:
			color.g = lightId;
			break;
		case 2:
			color.b = lightId;
			break;
		case 3:
			color.a = lightId;
			break;
		default:
			LN_UNREACHABLE();
			break;
		}

		m_clustersAddCount[clustersAddCountIndex]++;
	}
	else
	{
        LN_LOG_WARNING("The number of affect lights on clusters has exceeded 4.");
	}
}

} // namespace detail
} // namespace ln

