
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include "ParticleEffectRenderer.hpp"
#include "ParticleEffectInstance.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ParticleInstance2


ParticleInstance2::ParticleInstance2()
{
}

bool ParticleInstance2::init(ParticleModel2* model)
{
    if (LN_REQUIRE(model)) return false;
    if (!Object::init()) return false;

    m_model = model;

    for (auto& emitterModel : m_model->emitters()) {
        auto instance = makeObject<ParticleEmitterInstance2>(this, emitterModel);
        m_emitterInstances.add(instance);
    }

    return true;
}

void ParticleInstance2::setWorldTransform(const Matrix& value)
{
    m_worldTransform = value;
}

void ParticleInstance2::updateFrame(float deltaTime)
{
    for (auto& emitter : m_emitterInstances) {
        emitter->updateFrame(deltaTime);
    }
}

void ParticleInstance2::render(RenderingContext* context)
{
    for (auto& renderer : m_renderers) {
        renderer->resetBatch();
    }

    for (auto& emitter : m_emitterInstances) {
        emitter->render();
    }
    
    for (auto& renderer : m_renderers) {
        renderer->submit(context);
    }
}

ParticleRenderer2* ParticleInstance2::acquireRenderer(ParticleGeometry* geometry)
{
    uint64_t hashKey = geometry->calculateRendererHashKey();

    for (auto& renderer : m_renderers) {
        if (renderer->type() == geometry->type() && renderer->hashKey() == hashKey) {
            return renderer;
        }
    }

    if (geometry->type() == ParticleGeometryType::Sprite) {
        auto* g = static_cast<SpriteParticleGeometry*>(geometry);
        auto renderer = makeObject<SpriteParticleRenderer>(hashKey, g->material());
        m_renderers.add(renderer);
        return renderer;
    }
    else {
        LN_UNREACHABLE();
        return nullptr;
    }
}

//==============================================================================
// ParticleEmitterInstance2

ParticleEmitterInstance2::ParticleEmitterInstance2()
    : m_emitterModel(nullptr)
    , m_renderer(nullptr)
    , m_time(0.0f)
    , m_lastSpawnTime(0.0f)
{
}

bool ParticleEmitterInstance2::init(ParticleInstance2* particleInstance, ParticleEmitterModel2* emitterModel)
{
    if (!Object::init()) return false;
    if (LN_REQUIRE(emitterModel)) return false;
    if (LN_REQUIRE(emitterModel->m_maxParticles < UINT16_MAX)) return false;

    m_emitterModel = emitterModel;

    m_renderer = particleInstance->acquireRenderer(m_emitterModel->geometry());

    m_particleData.resize(m_emitterModel->m_maxParticles);
    m_activeParticles = 0;

    m_particleIndices.resize(m_emitterModel->m_maxParticles);
    for (int i = 0; i < m_emitterModel->m_maxParticles; i++) {
        m_particleIndices[i] = i;
    }

    return true;
}

void ParticleEmitterInstance2::updateFrame(float deltaTime)
{
    if (1) {

        killDeactiveParticles(deltaTime);

        {
            updateSpawn(deltaTime);
        }


        for (int i = 0; i < m_activeParticles; i++) {
            const int currentIndex = m_particleIndices[i];
            simulateParticle(&m_particleData[currentIndex], deltaTime);
        }
    }
}

void ParticleEmitterInstance2::render()
{
    for (int i = 0; i < m_activeParticles; i++) {
        const int currentIndex = m_particleIndices[i];
        m_renderer->draw(&m_particleData[currentIndex]);
    }
}

void ParticleEmitterInstance2::killDeactiveParticles(float deltaTime)
{
    for (int i = m_activeParticles - 1; i >= 0; i--) {
        const int currentIndex = m_particleIndices[i];
        const ParticleData2& particle = m_particleData[currentIndex];

        // 今回の updateFrame で消滅する予定のものも含めて kill しておく。
        // こうしておくことで、空いた領域を 今回の updateFrame ですぐに使いまわすことができる。
        if (particle.time + deltaTime > particle.lastLifeTime) {
            killParticle(currentIndex);
        }
    }
}

void ParticleEmitterInstance2::updateSpawn(float deltaTime)
{

    m_time += deltaTime;

    // create new particles
    {
        const float oneSpawnDeltaTime = m_emitterModel->m_spawnRate;
        while (m_lastSpawnTime <= m_time)
        {
            for (int i = 0; i < m_emitterModel->m_burstCount; i++) {
                if (m_activeParticles < maxParticles()) {
                    spawnParticle();
                }
            }
            m_lastSpawnTime += oneSpawnDeltaTime;
        }
    }
}

void ParticleEmitterInstance2::spawnParticle()
{
    LN_CHECK(m_activeParticles < maxParticles());

    const int newParticleIndex = m_activeParticles;
    m_activeParticles++;

    ParticleData2* particle = &m_particleData[newParticleIndex];

    // Initialize particle
    {
        particle->position = Vector3::Zero;

        particle->time = 0.0f;
        particle->lastLifeTime = 5.0f;
    }
}

void ParticleEmitterInstance2::killParticle(int index)
{
    // m_activeParticles-1 は、有効な最後の Particle。
    // これと、kill したい currentIndex を入れ替えることで、0~m_activeParticles までは
    // 有効なパーティクルだけ残しつつ、効率的にインデックスを返却できる。
    m_particleIndices[index] = m_particleIndices[m_activeParticles - 1];
    m_particleIndices[m_activeParticles - 1] = index;
    m_activeParticles--;
}

void ParticleEmitterInstance2::simulateParticle(ParticleData2* particle, float deltaTime)
{
    particle->time += deltaTime;
    particle->position.x += 0.01;
}

} // namespace detail
} // namespace ln
