
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
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
        emitter->render(context);
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
            const int currentDataIndex = m_particleIndices[i];
            simulateParticle(&m_particleData[currentDataIndex], deltaTime);
        }
    }
}

void ParticleEmitterInstance2::render(RenderingContext* context)
{
    if (m_emitterModel->m_sortMode == ParticleSortMode::DistanceToView) {
        const auto& viewPosition = context->viewPoint()->viewPosition;
        const auto& viewDirection = context->viewPoint()->viewDirection;

        for (int i = m_sleepCount; i < m_activeParticles; i++) {
            ParticleData2& particle = m_particleData[m_particleIndices[i]];
            particle.zDistance = Vector3::dot(particle.position - viewPosition, viewDirection);
        }

        class SpriteCmpDepthBackToFront
        {
        public:
            std::vector<detail::ParticleData2>* particleData;

            bool operator()(int left, int right)
            {
                const auto& lsp = particleData->at(left);
                const auto& rsp = particleData->at(right);
                return lsp.zDistance > rsp.zDistance;
            }
        };

        SpriteCmpDepthBackToFront cmp;
        cmp.particleData = &m_particleData;
        std::sort(m_particleIndices.begin(), m_particleIndices.begin() + m_activeParticles, cmp);
    }
    

    for (int i = m_sleepCount; i < m_activeParticles; i++) {
        const int currentDataIndex = m_particleIndices[i];
        m_renderer->draw(context , &m_particleData[currentDataIndex]);
    }
}

void ParticleEmitterInstance2::killDeactiveParticles(float deltaTime)
{
    for (int i = m_activeParticles - 1; i >= 0; i--) {
        const int currentDataIndex = m_particleIndices[i];
        const ParticleData2& particle = m_particleData[currentDataIndex];

        // 今回の updateFrame で消滅する予定のものも含めて kill しておく。
        // こうしておくことで、空いた領域を 今回の updateFrame ですぐに使いまわすことができる。
        if (particle.time + deltaTime >= particle.endLifeTime) {
            //printf("kill: %f, %f\n", particle.time, particle.endLifeTime);
            //killParticle(currentIndex);


            if (isLoop()) {
                // m_activeParticles-1 は、有効な最後の Particle。
                // これと、kill したい currentIndex を入れ替えることで、0~m_activeParticles までは
                // 有効なパーティクルだけ残しつつ、効率的にインデックスを返却できる。
                m_particleIndices[i] = m_particleIndices[m_activeParticles - 1];
                m_particleIndices[m_activeParticles - 1] = currentDataIndex;
                m_activeParticles--;
            }
            else {
                ParticleData2& particle = m_particleData[m_particleIndices[i]];
                particle.time = particle.endLifeTime;
                m_sleepCount++;
            }


        }
    }
}

void ParticleEmitterInstance2::updateSpawn(float deltaTime)
{
    m_time += deltaTime;

    // create new particles
    {
        const float oneSpawnDeltaTime = 1.0f / m_emitterModel->m_spawnRate;
        while (m_lastSpawnTime <= m_time)
        {
            // burstCount 分、まとめて spawn
            for (int i = 0; i < m_emitterModel->m_burstCount; i++) {
                if (m_activeParticles < maxParticles()) {
                    spawnParticle(-(m_time - m_lastSpawnTime));
                }
            }
            m_lastSpawnTime += oneSpawnDeltaTime;
        }
    }
}

