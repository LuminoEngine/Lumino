#pragma once
#include <LuminoEngine/Effect/ParticleEffectModel2.hpp>
#include "ParticleEffectInstance.hpp"

namespace ln {
class InstancedMeshList;

namespace detail {

class ParticleRenderer2
	: public ln::Object
{
public:
	ParticleGeometryType type() const { return m_type; }
	uint64_t hashKey() const { return m_hashKey; }

	virtual void draw(RenderingContext* context, const ParticleData2* particle) = 0;
	virtual void resetBatch() = 0;
	virtual void submit(RenderingContext* context) = 0;
	
LN_CONSTRUCT_ACCESS:
	ParticleRenderer2(ParticleGeometryType type);
	bool init(uint64_t hashKey);

private:
	ParticleGeometryType m_type;
	uint64_t m_hashKey;
};


class SpriteParticleRenderer
	: public ParticleRenderer2
{
public:
	void draw(RenderingContext* context, const ParticleData2* particle) override;
	void resetBatch() override;
	void submit(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	SpriteParticleRenderer();
	bool init(uint64_t hashKey, Material* material);

private:
	Ref<InstancedMeshList> m_batch;
	Ref<Material> m_material;
};

} // namespace detail
} // namespace ln
