
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


    return true;
}

void SpriteParticleRenderer::draw(RenderingContext* context, const ParticleData2* particle)
{
    // particle の進行方向を lookat, 視点への方向を up として view 行列を作ってみる
    Matrix rot;
    {
        ///auto target = Vector3::safeNormalize(particle->position + particle->linearVelocity, Vector3::UnitY);

        // left-hand coord
        //Vector3 f = Vector3::normalize(target - particle->position);
        //f *= -1;
        Vector3 f = Vector3::safeNormalize(particle->linearVelocity, Vector3::UnitZ);

        Vector3 up = Vector3::normalize(context->viewPoint()->viewPosition - particle->position);
        //up *= -1;
        //up = -Vector3::UnitZ;
        //Vector3 up = Vector3::UnitY;

        Vector3 s = Vector3::cross(up, f);
        if (Vector3::nearEqual(s, Vector3::Zero))
        {
            // TODO: https://jp.mathworks.com/help/matlab/ref/circshift.html?requestedDomain=www.mathworks.com
            Vector3 u2 = Vector3::UnitZ;
            //std::rotate<float*>(&u2.x, (&u2.x)+2, &u2.z);
            s = Vector3::cross(u2, f);
        }

        s.mutatingNormalize();

        Vector3 u = Vector3::cross(f, s);
        rot = Matrix(
            s.x, s.y, s.z, 0.0f,
            u.x, u.y, u.z, 0.0f,
            f.x, f.y, f.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }


    m_batch->setTransform(/*Matrix::makeRotationX(0.5) *  */rot * Matrix::makeTranslation(particle->position));
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
