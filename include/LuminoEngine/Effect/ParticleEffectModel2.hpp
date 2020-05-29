#pragma once
#include "../Graphics/ColorStructs.hpp"
#include "Common.hpp"
#include "EffectModel.hpp"

namespace ln {

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
	const List<Ref<ParticleEmitterModel2>>& emitters() const { return m_emitters; }
	
LN_CONSTRUCT_ACCESS:
	ParticleModel2();
	bool init();

private:
	List<Ref<ParticleEmitterModel2>> m_emitters;
};

namespace detail {
class ParticleEmitterInstance2;
class ParticleRenderer2;

class ParticleInstance2
	: public ln::Object
{
public:
	void setWorldTransform(const Matrix& value);

	void updateFrame(float deltaTime);

	void render(RenderingContext* context);

	ParticleRenderer2* acquireRenderer(ParticleGeometry* geometry);

LN_CONSTRUCT_ACCESS:
	ParticleInstance2();
	bool init(ParticleModel2* model);

private:

	Ref<ParticleModel2> m_model;
	Matrix m_worldTransform;
	List<Ref<ParticleEmitterInstance2>> m_emitterInstances;
	List<Ref<ParticleRenderer2>> m_renderers;
};

class ParticleEmitterInstance2
	: public ln::Object
{
public:
	void render();

LN_CONSTRUCT_ACCESS:
	ParticleEmitterInstance2();
	bool init(ParticleInstance2* particleInstance, ParticleEmitterModel2* emitterModel);

private:
	ParticleEmitterModel2* m_emitterModel;
	ParticleRenderer2* m_renderer;

	// Sub-emitter がある場合、粒子の数だけ作られる
	List<Ref<ParticleEmitterInstance2>> m_subEmitters;
};

class ParticleRenderer2
	: public ln::Object
{
public:
	ParticleGeometryType type() const { return m_type; }
	uint64_t hashKey() const { return m_hashKey; }

	virtual void draw(const Matrix& transform) = 0;
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
	void draw(const Matrix& transform) override;
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
