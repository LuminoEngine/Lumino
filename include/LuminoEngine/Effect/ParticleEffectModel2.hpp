#pragma once
#include "../Graphics/ColorStructs.hpp"
#include "Common.hpp"
#include "EffectModel.hpp"
#include "ParticleEffectModel.hpp"

namespace ln {
class Material;

enum class ParticleGeometryType
{
	Sprite,
};

/** パーティクルのソート方法 */
enum class ParticleSortMode : uint8_t
{
	/** 
	 * ソートを行いません。加算合成や減算合成で描画するパーティクルは、このモードにすると高速に処理できます。
	 * 一方アルファ値を持つテクスチャは正しく描画できなくなります。(深度テストを無効化することである程度カバーできます)
	 */
	None,

	/**
	 * ビュー平面からの距離でソートを行います。
	 */
	DistanceToView,
};

class ParticleGeometry
	: public ln::Object
{
public:
	ParticleGeometryType type() const { return m_type; }
	virtual uint64_t calculateRendererHashKey() const = 0;

LN_CONSTRUCT_ACCESS:
	ParticleGeometry(ParticleGeometryType type);
	bool init();

private:
	ParticleGeometryType m_type;
};

class SpriteParticleGeometry
	: public ParticleGeometry
{
public:
	void setMaterial(Material* material);
	const Ref<Material>& material() const { return m_material; }
	uint64_t calculateRendererHashKey() const override;

LN_CONSTRUCT_ACCESS:
	SpriteParticleGeometry();
	bool init();

private:
	Ref<Material> m_material;
};


class ParticleEmitterModel2
	: public EffectResource
{
public:
	const Ref<ParticleGeometry>& geometry() const { return m_geometry; }


	void setSpriteModule(Material* material);


	/** 同時に表示できるパーティクルの最大数を設定します。(default: 100) */
	void setMaxParticles(int count) { m_maxParticles = count; }

	/** 1秒間に放出するパーティクルの数を設定します。(default: 1) */
	void setSpawnRate(float rate) { m_spawnRate = rate; }

	/** パーティクルの生存時間を設定します。(default: 5.0) */
	void setLifeTime(float time) { m_lifeTime.minValue = m_lifeTime.maxValue = time; }

	// パーティクルのライフタイムを秒単位で
	RadomRangeValue<float> m_lifeTime;



	int m_maxParticles = 1;		// 粒子最大数
	float m_spawnRate = 1;	// 1秒間に放出するパーティクル数
	int m_burstCount = 1;	// 1度の放出タイミングで生成するパーティクル数


	// Dynamics module
	RadomRangeValue<Vector3> m_velocity;
	RadomRangeValue<Vector3> m_acceleration;



	ParticleEmitterShapeType m_shapeType = ParticleEmitterShapeType::Sphere;
	Vector3 m_shapeParam;	// Coneの場合、.x が有効角度。0~PI。Boxの場合は各辺の幅
	RadomRangeValue<float> m_forwardPosition;
	RadomRangeValue<float> m_forwardVelocity = { 1, 1 };
	RadomRangeValue<float> m_forwardAccel;
	// ↑ forwardXXXX は、 ShapeType で決まった正面方向に対する Dynamics.
	// Sphere,Cone の場合は放射方向、Box の場合は Y+.
	


	RadomRangeValue<float> m_size;			// default:1 ベースの geom 対するスケール値。
	RadomRangeValue<float> m_forwardScale;	// default:1 進行方向に対するスケール値。通常、Z軸
	RadomRangeValue<float> m_crossScale;	// default:1 進行方向以外に対するスケール値。XとY軸


	ParticleSortMode m_sortMode = ParticleSortMode::None;


LN_CONSTRUCT_ACCESS:
	ParticleEmitterModel2();
	bool init();

private:
	Ref<ParticleGeometry> m_geometry;
};

class ParticleModel2
	: public EffectResource
{
public:
	bool m_loop = true;

	const List<Ref<ParticleEmitterModel2>>& emitters() const { return m_emitters; }


LN_CONSTRUCT_ACCESS:
	ParticleModel2();
	bool init();

private:
	List<Ref<ParticleEmitterModel2>> m_emitters;
};

} // namespace ln
