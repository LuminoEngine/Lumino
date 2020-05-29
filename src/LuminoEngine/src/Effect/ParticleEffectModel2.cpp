/*
 [2020/5/29] New ver 設計
 ----------

 モジュール分けを意識していきたい。今後も変化、というか機能追加ありそう。

 ### ParticleStorage
 個々のパーティクルの姿勢などの情報やライフタイム、生成と、そのデータのやりくりをするリングバッファ的なものを管理するもの。

 ### Modules
 Unity や UE4 のモジュール。パーティクルに対して何か変化を与えたいときはこれを Model に追加していく。

 ### Renderer
 Shape の Module ごとに存在。

 ### 関係

 - ParticleEffectModel
 - ParticleEffectEmitter
 この 2 つが 1:1 で、ユーザープログラムに公開するもののメイン。

 ParticleEffectEmitter
 - 最低1つの ParticleEmitterInstance を持つ。Model が Sub-emitter の定義を持っていれば、ParticleEmitterInstance は必要に応じてたくさんつくられる。
 - ShapeModule の key ごとに ParticleRenderer を持つ。
 
 ParticleModel と ParticleModelInstance が 1:1。それぞれ、ParticleEffectModel と ParticleEmitterInstance が複数持つ。



*/
#include "Internal.hpp"
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Effect/ParticleEffectModel2.hpp>

namespace ln {

//==============================================================================
// ParticleGeometry

ParticleGeometry::ParticleGeometry(ParticleGeometryType type)
    : m_type(type)
{
}

bool ParticleGeometry::init()
{
    if (!Object::init()) return false;
    return true;
}

//==============================================================================
// SpriteParticleGeometry

SpriteParticleGeometry::SpriteParticleGeometry()
    : ParticleGeometry(ParticleGeometryType::Sprite)
{
}

bool SpriteParticleGeometry::init()
{
    if (!Object::init()) return false;

    m_material = makeObject<Material>();
    m_material->setMainTexture(Texture2D::load(u"Sprite1"));

    return true;
}

void SpriteParticleGeometry::setMaterial(Material* material)
{
    m_material = material;
}

uint64_t SpriteParticleGeometry::calculateRendererHashKey() const
{
    return reinterpret_cast<uint64_t>(m_material.get());
}

//==============================================================================
// ParticleEmitterModel2

ParticleEmitterModel2::ParticleEmitterModel2()
{
}

bool ParticleEmitterModel2::init()
{
    if (!Object::init()) return false;

    auto geom = makeObject<SpriteParticleGeometry>();
    m_geometry = geom;

    return true;
}

//==============================================================================
// ParticleModel2

ParticleModel2::ParticleModel2()
{
}

bool ParticleModel2::init()
{
    if (!Object::init()) return false;

    auto emitter = makeObject<ParticleEmitterModel2>();
    m_emitters.add(emitter);

    return true;
}

//==============================================================================
// ParticleInstance2

namespace detail {

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
{
}

bool ParticleEmitterInstance2::init(ParticleInstance2* particleInstance, ParticleEmitterModel2* emitterModel)
{
    if (!Object::init()) return false;

    m_emitterModel = emitterModel;

    m_renderer = particleInstance->acquireRenderer(m_emitterModel->geometry());

    return true;
}

void ParticleEmitterInstance2::render()
{
    m_renderer->draw(Matrix::Identity);
}

//==============================================================================
// ParticleRenderer2

ParticleRenderer2::ParticleRenderer2(ParticleGeometryType type)
    : m_type(type)
    , m_hashKey(0)
{
}

bool ParticleRenderer2::init(uint64_t hashKey)
{
    if (!Object::init()) return false;
    m_hashKey = hashKey;
    return true;
}

//==============================================================================
// SpriteParticleRenderer

SpriteParticleRenderer::SpriteParticleRenderer()
    : ParticleRenderer2(ParticleGeometryType::Sprite)
{
}

bool SpriteParticleRenderer::init(uint64_t hashKey, Material* material)
{
    if (!ParticleRenderer2::init(hashKey)) return false;
    m_material = material;

    auto mesh = makeObject<Mesh>(4, 6, IndexBufferFormat::UInt16);
    auto vertices = reinterpret_cast<Vertex*>(mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main));
    auto indices = reinterpret_cast<uint16_t*>(mesh->acquireMappedIndexBuffer());

    // Front: Z+
    vertices[0] = Vertex{ Vector3(-0.5,  0.5, 0.0), Vector3::UnitZ, Vector2(0, 0), Color::White };
    vertices[1] = Vertex{ Vector3( 0.5,  0.5, 0.0), Vector3::UnitZ, Vector2(1, 0), Color::White };
    vertices[2] = Vertex{ Vector3(-0.5, -0.5, 0.0), Vector3::UnitZ, Vector2(0, 1), Color::White };
    vertices[3] = Vertex{ Vector3( 0.5, -0.5, 0.0), Vector3::UnitZ, Vector2(1, 1), Color::White };
    indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 2; indices[4] = 1; indices[5] = 3;

    mesh->addSection(0, 2, 0, PrimitiveTopology::TriangleList);

    m_batch = makeObject<InstancedMeshList>(mesh, 0);


    return true;
}

void SpriteParticleRenderer::draw(const Matrix& transform)
{
    m_batch->setTransform(transform);
    m_batch->drawMesh();
}

void SpriteParticleRenderer::resetBatch()
{
    m_batch->reset();
}

void SpriteParticleRenderer::submit(RenderingContext* context)
{


    context->setMaterial(m_material);
    context->drawMeshInstanced(m_batch);
}

} // namespace detail

} // namespace ln
