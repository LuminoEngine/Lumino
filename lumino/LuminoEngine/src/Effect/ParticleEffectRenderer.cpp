
#include "Internal.hpp"
#include <LuminoGraphics/Mesh/MeshModel.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/RenderView.hpp>
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoGraphics/Rendering/InstancedMeshesModel.hpp>
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

bool SpriteParticleRenderer::init(uint64_t hashKey, Material* material, ParticleGeometryDirection geometryDirection)
{
    if (!ParticleRenderer2::init(hashKey)) return false;
    m_material = material;
    m_geometryDirection = geometryDirection;

    auto mesh = makeObject_deprecated<MeshPrimitive>(4, 6, IndexBufferFormat::UInt16, GraphicsResourceUsage::Static);
    auto vertices = reinterpret_cast<Vertex*>(mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main));
    auto indices = reinterpret_cast<uint16_t*>(mesh->acquireMappedIndexBuffer());

    switch (m_geometryDirection)
    {
    case ln::ParticleGeometryDirection::ToView:
    case ln::ParticleGeometryDirection::VerticalBillboard:
        //// Front: Z-
        //vertices[0] = Vertex{ Vector3(-0.5,  0.5, 0.0), Vector3::UnitZ, Vector2(0, 0), Color::White };
        //vertices[1] = Vertex{ Vector3( 0.5,  0.5, 0.0), Vector3::UnitZ, Vector2(1, 0), Color::White };
        //vertices[2] = Vertex{ Vector3(-0.5, -0.5, 0.0), Vector3::UnitZ, Vector2(0, 1), Color::White };
        //vertices[3] = Vertex{ Vector3( 0.5, -0.5, 0.0), Vector3::UnitZ, Vector2(1, 1), Color::White };
        //indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 2; indices[4] = 1; indices[5] = 3;
        // Front: Z+
#if LN_COORD_RH
        vertices[0] = Vertex{ Vector3(-0.5,  0.5, 0.0), Vector3::UnitZ, Vector2(0, 0), Color::White };
        vertices[1] = Vertex{ Vector3(-0.5, -0.5, 0.0), Vector3::UnitZ, Vector2(0, 1), Color::White };
        vertices[2] = Vertex{ Vector3( 0.5,  0.5, 0.0), Vector3::UnitZ, Vector2(1, 0), Color::White };
        vertices[3] = Vertex{ Vector3( 0.5, -0.5, 0.0), Vector3::UnitZ, Vector2(1, 1), Color::White };
        indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 2; indices[4] = 1; indices[5] = 3;
#else
        vertices[0] = Vertex{ Vector3( 0.5,  0.5, 0.0), Vector3::UnitZ, Vector2(0, 0), Color::White };
        vertices[1] = Vertex{ Vector3(-0.5,  0.5, 0.0), Vector3::UnitZ, Vector2(1, 0), Color::White };
        vertices[2] = Vertex{ Vector3( 0.5, -0.5, 0.0), Vector3::UnitZ, Vector2(0, 1), Color::White };
        vertices[3] = Vertex{ Vector3(-0.5, -0.5, 0.0), Vector3::UnitZ, Vector2(1, 1), Color::White };
        indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 2; indices[4] = 1; indices[5] = 3;
#endif
        break;
    case ln::ParticleGeometryDirection::HorizontalBillboard:
    case ln::ParticleGeometryDirection::Top:
        // Front: Y+, 進行方向を Z+ と考えたパターン。
        vertices[0] = Vertex{ Vector3(0.5, 0.0, -0.5), Vector3::UnitZ, Vector2(0, 0), Color::White };
        vertices[1] = Vertex{ Vector3(-0.5, 0.0, -0.5), Vector3::UnitZ, Vector2(1, 0), Color::White };
        vertices[2] = Vertex{ Vector3(0.5, 0.0,  0.5), Vector3::UnitZ, Vector2(0, 1), Color::White };
        vertices[3] = Vertex{ Vector3(-0.5, 0.0,  0.5), Vector3::UnitZ, Vector2(1, 1), Color::White };
        indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 2; indices[4] = 1; indices[5] = 3;
        break;
    default:
        LN_UNREACHABLE();
        break;
    }


    mesh->addSection(0, 2, 0, PrimitiveTopology::TriangleList);

    m_batch = makeObject_deprecated<InstancedMeshList>(mesh, 0);



    //m_ribbonRenderer = makeRef<RibbonRenderer>();
    //if (!m_ribbonRenderer->init(1000)) {
    //    return false;
    //}

    return true;
}

