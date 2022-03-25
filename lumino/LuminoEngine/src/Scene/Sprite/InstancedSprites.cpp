
#include "Internal.hpp"
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/InstancedMeshesModel.hpp>
#include <LuminoEngine/Mesh/MeshPrimitive.hpp>
#include <LuminoEngine/Scene/Sprite/InstancedSprites.hpp>
#include "../../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// InstancedSpritesModel
    
InstancedSpritesModel::InstancedSpritesModel()
    : m_meshList(nullptr)
    , m_material(nullptr)
{}

InstancedSpritesModel::~InstancedSpritesModel() = default;

bool InstancedSpritesModel::init()
{
    if (!Object::init()) return false;

    m_mesh = makeObject<MeshPrimitive>(4, 6);
    m_mesh->setVertex(0, Vertex(Vector3(-0.5f, 0.5f, 0.0f), Vector3::UnitZ, Vector2(0.0f, 0.0f), Color::White));
    m_mesh->setVertex(1, Vertex(Vector3(-0.5f, -0.5f, 0.0f), Vector3::UnitZ, Vector2(0.0f, 1.0f), Color::White));
    m_mesh->setVertex(2, Vertex(Vector3(0.5f, 0.5f, 0.0f), Vector3::UnitZ, Vector2(1.0f, 0.0f), Color::White));
    m_mesh->setVertex(3, Vertex(Vector3(0.5f, -0.5f, 0.0f), Vector3::UnitZ, Vector2(1.0f, 1.0f), Color::White));
    m_mesh->setIndex(0, 0);
    m_mesh->setIndex(1, 1);
    m_mesh->setIndex(2, 2);
    m_mesh->setIndex(3, 2);
    m_mesh->setIndex(4, 1);
    m_mesh->setIndex(5, 3);
    m_mesh->addSection(0, 2, 0, PrimitiveTopology::TriangleList);

    m_meshList = makeObject<InstancedMeshList>(m_mesh, 0);

    //m_material = makeObject<Material>();
    //m_material->setMainTexture(Texture2D::load(u"C:/Proj/LN/Lumino/src/Engine/sandbox/Assets/Window1.png"));
    //m_material->setShader(detail::EngineDomain::renderingManager()->builtinShader(detail::BuiltinShader::Sprite));
    //m_material->setShadingModel(ShadingModel::Unlit);

    return true;
}

void InstancedSpritesModel::setMaterial(Material* value)
{
    m_material = value;
}

Material* InstancedSpritesModel::material() const
{
    return m_material;
}

void InstancedSpritesModel::clearSprites()
{
    m_state.size = Size(1.0f, 1.0f);
    m_state.anchor = Vector2(0.5f, 0.5f);
    m_state.frameRect = Rect(0.0f, 0.0f, -1.0f, -1.0f);
    m_state.frameIndex = -1;
    m_state.flipX = false;
    m_state.flipY = false;
    m_state.billboard = BillboardType::None;
    m_state.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    m_state.transform = Matrix::Identity;
    m_sprites.clear();
}

void InstancedSpritesModel::setSize(const Size& value)
{
    m_state.size = value;
}

void InstancedSpritesModel::setAnchor(const Vector2& value)
{
    m_state.anchor = value;
}

void InstancedSpritesModel::setFrameRect(const Rect& value)
{
    m_state.frameRect = value;
}

void InstancedSpritesModel::setFrameIndex(int value)
{
    m_state.frameIndex = value;
}

void InstancedSpritesModel::setFlipX(bool value)
{
    m_state.flipX = value;
}

void InstancedSpritesModel::setFlipY(bool value)
{
    m_state.flipY = value;
}

void InstancedSpritesModel::setBillboard(BillboardType value)
{
    m_state.billboard = value;
}

void InstancedSpritesModel::setColor(const Color& value)
{
    m_state.color = value;
}

void InstancedSpritesModel::setTransform(const Matrix& value)
{
    m_state.transform = value;
    m_state.hasTransform = !m_state.transform.isIdentity();
}

void InstancedSpritesModel::addSprite(const Vector3& pos)
{
    m_sprites.emplace_back();
    Sprite& sprite = m_sprites.back();

    const Vector2 baseOffset = Vector2(m_state.anchor.x - 0.5f, m_state.anchor.y -0.5f);
    sprite.transform(0, 0) = m_state.size.width;
    sprite.transform(1, 1) = m_state.size.height;
    sprite.transform(3, 0) = baseOffset.x;
    sprite.transform(3, 1) = baseOffset.y;

    // 大量描画を想定するので、不要な行列乗算は避けたい
    if (m_state.hasTransform) {
        sprite.transform = Matrix::multiply(sprite.transform, m_state.transform);
    }

    sprite.transform(3, 0) += pos.x;
    sprite.transform(3, 1) += pos.y;


    const Texture* texture = (m_material) ? m_material->mainTexture() : nullptr;
    const float width = texture ? static_cast<float>(texture->width()) : 1.0f;
    const float height = texture ? static_cast<float>(texture->height()) : 1.0f;

    Rect frameRect;
    if (m_state.frameIndex >= 0) {
        LN_NOTIMPLEMENTED();
    }
    else {
        frameRect = m_state.frameRect;
    }

    if (frameRect.width < 0.0f) frameRect.width = width;
    if (frameRect.height < 0.0f) frameRect.height = height;

    sprite.uvScale.x = frameRect.width / width;
    sprite.uvScale.y = frameRect.height / height;
    sprite.uvOffset.x = frameRect.x / width;
    sprite.uvOffset.y = frameRect.y / height;

    sprite.color = m_state.color;
}

void InstancedSpritesModel::render(RenderingContext* context)
{
    m_meshList->reset();
    const size_t spriteCount = m_sprites.size();
    for (size_t i = 0; i < spriteCount; i++) {
        const Sprite& sprite = m_sprites[i];

        m_meshList->setTransform(sprite.transform);
        m_meshList->setUVOffset(Vector4(sprite.uvOffset, sprite.uvScale));
        m_meshList->setColorScale(sprite.color);
        m_meshList->drawMesh();
    }

    context->setMaterial(m_material);
    context->drawMeshInstanced(m_meshList);
}

//==============================================================================
// InstancedSpritesComponent

InstancedSpritesComponent::InstancedSpritesComponent()
{}

InstancedSpritesComponent::~InstancedSpritesComponent() = default;

bool InstancedSpritesComponent::init()
{
    if (!VisualComponent::init()) return false;

    m_model = makeObject<InstancedSpritesModel>();

    return true;
}

void InstancedSpritesComponent::setModel(InstancedSpritesModel* value)
{
    m_model = value;
}

InstancedSpritesModel* InstancedSpritesComponent::model() const
{
    return m_model;
}

void InstancedSpritesComponent::onRender(RenderingContext* context)
{
    if (m_model) {
        m_model->render(context);
    }
}

//==============================================================================
// InstancedSprites

InstancedSprites::InstancedSprites()
{
}

InstancedSprites::~InstancedSprites()
{
}

bool InstancedSprites::init()
{
    if (!VisualObject::init()) return false;
    m_component = makeObject<InstancedSpritesComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
    return true;
}

InstancedSpritesComponent* InstancedSprites::instancedSpritesComponent() const
{
    return m_component;
}

//==============================================================================
// InstancedSprites::Builder

InstancedSprites::BuilderDetails::BuilderDetails()
    : material(nullptr)
{
}

void InstancedSprites::BuilderDetails::apply(InstancedSprites* p) const
{
    VisualObject::BuilderDetails::apply(p);
    p->setMaterial(material);
}

} // namespace ln

