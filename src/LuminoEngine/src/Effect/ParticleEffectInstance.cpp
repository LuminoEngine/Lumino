
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
    : m_particleInstance(nullptr)
    , m_emitterModel(nullptr)
    , m_renderer(nullptr)
    , m_sleepCount(0)
    , m_time(0.0f)
    , m_lastSpawnTime(0.0f)
{
}

bool ParticleEmitterInstance2::init(ParticleInstance2* particleInstance, ParticleEmitterModel2* emitterModel)
{
    if (!Object::init()) return false;
    if (LN_REQUIRE(emitterModel)) return false;
    if (LN_REQUIRE(emitterModel->m_maxParticles < UINT16_MAX)) return false;

    m_particleInstance = particleInstance;
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

float ParticleEmitterInstance2::makeRandom(detail::ParticleData2* data, const RadomRangeValue<float>& value) const
{
    return makeRandom(data, value.minValue, value.maxValue, value.randomSource);
}

Vector3 ParticleEmitterInstance2::makeRandom(detail::ParticleData2* data, const RadomRangeValue<Vector3>& value) const
{
    return Vector3(
        makeRandom(data, value.minValue.x, value.maxValue.x, value.randomSource),
        makeRandom(data, value.minValue.y, value.maxValue.y, value.randomSource),
        makeRandom(data, value.minValue.z, value.maxValue.z, value.randomSource));
}

float ParticleEmitterInstance2::makeRandom(detail::ParticleData2* data, float minValue, float maxValue, ParticleRandomSource source) const
{
    if (minValue == maxValue) {
        return minValue;
    }
    else if (source == ParticleRandomSource::ByBaseValue)
    {
        return Math::lerp(minValue, maxValue, data->randomMasterValue);
    }
    else if (source == ParticleRandomSource::ByBaseValueInverse)
    {
        return Math::lerp(minValue, maxValue, data->randomMasterValue);
    }
    else
    {
        return m_particleInstance->rand().nextFloat(minValue, maxValue);
    }
}

void ParticleEmitterInstance2::updateFrame(float deltaTime)
{
    if (1) {

        killDeactiveParticles(deltaTime);

        {
            updateSpawn(deltaTime);
        }


        for (int i = m_sleepCount; i < m_activeParticles; i++) {
            const int currentIndex = m_particleIndices[i];
            simulateParticle(&m_particleData[currentIndex], deltaTime);
        }
    }
}

void ParticleEmitterInstance2::render()
{
    for (int i = m_sleepCount; i < m_activeParticles; i++) {
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
        if (particle.time + deltaTime >= particle.endLifeTime) {
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
            // burstCount 分、まとめて spawn
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

        particle->endLifeTime = makeRandom(particle, m_emitterModel->m_lifeTime.minValue, m_emitterModel->m_lifeTime.maxValue, m_emitterModel->m_lifeTime.randomSource);
    }
}

void ParticleEmitterInstance2::killParticle(int index)
{
    if (isLoop()) {
        // m_activeParticles-1 は、有効な最後の Particle。
        // これと、kill したい currentIndex を入れ替えることで、0~m_activeParticles までは
        // 有効なパーティクルだけ残しつつ、効率的にインデックスを返却できる。
        m_particleIndices[index] = m_particleIndices[m_activeParticles - 1];
        m_particleIndices[m_activeParticles - 1] = index;
        m_activeParticles--;
    }
    else {
        ParticleData2& particle = m_particleData[m_particleIndices[index]];
        particle.time = particle.endLifeTime;
        m_sleepCount++;
    }
}

void ParticleEmitterInstance2::simulateParticle(ParticleData2* particle, float deltaTime)
{
    particle->time += deltaTime;
    particle->position.x += 0.01;
}

} // namespace detail
} // namespace ln
