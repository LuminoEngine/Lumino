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

	// パーティクルのライフタイムを秒単位で
	RadomRangeValue<float> m_lifeTime;



	int m_maxParticles = 1;		// 粒子最大数
	float m_spawnRate = 1;	// 1秒間に放出するパーティクル数
	int m_burstCount = 1;	// 1度の放出タイミングで生成するパーティクル数


LN_CONSTRUCT_ACCESS :
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