void ParticleEmitterInstance2::spawnParticle(float delayTime)
{
    LN_CHECK(m_activeParticles < maxParticles());

    const int newParticleDataIndex = m_particleIndices[m_activeParticles];
    m_activeParticles++;

    ParticleData2* particle = &m_particleData[newParticleDataIndex];

    // Initialize particle
    {
        particle->position = Vector3::Zero;

        // 次の simulateParticle() で time を加算するとき、本来の時間になるようにする。
        // 例えば 1s に 10 個生成する設定で、updateFrame() の deltaTime が 1 だった場合、
        // updateSpawn() で 10 個作られるが、その際 delayTime は順に [-1.0, -0.9, -0.8...] といったように設定される。
        // 次の updateFrame() 全 Particle に +1.0 されるので、更新が終わった時には本来の時間になる。
        particle->time = delayTime;

        particle->endLifeTime = makeRandom(particle, m_emitterModel->m_lifeTime.minValue, m_emitterModel->m_lifeTime.maxValue, m_emitterModel->m_lifeTime.randomSource);
    
    
        particle->size = makeRandom(particle, m_emitterModel->m_size);
        particle->forwardScale = makeRandom(particle, m_emitterModel->m_forwardScale);
        particle->crossScale = makeRandom(particle, m_emitterModel->m_crossScale);
    }

    // Emitter shape
    {
        Vector3 localPosition = Vector3::Zero;
        Vector3 localFront = Vector3::UnitZ;
        const auto& shapeParam = m_emitterModel->m_shapeParam;

        switch (m_emitterModel->m_shapeType)
        {
            case ParticleEmitterShapeType::Sphere:
            {
                localFront.x = makeRandom(particle, -1.0, 1.0, ParticleRandomSource::Self);
                localFront.y = makeRandom(particle, -1.0, 1.0, ParticleRandomSource::Self);
                localFront.z = makeRandom(particle, -1.0, 1.0, ParticleRandomSource::Self);
                localFront = Vector3::safeNormalize(localFront, Vector3::UnitZ);
                break;
            }
            case ParticleEmitterShapeType::Cone:
            {
                // まず、XZ 平面で Y+ を前方として角度制限付きの位置を求める。
                float r = makeRandom(particle, 0.0f, shapeParam.x, ParticleRandomSource::Self);
                Vector3 vec;
                vec.y = sinf(r);	// TODO: Asm::sincos
                vec.z = cosf(r);

                // 次に、Y 軸周りの回転を行う。回転角度は 360度 ランダム。
                r = makeRandom(particle, 0.0f, Math::PI * 2, ParticleRandomSource::Self);
                localFront.x = sinf(r) * vec.y;
                localFront.y = vec.z;
                localFront.z = cosf(r) * vec.y;
                break;
            }
            case ParticleEmitterShapeType::Box:
            {
                localPosition.x = makeRandom(particle, -shapeParam.x * 0.5f, shapeParam.x * 0.5f, ParticleRandomSource::Self);
                localPosition.y = makeRandom(particle, -shapeParam.y * 0.5f, shapeParam.y * 0.5f, ParticleRandomSource::Self);
                localPosition.z = makeRandom(particle, -shapeParam.z * 0.5f, shapeParam.z * 0.5f, ParticleRandomSource::Self);
                localFront = Vector3::UnitY;
                break;
            }
            default:
                LN_UNREACHABLE();
                break;
        }

        const Matrix& emitterTransform = worldTransform();
        Vector3 worldFront = Vector3::transformCoord(localFront, emitterTransform);
        //Vector3 worldPosition = Vector3::transformCoord(localFront, emitterTransform);

        particle->position = localPosition + localFront * makeRandom(particle, m_emitterModel->m_forwardPosition);

        particle->position.transformCoord(emitterTransform);
        //TODO: 回転だけのtransformCoord
        //data->positionVelocity.transformCoord(emitterTransform);
        //data->positionAccel.transformCoord(emitterTransform);


        particle->linearVelocity = worldFront * makeRandom(particle, m_emitterModel->m_forwardVelocity);
        particle->linearAccel = worldFront * makeRandom(particle, m_emitterModel->m_forwardAccel);



        // Apply dinamics-module
        particle->linearVelocity += makeRandom(particle, m_emitterModel->m_velocity);
        particle->linearAccel += makeRandom(particle, m_emitterModel->m_acceleration);
    }
}

//void ParticleEmitterInstance2::killParticle(int index)
//{
//    if (isLoop()) {
//        // m_activeParticles-1 は、有効な最後の Particle。
//        // これと、kill したい currentIndex を入れ替えることで、0~m_activeParticles までは
//        // 有効なパーティクルだけ残しつつ、効率的にインデックスを返却できる。
//        m_particleIndices[index] = m_particleIndices[m_activeParticles - 1];
//        m_particleIndices[m_activeParticles - 1] = index;
//        m_activeParticles--;
//    }
//    else {
//        ParticleData2& particle = m_particleData[m_particleIndices[index]];
//        particle.time = particle.endLifeTime;
//        m_sleepCount++;
//    }
//}

void ParticleEmitterInstance2::simulateParticle(ParticleData2* particle, float deltaTime)
{
    particle->time += deltaTime;
    //particle->position.x += 0.01;


    particle->linearVelocity += particle->linearAccel * deltaTime;
    particle->position += particle->linearVelocity * deltaTime;

}

} // namespace detail
} // namespace ln
