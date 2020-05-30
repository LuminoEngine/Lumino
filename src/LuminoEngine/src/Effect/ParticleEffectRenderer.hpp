#pragma once
#include <LuminoEngine/Effect/ParticleEffectModel2.hpp>
#include "ParticleEffectInstance.hpp"

namespace ln {
class InstancedMeshList;

namespace detail {
class RibbonRenderer;

class ParticleRenderer2
	: public ln::Object
{
public:
	ParticleGeometryType type() const { return m_type; }
	uint64_t hashKey() const { return m_hashKey; }

	//void applySort(RenderingContext* context, ParticleEmitterInstance2* emitterInstance);

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
	void resetBatch() override;
	void draw(RenderingContext* context, const ParticleData2* particle) override;
	void submit(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	SpriteParticleRenderer();
	bool init(uint64_t hashKey, Material* material);

private:
	Ref<InstancedMeshList> m_batch;
	Ref<Material> m_material;

	Ref<RibbonRenderer> m_ribbonRenderer;	// TODO: tmp
};


class RibbonRenderer
	: public RefObject
{
public:
	RibbonRenderer();
	bool init(int maxPoints);

	void setMaterial(Material* material);
	void resetBatch();
	void addPoint(RenderingContext* context, const Vector3& pos, float width);
	void submit(RenderingContext* context);

private:
	Ref<Material> m_material;
	Ref<Mesh> m_mesh;
};

} // namespace detail
} // namespace ln
