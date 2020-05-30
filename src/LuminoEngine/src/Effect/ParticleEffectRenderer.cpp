
#include "Internal.hpp"
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include "ParticleEffectRenderer.hpp"

namespace ln {
namespace detail {

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

//void ParticleRenderer2::applySort(RenderingContext* context, ParticleEmitterInstance2* emitterInstance)
//{
//
//}

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

    // Front: Z-
    //vertices[0] = Vertex{ Vector3(-0.5,  0.5, 0.0), Vector3::UnitZ, Vector2(0, 0), Color::White };
    //vertices[1] = Vertex{ Vector3( 0.5,  0.5, 0.0), Vector3::UnitZ, Vector2(1, 0), Color::White };
    //vertices[2] = Vertex{ Vector3(-0.5, -0.5, 0.0), Vector3::UnitZ, Vector2(0, 1), Color::White };
    //vertices[3] = Vertex{ Vector3( 0.5, -0.5, 0.0), Vector3::UnitZ, Vector2(1, 1), Color::White };
    //indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 2; indices[4] = 1; indices[5] = 3;

    // Front: Y+, 進行方向を Z+ と考えたパターン。
    vertices[0] = Vertex{ Vector3( 0.5, 0.0, -0.5), Vector3::UnitZ, Vector2(0, 0), Color::White };
    vertices[1] = Vertex{ Vector3(-0.5, 0.0, -0.5), Vector3::UnitZ, Vector2(1, 0), Color::White };
    vertices[2] = Vertex{ Vector3( 0.5, 0.0,  0.5), Vector3::UnitZ, Vector2(0, 1), Color::White };
    vertices[3] = Vertex{ Vector3(-0.5, 0.0,  0.5), Vector3::UnitZ, Vector2(1, 1), Color::White };
    indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 2; indices[4] = 1; indices[5] = 3;



    mesh->addSection(0, 2, 0, PrimitiveTopology::TriangleList);

    m_batch = makeObject<InstancedMeshList>(mesh, 0);



    m_ribbonRenderer = makeRef<RibbonRenderer>();
    if (!m_ribbonRenderer->init(1000)) {
        return false;
    }

    return true;
}

void SpriteParticleRenderer::draw(RenderingContext* context, const ParticleData2* particle)
{
    // Rotation
    //  やってることは WorldObjectTransform::lookAt() とかと同じ。
    //  View 行列の回転成分を作るのと同じ要領で、Front を進行方向、Up をカメラへの方向と考えて計算する。
    const auto& viewPosition = context->viewPoint()->viewPosition;
    const auto rotUp = Vector3::safeNormalize(viewPosition - particle->position, Vector3::UnitZ);
    const auto rotFront = Vector3::safeNormalize(particle->linearVelocity, Vector3::UnitY); // 速度を持っていない場合は Y+ に進んでいることにする (Z+ だと Lumino デフォルト状態で表示できない)
    auto rotRight = Vector3::cross(rotUp, rotFront);
    if (Vector3::nearEqual(rotRight, Vector3::Zero)) {
        rotRight = Vector3::cross(Vector3::UnitZ, rotFront);
    }
    rotRight.mutatingNormalize();

    // Scale
    const auto scale = Vector3(
        particle->size * particle->crossScale,
        particle->size * particle->crossScale,
        particle->size * particle->forwardScale);

    // Position
    const auto pos = particle->position;

    // 愚直に Scale * Rotation * Position の行列乗算をパーティクルごとに繰り返すと
    // 計算負荷が馬鹿にならなくなるので、各要素を直接設定してしまう。
    const auto transform = Matrix(
        scale.x * rotRight.x, scale.x * rotRight.y, scale.x * rotRight.z, 0.0f,
        scale.y * rotUp.x, scale.y * rotUp.y, scale.y * rotUp.z, 0.0f,
        scale.z * rotFront.x, scale.z * rotFront.y, scale.z * rotFront.z, 0.0f,
        pos.x, pos.y, pos.z, 1.0f
    );

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

    m_ribbonRenderer->submit(context);
}


//==============================================================================
// RibbonRenderer

RibbonRenderer::RibbonRenderer()
{
}

bool RibbonRenderer::init(int maxPoints)
{
    int squareCount = maxPoints;
    int vertexCount = squareCount * 4;
    if (LN_REQUIRE(vertexCount < UINT16_MAX)) return false;

    m_mesh = makeObject<Mesh>(vertexCount, squareCount * 6, IndexBufferFormat::UInt16);
    auto vertices = reinterpret_cast<Vertex*>(m_mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main));
    auto indices = reinterpret_cast<uint16_t*>(m_mesh->acquireMappedIndexBuffer());

    // Front: Z-
    //vertices[0] = Vertex{ Vector3(-0.5,  0.5, 0.0), Vector3::UnitZ, Vector2(0, 0), Color::White };
    //vertices[1] = Vertex{ Vector3( 0.5,  0.5, 0.0), Vector3::UnitZ, Vector2(1, 0), Color::White };
    //vertices[2] = Vertex{ Vector3(-0.5, -0.5, 0.0), Vector3::UnitZ, Vector2(0, 1), Color::White };
    //vertices[3] = Vertex{ Vector3( 0.5, -0.5, 0.0), Vector3::UnitZ, Vector2(1, 1), Color::White };
    //indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 2; indices[4] = 1; indices[5] = 3;

    // Front: Y+, 進行方向を Z+ と考えたパターン。
    vertices[0] = Vertex{ Vector3(0.5, 0.0, -0.5), Vector3::UnitZ, Vector2(0, 0), Color::White };
    vertices[1] = Vertex{ Vector3(-0.5, 0.0, -0.5), Vector3::UnitZ, Vector2(1, 0), Color::White };
    vertices[2] = Vertex{ Vector3(0.5, 0.0,  0.5), Vector3::UnitZ, Vector2(0, 1), Color::White };
    vertices[3] = Vertex{ Vector3(-0.5, 0.0,  0.5), Vector3::UnitZ, Vector2(1, 1), Color::White };
    indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 2; indices[4] = 1; indices[5] = 3;


    m_mesh->addSection(0, 2, 0, PrimitiveTopology::TriangleList);
    //mesh->addSection(0, squareCount / 3, 0, PrimitiveTopology::TriangleList);




    auto m_material = Material::create();
    m_material->setMainTexture(Texture2D::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/Sprite1.png"));
    m_material->shadingModel = ShadingModel::Unlit;
    m_material->setShader(Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx"));



    return true;
}

void RibbonRenderer::setMaterial(Material* material)
{
    m_material = material;
}

void RibbonRenderer::resetBatch()
{
}

void RibbonRenderer::addPoint(RenderingContext* context, const Vector3& pos, float width)
{
}

void RibbonRenderer::submit(RenderingContext* context)
{
    context->setTransfrom(Matrix::Identity);
    context->setMaterial(m_material);
    context->drawMesh(m_mesh, 0);
}


} // namespace detail
} // namespace ln
