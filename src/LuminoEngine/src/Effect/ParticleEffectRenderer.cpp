
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

    auto mesh = makeObject<Mesh>(4, 6, IndexBufferFormat::UInt16, GraphicsResourceUsage::Static);
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



    //m_ribbonRenderer = makeRef<RibbonRenderer>();
    //if (!m_ribbonRenderer->init(1000)) {
    //    return false;
    //}

    return true;
}

void SpriteParticleRenderer::draw(RenderingContext* context, const ParticleData2* particle)
{
    // Scale
    const auto scale = Vector3(
        particle->size * particle->crossScale,
        particle->size * particle->crossScale,
        particle->size * particle->forwardScale);

    // Position
    const auto pos = particle->position;

#if 1

    // Rotation
    const auto& viewPosition = context->viewPoint()->viewPosition;
    auto rotMat = makeRotationMatrix(viewPosition, particle->position, particle->linearVelocity);

    const auto transform = Matrix(
        scale.x * rotMat(0, 0), scale.x * rotMat(0, 1), scale.x * rotMat(0, 2), 0.0f,
        scale.y * rotMat(1, 0), scale.y * rotMat(1, 1), scale.y * rotMat(1, 2), 0.0f,
        scale.z * rotMat(2, 0), scale.z * rotMat(2, 1), scale.z * rotMat(2, 2), 0.0f,
        pos.x, pos.y, pos.z, 1.0f
    );

#else
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


    // 愚直に Scale * Rotation * Position の行列乗算をパーティクルごとに繰り返すと
    // 計算負荷が馬鹿にならなくなるので、各要素を直接設定してしまう。
    const auto transform = Matrix(
        scale.x * rotRight.x, scale.x * rotRight.y, scale.x * rotRight.z, 0.0f,
        scale.y * rotUp.x, scale.y * rotUp.y, scale.y * rotUp.z, 0.0f,
        scale.z * rotFront.x, scale.z * rotFront.y, scale.z * rotFront.z, 0.0f,
        pos.x, pos.y, pos.z, 1.0f
    );
#endif

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

    //m_ribbonRenderer->submit(context);
}

Matrix SpriteParticleRenderer::makeRotationMatrix(const Vector3& viewPosition, const Vector3& nodePosition, const Vector3& nodeFront)
{
    // Rotation
    //  やってることは WorldObjectTransform::lookAt() とかと同じ。
    //  View 行列の回転成分を作るのと同じ要領で、Front を進行方向、Up をカメラへの方向と考えて計算する。
    const auto rotUp = Vector3::safeNormalize(viewPosition - nodePosition, Vector3::UnitZ);
    const auto rotFront = Vector3::safeNormalize(nodeFront, Vector3::UnitY); // 速度を持っていない場合は Y+ に進んでいることにする (Z+ だと Lumino デフォルト状態で表示できない)
    auto rotRight = Vector3::cross(rotUp, rotFront);
    if (Vector3::nearEqual(rotRight, Vector3::Zero)) {
        rotRight = Vector3::cross(Vector3::UnitZ, rotFront);
    }
    rotRight.mutatingNormalize();

    return Matrix(
        rotRight.x, rotRight.y, rotRight.z, 0.0f,
        rotUp.x, rotUp.y, rotUp.z, 0.0f,
        rotFront.x, rotFront.y, rotFront.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
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

    m_mesh = makeObject<Mesh>(vertexCount, squareCount * 6, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
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
    m_vertexCount = 0;
    m_indexCount = 0;
    m_vertices = reinterpret_cast<Vertex*>(m_mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main));
    m_indices = reinterpret_cast<uint16_t*>(m_mesh->acquireMappedIndexBuffer());
}

void RibbonRenderer::beginRibbon()
{
    m_currentRibbonNodeIndex = 0;
}

void RibbonRenderer::endRibbon()
{
}

void RibbonRenderer::addPoint(RenderingContext* context, const Vector3& pos, float width)
{
    if (m_currentRibbonNodeIndex == 0) {

    }
    else {

        // Rotation
        const auto& viewPosition = context->viewPoint()->viewPosition;

        auto rotMat = SpriteParticleRenderer::makeRotationMatrix(viewPosition, pos, pos - m_lastPosition);
        auto normal = Vector3::transformCoord(Vector3::UnitY, rotMat);

        if (m_currentRibbonNodeIndex == 1) {
            // 基点位置
            auto m0 = rotMat;
            m0.translate(m_lastPosition);
            auto p0 = Vector3::transformCoord(Vector3(-width * 0.5f, 0, 0), m0);
            auto p1 = Vector3::transformCoord(Vector3( width * 0.5f, 0, 0), m0);
            m_vertices[m_vertexCount + 0] = { p0, normal, Vector2(0, 0), Color::White };
            m_vertices[m_vertexCount + 1] = { p1, normal, Vector2(0, 0), Color::White };

            // 今回の位置
            auto m1 = rotMat;
            m1.translate(pos);
            auto p2 = Vector3::transformCoord(Vector3(-width * 0.5f, 0, 0), m1);
            auto p3 = Vector3::transformCoord(Vector3( width * 0.5f, 0, 0), m1);
            m_vertices[m_vertexCount + 2] = { p2, normal, Vector2(0, 0), Color::White };
            m_vertices[m_vertexCount + 3] = { p3, normal, Vector2(0, 0), Color::White };

            uint32_t v = m_vertexCount;
            m_indices[m_indexCount + 0] = v + 0;
            m_indices[m_indexCount + 1] = v + 1;
            m_indices[m_indexCount + 2] = v + 2;
            m_indices[m_indexCount + 3] = v + 2;
            m_indices[m_indexCount + 4] = v + 1;
            m_indices[m_indexCount + 5] = v + 3;

            m_vertexCount += 4;
            m_indexCount += 6;
        }
        else {

            // 今回の位置
            auto m0 = rotMat;
            m0.translate(pos);
            auto p0 = Vector3::transformCoord(Vector3(-width * 0.5f, 0, 0), m0);
            auto p1 = Vector3::transformCoord(Vector3( width * 0.5f, 0, 0), m0);
            m_vertices[m_vertexCount + 0] = { p0, normal, Vector2(0, 0), Color::White };
            m_vertices[m_vertexCount + 1] = { p1, normal, Vector2(0, 0), Color::White };

            uint32_t v = m_vertexCount - 2;
            m_indices[m_indexCount + 0] = v + 0;
            m_indices[m_indexCount + 1] = v + 1;
            m_indices[m_indexCount + 2] = v + 2;
            m_indices[m_indexCount + 3] = v + 2;
            m_indices[m_indexCount + 4] = v + 1;
            m_indices[m_indexCount + 5] = v + 3;

            m_vertexCount += 2;
            m_indexCount += 6;
        }

    }

    m_lastPosition = pos;
    m_currentRibbonNodeIndex++;
}

void RibbonRenderer::submit(RenderingContext* context)
{
    //static  bool init = false;
    //if (!init) {
        resetBatch();
        beginRibbon();
        addPoint(context, Vector3(1, 0, 0), 1);
        addPoint(context, Vector3(2, 0, 0), 1);
        addPoint(context, Vector3(3, 0, 1), 1);
        endRibbon();

    //    init = true;
    //}


    m_mesh->setSection(0, 0, m_indexCount / 3, 0, PrimitiveTopology::TriangleList);



    context->setTransfrom(Matrix::Identity);
    context->setMaterial(m_material);
    context->drawMesh(m_mesh, 0);
}


} // namespace detail
} // namespace ln