void SpriteParticleRenderer::draw(RenderingContext* context, const ParticleData2* particle)
{
    switch (m_geometryDirection)
    {
        case ln::ParticleGeometryDirection::ToView:
        {
            const auto scale = Vector3(particle->size * particle->crossScale, particle->size * particle->forwardScale, 1.0f);
            const auto pos = particle->position;
#if 0
            const auto rotMat = Matrix::makeAffineLookAtLH(particle->position, context->viewPoint()->viewPosition, Vector3::UnitY);

            const auto transform = Matrix(
                scale.x * rotMat(0, 0), scale.x * rotMat(0, 1), scale.x * rotMat(0, 2), 0.0f,
                scale.y * rotMat(1, 0), scale.y * rotMat(1, 1), scale.y * rotMat(1, 2), 0.0f,
                scale.z * rotMat(2, 0), scale.z * rotMat(2, 1), scale.z * rotMat(2, 2), 0.0f,
                pos.x, pos.y, pos.z, 1.0f
            );
#endif
            const RenderViewPoint* viewPoint = context->viewPoint();

            // ビュー平面との距離
            float d = Vector3::dot(viewPoint->viewPosition - particle->position, viewPoint->viewDirection);

            // left-hand coord
            Vector3 f = Vector3::normalize(viewPoint->viewDirection * d);
            Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
            Vector3 u = Vector3::cross(f, r);
            const auto transform = Matrix(
                scale.x * r.x, scale.x * r.y, scale.x * r.z, 0.0f,
                scale.y * u.x, scale.y * u.y, scale.y * u.z, 0.0f,
                scale.z * f.x, scale.z * f.y, scale.z * f.z, 0.0f,
                pos.x, pos.y, pos.z, 1.0f);

            m_batch->setTransform(transform);
            m_batch->drawMesh();
            break;
        }
        
        case ln::ParticleGeometryDirection::VerticalBillboard:
        {
            const auto scale = Vector3(particle->size * particle->crossScale, particle->size * particle->forwardScale, 1.0f);
            const auto pos = particle->position;
            const RenderViewPoint* viewPoint = context->viewPoint();

            // ビュー平面との距離
            const auto viewDir = Vector3::normalize(viewPoint->viewDirection.x, 0, viewPoint->viewDirection.z);
            const auto viewPos = Vector3(viewPoint->viewPosition.x, 0, viewPoint->viewPosition.z);
            const auto p = Vector3(particle->position.x, 0, particle->position.z);
            //float d = Vector3::dot(viewDir -p, viewDir);

            // left-hand coord
            Vector3 f = Vector3::normalize(viewPos - p);
            Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
            Vector3 u = Vector3::cross(f, r);
            //const auto transform = Matrix(
            //    1, scale.x * r.y, 0, 0.0f,
            //    0, scale.y * u.y, 0, 0.0f,
            //    0, scale.z * f.y, 1, 0.0f,
            //    pos.x, pos.y, pos.z, 1.0f);
            //const auto transform = Matrix(
            //    1, 0,0, 0.0f,
            //    scale.y * u.x, scale.y * u.y, scale.y * u.z, 0.0f,
            //    0, 0, 1, 0.0f,
            //    pos.x, pos.y, pos.z, 1.0f);
            const auto transform = Matrix(
                scale.x * r.x, scale.x * r.y, scale.x * r.z, 0.0f,
                scale.y * u.x, scale.y * u.y, scale.y * u.z, 0.0f,
                scale.z * f.x, scale.z * f.y, scale.z * f.z, 0.0f,
                pos.x, pos.y, pos.z, 1.0f);

            m_batch->setTransform(transform);
            m_batch->drawMesh();
            break;
        }
        case ln::ParticleGeometryDirection::HorizontalBillboard:
        {
            const auto scale = Vector3(
                particle->size * particle->crossScale,
                particle->size * particle->crossScale,
                particle->size * particle->forwardScale);
            const auto pos = particle->position;
            const auto transform = Matrix(
                scale.x, 0.0f, 0.0f, 0.0f,
                0.0f, scale.y, 0.0f, 0.0f,
                0.0f, 0.0f, scale.z, 0.0f,
                pos.x, pos.y, pos.z, 1.0f);
            m_batch->setTransform(transform);
            m_batch->drawMesh();
            break;
        }

        case ln::ParticleGeometryDirection::Top:
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
        break;
    default:
        LN_UNREACHABLE();
        break;
    }
}

void SpriteParticleRenderer::resetBatch()
{
    m_batch->reset();
}

void SpriteParticleRenderer::submit(RenderingContext* context)
{


    context->drawMeshInstanced(m_material, m_batch);

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

bool RibbonRenderer::init(int maxNodes)
{
    int squareCount = maxNodes;
    int vertexCount = squareCount * 4;
    if (LN_REQUIRE(vertexCount < UINT16_MAX)) return false;

    m_maxNodes = maxNodes;
    m_mesh = makeObject_deprecated<MeshPrimitive>(vertexCount, squareCount * 6, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
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
    m_material->setMainTexture(Texture2D::load(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/Sprite1.png")));
    m_material->m_shadingModel = ShadingModel::Unlit;
    m_material->setShader(Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx")));



    return true;
}

void RibbonRenderer::setMaterial(Material* material)
{
    m_material = material;
}

void RibbonRenderer::resetBatch()
{
    m_activeNodes = 0;
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
    //printf("m_activeNodes: %d\n", m_activeNodes);
}

void RibbonRenderer::addPoint(RenderingContext* context, const Vector3& pos, float width)
{
    if (LN_REQUIRE(m_activeNodes < m_maxNodes)) return;

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
    m_activeNodes++;
}

void RibbonRenderer::submit(RenderingContext* context)
{
    ////static  bool init = false;
    ////if (!init) {
    //    resetBatch();
    //    beginRibbon();
    //    addPoint(context, Vector3(1, 0, 0), 1);
    //    addPoint(context, Vector3(2, 0, 0), 1);
    //    addPoint(context, Vector3(3, 0, 1), 1);
    //    endRibbon();

    ////    init = true;
    ////}


    m_mesh->setSection(0, 0, m_indexCount / 3, 0, PrimitiveTopology::TriangleList);



    context->setTransfrom(Matrix::Identity);
    context->setMaterial(m_material);
    context->drawMesh(m_mesh, 0);
}


} // namespace detail
} // namespace ln
