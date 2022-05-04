
#include "Internal.hpp"
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/RenderView.hpp>
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include "ParticleEffectRenderer.hpp"
#include "ParticleEffectInstance.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ParticleInstance2

ParticleInstance2::ParticleInstance2()
{
}

bool ParticleInstance2::init(ParticleModel* model)
{
    if (LN_REQUIRE(model)) return false;
    if (!Object::init()) return false;

    m_model = model;

    if (m_model->seed != 0) {
        m_rand.setSeed(m_model->seed);
    }
    

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

ParticleRenderer2* ParticleInstance2::acquireRenderer(ParticleEmitterModel* emitterModel)
{
    ParticleGeometry* geometry = emitterModel->geometry();
    uint64_t hashKey = geometry->calculateRendererHashKey(emitterModel);

    for (auto& renderer : m_renderers) {
        if (renderer->type() == geometry->type() && renderer->hashKey() == hashKey) {
            return renderer;
        }
    }

    if (geometry->type() == ParticleGeometryType::Sprite) {
        auto* g = static_cast<SpriteParticleGeometry*>(geometry);
        auto renderer = makeObject<SpriteParticleRenderer>(hashKey, g->material(), emitterModel->m_geometryDirection);
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

bool ParticleEmitterInstance2::init(ParticleInstance2* particleInstance, ParticleEmitterModel* emitterModel)
{
    if (!Object::init()) return false;
    if (LN_REQUIRE(emitterModel)) return false;
    if (LN_REQUIRE(emitterModel->m_maxParticles < UINT16_MAX)) return false;

    m_particleInstance = particleInstance;
    m_emitterModel = emitterModel;

    m_renderer = particleInstance->acquireRenderer(m_emitterModel);

    m_particleStorage.resize(m_emitterModel->m_maxParticles);

    if (emitterModel->m_trailSeconds > 0.0f) {
        m_moduleInsances = makeObject<TrailParticleModuleInstance>(this, emitterModel);
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


        for (int i = m_sleepCount; i < activeParticles(); i++) {
            const uint16_t currentDataId = m_particleStorage.dataId(i);
            simulateParticle(&m_particleStorage.data(currentDataId), deltaTime);
        }

        if (m_moduleInsances && m_moduleInsances->isTrailEnabled()) {
            const ParticleDataId* idList = m_particleStorage.idList().data();
            ParticleData2* dataList = m_particleStorage.dataList().data();
            m_moduleInsances->onUpdateParticles(activeParticles() - m_sleepCount, idList + m_sleepCount, dataList, deltaTime);
        }


    }
}

void ParticleEmitterInstance2::render(RenderingContext* context)
{
    if (m_emitterModel->m_sortMode == ParticleSortMode::DistanceToView) {
        const auto& viewPosition = context->viewPoint()->viewPosition;
        const auto& viewDirection = context->viewPoint()->viewDirection;

        for (int i = m_sleepCount; i < activeParticles(); i++) {
            const uint16_t currentDataId = m_particleStorage.dataId(i);
            ParticleData2& particle = m_particleStorage.data(currentDataId);
            particle.zDistance = Vector3::dot(particle.position - viewPosition, viewDirection);
        }

        class SpriteCmpDepthBackToFront
        {
        public:
            IndicedNodeDataStorage<ParticleData2>* storage;

            bool operator()(int leftId, int rightId)
            {
                const auto& lsp = storage->data(leftId);
                const auto& rsp = storage->data(rightId);
                return lsp.zDistance > rsp.zDistance;
            }
        };

        SpriteCmpDepthBackToFront cmp;
        cmp.storage = &m_particleStorage;
        std::sort(m_particleStorage.idList().begin(), m_particleStorage.idList().begin() + m_particleStorage.activeIdCount(), cmp);
    }
    

    for (int i = m_sleepCount; i < activeParticles(); i++) {
        const int currentDataId = m_particleStorage.dataId(i);
        m_renderer->draw(context , &m_particleStorage.data(currentDataId));
    }


    if (m_moduleInsances && m_moduleInsances->isTrailEnabled()) {
        const ParticleDataId* idList = m_particleStorage.idList().data();
        ParticleData2* dataList = m_particleStorage.dataList().data();
        m_moduleInsances->onRenderParticles(activeParticles() - m_sleepCount, idList + m_sleepCount, dataList, context);
    }

}

void ParticleEmitterInstance2::killDeactiveParticles(float deltaTime)
{
    for (int i = activeParticles() - 1; i >= 0; i--) {
        const int currentDataId = m_particleStorage.dataId(i);
        ParticleData2* particle = &m_particleStorage.data(currentDataId);

        // 今回の updateFrame で消滅する予定のものも含めて kill しておく。
        // こうしておくことで、空いた領域を 今回の updateFrame ですぐに使いまわすことができる。
        if (particle->time + deltaTime >= particle->endLifeTime) {
            //printf("kill: %f, %f\n", particle.time, particle.endLifeTime);
            //killParticle(currentIndex);


            if (isLoop()) {

                if (m_moduleInsances) {
                    m_moduleInsances->onKillParticle(particle);
                }


                m_particleStorage.freeId(i);
            }
            else {
                //ParticleData2& particle = m_particleStorage.data(currentDataId);
                particle->time = particle->endLifeTime;
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
                if (activeParticles() < maxParticles()) {
                    spawnParticle(-(m_time - m_lastSpawnTime));
                }
            }
            m_lastSpawnTime += oneSpawnDeltaTime;
        }
    }
}

void ParticleEmitterInstance2::spawnParticle(float delayTime)
{
    LN_CHECK(activeParticles() < maxParticles());

    const int newParticleDataId = m_particleStorage.newId();//m_particleIndices[m_activeParticles];
    //m_activeParticles++;

    ParticleData2* particle = &m_particleStorage.data(newParticleDataId);// &m_particleData[newParticleDataId];

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
        particle->sizeVelocity = makeRandom(particle, m_emitterModel->m_sizeVelocity);
        particle->sizeAccel = makeRandom(particle, m_emitterModel->m_sizeAcceleration);

        particle->forwardScale = makeRandom(particle, m_emitterModel->m_forwardScale);
        particle->forwardScaleVelocity = makeRandom(particle, m_emitterModel->m_forwardScaleVelocity);
        particle->forwardScaleAccel = makeRandom(particle, m_emitterModel->m_forwardScaleAcceleration);

        particle->crossScale = makeRandom(particle, m_emitterModel->m_crossScale);
        particle->crossScaleVelocity = makeRandom(particle, m_emitterModel->m_crossScaleVelocity);
        particle->crossScaleAccel = makeRandom(particle, m_emitterModel->m_crossScaleAcceleration);
    }

    // Emitter shape
    {
        Vector3 localPosition = Vector3::Zero;
        Vector3 localFront = Vector3::UnitZ;    // ローカル空間上での進行方向
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

        // ワールド空間上の進行方向
        Vector3 worldFront = localFront;
        worldFront.transformDirection(emitterTransform);
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



    particle->sizeVelocity += particle->sizeAccel * deltaTime;
    particle->size += particle->sizeVelocity * deltaTime;

    particle->forwardScaleVelocity += particle->forwardScaleAccel * deltaTime;
    particle->forwardScale += particle->forwardScaleVelocity * deltaTime;

    particle->crossScaleVelocity += particle->crossScaleAccel * deltaTime;
    particle->crossScale += particle->crossScaleVelocity * deltaTime;
}

//==============================================================================
// TrailParticleModuleInstance

TrailParticleModuleInstance::TrailParticleModuleInstance()
{
}

bool TrailParticleModuleInstance::init(ParticleEmitterInstance2* emitterInstance, ParticleEmitterModel* emitterModel)
{
    if (!Object::init()) return false;
    m_emitterModel = emitterModel;

    // Trail
    {
        int frameRate = 60;
        int estimationNodeCount = (m_emitterModel->m_trailSeconds * frameRate) * emitterInstance->emitterModel()->m_maxParticles;
        resizeTrailData(estimationNodeCount);
        m_trailRateTime = m_emitterModel->m_trailSeconds / frameRate;

        m_ribbonRenderer = makeRef<RibbonRenderer>();
        if (!m_ribbonRenderer->init(estimationNodeCount)) {
            return false;
        }
    }

    return true;
}

void TrailParticleModuleInstance::onUpdateParticles(int count, const ParticleDataId* idList, ParticleData2* dataList, float deltaTime)
{

    // Trail-module
    {
        for (int i = 0; i < count; i++) {
            const ParticleDataId particleId = idList[i];
            ParticleData2* particle = dataList + particleId;


            //while (particle->lastTrailTime <= particle->time)
            //{
                // 新しく Node を作って、List の先頭に差し込む
            {
                int prevHeadId = particle->headTrailNodeId;
                ParticleTrailNode* prevHead = nullptr;
                if (prevHeadId >= 0) {
                    prevHead = trailData(prevHeadId);
                    prevHead->headParticle = nullptr;
                }

                int newId = newTrailDataId();
                ParticleTrailNode* node = trailData(newId);
                node->time = 0.0f;
                node->prevId = -1;
                node->nextId = prevHeadId;

                // Insert to head of linked-list
                if (prevHead) {
                    prevHead->prevId = newId;
                }
                particle->headTrailNodeId = newId;
                node->headParticle = particle;
                //if (prevHeadId >= 0) {
                //    node->nextId = prevHeadId;
                //}


                node->pos = particle->position;
            }


            //particle->lastTrailTime += m_trailRateTime;
        //}
        }
    }

    // Trail-module
    {
        for (int i = m_trailNodeStorage.activeIdCount() - 1; i >= 0; i--) {
            const TrailDataId dataId = m_trailNodeStorage.dataId(i);
            ParticleTrailNode* node = &m_trailNodeStorage.data(dataId);

            node->time += deltaTime;
            if (node->time >= m_emitterModel->m_trailSeconds) {
                // kill

                // Remove from linked-list
                //if (node->prevId >= 0) {
                //    ParticleTrailNode* prev = &m_trailNodeStorage.data(node->prevId);
                //    prev->nextId = node->nextId;
                //}
                //if (node->nextId >= 0) {
                //    ParticleTrailNode* next = &m_trailNodeStorage.data(node->nextId);
                //    next->prevId = node->prevId;
                //}
                //node->prevId = -1;
                //node->nextId = -1;
                removeNode(dataId);

                removeAliveNode(dataId);

                m_trailNodeStorage.freeId(i);
            }
        }
    }
}

void TrailParticleModuleInstance::onKillParticle(ParticleData2* particle)
{
    // Trail-module
    if (particle->headTrailNodeId >= 0) {
        ParticleTrailNode* head = &m_trailNodeStorage.data(particle->headTrailNodeId);
        LN_DCHECK(head->headParticle == particle);
        head->headParticle = nullptr;
        particle->headTrailNodeId = -1;
        // TODO: head 以下すべて free する
    }
}

void TrailParticleModuleInstance::onRenderParticles(int count, const ParticleDataId* idList, ParticleData2* dataList, RenderingContext* context)
{
    // Trail-module
    if (m_ribbonRenderer) {
        m_ribbonRenderer->resetBatch();

        //for (int i = m_sleepCount; i < activeParticles(); i++) {
        //    const int currentDataId = m_particleStorage.dataId(i);
        //    const auto& particle = m_particleStorage.data(currentDataId);

        for (int i = 0; i < count; i++) {
            const ParticleDataId particleId = idList[i];
            ParticleData2* particle = dataList + particleId;

            m_ribbonRenderer->beginRibbon();

            int nodeId = particle->headTrailNodeId;
            while (nodeId >= 0) {
                const auto* node = trailData(nodeId);

                m_ribbonRenderer->addPoint(context, node->pos, 0.2);

                nodeId = node->nextId;
            }

            m_ribbonRenderer->endRibbon();
        }



        m_ribbonRenderer->submit(context);
    }
}

void TrailParticleModuleInstance::resizeTrailData(int size)
{
    m_trailNodeStorage.resize(size);
    //m_trailNodeData.resize(size);
    //for (int i = 0; i < size; i++) {
    //    m_trailDataIdStack.push(i);
    //}
}

TrailParticleModuleInstance::TrailDataId TrailParticleModuleInstance::newTrailDataId()
{
    TrailDataId id;
    if (m_trailNodeStorage.activeIdCount() >= m_trailNodeStorage.maxDataCount()) {
        // Data を使い切ってしまったら、一番古いものを消して再利用する
        id = dequeueAliveNode();
    }
    else {
        id = m_trailNodeStorage.newId();
    }
    LN_DCHECK(id >= 0);

    enqueueAliveNode(id);
    return id;
    //if (m_trailDataIdStack.empty()) return -1;
    //int id = m_trailDataIdStack.top();
    //m_trailDataIdStack.pop();
    //return id;
}

void TrailParticleModuleInstance::removeNode(TrailDataId id)
{

    // 既につながれている別の List から取り除く
    {
        const TrailDataId selfId = id;
        ParticleTrailNode* self = &m_trailNodeStorage.data(selfId);

        if (self->headParticle) {
            LN_DCHECK(self->headParticle->headTrailNodeId == selfId);
            self->headParticle->headTrailNodeId = self->nextId;
        }

        if (self->prevId >= 0) {
            ParticleTrailNode* prev = &m_trailNodeStorage.data(self->prevId);
            LN_DCHECK(prev->nextId == selfId);
            prev->nextId = self->nextId;
        }

        if (self->nextId >= 0) {
            ParticleTrailNode* next = &m_trailNodeStorage.data(self->nextId);
            LN_DCHECK(next->prevId == selfId);
            next->prevId = self->prevId;
        }

        self->headParticle = nullptr;
        self->prevId = -1;
        self->nextId = -1;
    }

}

void TrailParticleModuleInstance::enqueueAliveNode(TrailDataId id)
{
    ParticleTrailNode* self = &m_trailNodeStorage.data(id);
    LN_CHECK(self->aliveQueuePrevId == -1);
    LN_CHECK(self->aliveQueueNextId == -1);

    ParticleTrailNode* prev = nullptr;
    if (m_trailNodeAliveQueueTailId >= 0) {
        prev = &m_trailNodeStorage.data(m_trailNodeAliveQueueTailId);
        LN_CHECK(prev->aliveQueueNextId == -1);
        prev->aliveQueueNextId = id;
    }

    self->aliveQueuePrevId = m_trailNodeAliveQueueTailId;
    m_trailNodeAliveQueueTailId = id;

    if (m_trailNodeAliveQueueHeadId == -1) {
        m_trailNodeAliveQueueHeadId = id;

        // hook
        if (m_trailNodeAliveQueueHeadId == -1) {
            printf("");
        }
    }


}

TrailParticleModuleInstance::TrailDataId TrailParticleModuleInstance::dequeueAliveNode()
{
    TrailDataId retId = m_trailNodeAliveQueueHeadId;

    removeAliveNode(retId);
    //if (m_trailNodeAliveQueueHeadId >= 0) {
    //    ParticleTrailNode* head = &m_trailNodeStorage.data(m_trailNodeAliveQueueHeadId);
    //    LN_CHECK(head->aliveQueuePrevId == -1);

    //    if (head->aliveQueueNextId >= 0) {
    //        ParticleTrailNode* next = &m_trailNodeStorage.data(head->aliveQueueNextId);
    //        LN_CHECK(next->aliveQueuePrevId >= 0);
    //        next->aliveQueuePrevId = -1;
    //    }

    //    head->aliveQueueNextId = -1;
    //}


    removeNode(retId);

    return retId;
}

void TrailParticleModuleInstance::removeAliveNode(TrailDataId id)
{
    ParticleTrailNode* self = &m_trailNodeStorage.data(id);

    if (id == m_trailNodeAliveQueueHeadId) {
        LN_DCHECK(self->aliveQueuePrevId == -1);
        m_trailNodeAliveQueueHeadId = self->aliveQueueNextId;

        // hook
        if (m_trailNodeAliveQueueHeadId == -1) {
            printf("");
        }
    }

    if (id == m_trailNodeAliveQueueTailId) {
        LN_DCHECK(self->aliveQueueNextId == -1);
        m_trailNodeAliveQueueTailId = self->aliveQueuePrevId;
    }

    if (self->aliveQueuePrevId >= 0) {
        ParticleTrailNode* prev = &m_trailNodeStorage.data(self->aliveQueuePrevId);
        LN_DCHECK(prev->aliveQueueNextId == id);
        prev->aliveQueueNextId = self->aliveQueueNextId;
    }

    if (self->aliveQueueNextId >= 0) {
        ParticleTrailNode* next = &m_trailNodeStorage.data(self->aliveQueueNextId);
        LN_DCHECK(next->aliveQueuePrevId == id);
        next->aliveQueuePrevId = self->aliveQueuePrevId;
    }

    self->aliveQueuePrevId = -1;
    self->aliveQueueNextId = -1;
}

} // namespace detail
} // namespace ln
